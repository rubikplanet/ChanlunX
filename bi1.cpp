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

    unsigned int count = baohanChuli.kxianList.size();
    for (unsigned int i = 1; i < count; i++) {
        if (baohanChuli.kxianList.at(i-1).fx_type == 1) {
            pOut[i] = 1;
        } else {
            if (baohanChuli.kxianList.at(i-1).fx_type == -1) {
                pOut[i] = -1;
            } else {
                pOut[i] = 0;
            }
        }
    }
}
