#ifndef __BI_H__
#define __BI_H__

#include "IniReader.h"

#pragma pack(push,1)

void BaoHan(int nCount, float *pDirection, float *pOutHigh, float *pOutLow, float *pInclude, float *pHigh, float *pLow); // 包含处理
void Bi0(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow); // 优化笔
void Bi1(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow); // 简单笔
void Bi2(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow); // 严格笔

#pragma pack(pop)

#endif
