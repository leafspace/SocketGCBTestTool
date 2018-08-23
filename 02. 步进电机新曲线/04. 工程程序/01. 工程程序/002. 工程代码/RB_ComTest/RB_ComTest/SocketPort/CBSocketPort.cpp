#include "../stdafx.h"
#include "rpSocketBase.h"
#include "SocketPort_Protocol.h"
#include "CBSocketPort.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCBSocketPort_Data::CCBSocketPort_Data()
{
}
CCBSocketPort_Data::~CCBSocketPort_Data()
{
	Close(TRUE);
}
//	pszIP, uPort:[in]运动控制卡IP地址及操作端口
//	nBufferSize :[in]初始化接收缓冲区大小
//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
//	dwHeartBeatTime	:[in]心跳检测信号发送间隔，ms，为0时不自动发送检测信号
BOOL CCBSocketPort_Data::Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime)
{
	if (!CSocketBase::Open(pszIP, nPort, nBufferSize, nGrowSize, dwHeartBeatTime))
		return FALSE;

	return TRUE;
}

BOOL CCBSocketPort_Data::Close(BOOL bReleaseBuffer)
{
	CSocketBase::Close(bReleaseBuffer);

	return TRUE;
}
BOOL CCBSocketPort_Data::IsOpen() const
{
	return CSocketBase::IsOpen();
}

//对接收到的数据进行分析
//操作对象			： m_pBuffer, m_dwReceiveSize
//返回值：已处理的数据个数（<= m_dwReceiveSize），剩余部分留到下一次数据接收后处理
//			<= 0时表示未处理（当前数据帧未完全接收），
//			当当前数据均为无效数据时，也返回 m_dwReceiveSize
//备注：在实际应用时，此函数一般需重载后使用，
//	默认情况下是将当前缓冲m_pBuffer内容作为一次有效数据帧处理，返回值为 m_dwReceiveSize
//	在本函数中必须处理 m_bcommu_status 的信息，否则将影响 SendMsg_handshake(...)对指令有效性的判断
DWORD CCBSocketPort_Data::ReceiveDataEx()
{
	DWORD dwResult = m_dwReceiveSize;

	ASSERT(m_pBuffer != NULL);
	ASSERT(m_dwReceiveSize <= m_dwBufferSize);

	DWORD	i;
	BYTE	bReturnFlag = 0;
	for (i = 0; i < m_dwReceiveSize; i++)
	{
		bReturnFlag = m_pBuffer[i];
	}

	dwResult = i;
	return dwResult;
}

void GCB_Protocol_Control_Statue::Initialize()
{
	BZERO(*this);
}


CCBSocketPort_Control::CCBSocketPort_Control()
{
	m_pProtocolstatus = NULL;

	//是否输出Debug信息（写文件）
	m_debug_bWriteSendInfo = FALSE;
	m_debug_bWriteReceiveInfo = FALSE;

	m_debug_strWriteSendInfoFile = "";
	m_debug_strWriteReceiveInfoFile = "";

	m_pc_repeater_config_pFunc = NULL;
	m_pc_repeater_config_pFuncData = NULL;

	//初始化发送缓冲区
	m_sendBuffer.InitBuffer(NULL, (gcb_pt_data_len_max + 100));
}
CCBSocketPort_Control::~CCBSocketPort_Control()
{
	Close(TRUE);
}

void CCBSocketPort_Control::PC_Repeater_Config_SetProc(PC_REPEATER_CONFIG_PROC pFunc, void *pData)
{
	m_pc_repeater_config_pFunc = pFunc;
	m_pc_repeater_config_pFuncData = pData;
}
//	pszIP, uPort:[in]运动控制卡IP地址及操作端口
//	nBufferSize :[in]初始化接收缓冲区大小
//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
//	dwHeartBeatTime	:[in]心跳检测信号发送间隔，ms，为0时不自动发送检测信号
BOOL CCBSocketPort_Control::Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime)
{
	if (m_pProtocolstatus == NULL)
		m_pProtocolstatus = new GCB_Protocol_Control_Statue;
	if (m_pProtocolstatus == NULL)
		return FALSE;

	m_pProtocolstatus->Initialize();

	nBufferSize = max(gcb_pt_data_len_max, nBufferSize);		//至少要能存放一条完整的指令串
	if (!CSocketBase::Open(pszIP, nPort, nBufferSize, nGrowSize, dwHeartBeatTime))
		return FALSE;

	return TRUE;
}

BOOL CCBSocketPort_Control::Close(BOOL bReleaseBuffer)
{
	CSocketBase::Close(bReleaseBuffer);

	if (bReleaseBuffer)
	{
		if (m_pProtocolstatus != NULL)
		{
			delete m_pProtocolstatus;
			m_pProtocolstatus = NULL;
		}
	}
	m_sendBuffer.InitBuffer(NULL, 0);

	m_pc_repeater_config_pFunc = NULL;
	m_pc_repeater_config_pFuncData = NULL;
	return TRUE;
}
BOOL CCBSocketPort_Control::IsOpen() const
{
	return CSocketBase::IsOpen();
}

//是否输出Debug信息（写文件）
BOOL CCBSocketPort_Control::debug_IsWriteSendInfo() const
{
	return m_debug_bWriteSendInfo;
}
BOOL CCBSocketPort_Control::debug_IsWriteReceiveInfo() const
{
	return m_debug_bWriteReceiveInfo;
}
//	pszFileName :[in]Debug信息文件名
void CCBSocketPort_Control::debug_WriteSendInfo(const char *pszFileName, BOOL bWrite)
{
	m_debug_bWriteSendInfo = bWrite;
	m_debug_strWriteSendInfoFile = pszFileName;
}
//	pszFileName :[in]Debug信息文件名
void CCBSocketPort_Control::debug_WriteReceiveInfo(const char *pszFileName, BOOL bWrite)
{
	m_debug_bWriteReceiveInfo = bWrite;
	m_debug_strWriteReceiveInfoFile = pszFileName;
}
// 数据帧分析（仅处理一个有效帧）
//返回：被处理的缓冲数据个数
DWORD CCBSocketPort_Control::DataFrameAnalyze(const BYTE* pBuffer, DWORD dwBufferSize)
{
	BYTE		bCmd;
	WORD		wCmdSize;
	DWORD		dwFrameLen;
	DWORD		dwEndFlagPos;		//数据帧结束标识索引
	DWORD		dwResult = 0;
	const BYTE	*pCmdData;

	if (pBuffer == NULL || dwBufferSize == 0)
		return 0;

	if (dwBufferSize < gcb_pt_data_len_min)
		return 0;	//参数不全，达不到最小长度要求

	if (pBuffer[gcb_pt_data_start1] != gcb_protocol_dataframe_start1
		|| pBuffer[gcb_pt_data_start2] != gcb_protocol_dataframe_start2)
	{//非有效数据帧，第一个字节标识为无效值
		ASSERT(0);
		dwResult = 1;
		return dwResult;
	}

	bCmd = pBuffer[gcb_pt_data_cmd];
	wCmdSize = MAKEWORD(pBuffer[gcb_pt_data_cmdsize1], pBuffer[gcb_pt_data_cmdsize2]);
	dwEndFlagPos = gcb_pt_data_startfixcount;
	dwEndFlagPos += wCmdSize;
	dwFrameLen = dwEndFlagPos + gcb_pt_data_endfixcount;

	if (dwBufferSize < dwFrameLen)
	{//指令串不全
		dwResult = 0;
		goto end;
	}

	if (pBuffer[dwEndFlagPos] != gcb_protocol_dataframe_end)
	{//非有效结束标识，前2个字节标识为无效值
		dwResult = 2;
		goto end;
	}

	//有效参数数据起始
	dwResult = dwFrameLen;
	pCmdData = pBuffer + gcb_pt_data_startfixcount;
	switch (bCmd)
	{
	case cmd_gcb_getversion:
		//版本
		ASSERT(wCmdSize == 2);
		m_pProtocolstatus->gcb_getversion.commState = gcb_comm_result_succeed;
		m_pProtocolstatus->gcb_getversion_val = PROTOCOL_MAKEDATA_WORD(pCmdData);
		break;
	case cmd_gcb_getbuffersize:
		//得设备定位参数
		ASSERT(wCmdSize == 4);
		m_pProtocolstatus->gcb_getbuffersize.commState = gcb_comm_result_succeed;
		m_pProtocolstatus->gcb_getbuffersize_val = PROTOCOL_MAKEDATA_DWORD(pCmdData);
		break;
	case cmd_gcb_init:
		//板卡初始化
		ASSERT(wCmdSize == 1);
		m_pProtocolstatus->gcb_init.commState = TRANS_COMMSTATE_GCB_TO_PC(pCmdData[0]);
		break;
	case cmd_gcb_print_ready:
		//打印准备
		ASSERT(wCmdSize == 1);
		m_pProtocolstatus->gcb_print_ready.commState = TRANS_COMMSTATE_GCB_TO_PC(pCmdData[0]);
		break;
	case cmd_gcb_repeater_config:
		//下级板卡配置指令（PC-->GCB-->板卡）
		ASSERT(wCmdSize == 1);
		m_pProtocolstatus->gcb_repeater_config.commState = TRANS_COMMSTATE_GCB_TO_PC(pCmdData[0]);
		break;
	case cmd_pc_repeater_config:
		//下级板卡配置指令（板卡-->GCB-->PC）
		Commu_SendMsg_confirmation(bCmd, gcb_pc_comm_result_succeed);		//即时反馈
		DataFrameAnalyze_repeater_config(pCmdData, wCmdSize);
		break;
	}
end:

	return dwResult;
}

//中继数据帧分析
//返回：被处理的缓冲数据个数
DWORD CCBSocketPort_Control::DataFrameAnalyze_repeater_config(const BYTE* pBuffer, DWORD dwBufferSize)
{
	DWORD dwResult = 0;
	if (m_pc_repeater_config_pFunc != NULL)
	{
		dwResult = m_pc_repeater_config_pFunc(m_pc_repeater_config_pFuncData, pBuffer, dwBufferSize);
	}
	return dwResult;
}

// 封装指令信息到数据帧中
//	pDstBuffer :[out]封装后的数据帧缓冲区，缓冲区大小 >= gcb_pt_data_len_max
int CCBSocketPort_Control::dataframe_transdata(BYTE* pDstBuffer, BYTE bCmd, BYTE bResultPC)
{
	BYTE bCmdData = bResultPC;
	return CCBSocketPort_Control::dataframe_transdata(pDstBuffer, bCmd, &bCmdData, 1);
}

int CCBSocketPort_Control::dataframe_transdata(BYTE* pDstBuffer, BYTE bCmd, const BYTE* pCmdData, WORD wCmdSize)
{
	if (pDstBuffer == NULL)
		return 0;

	int		nIndex;

	pDstBuffer[gcb_pt_data_start1] = gcb_protocol_dataframe_start1;
	pDstBuffer[gcb_pt_data_start2] = gcb_protocol_dataframe_start2;
	pDstBuffer[gcb_pt_data_cmd] = bCmd;
	pDstBuffer[gcb_pt_data_cmdsize1] = LOBYTE(wCmdSize);
	pDstBuffer[gcb_pt_data_cmdsize2] = HIBYTE(wCmdSize);
	nIndex = gcb_pt_data_startfixcount;
	if (wCmdSize > 0)
	{
		if (pCmdData == NULL)
			return 0;

		memcpy(pDstBuffer + gcb_pt_data_startfixcount, pCmdData, wCmdSize);
		nIndex += wCmdSize;
	}

	pDstBuffer[nIndex] = gcb_protocol_dataframe_end;
	nIndex++;

	return nIndex;
}
//反馈指令处理状态
//	bResult :[in]GCB_COMM_RESULT_PC
int CCBSocketPort_Control::Commu_SendMsg_confirmation(BYTE bCmd, BYTE bResult)
{
	BYTE	snd_msg[0xFF];		//即时反馈
	int		msg_len;

	BZERO(snd_msg);
	msg_len = CCBSocketPort_Control::dataframe_transdata(snd_msg, bCmd, &bResult, 1);
	ASSERT(msg_len > gcb_pt_data_len_min);
	return Commu_SendMsg_Direct(snd_msg, msg_len);
}

//得当前下位机返回的各种参数（状态）信息
const GCB_Protocol_Control_Statue *CCBSocketPort_Control::GetControlStatus() const
{
	return m_pProtocolstatus;
}

//对接收到的数据进行分析
//操作对象			： m_pBuffer, m_dwReceiveSize
//返回值：已处理的数据个数（<= m_dwReceiveSize），剩余部分留到下一次数据接收后处理
//			<= 0时表示未处理（当前数据帧未完全接收），
//			当当前数据均为无效数据时，也返回 m_dwReceiveSize
//备注：在实际应用时，此函数一般需重载后使用，
//	默认情况下是将当前缓冲m_pBuffer内容作为一次有效数据帧处理，返回值为 m_dwReceiveSize
//	在本函数中必须处理 m_bcommu_status 的信息，否则将影响 SendMsg_handshake(...)对指令有效性的判断
DWORD CCBSocketPort_Control::ReceiveDataEx()
{
	DWORD	dwResult = 0;
	DWORD	dwLen;
	BYTE	*pBuffer = m_pBuffer;
	BYTE	*pBufferEnd;
	BYTE	bFlag;

	if (m_pBuffer == NULL)
	{
		ASSERT(0);
		return 0;
	}
	pBufferEnd = pBuffer + m_dwReceiveSize;
	while (pBuffer < pBufferEnd)
	{
		dwLen = 1;			//默认处理1个字节
		bFlag = pBuffer[gcb_pt_data_start1];
		switch (bFlag)
		{
		case gcb_protocol_dataframe_start1:
			//数据帧起始
			//处理相关数据帧信息（仅处理一个有效帧）
			dwLen = DataFrameAnalyze(pBuffer, m_dwReceiveSize - (pBuffer - m_pBuffer));
			if (dwLen == 0)
			{//有效数据帧格式，但可能还未接收完整
				goto end;
			}
			break;
		default:
			break;
		}
		pBuffer += dwLen;
	}

end:
	dwResult = (pBuffer - m_pBuffer);

	if (debug_IsWriteReceiveInfo())
	{
		CString strIniFile;
		strIniFile = m_debug_strWriteSendInfoFile;
		if (strIniFile.IsEmpty())
		{
			strIniFile = g_AppPath.m_strDebug + "socket_control_debuginfo_receive.txt";
		}
		CSocketBase::Write_Communication_Info(strIniFile, m_pBuffer, dwResult, (32 * 1024), (16 * 1024), FALSE);
	}

	return dwResult;
}

//直接发送操作指令，一般仅在 ReceiveData 函数中使用
int CCBSocketPort_Control::Commu_SendMsg_Direct(const void* snd_msg, int msg_len)
{
	int nResult = 0;
	if (!IsOpen())
		return 0;

	nResult = CSocketBase::Commu_SendMsg_Direct(snd_msg, msg_len);

	return nResult;
}

//发送操作指令
int CCBSocketPort_Control::Commu_SendMsg(const void* snd_msg, int msg_len)
{
	int nResult = 0;
	if (!IsOpen())
		return 0;

	if (debug_IsWriteSendInfo())
	{
		CString strIniFile;
		strIniFile = m_debug_strWriteSendInfoFile;
		if (strIniFile.IsEmpty())
		{
			strIniFile = g_AppPath.m_strDebug + "socket_control_debuginfo.txt";
		}
		CSocketBase::Write_Communication_Info(strIniFile, (const BYTE*)snd_msg, msg_len, (256 * 1024), (32 * 1024), TRUE);
	}

	nResult = CSocketBase::Commu_SendMsg(snd_msg, msg_len);

	return nResult;
}


int CCBSocketPort_Control::Commu_SendMsg_handshake(GCB_COMMAND_STATE *pCmdState, const void *snd_msg, int msg_len, int time_check)
{
	DWORD	dwLast;
	int		nResult;

	ASSERT(pCmdState != NULL);
	ASSERT(snd_msg != NULL);
	ASSERT(msg_len > 0);

	nResult = shr_succeed;
	pCmdState->sendState = gcb_protocol_send_state_checking;
	pCmdState->commState = gcb_comm_result_init;
	if (msg_len != Commu_SendMsg(snd_msg, msg_len))
	{
		pCmdState->sendState = gcb_protocol_send_state_failed;
		nResult = shr_send_failed;
		goto end;
	}
	pCmdState->sendState = gcb_protocol_send_state_ok;
	if (time_check <= 0)
	{
		nResult = shr_succeed;
		goto end;
	}
	if (IsDemoMode())
	{
		nResult = shr_succeed;
		pCmdState->commState = gcb_comm_result_succeed;
		goto end;
	}

	nResult = shr_timeout;
	dwLast = ::GetTickCount();
	while (::GetTickCount() <= (dwLast + time_check))
	{
		switch (pCmdState->commState)
		{
		case gcb_comm_result_init:
			//初始化，继续等待
			break;
		case gcb_comm_result_succeed:
			nResult = shr_succeed;
			goto end;
			break;
		default:
		case gcb_comm_result_failed:
		case gcb_comm_result_repeater:
		case gcb_comm_result_other:
			nResult = shr_cmd_failed;
			goto end;
			break;
		}
	}
	//time out
	nResult = shr_timeout;

end:
	return nResult;
}

//版本
int CCBSocketPort_Control::sendorder_gcb_getversion(WORD *pwVersion, int time_check)
{
	if (pwVersion != NULL)
		*pwVersion = 0;

	if (!IsOpen())
		return shr_send_failed;

	int						nResult = shr_timeout;
	BYTE					*snd_msg = NULL;
	int						msg_len = 0;
	GCB_COMMAND_STATE	*pCmdState = &(m_pProtocolstatus->gcb_getversion);
	BYTE					bCmd = cmd_gcb_getversion;

	snd_msg = m_sendBuffer.LockBuffer(gcb_pt_data_len_max, TRUE);
	if (snd_msg == NULL)
		return shr_outofmemory;

	msg_len = CCBSocketPort_Control::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= gcb_pt_data_len_min);

	m_pProtocolstatus->gcb_getversion_val = 0;
	nResult = Commu_SendMsg_handshake(pCmdState, snd_msg, msg_len, time_check);
	if (nResult == shr_succeed)
	{
		ASSERT(time_check <= 0 || pCmdState->commState == gcb_comm_result_succeed);
		if (pwVersion != NULL)
		{
			if (pCmdState->commState == gcb_comm_result_succeed)
			{
				*pwVersion = m_pProtocolstatus->gcb_getversion_val;
			}
			else
			{//未正常获得版本信息
				nResult = shr_timeout;
			}
		}
	}

	m_sendBuffer.UnlockBuffer(snd_msg);

	return nResult;
}
int CCBSocketPort_Control::sendorder_gcb_getbuffersize(DWORD *pdwBufferSize, int time_check)
{
	if (pdwBufferSize != NULL)
		*pdwBufferSize = 0;

	if (!IsOpen())
		return shr_send_failed;

	int						nResult = shr_timeout;
	BYTE					*snd_msg = NULL;
	int						msg_len = 0;
	GCB_COMMAND_STATE	*pCmdState = &(m_pProtocolstatus->gcb_getbuffersize);
	BYTE					bCmd = cmd_gcb_getbuffersize;

	snd_msg = m_sendBuffer.LockBuffer(gcb_pt_data_len_max, TRUE);
	if (snd_msg == NULL)
		return shr_outofmemory;

	msg_len = CCBSocketPort_Control::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= gcb_pt_data_len_min);

	m_pProtocolstatus->gcb_getbuffersize_val = 0;
	nResult = Commu_SendMsg_handshake(pCmdState, snd_msg, msg_len, time_check);
	if (nResult == shr_succeed)
	{
		ASSERT(time_check <= 0 || pCmdState->commState == gcb_comm_result_succeed);
		if (pdwBufferSize != NULL)
		{
			if (pCmdState->commState == gcb_comm_result_succeed)
			{
				*pdwBufferSize = m_pProtocolstatus->gcb_getbuffersize_val;
			}
			else
			{//未正常获得版本信息
				nResult = shr_timeout;
			}
		}
	}

	m_sendBuffer.UnlockBuffer(snd_msg);

	return nResult;
}
//板卡初始化
int CCBSocketPort_Control::sendorder_gcb_init(BYTE bInitWay, int time_check)
{
	if (!IsOpen())
		return shr_send_failed;

	int						nResult = shr_timeout;
	BYTE					*snd_msg = NULL;
	int						msg_len = 0;
	GCB_COMMAND_STATE	*pCmdState = &(m_pProtocolstatus->gcb_init);
	BYTE					bCmd = cmd_gcb_init;

	snd_msg = m_sendBuffer.LockBuffer(gcb_pt_data_len_max, TRUE);
	if (snd_msg == NULL)
		return shr_outofmemory;

	msg_len = CCBSocketPort_Control::dataframe_transdata(snd_msg, bCmd, bInitWay);
	ASSERT(msg_len >= gcb_pt_data_len_min);

	nResult = Commu_SendMsg_handshake(pCmdState, snd_msg, msg_len, time_check);
	if (nResult == shr_succeed)
	{
		ASSERT(time_check <= 0 || pCmdState->commState == gcb_comm_result_succeed);
	}

	m_sendBuffer.UnlockBuffer(snd_msg);

	return nResult;
}
//打印准备
int CCBSocketPort_Control::sendorder_gcb_print_ready(int time_check)
{
	if (!IsOpen())
		return shr_send_failed;

	int						nResult = shr_timeout;
	BYTE					*snd_msg = NULL;
	int						msg_len = 0;
	GCB_COMMAND_STATE	*pCmdState = &(m_pProtocolstatus->gcb_print_ready);
	BYTE					bCmd = cmd_gcb_print_ready;

	snd_msg = m_sendBuffer.LockBuffer(gcb_pt_data_len_max, TRUE);
	if (snd_msg == NULL)
		return shr_outofmemory;

	msg_len = CCBSocketPort_Control::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= gcb_pt_data_len_min);

	nResult = Commu_SendMsg_handshake(pCmdState, snd_msg, msg_len, time_check);
	if (nResult == shr_succeed)
	{
		ASSERT(time_check <= 0 || pCmdState->commState == gcb_comm_result_succeed);
	}

	m_sendBuffer.UnlockBuffer(snd_msg);

	return nResult;
}

//下级板卡配置指令（PC-->GCB-->板卡）
int CCBSocketPort_Control::sendorder_gcb_repeater_config(void *pDataList, BYTE cmdSize, int time_check)
{
	if (!IsOpen())
		return shr_send_failed;

	int						nResult = shr_timeout;
	BYTE					*snd_msg = NULL;
	int						msg_len = 0;
	GCB_COMMAND_STATE	*pCmdState = &(m_pProtocolstatus->gcb_repeater_config);
	BYTE					bCmd = cmd_gcb_repeater_config;

	snd_msg = m_sendBuffer.LockBuffer(gcb_pt_data_len_max, TRUE);
	if (snd_msg == NULL)
		return shr_outofmemory;

	msg_len = CCBSocketPort_Control::dataframe_transdata(snd_msg, bCmd, (const BYTE*)pDataList, cmdSize);
	ASSERT(msg_len >= gcb_pt_data_len_min);

	nResult = Commu_SendMsg_handshake(pCmdState, snd_msg, msg_len, time_check);
	if (nResult == shr_succeed)
	{
		ASSERT(time_check <= 0 || pCmdState->commState == gcb_comm_result_succeed);
	}

	m_sendBuffer.UnlockBuffer(snd_msg);

	return nResult;
}

//下级板卡配置指令（板卡-->GCB-->PC）-- PC 反馈
int CCBSocketPort_Control::sendorder_confirmation_pc_repeater_config(void *pDataList, BYTE cmdSize)
{
	return sendorder_gcb_repeater_config(pDataList, cmdSize, 0);
}
