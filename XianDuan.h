#pragma once
#include "bi1.h"
#include "ZhongShu1.h"
#include<vector>

using namespace std;

enum class XianDuanType {NONE, TEMP_UP, TEMP_DOWN, FAILURE_TEMP_UP, FAILURE_TEMP_DOWN, UP, DOWN, FAILURE_UP, FAILURE_DOWN };
class XianDuan {
private:
    XianDuanType type = XianDuanType::NONE;
    Bi left, after_left, middle, after_middle;
    Bi start_bi, stop_bi;
    float high, low;
    float length;

public:
    XianDuan() {
        this->type = XianDuanType::NONE;
        this->high = 0;
        this->low = 0;
        this->length = 0;
        this->start_bi = Bi();
        this->stop_bi = Bi();
        this->left = Bi();
        this->after_left = Bi();
        this->middle = Bi();
        this->after_middle = Bi();
    }

    XianDuan(Bi start_bi, Bi stop_bi) {
        this->start_bi = start_bi;
        this->stop_bi = stop_bi;
        this->high = 0;
        this->low = 0;
        this->length = 0;
        if (start_bi.get_type() == BiType::DOWN) {
            this->high = start_bi.get_high();
            this->low = stop_bi.get_low();
            this->length = this->high - this->low;
        }
        else {
            if (start_bi.get_type() == BiType::UP) {
                this->low = start_bi.get_low();
                this->high = stop_bi.get_high();
                this->length = this->high - this->low;
            }
        }
    }

    XianDuanType get_type() {
        return(this->type);
    }

    void set_type(XianDuanType xd_type) {
        this->type = xd_type;
    }

    Bi get_start_bi() {
        return(this->start_bi);
    }

    Bi get_stop_bi() {
        return(this->stop_bi);
    }

    bool operator==(XianDuan that) {
        if (this->start_bi == that.start_bi && this->stop_bi == that.stop_bi && this->type == that.type)
            return(true);
        else
            return(false);
    }
};

enum class XianDuanChuLiStatus { LEFT, AFTER_LEFT, MIDDLE, MIDDLE_HIGH_LOW, MIDDLE_NORMAL, AFTER_MIDDLE, ZHONGSHU_A, ZHONGSHU_B };

class XianDuanChuLi {
private:
    static XianDuan last_xd;
    static XianDuanChuLiStatus status;
    static Bi last_bi;
    ZhongShu1 zhongshu_a, zhongshu_b;
    Bi left = Bi();
    Bi after_left = Bi();
    Bi middle = Bi();
    Bi  after_middle = Bi();
    Bi right = Bi();
    BiChuLi bicl;
    XianDuan find_xianduan(Bi bi);
    XianDuan failure_xd(Bi first_bi, Bi second_bi);
    Bi generate_bi(Bi a, Bi b, Bi c);
    Bi update_stop_bi(Bi a, FenXing fx);
public:
    XianDuanChuLi();
    void handle(vector<Kxian1>& kxlist);
    vector<XianDuan> xianDuanList;
    vector<XianDuan> keyXianDuanList;
};

void Bi3_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
