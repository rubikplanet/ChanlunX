#include "KxianChuLi.h"

using namespace std;

void KxianChuLi::add(float gao, float di)
{
    KxianRaw kxianRaw;
    kxianRaw.gao = gao;
    kxianRaw.di = di;
    // 保存原始K线
    this->kxianRawList.push_back(kxianRaw);
    if (this->kxianList.empty())
    {
        // 第一根K线先假设方向为上
        Kxian kxian;
        kxian.gao = gao;
        kxian.di = di;
        kxian.fangXiang = 1;
        kxian.kaiShi = 0;
        kxian.jieShu = 0;
        kxian.zhongJian = 0;
        this->kxianList.push_back(kxian);
    }
    else
    {
        if (gao > this->kxianList.back().gao && di > this->kxianList.back().di)
        {
            // 向上
            Kxian kxian;
            kxian.gao = gao;
            kxian.di = di;
            kxian.fangXiang = 1;
            kxian.kaiShi = this->kxianList.back().jieShu + 1;
            kxian.jieShu = kxian.kaiShi;
            kxian.zhongJian = kxian.kaiShi;
            // 新K线
            this->kxianList.push_back(kxian);
        }
        else if (gao < this->kxianList.back().gao && di < this->kxianList.back().di)
        {
            // 向下
            Kxian kxian;
            kxian.gao = gao;
            kxian.di = di;
            kxian.fangXiang = -1;
            kxian.kaiShi = this->kxianList.back().jieShu + 1;
            kxian.jieShu = kxian.kaiShi;
            kxian.zhongJian = kxian.kaiShi;
            // 新K线
            this->kxianList.push_back(kxian);
        }
        else if (gao <= this->kxianList.back().gao && di >= this->kxianList.back().di)
        {
            // 前包含
            if (this->kxianList.back().fangXiang == 1)
            {
                this->kxianList.back().di = di;
            }
            else
            {
                this->kxianList.back().gao = gao;
            }
            this->kxianList.back().jieShu = this->kxianList.back().jieShu + 1;
        }
        else
        {
            // 后包含
            if (this->kxianList.back().fangXiang == 1)
            {
                this->kxianList.back().gao = gao;
            }
            else
            {
                this->kxianList.back().di = di;
            }
            this->kxianList.back().jieShu = this->kxianList.back().jieShu + 1;
            this->kxianList.back().zhongJian = this->kxianList.back().jieShu;
        }
    }
}
