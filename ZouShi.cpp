#include "ZouShi.h"

using namespace std;

XianDuan ZouShiChuLi::generate_xd(XianDuan xd1, XianDuan xd2, XianDuan xd3) {
    XianDuan ret_xd = XianDuan();
    Bi start_bi, stop_bi;
    start_bi = xd1.get_start_bi();
    stop_bi = xd3.get_stop_bi();
    ret_xd = XianDuan(start_bi, stop_bi);
    return(ret_xd);
}

bool ZouShiChuLi::match_zhongshu_xianduan(XianDuan xd1, XianDuan xd2) {
    float bilv = 0;

    if (xd1.get_length() > xd2.get_length()) {
        bilv = xd1.get_length() / xd2.get_length();
    } else {
        bilv = xd2.get_length() / xd1.get_length();
    }

    if (bilv > 0.618)
        return(true);
    else
        return(false);
}

ZouShiChuLi::ZouShiChuLi(){
    this->status = ZouShiChuLiStatus::a;
    this->xdcl = XianDuanChuLi();
    this->zhongshucl = ZhongShuChuLi();
}

void ZouShiChuLi::handle(vector<Kxian1>& kxianList) {
    FindZouShiReturn ret_zoushi;
    ZouShi zoushi = ZouShi();
    XianDuan xd = XianDuan();
    this->xdcl.handle(kxianList);

    int count = this->xdcl.key_xianduan_list.size();
    for (int i = 0; i < count; i++) {
        xd = this->xdcl.key_xianduan_list[i];
        ret_zoushi = this->find_zoushi(xd);
    }
}

FindZouShiReturn ZouShiChuLi::find_zoushi(XianDuan xd) {
    ZhongShu ret_zhongshu;
    FindZouShiReturn ret_zoushi;

    switch(this->status) {
        case ZouShiChuLiStatus::a:
            this->a = xd;
            this->status = ZouShiChuLiStatus::A_xd1;
            break;
        case ZouShiChuLiStatus::A_xd1:
            if (this->match_zhongshu_xianduan(xd, this->a)) {
                //和this->a相差不多
                this->A_xd1 = this->a;
                this->A_xd2 = xd;
                this->status = ZouShiChuLiStatus::A_xd3;
            } else {
                if (xd.get_type() == XianDuanType::DOWN && xd.get_low() < this->a.get_low()) {
                        //创新低
                        this->status = ZouShiChuLiStatus::a;
                        ret_zoushi.type = FindZouShiReturnType::Failure;
                        ret_zoushi.zoushi1 = ZouShi(ZouShiType::DOWN, this->A_xd1, this->A_xd1);

                } else {
                    if (xd.get_type() == XianDuanType::UP && xd.get_high() > this->a.get_high()) {
                        //创新高
                        this->status = ZouShiChuLiStatus::a;
                        ret_zoushi.type = FindZouShiReturnType::Failure;
                        ret_zoushi.zoushi1 = ZouShi(ZouShiType::UP, this->A_xd1, this->A_xd1);

                    } else {
                        this->A_xd1 = xd;
                        this->status = ZouShiChuLiStatus::A_xd2;
                    }
                }
            }
            break;
        case ZouShiChuLiStatus::A_xd2:
            if (this->match_zhongshu_xianduan(xd, this->A_xd1)) {
                this->A_xd2 = xd;
                this->status = ZouShiChuLiStatus::A_xd3;
            } else {
                if (xd.get_type() == XianDuanType::UP && xd.get_high() > this->a.get_high()) {
                    this->a = this->generate_xd(this->a, this->A_xd1, xd);
                    this->status = ZouShiChuLiStatus::A_xd1;
                } else {
                    if (xd.get_type() == XianDuanType::DOWN && xd.get_low() < this->a.get_low()) {
                        this->a = this->generate_xd(this->a, this->A_xd1, xd);
                        this->status = ZouShiChuLiStatus::A_xd1;
                    } else {
                        this->a_0 = this->A_xd1;
                        this->a_1 = xd;
                        this->status = ZouShiChuLiStatus::A_xd2_normal;
                    }
                }
            }
            break;

        case ZouShiChuLiStatus::A_xd2_highlow:
            if (xd.get_type() == XianDuanType::UP) {
                //上升线段
                if (xd.get_high() > this->a.get_high()) {
                    //创新高
                    ret_zoushi.type = FindZouShiReturnType::One;
                    ret_zoushi.zoushi1 = ZouShi(ZouShiType::PANZHENG, this->a, this->A_xd2);
                    this->a = xd;
                    this->status = ZouShiChuLiStatus::A_xd1;
                } else {
                    if (xd.get_high() < this->a.get_low() - 0.01) {
                        //有缺口
                        this->a = this->generate_xd(this->a, this->A_xd1, this->A_xd2);
                        this->A_xd1 = xd;
                        this->status = ZouShiChuLiStatus::A_xd2;                        
                    } else {
                        //生成中枢
                        this->zhongshucl = ZhongShuChuLi(this->a, this->A_xd1, this->A_xd2, xd);
                        this->status = ZouShiChuLiStatus::A;
                    }
                }

            } else {
                //下降线段
                if (xd.get_low() < this->a.get_low()) {
                    //创新低
                    ret_zoushi.type = FindZouShiReturnType::One;
                    ret_zoushi.zoushi1 = ZouShi(ZouShiType::PANZHENG, this->a, this->A_xd2);
                    this->a = xd;
                    this->status = ZouShiChuLiStatus::A_xd1;
                } else {
                    if (xd.get_low() > this->a.get_high() + 0.01) {
                        //有缺口
                        this->a = this->generate_xd(this->a, this->A_xd1, this->A_xd2);
                        this->A_xd1 = xd;
                        this->status = ZouShiChuLiStatus::A_xd2;
                    } else {
                        //生成中枢
                        this->zhongshucl = ZhongShuChuLi(this->a, this->A_xd1, this->A_xd2, xd);
                        this->status = ZouShiChuLiStatus::A;
                    }
                }
            }
            break;
        case ZouShiChuLiStatus::A_xd2_normal:
            if (xd.get_type() == XianDuanType::UP) {
                //上升线段
                if (xd.get_high() > this->a.get_high()) {
                    //创新高
                    this->A_xd1 = this->a;
                    this->A_xd2 = this->generate_xd(this->A_xd1, this->A_xd2, xd);
                    this->status = ZouShiChuLiStatus::A_xd3;
                } else {
                    this->zhongshucl = ZhongShuChuLi(this->a, this->A_xd1, this->A_xd2, xd);
                    this->status = ZouShiChuLiStatus::A;
                }
            } else {
                //下降线段
                if (xd.get_low() < this->a.get_low()) {
                    //创新低
                    this->A_xd1 = this->a;
                    this->A_xd2 = this->generate_xd(this->A_xd1, this->A_xd2, xd);
                    this->status = ZouShiChuLiStatus::A_xd3;
                } else {
                    this->zhongshucl = ZhongShuChuLi(this->a, this->A_xd1, this->A_xd2, xd);
                    this->status = ZouShiChuLiStatus::A;
                }
            }
            break;

        case ZouShiChuLiStatus::A:
            if (xd.get_type() == XianDuanType::UP) {
                //上升线段
                if (xd.get_high() < this->zhongshucl.get_zhongshu().get_low()) {
                    //3卖
                    this->b_0 = this->zhongshucl.get_zhongshu().xd_list[-1];
                    this->b_1 = xd;
                    if (this->a.get_type() == XianDuanType::UP) {
                        this->status = ZouShiChuLiStatus::A_REVERSE_THREESELL;
                    } else {
                        this->status = ZouShiChuLiStatus::A_THREESELL;
                    }
                }
            } else {
                //下降线段
                if (xd.get_low() > this->zhongshucl.get_zhongshu().get_high()) {
                    //3买
                    this->b_0 = this->zhongshucl.get_zhongshu().xd_list[-1];
                    this->b_1 = xd;
                    if (this->a.get_type() == XianDuanType::DOWN) {
                        this->status = ZouShiChuLiStatus::A_REVERSE_THREEBUY;
                    } else {
                        this->status = ZouShiChuLiStatus::A_THREEBUY;
                    }
                } else {
                    ret_zhongshu = this->zhongshucl.find_zhongshu(xd);
                    this->status = ZouShiChuLiStatus::A;
                }
            }
            break;
        
        case ZouShiChuLiStatus::A_THREEBUY:
            if (xd.get_high() > this->b_0.get_high()) {
                this->b = this->generate_xd(this->b_0, this->b_1, xd);
                this->A = this->zhongshucl.get_zhongshu();
                this-A.stop(this->b);
                this->status = ZouShiChuLiStatus::B_xd1;
            } else {
                this->status = ZouShiChuLiStatus::A_THREEBUY_NORMAL;
            }
            break;
        case ZouShiChuLiStatus::A_THREESELL:
            if (xd.get_low() < this->b_0.get_low()) {
                this->b = this->generate_xd(this->b_0, this->b_1, xd);
                this->A = this->zhongshucl.get_zhongshu();
                this->A.stop(xd);
                this->status = ZouShiChuLiStatus::B_xd1;
            } else {
                this->b_2 = this->b_1;
                this->b_1 = this->b_0;
                this->status = ZouShiChuLiStatus::A_THREESELL_NORMAL;
            }
            break;
        case ZouShiChuLiStatus::A_REVERSE_THREEBUY:
            ret_zoushi.type = FindZouShiReturnType::Two;
            ret_zoushi.zoushi1 = ZouShi(ZouShiType::PANZHENG, this->a, this->A_xd2);
            ret_zoushi.zoushi2 = ZouShi(ZouShiType::PANZHENG, this->b_0, xd);
            this->status = ZouShiChuLiStatus::a;
            break;
        case ZouShiChuLiStatus::A_REVERSE_THREESELL:
            ret_zoushi.type = FindZouShiReturnType::Two;
            ret_zoushi.zoushi1 = ZouShi(ZouShiType::PANZHENG, this->a, this->A_xd2);
            ret_zoushi.zoushi2 = ZouShi(ZouShiType::PANZHENG, this->b_0, xd);
            this->status = ZouShiChuLiStatus::a;
            break;
        case ZouShiChuLiStatus::A_THREEBUY_NORMAL:
            if (xd.get_low() > this->zhongshucl.get_zhongshu().get_high()) {
                this->b_1 = this->generate_xd(this->b_1, this->b_2, xd);
                this->status = ZouShiChuLiStatus::A_THREEBUY;
            } else {
                ret_zoushi.type = FindZouShiReturnType::One;
                ret_zoushi.zoushi1 = ZouShi(ZouShiType::PANZHENG, this->a, this->b_0);
                this->a = this->generate_xd(this->b_1, this->b_2, xd);
                this->status = ZouShiChuLiStatus::A_xd1;
            }
            break;
        case ZouShiChuLiStatus::A_THREESELL_NORMAL:
            if (xd.get_high() < this->zhongshucl.get_zhongshu().get_low()) {
                this->b_1 = this->generate_xd(this->b_1, this->b_2, xd);
                this->status = ZouShiChuLiStatus::A_THREESELL;
            } else {
                ret_zoushi.type = FindZouShiReturnType::One;
                ret_zoushi.zoushi1 = ZouShi(ZouShiType::PANZHENG, this->a, this->b_0);
                this->a = this->generate_xd(this->b_1, this->b_2, xd);
                this->status = ZouShiChuLiStatus::A_xd1;                
            }
            break;

        case ZouShiChuLiStatus::b:
            break;

        case ZouShiChuLiStatus::B_xd1:
            if (xd.get_type() == XianDuanType::UP) {
                //上升线段
                if (xd.get_high() < this->A.get_low()) {
                    this->B_xd1 = xd;
                    this->status = ZouShiChuLiStatus::B_xd2;
                } else {
                    ret_zoushi.type = FindZouShiReturnType::One;
                    ret_zoushi.zoushi1 = ZouShi(ZouShiType::PANZHENG, this->a, xd);
                }
            } else {
                //下降线段
                if (xd.get_low() > this->A.get_high()) {
                    this->B_xd1 = xd;
                    this->status = ZouShiChuLiStatus::B_xd2;
                } else {
                    ret_zoushi.type = FindZouShiReturnType::One;
                    ret_zoushi.zoushi1 = ZouShi(ZouShiType::PANZHENG, this->a, xd);
                    this->a = xd;

                }
            }
            break;
        case ZouShiChuLiStatus::B_xd2:
            if (this->match_zhongshu_xianduan(xd, this->B_xd1)) {
                this->B_xd2 = xd;
                this->status = ZouShiChuLiStatus::B_xd3;
            } else {
                if (xd.get_type() == XianDuanType::UP && xd.get_high() > this->b.get_high()) {
                    this->b = this->generate_xd(this->b, this->B_xd1, xd);
                    this->status = ZouShiChuLiStatus::B_xd1;
                } else {
                    if (xd.get_type() == XianDuanType::DOWN && xd.get_low() < this->b.get_low()) {
                        this->b = this->generate_xd(this->b, this->B_xd1, xd);
                        this->status = ZouShiChuLiStatus::B_xd1;
                    } else {
                        this->b_0 = this->B_xd1;
                        this->b_1 = xd;
                        this->status = ZouShiChuLiStatus::B_xd2_normal;
                    }

                }
            }
            break;
        case ZouShiChuLiStatus::B_xd2_normal:
            if (xd.get_type() == XianDuanType::UP) {
                //上升线段
            }else {
                //下降线段
                if (xd.get_low() > this->A.get_high()) {

                }
            }
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
