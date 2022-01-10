#include "ZouShi.h"
#include "Bi1.h"

using namespace std;

XianDuan ZouShiChuLi::generate_xd(XianDuan xd1, XianDuan xd2, XianDuan xd3) {
    XianDuan ret_xd = XianDuan();
    Bi start_bi, stop_bi;
    start_bi = xd1.get_start_bi();
    stop_bi = xd3.get_stop_bi();
    ret_xd = XianDuan(start_bi, stop_bi);
    return(ret_xd);
}

ZouShiChuLi::ZouShiChuLi(){
    this->status = ZouShiChuLiStatus::a;
    this->xdcl = XianDuanChuLi();
    this->zhongshucl = ZhongShuChuLi();
}

void ZouShiChuLi::handle(vector<Kxian1>& kxianList) {
    ZouShi ret_zoushi = ZouShi();
    ZouShi zoushi = ZouShi();
    XianDuan xd = XianDuan();
    this->xdcl.handle(kxianList);

    int count = this->xdcl.key_xianduan_list.size();
    for (int i = 0; i < count; i++) {
        xd = this->xdcl.key_xianduan_list[i];
        ret_zoushi = this->find_zoushi(xd);
    }
}

ZouShi ZouShiChuLi::find_zoushi(XianDuan xd) {
    ZhongShu ret_zhongshu;
    ZouShi ret_zoushi;

    switch(this->status) {
        case ZouShiChuLiStatus::a:
            this->a = xd;
            this->status = ZouShiChuLiStatus::A_xd1;
            break;
        case ZouShiChuLiStatus::A_xd1:
            this->A_xd1 = xd;
            this->status = ZouShiChuLiStatus::A_xd2;
            break;
        case ZouShiChuLiStatus::A_xd2:
            this->A_xd2 = xd;
            this->status = ZouShiChuLiStatus::A_xd3;
            break;
        case ZouShiChuLiStatus::A_xd3:
            this->A_xd3 = xd;
            this->status = ZouShiChuLiStatus::A;
            break;
        case ZouShiChuLiStatus::A:
            this->zhongshucl = ZhongShuChuLi(this->a, this->A_xd1, this->A_xd2, this->A_xd3);
            ret_zhongshu = this->zhongshucl.find_zhongshu(xd);
            if (ret_zhongshu.get_high() > 0.0) {
                this->A = ret_zhongshu;
                this->b_0 = this->A.get_output();
                this->b_1 = xd;
                this->status = ZouShiChuLiStatus::b;
            } else {
                this->status = ZouShiChuLiStatus::A;
            }
            break;
        case ZouShiChuLiStatus::b:
            if (xd.get_type() == XianDuanType::UP) {
                if (xd.get_high() < this->b_0.get_high()) {
                    ret_zoushi = ZouShi(ZouShiType::PANZHENG, this->a, this->b_0);
                    return(ret_zoushi);
                }
            } else {
                if (xd.get_type() == XianDuanType::DOWN) {
                    ret_zoushi = ZouShi(ZouShiType::PANZHENG, this->a, this->b_0);
                    return(ret_zoushi);
                }
            }
            this->b = this->generate_xd(this->b_0, this->b_1, xd);
            this->status = ZouShiChuLiStatus::B_xd1;
            break;
        case ZouShiChuLiStatus::B_xd1:
            this->B_xd1 = xd;
            this->status = ZouShiChuLiStatus::B_xd2;
            break;
        case ZouShiChuLiStatus::B_xd2:
            this->B_xd2 = xd;
            this->status = ZouShiChuLiStatus::B_xd3;
            break;
        case ZouShiChuLiStatus::B_xd3:
            this->B_xd3 = xd;
            this->status = ZouShiChuLiStatus::B;
        case ZouShiChuLiStatus::B:
            this->zhongshucl = ZhongShuChuLi(this->b, this->B_xd1, this->B_xd2, this->B_xd3);
            ret_zhongshu = this->zhongshucl.find_zhongshu(xd);
            if (ret_zhongshu.get_high() > 0.0) {
                this->B = ret_zhongshu;
                this->c_0 = this->A.get_output();
                this->c_1 = xd;
                this->status = ZouShiChuLiStatus::c;
            } else {
                this->status = ZouShiChuLiStatus::B;
            }
            break;
        case ZouShiChuLiStatus::c:
            break;
    }
    return(ret_zoushi);
}

void Bi3_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn){
    BaoHanChuLi baohanChuli;
    ZouShiChuLi zoushichuli;
    ZouShi zoushi;
    int start_pos, stop_pos, verify_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    zoushichuli.handle(baohanChuli.kxianList);

    unsigned int count = zoushichuli.zoushi_list.size() - 1;
    if (count == 0)
        return;

    if (!zoushichuli.zoushi_list.empty()) {
        ZouShi start_zoushi = zoushichuli.zoushi_list[0];
        start_pos = start_zoushi.get_start_pos();
        stop_pos = start_zoushi.get_stop_pos();
        if (start_zoushi.get_start_xd().get_type() == XianDuanType::UP) {
            pOut[start_pos] = -3;
            pOut[stop_pos] = 3;
        } else {
            pOut[start_pos] = 3;
            pOut[start_pos] = -3;
        }

        for (unsigned int i = count; i > 0; i--) {
            zoushi = zoushichuli.zoushi_list[i - 1];
            stop_pos = zoushi.get_stop_pos();
            if (zoushi.get_start_xd().get_type() == XianDuanType::UP)
                pOut[stop_pos] = 3;
            else
                pOut[stop_pos] = -3;
        }
        
    }

}

void Bi4_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn){
    BaoHanChuLi baohanChuli;
    ZouShiChuLi zoushichuli;
    ZouShi zoushi;
    int start_pos, stop_pos, verify_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    zoushichuli.handle(baohanChuli.kxianList);

    unsigned int count = zoushichuli.zoushi_list.size() - 1;
    if (count == 0)
        return;

    if (!zoushichuli.zoushi_list.empty()) {
        ZouShi start_zoushi = zoushichuli.zoushi_list[0];
        start_pos = start_zoushi.get_start_pos();
        stop_pos = start_zoushi.get_stop_pos();
        if (start_zoushi.get_start_xd().get_type() == XianDuanType::UP) {
            pOut[start_pos] = -3;
            pOut[stop_pos] = 3;
        } else {
            pOut[start_pos] = 3;
            pOut[start_pos] = -3;
        }

        for (unsigned int i = count; i > 0; i--) {
            zoushi = zoushichuli.zoushi_list[i - 1];
            stop_pos = zoushi.get_stop_pos();
            if (zoushi.get_start_xd().get_type() == XianDuanType::UP)
                pOut[stop_pos] = 3;
            else
                pOut[stop_pos] = -3;
        }
        
    }

}
