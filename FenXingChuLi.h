#ifndef __FENXINGCHULI_H
#define __FENXINGCHULI_H

#include <vector>
#include <string>
#include "BaoHanChuLi.h"

using namespace std;

#pragma pack(push, 1)

enum class FenXingType { NONE, TOP, BOTTOM, FAILURE_TOP, FAILURE_BOTTOM, VERIFY_TOP, VERIFY_BOTTOM, FAILURE_VERIFY_TOP, FAILURE_VERIFY_BOTTOM };
enum class HighLowType {NONE, NEW_HIGH, NEW_LOW};

//分型
class FenXing {
private:
    FenXingType type = FenXingType::NONE;
    HighLowType high_low_type = HighLowType::NONE;
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

    FenXing(Kxian1 left, Kxian1 middle, Kxian1 right, Kxian1 free) {
        if (middle.get_high() > left.get_high()) {
            this->type = FenXingType::TOP;
            this->gao = middle.get_high();
            this->di = min(left.get_low(), right.get_low());
        }
        else {
            this->type = FenXingType::BOTTOM;
            this->di = middle.get_low();
            this->gao = max(left.get_high(), right.get_high());
        }
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

    void set_free(Kxian1 kx) {
        this->free = kx;
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

    void set_high_low_type(HighLowType status) {
        this->high_low_type = status;
    }

    HighLowType get_high_low_type() {
        return(this->high_low_type);
    }

    bool operator==(FenXing comp) {
        if (this->type == comp.type && this->left == comp.get_left() && this->middle == comp.get_middle() && this->right == comp.get_right())
            return(true);
        else
            return(false);
    }
};

//enum class FenXingChuLiStatus { LEFT, MIDDLE, RIGHT, FREE };
enum class FenXingChuLiStatus {ONE, TWO, THREE, FOUR, FIVE, SIX, FENXING_END};

class FenXingChuLi {
private:
    //static Kxian1 left, middle, right, free, last_bar;
    static Kxian1 one, two, three, four, five, six, last_bar;
    static float min_low, max_high;
    static FenXingChuLiStatus status;
    static float comp_fx_gao, comp_fx_di;
    static int comp_fx_gao_count, comp_fx_di_count;
    static FenXing fx, temp_fx, last_fx;
    bool __highlow_process(Kxian1 kx);
    FenXing __quekou_process(Kxian1 kx);

public:
    vector<Kxian1> kxianList;
    vector<FenXing> fenXingList;
    vector<FenXing> keyKxianList;
    void handle(vector<Kxian1>& kxianList);
    FenXing __find_fenxing(Kxian1 kx);
    FenXingChuLi();
};

//extern "C" _declspec(dllexport) void Bi3(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi3_fenxing(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_fenxing(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void OutputDebugPrintf(const char* strOutputString, ...);

#pragma pack(pop)

#endif
