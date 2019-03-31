#include <iostream>
#include <fstream>
#include "Bi.h"
#include "KxianChuLi.h"
#include "BiChuLi.h"

using namespace std;

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
