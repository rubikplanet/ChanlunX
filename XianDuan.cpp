#include "XianDuan.h"

using namespace std;

XianDuan XianDuanChuLi::last_xd = XianDuan();
XianDuanChuLiStatus  XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
Bi XianDuanChuLi::last_bi = Bi();

XianDuanChuLi::XianDuanChuLi() {
    this->last_xd = XianDuan();
    this->last_bi = Bi();
    this->bicl = BiChuLi();
    this->status = XianDuanChuLiStatus::START;

}

void XianDuanChuLi::handle(vector<Kxian1>& kxianList) {
    FindXianDuanReturn ret_fd;
    XianDuan xd = XianDuan();
    Bi bi = Bi();
    this->bicl.handle(kxianList);
 
    int count = this->bicl.keyBiList.size();
    for (int i = 0; i < count; i++) {
        bi = this->bicl.keyBiList[i];
        ret_fd = this->find_xianduan(bi);
        if (ret_fd.type != FindXianDuanReturnType::None) {
            switch (ret_fd.type) {
            case FindXianDuanReturnType::One:
                debug_xianduan(ret_fd.xd1);
                this->xianDuanList.push_back(ret_fd.xd1);
                if (ret_fd.xd1.get_type() == XianDuanType::UP || ret_fd.xd1.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                break;
            case FindXianDuanReturnType::Two:
                debug_xianduan(ret_fd.xd1);
                this->xianDuanList.push_back(ret_fd.xd1);
                if (ret_fd.xd1.get_type() == XianDuanType::UP || ret_fd.xd1.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                debug_xianduan(ret_fd.xd2);
                this->xianDuanList.push_back(ret_fd.xd2);
                if (ret_fd.xd2.get_type() == XianDuanType::UP || ret_fd.xd2.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd2);
                break;
            case FindXianDuanReturnType::Three:
                debug_xianduan(ret_fd.xd1);
                this->xianDuanList.push_back(ret_fd.xd1);
                if (ret_fd.xd1.get_type() == XianDuanType::UP || ret_fd.xd1.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                debug_xianduan(ret_fd.xd2);
                this->xianDuanList.push_back(ret_fd.xd2);
                if (ret_fd.xd2.get_type() == XianDuanType::UP || ret_fd.xd2.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd2);
                debug_xianduan(ret_fd.xd3);
                this->xianDuanList.push_back(ret_fd.xd3);
                if (ret_fd.xd3.get_type() == XianDuanType::UP || ret_fd.xd3.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd3);
                break;
            case FindXianDuanReturnType::Failure:
                if (ret_fd.xd1.get_type() == XianDuanType::UP) 
                    OutputDebugPrintf("FAILURE XD: UP %f %f", ret_fd.xd1.get_low(), ret_fd.xd1.get_high());
                else {
                    if (ret_fd.xd1.get_type() == XianDuanType::DOWN)
                        OutputDebugPrintf("FAILURE XD: DOWN %f %f", ret_fd.xd1.get_high(), ret_fd.xd1.get_low());
                }
                if (!this->xianDuanList.empty()) {
                    Bi start_bi = this->xianDuanList.back().get_start_bi();
                    Bi stop_bi = ret_fd.xd1.get_stop_bi();
                    XianDuan xd = XianDuan(start_bi, stop_bi);
                    if (!this->key_xianduan_list.empty()) {
                        this->key_xianduan_list.pop_back();
                        this->key_xianduan_list.push_back(xd);
                        this->xianDuanList.push_back(xd);
                    }
                }
                else {
                    this->xianDuanList.push_back(ret_fd.xd1);
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                }               
                break;
            case FindXianDuanReturnType::None:
                break;
            }
        }
    }

    int ncount = this->xianDuanList.size();
    int start_num = 0;
    if (!this->key_xianduan_list.empty()) {
        XianDuan comp_xd = this->key_xianduan_list.back();
        for (int i = 0; i < ncount; i++) {
            xd = this->xianDuanList[i];
            if (xd == comp_xd) {
                start_num = i + 1;
                break;
            }
        }

        for (int i = start_num; i < ncount; i++) {
            xd = this->xianDuanList[i];
            this->key_xianduan_list.push_back(xd);
        }
    }

}

FindXianDuanReturn XianDuanChuLi::failure_xd(Bi start_bi, Bi stop_bi) {
    FindXianDuanReturn ret_fd = FindXianDuanReturn();
    XianDuan tmp_xd = XianDuan();
    Bi xd_start_bi = Bi();

    if (this->xianDuanList.empty()) {
        ret_fd.xd1 = XianDuan(this->start, this->start);
        ret_fd.xd1.save_bi_list(this->get_xd_bi_list());
        ret_fd.type = FindXianDuanReturnType::One;
    }
    else {
        if (!this->xianDuanList.empty()) {
            XianDuan last_xd = this->xianDuanList.back();
            if (last_xd.get_type() == XianDuanType::UP) {
                //上升笔处理
                if (start_bi.get_low() < last_xd.get_low()) {
                    ret_fd.xd1 = XianDuan(this->start, this->start);
                    ret_fd.type = FindXianDuanReturnType::One;
                }
                else {
                    ret_fd.xd1 = XianDuan(last_xd.get_start_bi(), stop_bi);
                    ret_fd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_fd.type = FindXianDuanReturnType::Failure;
                    this->status = XianDuanChuLiStatus::START;
                    return(ret_fd);
                }
            }
            else {
                //下降笔处理
                if (start_bi.get_high() > last_xd.get_high()) {
                    ret_fd.xd1 = XianDuan(this->start, this->start);
                    ret_fd.type = FindXianDuanReturnType::One;
                }
                else {
                    ret_fd.xd1 = XianDuan(last_xd.get_start_bi(), stop_bi);
                    ret_fd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_fd.type = FindXianDuanReturnType::Failure;
                    this->status = XianDuanChuLiStatus::START;
                    return(ret_fd);
                }
            }
        }
    }
    this->start = stop_bi;
    this->status = XianDuanChuLiStatus::LEFT;
    return(ret_fd);
}



vector<Bi>  XianDuanChuLi::get_xd_bi_list() {
    vector<Bi> bi_list;
    switch (this->status) {
    case XianDuanChuLiStatus::START:
        break;
    case XianDuanChuLiStatus::LEFT:
        bi_list.push_back(this->start);
        break;
    case XianDuanChuLiStatus::AFTER_LEFT:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        break;
    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        break;
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->after_middle);
        break;
    case XianDuanChuLiStatus::AFTER_MIDDLE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        break;
    case XianDuanChuLiStatus::RIGHT_HIGHLOW:
    case XianDuanChuLiStatus::RIGHT_NORMAL:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        break;
    case XianDuanChuLiStatus::MIDDLE_INCLUDE_RIGHT:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        break;
    case XianDuanChuLiStatus::AFTER_RIGHT:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        break;
    case XianDuanChuLiStatus::FREE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        bi_list.push_back(this->after_right);
        break;
    case XianDuanChuLiStatus::AFTER_FREE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        bi_list.push_back(this->after_right);
        bi_list.push_back(this->free);
        break;
    }
    return(bi_list);
}

FindXianDuanReturn  XianDuanChuLi::find_xianduan(Bi bi) {
    FindXianDuanReturn ret_fd;
    BiType bi_type;
    XianDuan xd = XianDuan();
    if (this->last_bi.get_type() == BiType::NONE) {
        if (bi.get_type() == BiType::UP || bi.get_type() == BiType::DOWN) {
            this->last_bi = bi;
            this->start= bi;
            XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
        }
    }
    else {
        bi_type = bi.get_type();
        switch (bi_type) {
        case BiType::UP:
        case BiType::DOWN:
            return(this->__find_xianduan(bi));
        case BiType::TEMP_DOWN:
        case BiType::TEMP_UP:
            break;
        default:
            this->__backroll(bi);
        }
    }
    ret_fd.type = FindXianDuanReturnType::None;
    ret_fd.xd1.save_bi_list(this->get_xd_bi_list());
    return(ret_fd);
}

void XianDuanChuLi::__backroll(Bi bi) {
    switch (this->status) {
    case XianDuanChuLiStatus::START:
        break;
    case XianDuanChuLiStatus::LEFT:
        this->status = XianDuanChuLiStatus::START;
        break;
    case XianDuanChuLiStatus::AFTER_LEFT:
        this->status = XianDuanChuLiStatus::LEFT;
        break;
    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
        this->status = XianDuanChuLiStatus::AFTER_LEFT;
        break;
    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        this->status = XianDuanChuLiStatus::AFTER_LEFT;
    case XianDuanChuLiStatus::AFTER_MIDDLE:
        //this->status = XianDuanChuLiStatus::MIDDLE;
        break;
    case XianDuanChuLiStatus::RIGHT_HIGHLOW:
        this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
        break;
    case XianDuanChuLiStatus::RIGHT_NORMAL:
        this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
    case XianDuanChuLiStatus::AFTER_RIGHT:
       // this->status = XianDuanChuLiStatus::RIGHT;
        break;
    case XianDuanChuLiStatus::FREE:
        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
        break;
    case XianDuanChuLiStatus::AFTER_FREE:
        this->status = XianDuanChuLiStatus::FREE;
        break;
    }
}

void XianDuanChuLi::debug_xianduan(XianDuan xd) {
    XianDuanType xd_type = xd.get_type();
    switch (xd_type) {
    case XianDuanType::UP:
        OutputDebugPrintf("UP %f %f", xd.get_low(), xd.get_high());
        break;
    case XianDuanType::FAILURE_UP:
        OutputDebugPrintf("FAILURE UP %f %f", xd.get_low(), xd.get_high());
        break;
    case XianDuanType::DOWN:
        OutputDebugPrintf("DOWN %f %f", xd.get_high(), xd.get_low());
        break;
    case XianDuanType::FAILURE_DOWN:
        OutputDebugPrintf("FAILURE DOWN %f %f", xd.get_high(), xd.get_low());
        break;
    }
}

char* get_xianduan_status(XianDuanChuLiStatus status) {
    switch (status) {
    case XianDuanChuLiStatus::START:
        return("START");
    case XianDuanChuLiStatus::LEFT:
        return("LEFT");
    case XianDuanChuLiStatus::AFTER_LEFT:
        return("AFTER_LEFT");
    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
        return("MIDDLE_HIGHLOW");
    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        return("MIDDLE_NORMAL");
    case XianDuanChuLiStatus::AFTER_MIDDLE:
        return("AFTER_MIDDLE");
    case XianDuanChuLiStatus::RIGHT_HIGHLOW:
        return("RIGHT_HIGHLOW");
    case XianDuanChuLiStatus::RIGHT_NORMAL:
        return("RIGHT_NORMAL");
    case XianDuanChuLiStatus::AFTER_RIGHT:
        return("AFTER_RIGHT");
    case XianDuanChuLiStatus::FREE:
        return("FREE");
    case XianDuanChuLiStatus::AFTER_FREE:
        return("AFTER_RIGHT");
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE:
        return("LEFT_INCLUDE_MIDDLE");
    case XianDuanChuLiStatus::MIDDLE_INCLUDE_RIGHT:
        return("MIDDLE_INCLUDE_RIGHT");
    default:
        return("Not Process Item");
    }
}


FindXianDuanReturn XianDuanChuLi::__find_xianduan(Bi bi) {
    XianDuan xd = XianDuan();
    FindXianDuanReturn ret_xd;

    if (bi.get_type() == BiType::UP)
        OutputDebugPrintf("%s 【UP %f  %f】", get_xianduan_status(this->status), bi.get_low(), bi.get_high());
    else
        OutputDebugPrintf("%s 【DOWN %f  %f】", get_xianduan_status(this->status), bi.get_high(), bi.get_low());

    switch (this->status) {
    case XianDuanChuLiStatus::START:
        this->start = bi;
        this->status = XianDuanChuLiStatus::LEFT;
        break;

    case XianDuanChuLiStatus::LEFT:
        if (this->start.get_type() == BiType::UP && bi.get_low() < this->start.get_low() || (this->start.get_type() == BiType::DOWN && bi.get_high() > this->start.get_high())) {
            return(this->failure_xd(this->start, bi));
        }
        this->left = bi;
        this->status = XianDuanChuLiStatus::AFTER_LEFT;
        break;

    case XianDuanChuLiStatus::AFTER_LEFT:
        this->after_left = bi;
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                ret_xd.xd1 = XianDuan(this->start, bi);
                ret_xd.xd1.set_type(XianDuanType::TEMP_UP);
                ret_xd.type = FindXianDuanReturnType::One;
                return(ret_xd);
            }
            else {
                this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                ret_xd.xd1 = XianDuan(this->start, bi);
                ret_xd.xd1.set_type(XianDuanType::TEMP_DOWN);
                ret_xd.type = FindXianDuanReturnType::One;
                return(ret_xd);
            }
            else {
                this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
            }
        }
        break;

    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->start.get_high()) {
                //创新高
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(bi, bi);
                ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = bi;
                this->status = XianDuanChuLiStatus::LEFT;
                return(ret_xd);
            }
            if (bi.get_high() >= this->left.get_high() && bi.get_low() <= this->left.get_low()) {
                //middle 包含 left
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi;
                this->status = XianDuanChuLiStatus::AFTER_LEFT;
            }
            else {
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                //创新低
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(bi, bi);
                ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = bi;
                this->status = XianDuanChuLiStatus::LEFT;
                return(ret_xd);
            }
            if (bi.get_high() >= this->left.get_high() && bi.get_low() <= this->left.get_low()) {
                //middle 包含 left
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi;
                this->status = XianDuanChuLiStatus::AFTER_LEFT;
            }
            else {
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
        }
        break;

    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                //创新高
                ret_xd.xd1 = XianDuan(this->start, this->start);
                ret_xd.xd2 = XianDuan(this->left, bi);
                ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = this->left;
                this->left = this->after_left;
                this->after_left = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                return(ret_xd);
            }
            if (bi.get_high() <= this->left.get_high()) {
                //left 包含middle
                this->middle = bi;
                this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE;
            }
            else {
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                this->status = XianDuanChuLiStatus::AFTER_LEFT;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                //创新低
                ret_xd.xd1 = XianDuan(this->start, this->start);
                ret_xd.xd2 = XianDuan(this->left, bi);
                ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = this->left;
                this->left = this->after_left;
                this->after_left = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                return(ret_xd);
            }
            if (bi.get_low() >= this->left.get_low()) {
                //left 包含middle
                this->middle = bi;
                this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE;
            }
            else {
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                this->status = XianDuanChuLiStatus::AFTER_LEFT;
            }
        }
        break;

    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->start.get_high()) {
                this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_high() > this->after_left.get_high()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
                else {
                    this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_low() < this->after_left.get_low()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
                else {
                    this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::AFTER_MIDDLE:
        this->after_middle = bi;
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->middle.get_high()) {
                this->status = XianDuanChuLiStatus::RIGHT_HIGHLOW;
            }
            else {
                this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                this->status = XianDuanChuLiStatus::RIGHT_HIGHLOW;
            }
            else {
                this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
            }
        }
        break;

    case XianDuanChuLiStatus::RIGHT_HIGHLOW:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->left.get_high()) {
                //创新高
                ret_xd.xd1 = XianDuan(this->start, this->after_middle);
                ret_xd.xd2 = XianDuan(bi, bi);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = bi;
                this->status = XianDuanChuLiStatus::LEFT;
                return(ret_xd);
            }
            if (bi.get_high() >= this->after_middle.get_high()) {
                //right包含middle
                this->after_left = bi.generate_bi(this->after_left, this->middle, this->after_middle);
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
            else {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = this->after_middle;
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->left.get_low()) {
                //创新低
                ret_xd.xd1 = XianDuan(this->start, this->after_middle);
                ret_xd.xd2 = XianDuan(bi, bi);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = bi;
                this->status = XianDuanChuLiStatus::LEFT;
                return(ret_xd);
            }
            if (bi.get_low() <= this->after_middle.get_low()) {
                //right 包含 middle
                this->after_left = bi.generate_bi(this->after_left, this->middle, this->after_middle);
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
            else {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = this->after_middle;
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
        }
        break;

    case XianDuanChuLiStatus::RIGHT_NORMAL:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() < this->middle.get_high()) {
                //middle 包含right
                this->right = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_INCLUDE_RIGHT;
            }
            else {
                return(this->__right_process(bi));
            }
        }
        else {
            //下降笔
            if (bi.get_low() > this->middle.get_low()) {
                this->right = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_INCLUDE_RIGHT;
            }
            else {
                return(this->__right_process(bi));
            }
        }
        break;

        case XianDuanChuLiStatus::MIDDLE_INCLUDE_RIGHT:
            if (bi.get_type() == BiType::UP) {
                //上升笔
                if (bi.get_high() > this->middle.get_high()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT_HIGHLOW;
                }
                else {
                    if (bi.get_high() > this->right.get_high()) {
                        this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                        this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                    }
                    else {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                        //增加判断是否包含left
                        if (this->middle.get_low() < this->left.get_low()) {
                            //middle 包含 left
                            this->start = bi.generate_bi(this->start, this->left, this->after_left);
                            this->left = this->middle;
                            this->after_left = bi;
                            this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                        }
                        else {
                            this->after_middle = bi;
                            this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                        }
                    }
                }
            }
            else {
                //下降笔
                if (bi.get_low() < this->middle.get_low()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT_HIGHLOW;
                }
                else {
                    if (bi.get_low() < this->right.get_low()) {
                        this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                        this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                    }
                    else {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                        //判断是否包含left
                        if (this->middle.get_high() > this->left.get_high()) {
                            this->start = bi.generate_bi(this->start, this->left, this->after_left);
                            this->left = this->middle;
                            this->after_left = bi;
                            this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                        }
                        else {
                            this->after_middle = bi;
                            this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                        }
                    }
                }
            }
            break;

    case XianDuanChuLiStatus::AFTER_RIGHT:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->middle.get_high()) {
                //创新高
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                //判断是否包含
                this->after_middle = bi;
                this->status = XianDuanChuLiStatus::RIGHT_HIGHLOW;
            }
            else {
                if (bi.get_high() > this->after_middle.get_high()) {
                    //after_right 包含 after_middle
                    this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                    this->after_middle = bi;
                    this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                }
                else {
                    this->after_right = bi;
                    this->status = XianDuanChuLiStatus::FREE;
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                //创新低
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                //是否包含
                this->after_middle = bi;
                this->status = XianDuanChuLiStatus::RIGHT_HIGHLOW;
            }
            else {
                if (bi.get_low() < this->after_middle.get_low()) {
                    this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                    this->after_middle = bi;
                    this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                }
                else {
                this->after_right = bi;
                this->status = XianDuanChuLiStatus::FREE;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::FREE:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->start.get_high()) {
                //创新高
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, bi);
                ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = bi.generate_bi(this->middle, this->after_middle, this->right);
                this->left = this->after_right;
                this->after_left = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                return(ret_xd);
            }
            if (bi.get_high() > this->after_right.get_high()) {
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                //是否包含
                this->after_middle = this->after_right;
                return(this->__right_process(bi));
            }
            else {
                this->free = bi;
                this->status = XianDuanChuLiStatus::AFTER_FREE;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                //创新低
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, bi);
                ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = bi.generate_bi(this->middle, this->after_middle, this->right);
                this->left = this->after_right;
                this->after_left = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                return(ret_xd);
            }
            if (bi.get_low() < this->after_right.get_low()) {
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                //是否包含
                this->after_middle = this->after_right;
                return(this->__right_process(bi));
            }
            else {
                this->free = bi;
                this->status = XianDuanChuLiStatus::AFTER_FREE;
            }
        }
        break;

    case XianDuanChuLiStatus::AFTER_FREE:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() < this->after_right.get_high()) {
                //after_right 包含 after_free
                this->after_right = bi.generate_bi(this->after_right, this->free, bi);
                this->status = XianDuanChuLiStatus::FREE;
            }
            else {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, this->right);
                if (this->after_middle.get_low() > this->after_right.get_high() + 0.01) {
                    //after_middle和after_right有缺口
                    ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->middle;
                    this->left = this->after_middle;
                    this->after_left = this->right;
                    this->middle = this->after_right;
                    this->after_middle = this->free;
                    this->right = bi;
                    this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    return(ret_xd);
                }
                else {
                    ret_xd.xd3 = XianDuan(this->after_right, bi);
                    if (this->after_right.get_type() == BiType::UP)
                        ret_xd.xd3.set_type(XianDuanType::TEMP_UP);
                    else
                        ret_xd.xd3.set_type(XianDuanType::TEMP_DOWN);
                    ret_xd.type = FindXianDuanReturnType::Three;
                    this->start = this->after_right;
                    this->left = this->free;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }

            }
        }
        else {
            //下降笔
            if (bi.get_low() > this->after_right.get_low()) {
                //after_right 包含 after_free
                this->after_right = bi.generate_bi(this->after_right, this->free, bi);
                this->status = XianDuanChuLiStatus::FREE;
            }
            else {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                ret_xd.xd2 = XianDuan(this->middle, this->right);
                if (this->after_right.get_low() > this->after_middle.get_high() + 0.01) {
                    //有缺口
                    ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->middle;
                    this->left = this->after_middle;
                    this->after_left = this->right;
                    this->middle = this->after_right;
                    this->after_middle = this->free;
                    this->right = bi;
                    this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    return(ret_xd);
                }
                else {
                    ret_xd.xd3 = XianDuan(this->after_right, bi);
                    if (this->after_right.get_type() == BiType::UP)
                        ret_xd.xd3.set_type(XianDuanType::TEMP_UP);
                    else
                        ret_xd.xd3.set_type(XianDuanType::TEMP_DOWN);
                    ret_xd.type = FindXianDuanReturnType::Three;
                    this->start = this->after_right;
                    this->left = this->free;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }
            }
        }
        break;
    }

    ret_xd.type = FindXianDuanReturnType::None;
    ret_xd.xd1 = XianDuan();
    ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
    ret_xd.xd2 = XianDuan();
    return(ret_xd);
}

FindXianDuanReturn XianDuanChuLi::__right_process(Bi bi) {
    FindXianDuanReturn ret_xd = FindXianDuanReturn();
    if (this->middle.get_high() > this->left.get_high()) {
        if (bi.get_high() > this->middle.get_high()) {
            //上升处理
            if (this->start.get_type() == BiType::DOWN) {
                if (bi.get_high() > this->start.get_high()) {
                    ret_xd.xd1 = XianDuan(this->start, this->start);
                    ret_xd.xd2 = XianDuan(this->left, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->left = this->middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }
                else {
                    this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->after_left = this->after_middle;
                    this->middle = bi;
                    this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
                }
            }
            else {
                this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                this->after_left = this->after_middle;
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
        }
        else {
            //顶分型
            if (this->middle.get_low() > this->left.get_high() + 0.01) {
                //left 和 middle 有缺口
                this->right = bi;
                this->status = XianDuanChuLiStatus::AFTER_RIGHT;
            }
            else {
                if (this->start.get_type() == BiType::UP) {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_xd.xd2 = XianDuan(this->middle, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->middle;
                    this->left = this->after_middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                    }
                else {
                    ret_xd.xd1 = XianDuan(this->start, this->start);
                    ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_xd.xd2 = XianDuan(this->left, this->middle);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->after_middle;
                    this->left = bi;
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                    return(ret_xd);
                }
            }
        }
    }
    else {
        if (bi.get_low() < this->middle.get_low()) {
            //下降处理
            if (this->start.get_type() == BiType::UP) {
                if (bi.get_low() < this->start.get_low()) {
                    //创新低
                    ret_xd.xd1 = XianDuan(this->start, this->start);
                    ret_xd.xd2 = XianDuan(this->left, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                    this->start = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->left = this->after_middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }
                else {
                    this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->after_left = this->after_middle;
                    this->middle = bi;
                    this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
                }
            }
            else {
                this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                this->after_left = this->after_middle;
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }

        }
        else {
            //底分型
            if (this->left.get_low() > this->middle.get_high() + 0.01) {
                //有缺口
                this->right = bi;
                this->status = XianDuanChuLiStatus::AFTER_RIGHT;
            }
            else {
                if (this->start.get_type() == BiType::DOWN) {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_xd.xd2 = XianDuan(this->middle, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->middle;
                    this->left = this->after_middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }
                else {
                    ret_xd.xd1 = XianDuan(this->start, this->start);
                    ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_xd.xd2 = XianDuan(this->left, this->middle);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->after_middle;
                    this->left = bi;
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                    return(ret_xd);
                }
            }
        }
    }
    ret_xd.type = FindXianDuanReturnType::None;
    return(ret_xd);
}

void Bi3_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    int pos, start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

    unsigned int count = xdchuli.key_xianduan_list.size() - 1;
    if (count == 0) {
        return;
    }

    if (!xdchuli.key_xianduan_list.empty()) {
        XianDuan start_xd = xdchuli.key_xianduan_list[0];
        start_pos = start_xd.get_start_pos();
        stop_pos = start_xd.get_stop_pos();

        if (start_xd.get_type() == XianDuanType::UP) {
            pOut[start_pos] = -3;
            pOut[stop_pos] = 3;
        }
        else {
            pOut[start_pos] = +3;
            pOut[stop_pos] = -3;
        }
        bool last_xd_status = true;
        for (unsigned int i = count; i > 0; i--) {
            xd = xdchuli.key_xianduan_list[i - 1];
            stop_pos = xd.get_stop_pos();
            switch (xd.get_type()) {
            case XianDuanType::TEMP_UP:
                pOut[stop_pos] = 1;
                break;
            case XianDuanType::FAILURE_TEMP_UP:
                pOut[stop_pos] = 2;
                break;
            case XianDuanType::UP:
                if (last_xd_status) {
                    pOut[stop_pos] = 6;
                    last_xd_status = false;
                }
                else
                    pOut[stop_pos] = 3;
                break;
            case XianDuanType::FAILURE_UP:
                pOut[stop_pos] = 4;
                break;
            case XianDuanType::TEMP_DOWN:
                pOut[stop_pos] = -1;
                break;
            case XianDuanType::FAILURE_TEMP_DOWN:
                pOut[stop_pos] = -2;
                break;
            case XianDuanType::DOWN:
                if (last_xd_status) {
                    pOut[stop_pos] = -6;
                    last_xd_status = false;
                }
                else
                    pOut[stop_pos] = -3;
                break;
            case XianDuanType::FAILURE_DOWN:
                pOut[stop_pos] = -4;
                break;
            }
        }

        vector<Bi> bi_list = xdchuli.get_xd_bi_list();
        if (!bi_list.empty()) {
            Bi bi = bi_list[0];
            pos = bi.get_start_pos();
            if (bi.get_type() == BiType::DOWN) {
                pOut[pos] = 10;
            }
            else {
                pOut[pos] = -10;
            }
            for (unsigned int i = 1; i < bi_list.size(); i++) {
                bi = bi_list[i];
                if (bi.get_type() != BiType::NONE) {
                    pos = bi.get_start_pos();
                    if (bi.get_type() == BiType::UP)
                        pOut[pos] = -11;
                    else
                        pOut[pos] = +11;
                }
                else {
                    bi = bi_list[i - 1];
                }
            }

            pos = bi.get_stop_pos();
            if (bi.get_type() == BiType::UP) {
                if (pOut[pos] == 0)
                    pOut[pos] = 11;
            }
            else {
                if (pOut[pos] == 0)
                    pOut[pos] = -11;
            }
        }
        }


}

void Bi4_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    int stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

    unsigned int count = xdchuli.key_xianduan_list.size();
    if (count == 0) {
        return;
    }

    XianDuan start_xd = xdchuli.key_xianduan_list[0];
    int start_pos = start_xd.get_start_pos();
    stop_pos = start_xd.get_stop_verify_pos();
    if (start_xd.get_type() == XianDuanType::UP) {
        pOut[start_pos] = -3;
        pOut[stop_pos] = +3;
    }
    else {
        pOut[start_pos] = +3;
        pOut[stop_pos] = -3;
    }
    for (unsigned int i = 1; i < count;  i++) {
        xd = xdchuli.key_xianduan_list[i];
        stop_pos = xd.get_stop_verify_pos();
        switch (xd.get_type()) {
        case XianDuanType::TEMP_UP:
            pOut[stop_pos] = 1;
            break;
        case XianDuanType::FAILURE_TEMP_UP:
            pOut[stop_pos] = 2;
            break;
        case XianDuanType::UP:
            pOut[stop_pos] = 3;
            break;
        case XianDuanType::FAILURE_UP:
            pOut[stop_pos] = 4;
            break;
        case XianDuanType::TEMP_DOWN:
            pOut[stop_pos] = -1;
            break;
        case XianDuanType::FAILURE_TEMP_DOWN:
            pOut[stop_pos] = -2;
            break;
        case XianDuanType::DOWN:
            pOut[stop_pos] = -3;
            break;
        case XianDuanType::FAILURE_DOWN:
            pOut[stop_pos] = -4;
            break;
        }
    }
}
