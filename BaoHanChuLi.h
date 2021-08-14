#ifndef __BAOHANCHULI_H
#define __BAOHANCHULI_H

#include <vector>

using namespace std;

#pragma pack(push, 1)

//包含处理后的K线

//K线方向
#define Kxian_UP        1   
#define Kxian_DOWN      -1

struct Kxian1
{
    float gao;
    float di;
    int fangxiang;
    int position; /*从起始K线开始的位置 */
};

class BaoHanChuLi
{
    public:
      vector<Kxian1> kxianList;
      Kxian1 add(float gao, float di);
      int count = 0;
};

#pragma pack(pop)

#endif
