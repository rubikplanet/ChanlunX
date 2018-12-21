#include "BaoHan.h"

void BaoHan(int nCount, float *pDirection, float *pOutHigh, float *pOutLow,
		float *pInclude, float *pHigh, float *pLow) {
	// 先判断第1根和第2根两根K线是什么状态
	// 排列可能有4种情况
	if (pHigh[1] > pHigh[0] && pLow[1] > pLow[0]) {
		// 前后两根K线没有包含关系
		// 且K线上升
		pDirection[0] = 1;
		pDirection[1] = 1;

		pInclude[0] = 0;
		pInclude[1] = 0;

		pOutHigh[0] = pHigh[0];
		pOutHigh[1] = pHigh[1];

		pOutLow[0] = pLow[0];
		pOutLow[1] = pLow[1];
	} else if (pHigh[1] < pHigh[0] && pLow[1] < pLow[0]) {
		// 前后两根K线没有包含关系
		// 且下降
		pDirection[0] = -1;
		pDirection[1] = -1;

		pInclude[0] = 0;
		pInclude[1] = 0;

		pOutHigh[0] = pHigh[0];
		pOutHigh[1] = pHigh[1];

		pOutLow[0] = pLow[0];
		pOutLow[1] = pLow[1];
	} else if (pHigh[1] <= pHigh[0] && pLow[1] >= pLow[0]) {
		// 后面K线被前面K线包含
		// 初始化时我们把这种情况定义为K先下降
		// 而且第2根K先设为包含状态
		// 被前面K线包含或者把前面K线包含，我们都把这个K线设置为包含状态
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
	} else {
		// 后面K线包含了前面的K线
		// 这种状态我们是认为K线是上升的
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
	// 从第3根K线开始，我们就依次按课文标准处理
	for (int i = 2; i < nCount; i++) {
		if (pHigh[i] > pOutHigh[i - 1] && pLow[i] > pOutLow[i - 1]) {
			pDirection[i] = 1;
			pInclude[i] = 0;
			pOutHigh[i] = pHigh[i];
			pOutLow[i] = pLow[i];
		} else if (pHigh[i] < pOutHigh[i - 1] && pLow[i] < pOutLow[i - 1]) {
			pDirection[i] = -1;
			pInclude[i] = 0;
			pOutHigh[i] = pHigh[i];
			pOutLow[i] = pLow[i];
		} else if (pHigh[i] <= pOutHigh[i - 1] && pLow[i] >= pOutLow[i - 1]) {
			pDirection[i] = pDirection[i - 1];
			pInclude[i] = 1;
			if (pDirection[i - 1] == 1) {
				float high = pOutHigh[i - 1];
				float low = pLow[i];
				int j = i;
				while (j >= 0) {
					pOutHigh[j] = high;
					pOutLow[j] = low;
					if (pInclude[j] == 0) {
						break;
					}
					j--;
				}
			} else {
				float high = pHigh[i];
				float low = pOutLow[i - 1];
				int j = i;
				while (j >= 0) {
					pOutHigh[j] = high;
					pOutLow[j] = low;
					if (pInclude[j] == 0) {
						break;
					}
					j--;
				}
			}
		} else {
			pDirection[i] = pDirection[i - 1];
			pInclude[i] = 1;
			if (pDirection[i - 1] == 1) {
				float high = pHigh[i];
				float low = pOutLow[i - 1];
				int j = i;
				while (j >= 0) {
					pOutHigh[j] = high;
					pOutLow[j] = low;
					if (pInclude[j] == 0) {
						break;
					}
					j--;
				}
			} else {
				float high = pOutHigh[i - 1];
				float low = pLow[i];
				int j = i;
				while (j >= 0) {
					pOutHigh[j] = high;
					pOutLow[j] = low;
					if (pInclude[j] == 0) {
						break;
					}
					j--;
				}
			}
		}
	}
}
