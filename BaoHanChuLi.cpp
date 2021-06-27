#include "BaoHanChuLi.h"

using namespace std;

int BaoHanChuLi::count = 0;

Kxian1 BaoHanChuLi::add(float gao, float di)
{
    KxianRaw1 kxianRaw;
    kxianRaw.gao = gao;
    kxianRaw.di = di;
    this->kxianRawList.push_back(kxianRaw);

    if (this->kxianList.empty()) {
        //假设第一根K线方向向上
        Kxian1 kxian;
        
        kxian.gao = gao;
        kxian.di = di;
        kxian.fangxiang = 1;
        kxian.status = true;
        kxian.position = BaoHanChuLi::count;
        this->kxianList.push_back(kxian);
        BaoHanChuLi::count += 1;
        return(kxian);
    }
    else {
        if (gao > this->kxianList.back().gao && di > this->kxianList.back().di) {
            //向上
            Kxian1 kxian;

            kxian.gao = gao;
            kxian.di = di;
            kxian.fangxiang = 1;
            kxian.status = true;
            kxian.position = BaoHanChuLi::count;
            this->kxianList.push_back(kxian);
            BaoHanChuLi::count += 1;
            return(kxian);
        } else {
            if (gao < this->kxianList.back().gao && di < this->kxianList.back().di) {
                //向下
                Kxian1 kxian;

                kxian.gao = gao;
                kxian.di = di;
                kxian.fangxiang = -1;
                kxian.status = true;
                kxian.position = BaoHanChuLi::count;
                this->kxianList.push_back(kxian);
                BaoHanChuLi::count += 1;
                return(kxian);
            } else{
                Kxian1 kxian;

                kxian = this->kxianList.back();
                if (gao <= kxian.gao && di >= kxian.di){
                    //前包含
                    if (kxian.fangxiang == 1) {
                        kxian.di = di;
                    } else {
                        kxian.gao = gao;
                    }
                } else {
                    //后包含
                    if (kxian.fangxiang == 1) {
                        kxian.gao = gao;
                    } else {
                        kxian.di = di;
                    }
                }
                kxian.status = false;
                kxian.position = BaoHanChuLi::count;
                this->kxianList.push_back(kxian);
                BaoHanChuLi::count += 1;
                return(kxian);
            }
        }
    }
}
