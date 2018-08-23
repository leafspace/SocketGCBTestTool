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
	m_bDemoMode			= FALSE;	//ģ��ģʽ����ʹ��ʵ�ʵĴ���ͨѶ

	m_dwGrowSize		= 4*1024;		//���ջ��������ݲ���
	m_dwInitBufferSize	= 4*1024;		//��ʼ�����ջ�������С

	//�������ݻ�����
	m_dwBufferSize		= 0;
	m_pBuffer			= NULL;
	m_dwReceiveSize		= 0;

	m_pComPort			= new zmComPort::CComPort;			//���ڲ�����ָ��
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

//	bCopy :[in]�������뻺����ʱ�Ƿ�Ҫ����ԭ������
BOOL CComBase::ReAllocBuffer(DWORD dwBufferSize, BOOL bCopy)
{
	BYTE	*pBuffer = NULL;
	int		nRecSize = m_dwReceiveSize;

	if (m_pBuffer!=NULL && m_dwBufferSize >= dwBufferSize)
	{//Ԥ���建�����Ѿ�����
		return TRUE;
	}

	//�����µĻ�����
	pBuffer = (LPBYTE)AGV::AVirtualAlloc(NULL, dwBufferSize, MEM_COMMIT,PAGE_READWRITE);
	if (pBuffer==NULL)
	{//�ڴ����벻�ɹ�����ʹ��ԭ������
		return FALSE;
	}

	TRACE0(">> CComBase::ReAllocBuffer ==\n");
	SyncOpt_Data_Enter();

	memset(pBuffer, 0, dwBufferSize);
	m_dwReceiveSize = 0;
	if (m_pBuffer)
	{//�ͷ�ԭ���Ļ������ռ�
		if (bCopy)
		{//����ԭ������
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

//��ʼ����ز���
void CComBase::Init(BOOL bReAllocBuffer)
{
	SendMsg_handshake_Break(FALSE);			//�Ƿ��жϵ�ǰ����ָ�����״̬������

	if (m_pComPort==NULL)
		m_pComPort			= new zmComPort::CComPort;			//���ڲ�����ָ��
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
	
	//����ָ���״̬��Ϣ
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
//����ָ���״̬��Ϣ
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
	SendMsg_handshake_Break(FALSE);			//�Ƿ��жϵ�ǰ����ָ�����״̬������
	
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
//	nBufferSize :[in]��ʼ�����ջ�������С
//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
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
		m_pComPort			= new zmComPort::CComPort;			//���ڲ�����ָ��
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
//ֱ�ӷ��Ͳ���ָ�һ����� ReceiveData ������ʹ��
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

//����ָ�����
//�������壺
//	snd_msg, s_msg_len	:[in]���͵�ָ�������
//	timeout				:[in]����ָ�������ʱֵ ms
//	time_repeatsend		:[in]������ʧ��ʱ���ط�ָ��ڴ���ʱ�������ۻ�ֵ�� timeout��Χ�ڣ� ms
//	sendmsg_level		:[in] SENDMSG_LEVEL
//����ֵ��SENDMSG_STATUS
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
			{//���ط�ʱ����
				if (s_msg_len <= 1)
				{//���ֽ�ָ��ͳɹ�������
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
						//MB ����ͨѶ�ɹ��ź�
						sendmsg_status = sms_handshake_ok;
						goto end;
						break;
					case commu_status_failed:
						//MB ����ͨѶʧ���ź�
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
				//�Ƿ��жϵ�ǰ����ָ�����״̬������
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
			//�ط�ָ�
			bSendOK |= Commu_SendMsg(snd_msg, s_msg_len);
		}
		if (s_msg_len <= 1)
		{//���ֽ�ָ��ͳɹ�������
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
				//MB ����ͨѶ�ɹ��ź�
				sendmsg_status = sms_handshake_ok;
				goto end;
				break;
			case commu_status_failed:
				//MB ����ͨѶʧ���ź�
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
		//������յ�MB���ص� commu_status_succeed ���˳�
		start_time		= ::GetTickCount();
		while(1)
		{
			pre_send_time = ::GetTickCount();
			while (1)
			{//���ط�ʱ����
				if (s_msg_len <= 1 
					|| snd_msg[pt_data_cmd] == cmd_com_motor_move_relative
					|| snd_msg[pt_data_cmd] == cmd_com_get_board_condition)
				{//���ֽ�ָ��ͳɹ�������
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
						//MB ����ͨѶ�ɹ��ź�
						sendmsg_status = sms_handshake_ok;
						goto end;
						break;
					case commu_status_failed:
						//MB ����ͨѶʧ���ź�
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
				//�Ƿ��жϵ�ǰ����ָ�����״̬������
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
			//�Ƿ��жϵ�ǰ����ָ�����״̬������
			if (m_sendMsg_handshake_bBreak)
			{
				break;
			}
			//�ط�ָ�
			snd_msg[pt_data_cmdcount]++;
			bSendOK |= Commu_SendMsg(snd_msg, s_msg_len);
		}
		if (s_msg_len <= 1 
			|| snd_msg[pt_data_cmd] == cmd_com_motor_move_relative
			|| snd_msg[pt_data_cmd] == cmd_com_get_board_condition)
		{//���ֽ�ָ��ͳɹ�������
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
				//MB ����ͨѶ�ɹ��ź�
				sendmsg_status = sms_handshake_ok;
				goto end;
				break;
			case commu_status_failed:
				//MB ����ͨѶʧ���ź�
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
			{//���ط�ʱ����
				if (s_msg_len <= 1)
				{//���ֽ�ָ��ͳɹ�������
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
						//MB ����ͨѶ�ɹ��ź�
						sendmsg_status = sms_handshake_ok;
						goto end;
						break;
					case commu_status_failed:
						//MB ����ͨѶʧ���ź�
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
				//�Ƿ��жϵ�ǰ����ָ�����״̬������
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
			//�ط�ָ�
			bSendOK |= Commu_SendMsg(snd_msg, s_msg_len);
		}
		if (s_msg_len <= 1)
		{//���ֽ�ָ��ͳɹ�������
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
				//MB ����ͨѶ�ɹ��ź�
				sendmsg_status = sms_handshake_ok;
				goto end;
				break;
			case commu_status_failed:
				//MB ����ͨѶʧ���ź�
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
		//������յ�MB���ص� commu_status_succeed ���˳�
		start_time		= ::GetTickCount();
		while(1)
		{
			pre_send_time = ::GetTickCount();
			while (1)
			{//���ط�ʱ����
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
				{//���ֽ�ָ��ͳɹ�������
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
						//MB ����ͨѶ�ɹ��ź�
						sendmsg_status = sms_handshake_ok;
						goto end;
						break;
					case commu_status_failed:
						//MB ����ͨѶʧ���ź�
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
				//�Ƿ��жϵ�ǰ����ָ�����״̬������
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
			//�Ƿ��жϵ�ǰ����ָ�����״̬������
			if (m_sendMsg_handshake_bBreak)
			{
				break;
			}
			//�ط�ָ�
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
		{//���ֽ�ָ��ͳɹ�������
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
				//MB ����ͨѶ�ɹ��ź�
				sendmsg_status = sms_handshake_ok;
				goto end;
				break;
			case commu_status_failed:
				//MB ����ͨѶʧ���ź�
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

//����ָ�����
//snd_msg, s_msg_len	:[in]���͵�ָ�������
//timeout				:[in]����ָ�������ʱֵ ms
//time_repeatsend		:[in]������ʧ��ʱ���ط�ָ��ڴ���ʱ�������ۻ�ֵ�� timeout��Χ�ڣ� ms
//pFlag					:[in/out]ָ���״̬��ʶ�����յ���Ӧ��Ϣʱ���Զ���ֵ������ΪNULLʱ��ʹ�� sml_normal ģʽ����
//pFlagCheck,nCheckCount:[in]ָ�������˳��������� TimeOut�⣬����  *pFlag==pFlagCheck[i] ʱ�ŷ���
//time_check			:[in]�����˳�����(pFlagCheck[i])�жϵĳ�ʱ���ã�ms
int CComBase::SendMsg_handshake(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check)
{
	if (pFlag==NULL || pFlagCheck==NULL || nCheckCount<=0)
	{//δָ���˳�������ʹ��Ĭ�Ϸ�ʽ
		return CComBase::SendMsg_handshake(snd_msg, s_msg_len, timeout, time_repeatsend, rpComBase::sml_normal);
	}

	//ָ����뷢�ͳɹ���sml_crucial
	DWORD	start_time;
	int		sendmsg_status = sms_err_send;
	int		i;

	//������յ�MB���ص� commu_status_succeed ���˳�
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
			{//ָ��ɹ����ͣ�����MB���Ѿ�����
				return pFlagCheck[i].nResult;
			}
		}
		if ((DWORD)(start_time + time_check) < ::GetTickCount())
		{
			//time out
			break;
		}
		//�Ƿ��жϵ�ǰ����ָ�����״̬������
		if (m_sendMsg_handshake_bBreak)
		{
			break;
		}
		Sleep(1);
	}
	for (i=0; i<nCheckCount; i++)
	{
		if (*pFlag == pFlagCheck[i].bFlag)
		{//ָ��ɹ����ͣ�����MB���Ѿ�����
			return pFlagCheck[i].nResult;
		}
	}
	return sms_err_no_ok_fail;
}

int CComBase::SendMsg_handshake0(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check, BOOL bFlag)
{
	if (pFlag==NULL || pFlagCheck==NULL || nCheckCount<=0)
	{//δָ���˳�������ʹ��Ĭ�Ϸ�ʽ
		return CComBase::SendMsg_handshake0(snd_msg, s_msg_len, timeout, time_repeatsend, rpComBase::sml_normal, bFlag);
	}

	//ָ����뷢�ͳɹ���sml_crucial
	DWORD	start_time;
	int		sendmsg_status = sms_err_send;
	int		i;

	//������յ�MB���ص� commu_status_succeed ���˳�
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
			{//ָ��ɹ����ͣ�����MB���Ѿ�����
				return pFlagCheck[i].nResult;
			}
		}
		if ((DWORD)(start_time + time_check) < ::GetTickCount())
		{
			//time out
			break;
		}
		//�Ƿ��жϵ�ǰ����ָ�����״̬������
		if (m_sendMsg_handshake_bBreak)
		{
			break;
		}
		Sleep(1);
	}
	for (i=0; i<nCheckCount; i++)
	{
		if (*pFlag == pFlagCheck[i].bFlag)
		{//ָ��ɹ����ͣ�����MB���Ѿ�����
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
	{//������δ����
		m_dwBufferSize = 0;
		m_dwReceiveSize = 0;
	}

	dwLen = m_dwReceiveSize + InBufferCount;
	if (dwLen > m_dwBufferSize)
	{//������������
		if (m_dwGrowSize > 0)
		{
			dwLen -= m_dwBufferSize;			//���������Ӵ�С
			if (dwLen < m_dwGrowSize)
				dwLen = m_dwGrowSize;
			dwLen += m_dwBufferSize;
			ReAllocBuffer(dwLen, TRUE);
		}
	}
	if (m_pBuffer==NULL || m_dwBufferSize <= 0)
	{//���ջ�����Ϊ�գ��޷�������Ӧ����
		ASSERT(0);
		return;
	}

	TRACE0(">> CComBase::ReceiveData ==\n");
	SyncOpt_Data_Enter();
	
	//���յ������ݸ�����δ����
	while(InBufferCount > 0)
	{
		dwLen = m_dwReceiveSize + InBufferCount;
		if (dwLen <= m_dwBufferSize)
		{//��ǰ���յ������ݿ�ȫ������뻺������
			dwLen = InBufferCount;
		}
		else
		{//������������ѭ������
			dwLen = m_dwBufferSize - m_dwReceiveSize;
		}

		//dwLen :��ǰ���뻺���������ݸ���
		::memcpy(m_pBuffer+m_dwReceiveSize, pInBuffer, dwLen);
		m_dwReceiveSize		+= dwLen;
		InBufferCount	-= dwLen;
		pInBuffer		+= dwLen;

		//�Զ��崦����ջ������ڵ�����
		dwLen = ReceiveDataEx();
		
		//dwLen :��ǰ��������������ݸ���
		if (dwLen <= 0)
		{//��ǰ��ָ����������ȴ���һ�ν��յ�����
			if (m_dwReceiveSize == m_dwBufferSize)
			{//����������������������������Ч���ݴ���
				ASSERT(0);
				memset(m_pBuffer, 0, m_dwBufferSize);
				m_dwReceiveSize = 0;
			}
		}
		else if (dwLen >= m_dwReceiveSize)
		{//�������ݾ�����Ч���ݣ��ѱ�����
			memset(m_pBuffer, 0, m_dwBufferSize);
			m_dwReceiveSize = 0;
		}
		else
		{//�������ݱ������豣������һ�δ��������
			ASSERT(dwLen < m_dwReceiveSize);

			memcpy(m_pBuffer, m_pBuffer+dwLen, m_dwReceiveSize-dwLen);
			m_dwReceiveSize = m_dwReceiveSize - dwLen;
		}
	}
	SyncOpt_Data_Leave();
	TRACE0("<< CComBase::ReceiveData ==\n");

	ASSERT(InBufferCount<=0);
}

//�Խ��յ������ݽ��з���
//��������			�� m_pBuffer, m_dwReceiveSize
//����ֵ���Ѵ�������ݸ�����<= m_dwReceiveSize����ʣ�ಿ��������һ�����ݽ��պ���
//			<= 0ʱ��ʾδ������ǰ����֡δ��ȫ���գ���
//			����ǰ���ݾ�Ϊ��Ч����ʱ��Ҳ���� m_dwReceiveSize
//��ע����ʵ��Ӧ��ʱ���˺���һ�������غ�ʹ�ã�
//	Ĭ��������ǽ���ǰ����m_pBuffer������Ϊһ����Ч����֡��������ֵΪ m_dwReceiveSize
//	�ڱ������б��봦�� m_bcommu_status ����Ϣ������Ӱ�� SendMsg_handshake(...)��ָ����Ч�Ե��ж�
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

//�жϵ�ǰ����ָ�����״̬������
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
