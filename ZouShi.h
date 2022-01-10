#pragma once
#include "XianDuan.h"
#include "ZhongShu.h"

enum class ZouShiType {NONE, PANZHENG, UP, DOWN};
class ZouShi {
    private:
        ZouShiType type;
        float high, low, lenght;
        int start_pos, stop_pos;
        XianDuan start_xd, stop_xd;
    public:
        ZouShi() {
            this->type = ZouShiType::NONE;
            this->high = 0;
            this->low = 0;
            this->lenght = 0;
            this->start_pos = 0;
            this->stop_pos = 0;
            this->start_xd = XianDuan();
            this->stop_xd = XianDuan();
        }

        ZouShi(ZouShiType type, XianDuan start_xd, XianDuan stop_xd) {
            this->type = type;
            this->start_xd = start_xd;
            this->stop_xd = stop_xd;
            if (start_xd.get_type() == XianDuanType::UP) {
                this->low = start_xd.get_low();
                this->high = stop_xd.get_high();
            } else {
                if (start_xd.get_type() == XianDuanType::DOWN) {
                    this->high = start_xd.get_high();
                    this->low = stop_xd.get_low();
                }
            }
            this->lenght = this->high - this->low;
            this->start_pos = start_xd.get_start_pos();
            this->stop_pos = stop_xd.get_stop_pos();
        }

        int get_start_pos() {
            return(this->start_pos);
        }

        int get_stop_pos() {
            return(this->stop_pos);
        }

        XianDuan get_start_xd(){
            return(this->start_xd);
        }

        XianDuan get_stop_xd() {
            return(this->stop_xd);
        }
};

enum class ZouShiChuLiStatus {NONE, START, a, A_xd1, A_xd2, A_xd3, A, b, B_xd1, B_xd2, B_xd3, B, c};
class ZouShiChuLi {
    private:
        XianDuanChuLi xdcl;
        ZouShiChuLiStatus status;
        XianDuan a, b, c;
        XianDuan A_xd1, A_xd2, A_xd3, B_xd1, B_xd2, B_xd3;
        XianDuan b_0, b_1;
        XianDuan c_0, c_1;
        ZhongShu A, B;
        ZhongShuChuLi zhongshucl;
    public:
        ZouShiChuLi();
        XianDuan generate_xd(XianDuan xd1, XianDuan xd2, XianDuan xd3);
        void handle(vector<Kxian1>& kxianList);
        ZouShi find_zoushi(XianDuan xd);
        vector<ZouShi> zoushi_list;
};

void Bi3_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
