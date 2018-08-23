#include "../stdafx.h"
#include "ComPort.h"
#include "../include/rpBasexp.h"
#include "rpComBase.h"
#include "..\MBComPort\Protocol.h"
#include "../MBComPort/MBComPort.h"

#define currentComPort()		((zmComPort::CComPort*)m_pComPort)

namespace rpComBase
{


/////////////////////////////////////////////////////////////////////
//pBuf[out]: Com Read Buffer
void ComBase_ReceiveData(LPVOID pSender, const void* pBuf,DWORD InBufferCount)
{
	CComBase* pComBase = (CComBase*)pSender;
	if (pComBase)
	{
		pComBase->ReceiveData(pBuf, InBufferCount);
	}
}

CComBase::CComBase()
{
	m_bDemoMode			= FALSE;	//模拟模式，不使用实际的串口通讯

	m_dwGrowSize		= 4*1024;		//接收缓冲区扩容参数
	m_dwInitBufferSize	= 4*1024;		//初始化接收缓冲区大小

	//接收数据缓冲区
	m_dwBufferSize		= 0;
	m_pBuffer			= NULL;
	m_dwReceiveSize		= 0;

	m_pComPort			= new zmComPort::CComPort;			//串口操作类指针
	ASSERT(m_pComPort != NULL);

	InitializeCriticalSection(&m_csComBaseSync);
	InitializeCriticalSection(&m_csSendSync);

	Init(FALSE);
}
CComBase::~CComBase()
{
	Close(TRUE);

	if (m_pComPort != NULL)
	{
		delete m_pComPort;
		m_pComPort = NULL;
	}
	DeleteCriticalSection(&m_csSendSync);
	DeleteCriticalSection(&m_csComBaseSync);
}	

//	bCopy :[in]重新申请缓冲区时是否要保留原有数据
BOOL CComBase::ReAllocBuffer(DWORD dwBufferSize, BOOL bCopy)
{
	BYTE	*pBuffer = NULL;
	int		nRecSize = m_dwReceiveSize;

	if (m_pBuffer!=NULL && m_dwBufferSize >= dwBufferSize)
	{//预定义缓冲区已经存在
		return TRUE;
	}

	//申请新的缓冲区
	pBuffer = (LPBYTE)AGV::AVirtualAlloc(NULL, dwBufferSize, MEM_COMMIT,PAGE_READWRITE);
	if (pBuffer==NULL)
	{//内存申请不成功，仍使用原缓冲区
		return FALSE;
	}

	TRACE0(">> CComBase::ReAllocBuffer ==\n");
	SyncOpt_Data_Enter();

	memset(pBuffer, 0, dwBufferSize);
	m_dwReceiveSize = 0;
	if (m_pBuffer)
	{//释放原来的缓冲区空间
		if (bCopy)
		{//复制原有数据
			m_dwReceiveSize = nRecSize;
			memcpy(pBuffer, m_pBuffer, m_dwReceiveSize);
		}
		VERIFY(AGV::AVirtualFree(m_pBuffer, 0, MEM_RELEASE));
		m_dwBufferSize = 0;
	}
	m_dwBufferSize	= dwBufferSize;
	m_pBuffer		= pBuffer;
	pBuffer			= NULL;

	SyncOpt_Data_Leave();
	TRACE0("<< CComBase::ReAllocBuffer ==\n");

	return TRUE;
}

//初始化相关参数
void CComBase::Init(BOOL bReAllocBuffer)
{
	SendMsg_handshake_Break(FALSE);			//是否中断当前操作指令串发送状态检测过程

	if (m_pComPort==NULL)
		m_pComPort			= new zmComPort::CComPort;			//串口操作类指针
	if (m_pComPort==NULL)
	{
		ASSERT(0);
		return;
	}

	if (bReAllocBuffer)
	{
		ReAllocBuffer(m_dwInitBufferSize, FALSE);
	}
	
	ClearReadBuffer();
	
	//重设指令发送状态信息
    ResetCommu_Status_Flags();

}
void CComBase::ClearReadBuffer()
{
	if (m_pComPort==NULL)
	{
		ASSERT(0);
		return;
	}
	if (m_dwReceiveSize==0)
		return;

	TRACE0(">> CComBase::ClearReadBuffer ==\n");
	SyncOpt_Data_Enter();

	m_dwReceiveSize = 0;
	
	SyncOpt_Data_Leave();
	TRACE0("<< CComBase::ClearReadBuffer ==\n");
}
//重设指令发送状态信息
void CComBase::ResetCommu_Status_Flags()
{
	if (m_pComPort==NULL)
	{
		ASSERT(0);
		return;
	}
	if (m_bcommu_status == commu_status_none)
		return;

	TRACE0(">> CComBase::ResetCommu_Status_Flags ==\n");
	SyncOpt_Data_Enter();

	m_bcommu_status = commu_status_none;
	SendMsg_handshake_Break(FALSE);			//是否中断当前操作指令串发送状态检测过程
	
	SyncOpt_Data_Leave();
	TRACE0("<< CComBase::ResetCommu_Status_Flags ==\n");
}


BOOL CComBase::IsOpen() const
{
	if (m_pComPort==NULL)
	{
		ASSERT(0);
		return FALSE;
	}
    if (m_bDemoMode)
    {
        return TRUE;
    }

	return currentComPort()->IsOpen(); 
}


BOOL CComBase::IsDemoMode() const
{
	return m_bDemoMode;
}
BOOL CComBase::DemoMode(BOOL bDemoMode)
{
	m_bDemoMode = bDemoMode ? TRUE : FALSE;
	return m_bDemoMode;
}


// port initialisation
//	nBufferSize :[in]初始化接收缓冲区大小
//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
BOOL CComBase::Open(UINT portnr, DWORD dwBaud, int nBufferSize, int nGrowSize)
{
	ASSERT(portnr > 0 && portnr < 17);

	if (nBufferSize > 0)
		m_dwInitBufferSize = nBufferSize;
	if (nGrowSize <= 0)
		nGrowSize = 0;
	else if (nGrowSize < 1024)
		nGrowSize = 1024;

	Close(FALSE);
	if (m_pComPort==NULL)
		m_pComPort			= new zmComPort::CComPort;			//串口操作类指针
	if (m_pComPort==NULL)
	{
		ASSERT(0);
		return FALSE;
	}

    if (m_bDemoMode)
    {
        return TRUE;
    }

	currentComPort()->SetReceiveFunc((FOnReceiveData)ComBase_ReceiveData,this);
	if (!currentComPort()->Open(portnr,
						zmComPort::CComPort::AutoReceiveBySignal, 
						dwBaud, 
						zmComPort::CComPort::NoParity, 
						8, 
						zmComPort::CComPort::OneStopBit,
						zmComPort::CComPort::NoFlowControl
					)
		)
	{
//		CString sMsg;
//		sMsg.Format("ERROR: Can't open com%d!",portnr);
//		AfxMessageBox(sMsg, MB_ICONINFORMATION | MB_OK);
		Close(FALSE);
        return FALSE;
	} 
    
    return TRUE;
}

BOOL CComBase::Close(BOOL bReleaseBuffer)
{
	BOOL bReAllocBuffer = TRUE;

	if (m_pComPort != NULL)
	{
		currentComPort()->SetReceiveFunc((FOnReceiveData)NULL,NULL);
		if (currentComPort()->IsOpen())
		{
			TRACE0(">> CComBase::Close == 1 \n");
			SyncOpt_Send_Enter();
			currentComPort()->Close();
			SyncOpt_Send_Leave();
			TRACE0("<< CComBase::Close == 1 \n");
		}
	}
	if (bReleaseBuffer)
	{
		if (m_pBuffer != NULL)
		{
			TRACE0(">> CComBase::Close == 2 \n");
			SyncOpt_Data_Enter();

			VERIFY(AGV::AVirtualFree(m_pBuffer, 0, MEM_RELEASE));
			m_pBuffer = NULL;

			m_dwBufferSize = 0;
			m_dwReceiveSize = 0;

			SyncOpt_Data_Leave();
			TRACE0("<< CComBase::Close == 2 \n");
		}
		bReAllocBuffer = FALSE;
	}

	Init(bReAllocBuffer);

	return TRUE;
}
//直接发送操作指令，一般仅在 ReceiveData 函数中使用
BOOL CComBase::Commu_SendMsg_Direct(const BYTE* snd_msg, int msg_len)
{
	int nLen;
	if (msg_len<=0 || !snd_msg)
		return FALSE;
	if (m_bDemoMode)
	{
		Sleep(1);
		return TRUE;
	}
	if (m_pComPort==NULL)
	{
		ASSERT(0);
		return FALSE;
	}
	if (!currentComPort()->IsOpen())
	{
		ASSERT(0);
		return FALSE;
	}
	nLen = (int)currentComPort()->Output((BYTE*)snd_msg, msg_len);

	return nLen==msg_len ? TRUE : FALSE;
}

BOOL CComBase::Commu_SendMsg(const BYTE* snd_msg, int msg_len)
{
	int nLen;
	if (msg_len<=0 || !snd_msg)
		return FALSE;

    if (m_bDemoMode)
    {
		Sleep(1);
        return TRUE;
    }

	if (m_pComPort==NULL)
	{
		ASSERT(0);
		return FALSE;
	}

    if (!currentComPort()->IsOpen())
	{
		ASSERT(0);
        return FALSE;
	}

	TRACE0(">> CComBase::Commu_SendMsg ==\n");
	SyncOpt_Send_Enter();
	nLen = (int)currentComPort()->Output((BYTE*)snd_msg, msg_len);
	SyncOpt_Send_Leave();
	TRACE0("<< CComBase::Commu_SendMsg ==\n");

	return nLen==msg_len ? TRUE : FALSE;
}

//操作指令串发送
//参数定义：
//	snd_msg, s_msg_len	:[in]发送的指令串及长度
//	timeout				:[in]发送指令的总延时值 ms
//	time_repeatsend		:[in]当发送失败时，重发指令口串的时间间隔（累积值在 timeout范围内） ms
//	sendmsg_level		:[in] SENDMSG_LEVEL
//返回值：SENDMSG_STATUS
int CComBase::SendMsg_handshake( BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, int sendmsg_level)
{
	DWORD	start_time, pre_send_time;
	int		sendmsg_status = sms_err_send;
	BOOL	bSendOK;

	ResetCommu_Status_Flags();

	sendmsg_status	= sms_err_send;
	bSendOK			= Commu_SendMsg(snd_msg, s_msg_len);

	if (m_bDemoMode)
	{
		Sleep(1);
		return sms_handshake_ok;
	}

// 	if (!bSendOK)
// 	{
// 		goto end;
// 	}


	switch(sendmsg_level)
	{
	case sml_onlyone:
		sendmsg_status	= bSendOK ? sms_handshake_ok : sms_err_send;
		break;
	case sml_normal:
	default:
		start_time		= ::GetTickCount();
		while(1)
		{
			pre_send_time = ::GetTickCount();
			while (1)
			{//在重发时间内
				if (s_msg_len <= 1)
				{//单字节指令发送成功即返回
					if (bSendOK)
					{
						sendmsg_status = sms_handshake_ok;
						goto end;
					}
				}
				else
				{
					switch(m_bcommu_status)
					{
					case commu_status_succeed:
						//MB 返回通讯成功信号
						sendmsg_status = sms_handshake_ok;
						goto end;
						break;
					case commu_status_failed:
						//MB 返回通讯失败信号
						sendmsg_status = sms_err_fail;
						goto end;
						break;
					default:
						break;
					}
				}
				if((DWORD)(pre_send_time + time_repeatsend) < ::GetTickCount())
				{
					//time_repeatsend
					break;
				}	
				//是否中断当前操作指令串发送状态检测过程
				if (m_sendMsg_handshake_bBreak)
				{
					break;
				}
				Sleep(1);
			}
			if ((DWORD)(start_time + timeout) < ::GetTickCount())
			{
				//time out
				break;
			}
			//重发指令串
			bSendOK |= Commu_SendMsg(snd_msg, s_msg_len);
		}
		if (s_msg_len <= 1)
		{//单字节指令发送成功即返回
			if (bSendOK)
			{
				sendmsg_status = sms_handshake_ok;
				goto end;
			}
		}
		else
		{
			switch(m_bcommu_status)
			{
			case commu_status_succeed:
				//MB 返回通讯成功信号
				sendmsg_status = sms_handshake_ok;
				goto end;
				break;
			case commu_status_failed:
				//MB 返回通讯失败信号
				sendmsg_status = sms_err_fail;
				goto end;
				break;
			default:
				break;
			}
		}
		sendmsg_status	= bSendOK ? sms_err_no_ok_fail : sms_err_send;
		break;

	case sml_crucial:
		//必须接收到MB返回的 commu_status_succeed 才退出
		start_time		= ::GetTickCount();
		while(1)
		{
			pre_send_time = ::GetTickCount();
			while (1)
			{//在重发时间内
				if (s_msg_len <= 1 
					|| snd_msg[pt_data_cmd] == cmd_com_motor_move_relative
					|| snd_msg[pt_data_cmd] == cmd_com_get_board_condition)
				{//单字节指令发送成功即返回
					if (bSendOK)
					{
						sendmsg_status = sms_handshake_ok;
						goto end;
					}
				}
				else
				{
					switch(m_bcommu_status)
					{
					case commu_status_succeed:
						//MB 返回通讯成功信号
						sendmsg_status = sms_handshake_ok;
						goto end;
						break;
					case commu_status_failed:
						//MB 返回通讯失败信号
						sendmsg_status = sms_err_fail;
						goto end;
						break;
					default:
						break;
					}
				}
				if((DWORD)(pre_send_time + time_repeatsend) < ::GetTickCount())
				{
					//time_repeatsend
					break;
				}	
				//是否中断当前操作指令串发送状态检测过程
				if (m_sendMsg_handshake_bBreak)
				{
					break;
				}
				Sleep(1);
			}
			if ((DWORD)(start_time + timeout) < ::GetTickCount())
			{
				//time out
				break;
			}
			//是否中断当前操作指令串发送状态检测过程
			if (m_sendMsg_handshake_bBreak)
			{
				break;
			}
			//重发指令串
			snd_msg[pt_data_cmdcount]++;
			bSendOK |= Commu_SendMsg(snd_msg, s_msg_len);
		}
		if (s_msg_len <= 1 
			|| snd_msg[pt_data_cmd] == cmd_com_motor_move_relative
			|| snd_msg[pt_data_cmd] == cmd_com_get_board_condition)
		{//单字节指令发送成功即返回
			if (bSendOK)
			{
				sendmsg_status = sms_handshake_ok;
				goto end;
			}
		}
		else
		{
			switch(m_bcommu_status)
			{
			case commu_status_succeed:
				//MB 返回通讯成功信号
				sendmsg_status = sms_handshake_ok;
				goto end;
				break;
			case commu_status_failed:
				//MB 返回通讯失败信号
				sendmsg_status = sms_err_fail;
				goto end;
				break;
			default:
				break;
			}
		}
		sendmsg_status	= bSendOK ? sms_err_no_ok_fail : sms_err_send;
		break;
	}
end:
	return sendmsg_status;
}

int CComBase::SendMsg_handshake0( BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, int sendmsg_level, BOOL bFlag)
{
	DWORD	start_time, pre_send_time;
	int		sendmsg_status = sms_err_send;
	BOOL	bSendOK;

	ResetCommu_Status_Flags();

	sendmsg_status	= sms_err_send;

	if (bFlag==TRUE)
	{
		snd_msg[pt_data_cmdcount]|=(0x01<<7);
	}
	
	BYTE bCRC = 0x00;
	DWORD i;
	for (i=0; i<s_msg_len-1; i++)
	{
		bCRC += snd_msg[i];
	}
	snd_msg[s_msg_len-1] = bCRC;
	bSendOK			= Commu_SendMsg(snd_msg, s_msg_len);

	if (m_bDemoMode)
	{
		Sleep(1);
		return sms_handshake_ok;
	}

	// 	if (!bSendOK)
	// 	{
	// 		goto end;
	// 	}


	switch(sendmsg_level)
	{
	case sml_onlyone:
		sendmsg_status	= bSendOK ? sms_handshake_ok : sms_err_send;
		break;
	case sml_normal:
	default:
		start_time		= ::GetTickCount();
		while(1)
		{
			pre_send_time = ::GetTickCount();
			while (1)
			{//在重发时间内
				if (s_msg_len <= 1)
				{//单字节指令发送成功即返回
					if (bSendOK)
					{
						sendmsg_status = sms_handshake_ok;
						goto end;
					}
				}
				else
				{
					switch(m_bcommu_status)
					{
					case commu_status_succeed:
						//MB 返回通讯成功信号
						sendmsg_status = sms_handshake_ok;
						goto end;
						break;
					case commu_status_failed:
						//MB 返回通讯失败信号
						sendmsg_status = sms_err_fail;
						goto end;
						break;
					default:
						break;
					}
				}
				if((DWORD)(pre_send_time + time_repeatsend) < ::GetTickCount())
				{
					//time_repeatsend
					break;
				}	
				//是否中断当前操作指令串发送状态检测过程
				if (m_sendMsg_handshake_bBreak)
				{
					break;
				}
				Sleep(1);
			}
			if ((DWORD)(start_time + timeout) < ::GetTickCount())
			{
				//time out
				break;
			}
			//重发指令串
			bSendOK |= Commu_SendMsg(snd_msg, s_msg_len);
		}
		if (s_msg_len <= 1)
		{//单字节指令发送成功即返回
			if (bSendOK)
			{
				sendmsg_status = sms_handshake_ok;
				goto end;
			}
		}
		else
		{
			switch(m_bcommu_status)
			{
			case commu_status_succeed:
				//MB 返回通讯成功信号
				sendmsg_status = sms_handshake_ok;
				goto end;
				break;
			case commu_status_failed:
				//MB 返回通讯失败信号
				sendmsg_status = sms_err_fail;
				goto end;
				break;
			default:
				break;
			}
		}
		sendmsg_status	= bSendOK ? sms_err_no_ok_fail : sms_err_send;
		break;

	case sml_crucial:
		//必须接收到MB返回的 commu_status_succeed 才退出
		start_time		= ::GetTickCount();
		while(1)
		{
			pre_send_time = ::GetTickCount();
			while (1)
			{//在重发时间内
				if (s_msg_len <= 1 
					|| snd_msg[pt_data_add_dst] == addr_servo_x
					|| snd_msg[pt_data_add_dst] == addr_servo_y
					|| snd_msg[pt_data_add_dst] == addr_ink
					|| snd_msg[pt_data_add_dst] == addr_rec_cloth
					|| snd_msg[pt_data_add_dst] == addr_sec_cloth
					|| snd_msg[pt_data_add_dst] == addr_correct
					|| snd_msg[pt_data_add_dst] == addr_com
					|| snd_msg[pt_data_add_dst] == addr_servo_three_axis
					)
				{//单字节指令发送成功即返回
					if (bSendOK)
					{
						sendmsg_status = sms_handshake_ok;
						goto end;
					}
				}
				else
				{
					switch(m_bcommu_status)
					{
					case commu_status_succeed:
						//MB 返回通讯成功信号
						sendmsg_status = sms_handshake_ok;
						goto end;
						break;
					case commu_status_failed:
						//MB 返回通讯失败信号
						sendmsg_status = sms_err_fail;
						goto end;
						break;
					default:
						break;
					}
				}
				if((DWORD)(pre_send_time + time_repeatsend) < ::GetTickCount())
				{
					//time_repeatsend
					break;
				}	
				//是否中断当前操作指令串发送状态检测过程
				if (m_sendMsg_handshake_bBreak)
				{
					break;
				}
				Sleep(1);
			}
			if ((DWORD)(start_time + timeout) < ::GetTickCount())
			{
				//time out
				break;
			}
			//是否中断当前操作指令串发送状态检测过程
			if (m_sendMsg_handshake_bBreak)
			{
				break;
			}
			//重发指令串
			snd_msg[pt_data_cmdcount]++;

			BYTE bCRC = 0x00;
			DWORD i;
			for (i=0; i<s_msg_len-1; i++)
			{
				bCRC += snd_msg[i];
			}
			snd_msg[s_msg_len-1] = bCRC;

			bSendOK |= Commu_SendMsg(snd_msg, s_msg_len);
		}
		if (s_msg_len <= 1 
			|| snd_msg[pt_data_add_dst] == addr_servo_x
			|| snd_msg[pt_data_add_dst] == addr_servo_y
			|| snd_msg[pt_data_add_dst] == addr_ink
			|| snd_msg[pt_data_add_dst] == addr_rec_cloth
			|| snd_msg[pt_data_add_dst] == addr_sec_cloth
			|| snd_msg[pt_data_add_dst] == addr_correct
			|| snd_msg[pt_data_add_dst] == addr_com
			|| snd_msg[pt_data_add_dst] == addr_servo_three_axis
			)
		{//单字节指令发送成功即返回
			if (bSendOK)
			{
				sendmsg_status = sms_handshake_ok;
				goto end;
			}
		}
		else
		{
			switch(m_bcommu_status)
			{
			case commu_status_succeed:
				//MB 返回通讯成功信号
				sendmsg_status = sms_handshake_ok;
				goto end;
				break;
			case commu_status_failed:
				//MB 返回通讯失败信号
				sendmsg_status = sms_err_fail;
				goto end;
				break;
			default:
				break;
			}
		}
		sendmsg_status	= bSendOK ? sms_err_no_ok_fail : sms_err_send;
		break;
	}
end:
	return sendmsg_status;
}

//操作指令串发送
//snd_msg, s_msg_len	:[in]发送的指令串及长度
//timeout				:[in]发送指令的总延时值 ms
//time_repeatsend		:[in]当发送失败时，重发指令口串的时间间隔（累积值在 timeout范围内） ms
//pFlag					:[in/out]指令发送状态标识（接收到相应信息时被自动赋值），当为NULL时，使用 sml_normal 模式发送
//pFlagCheck,nCheckCount:[in]指定函数退出条件，除 TimeOut外，仅当  *pFlag==pFlagCheck[i] 时才返回
//time_check			:[in]函数退出条件(pFlagCheck[i])判断的超时设置，ms
int CComBase::SendMsg_handshake(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check)
{
	if (pFlag==NULL || pFlagCheck==NULL || nCheckCount<=0)
	{//未指定退出条件，使用默认方式
		return CComBase::SendMsg_handshake(snd_msg, s_msg_len, timeout, time_repeatsend, rpComBase::sml_normal);
	}

	//指令串必须发送成功，sml_crucial
	DWORD	start_time;
	int		sendmsg_status = sms_err_send;
	int		i;

	//必须接收到MB返回的 commu_status_succeed 才退出
	sendmsg_status = CComBase::SendMsg_handshake(snd_msg, s_msg_len, timeout, time_repeatsend, rpComBase::sml_crucial);

	if (m_bDemoMode)
	{
		Sleep(1);
		return sms_handshake_ok;
	}

	switch(sendmsg_status)
	{
	case sms_err_send:
	case sms_err_fail:
	case sms_err_no_ok_fail:
	default:
		return sendmsg_status;
		break;
	case sms_handshake_ok:
		break;
	}
	
	start_time		= ::GetTickCount();
	while(1)
	{
		for (i=0; i<nCheckCount; i++)
		{
			if (*pFlag == pFlagCheck[i].bFlag)
			{//指令成功发送，并且MB端已经处理
				return pFlagCheck[i].nResult;
			}
		}
		if ((DWORD)(start_time + time_check) < ::GetTickCount())
		{
			//time out
			break;
		}
		//是否中断当前操作指令串发送状态检测过程
		if (m_sendMsg_handshake_bBreak)
		{
			break;
		}
		Sleep(1);
	}
	for (i=0; i<nCheckCount; i++)
	{
		if (*pFlag == pFlagCheck[i].bFlag)
		{//指令成功发送，并且MB端已经处理
			return pFlagCheck[i].nResult;
		}
	}
	return sms_err_no_ok_fail;
}

int CComBase::SendMsg_handshake0(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check, BOOL bFlag)
{
	if (pFlag==NULL || pFlagCheck==NULL || nCheckCount<=0)
	{//未指定退出条件，使用默认方式
		return CComBase::SendMsg_handshake0(snd_msg, s_msg_len, timeout, time_repeatsend, rpComBase::sml_normal, bFlag);
	}

	//指令串必须发送成功，sml_crucial
	DWORD	start_time;
	int		sendmsg_status = sms_err_send;
	int		i;

	//必须接收到MB返回的 commu_status_succeed 才退出
	sendmsg_status = CComBase::SendMsg_handshake0(snd_msg, s_msg_len, timeout, time_repeatsend, rpComBase::sml_crucial, bFlag);

	if (m_bDemoMode)
	{
		Sleep(1);
		return sms_handshake_ok;
	}

	switch(sendmsg_status)
	{
	case sms_err_send:
	case sms_err_fail:
	case sms_err_no_ok_fail:
	default:
		return sendmsg_status;
		break;
	case sms_handshake_ok:
		break;
	}

	start_time		= ::GetTickCount();
	while(1)
	{
		for (i=0; i<nCheckCount; i++)
		{
			if (*pFlag == pFlagCheck[i].bFlag)
			{//指令成功发送，并且MB端已经处理
				return pFlagCheck[i].nResult;
			}
		}
		if ((DWORD)(start_time + time_check) < ::GetTickCount())
		{
			//time out
			break;
		}
		//是否中断当前操作指令串发送状态检测过程
		if (m_sendMsg_handshake_bBreak)
		{
			break;
		}
		Sleep(1);
	}
	for (i=0; i<nCheckCount; i++)
	{
		if (*pFlag == pFlagCheck[i].bFlag)
		{//指令成功发送，并且MB端已经处理
			return pFlagCheck[i].nResult;
		}
	}
	return sms_err_no_ok_fail;
}

void CComBase::ReceiveData(const void* pBuf, DWORD InBufferCount)
{
	DWORD	dwLen;
	BYTE	*pInBuffer = (BYTE*)pBuf;

	if (pInBuffer==NULL || InBufferCount <= 0)
		return;

	if (m_pBuffer==NULL || m_dwBufferSize <= 0)
	{//缓冲区未定义
		m_dwBufferSize = 0;
		m_dwReceiveSize = 0;
	}

	dwLen = m_dwReceiveSize + InBufferCount;
	if (dwLen > m_dwBufferSize)
	{//缓冲区不够大
		if (m_dwGrowSize > 0)
		{
			dwLen -= m_dwBufferSize;			//缓冲区增加大小
			if (dwLen < m_dwGrowSize)
				dwLen = m_dwGrowSize;
			dwLen += m_dwBufferSize;
			ReAllocBuffer(dwLen, TRUE);
		}
	}
	if (m_pBuffer==NULL || m_dwBufferSize <= 0)
	{//接收缓冲区为空，无法处理相应数据
		ASSERT(0);
		return;
	}

	TRACE0(">> CComBase::ReceiveData ==\n");
	SyncOpt_Data_Enter();
	
	//接收到的数据个数（未处理）
	while(InBufferCount > 0)
	{
		dwLen = m_dwReceiveSize + InBufferCount;
		if (dwLen <= m_dwBufferSize)
		{//当前接收到的数据可全部存放入缓冲区中
			dwLen = InBufferCount;
		}
		else
		{//缓冲区不够大，循环处理
			dwLen = m_dwBufferSize - m_dwReceiveSize;
		}

		//dwLen :当前填入缓冲区的数据个数
		::memcpy(m_pBuffer+m_dwReceiveSize, pInBuffer, dwLen);
		m_dwReceiveSize		+= dwLen;
		InBufferCount	-= dwLen;
		pInBuffer		+= dwLen;

		//自定义处理接收缓冲区内的数据
		dwLen = ReceiveDataEx();
		
		//dwLen :当前被正常处理的数据个数
		if (dwLen <= 0)
		{//当前的指令串不完整，等待下一次接收的数据
			if (m_dwReceiveSize == m_dwBufferSize)
			{//缓冲区已满，将缓冲区数据作无效数据处理
				ASSERT(0);
				memset(m_pBuffer, 0, m_dwBufferSize);
				m_dwReceiveSize = 0;
			}
		}
		else if (dwLen >= m_dwReceiveSize)
		{//所有数据均是有效数据（已被处理）
			memset(m_pBuffer, 0, m_dwBufferSize);
			m_dwReceiveSize = 0;
		}
		else
		{//数据数据被处理，需保留到下一次处理过程中
			ASSERT(dwLen < m_dwReceiveSize);

			memcpy(m_pBuffer, m_pBuffer+dwLen, m_dwReceiveSize-dwLen);
			m_dwReceiveSize = m_dwReceiveSize - dwLen;
		}
	}
	SyncOpt_Data_Leave();
	TRACE0("<< CComBase::ReceiveData ==\n");

	ASSERT(InBufferCount<=0);
}

//对接收到的数据进行分析
//操作对象			： m_pBuffer, m_dwReceiveSize
//返回值：已处理的数据个数（<= m_dwReceiveSize），剩余部分留到下一次数据接收后处理
//			<= 0时表示未处理（当前数据帧未完全接收），
//			当当前数据均为无效数据时，也返回 m_dwReceiveSize
//备注：在实际应用时，此函数一般需重载后使用，
//	默认情况下是将当前缓冲m_pBuffer内容作为一次有效数据帧处理，返回值为 m_dwReceiveSize
//	在本函数中必须处理 m_bcommu_status 的信息，否则将影响 SendMsg_handshake(...)对指令有效性的判断
DWORD CComBase::ReceiveDataEx()
{
	DWORD dwResult = m_dwReceiveSize;
	
	ASSERT(m_pBuffer != NULL);
	ASSERT(m_dwReceiveSize <= m_dwBufferSize);

	DWORD	i;
	BYTE	bReturnFlag = 0;
	for(i=0; i<m_dwReceiveSize; i++)
	{
		bReturnFlag = m_pBuffer[i];
	}

	dwResult = i;
	return dwResult;
}


void CComBase::SyncOpt_Data_Enter()
{
	EnterCriticalSection(&m_csComBaseSync);

	TRACE3("\tSyncOpt_Data_Enter()_%d2,%2d, %.8X\n",
		m_csComBaseSync.LockCount, 
		m_csComBaseSync.RecursionCount,
		m_csComBaseSync.OwningThread);
}
void CComBase::SyncOpt_Data_Leave()
{
	TRACE3("\tSyncOpt_Data_Leave()_%d2,%2d, %.8X\n",
		m_csComBaseSync.LockCount, 
		m_csComBaseSync.RecursionCount,
		m_csComBaseSync.OwningThread);

	LeaveCriticalSection(&m_csComBaseSync);
}

void CComBase::SyncOpt_Send_Enter()
{
	EnterCriticalSection(&m_csSendSync);
	TRACE3("\tSyncOpt_Send_Enter()_%d2,%2d, %.8X\n",
		m_csSendSync.LockCount, 
		m_csSendSync.RecursionCount,
		m_csSendSync.OwningThread);
}
void CComBase::SyncOpt_Send_Leave()
{
	TRACE3("\tSyncOpt_Send_Leave()_%d2,%2d, %.8X\n",
		m_csSendSync.LockCount, 
		m_csSendSync.RecursionCount,
		m_csSendSync.OwningThread);
	LeaveCriticalSection(&m_csSendSync);
}

//中断当前操作指令串发送状态检测过程
void CComBase::SendMsg_handshake_Break(BOOL bBreak)
{
	m_sendMsg_handshake_bBreak = bBreak;
}

#define FILE_MAX_LIMIT		(64*1024)
BOOL CComBase::Write_Communication_Info(const char *pszFileName, const BYTE *snd_msg, int msg_len, DWORD sizeLimit, DWORD defaultSize, BOOL bTimeFlag)
{
	CString strIniFile;
	CFile	file;

	strIniFile = pszFileName;
	if (strIniFile.IsEmpty())
		return TRUE;
	if (sizeLimit < 1024)
		sizeLimit = FILE_MAX_LIMIT;
	if (defaultSize < (sizeLimit/2))
		defaultSize = sizeLimit / 2;

	if (!file.Open(strIniFile, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate, NULL))
		return FALSE;

	int		i, nInfoLen;
	LPSTR	pszInfo = NULL;
	CString strTmp, strInfo;
	ULONGLONG	uLength;
	CString		strTickCount;
	DWORD		dwTickcount;

	strInfo = "";
	for (i=0; i<msg_len; i++)
	{
		strTmp.Format("%.2X ", snd_msg[i]);
		strInfo += strTmp;
	}
	dwTickcount = GetTickCount();
	if (bTimeFlag)
	{
		CTime		tm;
		tm = CTime::GetCurrentTime();
		strTmp = strInfo;
		strTickCount.Format(_T("_%.8X"), dwTickcount);
		strInfo = tm.Format(_T("%Y-%m-%d-%H%M%S"));
		strInfo += strTickCount;
		strInfo += " = ";
		strInfo += strTmp;
	}
	else
	{
		strTmp = strInfo;
		strTickCount.Format(_T("%.8X="), dwTickcount);
		strInfo = strTickCount;
		strInfo += strTmp;
	}
	strInfo += "\r\n";

	nInfoLen	= strInfo.GetLength();
	pszInfo		= strInfo.LockBuffer();
	uLength	= file.GetLength();
	if (uLength > sizeLimit)
	{
		BYTE	*pBuf;
		DWORD	dwLimit;
		int		nFirst;

		dwLimit	= defaultSize;
		pBuf	= new BYTE[dwLimit+nInfoLen];
		if (pBuf != NULL)
		{
			file.Seek((uLength-dwLimit), CFile::begin);
			dwLimit = file.Read(pBuf, dwLimit);
			nFirst = 0;
			memcpy(pBuf + dwLimit, pszInfo, nInfoLen);
			for (i=0; i<(int)dwLimit; i++)
			{
				if (pBuf[i]=='\n' || pBuf[i]=='\r')
				{
					nFirst = i+1;
					if (i < (int)(dwLimit-1))
					{
						while (pBuf[nFirst]=='\n' || pBuf[nFirst]=='\r')
							nFirst ++;
					}
					dwLimit -= nFirst;
					break;
				}
			}
			dwLimit += nInfoLen;
			file.SetLength(dwLimit);
			file.SeekToBegin();
			file.Write(pBuf+nFirst, dwLimit);
			nInfoLen = 0;
			delete []pBuf;
			pBuf = NULL;
		}
	}
	if (nInfoLen > 0)
	{
		file.SeekToEnd();
		file.Write(pszInfo, nInfoLen);
		file.Flush();
	}
	strInfo.UnlockBuffer();
	file.Close();

	return TRUE;
}

}		//namespace rpComBase
