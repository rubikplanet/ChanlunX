#include "Bi1.h"
#include "ZhongShu1.h"

using namespace std;

ZhongShuChuLi::ZhongShuChuLi(ZhongShu1 zs) {
    this->zhongshu = zs;
}

void ZhongShuChuLi::set_status(ZhongShuChuLiStatus status) {
    this->status = status;
}

ZhongShu1 ZhongShuChuLi::find_zhongshu(Bi bi) {
    int count;
    Bi first, second, output_bi;
    ZhongShu1 zs = ZhongShu1();
    if (bi.get_high() > this->zhongshu.get_max_high()) {
        this->zhongshu.set_max_high(bi.get_high());
    }
    if (bi.get_low() < this->zhongshu.get_min_low()) {
        this->zhongshu.set_min_low(bi.get_low());
    }
    switch (this->status) {
    case ZhongShuChuLiStatus::NONE:
        break;
    case ZhongShuChuLiStatus::INPUT:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() <= this->zhongshu.get_zhongshu_low()) {
                //3卖
                this->set_status(ZhongShuChuLiStatus::THREE_SELL);
            } 
        }
        else {
            if (bi.get_type() == BiType::DOWN) {
                if (bi.get_low() >= this->zhongshu.get_zhongshu_high()) {
                    //3买
                    this->set_status(ZhongShuChuLiStatus::THREE_BUY);
                }
            }
        }
        break;

    case ZhongShuChuLiStatus::OUTPUT:
        count = this->zhongshu.biList.size();
        first = this->zhongshu.biList.at(count - 2);
        second = this->zhongshu.biList.at(count-1);
        output_bi = output_bi.generate_bi(first, second, bi);
        this->zhongshu.set_output_bi(output_bi);
        return(this->zhongshu);

    case ZhongShuChuLiStatus::THREE_BUY:
        count = this->zhongshu.biList.size();
        if (bi.get_high() > this->zhongshu.biList.back().get_high()) {
            this->set_status(ZhongShuChuLiStatus::OUTPUT);
        } else {
            this->set_status(ZhongShuChuLiStatus::INPUT);
        }
        break;
    case ZhongShuChuLiStatus::THREE_SELL:
        if (bi.get_low() < this->zhongshu.biList.back().get_low()) {
            this->set_status(ZhongShuChuLiStatus::OUTPUT);
        } else {
            this->set_status(ZhongShuChuLiStatus::INPUT);
        }
        break;

    }
    if (bi.get_high() > this->zhongshu.get_zhongshu_high()) {
        this->set_status(ZhongShuChuLiStatus::INPUT);
    }
    else {
        if (bi.get_low() < this->zhongshu.get_zhongshu_low()) {
            this->set_status(ZhongShuChuLiStatus::INPUT);
        }
        else {
            this->set_status(ZhongShuChuLiStatus::OUTPUT);
        }
    }
    this->zhongshu.biList.push_back(bi);
    return(zs);
}
