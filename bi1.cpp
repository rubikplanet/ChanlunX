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
    unsigned int count = fenXingChuLi.keyKxianList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.keyKxianList[i];
        switch (fx.get_type()) {
            case FenXingType::VERIFY_TOP:
                pOut[fx.get_stop_position()] = 1;
                break;
            case FenXingType::VERIFY_BOTTOM:
                pOut[fx.get_stop_position()] = -1;
                break;
            case FenXingType::FAILURE_VERIFY_BOTTOM:
                pOut[fx.get_start_position()] = -2;
                break;
            case FenXingType::FAILURE_VERIFY_TOP:
                pOut[fx.get_start_position()] = 2;
        }
    }

}
