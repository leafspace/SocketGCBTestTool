///////////////////////////////////////////////////////////////////////////
// rpPrnData.h : 定义打印设备相关参数信息（数据结构） 
//
// Created by hzm 2010-03-19
//
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "../include/rpBasexp.h"
#include "../include/ColorInfo.h"

//#ifdef /*RP_EXT_CLASS*/
//	#undef /*RP_EXT_CLASS*/
//	#undef RP_EXT_API
//	#undef RP_EXT_DATA
//#endif
//
//#ifdef APP_RPPRNBASE
//	#define /*RP_EXT_CLASS*/       AFX_CLASS_EXPORT
//	#define RP_EXT_API         AFX_API_EXPORT
//	#define RP_EXT_DATA        AFX_DATA_EXPORT
//#else
//	#define /*RP_EXT_CLASS*/       AFX_CLASS_IMPORT
//	#define RP_EXT_API         AFX_API_IMPORT
//	#define RP_EXT_DATA        AFX_DATA_IMPORT
//#endif

#define KY_MAX_GROUP				8									//系统最大支持的喷头组数
#define KY_MAX_CHANNELS				8									//系统最大支持的颜色通道数
#define KY_MAX_COLORS				(KY_MAX_GROUP*KY_MAX_CHANNELS)		//系统最大支持的喷头总数

#define _MAX_PRINTER_NAME_			100			//系统最大支持的打印机名称长度
#define _MAX_SPEED_INFO_			20			//系统最大支持的打印速度命名长度
#define _MAX_SPEED_LEVEL_			3			//系统最大支持的打印速度级数分类
#define _MAX_SPEED_LIMIT_			10			//系统最大支持的打印速度级数值

#define _MAX_XRES_LEVEL_			10			//系统最大支持的X向精度类型数
#define _MAX_YPASS_LEVEL_			20			//系统最大支持的Y向Pass类型数

#ifndef PPDWORD
typedef LPDWORD			*PPDWORD;			//DWORD data[m][n][k]
typedef LPINT			*PPINT;				//int	data[m][n][k]
typedef PUINT			*PPUINT;			//UINT	data[m][n][k]
#endif

#ifndef PZPSTR
#ifndef PSTR
typedef __nullterminated CHAR *NPSTR, *LPSTR, *PSTR;
#endif
typedef __nullterminated PSTR *PZPSTR;		//char	data[m][n][k]
#endif

#pragma pack(push)
#pragma pack(1)


//喷头参数
#define _PRINTHEAD_ROW_MAX_					16				//单个喷头中最大行列数（插值）
struct /*RP_EXT_CLASS*/ PRINTHEAD
{
	//喷头类型
	enum PRINTHEADTYPE
	{
		printhead_none = 0x0000,		//初始化状态（无效喷头类型）
		printhead_spectra_galxy_80 = 0x0001,		//Spectra Galxy 80
		printhead_spectra_galxy_30 = 0x0002,		//Spectra Galxy 30, default

		printhead_spectra_polaris_pq512 = 0x0003,		//Polaris PQ-512 Printhead

		printhead_spectra_StarFire_sg1024 = 0x0004,		//StarFire SG-1024/M-C printhead

		printhead_Unknown = 0xFFFF		//未知喷头类型
	};

	WORD	nType;					//喷头类型			: PRINTHEADTYPE

	WORD	nNozzleSize;			//单个喷头的喷嘴数	: 256
	double	dNozzleDistance;		//喷嘴间距			: 0.254mm（100 dpi）
	BYTE	bRowCount;				//每个喷头的喷嘴行数 : 1
									//		每行喷嘴数 = nNozzleSize / bRowCount

	double	dDistance_Hor[_PRINTHEAD_ROW_MAX_];		//每个喷头中各喷嘴行间的X向偏移（喷嘴插值序号 [0,1,2,...]），mm
													//允许有负值，但不允许存在相同值，有且仅有一个值为0，表示起始喷嘴（No.0）
													//当 bRowCount<=1时，忽略本参数项


	UINT	nColorLevel;			//灰度级别，相应值表示灰度级数，例 0x08 表示支持 8 级灰度打印
									//		其中 0,1,2 均表示不支持灰度打印，仅支持单色打印
	UINT	nNozzleDropSizeMin;		//最小墨滴			: 80pl，……（注：最大墨滴为最小墨滴 * （灰度级数-1））

public:
	//读入参数
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();
};

//喷头组参数（多组喷头，即 1*m, 2*m, 3*m, ..., n*m 模式）
struct /*RP_EXT_CLASS*/ PRINTHEADGROUP
{
	PRINTHEAD	printhead;									//喷头参数

	UINT		nHeadNumber;								//最大喷头数，<= KY_MAX_COLORS

	UINT		nGroupCount;								//喷头组数，1, 2, 3, ...，<= KY_MAX_GROUP
															//每组喷头可使用颜色数 nColor = nHeadNumber/nGroupCount (<=KY_MAX_CHANNELS)
															//本参数约束后续参数中颜色数(KY_MAX_COLORS)相关数组
	//当有多组喷头（相同色）时，定义同色喷头的组合方式
	enum COMBINEWAY
	{
		combineway_normal = 0,		//仅1组喷头(nGroupCount==1)

		//串行连接，组合后实际输出的喷嘴间距不变：dNozzleDistance
		combineway_series_connection_normal = 0x01,		//串行连接-无重叠喷嘴，相同色喷头Y向间距=(nNozzleSize*dNozzleDistance) * N（倍）
		combineway_series_connection_overlapping_1 = 0x02,		//串行连接-重叠1个喷嘴，相同色喷头Y向间距=((nNozzleSize-1)*dNozzleDistance) * N（倍），实际有效喷嘴数=nGroupCount*(nZozzleSize-1)+1
		combineway_series_connection_overlapping_2 = 0x03,		//串行连接-重叠2个喷嘴，相同色喷头Y向间距=((nNozzleSize-2)*dNozzleDistance) * N（倍），实际有效喷嘴数=nGroupCount*(nZozzleSize-2)+2

		//并行连接，组合后实际输出的喷嘴间距插值：dNozzleDistance / nGroupCount
		//并行连接-同色喷头插值提升一次输出精度，相同色喷头Y向间距=(m+1/nGroupCount)*dNozzleDistance（m为喷嘴个数，适当提高此值可改善喷头多Pass时的误差）
		combineway_parallel_connection_normal = 0x10,		//并行连接-普通模式(m==0)

	};
	BYTE	bCombineWay;										//同色喷头组合方式，注：仅在 nGroupCount > 1 时有效

	//对于同一款设备（确定机型），以下参数可通过应用程序进行更改（需配合机械）
	UINT	nFilterIndex[KY_MAX_COLORS];					//喷头组归类，指定每组中的喷头序号，不得有重复
															//	nHeadIndex = nFilterIndex[nGroupIndex*nColor+nIndex]，其中表示第nGroupIndex组中的第nIndex上喷头，nColor为一组的喷头个数（颜色通道）

	DWORD	nColorTable[KY_MAX_CHANNELS];					//指定每组喷头(nColor)的颜料，ky_printer_color（单色）
	UINT	nDataFilter[KY_MAX_CHANNELS];					//指定喷头对应的喷头板通道序号(nColor)

public:
	//读入参数
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;

	//喷头组归类信息是否有效
	//  pFilterIndex:[in]喷头组归类信息，同 nFilterIndex 定义
	//  nHead		:[in]总的喷头数
	//注：pFilterIndex 中的每个值不能相同
	static bool IsValidFilter(const UINT *pFilterIndex, int nHead);
	//每组喷头的颜料信息是否有效
	//  pColorTable :[in]喷头颜料信息
	//  nColor      :[in]颜色数
	//注：喷头颜料信息值不能相同
	static bool IsValidColorTable(const DWORD *pColorTable, int nColor);
	//喷头对应喷头板通道信息是否有效
	//	pDataFilter	:[in]喷头对应的喷头板通道信息，同 nDataFilter 定义
	//	nChannels	:[in]有效通道总数
	//注：通道序号值不能相同
	static bool IsValidDataFilter(const UINT *pDataFilter, int nChannels);

	//得喷嘴叠加个数
	//返回值：
	//	>= 0 	:喷嘴叠加个数
	//	< 0 	:无效组合方式
	static int GetOverlappingNozzles(BYTE bCombineWay);
	//得实际有效的喷嘴数
	//	nNozzles	:[in]单个喷头喷嘴数
	//	nGroupCount	:[in]喷头组数（同色）
	//	bCombineWay	:[in]喷头拼接方式
	static int GetTotalNozzles(int nNozzles, int nGroupCount, BYTE bCombineWay);

	//喷头组参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();
};
//从组号、颜色号计算实际的nFilterIndex序号
//	grpindex	:组号，< KY_MAX_GROUP
//	clrindex	:颜色号，< color
//	color		:每组颜色数， <= KY_MAX_CHANNELS
//	filterindex	:nFilterIndex序号， <= KY_MAX_COLORS，nFilterIndex[filterindex]即相应的喷头序号
#define TransToFilterIndex(grpindex,clrindex,color)					((grpindex)*(color) + (clrindex))
//从nFilterIndex序号计算组号、颜色号
#define TransFromFilterIndex(grpindex,clrindex,color,filterindex)	{(grpindex)=(filterindex)/(color); (clrindex)=(filterindex) % (color);}
//得有效的颜色通道数（<= KY_MAX_CHANNELS）
//	headnum		:喷头总数，<= KY_MAX_COLORS
//	grpcount	:喷头组数，<= KY_MAX_GROUP
#define GetValidColorChannels(headnum,grpcount)						((headnum)/(grpcount))

//控制参数
//散喷
struct /*RP_EXT_CLASS*/ SPRAYPARAM
{
	BYTE bSprayForIdleEanble;		//是否允许空闲时散喷
	BYTE bSprayForPrintingEanble;	//是否允许打印中散喷

	WORD wSprayFrequencyForIdel;	//空闲时散喷频率，Hz
	WORD wSprayPassForPrint;		//指定打印中散喷参数：间隔多少个 Pass 后进行散喷

public:
	//读入参数
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();
};
//打印中自动喷头清洗
//	流程：喷头移到清洗槽-->压墨-->延迟-->清洗n遍-->完成清洗
struct /*RP_EXT_CLASS*/ CLEANHEADPARAM
{
	BYTE	bEnable;				//是否允许打印中进行自动喷头清洗
	WORD	wPass;					//指定打印中喷头清洗参数：间隔多少个 Pass 后进行散喷
	WORD	wPurgeTime;				//指定自动喷头清洗时的压墨时长，毫秒
	WORD	wPurgeDelay;			//指定压墨结束后的延迟，毫秒
	BYTE	bCleanTime;				//指定自动喷头清洗时清洗（刮刀）次数

public:
	//读入参数
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();
};

//打印设备参数（运动/打印相关）
//	公共配置参数
struct /*RP_EXT_CLASS*/ MOTOPARAM_GENERAL
{
	WORD	X_MAX_WIDTH;				//x方向最大打印宽度(mm)
	WORD	PRINT_MAX_FREQ;				//打印最大频率(Hz)
	WORD	X_MAX_SPEED;				//x方向最大速度(mm/s)
	Float32 X_COEF;						//x方向速度较正系数

public:
	//读入参数
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();
};
//  X向配置参数
struct /*RP_EXT_CLASS*/ MOTOPARAM_X
{
	WORD	SERVE_PER_PULSE;			//电子齿轮比（若加减速机，则为经减速机后的比值）
	WORD	X_P_DIS;					//x方向打印时走加速距离(mm)
	WORD	X_S_DIS;					//x方向打印时走减速距离(mm)
	WORD	IO_SPACE;					//光栅IO间距(mm) IO_SPACE
	WORD	IO_PULSE;					//光栅IO间脉冲数 ，IO_PULSE
	DWORD	X_DELAY_S;					//小车慢速移动延时时间（us）,值越大，小车移动越慢
	DWORD	X_DELAY_F;					//小车快速移动延时时间（us），值越大，小车移动越慢
	Float32 SERVE_PER_CYCLE;			//伺服电机转一周小车走的距离（mm）
public:
	//读入参数
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();
};
//  Y向配置参数
struct /*RP_EXT_CLASS*/ MOTOPARAM_Y
{
	WORD	Y_PER_PULSE;				//Y向电子齿轮比（若加减速机，则为经减速机后的比值）
	WORD	Y_MAX_SPEED;				//Y方向最大速度（mm/s）
	DWORD	Y_DELAY;					//打印时匀速延时时间（us）
	DWORD	Y_DELAY_S;					//Y向慢速移动延时时间(us) 
	DWORD	Y_DELAY_F;					//Y向快速移动延时时间(us) 
	Float32	Y_WHORL;					//Y方向螺矩(mm) 
public:
	//读入参数
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();
};

//  Z向配置参数
struct /*RP_EXT_CLASS*/ MOTOPARAM_Z
{
	DWORD	Z_DELAY;					//Z方向移动延时（us）
	Float32	Z_WHORL;					//Z方向螺矩(mm) 
	Float32 Z_POSITION_MIN;				//Z方向最小距离(pulse) 
	Float32 Z_POSITION_MAX;				//Z方向最大距离(pulse) 
public:
	//读入参数
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();
};

//打印机参数
#define _FLATMACHINE_HEIGHT_LIMIT_		50				//平板机型最小有效打印长度值，mm
struct /*RP_EXT_CLASS*/ PRINTER
{
	//打印机类型
	enum PRINTERTYPE
	{
		printer_none = 0x0000,		//初始化状态（无效打印机类型）

		printer_RainBow_08SG = 0x0001,		//8喷头，1*n单组，Spectra Galxy 喷头，PCI9052，Com

		printer_RainBow_64MG = 0x0002,		//m*8 组模式(m=1,2,4,8)，最大8*8，Spectra Galxy喷头，PCI9052， Com

		printer_RainBow_08SS = 0x0003,		//8喷头，1*n单组，Spectra SG-1024/M-C 喷头，PCI9052，Com

		printer_RainBow_Virtual = 0xFFFD,		//虚拟打印机，仅用于软件模拟调试用

		printer_RainBow_NewDebug = 0xFFFE,		//新增的调试用打印机类型（未确定标准类型）

		printer_Unknown = 0xFFFF		//未知打印机类型
	};

	WORD			nType;							//打印机类型: PRINTERTYPE
	char			szName[_MAX_PRINTER_NAME_ + 1];	//打印机名称，如“RainBow_1816DG”

	double			dMinStartX;						//设备可打印的最小起始位置（mm，X向）
	double			dMinStartY;						//设备可打印的最小起始位置（mm，Y向），仅平板机使用，导带机固定为0
	double			dMaxWidth;						//设备可打印的最大宽度（mm）
	double			dMaxHeight;						//设备可打印的最大高度（mm），<= 0 时表示无限制（仅导带机）

	PRINTHEADGROUP	printgroup;						//喷头组参数

	//颜色模式
	UINT	nInkModeLevel[INKMODE_MAX + 1];		//指定当前喷头组支持的颜色模式列表，以 INK_NONE 结束
	int		nInkModeCount;						//有效的颜色模式数

	char	szSpeedLevel[_MAX_SPEED_LEVEL_ + 1][_MAX_SPEED_INFO_];	//支持的打印速度级数对应的速度命名，以空字符串标识结束
	BYTE	bSpeedValue[_MAX_SPEED_LEVEL_ + 1];						//实际的打印速度级数值，(0, _MAX_SPEED_LIMIT_]
	int		nSpeedCount;						//有效的打印速度级数

	UINT	nXResLevel[_MAX_XRES_LEVEL_ + 1];		//支持的X方向精度值，dpi，300，400，600，...，以 0 标识结束
	int		nXResCount;							//有效的X方向精度级数

	UINT	nYPassLevel[_MAX_YPASS_LEVEL_ + 1];	//支持的Y方向Pass数，1，2，3，4，...，以 0 标识结束
	enum PASSWAY
	{
		passway_normal = 0,		//普通模式
		passway_plus,				//Pass+ 模式
		passway_count
	};
	BYTE	bYPassWay[_MAX_YPASS_LEVEL_ + 1];		//支持的Y方向Pass数类别
	int		nYPassCount;						//有效的Y方向Pass数级数

	BYTE	bSprayEnable;						//是否支持散喷操作（硬件决定，不得更改）
	BYTE	bCleanHeadEnable;					//是否支持自动喷头清洗操作

	//对于同一款设备（确定机型），以下参数可通过应用程序进行更改（需配合机械）
	UINT	nInkModeType;						//当前使用的颜色模式，< INKMODE_MAX
	UINT	nSpeedType;							//当前使用的打印速度值，< _MAX_SPEED_LEVEL_
	UINT	nXResType;							//当前使用的X方向精度，< _MAX_XRES_LEVEL_
	UINT	nYPassType;							//当前使用的Y向Pass数，< _MAX_YPASS_LEVEL_

	int		nDistance_Hor[_MAX_SPEED_LEVEL_][_MAX_XRES_LEVEL_][KY_MAX_COLORS];			//指定每个喷头的X向间距（Pixel），nDistance_Hor[s][r][m]，m:喷头序号（nHeadNumber），s:速度，r:X向精度
	int		nDistance_Hor_Reversed[_MAX_SPEED_LEVEL_][_MAX_XRES_LEVEL_][KY_MAX_COLORS];	//指定每个喷头的X向反向间距（Pixel），nDistance_Hor[s][r][m]，m:喷头序号（nHeadNumber），s:速度，r:X向精度
	double	dDistance_Ver[KY_MAX_COLORS];												//指定每个喷头的Y向间距（mm），dDistance_Ver[m]，m:喷头序号（nHeadNumber）

	int		nAdjust_bi_directional_compensation[_MAX_SPEED_LEVEL_][_MAX_XRES_LEVEL_];	//双向补偿值，打印速度或精度不同，补偿值也不同，Pixel（脉冲）
	double	dAdjust_step_compensation[_MAX_YPASS_LEVEL_];								//步进补偿值（1/pixel，每个像素偏差值），Y向Pass数不同，补偿值也不同

	int		fade_nLevel[_MAX_YPASS_LEVEL_];				//淡入淡出处理-效果定义，Y向Pass数不同，参数值也不同

	enum TRACELESSPRINT_WAY
	{
		tracelessprint_way_none = 0,				//不使用无痕处理（直线型）

		tracelessprint_way_micro = 1,				//微波浪
		tracelessprint_way_small,						//小波浪
		tracelessprint_way_standard,					//标准型（普通型），与最小单元尺寸对应
		tracelessprint_way_middle,						//中波浪
		tracelessprint_way_large,						//大波浪
		tracelessprint_way_huge,						//巨波浪

		tracelessprint_way_limit = 0xFF
	};
	BYTE	bTracelessPrintWay;							//无痕打印处理-效果定义，TRACELESSPRINT_WAY
	double	dTracelessPrintDefWidth;					//无痕打印处理-最小单元默认宽度，mm
	double	dTracelessPrintDefHeight;					//无痕打印处理-最小单元默认高度，mm

	UINT	nPrintColorLevel;							//实际打印使用的灰度级别，<= PRINTHEAD::nColorLevel

	SPRAYPARAM		sprayparam;							//散喷参数

	CLEANHEADPARAM	cleanheadparam;						//打印中自动喷头清洗参数

	//打印方式
	enum PRINTWAY
	{
		printway_unidirection = 0,		//单向打印
		printway_bidirection = 1,		//双向打印

		printway_count                      //有效的打印方式数
	};
	BYTE	bPrintway;				//打印方式：PRINTWAY

	enum JUMPWAY
	{
		jumpway_normal = 0x00,		//无快进（跳白）功能
		jumpway_x = 0x01,		//X向（小车）允许快进
		jumpway_y = 0x02,		//Y向（导带/丝杆）允许快进
		jumpway_all = jumpway_x | jumpway_y,	//允许全部快进

	};
	BYTE	bJumpWay;			//快进（跳白）处理方式
	double	dJumpLimit_x;		//X向最小跳白宽度，mm
	double	dJumpLimit_y;		//Y向最大跳白长度，mm

	BYTE	bPrintToFile;		//输出到PRN文件，为FALSE时直接输出到打印机

	BYTE	bVirtualPrinter;	//是否为虚拟打印机（不连接实际硬件设备）

	//原点位置定义
	enum ORIGIN_POSITION
	{
		origin_pos_unknown = 0x00,		//未知

		origin_pos_x_zero = 0x01,		//X向打印原点
		origin_pos_x_max = 0x02,		//X向清洗槽位置（X向结束位置）
		origin_pos_x_current = 0x03,		//X向当前位置

		origin_pos_y_zero = 0x10,		//Y向打印原点
		origin_pos_y_max = 0x20,		//Y向结束位置
		origin_pos_y_current = 0x30,		//Y向当前位置

		origin_pos_zero_xy = origin_pos_x_zero | origin_pos_y_zero,				//打印原点
		origin_pos_max_xy = origin_pos_x_max | origin_pos_y_max,					//平板机待机位
		origin_pos_zero_x_max_y = origin_pos_x_zero | origin_pos_y_max,
		origin_pos_max_x_zero_y = origin_pos_x_max | origin_pos_y_zero,

		origin_pos_current_xy = origin_pos_x_current | origin_pos_y_current,		//当前位置
		origin_pos_current_x_zero_y = origin_pos_x_current | origin_pos_y_zero,			//丝杆方向打印原点（小车不动）
		origin_pos_zero_x_current_y = origin_pos_x_zero | origin_pos_y_current,			//小车方向打印原点（丝杆不动）

		origin_pos_max_x_current_y = origin_pos_x_max | origin_pos_y_current,			//散喷位，也即导带机待机位

		origin_pos_mask_x = 0x0F,		//X向位置参数掩码
		origin_pos_mask_y = 0xF0		//Y向位置参数掩码
	};
	BYTE	bOriginPosition_start;			//打印起始前小车位置，PRINTER::ORIGIN_POSITION
	BYTE	bOriginPosition_end;			//打印结束时小车位置，PRINTER::ORIGIN_POSITION

	BOOL	bFlatMachine;					//是否为平板机类型（FALSE为导带机类型）
	BOOL	bOriginPosition_posXEnable;		//是否支持X向相对定位操作，IO定位控制模式暂不支持，需设置为FALSE

	BOOL	bThicknessEnable;				//是否可以自定义布厚参数

	int		nPriority;					//打印处理线程优先级，默认为 THREAD_PRIORITY_NORMAL

public:
	BOOL	bOriginPosition_prompt;		//打印起始位置不是“打印原点”时提示

public:
	//读入参数
	bool GetFromProfile(const char *pszProfile);
	//写入参数
	bool SetToProfile(const char *pszProfile) const;
	//参数是否有效
	bool IsValid() const;
	//初始化
	void Initialize();

	BOOL fade_IsValid(int nYPassType) const;	//淡入淡出处理-是否有效

	//修正打印起始前小车位置参数
	//	IsPosXEnable	:[in]是否支持X向相对定位操作，IO定位控制模式暂不支持，需设置为FALSE
	static BYTE Check_OriginPosition_Start(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL IsPosXEnable);
	//修正打印结束时小车位置参数
	static BYTE Check_OriginPosition_End(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL IsPosXEnable);

	//-------- 与打印原点定义相关的控制选项操作函数-----
	//	IsFlatMachine	:[in]是否为平板机型，为FALSE时表示导带机型
	//	bStart			:[in]是否打印起始前的定位状态，为FALSE时表示针对打印结束时定位状态操作
	//	IsPosXEnable	:[in]是否支持X向相对定位操作，IO定位控制模式暂不支持，需设置为FALSE
	static BYTE TransIndexToOriginPosition(int nIndex, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable);
	static int TransOriginPositionToIndex(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable);
	static void InitOriginPositionList(CComboBox *pList, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable);

	//计算Pass步进参数
	//	pLeapNum	:[out]每Pass步进值（Pixel）
	//	nozzlesize	:[in]喷嘴数
	//	pass		:[in]PASS数
	//返回：
	//	最大的步进值(Pixel)
	static int step_calcleapnum(DWORD *pLeapNum, int nozzlesize, int pass, int nFadeLevel);
	//淡入淡出处理-得淡入参数值
	//	blimit	:[in]是否计算最大值，当为FALSE时，计算默认参数值
	static int fade_calclimit(int nozzlesize, int pass, bool blimit);

	//计算每个喷嘴占用的数据位数
	//	nPrintColorLevel	:[in]实际打印使用的灰度级别，<= PRINTHEAD::nColorLevel
	static int ColorLevel_GetStepPixel(UINT nPrintColorLevel);
};

//外部命令状态，应用程序需对此进行相应操作
enum ky_printer_command_status
{
	kypcs_mb_none = 0x00000000,	//正常，无外部命令

	kypcs_mb_reset = 0x00000001,	//复位

	kypcs_mb_pause = 0x00000002,	//暂停打印（与 kypcs_mb_resume 不同时使用）
	kypcs_mb_resume = 0x00000004,	//恢复打印（与 kypcs_mb_pause 不同时使用）

	kypcs_mb_syserror = 0x00000008,	//系统错误

	kypcs_mb_sysstatus = 0x00000010,	//系统异常状态

	kypcs_mb_cleanhead = 0x00000020,	//喷头清洗

	kypcs_mb_count
};

// 模块外部命令扩展
// UI
//
////扩展接口（界面相关）使能状态
enum ky_ui_type
{
	kyuit_offline = 0x80, //在非打印状态（脱机，未调用 print_start_print 或者调用过 print_end 之后）下有效 

	kyuit_online_inside_printdata = 0x01, //在数据打印状态（调用 print_data_print）下有效，一般仅是返回当前设备状态，此时底层可能以创建无模式对话框形式处理，创建成功后立即返回； 

	kyuit_online_outside_printdata = 0x02, //在非数据打印状态（已联机，但在 print_data_print 调用的前后）下有效，但它们在应用程序中的调用时机很难把握（★）； 

	kyuit_online_pause = 0x06, //在暂停状态下有效（包括 kyuit_online_outside_printdata 相关命令） 

	kyuit_online = 0x7F, //在联机状态（以成功调用 print_start_print 到 调用 print_end 为限）下有效(所有 kyuit_online_xxxx形式的状态的集合) 

	kyuit_all = 0xFF	//所有状态下均有效 
};
// hparent :[in]调用窗口
//	wParam, lParam :[in]直接使用 ky_ui_command_info 的两个参数
typedef void(_stdcall *ui_command_proc)(HWND hparent, WPARAM wParam, LPARAM lParam);
struct ky_ui_command_info
{
	const char			*ptext;		//命令的描述，将用于菜单的显示
	ui_command_proc		pfncommand;	//实际的命令，应用程序通过 pfncommand(hparent, wParam, lParam)来实现功能调用

	WPARAM				wParam;		//相关参数，仅用于函数调用	
									//	command_id : (dwParam & 0x00FFFFFF)，由模块内部定义
									//	ky_ui_type : (dwParam>>28) & 0xFF
	LPARAM				lParam;		//其它参数，仅用于函数调用
};


//当前花样打印信息
struct ky_print_info
{
	double	width;			//宽度 mm
	double	height;			//长度 mm

	UINT	nHeadNumber;	//最大喷头数，<= KY_MAX_COLORS
	UINT	nGroupCount;	//喷头组数，1, 2, 3, ...，<= KY_MAX_GROUP
	DWORD	nColorTable[KY_MAX_CHANNELS];				//指定每组喷头的颜料，ky_printer_color（单色）

	UINT    color_level;    //灰度打印方式
	UINT	xres;			//X方向的精度,DPI
	UINT	pass;			//Y方向的Pass值

	BYTE	bPrintway;		//打印方式：PRINTER::PRINTWAY
	UINT	nSpeedType;		//当前使用的打印速度值，< _MAX_SPEED_LEVEL_

	BYTE	bOriginPosition_start;	//打印起始前小车位置，PRINTER::ORIGIN_POSITION
	BYTE	bOriginPosition_end;	//打印结束时小车位置，PRINTER::ORIGIN_POSITION
};

#pragma pack(pop)