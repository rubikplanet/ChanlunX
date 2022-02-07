#include <iostream>
#include <fstream>
#include "Bi1.h"
#include <vector>

using namespace std;

bool debug_bi_status = true;

BiChuLi::BiChuLi() {
    this->fxcl = FenXingChuLi();
    this->status = BiChuLiStatus::START;
}

void debug_bi(Bi bi){
    BiType bi_type = bi.get_type();
    switch(bi_type) {
        case BiType::NONE:
            break;
        case BiType::UP:
            OutputDebugPrintf("【上升笔】 %f %f %d", bi.get_low(), bi.get_high(), bi.get_start_pos());
            break;
        case BiType::FAILURE_UP:
            OutputDebugPrintf("【失败上升笔】 %f %f %d", bi.get_low(), bi.get_high(), bi.get_start_pos());
            break;        
        case BiType::TEMP_UP: 
            OutputDebugPrintf("【临时上升笔】 %f %f %d", bi.get_low(), bi.get_high(), bi.get_start_pos());
            break;
        case BiType::FAILURE_TEMP_UP:
            OutputDebugPrintf("【失败 临时上升笔】 %f %f %d", bi.get_low(), bi.get_high(), bi.get_start_pos());
            break;        
        case BiType::NEW_UP:
            OutputDebugPrintf("【新的 上升笔】 %f %f %d", bi.get_low(), bi.get_high(), bi.get_start_pos());
            break;            
        case BiType::DOWN:
            OutputDebugPrintf("【下降笔】 %f %f %d", bi.get_high(), bi.get_low(), bi.get_start_pos());
            break;
        case BiType::FAILURE_DOWN:
            OutputDebugPrintf("【失败 下降笔】 %f %f %d", bi.get_high(), bi.get_low(), bi.get_start_pos());
            break;        
        case BiType::TEMP_DOWN:
            OutputDebugPrintf("【临时下降笔】 %f %f %d", bi.get_high(), bi.get_low(), bi.get_start_pos());
            break;  
        case BiType::FAILURE_TEMP_DOWN:
            OutputDebugPrintf("【失败 临时下降笔】 %f %f %d", bi.get_high(), bi.get_low(), bi.get_start_pos());
            break;
        case BiType::NEW_DOWN:
            OutputDebugPrintf("【新的 下降笔】 %f %f %d", bi.get_high(), bi.get_low(), bi.get_start_pos());
            break;
        default:
            OutputDebugPrintf("【未处理的选项】%d ", bi_type);
            break;          
    }
}

void BiChuLi::handle(vector<Kxian1>& kxianList) {
    Bi bi = Bi();
    this->fxcl.handle(kxianList);
    int count = this->fxcl.keyFenXingList.size();

    for (int i = 0; i < count; i++) {
        FenXing fx = this->fxcl.keyFenXingList[i];
        bi = this->__find_bi(fx);
        if (bi.get_type() != BiType::NONE) {
            if (debug_bi_status)
                debug_bi(bi);
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


Bi BiChuLi::__find_bi(FenXing fx) {
    FenXingType last_fx_type = this->last_fx.get_type();
    Bi bi = Bi();

    if (last_fx_type == FenXingType::NONE) {
        this->last_fx = fx;
        return(bi);
    }

    switch (fx.get_type()) {
        case FenXingType::VERIFY_BOTTOM:
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
            break;

        case FenXingType::VERIFY_TOP:
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
    return(bi);
}

void Bi3_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    Bi bi;
    int start_pos, stop_pos;

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

    bi = bichuli.biList[0];
    start_pos = bi.get_start_pos();
    if (bi.get_type() == BiType::UP)
        pOut[start_pos] = -3;
    else
        pOut[start_pos] = 3;

    for (int i = 1; i < count; i++) {
        bi = bichuli.biList[i];
        stop_pos = bi.get_stop_pos();
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
    for (int i = count - 1; i >= 0; i--) {
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
