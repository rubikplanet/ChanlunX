#ifndef __BI_H__
#define __BI_H__

#include "IniReader.h"

#pragma pack(push,1)

void Bi1(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow); // 简笔
void Bi2(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow); // 标准笔

#pragma pack(pop)

#endif
