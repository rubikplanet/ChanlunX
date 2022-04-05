#include "Main.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "ChanlunZb.h"

using namespace std;

//定义DLL程序的入口函数
BOOL  APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

//=============================================================================
// 输出函数1号：主动-分型
//=============================================================================
void Func1(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi3_fenxing(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数2号：被动-分型
//=============================================================================
void Func2(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi4_fenxing(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数3号：主动-笔
//=============================================================================
void Func3(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi3_bi(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数4号：被动-笔
//=============================================================================
void Func4(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    Bi4_bi(nCount, pOut, pIn, pHigh, pLow);
}

//=============================================================================
// 输出函数5号：主动-线段
//=============================================================================
void Func5(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi3_xianduan(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数6号：被动-线段
//=============================================================================
void Func6(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi4_xianduan(nCount, pOut, pHigh, pLow, pIgnore);
}

//============================================================================
// 输出函数7号：主动-走势
//============================================================================
void Func7(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi3_zoushi(nCount, pOut, pHigh, pLow, pIgnore);
}

//============================================================================
//输出函数8号：被动-走势
//============================================================================
void Func8(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi4_zoushi(nCount, pOut, pHigh, pLow, pIgnore);
}

//============================================================================
// 输出函数9号：主动-笔走势
//============================================================================
void Func9(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi3_bi_zoushi(nCount, pOut, pHigh, pLow, pIgnore);
}

//============================================================================
//输出函数8号：被动-笔走势
//============================================================================
void Func10(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi4_bi_zoushi(nCount, pOut, pHigh, pLow, pIgnore);
}

//笔中枢开始结束
void Func11(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    bi_zhongshu_start_stop(nCount, pOut, pHigh, pLow, pIgnore);
}

//笔中枢--高
void Func12(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    bi_zhongshu_high(nCount, pOut, pHigh, pLow, pIgnore);
}

//笔中枢--低
void Func13(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    bi_zhongshu_low(nCount, pOut, pHigh, pLow, pIgnore);
}
static PluginTCalcFuncInfo Info[] =
{
    { 1, &Func1},
    { 2, &Func2},
    { 3, &Func3},
    { 4, &Func4},
    { 5, &Func5},
    { 6, &Func6},
    { 7, &Func7},
    { 8, &Func8},
    { 9, &Func9},
    {10, &Func10},
    {11, &Func11},
    {12, &Func12},
    {13, &Func13},
    { 0, NULL}
};

BOOL RegisterTdxFunc(PluginTCalcFuncInfo **pInfo)
{
    if (*pInfo == NULL)
    {
        *pInfo = Info;

        return TRUE;
    }

    return FALSE;
}
