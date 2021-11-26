﻿#pragma once
#include "bi1.h"
#include<vector>

using namespace std;

enum class XianDuanType {NONE, TEMP_UP, TEMP_DOWN, FAILURE_TEMP_UP, FAILURE_TEMP_DOWN, UP, DOWN, FAILURE_UP, FAILURE_DOWN, LONG_XIANDUAN };
class XianDuan {
private:
    XianDuanType type = XianDuanType::NONE;
    vector <Bi>bi_list;
    Bi start_bi, stop_bi;
    int start_pos, stop_pos, verify_pos;
    float high, low;
    float length;
    HighLowType high_low_type;

public:
    XianDuan() {
        this->type = XianDuanType::NONE;
        this->high = 0;
        this->low = 0;
        this->length = 0;
        this->start_bi = Bi();
        this->stop_bi = Bi();
        this->start_pos = 0;
        this->stop_pos = 0;
        this->verify_pos = 0;
        this->high_low_type = HighLowType::NONE;
    }

    XianDuan(Bi start_bi, Bi stop_bi) {
        this->start_bi = start_bi;
        this->stop_bi = stop_bi;
        this->high = 0;
        this->low = 0;
        this->length = 0;
        this->start_pos = start_bi.get_start_fx().get_middle().get_position();
        this->stop_pos = stop_bi.get_stop_fx().get_middle().get_position();
        this->verify_pos = stop_bi.get_stop_fx().get_free().get_position();
        if (start_bi.get_type() == BiType::DOWN) {
            this->high = start_bi.get_high();
            this->low = stop_bi.get_low();
            this->length = this->high - this->low;
            this->type = XianDuanType::DOWN;
        }
        else {
            if (start_bi.get_type() == BiType::UP) {
                this->low = start_bi.get_low();
                this->high = stop_bi.get_high();
                this->length = this->high - this->low;
                this->type = XianDuanType::UP;
            }
        }
        this->high_low_type = stop_bi.get_high_low_type();
    }

    XianDuanType get_type() {
        return(this->type);
    }

    void set_type(XianDuanType xd_type) {
        this->type = xd_type;
    }

    float get_high() {
        return(this->high);
    }

    float get_low() {
        return(this->low);
    }

    Bi get_start_bi() {
        return(this->start_bi);
    }

    Bi get_stop_bi() {
        return(this->stop_bi);
    }

    int get_start_pos() {
        return(this->start_pos);
    }

    int get_stop_pos() {
        return(this->stop_pos);
    }

    int get_stop_verify_pos() {
        return(this->verify_pos);
    }

    bool operator==(XianDuan that) {
        if (this->start_bi == that.start_bi && this->stop_bi == that.stop_bi && this->type == that.type)
            return(true);
        else
            return(false);
    }

    void save_bi_list(vector<Bi> bilist) {
        this->bi_list = bilist;
    }

    vector<Bi> get_bi_list() {
        return(this->bi_list);
    }
    XianDuan generate_xd(XianDuan xd1, XianDuan xd2, XianDuan xd3) {
        Bi start_bi, stop_bi;
        XianDuan tmp_xd;

        start_bi = xd1.get_start_bi();
        stop_bi = xd3.get_stop_bi();
        tmp_xd = XianDuan(start_bi, stop_bi);
        return(tmp_xd);
    }
};

enum class XianDuanChuLiStatus { START, LEFT, AFTER_LEFT, MIDDLE_HIGHLOW, MIDDLE_NORMAL, LEFT_INCLUDE_MIDDLE,  AFTER_MIDDLE, RIGHT_HIGHLOW, RIGHT_NORMAL,  MIDDLE_INCLUDE_RIGHT, AFTER_RIGHT, FREE, AFTER_FREE};

enum class FindXianDuanReturnType {None, Failure, One, Two, Three};
struct FindXianDuanReturn {
    FindXianDuanReturnType type;
    XianDuan xd1;
    XianDuan xd2;
    XianDuan xd3;
};

class XianDuanChuLi {
private:
    static XianDuan last_xd;
    static XianDuanChuLiStatus status;
    static Bi last_bi;

    Bi start = Bi();
    Bi left = Bi();
    Bi after_left = Bi();
    Bi middle = Bi();
    Bi  after_middle = Bi();
    Bi right = Bi();
    Bi after_right = Bi();
    Bi free = Bi();
    Bi after_free = Bi();
    BiChuLi bicl;

    FindXianDuanReturn find_xianduan(Bi bi);
    FindXianDuanReturn __find_xianduan(Bi bi);
    void __backroll(Bi bi);
    FindXianDuanReturn failure_xd(Bi first_bi, Bi second_bi);
    FindXianDuanReturn __right_process(Bi bi);
public:
    XianDuanChuLi();
    void handle(vector<Kxian1>& kxlist);
    vector<Bi> get_xd_bi_list();
    
    vector<XianDuan> xianDuanList;
    vector<XianDuan> key_xianduan_list;
};

void Bi3_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);