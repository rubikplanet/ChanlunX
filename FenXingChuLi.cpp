#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"

using namespace std;

FenXingChuLiStatus FenXingChuLi::status;
Kxian1 FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free, FenXingChuLi::last_bar;
FenXing FenXingChuLi::fx, FenXingChuLi::temp_fx, FenXingChuLi::last_fx;
float FenXingChuLi::comp_fx_gao, FenXingChuLi::comp_fx_di;
int FenXingChuLi::comp_fx_gao_count, FenXingChuLi::comp_fx_di_count;
float FenXingChuLi::max_high, FenXingChuLi::min_low;

FenXingChuLi::FenXingChuLi() {
    FenXing fx = FenXing();
    FenXingChuLi::status = FenXingChuLiStatus::LEFT;

    Kxian1 kx = Kxian1();
    FenXingChuLi::left = kx;
    FenXingChuLi::middle = kx;
    FenXingChuLi::right = kx;
    FenXingChuLi::free = kx;
    FenXingChuLi::fx = fx;
    FenXingChuLi::temp_fx = fx;
    this->last_fx = FenXing();
    this->max_high = 0.0;
    this->min_low = 0.0;
    this->last_bar = Kxian1();
}

void debug_fenxing(FenXing fx) {
    FenXingType fx_type = fx.get_type();
    switch (fx_type) {
    case FenXingType::NONE:
        break;
    case FenXingType::TOP:
        OutputDebugPrintf("【顶分型】 %f %d", fx.get_high(), fx.get_start_position());
        break;
    case FenXingType::FAILURE_TOP:
        OutputDebugPrintf("【顶分型失败】%f %d", fx.get_high(), fx.get_stop_position());
        break;
    case FenXingType::VERIFY_TOP:
        OutputDebugPrintf("【验证顶分型】%f %d", fx.get_high(), fx.get_stop_position());
        break;
    case FenXingType::FAILURE_VERIFY_TOP:
        OutputDebugPrintf("【验证顶分型失败】%f %d", fx.get_high(), fx.get_stop_position());
        break;
    case FenXingType::BOTTOM:
        OutputDebugPrintf("【底分型】 %f %d", fx.get_low(), fx.get_start_position());
        break;
    case FenXingType::FAILURE_BOTTOM:
        OutputDebugPrintf("【底分型失败】%f %d", fx.get_low(), fx.get_stop_position());
        break;
    case FenXingType::VERIFY_BOTTOM:
        OutputDebugPrintf("【验证底分型】 %f %d", fx.get_low(), fx.get_stop_position());
        break;
    case FenXingType::FAILURE_VERIFY_BOTTOM:
        OutputDebugPrintf("【验证底分型失败】 %f %d", fx.get_low(), fx.get_stop_position());
        break;
    }
}

void FenXingChuLi::handle(vector<Kxian1>& kxianList) {
    FenXing tmp_fx;
    FenXingType fx_type;

    this->kxianList = kxianList;
    for (vector<Kxian1>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++) {
        tmp_fx = this->__find_fenxing((*iter));
        fx_type = tmp_fx.get_type();
        if (fx_type != FenXingType::NONE) {
            debug_fenxing(tmp_fx);
            switch (fx_type) {
            case FenXingType::TOP:
                this->temp_fx = tmp_fx;
                break;
            case FenXingType::FAILURE_TOP:
                this->temp_fx = FenXing();
                break;
            case FenXingType::BOTTOM:
                this->temp_fx = tmp_fx;
                break;
            case FenXingType::FAILURE_BOTTOM:
                this->temp_fx = FenXing();
                break;
            case FenXingType::VERIFY_BOTTOM:
                if (this->keyKxianList.empty()) {
                    this->keyKxianList.push_back(tmp_fx);
                } else {
                    FenXing last_fx = this->keyKxianList.back();
                    if (last_fx.get_type() == FenXingType::VERIFY_TOP) {
                        this->keyKxianList.push_back(tmp_fx);
                    } else {
                        //上一个分型也为底分型
                        if (last_fx.get_low() < tmp_fx.get_low()) {
                            tmp_fx = FenXing();
                        }
                        else {
                                this->keyKxianList.pop_back();
                                this->keyKxianList.push_back(tmp_fx);
                        }
                    }
                }
                if (tmp_fx.get_type() != FenXingType::NONE) {
                    this->fx = tmp_fx;
                    this->fenXingList.push_back(tmp_fx);
                }
                break;
            case FenXingType::VERIFY_TOP:
                if (this->keyKxianList.empty()) {
                    this->keyKxianList.push_back(tmp_fx);
                }
                else {
                    FenXing last_fx = this->keyKxianList.back();
                    if (last_fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                        this->keyKxianList.push_back(tmp_fx);
                    } else {
                        //上一个分型也为顶分型
                        if (last_fx.get_high() > tmp_fx.get_high()) {
                            tmp_fx = FenXing();
                        }
                        else {
                            this->keyKxianList.pop_back();
                            this->keyKxianList.push_back(tmp_fx);
                        }
                    }
                }
                if (tmp_fx.get_type() != FenXingType::NONE) {
                    this->fx = tmp_fx;
                    this->fenXingList.push_back(tmp_fx);
                }
                break;

            case FenXingType::FAILURE_VERIFY_TOP:
                if (!this->keyKxianList.empty()) {
                    this->keyKxianList.pop_back();
                    if (!this->keyKxianList.empty())
                        this->fx = this->keyKxianList.back();
                }
                else
                    this->fx = FenXing();
                break;

            case FenXingType::FAILURE_VERIFY_BOTTOM:
                if (!this->keyKxianList.empty()) {
                    this->keyKxianList.pop_back();
                    if (!this->keyKxianList.empty())
                        this->fx = this->keyKxianList.back();
                }
                else
                    this->fx = FenXing();
                break;
            }
            if (tmp_fx.get_type() != FenXingType::NONE)
                this->fenXingList.push_back(tmp_fx);
        }
    }

    if (!this->keyKxianList.empty()) {
        FenXing start_fx = this->keyKxianList.back();
        int count = this->fenXingList.size();
        int start_num = 0;
        for (int i = 0; i < count; i++) {
            if (this->fenXingList[i] == start_fx) {
                start_num = i;
                break;
            }
        }
        for (int i = start_num + 1; i < count; i++) {
            FenXing fx = this->fenXingList[i];
            this->keyKxianList.push_back(fx);
        }
    }
}

char* get_fenxing_status(FenXingChuLiStatus fx_type) {
    switch (fx_type) {
    case FenXingChuLiStatus::LEFT:
        return("LEFT");
    case FenXingChuLiStatus::MIDDLE:
        return("MIDDLE");
    case FenXingChuLiStatus::RIGHT:
        return("RIGHT");
    case FenXingChuLiStatus::FREE:
        return("FREE");

    default:
        return("没有处理的Token");
    }
}

FenXing FenXingChuLi::__highlow_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();

    if (this->max_high == 0.0) {
        this->max_high = kxian.get_high();
    }
    if (this->min_low == 0.0) {
        this->min_low = kxian.get_low();
    }
    else {
        if (kxian.get_high() > this->max_high) {
                if (this->status == FenXingChuLiStatus::FREE) {
                    if (this->temp_fx.get_type() == FenXingType::NONE) {
                        tmp_fx = this->temp_fx;
                        if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
                            tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        }
                        else {
                            tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        }
                    }
                }
                else {
                    if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
                        tmp_fx = this->fx;
                        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                    }
                }
            this->max_high = kxian.get_high();
            this->left = this->last_bar;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
            this->last_bar = kxian;
            OutputDebugPrintf("【__highlow_process】 最高点:%f, position:%d", this->max_high, kxian.get_position());
        }
        else {
            if (kxian.get_low() < this->min_low) {
                //ToDo: get_position == 0
                if (this->status == FenXingChuLiStatus::FREE) {
                    if (this->temp_fx.get_type() != FenXingType::NONE) {
                        tmp_fx = this->temp_fx;
                        if (this->temp_fx.get_type() == FenXingType::TOP)
                            tmp_fx.set_type(FenXingType::VERIFY_TOP);
                        else
                            tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                    }
                }
                else {
                    if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                        tmp_fx = this->fx;
                        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                    }
                }
                this->min_low = kxian.get_low();
                this->left = this->last_bar;
                this->middle = kxian;
                this->status = FenXingChuLiStatus::RIGHT;
                this->last_bar = kxian;
                OutputDebugPrintf("【__highlow_process】  最低点:%f, position:%d ", this->min_low, kxian.get_position());
            }
        }

    }
    return(tmp_fx);
}

FenXing FenXingChuLi::__quekou_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    if (kxian.get_high() < this->last_bar.get_low() - 0.01) {
        //向下跳空缺口
        OutputDebugPrintf("【__highlow_process】向下缺口 %d", kxian.get_position());
        if (this->status == FenXingChuLiStatus::FREE) {
            if (this->temp_fx.get_type() != FenXingType::NONE) {
                if (this->temp_fx.get_type() == FenXingType::TOP) {
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::VERIFY_TOP);
                }
                else {
                    if (kxian.get_low() < this->temp_fx.get_low()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                    }
                }
            }
        }
        else {
            if (kxian.get_low() < this->fx.get_low()) {
                if (this->fx.get_type() != FenXingType::NONE) {
                    tmp_fx = this->fx;
                    tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                }
            }
        }
        this->left = this->last_bar;
        this->middle = kxian;
        this->status = FenXingChuLiStatus::RIGHT;
        this->last_bar = kxian;
    }
    else {
        if (kxian.get_low() > this->last_bar.get_high() + 0.01) {
            //向上跳空缺口
            OutputDebugPrintf("【__highlow_process】向上缺口%d", kxian.get_position());
            if (this->status == FenXingChuLiStatus::FREE) {
                if (this->temp_fx.get_type() != FenXingType::NONE) {
                    if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                    }
                    else {
                        if (kxian.get_high() > this->temp_fx.get_high()) {
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        }
                    }
                }
            }
            else {
                if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
                    if (kxian.get_high() > this->fx.get_high()) {
                        if (this->fx.get_type() != FenXingType::NONE) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                        }
                    }
                    else {

                    }
                }
            }
            this->left = this->last_bar;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
            this->last_bar = kxian;
        }
    }
    return(tmp_fx);
}

FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();

    OutputDebugPrintf(" % s % f % f %d ", get_fenxing_status(FenXingChuLi::status), kxian.get_high(), kxian.get_low(), kxian.get_position());

    if (this->last_bar.get_high() == 0) {
        this->last_bar = kxian;
        this->left = kxian;
        this->max_high = kxian.get_high();
        this->min_low = kxian.get_low();
        this->status = FenXingChuLiStatus::MIDDLE;
        return(tmp_fx);
    }

    if ((kxian.get_high() > this->max_high) || (kxian.get_low() < this->min_low)) {
        return(this->__highlow_process(kxian));
    }

    /*
    if ((kxian.get_high() < this->last_bar.get_low() - 0.01) || (kxian.get_low() > this->last_bar.get_high() + 0.01)) {
        return(this->__quekou_process(kxian));
    }
    */

    switch (FenXingChuLi::status) {
    case FenXingChuLiStatus::LEFT:
        tmp_fx = this->__last_fx_process(kxian);
        if (tmp_fx.get_type() == FenXingType::NONE) {
            this->left = kxian;
            this->status = FenXingChuLiStatus::MIDDLE;
        }
        break;

    case FenXingChuLiStatus::MIDDLE:
        tmp_fx = this->__last_fx_process(kxian);
        if (tmp_fx.get_type() == FenXingType::NONE) {
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
        }
        break;

    case FenXingChuLiStatus::RIGHT:
        //分型处理
        tmp_fx = this->__right_process(kxian);
        /*
        tmp_fx = this->__last_fx_process(kxian);
        if (tmp_fx.get_type() == FenXingType::NONE) {
            tmp_fx = this->__right_process(kxian);
        }
        */
        break;

    case FenXingChuLiStatus::FREE:
        tmp_fx = this->__free_process(kxian);
        break;
    }
    this->last_bar = kxian;
    return(tmp_fx);
}

FenXing FenXingChuLi::__right_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    if (this->middle.get_high() > this->left.get_high()) {
        //middle.gao > left.gao
        if (kxian.get_high() > this->middle.get_high()) {
            //继续新高
            this->left = this->middle;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
            return(tmp_fx);
        }
        else {
            //顶分型
            this->right = kxian;
            this->status = FenXingChuLiStatus::FREE;
            float gao = this->middle.get_high();
            float di = min(this->left.get_low(), this->right.get_low());
            this->comp_fx_gao = 0;
            this->comp_fx_gao_count = 0;
            this->comp_fx_di = kxian.get_low();
            this->comp_fx_di_count = 2;
            this->free = Kxian1();
            tmp_fx = FenXing(FenXingType::TOP, gao, di, this->left, this->middle, this->right, this->free);
            tmp_fx.set_free(kxian);
            if (gao == this->max_high)
                tmp_fx.set_high_low_type(HighLowType::NEW_HIGH);
            return(tmp_fx);
        }
    }
    else {
        //middle.gao < left.gao
        if (kxian.get_low() < this->middle.get_low()) {
            //继续新低
            this->left = this->middle;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
            return(tmp_fx);
        }
        else {
            //底分型
            this->right = kxian;
            this->status = FenXingChuLiStatus::FREE;
            float di = FenXingChuLi::middle.get_low();
            float gao = max(this->left.get_high(), this->right.get_high());
            this->comp_fx_gao = kxian.get_high();
            this->comp_fx_gao_count = 2;
            this->comp_fx_di = 0;
            this->comp_fx_di_count = 0;
            this->free = Kxian1();
            tmp_fx = FenXing(FenXingType::BOTTOM, gao, di, this->left, this->middle, this->right, this->free);
            tmp_fx.set_free(kxian);
            if (di == this->min_low)
                tmp_fx.set_high_low_type(HighLowType::NEW_LOW);
            return(tmp_fx);
        }
    }
}

FenXing FenXingChuLi::__free_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();

    if (this->temp_fx.get_type() == FenXingType::TOP) {
        //顶分型处理
        if (kxian.get_high() > this->temp_fx.get_high()) {
            //创新高
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::FAILURE_TOP);
            tmp_fx.set_free(kxian);
            this->left = this->last_bar;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
        }
        else {
            /*
            if (kxian.get_high() < this->temp_fx.get_low()) {
                //确认顶分型
                tmp_fx = this->temp_fx;
                tmp_fx.set_type(FenXingType::VERIFY_TOP);
                tmp_fx.set_free(kxian);
            }
            */
            if (kxian.get_low() < this->comp_fx_di) {
                if (kxian.get_high() < this->last_bar.get_low() + 0.01) {
                    //有缺口
                    this->comp_fx_di_count = 4;
                }
                if (this->comp_fx_di_count >= 4) {
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::VERIFY_TOP);
                    tmp_fx.set_free(kxian);
                    this->left = this->last_bar;
                    this->middle = kxian;
                    this->status = FenXingChuLiStatus::RIGHT;
                }
                else {
                    this->comp_fx_di = kxian.get_low();
                    this->comp_fx_di_count++;
                }
            }
        }
    }
    else {
        //底分型处理
        if (kxian.get_low() < this->temp_fx.get_low()) {
            //创新低
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
            tmp_fx.set_free(kxian);
            this->left = this->last_bar;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
        }
        else {
            /*
            if (kxian.get_low() > this->temp_fx.get_high()) {
                tmp_fx = this->temp_fx;
                tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                tmp_fx.set_free(kxian);
            }
            */
            if (kxian.get_high() > this->comp_fx_gao) {
                if (kxian.get_low() > this->last_bar.get_high() + 0.01) {
                    this->comp_fx_gao_count = 4;
                }
                if (this->comp_fx_gao_count >= 4) {
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                    tmp_fx.set_free(kxian);
                    this->left = this->last_bar;
                    this->middle = kxian;
                    this->status = FenXingChuLiStatus::RIGHT;
                }
                else {
                    this->comp_fx_gao = kxian.get_high();
                    this->comp_fx_gao_count++;
                }
            }
        }
    }
    return(tmp_fx);
}

FenXing FenXingChuLi::__last_fx_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();

    if (this->fx.get_type() == FenXingType::NONE) {
        return(tmp_fx);
    }

    if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
        if (kxian.get_high() > this->fx.get_high()) {
            //创新高
            tmp_fx = this->fx;
            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
            tmp_fx.set_free(kxian);
            this->left = this->last_bar;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
        }
    }
    else {
        if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
            if (kxian.get_low() < this->fx.get_low()) {
                //创新低
                tmp_fx = this->fx;
                tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                tmp_fx.set_free(kxian);
                this->left = this->last_bar;
                this->middle = kxian;
                this->status = FenXingChuLiStatus::RIGHT;
            }
        }
    }
    return(tmp_fx);
}

void Bi3_fenxing(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
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
    unsigned int count = fenXingChuLi.keyKxianList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.keyKxianList[i];
        position_start = fx.get_start_position();
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_start] = -1;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_start] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_start] = -3;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_start] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_start] = 1;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_start] = 2;
            break;
        case FenXingType::VERIFY_TOP:
            pOut[position_start] = 3;
            break;
        case FenXingType::FAILURE_VERIFY_TOP:
            pOut[position_start] = 4;
            break;
        }
    }
}

void Bi4_fenxing(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
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
    unsigned int count = fenXingChuLi.keyKxianList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.keyKxianList[i];
        position_start = fx.get_start_position();
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_start] = -1;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_start] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_stop] = -3;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_stop] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_start] = 1;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_start] = 2;
            break;
        case FenXingType::VERIFY_TOP:
            pOut[position_stop] = 3;
            break;
        case FenXingType::FAILURE_VERIFY_TOP:
            pOut[position_stop] = 4;
            break;
        }
    }
}
