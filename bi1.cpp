#include <iostream>
#include <fstream>
#include "Bi1.h"
#include "BaoHanChuLi.h"
#include "FenXingChuLi.h"

using namespace std;

void Bi3(int nCount, float *pOut, float *pHigh, float *pLow, float *pIn) {
    BaoHanChuLi baohanChuli;
    FenXingChuLi fenXingChuLi;
    unsigned int count;
    FenXing fx;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = NORMAL;
    }

    fenXingChuLi.handle(baohanChuli.kxianList);
    count = fenXingChuLi.fenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        fx = fenXingChuLi.fenXingList[i];
        switch (fx.type) {
            case TOP:
                pOut[fx.middle_position] = TOP;
                break;

            case VERIFY_TOP:
                pOut[fx.free_position] = VERIFY_TOP;
                break;

            case FAILURE_TOP:
                pOut[fx.free_position] = FAILURE_TOP;
                break;

            case BOTTOM:
                pOut[fx.middle_position] = BOTTOM;
                break;

            case VERIFY_BOTTOM:
                pOut[fx.free_position] = VERIFY_BOTTOM;
                break;

            case FAILURE_BOTTOM:
                pOut[fx.free_position] = FAILURE_BOTTOM;
                break;
        }

    }

}
