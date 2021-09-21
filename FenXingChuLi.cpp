#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

FenXingStatus FenXingChuLi::status;
Kxian1 FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free, FenXingChuLi::last_bar;
FenXing FenXingChuLi::fx, FenXingChuLi::temp_fx;
float FenXingChuLi::comp_fx_gao, FenXingChuLi::comp_fx_di;
int FenXingChuLi::comp_fx_gao_count, FenXingChuLi::comp_fx_di_count;
float FenXingChuLi::max_high, FenXingChuLi::min_low;

FenXingChuLi::FenXingChuLi() {
    FenXing fx = FenXing();
    FenXingChuLi::status = FenXingStatus::LEFT;

    Kxian1 kx = Kxian1();
    FenXingChuLi::left = kx;
    FenXingChuLi::middle = kx;
    FenXingChuLi::right = kx;
    FenXingChuLi::free = kx;
    FenXingChuLi::fx = fx;
    FenXingChuLi::temp_fx = fx;
    FenXingChuLi::max_high = 0;
    FenXingChuLi::min_low = 9999;
    FenXingChuLi::last_bar = Kxian1();
}

void FenXingChuLi::handle(vector<Kxian1>& kxianList) {
    FenXing fx;
    this->kxianList = kxianList;
    for (vector<Kxian1>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++) {
        fx = this->__find_fenxing((*iter));
        FenXingType fx_type = fx.get_type();
        if (fx_type != FenXingType::NONE) {
            switch (fx_type) {
            case FenXingType::VERIFY_BOTTOM:
                FenXingChuLi::fx = fx;
                this->keyKxianList.push_back(fx);
                break;
            case FenXingType::VERIFY_TOP:
                FenXingChuLi::fx = fx;
                this->keyKxianList.push_back(fx);
                break;
            case FenXingType::FAILURE_VERIFY_TOP:
                this->keyKxianList.pop_back();
                if (!this->keyKxianList.empty())
                    FenXingChuLi::fx = this->keyKxianList.back();
                else
                    FenXingChuLi::fx = FenXing();
                break;
            case FenXingType::FAILURE_VERIFY_BOTTOM:
                this->keyKxianList.pop_back();
                if (!this->keyKxianList.empty())
                    FenXingChuLi::fx = this->keyKxianList.back();
                else
                    FenXingChuLi::fx = FenXing();
                break;
            }
            this->fenXingList.push_back(fx);
        }
    }

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

void OutputDebugPrintf(const char* strOutputString, ...)
{
    char strBuffer[4096] = { 0 };
    va_list vlArgs;
    va_start(vlArgs, strOutputString);
    _vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
    //vsprintf(strBuffer,strOutputString,vlArgs);
    va_end(vlArgs);
    OutputDebugString(strBuffer);
}

char* get_fenxing_status(FenXingStatus fx_type) {
    switch (fx_type) {
    case FenXingStatus::LEFT:
        return("LEFT");
    case FenXingStatus::MIDDLE:
        return("MIDDLE");
    case FenXingStatus::RIGHT:
        return("RIGHT");
    case FenXingStatus::TOP:
        return("TOP");
    case FenXingStatus::BOTTOM:
        return("BOTTOM");
    case FenXingStatus::FREE_NEW_TOP:
        return("FREE_NEW_TOP");
    case FenXingStatus::FREE_NEW_BOTTOM:
        return("FREE_NEW_BOTTOM");
    case FenXingStatus::VERIFY_TOP:
        return("VERIFY_TOP");
    case FenXingStatus::VERIFY_BOTTOM:
        return("VERIFY_BOTTOM");
    case FenXingStatus::VERIFY_NEW_TOP:
        return("VERIFY_NEW_TOP");
    case FenXingStatus::VERIFY_NEW_BOTTOM:
        return("VERIFY_NEW_BOTTOM");
    case FenXingStatus::BACK_ZHONGYIN_TOP:
        return("BACK_ZHONGSHUYIN_TOP");
    case FenXingStatus::BACK_ZHONGYIN_BOTTOM:
        return("BACK_ZHONGSHUYIN_BOTTOM");
    case FenXingStatus::AFTER_VERIFY_TOP:
        return("AFTER_VERIFY_TOP");
    case FenXingStatus::AFTER_VERIFY_BOTTOM:
        return("AFTER_VERIFY_BOTTOM");
    default:
        char msg[100];
        strcpy(msg, "没有处理的Token");
        return(msg);
    }
}

bool FenXingChuLi::_find_new_highlow(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    if (kxian.get_high() > this->max_high) {
        this->max_high = kxian.get_high();
        return(true);
    }
    else {
        if (kxian.get_low() < this->min_low) {
            this->min_low = kxian.get_low();
            return(true);
        }
    }
    return(false);
}

FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    OutputDebugPrintf(" % s % f % f %d \n", get_fenxing_status(FenXingChuLi::status), kxian.get_high(), kxian.get_low(), kxian.get_position());
    if (this->last_bar.get_high() == 0 && this->last_bar.get_low() == 0) {
        this->last_bar = kxian;
        this->max_high = kxian.get_high();
        this->min_low = kxian.get_low();
        this->left = kxian;
        this->status = FenXingStatus::MIDDLE;
        return(tmp_fx);
    }

    switch (FenXingChuLi::status) {
    case FenXingStatus::LEFT:
        if (this->_find_new_highlow(kxian)) {
            //ToDo: 增加最低点最高点处理
        }
        else {
            FenXingChuLi::left = kxian;
            FenXingChuLi::status = FenXingStatus::MIDDLE;
            tmp_fx.set_type(FenXingType::NONE);
            FenXingChuLi::last_bar = kxian;
            return(tmp_fx);
        }
    case FenXingStatus::MIDDLE:
        this->_find_new_highlow(kxian);
        FenXingChuLi::middle = kxian;
        FenXingChuLi::status = FenXingStatus::RIGHT;
        tmp_fx.set_type(FenXingType::NONE);
        FenXingChuLi::last_bar = kxian;
        return(tmp_fx);
    case FenXingStatus::RIGHT:
        //分型处理
        if (this->_find_new_highlow(kxian)) {
            this->left = this->middle;
            this->middle = kxian;
        }
        else {
            tmp_fx = this->__right_process(kxian);
            if ((tmp_fx.get_type() == FenXingType::TOP) || (tmp_fx.get_type() == FenXingType::BOTTOM))
                this->__set_temp_fx(tmp_fx);
        }
        FenXingChuLi::last_bar = kxian;
        return(tmp_fx);

    case FenXingStatus::TOP:
        if (this->_find_new_highlow(kxian)) {
            if (kxian.get_low() == this->min_low) {
                this->left = this->last_bar;
                this->middle = kxian;
                this->last_bar = kxian;
                this->status = FenXingStatus::RIGHT;
                return(tmp_fx);
            }
        }
        if (kxian.get_high()> FenXingChuLi::temp_fx.get_high()) {
            tmp_fx = FenXingChuLi::temp_fx;
            tmp_fx.set_type(FenXingType::FAILURE_TOP);
            //ToDo: 增加last_bar逻辑
            FenXingChuLi::left = FenXingChuLi::last_bar;
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            FenXingChuLi::last_bar = kxian;
            return(tmp_fx);
        }
        else {
            if (kxian.get_high()<= FenXingChuLi::temp_fx.get_low()) {
                FenXingChuLi::free = kxian;
                tmp_fx = FenXing(FenXingType::VERIFY_TOP, FenXingChuLi::temp_fx.get_high(), FenXingChuLi::temp_fx.get_low(), FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free);
                if (FenXingChuLi::fx.get_type() == FenXingType::VERIFY_TOP && tmp_fx.get_high() < FenXingChuLi::fx.get_high()) {
                    FenXingChuLi::status = FenXingStatus::LEFT;
                    FenXingChuLi::last_bar = kxian;
                    return(FenXing());
                }
                FenXingChuLi:comp_fx_di = kxian.get_low();
                FenXingChuLi::comp_fx_di_count += 1;
                FenXingChuLi::status = FenXingStatus::AFTER_VERIFY_TOP;
                FenXingChuLi::last_bar = kxian;
                return(tmp_fx);
            }
            else {
                FenXingChuLi::free = kxian;
            }
        }
        break;
    case FenXingStatus::BOTTOM:
        if (this->_find_new_highlow(kxian)) {
            if (kxian.get_high() == this->max_high) {
                this->left = this->last_bar;
                this->middle = kxian;
                this->last_bar = kxian;
                this->status = FenXingStatus::RIGHT;
                return(tmp_fx);
            }
        }
        if (kxian.get_low() < FenXingChuLi::temp_fx.get_low()) {
            tmp_fx = FenXingChuLi::temp_fx;
            tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
            FenXingChuLi::left = FenXingChuLi::last_bar;
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            FenXingChuLi::last_bar = kxian;
            return(tmp_fx);
        }
        else {
            if (kxian.get_low() >= FenXingChuLi::temp_fx.get_high()) {
                FenXingChuLi::free = kxian;
                tmp_fx = FenXing(FenXingType::VERIFY_BOTTOM, FenXingChuLi::temp_fx.get_high(), FenXingChuLi::temp_fx.get_low(), FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free);
                if (FenXingChuLi::fx.get_type() == FenXingType::VERIFY_BOTTOM && tmp_fx.get_low() > FenXingChuLi::fx.get_low()) {
                    FenXingChuLi::status = FenXingStatus::LEFT;
                    tmp_fx = FenXing();
                    FenXingChuLi::last_bar = kxian;
                    return(tmp_fx);
                }
               FenXingChuLi::comp_fx_gao = kxian.get_high();
                FenXingChuLi::comp_fx_gao_count += 1;
                FenXingChuLi::status = FenXingStatus::AFTER_VERIFY_BOTTOM;
                FenXingChuLi::last_bar = kxian;
                return(tmp_fx);
            }
            else {
                FenXingChuLi::free = kxian;
            }
        }
        break;
    case FenXingStatus::AFTER_VERIFY_TOP:
        if (this->_find_new_highlow(kxian)) {
            if (kxian.get_low() == this->min_low) {
                this->left = this->last_bar;
                this->middle = kxian;
                this->last_bar = kxian;
                this->status = FenXingStatus::RIGHT;
                return(tmp_fx);
            }
        }
        if (kxian.get_high() > FenXingChuLi::fx.get_high()) {
            tmp_fx = FenXingChuLi::fx;
            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
            FenXingChuLi::left = FenXingChuLi::last_bar;
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            FenXingChuLi::last_bar = kxian;
            return(tmp_fx);
        }
        else {
            if (kxian.get_high() > FenXingChuLi::fx.get_low()) {
                tmp_fx = FenXingChuLi::fx;
                tmp_fx.set_type(FenXingType::TOP);
                FenXingChuLi::temp_fx = tmp_fx;
                tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                FenXingChuLi::left = tmp_fx.get_left();
                FenXingChuLi::middle = tmp_fx.get_middle();
                FenXingChuLi::right = tmp_fx.get_right();
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = FenXingStatus::TOP;
                FenXingChuLi::last_bar = kxian;
                return(tmp_fx);
            }
            else {
                if (kxian.get_low() < FenXingChuLi::comp_fx_di) {
                    if (FenXingChuLi::comp_fx_di_count <= 2) {
                        FenXingChuLi::left = kxian;
                        FenXingChuLi::comp_fx_di = kxian.get_low();
                        FenXingChuLi::comp_fx_di_count += 1;
                    }
                    else {
                        FenXingChuLi::middle = kxian;
                        FenXingChuLi::status = FenXingStatus::RIGHT;
                    }
                }
            }
            tmp_fx.set_type(FenXingType::NONE);
            FenXingChuLi::last_bar = kxian;
            return(tmp_fx);
        }
        break;
    case FenXingStatus::AFTER_VERIFY_BOTTOM:
        if (this->_find_new_highlow(kxian)) {
            if (kxian.get_high() == this->max_high) {
                this->left = this->last_bar;
                this->middle = kxian;
                this->last_bar = kxian;
                this->status = FenXingStatus::RIGHT;
                return(tmp_fx);
            }
        }
        if (kxian.get_low() < FenXingChuLi::fx.get_low()) {
            tmp_fx = FenXingChuLi::fx;
            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
            FenXingChuLi::left = FenXingChuLi::last_bar;
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            FenXingChuLi::last_bar = kxian;
            return(tmp_fx);
        }
        else {
            if (kxian.get_low() < FenXingChuLi::fx.get_high()) {
                tmp_fx = FenXingChuLi::fx;
                tmp_fx.set_type(FenXingType::BOTTOM);
                FenXingChuLi::temp_fx = tmp_fx;
                tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                FenXingChuLi::left = tmp_fx.get_left();
                FenXingChuLi::middle = tmp_fx.get_middle();
                FenXingChuLi::right = tmp_fx.get_right();
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = FenXingStatus::BOTTOM;
                FenXingChuLi::last_bar = kxian;
                return(tmp_fx);
            }
            else {
                if (kxian.get_high() > FenXingChuLi::comp_fx_gao) {
                    if (FenXingChuLi::comp_fx_gao_count <= 2) {
                        FenXingChuLi::left = kxian;
                        FenXingChuLi::comp_fx_gao = kxian.get_high();
                        FenXingChuLi::comp_fx_gao_count += 1;
                    }
                    else {
                        FenXingChuLi::middle = kxian;
                        FenXingChuLi::status = FenXingStatus::RIGHT;
                    }
                }
            }
        }
    }
    tmp_fx.set_type(FenXingType::NONE);
    FenXingChuLi::last_bar = kxian;
    return(tmp_fx);
}

FenXing FenXingChuLi::__right_process(Kxian1 kxian){
    FenXing tmp_fx = FenXing();
    if (this->middle.get_high() > this->left.get_high()) {
        //middle.gao > left.gao
        if (kxian.get_high()> FenXingChuLi::middle.get_high()) {
            //继续新高
            FenXingChuLi::left = FenXingChuLi::middle;
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            temp_fx.set_type(FenXingType::NONE);
            return(temp_fx);
        } else {
            //顶分型
            FenXingChuLi::right = kxian;
            FenXingChuLi::status = FenXingStatus::TOP;
            float gao = FenXingChuLi::middle.get_high();
            float di = min(FenXingChuLi::left.get_low(), FenXingChuLi::right.get_low());
            FenXingChuLi::comp_fx_di = di;
            FenXingChuLi::comp_fx_di_count = 2;
            tmp_fx = FenXing(FenXingType::TOP, gao, di, FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right);
            return(tmp_fx);
        }
    } else {
        //middle.gao < left.gao
        if (kxian.get_low() < FenXingChuLi::middle.get_low()){
            //继续新低
            FenXingChuLi::left = FenXingChuLi::middle;
            FenXingChuLi::middle = kxian;
            this->status = FenXingStatus::RIGHT;
            temp_fx.set_type(FenXingType::NONE);
            return(temp_fx);
        } else {
            //底分型
            FenXingChuLi::right = kxian;
            FenXingChuLi::status = FenXingStatus::BOTTOM;
            float di = FenXingChuLi::middle.get_low();
            float gao = max(FenXingChuLi::left.get_high(), FenXingChuLi::right.get_high());
            FenXingChuLi::comp_fx_gao = gao;
            FenXingChuLi::comp_fx_gao_count = 2;
            tmp_fx = FenXing(FenXingType::BOTTOM, gao, di, FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right);
            return(tmp_fx);
        }
    }
}

Kxian1 FenXingChuLi::__get_last_kxian() {
    Kxian1 last_kx = Kxian1();
    switch (FenXingChuLi::status) {
        case FenXingStatus::LEFT:
            last_kx = FenXingChuLi::free;
            break;
        case FenXingStatus::MIDDLE:
            last_kx = FenXingChuLi::left;
            break;
        case FenXingStatus::RIGHT:
            last_kx = FenXingChuLi::middle;
            break;
        case FenXingStatus::FREE_FIRST:
            last_kx = FenXingChuLi::right;
            break;
        case FenXingStatus::FREE:
            last_kx = FenXingChuLi::free;
            break;
    }
    return(last_kx);
}


FenXing FenXingChuLi::__last_fx_process(Kxian1 kxian) {
    FenXing fx = FenXing();

    fx.set_type(FenXingType::NONE);

    if (FenXingChuLi::fx.get_type() == FenXingType::VERIFY_TOP) {
        //上一个为顶分型
        if (kxian.get_high() > FenXingChuLi::fx.get_high()) {
            //创新高
            fx = this->__get_last_fx(kxian);
            FenXingChuLi::left = this->__get_last_kxian();
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            return(fx);
        }
        else {
            if (kxian.get_high() > FenXingChuLi::fx.get_low()) {
                //回到上一个顶分型的中阴区间
                fx = FenXingChuLi::fx;
                if (fx.get_type() == FenXingType::VERIFY_TOP) {
                    fx.set_type(FenXingType::TOP);
                }
                else {
                    if (fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                        fx.set_type(FenXingType::BOTTOM);
                    }
                }
                this->__set_temp_fx(fx);
                fx = this->__get_last_fx(kxian);
                FenXingChuLi::status = FenXingStatus::FREE;
                FenXingChuLi::free = kxian;
                return(fx);
            }
            else {
                if (kxian.get_low() < FenXingChuLi::comp_fx_di) {
                    if (FenXingChuLi::status == FenXingStatus::LEFT) {
                        FenXingChuLi::left = kxian;
                        FenXingChuLi::status = FenXingStatus::MIDDLE;
                    }
                    else {
                        if (FenXingChuLi::status == FenXingStatus::MIDDLE) {
                            this->middle = kxian;
                            this->status = FenXingStatus::RIGHT;
                        }
                    }
                }
            }
        }
    }
    else {
        if (FenXingChuLi::fx.get_type() == FenXingType::VERIFY_BOTTOM) {
            //上一个为底分型
            if (kxian.get_low() < FenXingChuLi::fx.get_low()) {
                //创新低
                fx = this->__get_last_fx(kxian);
                FenXingChuLi::left = this->__get_last_kxian();
                FenXingChuLi::middle = kxian;
                FenXingChuLi::status = FenXingStatus::RIGHT;
                return(fx);
            }
            else {
                if (kxian.get_low() < FenXingChuLi::fx.get_high()) {
                    //回到上一个底分型的中阴区间
                    fx = FenXingChuLi::fx;
                    if (fx.get_type() == FenXingType::VERIFY_TOP) {
                        fx.set_type(FenXingType::TOP);
                    }
                    else {
                        if (fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                            fx.set_type(FenXingType::BOTTOM);
                        }
                    }
                    this->__set_temp_fx(fx);
                    fx = this->__get_last_fx(kxian);
                    FenXingChuLi::status = FenXingStatus::FREE;
                    FenXingChuLi::free = kxian;
                    return(fx);
                }
                else {
                    if (kxian.get_low() > this->comp_fx_gao) {
                        if (this->status == FenXingStatus::LEFT) {
                            this->left = kxian;
                            this->status = FenXingStatus::MIDDLE;
                        }
                        else {
                            if (this->status == FenXingStatus::MIDDLE) {
                                this->middle = kxian;
                                this->status = FenXingStatus::RIGHT;
                            }
                        }
                    }
                }
            }
        } 
    }
    return(fx);
}

void FenXingChuLi::__set_fx(FenXing fx) {
    FenXingChuLi::fx = fx;
}

void FenXingChuLi::__set_temp_fx(FenXing fx) {
    FenXingChuLi::temp_fx = fx;
}
FenXing  FenXingChuLi::__get_last_fx(Kxian1 kxian) {
    FenXing fx = FenXing();
    if (this->fenXingList.size() > 1) {
        fx = this->fenXingList.back();
        this->fenXingList.pop_back();
        if (fx.get_type() == FenXingType::VERIFY_TOP)
            fx.set_type(FenXingType::FAILURE_TOP);
        else {
            if (fx.get_type() == FenXingType::VERIFY_BOTTOM)
                fx.set_type(FenXingType::FAILURE_BOTTOM);
        }
        this->fenXingList.push_back(fx);


        for (int num = this->fenXingList.size() - 1; num > 0; num--) {
            if (this->fenXingList[num].get_type() == FenXingType::VERIFY_BOTTOM || this->fenXingList[num].get_type() == FenXingType::VERIFY_TOP) {
                this->__set_fx(this->fenXingList[num]);
                break;
            }
        }
    }

    return(fx);
}

Kxian1 FenXingChuLi::get_kx_item(string pos) {
    Kxian1 kx;
    if (pos == "left") {
        kx = FenXingChuLi::left;
    } else if (pos == "middle") {
        kx = FenXingChuLi::middle;
    } else if (pos == "right") {
        kx = FenXingChuLi::right;
    } else if (pos == "free") {
        kx = FenXingChuLi::free;
    } else {
        kx = Kxian1();
    }
    return kx;
}

