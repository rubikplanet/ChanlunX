#include "BaoHanChuLi.h"

using namespace std;

Kxian1 BaoHanChuLi::add(float gao, float di)
{
    Kxian1 kxian;

    if (this->kxianList.empty()) {
        //假设第一根K线方向向上
        kxian = Kxian1(gao, di, Kxian_UP, BaoHanChuLi::count);
    }
    else {
        if (gao > this->kxianList.back().get_high() && di > this->kxianList.back().get_low()) {
            //向上
            kxian = Kxian1(gao, di, Kxian_UP, BaoHanChuLi::count);
        }
        else {
            if (gao < this->kxianList.back().get_high() && di < this->kxianList.back().get_low()) {
                //向下
                kxian = Kxian1(gao, di, Kxian_DOWN, BaoHanChuLi::count);
            }
            else {
                kxian = this->kxianList.back();
                this->kxianList.pop_back();
                if (gao <= kxian.get_high() && di >= kxian.get_low()) {
                    //1包含2
                    if (kxian.get_direction() == Kxian_UP)
                        kxian = Kxian1(kxian.get_high(), di, Kxian_UP, BaoHanChuLi::count);
                    else
                        kxian = Kxian1(gao, kxian.get_low(), Kxian_DOWN, BaoHanChuLi::count);
                }
                else {
                    //2包含1
                    if (kxian.get_direction() == Kxian_UP)
                        kxian = Kxian1(gao, kxian.get_low(), Kxian_UP, BaoHanChuLi::count);
                    else
                        kxian = Kxian1(kxian.get_high(), di, Kxian_DOWN, BaoHanChuLi::count);
                }
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
