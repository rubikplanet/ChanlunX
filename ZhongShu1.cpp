#include "Bi1.h"
#include "ZhongShu1.h"

using namespace std;

ZhongShuChuLi::ZhongShuChuLi() {
}

ZhongShuChuLi::ZhongShuChuLi(ZhongShu1 zs) {
    this->zhongshu = zs;
}

void ZhongShuChuLi::set_status(ZhongShuChuLiStatus status) {
    this->status = status;
}

Bi ZhongShuChuLi::generate_bi(Bi bi) {
    Bi first, second, new_bi;

    second = this->zhongshu.biList.back();
    this->zhongshu.biList.pop_back();
    first = this->zhongshu.biList.back();
    this->zhongshu.biList.pop_back();
    new_bi = Bi();
    new_bi.generate_bi(first, second, bi);
    this->zhongshu.biList.push_back(new_bi);
    return(new_bi);
}

ZhongShu1 ZhongShuChuLi::find_zhongshu(Bi bi) {
    Bi first, second, output_bi, input_bi, new_bi;
    ZhongShu1 zs = ZhongShu1();

    input_bi = this->zhongshu.get_input();
    if (bi.get_high() > this->zhongshu.get_max_high()) {
        this->zhongshu.set_max_high(bi.get_high());
    }
    if (bi.get_low() < this->zhongshu.get_min_low()) {
        this->zhongshu.set_min_low(bi.get_low());
    }
    switch (this->status) {
    case ZhongShuChuLiStatus::NONE:
        break;
    case ZhongShuChuLiStatus::BOTTOM_UP:
        if (input_bi.get_type() == BiType::DOWN) {
            if (bi.get_high() > input_bi.get_high()) {
                this->zhongshu.biList.push_back(bi);
                this->zhongshu.set_type(ZhongShuType::NEW_HIGH);
                return(this->zhongshu);
            }
            else {
                if (bi.get_high() > this->zhongshu.get_zhongshu_high()) {
                    this->zhongshu.biList.push_back(bi);
                    this->status = ZhongShuChuLiStatus::TOP_DOWN;
                    return(zs);
                }
                else {
                    if (bi.get_high() <= this->zhongshu.get_zhongshu_low()) {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::THREE_SELL);
                        return(zs);
                    }
                    else {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        else {
            if (input_bi.get_type() == BiType::UP) {
                if (bi.get_high() <= this->zhongshu.get_zhongshu_low()) {
                    this->zhongshu.biList.push_back(bi);
                    this->zhongshu.set_type(ZhongShuType::REVERSE_THREE_SELL);
                    return(this->zhongshu);
                }
                else {
                    if (bi.get_high() > this->zhongshu.get_zhongshu_high()) {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::TOP_DOWN);
                        return(zs);
                    }
                    else {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        break;

    case ZhongShuChuLiStatus::TOP_DOWN:
        if (input_bi.get_type() == BiType::UP) {
            if (bi.get_low() < input_bi.get_low()) {
                this->zhongshu.biList.push_back(bi);
                this->zhongshu.set_type(ZhongShuType::NEW_LOW);
                return(this->zhongshu);
            }
            else {
                if (bi.get_low() < this->zhongshu.get_zhongshu_low()) {
                    this->zhongshu.biList.push_back(bi);
                    this->set_status(ZhongShuChuLiStatus::BOTTOM_UP);
                    return(zs);
                }
                else {
                    if (bi.get_low() >= this->zhongshu.get_zhongshu_high()) {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::THREE_BUY);
                        return(zs);
                    }
                    else {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        else {
            if (input_bi.get_type() == BiType::DOWN) {
                if (bi.get_low() >= this->zhongshu.get_zhongshu_high()) {
                    this->zhongshu.biList.push_back(bi);
                    this->zhongshu.set_type(ZhongShuType::REVERSE_THREE_BUY);
                    return(this->zhongshu);
                }
                else {
                    if (bi.get_low() < this->zhongshu.get_zhongshu_low()) {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::BOTTOM_UP);
                        return(zs);
                    }
                    else {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        break;

    case ZhongShuChuLiStatus::INPUT:
        if (input_bi.get_type() == BiType::UP) {
            if (bi.get_low() < input_bi.get_low()) {
                new_bi = this->generate_bi(bi);
                this->zhongshu.biList.push_back(new_bi);
                this->zhongshu.set_type(ZhongShuType::NEW_LOW);
                return(this->zhongshu);
            }
            else {
                if (bi.get_low() >= this->zhongshu.get_zhongshu_low()) {
                    this->zhongshu.biList.push_back(bi);
                    this->set_status(ZhongShuChuLiStatus::INPUT);
                    return(zs);
                }
                else {
                    this->zhongshu.biList.push_back(bi);
                    this->set_status(ZhongShuChuLiStatus::BOTTOM_UP);
                    return(zs);
                }
            }
        }
        else {
            if (input_bi.get_type() == BiType::DOWN) {
                if (bi.get_high() > input_bi.get_high()) {
                    new_bi = this->generate_bi(bi);
                    this->zhongshu.biList.push_back(new_bi);
                    this->zhongshu.set_type(ZhongShuType::NEW_HIGH);
                    return(this->zhongshu);
                }
                else {
                    if (bi.get_high() > this->zhongshu.get_zhongshu_high()) {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::TOP_DOWN);
                        return(zs);
                    }
                    else {
                        this->zhongshu.biList.push_back(bi);
                        this->set_status(ZhongShuChuLiStatus::INPUT);
                        return(zs);
                    }
                }
            }
        }
        break;

    case ZhongShuChuLiStatus::FINISH:
        output_bi = this->generate_bi(bi);
        this->zhongshu.set_output_bi(output_bi);
        return(this->zhongshu);

    case ZhongShuChuLiStatus::THREE_BUY:
        if (bi.get_high() > this->zhongshu.biList.back().get_high()) {
            this->zhongshu.biList.push_back(bi);
            this->zhongshu.set_type(ZhongShuType::FINISH);
            return(this->zhongshu);
        } else {
            this->zhongshu.biList.push_back(bi);
            this->set_status(ZhongShuChuLiStatus::AFTER_THREE_BUY);
            return(zs);
        }
        break;

    case ZhongShuChuLiStatus::AFTER_THREE_BUY:
        new_bi = this->generate_bi(bi);
        if (new_bi.get_low() < input_bi.get_low()) {
            this->zhongshu.biList.push_back(new_bi);
            this->zhongshu.set_type(ZhongShuType::NEW_LOW);
            return(this->zhongshu);
        }
        else {
            if (new_bi.get_low() < this->zhongshu.get_zhongshu_low()) {
                this->zhongshu.biList.push_back(new_bi);
                this->set_status(ZhongShuChuLiStatus::BOTTOM_UP);
                return(zs);
            }
            else {
                if (new_bi.get_low() >= this->zhongshu.get_zhongshu_high()) {
                    this->zhongshu.biList.push_back(new_bi);
                    this->set_status(ZhongShuChuLiStatus::THREE_BUY);
                    return(zs);
                }
                else {
                    this->zhongshu.biList.push_back(new_bi);
                    this->set_status(ZhongShuChuLiStatus::INPUT);
                    return(zs);
                }
            }
        }
        break;

    case ZhongShuChuLiStatus::THREE_SELL:
        if (bi.get_low() < this->zhongshu.biList.back().get_low()) {
            this->zhongshu.biList.push_back(bi);
            this->zhongshu.set_type(ZhongShuType::FINISH);
            return(this->zhongshu);
        } else {
            this->zhongshu.biList.push_back(bi);
            this->set_status(ZhongShuChuLiStatus::AFTER_THREE_SELL);
            return(zs);
        }
        break;

    case ZhongShuChuLiStatus::AFTER_THREE_SELL:
        new_bi = this->generate_bi(bi);
        this->zhongshu.biList.push_back(new_bi);
        if (new_bi.get_high() > input_bi.get_high()) {
            this->zhongshu.set_type(ZhongShuType::NEW_HIGH);
            return(this->zhongshu);
        }
        else {
            if (new_bi.get_high() > this->zhongshu.get_zhongshu_high()) {
                this->set_status(ZhongShuChuLiStatus::TOP_DOWN);
                return(zs);
            }
            else {
                if (new_bi.get_high() <= this->zhongshu.get_zhongshu_low()) {
                    this->set_status(ZhongShuChuLiStatus::THREE_SELL);
                    return(zs);
                }
                else {
                    this->set_status(ZhongShuChuLiStatus::INPUT);
                    return(zs);
                }
            }
        }
        break;
    }
    return(zs);
}
