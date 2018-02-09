#ifndef __ZHONGSHU_H__
#define __ZHONGSHU_H__

#pragma pack(push,1)

struct ZhongShu
{
    bool  bValid;
    int   nTop1, nTop2, nTop3, nBot1, nBot2, nBot3;
    float fTop1, fTop2, fTop3, fBot1, fBot2, fBot3;
    int   nLines, nStart, nEnd;
    float fHigh, fLow, fPHigh, fPLow;
    int nDirection;
    int nTerminate;

    ZhongShu();
    ~ZhongShu();

    void Reset();
    bool PushHigh(int nIndex, float fValue);
    bool PushLow (int nIndex, float fValue);
};

#pragma pack(pop)

#endif
