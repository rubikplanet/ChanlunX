#include "BaoHanChuLi.h"

using namespace std;

Kxian1 BaoHanChuLi::add(float gao, float di)
{
    Kxian1 kxian;

    if (this->kxianList.empty()) {
        //假设第一根K线方向向上
        kxian.gao = gao;
        kxian.di = di;
        kxian.fangxiang = Kxian_UP;
        kxian.position = BaoHanChuLi::count;
    }
    else {
        if (gao > this->kxianList.back().gao && di > this->kxianList.back().di) {
            //向上
            kxian.gao = gao;
            kxian.di = di;
            kxian.fangxiang = Kxian_UP;
            kxian.position = BaoHanChuLi::count;
        }
        else {
            if (gao < this->kxianList.back().gao && di < this->kxianList.back().di) {
                //向下
                kxian.gao = gao;
                kxian.di = di;
                kxian.fangxiang = Kxian_DOWN;
                kxian.position = BaoHanChuLi::count;
            }
            else {
                kxian = this->kxianList.back();
                this->kxianList.pop_back();
                if (gao <= kxian.gao && di >= kxian.di) {
                    //1包含2
                    if (kxian.fangxiang == Kxian_UP)
                        kxian.di = di;
                    else
                        kxian.gao = gao;
                }
                else {
                    //2包含1
                    if (kxian.fangxiang == Kxian_UP)
                        kxian.gao = gao;
                    else
                        kxian.di = di;
                }
                kxian.position = BaoHanChuLi::count;
                this->kxianList.push_back(kxian);
                BaoHanChuLi::count += 1;
                return(kxian);
            }
        }
    }

    this->kxianList.push_back(kxian);
    BaoHanChuLi::count += 1;
    return(kxian);
}
