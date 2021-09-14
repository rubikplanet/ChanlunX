#ifndef __BICHULI_H__
#define __BICHULI_H__

#include <vector>
#include "KxianChuLi.h"

using namespace std;

#pragma pack(push, 1)

struct Bi11
{
    int fangXiang;           // 笔方向
    int kaiShi;              // 笔起点
    int jieShu;              // 笔终点
    float gao;               // 笔最高价
    float di;                // 笔最低价
    vector<Kxian> kxianList; // 一笔当中的K线
};

class BiChuLi1
{
  public:
    vector<Bi11> biList; // 笔的表格
    void handle(vector<Kxian> &kxianList);
};

#pragma pack(pop)

#endif
