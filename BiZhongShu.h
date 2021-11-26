#pragma once
#include "Bi1.h"
#include <vector>

using namespace std;

enum class BiZhongShuType {NONE, PANZHENG, UP, DOWN, NEW_HIGH, NEW_LOW, REVERSE_THREE_BUY, REVERSE_THREE_SELL, THREE_BUY, THREE_SELL, FINISH};
class BiZhongShu {
private:
    BiZhongShuType type = BiZhongShuType::NONE;
    float zhongshu_high = 0;
    float zhongshu_low = 0;
    float max_high = 0;
    float min_low = 0;
    Bi input_bi = Bi();
    Bi output_bi = Bi();
public:
    vector<Bi> bi_list;
    BiZhongShu() {
        zhongshu_high = 0;
        zhongshu_low = 0;
        max_high = 0;
        min_low = 0;
    }

    BiZhongShu(Bi in, Bi a,  Bi b, Bi c) {
        this->input_bi = in;
        this->bi_list.push_back(a);
        this->bi_list.push_back(b);
        this->bi_list.push_back(c);
        this->max_high = max(a.get_high(), c.get_high());
        this->zhongshu_high = min(a.get_high(), c.get_high());
        this->min_low = min(a.get_low(), c.get_low());
        this->zhongshu_low = max(a.get_low(), c.get_low());
    }

    float get_zhongshu_high() {
        return(this->zhongshu_high);
    }

    float get_zhongshu_low() {
        return(this->zhongshu_low);
    }

    float get_max_high() {
        return(this->max_high);
    }

    void set_max_high(float high) {
        this->max_high = high;
    }

    void set_min_low(float low) {
        this->min_low = low;
    }

    float get_min_low() {
        return(this->min_low);
    }

    Bi get_output_xd() {
        return(this->output_bi);
    }

    void set_output_xd(Bi bi) {
        this->output_bi = bi;
    }

    BiZhongShuType get_type() {
        return(this->type);
    }

    Bi get_input() {
        return(this->input_bi);
    }

    void set_type(BiZhongShuType type) {
        this->type = type;
    }
};

enum class BiZhongShuChuLiStatus {NONE, TOP_DOWN, BOTTOM_UP,  INPUT, OUTPUT, THREE_BUY, THREE_SELL, FINISH, AFTER_THREE_BUY, AFTER_THREE_SELL};

class BiZhongShuChuLi {
private:
    BiZhongShu zhongshu;
    BiZhongShuChuLiStatus status;
public:
    BiZhongShuChuLi();
    BiZhongShuChuLi(Bi input, Bi xd1, Bi xd2, Bi xd3);
    BiZhongShu find_zhongshu(Bi xd);
    void set_status(BiZhongShuChuLiStatus st);
    Bi generate_bi(Bi xd);
};
