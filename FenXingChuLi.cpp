#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"
#include "OutputDebug.h"

using namespace std;

bool debug_fenxing_status = true;
FenXingChuLiStatus FenXingChuLi::status;
Kxian1 FenXingChuLi::one, FenXingChuLi::two, FenXingChuLi::three, FenXingChuLi::four,  FenXingChuLi::five, FenXingChuLi::six, FenXingChuLi::last_bar;
FenXing FenXingChuLi::fx, FenXingChuLi::temp_fx, FenXingChuLi::last_fx;
float FenXingChuLi::comp_fx_gao, FenXingChuLi::comp_fx_di;
int FenXingChuLi::comp_fx_gao_count, FenXingChuLi::comp_fx_di_count;
float FenXingChuLi::max_high, FenXingChuLi::min_low;

FenXingChuLi::FenXingChuLi() {
    FenXing fx = FenXing();
    FenXingChuLi::status = FenXingChuLiStatus::ONE;

    Kxian1 kx = Kxian1();
    this->one = kx;
    this->two = kx;
    this->three = kx;
    this->four = kx;
    this->five = kx;
    this->six = kx;

    this->fx = fx;
    this->temp_fx = fx;
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
            if (debug_fenxing_status)
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
                if (this->keyFenXingList.empty()) {
                    this->keyFenXingList.push_back(tmp_fx);
                } else {
                    FenXing last_fx = this->keyFenXingList.back();
                    if (last_fx.get_type() == FenXingType::VERIFY_TOP) {
                        this->keyFenXingList.push_back(tmp_fx);
                    } else {
                        //上一个分型也为底分型
                        if (last_fx.get_low() < tmp_fx.get_low()) {
                            tmp_fx = FenXing();
                        }
                        else {
                            if (!this->keyFenXingList.empty()) {
                                this->keyFenXingList.pop_back();
                                this->keyFenXingList.push_back(tmp_fx);
                            }
                        }
                    }
                }
                if (tmp_fx.get_type() != FenXingType::NONE) {
                    this->fx = tmp_fx;
                }
                break;
            case FenXingType::VERIFY_TOP:
                if (this->keyFenXingList.empty()) {
                    this->keyFenXingList.push_back(tmp_fx);
                }
                else {
                    FenXing last_fx = this->keyFenXingList.back();
                    if (last_fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                        this->keyFenXingList.push_back(tmp_fx);
                    } else {
                        //上一个分型也为顶分型
                        if (last_fx.get_high() > tmp_fx.get_high()) {
                            tmp_fx = FenXing();
                        }
                        else {
                            if (!this->keyFenXingList.empty()) {
                                this->keyFenXingList.pop_back();
                                this->keyFenXingList.push_back(tmp_fx);
                            }
                        }
                    }
                }
                if (tmp_fx.get_type() != FenXingType::NONE) {
                    this->fx = tmp_fx;
                }
                break;

            case FenXingType::FAILURE_VERIFY_TOP:
                if (!this->keyFenXingList.empty()) {
                    this->keyFenXingList.pop_back();
                    if (!this->keyFenXingList.empty())
                        this->fx = this->keyFenXingList.back();
                }
                else
                    this->fx = FenXing();
                break;

            case FenXingType::FAILURE_VERIFY_BOTTOM:
                if (!this->keyFenXingList.empty()) {
                    this->keyFenXingList.pop_back();
                    if (!this->keyFenXingList.empty())
                        this->fx = this->keyFenXingList.back();
                }
                else
                    this->fx = FenXing();
                break;
            }
            if (tmp_fx.get_type() != FenXingType::NONE)
                this->fenXingList.push_back(tmp_fx);
        }
    }

    if (!this->keyFenXingList.empty()) {
        FenXing start_fx = this->keyFenXingList.back();
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
            this->keyFenXingList.push_back(fx);
        }
    }
}

char* get_fenxing_status(FenXingChuLiStatus fx_type) {
    switch (fx_type) {
    case FenXingChuLiStatus::ONE:
        return("ONE");
    case FenXingChuLiStatus::TWO:
        return("TWO");
    case FenXingChuLiStatus::THREE:
        return("THREE");
    case FenXingChuLiStatus::FOUR:
        return("FOUR");
    case FenXingChuLiStatus::FIVE:
        return("FIVE");
    case FenXingChuLiStatus::SIX:
        return("SIX");
    default:
        return("没有处理的Token");
    }
}

void FenXingChuLi::__init_fenxing(Kxian1 kx) {
    this->one = this->last_bar;
    this->two = kx;
    this->status = FenXingChuLiStatus::THREE;
}

FenXing FenXingChuLi::__high_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
        tmp_fx = this->fx;
        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
    } else {
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::FAILURE_TOP);
        }
        else {
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
        }
    }
    this->__init_fenxing(kxian);
    this->max_high = kxian.get_high();
    return(tmp_fx);
}

FenXing FenXingChuLi::__low_process(Kxian1 kxian){
    FenXing tmp_fx = FenXing();
    if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
        tmp_fx = this->fx;
        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
    } else {
        if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
        }
        else {
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::VERIFY_TOP);
        }
    }

    this->__init_fenxing(kxian);
    this->min_low = kxian.get_low();
    return(tmp_fx);
}

FenXing FenXingChuLi::__high_low_process(Kxian1 kxian){
    FenXing tmp_fx = FenXing();
    if (kxian.get_high() > this->max_high) {
        tmp_fx = this->__high_process(kxian);
    } else {
        if (kxian.get_low() < this->min_low) {
            tmp_fx = this->__low_process(kxian);
        }
    }
    return(tmp_fx);
}

FenXing FenXingChuLi::__failure_verify_top(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    this->one = this->two;
    this->two = kxian;
    tmp_fx = this->fx;
    tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
    return(tmp_fx);
}

FenXing FenXingChuLi::__failue_verify_bottom(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    this->one = this->last_bar;
    this->two = kxian;
    tmp_fx = this->fx;
    tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
    return(tmp_fx);
}

FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    float kx_gao = kxian.get_high();
    float kx_di = kxian.get_low();

    if (debug_fenxing_status)
        OutputDebugPrintf(" % s % f % f %d ", get_fenxing_status(FenXingChuLi::status), kxian.get_high(), kx_di, kxian.get_position());

    if (this->last_bar.get_high() == 0) {
        this->last_bar = kxian;
        this->one = kxian;
        this->max_high = kx_gao;
        this->min_low = kx_di;
        this->status = FenXingChuLiStatus::TWO;
        return(tmp_fx);
    }

    switch (FenXingChuLi::status) {
    case FenXingChuLiStatus::ONE:
        this->one = kxian;
        this->status = FenXingChuLiStatus::TWO;
        break;

    case FenXingChuLiStatus::TWO:
        if (kx_gao > this->max_high) {
            this->max_high = kx_gao;
        } else {
            if (kx_di < this->min_low) {
                this->min_low = kx_di;
            }
        } 
        //ToDo 增加缺口处理
        this->two = kxian;
        this->status = FenXingChuLiStatus::THREE;
        break;

    case FenXingChuLiStatus::THREE:
        if (this->two.get_high() > this->one.get_high()) {
            //第2根K线高于第1根
            if (kx_gao > this->two.get_high()) {
                //上升趋势
                if (kx_gao > this->max_high)
                    this->max_high = kx_gao;
                if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                    //比前高还高
                    tmp_fx = this->fx;
                   tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                }
                this->__init_fenxing(kxian);
            } else {
                //顶分型
                if (kx_di < this->min_low) {
                    this->min_low = kx_di;
                    this->__init_fenxing(kxian);
                } else {
                    tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                    this->three = kxian;
                    this->status = FenXingChuLiStatus::FOUR;
                }
            }
        } else {
            //第2根K线低于第1根
            if (kx_di < this->two.get_low()) {
                //下降趋势
                if (kx_di < this->min_low)
                    this->min_low = kx_di;
                if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low()) {
                    tmp_fx = this->fx;
                    tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                }
                this->__init_fenxing(kxian);
            } else {
                //底分型
                if (kx_gao > this->max_high) {
                    this->max_high = kx_gao;
                    this->__init_fenxing(kxian);
                } else {
                    tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                    this->three = kxian;
                    this->status = FenXingChuLiStatus::FOUR;
                }
            }
        }
        break;

    case FenXingChuLiStatus::FOUR:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型处理
            if (kx_di < this->three.get_low()) {
                if (kx_di < this->min_low) {
                    this->min_low = kx_di;
                    this->__init_fenxing(kxian);
                } else {
                    if (kx_gao < this->three.get_low() - 0.01) {
                        //有缺口
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::VERIFY_TOP);
                        this->__init_fenxing(kxian);
                    } else {
                        this->four = kxian;
                        this->status = FenXingChuLiStatus::FIVE;
                    }
                }
            } else {
                //反向K线处理
                if (kx_gao > this->max_high)
                    //创新高
                    this->max_high = kx_gao;
                if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                    //高于前一个分型
                    tmp_fx = this->__failure_verify_top(kxian);
                } else {
                    if (kx_gao > this->temp_fx.get_high()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        } else {
            //底分型处理
            if (kx_gao > this->three.get_high()) {
                if (kx_gao > this->max_high) {
                    this->max_high = kx_gao;
                    this->__init_fenxing(kxian);
                } else {
                    if (kx_di > this->three.get_high() + 0.01) {
                        //有缺口
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        this->__init_fenxing(kxian);
                    } else {
                        this->four = kxian;
                        this->status = FenXingChuLiStatus::FIVE;
                    }
                }
            } else {
                //反向K线处理
                if (kx_di < this->min_low) 
                    //创新低
                    this->min_low = kx_di;
                if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low()) {
                    //低于上一个底分型
                    tmp_fx = this->__failue_verify_bottom(kxian);
                } else {
                    if (kx_di < this->temp_fx.get_low()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        }
        break;

    case FenXingChuLiStatus::FIVE:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型处理
            if (kx_di < this->four.get_low()) {
                if (kx_di < this->min_low) {
                    this->min_low = kx_di;
                    this->__init_fenxing(kxian);
                } else {
                    if (kx_gao < this->four.get_low() - 0.01) {
                        //有缺口
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::VERIFY_TOP);
                        this->__init_fenxing(kxian);
                    } else {
                        this->five = kxian;
                        this->status = FenXingChuLiStatus::SIX;
                    }
                }
            } else {
                //反向K线处理
                if (kx_gao > this->max_high) 
                    //创新高
                    this->max_high = kx_gao;
                if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                    //高于前一个分型
                    tmp_fx = this->__failure_verify_top(kxian);
                }
                else {
                    if (kx_gao > this->temp_fx.get_high()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        } else {
            //底分型处理
            if (kx_gao > this->four.get_high()) {
                if (kx_gao > this->max_high) {
                    this->max_high = kx_gao;
                    this->__init_fenxing(kxian);
                } else {
                    if (kx_di > this->four.get_high() + 0.01) {
                        //有缺口
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        this->__init_fenxing(kxian);
                    } else {
                        this->five = kxian;
                        this->status = FenXingChuLiStatus::SIX;
                    }
                }

            } else {
                //反向K线处理
                if (kx_di < this->min_low) 
                    //创新低
                    this->min_low = kx_di;
                if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low()) {
                    //低于上一个底分型
                    return(this->__failue_verify_bottom(kxian));
                }
                else {
                    if (kx_di < this->temp_fx.get_low()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        }
        break;

    case FenXingChuLiStatus::SIX:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型
            if (kx_di < this->five.get_low()) {
                if (kx_di < this->min_low) {
                    this->min_low  = kx_di;
                } 
                tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                tmp_fx.set_type(FenXingType::VERIFY_TOP);
                this->__init_fenxing(kxian);  
            
            } else {
                //反方向处理
                if (kx_gao > this->max_high) 
                    //创新高
                    this->max_high = kx_gao;
                if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                    //高于前一个分型
                    tmp_fx = this->__failure_verify_top(kxian);
                }
                else {
                    if (kx_gao > this->temp_fx.get_high()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        } else {
            //底分型
            if (kx_gao > this->five.get_high()) {
                if (kx_gao > this->max_high) {
                    this->max_high = kx_gao;
                }
                tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                this->__init_fenxing(kxian);  
            } else {
                //反向处理
                if (kx_di < this->min_low)
                    this->min_low = kx_di;
                if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low())
                    tmp_fx = this->__failue_verify_bottom(kxian);
                else {
                    if (kx_di < this->temp_fx.get_low()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        }
        break;
    }

    this->last_bar = kxian;
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
    unsigned int count = fenXingChuLi.keyFenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.keyFenXingList[i];
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
    unsigned int count = fenXingChuLi.keyFenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.keyFenXingList[i];
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
