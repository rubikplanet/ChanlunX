#include "bi_zoushi.h"

using namespace std;

bool debug_bi_zoushi_status = true;

Bi_ZouShiChuLi::Bi_ZouShiChuLi() {
    this->status = Bi_ZouShiChuLiStatus::a_0;
    this->bicl = BiChuLi();
}

void debug_bi_zoushi(Bi_ZouShi bi_zoushi) {
    if (debug_bi_zoushi_status) {
        switch(bi_zoushi.get_type()) {
            case Bi_ZouShiType::UP:
                break;
            case Bi_ZouShiType::DOWN:
                break;
        }
    }
}

void Bi_ZouShiChuLi::handle(vector<Kxian1>& kxianList) {
    FindBiZouShiReturn ret_fd;
    Bi_ZouShi bi_zoushi = Bi_ZouShi();
    Bi bi = Bi();
    this->bicl.handle(kxianList);

    int count = this->bicl.keyBiList.size();
    for (int i = 0; i < count; i++) {
        bi = this->bicl.keyBiList[i];
        ret_fd = this->find_bi_zoushi(bi);
        switch(ret_fd.type) {
            case FindBiZouShiReturnType::Failue:
                this->bi_zoushi_failure(ret_fd.bi_zoushi_1);
                break;
            case FindBiZouShiReturnType::One:
                this->bi_zoushi_list.push_back(ret_fd.bi_zoushi_1);
                break;
            case FindBiZouShiReturnType::Two:
                this->bi_zoushi_list.push_back(ret_fd.bi_zoushi_1);
                this->bi_zoushi_list.push_back(ret_fd.bi_zoushi_2);
                break;
        }
    }
}

FindBiZouShiReturn Bi_ZouShiChuLi::bi_zoushi_failure(Bi_ZouShi bi_zoushi) {
    FindBiZouShiReturn ret_fd;
    vector<Bi> bi_list;
    Bi start_bi, stop_bi;
    ret_fd.type = FindBiZouShiReturnType::None;
    Bi_ZouShiKind bi_zoushi_kind;

    
    if (!this->bi_zoushi_list.empty()) {
        Bi_ZouShi last_bi_zoushi = this->bi_zoushi_list.back();
        bi_list = bi_zoushi.get_bilist();
        for (int i = 0; i < bi_list.size(); i++) {
            last_bi_zoushi.get_bilist().push_back(bi_list[i]);
        }
        ret_fd.bi_zoushi_1 = Bi_ZouShi(last_bi_zoushi.get_kind(), last_bi_zoushi.get_start_bi(), bi_zoushi.get_stop_bi(), last_bi_zoushi.get_stop_status(), last_bi_zoushi.get_bilist());
        ret_fd.type = FindBiZouShiReturnType::One;
        this->bi_zoushi_list.push_back(ret_fd.bi_zoushi_1);
    }
    else {
        bi_list.clear();
        start_bi = bi_zoushi.get_start_bi();
        stop_bi = bi_zoushi.get_stop_bi();
        bi_list.push_back(start_bi);
        if (start_bi.get_type() == BiType::UP) {
            bi_zoushi_kind = Bi_ZouShiKind::LONGXIANDUAN;
        }
        else {
            if (start_bi.get_type() == BiType::DOWN) {
                bi_zoushi_kind == Bi_ZouShiKind::LONGXIANDUAN;
            }
        }
        ret_fd.bi_zoushi_1 = Bi_ZouShi(bi_zoushi_kind, start_bi, start_bi, Bi_ZouShiChuLiStatus::a_0, bi_list);
        bi_list.clear();
        bi_list.push_back(stop_bi);
        ret_fd.bi_zoushi_2 = Bi_ZouShi(bi_zoushi_kind, stop_bi, stop_bi, Bi_ZouShiChuLiStatus::a_0, bi_list);
        ret_fd.type = FindBiZouShiReturnType::Two;
        this->bi_zoushi_list.push_back(ret_fd.bi_zoushi_1);
        this->bi_zoushi_list.push_back(ret_fd.bi_zoushi_2);
    }
    return(ret_fd);
}

void Bi_ZouShiChuLi::enter_zhongshu(Bi bi, Bi_ZouShiChuLiStatus status){
    Bi_ZhongShu bi_zhongshu;
    if (status == Bi_ZouShiChuLiStatus::A) {
        bi_zhongshu = Bi_ZhongShu(this->a, this->A_a0, this->A_a1, bi);
        this->bi_zhongshu_chuli = Bi_ZhongShuChuLi(this->a, this->A_a0, this->A_a1, bi);
        this->bi_zhongshu_list.push_back(bi_zhongshu);
        this->status = status;
    } else {
        if (status == Bi_ZouShiChuLiStatus::B) {
            bi_zhongshu = Bi_ZhongShu(this->b, this->B_b0, this->B_b1, bi);
            this->bi_zhongshu_chuli = Bi_ZhongShuChuLi(this->b, this->B_b0, this->B_b1, bi);
            this->bi_zhongshu_list.push_back(bi_zhongshu);
            this->status = status;
        }
    }

}

vector<Bi> Bi_ZouShiChuLi::save_bi_list(Bi_ZouShiChuLiStatus status) {
    vector<Bi> bi_list;
    switch(status) {
        case Bi_ZouShiChuLiStatus::a_0:
            bi_list.push_back(this->a_0);
            break;
        case Bi_ZouShiChuLiStatus::a_1:
            bi_list.push_back(this->a_0);
            bi_list.push_back(this->a_1);
            break;
        case Bi_ZouShiChuLiStatus::A_a0:
            bi_list.push_back(this->a_0);
            bi_list.push_back(this->a_1);
            bi_list.push_back(this->a_2);
            break;
        case Bi_ZouShiChuLiStatus::a_2_normal:
            bi_list.push_back(this->a_0);
            bi_list.push_back(this->a_1);
            bi_list.push_back(this->a_2);
            break;
        case Bi_ZouShiChuLiStatus::a_2_longxd:
            bi_list.push_back(this->a_0);
            bi_list.push_back(this->a_1);
            bi_list.push_back(this->a_2);
            bi_list.push_back(this->a_3);
            bi_list.push_back(this->a_4);
            break;
    }
    return(bi_list);
}

FindBiZouShiReturn Bi_ZouShiChuLi::find_bi_zoushi(Bi bi) {
    float bi_high = bi.get_high();
    float bi_low = bi.get_low();
    float a_0_radio, a_1_radio, a_2_radio;

    FindBiZouShiReturn ret_fd;
    ret_fd.type = FindBiZouShiReturnType::None;
    Bi_ZhongShu bi_zhongshu = Bi_ZhongShu();

    a_0_radio = 0;
    a_1_radio = 0;

    switch(this->status) {
        case Bi_ZouShiChuLiStatus::a_0:
            this->a_0 = bi;
            this->status = Bi_ZouShiChuLiStatus::a_1;
            break;

        case Bi_ZouShiChuLiStatus::a_1:
            a_0_radio = get_bi_radio(this->a_0, bi);
            if ((bi.get_type() == BiType::UP && bi_high > this->a_0.get_high()) || (bi.get_type() == BiType::DOWN && bi_low < this->a_0.get_low()) ) {
                ret_fd.type = FindBiZouShiReturnType::Failue;
                vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::a_1);
                ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN, this->a_0, bi, Bi_ZouShiChuLiStatus::a_1, bi_list);
                this->status = Bi_ZouShiChuLiStatus::a_0;
            } else {
                    this->a_1 = bi;
                    this->status = Bi_ZouShiChuLiStatus::a_2;
            }
            break;

        case Bi_ZouShiChuLiStatus::a_2:
            a_0_radio = get_bi_radio(this->a_0, this->a_1);
            a_1_radio = get_bi_radio(this->a_1, bi);
            printf("radio1=%f, radio2=%f", a_0_radio, a_1_radio);
            if (this->a_0.equal(this->a_1)) {
                this->a_0 = bi.generate_bi(this->a_0, this->a_1, bi);
                this->status = Bi_ZouShiChuLiStatus::a_1;
            } else {
                if (abs(a_0_radio) < 0.382 && abs(a_1_radio) > 0.618) {
                    this->a = this->a_0;
                    this->A_a0 = this->a_1;
                    this->A_a1 = bi;
                    this->status = Bi_ZouShiChuLiStatus::A_a2;
                }
                else {
                    this->a_2 = bi;
                    if (bi.get_type() == BiType::UP) {
                        if (bi_high > this->a_0.get_high()) {
                            this->status = Bi_ZouShiChuLiStatus::a_2_highlow;
                        }
                        else {
                            this->status = Bi_ZouShiChuLiStatus::a_2_normal;
                        }
                    }
                    else {
                        if (bi.get_type() == BiType::DOWN) {
                            if (bi_low < this->a.get_low()) {
                                this->a = bi.generate_bi(this->a_0, this->a_1, bi);
                                this->status = Bi_ZouShiChuLiStatus::A_a0;
                            }
                            else {
                                this->status = Bi_ZouShiChuLiStatus::a_2_normal;
                            }
                        }
                    }
                }
            }
            break;

        case Bi_ZouShiChuLiStatus::a_2_highlow:
            a_0_radio = get_bi_radio(this->a_0, this->a_1);
            a_1_radio = get_bi_radio(this->a_1, this->a_2);
            a_2_radio = get_bi_radio(this->a_2, bi);

            if (abs(a_1_radio) > 0.618 && abs(a_2_radio) > 0.618) {
                this->A = Bi_ZhongShu(this->a_0, this->a_1, this->a_2, bi);
                this->enter_zhongshu(bi, Bi_ZouShiChuLiStatus::A);
                this->status = Bi_ZouShiChuLiStatus::A;
            } else {
                if (bi.get_type() == BiType::UP) {
                //向上笔
                    if (bi_low > this->a_0.get_high()) {
                        //长线段
                        this->a_3 = bi;
                        this->status = Bi_ZouShiChuLiStatus::a_2_longxd;
                    } else {
                        if (bi_low < this->a_0.get_low()) {
                            //创新低
                            ret_fd.type = FindBiZouShiReturnType::Failue;
                            bi_list = save_bi_list(Bi_ZouShiChuLiStatus::a_2);
                            ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN, this->a_0, this->a_2, Bi_ZouShiChuLiStatus::a_2, bi_list);
                            this->a_0 = bi;
                            this->status = Bi_ZouShiChuLiStatus::a_1;
                        } 
                    }
                } else {
                    //向下笔
                    if (bi_low < this->a_0.get_low()) {
                        //长线段
                        this->a_3 = bi;
                        this->status = Bi_ZouShiChuLiStatus::a_2_longxd;
                    } else {
                        if (bi_high > this->a_0.get_high()) {
                            //创新高
                            ret_fd.type = FindBiZouShiReturnType::Failue;
                            bi_list = save_bi_list(Bi_ZouShiChuLiStatus::a_2);
                            ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN, this->a_0, this->a_2, Bi_ZouShiChuLiStatus::a_2, bi_list);
                            this->a_0 = bi;
                            this->status = Bi_ZouShiChuLiStatus::a_1;
                        }

                    }

                }
                this->a = bi.generate_bi(this->a_0, this->a_1, this->a_2);
                this->A_a0 = bi;
                this->status = Bi_ZouShiChuLiStatus::A_a1;
            }
            break;

        case Bi_ZouShiChuLiStatus::A_a0:
            if (bi.get_type() == BiType::UP) {
                if (bi_high > this->a_0.get_high()) {
                    //盘整
                    ret_fd.type = FindBiZouShiReturnType::One;
                    vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::A_a0);
                    ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN,this->a, this->a_1, Bi_ZouShiChuLiStatus::A_a0, bi_list);
                    this->a_0 = bi;
                    this->status = Bi_ZouShiChuLiStatus::a_1;
                } else {
                    if (bi_high < this->a_0.get_low()) {
                        //长线段
                        this->a = bi.generate_bi(this->a_0, this->a_1, this->a_2);
                        this->A_a0 = bi;
                        this->status = Bi_ZouShiChuLiStatus::A_a1;
                    } else {
                        this->A_a0 = bi;
                        this->status = Bi_ZouShiChuLiStatus::A_a1;
                        break;
                    }
                }
            } else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi_low < this->a.get_low()) {
                        //盘整
                        ret_fd.type = FindBiZouShiReturnType::One;
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::A_a0);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN,this->a, this->a_1, Bi_ZouShiChuLiStatus::A_a0, bi_list);
                        this->a_0 = bi;
                        this->status = Bi_ZouShiChuLiStatus::a_1;
                    } else {
                        if (bi_low > this->a.get_high()) {
                        //长线段
                            this->a = bi.generate_bi(this->a_0, this->a_1, this->a_2);
                            this->A_a0 = bi;
                            this->status = Bi_ZouShiChuLiStatus::A_a1;
                        } else {
                            this->A_a0 = bi;
                            this->status = Bi_ZouShiChuLiStatus::A_a1;
                        }
                    }
                }
            }
            break;
        case Bi_ZouShiChuLiStatus::A_a1:
            this->A_a1 = bi;
            this->status = Bi_ZouShiChuLiStatus::A_a2;
            break;
        case Bi_ZouShiChuLiStatus::A_a2:
            a_0_radio = get_bi_radio(this->a, this->A_a0);
            a_1_radio = get_bi_radio(this->A_a0, this->A_a1);
            a_2_radio = get_bi_radio(this->A_a1, bi);
            if (abs(a_0_radio) < 0.382 && abs(a_1_radio) > 0.618 && abs(a_2_radio) > 0.618) {
                this->A = Bi_ZhongShu(this->a, this->A_a0, this->A_a1, bi);
                this->enter_zhongshu(bi, Bi_ZouShiChuLiStatus::A);
            }
            break;

        case Bi_ZouShiChuLiStatus::a_2_normal:
            a_0_radio = get_bi_radio(this->a_0, this->a_1);
            a_1_radio = get_bi_radio(this->a_1, this->a_2);
            a_2_radio = get_bi_radio(this->a_2, bi);

            if (abs(a_1_radio) > 0.618 && abs(a_2_radio) > 0.618) {
                this->A = Bi_ZhongShu(this->a_0, this->a_1, this->a_2, bi);
                this->enter_zhongshu(bi, Bi_ZouShiChuLiStatus::A);
                this->status = Bi_ZouShiChuLiStatus::A;
            } else {
                if (bi.get_type() == BiType::UP) {
                    if (bi_high > this->a_0.get_high()) {
                        //盘整
                        ret_fd.type = FindBiZouShiReturnType::One;
                        vector<Bi>& bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::a_0);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN, this->a_0, this->a_0, Bi_ZouShiChuLiStatus::a_0, bi_list);
                        this->a_0 = this->a_1;
                        this->a_1 = this->a_2;
                        this->a_2 = bi;
                        this->status = Bi_ZouShiChuLiStatus::A_a0;
                    }
                    else {
                        if (bi_high > this->a_1.get_high()) {
                            //重新生成a_1
                            this->a_1 = bi.generate_bi(this->a_1, a_2, bi);
                            this->status = Bi_ZouShiChuLiStatus::a_2;
                        }
                        else {
                            this->a_3 = bi;
                            this->status = Bi_ZouShiChuLiStatus::a_2_normal_normal;
                        }
                    }
                }
                else {
                    if (bi.get_type() == BiType::DOWN) {
                        if (bi_low < this->a_0.get_low()) {
                            //盘整
                            ret_fd.type = FindBiZouShiReturnType::One;
                            vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::a_0);
                            ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN, this->a_0, this->a_0, Bi_ZouShiChuLiStatus::a_0, bi_list);
                            this->a_0 = this->a_1;
                            this->a_1 = this->a_2;
                            this->a_2 = bi;
                            this->status = Bi_ZouShiChuLiStatus::A_a0;
                        }
                        else {
                            if (bi_low < this->a_1.get_low()) {
                                //重新生成a_1
                                this->a_1 = bi.generate_bi(this->a_1, this->a_2, bi);
                                this->status = Bi_ZouShiChuLiStatus::a_2;
                            }
                            else {
                                this->a_3 = bi;
                                this->status = Bi_ZouShiChuLiStatus::a_2_normal_normal;
                            }
                        }
                    }
                }
            }
            break;

        case Bi_ZouShiChuLiStatus::a_2_normal_normal:
            if (bi.get_type() == BiType::UP) {
                if (bi_high > this->a_0.get_high()) {
                    this->a_2 = bi.generate_bi(this->a_2, this->a_3, bi);
                    this->status = Bi_ZouShiChuLiStatus::a_2_highlow;
                } else {
                    if (bi_high > this->a_2.get_high()) {
                        this->a_2 = bi.generate_bi(this->a_2, this->a_3, bi);
                        this->status = Bi_ZouShiChuLiStatus::a_2_normal;
                    } else {
                        printf("a_2_normal_normal 未处理");
                    }
                }
            } else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi_low < this->a_0.get_low()) {
                        this->a_2 = bi.generate_bi(this->a_2, this->a_3, bi);
                        this->status = Bi_ZouShiChuLiStatus::a_2_highlow;
                    } else {
                        if (bi_low < this->a_2.get_low()) {
                            this->a_2 = bi.generate_bi(this->a_2, this->a_3, bi);
                            this->status = Bi_ZouShiChuLiStatus::a_2_normal;
                        } else {
                            printf("a_2_normal_normal 未处理");
                        }
                    }
                }
            }
            break;

        case Bi_ZouShiChuLiStatus::a_2_longxd:
            if (bi.get_type() == BiType::UP) {
                if (bi_high > this->a_2.get_high()) {
                    ret_fd.type = FindBiZouShiReturnType::One;
                    this->a_3 = bi;
                    vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::a_2_longxd);
                    ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN, this->a_0, bi, Bi_ZouShiChuLiStatus::a_2_longxd, bi_list);
                    this->status = Bi_ZouShiChuLiStatus::a_0;
                } else {
                    this->a_4 = bi;
                    this->status = Bi_ZouShiChuLiStatus::a_2_longxd_normal;
                }
            } else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi_low < this->a_2.get_low()) {
                        ret_fd.type = FindBiZouShiReturnType::One;
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::a_2_longxd);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::LONGXIANDUAN, this->a_0, bi, Bi_ZouShiChuLiStatus::a_2_longxd, bi_list);
                        this->status = Bi_ZouShiChuLiStatus::a_0;
                    } else {
                        this->a_4 = bi;
                        this->status = Bi_ZouShiChuLiStatus::a_2_longxd_normal;
                    }
                }
            }
            break;

        /*
        case Bi_ZouShiChuLiStatus::a_2_longxd_normal:
            if (bi.get_type() == BiType::UP) {
                if (bi_high > this->a_0.get_high()) {
                    ret_fd.type = FindBiZouShiReturnType::Two;
                    ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a_0, this->a_2);
                    ret_fd.bi_zoushi_2 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a_3, bi);
                    this->status = Bi_ZouShiChuLiStatus::a_0;
                } else {
                    if (bi_high < this->a_0.get_low()) {
                        this->a_3 = bi.generate_bi(this->a_3, this->a_4, bi);
                        this->status = Bi_ZouShiChuLiStatus::a_2_longxd;
                    } else {
                        this->a_0 = bi.generate_bi(this->a_0, this->a_1, this->a_2);
                        this->a_1 = bi.generate_bi(this->a_3, this->a_4, bi);
                        this->status = Bi_ZouShiChuLiStatus::a_2;
                    }
                }
            } else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi_low < this->a.get_low()) {
                        ret_fd.type = FindBiZouShiReturnType::Two;
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a_0, this->a_2);
                        ret_fd.bi_zoushi_2 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a_3, bi);
                        this->status = Bi_ZouShiChuLiStatus::a_0;
                    } else {
                        if (bi_low > this->b_0.get_high()) {
                                this->a_3 = bi.generate_bi(this->a_3, this->a_4, bi);
                                this->status = Bi_ZouShiChuLiStatus::a_2_longxd;
                        } else {
                            this->a_0 = bi.generate_bi(this->a_0, this->a_1, this->a_2);
                            this->a_1 = bi.generate_bi(this->a_3, this->a_4, bi);
                            this->status = Bi_ZouShiChuLiStatus::a_2;
                        }
                    }
                }
            }
            break;
        */
        case Bi_ZouShiChuLiStatus::A:
            bi_zhongshu = this->bi_zhongshu_chuli.find_Bi_ZhongShu(bi);
            if (bi_zhongshu.get_output().get_type() != BiType::NONE) {
                if (bi_zhongshu_chuli.get_status() == Bi_ZhongShuChuLiStatus::THREE_SELL) {
                    //出现3卖
                    this->A = bi_zhongshu_chuli.get_Bi_ZhongShu();
                    if (this->a.get_type() == BiType::DOWN) {
                        this->b_0 = this->A.get_output();
                        this->b_1 = bi;
                        this->status = Bi_ZouShiChuLiStatus::b_2;
                        this->bi_zhongshu_list.push_back(this->A);
                    }
                    else {
                        //出现反向3卖
                        ret_fd.type = FindBiZouShiReturnType::One;
                        Bi zs_input = this->A.get_input();
                        Bi zs_max_bi = this->A.get_max_bi();
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::A);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, zs_input, zs_max_bi, Bi_ZouShiChuLiStatus::A, bi_list);
                        this->a_0 = this->A.find_next_bi(zs_max_bi);
                        this->a_1 = bi;
                        this->status = Bi_ZouShiChuLiStatus::a_2;
                    }
                }
                else {
                    if (bi_zhongshu_chuli.get_status() == Bi_ZhongShuChuLiStatus::THREE_BUY) {
                        //出现3买
                        this->A = bi_zhongshu_chuli.get_Bi_ZhongShu();
                        if (this->a.get_type() == BiType::UP) {
                            this->b_0 = this->A.get_output();
                            this->b_1 = bi;
                            this->status = Bi_ZouShiChuLiStatus::b_2;
                            this->bi_zhongshu_list.push_back(this->A);
                        }
                        else {
                            //ToDo: 出现反向3买
                            ret_fd.type = FindBiZouShiReturnType::One;
                            Bi zs_input = this->A.get_input();
                            Bi zs_min_bi = this->A.get_min_bi();
                            vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::A);
                            ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, zs_input, zs_min_bi, Bi_ZouShiChuLiStatus::A, bi_list);
                            this->a_0 = this->A.find_next_bi(zs_min_bi);
                            this->a_1 = bi;
                            this->status = Bi_ZouShiChuLiStatus::a_2;
                        }
                    }
                }
            } else {
                    //继续中枢处理
            }
            break;
        
        case Bi_ZouShiChuLiStatus::b_2:
            if ((bi.get_type() == BiType::UP && bi_high > this->b_0.get_high()) || (bi.get_type()==BiType::DOWN && bi_low < this->b_0.get_low())) {
                this->b = bi.generate_bi(this->b_0, this->b_1, bi);
                this->status = Bi_ZouShiChuLiStatus::B_b0;
            } else {
                this->b_2 = bi;
                this->status = Bi_ZouShiChuLiStatus::b_2_normal;
            }
            break;


        case Bi_ZouShiChuLiStatus::B_b0:
            if (bi.get_type() == BiType::UP) {
                if (bi_high > this->a_0.get_high()) {
                    //创新高
                    ret_fd.type = FindBiZouShiReturnType::One;
                    vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::B_b0);
                    ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a_0, this->b, Bi_ZouShiChuLiStatus::B_b0, bi_list);
                    this->a_0 = bi;
                    this->status = Bi_ZouShiChuLiStatus::a_1;
                } else {
                    if (bi_high > this->A.get_low()) {
                        //进入上一个中枢， 进行扩展模式
                        ret_fd.type = FindBiZouShiReturnType::One;
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::B_b0);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a_0, this->b, Bi_ZouShiChuLiStatus::B_b0, bi_list);
                        this->a_0 = bi;
                        this->status = Bi_ZouShiChuLiStatus::a_1;

                    } else {
                        //大线段
                        if (bi_high < this->b_0.get_low()) {
                            this->B_b0 = bi;
                            this->status = Bi_ZouShiChuLiStatus::b_1;
                        } else {
                            this->B_b0 = bi;
                            this->status = Bi_ZouShiChuLiStatus::B_b1;
                        }
                    }
                }
            } else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi_low < this->a_0.get_low()) {
                        //创新低
                        ret_fd.type = FindBiZouShiReturnType::One;
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::B_b0);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a_0, this->b, Bi_ZouShiChuLiStatus::B_b0, bi_list);
                        this->a_0 = bi;
                        this->status = Bi_ZouShiChuLiStatus::a_1;
                    } else {
                        if (bi_low < this->A.get_high()) {
                            //进入上一个中枢
                            ret_fd.type = FindBiZouShiReturnType::One;
                            vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::B_b0);
                            ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a_0, this->b, Bi_ZouShiChuLiStatus::B_b0, bi_list);
                            this->a_0 = bi;
                            this->status = Bi_ZouShiChuLiStatus::a_1;
                        } else {
                            //大线段
                            if (bi_high < this->b_0.get_low()) {
                                this->B_b0 = bi;
                                this->status = Bi_ZouShiChuLiStatus::B_b1;
                            }
                            else {
                                this->B_b0 = bi;
                                this->status = Bi_ZouShiChuLiStatus::B_b1;
                            }
                        }
                    }

                }
            }
            break;
/*
        case Bi_ZouShiChuLiStatus::b_2_longxd:
            if (bi.get_type() == BiType::UP) {
                if (bi_high > this->b.get_high()) {
                    ret_fd.type = FindBiZouShiReturnType::One;
                    ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a, bi);
                    this->status = Bi_ZouShiChuLiStatus::a_0;
                } else {
                    this->b_4 = bi;
                    this->status = Bi_ZouShiChuLiStatus::b_2_longxd_normal;
                }
            } else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi_low < this->b.get_low()) {
                        ret_fd.type = FindBiZouShiReturnType::One;
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a, bi);
                        this->status = Bi_ZouShiChuLiStatus::a_0;
                    } else {
                        this->b_4 = bi;
                        this->status = Bi_ZouShiChuLiStatus::b_2_longxd_normal;
                    }
                }
            }
            break;
        case Bi_ZouShiChuLiStatus::b_2_longxd_normal:
            if (bi.get_type() == BiType::UP) {
            } else {
                if (bi.get_type() == BiType::DOWN) {

                }
            }
            break;
*/
        case Bi_ZouShiChuLiStatus::B_b1:
            this->B_b1 = bi;
            this->status = Bi_ZouShiChuLiStatus::B_b2;
            break;
        case Bi_ZouShiChuLiStatus::B_b2:
            this->B = Bi_ZhongShu(this->b, this->B_b0, this->B_b1, bi);
            this->enter_zhongshu(bi, Bi_ZouShiChuLiStatus::B);
            break;
        case Bi_ZouShiChuLiStatus::B:
            bi_zhongshu_chuli.find_Bi_ZhongShu(bi);
            if (bi_zhongshu_chuli.get_status() == Bi_ZhongShuChuLiStatus::THREE_SELL) {
                //出现3卖
                if (this->b.get_type() == BiType::DOWN) {
                    this->c_0 = this->B.get_output();
                    this->c_1 = bi;
                    this->status = Bi_ZouShiChuLiStatus::c_2;
                    this->bi_zhongshu_list.push_back(this->B);
                } else {
                    //出现反向3卖
                    ret_fd.type = FindBiZouShiReturnType::One;
                    Bi zs_input = this->B.bi_list[0];
                    Bi zs_max_bi = this->B.get_max_bi();
                    vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::B);
                    ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a, zs_max_bi, Bi_ZouShiChuLiStatus::B, bi_list);
                    this->a_0 = this->B.find_next_bi(zs_max_bi);
                    this->a_1 = bi;
                    this->status = Bi_ZouShiChuLiStatus::a_2;

                }
            } else {
                if (bi_zhongshu_chuli.get_status() == Bi_ZhongShuChuLiStatus::THREE_BUY) {
                    //出现3买
                    if (this->a.get_type() == BiType::UP) {
                        this->B = bi_zhongshu_chuli.get_Bi_ZhongShu();
                        this->c_0 = this->B.get_output();
                        this->c_1 = bi;
                        this->status = Bi_ZouShiChuLiStatus::c_2;
                        this->bi_zhongshu_list.push_back(this->B);
                    } else {
                        //ToDo: 出现反向3买
                        ret_fd.type = FindBiZouShiReturnType::One;
                        Bi zs_input = this->B.bi_list[0];
                        Bi zs_min_bi = this->B.get_min_bi();
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::B);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::PANZHENG, this->a, zs_min_bi, Bi_ZouShiChuLiStatus::B, bi_list);

                        this->a_0 = this->B.find_next_bi(zs_min_bi);
                        this->a_1 = bi;
                        this->status = Bi_ZouShiChuLiStatus::a_2;
                    }
                } else {
                    //继续中枢处理
                }
            }
            break;

        case Bi_ZouShiChuLiStatus::c_2:
            if (bi.get_type() == BiType::UP) {
                if (bi_high > this->c_0.get_high()) {
                    ret_fd.type == FindBiZouShiReturnType::One;
                    vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::c_2);
                    ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::UP, this->a, bi, Bi_ZouShiChuLiStatus::c_2, bi_list);
                    this->status = Bi_ZouShiChuLiStatus::a_0;
                } else {
                    this->c_2 = bi;
                    this->status = Bi_ZouShiChuLiStatus::c_2_normal;
                }
            } else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi_low < this->c_0.get_low()) {
                        ret_fd.type = FindBiZouShiReturnType::One;
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::c_2);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::UP, this->a, bi, Bi_ZouShiChuLiStatus::c_2, bi_list);
                        this->status = Bi_ZouShiChuLiStatus::a_0;
                    } else {
                        this->c_2 = bi;
                        this->status = Bi_ZouShiChuLiStatus::c_2_normal;
                    }
                }
            }
            break;

        case Bi_ZouShiChuLiStatus::c_2_normal:
            if (bi.get_type() == BiType::UP) {
                if (bi_high > this->a.get_high()) {
                    ret_fd.type = FindBiZouShiReturnType::One;
                    vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::c_0);
                    ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::DOWN, this->a, this->c_0, Bi_ZouShiChuLiStatus::c_0, bi_list);
                    this->a_0 = this->c_1;
                    this->a_1 = this->c_2;
                    this->a_3 = bi;
                    this->status = Bi_ZouShiChuLiStatus::A_a0;
                } else {
                    if (bi_high < this->B.get_low()) {
                        ret_fd.type = FindBiZouShiReturnType::One;
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::c_0);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::DOWN, this->a, this->c_0, Bi_ZouShiChuLiStatus::c_0, bi_list);
                        this->a = bi.generate_bi(this->c_1, this->c_2, bi);
                        this->status = Bi_ZouShiChuLiStatus::a_1;
                    } else {
                        this->c_1 = bi.generate_bi(this->c_1, this->c_2, bi);
                        this->status = Bi_ZouShiChuLiStatus::c_2;
                    }
                }
            } else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi_low < this->a.get_low()) {
                        ret_fd.type = FindBiZouShiReturnType::One;
                        vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::c_0);
                        ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::DOWN, this->a, this->c_0, Bi_ZouShiChuLiStatus::c_0, bi_list);
                        this->a_0 = this->c_1;
                        this->a_1 = this->c_2;
                        this->a_3 = bi;
                        this->status = Bi_ZouShiChuLiStatus::A_a0;
                    } else {
                        if (bi_low > this->B.get_high()) {
                            ret_fd.type = FindBiZouShiReturnType::One;
                            vector<Bi> bi_list = this->save_bi_list(Bi_ZouShiChuLiStatus::c_0);
                            ret_fd.bi_zoushi_1 = Bi_ZouShi(Bi_ZouShiKind::DOWN, this->a, this->c_0, Bi_ZouShiChuLiStatus::c_0, bi_list);
                            this->a = bi.generate_bi(this->c_1, this->c_2, bi);
                            this->status = Bi_ZouShiChuLiStatus::a_1;
                        } else {
                            this->c_1 = bi.generate_bi(this->c_1, this->c_2, bi);
                            this->status = Bi_ZouShiChuLiStatus::c_2;
                        }
                    }
                }
            }
            break;
        
    }
    return(ret_fd);
}


void bi_zhongshu_start_stop(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgno){
    BaoHanChuLi baohanChuli;
    Bi_ZouShiChuLi bi_zoushichuli;
    Bi_ZhongShu bi_zhongshu;
    int start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }
    bi_zoushichuli.handle(baohanChuli.kxianList);

    int count = bi_zoushichuli.bi_zhongshu_list.size();
    if (count == 0)
        return;

    for (int i = 0; i < count; i++) {
        bi_zhongshu = bi_zoushichuli.bi_zhongshu_list[i];
        start_pos = bi_zhongshu.get_start_pos();
        stop_pos = bi_zhongshu.get_stop_pos();
        pOut[start_pos + 1] = 1;
        if (stop_pos > 0)
            pOut[stop_pos - 1] = 2;

    }
}

void bi_zhongshu_high(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgno){
    BaoHanChuLi baohanChuli;
    Bi_ZouShiChuLi bi_zoushichuli;
    Bi_ZhongShu bi_zhongshu;
    int start_pos, stop_pos;
    float zs_high;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }
    bi_zoushichuli.handle(baohanChuli.kxianList);

    unsigned int count = bi_zoushichuli.bi_zhongshu_list.size();
    if (count == 0)
        return;

    for (int i = 0; i < count; i++) {
        bi_zhongshu = bi_zoushichuli.bi_zhongshu_list[i];
        start_pos = bi_zhongshu.get_start_pos();
        stop_pos = bi_zhongshu.get_stop_pos();
        zs_high = bi_zhongshu.get_high();
        for (int num = start_pos + 1; num < stop_pos - 1; num++)
            pOut[num] = zs_high;
    }
}

void bi_zhongshu_low(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgno){
    BaoHanChuLi baohanChuli;
    Bi_ZouShiChuLi bi_zoushichuli;
    Bi_ZhongShu bi_zhongshu;
    int start_pos, stop_pos;
    float zs_low;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }
    bi_zoushichuli.handle(baohanChuli.kxianList);

    unsigned int count = bi_zoushichuli.bi_zhongshu_list.size();
    if (count == 0)
        return;

    for (int i = 0; i < count; i++) {
        bi_zhongshu = bi_zoushichuli.bi_zhongshu_list[i];
        start_pos = bi_zhongshu.get_start_pos();
        stop_pos = bi_zhongshu.get_stop_pos();
        zs_low = bi_zhongshu.get_low();
        for (int num = start_pos + 1; num < stop_pos - 1; num++)
            pOut[num] = zs_low;
    }
}

void Bi3_bi_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgno) {
    BaoHanChuLi baohanChuli;
    Bi_ZouShiChuLi bi_zoushichuli;
    Bi_ZouShi bi_zoushi;
    int start_pos, stop_pos, verify_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    bi_zoushichuli.handle(baohanChuli.kxianList);

    unsigned int count = bi_zoushichuli.bi_zoushi_list.size() - 1;
    if (count == 0)
        return;

    if (!bi_zoushichuli.bi_zoushi_list.empty()) {
        Bi_ZouShi start_zoushi = bi_zoushichuli.bi_zoushi_list[0];
        start_pos = start_zoushi.get_start_pos();
        stop_pos = start_zoushi.get_stop_pos();
        if (start_zoushi.get_start_bi().get_type() == BiType::UP) {
            pOut[start_pos] = -3;
            pOut[stop_pos] = 3;
        } else {
            pOut[start_pos] = 3;
            pOut[start_pos] = -3;
        }

        for (unsigned int i = count; i > 0; i--) {
            bi_zoushi = bi_zoushichuli.bi_zoushi_list[i - 1];
            stop_pos = bi_zoushi.get_stop_pos();
            if (bi_zoushi.get_start_bi().get_type() == BiType::UP)
                pOut[stop_pos] = 3;
            else
                pOut[stop_pos] = -3;
        }
        
    }

}

void Bi4_bi_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIgno) {

}
