#include "BiChuLi.h"
#include "KxianChuLi.h"

using namespace std;

void BiChuLi::handle(vector<Kxian> &kxianList)
{
    for (vector<Kxian>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++)
    {
        if (this->biList.empty())
        {
            // 第一笔生成中，也是假设第一笔是向上的
            Bi bi;
            bi.fangXiang = 1;
            bi.kaiShi = iter->kaiShi;
            bi.jieShu = iter->jieShu;
            bi.gao = iter->gao;
            bi.di = iter->di;
            this->biList.push_back(bi);
        }
        else
        {

        }
    }
}
