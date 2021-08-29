#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"
#include <string>

using namespace std;

FenXingStatus FenXingChuLi::status;
Kxian1 FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free;
FenXing FenXingChuLi::fx, FenXingChuLi::temp_fx;
float FenXingChuLi::comp_fx_gao, FenXingChuLi::comp_fx_di;

FenXingChuLi::FenXingChuLi() {
    FenXing fx = FenXing();
    FenXingChuLi::status = FenXingStatus::LEFT;

    Kxian1 kx = {0};
    FenXingChuLi::left = kx;
    FenXingChuLi::middle = kx;
    FenXingChuLi::right = kx;
    FenXingChuLi::free = kx;
    FenXingChuLi::fx = fx;
    FenXingChuLi::temp_fx = fx;
}

FenXing FenXingChuLi::handle(vector<Kxian1>& kxianList) {
    FenXing fx;
    this->kxianList = kxianList;
    for (vector<Kxian1>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++) {
        fx = this->__find_fenxing((*iter));
        if (fx.get_type() != FenXingType::NONE)
            this->fenXingList.push_back(fx);
    }
    return(fx);
}


FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    switch (FenXingChuLi::status) {
    case FenXingStatus::LEFT: //left
        if ((FenXingChuLi::fx.get_type() == FenXingType::VERIFY_TOP) || (FenXingChuLi::fx.get_type() == FenXingType::VERIFY_BOTTOM)) {
            tmp_fx = this->__last_fx_process(kxian);
            if (tmp_fx.get_type() != FenXingType::NONE) {
                if (this->keyKxianList.size() >= 1)
                    this->keyKxianList.pop_back();
            }
            return(tmp_fx);
        }
        else {
            FenXingChuLi::left = kxian;
            FenXingChuLi::status = FenXingStatus::MIDDLE;
            tmp_fx.set_type(FenXingType::NONE);
            return(tmp_fx);
        }
        break;
    case FenXingStatus::MIDDLE:
        if ((FenXingChuLi::fx.get_type() == FenXingType::VERIFY_TOP) || (FenXingChuLi::fx.get_type() == FenXingType::VERIFY_BOTTOM)) {
            tmp_fx = this->__last_fx_process(kxian);
            if (tmp_fx.get_type() != FenXingType::NONE) {
                if (this->keyKxianList.size() >= 1)
                    this->keyKxianList.pop_back();
            }
            return(tmp_fx);
        }
        else {
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            tmp_fx.set_type(FenXingType::NONE);
            return(tmp_fx);
        }
        break;
    case FenXingStatus::RIGHT:
        //分型处理
        tmp_fx = this->__right_process(kxian);
        if ((tmp_fx.get_type() == FenXingType::TOP) || (tmp_fx.get_type() == FenXingType::BOTTOM)) {
            this->__set_temp_fx(tmp_fx);
            return(tmp_fx);
        }
        return(tmp_fx);
    case FenXingStatus::FREE_FIRST:
    case FenXingStatus::FREE:
        tmp_fx = this->__free_process(kxian);
        if (tmp_fx.get_type() == FenXingType::VERIFY_BOTTOM || tmp_fx.get_type() == FenXingType::VERIFY_TOP) {
            if (this->keyKxianList.size() >= 1) {
                FenXing back_fx = this->keyKxianList.back();
                if (back_fx.get_type() != tmp_fx.get_type() && back_fx.get_high() != tmp_fx.get_high() && back_fx.get_low() != tmp_fx.get_low())
                    this->keyKxianList.push_back(tmp_fx);
            }
            else {
                this->keyKxianList.push_back(tmp_fx);
            }
        }
        if (tmp_fx.get_type() != FenXingType::NONE)
            this->__set_fx(tmp_fx);
        return(tmp_fx);
    }
    tmp_fx.set_type(FenXingType::NONE);
    return(tmp_fx);
}


FenXing FenXingChuLi::__right_process(Kxian1 kxian){
    FenXing tmp_fx = FenXing();
    if (this->middle.gao > this->left.gao) {
        //middle.gao > left.gao
        if (kxian.gao > FenXingChuLi::middle.gao) {
            //继续新高
            FenXingChuLi::left = FenXingChuLi::middle;
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            temp_fx.set_type(FenXingType::NONE);
            return(temp_fx);
        } else {
            //顶分型
            FenXingChuLi::right = kxian;
            FenXingChuLi::status = FenXingStatus::FREE_FIRST;
            float gao = FenXingChuLi::middle.gao;
            float di = min(FenXingChuLi::left.di, FenXingChuLi::right.di);
            tmp_fx = FenXing(FenXingType::TOP, gao, di, FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right);
            return(tmp_fx);
        }
    } else {
        //middle.gao < left.gao
        if (kxian.di < FenXingChuLi::middle.di){
            //继续新低
            FenXingChuLi::left = FenXingChuLi::middle;
            FenXingChuLi::middle = kxian;
            this->status = FenXingStatus::RIGHT;
            temp_fx.set_type(FenXingType::NONE);
            return(temp_fx);
        } else {
            //底分型
            FenXingChuLi::right = kxian;
            FenXingChuLi::status = FenXingStatus::FREE_FIRST;
            float di = FenXingChuLi::middle.di;
            float gao = max(FenXingChuLi::left.gao, FenXingChuLi::right.gao);
            tmp_fx = FenXing(FenXingType::BOTTOM, gao, di, FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right);
            return(tmp_fx);
        }
    }
}

FenXing FenXingChuLi::__free_process(Kxian1 kxian){
    FenXing fx = FenXing();

    if (FenXingChuLi::temp_fx.get_type() == FenXingType::TOP) {
        //顶分型
        if (kxian.gao > FenXingChuLi::temp_fx.get_high()) {
            //创新高
            if (FenXingChuLi::status == FenXingStatus::FREE_FIRST) {
                FenXingChuLi::left = FenXingChuLi::right;
            }else {
                FenXingChuLi::left = FenXingChuLi::free;
            }
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            fx = FenXingChuLi::temp_fx;
            fx.set_type(FenXingType::NEW_TOP);
            return(fx);
        } else {
            if (kxian.gao <= FenXingChuLi::temp_fx.get_low()) {
                //离开中阴段,顶分型成功
                if (FenXingChuLi::fx.get_type() == FenXingType::VERIFY_TOP) {
                    if (FenXingChuLi::temp_fx.get_high() < FenXingChuLi::fx.get_high()) {
                        FenXingChuLi::left = kxian;
                        FenXingChuLi::status = FenXingStatus::MIDDLE; //重新进入middle
                        fx.set_type(FenXingType::NONE);
                        return(fx);
                    }
                    else {
                        if (this->fenXingList.size() > 1) {
                            fx = this->fenXingList.back();
                            this->fenXingList.pop_back();
                            fx.set_type(FenXingType::FAILURE_TOP);
                            this->fenXingList.push_back(fx);
                        }
                    }

                }
                FenXingChuLi::free = kxian;
                fx = FenXing(FenXingType::VERIFY_TOP, FenXingChuLi::temp_fx.get_high(), FenXingChuLi::temp_fx.get_low(), FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free);
                this->comp_fx_di = fx.get_low();
                FenXingChuLi::status = FenXingStatus::LEFT; //重新进入left
                return(fx);
            } else {
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = FenXingStatus::FREE;
            }
        }
    } else {
        //底分型
        if (kxian.di < FenXingChuLi::temp_fx.get_low()) {
            //创新低
            if (FenXingChuLi::status == FenXingStatus::FREE_FIRST) {
                FenXingChuLi::left = FenXingChuLi::right;
            }else {
                FenXingChuLi::left = FenXingChuLi::free;
            }
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            fx = FenXingChuLi::temp_fx;
            fx.set_type(FenXingType::NEW_BOTTOM);
            return(fx);
        } else {
            if (kxian.di >= FenXingChuLi::temp_fx.get_high()) {
                //离开中阴段,底分型成功
                if (FenXingChuLi::fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                    if (FenXingChuLi::temp_fx.get_low() > FenXingChuLi::fx.get_low()) {
                        FenXingChuLi::left = kxian;
                        FenXingChuLi::status =FenXingStatus:: MIDDLE; //重新进入middle
                        fx.set_type(FenXingType::NONE);
                        return(fx);
                    }
                    else {
                        if (this->fenXingList.size() > 1) {
                            fx = this->fenXingList.back();
                            this->fenXingList.pop_back();
                            fx.set_type(FenXingType::FAILURE_TOP);
                            this->fenXingList.push_back(fx);
                        }
                    }
                }
                FenXingChuLi::free = kxian;
                fx = FenXing(FenXingType::VERIFY_BOTTOM, FenXingChuLi::temp_fx.get_high(), FenXingChuLi::temp_fx.get_low(), FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free);
                FenXingChuLi::comp_fx_gao = fx.get_high();
                FenXingChuLi::status = FenXingStatus::LEFT; //重新进入middle
                return(fx);
            } else {
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = FenXingStatus::FREE;
            }
        }
    }
    fx.set_type(FenXingType::NONE);
    return(fx);
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
        if (kxian.gao > FenXingChuLi::fx.get_high()) {
            //创新高
            fx = this->__get_last_fx(kxian);
            FenXingChuLi::left = this->__get_last_kxian();
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = FenXingStatus::RIGHT;
            return(fx);
        }
        else {
            if (kxian.gao > FenXingChuLi::fx.get_low()) {
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
                if (kxian.di < FenXingChuLi::comp_fx_di) {
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
            if (kxian.di < FenXingChuLi::fx.get_low()) {
                //创新低
                fx = this->__get_last_fx(kxian);
                FenXingChuLi::left = this->__get_last_kxian();
                FenXingChuLi::middle = kxian;
                FenXingChuLi::status = FenXingStatus::RIGHT;
                return(fx);
            }
            else {
                if (kxian.di < FenXingChuLi::fx.get_high()) {
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
                    if (kxian.di > this->comp_fx_gao) {
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
        kx = {0};
    }
    return kx;
}

