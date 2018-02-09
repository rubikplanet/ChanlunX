#include "ZhongShu.h"

ZhongShu::ZhongShu()
{
    this->bValid = false;
    this->nTop1  = 0;
    this->nTop2  = 0;
    this->nTop3  = 0;
    this->nBot1  = 0;
    this->nBot2  = 0;
    this->nBot3  = 0;
    this->fTop1  = 0;
    this->fTop2  = 0;
    this->fTop3  = 0;
    this->fBot1  = 0;
    this->fBot2  = 0;
    this->fBot3  = 0;
    this->nLines = 0;
    this->nStart = 0;
    this->nEnd   = 0;
    this->fHigh  = 0;
    this->fLow   = 0;
    this->nDirection = 0;
    this->nTerminate = 0;
}

ZhongShu::~ZhongShu()
{
}

void ZhongShu::Reset()
{
    this->bValid = false;
    this->nTop1  = 0;
    this->nTop2  = 0;
    this->nTop3  = 0;
    this->nBot1  = 0;
    this->nBot2  = 0;
    this->nBot3  = 0;
    this->fTop1  = 0;
    this->fTop2  = 0;
    this->fTop3  = 0;
    this->fBot1  = 0;
    this->fBot2  = 0;
    this->fBot3  = 0;
    this->nLines = 0;
    this->nStart = 0;
    this->nEnd   = 0;
    this->fHigh  = 0;
    this->fLow   = 0;
    this->nDirection = 0;
    this->nTerminate = 0;
}

// 推入高点并计算状态
bool ZhongShu::PushHigh(int nIndex, float fValue)
{
    nTop3 = nTop2;
    fTop3 = fTop2;
    nTop2 = nTop1;
    fTop2 = fTop1;
    nTop1 = nIndex;
    fTop1 = fValue;
    if (bValid == true)
    {
        if (fTop1 < fLow)   // 中枢终结
        {
            nTerminate = -1;
            if (nTop2 > nEnd)
            {
                nEnd = nTop2;
            }
            return true;
        }
        else
        {
            if (nBot1 > nEnd)
            {
                nEnd = nBot1; // 中枢终结点后移
            }
        }
    }
    else
    {
        if (nTop3 > 0 && nTop2 > 0 && nTop1 > 0 && nBot2 > 0 && nBot1 > 0)   // 有两个高点和两个低点
        {
            float fTempHigh = (fTop1 < fTop2 ? fTop1 : fTop2);
            float fTempLow = (fBot1 > fBot2 ? fBot1 : fBot2);
            if (fTop3 > fTop2 && fTempHigh > fTempLow)   // 有中枢
            {
                nDirection = -1; // 向下中枢
                nStart = nBot2;
                nEnd = nTop1;
                fHigh = fTempHigh;
                fLow = fTempLow;
                bValid = true;
            }
        }
    }
    return false;
}

bool ZhongShu::PushLow(int nIndex, float fValue)
{
    nBot3 = nBot2;
    fBot3 = fBot2;
    nBot2 = nBot1;
    fBot2 = fBot1;
    nBot1 = nIndex;
    fBot1 = fValue;
    if (bValid == true)
    {
        if (fBot1 > fHigh)   // 中枢终结
        {
            nTerminate = 1;
            if (nBot2 > nEnd)
            {
                nEnd = nBot2;
            }
            return true;
        }
        else
        {
            if (nTop1 > nEnd)
            {
                nEnd = nTop1; // 中枢终结点后移
            }
        }
    }
    else
    {
        if (nTop2 > 0 && nTop1 > 0 && nBot3 > 0 && nBot2 > 0 && nBot1 > 0)   // 有两个高点和两个低点
        {
            float fTempHigh = (fTop1 < fTop2 ? fTop1 : fTop2);
            float fTempLow = (fBot1 > fBot2 ? fBot1 : fBot2);
            if (fBot3 < fBot2 && fTempHigh > fTempLow)   // 有中枢
            {
                nDirection = 1; // 向上中枢
                nStart = nTop2;
                nEnd = nBot1;
                fHigh = fTempHigh;
                fLow = fTempLow;
                bValid = true;
            }
        }
    }
    return false;
}
