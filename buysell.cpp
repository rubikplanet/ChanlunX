#include "buysell.h"

using namespace std;

bool debug_buysell_status = true;

BuySellChuLi::BuySellChuLi() {
    this->status = BuySellChuLiStatus::Start;
    this->xdcl = XianDuanChuLi();
    this->zscl = ZhongShuChuLi();
}

void BuySellChuLi::handle(vector<Kxian1>& kxlist) {
    XianDuan xd;
    BuySell buysell;
    BuySellChuLiStatus status;

    this->xdcl.handle(kxlist);
    
    int count = this->xdcl.key_xianduan_list.size();
    for (int i = 0; i < count; i++) {
        xd = this->xdcl.key_xianduan_list[i];
        buysell = this->find_buysell(xd, Period::OneMin);
    }

}

bool match_xd(XianDuan xd1, XianDuan xd2) {
    float radio = 0.0;

    if (xd1.get_high() > xd2.get_high()) {
        radio = xd1.get_length() / xd2.get_length();
    } else {
        radio = xd2.get_length() / xd1.get_length();
    }
    if (radio >= 0.618)
        return(true);
    else 
        return(false);
}

XianDuan generate_xianduan(XianDuan xd1, XianDuan xd2) {
    Bi start_bi = xd1.get_start_bi();
    Bi Stop_bi = xd2.get_stop_bi();
    return(XianDuan(start_bi, Stop_bi));
}

BuySell BuySellChuLi::find_buysell(XianDuan xd, Period pd) {
    XianDuanType xd_type = xd.get_type();
    BuySell ret_value = BuySell();
    float xd_high = xd.get_high();
    float xd_low = xd.get_low();
    XianDuan new_xd;

    switch(this->status) {
        case BuySellChuLiStatus::Start: //0
            this->one = xd;
            if (xd_type == XianDuanType::UP || xd_type == XianDuanType::TEMP_UP) {
                this->status = BuySellChuLiStatus::UpOne;
                ret_value = BuySell(BuySellType::OneBuy, xd.get_low(), xd.get_start_pos(), pd);
            } else {
                if (xd_type == XianDuanType::DOWN || xd_type == XianDuanType::TEMP_DOWN)
                    this->status = BuySellChuLiStatus::DownOne;
                    ret_value = BuySell(BuySellType::OneSell, xd.get_high(), xd.get_start_pos(), pd);
            }
            break;

        case BuySellChuLiStatus::UpOne: //1
            if (xd_low > this->one.get_low()) {
                this->two = xd;
                this->status = BuySellChuLiStatus::UpTwo;
                ret_value = BuySell(BuySellType::TwoBuy, xd.get_low(), xd.get_stop_pos(), pd);
            } else {
                this->one = xd;
                this->status = BuySellChuLiStatus::Start;
                ret_value = BuySell(BuySellType::FailureOneBuy, xd.get_low(), xd.get_stop_pos(), pd);
            }
            break;

        case BuySellChuLiStatus::UpTwo: //12
            this->three = xd;
            if (match_xd(this->two, xd)) {
                this->status = BuySellChuLiStatus::UpThree;
            } else {
                if (xd_high > this->one.get_high()) {
                    this->status = BuySellChuLiStatus::UpThree_HighLow;
                } else {
                    this->status = BuySellChuLiStatus::UpThree_normal;
                }
            }
            break;

        case BuySellChuLiStatus::UpThree:  //121
            if (match_xd(this->three, xd)) {
                this->zscl.get_zhongshu().start(this->one, this->two, this->three, xd);
                this->status = BuySellChuLiStatus::A;
            } else {
                if (xd_low < this->one.get_low()) {
                    ret_value = BuySell(BuySellType::FailureOneBuy, xd.get_low(), xd.get_stop_pos(), pd);
                    this->status = BuySellChuLiStatus::Start;
                } else {
                    this->four = xd;
                    this->status = BuySellChuLiStatus::UpFour;
                }
            }
            break;

        case BuySellChuLiStatus::UpFour: //123
            if (xd_high > this->three.get_high()) {
                new_xd = generate_xianduan(this->three, xd);
                this->status = BuySellChuLiStatus::UpThree;
                return(this->find_buysell(new_xd, Period::OneMin));
            } else {
                this->five = xd;
                this->status = BuySellChuLiStatus::UpFour_Normal;
            }
            break;

        case BuySellChuLiStatus::UpFour_Normal: //1233
            new_xd = generate_xianduan(this->four, xd);
            this->status = BuySellChuLiStatus::UpFour;
            return(this->find_buysell(new_xd, pd));

        case BuySellChuLiStatus::A:
            this->A = this->zscl.find_zhongshu(xd);
            if (this->A.get_high() > 0.0) {

            }
            break;

        case BuySellChuLiStatus::UpThree_HighLow:
            if (xd_low >= this->one.get_high()) {
                this->one = generate_xianduan(this->one, this->three);
                this->two = xd;
                this->status = BuySellChuLiStatus::UpTwo;
                ret_value = BuySell(BuySellType::TwoBuy, xd_low, xd.get_stop_pos(), pd);
            } else {
                if (xd_low < this->one.get_low()) {
                    ret_value = BuySell(BuySellType::FailureOneBuy, xd_low, xd.get_stop_pos(), pd);
                    this->status = BuySellChuLiStatus::Start;
                } else {
                    this->one = generate_xianduan(this->one, this->three);
                    this->two = xd;
                    ret_value = BuySell(BuySellType::TwoBuy, xd.get_low(), xd.get_stop_pos(), pd);
                }
            }
            break;
        

        case BuySellChuLiStatus::UpThree_normal:
            if (xd_low < this->one.get_low()) {
                ret_value = BuySell(BuySellType::FailureOneBuy, xd_low, xd.get_stop_pos(), pd);
                this->status = BuySellChuLiStatus::Start;
            } else {
                if (xd_low < this->two.get_low()) {
                    this->four = xd;
                    this->status = BuySellChuLiStatus::DownFourNormal;
                } else {
                    this->two = generate_xianduan(this->two, xd);
                    ret_value = BuySell(BuySellType::TwoBuy, xd_low, xd.get_stop_pos(), pd);
                }
            }
            break;

        case BuySellChuLiStatus::DownFourNormal:
            break;
    }
}

void Bi3_buysell(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuLi;
    BuySellChuLi buysellChuli;
    BuySell buysell;
    int stop_pos;

    
    for (int i = 0; i < nCount; i++) {
        baohanChuLi.add(pHigh[i], pLow[i]);
    }
    buysellChuli.handle(baohanChuLi.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = buysellChuli.key_buysell_List.size();
    if (count == 0)
        return;

    if (buysellChuli.key_buysell_List.empty()) {
        for (unsigned int i = 0; i < count; i++) {
            buysell = buysellChuli.key_buysell_List[i];
            stop_pos = buysell.get_pos();
            switch(buysell.get_type()) {
                case BuySellType::OneBuy:
                    pOut[stop_pos] = -1;
                    break;
                case BuySellType::TwoBuy:
                    pOut[stop_pos] = -2;
                    break;
                case BuySellType::SecondTwoBuy:
                    pOut[stop_pos] = -2.5;
                    break;
                case BuySellType::ThreeBuy:
                    pOut[stop_pos] = -3;
                    break;
                case BuySellType::OneSell:
                    pOut[stop_pos] = 1;
                    break;
                case BuySellType::TwoSell:
                    pOut[stop_pos] = 2;
                    break;
                case BuySellType::SecondTwoSell:
                    pOut[stop_pos] = 2.5;
                    break;
                case BuySellType::ThreeSell:
                    pOut[stop_pos] = 3;
                    break;
            }
        }
    }
}