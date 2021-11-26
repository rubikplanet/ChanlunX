#include "Bi1.h"
#include "BiZhongShu.h"

using namespace std;

BiZhongShuChuLi::BiZhongShuChuLi() {
}

BiZhongShuChuLi::BiZhongShuChuLi(Bi input, Bi xd1, Bi xd2, Bi xd3) {
}

void BiZhongShuChuLi::set_status(BiZhongShuChuLiStatus status) {
    this->status = status;
}

 Bi BiZhongShuChuLi::generate_bi(Bi bi) {
     Bi new_bi = Bi();

     return(new_bi);
}

BiZhongShu BiZhongShuChuLi::find_zhongshu(Bi xd) {
    Bi first, second, output_xd, input_xd, new_xd;
    BiZhongShu zs = BiZhongShu();

    input_xd = this->zhongshu.get_input();
    if (xd.get_high() > this->zhongshu.get_max_high()) {
        this->zhongshu.set_max_high(xd.get_high());
    }
    if (xd.get_low() < this->zhongshu.get_min_low()) {
        this->zhongshu.set_min_low(xd.get_low());
    }
    switch (this->status) {
    case BiZhongShuChuLiStatus::NONE:
        break;
    case BiZhongShuChuLiStatus::BOTTOM_UP:
        if (input_xd.get_type() == BiType::DOWN) {
            if (xd.get_high() > input_xd.get_high()) {
                this->zhongshu.bi_list.push_back(xd);
                this->zhongshu.set_type(BiZhongShuType::NEW_HIGH);
                return(this->zhongshu);
            }
            else {
                if (xd.get_high() > this->zhongshu.get_zhongshu_high()) {
                    this->zhongshu.bi_list.push_back(xd);
                    this->status = BiZhongShuChuLiStatus::TOP_DOWN;
                    return(zs);
                }
                else {
                    if (xd.get_high() <= this->zhongshu.get_zhongshu_low()) {
                        this->zhongshu.bi_list.push_back(xd);
                        this->set_status(BiZhongShuChuLiStatus::THREE_SELL);
                        return(zs);
                    }
                    else {
                        this->zhongshu.bi_list.push_back(xd);
                        this->set_status(BiZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        else {
            if (input_xd.get_type() == BiType::UP) {
                if (xd.get_high() <= this->zhongshu.get_zhongshu_low()) {
                    this->zhongshu.bi_list.push_back(xd);
                    this->zhongshu.set_type(BiZhongShuType::REVERSE_THREE_SELL);
                    return(this->zhongshu);
                }
                else {
                    if (xd.get_high() > this->zhongshu.get_zhongshu_high()) {
                        this->zhongshu.bi_list.push_back(xd);
                        this->set_status(BiZhongShuChuLiStatus::TOP_DOWN);
                        return(zs);
                    }
                    else {
                        this->zhongshu.bi_list.push_back(xd);
                        this->set_status(BiZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        break;

    case BiZhongShuChuLiStatus::TOP_DOWN:
        if (input_xd.get_type() == BiType::UP) {
            if (xd.get_low() < input_xd.get_low()) {
                this->zhongshu.bi_list.push_back(xd);
                this->zhongshu.set_type(BiZhongShuType::NEW_LOW);
                return(this->zhongshu);
            }
            else {
                if (xd.get_low() < this->zhongshu.get_zhongshu_low()) {
                    this->zhongshu.bi_list.push_back(xd);
                    this->set_status(BiZhongShuChuLiStatus::BOTTOM_UP);
                    return(zs);
                }
                else {
                    if (xd.get_low() >= this->zhongshu.get_zhongshu_high()) {
                        this->zhongshu.bi_list.push_back(xd);
                        this->set_status(BiZhongShuChuLiStatus::THREE_BUY);
                        return(zs);
                    }
                    else {
                        this->zhongshu.bi_list.push_back(xd);
                        this->set_status(BiZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        else {
            if (input_xd.get_type() == BiType::DOWN) {
                if (xd.get_low() >= this->zhongshu.get_zhongshu_high()) {
                    this->zhongshu.bi_list.push_back(xd);
                    this->zhongshu.set_type(BiZhongShuType::REVERSE_THREE_BUY);
                    return(this->zhongshu);
                }
                else {
                    if (xd.get_low() < this->zhongshu.get_zhongshu_low()) {
                        this->zhongshu.bi_list.push_back(xd);
                        this->set_status(BiZhongShuChuLiStatus::BOTTOM_UP);
                        return(zs);
                    }
                    else {
                        this->zhongshu.bi_list.push_back(xd);
                        this->set_status(BiZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        break;

    case BiZhongShuChuLiStatus::INPUT:
        if (input_xd.get_type() == BiType::UP) {
            if (xd.get_high() >= this->zhongshu.get_zhongshu_high()) {
                this->set_status(BiZhongShuChuLiStatus::TOP_DOWN);
            }
            else {
                this->set_status(BiZhongShuChuLiStatus::INPUT);
            }
        }
        else {
            if (xd.get_low() <= this->zhongshu.get_zhongshu_low()) {
                this->set_status(BiZhongShuChuLiStatus::BOTTOM_UP);
            }
            else {
                this->set_status(BiZhongShuChuLiStatus::INPUT);
            }
        }
        this->zhongshu.bi_list.push_back(xd);
        return(zs);
        break;

    case BiZhongShuChuLiStatus::FINISH:
        output_xd = this->generate_bi(xd);
        this->zhongshu.set_output_xd(output_xd);
        return(this->zhongshu);

    case BiZhongShuChuLiStatus::THREE_BUY:
        if (xd.get_high() > this->zhongshu.bi_list.back().get_high()) {
            this->zhongshu.bi_list.push_back(xd);
            this->zhongshu.set_type(BiZhongShuType::FINISH);
            return(this->zhongshu);
        } else {
            this->zhongshu.bi_list.push_back(xd);
            this->set_status(BiZhongShuChuLiStatus::AFTER_THREE_BUY);
            return(zs);
        }
        break;

    case BiZhongShuChuLiStatus::AFTER_THREE_BUY:
        new_xd = this->generate_bi(xd);
        if (new_xd.get_low() < input_xd.get_low()) {
            this->zhongshu.bi_list.push_back(new_xd);
            this->zhongshu.set_type(BiZhongShuType::NEW_LOW);
            return(this->zhongshu);
        }
        else {
            if (new_xd.get_low() < this->zhongshu.get_zhongshu_low()) {
                this->zhongshu.bi_list.push_back(new_xd);
                this->set_status(BiZhongShuChuLiStatus::BOTTOM_UP);
                return(zs);
            }
            else {
                if (new_xd.get_low() >= this->zhongshu.get_zhongshu_high()) {
                    this->zhongshu.bi_list.push_back(new_xd);
                    this->set_status(BiZhongShuChuLiStatus::THREE_BUY);
                    return(zs);
                }
                else {
                    this->zhongshu.bi_list.push_back(new_xd);
                    this->set_status(BiZhongShuChuLiStatus::INPUT);
                    return(zs);
                }
            }
        }
        break;

    case BiZhongShuChuLiStatus::THREE_SELL:
        if (xd.get_low() < this->zhongshu.bi_list.back().get_low()) {
            this->zhongshu.bi_list.push_back(xd);
            this->zhongshu.set_type(BiZhongShuType::FINISH);
            return(this->zhongshu);
        } else {
            this->zhongshu.bi_list.push_back(xd);
            this->set_status(BiZhongShuChuLiStatus::AFTER_THREE_SELL);
            return(zs);
        }
        break;

    case BiZhongShuChuLiStatus::AFTER_THREE_SELL:
        new_xd = this->generate_bi(xd);
        this->zhongshu.bi_list.push_back(new_xd);
        if (new_xd.get_high() > input_xd.get_high()) {
            this->zhongshu.set_type(BiZhongShuType::NEW_HIGH);
            return(this->zhongshu);
        }
        else {
            if (new_xd.get_high() > this->zhongshu.get_zhongshu_high()) {
                this->set_status(BiZhongShuChuLiStatus::TOP_DOWN);
                return(zs);
            }
            else {
                if (new_xd.get_high() <= this->zhongshu.get_zhongshu_low()) {
                    this->set_status(BiZhongShuChuLiStatus::THREE_SELL);
                    return(zs);
                }
                else {
                    this->set_status(BiZhongShuChuLiStatus::INPUT);
                    return(zs);
                }
            }
        }
        break;
    }
    return(zs);
}
