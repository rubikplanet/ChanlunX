#ifndef __BAOHANCHULI_H
#define __BAOHANCHULI_H

#include <vector>

using namespace std;

#pragma pack(push, 1)

//原始K线
struct KxianRaw1
{
    float gao;
    float di;
};

//包含处理后的K线
struct Kxian1
{
    float gao;
    float di;
    int fangxiang;
    bool status;
    int position; /*从起始K线开始的位置 */
    int fx_type; /* 1-顶分型, -1-底分型, 0-普通 */
};

class BaoHanChuLi
{
    public:
      vector<KxianRaw1> kxianRawList;
      vector<Kxian1> kxianList;
      Kxian1 add(float gao, float di);
      static int count;
};

#pragma pack(pop)

#endif
