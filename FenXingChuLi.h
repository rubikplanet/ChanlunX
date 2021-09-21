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
        Kxian1 left = Kxian1();
        Kxian1 middle = Kxian1();
        Kxian1 right = Kxian1();
        Kxian1 free = Kxian1();
    public:
        FenXing() {
            this->type = FenXingType::NONE;
            this->gao = 0;
            this->di = 0;
            this->left = this->middle = this->right = this->free = Kxian1();
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
            return(this->middle.get_position());
        }

        int get_stop_position() {
            return(this->free.get_position());
        }

        Kxian1 get_left() {
            return(this->left);
        }

        Kxian1 get_middle() {
            return(this->middle);
        }

        Kxian1 get_right() {
            return(this->right);
        }

        Kxian1 get_free() {
            return(this->free);
        }

        bool operator==(FenXing comp) {
            if (this->type == comp.type && this->left == comp.left && this->middle == comp.middle && this->right == comp.right)
                return(true);
            else
                return(false);
        }
};

enum class FenXingStatus {LEFT, MIDDLE, RIGHT, FREE_FIRST, FREE, TOP, FREE_NEW_TOP, VERIFY_TOP, AFTER_VERIFY_TOP, VERIFY_NEW_TOP, BACK_ZHONGYIN_TOP,  BOTTOM, FREE_NEW_BOTTOM, VERIFY_BOTTOM, AFTER_VERIFY_BOTTOM, VERIFY_NEW_BOTTOM, BACK_ZHONGYIN_BOTTOM};

class FenXingChuLi {
    private:
        static Kxian1 left,middle, right, free, last_bar;
        static FenXingStatus status;
        static float comp_fx_gao, comp_fx_di;
        static int comp_fx_gao_count, comp_fx_di_count;
        static float min_low, max_high;
        static FenXing fx, temp_fx;
        FenXing __right_process(Kxian1 kx);
        Kxian1 __get_last_kxian();
        FenXing __last_fx_process(Kxian1 kx);
        FenXing __get_last_fx(Kxian1 kx);
        void __set_fx(FenXing fx);
        void __set_temp_fx(FenXing fx);
        bool _find_new_highlow(Kxian1 kx);
    public:
        vector<Kxian1> kxianList;
        vector<FenXing> fenXingList;
        vector<FenXing> keyKxianList;
        void handle(vector<Kxian1> &kxianList);
        FenXing __find_fenxing(Kxian1 kx);
        Kxian1 get_kx_item(string pos);
        FenXingChuLi();
};

#pragma pack(pop)

#endif
