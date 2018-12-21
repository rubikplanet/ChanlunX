#include "BaoHan.h"
#include "Bi.h"

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

void Bi0(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow) {
	float *pDirection = new float[nCount];
	float *pOutHigh = new float[nCount];
	float *pOutLow = new float[nCount];
	float *pInclude = new float[nCount];

	BaoHan(nCount, pDirection, pOutHigh, pOutLow, pInclude, pHigh, pLow);

	pOut[0] = -1; // -1表示笔底，1表示笔顶，初始化是第一根K线认为是一个底。
	int nState = -1; // -1表示向下笔中，1表示向上笔中，初始化时候认为是向下笔的延续中
	int nLastD = 0; // 底位置
	int nLastG = -1; // 顶位置，初始化没有顶
	int kCountDown = 5; // 计数K线数量，初始值假设已经向下笔第五根K线
	int kCountUp = 1;

	for (int i = 1; i < nCount; i++) {
		if (nState == -1) {
			// 向下笔中遇到K线出新低，底就要移到新低这里，这里不需要管K线的方向。
			if (pLow[i] < pLow[nLastD]) {
				pOut[nLastD] = 0;
				nLastD = i;
				pOut[nLastD] = -1;
				kCountUp = 1;
			}
			// 向下笔中遇到K线不出新低。
			else {
				if (pInclude[i] == 0) {
					if (pDirection[i] == 1) {
						kCountUp++;
					} else {
						if (pOutHigh[i] > RangeHign(pOutHigh, nLastD)) {
							kCountUp++;
						} else {
							kCountUp = 1;
						}
					}
				}
				if ((kCountUp >= 5
						|| IsStrongMove(i, nState, nLastD, nLastG, pHigh, pLow,
								pOut)
						|| IsReverseJump(i, nState, nLastD, nLastG, pHigh, pLow))
						&& LastIsMax(pHigh, nLastD, i)) {
					nState = 1;
					nLastG = i;
					pOut[nLastG] = 1;
					kCountDown = 1;
				}
			}
		} else if (nState == 1) {
			// 向上笔中遇到K线出新高，顶就要移到新高这里，这里不需要管K线的方向。
			if (pHigh[i] > pHigh[nLastG]) {
				pOut[nLastG] = 0;
				nLastG = i;
				pOut[nLastG] = 1;
				kCountDown = 1;
			}
			// 向上笔中遇到K线不出新高。
			else {
				if (pInclude[i] == 0) {
					if (pDirection[i] == -1) {
						kCountDown++;
					} else {
						if (pOutLow[i] < RangeLow(pOutLow, nLastG)) {
							kCountDown++;
						} else {
							kCountDown = 1;
						}
					}
				}
				if ((kCountDown >= 5
						|| IsStrongMove(i, nState, nLastD, nLastG, pHigh, pLow,
								pOut)
						|| IsReverseJump(i, nState, nLastD, nLastG, pHigh, pLow))
						&& LastIsMin(pLow, nLastG, i)) {
					// 转向到向下笔状态
					nState = -1;
					nLastD = i;
					pOut[nLastD] = -1;
					kCountUp = 1;
				}
			}

		}

		// 向下笔中有高点大于前高，前高要调整。
		// 向上笔中有低点小于前低，低点要调整。
		while (pOut[i] == -1 || pOut[i] == 1) {
			if (pOut[i] == -1) {
				// 中间如果有更高的K线，前面的顶要移到更高K线那里
				int IsLastGMoved = 0;
				int nTemp = 0;
				if (nLastG >= 0) {
					nTemp = nLastG + 1;
					while (nTemp <= i) {
						if (pHigh[nTemp] > pHigh[nLastG]) {
							IsLastGMoved = 1;
							break;
						}
						nTemp++;
					}
				}
				if (IsLastGMoved == 1) {
					pOut[nLastG] = 0; // 消掉原来顶
					nLastG = nTemp;
					pOut[nLastD] = 0;
					pOut[nLastG] = 1; // 顶后移
					nLastD = GetLastDIndex(nLastG, pOut); // 底归位
					i = nLastG; // 从新的顶重新开始计算
					kCountDown = 1; // 向下K线数量归位
					nState = 1;
				} else {
					break;
				}
			} else if (pOut[i] == 1) {
				// 中间如果有更低的K线，前面的底要移到更低K线那里
				int IsLastDMoved = 0;
				int nTemp = 0;
				if (nLastD >= 0) {
					nTemp = nLastD + 1;
					while (nTemp <= i) {
						if (pLow[nTemp] < pLow[nLastD]) {
							IsLastDMoved = 1;
							break;
						}
						nTemp++;
					}
				}
				if (IsLastDMoved == 1) {
					pOut[nLastD] = 0; // 消掉原来底
					nLastD = nTemp;
					pOut[nLastG] = 0;
					pOut[nLastD] = -1; // 底后移
					nLastG = GetLastGIndex(nLastD, pOut); // 底归位
					i = nLastD; // 从新的顶重新开始计算
					kCountUp = 1; // 向上K线数量归位
					nState = -1;
				} else {
					break;
				}
			}
		}
	}

	delete[] pDirection;
	delete[] pOutHigh;
	delete[] pOutLow;
	delete[] pInclude;
}

void Bi1(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow) {
	CIniReader iniReader(".\\T0002\\dlls\\ChanlunX.ini");
	int iFenXingQuJian = iniReader.ReadInteger("PeiZhi", "FenXingQuJian", 2);

	float *pDirection = new float[nCount];
	float *pOutHigh = new float[nCount];
	float *pOutLow = new float[nCount];
	float *pInclude = new float[nCount];

	BaoHan(nCount, pDirection, pOutHigh, pOutLow, pInclude, pHigh, pLow);

	pOut[0] = -1; // -1表示笔底，1表示笔顶，初始化是第一根K线认为是一个底。
	int nState = -1; // -1表示向下笔中，1表示向上笔中，初始化时候认为是向下笔的延续中
	int nLastD = 0; // 底位置
	int nLastG = -1; // 顶位置，初始化没有顶
	int kCountDown = 5; // 计数K线数量，初始值假设已经向下笔第五根K线
	int kCountDownRaw = 5;
	int kCountUp = 1;
	int kCountUpRaw = 1;

	for (int i = 1; i < nCount; i++) {
		if (nState == -1) {
			// 向下笔中遇到K线出新低，底就要移到新低这里，这里不需要管K线的方向。
			if (pLow[i] < pLow[nLastD]) {
				pOut[nLastD] = 0;
				nLastD = i;
				pOut[nLastD] = -1;
				kCountUp = 1;
				kCountUpRaw = 1;
			}
			// 向下笔中遇到K线不出新低。
			else {
				if (pInclude[i] == 0) {
					kCountUp++;
				}
				kCountUpRaw++;
				if (((kCountUp >= 4 && kCountUpRaw >= 5)
						|| IsStrongMove(i, nState, nLastD, nLastG, pHigh, pLow,
								pOut)
						|| IsReverseJump(i, nState, nLastD, nLastG, pHigh, pLow))
						&& LastIsMax(pHigh, nLastD, i)
						&& (iFenXingQuJian == 1 ?
								pOutHigh[i] > RangeHign(pOutHigh, nLastD) :
								pOutHigh[i] > pOutHigh[nLastD])) {
					nState = 1;
					nLastG = i;
					pOut[nLastG] = 1;
					kCountDown = 1;
					kCountDownRaw = 1;
				}
			}
		} else if (nState == 1) {
			// 向上笔中遇到K线出新高，顶就要移到新高这里，这里不需要管K线的方向。
			if (pHigh[i] > pHigh[nLastG]) {
				pOut[nLastG] = 0;
				nLastG = i;
				pOut[nLastG] = 1;
				kCountDown = 1;
				kCountDownRaw = 1;
			}
			// 向上笔中遇到K线不出新高。
			else {
				if (pInclude[i] == 0) {
					kCountDown++;
				}
				kCountDownRaw++;
				if (((kCountDown >= 4 && kCountDownRaw >= 5)
						|| IsStrongMove(i, nState, nLastD, nLastG, pHigh, pLow,
								pOut)
						|| IsReverseJump(i, nState, nLastD, nLastG, pHigh, pLow))
						&& LastIsMin(pLow, nLastG, i)
						&& (iFenXingQuJian == 1 ?
								pOutLow[i] < RangeLow(pOutLow, nLastG) :
								pOutLow[i] < pOutLow[nLastG])) {
					// 转向到向下笔状态
					nState = -1;
					nLastD = i;
					pOut[nLastD] = -1;
					kCountUp = 1;
					kCountUpRaw = 1;
				}
			}

		}

		// 向下笔中有高点大于前高，前高要调整。
		// 向上笔中有低点小于前低，低点要调整。
		while (pOut[i] == -1 || pOut[i] == 1) {
			if (pOut[i] == -1) {
				// 中间如果有更高的K线，前面的顶要移到更高K线那里
				int IsLastGMoved = 0;
				int nTemp = 0;
				if (nLastG >= 0) {
					nTemp = nLastG + 1;
					while (nTemp <= i) {
						if (pHigh[nTemp] > pHigh[nLastG]) {
							IsLastGMoved = 1;
							break;
						}
						nTemp++;
					}
				}
				if (IsLastGMoved == 1) {
					pOut[nLastG] = 0; // 消掉原来顶
					nLastG = nTemp;
					pOut[nLastD] = 0;
					pOut[nLastG] = 1; // 顶后移
					nLastD = GetLastDIndex(nLastG, pOut); // 底归位
					i = nLastG; // 从新的顶重新开始计算
					kCountDown = 1; // 向下K线数量归位
					kCountDownRaw = 1;
					nState = 1;
				} else {
					break;
				}
			} else if (pOut[i] == 1) {
				// 中间如果有更低的K线，前面的底要移到更低K线那里
				int IsLastDMoved = 0;
				int nTemp = 0;
				if (nLastD >= 0) {
					nTemp = nLastD + 1;
					while (nTemp <= i) {
						if (pLow[nTemp] < pLow[nLastD]) {
							IsLastDMoved = 1;
							break;
						}
						nTemp++;
					}
				}
				if (IsLastDMoved == 1) {
					pOut[nLastD] = 0; // 消掉原来底
					nLastD = nTemp;
					pOut[nLastG] = 0;
					pOut[nLastD] = -1; // 底后移
					nLastG = GetLastGIndex(nLastD, pOut); // 底归位
					i = nLastD; // 从新的顶重新开始计算
					kCountUp = 1; // 向上K线数量归位
					kCountUpRaw = 1;
					nState = -1;
				} else {
					break;
				}
			}
		}
	}

	delete[] pDirection;
	delete[] pOutHigh;
	delete[] pOutLow;
	delete[] pInclude;
}

void Bi2(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow) {
	CIniReader iniReader(".\\T0002\\dlls\\ChanlunX.ini");
	int iFenXingQuJian = iniReader.ReadInteger("PeiZhi", "FenXingQuJian", 2);

	float *pDirection = new float[nCount];
	float *pOutHigh = new float[nCount];
	float *pOutLow = new float[nCount];
	float *pInclude = new float[nCount];

	BaoHan(nCount, pDirection, pOutHigh, pOutLow, pInclude, pHigh, pLow);

	pOut[0] = -1; // -1表示笔底，1表示笔顶，初始化是第一根K线认为是一个底。
	int nState = -1; // -1表示向下笔中，1表示向上笔中，初始化时候认为是向下笔的延续中
	int nLastD = 0; // 底位置
	int nLastG = -1; // 顶位置，初始化没有顶
	int kCountDown = 5; // 计数K线数量，初始值假设已经向下笔第五根K线
	int kCountUp = 1;

	for (int i = 1; i < nCount; i++) {
		if (nState == -1) {
			// 向下笔中遇到K线出新低，底就要移到新低这里，这里不需要管K线的方向。
			if (pLow[i] < pLow[nLastD]) {
				pOut[nLastD] = 0;
				nLastD = i;
				pOut[nLastD] = -1;
				kCountUp = 1;
			}
			// 向下笔中遇到K线不出新低。
			else {
				if (pInclude[i] == 0) {
					kCountUp++;
				}
				if ((kCountUp >= 5
						|| IsStrongMove(i, nState, nLastD, nLastG, pHigh, pLow,
								pOut)
						|| IsReverseJump(i, nState, nLastD, nLastG, pHigh, pLow))
						&& LastIsMax(pHigh, nLastD, i)
						&& (iFenXingQuJian == 1 ?
								pOutHigh[i] > RangeHign(pOutHigh, nLastD) :
								pOutHigh[i] > pOutHigh[nLastD])) {
					nState = 1;
					nLastG = i;
					pOut[nLastG] = 1;
					kCountDown = 1;
				} else {
					int d1 = GetLastDIndex(i, pOut);
					int d2 = GetLastDIndex(d1 - 1, pOut);
					int g1 = GetLastGIndex(i, pOut);
					if (d1 >= 0 && d2 >= 0 && g1 >= 0 && g1 < d1 && d2 < g1
							&& pLow[d1] > pLow[d2] && pHigh[i] > pHigh[g1]
							&& HasTempBi(nState, nCount, i, pHigh, pLow,
									pInclude, pOutHigh, pOutLow)) {
						pOut[d1] = 0;
						pOut[g1] = 0;
						nState = 1;
						nLastG = i;
						pOut[nLastG] = 1;
						kCountDown = 1;
					} else {
						pOut[i] = 0;
					}
				}
			}
		} else if (nState == 1) {
			// 向上笔中遇到K线出新高，顶就要移到新高这里，这里不需要管K线的方向。
			if (pHigh[i] > pHigh[nLastG]) {
				pOut[nLastG] = 0;
				nLastG = i;
				pOut[nLastG] = 1;
				kCountDown = 1;
			}
			// 向上笔中遇到K线不出新高。
			else {
				if (pInclude[i] == 0) {
					kCountDown++;
				}
				if ((kCountDown >= 5
						|| IsStrongMove(i, nState, nLastD, nLastG, pHigh, pLow,
								pOut)
						|| IsReverseJump(i, nState, nLastD, nLastG, pHigh, pLow))
						&& LastIsMin(pLow, nLastG, i)
						&& (iFenXingQuJian == 1 ?
								pOutLow[i] < RangeLow(pOutLow, nLastG) :
								pOutLow[i] < pOutLow[nLastG])) {
					// 转向到向下笔状态
					nState = -1;
					nLastD = i;
					pOut[nLastD] = -1;
					kCountUp = 1;
				} else {
					int g1 = GetLastGIndex(i, pOut);
					int g2 = GetLastGIndex(g1 - 1, pOut);
					int d1 = GetLastDIndex(i, pOut);
					if (g1 >= 0 && g2 >= 0 && d1 >= 0 && d1 < g1 && g2 < d1
							&& pHigh[g1] < pHigh[g2] && pLow[i] < pLow[d1]
							&& HasTempBi(nState, nCount, i, pHigh, pLow,
									pInclude, pOutHigh, pOutLow)) {
						pOut[g1] = 0;
						pOut[d1] = 0;
						nState = -1;
						nLastD = i;
						pOut[nLastD] = -1;
						kCountUp = 1;
					} else {
						pOut[i] = 0;
					}
				}
			}

		}

		// 向下笔中有高点大于前高，前高要调整。
		// 向上笔中有低点小于前低，低点要调整。
		while (pOut[i] == -1 || pOut[i] == 1) {
			if (pOut[i] == -1) {
				// 中间如果有更高的K线，前面的顶要移到更高K线那里
				int IsLastGMoved = 0;
				int nTemp = 0;
				if (nLastG >= 0) {
					nTemp = nLastG + 1;
					while (nTemp <= i) {
						if (pHigh[nTemp] > pHigh[nLastG]) {
							IsLastGMoved = 1;
							break;
						}
						nTemp++;
					}
				}
				if (IsLastGMoved == 1) {
					pOut[nLastG] = 0; // 消掉原来顶
					nLastG = nTemp;
					pOut[nLastD] = 0;
					pOut[nLastG] = 1; // 顶后移
					nLastD = GetLastDIndex(nLastG, pOut); // 底归位
					i = nLastG; // 从新的顶重新开始计算
					kCountDown = 1; // 向下K线数量归位
					nState = 1;
				} else {
					break;
				}
			} else if (pOut[i] == 1) {
				// 中间如果有更低的K线，前面的底要移到更低K线那里
				int IsLastDMoved = 0;
				int nTemp = 0;
				if (nLastD >= 0) {
					nTemp = nLastD + 1;
					while (nTemp <= i) {
						if (pLow[nTemp] < pLow[nLastD]) {
							IsLastDMoved = 1;
							break;
						}
						nTemp++;
					}
				}
				if (IsLastDMoved == 1) {
					pOut[nLastD] = 0; // 消掉原来底
					nLastD = nTemp;
					pOut[nLastG] = 0;
					pOut[nLastD] = -1; // 底后移
					nLastG = GetLastGIndex(nLastD, pOut); // 底归位
					i = nLastD; // 从新的顶重新开始计算
					kCountUp = 1; // 向上K线数量归位
					nState = -1;
				} else {
					break;
				}
			}
		}
	}

	delete[] pDirection;
	delete[] pOutHigh;
	delete[] pOutLow;
	delete[] pInclude;
}
