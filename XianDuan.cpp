#include "XianDuan.h"
#include "ZhongShu1.h"

using namespace std;

XianDuan XianDuanChuLi::last_xd = XianDuan();
XianDuanChuLiStatus  XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
Bi XianDuanChuLi::last_bi = Bi();

XianDuanChuLi::XianDuanChuLi() {
    this->last_xd = XianDuan();
    this->last_bi = Bi();
    this->bicl = BiChuLi();
    this->status = XianDuanChuLiStatus::LEFT;
    this->zscl = ZhongShuChuLi();
}

void XianDuanChuLi::handle(vector<Kxian1>& kxianList) {
    XianDuan xd = XianDuan();
    Bi bi = Bi();
    this->bicl.handle(kxianList);
 
    int count = this->bicl.keyBiList.size();
    for (int i = 0; i < count; i++) {
        bi = this->bicl.keyBiList[i];
        xd = this->find_xianduan(bi);
        if (xd.get_type() != XianDuanType::NONE) {
            this->xianDuanList.push_back(xd);
        }
    }

    if (!this->keyXianDuanList.empty()){
        XianDuan start_xd = this->keyXianDuanList.back();
        int start_num = 0;
        count = this->xianDuanList.size();
        for (int i =0; i< count; i++){
            if (this->xianDuanList[i] == start_xd){
                start_num = i + 1;
                break;
            }
        }
        for (int i = start_num; i < count; i++) {
            this->keyXianDuanList.push_back(this->xianDuanList[i]);
        }
    }
}

XianDuan XianDuanChuLi::failure_xd(Bi start_bi, Bi stop_bi) {
    XianDuanType xd_type = this->last_xd.get_type();
    XianDuan xd = XianDuan();
    Bi xd_start_bi = Bi();

    if (xd_type != XianDuanType::NONE) {
        xd_start_bi = this->last_xd.get_start_bi();
        xd = XianDuan(xd_start_bi, stop_bi);
        if (xd_type == XianDuanType::UP)
            xd.set_type(XianDuanType::FAILURE_UP);
        else {
            if (xd_type == XianDuanType::DOWN) {
                xd.set_type(XianDuanType::FAILURE_DOWN);
            }
        }
    }
    if (start_bi.get_type() == BiType::UP) {
        xd.set_type(XianDuanType::FAILURE_UP);
    }
    else {
        if (start_bi.get_type() == BiType::DOWN) {
            xd.set_type(XianDuanType::FAILURE_DOWN);
        }
    }
    this->left = stop_bi;
    this->status = XianDuanChuLiStatus::AFTER_LEFT;
    return(xd);
}

XianDuan XianDuanChuLi::find_xianduan(Bi bi) {
    BiType bi_type;
    XianDuan xd = XianDuan();
    if (this->last_bi.get_type() == BiType::NONE) {
        if (bi.get_type() == BiType::UP || bi.get_type() == BiType::DOWN) {
            this->last_bi = bi;
            this->left = bi;
            XianDuanChuLi::status = XianDuanChuLiStatus::AFTER_LEFT;
        }
    }
    else {
        bi_type = bi.get_type();
        switch (bi_type) {
        case BiType::UP:
        case BiType::DOWN:
        case BiType::TEMP_DOWN:
        case BiType::TEMP_UP:
            return(this->__find_xianduan(bi));
        default:
            this->__backroll(bi);
        }
    }
    return(xd);
}

void XianDuanChuLi::__backroll(Bi bi) {
    switch (this->status) {
    case XianDuanChuLiStatus::LEFT:
        break;
    case XianDuanChuLiStatus::AFTER_LEFT:
        this->status = XianDuanChuLiStatus::LEFT;
        break;
    case XianDuanChuLiStatus::MIDDLE:
        this->status = XianDuanChuLiStatus::AFTER_LEFT;
        break;
    case XianDuanChuLiStatus::AFTER_MIDDLE:
        this->status = XianDuanChuLiStatus::MIDDLE;
        break;
    }
}

XianDuan XianDuanChuLi::__find_xianduan(Bi bi) {
    XianDuan xd = XianDuan();
        switch (XianDuanChuLi::status) {
        case XianDuanChuLiStatus::LEFT:
            this->left = bi;
            XianDuanChuLi::status = XianDuanChuLiStatus::AFTER_LEFT;
            break;

        case XianDuanChuLiStatus::AFTER_LEFT:
            if (this->left.get_type() == BiType::UP && bi.get_low() < this->left.get_low()) {
                xd = this->failure_xd(this->left, bi);
                XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                return(xd);
            }
            else {
                if (this->left.get_type() == BiType::DOWN && bi.get_high() > this->left.get_high()) {
                    xd = this->failure_xd(this->left, bi);
                    XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                    return(xd);
                }
            }
            this->after_left = bi;
            XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
            break;
            
        case XianDuanChuLiStatus::MIDDLE:
            this->middle = bi;
            if ((bi.get_type() == BiType::UP) && (bi.get_high() > this->left.get_high())){
                    XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE_HIGH_LOW;
            } 
            else {
                if ((bi.get_type() == BiType::DOWN) && (bi.get_low() < this->left.get_low())) {
                    XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE_HIGH_LOW;
                }
                else {
                    XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
            }
            if (bi.get_length() <= this->left.get_length()) {
                if (bi.get_type() == BiType::UP) {
                    xd = XianDuan(this->left, bi);
                    xd.set_type(XianDuanType::TEMP_UP);
                } else {
                    if (bi.get_type() == BiType::DOWN){
                        xd = XianDuan(this->left, bi);
                        xd.set_type(XianDuanType::TEMP_DOWN);
                    }
                }
            }
            return(xd);

        case XianDuanChuLiStatus::MIDDLE_HIGH_LOW:
            if (bi.get_type() == BiType::UP) {
                if (bi.get_high() > this->left.get_high()) {
                    Bi start_bi = bi.generate_bi(this->left, this->after_left, this->middle);
                    xd = this->failure_xd(start_bi, bi);
                    XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                }
                else {
                    if (bi.get_high() >= this->after_left.get_high()) {
                        this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                        this->after_left = bi.update_stop_bi(bi, this->middle.get_start_fx());
                        this->status = XianDuanChuLiStatus::MIDDLE;
                    }
                    else {
                        if (bi.get_high() <= this->left.get_low()) {
                            this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                            this->after_left = bi;
                            XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                        }
                        else {
                            this->after_middle = bi;
                            this->zhongshu_a = ZhongShu1(this->left, this->after_left, this->middle, bi);
                            this->zscl = ZhongShuChuLi(this->zhongshu_a);
                            this->zscl.set_status(ZhongShuChuLiStatus::OUTPUT);
                            XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                        }
                    }
                }
            }
            else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi.get_low() < this->left.get_low()) {
                        Bi start_bi = bi.generate_bi(this->left, this->after_left, this->middle);
                        xd = this->failure_xd(start_bi, bi);
                        XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                    }
                    else {
                        if (bi.get_low() <= this->after_left.get_low()) {
                            this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                            this->after_left = bi.update_stop_bi(bi, this->middle.get_start_fx());
                            XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                        }
                        else {
                            if (bi.get_low() >= this->left.get_high()) {
                                this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                                this->after_left = bi;
                                XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                            }
                            else {
                                this->after_middle = bi;
                                this->zhongshu_a = ZhongShu1(this->left, this->after_left, this->middle, bi);
                                this->zscl = ZhongShuChuLi(this->zhongshu_a);
                                this->zscl.set_status(ZhongShuChuLiStatus::OUTPUT);
                                XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                            }
                        }
                    }
                }
            }
            break;
        case XianDuanChuLiStatus::MIDDLE_NORMAL:
            if (bi.get_type() == BiType::DOWN) {
                if (bi.get_low() < this->left.get_low()) {
                    Bi stop_bi = bi.generate_bi(this->after_left, this->middle, bi);
                    xd = this->failure_xd(this->left, stop_bi);
                }
                else {
                    if (bi.get_low() >= this->after_left.get_low()) {
                        this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                        XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                    }
                    else {
                        this->after_middle = bi;
                        this->zhongshu_a = ZhongShu1(this->left, this->after_left, this->middle, bi);
                        XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                        this->zscl = ZhongShuChuLi(this->zhongshu_a);
                        this->zscl.set_status(ZhongShuChuLiStatus::OUTPUT);
                        XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                    }
                }
            }
            else {
                if (bi.get_type() == BiType::UP) {
                    if (bi.get_high() > this->left.get_high()) {
                        Bi stop_bi = bi.generate_bi(this->after_left, this->middle, bi);
                        xd = this->failure_xd(this->left, stop_bi);
                    }
                    else {
                        if (bi.get_high() <= this->after_left.get_high()) {
                            this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                            XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                        }
                        else {
                            this->after_middle = bi;
                           this->zhongshu_a = ZhongShu1(this->left, this->after_left, this->middle, bi);
                            XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                            this->zscl = ZhongShuChuLi(this->zhongshu_a);
                            this->zscl.set_status(ZhongShuChuLiStatus::OUTPUT);
                            XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                        }
                    }
                }
            }
            break;
        case XianDuanChuLiStatus::ZHONGSHU_A:
            ZhongShu1 zs = this->zscl.find_zhongshu(bi);
            if (zs.get_type() != ZhongShuType::NONE) {

            }
            break;
        }
    return(xd);
}

void Bi3_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    //XianDuanType xd_type;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

}
