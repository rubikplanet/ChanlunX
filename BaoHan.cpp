#include "BaoHan.h"

void BaoHan(int nCount, float *pDirection, float *pOutHigh, float *pOutLow, float *pInclude, float *pHigh, float *pLow)
{
    if (pHigh[1] > pHigh[0] && pLow[1] > pLow[0])
    {
        pDirection[0] = 1;
        pDirection[1] = 1;

        pInclude[0] = 0;
        pInclude[1] = 0;

        pOutHigh[0] = pHigh[0];
        pOutHigh[1] = pHigh[1];

        pOutLow[0] = pLow[0];
        pOutLow[1] = pLow[1];
    }
    else if (pHigh[1] < pHigh[0] && pLow[1] < pLow[0])
    {
        pDirection[0] = -1;
        pDirection[1] = -1;

        pInclude[0] = 0;
        pInclude[1] = 0;

        pOutHigh[0] = pHigh[0];
        pOutHigh[1] = pHigh[1];

        pOutLow[0] = pLow[0];
        pOutLow[1] = pLow[1];
    }
    else if (pHigh[1] <= pHigh[0] && pLow[1] >= pLow[0])
    {
        pDirection[0] = -1;
        pDirection[1] = -1;

        pInclude[0] = 0;
        pInclude[1] = 1;

        float hign = pHigh[1];
        float low = pLow[0];

        pOutHigh[0] = hign;
        pOutHigh[1] = hign;

        pOutLow[0] = low;
        pOutLow[1] = low;
    }
    else
    {
        pDirection[0] = 1;
        pDirection[1] = 1;

        pInclude[0] = 0;
        pInclude[1] = 1;

        float hign = pHigh[1];
        float low = pLow[0];

        pOutHigh[0] = hign;
        pOutHigh[1] = hign;

        pOutLow[0] = low;
        pOutLow[1] = low;
    }

    for (int i = 2; i < nCount; i++)
    {
        if (pHigh[i] > pOutHigh[i-1] && pLow[i] > pOutLow[i-1])
        {
            pDirection[i] = 1;
            pInclude[i] = 0;
            pOutHigh[i] = pHigh[i];
            pOutLow[i] = pLow[i];
        }
        else if (pHigh[i] < pOutHigh[i-1] && pLow[i] < pOutLow[i-1])
        {
            pDirection[i] = -1;
            pInclude[i] = 0;
            pOutHigh[i] = pHigh[i];
            pOutLow[i] = pLow[i];
        }
        else if (pHigh[i] <= pOutHigh[i-1] && pLow[i] >= pOutLow[i-1])
        {
            pDirection[i] = pDirection[i-1];
            pInclude[i] = 1;
            if (pDirection[i-1] == 1)
            {
                float high = pOutHigh[i-1];
                float low = pLow[i];
                int j = i;
                while (j >=0)
                {
                    pOutHigh[j] = high;
                    pOutLow[j] = low;
                    if (pInclude[j] == 0)
                    {
                        break;
                    }
                    j--;
                }
            }
            else
            {
                float high = pHigh[i];
                float low = pOutLow[i-1];
                int j = i;
                while (j >=0)
                {
                    pOutHigh[j] = high;
                    pOutLow[j] = low;
                    if (pInclude[j] == 0)
                    {
                        break;
                    }
                    j--;
                }
            }
        }
        else
        {
            pDirection[i] = pDirection[i-1];
            pInclude[i] = 1;
            if (pDirection[i-1] == 1)
            {
                float high = pHigh[i];
                float low = pOutLow[i-1];
                int j = i;
                while (j >=0)
                {
                    pOutHigh[j] = high;
                    pOutLow[j] = low;
                    if (pInclude[j] == 0)
                    {
                        break;
                    }
                    j--;
                }
            }
            else
            {
                float high = pOutHigh[i-1];
                float low = pLow[i];
                int j = i;
                while (j >=0)
                {
                    pOutHigh[j] = high;
                    pOutLow[j] = low;
                    if (pInclude[j] == 0)
                    {
                        break;
                    }
                    j--;
                }
            }
        }
    }
}
