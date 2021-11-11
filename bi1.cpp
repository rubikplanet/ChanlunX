#include <iostream>
#include <fstream>
#include "Bi1.h"
#include <vector>

using namespace std;

BiChuLi::BiChuLi() {
    this->fxcl = FenXingChuLi();
    this->status = BiChuLiStatus::START;
}

void BiChuLi::handle(vector<Kxian1>& kxianList) {
    Bi bi = Bi();
    this->fxcl.handle(kxianList);
    int count = this->fxcl.keyKxianList.size();
    for (int i = 0; i < count; i++) {
        FenXing fx = this->fxcl.keyKxianList[i];
        bi = this->__find_fenxing(fx);
        if (bi.get_type() != BiType::NONE) {
            this->biList.push_back(bi);
        }
    }
}

Bi BiChuLi::find_new_bi(FenXing stop_fx) {
    Bi last_bi = Bi();
    Bi bi = Bi();
    if (!this->keyBiList.empty()) {
        last_bi = this->keyBiList.back();
        if (!this->keyBiList.empty()) {
            this->keyBiList.pop_back();
            bi = Bi(last_bi.get_start_fx(), stop_fx);
            this->keyBiList.push_back(bi);
            if (bi.get_type() == BiType::UP)
                bi.set_type(BiType::NEW_UP);
            else
                bi.set_type(BiType::NEW_DOWN);
            this->last_fx = stop_fx;
        }
    }
    return(bi);
}

Bi BiChuLi::__find_bi(Kxian1 kx) {
    Bi bi = Bi();
    FenXing fx = FenXing();

    fx = this->fxcl.__find_fenxing(kx);
    if (fx.get_type() != FenXingType::NONE) {
        return(this->__find_fenxing(fx));
    }
    return(bi);
}

Bi BiChuLi::__find_fenxing(FenXing fx) {
    FenXingType last_fx_type = this->last_fx.get_type();
    Bi bi = Bi();

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
                    this->keyBiList.push_back(bi);
                    this->last_fx = fx;
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
                            this->last_fx = fx;
                            this->keyBiList.push_back(bi);
                            return(bi);
                        }
                }
            break;

        case FenXingType::FAILURE_VERIFY_BOTTOM:
            if (!this->keyBiList.empty()) {
                this->keyBiList.pop_back();
                if (!this->keyBiList.empty()) {
                    this->last_bi = this->keyBiList.back();
                    if (this->last_bi.get_type() != BiType::NONE) {
                        this->last_fx = this->last_bi.get_stop_fx();
                    }
                }
                else {
                    this->last_bi = Bi();
                    this->last_fx = FenXing();
                }
            }
            else {
                this->last_bi = Bi();
                this->last_fx = FenXing();
            }
            bi.set_type(BiType::FAILURE_DOWN);
            return(bi);
            break;

        case FenXingType::FAILURE_VERIFY_TOP:
            if (!this->keyBiList.empty()) {
                this->keyBiList.pop_back();
                if (!this->keyBiList.empty()) {
                    this->last_bi = this->keyBiList.back();
                    if (this->last_bi.get_type() != BiType::NONE) {
                        this->last_fx = this->last_bi.get_stop_fx();
                    }
                }
                else {
                    this->last_bi = Bi();
                    this->last_fx = FenXing();
                }
            }
            else {
                this->last_bi = Bi();
                this->last_fx = FenXing();
            }
            bi.set_type(BiType::FAILURE_UP);
            return(bi);
            break;

        case FenXingType::BOTTOM:
            if (last_fx_type == FenXingType::VERIFY_TOP) {
                bi = Bi(this->last_fx, fx);
                bi.set_type(BiType::TEMP_DOWN);
                return(bi);
            }
            break;

        case FenXingType::TOP:
            if (last_fx_type == FenXingType::VERIFY_BOTTOM) {
                bi = Bi(this->last_fx, fx);
                bi.set_type(BiType::TEMP_UP);
            }
            break;

        case FenXingType::FAILURE_BOTTOM:
            if (last_fx_type == FenXingType::VERIFY_TOP) {
                bi = Bi(this->last_fx, fx);
                bi.set_type(BiType::FAILURE_TEMP_DOWN);
            }
            break;

        case FenXingType::FAILURE_TOP:
            if (last_fx_type == FenXingType::VERIFY_BOTTOM) {
                bi = Bi(this->last_fx, fx);
                bi.set_type(BiType::FAILURE_TEMP_UP);
            }
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
        int start_pos = bi.get_stop_fx().get_middle().get_position();
        int stop_pos = bi.get_stop_fx().get_free().get_position();
        switch (bi.get_type()) {
        case BiType::UP:
            pOut[start_pos] = 3;
            break;
        case BiType::DOWN:
            pOut[start_pos] = -3;
            break;
        case BiType::FAILURE_UP:
            pOut[start_pos] = 4;
            break;
        case BiType::FAILURE_DOWN:
            pOut[start_pos] = -4;
            break;
        case BiType::TEMP_DOWN:
            pOut[start_pos] = -1;
            break;
        case BiType::FAILURE_TEMP_DOWN:
            pOut[start_pos] = -2;
            break;
        case BiType::TEMP_UP:
            pOut[start_pos] = 1;
            break;
        case BiType::FAILURE_TEMP_UP:
            pOut[start_pos] = 2;
            break;
        }
    }
}

void Bi4_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
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
        int start_pos = bi.get_stop_fx().get_middle().get_position();
        int stop_pos = bi.get_stop_fx().get_free().get_position();
        switch (bi.get_type()) {
        case BiType::UP:
            pOut[stop_pos] = 3;
            break;
        case BiType::DOWN:
            pOut[stop_pos] = -3;
            break;
        case BiType::FAILURE_UP:
            pOut[stop_pos] = 4;
            break;
        case BiType::FAILURE_DOWN:
            pOut[stop_pos] = -4;
            break;
        case BiType::TEMP_DOWN:
            pOut[stop_pos] = -1;
            break;
        case BiType::FAILURE_TEMP_DOWN:
            pOut[stop_pos] = -2;
            break;
        case BiType::TEMP_UP:
            pOut[stop_pos] = 1;
            break;
        case BiType::FAILURE_TEMP_UP:
            pOut[stop_pos] = 2;
            break;
        }
    }
}
