#include "../stdafx.h"
#include "rpSocketBase.h"
#include "SocketPort_Protocol.h"
#include "CBSocketPort.h"
#include "../MBComPort/MBComPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//��ӡ���
class CSocketDataThread : public ABaseThread
{
public:
	/*1.������ϵ������ */
	CSocketDataThread();
	virtual ~CSocketDataThread();

protected:
	/*5.ִ���߳� */
	void Execute(void);						//�߳�ִ��

	friend class CCommunicationPort;
};

//CSocketDataThread
CSocketDataThread::CSocketDataThread()
{
}

CSocketDataThread::~CSocketDataThread()
{
}

// CSocketDataThread ��Ա����
void CSocketDataThread::Execute()
{
	static bool bPause = false;

	CCommunicationPort* pPort = (CCommunicationPort*)m_pThreadData;
	ASSERT(pPort != NULL);
	bPause = false;
	while (1)
	{
		if (m_IsNeedExit)
			break;
	}
}


//���������߳�
bool CCommunicationPort::socket_thread_create()
{
	CSocketDataThread*	pThread;

	socket_thread_close();
	if (m_socket_pThread == NULL)
		m_socket_pThread = (void*)new CSocketDataThread;
	if (m_socket_pThread == NULL)
		goto failed;

	pThread = (CSocketDataThread*)m_socket_pThread;
	ASSERT(pThread != NULL);
	if (!pThread->Create(this))
	{
		goto failed;
	}

	ASSERT(socket_thread_isready());

	return socket_thread_isready();

failed:
	socket_thread_close();
	return false;
}
//�رմ����߳�
bool CCommunicationPort::socket_thread_close()
{
	socket_thread_wait_for_terminated(5000);
	return true;
}
//�ͷŴ����߳�
bool CCommunicationPort::socket_thread_delete()
{
	CSocketDataThread* pThread;

	socket_thread_close();

	pThread = (CSocketDataThread*)m_socket_pThread;
	if (pThread != NULL)
	{
		delete pThread;
	}
	m_socket_pThread = NULL;

	return true;
}

//�߳��Ƿ��Ѿ������ɹ������������д�ӡ��ش���
bool CCommunicationPort::socket_thread_isready() const
{
	CSocketDataThread* pThread;

	pThread = (CSocketDataThread*)m_socket_pThread;
	if (pThread == NULL)
	{
		return false;
	}
	if (!pThread->IsCreated())
	{
		return false;
	}

	return true;
}
//�߳��Ƿ��Ѿ�����������ת�У�������ӡ�����Ѿ�������
bool CCommunicationPort::socket_thread_isrunning() const
{
	CSocketDataThread* pThread;

	pThread = (CSocketDataThread*)m_socket_pThread;
	if (pThread == NULL)
		return false;

	if (pThread->IsTerminated())
		return false;

	//ֻҪ��һ���̻߳�������������״̬��������TRUE
	return TRUE;
}
//�ȴ��� Write Thread ��������̹߳ر�
//	dwTimeOut	:[in]��ʱ������ms, Ϊ0ʱ��ʾ����ʱ�ȴ�
//ע��һ������ Write Thread �˳�ǰ����ʱ����
void CCommunicationPort::socket_thread_wait_for_terminated(DWORD dwTimeOut)
{
	DWORD	dwStart;
	CSocketDataThread* pThread;

	pThread = (CSocketDataThread*)m_socket_pThread;
	if (pThread == NULL)
		return;

	if (pThread != NULL)
	{
		pThread->NeedExit(true);
		pThread->Resume();
	}
	Sleep(100);
	//�ȴ��̴߳������
	dwStart = GetTickCount();
	while (1)
	{
		if (pThread->IsTerminated())
			return;

		if (dwTimeOut > 0 && (::GetTickCount() - dwStart) > dwTimeOut)
			break;		//��ʱ��ǿ�ƽ����߳�
		Sleep(1);
	}

	//ǿ�ƽ����̴߳������
	if (pThread != NULL)
	{
		pThread->Terminate(2000);
	}
	return;
}

//ǿ�Ƴ�ʼ������
void CCommunicationPort::socket_init(BOOL bReAllocBuffer)
{
	socket_Close(bReAllocBuffer);
}
//	pszIP, uPort:[in]�˶����ƿ�IP��ַ�������˿�
//	nBufferSize :[in]��ʼ�����ջ�������С
//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
//	dwHeartBeatTime	:[in]��������źŷ��ͼ����ms��Ϊ0ʱ���Զ����ͼ���ź�
//	pErrorInfo	:[out]������Ϣ����������ֵΪFALSEʱ��Ч
BOOL CCommunicationPort::socket_Open(const char *pszIP, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime, CString *pErrorInfo)
{
	CSocketDataThread *pThread = NULL;
	CString strErrorInfo, strErr;

	if (pErrorInfo == NULL)
		pErrorInfo = &strErrorInfo;

	*pErrorInfo = "";

	Close(TRUE);

	pThread = new CSocketDataThread;
	if (pThread == NULL)
	{
		*pErrorInfo = "Out of memory!";
		return FALSE;
	}
	if (!pThread->Create(this))
	{
		delete pThread;
		pThread = NULL;

		*pErrorInfo = "Failed to create socket data thread!";
		return FALSE;
	}
	m_socket_pThread = pThread;
	if (!m_socket_Data.Open(pszIP, tcp_default_port_data, nBufferSize, nGrowSize, dwHeartBeatTime))
	{
		strErr = "";
		m_socket_Data.GetLastErrorInfo(strErr);
		pErrorInfo->Format("Failed to open data link port! \r\t%s", strErr);
		return FALSE;
	}
	if (!m_socket_Control.Open(pszIP, tcp_default_port_control, nBufferSize, nGrowSize, dwHeartBeatTime))
	{
		strErr = "";
		m_socket_Control.GetLastErrorInfo(strErr);
		pErrorInfo->Format("Failed to open control link port! \r\t%s", strErr);
		return FALSE;
	}
	m_socket_Control.PC_Repeater_Config_SetProc(CCommunicationPort::socket_repeater_config_proc, this);

	m_socket_bEnable = TRUE;

	return TRUE;
}
BOOL CCommunicationPort::socket_IsEnable() const
{
	return m_socket_bEnable;
}
BOOL CCommunicationPort::socket_IsOpen() const
{
	if (!socket_IsEnable())
		return FALSE;
	if (!m_socket_Control.IsOpen())
		return FALSE;
	if (!m_socket_Data.IsOpen())
		return FALSE;
	if (!socket_thread_isready())
		return FALSE;
	return TRUE;
}
void CCommunicationPort::socket_Close(BOOL bReleaseBuffer)
{
	socket_thread_delete();
	m_socket_Control.Close(bReleaseBuffer);
	m_socket_Data.Close(bReleaseBuffer);
	m_socket_bEnable = FALSE;
}
//���Ͳ���ָ��
int CCommunicationPort::socket_senddata(const void* snd_msg, int msg_len)
{
	if (!socket_IsEnable())
		return 0;

	if (m_socket_Data.IsOpen())
		return m_socket_Data.Commu_SendMsg(snd_msg, msg_len);

	return 0;
}
//�м�����֡����
DWORD CCommunicationPort::socket_repeater_config_proc(void *pData, const void *pDataList, int dataSize)
{
	CCommunicationPort *pPort = (CCommunicationPort*)pData;
	DWORD dwResult = 0;

	dwResult = pPort->socket_AddReceiveData(pDataList, dataSize);

	return dwResult;
}
//�м�����֡����-���������Ϣ
DWORD CCommunicationPort::socket_AddReceiveData(const void* pBuf, DWORD InBufferCount)
{
	if (!socket_IsEnable())
		return 0;

	if (!m_socket_Control.IsOpen())
		return 0;

	ReceiveData(pBuf, InBufferCount);

	return InBufferCount;
}