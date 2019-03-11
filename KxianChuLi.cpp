#include "KxianChuLi.h"

using namespace std;

void KxianChuLi::add(float gao, float di) {
    KxianRaw kxianRaw;
    kxianRaw.gao = gao;
    kxianRaw.di = di;
    // 保存原始K线
    this->kxianRawList.push_back(kxianRaw);
    if (this->kxianList.empty()) {
        // 刚开始先假设方向为上
        Kxian kxian;
        kxian.gao = gao;
        kxian.di = di;
        kxian.fangXiang = 1;
        kxian.kaiShi = 0;
        kxian.jieShu = 0;
        this->kxianList.push_back(kxian);
    } else {
        Kxian back = this->kxianList.back();
        if (gao > back.gao && di > back.di) {
            // 向上
            Kxian kxian;
            kxian.gao = gao;
            kxian.di = di;
            kxian.fangXiang = 1;
            kxian.kaiShi = back.jieShu + 1;
            kxian.jieShu = kxian.kaiShi;
            // 新K线
            this->kxianList.push_back(kxian);
        } else if (gao < back.gao && di < back.di) {
            // 向下
            Kxian kxian;
            kxian.gao = gao;
            kxian.di = di;
            kxian.fangXiang = -1;
            kxian.kaiShi = back.jieShu + 1;
            kxian.jieShu = kxian.kaiShi;
            // 新K线
            this->kxianList.push_back(kxian);
        } else if (gao <= back.gao && di >= back.di) {
            // 前包含
            if (back.fangXiang == 1) {
                back.di = di;
            } else {
                back.gao = gao;
            }
            back.jieShu = back.jieShu + 1;
        } else {
            // 后包含
            if (back.fangXiang == 1) {
                back.gao = gao;
            } else {
                back.di = di;
            }
            back.jieShu = back.jieShu + 1;
        }
    }
}
