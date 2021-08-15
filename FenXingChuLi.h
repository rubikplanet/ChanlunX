#ifndef __FENXINGCHULI_H
#define __FENXINGCHULI_H

#include <vector>
#include <string>
#include "BaoHanChuLi.h"

using namespace std;

#pragma pack(push, 1)
//分型类型
#define TOP 1
#define VERIFY_TOP 2
#define FAILURE_TOP 3
#define BOTTOM -1
#define VERIFY_BOTTOM -2
#define FAILURE_BOTTOM -3
#define NORMAL 0
#define OTHER 4

//分型
struct FenXing {
    int type; //1 顶分型， -1底分型, 0-不是分型
    float gao, di;
    Kxian1 left, middle, right, free;
    int left_position; //顶、底分型的位置
    int middle_position;
    int free_position; //确认分型的位置

    bool operator==(const FenXing& d_fx) {
        return(this->type == d_fx.type && this->gao == d_fx.gao && this->di == d_fx.di && this->left_position == d_fx.left_position && this->middle_position == d_fx.middle_position);

    }
};

//分型状态
#define LEFT 0
#define MIDDLE 1
#define RIGHT 2
#define FREE_FIRST 3
#define FREE 4
#define FREE_1  5
#define FREE_2  6
#define SUCCESS 5
#define FAILURE 6

class FenXingChuLi {
    private:
        static Kxian1 left, middle, right, free;
        static int status; /* 0 left, 1 middle, 2 right, 3 free-first, 4 free, 5 success, 6 failure */
        static float comp_fx_gao, comp_fx_di;
        FenXing fx = { 0 }, temp_fx = { 0 };
        FenXing __right_process(Kxian1 kx);
        FenXing __free_process(Kxian1 kx);
        FenXing __find_fenxing(Kxian1 kx);
        FenXing set_fenxing(int type);
        FenXing __back_temp_fx(Kxian1 kx);
        FenXing __get_last_fx(FenXing fx);
        Kxian1 __get_last_kxian();
        FenXing last_fx_process(Kxian1 kx);
    public:
        vector<Kxian1> kxianList;
        vector<FenXing> fenXingList;
        FenXing handle(vector<Kxian1> &kxianList);
        Kxian1 get_kx_item(string pos);
        int get_status();
        void __initial();
        FenXing get_temp_fenxing();
};

#pragma pack(pop)

#endif
