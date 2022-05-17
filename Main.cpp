#include "Main.h"
#include <iostream>
#include <fstream>

using namespace std;

//定义DLL程序的入口函数
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
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
// 输出函数1号：输出简笔顶底端点
//=============================================================================
void Func1(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<float> out = Bi1(nCount, high, low);
    memset(pOut, 0, nCount);
    for (int i = 0; i < nCount; i++)
    {
        pOut[i] = out[i];
    }
}

//=============================================================================
// 输出函数2号：输出标准笔顶底端点
//=============================================================================
void Func2(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<float> out = Bi2(nCount, high, low);
    memset(pOut, 0, nCount);
    for (int i = 0; i < nCount; i++)
    {
        pOut[i] = out[i];
    }
}

//=============================================================================
// 输出函数3号：输出段的端点标准画法
//=============================================================================
void Func3(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    std::vector<float> bi(pIn, pIn + nCount);
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<float> out = Duan1(nCount, bi, high, low);
    memset(pOut, 0, nCount);
    for (int i = 0; i < nCount; i++)
    {
        pOut[i] = out[i];
    }
}

//=============================================================================
// 输出函数4号：输出段的端点1+1终结画法
//=============================================================================
void Func4(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    std::vector<float> bi(pIn, pIn + nCount);
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<float> out = Duan2(nCount, bi, high, low);
    memset(pOut, 0, nCount);
    for (int i = 0; i < nCount; i++)
    {
        pOut[i] = out[i];
    }
}

//=============================================================================
// 输出函数5号：中枢高点数据
//=============================================================================
void Func5(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    std::vector<float> bi(pIn, pIn + nCount);
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<Pivot> ZhongShuList = ZS(nCount, bi, high, low);
    memset(pOut, 0, nCount);
    for (size_t i = 0; i < ZhongShuList.size(); i++)
    {
        Pivot ZhongShuOne = ZhongShuList.at(i);
        for (int j = ZhongShuOne.s + 1; j <= ZhongShuOne.e - 1; j++)
        {
            pOut[j] = ZhongShuOne.zg;
        }
    }
}

//=============================================================================
// 输出函数6号：中枢低点数据
//=============================================================================
void Func6(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    std::vector<float> bi(pIn, pIn + nCount);
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<Pivot> ZhongShuList = ZS(nCount, bi, high, low);
    memset(pOut, 0, nCount);
    for (size_t i = 0; i < ZhongShuList.size(); i++)
    {
        Pivot ZhongShuOne = ZhongShuList.at(i);
        for (int j = ZhongShuOne.s + 1; j <= ZhongShuOne.e - 1; j++)
        {
            pOut[j] = ZhongShuOne.zd;
        }
    }
}

//=============================================================================
// 输出函数7号：中枢起点、终点信号
//=============================================================================
void Func7(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    std::vector<float> bi(pIn, pIn + nCount);
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<Pivot> ZhongShuList = ZS(nCount, bi, high, low);
    memset(pOut, 0, nCount);
    for (size_t i = 0; i < ZhongShuList.size(); i++)
    {
        Pivot ZhongShuOne = ZhongShuList.at(i);
        pOut[ZhongShuOne.s + 1] = 1;
        pOut[ZhongShuOne.e - 1] = 2;
    }
}

//=============================================================================
// 输出函数8号：中枢方向数据
//=============================================================================
void Func8(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    std::vector<float> bi(pIn, pIn + nCount);
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<Pivot> ZhongShuList = ZS(nCount, bi, high, low);
    memset(pOut, 0, nCount);
    for (size_t i = 0; i < ZhongShuList.size(); i++)
    {
        Pivot ZhongShuOne = ZhongShuList.at(i);
        for (int j = ZhongShuOne.s + 1; j <= ZhongShuOne.e - 1; j++)
        {
            pOut[j] = (float)ZhongShuOne.direction;
        }
    }
}

//=============================================================================
// 输出函数9号：同方向的第几个中枢
//=============================================================================
void Func9(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    std::vector<float> bi(pIn, pIn + nCount);
    std::vector<float> high(pHigh, pHigh + nCount);
    std::vector<float> low(pLow, pLow + nCount);
    std::vector<Pivot> ZhongShuList = ZS(nCount, bi, high, low);
    memset(pOut, 0, nCount);
    for (size_t i = 0; i < ZhongShuList.size(); i++)
    {
        Pivot ZhongShuOne = ZhongShuList.at(i);
        float c = 1;
        for (int j = i - 1; j >= 0; j--)
        {
            if (ZhongShuList.at(j).direction == ZhongShuList.at(i).direction)
            {
                c++;
            }
            else
            {
                break;
            }
        }
        for (int j = ZhongShuOne.s + 1; j <= ZhongShuOne.e - 1; j++)
        {
            pOut[j] = c;
        }
    }
}

static PluginTCalcFuncInfo Info[] =
    {
        {1, &Func1},
        {2, &Func2},
        {3, &Func3},
        {4, &Func4},
        {5, &Func5},
        {6, &Func6},
        {7, &Func7},
        {8, &Func8},
        {9, &Func9},
        {0, NULL}};

BOOL RegisterTdxFunc(PluginTCalcFuncInfo **pInfo)
{
    if (*pInfo == NULL)
    {
        *pInfo = Info;

        return TRUE;
    }

    return FALSE;
}
