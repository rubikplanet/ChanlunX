#ifndef __FENXINGCHULI_H
#define __FENXINGCHULI_H

#include <vector>
#include <string>
#include "BaoHanChuLi.h"

using namespace std;

#pragma pack(push, 1)

//分型
struct FenXing {
    int type; //1 顶分型， -1底分型, 0-不是分型
    bool failure_status; //true，分型失败
    float gao, di;
    Kxian1 left, middle, right, free;
    int left_position; //顶、底分型的位置
    int middle_position;
    int free_position; //确认分型的位置
    vector<Kxian1> kxianList;
};

#define LEFT 0
#define MIDDLE 1
#define RIGHT 2
#define FREE_FIRST 3
#define FREE 4
#define SUCCESS 5
#define FAILURE 6

class FenXingChuLi {
    private:
        static Kxian1 left, middle, right, free;
        static int status; /* 0 left, 1 middle, 2 right, 3 free-first, 4 free, 5 success, 6 failure */
        FenXing fx, temp_fx;
        void __right_process(Kxian1 kx);
        FenXing __free_process(Kxian1 kx);
        FenXing __find_fenxing(Kxian1 kx);
        FenXing __back_temp_fx(Kxian1 kx);
        Kxian1 __get_last_kxian();
        FenXing __last_fx_process(Kxian1 kx);
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
