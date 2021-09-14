#ifndef __BAOHANCHULI_H
#define __BAOHANCHULI_H

#include <vector>

using namespace std;

#pragma pack(push, 1)

//包含处理后的K线

//K线方向
#define Kxian_UP        1   
#define Kxian_DOWN      -1

class Kxian1
{
private:
    float gao;
    float di;
    int fangxiang;
    int position; /*从起始K线开始的位置 */
public:
    Kxian1() {
        this->di = 0;
        this->gao = 0;
        this->fangxiang = 1;
        this->position = 0;
    }
    Kxian1(float gao, float di, int direction, int position) {
        this->di = di;
        this->gao = gao;
        this->fangxiang = direction;
        this->position = position;
    }
    bool operator==(Kxian1 kx) {
        if (this->position == kx.position && this->gao == kx.gao && this->di == kx.di && this->fangxiang == kx.fangxiang)
            return(true);
        else
            return(false);
    }

    float get_high() {
        return(this->gao);
    }

    float get_low() {
        return(this->di);
    }

    int get_direction() {
        return(this->fangxiang);
    }
    int get_position() {
        return(this->position);
    }

    void set_high(float high) {
        this->gao = high;
    }

    void set_low(float low) {
        this->di = low;
    }

    void set_direction(int direction) {
        this->fangxiang = direction;
    }

    void set_position(int position) {
        this->position = position;
    }

};

class BaoHanChuLi
{
    public:
      vector<Kxian1> kxianList;
      Kxian1 add(float gao, float di);
      int count = 0;
};

#pragma pack(pop)

#endif
