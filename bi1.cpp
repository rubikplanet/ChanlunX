#include <iostream>
#include <fstream>
#include "Bi1.h"
#include <vector>

using namespace std;

BiChuLiStatus status = BiChuLiStatus::START;

void BiChuLi::handle(vector<Kxian1>& kxianList) {
    Bi bi = Bi();
    for (vector<Kxian1>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++) {
        bi = this->__find_bi(*iter);
    }
}
Bi BiChuLi::find_new_bi(FenXing stop_fx) {
    Bi last_bi = Bi();
    Bi bi = Bi();
    if (!this->biList.empty()) {
        last_bi = this->biList.back();
        this->biList.pop_back();
        bi = Bi(last_bi.get_start_fx(), stop_fx);
        this->biList.push_back(bi);
        if (bi.get_type() == BiType::UP)
            bi.set_type(BiType::NEW_UP);
        else
            bi.set_type(BiType::NEW_DOWN);
        this->last_fx = stop_fx;
    }
    return(bi);
}


Bi BiChuLi::__find_bi(Kxian1 kx) {
    FenXingType last_fx_type = this->last_fx.get_type();
    Bi bi = Bi();
    FenXing fx = this->fxcl.__find_fenxing(kx);
    if (fx.get_type() != FenXingType::NONE) {
        switch (fx.get_type()) {
        case FenXingType::VERIFY_BOTTOM:
            if (last_fx_type == FenXingType::NONE) {
                this->last_fx = fx;
                return(bi);
            }
            else {
                if (last_fx_type == FenXingType::VERIFY_BOTTOM) {
                    if (fx.get_low() >= this->last_fx.get_low()) {
                        return(bi);
                    }
                    else {
                        return(this->find_new_bi(fx));
                    }
                }
                else {
                    bi = Bi(this->last_fx, fx);
                    return(bi);
                }
            }
            break;

        case FenXingType::VERIFY_TOP:
            if (last_fx_type == FenXingType::NONE) {
                this->last_fx = fx;
            }
            else {
                        if (last_fx_type == FenXingType::VERIFY_TOP) {
                            if (fx.get_high() <= this->last_fx.get_high()) {
                                return(bi);
                            }
                            else {
                                this->last_fx = fx;
                                return(this->find_new_bi(fx));
                            }
                        }
                        else {
                            bi = Bi(this->last_fx, fx);
                            this->biList.push_back(bi);
                            this->last_fx = fx;
                            return(bi);
                        }
                }
            break;

        case FenXingType::FAILURE_VERIFY_BOTTOM:
            if (!this->biList.empty()) {
                this->biList.pop_back();
                bi = this->biList.back();
                this->last_fx = bi.get_start_fx();
            }
            else {
                this->last_fx = FenXing();
            }
            bi.set_type(BiType::FAILURE_DOWN);
            return(bi);
            break;

        case FenXingType::FAILURE_VERIFY_TOP:
            if (!this->biList.empty()) {
                this->biList.pop_back();
                bi = this->biList.back();
                this->last_fx = bi.get_start_fx();
            }
            else {
                this->last_fx = FenXing();
            }
            bi.set_type(BiType::FAILURE_UP);
            return(bi);
            break;

        case FenXingType::BOTTOM:
            break;

        case FenXingType::TOP:
            break;

        case FenXingType::FAILURE_BOTTOM:
            break;

        case FenXingType::FAILURE_TOP:
            break;
        }
    }
    return(bi);
}



Bi BiChuLi::now_bi(Kxian1 kx) {
    Bi bi = Bi();
    return(bi);
}

void Bi3_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    BiChuLi bichuli;
    bichuli.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = bichuli.biList.size();
    for (int i = count - 1; i > 0; i--) {
        Bi bi = bichuli.biList[i];
        
    }
}

void Bi3(int nCount, float *pOut, float *pHigh, float *pLow, float *pIn) {
    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    FenXingChuLi fenXingChuLi;
    fenXingChuLi.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    int position_start, position_stop;
    unsigned int count = fenXingChuLi.fenXingList.size();
    bool find_top_bottom_flag = true;
    for (unsigned int i = count - 1; i > 0; i--) {
        FenXing fx = fenXingChuLi.fenXingList[i];
        position_start = fx.get_start_position();
        position_stop = fx.get_stop_position();
        if (find_top_bottom_flag) {
            if (fx.get_type() == FenXingType::VERIFY_BOTTOM || fx.get_type() == FenXingType::VERIFY_TOP) {
                find_top_bottom_flag = false;
            }
        }
        if (pOut[position_start] == 0) {
            switch (fx.get_type()) {
            case FenXingType::BOTTOM:
                if (find_top_bottom_flag)
                    pOut[position_start] = -3;
                break;
            case FenXingType::FAILURE_BOTTOM:
                if (find_top_bottom_flag)
                    pOut[position_start] = -2;
                break;
            case FenXingType::VERIFY_BOTTOM:
                pOut[position_start] = -1;
                break;
            case FenXingType::FAILURE_VERIFY_BOTTOM:
                pOut[position_start] = -4;
                break;
            case FenXingType::TOP:
                if (find_top_bottom_flag)
                    pOut[position_start] = 3;
                break;
            case FenXingType::FAILURE_TOP:
                if (find_top_bottom_flag)
                    pOut[position_start] = 2;
                break;
            case FenXingType::VERIFY_TOP:
                pOut[position_start] = 1;
                break;
            case FenXingType::FAILURE_VERIFY_TOP:
                pOut[position_start] = 4;
                break;
            }
        }

    }
}

void Bi4(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    FenXingChuLi fenXingChuLi;
    fenXingChuLi.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    int position_start, position_stop;
    unsigned int count = fenXingChuLi.fenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.fenXingList[i];
        position_start = fx.get_start_position();
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_start] = -3;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_start] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_stop] = -1;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_stop] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_start] = 3;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_start] = 2;
            break;
        case FenXingType::VERIFY_TOP:
            pOut[position_stop] = 1;
            break;
        case FenXingType::FAILURE_VERIFY_TOP:
            pOut[position_stop] = 4;
            break;
        }
    }
}
