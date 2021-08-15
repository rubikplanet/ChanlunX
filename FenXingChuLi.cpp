#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"
#include <string>

using namespace std;

int FenXingChuLi::status = LEFT;
Kxian1 FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free;
float FenXingChuLi::comp_fx_di, FenXingChuLi::comp_fx_gao;

void FenXingChuLi::__initial() {
    FenXingChuLi::status = 0;
    Kxian1 kx = { 0 };
    FenXingChuLi::left = kx;
    FenXingChuLi::middle = kx;
    FenXingChuLi::right = kx;
    FenXingChuLi::free = kx;
    FenXingChuLi::comp_fx_di = 0;
    FenXingChuLi::comp_fx_gao = 0;
    this->fx = { 0 };
    this->temp_fx = { 0 };
}

FenXing FenXingChuLi::set_fenxing(int fenxing_type) {
    FenXing fx = FenXing();
    float price;
    switch (fenxing_type) {
        case NORMAL:
            fx = { 0 };
            return(fx);
        case TOP:
            fx.gao = FenXingChuLi::middle.gao;
            price = min(FenXingChuLi::left.di, FenXingChuLi::right.di);
            //fx.di = fx.gao - (fx.gao - price) / 2;
            fx.di = price;
            break;
        case BOTTOM:
            fx.di = FenXingChuLi::middle.di;
            price = max(FenXingChuLi::left.gao, FenXingChuLi::right.gao);
            //fx.gao = fx.di + (price - fx.di) / 2;
            fx.gao = price;
            break;
        case VERIFY_TOP:
        case VERIFY_BOTTOM:
            fx.di = this->temp_fx.di;
            fx.gao = this->temp_fx.gao;
            fx.free = FenXingChuLi::free;
            fx.free_position = fx.free.position;
            break;
        case FAILURE_TOP:
        case FAILURE_BOTTOM:
            fx = this->fx;
            fx.type = fenxing_type;
            return(fx);
    }
    fx.type = fenxing_type;
    fx.left = FenXingChuLi::left;
    fx.left_position = fx.left.position;
    fx.middle = FenXingChuLi::middle;
    fx.middle_position = fx.middle.position;
    fx.right = FenXingChuLi::right;
    return(fx);
}

FenXing FenXingChuLi::__right_process(Kxian1 kxian){
    if (FenXingChuLi::middle.gao > FenXingChuLi::left.gao) {
        //middle.gao > left.gao
        if (kxian.gao > FenXingChuLi::middle.gao) {
            //继续新高
            FenXingChuLi::left = FenXingChuLi::middle;
            FenXingChuLi::middle = kxian;
            this->status = RIGHT;
        } else {
            //顶分型
            FenXingChuLi::right = kxian;
            FenXingChuLi::status = FREE_FIRST;
            this->temp_fx = this->set_fenxing(TOP);
            return(this->temp_fx);
        }
    } else {
        //middle.gao < left.gao
        if (kxian.di < FenXingChuLi::middle.di){
            //继续新低
            FenXingChuLi::left = FenXingChuLi::middle;
            FenXingChuLi::middle = kxian;
            this->status = RIGHT;
        } else {
            //底分型
            FenXingChuLi::right = kxian;
            FenXingChuLi::status = FREE_FIRST;
            this->temp_fx = this->set_fenxing(BOTTOM);
            return(this->temp_fx);
        }
    }
    FenXing fx = { 0 };
    return(fx);
}

FenXing FenXingChuLi::__free_process(Kxian1 kxian){
    if (this->temp_fx.type == TOP) {
        //顶分型
        if (kxian.gao > this->temp_fx.gao) {
            //创新高
            if (FenXingChuLi::status == FREE_FIRST) {
                FenXingChuLi::left = FenXingChuLi::right;
            }else {
                FenXingChuLi::left = FenXingChuLi::free;
            }
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = RIGHT;
        } else {
            if (kxian.di < this->temp_fx.di) {
                //离开中阴段,顶分型成功
                FenXingChuLi::free = kxian;
                this->fx = this->set_fenxing(VERIFY_TOP);
                FenXingChuLi::status = LEFT; //重新进入left
                FenXingChuLi::comp_fx_di = kxian.di;
                return(this->fx);
            } else {
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = FREE;
            }
        }
    } else {
        //底分型
        if (kxian.di < this->temp_fx.di) {
            //创新低
            if (FenXingChuLi::status == FREE_FIRST) {
                FenXingChuLi::left = FenXingChuLi::right;
            }else {
                FenXingChuLi::left = FenXingChuLi::free;
            }
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = RIGHT;
        } else {
            if (kxian.di <= this->temp_fx.gao) {
                //离开中阴段,底分型成功
                FenXingChuLi::free = kxian;
                this->fx = this->set_fenxing(VERIFY_BOTTOM);
                FenXingChuLi::status = LEFT; //重新进入left
                FenXingChuLi::comp_fx_gao = kxian.gao;
                return(this->fx);
            } else {
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = FREE;
            }
        }
    }
    FenXing fx = {0};
    return(fx);
}

FenXing FenXingChuLi::handle(vector<Kxian1> &kxianList) {
    FenXing fx = { 0 };
    FenXingChuLi::status = LEFT;
    this->kxianList = kxianList;
    for (vector<Kxian1>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++) {
        fx = this->__find_fenxing((*iter));
        if (fx.type != 0)
            this->fenXingList.push_back(fx);
    }
    return(fx);
}

FenXing FenXingChuLi::__back_temp_fx(Kxian1 kxian) {
    this->fenXingList.pop_back();
    FenXing last_fx = this->fenXingList.back();
    this->temp_fx.gao = last_fx.gao;
    this->temp_fx.di = last_fx.di;
    FenXingChuLi::left = last_fx.left;
    FenXingChuLi::middle = last_fx.middle;
    FenXingChuLi::right = last_fx.right;
    FenXingChuLi::free = kxian;
    FenXingChuLi::status = 4;
    return(last_fx);
}

Kxian1 FenXingChuLi::__get_last_kxian() {
    Kxian1 last_kx = { 0 };
    switch (FenXingChuLi::status) {
        case 0:
            last_kx = FenXingChuLi::free;
            break;
        case 1:
            last_kx = FenXingChuLi::left;
            break;
        case 2:
            last_kx = FenXingChuLi::middle;
            break;
        case 3:
            last_kx = FenXingChuLi::right;
            break;
        case 4:
            last_kx = FenXingChuLi::free;
            break;
    }
    return(last_kx);
}


FenXing FenXingChuLi::__get_last_fx(FenXing fx) {
    FenXing tmp_fx = { 0 };
    int begin = 0, stop;
    for (int num = this->fenXingList.size(); num > 0; num--) {
        if (this->fenXingList[num] == fx) {
            begin = num;
            break;
        }
    }
    if (begin > 0) {
        for (int num = begin - 1; num > 0; num--) {
            if (this->fenXingList[num].type == VERIFY_BOTTOM || this->fenXingList[num].type == VERIFY_TOP) {
                tmp_fx = this->fenXingList[num];
            }
        }
    }
    return(tmp_fx);
}

FenXing FenXingChuLi::last_fx_process(Kxian1 kxian) {
    FenXing fx = FenXing();
    if (this->fx.type == 0) {
        fx = { 0 };
        return(fx);
    }

    if (this->fx.type == VERIFY_TOP) {
        //上一个为验证顶分型
        if (kxian.gao > this->fx.gao) {
            //创新高
            fx = this->set_fenxing(FAILURE_TOP);
            FenXingChuLi::left = this->__get_last_kxian();
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = RIGHT;
            this->fx = this->__get_last_fx(this->fx);
            return(fx);
        }
        else {
            if (kxian.gao > this->fx.di) {
                //回到上一个顶分型的中阴区间
                fx = this->set_fenxing(FAILURE_TOP);
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = FREE;
                this->temp_fx = fx;
                this->temp_fx.type = TOP;
                this->fx = this->__get_last_fx(this->fx);
                return(fx);
            }
            else {
                if (kxian.di < FenXingChuLi::comp_fx_di) {
                    if (FenXingChuLi::status == LEFT) {
                        FenXingChuLi::left = kxian;
                        FenXingChuLi::status = MIDDLE;
                        FenXingChuLi::comp_fx_di = kxian.di;
                    }
                    else {
                        if (FenXingChuLi::status == MIDDLE) {
                            FenXingChuLi::middle = kxian;
                            FenXingChuLi::status = RIGHT;
                        }
                    }
                }
            }
        }
    }
    else {
        if (this->fx.type == VERIFY_BOTTOM) {
            //上一个为底分型
            if (kxian.di < this->fx.di) {
                //创新高
                fx = this->set_fenxing(FAILURE_BOTTOM);
                FenXingChuLi::left = this->__get_last_kxian();
                FenXingChuLi::middle = kxian;
                FenXingChuLi::status = RIGHT;
                this->fx = this->__get_last_fx(this->fx);
                return(fx);
            }
            else {
                if (kxian.gao > this->fenXingList.back().di) {
                    //回到上一个顶分型的中阴区间
                    fx = this->set_fenxing(FAILURE_BOTTOM);
                    FenXingChuLi::free = kxian;
                    FenXingChuLi::status = FREE;
                    this->temp_fx = fx;
                    this->temp_fx.type = TOP;
                    this->fx = this->__get_last_fx(this->fx);
                    return(fx);
                }
                else {
                    if (kxian.gao > FenXingChuLi::comp_fx_gao) {
                        if (FenXingChuLi::status == LEFT) {
                            FenXingChuLi::left = kxian;
                            FenXingChuLi::status = MIDDLE;
                            FenXingChuLi::comp_fx_gao = kxian.gao;
                        }
                        else {
                            if (FenXingChuLi::status == MIDDLE) {
                                FenXingChuLi::middle = kxian;
                                FenXingChuLi::status = RIGHT;
                            }
                        }
                    }
                }
            }
        }
    }
    fx = { 0 };
    return(fx);
}

FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();

    switch (FenXingChuLi::status) {
        case LEFT: //left
            tmp_fx = this->last_fx_process(kxian);
            if (tmp_fx.type != NORMAL) {
                return(tmp_fx);
            }
            else {
                FenXingChuLi::left = kxian;
                FenXingChuLi::status = MIDDLE;
            }
            break;
        case MIDDLE: //middle
            //tmp_fx = this->last_fx_process(kxian);
            if (tmp_fx.type != NORMAL) {
                return(tmp_fx);
            }
            else {
                FenXingChuLi::middle = kxian;
                FenXingChuLi::status = RIGHT;
            }
            break;
        case RIGHT: //right
            //分型处理
            return(this->__right_process(kxian));
            break;
        case FREE_FIRST: //free_first
            return(this->__free_process(kxian));
            break;
        case FREE: //free
            return(this->__free_process(kxian));
    }
    FenXing fx = { 0 };
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

int FenXingChuLi::get_status() {
    return(FenXingChuLi::status);
}

FenXing FenXingChuLi::get_temp_fenxing() {
    return(this->temp_fx);
}
