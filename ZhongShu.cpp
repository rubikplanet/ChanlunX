#include "ZhongShu.h"

ZhongShu::ZhongShu()
{
    this->bValid = false;
    this->nTop1 = 0;
    this->nTop2 = 0;
    this->nTop3 = 0;
    this->nBot1 = 0;
    this->nBot2 = 0;
    this->nBot3 = 0;
    this->fTop1 = 0;
    this->fTop2 = 0;
    this->fTop3 = 0;
    this->fBot1 = 0;
    this->fBot2 = 0;
    this->fBot3 = 0;
    this->nLines = 0;
    this->nStart = 0;
    this->nEnd = 0;
    this->fHigh = 0;
    this->fLow = 0;
    this->nDirection = 0;
    this->nTerminate = 0;
}

ZhongShu::~ZhongShu()
{
}

void ZhongShu::Reset()
{
    this->bValid = false;
    this->nTop1 = 0;
    this->nTop2 = 0;
    this->nTop3 = 0;
    this->nBot1 = 0;
    this->nBot2 = 0;
    this->nBot3 = 0;
    this->fTop1 = 0;
    this->fTop2 = 0;
    this->fTop3 = 0;
    this->fBot1 = 0;
    this->fBot2 = 0;
    this->fBot3 = 0;
    this->nLines = 0;
    this->nStart = 0;
    this->nEnd = 0;
    this->fHigh = 0;
    this->fLow = 0;
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
        if (fTop1 < fLow) // 中枢终结
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
        if (nTop3 > 0 && nTop2 > 0 && nTop1 > 0 && nBot2 > 0 && nBot1 > 0) // 有两个高点和两个低点
        {
            float fTempHigh = (fTop1 < fTop2 ? fTop1 : fTop2);
            float fTempLow = (fBot1 > fBot2 ? fBot1 : fBot2);
            if (fTop3 > fTop2 && fTempHigh > fTempLow) // 有中枢
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
        if (fBot1 > fHigh) // 中枢终结
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
        if (nTop2 > 0 && nTop1 > 0 && nBot3 > 0 && nBot2 > 0 && nBot1 > 0) // 有两个高点和两个低点
        {
            float fTempHigh = (fTop1 < fTop2 ? fTop1 : fTop2);
            float fTempLow = (fBot1 > fBot2 ? fBot1 : fBot2);
            if (fBot3 < fBot2 && fTempHigh > fTempLow) // 有中枢
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

std::vector<Pivot> ZS(int nCount, std::vector<float> pIn, std::vector<float> pHigh, std::vector<float> pLow)
{
    std::vector<Pivot> ZhongShuList;
    ZhongShu ZhongShuOne;
    for (int i = 0; i < nCount; i++)
    {
        if (pIn[i] == 1)
        {
            // 遇到线段高点，推入中枢算法
            if (ZhongShuOne.PushHigh(i, pHigh[i]))
            {
                bool bValid = true;
                float fHighValue = 0;
                int nHignIndex = 0;
                int nLowIndex = 0;
                int nLowIndexTemp = 0;
                int nHighCount = 0;
                if (ZhongShuOne.nDirection == 1 && ZhongShuOne.nTerminate == -1) // 向上中枢被向下终结
                {
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == 1)
                        {
                            if (nHighCount == 0)
                            {
                                nHighCount++;
                                fHighValue = pHigh[x];
                                nHignIndex = x;
                            }
                            else
                            {
                                nHighCount++;
                                if (pHigh[x] >= fHighValue)
                                {
                                    if (nHighCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fHighValue = pHigh[x];
                                    nHignIndex = x;
                                    nLowIndex = nLowIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == -1)
                        {
                            nLowIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        ZhongShuOne.nEnd = nLowIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    i = nHignIndex - 1;
                }
                else
                {
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    Pivot pivot;
                    pivot.s = ZhongShuOne.nStart;
                    pivot.e = ZhongShuOne.nEnd;
                    pivot.zg = ZhongShuOne.fHigh;
                    pivot.zd = ZhongShuOne.fLow;
                    pivot.direction = (float)ZhongShuOne.nDirection;
                    pivot.gg = *std::max_element(pHigh.begin() + pivot.s + 1, pHigh.begin() + pivot.e, [](float a, float b)
                                                 { return a < b; });
                    pivot.dd = *std::min_element(pLow.begin() + pivot.s + 1, pLow.begin() + pivot.e, [](float a, float b)
                                                 { return a < b; });
                    ZhongShuList.push_back(pivot);
                }
                ZhongShuOne.Reset();
            }
        }
        else if (pIn[i] == -1)
        {
            // 遇到线段低点，推入中枢算法
            if (ZhongShuOne.PushLow(i, pLow[i]))
            {
                bool bValid = true;
                float fLowValue = 0;
                int nLowIndex = 0;
                int nHighIndex = 0;
                int nHighIndexTemp = 0;
                int nLowCount = 0;
                if (ZhongShuOne.nDirection == -1 && ZhongShuOne.nTerminate == 1) // 向下中枢被向上终结
                {
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == -1)
                        {
                            if (nLowCount == 0)
                            {
                                nLowCount++;
                                fLowValue = pLow[x];
                                nLowIndex = x;
                            }
                            else
                            {
                                nLowCount++;
                                if (pLow[x] <= fLowValue)
                                {
                                    if (nLowCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fLowValue = pLow[x];
                                    nLowIndex = x;
                                    nHighIndex = nHighIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == 1)
                        {
                            nHighIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        ZhongShuOne.nEnd = nHighIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    i = nLowIndex - 1;
                }
                else
                {
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    Pivot pivot;
                    pivot.s = ZhongShuOne.nStart;
                    pivot.e = ZhongShuOne.nEnd;
                    pivot.zg = ZhongShuOne.fHigh;
                    pivot.zd = ZhongShuOne.fLow;
                    pivot.direction = (float)ZhongShuOne.nDirection;
                    pivot.gg = *std::max_element(pHigh.begin() + pivot.s + 1, pHigh.begin() + pivot.e, [](float a, float b)
                                                 { return a < b; });
                    pivot.dd = *std::min_element(pLow.begin() + pivot.s + 1, pLow.begin() + pivot.e, [](float a, float b)
                                                 { return a < b; });
                    ZhongShuList.push_back(pivot);
                }
                ZhongShuOne.Reset();
            }
        }
    }
    if (ZhongShuOne.bValid) // 最后一个还没有被终结的中枢。
    {
        Pivot pivot;
        pivot.s = ZhongShuOne.nStart;
        pivot.e = ZhongShuOne.nEnd;
        pivot.zg = ZhongShuOne.fHigh;
        pivot.zd = ZhongShuOne.fLow;
        pivot.direction = (float)ZhongShuOne.nDirection;
        pivot.gg = *std::max_element(pHigh.begin() + pivot.s + 1, pHigh.begin() + pivot.e, [](float a, float b)
                                     { return a < b; });
        pivot.dd = *std::min_element(pLow.begin() + pivot.s + 1, pLow.begin() + pivot.e, [](float a, float b)
                                     { return a < b; });
        ZhongShuList.push_back(pivot);
    }
    return ZhongShuList;
}