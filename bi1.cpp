#include <iostream>
#include <fstream>
#include "Bi1.h"
#include "BaoHanChuLi.h"
#include "FenXingChuLi.h"

using namespace std;

void Bi3(int nCount, float *pOut, float *pHigh, float *pLow, float *pIn) {
    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    FenXingChuLi fenXingChuLi;
    fenXingChuLi.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }
    unsigned int count = fenXingChuLi.fenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.fenXingList[i];
        if (fx.failure_status == false) {
            if (fx.type == 1) {
                pOut[fx.free_position] = 1.0;
            }
            else {
                pOut[fx.free_position] = -1.0;
            }
        }
        else {
            if (fx.type == 1) {
                pOut[fx.free_position] = -1.0;
            }
            else {
                pOut[fx.free_position] = 1.0;
            }
        }
    }

}
