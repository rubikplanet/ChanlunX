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

    int position_start, position_stop;
    unsigned int count = fenXingChuLi.fenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.fenXingList[i];
        position_start = fx.get_start_position();
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_start] = -3;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_start] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_start] = -1;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_start] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_start] = 3;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_start] = 2;
            break;
        case FenXingType::VERIFY_TOP:
            pOut[position_start] = 1;
            break;
        case FenXingType::FAILURE_VERIFY_TOP:
            pOut[position_start] = 4;
            break;
        }
    }
}

void Bi4(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    FenXingChuLi fenXingChuLi;
    fenXingChuLi.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    int position_start, position_stop;
    unsigned int count = fenXingChuLi.fenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.fenXingList[i];
        position_start = fx.get_start_position();
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_start] = -3;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_start] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_stop] = -1;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_stop] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_start] = 3;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_start] = 2;
            break;
        case FenXingType::VERIFY_TOP:
            pOut[position_stop] = 1;
            break;
        case FenXingType::FAILURE_VERIFY_TOP:
            pOut[position_stop] = 4;
            break;
        }
    }
}
