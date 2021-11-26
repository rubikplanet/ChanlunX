#pragma once
#include "XianDuan.h"
#include "ZhongShu1.h"

using namespace std;

enum class ZouShiType {NONE, PANZHENG, UP, DOWN};
class ZouShi {
    private:
        ZouShiType type = ZouShiType::NONE;
        XianDuan a, b, c;
    public:
        ZouShi() {
        };

        ZouShiType get_type() {
            return(this->type);
        }
        void set_type(ZouShiType zs_type) {
            this->type = zs_type;
        }
}; 

enum class ZouShiChuLiStatus {A_INPUT, A_XD1, A_XD2, A_XD3, Find_ZhongShu_A,  B_INPUT, B_XD1, B_XD2, B_XD3, Find_ZhongShu_B, C};
class BiZouShiChuLi {
    private:
        ZouShiChuLiStatus status = ZouShiChuLiStatus::Find_ZhongShu_A;
        XianDuan a, b, c;
        XianDuan a_input, a_xd_1, a_xd_2, a_xd_3;
        XianDuan b_input, b_xd_1, b_xd_2, b_xd_3;
        ZhongShu1 A, B;
        XianDuanChuLi xdcl;
    public:
        BiZouShiChuLi();
        void handle(vector<Kxian1>& kxianList);
        ZouShi find_zoushi(XianDuan xd);
        vector<ZouShi> zoushi_list;
        
};

void Bi3_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
