#include "BaoHanChuLi.h"

using namespace std;

Kxian1 BaoHanChuLi::add(float gao, float di)
{
    Kxian1 kxian, last_kx;
    float last_high, last_low;
    Direction direction;
    

    if (this->kxianList.empty()) {
        //假设第一根K线方向向上
        kxian = Kxian1(gao, di, Direction::UP, BaoHanChuLi::count);
    }
    else {
        last_kx = this->kxianList.back();
        last_high = last_kx.get_high();
        last_low = last_kx.get_low();
        direction = last_kx.get_direction();

        if (gao > last_high && di > last_low) {
            //向上
            kxian = Kxian1(gao, di, Direction::UP, BaoHanChuLi::count);
        }
        else {
            if (gao < last_high && di < last_low) {
                //向下
                kxian = Kxian1(gao, di, Direction::DOWN, BaoHanChuLi::count);
            }
            else {
                this->kxianList.pop_back();
                if (gao <= last_high && di >= last_low) {
                    //1包含2
                    if (direction == Direction::UP)
                        kxian = Kxian1(last_high, di, Direction::UP, BaoHanChuLi::count);
                    else
                        kxian = Kxian1(gao, last_low, Direction::DOWN, BaoHanChuLi::count);
                }
                else {
                    //2包含1
                    if (direction == Direction::UP)
                        kxian = Kxian1(gao, last_low, Direction::UP, BaoHanChuLi::count);
                    else
                        kxian = Kxian1(last_high, di, Direction::DOWN, BaoHanChuLi::count);
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

void BaoHanChuLi::handle() {

}
