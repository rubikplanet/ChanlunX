#ifndef __FENXINGCHULI_H
#define __FENXINGCHULI_H

#include <vector>
#include <string>
#include "BaoHanChuLi.h"

using namespace std;

#pragma pack(push, 1)

enum class FenXingType {NONE, TOP, BOTTOM, FAILURE_TOP, FAILURE_BOTTOM, VERIFY_TOP, VERIFY_BOTTOM, FAILURE_VERIFY_TOP, FAILURE_VERIFY_BOTTOM};

//分型
class FenXing {
    private:
        FenXingType type = FenXingType::NONE;
        float gao = 0;
        float di = 0;
        Kxian1 left = { 0 };
        Kxian1 middle = { 0 };
        Kxian1 right = { 0 };
        Kxian1 free = { 0 };
    public:
        FenXing() {
            this->type = FenXingType::NONE;
            this->gao = 0;
            this->di = 0;
            this->left = this->middle = this->right = this->free = { 0 };
        }

        FenXing(FenXingType type, float gao, float di, Kxian1 left, Kxian1 middle, Kxian1 right) {
            this->type = type;
            this->gao = gao;
            this->di = di;
            this->left = left;
            this->middle = middle;
            this->right = right;
        }

        FenXing(FenXingType type, float gao, float di, Kxian1 left, Kxian1 middle, Kxian1 right, Kxian1 free) {
            this->type = type;
            this->gao = gao;
            this->di = di;
            this->left = left;
            this->middle = middle;
            this->right = right;
            this->free = free;
        }

        FenXingType get_type() {
            return(this->type);
        }

        void set_type(FenXingType type) {
            this->type = type;
        }

        float get_high() {
            return(this->gao);
        }

        float get_low() {
            return(this->di);
        }

        int get_start_position() {
            return(this->middle.position);
        }

        int get_stop_position() {
            return(this->free.position);
        }
};

enum class FenXingStatus {LEFT, MIDDLE, RIGHT, FREE_FIRST, FREE};

class FenXingChuLi {
    private:
        static Kxian1 left,middle, right, free;
        static FenXingStatus status;
        static float comp_fx_gao, comp_fx_di;
        static FenXing fx, temp_fx;
        FenXing __right_process(Kxian1 kx);
        FenXing __free_process(Kxian1 kx);
        FenXing __find_fenxing(Kxian1 kx);
        FenXing __set_fenxing(FenXingType type, float gao, float di);
        FenXing __back_temp_fx(Kxian1 kx);
        Kxian1 __get_last_kxian();
        FenXing __last_fx_process(Kxian1 kx);
        FenXing __get_last_fx(Kxian1 kx);
        void __set_fx(FenXing fx);
        void __set_temp_fx(FenXing fx);
    public:
        vector<Kxian1> kxianList;
        vector<FenXing> fenXingList;
        vector<FenXing> keyKxianList;
        FenXing handle(vector<Kxian1> &kxianList);
        Kxian1 get_kx_item(string pos);
        int get_status();
        FenXingChuLi();
        FenXing get_temp_fenxing();
};

#pragma pack(pop)

#endif
