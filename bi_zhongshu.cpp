#include "Bi_ZhongShu.h"

using namespace std;

Bi_ZhongShuChuLi::Bi_ZhongShuChuLi(){
    this->bi_zhongshu = Bi_ZhongShu();
}

Bi_ZhongShuChuLi::Bi_ZhongShuChuLi(Bi input, Bi xd1, Bi xd2, Bi xd3) {
    this->bi_zhongshu = Bi_ZhongShu(input, xd1, xd2, xd3);

    if (xd3.get_type() == BiType::UP && xd3.get_high() > this->bi_zhongshu.get_high()) {
        this->status = Bi_ZhongShuChuLiStatus::UP;
    } else {
        if (xd3.get_type() == BiType::DOWN && xd3.get_low() < this->bi_zhongshu.get_low())
            this->status = Bi_ZhongShuChuLiStatus::DOWN;
        else
            this->status = Bi_ZhongShuChuLiStatus::INSIDE;
    }
}

Bi_ZhongShu Bi_ZhongShuChuLi::get_Bi_ZhongShu() {
    return(this->bi_zhongshu);
}

float get_bi_radio(Bi bi1, Bi bi2) {
    float radio = 0.0;
    float bi1_len = bi1.get_length();
    float bi2_len = bi2.get_length();
    if (bi2_len < bi1_len)
        radio = bi2_len / bi1_len;
    else
        radio = bi1_len / bi2_len * -1;
    return(radio);
}

Bi_ZhongShu Bi_ZhongShuChuLi::find_Bi_ZhongShu(Bi xd) {
    Bi last_bi = this->bi_zhongshu.bi_list.back();
    this->bi_zhongshu.bi_list.push_back(xd);
    Bi_ZhongShu ret_Bi_ZhongShu = Bi_ZhongShu();
    float bi_high = xd.get_high();
    float bi_low = xd.get_low();
    float bi_radio = 0.0;

    if (this->bi_zhongshu.bi_list.size() >= 9) {
        //TODO: 中枢升级
        printf("TODO 中枢升级");
    } else {
        switch(this->status) {

            case Bi_ZhongShuChuLiStatus::INSIDE:
                if (xd.get_type() == BiType::UP) {
                    if (bi_high > this->bi_zhongshu.get_high()) {
                        this->status = Bi_ZhongShuChuLiStatus::UP;
                        if (bi_high > this->bi_zhongshu.get_max_high())
                            this->bi_zhongshu.set_max_high(xd);
                    } else {
                        this->status = Bi_ZhongShuChuLiStatus::INSIDE;
                    }
                } else {
                    if (xd.get_type() == BiType::DOWN) {
                        if (bi_low < this->bi_zhongshu.get_low()) {
                            this->status = Bi_ZhongShuChuLiStatus::DOWN;
                            if (bi_low < this->bi_zhongshu.get_min_low())
                                this->bi_zhongshu.set_min_low(xd);
                        } else {
                            this->status = Bi_ZhongShuChuLiStatus::INSIDE;
                        }
                    }
                }
                break;

            case Bi_ZhongShuChuLiStatus::UP:
                if (this->bi_zhongshu.get_input().get_type() == BiType::UP) {
                    bi_radio = get_bi_radio(last_bi, xd);
                    if (abs(bi_radio) < 0.5) {
                        this->bi_zhongshu.stop();
                        return(this->bi_zhongshu);
                    }
                } else {
                    if (bi_low >= this->bi_zhongshu.get_high()) {
                        //3买
                        this->status = Bi_ZhongShuChuLiStatus::THREE_BUY;
                        this->bi_zhongshu.stop();
                        return(this->bi_zhongshu);
                    } else {
                        if (bi_low < this->bi_zhongshu.get_low())
                            this->status = Bi_ZhongShuChuLiStatus::DOWN;
                            if (bi_low < this->bi_zhongshu.get_min_low()) {
                                this->bi_zhongshu.set_min_low(xd);
                            }
                        else
                            this->status = Bi_ZhongShuChuLiStatus::INSIDE;
                    }
                }

                break;

            case Bi_ZhongShuChuLiStatus::DOWN:
                if (this->bi_zhongshu.get_input().get_type() == BiType::DOWN) {
                    bi_radio = get_bi_radio(last_bi, xd);
                    if (abs(bi_radio) < 0.5) {
                        this->bi_zhongshu.stop();
                        return(this->bi_zhongshu);
                    }
                } else {
                    if (bi_high <= this->bi_zhongshu.get_low()) {
                        //3卖
                        this->status = Bi_ZhongShuChuLiStatus::THREE_SELL;
                        this->bi_zhongshu.stop();
                        return(this->bi_zhongshu);
                    } else {
                        if (bi_high > this->bi_zhongshu.get_high()) 
                            this->status = Bi_ZhongShuChuLiStatus::UP;
                            if (bi_high > this->bi_zhongshu.get_max_high()) {
                                this->bi_zhongshu.set_max_high(xd);
                            }
                        else
                            this->status = Bi_ZhongShuChuLiStatus::INSIDE;
                    }
                }
                break;

            default:
                printf("find_Bi_ZhongShu Status Error: [%d]", this->status);
        }
    }
    return(ret_Bi_ZhongShu);
}
