#pragma once
#include "XianDuan.h"

class ZhongShu {
    private:
        float high, low, max_high, min_low;
        XianDuan input_xd, output_xd;
    public:
        vector<XianDuan> xd_list;
        ZhongShu() {
            this->input_xd = XianDuan();
            this->output_xd = XianDuan();
            this->high = 0;
            this->low = 0;
            this->max_high = 0;
            this->min_low = 0;
            this->xd_list.clear();
        }

        int start(XianDuan in, XianDuan xd1, XianDuan xd2, XianDuan xd3) {
            this->input_xd = in;
            this->xd_list.push_back(xd1);
            this->xd_list.push_back(xd2);
            this->xd_list.push_back(xd3);
            this->max_high = max(xd1.get_high(), xd3.get_high());
            this->high = min(xd1.get_high(), xd3.get_high());
            this->min_low = min(xd1.get_low(), xd3.get_low());
            this->low = max(xd1.get_low(), xd3.get_low());
            return(1);           
        } 

        int stop(XianDuan xd) {
            this->output_xd = xd;
            return(1);
        }

        float get_high() {
            return(this->high);
        }

        float get_max_high() {
            return(this->max_high);
        } 

        void set_max_high(float max_high) {
            this->max_high = max_high;
        }    

        float get_low() {
            return(this->low);
        }

        float get_min_low() {
            return(this->min_low);
        }

        void set_min_low(float min_low) {
            this->min_low = min_low;
        }

        XianDuan get_input() {
            return(this->input_xd);
        }

        XianDuan get_output() {
            return(this->output_xd);
        }
};

enum class ZhongShuChuLiStatus {NONE, UP, DOWN, INSIDE, THREE_BUY, THREE_SELL};
class ZhongShuChuLi {
    private:
        ZhongShu zhongshu;
        ZhongShuChuLiStatus status;
    public:
        ZhongShuChuLi();
        ZhongShuChuLi(XianDuan input, XianDuan xd1, XianDuan xd2, XianDuan xd3);
        ZhongShu find_zhongshu(XianDuan xd);
        ZhongShu get_zhongshu();
};