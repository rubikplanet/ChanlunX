#ifndef __CHANLUNZB_H__
#define __CHANLUNZB_H__

#include <windows.h>

#pragma pack(push,1)

// 函数(数据个数,输出,输入a,输入b,输入c)
typedef void(*pPluginFUNC)(int nCount, float *pOut, float *a, float *b, float *c);

typedef struct tagPluginTCalcFuncInfo
{
    unsigned short nFuncMark; // 函数编号
    pPluginFUNC    pCallFunc; // 函数地址
} PluginTCalcFuncInfo;

/********************************************************************/
//******************************大智慧******************************//
/********************************************************************/
/*
///////////////////////////////////////////////////////////////////////////
大智慧扩展函数规范V1.10
1.本规范适用于大智慧1.10标准版和专业版公式系统.
2.扩展函数用于实现系统函数不能实现的特殊算法.
3.扩展函数用windows 32位动态连接库实现,建议使用Microsoft Visual C++编程.
4.调用时在公式编辑器中写"动态库名称@函数名称"(参数表)即可,例如下面函数可以写为"DZHFUNC@MYCMALOSE"(5)
5.动态连接库名称和函数名称可以自己定义.
6.使用时可以将动态库拷贝到大智慧目录下使用.
*/
///////////////////////////////////////////////////////////////////////////
//分析周期
enum DATA_TYPE
{
    TICK_DATA = 2,								//分笔成交
    MIN1_DATA,									//1分钟线
    MIN5_DATA,									//5分钟线
    MIN15_DATA,									//15分钟线
    MIN30_DATA,									//30分钟线
    MIN60_DATA,									//60分钟线
    DAY_DATA,									//日线
    WEEK_DATA,									//周线
    MONTH_DATA,									//月线
    MULTI_DATA									//多日线
};

///////////////////////////////////////////////////////////////////////////
//基本数据
typedef struct tagSTKDATA
{
    long	m_time;							    //时间,UCT
    float	m_fOpen;							//开盘
    float	m_fHigh;							//最高
    float	m_fLow;								//最低
    float	m_fClose;							//收盘
    float	m_fVolume;							//成交量
    float	m_fAmount;							//成交额
    WORD	m_wAdvance;							//上涨家数(仅大盘有效)
    WORD	m_wDecline;							//下跌家数(仅大盘有效)
} STKDATA;

////////////////////////////////////////////////////////////////////////////
//扩展数据,用于描述分笔成交数据的买卖盘
typedef union tagSTKDATAEx
{
    struct
    {
        float m_fBuyPrice[3];					//买1--买3价
        float m_fBuyVol[3];						//买1--买3量
        float m_fSellPrice[3];					//卖1--卖3价
        float m_fSellVol[3];					//卖1--卖3量
    };
    float m_fDataEx[12];						//保留
} STKDATAEx;

/////////////////////////////////////////////////////////////////////////////
//函数数据结构
typedef struct tagCALCINFO
{
    const DWORD			m_dwSize;				//结构大小
    const DWORD			m_dwVersion;			//调用软件版本(V2.10 : 0x210)
    const DWORD			m_dwSerial;				//调用软件序列号
    const char*			m_strStkLabel;			//股票代码
    const BOOL			m_bIndex;				//大盘

    const int			m_nNumData;				//数据数量(pData,pDataEx,pResultBuf数据数量)
    const STKDATA*		m_pData;				//常规数据,注意:当m_nNumData==0时可能为 NULL
    const STKDATAEx*	m_pDataEx;				//扩展数据,分笔成交买卖盘,注意:可能为 NULL

    const int			m_nParam1Start;			//参数1有效位置
    const float*		m_pfParam1;				//调用参数1
    const float*		m_pfParam2;				//调用参数2
    const float*		m_pfParam3;				//调用参数3
    const float*		m_pfParam4;				//调用参数3

    float*				m_pResultBuf;			//结果缓冲区
    const DATA_TYPE		m_dataType;				//数据类型
    const float*		m_pfFinData;			//财务数据
} CALCINFO;

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************/
//******************************通达信******************************//
/********************************************************************/
__declspec(dllexport) BOOL RegisterTdxFunc(PluginTCalcFuncInfo **pInfo);
/********************************************************************/
//************************交易师 大智慧******************************//
/********************************************************************/
__declspec(dllexport) int WINAPI RUNMODE();
__declspec(dllexport) int WINAPI KXFX(CALCINFO* pData); // K线方向
__declspec(dllexport) int WINAPI KXBH(CALCINFO* pData); // K线包含
__declspec(dllexport) int WINAPI KXBH2VAR(CALCINFO* pData); // K线包含处理
__declspec(dllexport) int WINAPI KXBHG(CALCINFO* pData); // K线包含处理后的高
__declspec(dllexport) int WINAPI KXBHD(CALCINFO* pData); // K线包含处理后的低
__declspec(dllexport) int WINAPI BI(CALCINFO* pData); // 笔端点
__declspec(dllexport) int WINAPI DUANVAR(CALCINFO* pData); // 段端点
__declspec(dllexport) int WINAPI ZSZGVAR(CALCINFO* pData); // 中枢高
__declspec(dllexport) int WINAPI ZSZDVAR(CALCINFO* pData); // 中枢低
__declspec(dllexport) int WINAPI ZSSEVAR(CALCINFO* pData); // 中枢开始结束
__declspec(dllexport) int WINAPI ZSFXVAR(CALCINFO* pData); // 中枢方向

#ifdef __cplusplus
};
#endif

#pragma pack(pop)
#endif
