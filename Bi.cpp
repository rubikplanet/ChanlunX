#include <iostream>
#include <fstream>
#include "BaoHan.h"
#include "Bi.h"
#include "KxianChuLi.h"
#include "BiChuLi.h"

using namespace std;

// 从fromIndex到toIndex，最后一个值是最低的
int LastIsMin(float *pLow, int fromIndex, int toIndex) {
	for (int i = fromIndex; i < toIndex; i++) {
		if (pLow[i] < pLow[toIndex]) {
			return 0;
		}
	}
	return 1;
}

// 从fromIndex到toIndex，最后一个值是最高的
int LastIsMax(float *pHigh, int fromIndex, int toIndex) {
	for (int i = fromIndex; i < toIndex; i++) {
		if (pHigh[i] > pHigh[toIndex]) {
			return 0;
		}
	}
	return 1;
}

// 计算底分型中间K线区间高
float RangeHign(float *pOutHigh, int nLastD) {
	for (int i = nLastD - 1; i >= 0; i--) {
		if (pOutHigh[i] > pOutHigh[nLastD]) {
			return pOutHigh[i];
		}
	}
	return pOutHigh[0];
}

// 计算顶分型中间K线区间低
float RangeLow(float *pOutLow, int nLastG) {
	for (int i = nLastG - 1; i >= 0; i--) {
		if (pOutLow[i] < pOutLow[nLastG]) {
			return pOutLow[i];
		}
	}
	return pOutLow[0];
}

// 反向跳空
int IsReverseJump(int i, int nState, int nLastD, int nLastG, float *pHigh,
		float *pLow) {
	if (nState == 1) {
		if (pHigh[i] < pLow[nLastD]) {
			if (i > nLastG && pHigh[i] < pLow[i - 1]) {
				return 1;
			}
		}
	} else if (nState == -1) {
		if (pLow[i] > pHigh[nLastG]) {
			if (i > nLastD && pLow[i] > pHigh[i - 1]) {
				return 1;
			}
		}
	}
	return 0;
}

// 从index往前找低点坐标
int GetLastDIndex(int index, float *pSig) {
	for (int x = index; x >= 0; x--) {
		if (pSig[x] == -1) {
			return x;
		}
	}
	return -1;
}

// 从index往前找高点坐标
int GetLastGIndex(int index, float *pSig) {
	for (int x = index; x >= 0; x--) {
		if (pSig[x] == 1) {
			return x;
		}
	}
	return -1;
}

// 比较强势的上涨或者下跌也成笔
int IsStrongMove(int i, int nState, int nLastD, int nLastG, float *pHigh,
		float *pLow, float* pSig) {
	if (nState == 1) {
		if (pLow[i] < pLow[nLastD]) {
			int g1 = GetLastGIndex(i, pSig);
			int g2 = GetLastGIndex(g1 - 1, pSig);
			if (g1 >= 0 && g2 >= 0 && pHigh[g1] > pHigh[g2] && i - g1 >= 4) {
				return 1;
			}
		}
	} else if (nState == -1) {
		if (pHigh[i] > pHigh[nLastG]) {
			int d1 = GetLastDIndex(i, pSig);
			int d2 = GetLastDIndex(d1 - 1, pSig);
			if (d1 >= 0 && d2 >= 0 && pLow[d1] < pLow[d2] && i - d1 >= 4) {
				return 1;
			}
		}
	}
	return 0;
}

bool HasTempBi(int nState, int nCount, int i, float *pHigh, float *pLow,
		float *pInclude, float *pOutHigh, float *pOutLow) {
	int kCount = 1;
	if (nState == 1) // 找向上笔
	{
		for (int x = i; x < nCount; x++) {
			if (pLow[x] < pLow[i]) {
				return false;
			} else {
				if (pInclude[x] == 0) {
					kCount++;
				}
				if (kCount >= 5 && LastIsMax(pHigh, i, x)
						&& pOutHigh[x] > pOutHigh[i]) {
					return true;
				}
			}
		}
	} else if (nState == -1) // 找向下笔
			{
		for (int x = i; x < nCount; x++) {
			if (pHigh[x] > pHigh[i]) {
				return false;
			} else {
				if (pInclude[x] == 0) {
					kCount++;
				}
				if (kCount >= 5 && LastIsMin(pLow, i, x)
						&& pOutLow[x] < pOutLow[i]) {
					return true;
				}
			}
		}
	}
	return true;
}

void Bi1(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore) {
    KxianChuLi kxianChuLi;
    for (int i = 0; i < nCount; i++) {
        kxianChuLi.add(pHigh[i], pLow[i]);
    }
    for (unsigned int i = 1; i < kxianChuLi.kxianList.size(); i++)
    {
       if (kxianChuLi.kxianList.at(i-1).fangXiang != kxianChuLi.kxianList.at(i).fangXiang)
       {
           if (kxianChuLi.kxianList.at(i-1).fangXiang == 1)
           {
               pOut[kxianChuLi.kxianList.at(i-1).zhongJian] = 1;
           } else if (kxianChuLi.kxianList.at(i-1).fangXiang == -1)
           {
                pOut[kxianChuLi.kxianList.at(i-1).zhongJian] = -1;
           }
       }
    }
    if(kxianChuLi.kxianList.back().fangXiang == 1)
    {
        pOut[kxianChuLi.kxianList.back().zhongJian] = 1;
    }
    else if(kxianChuLi.kxianList.back().fangXiang == -1)
    {
        pOut[kxianChuLi.kxianList.back().zhongJian] = -1;
    }
}

void Bi2(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore) {
    KxianChuLi kxianChuLi;
    for (int i = 0; i < nCount; i++) {
        kxianChuLi.add(pHigh[i], pLow[i]);
    }
    BiChuLi biChuLi;
    biChuLi.handle(kxianChuLi.kxianList);
    for (vector<Bi>::iterator iter = biChuLi.biList.begin(); iter != biChuLi.biList.end(); iter++)
    {
        if ((*iter).fangXiang == 1)
        {
            pOut[(*iter).kxianList.back().zhongJian] = 1;
        }
        else if ((*iter).fangXiang == -1)
        {
            pOut[(*iter).kxianList.back().zhongJian] = -1;
        }
    }
}

void Bi3(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{

    float *pDirection = new float[nCount];
    float *pOutHigh = new float[nCount];
    float *pOutLow = new float[nCount];
    float *pInclude = new float[nCount];

    BaoHan(nCount, pDirection, pOutHigh, pOutLow, pInclude, pHigh, pLow);

    for (int i = 1; i < nCount; i++)
    {
        if (pDirection[i-1] == 1 && pDirection[i] == -1)
        {
            pOut[i-1] = 1;
        }
        else if (pDirection[i-1] == -1 && pDirection[i] == 1)
        {
            pOut[i-1] = -1;
        }
        else
        {
            pOut[i-1] = 0;
        }
    }
    if (pDirection[nCount-1]== 1)
    {
        pOut[nCount-1] = 1;
    }
    else if (pDirection[nCount-1]== -1)
    {
        pOut[nCount-1] = -1;
    }

    for (int i = 0; i < nCount; i++)
    {
        if (pOut[i] == 1)
        {
            int d = i;
            for (int j = i - 1; j >= 0; j--)
            {
                if (pOut[j] == -1) break;
                if (pHigh[j] > pHigh[d])
                {
                    pOut[d] = 0;
                    d = j;
                    pOut[d] = 1;
                }
            }
        }
        else if (pOut[i] == -1)
        {
            int d = i;
            for (int j = i - 1; j >= 0; j--)
            {
                if (pOut[j] == 1) break;
                if (pLow[j] < pLow[d])
                {
                    pOut[d] = 0;
                    d = j;
                    pOut[d] = -1;
                }
            }
        }
    }

    delete []pDirection;
    delete []pOutHigh;
    delete []pOutLow;
    delete []pInclude;
}
