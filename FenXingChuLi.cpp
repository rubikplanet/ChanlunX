#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"
#include <string>

using namespace std;

int FenXingChuLi::status = 0;
Kxian1 FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free;

void FenXingChuLi::__initial() {
    FenXingChuLi::status = 0;
    Kxian1 kx = {0};
    FenXingChuLi::left = kx;
    FenXingChuLi::middle = kx;
    FenXingChuLi::right = kx;
    FenXingChuLi::free = kx;
}

void FenXingChuLi::__right_process(Kxian1 kxian){
    if (FenXingChuLi::middle.gao > FenXingChuLi::left.gao) {
        //middle.gao > left.gao
        if (kxian.gao > FenXingChuLi::middle.gao) {
            //继续新高
            FenXingChuLi::left = FenXingChuLi::middle;
            FenXingChuLi::middle = kxian;
            this->status = 2;
        } else {
            //顶分型
            FenXingChuLi::right = kxian;
            FenXingChuLi::status = 3;
            this->temp_fx.type = 1;
            this->temp_fx.gao = FenXingChuLi::middle.gao;
            this->temp_fx.di = min(FenXingChuLi::left.di, FenXingChuLi::right.di);
        }
    } else {
        //middle.gao < left.gao
        if (kxian.di < FenXingChuLi::middle.di){
            //继续新低
            FenXingChuLi::left = FenXingChuLi::middle;
            FenXingChuLi::middle = kxian;
            this->status = 2;
        } else {
            //底分型
            FenXingChuLi::right = kxian;
            FenXingChuLi::status = 3;
            this->temp_fx.type = -1;
            this->temp_fx.di = FenXingChuLi::middle.di;
            this->temp_fx.gao = max(FenXingChuLi::left.gao, FenXingChuLi::right.gao);
        }
    }
}

FenXing FenXingChuLi::__free_process(Kxian1 kxian){
    if (this->temp_fx.type == 1) {
        //顶分型
        if (kxian.gao > this->temp_fx.gao) {
            //创新高
            if (FenXingChuLi::status == 3) {
                FenXingChuLi::left = FenXingChuLi::right;
            }else {
                FenXingChuLi::left = FenXingChuLi::free;
            }
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = 2;
        } else {
            if (kxian.gao <= this->temp_fx.di) {
                //离开中阴段,顶分型成功
                this->fx.type = this->temp_fx.type;
                this->fx.gao = this->temp_fx.gao;
                this->fx.di = this->temp_fx.di;
                this->fx.left = FenXingChuLi::left;
                this->fx.middle = FenXingChuLi::middle;
                this->fx.right = FenXingChuLi::right;
                this->fx.free = FenXingChuLi::free;
                this->fx.middle_position= this->fx.middle.position;
                this->fx.left_position = this->fx.left.position;
                this->fx.free_position = this->fx.free.position;
                for(auto i = this->fx.left_position; i < this->fx.free_position; ++i)
                    this->fx.kxianList.push_back(this->kxianList[i]);
                this->fenXingList.push_back(this->fx);
                
                FenXingChuLi::left = kxian;
                FenXingChuLi::status = 0; //重新进入left
                return(this->fx);
            } else {
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = 4;
            }
        }
    } else {
        //底分型
        if (kxian.di < this->temp_fx.di) {
            //创新低
            if (FenXingChuLi::status == 3) {
                FenXingChuLi::left = FenXingChuLi::right;
            }else {
                FenXingChuLi::left = FenXingChuLi::free;
            }
            FenXingChuLi::middle = kxian;
            FenXingChuLi::status = 2;
        } else {
            if (kxian.di >= this->temp_fx.gao) {
                //离开中阴段,底分型成功
                this->fx.type = this->temp_fx.type;
                this->fx.gao = this->temp_fx.gao;
                this->fx.di = this->temp_fx.di;
                this->fx.left = FenXingChuLi::left;
                this->fx.middle = FenXingChuLi::middle;
                this->fx.right = FenXingChuLi::right;
                this->fx.free = FenXingChuLi::free;
                this->fx.middle_position= this->fx.middle.position;
                this->fx.left_position = this->fx.left.position;
                this->fx.free_position = this->fx.free.position;
                for(auto i = this->fx.left_position; i < this->fx.free_position; ++i)
                    this->fx.kxianList.push_back(this->kxianList[i]);
                
                //this->fenXingList.push_back(this->fx);
                FenXingChuLi::left = kxian;
                FenXingChuLi::status = 0; //重新进入left
                return(this->fx);
            } else {
                FenXingChuLi::free = kxian;
                FenXingChuLi::status = 4;
            }
        }
    }
    FenXing fx = {0};
    return(fx);
}

FenXing FenXingChuLi::handle(vector<Kxian1> &kxianList) {
    FenXing fx;
    this->kxianList = kxianList;
    for (vector<Kxian1>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++) {
        fx = this->__find_fenxing((*iter));
        if (fx.type == 1 || fx.type == -1)
            this->fenXingList.push_back(fx);
            (*iter).fx_type = fx.type;
    }
    return(fx);
}

FenXing FenXingChuLi::__back_temp_fx(Kxian1 kxian) {
    FenXing last_fx = this->fenXingList.back();
    this->fenXingList.pop_back();
    this->temp_fx.gao = last_fx.gao;
    this->temp_fx.di = last_fx.di;
    FenXingChuLi::left = last_fx.left;
    FenXingChuLi::middle = last_fx.middle;
    FenXingChuLi::right = last_fx.right;
    FenXingChuLi::free = kxian;
    FenXingChuLi::status = 4;
    return(last_fx);
}

FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    if (this->fenXingList.size() > 0) {
        if (this->fenXingList.back().type == 1) {
            //上一个分型是顶分型
            if (kxian.gao > this->fenXingList.back().di) {
                //重新进入中阴阶段
                if (FenXingChuLi::status == 4) {
                    return(this->__free_process(kxian));
                } else {
                    return(this->__back_temp_fx(kxian));
                }
            }
        } else {
            if (this->fenXingList.back().type == -1) {
                //上一个分型是底分型
                if (kxian.di < this->fenXingList.back().gao) {
                    //重新进入中阴阶段
                    if (FenXingChuLi::status == 4) {
                        return(this->__free_process(kxian));
                    } else {
                        return(this->__back_temp_fx(kxian));
                    }
                }
            }
        }
    }

    switch (FenXingChuLi::status) {
        case 0: //left
            if (kxian.status == false) {
                //和上一根K线有包含关系, free处理
            } else {
                FenXingChuLi::left = kxian;
                FenXingChuLi::status = 1;
            }
            break;
        case 1: //middle
            if (kxian.status == false){
                //left处理
                FenXingChuLi::left = kxian;
                FenXingChuLi::status = 1;
            } else {
                FenXingChuLi::middle = kxian;
                FenXingChuLi::status = 2;
            }
            break;
        case 2: //right
            //分型处理
            if (kxian.status == false) {
                FenXingChuLi::middle = kxian;
                FenXingChuLi::status = 2;
            } else {
                this->__right_process(kxian);
            }
            break;
        case 3: //free_first
            if (kxian.status == false){
                //right处理
                this->__right_process(kxian);
            } else {
                return(this->__free_process(kxian));
            }
            break;
        case 4: //free
            return(this->__free_process(kxian));
    }
    FenXing fx;
    fx.type = 0;
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
