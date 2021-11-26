#include "BiZouShi.h"
#include "BiZhongShu.h"
#include "Bi1.h"

using namespace std;

BiZouShiChuLi::BiZouShiChuLi() {
    this->a = Bi();
    this->b = Bi();
    this->c = Bi();
    this->A = BiZhongShu();
    this->B = BiZhongShu();
    this->bicl = BiChuLi();
    this->last_zoushi = BiZouShi();
}

void BiZouShiChuLi::handle(vector<Kxian1>& kxianList) {
    BiZouShi zoushi = BiZouShi();
    Bi bi;
    this->bicl.handle(kxianList);

    int count = this->bicl.biList.size();
    for (int i = 0; i < count; i++) {
        bi = this->bicl.biList[i];
        zoushi = this->find_zoushi(bi);
        BiZouShiType zs_type = zoushi.get_type();
        switch(zs_type) {
            case BiZouShiType::PANZHENG:
                this->zoushi_list.push_back(zoushi);
                break;
            case BiZouShiType::UP:
                this->zoushi_list.push_back(zoushi);
                break;
            case BiZouShiType::DOWN:
                this->zoushi_list.push_back(zoushi);
                break;
            case BiZouShiType::NONE:
                break;
        }
    }

}

BiZouShi BiZouShiChuLi::failure_zoushi(Bi bi) {
    BiZouShi bizoushi = BiZouShi();
    if (this->last_zoushi.get_type() == BiZouShiType::NONE) {
        bizoushi = BiZouShi(this->a_input, bi);
    }
    else {
        bizoushi = BiZouShi(this->last_zoushi.get_start(), bi);
        bizoushi.set_type(BiZouShiType::FAILURE);
    }
    return(bizoushi);
}

BiZouShi BiZouShiChuLi::find_zoushi(Bi bi) {
    BiZhongShu zs = BiZhongShu();
    BiZouShi zoushi = BiZouShi();

    switch(this->status) {
        case BiZouShiChuLiStatus::A_INPUT:
            this->a_input = bi;
            this->status = BiZouShiChuLiStatus::A_XD1;
            break;
        case BiZouShiChuLiStatus::A_XD1:
            if (bi.get_type() == BiType::UP && bi.get_high() > this->a_input.get_high()) {
                //创新高
                return(this->failure_zoushi(bi));
            }
            else {
                if (bi.get_type() == BiType::DOWN && bi.get_low() < this->a_input.get_low()) {
                    //创新低
                    return(this->failure_zoushi(bi));
                }
                else {
                    this->a_xd_1 = bi;
                    this->status = BiZouShiChuLiStatus::A_XD2;
                }
            }
            break;
        case BiZouShiChuLiStatus::A_XD2:
            this->a_xd_2 = bi;
            this->status = BiZouShiChuLiStatus::A_XD3;
            break;
        case BiZouShiChuLiStatus::A_XD3:
            this->a_xd_3 = bi;
            zs = BiZhongShu(this->a_input, this->a_xd_1, this->a_xd_2, this->a_xd_3);
            this->status = BiZouShiChuLiStatus::Find_ZhongShu_A;
           //this->zscl = ZhongShuChuLi(this->a_input, this->a_xd_1, this->a_xd_2, this->a_xd_3);
            break;
        case BiZouShiChuLiStatus::Find_ZhongShu_A:
            /* zs = zscl.find_zhongshu(xd);
            ZhongShuType zs_type = zs.get_type();
            if (zs_type != ZhongShuType::NONE) {
                switch (zs_type) {
                    case ZhongShuType::THREE_BUY:
                        break;
                    case ZhongShuType::THREE_SELL:
                        break;
                }
            }
            */
            break;
        case BiZouShiChuLiStatus::B_XD1:
            break;
        case BiZouShiChuLiStatus::B_XD2:
            break;
        case BiZouShiChuLiStatus::B_XD3:
            break;
        case BiZouShiChuLiStatus::Find_ZhongShu_B:
            break;
    }
    return(zoushi);
}

void Bi3_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuLi;
    BiZouShiChuLi zoushichuli;
    BiZouShi zoushi;
    int start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuLi.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    zoushichuli.handle(baohanChuLi.kxianList);


}

void Bi4_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {

}
