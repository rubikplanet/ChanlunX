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
// 输出函数1号：输出简笔顶底端点
//=============================================================================
void Func1(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi1(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数2号：输出标准笔顶底端点
//=============================================================================
void Func2(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi2(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数3号：输出段的端点
//=============================================================================
void Func3(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    Duan(nCount, pOut, pIn, pHigh, pLow);
}

//=============================================================================
// 输出函数4号：包含处理后的K线低
//=============================================================================
void Func4(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    float *pDirection = new float[nCount];
    float *pOutHigh = new float[nCount];
    float *pOutLow = new float[nCount];
    float *pInclude = new float[nCount];

    BaoHan(nCount, pDirection, pOutHigh, pOutLow, pInclude, pHigh, pLow);

    for (int i = 0; i < nCount; i++)
    {
        pOut[i] = pOutLow[i];
    }

    delete []pDirection;
    delete []pOutHigh;
    delete []pOutLow;
    delete []pInclude;
}

//=============================================================================
// 输出函数5号：处理一下包含信号，方便通达信画线
//=============================================================================
void Func5(int nCount, float *pOut, float *pIn1, float *pIn2, float *pInclude)
{
    pOut[0] = 0;
    float n = 1;
    for (int i = 1; i < nCount; i++)
    {
        if (pInclude[i-1] == 0 && pInclude[i] == 0)
        {
            pOut[i] = 0;
        }
        else if (pInclude[i-1] == 0 && pInclude[i] == 1)
        {
            pOut[i-1] = n;
            pOut[i] = n;
        }
        else if (pInclude[i-1] == 1 && pInclude[i] == 1)
        {
            pOut[i] = n;
        }
        else if (pInclude[i-1] == 1 && pInclude[i] == 0)
        {
            pOut[i] = 0;
            if (n == 1)
            {
                n = 2;
            }
            else
            {
                n = 1;
            }

        }
    }
}

//=============================================================================
// 输出函数6号：输出笔顶底端点
//=============================================================================
void Func6(int nCount, float *pOut, float *pHigh, float *pLow, float *pIgnore)
{
    Bi1(nCount, pOut, pHigh, pLow, pIgnore);
}

//=============================================================================
// 输出函数7号：线段顶底信号
//=============================================================================
void Func7(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    CIniReader IniReader(".\\T0002\\dlls\\ChanLunX.ini");
    int iDuan = IniReader.ReadInteger("PeiZhi", "Duan", 0);
    if (iDuan == 0)
    {
        Duan0(nCount, pOut, pIn, pHigh, pLow);
    }
    else if (iDuan == 1)
    {
        Duan1(nCount, pOut, pIn, pHigh, pLow);
    }
    else
    {
        Duan0(nCount, pOut, pIn, pHigh, pLow);
    }
}

//=============================================================================
// 输出函数8号：中枢高点数据
//=============================================================================
void Func8(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
    ZhongShu ZhongShuOne;

    for (int i = 0; i < nCount; i++)
    {
        if (pIn[i] == 1)
        {
            // 遇到线段高点，推入中枢算法
            if (ZhongShuOne.PushHigh(i, pHigh[i]))
            {
                bool bValid = true;
                float fHighValue;
                int nHignIndex;
                int nLowIndex;
                int nLowIndexTemp;
                int nHighCount = 0;
                if (ZhongShuOne.nDirection == 1 && ZhongShuOne.nTerminate == -1) // 向上中枢被向下终结
                {
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == 1)
                        {
                            if (nHighCount == 0)
                            {
                                nHighCount++;
                                fHighValue = pHigh[x];
                                nHignIndex = x;
                            }
                            else
                            {
                                nHighCount++;
                                if (pHigh[x] >= fHighValue)
                                {
                                    if (nHighCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fHighValue = pHigh[x];
                                    nHignIndex = x;
                                    nLowIndex = nLowIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == -1)
                        {
                            nLowIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        ZhongShuOne.nEnd = nLowIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    i = nHignIndex - 1;
                }
                else
                {
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    // 区段内更新算得的中枢高数据
                    for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
                    {
                        pOut[j] = ZhongShuOne.fHigh;
                    }

                }

                ZhongShuOne.Reset();
            }
        }
        else if (pIn[i] == -1)
        {
            // 遇到线段低点，推入中枢算法
            if (ZhongShuOne.PushLow(i, pLow[i]))
            {
                bool bValid = true;
                float fLowValue;
                int nLowIndex;
                int nHighIndex;
                int nHighIndexTemp;
                int nLowCount = 0;
                if (ZhongShuOne.nDirection == -1 && ZhongShuOne.nTerminate == 1) // 向下中枢被向上终结
                {
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == -1)
                        {
                            if (nLowCount == 0)
                            {
                                nLowCount++;
                                fLowValue = pLow[x];
                                nLowIndex = x;
                            }
                            else
                            {
                                nLowCount++;
                                if (pLow[x] <= fLowValue)
                                {
                                    if (nLowCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fLowValue = pLow[x];
                                    nLowIndex = x;
                                    nHighIndex = nHighIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == 1)
                        {
                            nHighIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        ZhongShuOne.nEnd = nHighIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    i = nLowIndex - 1;
                }
                else
                {
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    // 区段内更新算得的中枢高数据
                    for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
                    {
                        pOut[j] = ZhongShuOne.fHigh;
                    }

                }

                ZhongShuOne.Reset();
            }
        }
    }
    if (ZhongShuOne.bValid) // 最后一个还没有被终结的中枢。
    {
        // 区段内更新算得的中枢高数据
        for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
        {
            pOut[j] = ZhongShuOne.fHigh;
        }
    }
}

//=============================================================================
// 输出函数9号：中枢低点数据
//=============================================================================
void Func9(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{

    ZhongShu ZhongShuOne;

    for (int i = 0; i < nCount; i++)
    {
        if (pIn[i] == 1)
        {
            // 遇到线段高点，推入中枢算法
            if (ZhongShuOne.PushHigh(i, pHigh[i]))
            {
                bool bValid = true;
                float fHighValue;
                int nHignIndex;
                int nLowIndex;
                int nLowIndexTemp;
                int nHighCount = 0;
                if (ZhongShuOne.nDirection == 1 && ZhongShuOne.nTerminate == -1) // 向上中枢被向下终结
                {
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == 1)
                        {
                            if (nHighCount == 0)
                            {
                                nHighCount++;
                                fHighValue = pHigh[x];
                                nHignIndex = x;
                            }
                            else
                            {
                                nHighCount++;
                                if (pHigh[x] >= fHighValue)
                                {
                                    if (nHighCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fHighValue = pHigh[x];
                                    nHignIndex = x;
                                    nLowIndex = nLowIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == -1)
                        {
                            nLowIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        ZhongShuOne.nEnd = nLowIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    i = nHignIndex - 1;
                }
                else
                {
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    // 区段内更新算得的中枢低数据
                    for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
                    {
                        pOut[j] = ZhongShuOne.fLow;
                    }

                }

                ZhongShuOne.Reset();
            }
        }
        else if (pIn[i] == -1)
        {
            // 遇到线段低点，推入中枢算法
            if (ZhongShuOne.PushLow(i, pLow[i]))
            {
                bool bValid = true;
                float fLowValue;
                int nLowIndex;
                int nHighIndex;
                int nHighIndexTemp;
                int nLowCount = 0;
                if (ZhongShuOne.nDirection == -1 && ZhongShuOne.nTerminate == 1) // 向下中枢被向上终结
                {
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == -1)
                        {
                            if (nLowCount == 0)
                            {
                                nLowCount++;
                                fLowValue = pLow[x];
                                nLowIndex = x;
                            }
                            else
                            {
                                nLowCount++;
                                if (pLow[x] <= fLowValue)
                                {
                                    if (nLowCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fLowValue = pLow[x];
                                    nLowIndex = x;
                                    nHighIndex = nHighIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == 1)
                        {
                            nHighIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        ZhongShuOne.nEnd = nHighIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    i = nLowIndex - 1;
                }
                else
                {
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    // 区段内更新算得的中枢低数据
                    for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
                    {
                        pOut[j] = ZhongShuOne.fLow;
                    }

                }

                ZhongShuOne.Reset();
            }
        }
    }
    if (ZhongShuOne.bValid)
    {
        // 区段内更新算得的中枢低数据
        for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
        {
            pOut[j] = ZhongShuOne.fLow;
        }
    }
}

//=============================================================================
// 输出函数10号：中枢起点、终点信号
//=============================================================================
void Func10(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{

    //std::ofstream fout;
    //fout.open("D:\\CHANLUN.TXT", std::ofstream::out);

    ZhongShu ZhongShuOne;

    for (int i = 0; i < nCount; i++)
    {
        if (pIn[i] == 1)
        {
            // 遇到线段高点，推入中枢算法
            if (ZhongShuOne.PushHigh(i, pHigh[i]))
            {
                //fout<<"中枢终结"<<pHigh[i]<<endl;
                bool bValid = true;
                float fHighValue;
                int nHignIndex;
                int nLowIndex;
                int nLowIndexTemp;
                int nHighCount = 0;
                if (ZhongShuOne.nDirection == 1 && ZhongShuOne.nTerminate == -1) // 向上中枢被向下终结
                {
                    //fout<<"向上中枢被向下终结"<<endl;
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == 1)
                        {
                            if (nHighCount == 0)
                            {
                                nHighCount++;
                                fHighValue = pHigh[x];
                                nHignIndex = x;
                            }
                            else
                            {
                                nHighCount++;
                                if (pHigh[x] >= fHighValue)
                                {
                                    if (nHighCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fHighValue = pHigh[x];
                                    nHignIndex = x;
                                    nLowIndex = nLowIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == -1)
                        {
                            nLowIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        //fout<<"同级别分解保留最后中枢"<<endl;
                        //fout<<"中枢结束点移到"<<pLow[nLowIndex]<<endl;
                        ZhongShuOne.nEnd = nLowIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    else
                    {
                        //fout<<"同级别分解最后中枢无效"<<endl;
                    }
                    i = nHignIndex - 1;
                }
                else
                {
                    //fout<<"向下中枢被向下终结"<<endl;
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    // 进行标记
                    pOut[ZhongShuOne.nStart + 1] = 1;
                    pOut[ZhongShuOne.nEnd - 1]   = 2;
                }
                ZhongShuOne.Reset();
            }
        }
        else if (pIn[i] == -1)
        {
            // 遇到线段低点，推入中枢算法
            if (ZhongShuOne.PushLow(i, pLow[i]))
            {
                //fout<<"中枢终结"<<pHigh[i]<<endl;
                bool bValid = true;
                float fLowValue;
                int nLowIndex;
                int nHighIndex;
                int nHighIndexTemp;
                int nLowCount = 0;
                if (ZhongShuOne.nDirection == -1 && ZhongShuOne.nTerminate == 1) // 向下中枢被向上终结
                {
                    //fout<<"向下中枢被向上终结"<<endl;
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == -1)
                        {
                            if (nLowCount == 0)
                            {
                                nLowCount++;
                                fLowValue = pLow[x];
                                nLowIndex = x;
                            }
                            else
                            {
                                nLowCount++;
                                if (pLow[x] <= fLowValue)
                                {
                                    if (nLowCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fLowValue = pLow[x];
                                    nLowIndex = x;
                                    nHighIndex = nHighIndexTemp;
                                }
                            }
                            //fout<<"低点数量"<<nLowCount<<endl;
                        }
                        else if (pIn[x] == 1)
                        {
                            nHighIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        //fout<<"同级别分解保留最后中枢"<<endl;
                        //fout<<"中枢结束点移到"<<pHigh[nHighIndex]<<endl;
                        ZhongShuOne.nEnd = nHighIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    else
                    {
                        //fout<<"同级别分解最后中枢无效"<<endl;
                    }
                    i = nLowIndex - 1;
                }
                else
                {
                    //fout<<"向上中枢被向上终结"<<endl;
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    // 进行标记
                    pOut[ZhongShuOne.nStart + 1] = 1;
                    pOut[ZhongShuOne.nEnd - 1]   = 2;

                }
                ZhongShuOne.Reset();
            }
        }
    }
    if (ZhongShuOne.bValid)
    {
        // 进行标记
        pOut[ZhongShuOne.nStart + 1] = 1;
        pOut[ZhongShuOne.nEnd - 1]   = 2;
    }
    //fout.close();
}

//=============================================================================
// 输出函数11号：中枢方向数据
//=============================================================================
void Func11(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{

    ZhongShu ZhongShuOne;

    for (int i = 0; i < nCount; i++)
    {
        if (pIn[i] == 1)
        {
            // 遇到线段高点，推入中枢算法
            if (ZhongShuOne.PushHigh(i, pHigh[i]))
            {
                bool bValid = true;
                float fHighValue;
                int nHignIndex;
                int nLowIndex;
                int nLowIndexTemp;
                int nHighCount = 0;
                if (ZhongShuOne.nDirection == 1 && ZhongShuOne.nTerminate == -1) // 向上中枢被向下终结
                {
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == 1)
                        {
                            if (nHighCount == 0)
                            {
                                nHighCount++;
                                fHighValue = pHigh[x];
                                nHignIndex = x;
                            }
                            else
                            {
                                nHighCount++;
                                if (pHigh[x] >= fHighValue)
                                {
                                    if (nHighCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fHighValue = pHigh[x];
                                    nHignIndex = x;
                                    nLowIndex = nLowIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == -1)
                        {
                            nLowIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        ZhongShuOne.nEnd = nLowIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    i = nHignIndex - 1;
                }
                else
                {
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    // 区段内更新算得的中枢方向数据
                    for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
                    {
                        pOut[j] = (float) ZhongShuOne.nDirection;
                    }

                }

                ZhongShuOne.Reset();
            }
        }
        else if (pIn[i] == -1)
        {
            // 遇到线段低点，推入中枢算法
            if (ZhongShuOne.PushLow(i, pLow[i]))
            {
                bool bValid = true;
                float fLowValue;
                int nLowIndex;
                int nHighIndex;
                int nHighIndexTemp;
                int nLowCount = 0;
                if (ZhongShuOne.nDirection == -1 && ZhongShuOne.nTerminate == 1) // 向下中枢被向上终结
                {
                    bValid = false;
                    for (int x = ZhongShuOne.nStart; x <= ZhongShuOne.nEnd; x++)
                    {
                        if (pIn[x] == -1)
                        {
                            if (nLowCount == 0)
                            {
                                nLowCount++;
                                fLowValue = pLow[x];
                                nLowIndex = x;
                            }
                            else
                            {
                                nLowCount++;
                                if (pLow[x] <= fLowValue)
                                {
                                    if (nLowCount > 2)
                                    {
                                        bValid = true;
                                    }
                                    fLowValue = pLow[x];
                                    nLowIndex = x;
                                    nHighIndex = nHighIndexTemp;
                                }
                            }
                        }
                        else if (pIn[x] == 1)
                        {
                            nHighIndexTemp = x;
                        }
                    }
                    if (bValid)
                    {
                        ZhongShuOne.nEnd = nHighIndex; // 中枢结束点移到最高点的前一个低点。
                    }
                    i = nLowIndex - 1;
                }
                else
                {
                    i = ZhongShuOne.nEnd - 1;
                }
                if (bValid)
                {
                    // 区段内更新算得的中枢方向数据
                    for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
                    {
                        pOut[j] = (float) ZhongShuOne.nDirection;
                    }

                }

                ZhongShuOne.Reset();
            }
        }
    }
    if (ZhongShuOne.bValid)
    {
        // 区段内更新算得的中枢方向数据
        for (int j = ZhongShuOne.nStart + 1; j <= ZhongShuOne.nEnd - 1; j++)
        {
            pOut[j] = (float) ZhongShuOne.nDirection;
        }
    }
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

/********************************************************************/
//************************交易师 大智慧******************************//
/********************************************************************/
__declspec(dllexport) int WINAPI RUNMODE()
{
    return 0;
}

__declspec(dllexport) int WINAPI KXFX(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
    }
    Func1(nDataLen, pData->m_pResultBuf, NULL, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    return 0;
}

__declspec(dllexport) int WINAPI KXBH(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
    }
    Func2(nDataLen, pData->m_pResultBuf, NULL, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    return 0;
}

__declspec(dllexport) int WINAPI KXBH2VAR(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pIn = new float[nDataLen];
    for (int i = 0; i < nDataLen; i++)
    {
        pIn[i] = pData->m_pfParam1[i];
    }
    Func5(nDataLen, pData->m_pResultBuf, NULL, NULL, pIn);
    delete []pIn;
    return 0;
}

__declspec(dllexport) int WINAPI KXBHG(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
    }
    Func3(nDataLen, pData->m_pResultBuf, NULL, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    return 0;
}

__declspec(dllexport) int WINAPI KXBHD(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
    }
    Func4(nDataLen, pData->m_pResultBuf, NULL, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    return 0;
}

__declspec(dllexport) int WINAPI BI(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
    }
    Func6(nDataLen, pData->m_pResultBuf, NULL, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    return 0;
}

__declspec(dllexport) int WINAPI DUANVAR(CALCINFO* pData)
{
    if(pData->m_pfParam1 && pData->m_nParam1Start >= 0)
    {
        int nDataLen = pData->m_nNumData;
        float *pHigh = new float[nDataLen];
        float *pLow = new float[nDataLen];
        float *pIn = new float[nDataLen];

        for (int i = 0; i < nDataLen; i++)
        {
            pHigh[i] = pData->m_pData[i].m_fHigh;
            pLow[i] = pData->m_pData[i].m_fLow;
            pIn[i] = pData->m_pfParam1[i];
        }

        Func7(nDataLen, pData->m_pResultBuf, pIn, pHigh, pLow);
        delete []pHigh;
        delete []pLow;
        delete []pIn;
        return 0;
    }
    return -1;
}

__declspec(dllexport) int WINAPI ZSZGVAR(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];
    float *pIn = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
        pIn[i] = pData->m_pfParam1[i];
    }
    Func8(nDataLen, pData->m_pResultBuf, pIn, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    delete []pIn;
    return 0;
}

__declspec(dllexport) int WINAPI ZSZDVAR(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];
    float *pIn = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
        pIn[i] = pData->m_pfParam1[i];
    }
    Func9(nDataLen, pData->m_pResultBuf, pIn, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    delete []pIn;
    return 0;
}

__declspec(dllexport) int WINAPI ZSSEVAR(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];
    float *pIn = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
        pIn[i] = pData->m_pfParam1[i];
    }
    Func10(nDataLen, pData->m_pResultBuf, pIn, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    delete []pIn;
    return 0;
}

__declspec(dllexport) int WINAPI ZSFXVAR(CALCINFO* pData)
{
    int nDataLen = pData->m_nNumData;
    float *pHigh = new float[nDataLen];
    float *pLow = new float[nDataLen];
    float *pIn = new float[nDataLen];

    for (int i = 0; i < nDataLen; i++)
    {
        pHigh[i] = pData->m_pData[i].m_fHigh;
        pLow[i] = pData->m_pData[i].m_fLow;
        pIn[i] = pData->m_pfParam1[i];
    }
    Func11(nDataLen, pData->m_pResultBuf, pIn, pHigh, pLow);
    delete []pHigh;
    delete []pLow;
    delete []pIn;
    return 0;
}
