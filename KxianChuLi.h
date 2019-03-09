#ifndef __KXIANCHULI_H__
#define __KXIANCHULI_H__

#include <vector>

using namespace std;

#pragma pack(push,1)

// 原始K线
struct KxianRaw {
    float gao;
    float di;
};

// 表示合并后的K线
struct Kxian {
    float gao; // K线高
    float di; // K线低
    int fangXiang; // K线方向
    int kaiShi; // 开始K线坐标
    int jieShu; // 结束K线坐标
};

class KxianChuLi {
    public:
        vector<KxianRaw> kxianRawList;
        vector<Kxian> kxianList;
        void add(float gao, float di);
};

#pragma pack(pop)

#endif
