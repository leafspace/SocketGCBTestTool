#pragma once


//数码印花核心板网络通讯操作类
#include "../include/rpBasexp.h"
#include "rpSocketBase.h"

#ifdef RP_EXT_CLASS
#undef RP_EXT_CLASS
#undef RP_EXT_API
#undef RP_EXT_DATA
#endif

#define RP_EXT_CLASS       
#define RP_EXT_API         
#define RP_EXT_DATA     

//#ifdef RP_EXT_CLASS
//#undef RP_EXT_CLASS
//#undef RP_EXT_API
//#undef RP_EXT_DATA
//#endif
//
//#ifdef APP_CLAYOUT
//	#define RP_EXT_CLASS       AFX_CLASS_EXPORT
//	#define RP_EXT_API         AFX_API_EXPORT
//	#define RP_EXT_DATA        AFX_DATA_EXPORT
//#else
//	#define RP_EXT_CLASS       AFX_CLASS_IMPORT
//	#define RP_EXT_API         AFX_API_IMPORT
//	#define RP_EXT_DATA        AFX_DATA_IMPORT
//#endif

#pragma pack(push)
#pragma pack(1)

//指令执行状态反馈（正常接收指令串，在执行过程中的反馈信息）
//下位机反馈信息定义（GCB-->PC）（为PC程序操作方便，实际下位机传送参数关系 (PC-1)==(GCB)）
// enum COMM_RESULT_GCB_0
// {
// 	gcb_0_comm_result_succeed	= 0x00,	//指令正常执行完成
// 
// 	gcb_0_comm_result_failed	= 0x01,	//指令操作失败
// 	gcb_0_comm_result_repeater	= 0x02,	//下级板卡配置指令转发（中继）失败
// 	gcb_0_comm_result_invalid	= 0x03,	//无效指令
// 
// 	gcb_0_comm_result_other		= 0xFF	//其它错误
// };
enum GCB_COMM_RESULT_GCB
{
	gcb_comm_result_init = 0x00,	//初始化，未设定状态

	gcb_comm_result_succeed = 0x01,	//指令正常执行完成

	gcb_comm_result_failed = 0x02,	//指令操作失败
	gcb_comm_result_repeater = 0x03,	//下级板卡配置指令转发（中继）失败
	gcb_comm_result_invalid = 0x04,	//无效指令

	gcb_comm_result_other = 0xFF	//其它错误
};
//上位机反馈信息定义（PC-->GCB）
enum GCB_COMM_RESULT_PC
{
	gcb_pc_comm_result_succeed = 0x00,	//指令正常执行完成

	gcb_pc_comm_result_failed = 0x01,	//指令操作失败
	gcb_pc_comm_result_invalid = 0x02,	//无效指令

	gcb_pc_comm_result_other = 0xFF	//其它错误
};

//主动发送方式下指令状态
enum GCB_PROTOCOL_SEND_STATE
{
	gcb_protocol_send_state_failed = 0,	//失败
	gcb_protocol_send_state_checking,		//发送中
	gcb_protocol_send_state_ok,				//发送成功

	gcb_protocol_send_state_count
};

struct GCB_COMMAND_STATE
{
	BYTE sendState;				//指令发送及执行状态，GCB_PROTOCOL_SEND_STATE
	BYTE commState;				//指令反馈状态，GCB_COMM_RESULT_GCB
};
#define TRANS_COMMSTATE_GCB_TO_PC(gcb)	BYTE(((BYTE)(gcb)==0xFF) ? 0xFF : ((BYTE)(gcb)+1))

enum GCB_TCP_DEFAULT_PORT
{
	tcp_default_port_control = 10000,	//GCB配置及打印控制指令
	tcp_default_port_data = 10001,	//打印数据指令

	tcp_default_port_unknown = 0xFFFF
};

enum GCB_PROTOCOL_VERSION_INFO
{
	gcb_protocol_version_none = 0,			//初始化

	gcb_protocol_version_1000 = 1000,			// V1.0.00

	gcb_protocol_version_unknown = 0xFFFF		//无效版本信息
};

struct RP_EXT_CLASS GCB_Protocol_Control_Statue
{
	// PC --> GCB --> PC
	GCB_COMMAND_STATE	gcb_getversion;				//版本
	WORD				gcb_getversion_val;

	GCB_COMMAND_STATE	gcb_getbuffersize;			//得缓冲区大小
	DWORD				gcb_getbuffersize_val;

	GCB_COMMAND_STATE	gcb_init;					//板卡初始化

	GCB_COMMAND_STATE	gcb_print_ready;			//打印准备

	GCB_COMMAND_STATE	gcb_repeater_config;		//下级板卡配置指令（PC-->GCB-->板卡）

public:
	void Initialize();
};
#pragma pack(pop)

class RP_EXT_CLASS CCBSocketPort_Data : public CSocketBase
{

public:
	CCBSocketPort_Data();
	virtual ~CCBSocketPort_Data();

public:
	//	pszIP, uPort:[in]运动控制卡IP地址及操作端口
	//	nBufferSize :[in]初始化接收缓冲区大小
	//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
	//	dwHeartBeatTime	:[in]心跳检测信号发送间隔，ms，为0时不自动发送检测信号
	virtual BOOL Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime);

	virtual BOOL Close(BOOL bReleaseBuffer);
	virtual BOOL IsOpen() const;

	//对接收到的数据进行分析
	//操作对象			： m_pBuffer, m_dwReceiveSize
	//返回值：已处理的数据个数（<= m_dwReceiveSize），剩余部分留到下一次数据接收后处理
	//			<= 0时表示未处理（当前数据帧未完全接收），
	//			当当前数据均为无效数据时，也返回 m_dwReceiveSize
	//备注：在实际应用时，此函数一般需重载后使用，
	//	默认情况下是将当前缓冲m_pBuffer内容作为一次有效数据帧处理，返回值为 m_dwReceiveSize
	//	在本函数中必须处理 m_bcommu_status 的信息，否则将影响 SendMsg_handshake(...)对指令有效性的判断
	virtual DWORD ReceiveDataEx();
};
enum SENDMSG_HANDSHAKE_RESULT
{
	shr_succeed = 0,		//正常

	shr_parameter,			//参数错误

	shr_outofmemory,		//内存不足

	shr_send_failed,		//指令发送失败
	shr_cmd_failed,			//指令执行返回错误

	shr_stop,				//异常中止，stop, print end, sys error

	shr_timeout,			//指令执行操作（在规定时间内未接收到返回信息）

	shr_count
};

//中继数据帧分析
//返回：被处理的缓冲数据个数
//板卡-->GCB-->PC
//	pData				:[in]外部调用函数传入的参数信息，PC_Repeater_Config_SetProc(...)
//	pDataList,dataSize	:[in]中继数据帧数据缓冲区及数据长度
//返回：中继数据中当前处理的数据长度，<= dataSize
typedef DWORD(*PC_REPEATER_CONFIG_PROC)(void *pData, const void *pDataList, int dataSize);
class RP_EXT_CLASS CCBSocketPort_Control : public CSocketBase
{

public:
	CCBSocketPort_Control();
	virtual ~CCBSocketPort_Control();

protected:
	GCB_Protocol_Control_Statue	*m_pProtocolstatus;		//由下位机返回的各种参数（状态）信息-当前

	CBufferManager				m_sendBuffer;			//发送缓冲区

public:
	//	pszIP, uPort:[in]运动控制卡IP地址及操作端口
	//	nBufferSize :[in]初始化接收缓冲区大小
	//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
	//	dwHeartBeatTime	:[in]心跳检测信号发送间隔，ms，为0时不自动发送检测信号
	virtual BOOL Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime);

	virtual BOOL Close(BOOL bReleaseBuffer);
	virtual BOOL IsOpen() const;

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
	virtual int Commu_SendMsg_Direct(const void* snd_msg, int msg_len);
	virtual int Commu_SendMsg(const void* snd_msg, int msg_len);

	//发送操作指令（在指定时间段内检测指令返回信息）
	//返回值 ：SENDMSG_HANDSHAKE_RESULT
	int Commu_SendMsg_handshake(GCB_COMMAND_STATE *pCmdState, const void *snd_msg, int msg_len, int time_check);

protected:
	// 数据帧分析（仅处理一个有效帧）
	//返回：被处理的缓冲数据个数
	DWORD DataFrameAnalyze(const BYTE* pBuffer, DWORD dwBufferSize);

	//中继数据帧分析
	//返回：被处理的缓冲数据个数
	virtual DWORD DataFrameAnalyze_repeater_config(const BYTE* pBuffer, DWORD dwBufferSize);

	//反馈指令处理状态
	//	bResult :[in]GCB_COMM_RESULT_PC
	int Commu_SendMsg_confirmation(BYTE bCmd, BYTE bResult);

public:
	// 封装指令信息到数据帧中
	//	pDstBuffer :[out]封装后的数据帧缓冲区，缓冲区大小 >= pt_data_len_max
	static int dataframe_transdata(BYTE* pDstBuffer, BYTE bCmd, const BYTE* pCmdData, WORD wCmdSize);
	static int dataframe_transdata(BYTE* pDstBuffer, BYTE bCmd, BYTE bResultPC);

	//得当前下位机返回的各种参数（状态）信息
	const GCB_Protocol_Control_Statue *GetControlStatus() const;

public:
	//返回值 ： SENDMSG_HANDSHAKE_RESULT

	// PC --> GCB --> PC
	//版本
	int sendorder_gcb_getversion(WORD *pwVersion, int time_check);
	//得缓冲区大小
	int sendorder_gcb_getbuffersize(DWORD *pdwBufferSize, int time_check);

	//板卡初始化
	int sendorder_gcb_init(BYTE bInitWay, int time_check);
	//打印准备
	int sendorder_gcb_print_ready(int time_check);

	//下级板卡配置指令（PC-->GCB-->板卡）
	int sendorder_gcb_repeater_config(void *pDataList, BYTE cmdSize, int time_check);

	//下级板卡配置指令（板卡-->GCB-->PC）-- PC 反馈
	int sendorder_confirmation_pc_repeater_config(void *pDataList, BYTE cmdSize);

public:
	//是否输出Debug信息（写文件）
	BOOL debug_IsWriteSendInfo() const;
	BOOL debug_IsWriteReceiveInfo() const;
	//	pszFileName :[in]Debug信息文件名
	void debug_WriteSendInfo(const char *pszFileName, BOOL bWrite);
	void debug_WriteReceiveInfo(const char *pszFileName, BOOL bWrite);

protected:
	//是否输出Debug信息（写文件）
	BOOL m_debug_bWriteSendInfo;
	BOOL m_debug_bWriteReceiveInfo;
	CString m_debug_strWriteSendInfoFile;
	CString m_debug_strWriteReceiveInfoFile;

	//中继数据分析
private:
	PC_REPEATER_CONFIG_PROC m_pc_repeater_config_pFunc;
	void *m_pc_repeater_config_pFuncData;
public:
	void PC_Repeater_Config_SetProc(PC_REPEATER_CONFIG_PROC pFunc, void *pData);

};
