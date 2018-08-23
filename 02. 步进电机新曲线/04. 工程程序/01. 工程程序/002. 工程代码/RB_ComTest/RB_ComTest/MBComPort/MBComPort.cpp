
#include "../stdafx.h"
#include "Protocol.h"
#include "MBComPort.h"

//	bAll :[in]是否包括所有参数，FALSE时不初始化 wProtocolVersion
void Protocol_Control_Statue::Initialize(BOOL bAll) 
{
	memset(this, 0, sizeof(*this));

	// PC-->MB-->PC
	bCheckedWay			= protocol_queryway_failed;	//Check 正常连接状态，PROTOCOL_QUERY_WAY

	//if (bAll)
	//{
	//	wProtocolVersion	= protocol_version_none;	//下位机协议版本信息
	//}

	BZERO(bControlAmplitude);						//当前的喷头电压幅值设定
	BZERO(bControlPulsewidth);						//当前的喷头电压脉宽设定
	wThicknessPluse		= 0;						//当前的布厚参数设定值，脉冲数
	sprayparam.Initialize();						//散喷参数
	cleanheadparam.Initialize();					//打印中自动喷头清洗参数

	motoparam_general.Initialize();
	motoparam_x.Initialize();
	motoparam_y.Initialize();
	motoparam_z.Initialize();

	bQueryWay_ProtocolVersion	= protocol_queryway_failed;				//信息检索状态-下位机协议版本，PROTOCOL_QUERY_WAY
	bQueryWay_ControlAmplitude	= protocol_queryway_failed;				//信息检索状态-当前的喷头电压幅值设定，PROTOCOL_QUERY_WAY
	bQueryWay_ControlPulsewidth	= protocol_queryway_failed;				//信息检索状态-当前的喷头电压脉宽设定，PROTOCOL_QUERY_WAY
	bQueryWay_ControlThickness	= protocol_queryway_failed;				//信息检索状态-当前的布厚参数设定，PROTOCOL_QUERY_WAY
	bQueryWay_sprayparam		= protocol_queryway_failed;				//信息检索状态-当前散喷参数设定值，PROTOCOL_QUERY_WAY
	bQueryWay_cleanheadparam	= protocol_queryway_failed;				//信息检索状态-当前自动清洗参数设定值，PROTOCOL_QUERY_WAY

	bQueryWay_motoparam_general = protocol_queryway_failed;
	bQueryWay_motoparam_x		= protocol_queryway_failed;
	bQueryWay_motoparam_y		= protocol_queryway_failed;
	bQueryWay_motoparam_z		= protocol_queryway_failed;

	bConfigWay_ControlAmplitude	= protocol_configway_failed;			//MB反馈：参数设定状态-当前喷头电压幅值设定，PROTOCOL_CONFIG_WAY
	bConfigWay_ControlPulsewidth= protocol_configway_failed;			//MB反馈：参数设定状态-当前喷头电压脉宽设定，PROTOCOL_CONFIG_WAY
	bConfigWay_ControlThickness	= protocol_configway_failed;			//MB反馈：参数设定状态-当前布厚参数设定，PROTOCOL_CONFIG_WAY
	bConfigWay_sprayparam		= protocol_configway_failed;			//MB反馈：参数设定状态-当前散喷参数设定，PROTOCOL_CONFIG_WAY
	bConfigWay_cleanheadparam	= protocol_configway_failed;			//MB反馈：参数设定状态-当前自动清洗参数设定，PROTOCOL_CONFIG_WAY

	bConfigWay_motoparam_general= protocol_configway_failed;
	bConfigWay_motoparam_x		= protocol_configway_failed;
	bConfigWay_motoparam_y		= protocol_configway_failed;
	bConfigWay_motoparam_z		= protocol_configway_failed;

	bPrintWay_printstart		= protocol_printway_failed;				//MB反馈：打印指令处理状态-打印准备，PROTOCOL_PRINT_WAY
	bPrintWay_printline			= protocol_printway_failed;				//MB反馈：打印指令处理状态-打印一行，PROTOCOL_PRINT_WAY
	bPrintWay_printblock		= protocol_printway_failed;				//MB反馈：打印指令处理状态-打印一块，PROTOCOL_PRINT_WAY
	bPrintWay_printend			= protocol_printway_failed;				//MB反馈：打印指令处理状态-打印结束，PROTOCOL_PRINT_WAY

	bPrintWay_pc_reset			= protocol_printway_failed;				//MB反馈：打印指令处理状态-PC复位，PROTOCOL_PRINT_WAY
	bPrintWay_pc_pause			= protocol_printway_failed;				//MB反馈：打印指令处理状态-PC暂停，PROTOCOL_PRINT_WAY
	bPrintWay_pc_resume			= protocol_printway_failed;				//MB反馈：打印指令处理状态-PC继续，PROTOCOL_PRINT_WAY

	bPrintWay_print_back2origin	= protocol_printway_failed;				//MB反馈：打印指令处理状态-回到打印原点，PROTOCOL_PRINT_WAY
	bPrintWay_print_cleanhead	= protocol_printway_failed;				//MB反馈：打印指令处理状态-执行一次自动清洗操作，PROTOCOL_PRINT_WAY
	bPrintWay_print_spray		= protocol_printway_failed;				//MB反馈：打印指令处理状态-执行一次散喷操作，PROTOCOL_PRINT_WAY

	// MB-->PC-->MB
	bSignal_mb_reset	= 0;											//接收到MB复位指令
	bSignal_mb_resume	= 0;											//接收到MB继续指令
	bSignal_mb_pause	= 0;											//接收到MB暂停指令
	bSignal_mb_syserror = 0;											//接收到MB系统错误指令
	bSignal_mb_sysstatus= mb_system_normal;								//接收到MB系统异常指令
	bSignal_mb_cleanhead= 0;											//接收到MB喷头清洗指令

	//SF
	bQueryWay_SF_GetExistentSpeedlevel				= comm_result_para_invalid;
	bExistSpeedLevel = 0;
	bConfigWay_SF_DownloadOnlineTimeTableParameter	= comm_result_para_invalid;
	bConfigWay_SF_DownloadTimeTableValue			= comm_result_para_invalid;
	bConfigWay_SF_DownloadOfflineTimeTableParameter = comm_result_para_invalid;
	bQueryWay_SF_GetOnlineTimeTableParameter		= comm_result_para_invalid;
	bQueryWay_SF_GetOfflineTimeTableParameter		= comm_result_para_invalid;
	bPrintWay_SF_CountFullPulse				= comm_result_para_invalid;
	bPrintWay_SF_MotorMoveRelative			= comm_result_para_invalid;
	bPrintWay_SF_MotorMoveAbsolute			= comm_result_para_invalid;
	bQueryWay_SF_MotorGetPosition			= comm_result_para_invalid;
	dwCurrentPosition = 0;
	bPrintWay_SF_MotorMoveSpecialPosition	= comm_result_para_invalid;
	bPrintWay_SF_MotorMoveStop				= comm_result_para_invalid;
	bPrintWay_SF_MotorMove					= comm_result_para_invalid;
	bQueryWay_SF_GetFullPulse				= comm_result_para_invalid;
	dwFullPulse = 0;
	bQueryWay_SF_GetSpecialPosition			= comm_result_para_invalid;
	dwSpecialPosition = 0;
	bConfigWay_SF_SetSpecialPosition		= comm_result_para_invalid;
	bConfigWay_SF_DeleteAllTimeTableData	= comm_result_para_invalid;

	bConfigWay_SF_SetFirmParameterX			= comm_result_para_invalid;
	bConfigWay_SF_SetFirmParameterY			= comm_result_para_invalid;
	bConfigWay_SF_SetFirmParameterZ			= comm_result_para_invalid;

	bQueryWay_SF_GetFirmParameterX			= comm_result_para_invalid;
	bQueryWay_SF_GetFirmParameterY			= comm_result_para_invalid;
	bQueryWay_SF_GetFirmParameterZ			= comm_result_para_invalid;

	BZERO(sf_board_parameter_x);
	BZERO(sf_board_parameter_y);
	BZERO(sf_board_parameter_z);
	
	bQueryWay_SF_GetTimeTableVitalParameter = comm_result_para_invalid;
	dwVitalParaAecPulse = 0;
	dwVitalParaDecPulse = 0;
	dwVitalEvenT = 0;

	bPrintWay_SF_MotorXY_MoveRelative		= comm_result_para_invalid;
	bPrintWay_SF_MotorXY_MoveAbsolute		= comm_result_para_invalid;
	bPrintWay_SF_MotorXY_MoveSpecialPosition = comm_result_para_invalid;
	bPrintWay_SF_MotorXY_PrintLine			= comm_result_para_invalid;

	//MB
	bConfigWay_MB_OnlineState = mb_comm_result_failed;
	//bConfigWay_MB_CleanHead = mb_comm_result_failed;
	//bQueryWay_MB_CleanHead = mb_comm_result_failed;
	//bPrintWay_MB_CleanHead = mb_comm_result_failed;
	//BZERO(mb_clean_head_parameter);

	bPrintWay_MB_PrintStart		= mb_comm_result_failed;
	bPrintWay_MB_PrintLine		= mb_comm_result_failed;
	bPrintWay_MB_PrintEnd		= mb_comm_result_failed;
	bPrintWay_MB_PrintPause		= mb_comm_result_failed;
	bPrintWay_MB_PrintResume	= mb_comm_result_failed;
	bPrintWay_MB_PrintStop		= mb_comm_result_failed;

	bPrintWay_MB_MoveAbsolutePosition = mb_comm_result_failed;
	bPrintWay_MB_MoveRelativePosition = mb_comm_result_failed;
	bPrintWay_MB_Move2SpecialPosition = mb_comm_result_failed;
	bPrintWay_MB_StoreCurrentPosition = mb_comm_result_failed;

	//板卡
	bConfigWay_BoardCard_SetInfo = mb_comm_result_failed;
	bQueryWay_BoardCard_GetInfo = mb_comm_result_failed;
	BZERO(board_card_info);
	
	bConfigWay_BoardCard_SetAddr = mb_comm_result_failed;
	bQueryWay_BoardCard_GetAddr = mb_comm_result_failed;
	bBoardCardAddr = 0;
	
	//供墨
	bConfigWay_Ink_Check485		= mb_comm_result_failed;
	bQueryWay_Ink_CheckState	= mb_comm_result_failed;
	bInkState = 0;
	wInkState = 0;
	bConfigWay_Ink_Force		= mb_comm_result_failed;
	bConfigWay_Ink_Press		= mb_comm_result_failed;
	bConfigWay_Ink_Scrape		= mb_comm_result_failed;
	bConfigWay_Ink_Moisturize	= mb_comm_result_failed;
	bConfigWay_Ink_CycleMotor	= mb_comm_result_failed;

	//收布
	bQueryWay_RC_CheckState = mb_comm_result_failed;
	bRcState = 0;
	bConfigWay_RC_Pass		= mb_comm_result_failed;
	bConfigWay_RC_PrintStop = mb_comm_result_failed;
	bConfigWay_RC_PrintOver = mb_comm_result_failed;
	
	//送布
	bQueryWay_SC_CheckState = mb_comm_result_failed;
	bScState0 = 0;
	bScState1 = 0;
	bConfigWay_SC_BandDry	= mb_comm_result_failed;
	bConfigWay_SC_PreDry	= mb_comm_result_failed;
	bConfigWay_SC_Stop		= mb_comm_result_failed;
	bConfigWay_SC_CancleWrongLabel	= mb_comm_result_failed;

	//纠偏
	bConfigWay_CB_AutoCorrect		= mb_comm_result_failed;
	bConfigWay_CB_ManualCorrect		= mb_comm_result_failed;
	bConfigWay_CB_ManualCorrectStop	= mb_comm_result_failed;
	
	//COM
	bQueryWay_COM_GetBoardCondition = mb_comm_result_failed;
	wComBoardCondition = 0;
	bPrintWay_COM_Motor_MoveRelative = mb_comm_result_failed;

	//LD
	bQueryWay_ld_all_power			= ld_comm_result_failed;
	bConfigWay_ld_power_off			= ld_comm_result_failed;
	bConfigWay_ld_check_power		= ld_comm_result_failed;
	bConfigWay_ld_all_power			= ld_comm_result_failed;
	bPrintWay_ld_voltage			= ld_comm_result_failed;
	bPrintWay_ld_check_power		= ld_comm_result_failed;
	bConfigWay_ld_write_config		= ld_comm_result_failed;
	bConfigWay_ld_write_data		= ld_comm_result_failed;
	bConfigWay_ld_read_config		= ld_comm_result_failed;
	bConfigWay_ld_light_on			= ld_comm_result_failed;
	bConfigWay_ld_light_off			= ld_comm_result_failed;
	bConfigWay_ld_light_on_time		= ld_comm_result_failed;
	bQueryWay_ld_get_current_power	= ld_comm_result_failed;

	//map data
	bConfigWay_map_data				= ld_comm_result_failed;
	bQueryWay_map_data				= ld_comm_result_failed;

}

CCommunicationPort::CCommunicationPort(void)
{
	m_protocolstatus.Initialize(TRUE);

	m_socket_Control.Init(TRUE);
	m_socket_Data.Init(TRUE);
	m_socket_bEnable = FALSE;
	m_socket_pThread = NULL;

	Init(FALSE);

	//是否输出Debug信息（写文件）
	m_debug_bWriteSendInfo		= FALSE;
	m_debug_bWriteReceiveInfo	= FALSE;

	//m_printer_plugin_proc		= NULL;
	m_printer_plugin_bChannels	= _PLUGIN_MAX_COLOR_DEFAULT_;
	m_strSendData = "";
	m_strReciveData = "";

	socket_init(FALSE);
}

CCommunicationPort::~CCommunicationPort(void)
{
	Close(TRUE);
}

//打印设备驱动模块（插件）信息
//	infoProc		:[in]获取插件信息的函数地址
//	bChannels		:[in]设备支持的颜色通道数， 小于等于 _PLUGIN_MAX_COLOR_LIMIT_
//void CCommunicationPort::SetPrinterPluginInfo(IS_PRINTER_PLUGIN_PROC infoProc, BYTE bChannels)
//{
//	m_printer_plugin_proc		= infoProc;
//	
//	if (bChannels < 4)
//		bChannels = _PLUGIN_MAX_COLOR_DEFAULT_;
//	else if (bChannels > _PLUGIN_MAX_COLOR_LIMIT_)
//		bChannels = _PLUGIN_MAX_COLOR_LIMIT_;
//	m_printer_plugin_bChannels	= bChannels;
//
//}

// port initialisation
//	nBufferSize :[in]初始化接收缓冲区大小
//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
BOOL CCommunicationPort::Open(UINT portnr, DWORD dwBaud, int nBufferSize, int nGrowSize)
{
	Init(FALSE);
	if (CComBase::Open(portnr, dwBaud, nBufferSize, nGrowSize))
	{
		//Sleep(100);
		//sendorder_check(0);
		//Sleep(100);
		//sendorder_getverion(0);
		//Sleep(500);
		return TRUE;
	}

	m_socket_bEnable = FALSE;

	return FALSE;
}
BOOL CCommunicationPort::Close(BOOL bReleaseBuffer)
{
	BOOL bResult;

	socket_Close(bReleaseBuffer);

	bResult = CComBase::Close(bReleaseBuffer);

	Init(FALSE);

	return bResult;
}

BOOL CCommunicationPort::IsOpen() const
{
	if (socket_IsEnable())
		return socket_IsOpen();

	return CComBase::IsOpen();
}

//对接收到的数据进行分析
//操作对象			： m_pBuffer, m_dwReceiveSize
//返回值：已处理的数据个数（<= m_dwReceiveSize），剩余部分留到下一次数据接收后处理
//			<= 0时表示未处理（当前数据帧未完全接收），
//			当当前数据均为无效数据时，也返回 m_dwReceiveSize
//备注：在实际应用时，此函数一般需重载后使用，
//	默认情况下是将当前缓冲m_pBuffer内容作为一次有效数据帧处理，返回值为 m_dwReceiveSize
//	在本函数中必须处理 m_bcommu_status 的信息，否则将影响 SendMsg_handshake(...)对指令有效性的判断
DWORD CCommunicationPort::ReceiveDataEx()
{
	DWORD	dwResult = 0;
	DWORD	dwLen;
	BYTE	*pBuffer = m_pBuffer;
	BYTE	*pBufferEnd;
	BYTE	bFlag;

	if (m_pBuffer==NULL)
	{
		ASSERT(0);
		return 0;
	}
	pBufferEnd = pBuffer + m_dwReceiveSize;
	while(pBuffer < pBufferEnd)
	{
		dwLen = 1;			//默认处理1个字节
		bFlag = pBuffer[pt_data_start0];
		switch(bFlag)
		{
		case protocol_command_ok:
			//OK帧
			m_bcommu_status = rpComBase::commu_status_succeed;
			break;
		case protocol_command_fail:
			//Fail帧
			m_bcommu_status = rpComBase::commu_status_failed;
			break;
		case protocol_command_check:
			//Check帧
			//单字节指令，强制设置为OK
			m_protocolstatus.bCheckedWay = protocol_queryway_ok;
			break;
		case protocol_dataframe_start1:
			//数据帧起始
			if (pBuffer[pt_data_start1] != protocol_dataframe_start2)
			{//非有效数据帧
				break;
			}
			//处理相关数据帧信息（仅处理一个有效帧）
			m_receivedataex_nReturnFlagLen = 0;
			if (m_nProtocolMode==debug_mode)
			{
				dwLen = DataFrameAnalyze(pBuffer, m_dwReceiveSize-(pBuffer-m_pBuffer));
			}
			else
			{//仅对RAS板的打印模式
				dwLen = DataFrameAnalyze_PrintMode(pBuffer, m_dwReceiveSize-(pBuffer-m_pBuffer));
			}
			if (dwLen==0)
			{//有效数据帧格式，但可能还未接收完整
				goto end;
			}
			//if (m_receivedataex_nReturnFlagLen > 0)
			//{//发送反馈信息
			//	TRACE1("\t-- %.2X --\n", m_receivedataex_bReturnFlag[0]);
			//	Commu_SendMsg_Direct(m_receivedataex_bReturnFlag, m_receivedataex_nReturnFlagLen);
			//	m_receivedataex_nReturnFlagLen = 0;
			//}
			break;
		default:
			break;
		}

		pBuffer += dwLen;
	}

end:
	dwResult = (pBuffer-m_pBuffer);

	if (dwResult>0)
	{
		m_strReciveData = "";
		for (int i=0; i<dwResult; i++)
		{
			CString str;
			str.Format("%02X ",m_pBuffer[i]);
			m_strReciveData += str;
		}
		m_strReciveData += "\r\n";

		::PostMessage(m_pComWnd->m_hWnd,WM_COMM_RXCHAR,(WPARAM)(&m_strReciveData), 0);
	}
	//if (debug_IsWriteReceiveInfo())
	//{
	//	if (m_printer_plugin_proc != NULL)
	//	{
	//		PRINTER_PLUGIN_INFO pluginInfo;
	//		if (m_printer_plugin_proc(&pluginInfo))
	//		{
	//			CString strIniFile;
	//			strIniFile.Format("%s%s_debuginfo_receive.txt", g_AppPath.m_strDebug, pluginInfo.szName);
	//			CComBase::Write_Communication_Info(strIniFile, m_pBuffer, dwResult, 0, 0, FALSE);
	//		}
	//	}
	//}
	return dwResult;
}

//直接发送操作指令，一般仅在 ReceiveData 函数中使用
BOOL CCommunicationPort::Commu_SendMsg_Direct(const BYTE* snd_msg, int msg_len)
{
	BOOL bResult = FALSE;
	if (!IsOpen())
		return FALSE;

	if (socket_IsEnable())
	{
		if (shr_succeed==m_socket_Control.sendorder_confirmation_pc_repeater_config((void*)snd_msg, msg_len))
		{
			bResult = TRUE;
		}
	}
	else
	{
		bResult = CComBase::Commu_SendMsg_Direct(snd_msg, msg_len);

	}
	return bResult;
}

//发送操作指令
BOOL CCommunicationPort::Commu_SendMsg(const BYTE* snd_msg, int msg_len)
{
	BOOL bResult = FALSE;
	if (!IsOpen())
		return FALSE;

	if (debug_IsWriteSendInfo())
	{
		CString strIniFile;
		strIniFile.Format("%s%s_debuginfo_send.txt", g_AppPath.m_strDebug, "output"/*pluginInfo.szName*/);
		CComBase::Write_Communication_Info(strIniFile, snd_msg, msg_len, (64*1024), (32*1024), TRUE);
	}

	if (msg_len>0)
	{
		m_strSendData = "";
		for (int i=0; i<msg_len; i++)
		{
			CString str;
			str.Format("%02X ",snd_msg[i]);
			m_strSendData += str;
		}
		m_strSendData += "\r\n";

		::PostMessage(m_pComWnd->m_hWnd,WM_COMM_TXCHAR, (WPARAM)(&m_strSendData), 0);
	}

	if (socket_IsEnable())
	{
		if (shr_succeed==m_socket_Control.sendorder_confirmation_pc_repeater_config((void*)snd_msg, msg_len))
		{
			bResult = TRUE;
		}
	}
	else
	{
		bResult = CComBase::Commu_SendMsg(snd_msg, msg_len);
	}


	return bResult;
}

//操作指令串发送
//参数定义：
//snd_msg, s_msg_len	:[in]发送的指令串及长度
//timeout				:[in]发送指令的总延时值 ms
//time_repeatsend		:[in]当发送失败时，重发指令口串的时间间隔（累积值在 timeout范围内） ms
//sendmsg_level			:[in] SENDMSG_LEVEL
//返回值：SENDMSG_STATUS
int CCommunicationPort::SendMsg_handshake( BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, int sendmsg_level)
{
	int nResult = rpComBase::sms_err_send;
	if (!IsOpen())
		return nResult;

	m_sendMsg_isInLoop = true;
	nResult = CComBase::SendMsg_handshake(snd_msg, s_msg_len, timeout, time_repeatsend, sendmsg_level);
	m_sendMsg_isInLoop = false;
	return nResult;
}

int CCommunicationPort::SendMsg_handshake0( BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, int sendmsg_level, BOOL bFlag)
{
	int nResult = rpComBase::sms_err_send;
	if (!IsOpen())
		return nResult;

	m_sendMsg_isInLoop = true;
	nResult = CComBase::SendMsg_handshake0(snd_msg, s_msg_len, timeout, time_repeatsend, sendmsg_level, bFlag);
	m_sendMsg_isInLoop = false;
	return nResult;
}

//操作指令串发送
//snd_msg, s_msg_len	:[in]发送的指令串及长度
//timeout				:[in]发送指令的总延时值 ms
//time_repeatsend		:[in]当发送失败时，重发指令口串的时间间隔（累积值在 timeout范围内） ms
//pFlag					:[in/out]指令发送状态标识（接收到相应信息时被自动赋值）
//pFlagCheck,nCheckCount:[in]指定函数退出条件，除 TimeOut外，仅当  *pFlag==pFlagCheck[i] 时才返回
//time_check			:[in]函数退出条件(pFlagCheck[i])判断的超时设置，ms
int CCommunicationPort::SendMsg_handshake(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check)
{
	int nResult = rpComBase::sms_err_send;
	if (!IsOpen())
		return nResult;

	m_sendMsg_isInLoop = true;
	nResult = CComBase::SendMsg_handshake(snd_msg, s_msg_len, timeout, time_repeatsend, pFlag, pFlagCheck, nCheckCount, time_check);
	m_sendMsg_isInLoop = false;
	return nResult;
}

int CCommunicationPort::SendMsg_handshake0(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check, BOOL bFlag)
{
	int nResult = rpComBase::sms_err_send;
	if (!IsOpen())
		return nResult;

	m_sendMsg_isInLoop = true;
	nResult = CComBase::SendMsg_handshake0(snd_msg, s_msg_len, timeout, time_repeatsend, pFlag, pFlagCheck, nCheckCount, time_check, bFlag);
	m_sendMsg_isInLoop = false;
	return nResult;
}
//清接收缓冲区
void CCommunicationPort::ClearReadBuffer()
{
	CComBase::ClearReadBuffer();
}

//重设指令发送状态信息
void CCommunicationPort::ResetCommu_Status_Flags()
{
	CComBase::ResetCommu_Status_Flags();
}

//初始化相关参数
void CCommunicationPort::Init(BOOL bReAllocBuffer)
{
	CComBase::Init(bReAllocBuffer);

	m_protocolstatus.Initialize(FALSE);					//由下位机返回的各种参数（状态）信息

	BZERO(m_receivedataex_bReturnFlag);
	m_receivedataex_nReturnFlagLen = 0;				//默认不反馈信息给MB
	
	m_nProtocolMode = debug_mode;
}

//是否输出Debug信息（写文件）
BOOL CCommunicationPort::debug_IsWriteSendInfo() const
{
	return m_debug_bWriteSendInfo;
}
BOOL CCommunicationPort::debug_IsWriteReceiveInfo() const
{
	return m_debug_bWriteReceiveInfo;
}
void CCommunicationPort::debug_WriteSendInfo(BOOL bWrite)
{
	m_debug_bWriteSendInfo = bWrite;

	m_socket_Control.debug_WriteSendInfo(NULL, bWrite);
}
void CCommunicationPort::debug_WriteReceiveInfo(BOOL bWrite)
{
	m_debug_bWriteReceiveInfo = bWrite;
	m_socket_Control.debug_WriteReceiveInfo(NULL, bWrite);
}

//计算 CRC 值
BYTE CCommunicationPort::CalcCRC(const BYTE *pBuffer, DWORD dwSize)
{
	BYTE bCRC = 0x00;
	DWORD i;
	for (i=0; i<dwSize; i++)
	{
		bCRC += pBuffer[i];
	}
	return bCRC;
}


// 数据帧分析（仅处理一个有效帧）
//返回：被处理的缓冲数据个数
DWORD CCommunicationPort::DataFrameAnalyze(const BYTE* pBuffer, DWORD dwBufferSize)
{
	BYTE	bCmd;
	BYTE	bCmdSize;
	DWORD	dwFrameLen;
	BYTE	bCRC;
	DWORD	dwEndFlagPos;		//数据帧结束标识索引
	DWORD	dwCRCDataPos;		//数据帧CRC校验位索引
	DWORD	dwResult = 0;
	int		nIndex;
	BYTE	bSrcAddr = addr_unknown;
	BYTE	bDstAddr = addr_unknown;
	BYTE	bCmdCount;
	CString str;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	m_receivedataex_nReturnFlagLen = 0;		//默认不反馈信息给MB

	if (pBuffer==NULL || dwBufferSize==0)
		return 0;

	if (dwBufferSize < pt_data_len_min)
		return 0;	//参数不全，达不到最小长度要求

	if (pBuffer[pt_data_start0] != protocol_dataframe_start1
		|| pBuffer[pt_data_start1] != protocol_dataframe_start2)
	{//非有效数据帧，第一个字节标识为无效值
		ASSERT(0);
		dwResult = 1;
		return dwResult;
	}

	bSrcAddr		= pBuffer[pt_data_add_src];
	bDstAddr		= pBuffer[pt_data_add_dst];
	bCmd			= pBuffer[pt_data_cmd];
	bCmdCount		= pBuffer[pt_data_cmdcount];
	bCmdSize		= pBuffer[pt_data_cmdsize];
	dwEndFlagPos	= pt_data_startfixcount;
	dwEndFlagPos	+= bCmdSize;
	dwFrameLen		= dwEndFlagPos + pt_data_endfixcount;
	dwCRCDataPos	= dwFrameLen - 1;

	if (dwBufferSize < dwFrameLen)
	{//指令串不全
		dwResult = 0;
		goto end;		
	}

	if (pBuffer[dwEndFlagPos] != protocol_dataframe_end)
	{//非有效结束标识，前2个字节标识为无效值
dataerror:
		m_receivedataex_bReturnFlag[0] = protocol_command_fail;
		m_receivedataex_nReturnFlagLen = 1;
		dwResult = 2;
		goto end;
	}
	//计算CRC校验位值
	bCRC = CalcCRC(pBuffer, dwCRCDataPos);
	if (pBuffer[dwCRCDataPos] != bCRC)
	{//检验位出错，前2个字节标识为无效值
		goto dataerror;
	}

	if (bDstAddr != addr_pc && bDstAddr != addr_gcb && bDstAddr != addr_mb)
	{//非有效地址 必须是发给PC的
		return dwFrameLen;
	}

	//有效参数数据起始
	pBuffer += pt_data_startfixcount;
	dwResult = dwFrameLen;
	
	switch(bCmd)
	{
	case cmd_card_setinfo:
		m_protocolstatus.bConfigWay_BoardCard_SetInfo = *pBuffer;
		break;
	case cmd_card_queryinfo:
		m_protocolstatus.board_card_info.m_wDeviceVersion = PROTOCOL_MAKEDATA_WORD(pBuffer);
		pBuffer+=2;
		m_protocolstatus.board_card_info.m_wProtocolVersion = PROTOCOL_MAKEDATA_WORD(pBuffer);
		pBuffer+=2;
		m_protocolstatus.board_card_info.m_dwSN = PROTOCOL_MAKEDATA_DWORD(pBuffer);
		pBuffer+=4;
		m_protocolstatus.board_card_info.m_dwDate = PROTOCOL_MAKEDATA_DWORD(pBuffer);
		pBuffer+=4;
		m_protocolstatus.board_card_info.m_dwProducerID = PROTOCOL_MAKEDATA_DWORD(pBuffer);
		pBuffer+=4;
		m_protocolstatus.board_card_info.m_dwCheckerID = PROTOCOL_MAKEDATA_DWORD(pBuffer);

		m_protocolstatus.bQueryWay_BoardCard_GetInfo = mb_comm_result_succeed;
		break;
	case cmd_card_setaddr:
		m_protocolstatus.bConfigWay_BoardCard_SetAddr = *pBuffer;
		break;
	case cmd_card_query:
		pBuffer+=4;
		m_protocolstatus.bBoardCardAddr = *pBuffer;
		m_protocolstatus.bQueryWay_BoardCard_GetAddr = mb_comm_result_succeed;
		break;
	}
	
	if (bSrcAddr==addr_raster)
	{
		switch (bCmd)
		{
		case cmd_ras_print_stop:
		case cmd_ras_print_pause:
		case cmd_ras_print_resume:
			::PostMessage(m_pRasDlg->m_hWnd,WM_RASSTATE, (WPARAM)(bCmd), 0);
			break;
		case cmd_ras_sys_error:
			m_protocolstatus.dwErrorCode = PROTOCOL_MAKEDATA_DWORD(pBuffer);
			str.Format("系统错误:%X",m_protocolstatus.dwErrorCode);
			AfxMessageBox(str);
			::PostMessage(m_pRasDlg->m_hWnd,WM_RASSTATE, (WPARAM)(bCmd), 0);
			break;
		case cmd_ras_sys_exception:
			m_protocolstatus.dwException = PROTOCOL_MAKEDATA_DWORD(pBuffer);
			str.Format("系统异常:%X",m_protocolstatus.dwException);
			AfxMessageBox(str);
			::PostMessage(m_pRasDlg->m_hWnd,WM_RASSTATE, (WPARAM)(bCmd), 0);
			break;
		case cmd_ras_print_start:
			m_protocolstatus.bPrintWay_RAS_PrintStart = *pBuffer;
			break;
		case cmd_ras_print_line:
			m_protocolstatus.bPrintWay_RAS_PrintLine = *pBuffer;
			break;
		case cmd_ras_print_end:
			m_protocolstatus.bPrintWay_RAS_PrintEnd = *pBuffer;
			break;
		case cmd_ras_select:
			m_protocolstatus.bPrintWay_RAS_Select = *pBuffer;
			break;
		case cmd_ras_print_stop_gcb:
			m_protocolstatus.bPrintWay_RAS_PrintStop = *pBuffer;
			break;
		case cmd_ras_getposition:
			{
				BZERO(m_protocolstatus.positionPara);
				m_protocolstatus.positionPara.pos_x_min = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.positionPara.pos_x_max = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.positionPara.pos_x_clean = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.positionPara.pos_x_moisture = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.positionPara.pos_y_min = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.positionPara.pos_y_max = PROTOCOL_MAKEDATA_DWORD(pBuffer);

				m_protocolstatus.bPrintWay_RAS_GetPosition = mb_comm_result_succeed;
			}
			break;
		case cmd_ras_getspeed:
			{
				BZERO(m_protocolstatus.onlinePara);
				m_protocolstatus.nSpeedLevelNum = *pBuffer;
				pBuffer++;
				if (m_protocolstatus.nSpeedLevelNum>10)
				{
					m_protocolstatus.nSpeedLevelNum = 10;
				}
				for (int i=0;i<m_protocolstatus.nSpeedLevelNum;i++)
				{
					m_protocolstatus.onlinePara[i].dist_acce = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
					m_protocolstatus.onlinePara[i].dist_dece = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
					m_protocolstatus.onlinePara[i].speed = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
				}
				m_protocolstatus.bPrintWay_RAS_GetSpeed = mb_comm_result_succeed;
			}
			break;
		case cmd_ras_move_absolute_position:
			m_protocolstatus.bPrintWay_RAS_Move2Position = *pBuffer;
			break;
		}
		goto finished;
	}
	else if (bSrcAddr==addr_correct)
	{
		switch (bCmd)
		{
		case cmd_cb_auto_correct:
			m_protocolstatus.bConfigWay_CB_AutoCorrect = *pBuffer;
			break;
		case cmd_cb_manual_correct:
			m_protocolstatus.bConfigWay_CB_ManualCorrect = *pBuffer;
			break;
		case cmd_cb_manual_correct_stop:
			m_protocolstatus.bConfigWay_CB_ManualCorrectStop = *pBuffer;
			break;
		}
		goto finished;
	}
	else if (bSrcAddr==addr_sec_cloth)
	{
		switch(bCmd)
		{
		case cmd_sc_new_sendcloth_check_state:
			m_protocolstatus.bScState0 = *pBuffer;
			pBuffer++;
			m_protocolstatus.bScState1 = *pBuffer;
			m_protocolstatus.bPrintWay_SC_ComState = comm_result_succeed;
			break;
		case cmd_sc_new_get_temperature:
	        m_protocolstatus.dwTemperature = PROTOCOL_MAKEDATA_DWORD(pBuffer);
            m_protocolstatus.bPrintWay_SC_ComState = comm_result_succeed;
			break;
		case cmd_sc_new_band_dry:
		case cmd_sc_new_band_wash:
		case cmd_sc_new_pre_dry:
		case cmd_sc_new_dd_band_wash:
		case cmd_sc_new_press_cloth:
		case cmd_sc_new_cloth_roller:
		case cmd_sc_new_cancle_wrong_lable:
		case cmd_sc_new_manual_correct:
		case cmd_sc_new_manual_correct_stop:
			m_protocolstatus.bPrintWay_SC_ComState = *pBuffer;
			break;
		}
		goto finished;
	}
	else if (bSrcAddr==addr_rec_cloth)
	{
		switch(bCmd)
		{
		case cmd_rc_servo_motor_move_distance:
		case cmd_rc_up_down_motor_move:
		case cmd_rc_servo_motor_move:
		case cmd_rc_heater_con:
		case cmd_rc_cloth_motor_con:
		case cmd_rc_up_down_motor_reset:
			m_protocolstatus.bPrintWay_RC_ComState = *pBuffer;
			break;
		}
		goto finished;
	}
	else if (bSrcAddr==addr_ink)
	{
		switch(bCmd)
		{
		case cmd_ink_check_485:
			m_protocolstatus.bConfigWay_Ink_Check485 = *pBuffer;
			break;
		case cmd_ink_check_state:
			m_protocolstatus.bInkState = *pBuffer;
			pBuffer++;
			m_protocolstatus.wInkState = PROTOCOL_MAKEDATA_WORD(pBuffer);
			m_protocolstatus.bQueryWay_Ink_CheckState = mb_comm_result_succeed;
			break;
		case cmd_ink_force:
			m_protocolstatus.bConfigWay_Ink_Force = *pBuffer;
			break;
		case cmd_ink_press:
			m_protocolstatus.bConfigWay_Ink_Press = *pBuffer;
			break;
		case cmd_ink_scrape:
			m_protocolstatus.bConfigWay_Ink_Scrape = *pBuffer;
			break;
		case cmd_ink_moisturize:
			m_protocolstatus.bConfigWay_Ink_Moisturize = *pBuffer;
			break;
		case cmd_ink_cycle_motor:
			m_protocolstatus.bConfigWay_Ink_CycleMotor = *pBuffer;
			break;
		}
		goto finished;
	}
	else if (bSrcAddr==addr_mb)
	{
		switch (bCmd)
		{
		case cmd_gcb_online_state:
			mb_confirmation_online_state();
			break;
		case cmd_mb_print_start:
			m_protocolstatus.bPrintWay_MB_PrintStart = *pBuffer;
			break;
		case cmd_mb_print_line:
			m_protocolstatus.bPrintWay_MB_PrintLine = *pBuffer;
			break;
		case cmd_mb_print_end:
			m_protocolstatus.bPrintWay_MB_PrintEnd = *pBuffer;
			break;
		case cmd_mb_print_pause_gcb:
			m_protocolstatus.bPrintWay_MB_PrintPause = *pBuffer;
			break;
		case cmd_mb_print_resume_gcb:
			m_protocolstatus.bPrintWay_MB_PrintResume = *pBuffer;
			break;
		case cmd_mb_print_stop_gcb:
			m_protocolstatus.bPrintWay_MB_PrintStop = *pBuffer;
			break;
		case cmd_mb_move_absolute_position:
			m_protocolstatus.bPrintWay_MB_MoveAbsolutePosition = *pBuffer;
			break;
		case cmd_mb_move_relative_position:
			m_protocolstatus.bPrintWay_MB_MoveRelativePosition = *pBuffer;
			break;
		case cmd_mb_move_to_special_position:
			m_protocolstatus.bPrintWay_MB_Move2SpecialPosition = *pBuffer;
			break;
		case cmd_mb_store_current_position:
			m_protocolstatus.bPrintWay_MB_StoreCurrentPosition = *pBuffer;
			break;
		}
		goto finished;
	}
	else if (bSrcAddr==addr_com)
	{
		switch (bCmd)
		{
		case cmd_com_get_board_condition:
			m_protocolstatus.wComBoardCondition = PROTOCOL_MAKEDATA_WORD(pBuffer);
			m_protocolstatus.bQueryWay_COM_GetBoardCondition = mb_comm_result_succeed;
			break;
		}
		goto finished;
	}
	else if (bSrcAddr==addr_servo_x||bSrcAddr==addr_servo_y || bSrcAddr==addr_servo_three_axis)
	{
		if (bSrcAddr == addr_servo_three_axis)
		{//三轴伺服控制板
			
			//多轴联动指令处理
			switch(bCmd)
			{
			case cmd_sf_motor_xy_move_relative:
				m_protocolstatus.bPrintWay_SF_MotorXY_MoveRelative = *pBuffer;
				break;
			case cmd_sf_motor_xy_move_absolute:
				m_protocolstatus.bPrintWay_SF_MotorXY_MoveAbsolute = *pBuffer;
				break;
			case cmd_sf_motor_xy_move_special_position:
				m_protocolstatus.bPrintWay_SF_MotorXY_MoveSpecialPosition = *pBuffer;
				break;
			case cmd_sf_motor_xy_print_line:
				m_protocolstatus.bPrintWay_SF_MotorXY_PrintLine		= *pBuffer;
				break;
			default:
				//单轴控制指令
				bAxisWay = *pBuffer;
				pBuffer++;
				goto servo_normal;
			}
			goto finished;
		}
servo_normal:
		if (bDstAddr==addr_mb)
		{
			switch (bCmd)
			{
			case cmd_com_motor_move_relative:
				m_protocolstatus.bPrintWay_COM_Motor_MoveRelative = *pBuffer;
				break;
			}
		}
		else
		{
			switch (bCmd)
			{
			case cmd_sf_get_timetable_vital_parameter:
				pBuffer++;		//档位值
				m_protocolstatus.dwVitalParaAecPulse	= PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.dwVitalParaDecPulse	= PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.dwVitalEvenT			= PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.bQueryWay_SF_GetTimeTableVitalParameter = comm_result_succeed;
				break;
			case cmd_sf_get_existent_speedlevel:
				m_protocolstatus.bExistSpeedLevel = *pBuffer;
				m_protocolstatus.bQueryWay_SF_GetExistentSpeedlevel = comm_result_succeed;
				break;
			case cmd_sf_download_on_timetable_paramater:
				m_protocolstatus.bConfigWay_SF_DownloadOnlineTimeTableParameter = *pBuffer;
				break;
			case cmd_sf_download_off_timetable_paramater:
				m_protocolstatus.bConfigWay_SF_DownloadOfflineTimeTableParameter = *pBuffer;
				break;
			case cmd_sf_download_timetable_timevalue:
				m_protocolstatus.bConfigWay_SF_DownloadTimeTableValue = *pBuffer;
				break;
			case cmd_sf_get_on_timetable_pamarater:
				if (bSrcAddr == addr_servo_three_axis)
				{//三轴伺服控制板
					m_parameterOnline.m_bAecRecordNum = *pBuffer;
					pBuffer++;
					m_parameterOnline.m_bDecRecordNum = *pBuffer;
					pBuffer++;
					m_parameterOnline.m_wAecSUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
					pBuffer+=2;
					m_parameterOnline.m_wDecSUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
					pBuffer+=2;
					m_parameterOnline.m_wTimeUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
					pBuffer+=2;

					m_parameterOnline.m_dwAecNum = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
					m_parameterOnline.m_dwDecNum = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
					m_parameterOnline.m_dwEvenV = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;

					for (int i=0;i<m_parameterOnline.m_bAecRecordNum;i++)
					{
						m_parameterOnline.m_dwAecRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
						pBuffer+=4;
					}

					for (int i=0;i<m_parameterOnline.m_bDecRecordNum;i++)
					{
						m_parameterOnline.m_dwDecRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
						pBuffer+=4;
					}
					m_protocolstatus.bQueryWay_SF_GetOnlineTimeTableParameter = comm_result_succeed;
					break;
				}
				pBuffer++;
				m_parameterOnline.m_bAecRecordNum = *pBuffer;
				pBuffer++;
				m_parameterOnline.m_bDecRecordNum = *pBuffer;
				pBuffer++;
				pBuffer++;	//下位机字节对齐
				m_parameterOnline.m_wAecSUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_parameterOnline.m_wDecSUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_parameterOnline.m_wTimeUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				pBuffer+=2;
				m_parameterOnline.m_dwAecNum = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_parameterOnline.m_dwDecNum = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_parameterOnline.m_dwEvenV = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;

				for (int i=0;i<m_parameterOnline.m_bAecRecordNum;i++)
				{
					m_parameterOnline.m_dwAecRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
					pBuffer+=4;
				}

				for (int i=0;i<m_parameterOnline.m_bDecRecordNum;i++)
				{
					m_parameterOnline.m_dwDecRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
					pBuffer+=4;
				}
				m_protocolstatus.bQueryWay_SF_GetOnlineTimeTableParameter = comm_result_succeed;
				break;
			case cmd_sf_get_off_timetable_pamarater:
				if (bSrcAddr == addr_servo_three_axis)
				{//三轴伺服控制板
					m_parameterOffline.m_bAecRecordNum			= *pBuffer;
					pBuffer++;
					m_parameterOffline.m_bFast2SlowRecordNum	= *pBuffer;
					pBuffer++;
					m_parameterOffline.m_bSlow2StopRecordNum	= *pBuffer;
					pBuffer++;

					m_parameterOffline.m_wAecSUnit			= PROTOCOL_MAKEDATA_WORD(pBuffer);
					pBuffer+=2;
					m_parameterOffline.m_wFast2SlowSUnit	= PROTOCOL_MAKEDATA_WORD(pBuffer);
					pBuffer+=2;
					m_parameterOffline.m_wSlow2StopSUnit	= PROTOCOL_MAKEDATA_WORD(pBuffer);
					pBuffer+=2;
					m_parameterOffline.m_wTimeUnit			= PROTOCOL_MAKEDATA_WORD(pBuffer);
					pBuffer+=2;

					m_parameterOffline.m_dwAecNum			= PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
					m_parameterOffline.m_dwFast2SlowNum		= PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
					m_parameterOffline.m_dwSlow2StopNum		= PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
					m_parameterOffline.m_dwFastV			= PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;
					m_parameterOffline.m_dwSlowV			= PROTOCOL_MAKEDATA_DWORD(pBuffer);
					pBuffer+=4;

					for (int i=0;i<m_parameterOffline.m_bAecRecordNum;i++)
					{
						m_parameterOffline.m_dwAecRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
						pBuffer+=4;
					}

					for (int i=0;i<m_parameterOffline.m_bFast2SlowRecordNum;i++)
					{
						m_parameterOffline.m_dwFast2SlowRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
						pBuffer+=4;
					}

					for (int i=0;i<m_parameterOffline.m_bSlow2StopRecordNum;i++)
					{
						m_parameterOffline.m_dwSlow2StopRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
						pBuffer+=4;
					}
					m_protocolstatus.bQueryWay_SF_GetOfflineTimeTableParameter = comm_result_succeed;
					break;
				}
				pBuffer++;
				m_parameterOffline.m_bAecRecordNum = *pBuffer;
				pBuffer++;
				m_parameterOffline.m_bFast2SlowRecordNum = *pBuffer;
				pBuffer++;
				m_parameterOffline.m_bSlow2StopRecordNum = *pBuffer;
				pBuffer++;
				m_parameterOffline.m_wAecSUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_parameterOffline.m_wFast2SlowSUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_parameterOffline.m_wSlow2StopSUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_parameterOffline.m_wTimeUnit = PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_parameterOffline.m_dwAecNum = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_parameterOffline.m_dwFast2SlowNum = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_parameterOffline.m_dwSlow2StopNum = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_parameterOffline.m_dwFastV = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_parameterOffline.m_dwSlowV = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;

				for (int i=0;i<m_parameterOffline.m_bAecRecordNum;i++)
				{
					m_parameterOffline.m_dwAecRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
					pBuffer+=4;
				}

				for (int i=0;i<m_parameterOffline.m_bFast2SlowRecordNum;i++)
				{
					m_parameterOffline.m_dwFast2SlowRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
					pBuffer+=4;
				}

				for (int i=0;i<m_parameterOffline.m_bSlow2StopRecordNum;i++)
				{
					m_parameterOffline.m_dwSlow2StopRecordArray.Add(PROTOCOL_MAKEDATA_DWORD(pBuffer));
					pBuffer+=4;
				}
				m_protocolstatus.bQueryWay_SF_GetOfflineTimeTableParameter = comm_result_succeed;
				break;
			case cmd_sf_count_fullpulse:
				m_protocolstatus.bPrintWay_SF_CountFullPulse = *pBuffer;
				break;
			case cmd_sf_motor_move_relative:
				m_protocolstatus.bPrintWay_SF_MotorMoveRelative = *pBuffer;
				break;
			case cmd_sf_motor_move_absolute:
				m_protocolstatus.bPrintWay_SF_MotorMoveAbsolute = *pBuffer;
				break;
			case cmd_sf_motor_get_position:
				m_protocolstatus.dwCurrentPosition = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				m_protocolstatus.bQueryWay_SF_MotorGetPosition = comm_result_succeed;
				break;
			case cmd_sf_motor_move_special_position:
				m_protocolstatus.bPrintWay_SF_MotorMoveSpecialPosition = *pBuffer;
				break;
			case cmd_sf_motor_move_stop:
				m_protocolstatus.bPrintWay_SF_MotorMoveStop = *pBuffer;
				break;
			case cmd_sf_motor_move_offline:
				m_protocolstatus.bPrintWay_SF_MotorMove = *pBuffer;
				break;
			case cmd_sf_get_fullpulse:
				m_protocolstatus.dwFullPulse = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				m_protocolstatus.bQueryWay_SF_GetFullPulse = comm_result_succeed;
				break;
			case cmd_sf_get_special_position:
				pBuffer++;
				m_protocolstatus.dwSpecialPosition = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				m_protocolstatus.bQueryWay_SF_GetSpecialPosition = comm_result_succeed;
				break;
			case cmd_sf_set_special_position:
				m_protocolstatus.bConfigWay_SF_SetSpecialPosition = *pBuffer;
				break;
			case cmd_sf_delete_all_timetable_data:
				m_protocolstatus.bConfigWay_SF_DeleteAllTimeTableData = *pBuffer;
				break;
			case cmd_sf_set_firm_parameter:
				switch(bSrcAddr)
				{
				case addr_servo_x:
					m_protocolstatus.bConfigWay_SF_SetFirmParameterX = *pBuffer;
					break;
				case addr_servo_y:
					m_protocolstatus.bConfigWay_SF_SetFirmParameterY = *pBuffer;
					break;
				case addr_servo_three_axis:
					switch(bAxisWay)
					{
					case sv_three_axis_way_x:
						m_protocolstatus.bConfigWay_SF_SetFirmParameterX = *pBuffer;
						break;
					case sv_three_axis_way_y:
						m_protocolstatus.bConfigWay_SF_SetFirmParameterY = *pBuffer;
						break;
					case sv_three_axis_way_z:
						m_protocolstatus.bConfigWay_SF_SetFirmParameterZ = *pBuffer;
						break;
					}
				}
				break;
			case cmd_sf_get_firm_parameter:
				switch(bSrcAddr)
				{
				case addr_servo_x:
				{
					DWORD dwPara;

					dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_x.m_fReducerX,dwPara);
					pBuffer+=4;
					dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_x.m_fDollyPerimeter,dwPara);
					pBuffer+=4;
					dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_x.m_fPulsesPerRevolution_x,dwPara);

					m_protocolstatus.bQueryWay_SF_GetFirmParameterX = comm_result_succeed;
					break;
				}
				case addr_servo_y:
				{
					DWORD dwPara;

					dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_y.m_fReducerY,dwPara);
					pBuffer+=4;
					dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_y.m_fDeferentPerimeter,dwPara);
					pBuffer+=4;
					dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_y.m_fDeferentPitch,dwPara);
					pBuffer+=4;
					dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
					PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_y.m_fPulsesPerRevolution_y,dwPara);

					m_protocolstatus.bQueryWay_SF_GetFirmParameterY = comm_result_succeed;
					break;
				}
				case addr_servo_three_axis:
					switch(bAxisWay)
					{
					case sv_three_axis_way_x:
						{
							DWORD dwPara;

							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_x.m_fReducerX,dwPara);
							pBuffer+=4;
							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_x.m_fDollyPerimeter,dwPara);
							pBuffer+=4;
							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_x.m_fPulsesPerRevolution_x,dwPara);

							m_protocolstatus.bQueryWay_SF_GetFirmParameterX = comm_result_succeed;
							break;
						}
					case sv_three_axis_way_y:
						{
							DWORD dwPara;

							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_y.m_fReducerY,dwPara);
							pBuffer+=4;
							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_y.m_fDeferentPerimeter,dwPara);
							pBuffer+=4;
							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_y.m_fDeferentPitch,dwPara);
							pBuffer+=4;
							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_y.m_fPulsesPerRevolution_y,dwPara);

							m_protocolstatus.bQueryWay_SF_GetFirmParameterY = comm_result_succeed;
							break;
						}						break;
					case sv_three_axis_way_z:
						{
							DWORD dwPara;

							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_z.m_fReducer_Z,dwPara);
							pBuffer+=4;
							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_z.m_fDeferentPerimeter_Z,dwPara);
							pBuffer+=4;
							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_z.m_fDeferentPitch_Z,dwPara);
							pBuffer+=4;
							dwPara = PROTOCOL_MAKEDATA_DWORD(pBuffer);
							PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(m_protocolstatus.sf_board_parameter_z.m_fPulsesPerRevolution_Z,dwPara);

							m_protocolstatus.bQueryWay_SF_GetFirmParameterZ = comm_result_succeed;
							break;
						}						
					}
					break;
				}
				break;
			}
			goto finished;
		}
	}
	else if (bSrcAddr==addr_ld)
	{//光栅板 - 平网蓝光
		CString str = "";
		BYTE bCnt,bLen;

		switch(bCmd)
		{
		case cmd_ld_power_get_last:
			if (bCmdSize==7)
			{
				m_protocolstatus.bFeedBackChannel		= PROTOCOL_MAKEDATA_BYTE(pBuffer);
				pBuffer+=1;
				m_protocolstatus.ldVoltage.m_wPC		= PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_protocolstatus.ldVoltage.m_wLastPD	= PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_protocolstatus.ldVoltage.m_wLastLD	= PROTOCOL_MAKEDATA_WORD(pBuffer);

				m_protocolstatus.bPrintWay_ld_voltage	= ld_comm_result_succeed;
			}
			else
			{
				m_protocolstatus.bPrintWay_ld_voltage	= *pBuffer;
			}
			break;
		case cmd_ld_power_get_now:
		case cmd_ld_power_correct:
		case cmd_ld_power_set_pc:
			if (bCmdSize==7)
			{
				m_protocolstatus.bFeedBackChannel		= PROTOCOL_MAKEDATA_BYTE(pBuffer);
				pBuffer+=1;
				m_protocolstatus.ldVoltage.m_wPC		= PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_protocolstatus.ldVoltage.m_wNowPD		= PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer+=2;
				m_protocolstatus.ldVoltage.m_wNowLD		= PROTOCOL_MAKEDATA_WORD(pBuffer);

				m_protocolstatus.bPrintWay_ld_voltage	= ld_comm_result_succeed;
			}
			else
			{
				m_protocolstatus.bPrintWay_ld_voltage	= *pBuffer;
			}
			break;
		case cmd_ld_power_set_ld:
		case cmd_ld_set_laser_test_power:
			if (bCmdSize = 5)
			{
				m_protocolstatus.bFeedBackChannel		= PROTOCOL_MAKEDATA_BYTE(pBuffer);
				pBuffer+=1;
				m_protocolstatus.ldVoltage.m_wNowPD		= PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer += 2;
				m_protocolstatus.ldVoltage.m_wNowLD		= PROTOCOL_MAKEDATA_WORD(pBuffer);
				pBuffer += 2;
				m_protocolstatus.bPrintWay_ld_voltage	= ld_comm_result_succeed;
			}
			else
			{
				m_protocolstatus.bPrintWay_ld_voltage	= *pBuffer;
			}
		case cmd_ld_get_current_power:
			if (bCmdSize==3)
			{
				m_protocolstatus.bFeedBackChannel		= PROTOCOL_MAKEDATA_BYTE(pBuffer);
				pBuffer+=1;
				m_protocolstatus.wFeedBackVol			= PROTOCOL_MAKEDATA_WORD(pBuffer);
				m_protocolstatus.nProgress++;
				::PostMessage(m_pLDWnd->m_hWnd, WM_LD_SET_POWER_STATUS, 0, 0);
			}
			else
			{
				m_protocolstatus.bQueryWay_ld_get_current_power = *pBuffer;
			}
			break;
		case cmd_ld_light_on:
			if (bCmdSize==1)
			{
				m_protocolstatus.bConfigWay_ld_light_on = *pBuffer;
			}
			break;
		case cmd_ld_light_off:
			if (bCmdSize==1)
			{
				m_protocolstatus.bConfigWay_ld_light_off = *pBuffer;
			}
			break;
		case cmd_ld_light_on_time:
			if (bCmdSize == 1)
			{
				m_protocolstatus.bConfigWay_ld_light_on_time = *pBuffer;
			}
			break;
		case cmd_ld_set_check_power:
			if (bCmdSize==3)
			{

				m_protocolstatus.bFeedBackChannel = PROTOCOL_MAKEDATA_BYTE(pBuffer);
				pBuffer+=1;
				m_protocolstatus.wFeedBackCheckVol = PROTOCOL_MAKEDATA_WORD(pBuffer);
				m_protocolstatus.bConfigWay_ld_check_power = ld_comm_result_succeed;
			}
			else
			{
				m_protocolstatus.bConfigWay_ld_check_power = *pBuffer;
			}
			break;
		}
	}
	else
	{
	}

finished:
	m_receivedataex_bReturnFlag[0] = protocol_command_ok;
	m_receivedataex_nReturnFlagLen = 1;

end:
	return dwResult;
}


// 封装指令信息到数据帧中
//	pDstBuffer :[out]封装后的数据帧缓冲区，缓冲区大小 >= pt_data_len_max
int CCommunicationPort::dataframe_transdata(BYTE* pDstBuffer,BYTE bCmd,const BYTE* pCmdData, BYTE bCmdSize)
{
	if (pDstBuffer==NULL)
		return 0;

	int		nIndex;

	pDstBuffer[pt_data_start0]	= protocol_dataframe_start1;
	pDstBuffer[pt_data_start1]	= protocol_dataframe_start2;
	pDstBuffer[pt_data_cmd]		= bCmd;	
	pDstBuffer[pt_data_cmdsize]	= bCmdSize;

	nIndex = pt_data_startfixcount;
	if (bCmdSize > 0)
	{
		if (pCmdData==NULL)
			return 0;
		memcpy(pDstBuffer+pt_data_startfixcount, pCmdData, bCmdSize);
		nIndex += bCmdSize;
	}

	pDstBuffer[nIndex] = protocol_dataframe_end;
	nIndex++;

	pDstBuffer[nIndex] = CCommunicationPort::CalcCRC(pDstBuffer, nIndex);
	nIndex++;

	return nIndex;
}

int CCommunicationPort::dataframe_transdata0(BYTE* pDstBuffer,BYTE bSrcAddress,BYTE bDstAddress,BYTE bCmd, BYTE bCmdCount,const BYTE* pCmdData, BYTE bCmdSize)
{
	if (pDstBuffer==NULL)
		return 0;

	int		nIndex;

	pDstBuffer[pt_data_start0]	= protocol_dataframe_start1;
	pDstBuffer[pt_data_start1]	= protocol_dataframe_start2;
	pDstBuffer[pt_data_add_src] = bSrcAddress;
	pDstBuffer[pt_data_add_dst]	= bDstAddress;
	pDstBuffer[pt_data_cmd]		= bCmd;
	pDstBuffer[pt_data_cmdcount] = bCmdCount;	
	pDstBuffer[pt_data_cmdsize]	= bCmdSize;

	nIndex = pt_data_startfixcount;
	if (bCmdSize > 0)
	{
		if (pCmdData==NULL)
			return 0;
		memcpy(pDstBuffer+pt_data_startfixcount, pCmdData, bCmdSize);
		nIndex += bCmdSize;
	}

	pDstBuffer[nIndex] = protocol_dataframe_end;
	nIndex++;

	pDstBuffer[nIndex] = CCommunicationPort::CalcCRC(pDstBuffer, nIndex);
	nIndex++;

	return nIndex;
}

//得当前下位机返回的各种参数（状态）信息
const Protocol_Control_Statue *CCommunicationPort::GetControlStatus() const
{
	return &m_protocolstatus;
}

// 发送指令-检测联机状态（PC<-->MB通讯是否正常）
BOOL CCommunicationPort::sendorder_check(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_check;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	m_protocolstatus.bCheckedWay = protocol_queryway_checking;
	nResult = SendMsg_handshake(&bCmd, 1, timeout, timerepeatsend, &m_protocolstatus.bCheckedWay, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

// 发送指令-查询 MB 协议版本
BOOL CCommunicationPort::sendorder_getverion(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getversion;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_ProtocolVersion = protocol_queryway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_ProtocolVersion, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}


// 发送指令-查询当前电压设定值
BOOL CCommunicationPort::sendorder_query_controlamplitude(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_amplitude;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_ControlAmplitude = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_ControlAmplitude, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

// 发送指令-查询当前脉宽设定值
BOOL CCommunicationPort::sendorder_query_controlpulsewidth(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_pulsewidth;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_ControlPulsewidth = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_ControlPulsewidth, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-查询当前布厚参数设定值
BOOL CCommunicationPort::sendorder_query_controlthickness(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_thickness;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_ControlThickness = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_ControlThickness, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-查询当前散喷参数值
BOOL CCommunicationPort::sendorder_query_sprayparam(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_spray;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_sprayparam = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_sprayparam, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

// 发送指令-查询当前清洗参数值
BOOL CCommunicationPort::sendorder_query_cleanheadparam(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_cleanhead;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_cleanheadparam = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_cleanheadparam, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-运动-查询-公共配置参数
BOOL CCommunicationPort::sendorder_query_moto_general(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_moto_general;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_motoparam_general = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_motoparam_general, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

// 发送指令-运动-查询-X向配置参数
BOOL CCommunicationPort::sendorder_query_moto_x(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_moto_x;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_motoparam_x = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_motoparam_x, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-运动-查询-Y向配置参数
BOOL CCommunicationPort::sendorder_query_moto_y(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_moto_y;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_motoparam_y = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_motoparam_y, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-运动-查询-Z向配置参数
BOOL CCommunicationPort::sendorder_query_moto_z(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_getconfig_moto_z;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_queryway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_queryway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_motoparam_z = protocol_queryway_checking;
	if (time_check <= 0)
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, NULL, NULL, 0, (-time_check));
	else
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_motoparam_z, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}


// 发送指令-设置电压控制值
//	sizeof (...) >= 8
BOOL CCommunicationPort::sendorder_set_controlamplitude(const BYTE *pbControlAmplitude, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_amplitude;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	ASSERT(pbControlAmplitude != NULL);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, pbControlAmplitude, m_printer_plugin_bChannels);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_ControlAmplitude = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_ControlAmplitude, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;

}
// 发送指令-设置脉宽控制值
//	sizeof (...) >= 8
BOOL CCommunicationPort::sendorder_set_controlpulsewidth(const BYTE *pbControlPulsewidth, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_pulsewidth;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	ASSERT(pbControlPulsewidth != NULL);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, pbControlPulsewidth, m_printer_plugin_bChannels);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_ControlPulsewidth = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_ControlPulsewidth, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-设置布厚参数控制值
BOOL CCommunicationPort::sendorder_set_controlthickness(int nThickness, BOOL bTestMode, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_thickness;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bTestMode ? 1 : 0;		//是否需要作一次布厚测试动作
	bCmdData[bCmdSize++] = LOBYTE((WORD)nThickness);
	bCmdData[bCmdSize++] = HIBYTE((WORD)nThickness);

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_ControlThickness = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_ControlThickness, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-设置散喷参数值
BOOL CCommunicationPort::sendorder_set_sprayparam(const SPRAYPARAM &sprayparam, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_spray;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = sprayparam.bSprayForIdleEanble;
	bCmdData[bCmdSize++] = LOBYTE(sprayparam.wSprayFrequencyForIdel);
	bCmdData[bCmdSize++] = HIBYTE(sprayparam.wSprayFrequencyForIdel);
	bCmdData[bCmdSize++] = sprayparam.bSprayForPrintingEanble;
	bCmdData[bCmdSize++] = LOBYTE(sprayparam.wSprayPassForPrint);
	bCmdData[bCmdSize++] = HIBYTE(sprayparam.wSprayPassForPrint);

	ASSERT(bCmdSize==6);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_sprayparam = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_sprayparam, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-设置清洗参数值
BOOL CCommunicationPort::sendorder_set_cleanheadparam(const CLEANHEADPARAM &cleanheadparam, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_cleanhead;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = cleanheadparam.bEnable;
	bCmdData[bCmdSize++] = LOBYTE(cleanheadparam.wPass);
	bCmdData[bCmdSize++] = HIBYTE(cleanheadparam.wPass);
	bCmdData[bCmdSize++] = LOBYTE(cleanheadparam.wPurgeTime);
	bCmdData[bCmdSize++] = HIBYTE(cleanheadparam.wPurgeTime);
	bCmdData[bCmdSize++] = LOBYTE(cleanheadparam.wPurgeDelay);
	bCmdData[bCmdSize++] = HIBYTE(cleanheadparam.wPurgeDelay);
	bCmdData[bCmdSize++] = cleanheadparam.bCleanTime;

	ASSERT(bCmdSize==8);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_cleanheadparam = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_cleanheadparam, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-运动-设定-公共配置参数
BOOL CCommunicationPort::sendorder_set_moto_general(const MOTOPARAM_GENERAL &param, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_moto_general;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;
	DWORD	dwTmp;
	WORD	wHi, wLo;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = LOBYTE(param.X_MAX_WIDTH);
	bCmdData[bCmdSize++] = HIBYTE(param.X_MAX_WIDTH);
	bCmdData[bCmdSize++] = LOBYTE(param.PRINT_MAX_FREQ);
	bCmdData[bCmdSize++] = HIBYTE(param.PRINT_MAX_FREQ);
	bCmdData[bCmdSize++] = LOBYTE(param.X_MAX_SPEED);
	bCmdData[bCmdSize++] = HIBYTE(param.X_MAX_SPEED);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwTmp, param.X_COEF);
	wHi			= HIWORD(dwTmp);
	wLo			= LOWORD(dwTmp);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	ASSERT(bCmdSize==10);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_motoparam_general = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_motoparam_general, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-运动-设定-X向配置参数
BOOL CCommunicationPort::sendorder_set_moto_x(const MOTOPARAM_X &param, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_moto_x;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;
	DWORD	dwTmp;
	WORD	wHi, wLo;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = LOBYTE(param.SERVE_PER_PULSE);
	bCmdData[bCmdSize++] = HIBYTE(param.SERVE_PER_PULSE);
	bCmdData[bCmdSize++] = LOBYTE(param.X_P_DIS);
	bCmdData[bCmdSize++] = HIBYTE(param.X_P_DIS);
	bCmdData[bCmdSize++] = LOBYTE(param.X_S_DIS);
	bCmdData[bCmdSize++] = HIBYTE(param.X_S_DIS);
	bCmdData[bCmdSize++] = LOBYTE(param.IO_SPACE);
	bCmdData[bCmdSize++] = HIBYTE(param.IO_SPACE);

	wHi			= HIWORD(param.IO_PULSE);
	wLo			= LOWORD(param.IO_PULSE);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wHi			= HIWORD(param.X_DELAY_S);
	wLo			= LOWORD(param.X_DELAY_S);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wHi			= HIWORD(param.X_DELAY_F);
	wLo			= LOWORD(param.X_DELAY_F);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwTmp, param.SERVE_PER_CYCLE);
	wHi			= HIWORD(dwTmp);
	wLo			= LOWORD(dwTmp);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	ASSERT(bCmdSize==24);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_motoparam_x = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_motoparam_x, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-运动-设定-Y向配置参数
BOOL CCommunicationPort::sendorder_set_moto_y(const MOTOPARAM_Y &param, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_moto_y;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;
	DWORD	dwTmp;
	WORD	wHi, wLo;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = LOBYTE(param.Y_PER_PULSE);
	bCmdData[bCmdSize++] = HIBYTE(param.Y_PER_PULSE);
	bCmdData[bCmdSize++] = LOBYTE(param.Y_MAX_SPEED);
	bCmdData[bCmdSize++] = HIBYTE(param.Y_MAX_SPEED);

	wHi			= HIWORD(param.Y_DELAY);
	wLo			= LOWORD(param.Y_DELAY);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wHi			= HIWORD(param.Y_DELAY_S);
	wLo			= LOWORD(param.Y_DELAY_S);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wHi			= HIWORD(param.Y_DELAY_F);
	wLo			= LOWORD(param.Y_DELAY_F);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwTmp, param.Y_WHORL);
	wHi			= HIWORD(dwTmp);
	wLo			= LOWORD(dwTmp);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	ASSERT(bCmdSize==20);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_motoparam_y = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_motoparam_y, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-运动-设定-Z向配置参数
BOOL CCommunicationPort::sendorder_set_moto_z(const MOTOPARAM_Z &param, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_setconfig_moto_z;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;
	DWORD	dwTmp;
	WORD	wHi, wLo;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_configway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_configway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	wHi			= HIWORD(param.Z_DELAY);
	wLo			= LOWORD(param.Z_DELAY);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwTmp, param.Z_WHORL);
	wHi			= HIWORD(dwTmp);
	wLo			= LOWORD(dwTmp);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwTmp, param.Z_POSITION_MIN);
	wHi			= HIWORD(dwTmp);
	wLo			= LOWORD(dwTmp);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwTmp, param.Z_POSITION_MAX);
	wHi			= HIWORD(dwTmp);
	wLo			= LOWORD(dwTmp);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	ASSERT(bCmdSize==16);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_motoparam_z = protocol_configway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_motoparam_z, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

// 发送指令-打印准备
BOOL CCommunicationPort::sendorder_print_start(const ky_print_info *pPrintInfo, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_print_start;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	WORD	wWidth, wHi, wLo;
	DWORD	dwHeight;
	int		timeout			= 20000;
	int		timerepeatsend	= 5000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	ASSERT(pPrintInfo != NULL);
	if (pPrintInfo==NULL)
		return FALSE;

	wWidth		= (WORD)(pPrintInfo->width + 0.5);
	dwHeight	= (DWORD)(pPrintInfo->height + 0.5);
	wHi			= HIWORD(dwHeight);
	wLo			= LOWORD(dwHeight);

	bCmdData[bCmdSize++] = LOBYTE(pPrintInfo->xres);
	bCmdData[bCmdSize++] = HIBYTE(pPrintInfo->xres);

	bCmdData[bCmdSize++] = (BYTE)(pPrintInfo->pass);

	bCmdData[bCmdSize++] = LOBYTE(wWidth);
	bCmdData[bCmdSize++] = HIBYTE(wWidth);

	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	bCmdData[bCmdSize++] = pPrintInfo->bPrintway;
	bCmdData[bCmdSize++] = (BYTE)(pPrintInfo->nSpeedType);

	bCmdData[bCmdSize++] = CCommunicationPort::TransOriginPosition(pPrintInfo->bOriginPosition_start);

	ASSERT(bCmdSize==12);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	int		n;
	while (1)
	{
		nResult		= rpComBase::sms_err_send;
		m_protocolstatus.bPrintWay_printstart = protocol_printway_checking;
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_printstart, flagCheck, nCheckCount, time_check);
		n = CCommunicationPort::SendMsg_handshake_Failed_and_Resend(nResult);
		if (m_protocolstatus.bPrintWay_printstart==protocol_printway_ok)
		{//此时收到正常的返回信号也算成功
			nResult = rpComBase::sms_handshake_ok;
			break;
		}
		if (n==0)
		{//出错并重发
			Sleep(200);
			continue;
		}
		else if (n > 0)
		{//正常退出或忽略错误
			nResult = rpComBase::sms_handshake_ok;
		}
		break;
	}
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

// 发送指令-打印准备
//	wProtocolVersion	:[in]//下位机协议版本信息，PROTOCOL_VERSION_INFO，protocol_version_none 表示自适应
//BOOL CCommunicationPort::sendorder_print_line(const PRINTLINE_INFO &lineInfo, WORD wProtocolVersion, int time_check)
//{
//	int		nResult		= rpComBase::sms_err_send;
//	BYTE	bCmd		= protocol_command_print_line;
//	BYTE	bCmdData[0xFF];
//	BYTE	bCmdSize;
//	BYTE	snd_msg[pt_data_len_max];
//	int		msg_len = 0;
//	WORD	wHi, wLo;
//	int		timeout			= 20000;
//	int		timerepeatsend	= 5000;
//	CComBase::FLAGCHECK flagCheck[2];
//	int		nCheckCount;
//
//	if (time_check > 0)
//	{
//		timeout			= min(timeout, time_check);
//		timerepeatsend	= min(timerepeatsend, timeout);
//	}
//
//	nCheckCount = 0;
//	BZERO(flagCheck);
//	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
//	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
//	nCheckCount++;
//
//	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
//	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
//	nCheckCount++;
//
//	if (!IsOpen())
//		return FALSE;
//
//	bCmdSize = 0;
//
//	bCmdData[bCmdSize++] = (lineInfo.start_io);
//	bCmdData[bCmdSize++] = LOBYTE(lineInfo.start_pos);
//	bCmdData[bCmdSize++] = HIBYTE(lineInfo.start_pos);
//	wHi			= HIWORD(lineInfo.width);
//	wLo			= LOWORD(lineInfo.width);
//	bCmdData[bCmdSize++] = LOBYTE(wLo);
//	bCmdData[bCmdSize++] = HIBYTE(wLo);
//	bCmdData[bCmdSize++] = LOBYTE(wHi);
//	bCmdData[bCmdSize++] = HIBYTE(wHi);
//	bCmdData[bCmdSize++] = (lineInfo.end_io);
//
//	bCmdData[bCmdSize++] = (lineInfo.re_start_io);
//	bCmdData[bCmdSize++] = LOBYTE(lineInfo.re_start_pos);
//	bCmdData[bCmdSize++] = HIBYTE(lineInfo.re_start_pos);
//	wHi			= HIWORD(lineInfo.re_width);
//	wLo			= LOWORD(lineInfo.re_width);
//	bCmdData[bCmdSize++] = LOBYTE(wLo);
//	bCmdData[bCmdSize++] = HIBYTE(wLo);
//	bCmdData[bCmdSize++] = LOBYTE(wHi);
//	bCmdData[bCmdSize++] = HIBYTE(wHi);
//	bCmdData[bCmdSize++] = (lineInfo.re_end_io);
//
//	wHi			= HIWORD(lineInfo.ydist);
//	wLo			= LOWORD(lineInfo.ydist);
//	bCmdData[bCmdSize++] = LOBYTE(wLo);
//	bCmdData[bCmdSize++] = HIBYTE(wLo);
//	bCmdData[bCmdSize++] = LOBYTE(wHi);
//	bCmdData[bCmdSize++] = HIBYTE(wHi);
//
//	if (wProtocolVersion==protocol_version_none)
//	{//自适应，通过 sendorder_getverion 得到
//		wProtocolVersion = m_protocolstatus.wProtocolVersion;
//	}
//	if (wProtocolVersion <= protocol_version_4001)
//	{// V40.01
//		;
//	}
//	if (wProtocolVersion >= protocol_version_4002)
//	{
//		// V40.02
//		//	1、protocol_command_print_line 中增加是否开放 DPC 的选项 (bDPCDisable)
//		//	2、数据帧返回值（MB）中允许定义操作失败模式（PC接收返回值时判断相应信息）
//		// V40.03
//		//  增加打印时运动参数配置信息
//		bCmdData[bCmdSize++] = lineInfo.bDPCDisable ? 0 : 1;
//	}
//
//	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
//	ASSERT(msg_len >= pt_data_len_min);
//
//	int		n;
//	while (1)
//	{
//		nResult		= rpComBase::sms_err_send;
//		m_protocolstatus.bPrintWay_printline = protocol_printway_checking;
//		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_printline, flagCheck, nCheckCount, time_check);
//		n = CCommunicationPort::SendMsg_handshake_Failed_and_Resend(nResult);
//		if (m_protocolstatus.bPrintWay_printline==protocol_printway_ok)
//		{//此时收到正常的返回信号也算成功
//			nResult = rpComBase::sms_handshake_ok;
//			break;
//		}
//		if (n==0)
//		{//出错并重发
//			Sleep(200);
//			continue;
//		}
//		else if (n > 0)
//		{//正常退出或忽略错误
//			nResult = rpComBase::sms_handshake_ok;
//		}
//		break;
//	}
//	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
//}
// 发送指令-打印结束
//	bOriginPosition		:[in]指定原点位置，PRINTER::ORIGIN_POSITION
BOOL CCommunicationPort::sendorder_print_end(BYTE bOriginPosition, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_print_end;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 2000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = CCommunicationPort::TransOriginPosition(bOriginPosition);

	ASSERT(bCmdSize==1);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_printend = protocol_printway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_printend, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

// 发送指令-复位
BOOL CCommunicationPort::sendorder_pc_reset(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_pc_reset;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 2000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_pc_reset = protocol_printway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_pc_reset, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-暂停
BOOL CCommunicationPort::sendorder_pc_pause(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_pc_pause;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_pc_pause = protocol_printway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_pc_pause, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-继续
BOOL CCommunicationPort::sendorder_pc_resume(int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_pc_resume;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 20000;
	int		timerepeatsend	= 5000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	int		n;
	while (1)
	{
		nResult		= rpComBase::sms_err_send;
		m_protocolstatus.bPrintWay_pc_resume = protocol_printway_checking;
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_pc_resume, flagCheck, nCheckCount, time_check);
		n = CCommunicationPort::SendMsg_handshake_Failed_and_Resend(nResult);
		if (m_protocolstatus.bPrintWay_pc_resume==protocol_printway_ok)
		{//此时收到正常的返回信号也算成功
			nResult = rpComBase::sms_handshake_ok;
			break;
		}
		if (n==0)
		{//出错并重发
			Sleep(200);
			continue;
		}
		else if (n > 0)
		{//正常退出或忽略错误
			nResult = rpComBase::sms_handshake_ok;
		}
		break;
	}
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

// 发送确认指令-MB复位
BOOL CCommunicationPort::sendorder_confirmation_mb_reset(void)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_mb_reset;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;

	m_protocolstatus.bSignal_mb_reset = 0;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, rpComBase::sml_crucial);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送确认指令-MB暂停
BOOL CCommunicationPort::sendorder_confirmation_mb_pause(void)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_mb_pause;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;

	m_protocolstatus.bSignal_mb_pause = 0;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, rpComBase::sml_crucial);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送确认指令-MB继续
BOOL CCommunicationPort::sendorder_confirmation_mb_resume(void)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_mb_resume;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;

	m_protocolstatus.bSignal_mb_resume = 0;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, rpComBase::sml_crucial);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送确认指令-MB喷头清洗
BOOL CCommunicationPort::sendorder_confirmation_mb_cleanhead(void)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_mb_cleanhead;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;

	m_protocolstatus.bSignal_mb_cleanhead = 0;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, rpComBase::sml_crucial);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}


//转换原点位置参数值：打印机参数 --> 通讯协议参数
//	bOriginPosition		:[in]指定原点位置，PRINTER::ORIGIN_POSITION
BYTE CCommunicationPort::TransOriginPosition(BYTE bOriginPosition)
{
	//原点位置定义 
	enum COMMU_ORIGIN_POSITION
	{  
		commu_origin_pos_default		= 0x00,    //缺省，无特殊操作 
		commu_origin_pos_zero_x			= 0x01,    //X向打印原点 
		commu_origin_pos_zero_y			= 0x10,    //Y向打印原点 

		commu_origin_pos_max_x			= 0x02,    //X向清洗槽位置（X向结束位置） 
		commu_origin_pos_max_y			= 0x20,    //Y向结束位置 

		commu_origin_pos_zero_xy		= commu_origin_pos_zero_x  |  commu_origin_pos_zero_y , 
		commu_origin_pos_max_xy			= commu_origin_pos_max_x  | commu_origin_pos_max_y, 
		commu_origin_pos_zero_x_max_y	= commu_origin_pos_zero_x  |  commu_origin_pos_max_y, 
		commu_origin_pos_max_x_zero_y	= commu_origin_pos_max_x  | commu_origin_pos_zero_y 
	}; 

	BYTE bResult = commu_origin_pos_default;
	switch(bOriginPosition & PRINTER::origin_pos_mask_x)
	{
	case PRINTER::origin_pos_x_max:
		bResult |= commu_origin_pos_max_x;
		break;
	case PRINTER::origin_pos_x_zero:
	default:
		bResult |= commu_origin_pos_zero_x;
		break;
	case PRINTER::origin_pos_x_current:
		bResult |= commu_origin_pos_default;
		break;
	}
	switch(bOriginPosition & PRINTER::origin_pos_mask_y)
	{
	case PRINTER::origin_pos_y_max:
		bResult |= commu_origin_pos_max_y;
		break;
	case PRINTER::origin_pos_y_zero:
	default:
		bResult |= commu_origin_pos_zero_y;
		break;
	case PRINTER::origin_pos_y_current:
		bResult |= commu_origin_pos_default;
		break;
	}
	return bResult;
}

// 发送指令-回到打印原点
//	bOriginPosition		:[in]指定原点位置，PRINTER::ORIGIN_POSITION
BOOL CCommunicationPort::sendorder_print_back2origin(BYTE bOriginPosition, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_print_back2origin;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 20000;
	int		timerepeatsend	= 2000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = CCommunicationPort::TransOriginPosition(bOriginPosition);

	ASSERT(bCmdSize==1);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	int		n;
	while (1)
	{
		nResult		= rpComBase::sms_err_send;
		m_protocolstatus.bPrintWay_print_back2origin = protocol_printway_checking;
		nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_print_back2origin, flagCheck, nCheckCount, time_check);
		n = CCommunicationPort::SendMsg_handshake_Failed_and_Resend(nResult);
		if (m_protocolstatus.bPrintWay_print_back2origin==protocol_printway_ok)
		{//此时收到正常的返回信号也算成功
			nResult = rpComBase::sms_handshake_ok;
			break;
		}
		if (n==0)
		{//出错并重发
			Sleep(200);
			continue;
		}
		else if (n > 0)
		{//正常退出或忽略错误
			nResult = rpComBase::sms_handshake_ok;
		}
		break;
	}
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-执行一次自动清洗操作
//	nPurgeTime	:[in]压墨时长（ms）
//	nPurgeDelay	:[in]压墨后延时（ms）
//	nCleanTime	:[in]刮刀次数
BOOL CCommunicationPort::sendorder_print_cleanhead(int nPurgeTime, int nPurgeDelay, BYTE bCleanTime, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_print_cheanhead;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 2000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	m_protocolstatus.bSignal_mb_cleanhead = 0;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = LOBYTE(nPurgeTime);
	bCmdData[bCmdSize++] = HIBYTE(nPurgeTime);

	bCmdData[bCmdSize++] = LOBYTE(nPurgeDelay);
	bCmdData[bCmdSize++] = HIBYTE(nPurgeDelay);

	bCmdData[bCmdSize++] = bCleanTime;

	ASSERT(bCmdSize==5);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_print_cleanhead = protocol_printway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_print_cleanhead, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送指令-执行一次散喷操作
//	bSprayTime	:[in]散喷次数
BOOL CCommunicationPort::sendorder_print_spray(BYTE bSprayTime, int time_check)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_print_spray;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 2000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= protocol_printway_ok;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= protocol_printway_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bSprayTime;

	ASSERT(bCmdSize==1);
	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_print_spray = protocol_printway_checking;
	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_print_spray, flagCheck, nCheckCount, time_check);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
//中断当前操作指令串发送状态检测过程
void CCommunicationPort::sendorder_handshake_Break(BOOL bBreak)
{
	CComBase::SendMsg_handshake_Break(bBreak);
}
//当前是否在发送消息时的循环等待中
bool CCommunicationPort::sendorder_handshake_isinloop() const
{
	return m_sendMsg_isInLoop;
}

// 发送确认指令-MB系统错误
BOOL CCommunicationPort::sendorder_confirmation_mb_syserror(void)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_mb_syserror;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 2000;

	m_protocolstatus.bSignal_mb_syserror = 0;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, rpComBase::sml_crucial);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
// 发送确认指令-MB系统异常
BOOL CCommunicationPort::sendorder_confirmation_mb_status(void)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= protocol_command_mb_sysstatus;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 2000;

	m_protocolstatus.bSignal_mb_sysstatus = mb_system_normal;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata(snd_msg, bCmd, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	nResult = SendMsg_handshake(snd_msg, msg_len, timeout, timerepeatsend, rpComBase::sml_crucial);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

//出错提示并且要求重发指令
//	resultHandshake		: [in]SENDMSG_STATUS，SendMsg_handshake指令发送返回状态
//返回值：
//	==0	：出错并重发指令
//	> 0	：正常退出
//	< 0	：出错但不重发指令
int CCommunicationPort::SendMsg_handshake_Failed_and_Resend(int resultHandshake)
{
	CString strTmp, strInfo;
	int		nResult = 0;		//正常退出

	strInfo = "发送打印指令失败：\r\n\t";
	strTmp	= "";
	switch(resultHandshake)
	{
	default:
		ASSERT(0);
	case rpComBase::sms_handshake_ok:
		//正常退出
		nResult = 1;
		goto end;
		break;
	case rpComBase::sms_err_send:
		strTmp = "发送失败！";
		break;
	case rpComBase::sms_err_no_ok_fail:
		strTmp = "发送成功，但设备无响应！";
		break;
	case rpComBase::sms_err_fail:
		strTmp = "发送成功，但设备返回错误！";
		break;
	}
	strInfo += strTmp;
	strInfo += "\r\n\r\n是否需要重新发送打印指令?";
	nResult = AnxDialogMsg(NULL, 0, 0, MB_ABORTRETRYIGNORE, strInfo);
	switch(nResult)
	{
	case IDABORT:
		nResult = -1;		//出错但不重发
		break;
	case IDRETRY:
		nResult = 1;		//忽略本错误
		break;
	case IDIGNORE:
		nResult = 0;		//出错并重发
		break;
	}
end:
	return nResult;
}

//BOOL CCommunicationPort::mb_set_online_state(BYTE bOnlineState,int time_check,BOOL bFlag)
//{
//	int		nResult		= rpComBase::sms_err_send;
//	BYTE	bCmd		= cmd_mb_online_state;
//	BYTE	snd_msg[pt_data_len_max];
//	int		msg_len = 0;
//	BYTE	bCmdData[0xFF], bCmdSize;
//	int		timeout			= 1000;
//	int		timerepeatsend	= 1000;
//	CComBase::FLAGCHECK flagCheck[4];
//	int		nCheckCount;
//
//	if (time_check > 0)
//	{
//		timeout			= min(timeout, time_check);
//		timerepeatsend	= min(timerepeatsend, timeout);
//	}
//
//	nCheckCount = 0;
//	BZERO(flagCheck);
//	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
//	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
//	nCheckCount++;
//
//	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
//	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
//	nCheckCount++;
//
//	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
//	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
//	nCheckCount++;
//
//	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
//	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
//	nCheckCount++;
//
//	if (!IsOpen())
//		return FALSE;
//
//	bCmdSize = 0;
//	bCmdData[bCmdSize++] = bOnlineState;
//
//	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_mb,bCmd,0, bCmdData, bCmdSize);
//	ASSERT(msg_len >= pt_data_len_min);
//
//	m_protocolstatus.bConfigWay_MB_OnlineState = mb_comm_result_checking;
//	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_MB_OnlineState, flagCheck, nCheckCount, time_check, bFlag);
//	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
//}

BOOL CCommunicationPort::com_motor_move_relative(BYTE bMotoWay, BYTE bSpeedType,DWORD dwPulse,DWORD dwRePulse,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_com_motor_move_relative;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;
	WORD	wLo,wHi;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpeedType;

	wLo = LOWORD(dwPulse);
	wHi = HIWORD(dwPulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(dwRePulse);
	wHi = HIWORD(dwRePulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_mb, bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);
	
	m_protocolstatus.bPrintWay_COM_Motor_MoveRelative = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_COM_Motor_MoveRelative, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::com_get_board_condition(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_com_get_board_condition;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,addr_com,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_COM_GetBoardCondition = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_COM_GetBoardCondition, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_existent_speedlevel(BYTE bMotoWay,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_existent_speedlevel;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetExistentSpeedlevel = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetExistentSpeedlevel, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

void TIME_TABLE_ONLINE_PAMARATER::Init()
{
	m_wAecSUnit = 0;
	m_wDecSUnit = 0;
	m_wTimeUnit = 0;
	m_dwAecNum = 0;
	m_dwDecNum = 0;
	m_dwEvenV = 0;
	m_bAecRecordNum = 0;
	m_bDecRecordNum = 0;
	m_dwAecRecordArray.RemoveAll();
	m_dwDecRecordArray.RemoveAll();
}

void TIME_TABLE_OFFLINE_PAMARATER::Init()
{
	m_wAecSUnit= 0;
	m_wFast2SlowSUnit = 0;
	m_wSlow2StopSUnit = 0;
	m_wTimeUnit = 0;
	m_dwAecNum = 0;
	m_dwFast2SlowNum = 0;
	m_dwSlow2StopNum = 0;
	m_dwFastV = 0;
	m_dwSlowV = 0;
	m_bAecRecordNum = 0;
	m_bFast2SlowRecordNum = 0;
	m_bSlow2StopRecordNum = 0;
	m_dwAecRecordArray.RemoveAll();
	m_dwFast2SlowRecordArray.RemoveAll();
	m_dwSlow2StopRecordArray.RemoveAll();
}

BOOL CCommunicationPort::servo_set_online_timetable_paramater(BYTE bMotoWay,const TIME_TABLE_ONLINE &timetableOnline,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_download_on_timetable_paramater;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	WORD	wHi, wLo;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	//速度档
	bCmdData[bCmdSize++] = timetableOnline.m_bSpeedLevelOnline;

	//特殊点数目
	bCmdData[bCmdSize++] = timetableOnline.m_parameterOnline->m_bAecRecordNum;
	bCmdData[bCmdSize++] = timetableOnline.m_parameterOnline->m_bDecRecordNum;

	//加速每格距离
	bCmdData[bCmdSize++] = LOBYTE(timetableOnline.m_parameterOnline->m_wAecSUnit);
	bCmdData[bCmdSize++] = HIBYTE(timetableOnline.m_parameterOnline->m_wAecSUnit);

	//减速每格距离
	bCmdData[bCmdSize++] = LOBYTE(timetableOnline.m_parameterOnline->m_wDecSUnit);
	bCmdData[bCmdSize++] = HIBYTE(timetableOnline.m_parameterOnline->m_wDecSUnit);

	//最小时间单元
	bCmdData[bCmdSize++] = LOBYTE(timetableOnline.m_parameterOnline->m_wTimeUnit);
	bCmdData[bCmdSize++] = HIBYTE(timetableOnline.m_parameterOnline->m_wTimeUnit);

	//加速格数
	wLo = LOWORD(timetableOnline.m_parameterOnline->m_dwAecNum);
	wHi = HIWORD(timetableOnline.m_parameterOnline->m_dwAecNum);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//减速格数
	wLo = LOWORD(timetableOnline.m_parameterOnline->m_dwDecNum);
	wHi = HIWORD(timetableOnline.m_parameterOnline->m_dwDecNum);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//平均速度
	wLo = LOWORD(timetableOnline.m_parameterOnline->m_dwEvenV);
	wHi = HIWORD(timetableOnline.m_parameterOnline->m_dwEvenV);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//加速曲线特殊点
	for (int i=0; i<timetableOnline.m_parameterOnline->m_bAecRecordNum; i++)
	{
		wLo = LOWORD(timetableOnline.m_parameterOnline->m_dwAecRecordArray.GetAt(i));
		wHi = HIWORD(timetableOnline.m_parameterOnline->m_dwAecRecordArray.GetAt(i));
		bCmdData[bCmdSize++] = LOBYTE(wLo);
		bCmdData[bCmdSize++] = HIBYTE(wLo);
		bCmdData[bCmdSize++] = LOBYTE(wHi);
		bCmdData[bCmdSize++] = HIBYTE(wHi);
	}

	//减速曲线特殊点
	for (int i=0; i<timetableOnline.m_parameterOnline->m_bDecRecordNum; i++)
	{
		wLo = LOWORD(timetableOnline.m_parameterOnline->m_dwDecRecordArray.GetAt(i));
		wHi = HIWORD(timetableOnline.m_parameterOnline->m_dwDecRecordArray.GetAt(i));
		bCmdData[bCmdSize++] = LOBYTE(wLo);
		bCmdData[bCmdSize++] = HIBYTE(wLo);
		bCmdData[bCmdSize++] = LOBYTE(wHi);
		bCmdData[bCmdSize++] = HIBYTE(wHi);
	}

	ASSERT(bCmdSize<=0xFF);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SF_DownloadOnlineTimeTableParameter = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_DownloadOnlineTimeTableParameter, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_set_online_timetable_timevalue(BYTE bMotoWay,const TIME_TABLE_ONLINE &timetableOnline,int time_check,BOOL bFlag)
{
	int		nResult	= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_download_timetable_timevalue;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;

	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;

	if (!IsOpen())
		return FALSE;

	WORD wCount = 0;
	int j = 0;
	BOOL bFinish = FALSE;

	int nAec = timetableOnline.m_parameterOnline->m_dwAecNum;
	int nDec = timetableOnline.m_parameterOnline->m_dwDecNum;

	while(1)
	{	
		nResult	= rpComBase::sms_err_send;
		wCount++;

		bCmdSize = 0;
		if (bDstAddr == addr_servo_three_axis)
		{
			bCmdData[bCmdSize++] = bAxisWay;
		}

		bCmdData[bCmdSize++] = LOBYTE(wCount);
		bCmdData[bCmdSize++] = HIBYTE(wCount);

		for(int i=0; i<(servo_download_timevalue_length-2)/2; i++)
		{
			if (j>=nAec+nDec)
			{	
				bFinish = TRUE;
				break;
			}
			if (j>=nAec)
			{
				bCmdData[bCmdSize++] = LOBYTE(timetableOnline.m_wDecTimeArray.GetAt(j-nAec));
				bCmdData[bCmdSize++] = HIBYTE(timetableOnline.m_wDecTimeArray.GetAt(j-nAec));
				j++;
				continue;
			}
			bCmdData[bCmdSize++] = LOBYTE(timetableOnline.m_wAecTimeArray.GetAt(j));
			bCmdData[bCmdSize++] = HIBYTE(timetableOnline.m_wAecTimeArray.GetAt(j));
			j++;
		}

		ASSERT(bCmdSize<=servo_download_timevalue_length);	

		msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
		ASSERT(msg_len >= pt_data_len_min);

		m_protocolstatus.bConfigWay_SF_DownloadTimeTableValue = comm_result_checking;
		nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_DownloadTimeTableValue, flagCheck, nCheckCount, time_check, bFlag);

		if (nResult!=rpComBase::sms_handshake_ok)
		{
			break;
		}

		::SendMessage(m_pOnlineTimeTableDlg->m_hWnd, WM_SFDOWNLOAD_PROGRESS, (WPARAM)j, 0);

		if (bFinish==TRUE)
		{
			break;
		}
	}

	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_set_offline_timetable_paramater(BYTE bMotoWay,const TIME_TABLE_OFFLINE &timetableOffline,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_download_off_timetable_paramater;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	WORD	wHi, wLo;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	//速度档
	bCmdData[bCmdSize++] = timetableOffline.m_bSpeedLevelOffline;

	//特殊点
	bCmdData[bCmdSize++] = timetableOffline.m_parameterOffline->m_bAecRecordNum;
	bCmdData[bCmdSize++] = timetableOffline.m_parameterOffline->m_bFast2SlowRecordNum;
	bCmdData[bCmdSize++] = timetableOffline.m_parameterOffline->m_bSlow2StopRecordNum;

	//加速段每格距离
	bCmdData[bCmdSize++] = LOBYTE(timetableOffline.m_parameterOffline->m_wAecSUnit);
	bCmdData[bCmdSize++] = HIBYTE(timetableOffline.m_parameterOffline->m_wAecSUnit);

	//快到慢每格距离
	bCmdData[bCmdSize++] = LOBYTE(timetableOffline.m_parameterOffline->m_wFast2SlowSUnit);
	bCmdData[bCmdSize++] = HIBYTE(timetableOffline.m_parameterOffline->m_wFast2SlowSUnit);

	//慢到停每格距离
	bCmdData[bCmdSize++] = LOBYTE(timetableOffline.m_parameterOffline->m_wSlow2StopSUnit);
	bCmdData[bCmdSize++] = HIBYTE(timetableOffline.m_parameterOffline->m_wSlow2StopSUnit);

	//最小时间单元
	bCmdData[bCmdSize++] = LOBYTE(timetableOffline.m_parameterOffline->m_wTimeUnit);
	bCmdData[bCmdSize++] = HIBYTE(timetableOffline.m_parameterOffline->m_wTimeUnit);

	//加速格数
	wLo = LOWORD(timetableOffline.m_parameterOffline->m_dwAecNum);
	wHi = HIWORD(timetableOffline.m_parameterOffline->m_dwAecNum);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//减速（快到慢）格数
	wLo = LOWORD(timetableOffline.m_parameterOffline->m_dwFast2SlowNum);
	wHi = HIWORD(timetableOffline.m_parameterOffline->m_dwFast2SlowNum);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//减速（慢到停）距离
	wLo = LOWORD(timetableOffline.m_parameterOffline->m_dwSlow2StopNum);
	wHi = HIWORD(timetableOffline.m_parameterOffline->m_dwSlow2StopNum);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//快速度
	wLo = LOWORD(timetableOffline.m_parameterOffline->m_dwFastV);
	wHi = HIWORD(timetableOffline.m_parameterOffline->m_dwFastV);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//慢速度
	wLo = LOWORD(timetableOffline.m_parameterOffline->m_dwSlowV);
	wHi = HIWORD(timetableOffline.m_parameterOffline->m_dwSlowV);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//加速曲线特殊点
	for (int i=0; i<timetableOffline.m_parameterOffline->m_bAecRecordNum; i++)
	{
		wLo = LOWORD(timetableOffline.m_parameterOffline->m_dwAecRecordArray.GetAt(i));
		wHi = HIWORD(timetableOffline.m_parameterOffline->m_dwAecRecordArray.GetAt(i));
		bCmdData[bCmdSize++] = LOBYTE(wLo);
		bCmdData[bCmdSize++] = HIBYTE(wLo);
		bCmdData[bCmdSize++] = LOBYTE(wHi);
		bCmdData[bCmdSize++] = HIBYTE(wHi);
	}

	//快到慢曲线特殊点
	for (int i=0; i<timetableOffline.m_parameterOffline->m_bFast2SlowRecordNum; i++)
	{
		wLo = LOWORD(timetableOffline.m_parameterOffline->m_dwFast2SlowRecordArray.GetAt(i));
		wHi = HIWORD(timetableOffline.m_parameterOffline->m_dwFast2SlowRecordArray.GetAt(i));
		bCmdData[bCmdSize++] = LOBYTE(wLo);
		bCmdData[bCmdSize++] = HIBYTE(wLo);
		bCmdData[bCmdSize++] = LOBYTE(wHi);
		bCmdData[bCmdSize++] = HIBYTE(wHi);
	}

	//慢到停曲线特殊点
	for (int i=0; i<timetableOffline.m_parameterOffline->m_bSlow2StopRecordNum; i++)
	{
		wLo = LOWORD(timetableOffline.m_parameterOffline->m_dwSlow2StopRecordArray.GetAt(i));
		wHi = HIWORD(timetableOffline.m_parameterOffline->m_dwSlow2StopRecordArray.GetAt(i));
		bCmdData[bCmdSize++] = LOBYTE(wLo);
		bCmdData[bCmdSize++] = HIBYTE(wLo);
		bCmdData[bCmdSize++] = LOBYTE(wHi);
		bCmdData[bCmdSize++] = HIBYTE(wHi);
	}

	ASSERT(bCmdSize<=0xFF);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SF_DownloadOfflineTimeTableParameter = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_DownloadOfflineTimeTableParameter, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_set_offline_timetable_timevalue(BYTE bMotoWay,const TIME_TABLE_OFFLINE &timetableOffline,int time_check,BOOL bFlag)
{
	int		nResult	= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_download_timetable_timevalue;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;

	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;

	if (!IsOpen())
		return FALSE;

	WORD wCount = 0;
	int j = 0;
	BOOL bFinish = FALSE;

	int nAec = timetableOffline.m_parameterOffline->m_dwAecNum;
	int nFast2Slow = timetableOffline.m_parameterOffline->m_dwFast2SlowNum;
	int nSlow2Stop = timetableOffline.m_parameterOffline->m_dwSlow2StopNum;

	while(1)
	{	
		nResult	= rpComBase::sms_err_send;
		wCount++;

		bCmdSize = 0;
		if (bDstAddr == addr_servo_three_axis)
		{
			bCmdData[bCmdSize++] = bAxisWay;
		}

		bCmdData[bCmdSize++] = LOBYTE(wCount);
		bCmdData[bCmdSize++] = HIBYTE(wCount);

		for(int i=0; i<(servo_download_timevalue_length-2)/2; i++)
		{
			if (j>=nAec+nFast2Slow+nSlow2Stop)
			{
				bFinish = TRUE;
				break;
			}
			if (j>=nAec+nFast2Slow)
			{
				bCmdData[bCmdSize++] = LOBYTE(timetableOffline.m_wSlow2StopTimeArray.GetAt(j-(nAec+nFast2Slow)));
				bCmdData[bCmdSize++] = HIBYTE(timetableOffline.m_wSlow2StopTimeArray.GetAt(j-(nAec+nFast2Slow)));
				j++;
				continue;
			}
			if (j>=nAec)
			{
				bCmdData[bCmdSize++] = LOBYTE(timetableOffline.m_wFast2SlowTimeArray.GetAt(j-nAec));
				bCmdData[bCmdSize++] = HIBYTE(timetableOffline.m_wFast2SlowTimeArray.GetAt(j-nAec));
				j++;
				continue;
			}
			bCmdData[bCmdSize++] = LOBYTE(timetableOffline.m_wAecTimeArray.GetAt(j));
			bCmdData[bCmdSize++] = HIBYTE(timetableOffline.m_wAecTimeArray.GetAt(j));
			j++;
		}

		ASSERT(bCmdSize<=servo_download_timevalue_length);

		msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
		ASSERT(msg_len >= pt_data_len_min);

		m_protocolstatus.bConfigWay_SF_DownloadTimeTableValue = comm_result_checking;
		nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_DownloadTimeTableValue, flagCheck, nCheckCount, time_check, bFlag);
		if (nResult!=rpComBase::sms_handshake_ok)
		{
			break;
		}

		::SendMessage(m_pOfflineTimeTableDlg->m_hWnd, WM_SFDOWNLOAD_PROGRESS, (WPARAM)j, 0);

		if (bFinish==TRUE)
		{
			break;
		}
	}

	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_timetable_vital_parameter(BYTE bMotoWay,BYTE bSpeedLevel,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_timetable_vital_parameter;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpeedLevel;

	ASSERT(bCmdSize==1);
	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetTimeTableVitalParameter = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetTimeTableVitalParameter, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_online_timetable_paramater(BYTE bMotoWay,BYTE bSpeedLevel,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_on_timetable_pamarater;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpeedLevel;

	ASSERT(bCmdSize==1);
	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetOnlineTimeTableParameter = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetOnlineTimeTableParameter, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_offline_timetable_paramater(BYTE bMotoWay,BYTE bSpeedLevel,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_off_timetable_pamarater;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpeedLevel;

	ASSERT(bCmdSize==1);
	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetOfflineTimeTableParameter = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetOfflineTimeTableParameter, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_count_fullpulse(BYTE bMotoWay,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_count_fullpulse;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_CountFullPulse = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_CountFullPulse, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_motor_move_relative(BYTE bMotoWay, BYTE bSpeedType,DWORD dwPulse,DWORD dwRePulse,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_move_relative;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpeedType;

	wLo = LOWORD(dwPulse);
	wHi = HIWORD(dwPulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(dwRePulse);
	wHi = HIWORD(dwRePulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);
	
	m_protocolstatus.bPrintWay_SF_MotorMoveRelative = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorMoveRelative, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_motor_move_absolute(BYTE bMotoWay, BYTE bSpeedType,DWORD dwPulse,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_move_absolute;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpeedType;

	wLo = LOWORD(dwPulse);
	wHi = HIWORD(dwPulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_MotorMoveAbsolute = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorMoveAbsolute, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_motor_get_position(BYTE bMotoWay,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_get_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_MotorGetPosition = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_MotorGetPosition, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_motor_move_special_position(BYTE bMotoWay, BYTE bSpeedType,BYTE bSpecialPosition,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_move_special_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpeedType;
	bCmdData[bCmdSize++] = bSpecialPosition;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_MotorMoveSpecialPosition = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorMoveSpecialPosition, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_motor_move_stop(BYTE bMotoWay,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_move_stop;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}
	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_MotorMoveStop = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorMoveStop, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_motor_move(BYTE bMotoWay, BYTE bMoveType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_move_offline;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bMoveType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_MotorMove = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorMove, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_fullpulse(BYTE bMotoWay,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_fullpulse;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}
	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetFullPulse = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetFullPulse, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_special_position(BYTE bMotoWay, BYTE bSpecialPosition,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_special_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpecialPosition;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetSpecialPosition = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetSpecialPosition, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_set_special_position(BYTE bMotoWay, BYTE bSpecialPosition,DWORD dwSpecialPosition,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_set_special_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	bCmdData[bCmdSize++] = bSpecialPosition;

	wLo = LOWORD(dwSpecialPosition);
	wHi = HIWORD(dwSpecialPosition);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SF_SetSpecialPosition = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_SetSpecialPosition, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_delete_all_timetable_data(BYTE bMotoWay,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_delete_all_timetable_data;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;
	BYTE	bAxisWay = sv_three_axis_way_unknown;

	switch(bMotoWay)
	{
	default:
		ASSERT(0);
		return FALSE;
	case motor_way_x:
		bDstAddr = addr_servo_x;
		break;
	case motor_way_y:
		bDstAddr = addr_servo_y;
		break;
	case motor_way_3Axis_x:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_x;
		break;
	case motor_way_3Axis_y:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_y;
		break;
	case motor_way_3Axis_z:
		bDstAddr = addr_servo_three_axis;
		bAxisWay = sv_three_axis_way_z;
		break;
	}
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = bAxisWay;
	}

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SF_DeleteAllTimeTableData = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_DeleteAllTimeTableData, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_set_board_parameter_x(SF_BOARD_PARAMETER_X sf_board_parameter_x,int time_check,BOOL bFlag, BOOL bThreeAxisMotor)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_set_firm_parameter;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;
	
	DWORD dwPara;
	bCmdSize = 0;

	BYTE	bDstAddr;

	bDstAddr = addr_servo_x;
	if (bThreeAxisMotor)
		bDstAddr = addr_servo_three_axis;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = sv_three_axis_way_x;
	}

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_x.m_fReducerX);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_x.m_fDollyPerimeter);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_x.m_fPulsesPerRevolution_x);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, bDstAddr, bCmd, 0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SF_SetFirmParameterX = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_SetFirmParameterX, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_set_board_parameter_y(SF_BOARD_PARAMETER_Y sf_board_parameter_y,int time_check,BOOL bFlag, BOOL bThreeAxisMotor)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_set_firm_parameter;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	DWORD dwPara;
	bCmdSize = 0;

	BYTE	bDstAddr;

	bDstAddr = addr_servo_y;
	if (bThreeAxisMotor)
		bDstAddr = addr_servo_three_axis;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = sv_three_axis_way_y;
	}

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_y.m_fReducerY);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_y.m_fDeferentPerimeter);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_y.m_fDeferentPitch);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_y.m_fPulsesPerRevolution_y);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SF_SetFirmParameterY = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_SetFirmParameterY, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
BOOL CCommunicationPort::servo_set_board_parameter_z(SF_BOARD_PARAMETER_Z sf_board_parameter_z,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_set_firm_parameter;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	DWORD dwPara;
	bCmdSize = 0;

	BYTE	bDstAddr;

	bDstAddr = addr_servo_three_axis;
	bCmdData[bCmdSize++] = sv_three_axis_way_z;

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_z.m_fReducer_Z);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_z.m_fDeferentPerimeter_Z);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_z.m_fDeferentPitch_Z);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwPara,sf_board_parameter_z.m_fPulsesPerRevolution_Z);
	wLo = LOWORD(dwPara);
	wHi = HIWORD(dwPara);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SF_SetFirmParameterZ = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SF_SetFirmParameterZ, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_board_parameter_x(int time_check,BOOL bFlag, BOOL bThreeAxisMotor)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_firm_parameter;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	BYTE	bDstAddr;

	bCmdSize = 0;
	bDstAddr = addr_servo_x;
	if (bThreeAxisMotor)
		bDstAddr = addr_servo_three_axis;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = sv_three_axis_way_x;
	}

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetFirmParameterX = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetFirmParameterX, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_board_parameter_y(int time_check,BOOL bFlag, BOOL bThreeAxisMotor)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_firm_parameter;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	BYTE	bDstAddr;

	bCmdSize = 0;
	bDstAddr = addr_servo_y;
	if (bThreeAxisMotor)
		bDstAddr = addr_servo_three_axis;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = sv_three_axis_way_y;
	}
	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetFirmParameterY = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetFirmParameterY, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::servo_get_board_parameter_z(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_get_firm_parameter;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	BYTE	bDstAddr;

	bCmdSize = 0;
	bDstAddr = addr_servo_three_axis;
	if (bDstAddr == addr_servo_three_axis)
	{
		bCmdData[bCmdSize++] = sv_three_axis_way_z;
	}
	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SF_GetFirmParameterZ = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SF_GetFirmParameterZ, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_print_start(MB_PRINT_START_PARAMETER mb_print_start_parameter,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_print_start;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = mb_print_start_parameter.m_bSpeedLevelX;
	bCmdData[bCmdSize++] = mb_print_start_parameter.m_bSpeedLevelY;

	wLo = LOWORD(mb_print_start_parameter.m_dwPositionX);
	wHi = HIWORD(mb_print_start_parameter.m_dwPositionX);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	wLo = LOWORD(mb_print_start_parameter.m_dwPositionY);
	wHi = HIWORD(mb_print_start_parameter.m_dwPositionY);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	bCmdData[bCmdSize++] = LOBYTE(mb_print_start_parameter.m_wPurgeTime);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_start_parameter.m_wPurgeTime);

	bCmdData[bCmdSize++] = LOBYTE(mb_print_start_parameter.m_bSprayTime);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_start_parameter.m_bSprayTime);

	bCmdData[bCmdSize++] = LOBYTE(mb_print_start_parameter.m_wSprayFreq);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_start_parameter.m_wSprayFreq);

	bCmdData[bCmdSize++] = LOBYTE(mb_print_start_parameter.m_bSprayInterval);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_start_parameter.m_bSprayInterval);

	for (int i=0;i<8;i++)
	{
		bCmdData[bCmdSize++] = mb_print_start_parameter.m_bPower[i];
	}

	for (int i=0;i<8;i++)
	{
		bCmdData[bCmdSize++] = LOBYTE(mb_print_start_parameter.m_dwPulseWidth[i]);
		bCmdData[bCmdSize++] = HIBYTE(mb_print_start_parameter.m_dwPulseWidth[i]);
	}
	
	wLo = LOWORD(mb_print_start_parameter.m_dwWidth);
	wHi = HIWORD(mb_print_start_parameter.m_dwWidth);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(mb_print_start_parameter.m_dwHeight);
	wHi = HIWORD(mb_print_start_parameter.m_dwHeight);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	bCmdData[bCmdSize++] = LOBYTE(mb_print_start_parameter.m_wResX);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_start_parameter.m_wResX);
	bCmdData[bCmdSize++] = LOBYTE(mb_print_start_parameter.m_wResY);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_start_parameter.m_wResY);
	
	bCmdData[bCmdSize++] = mb_print_start_parameter.m_bPrintWay;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_gcb,addr_mb,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_PrintStart = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_PrintStart, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_print_line(MB_PRINT_LINE_PARAMETER mb_print_line_parameter,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_print_line;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	wLo = LOWORD(mb_print_line_parameter.m_dwStartPos);
	wHi = HIWORD(mb_print_line_parameter.m_dwStartPos);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	wLo = LOWORD(mb_print_line_parameter.m_dwWidth);
	wHi = HIWORD(mb_print_line_parameter.m_dwWidth);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	bCmdData[bCmdSize++] = LOBYTE(mb_print_line_parameter.m_wFreqPscM);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_line_parameter.m_wFreqPscM);
	bCmdData[bCmdSize++] = LOBYTE(mb_print_line_parameter.m_wFreqPscN);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_line_parameter.m_wFreqPscN);
	
	wLo = LOWORD(mb_print_line_parameter.m_dwFreqDistD1);
	wHi = HIWORD(mb_print_line_parameter.m_dwFreqDistD1);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(mb_print_line_parameter.m_dwFreqDistD2);
	wHi = HIWORD(mb_print_line_parameter.m_dwFreqDistD2);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(mb_print_line_parameter.m_dwFreqDistD3);
	wHi = HIWORD(mb_print_line_parameter.m_dwFreqDistD3);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	wLo = LOWORD(mb_print_line_parameter.m_dwPulse);
	wHi = HIWORD(mb_print_line_parameter.m_dwPulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(mb_print_line_parameter.m_dwRePulse);
	wHi = HIWORD(mb_print_line_parameter.m_dwRePulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(mb_print_line_parameter.m_dwYDist);
	wHi = HIWORD(mb_print_line_parameter.m_dwYDist);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	bCmdData[bCmdSize++] = mb_print_line_parameter.m_bAction;
	
	bCmdData[bCmdSize++] = LOBYTE(mb_print_line_parameter.m_wThousandth);
	bCmdData[bCmdSize++] = HIBYTE(mb_print_line_parameter.m_wThousandth);
	
	wLo = LOWORD(mb_print_line_parameter.m_dwPrintHeight);
	wHi = HIWORD(mb_print_line_parameter.m_dwPrintHeight);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	//bCmdData[bCmdSize++] = mb_print_line_parameter.m_bSelfN;
	//
	//for (int i=0;i<mb_print_line_parameter.m_bSelfN;i++)
	//{
	//	bCmdData[bCmdSize++] = mb_print_line_parameter.m_bSelfInfo[i];
	//}

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_gcb,addr_mb,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_PrintLine = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_PrintLine, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_print_end(BYTE bPosition,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_print_end;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bPosition;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_gcb,addr_mb,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_PrintEnd = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_PrintEnd, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_print_pause(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_print_pause_gcb;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_gcb,addr_mb,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_PrintPause = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_PrintPause, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_print_resume(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_print_resume_gcb;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_gcb, addr_mb,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_PrintResume = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_PrintResume, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_print_stop(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_print_stop_gcb;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_gcb, addr_mb,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_PrintStop = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_PrintStop, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_move_absolute_position(DWORD dwPositionX,DWORD dwPositionY,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_move_absolute_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;
	
	bCmdSize = 0;
	wLo = LOWORD(dwPositionX);
	wHi = HIWORD(dwPositionX);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(dwPositionY);
	wHi = HIWORD(dwPositionY);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_gcb, addr_mb,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_MoveAbsolutePosition = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_MoveAbsolutePosition, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_move_relative_position(DWORD dwPulseX,DWORD dwPulseY,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_move_relative_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	wLo = LOWORD(dwPulseX);
	wHi = HIWORD(dwPulseX);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(dwPulseY);
	wHi = HIWORD(dwPulseY);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_gcb, addr_mb,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_MoveRelativePosition = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_MoveRelativePosition, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_move_to_special_position(BYTE bSpecialPosition,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_move_to_special_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bSpecialPosition;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_gcb, addr_mb,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_Move2SpecialPosition = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_Move2SpecialPosition, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_store_current_position(BYTE bCurrentPosition,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_mb_store_current_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bCurrentPosition;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_gcb, addr_mb,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_MB_StoreCurrentPosition = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_MB_StoreCurrentPosition, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::mb_confirmation_online_state(void)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_gcb_online_state;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_gcb, addr_mb,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, rpComBase::sml_onlyone,FALSE);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::board_card_set_info(BYTE bDstAddr,BOARD_CARD_INFO board_card_info,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_card_setinfo;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = LOBYTE(board_card_info.m_wDeviceVersion);
	bCmdData[bCmdSize++] = HIBYTE(board_card_info.m_wDeviceVersion);

	bCmdData[bCmdSize++] = LOBYTE(board_card_info.m_wProtocolVersion);
	bCmdData[bCmdSize++] = HIBYTE(board_card_info.m_wProtocolVersion);

	wLo = LOWORD(board_card_info.m_dwSN);
	wHi = HIWORD(board_card_info.m_dwSN);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(board_card_info.m_dwDate);
	wHi = HIWORD(board_card_info.m_dwDate);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	wLo = LOWORD(board_card_info.m_dwProducerID);
	wHi = HIWORD(board_card_info.m_dwProducerID);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	wLo = LOWORD(board_card_info.m_dwCheckerID);
	wHi = HIWORD(board_card_info.m_dwCheckerID);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_BoardCard_SetInfo = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_BoardCard_SetInfo, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::board_card_get_info(BYTE bDstAddr,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_card_queryinfo;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_pc,bDstAddr,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_BoardCard_GetInfo = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_BoardCard_GetInfo, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::board_card_set_addr(DWORD dwSN,BYTE bAddr,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_card_setaddr;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	wLo = LOWORD(dwSN);
	wHi = HIWORD(dwSN);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	bCmdData[bCmdSize++] = bAddr;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_pc,addr_none,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_BoardCard_SetAddr = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_BoardCard_SetAddr, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::board_card_get_addr(DWORD dwSN,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_card_query;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	wLo = LOWORD(dwSN);
	wHi = HIWORD(dwSN);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_pc,addr_none,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_BoardCard_GetAddr = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_BoardCard_GetAddr, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ink_check_485(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ink_check_485;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_mb,addr_ink,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_Ink_Check485 = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_Ink_Check485, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ink_check_state(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ink_check_state;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_mb,addr_ink,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_Ink_CheckState = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_Ink_CheckState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ink_force(WORD wForce,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ink_force;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;
	
	bCmdSize = 0;
	bCmdData[bCmdSize++] = LOBYTE(wForce);
	bCmdData[bCmdSize++] = HIBYTE(wForce);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_ink,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_Ink_Force = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_Ink_Force, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ink_press(WORD wPress,WORD wPressTime,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ink_press;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = LOBYTE(wPress);
	bCmdData[bCmdSize++] = HIBYTE(wPress);
	
	bCmdData[bCmdSize++] = LOBYTE(wPressTime);
	bCmdData[bCmdSize++] = HIBYTE(wPressTime);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_ink,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_Ink_Press = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_Ink_Press, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ink_scrape(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ink_scrape;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_ink,bCmd,0,NULL,0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_Ink_Scrape = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_Ink_Scrape, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ink_moisturize(BYTE bMoisturize,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ink_moisturize;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;
	
	bCmdSize = 0;
	bCmdData[bCmdSize++] = bMoisturize;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_ink,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_Ink_Moisturize = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_Ink_Moisturize, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ink_cycle_motor(BYTE bOpen,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ink_cycle_motor;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bOpen;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_ink,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_Ink_CycleMotor= mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_Ink_CycleMotor, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_check_state(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_check_state;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_mb,addr_rec_cloth,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_RC_CheckState = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_RC_CheckState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_pass(BYTE bPass,DWORD dwPass,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_pass;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bPass;

	wLo = LOWORD(dwPass);
	wHi = HIWORD(dwPass);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_rec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_RC_Pass = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_RC_Pass, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_print_stop(BYTE bPrintStop,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_print_stop;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bPrintStop;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_rec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_RC_PrintStop = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_RC_PrintStop, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_print_over(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_print_over;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_mb,addr_rec_cloth,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_RC_PrintOver = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_RC_PrintOver, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_check_state(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_check_state;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,  addr_mb,addr_sec_cloth,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_SC_CheckState = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_SC_CheckState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_band_dry(BYTE bBandDry,WORD wBandDry,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_band_dry;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bBandDry;
	bCmdData[bCmdSize++] = LOBYTE(wBandDry);
	bCmdData[bCmdSize++] = HIBYTE(wBandDry);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SC_BandDry = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SC_BandDry, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_pre_dry(BYTE bPreDry,WORD wPreDry,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_pre_dry;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bPreDry;
	bCmdData[bCmdSize++] = LOBYTE(wPreDry);
	bCmdData[bCmdSize++] = HIBYTE(wPreDry);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SC_PreDry = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SC_PreDry, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_band_wash(BYTE bOpen,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_band_wash;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bOpen;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SC_BandWash = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SC_BandWash, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_press_roller(BYTE bOpen,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_press_roller;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bOpen;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SC_PressRoller = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SC_PressRoller, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_motor(BYTE bOpen,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_motor;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bOpen;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SC_Motor = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SC_Motor, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_stop(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_stop;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_sec_cloth,bCmd,0,NULL,0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SC_Stop = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SC_Stop, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_cancel_wrong_label(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_cancle_wrong_lable;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_sec_cloth,bCmd,0,NULL,0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_SC_CancleWrongLabel = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_SC_CancleWrongLabel, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::cb_auto_correct(DWORD dwPulse,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_cb_auto_correct;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	wLo = LOWORD(dwPulse);
	wHi = HIWORD(dwPulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_correct,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_CB_AutoCorrect = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_CB_AutoCorrect, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::cb_manual_correct(BYTE bType,DWORD dwPulse,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_cb_manual_correct;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	
	bCmdData[bCmdSize++] = bType;

	wLo = LOWORD(dwPulse);
	wHi = HIWORD(dwPulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_correct,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_CB_ManualCorrect = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_CB_ManualCorrect, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::cb_manual_correct_stop(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_cb_manual_correct_stop;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[2];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_mb,addr_correct,bCmd,0,NULL,0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_CB_ManualCorrectStop = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_CB_ManualCorrectStop, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ras_print_start(RAS_PRINT_START_PARAMETER ras_print_start_parameter,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ras_print_start;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = ras_print_start_parameter.m_bSpeedLevelX;
	bCmdData[bCmdSize++] = ras_print_start_parameter.m_bSpeedLevelY;

	wLo = LOWORD(ras_print_start_parameter.m_dwPositionX);
	wHi = HIWORD(ras_print_start_parameter.m_dwPositionX);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(ras_print_start_parameter.m_dwPositionY);
	wHi = HIWORD(ras_print_start_parameter.m_dwPositionY);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);
	
	bCmdData[bCmdSize++] = LOBYTE(ras_print_start_parameter.m_wPurgeTime);
	bCmdData[bCmdSize++] = HIBYTE(ras_print_start_parameter.m_wPurgeTime);

	wLo = LOWORD(ras_print_start_parameter.m_dwWidth);
	wHi = HIWORD(ras_print_start_parameter.m_dwWidth);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(ras_print_start_parameter.m_dwHeight);
	wHi = HIWORD(ras_print_start_parameter.m_dwHeight);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	bCmdData[bCmdSize++] = LOBYTE(ras_print_start_parameter.m_wResX);
	bCmdData[bCmdSize++] = HIBYTE(ras_print_start_parameter.m_wResX);
	bCmdData[bCmdSize++] = LOBYTE(ras_print_start_parameter.m_wResY);
	bCmdData[bCmdSize++] = HIBYTE(ras_print_start_parameter.m_wResY);

	bCmdData[bCmdSize++] = ras_print_start_parameter.m_bPrintWay;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd, 0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RAS_PrintStart = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RAS_PrintStart, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ras_print_line(RAS_PRINT_LINE_PARAMETER ras_print_line_parameter,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ras_print_line;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	wLo = LOWORD(ras_print_line_parameter.m_dwStartPos);
	wHi = HIWORD(ras_print_line_parameter.m_dwStartPos);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(ras_print_line_parameter.m_dwWidth);
	wHi = HIWORD(ras_print_line_parameter.m_dwWidth);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	bCmdData[bCmdSize++] = LOBYTE(ras_print_line_parameter.m_wFreq);
	bCmdData[bCmdSize++] = HIBYTE(ras_print_line_parameter.m_wFreq);

	wLo = LOWORD(ras_print_line_parameter.m_dwPulse);
	wHi = HIWORD(ras_print_line_parameter.m_dwPulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(ras_print_line_parameter.m_dwRePulse);
	wHi = HIWORD(ras_print_line_parameter.m_dwRePulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(ras_print_line_parameter.m_dwYDist);
	wHi = HIWORD(ras_print_line_parameter.m_dwYDist);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	bCmdData[bCmdSize++] = ras_print_line_parameter.m_bAction;

	bCmdData[bCmdSize++] = LOBYTE(ras_print_line_parameter.m_wThousandth);
	bCmdData[bCmdSize++] = HIBYTE(ras_print_line_parameter.m_wThousandth);

	wLo = LOWORD(ras_print_line_parameter.m_dwPrintHeight);
	wHi = HIWORD(ras_print_line_parameter.m_dwPrintHeight);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd, 0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RAS_PrintLine = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RAS_PrintLine, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ras_print_end(BYTE bPosition,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ras_print_end;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bPosition;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd, 0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RAS_PrintEnd = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RAS_PrintEnd, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ras_get_speed(BYTE bMotoDirection,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ras_getspeed;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bMotoDirection;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd, 0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RAS_GetSpeed = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RAS_GetSpeed, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ras_print_stop(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ras_print_stop_gcb;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd, 0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RAS_PrintStop = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RAS_PrintStop, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ras_get_position(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ras_getposition;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd, 0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RAS_GetPosition = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RAS_GetPosition, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ras_select(BYTE bSelect,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ras_select;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bSelect;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd, 0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RAS_Select = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RAS_Select, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ras_confirmation(BYTE bCmd)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, rpComBase::sml_onlyone, FALSE);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

DWORD CCommunicationPort::DataFrameAnalyze_PrintMode(const BYTE* pBuffer, DWORD dwBufferSize)
{
	BYTE	bCmd;
	BYTE	bCmdSize;
	DWORD	dwFrameLen;
	BYTE	bCRC;
	DWORD	dwEndFlagPos;		//数据帧结束标识索引
	DWORD	dwCRCDataPos;		//数据帧CRC校验位索引
	DWORD	dwResult = 0;
	int		nIndex;
	CString str;

	m_receivedataex_nReturnFlagLen = 0;		//默认不反馈信息给MB

	if (pBuffer==NULL || dwBufferSize==0)
		return 0;

	if (dwBufferSize < 6)
		return 0;	//参数不全，达不到最小长度要求

	if (pBuffer[0] != 0xE0
		|| pBuffer[1] != 0xE1)
	{//非有效数据帧，第一个字节标识为无效值
		ASSERT(0);
		dwResult = 1;
		return dwResult;
	}

	bCmd			= pBuffer[2];
	bCmdSize		= pBuffer[3];
	dwEndFlagPos	= 4;
	dwEndFlagPos	+= bCmdSize;
	dwFrameLen		= dwEndFlagPos + 2;
	dwCRCDataPos	= dwFrameLen - 1;

	if (dwBufferSize < dwFrameLen)
	{//指令串不全
		dwResult = 0;
		goto end;		
	}

	if (pBuffer[dwEndFlagPos] != 0xFC)
	{//非有效结束标识，前2个字节标识为无效值
dataerror:
		m_receivedataex_bReturnFlag[0] = 0xE4;
		m_receivedataex_nReturnFlagLen = 1;
		dwResult = 2;
		goto end;
	}
	//计算CRC校验位值
	bCRC = CalcCRC(pBuffer, dwCRCDataPos);
	if (pBuffer[dwCRCDataPos] != bCRC)
	{//检验位出错，前2个字节标识为无效值
		goto dataerror;
	}

	//有效参数数据起始
	pBuffer += 4;
	dwResult = dwFrameLen;

	switch (bCmd)
	{
	case cmd_ras_print_stop:
	case cmd_ras_print_pause:
	case cmd_ras_print_resume:
		::PostMessage(m_pRasDlg->m_hWnd,WM_RASSTATE, (WPARAM)(bCmd), 0);
		break;
	case cmd_ras_sys_error:
		m_protocolstatus.dwErrorCode = PROTOCOL_MAKEDATA_DWORD(pBuffer);
		str.Format("系统错误:%X",m_protocolstatus.dwErrorCode);
		AfxMessageBox(str);
		::PostMessage(m_pRasDlg->m_hWnd,WM_RASSTATE, (WPARAM)(bCmd), 0);
		break;
	case cmd_ras_sys_exception:
		m_protocolstatus.dwException = PROTOCOL_MAKEDATA_DWORD(pBuffer);
		str.Format("系统异常:%X",m_protocolstatus.dwException);
		AfxMessageBox(str);
		::PostMessage(m_pRasDlg->m_hWnd,WM_RASSTATE, (WPARAM)(bCmd), 0);
		break;
	case cmd_ras_print_start:
		m_protocolstatus.bPrintWay_RAS_PrintStart = (BYTE)((bCmdSize==0) ? mb_comm_result_succeed : mb_comm_result_failed);
		break;
	case cmd_ras_print_line:
		m_protocolstatus.bPrintWay_RAS_PrintLine = (BYTE)((bCmdSize==0) ? mb_comm_result_succeed : mb_comm_result_failed);
		break;
	case cmd_ras_print_end:
		m_protocolstatus.bPrintWay_RAS_PrintEnd = (BYTE)((bCmdSize==0) ? mb_comm_result_succeed : mb_comm_result_failed);
		break;
	case cmd_ras_select:
		m_protocolstatus.bPrintWay_RAS_Select = (BYTE)((bCmdSize==0) ? mb_comm_result_succeed : mb_comm_result_failed);
		break;
	case cmd_ras_print_stop_gcb:
		m_protocolstatus.bPrintWay_RAS_PrintStop = (BYTE)((bCmdSize==0) ? mb_comm_result_succeed : mb_comm_result_failed);
		break;
	case cmd_ras_getposition:
		{
			BZERO(m_protocolstatus.positionPara);
			m_protocolstatus.positionPara.pos_x_min = PROTOCOL_MAKEDATA_DWORD(pBuffer);
			pBuffer+=4;
			m_protocolstatus.positionPara.pos_x_max = PROTOCOL_MAKEDATA_DWORD(pBuffer);
			pBuffer+=4;
			m_protocolstatus.positionPara.pos_x_clean = PROTOCOL_MAKEDATA_DWORD(pBuffer);
			pBuffer+=4;
			m_protocolstatus.positionPara.pos_x_moisture = PROTOCOL_MAKEDATA_DWORD(pBuffer);
			pBuffer+=4;
			m_protocolstatus.positionPara.pos_y_min = PROTOCOL_MAKEDATA_DWORD(pBuffer);
			pBuffer+=4;
			m_protocolstatus.positionPara.pos_y_max = PROTOCOL_MAKEDATA_DWORD(pBuffer);

			m_protocolstatus.bPrintWay_RAS_GetPosition = mb_comm_result_succeed;
		}
		break;
	case cmd_ras_getspeed:
		{
			BZERO(m_protocolstatus.onlinePara);
			m_protocolstatus.nSpeedLevelNum = *pBuffer;
			pBuffer++;
			if (m_protocolstatus.nSpeedLevelNum>10)
			{
				m_protocolstatus.nSpeedLevelNum = 10;
			}
			for (int i=0;i<m_protocolstatus.nSpeedLevelNum;i++)
			{
				m_protocolstatus.onlinePara[i].dist_acce = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.onlinePara[i].dist_dece = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
				m_protocolstatus.onlinePara[i].speed = PROTOCOL_MAKEDATA_DWORD(pBuffer);
				pBuffer+=4;
			}
			m_protocolstatus.bPrintWay_RAS_GetSpeed = mb_comm_result_succeed;
		}
		break;
	case cmd_ras_move_absolute_position:
		m_protocolstatus.bPrintWay_RAS_Move2Position = (BYTE)((bCmdSize==0) ? mb_comm_result_succeed : mb_comm_result_failed);
		break;
	}
	m_receivedataex_bReturnFlag[0] = protocol_command_ok;
	m_receivedataex_nReturnFlagLen = 1;

end:
	return dwResult;
}

BOOL CCommunicationPort::ras_move2_position(DWORD x,DWORD y,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ras_move_absolute_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[4];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= mb_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= mb_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	wLo = LOWORD(x);
	wHi = HIWORD(x);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	wLo = LOWORD(y);
	wHi = HIWORD(y);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc, addr_raster, bCmd, 0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RAS_Move2Position = mb_comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RAS_Move2Position, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

//RC NEW
BOOL CCommunicationPort::rc_servo_motor_move_distance(BYTE bSpeedType,DWORD dwPulse,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_servo_motor_move_distance;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bSpeedType;

	wLo = LOWORD(dwPulse);
	wHi = HIWORD(dwPulse);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_rec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_up_down_motor_move(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_up_down_motor_move;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_rec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_servo_motor_move(BYTE bSpeedType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_servo_motor_move;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bSpeedType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_rec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_heater_con(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_heater_con;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_rec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_cloth_motor_con(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_cloth_motor_con;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_rec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::rc_up_down_motor_reset(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_rc_up_down_motor_reset;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_rec_cloth,bCmd,0,NULL,0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_RC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_RC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

//SC new
BOOL CCommunicationPort::sc_new_sendcloth_check_state(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_sendcloth_check_state;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,NULL,0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_band_dry(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_band_dry;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_band_wash(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_band_wash;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_pre_dry(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_pre_dry;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_dd_band_wash(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_dd_band_wash;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_get_temperature(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_get_temperature;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_press_cloth(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_press_cloth;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_cloth_roller(BYTE bType,DWORD dwType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_band_wash;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;
	
	wLo = LOWORD(dwType);
	wHi = HIWORD(dwType);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_cancle_wrong_lable(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_cancle_wrong_lable;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,NULL,0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_manual_correct(BYTE bType,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_manual_correct;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;
	bCmdData[bCmdSize++] = bType;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,bCmdData,bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::sc_new_manual_correct_stop(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sc_new_manual_correct_stop;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 1000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg,addr_pc,addr_sec_cloth,bCmd,0,NULL,0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SC_ComState = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SC_ComState, flagCheck, nCheckCount, time_check, bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}



BOOL CCommunicationPort::servo_motor_xy_move_relative(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD pulseY, int time_check, BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_xy_move_relative;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	*pBits, bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;

	bDstAddr = addr_servo_three_axis;
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize	= 0;
	pBits		= &(bCmdData[0]);

	*pBits++ = bSpeedTypeX;
	*pBits++ = bSpeedTypeY;

	PROTOCOL_GETDATA_DWORD(pulseX, pBits);
	pBits += 4;

	PROTOCOL_GETDATA_DWORD(pulseY, pBits);
	pBits += 4;

	bCmdSize = pBits - &(bCmdData[0]);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_MotorXY_MoveRelative = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorXY_MoveRelative, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
BOOL CCommunicationPort::servo_motor_xy_move_absolute(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD pulseY, int time_check, BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_xy_move_absolute;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	*pBits, bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;

	bDstAddr = addr_servo_three_axis;
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize	= 0;
	pBits		= &(bCmdData[0]);

	*pBits++ = bSpeedTypeX;
	*pBits++ = bSpeedTypeY;

	PROTOCOL_GETDATA_DWORD(pulseX, pBits);
	pBits += 4;

	PROTOCOL_GETDATA_DWORD(pulseY, pBits);
	pBits += 4;

	bCmdSize = pBits - &(bCmdData[0]);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_MotorXY_MoveAbsolute = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorXY_MoveAbsolute, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
BOOL CCommunicationPort::servo_motor_xy_move_special_position(BYTE bSpeedTypeX, BYTE bSpeedTypeY, BYTE bPosX, BYTE bPosY,int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_xy_move_special_position;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	*pBits, bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;

	bDstAddr = addr_servo_three_axis;
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize	= 0;
	pBits		= &(bCmdData[0]);

	*pBits++ = bSpeedTypeX;
	*pBits++ = bSpeedTypeY;

	*pBits++ = bPosX;
	*pBits++ = bPosY;

	bCmdSize = pBits - &(bCmdData[0]);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_MotorXY_MoveSpecialPosition = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorXY_MoveSpecialPosition, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
BOOL CCommunicationPort::servo_motor_xy_print_line(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD re_pulseX, DWORD yDist, int time_check, BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_sf_motor_xy_print_line;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	BYTE	*pBits, bCmdData[0xFF], bCmdSize;
	WORD	wLo,wHi;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[5];
	int		nCheckCount;
	BYTE	bDstAddr = addr_none;

	bDstAddr = addr_servo_three_axis;
	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_para_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_move_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_download_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize	= 0;
	pBits		= &(bCmdData[0]);

	*pBits++ = bSpeedTypeX;
	*pBits++ = bSpeedTypeY;

	PROTOCOL_GETDATA_DWORD(pulseX, pBits);
	pBits += 4;

	PROTOCOL_GETDATA_DWORD(re_pulseX, pBits);
	pBits += 4;

	PROTOCOL_GETDATA_DWORD(yDist, pBits);
	pBits += 4;

	bCmdSize = pBits - &(bCmdData[0]);

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_SF_MotorXY_PrintLine = comm_result_checking;
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_SF_MotorXY_PrintLine, flagCheck, nCheckCount, time_check,bFlag);
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_set_check_power(const BYTE bPowerChannel, const WORD wVolValue, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_set_check_power;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = bPowerChannel;
	bCmdData[bCmdSize++] = LOBYTE(wVolValue);
	bCmdData[bCmdSize++] = HIBYTE(wVolValue);

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_ld_check_power = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_ld_check_power, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_set_laser_test_power(const BYTE bPowerChannel, const WORD wVolValue, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_set_laser_test_power;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = bPowerChannel;
	bCmdData[bCmdSize++] = LOBYTE(wVolValue);
	bCmdData[bCmdSize++] = HIBYTE(wVolValue);

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_ld_check_power = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_ld_check_power, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
BOOL CCommunicationPort::ld_set_map_data(const WORD* wMapVolValue, const BYTE* bMapPowerValue, BYTE bMapNum, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_set_pd_mapdata;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdSize = 0;

	bCmdData[bCmdSize++] = bMapNum;

	for (int i=0; i<bMapNum; i++)
	{
		bCmdData[bCmdSize++] = LOBYTE(wMapVolValue[i]);
		bCmdData[bCmdSize++] = HIBYTE(wMapVolValue[i]);
		bCmdData[bCmdSize++] = bMapPowerValue[i];
	}

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_map_data = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_map_data, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_get_map_data(int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_get_pd_mapdata;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, NULL, 0);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bQueryWay_map_data = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bQueryWay_map_data, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_set_light_on(const DWORD* dwLightData, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_light_on;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		nSize;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	WORD wLo, wHi;
	bCmdSize = 0;
	nSize		= ld_voltage_max/(8*sizeof(DWORD));
	for (int i=0; i<nSize; i++)
	{
		wLo		= LOWORD(dwLightData[i]);
		wHi		= HIWORD(dwLightData[i]);
		bCmdData[bCmdSize++] = LOBYTE(wLo);
		bCmdData[bCmdSize++] = HIBYTE(wLo);
		bCmdData[bCmdSize++] = LOBYTE(wHi);
		bCmdData[bCmdSize++] = HIBYTE(wHi);
	}
	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_ld_light_on = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_ld_light_on, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_set_light_on_time(const BYTE bChannel, const DWORD dwContinueTime, int time_check,BOOL bFlag)
{
	int		nResult = rpComBase::sms_err_send;
	BYTE	bCmd	= cmd_ld_light_on_time;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;
	WORD	wTemp;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	WORD wLo, wHi;
	bCmdSize = 0;

	//通道
	bCmdData[bCmdSize++] = bChannel;
	//时间
	wLo			= LOWORD(dwContinueTime);
	wHi			= HIWORD(dwContinueTime);
	bCmdData[bCmdSize++] = LOBYTE(wLo);
	bCmdData[bCmdSize++] = HIBYTE(wLo);
	bCmdData[bCmdSize++] = LOBYTE(wHi);
	bCmdData[bCmdSize++] = HIBYTE(wHi);

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_ld_light_on_time = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_ld_light_on_time, flagCheck, nCheckCount, time_check, bFlag);
	
	//return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_set_light_off(const DWORD* dwLightData, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_light_off;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount, nSize;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	WORD wLo, wHi;
	bCmdSize = 0;
	nSize		= ld_voltage_max/(8*sizeof(DWORD));
	for (int i=0; i<nSize; i++)
	{
		wLo		= LOWORD(dwLightData[i]);
		wHi		= HIWORD(dwLightData[i]);
		bCmdData[bCmdSize++] = LOBYTE(wLo);
		bCmdData[bCmdSize++] = HIBYTE(wLo);
		bCmdData[bCmdSize++] = LOBYTE(wHi);
		bCmdData[bCmdSize++] = HIBYTE(wHi);
	}

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bConfigWay_ld_light_off = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bConfigWay_ld_light_off, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_set_pc_power(BYTE bChannel, WORD wVoltage_PC, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_power_set_pc;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize = 0;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdData[bCmdSize++] = bChannel;
	bCmdData[bCmdSize++] = LOBYTE(wVoltage_PC);
	bCmdData[bCmdSize++] = HIBYTE(wVoltage_PC);

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_ld_voltage = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_ld_voltage, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_set_ld_power(BYTE bChannel, WORD wVoltage_LD, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_power_set_ld;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize = 0;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdData[bCmdSize++] = bChannel;
	bCmdData[bCmdSize++] = LOBYTE(wVoltage_LD);
	bCmdData[bCmdSize++] = HIBYTE(wVoltage_LD);

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_ld_voltage = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_ld_voltage, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_get_last_power(BYTE bChannel, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_power_get_last;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize = 0;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdData[bCmdSize++] = bChannel;

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_ld_voltage = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_ld_voltage, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_get_now_power(BYTE bChannel, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_power_get_now;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize = 0;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdData[bCmdSize++] = bChannel;

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_ld_voltage = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_ld_voltage, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}

BOOL CCommunicationPort::ld_correct_power(BYTE bChannel, int time_check,BOOL bFlag)
{
	int		nResult		= rpComBase::sms_err_send;
	BYTE	bCmd		= cmd_ld_power_correct;
	BYTE	snd_msg[pt_data_len_max];
	int		msg_len = 0;
	int		timeout			= 5000;
	int		timerepeatsend	= 1000;
	CComBase::FLAGCHECK flagCheck[10];
	int		nCheckCount;
	BYTE	bCmdData[0xFF];
	BYTE	bCmdSize = 0;

	if (time_check > 0)
	{
		timeout			= min(timeout, time_check);
		timerepeatsend	= min(timerepeatsend, timeout);
	}

	nCheckCount = 0;
	BZERO(flagCheck);
	flagCheck[nCheckCount].bFlag	= ld_comm_result_succeed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_handshake_ok;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_failed;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_offline;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_power_invalid;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	flagCheck[nCheckCount].bFlag	= ld_comm_result_other;
	flagCheck[nCheckCount].nResult= rpComBase::sms_err_fail;
	nCheckCount++;

	if (!IsOpen())
		return FALSE;

	bCmdData[bCmdSize++] = bChannel;

	BYTE	bDstAddr = addr_ld;

	msg_len = CCommunicationPort::dataframe_transdata0(snd_msg, addr_pc,bDstAddr,bCmd,0, bCmdData, bCmdSize);
	ASSERT(msg_len >= pt_data_len_min);

	m_protocolstatus.bPrintWay_ld_voltage = ld_comm_result_checking;
	
	nResult = SendMsg_handshake0(snd_msg, msg_len, timeout, timerepeatsend, &m_protocolstatus.bPrintWay_ld_voltage, flagCheck, nCheckCount, time_check, bFlag);
	
	return nResult==rpComBase::sms_handshake_ok ? TRUE : FALSE;
}
