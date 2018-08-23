#pragma once


//数码印花主控板（V4.0）串口通讯操作类

#include "../include/rpBasexp.h"
#include "../SerialPort/rpComBase.h"
#include "../include/rpPrnData.h"

#define WORD_MAX					65535
#define servo_download_timevalue_length 242	//伺服下载时间表时间值一次发送长度
#define TIME_CONVERT_VALUE			1000000
#define TIME_CONVERT(s)				(s*TIME_CONVERT_VALUE)

#define ld_vol_max				5.0
#define ld_power_max			250
#define ld_power_channel_max	32
#define ld_buffer_size			32000
#define ld_data_length			250
#define ld_map_data_max			30
#define ld_light_num			8
#define ld_voltage_max			64

#define WM_COMM_RXCHAR				WM_USER+1
#define WM_COMM_TXCHAR				WM_USER+2
#define WM_SFDOWNLOAD_PROGRESS		WM_USER+3
#define WM_RASSTATE					WM_USER+4
#define WM_LD_PROGRESS				WM_USER+5
#define WM_LD_SET_POWER_STATUS		WM_USER+6

struct OnLinePara
{
	DWORD dist_acce; //第N档对应的加速距离，cts，DWORD
	DWORD dist_dece; //第N档对应的减速距离，cts，DWORD
	DWORD speed;     //第N档对应的速度，cts/us，DWORD
};

struct PositionPara
{
	DWORD pos_x_min; //小车最小位置
	DWORD pos_x_max; //小车最大位置
	DWORD pos_x_clean;//清洗/散喷位置
	DWORD pos_x_moisture;//保湿位置
	DWORD pos_y_min;//丝杆最小位置（导带机型忽略）
	DWORD pos_y_max;//丝杆最大位置（导带机型忽略）
};

enum PROTOCOL_MODE
{
	debug_mode = 0,
	print_mode = 1
};

//设备定位标识定义，实际定位值就以“小车标识 ｜ 丝杆标识”形式标识
enum DEVICE_POSITION
{
	//小车位置标识定义
	pos_x_device_current = 0x01,		//设备定位-当前位置（小车不动）
	pos_x_device_min = 0x02,		//设备定位-最小位置（设备原点）
	pos_x_device_max = 0x03,		//设备定位-最大位置
	pos_x_device_lastpos = 0x04,		//设备定位-上次位置
	pos_x_device_ready = 0x05,		//设备定位-待机位（设备启动并自检完成后待机位置）
	pos_x_device_check = 0x06,		//设备定位-检测位（功率检测位，暂仅平网蓝光使用）
	pos_x_device_clean = 0x07,		//设备定位-清洗位（平网蓝光暂不需要，但使用喷头的系统中需要） 
	pos_x_device_moisture = 0x08,		//设备定位-保湿位（平网蓝光暂不需要，但使用喷头的系统中需要）

	//丝杆/导带位置标识定义
	pos_y_device_current = 0x10,//设备定位-当前位置（丝杆/导带不动，导带机时除本参数项外，其它所有Y向标识均无效）
	pos_y_device_min = 0x20,		//设备定位-最小位置（设备原点，导带机不需要）
	pos_y_device_max = 0x30,		//设备定位-最大位置（导带机不需要）
	pos_y_device_ready = 0x40,		//设备定位-待机位（设备启动并自检完成后待机位置，导带机不需要）
	pos_y_device_lastpos = 0x50,		//设备定位-上次位置（导带机不需要）

	pos_zero_x_current_y = pos_x_device_min | pos_y_device_current,

	pos_unknown = 0x00		//未知，作“当前位置”（pos_x/y_device_current）处理
};
#define POS_X_DEVICE_MASK	0x0F		//定位标识掩码-小车方向
#define POS_Y_DEVICE_MASK	0xF0		//定位标识掩码-丝杆/导带方向

//保存当前位置-标识定义
enum DEVICE_CURRENT_POSITION_STORE
{
	store_current_pos_x = 0x01,		//保存小车（X向）电机当前位置
	store_current_pos_y = 0x02, 		//保存丝杆（Y向）电机当前位置
	store_current_pos_xy = store_current_pos_x | store_current_pos_y		//保存X和Y向电机当前位置信息
};

enum COMM_RESULT
{
	comm_result_succeed = 0x00,	//指令正常执行完成

	comm_result_para_invalid = 0x01,	//参数设置失败
	comm_result_move_invalid = 0x02,	//小车移动方向的位置已到极限，移动命令无效
	comm_result_download_invalid = 0x03, 	//写FLASH操作未成功

	comm_result_checking = 0x04,
	comm_result_other = 0xFF	//其它错误
};

//指令执行状态反馈（正常接收指令串，在执行过程中的反馈信息）
//下位机反馈信息定义（MB）
enum FLATLASER_COMM_RESULT
{
	mb_comm_result_succeed = 0x00,	//指令正常执行完成

	mb_comm_result_failed = 0x01,	//下位机分级处理时通信失败
	mb_comm_result_offline = 0x02,	//下位机处于脱机状态

	mb_comm_result_checking = 0x03,
	mb_comm_result_other = 0xFF	//其它错误
};

//指令执行状态反馈（正常接收指令串，在执行过程中的反馈信息）
//上位机（GCB）反馈信息定义
//enum FLATLASER_COMM_RESULT_GCB
//{
//	gcb_comm_result_succeed	= 0x00,	//指令正常执行完成
//
//	gcb_comm_result_failed	= 0x01,	//指令无法正常执行
//
//	gcb_comm_result_other		= 0xFF	//其它错误
//};


enum FLATLASER_ADDRESS_TYPE
{

	addr_none = 0x00,		//初始化，未知类型
	addr_pc = 0x02,		//GCB，电脑

	addr_ld = 0x03,		//LD，激光数据板

	addr_lp_1 = 0x04,		//LP，激光驱动板 1
	addr_lp_2 = 0x05,		//LP，激光驱动板 2
	addr_lp_3 = 0x06,		//LP，激光驱动板 3
	addr_lp_4 = 0x07,		//LP，激光驱动板 4

	addr_ts = 0x08,		//TS，触摸屏
	addr_com = 0x09,		//COM，通信板

	address_pd = 0x10,		//PD，功率检测板

	addr_servo_x = 0x11,		//SERVOX，小车电机伺服驱动板
	addr_servo_y = 0x12,		//SERVOY，导带电机伺服驱动板

	addr_power = 0x13,		//POWER，电源控制板

	addr_rec_cloth = 0x14,         //REC，收布板
	addr_sec_cloth = 0x15,         //SEC，送布板
	addr_ink = 0x16,         //INK，供墨板
	addr_correct = 0x17,         //CB，纠偏板
	addr_mb = 0x20,         //MB，高速数码底板
	addr_gcb = 0x21,         //GCB，高速数码核心板 
	addr_ghb1 = 0x22,          //GHB1，高速数码喷头板 1
	addr_ghb2 = 0x23,          //GHB2，高速数码喷头板 2
	addr_ghb3 = 0x24,          //GHB3，高速数码喷头板 3
	addr_ghb4 = 0x25,          //GHB4，高速数码喷头板 4
	addr_ghb5 = 0x26,          //GHB5，高速数码喷头板 5
	addr_ghb6 = 0x27,         //GHB6，高速数码喷头板 6
	addr_ghb7 = 0x28,         //GHB7，高速数码喷头板 7
	addr_ghb8 = 0x29,         //GHB8，高速数码喷头板 8

	addr_raster = 0x20,         //光栅控制板

	addr_servo_three_axis = 0x30,	//三轴伺服板

	addr_unknown = 0xFF		//未知
};

struct TIME_TABLE_ONLINE_PAMARATER	//表参数
{
	WORD			m_wAecSUnit;		//加速每格距离
	WORD			m_wDecSUnit;		//减速每格距离
	WORD			m_wTimeUnit;		//最小时间单元
	DWORD			m_dwAecNum;			//加速格数
	DWORD			m_dwDecNum;			//减速格数
	DWORD			m_dwEvenV;			//平均速度

	BYTE			m_bAecRecordNum;	//可以记录一些曲线特殊点数目	
	BYTE			m_bDecRecordNum;	//可以记录一些曲线特殊点数目

	CDWordArray		m_dwAecRecordArray;	//可以记录一些曲线特殊点
	CDWordArray		m_dwDecRecordArray;	//可以记录一些曲线特殊点
public:
	void Init();
};

struct TIME_TABLE_ONLINE
{
	TIME_TABLE_ONLINE_PAMARATER* m_parameterOnline;
	BYTE			m_bSpeedLevelOnline;		//速度档位
	CWordArray		m_wAecTimeArray;	//加速时间Word数组
	CWordArray		m_wDecTimeArray;	//减速时间Word数组
};

struct TIME_TABLE_OFFLINE_PAMARATER		//表参数
{
	WORD			m_wAecSUnit;			//加速段每格距离
	WORD			m_wFast2SlowSUnit;		//快到慢段每格距离
	WORD			m_wSlow2StopSUnit;		//慢到停段每格距离
	WORD			m_wTimeUnit;			//最小时间单元
	DWORD			m_dwAecNum;				//加速格数
	DWORD			m_dwFast2SlowNum;		//快到慢格数
	DWORD			m_dwSlow2StopNum;		//慢到停格数
	DWORD			m_dwFastV;			//快速速度
	DWORD			m_dwSlowV;			//慢速速度

	BYTE			m_bAecRecordNum;		//可以记录一些曲线特殊点数目
	BYTE			m_bFast2SlowRecordNum;	//可以记录一些曲线特殊点数目
	BYTE			m_bSlow2StopRecordNum;	//可以记录一些曲线特殊点数目

	CDWordArray		m_dwAecRecordArray;			//可以记录一些曲线特殊点
	CDWordArray		m_dwFast2SlowRecordArray;	//可以记录一些曲线特殊点
	CDWordArray		m_dwSlow2StopRecordArray;	//可以记录一些曲线特殊点
public:
	void Init();
};

struct TIME_TABLE_OFFLINE
{
	TIME_TABLE_OFFLINE_PAMARATER* m_parameterOffline;
	BYTE			m_bSpeedLevelOffline;		//速度档位
	CWordArray		m_wAecTimeArray;			//加速时间Word数组
	CWordArray		m_wFast2SlowTimeArray;		//减速（快到慢）时间Word数组
	CWordArray		m_wSlow2StopTimeArray;		//减速（慢到停）时间Word数组
};

struct SF_BOARD_PARAMETER_X
{
	float m_fReducerX;				//X减速机
	float m_fDollyPerimeter;		//小车周长
	float m_fPulsesPerRevolution_x;	//每圈脉冲
};

struct SF_BOARD_PARAMETER_Y
{
	float m_fReducerY;				//Y减速机
	float m_fDeferentPerimeter;		//导带周长
	float m_fDeferentPitch;			//丝杆螺距
	float m_fPulsesPerRevolution_y;	//每圈脉冲
};
struct SF_BOARD_PARAMETER_Z
{
	float m_fReducer_Z;				//Z减速机
	float m_fDeferentPerimeter_Z;	//导带周长
	float m_fDeferentPitch_Z;		//丝杆螺距
	float m_fPulsesPerRevolution_Z;	//每圈脉冲
};

struct BOARD_CARD_INFO
{
	WORD m_wDeviceVersion;		//板子版本
	WORD m_wProtocolVersion;	//程序版本
	DWORD m_dwSN;	//板卡序列
	DWORD m_dwDate;	//生产日期
	DWORD m_dwProducerID;	//调试员工
	DWORD m_dwCheckerID;	//检验员
};

//struct MB_CLEAN_HEAD_PARAMETER
//{
//	BYTE m_bEnable;		
//	WORD m_wPass;
//	WORD m_wPurgeTime;
//	WORD m_wPurgeDelay;
//	BYTE m_bCleanTime;
//};

struct MB_PRINT_START_PARAMETER
{
	BYTE	m_bSpeedLevelX;
	BYTE	m_bSpeedLevelY;
	DWORD	m_dwPositionX;
	DWORD	m_dwPositionY;
	WORD	m_wPurgeTime;
	WORD	m_bSprayTime;
	WORD	m_wSprayFreq;
	WORD	m_bSprayInterval;
	BYTE	m_bPower[8];
	WORD	m_dwPulseWidth[8];
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	WORD	m_wResX;
	WORD	m_wResY;
	BYTE	m_bPrintWay;
};

struct MB_PRINT_LINE_PARAMETER
{
	DWORD	m_dwStartPos;
	DWORD	m_dwWidth;
	WORD	m_wFreqPscM;
	WORD	m_wFreqPscN;
	DWORD	m_dwFreqDistD1;
	DWORD	m_dwFreqDistD2;
	DWORD	m_dwFreqDistD3;
	DWORD	m_dwPulse;
	DWORD	m_dwRePulse;
	DWORD	m_dwYDist;
	BYTE	m_bAction;
	WORD	m_wThousandth;
	//BYTE	m_bSelfN;
	//BYTE	m_bSelfInfo[256];
	DWORD	m_dwPrintHeight;
};

struct RAS_PRINT_START_PARAMETER
{
	BYTE	m_bSpeedLevelX;
	BYTE	m_bSpeedLevelY;
	DWORD	m_dwPositionX;
	DWORD	m_dwPositionY;
	WORD	m_wPurgeTime;
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	WORD	m_wResX;
	WORD	m_wResY;
	BYTE	m_bPrintWay;
};

struct RAS_PRINT_LINE_PARAMETER
{
	DWORD	m_dwStartPos;
	DWORD	m_dwWidth;
	WORD	m_wFreq;
	DWORD	m_dwPulse;
	DWORD	m_dwRePulse;
	DWORD	m_dwYDist;
	BYTE	m_bAction;
	WORD	m_wThousandth;
	DWORD	m_dwPrintHeight;
};


//LD板电压参数
struct LD_VOLTAGE
{
	BYTE  m_bFeedbackChannel;
	DWORD m_wPC;
	DWORD m_wLastPD;
	DWORD m_wNowPD;
	DWORD m_wLastLD;
	DWORD m_wNowLD;
};

//信息检索状态
enum PROTOCOL_QUERY_WAY
{
	protocol_queryway_failed = 0,	//失败
	protocol_queryway_checking,			//信息检索中
	protocol_queryway_ok,				//成功（MB返回有效信息）

	protocol_queryway_count
};
//参数设定状态
enum PROTOCOL_CONFIG_WAY
{
	protocol_configway_failed = 0,	//失败
	protocol_configway_checking,		//参数设定中
	protocol_configway_ok,				//成功（MB返回有效信息）

	protocol_configway_count
};
//打印指令处理状态
enum PROTOCOL_PRINT_WAY
{
	protocol_printway_failed = 0,	//失败
	protocol_printway_checking,			//指令发送中
	protocol_printway_ok,				//成功（MB成功处理并返回有效确认信息）

	protocol_printway_count
};

enum FLATLASER_LD_COMM_RESULT
{
	ld_comm_result_succeed = 0x00,		//指令正常执行完成

	ld_comm_result_failed = 0x01,		//下位机分级处理时通信失败
	ld_comm_result_offline = 0x02,		//下位机处于脱机状态
	ld_comm_result_power_invalid = 0x03, 	//激光功率不达标
	ld_comm_result_config_invalid = 0x04, 	//存储单元无法操作（无效）
	ld_comm_result_config_invalidata = 0x05, 	//存储单元数据无效（或在写入时出错）

	ld_comm_result_checking = 0x06,

	ld_comm_result_other = 0xFF		//其它错误
};

//系统异常信息定义，不包括系统出现无法维系正常打印的情况（如触发打印限位来停止打印过程等）
//	以下各状态按位取值，同一状态字中可包含多个异常信息
enum MB_SYSTEM_STATUS
{
	mb_system_normal = 0x00,		//缺省，无异常状态

	mb_system_ink = 0x01,		//液位低

	mb_system_unknown = 0x80,		//其它异常状态

};

//enum PROTOCOL_VERSION_INFO
//{
//	protocol_version_none		= 0x0000,			//初始化
//
//	protocol_version_4001		= 0x4001,			// V40.01 （16385）
//	protocol_version_4002		= 0x4002,			// V40.02 （16386）
//                                                    //	1、protocol_command_print_line 中增加是否开放 DPC 的选项 (bDPCDisable)
//                                                    //	2、数据帧返回值（MB）中允许定义操作失败模式（PC接收返回值时判断相应信息）
//	protocol_version_4003		= 0x4003,			// V40.03  (16387)
//                                                    //  增加打印时运动参数配置信息
//
//	protocol_version_unknown	= 0xFFFF			//无效版本信息
//};

#define _PLUGIN_MAX_COLOR_LIMIT_		20			//最大支持的颜色通道数

#define _PLUGIN_MAX_COLOR_DEFAULT_		8			//默认颜色通道数

#pragma pack(push)
#pragma pack(1)
//由下位机返回的各种参数（状态）信息
struct /*RP_EXT_CLASS*/ Protocol_Control_Statue
{
	// PC-->MB-->PC
	volatile BYTE	bCheckedWay;							//Check 正常连接状态，PROTOCOL_QUERY_WAY

	WORD			wProtocolVersion;						//下位机协议版本信息，PROTOCOL_VERSION_INFO
	BYTE			bControlAmplitude[_PLUGIN_MAX_COLOR_LIMIT_];	//当前的喷头电压幅值设定
	BYTE			bControlPulsewidth[_PLUGIN_MAX_COLOR_LIMIT_];	//当前的喷头电压脉宽设定
	WORD			wThicknessPluse;						//当前的布厚参数设定值，脉冲数
	SPRAYPARAM		sprayparam;								//散喷参数
	CLEANHEADPARAM	cleanheadparam;							//打印中自动喷头清洗参数

	MOTOPARAM_GENERAL	motoparam_general;
	MOTOPARAM_X			motoparam_x;
	MOTOPARAM_Y			motoparam_y;
	MOTOPARAM_Z			motoparam_z;

	volatile BYTE	bQueryWay_ProtocolVersion;				//信息检索状态-下位机协议版本，PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_ControlAmplitude;				//信息检索状态-当前的喷头电压幅值设定，PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_ControlPulsewidth;			//信息检索状态-当前的喷头电压脉宽设定，PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_ControlThickness;				//信息检索状态-当前的布厚参数设定，PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_sprayparam;					//信息检索状态-当前散喷参数设定值，PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_cleanheadparam;				//信息检索状态-当前自动清洗参数设定值，PROTOCOL_QUERY_WAY

	volatile BYTE	bQueryWay_motoparam_general;
	volatile BYTE	bQueryWay_motoparam_x;
	volatile BYTE	bQueryWay_motoparam_y;
	volatile BYTE	bQueryWay_motoparam_z;

	volatile BYTE	bConfigWay_ControlAmplitude;			//MB反馈：参数设定状态-当前喷头电压幅值设定，PROTOCOL_CONFIG_WAY
	volatile BYTE	bConfigWay_ControlPulsewidth;			//MB反馈：参数设定状态-当前喷头电压脉宽设定，PROTOCOL_CONFIG_WAY
	volatile BYTE	bConfigWay_ControlThickness;			//MB反馈：参数设定状态-当前布厚参数设定，PROTOCOL_CONFIG_WAY
	volatile BYTE	bConfigWay_sprayparam;					//MB反馈：参数设定状态-当前散喷参数设定，PROTOCOL_CONFIG_WAY
	volatile BYTE	bConfigWay_cleanheadparam;				//MB反馈：参数设定状态-当前自动清洗参数设定，PROTOCOL_CONFIG_WAY

	volatile BYTE	bConfigWay_motoparam_general;
	volatile BYTE	bConfigWay_motoparam_x;
	volatile BYTE	bConfigWay_motoparam_y;
	volatile BYTE	bConfigWay_motoparam_z;

	volatile BYTE	bPrintWay_printstart;					//MB反馈：打印指令处理状态-打印准备，PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_printline;					//MB反馈：打印指令处理状态-打印一行，PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_printblock;					//MB反馈：打印指令处理状态-打印一块，PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_printend;						//MB反馈：打印指令处理状态-打印结束，PROTOCOL_PRINT_WAY

	volatile BYTE	bPrintWay_pc_reset;						//MB反馈：打印指令处理状态-PC复位，PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_pc_pause;						//MB反馈：打印指令处理状态-PC暂停，PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_pc_resume;					//MB反馈：打印指令处理状态-PC继续，PROTOCOL_PRINT_WAY

	volatile BYTE	bPrintWay_print_back2origin;			//MB反馈：打印指令处理状态-回到打印原点，PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_print_cleanhead;				//MB反馈：打印指令处理状态-执行一次自动清洗操作，PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_print_spray;					//MB反馈：打印指令处理状态-执行一次散喷操作，PROTOCOL_PRINT_WAY

	// MB-->PC-->MB
	volatile BYTE	bSignal_mb_reset;						//接收到MB复位指令
	volatile BYTE	bSignal_mb_pause;						//接收到MB暂停指令
	volatile BYTE	bSignal_mb_resume;						//接收到MB继续指令
	volatile BYTE	bSignal_mb_syserror;					//接收到MB系统错误指令
	volatile BYTE	bSignal_mb_sysstatus;					//接收到MB系统异常指令

	volatile BYTE	bSignal_mb_cleanhead;					//接收到MB喷头清洗指令

	//SF
	volatile BYTE	bQueryWay_SF_GetExistentSpeedlevel;
	BYTE bExistSpeedLevel;
	volatile BYTE	bConfigWay_SF_DownloadOnlineTimeTableParameter;
	volatile BYTE	bConfigWay_SF_DownloadTimeTableValue;
	volatile BYTE	bConfigWay_SF_DownloadOfflineTimeTableParameter;
	volatile BYTE	bQueryWay_SF_GetOnlineTimeTableParameter;
	volatile BYTE	bQueryWay_SF_GetOfflineTimeTableParameter;
	volatile BYTE	bPrintWay_SF_CountFullPulse;
	volatile BYTE	bPrintWay_SF_MotorMoveRelative;
	volatile BYTE	bPrintWay_SF_MotorMoveAbsolute;
	volatile BYTE	bQueryWay_SF_MotorGetPosition;
	DWORD dwCurrentPosition;
	volatile BYTE	bPrintWay_SF_MotorMoveSpecialPosition;
	volatile BYTE	bPrintWay_SF_MotorMoveStop;
	volatile BYTE	bPrintWay_SF_MotorMove;
	volatile BYTE	bQueryWay_SF_GetFullPulse;
	DWORD dwFullPulse;
	volatile BYTE	bQueryWay_SF_GetSpecialPosition;
	DWORD dwSpecialPosition;
	volatile BYTE	bConfigWay_SF_SetSpecialPosition;
	volatile BYTE	bConfigWay_SF_DeleteAllTimeTableData;

	volatile BYTE	bConfigWay_SF_SetFirmParameterX;
	volatile BYTE	bConfigWay_SF_SetFirmParameterY;
	volatile BYTE	bConfigWay_SF_SetFirmParameterZ;

	volatile BYTE	bQueryWay_SF_GetFirmParameterX;
	volatile BYTE	bQueryWay_SF_GetFirmParameterY;
	volatile BYTE	bQueryWay_SF_GetFirmParameterZ;

	SF_BOARD_PARAMETER_X sf_board_parameter_x;
	SF_BOARD_PARAMETER_Y sf_board_parameter_y;
	SF_BOARD_PARAMETER_Z sf_board_parameter_z;

	volatile BYTE	bQueryWay_SF_GetTimeTableVitalParameter;
	DWORD dwVitalParaAecPulse;
	DWORD dwVitalParaDecPulse;
	DWORD dwVitalEvenT;

	volatile BYTE	bPrintWay_SF_MotorXY_MoveRelative;
	volatile BYTE	bPrintWay_SF_MotorXY_MoveAbsolute;
	volatile BYTE	bPrintWay_SF_MotorXY_MoveSpecialPosition;
	volatile BYTE	bPrintWay_SF_MotorXY_PrintLine;

	//MB
	volatile BYTE	bConfigWay_MB_OnlineState;

	//volatile BYTE	bConfigWay_MB_CleanHead;
	//volatile BYTE	bQueryWay_MB_CleanHead;
	//volatile BYTE	bPrintWay_MB_CleanHead;
	//MB_CLEAN_HEAD_PARAMETER mb_clean_head_parameter;
	//
	//volatile BYTE	bConfigWay_MB_Thickness;
	//volatile BYTE	bQueryWay_MB_Thickness;
	//WORD wThickness;
	volatile BYTE	bPrintWay_MB_PrintStart;
	volatile BYTE	bPrintWay_MB_PrintLine;
	volatile BYTE	bPrintWay_MB_PrintEnd;
	volatile BYTE	bPrintWay_MB_PrintPause;
	volatile BYTE	bPrintWay_MB_PrintResume;
	volatile BYTE	bPrintWay_MB_PrintStop;
	volatile BYTE	bPrintWay_MB_MoveAbsolutePosition;
	volatile BYTE	bPrintWay_MB_MoveRelativePosition;
	volatile BYTE	bPrintWay_MB_Move2SpecialPosition;
	volatile BYTE	bPrintWay_MB_StoreCurrentPosition;

	//板卡
	volatile BYTE	bConfigWay_BoardCard_SetInfo;
	volatile BYTE	bQueryWay_BoardCard_GetInfo;
	BOARD_CARD_INFO	board_card_info;
	volatile BYTE	bConfigWay_BoardCard_SetAddr;
	volatile BYTE	bQueryWay_BoardCard_GetAddr;
	BYTE	bBoardCardAddr;

	//供墨
	volatile BYTE	bConfigWay_Ink_Check485;
	volatile BYTE	bQueryWay_Ink_CheckState;
	BYTE bInkState;
	WORD wInkState;
	volatile BYTE	bConfigWay_Ink_Force;
	volatile BYTE	bConfigWay_Ink_Press;
	volatile BYTE	bConfigWay_Ink_Scrape;
	volatile BYTE	bConfigWay_Ink_Moisturize;
	volatile BYTE	bConfigWay_Ink_CycleMotor;

	//收布
	volatile BYTE	bQueryWay_RC_CheckState;
	BYTE bRcState;
	volatile BYTE	bConfigWay_RC_Pass;
	volatile BYTE	bConfigWay_RC_PrintStop;
	volatile BYTE	bConfigWay_RC_PrintOver;

	//送布
	volatile BYTE	bQueryWay_SC_CheckState;
	//BYTE bScState0;
	//BYTE bScState1;
	volatile BYTE	bConfigWay_SC_BandDry;
	volatile BYTE	bConfigWay_SC_PreDry;
	volatile BYTE	bConfigWay_SC_BandWash;
	volatile BYTE	bConfigWay_SC_PressRoller;
	volatile BYTE	bConfigWay_SC_Motor;
	volatile BYTE	bConfigWay_SC_Stop;
	volatile BYTE	bConfigWay_SC_CancleWrongLabel;

	//纠偏
	volatile BYTE	bConfigWay_CB_AutoCorrect;
	volatile BYTE	bConfigWay_CB_ManualCorrect;
	volatile BYTE	bConfigWay_CB_ManualCorrectStop;

	//COM
	volatile BYTE	bQueryWay_COM_GetBoardCondition;
	WORD wComBoardCondition;
	volatile BYTE	bPrintWay_COM_Motor_MoveRelative;

	//RAS
	volatile BYTE	bPrintWay_RAS_PrintStart;
	volatile BYTE	bPrintWay_RAS_PrintLine;
	volatile BYTE	bPrintWay_RAS_PrintEnd;
	volatile BYTE	bPrintWay_RAS_Select;

	int nSpeedLevelNum;
	OnLinePara onlinePara[10];
	PositionPara positionPara;

	volatile BYTE bPrintWay_RAS_GetSpeed;
	volatile BYTE bPrintWay_RAS_GetPosition;
	volatile BYTE bPrintWay_RAS_PrintStop;
	volatile BYTE bPrintWay_RAS_Move2Position;

	DWORD dwErrorCode;
	DWORD dwException;

	//收布新协议
	volatile BYTE	bPrintWay_RC_ComState;

	//送布新协议
	volatile BYTE	bPrintWay_SC_ComState;
	BYTE bScState0;
	BYTE bScState1;
	DWORD dwTemperature;

	//LD
	WORD			wLDVersion;
	BYTE			bChannelNum;
	BYTE			bPowerChannel[ld_power_channel_max];
	WORD			wVolValue[ld_power_channel_max];
	BYTE			bPowerValue[ld_power_channel_max];
	WORD			wFeedBackCheckVol;

	//LD send
	BYTE			bFeedBackChannel;
	WORD			wFeedBackVol;
	int				nProgress;

	volatile BYTE	bQueryWay_ld_all_power;

	volatile BYTE	bConfigWay_ld_power_off;
	volatile BYTE	bConfigWay_ld_check_power;
	volatile BYTE	bConfigWay_ld_all_power;

	volatile BYTE	bPrintWay_ld_voltage;		//64路新增
	volatile BYTE	bPrintWay_ld_check_power;

	volatile BYTE	bConfigWay_ld_write_config;
	volatile BYTE	bConfigWay_ld_write_data;
	volatile BYTE	bConfigWay_ld_read_config;

	volatile BYTE	bConfigWay_ld_light_on;
	volatile BYTE	bConfigWay_ld_light_off;
	volatile BYTE	bConfigWay_ld_light_on_time;

	volatile BYTE	bQueryWay_ld_get_current_power;

	//map data
	volatile BYTE	bConfigWay_map_data;
	volatile BYTE	bQueryWay_map_data;
	BYTE			bMapNum;
	WORD			wMapVolValue[ld_map_data_max];
	BYTE			bMapPowerValue[ld_map_data_max];

	//LD
	LD_VOLTAGE		ldVoltage;  //64路新增

public:
	//	bAll :[in]是否包括所有参数，FALSE时不初始化 wProtocolVersion
	void Initialize(BOOL bAll);
};
#pragma pack(pop)

enum SERVO_THREE_AXIS_WAY
{
	sv_three_axis_way_x = 0x01,		//X轴
	sv_three_axis_way_y = 0x02,		//Y轴
	sv_three_axis_way_z = 0x03,		//Z轴

	sv_three_axis_way_unknown = 0x00		//未知
};
enum SF_MOTOR_WAY
{
	motor_way_none = 0,

	motor_way_x = 1,
	motor_way_y = 2,

	motor_way_3Axis_x,
	motor_way_3Axis_y,
	motor_way_3Axis_z,

	motor_way_count
};

#include "../SocketPort/CBSocketPort.h"
class /*RP_EXT_CLASS*/ CCommunicationPort : public rpComBase::CComBase
{
public:
	CCommunicationPort(void);
	virtual ~CCommunicationPort(void);

public:
	//打印设备驱动模块（插件）信息
	//	infoProc		:[in]获取插件信息的函数地址
	//	bChannels		:[in]设备支持的颜色通道数， 小于等于 _PLUGIN_MAX_COLOR_LIMIT_
	//void SetPrinterPluginInfo(IS_PRINTER_PLUGIN_PROC infoProc, BYTE bChannels);

	// port initialisation
	//	nBufferSize :[in]初始化接收缓冲区大小
	//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
	virtual BOOL Open(UINT portnr, DWORD dwBaud, int nBufferSize, int nGrowSize);
	virtual BOOL Close(BOOL bReleaseBuffer);

	virtual BOOL IsOpen() const;

public:
	CWnd* m_pComWnd;

	CString m_strReciveData;
	CString m_strSendData;
	//DWORD m_dwSendThreadID;
	//DWORD m_dwReciveThreadID;

public:
	//直接发送操作指令，一般仅在 ReceiveData 函数中使用
	virtual BOOL Commu_SendMsg_Direct(const BYTE* snd_msg, int msg_len);
	//对接收到的数据进行分析
	//操作对象			： m_pBuffer, m_dwReceiveSize
	//返回值：已处理的数据个数（<= m_dwReceiveSize），剩余部分留到下一次数据接收后处理
	//			<= 0时表示未处理（当前数据帧未完全接收），
	//			当当前数据均为无效数据时，也返回 m_dwReceiveSize
	//备注：在实际应用时，此函数一般需重载后使用，
	//	默认情况下是将当前缓冲m_pBuffer内容作为一次有效数据帧处理，返回值为 m_dwReceiveSize
	//	在本函数中必须处理 m_bcommu_status 的信息，否则将影响 SendMsg_handshake(...)对指令有效性的判断
	virtual DWORD ReceiveDataEx();

	//发送操作指令
	virtual BOOL Commu_SendMsg(const BYTE* snd_msg, int msg_len);

	//操作指令串发送
	//参数定义：
	//snd_msg, s_msg_len	:[in]发送的指令串及长度
	//timeout				:[in]发送指令的总延时值 ms
	//time_repeatsend		:[in]当发送失败时，重发指令口串的时间间隔（累积值在 timeout范围内） ms
	//sendmsg_level			:[in] SENDMSG_LEVEL
	//返回值：SENDMSG_STATUS
	virtual int SendMsg_handshake(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, int sendmsg_level);
	virtual int SendMsg_handshake0(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, int sendmsg_level, BOOL bFlag);
	//操作指令串发送
	//snd_msg, s_msg_len	:[in]发送的指令串及长度
	//timeout				:[in]发送指令的总延时值 ms
	//time_repeatsend		:[in]当发送失败时，重发指令口串的时间间隔（累积值在 timeout范围内） ms
	//pFlag					:[in/out]指令发送状态标识（接收到相应信息时被自动赋值），当为NULL时，使用 sml_normal 模式发送
	//pFlagCheck,nCheckCount:[in]指定函数退出条件，除 TimeOut外，仅当  *pFlag==pFlagCheck[i] 时才返回
	//time_check			:[in]函数退出条件(pFlagCheck[i])判断的超时设置，ms
	virtual int SendMsg_handshake(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check);
	virtual int SendMsg_handshake0(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check, BOOL bFlag);

	//清接收缓冲区
	virtual void ClearReadBuffer();

	//重设指令发送状态信息
	virtual void ResetCommu_Status_Flags();

	//初始化相关参数
	virtual void Init(BOOL bReAllocBuffer);

public:
	//是否输出Debug信息（写文件）
	BOOL debug_IsWriteSendInfo() const;
	BOOL debug_IsWriteReceiveInfo() const;
	void debug_WriteSendInfo(BOOL bWrite);
	void debug_WriteReceiveInfo(BOOL bWrite);

protected:
	//是否输出Debug信息（写文件）
	BOOL m_debug_bWriteSendInfo;
	BOOL m_debug_bWriteReceiveInfo;

	//protected:
public:
	Protocol_Control_Statue		m_protocolstatus;		//由下位机返回的各种参数（状态）信息-当前

	//打印设备驱动模块（插件）信息
	//IS_PRINTER_PLUGIN_PROC		m_printer_plugin_proc;
	//设备的颜色通道数， 小于等于 _PLUGIN_MAX_COLOR_LIMIT_
	BYTE						m_printer_plugin_bChannels;
protected:
	//计算 CRC 值
	static BYTE CalcCRC(const BYTE *pBuffer, DWORD dwSize);
	// 数据帧分析（仅处理一个有效帧）
	//返回：被处理的缓冲数据个数
	DWORD DataFrameAnalyze(const BYTE* pBuffer, DWORD dwBufferSize);
	DWORD DataFrameAnalyze_PrintMode(const BYTE* pBuffer, DWORD dwBufferSize);
	//转换原点位置参数值：打印机参数 --> 通讯协议参数
	//	bOriginPosition		:[in]指定原点位置，PRINTER::ORIGIN_POSITION
	static BYTE TransOriginPosition(BYTE bOriginPosition);
	//出错提示并且要求重发指令
	//	resultHandshake		: [in]SENDMSG_STATUS，SendMsg_handshake指令发送返回状态
	//返回值：
	//	==0	：出错并重发指令
	//	> 0	：正常退出
	//	< 0	：出错但不重发指令
	static int SendMsg_handshake_Failed_and_Resend(int resultHandshake);

protected:
	//仅用于 ReceivedDataEx(...)函数
	BYTE	m_receivedataex_bReturnFlag[0xFF];		//反馈给设备（主控板）
	int		m_receivedataex_nReturnFlagLen;			//反馈信息长度

public:
	// 封装指令信息到数据帧中
	//	pDstBuffer :[out]封装后的数据帧缓冲区，缓冲区大小 >= pt_data_len_max
	static int dataframe_transdata(BYTE* pDstBuffer, BYTE bCmd, const BYTE* pCmdData, BYTE bCmdSize);

	static int dataframe_transdata0(BYTE* pDstBuffer, BYTE bSrcAddress, BYTE bDstAddress, BYTE bCmd, BYTE bCmdCount, const BYTE* pCmdData, BYTE bCmdSize);

	//得当前下位机返回的各种参数（状态）信息
	const Protocol_Control_Statue *GetControlStatus() const;

	//=================联机检测=================

	// 发送指令-检测联机状态（PC<-->MB通讯是否正常）
	BOOL sendorder_check(int time_check);
	// 发送指令-查询 MB 协议版本
	BOOL sendorder_getverion(int time_check);

	//=================查询=================
	// 注：
	//		以下查询相关指令，当 time_check < 0 时表示指令发送成功后立即返回，
	//		不检测反馈的相关硬件参数值（使用 sml_normal 模式发送）
	//
	// 发送指令-查询当前电压设定值
	BOOL sendorder_query_controlamplitude(int time_check);
	// 发送指令-查询当前脉宽设定值
	BOOL sendorder_query_controlpulsewidth(int time_check);
	// 发送指令-查询当前布厚参数设定值
	BOOL sendorder_query_controlthickness(int time_check);

	// 发送指令-查询当前散喷参数值
	BOOL sendorder_query_sprayparam(int time_check);
	// 发送指令-查询当前清洗参数值
	BOOL sendorder_query_cleanheadparam(int time_check);

	// 发送指令-运动-查询-公共配置参数
	BOOL sendorder_query_moto_general(int time_check);
	// 发送指令-运动-查询-X向配置参数
	BOOL sendorder_query_moto_x(int time_check);
	// 发送指令-运动-查询-Y向配置参数
	BOOL sendorder_query_moto_y(int time_check);
	// 发送指令-运动-查询-Z向配置参数
	BOOL sendorder_query_moto_z(int time_check);

	//=================设置=================
	// 发送指令-设置电压控制值
	//	sizeof (...) >= 8
	BOOL sendorder_set_controlamplitude(const BYTE *pbControlAmplitude, int time_check);
	// 发送指令-设置脉宽控制值
	//	sizeof (...) >= 8
	BOOL sendorder_set_controlpulsewidth(const BYTE *pbControlPulsewidth, int time_check);
	// 发送指令-设置当前布厚参数设定值
	//	nThickness	:布厚参数，脉冲值
	//	bTestMode	:是否需要作一次布厚测试动作
	BOOL sendorder_set_controlthickness(int nThickness, BOOL bTestMode, int time_check);

	// 发送指令-设置散喷参数值
	BOOL sendorder_set_sprayparam(const SPRAYPARAM &sprayparam, int time_check);
	// 发送指令-设置清洗参数值
	BOOL sendorder_set_cleanheadparam(const CLEANHEADPARAM &cleanheadparam, int time_check);

	// 发送指令-运动-设定-公共配置参数
	BOOL sendorder_set_moto_general(const MOTOPARAM_GENERAL &param, int time_check);
	// 发送指令-运动-设定-X向配置参数
	BOOL sendorder_set_moto_x(const MOTOPARAM_X &param, int time_check);
	// 发送指令-运动-设定-Y向配置参数
	BOOL sendorder_set_moto_y(const MOTOPARAM_Y &param, int time_check);
	// 发送指令-运动-设定-Z向配置参数
	BOOL sendorder_set_moto_z(const MOTOPARAM_Z &param, int time_check);

	//=================打印=================

	// 发送指令-打印准备
	BOOL sendorder_print_start(const ky_print_info *pPrintInfo, int time_check);
	// 发送指令-打印准备
#pragma pack(push)
#pragma pack(1)				//字节对齐
	struct PRINTLINE_INFO
	{
		BYTE	start_io;
		WORD	start_pos;
		DWORD	width;
		BYTE	end_io;
		BYTE	re_start_io;
		WORD	re_start_pos;
		DWORD	re_width;
		BYTE	re_end_io;
		DWORD	ydist;

		BYTE	bDPCDisable;	//是否关闭DPC输出
	};
#pragma pack(pop)
	//	wProtocolVersion	:[in]//下位机协议版本信息，PROTOCOL_VERSION_INFO，protocol_version_none 表示自适应
	BOOL sendorder_print_line(const PRINTLINE_INFO &lineInfo, WORD wProtocolVersion, int time_check);

	// 发送指令-回到打印原点
	//	bOriginPosition		:[in]指定原点位置，PRINTER::ORIGIN_POSITION
	BOOL sendorder_print_back2origin(BYTE bOriginPosition, int time_check);
	// 发送指令-执行一次自动清洗操作
	//	nPurgeTime	:[in]压墨时长（ms）
	//	nPurgeDelay	:[in]压墨后延时（ms）
	//	nCleanTime	:[in]刮刀次数
	BOOL sendorder_print_cleanhead(int nPurgeTime, int nPurgeDelay, BYTE bCleanTime, int time_check);
	// 发送指令-执行一次散喷操作
	//	bSprayTime	:[in]散喷次数
	BOOL sendorder_print_spray(BYTE bSprayTime, int time_check);

	// 发送指令-打印结束
	//	bOriginPosition		:[in]指定原点位置，PRINTER::ORIGIN_POSITION
	BOOL sendorder_print_end(BYTE bOriginPosition, int time_check);

	//=================异常=================
	// 发送指令-复位
	BOOL sendorder_pc_reset(int time_check);
	// 发送指令-暂停
	BOOL sendorder_pc_pause(int time_check);
	// 发送指令-继续
	BOOL sendorder_pc_resume(int time_check);

	//================确认信号反馈===========

	// 发送确认指令-MB复位
	BOOL sendorder_confirmation_mb_reset(void);
	// 发送确认指令-MB暂停
	BOOL sendorder_confirmation_mb_pause(void);
	// 发送确认指令-MB继续
	BOOL sendorder_confirmation_mb_resume(void);
	// 发送确认指令-MB系统错误
	BOOL sendorder_confirmation_mb_syserror(void);
	// 发送确认指令-MB系统异常
	BOOL sendorder_confirmation_mb_status(void);
	// 发送确认指令-MB喷头清洗
	BOOL sendorder_confirmation_mb_cleanhead(void);

public:
	//中断当前操作指令串发送状态检测过程
	void sendorder_handshake_Break(BOOL bBreak);
	//当前是否在发送消息时的循环等待中
	bool sendorder_handshake_isinloop() const;

	CWnd* m_pOnlineTimeTableDlg;
	CWnd* m_pOfflineTimeTableDlg;

	//SF
	//	bMotoWay :[in]SF_MOTOR_WAY
	BOOL servo_get_existent_speedlevel(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_set_online_timetable_paramater(BYTE bMotoWay, const TIME_TABLE_ONLINE &timetableOnline, int time_check, BOOL bFlag);
	BOOL servo_set_online_timetable_timevalue(BYTE bMotoWay, const TIME_TABLE_ONLINE &timetableOnline, int time_check, BOOL bFlag);

	BOOL servo_set_offline_timetable_paramater(BYTE bMotoWay, const TIME_TABLE_OFFLINE &timetableOffline, int time_check, BOOL bFlag);
	BOOL servo_set_offline_timetable_timevalue(BYTE bMotoWay, const TIME_TABLE_OFFLINE &timetableOffline, int time_check, BOOL bFlag);

	TIME_TABLE_ONLINE_PAMARATER m_parameterOnline;
	TIME_TABLE_OFFLINE_PAMARATER m_parameterOffline;

	BOOL servo_get_online_timetable_paramater(BYTE bMotoWay, BYTE bSpeedLevel, int time_check, BOOL bFlag);
	BOOL servo_get_offline_timetable_paramater(BYTE bMotoWay, BYTE bSpeedLevel, int time_check, BOOL bFlag);

	BOOL servo_count_fullpulse(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_motor_move_relative(BYTE bMotoWay, BYTE bSpeedType, DWORD dwPulse, DWORD dwRePulse, int time_check, BOOL bFlag);
	BOOL servo_motor_move_absolute(BYTE bMotoWay, BYTE bSpeedType, DWORD dwPulse, int time_check, BOOL bFlag);

	BOOL servo_motor_get_position(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_motor_move_special_position(BYTE bMotoWay, BYTE bSpeedType, BYTE bSpecialPosition, int time_check, BOOL bFlag);
	BOOL servo_motor_move_stop(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_motor_move(BYTE bMotoWay, BYTE bMoveType, int time_check, BOOL bFlag);

	BOOL servo_motor_xy_move_relative(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD pulseY, int time_check, BOOL bFlag);
	BOOL servo_motor_xy_move_absolute(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD pulseY, int time_check, BOOL bFlag);
	BOOL servo_motor_xy_move_special_position(BYTE bSpeedTypeX, BYTE bSpeedTypeY, BYTE bPosX, BYTE bPosY, int time_check, BOOL bFlag);
	BOOL servo_motor_xy_print_line(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD re_pulseX, DWORD yDist, int time_check, BOOL bFlag);

	BOOL servo_get_fullpulse(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_get_special_position(BYTE bMotoWay, BYTE bSpecialPosition, int time_check, BOOL bFlag);
	BOOL servo_set_special_position(BYTE bMotoWay, BYTE bSpecialPosition, DWORD dwSpecialPosition, int time_check, BOOL bFlag);
	BOOL servo_delete_all_timetable_data(BYTE bMotoWay, int time_check, BOOL bFlag);

	BOOL servo_set_board_parameter_x(SF_BOARD_PARAMETER_X sf_board_parameter_x, int time_check, BOOL bFlag, BOOL bThreeAxisMotor);
	BOOL servo_set_board_parameter_y(SF_BOARD_PARAMETER_Y sf_board_parameter_y, int time_check, BOOL bFlag, BOOL bThreeAxisMotor);
	BOOL servo_set_board_parameter_z(SF_BOARD_PARAMETER_Z sf_board_parameter_z, int time_check, BOOL bFlag);

	BOOL servo_get_board_parameter_x(int time_check, BOOL bFlag, BOOL bThreeAxisMotor);
	BOOL servo_get_board_parameter_y(int time_check, BOOL bFlag, BOOL bThreeAxisMotor);
	BOOL servo_get_board_parameter_z(int time_check, BOOL bFlag);

	BOOL servo_get_timetable_vital_parameter(BYTE bMotoWay, BYTE bSpeedLevel, int time_check, BOOL bFlag);


	//MB
	//BOOL mb_set_online_state(BYTE bOnlineState,int time_check,BOOL bFlag);
	BOOL mb_print_start(MB_PRINT_START_PARAMETER mb_print_start_parameter, int time_check, BOOL bFlag);
	BOOL mb_print_line(MB_PRINT_LINE_PARAMETER mb_print_line_parameter, int time_check, BOOL bFlag);
	BOOL mb_print_end(BYTE bPosition, int time_check, BOOL bFlag);
	BOOL mb_print_pause(int time_check, BOOL bFlag);
	BOOL mb_print_resume(int time_check, BOOL bFlag);
	BOOL mb_print_stop(int time_check, BOOL bFlag);
	BOOL mb_move_absolute_position(DWORD dwPositionX, DWORD dwPositionY, int time_check, BOOL bFlag);
	BOOL mb_move_relative_position(DWORD dwPulseX, DWORD dwPulseY, int time_check, BOOL bFlag);
	BOOL mb_move_to_special_position(BYTE bSpecialPosition, int time_check, BOOL bFlag);
	BOOL mb_store_current_position(BYTE bCurrentPosition, int time_check, BOOL bFlag);
	//BOOL mb_set_clean_head_parameter(MB_CLEAN_HEAD_PARAMETER mb_clean_head_parameter,int time_check,BOOL bFlag);
	//BOOL mb_get_clean_head_parameter(int time_check,BOOL bFlag);
	//BOOL mb_print_clean_head(MB_CLEAN_HEAD_PARAMETER mb_clean_head_parameter,int time_check,BOOL bFlag);

	BOOL mb_confirmation_online_state(void);

	//板卡
	BOOL board_card_set_info(BYTE bDstAddr, BOARD_CARD_INFO board_card_info, int time_check, BOOL bFlag);
	BOOL board_card_get_info(BYTE bDstAddr, int time_check, BOOL bFlag);
	BOOL board_card_set_addr(DWORD dwSN, BYTE bAddr, int time_check, BOOL bFlag);
	BOOL board_card_get_addr(DWORD dwSN, int time_check, BOOL bFlag);

	//供墨
	BOOL ink_check_485(int time_check, BOOL bFlag);
	BOOL ink_check_state(int time_check, BOOL bFlag);
	BOOL ink_force(WORD wForce, int time_check, BOOL bFlag);
	BOOL ink_press(WORD wPress, WORD wPressTime, int time_check, BOOL bFlag);
	BOOL ink_scrape(int time_check, BOOL bFlag);
	BOOL ink_moisturize(BYTE bMoisturize, int time_check, BOOL bFlag);
	BOOL ink_cycle_motor(BYTE bOpen, int time_check, BOOL bFlag);

	//收布
	BOOL rc_check_state(int time_check, BOOL bFlag);
	BOOL rc_pass(BYTE bPass, DWORD dwPass, int time_check, BOOL bFlag);
	BOOL rc_print_stop(BYTE bPrintStop, int time_check, BOOL bFlag);
	BOOL rc_print_over(int time_check, BOOL bFlag);

	//送布
	BOOL sc_check_state(int time_check, BOOL bFlag);
	BOOL sc_band_dry(BYTE bBandDry, WORD wBandDry, int time_check, BOOL bFlag);
	BOOL sc_pre_dry(BYTE bPreDry, WORD wPreDry, int time_check, BOOL bFlag);
	BOOL sc_band_wash(BYTE bOpen, int time_check, BOOL bFlag);
	BOOL sc_press_roller(BYTE bOpen, int time_check, BOOL bFlag);
	BOOL sc_motor(BYTE bOpen, int time_check, BOOL bFlag);
	BOOL sc_stop(int time_check, BOOL bFlag);
	BOOL sc_cancel_wrong_label(int time_check, BOOL bFlag);

	//纠偏
	BOOL cb_auto_correct(DWORD dwPulse, int time_check, BOOL bFlag);
	BOOL cb_manual_correct(BYTE bType, DWORD dwPulse, int time_check, BOOL bFlag);
	BOOL cb_manual_correct_stop(int time_check, BOOL bFlag);

	//COM
	BOOL com_motor_move_relative(BYTE bMotoWay, BYTE bSpeedType, DWORD dwPulse, DWORD dwRePulse, int time_check, BOOL bFlag);
	BOOL com_get_board_condition(int time_check, BOOL bFlag);

	//RAS
	BOOL ras_print_start(RAS_PRINT_START_PARAMETER ras_print_start_parameter, int time_check, BOOL bFlag);
	BOOL ras_print_line(RAS_PRINT_LINE_PARAMETER ras_print_line_parameter, int time_check, BOOL bFlag);
	BOOL ras_print_end(BYTE bPosition, int time_check, BOOL bFlag);
	BOOL ras_select(BYTE bSelect, int time_check, BOOL bFlag);
	BOOL ras_confirmation(BYTE bCmd);
	BOOL ras_get_speed(BYTE bMotoDirection, int time_check, BOOL bFlag);
	BOOL ras_get_position(int time_check, BOOL bFlag);
	BOOL ras_print_stop(int time_check, BOOL bFlag);
	BOOL ras_move2_position(DWORD x, DWORD y, int time_check, BOOL bFlag);

	CWnd* m_pRasDlg;
	int m_nProtocolMode; //仅限RAS板打印模式

	//新收布板
	BOOL rc_servo_motor_move_distance(BYTE bSpeedType, DWORD dwPulse, int time_check, BOOL bFlag);
	BOOL rc_up_down_motor_move(BYTE bType, int time_check, BOOL bFlag);
	BOOL rc_servo_motor_move(BYTE bSpeedType, int time_check, BOOL bFlag);
	BOOL rc_heater_con(BYTE bType, int time_check, BOOL bFlag);
	BOOL rc_cloth_motor_con(BYTE bType, int time_check, BOOL bFlag);
	BOOL rc_up_down_motor_reset(int time_check, BOOL bFlag);

	//新送布板
	BOOL sc_new_sendcloth_check_state(int time_check, BOOL bFlag);
	BOOL sc_new_band_dry(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_band_wash(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_pre_dry(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_dd_band_wash(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_get_temperature(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_press_cloth(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_cloth_roller(BYTE bType, DWORD dwType, int time_check, BOOL bFlag);
	BOOL sc_new_cancle_wrong_lable(int time_check, BOOL bFlag);
	BOOL sc_new_manual_correct(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_manual_correct_stop(int time_check, BOOL bFlag);

	//LD
	CWnd* m_pLDWnd;

	BOOL ld_power_off(int time_check, BOOL bFlag);

	BOOL ld_set_check_power(const BYTE bPowerChannel, const WORD wVolValue, int time_check, BOOL bFlag);
	BOOL ld_set_laser_test_power(const BYTE bPowerChannel, const WORD wVolValue, int time_check, BOOL bFlag);
	BOOL ld_set_pc_power(const BYTE bChannel, WORD wVoltage_PC, int time_check, BOOL bFlag);
	BOOL ld_set_ld_power(const BYTE bChannel, WORD wVoltage_LD, int time_check, BOOL bFlag);
	BOOL ld_get_last_power(const BYTE bChannel, int time_check, BOOL bFlag);
	BOOL ld_get_now_power(const BYTE bChannel, int time_check, BOOL bFlag);
	BOOL ld_correct_power(const BYTE bChannel, int time_check, BOOL bFlag);

	//映射表
	BOOL ld_set_map_data(const WORD* wMapVolValue, const BYTE* bMapPowerValue, BYTE bMapNum, int time_check, BOOL bFlag);
	BOOL ld_get_map_data(int time_check, BOOL bFlag);

	BOOL ld_set_light_on(const DWORD* dwLightData, int time_check, BOOL bFlag);
	BOOL ld_set_light_off(const DWORD* dwLightData, int time_check, BOOL bFlag);
	BOOL ld_set_light_on_time(const BYTE bChannel, const DWORD dwContinueTime, int time_check, BOOL bFlag);

	//发送确认指令-LD中间过程反馈
	BOOL ld_confirmation_set_power_status(void);

	//以下参数/函数针对 TCP 封装通讯
public:
	CCBSocketPort_Control	m_socket_Control;
	CCBSocketPort_Data		m_socket_Data;
	BOOL					m_socket_bEnable;
	void					*m_socket_pThread;
private:
	//创建处理线程
	bool socket_thread_create();
	//关闭处理线程
	bool socket_thread_close();
	//释放处理线程
	bool socket_thread_delete();

	//线程是否已经创建成功（可正常进行打印相关处理）
	bool socket_thread_isready() const;
	//线程是否已经处理正常运转中（正常打印过程已经启动）
	bool socket_thread_isrunning() const;
	//等待除 Write Thread 外的其它线程关闭
	//	dwTimeOut	:[in]超时参数，ms, 为0时表示不限时等待
	//注：一般用于 Write Thread 退出前的延时处理
	void socket_thread_wait_for_terminated(DWORD dwTimeOut);
public:
	//初始化参数
	virtual void socket_init(BOOL bReAllocBuffer);
	//	pszIP, uPort:[in]运动控制卡IP地址及操作端口
	//	nBufferSize :[in]初始化接收缓冲区大小
	//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
	//	dwHeartBeatTime	:[in]心跳检测信号发送间隔，ms，为0时不自动发送检测信号
	//	pErrorInfo	:[out]错误信息，仅当返回值为FALSE时有效
	virtual BOOL socket_Open(const char *pszIP, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime, CString *pErrorInfo);
	//发送操作指令
	virtual int socket_senddata(const void* snd_msg, int msg_len);
	//中继数据帧分析
	static DWORD socket_repeater_config_proc(void *pData, const void *pDataList, int dataSize);
	//中继数据帧分析-添加数据信息
	DWORD socket_AddReceiveData(const void* pBuf, DWORD InBufferCount);
	BOOL socket_IsEnable() const;
	BOOL socket_IsOpen() const;
	void socket_Close(BOOL bReleaseBuffer);

};
