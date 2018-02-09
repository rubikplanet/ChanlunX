#ifndef __CHANLUNXG_H__
#define __CHANLUNXG_H__

#pragma pack(push,1)

#define ASK_ALL		-1

//NTime时间信息
typedef struct tag_NTime
{
    unsigned short	year;
    unsigned char	month;
    unsigned char	day;
    unsigned char	hour;
    unsigned char	minute;
    unsigned char	second;
} NTime;

//分析数据
typedef struct tag_HISDAT
{
    NTime Time;				//时间
    float Open;				//开盘价
    float High;				//最高价
    float Low;				//最低价
    float Close;			//收盘价
    union
    {
        float	Amount;		//成交金额
        DWORD	VolInStock;	//持仓量(期货有效)
    };
    float fVolume;			//成交量
    union
    {
        float Settle;		//结算价(期货有效)
        long  lYClose;		//
        struct
        {
            WORD up;		//上涨家数(指数有效)
            WORD down;		//下跌家数(指数有效)
        } zd;
    };
} HISDAT,*LPHISDAT;

//行情数据(第二版)
typedef struct tag_REPORTDAT2
{
    DWORD	ItemNum;            // 采样点数
    float   Close;              // 前收盘价
    float   Open;               // 今开盘价
    float   Max;                // 最高价
    float   Min;                // 最低价
    float   Now;                // 现价
    DWORD	RefreshNum;			// 刷新数
    DWORD   Volume;             // 总手
    DWORD   NowVol;             // 现手(总手差)
    float   Amount;             // 总成交金额
    DWORD   Inside;             // 内盘
    DWORD   Outside;            // 外盘
    float   TickDiff;           // 笔涨跌(价位差)
    BYTE	InOutFlag;			// 内外盘标志 0:Buy 1:Sell 2:None
    DWORD   CJBS;				// 成交笔数
    float	Jjjz;				// 基金净值
    union
    {
        struct  	            //个股
        {
            float	Buyp[5];	// 五个叫买价
            DWORD	Buyv[5];	// 对应五个叫买价的五个买盘
            float	Sellp[5];	// 五个叫卖价
            DWORD	Sellv[5];	// 对应五个叫卖价的五个卖盘
        } Ggpv;
        struct  	            //指数
        {
            float	LxValue;	// 领先指标
            float	Yield;		// 不含加权的指数
            long	UpHome;		// 上涨家数
            long	DownHome;	// 下跌家数
        } Zspv;
    } Other;
    char	ununsed[20];		//备用
} REPORTDAT2, *LPREPORTDAT2;


//品种基本数据
typedef struct tag_STOCKINFO
{
    char        Name[9];			// 证券名称

    short       Unit;               // 交易单位
    long		VolBase;			// 量比的基量
    short		Fz[8];				// 开收市时间(4段)
    short		InitTimer;			// 初始化时间
    short		EndTimer;			// 收盘时间
    short		nDelayMin;			// 延时分钟数

    char		bBelongHS300;		// 是否属于沪深300板块
    char		bBelongHasKQZ;		// 是否属于含可转债板块
    char		nBelongRZRQ;		// 是否属于融资融券板块

    char		bQH;				// 是否是期货品种
    char		bHKGP;				// 是否是港股品种
    short		QHVol_BaseRate;		// 期货的每手乘数
    float		MinPrice;			// 最小变动价位
    char		unused[1];			// 备用

    float       ActiveCapital;		// 流通股本
    long        J_start;			//上市日期
    short       J_addr;				//所属省份
    short       J_hy;				//所属行业
    float       J_zgb;				//总股本
    float       J_zjhhy;			//证监会行业
    float       J_oldjly;			//上年此期净利润
    float       J_oldzysy;			//上年此期营业收入
    float       J_bg;				//B股
    float       J_hg;				//H股
    float       J_mgsy2;			//季报每股收益 (财报中提供的每股收益,有争议的才填)
    float       J_zzc;				//总资产(元)
    float       J_ldzc;				//流动资产
    float       J_gdzc;				//固定资产
    float       J_wxzc;				//无形资产
    float       J_gdrs;				//股东人数
    float       J_ldfz;				//流动负债
    float       J_cqfz;				//少数股东权益
    float       J_zbgjj;			//资本公积金
    float       J_jzc;				//股东权益(就是净资产)
    float       J_yysy;				//营业收入
    float       J_yycb;				//营业成本
    float       J_yszk;				//应收帐款
    float       J_yyly;				//营业利润
    float       J_tzsy;				//投资收益
    float       J_jyxjl;			//经营现金净流量
    float       J_zxjl;				//总现金净流量
    float       J_ch;				//存货
    float       J_lyze;				//利益总额
    float       J_shly;				//税后利益
    float       J_jly;				//净利益
    float       J_wfply;			//未分配利益
    float       J_mgjzc2;			//季报每股净资产 (财报中提供的每股收益,有争议的才填)
    float		J_jyl;				//净益率%
    float		J_mgwfp;			//每股未分配
    float		J_mgsy;				//每股收益(折算成全年的)
    float		J_mggjj;			//每股公积金
    float		J_mgjzc;			//每股净资产
    float		J_gdqyb;			//股东权益比
} STOCKINFO, *LPSTOCKINFO;

//股本总股本信息
typedef struct tag_GBInfo
{
    float Zgb;
    float Ltgb;
} GBINFO, *LPGBINFO;

//股票涨跌停价格数据
typedef struct tag_TPPrice
{
    float Close;
    float TPTop;
    float TPBottom;
} TPPRICE, *LPTPPRICE;

//数据回调的类型
#define PER_MIN5		0		//5分钟数据
#define PER_MIN15		1		//15分钟数据
#define PER_MIN30		2		//30分钟数据
#define PER_HOUR		3		//1小时数据
#define PER_DAY			4		//日线数据
#define PER_WEEK		5		//周线数据
#define PER_MONTH		6		//月线数据
#define PER_MIN1		7		//1分钟数据
#define PER_MINN		8		//多分析数据(10)
#define PER_DAYN		9		//多天线数据(45)
#define PER_SEASON		10		//季线数据
#define PER_YEAR		11		//年线数据
#define PER_SEC5		12		//5秒线
#define PER_SECN		13		//多秒线(15)
#define PER_PRD_DIY0	14		//DIY周期
#define PER_PRD_DIY10	24		//DIY周期

#define REPORT_DAT2		102	//行情数据(第二版)
#define GBINFO_DAT		103	//股本信息
#define	STKINFO_DAT		105	//股票相关数据

#define TPPRICE_DAT		121		//涨跌停数据

typedef struct tag_PluginPara  	//参数信息的结构定义
{
    char  acParaName[14];		//参数的中文名称
    int   nMin;					//参数最小取值范围
    int   nMax;					//参数最大取值范围
    int   nDefault;				//系统推荐的缺省值
    int   nValue;				//用户定义的值
} PLUGINPARAM;

typedef struct tag_PlugInfo
{
    char  Name[50];				//名称与版本
    char  Dy[30];				//产地
    char  Author[30];			//设计人
    char  Descript[100];		//选股描述
    char  Period[30];			//适应周期
    char  OtherInfo[300];
    short ParamNum;				//0<=参数个数<=4
    PLUGINPARAM ParamInfo[4];	//参数信息，见上
} PLUGIN, *LPPLUGIN;

//回调函数,取数据接口
typedef long(CALLBACK * PDATAIOFUNC)(char * Code,short nSetCode,short DataType,void * pData,short nDataNum,NTime,NTime,BYTE nTQ,unsigned long);

#ifdef __cplusplus
extern "C" {
#endif
//注册回调函数
__declspec(dllexport) void  RegisterDataInterface(PDATAIOFUNC pfn);
//得到版权信息
__declspec(dllexport) void	 GetCopyRightInfo(LPPLUGIN info);
//按最近数据计算(nDataNum为ASK_ALL表示所有数据)
__declspec(dllexport) BOOL	 InputInfoThenCalc1(char * Code,short nSetCode,int Value[4],short DataType,short nDataNum,BYTE nTQ,unsigned long unused);
//选取区段计算
__declspec(dllexport) BOOL	 InputInfoThenCalc2(char * Code,short nSetCode,int Value[4],short DataType,NTime time1,NTime time2,BYTE nTQ,unsigned long unused);
#ifdef __cplusplus
};
#endif

#pragma pack(pop)
#endif
