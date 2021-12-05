#include "Main.h"
#include <iostream>
#include <fstream>

using namespace std;

//定义DLL程序的入口函数
BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
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
// 输出函数1号：主图的分型处理
//=============================================================================
void Func1(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi3_fenxing(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数2号：附图的分型处理
//=============================================================================
void Func2(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi4_fenxing(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数3号：主图的笔处理
//=============================================================================
void Func3(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi3_bi(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数4号：附图的笔处理
//=============================================================================
void Func4(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    Bi4_bi(nCount, pOut, pIn, pHigh, pLow);
}

//=============================================================================
// 输出函数5号：主图线段
//=============================================================================
void Func5(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi3_xianduan(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数6号：附图线段
//=============================================================================
void Func6(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi4_xianduan(nCount, pOut, pHigh, pLow, pIgnore);
}

static PluginTCalcFuncInfo Info[] =
{
    { 1, &Func1},
    { 2, &Func2},
    { 3, &Func3},
    { 4, &Func4},
    { 5, &Func5},
    { 6, &Func6},
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
