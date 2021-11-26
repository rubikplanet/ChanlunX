#pragma once
#include "Bi1.h"
#include "BiZhongShu.h"

using namespace std;

enum class BiZouShiType {NONE, FAILURE, PANZHENG, UP, DOWN};
class BiZouShi {
    private:
        BiZouShiType type = BiZouShiType::NONE;
        Bi a, b, c;
        float high, low;
        int start_pos, stop_pos;
        float length;
        Bi start, stop;

    public:
        BiZouShi() {
        };

        BiZouShi(Bi start, Bi stop) {
            this->start = start;
            this->stop = stop;
            if (start.get_type() == BiType::UP) {
                this->high = stop.get_high();
                this->low = start.get_low();
            }
            else {
                this->high = start.get_high();
                this->low = stop.get_low();
            }
            this->length = this->high - this->low;
        }

        BiZouShiType get_type() {
            return(this->type);
        }

        void set_type(BiZouShiType zs_type) {
            this->type = zs_type;
        }

        Bi get_start() {
            return(this->start);
        }

        Bi get_stop() {
            return(this->stop);
        }
}; 

enum class BiZouShiChuLiStatus {A_INPUT, A_XD1, A_XD2, A_XD3, Find_ZhongShu_A,  B_INPUT, B_XD1, B_XD2, B_XD3, Find_ZhongShu_B, C};
class BiZouShiChuLi {
    private:
        BiZouShiChuLiStatus status = BiZouShiChuLiStatus::Find_ZhongShu_A;
        Bi a, b, c;
        Bi a_input, a_xd_1, a_xd_2, a_xd_3;
        Bi b_input, b_xd_1, b_xd_2, b_xd_3;
        BiZhongShu A, B;
        BiChuLi bicl;
        BiZouShi last_zoushi;
    public:
        BiZouShiChuLi();
        void handle(vector<Kxian1>& kxianList);
        BiZouShi find_zoushi(Bi xd);
        BiZouShi failure_zoushi(Bi bi);
        vector<BiZouShi> zoushi_list;
        
};

void Bi3_bizoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_bizoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
