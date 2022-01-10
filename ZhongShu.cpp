#include "ZhongShu.h"

using namespace std;

ZhongShuChuLi::ZhongShuChuLi(){
    this->zhongshu = ZhongShu();
}

ZhongShuChuLi::ZhongShuChuLi(XianDuan input, XianDuan xd1, XianDuan xd2, XianDuan xd3) {
    this->zhongshu = ZhongShu();
    this->zhongshu.start(input, xd1, xd2, xd3);
    if (xd3.get_type() == XianDuanType::UP && xd3.get_high() > this->zhongshu.get_high()) {
        this->status = ZhongShuChuLiStatus::UP;
    } else {
        if (xd3.get_type() == XianDuanType::DOWN && xd3.get_low() < this->zhongshu.get_low())
            this->status = ZhongShuChuLiStatus::DOWN;
        else
            this->status = ZhongShuChuLiStatus::INSIDE;
    }
}

ZhongShu ZhongShuChuLi::find_zhongshu(XianDuan xd) {
    this->zhongshu.xd_list.push_back(xd);
    ZhongShu ret_zhongshu = ZhongShu();
    if (this->zhongshu.xd_list.size() >= 9) {
        //TODO: 中枢升级
        printf("TODO 中枢升级");
    } else {
        switch(this->status) {
            case ZhongShuChuLiStatus::INSIDE:
                if (xd.get_type() == XianDuanType::UP) {
                    if (xd.get_high() > this->zhongshu.get_high()) {
                        this->status = ZhongShuChuLiStatus::DOWN;
                        if (xd.get_high() > this->zhongshu.get_max_high())
                            this->zhongshu.set_max_high(xd.get_high());
                    } else {
                        this->status = ZhongShuChuLiStatus::INSIDE;
                    }
                } else {
                    if (xd.get_type() == XianDuanType::DOWN) {
                        if (xd.get_low() < this->zhongshu.get_low()) {
                            this->status = ZhongShuChuLiStatus::UP;
                            if (xd.get_low() < this->zhongshu.get_min_low())
                                this->zhongshu.set_min_low(xd.get_low());
                        } else {
                            this->status = ZhongShuChuLiStatus::INSIDE;
                        }
                    }
                }
                break;
            case ZhongShuChuLiStatus::UP:
                if (xd.get_low() >= this->zhongshu.get_high()) {
                    //Three Buy
                    this->status = ZhongShuChuLiStatus::THREE_BUY;
                    this->zhongshu.stop();
                    return(this->zhongshu);
                } else {
                    if (xd.get_low() < this->zhongshu.get_low())
                        this->status = ZhongShuChuLiStatus::DOWN;
                    else
                        this->status = ZhongShuChuLiStatus::INSIDE;
                }
                break;

            case ZhongShuChuLiStatus::DOWN:
                if (xd.get_high() <= this->zhongshu.get_low()) {
                    //Three Sell
                    this->status = ZhongShuChuLiStatus::THREE_SELL;
                    this->zhongshu.stop();
                    return(this->zhongshu);
                } else {
                    if (xd.get_high() > this->zhongshu.get_high()) 
                        this->status = ZhongShuChuLiStatus::UP;
                    else
                        this->status = ZhongShuChuLiStatus::INSIDE;
                }
                break;

            default:
                printf("find_zhongshu Status Error: [%d]", this->status);
        }
    }
    return(ret_zhongshu);
}
