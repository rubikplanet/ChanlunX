#include <vector>
#include "Duan.h"


void Duan1(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    int nState = 0;
    int nLastD = 0; // 前一个向下线段的底
    int nLastG = 0; // 前一个向上线段的顶
    float fTop0 = 0;
    float fTop1 = 0;
    float fTop2 = 0;
    float fBot0 = 0;
    float fBot1 = 0;
    float fBot2 = 0;
    for (int i = 0; i < nCount; i++)
    {
        if (pIn[i] == 1)
        {
            fTop1 = fTop2;
            fTop2 = pHigh[i];
        }
        else if (pIn[i] == -1) {
            fBot1 = fBot2;
            fBot2 = pLow[i];
        }

        if (nState == 0)
        {
            if (pIn[i] == 1)
            {
                nState = 1;
                nLastG = i;
                pOut[nLastG] = 1;
                fTop0 = 0;
                fBot0 = 0;
            }
            else if (pIn[i] == -1)
            {
                nState = -1;
                nLastD = i;
                pOut[nLastD] = -1;
                fTop0 = 0;
                fBot0 = 0;
            }
        }
        else if (nState == 1) // 向上线段运行中
        {
            if (pIn[i] == 1) // 遇到高点
            {
                if (pHigh[i] > pHigh[nLastG]) // 比线段最高点还高，高点后移
                {
                    pOut[nLastG] = 0;
                    nLastG = i;
                    pOut[nLastG] = 1;
                    fTop0 = 0;
                    fBot0 = 0;
                }
            }
            else if (pIn[i] == -1) // 遇到低点
            {
                if (pLow[i] < pLow[nLastD]) // 低点比向上线段最低点还低了，当一段处理，也是终结。
                {
                    nState = -1;
                    nLastD = i;
                    pOut[nLastD] = -1;
                    fTop0 = 0;
                    fBot0 = 0;
                }
                else if (fTop1 > 0 && fTop2 > 0 && fBot1 > 0 && fBot2 > 0 && fTop2 < fTop1 && fBot2 < fBot1) // 向上线段终结
                {
                    nState = -1;
                    nLastD = i;
                    pOut[nLastD] = -1;
                    fTop0 = 0;
                    fBot0 = 0;
                }
                else
                {
                    if (fBot0 == 0)
                    {
                        fBot0 = pLow[i];
                    }
                    else if (pLow[i] < fBot0) // 向上线段终结
                    {
                        nState = -1;
                        nLastD = i;
                        pOut[nLastD] = -1;
                        fTop0 = 0;
                        fBot0 = 0;
                    }
                }
            }
        }
        else if (nState == -1) // 向下线段运行中
        {
            if (pIn[i] == -1) // 遇到低点
            {
                if (pLow[i] < pLow[nLastD]) // 比线段最低点还低，低点后移
                {
                    pOut[nLastD] = 0;
                    nLastD = i;
                    pOut[nLastD] = -1;
                    fTop0 = 0;
                    fBot0 = 0;
                }
            }
            else if (pIn[i] == 1) // 遇到高点
            {
                if (pHigh[i] > pHigh[nLastG]) // 高点比向下线段最高点还高了，当一段处理，也是终结。
                {
                    nState = 1;
                    nLastG = i;
                    pOut[nLastG] = 1;
                    fTop0 = 0;
                    fBot0 = 0;
                }
                else if (fTop1 > 0 && fTop2 > 0 && fBot1 > 0 && fBot2 > 0 && fTop2 > fTop1 && fBot2 > fBot1) // 向下线段终结
                {
                    nState = 1;
                    nLastG = i;
                    pOut[nLastG] = 1;
                    fTop0 = 0;
                    fBot0 = 0;
                }
                else
                {
                    if (fTop0 == 0)
                    {
                        fTop0 = pHigh[i];
                    }
                    else if (pHigh[i] > fTop0) // 向下线段终结
                    {
                        nState = 1;
                        nLastG = i;
                        pOut[nLastG] = 1;
                        fTop0 = 0;
                        fBot0 = 0;
                    }
                }
            }
        }
    }

}

void Duan2(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    int nState = 0;
    int nLastD = 0; // 前一个向下线段的底
    int nLastG = 0; // 前一个向上线段的顶
    float fTop1 = 0;
    float fTop2 = 0;
    float fBot1 = 0;
    float fBot2 = 0;
    for (int i = 0; i < nCount; i++)
    {
        if (pIn[i] == 1)
        {
            fTop1 = fTop2;
            fTop2 = pHigh[i];
        }
        else if (pIn[i] == -1) {
            fBot1 = fBot2;
            fBot2 = pLow[i];
        }

        if (nState == 0)
        {
            if (pIn[i] == 1)
            {
                nState = 1;
                nLastG = i;
                pOut[nLastG] = 1;
            }
            else if (pIn[i] == -1)
            {
                nState = -1;
                nLastD = i;
                pOut[nLastD] = -1;
            }
        }
        else if (nState == 1) // 向上线段运行中
        {
            if (pIn[i] == 1) // 遇到高点
            {
                if (pHigh[i] > pHigh[nLastG]) // 比线段最高点还高，高点后移
                {
                    pOut[nLastG] = 0;
                    nLastG = i;
                    pOut[nLastG] = 1;
                }
            }
            else if (pIn[i] == -1) // 遇到低点
            {
                if (pLow[i] < pLow[nLastD]) // 低点比向上线段最低点还低了，当一段处理，也是终结。
                {
                    nState = -1;
                    nLastD = i;
                    pOut[nLastD] = -1;
                }
                else if (fTop1 > 0 && fTop2 > 0 && fBot1 > 0 && fBot2 > 0 && fTop2 < fTop1 && fBot2 < fBot1) // 向上线段终结
                {
                    nState = -1;
                    nLastD = i;
                    pOut[nLastD] = -1;
                }
            }
        }
        else if (nState == -1) // 向下线段运行中
        {
            if (pIn[i] == -1) // 遇到低点
            {
                if (pLow[i] < pLow[nLastD]) // 比线段最低点还低，低点后移
                {
                    pOut[nLastD] = 0;
                    nLastD = i;
                    pOut[nLastD] = -1;
                }
            }
            else if (pIn[i] == 1) // 遇到高点
            {
                if (pHigh[i] > pHigh[nLastG]) // 高点比向下线段最高点还高了，当一段处理，也是终结。
                {
                    nState = 1;
                    nLastG = i;
                    pOut[nLastG] = 1;
                }
                else if (fTop1 > 0 && fTop2 > 0 && fBot1 > 0 && fBot2 > 0 && fTop2 > fTop1 && fBot2 > fBot1) // 向下线段终结
                {
                    nState = 1;
                    nLastG = i;
                    pOut[nLastG] = 1;
                }
            }
        }
    }
}
