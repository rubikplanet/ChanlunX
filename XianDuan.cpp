#include "XianDuan.h"
#include "ZhongShu1.h"

using namespace std;

XianDuan XianDuanChuLi::last_xd = XianDuan();
XianDuanChuLiStatus  XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
Bi XianDuanChuLi::last_bi = Bi();

XianDuanChuLi::XianDuanChuLi() {
    this->last_xd = XianDuan();
    this->last_bi = Bi();
    this->bicl = BiChuLi();
    this->status = XianDuanChuLiStatus::START;
    this->zscl = ZhongShuChuLi();
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
                this->xianDuanList.push_back(ret_fd.xd1);
                break;
            case FindXianDuanReturnType::Two:
                this->xianDuanList.push_back(ret_fd.xd1);
                this->xianDuanList.push_back(ret_fd.xd2);
                break;
            case FindXianDuanReturnType::Three:
                this->xianDuanList.push_back(ret_fd.xd1);
                this->xianDuanList.push_back(ret_fd.xd2);
                this->xianDuanList.push_back(ret_fd.xd3);
                break;
            case FindXianDuanReturnType::Failure:
                if (!this->xianDuanList.empty())
                    this->xianDuanList.pop_back();
                break;
            case FindXianDuanReturnType::None:
                break;
            }
        }
    }
}

XianDuan XianDuanChuLi::failure_xd(Bi start_bi, Bi stop_bi) {
    XianDuanType last_xd_type = this->last_xd.get_type();
    XianDuan xd = XianDuan();
    Bi xd_start_bi = Bi();

    if (last_xd_type == XianDuanType::NONE) {
        xd = XianDuan(start_bi, start_bi);
    }
    else {
        xd = XianDuan(this->last_xd.get_start_bi(), start_bi);
    }
    this->start = stop_bi;
    this->status = XianDuanChuLiStatus::LEFT;
    return(xd);
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

FindXianDuanReturn XianDuanChuLi::__find_xianduan(Bi bi) {
    XianDuan xd = XianDuan();
    FindXianDuanReturn ret_xd;
    ZhongShu1 zs = ZhongShu1();

    switch (this->status) {

    case XianDuanChuLiStatus::START:
        this->start = bi;
        this->status = XianDuanChuLiStatus::LEFT;
        break;

    case XianDuanChuLiStatus::LEFT:
        if (this->start.get_type() == BiType::UP && bi.get_low() < this->start.get_low() || (this->start.get_type() == BiType::DOWN && bi.get_high() > this->start.get_high())) {
            ret_xd.xd1 = this->failure_xd(this->start, bi);
            ret_xd.type = FindXianDuanReturnType::Failure;
            return(ret_xd);
        }
        this->left = bi;
        this->status = XianDuanChuLiStatus::AFTER_LEFT;
        break;

    case XianDuanChuLiStatus::AFTER_LEFT:
        this->after_left = bi;
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
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
                ret_xd.type = FindXianDuanReturnType::One;
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
                ret_xd.type = FindXianDuanReturnType::One;
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
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
                return(ret_xd);
            }
            if (bi.get_high() <= this->left.get_high()) {
                //left 包含middle
                this->left = bi.generate_bi(this->left, this->after_left, bi);
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
                ret_xd.xd1 = XianDuan(this->start, this->start);
                ret_xd.xd2 = XianDuan(this->left, bi);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
                return(ret_xd);
            }
            if (bi.get_low() >= this->left.get_low()) {
                //left 包含middle
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                this->status = XianDuanChuLiStatus::AFTER_LEFT;
            }
            else {
                this->middle = bi;
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
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
            if (bi.get_high() > this->middle.get_high()) {
                //创新高
                ret_xd.xd1 = XianDuan(this->start, this->after_middle);
                ret_xd.type = FindXianDuanReturnType::One;
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
            if (bi.get_low() < this->middle.get_low()) {
                //创新低
                ret_xd.xd1 = XianDuan(this->start, this->after_middle);
                ret_xd.type = FindXianDuanReturnType::One;
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
            if (bi.get_high() > this->start.get_high()) {
                //创新高
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, bi);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
                return(ret_xd);
            }
            if (bi.get_high() < this->middle.get_high()) {
                //middle 包含right
                this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
            else {
                return(this->__right_process(bi));
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                //创新低
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, bi);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
                return(ret_xd);
            }
            if (bi.get_low() > this->middle.get_low()) {
                this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
            }
            else {
                return(this->__right_process(bi));
            }
        }
        break;

    case XianDuanChuLiStatus::AFTER_RIGHT:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->middle.get_high()) {
                //创新高
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
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
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
                return(ret_xd);
            }
            if (bi.get_high() > this->after_right.get_high()) {
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
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
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
                return(ret_xd);
            }
            if (bi.get_low() < this->after_right.get_low()) {
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
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
        else {
            //下降笔
            if (bi.get_low() > this->after_right.get_low()) {
                //after_right 包含 after_free
                this->after_right = bi.generate_bi(this->after_right, this->free, bi);
                this->status = XianDuanChuLiStatus::FREE;
            }
            else {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, this->right);
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
        break;
    }

    ret_xd.type = FindXianDuanReturnType::None;
    ret_xd.xd1 = XianDuan();
    ret_xd.xd2 = XianDuan();
    return(ret_xd);
}

FindXianDuanReturn XianDuanChuLi::__right_process(Bi bi) {
    FindXianDuanReturn ret_xd = FindXianDuanReturn();
    if (this->middle.get_high() > this->left.get_high()) {
        if (bi.get_high() > this->middle.get_high()) {
            //上升处理
            this->left = bi.generate_bi(this->left, this->after_left, this->middle);
            this->after_left = this->after_middle;
            this->middle = bi;
            this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
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
            this->left = bi.generate_bi(this->left, this->after_left, this->middle);
            this->after_left = this->after_middle;
            this->middle = bi;
            this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
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
                    ret_xd.xd2 = XianDuan(this->middle, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->middle;
                    this->left = this->after_middle;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }
                else {
                    ret_xd.xd1 = XianDuan(this->start, this->start);
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
    int start_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

    unsigned int count = xdchuli.xianDuanList.size();
    if (count == 0) {
        return;
    }

    for (unsigned int i = 0; i < count; i++) {
        xd = xdchuli.xianDuanList[i];
        start_pos = xd.get_start_pos();
        switch (xd.get_type()) {
        case XianDuanType::TEMP_UP:
            pOut[start_pos] = 1;
            break;
        case XianDuanType::FAILURE_TEMP_UP:
            pOut[start_pos] = 2;
            break;
        case XianDuanType::UP:
            pOut[start_pos] = 3;
            break;
        case XianDuanType::FAILURE_UP:
            pOut[start_pos] = 4;
            break;
        case XianDuanType::TEMP_DOWN:
            pOut[start_pos] = -1;
            break;
        case XianDuanType::FAILURE_TEMP_DOWN:
            pOut[start_pos] = -2;
            break;
        case XianDuanType::DOWN:
            pOut[start_pos] = -3;
            break;
        case XianDuanType::FAILURE_DOWN:
            pOut[start_pos] = -4;
            break;
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

    unsigned int count = xdchuli.xianDuanList.size();
    if (count == 0) {
        return;
    }

    for (unsigned int i = 0; i < count;  i++) {
        xd = xdchuli.xianDuanList[i];
        stop_pos = xd.get_stop_pos();
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
