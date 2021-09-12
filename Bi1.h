#ifndef __BI1_H__
#define __BI1_H__
#include "BaoHanChuLi.h"
#include "FenXingChuLi.h"
#include <vector>

using namespace std;

#pragma pack(push, 1)

enum class BiType {NONE, UP, DOWN, FAILURE_UP, FAILURE_DOWN, TEMP_TOP, TEMP_BOTTOM, FAILURE_TEMP_TOP, FAILURE_TEMP_BOTTOM, NEW_UP, NEW_DOWN};
class Bi {
private:
    BiType type = BiType::NONE;
    FenXing start_fx = FenXing();
    FenXing stop_fx = FenXing();
    float high = 0.0;
    float low = 0.0;
    float length = 0.0;
public:
    Bi() {
        this->type = BiType::NONE;
        this->start_fx = FenXing();
        this->stop_fx = FenXing();
        this->high = 0;
        this->low = 0;
        this->length = 0;
    }

    Bi(FenXing start, FenXing stop) {
        this->start_fx = start;
        this->stop_fx = stop;
        if (start.get_type() == FenXingType::VERIFY_TOP) {
            this->type = BiType::DOWN;
            this->high = start.get_middle().gao;
            this->low = stop.get_middle().di;
        }
        else {
            if (start.get_type() == FenXingType::VERIFY_BOTTOM) {
                this->type = BiType::UP;
                this->high = stop.get_middle().gao;
                this->low = start.get_middle().di;
            }
        }
        this->length = this->high - this->low;
    }

    BiType get_type() {
        return(this->type);
    }

    void set_type(BiType bi_type) {
        this->type = bi_type;
    }

    float get_high() {
        return(this->high);
    }

    float get_low() {
        return(this->low);
    }

    float get_length() {
        return(this->length);
    }

    FenXing get_start_fx() {
        return(this->start_fx);
    }

    FenXing get_stop_fx() {
        return(this->stop_fx);
    }
};


enum class BiChuLiStatus {START, FINDBI, FINDNEWBI};

class BiChuLi {
private:
    BiChuLiStatus status = BiChuLiStatus::START;
    FenXing last_fx = FenXing();
    FenXingChuLi fxcl;
    Bi find_new_bi(FenXing tmp_fx);
public:
    vector<Bi> biList;
    Bi last_bi = Bi();
    BiChuLi();
    void handle(vector<Kxian1> &kxlist);
    Bi __find_bi(Kxian1 kx);
    Bi now_bi(Kxian1 kx);
};


//extern "C" _declspec(dllexport) void Bi3(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi3(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
#pragma pack(pop)
#endif
