#pragma once
#include "bi1.h"
#include <vector>
#include "bi_zhongshu.h"

using namespace std;

enum class Bi_ZouShiChuLiStatus { NONE, a_0, a_1, a_2, a_2_highlow, a_2_normal, a_2_normal_normal, a_2_longxd, a_2_longxd_normal, a_2_longxd_normal_normal, a_3, A_a0, A_a1, A_a2, A, b_1, b_2, B_b0, b_2_normal, b_2_longxd, b_2_longxd_normal, b_3, B_b1, B_b2, B, c_0, c_1, c_2, c_2_normal };

enum class Bi_ZouShiType {NONE, UP, DOWN};
enum class Bi_ZouShiKind {NONE, LONGXIANDUAN, PANZHENG, UP, DOWN};
class Bi_ZouShi {
    private:
        Bi_ZouShiType type;
        Bi_ZouShiKind kind;
        vector <Bi> bi_list;
        Bi start_bi, stop_bi;
        int start_pos, stop_pos, verify_pos;
        float high, low;
        float length;
        HighLowType high_low_type;
        Bi_ZouShiChuLiStatus stop_status;
    public:
        Bi_ZouShi() {
            this->type = Bi_ZouShiType::NONE;
            this->kind = Bi_ZouShiKind::NONE;
            this->high = 0;
            this->low = 0;
            this->length = 0;
            this->start_bi = Bi();
            this->stop_bi = Bi();
            this->start_pos = 0;
            this->stop_pos = 0;
            this->verify_pos = 0;
            this->high_low_type = HighLowType::NONE;
            this->stop_status = Bi_ZouShiChuLiStatus::NONE;
            this->bi_list.clear();
        }

        Bi_ZouShi(Bi_ZouShiKind kind, Bi start_bi, Bi stop_bi, Bi_ZouShiChuLiStatus status, vector<Bi> bi_list){
            if (start_bi.get_type() == BiType::UP)
                this->type = Bi_ZouShiType::UP;
            else {
                if (start_bi.get_type() == BiType::DOWN)
                    this->type = Bi_ZouShiType::DOWN;
            }
            this->kind = kind;
            this->start_bi = start_bi;
            this->stop_bi = stop_bi;
            if (start_bi.get_type() == BiType::UP) {
                this->low = start_bi.get_low();
                this->high = stop_bi.get_high();
            } else {
                this->high = start_bi.get_high();
                this->low = stop_bi.get_low();
            }
            this->length = this->high - this->low;
            this->start_pos = start_bi.get_start_pos();
            this->stop_pos = stop_bi.get_stop_pos();
            this->verify_pos = stop_bi.get_stop_verify_position();
            this->high_low_type = this->get_stop_bi().get_high_low_type();
            this->stop_status = status;
            this->bi_list = bi_list;
        }

        Bi_ZouShiType get_type() {
            return(this->type);
        }

        Bi_ZouShiKind get_kind() {
            return(this->kind);
        }
        
        int get_start_pos() {
            return(this->start_pos);
        }

        int get_stop_pos() {
            return(this->stop_pos);
        }

        Bi get_start_bi(){
            return(this->start_bi);
        }

        Bi get_stop_bi() {
            return(this->stop_bi);
        }

        vector<Bi> get_bilist() {
            return(this->bi_list);
        }

        Bi_ZouShiChuLiStatus get_stop_status() {
            return(this->stop_status);
        }
};

enum class FindBiZouShiReturnType {None, Failue, One, Two, Three};
struct FindBiZouShiReturn{
    FindBiZouShiReturnType type;
    Bi_ZouShi bi_zoushi_1;
    Bi_ZouShi bi_zoushi_2;
    Bi_ZouShi bi_zoushi_3;
};

class Bi_ZouShiChuLi {
    private:
        BiChuLi bicl;
        Bi_ZouShiChuLiStatus status;
        Bi a, b, c;
        Bi a_0, a_1, a_2, a_3, a_4;
        Bi A_a0, A_a1, A_a2;
        Bi b_0, b_1, b_2, b_3, b_4;
        Bi B_b0, B_b1, B_b2;
        Bi c_0, c_1, c_2, c_3, c_4;
        Bi_ZhongShuChuLi bi_zhongshu_chuli;
        Bi_ZhongShu A, B;
    public:
        vector<Bi> bi_list;
        Bi_ZouShiChuLi();
        void handle(vector<Kxian1>& kxianList);
        vector<Bi_ZouShi> bi_zoushi_list;
        vector<Bi_ZhongShu> bi_zhongshu_list;
        FindBiZouShiReturn bi_zoushi_failure(Bi_ZouShi bi_zoushi);
        FindBiZouShiReturn find_bi_zoushi(Bi bi);
        void enter_zhongshu(Bi bi, Bi_ZouShiChuLiStatus status);
        vector<Bi> save_bi_list(Bi_ZouShiChuLiStatus status);
};

void bi_zhongshu_start_stop(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgon);
void bi_zhongshu_high(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgnoe);
void bi_zhongshu_low(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgnoe);

void Bi3_bi_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgon);
void Bi4_bi_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgno);
