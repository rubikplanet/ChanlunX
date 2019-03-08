#ifndef __BAO_HAN_H__
#define __BAO_HAN_H__

// 处理Ｋ线的包含关系
// 输入是K线高（*pHigh）和K线低（*pLow）
// 输出是K线的方向（*pDirection）、处理后的Ｋ线高（*pOutHigh）、处理后的K线低（*pOutLow）、K线是否有包含（*pInclude）
void BaoHan(int nCount, float *pDirection, float *pOutHigh, float *pOutLow, float *pInclude, float *pHigh, float *pLow);

#endif
