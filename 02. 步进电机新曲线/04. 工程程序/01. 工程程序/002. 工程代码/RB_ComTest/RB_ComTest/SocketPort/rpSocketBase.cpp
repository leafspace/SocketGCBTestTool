#include "../stdafx.h"
#include "rpSocketBase.h"
#include "SocketPort_Protocol.h"
#include "CBSocketPort.h"
#include "rpSocketBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//namespace rpSocketBase
//{
	//ABaseThread
	ABaseThread::ABaseThread()
	{
		m_pThreadData	= NULL;			//����ָ��

		m_hThread		= NULL;			//�߳̾��
		m_dwThreadId	= 0;			//�̺߳�

		m_IsTerminated	= TRUE;         //�Ƿ��ѽ����߳� 
		m_IsNeedExit	= FALSE;        //�Ƿ���Ҫ�����߳� 
	}

	ABaseThread::~ABaseThread()
	{
		Terminate(1000);
		//m_hThread = NULL;
	}

	//��ʼ���������Զ������
	//	�� Create(...)ʱ������
	bool ABaseThread::InitCustomParameter()
	{
		return true;
	}
	//�����������Զ������
	//	�� Terminate(...)ʱ������
	bool ABaseThread::ReleaseCustomParameter()
	{
		return true;
	}

	// ABaseThread ��Ա����
	DWORD WINAPI basethread_thread_func(LPVOID  lpParam) 
	{ 
		ABaseThread* pThread =(ABaseThread*)lpParam;
		ASSERT(pThread != NULL);

		pThread->m_IsTerminated = false;
		pThread->Execute(); 
		pThread->m_IsTerminated = true;

		return 0;
	}

	bool ABaseThread::Create(void* pThreadData)
	{
		ASSERT(pThreadData != NULL);

		m_pThreadData = pThreadData;
		if (!InitCustomParameter())
		{
			return false;
		}

		if (m_hThread != NULL)
		{//�Ѿ�������
			ASSERT(m_dwThreadId != 0);
			ReleaseCustomParameter();
			return true;
		}

		//Ĭ��Ҫ���� CREATE_SUSPENDED ״̬
		m_IsNeedExit	= false;
		m_IsTerminated	= false;
		m_dwThreadId	= 0;
		m_hThread = CreateThread( 
			NULL,							// no security attributes 
			0,								// use default stack size  
			basethread_thread_func,			// thread function 
			this,							// argument to thread function 
			CREATE_SUSPENDED,				// use default creation flags 
			&m_dwThreadId);					// returns the thread identifier 


		SetThreadPriority(THREAD_PRIORITY_NORMAL); 

		return m_hThread==NULL ? false : true;
	}

	//�ж��߳�
	bool ABaseThread::NeedExit(bool IsNeedExit)
	{
		m_IsNeedExit = IsNeedExit;

		return true;
	}
	bool ABaseThread::IsNeedExit() const
	{
		return m_IsNeedExit;
	}
	//�Ƿ��Ѿ������ɹ�
	bool ABaseThread::IsCreated() const
	{
		if (m_hThread==NULL)
		{
			return false;
		}
		ASSERT(m_dwThreadId != 0);
		return true;
	}

	bool ABaseThread::IsTerminated() const
	{
		return m_IsTerminated; 
	}
	bool ABaseThread::Terminate(DWORD dwTimeOut)
	{
		if (m_hThread == NULL) 
		{
			//char szMsg[80];
			//wsprintf( szMsg, "CreateThread failed." ); 
			//::MessageBox(NULL,szMsg,"ok",0);
		}
		else 
		{
			NeedExit(true);

			DWORD dwstart= GetTickCount();
			while (!IsTerminated())
			{
				if ((GetTickCount() - dwstart) > dwTimeOut)
					break;
				Sleep(1);
			}
			if (!IsTerminated())
			{
				TRACE0("TerminateThread__\n");
				::TerminateThread(m_hThread,0);
			}
			CloseHandle(m_hThread);
			m_hThread	= NULL;
			m_dwThreadId= 0;
		}
		ReleaseCustomParameter();

		return true;
	}

	bool ABaseThread::Resume()
	{
		if (m_hThread)
		{
			//m_IsTerminated= false;
			ResumeThread(m_hThread); 
		}
		return true;
	}
	//�����߳����ȼ�
	BOOL ABaseThread::SetThreadPriority(int nPriority)
	{
		if (m_hThread)
		{
			if (nPriority < THREAD_PRIORITY_LOWEST
				|| nPriority > THREAD_PRIORITY_HIGHEST)
			{
				nPriority = THREAD_PRIORITY_NORMAL;
			}
			return ::SetThreadPriority(m_hThread, nPriority);
		}
		return TRUE;
	}
	//����һ���̵߳���ѡ������
	DWORD ABaseThread::SetThreadIdealProcessor(DWORD dwIdealProcessor)
	{
		DWORD dwResult = (DWORD)(-1);
		if (m_hThread)
		{
			dwResult = ::SetThreadIdealProcessor(m_hThread, dwIdealProcessor);
			if (dwResult == (DWORD)(-1))
			{
				DWORD	dwErrCode;
				CString strErrInfo;

				dwErrCode = ::GetLastError();

				strErrInfo = CErrorInfo::TransSystemErrorCodeToString(dwErrCode);
				AnxStatusMsg(strErrInfo);
			}
		}
		else
		{
			SetLastError(ERROR_INVALID_HANDLE);
			dwResult = (DWORD)(-1);
		}
		return dwResult;
	}

//������⣬���ڼ�⵱ǰ���������Ƿ�����
#define HEARTBEAT_MSG		"\0"
#define HEARTBEAT_MSG_LEN	sizeof(HEARTBEAT_MSG)
/////////////////////////////////////////////////////////////////////

DWORD WINAPI SocketBase_RecvThread(LPVOID lpParam)
{
	CSocketBase *mySocket = (CSocketBase*)lpParam;
	ASSERT(mySocket != NULL);
	while (1)
	{
		CByteArray infoArray;
		int		nCount;

		infoArray.RemoveAll();
		nCount	= mySocket->ReceiveInfo(mySocket->m_serverSocket, infoArray);
		if (nCount <= 0)
		{//�˳���ǰ�߳�
			if (nCount != INVALID_SOCKET)
			{
				SOCKET		socketTmp = mySocket->m_serverSocket;

				mySocket->m_serverSocket = INVALID_SOCKET;
				shutdown(socketTmp, 2);
				closesocket(socketTmp);
			}
			break;
		}
		if (mySocket->m_sendMsg_handshake_bBreak)
			break;
		
		mySocket->SyncOpt_Data_Enter();
		mySocket->ReceiveData(infoArray.GetData(),infoArray.GetSize());
		mySocket->SyncOpt_Data_Leave();

		mySocket->m_dwLastReceiveTime = ::GetTickCount();

		if (mySocket->m_dwHeartBeatTime > 0
			&& (ULONGLONG)(mySocket->m_dwLastSendTime + mySocket->m_dwHeartBeatTime) < ::GetTickCount())
		{//���������źŷ��ͼ��
			mySocket->Commu_SendMsg(HEARTBEAT_MSG, HEARTBEAT_MSG_LEN);
		}
	}
	mySocket->m_sendMsg_handshake_bBreak = FALSE;
	mySocket->m_hThread = NULL;
	return 0;
} 

CSocketBase::CSocketBase()
{
	m_bDemoMode			= FALSE;	//ģ��ģʽ����ʹ��ʵ�ʵĴ���ͨѶ

	::memset(&m_serverSockAddr, 0, sizeof(m_serverSockAddr));
	m_serverSocket	= INVALID_SOCKET;
	m_strIP			= "127.0.0.1";
	m_nPort			= 10000;
	m_strErrorInfo	= "";

	m_hThread		= NULL;

	m_dwGrowSize		= 4*1024;		//���ջ��������ݲ���
	m_dwInitBufferSize	= 4*1024;		//��ʼ�����ջ�������С

	//�������ݻ�����
	m_dwBufferSize		= 0;
	m_pBuffer			= NULL;
	m_dwReceiveSize		= 0;

	InitializeCriticalSection(&m_csDataSync);
	InitializeCriticalSection(&m_csSendSync);

	Init(FALSE);
}
CSocketBase::~CSocketBase()
{
	Close(TRUE);

	DeleteCriticalSection(&m_csSendSync);
	DeleteCriticalSection(&m_csDataSync);
}	

//	bCopy :[in]�������뻺����ʱ�Ƿ�Ҫ����ԭ������
BOOL CSocketBase::ReAllocBuffer(DWORD dwBufferSize, BOOL bCopy)
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

	return TRUE;
}

//��ʼ����ز���
void CSocketBase::Init(BOOL bReAllocBuffer)
{
	SendMsg_handshake_Break(FALSE);			//�Ƿ��жϵ�ǰ����ָ�����״̬������

	m_sendMsg_isInLoop	= FALSE;			//��ǰ�Ƿ��ڷ�����Ϣʱ��ѭ���ȴ���

	if (bReAllocBuffer)
	{
		ReAllocBuffer(m_dwInitBufferSize, FALSE);
	}
	
	ClearReadBuffer();
	
	//����ָ���״̬��Ϣ
    ResetCommu_Status_Flags();

	::memset(&m_serverSockAddr, 0, sizeof(m_serverSockAddr));
	m_serverSocket	= INVALID_SOCKET;
	m_strIP			= "127.0.0.1";
	m_nPort			= 10000;
	m_hThread		= NULL;

	m_dwHeartBeatTime	= 0;		//�����ź��Զ����ͼ��

	m_dwLastSendTime	= 0;		//�������ʱ��
	m_dwLastReceiveTime	= 0;		//�������ʱ��


}
void CSocketBase::ClearReadBuffer()
{
	if (m_dwReceiveSize==0)
		return;

	SyncOpt_Data_Enter();

	m_dwReceiveSize = 0;
	
	SyncOpt_Data_Leave();
}
//����ָ���״̬��Ϣ
void CSocketBase::ResetCommu_Status_Flags()
{
	SyncOpt_Data_Enter();

	SendMsg_handshake_Break(FALSE);			//�Ƿ��жϵ�ǰ����ָ�����״̬������
	
	SyncOpt_Data_Leave();
}


BOOL CSocketBase::IsOpen() const
{
	if (m_strIP.IsEmpty())
		return FALSE;
	if (m_nPort <= 0)
		return FALSE;

	if (m_serverSocket==INVALID_SOCKET)
		return FALSE;

	if (IsDemoMode())
	{
		return TRUE;
	}

	if (m_hThread==NULL)
		return FALSE;

	return TRUE; 
}


BOOL CSocketBase::IsDemoMode() const
{
	return m_bDemoMode;
}
BOOL CSocketBase::DemoMode(BOOL bDemoMode)
{
	m_bDemoMode = bDemoMode ? TRUE : FALSE;
	return m_bDemoMode;
}


//	pszIP, uPort:[in]�˶����ƿ�IP��ַ�������˿�
//	nBufferSize :[in]��ʼ�����ջ�������С
//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
//	dwHeartBeatTime	:[in]��������źŷ��ͼ����ms��Ϊ0ʱ���Զ����ͼ���ź�
BOOL CSocketBase::Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime)
{
	unsigned long destAddr = INADDR_NONE;
	int			    status;

	if (nBufferSize <= 0)
		nBufferSize = 1024;
	m_dwInitBufferSize = nBufferSize;
	if (nGrowSize <= 0)
		nGrowSize = 0;
	m_dwGrowSize = nGrowSize;

	Close(FALSE);

	ClearErrorInfo();

	if (pszIP)
	{
		destAddr = inet_addr(pszIP);
		if (destAddr == INADDR_NONE)
		{
			m_strErrorInfo = "ERROR: IP Address!";
			return FALSE;
		}
	}
	if (nPort <= 0)
	{
		m_strErrorInfo = "ERROR: Port less than 1!";
		return FALSE;
	}
	memset(&m_serverSockAddr, 0, sizeof (m_serverSockAddr));
	m_serverSockAddr.sin_port = htons(nPort);
	m_serverSockAddr.sin_family = AF_INET;
	if (destAddr == INADDR_NONE)
		m_serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		memcpy(&m_serverSockAddr.sin_addr, &destAddr, sizeof (destAddr));


	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_serverSocket == INVALID_SOCKET)
	{
		m_strErrorInfo = CErrorInfo::TransSystemErrorCodeToString(WSAGetLastError());
		goto failed;
	}
	if (IsDemoMode())
	{
		m_strIP				= pszIP;
		m_nPort				= nPort;
		m_dwHeartBeatTime	= dwHeartBeatTime;
		m_dwLastReceiveTime	= 0;
		m_dwLastSendTime	= 0;

		//�Ƿ��жϵ�ǰ�����߳�
		m_sendMsg_handshake_bBreak = FALSE;

		return TRUE;
	}

	status = connect(m_serverSocket, (LPSOCKADDR)&m_serverSockAddr, sizeof (m_serverSockAddr));
	if (status == SOCKET_ERROR)
	{
		m_strErrorInfo = CErrorInfo::TransSystemErrorCodeToString(WSAGetLastError());
		goto failed;
	}

	m_strIP = pszIP;
	m_nPort = nPort;
    m_dwHeartBeatTime	= dwHeartBeatTime;
	m_dwLastReceiveTime	= 0;
	m_dwLastSendTime	= 0;

	//�Ƿ��жϵ�ǰ�����߳�
	m_sendMsg_handshake_bBreak = FALSE;
	DWORD dwThreadID;

	dwThreadID = 0;
	m_hThread = CreateThread(NULL, 0, SocketBase_RecvThread, this, 0, &dwThreadID);
	if (m_hThread==NULL)
	{
		m_strErrorInfo = "Can't create receive thread!";
		goto failed;
	}
	return TRUE;

failed:
	Close(TRUE);

	return FALSE;
}

BOOL CSocketBase::Close(BOOL bReleaseBuffer)
{
	BOOL bReAllocBuffer = TRUE;

	m_sendMsg_handshake_bBreak = TRUE;
	if (m_serverSocket != INVALID_SOCKET)
	{
		shutdown(m_serverSocket, 2);
		closesocket(m_serverSocket);
		m_serverSocket = INVALID_SOCKET;
	}
	if (m_hThread)
	{
		Sleep(1000);
		if (m_hThread)
		{
			CloseHandle( m_hThread );
			m_hThread= NULL;
		}
	}

	if (bReleaseBuffer)
	{
		if (m_pBuffer != NULL)
		{
			SyncOpt_Data_Enter();

			VERIFY(AGV::AVirtualFree(m_pBuffer, 0, MEM_RELEASE));
			m_pBuffer = NULL;

			m_dwBufferSize = 0;
			m_dwReceiveSize = 0;

			SyncOpt_Data_Leave();
		}
		bReAllocBuffer = FALSE;
	}

	Init(bReAllocBuffer);

	return TRUE;
}
//ֱ�ӷ��Ͳ���ָ�һ����� ReceiveData ������ʹ��
BOOL CSocketBase::Commu_SendMsg_Direct(const void* snd_msg, int msg_len)
{
	int nLen;
	if (msg_len<=0 || !snd_msg)
		return 0;

	if (IsDemoMode())
	{
		Sleep(1);
		return msg_len;
	}
	if (!IsOpen())
	{
		m_strErrorInfo = "offline!";
		return 0;
	}

	nLen = ::send(m_serverSocket, (const char*)snd_msg, msg_len, 0);
	m_dwLastSendTime = ::GetTickCount();
	if (nLen != msg_len)
	{
		m_strErrorInfo = CErrorInfo::TransSystemErrorCodeToString(WSAGetLastError());
		return 0;
	}
	return msg_len;
}

int CSocketBase::Commu_SendMsg(const void* snd_msg, int msg_len)
{
	int nLen;

	ClearErrorInfo();
	if (msg_len<=0 || !snd_msg)
		return 0;

    if (IsDemoMode())
    {
		Sleep(1);
        return msg_len;
    }

	if (!IsOpen())
	{
		m_strErrorInfo = "offline!";
		return 0;
	}

	SyncOpt_Send_Enter();
	nLen = ::send(m_serverSocket, (const char*)snd_msg, msg_len, 0);
	m_dwLastSendTime = ::GetTickCount();
	SyncOpt_Send_Leave();

	if (nLen != msg_len)
	{
		m_strErrorInfo = CErrorInfo::TransSystemErrorCodeToString(WSAGetLastError());
		return 0;
	}
	return msg_len;
}

int CSocketBase::ReceiveInfo(SOCKET clientSocket, CByteArray &infoArray)
{
	const int nBufferSize = 1024+1;
	char	szBuffer[nBufferSize];
	int nCount, nLen;
	BOOL bLoop = FALSE;

	if (clientSocket== INVALID_SOCKET)
		return 0;

	nCount = 0;
	infoArray.SetSize(0);
	do
	{
		bLoop = FALSE;
		if (clientSocket== INVALID_SOCKET)
			return 0;
		nLen = recv(clientSocket, szBuffer, nBufferSize, MSG_PEEK);
		if (nLen==0 || (nLen == SOCKET_ERROR))
		{
			return nLen;
		}
		if (nLen >= nBufferSize)
		{
			nLen = nBufferSize - 1;
			bLoop = TRUE;
		}

		//����ջ������е�ָ������
		if (clientSocket== INVALID_SOCKET)
			return 0;
		nLen = recv(clientSocket, szBuffer, nLen, 0);
		if (nLen==0 || (nLen == SOCKET_ERROR))
		{
			return nLen;
		}
		infoArray.SetSize(nCount+nLen);
		memcpy(infoArray.GetData()+nCount, szBuffer, nLen);
		nCount += nLen;

		if (nCount > 0x100000)
			break;
	}
	while (bLoop);

	return nCount;
}

void CSocketBase::ReceiveData(const void* pBuf, DWORD InBufferCount)
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
DWORD CSocketBase::ReceiveDataEx()
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

BOOL CSocketBase::GetLastErrorInfo(CString &strError) const
{
	strError = m_strErrorInfo;
	return TRUE;
}

void CSocketBase::ClearErrorInfo()
{
	m_strErrorInfo = "";
}

void CSocketBase::SyncOpt_Data_Enter()
{
	EnterCriticalSection(&m_csDataSync);
}
void CSocketBase::SyncOpt_Data_Leave()
{
	LeaveCriticalSection(&m_csDataSync);
}

void CSocketBase::SyncOpt_Send_Enter()
{
	EnterCriticalSection(&m_csSendSync);
}
void CSocketBase::SyncOpt_Send_Leave()
{
	LeaveCriticalSection(&m_csSendSync);
}

//�жϵ�ǰ����ָ�����״̬������
void CSocketBase::SendMsg_handshake_Break(BOOL bBreak)
{
	m_sendMsg_handshake_bBreak = bBreak;
}
//��ǰ�Ƿ��ڷ�����Ϣʱ��ѭ���ȴ���
bool CSocketBase::sendorder_handshake_isinloop() const
{
	return m_sendMsg_isInLoop;
}

#define FILE_MAX_LIMIT		(64*1024)
BOOL CSocketBase::Write_Communication_Info(const char *pszFileName, const BYTE *snd_msg, int msg_len, DWORD sizeLimit, DWORD defaultSize, BOOL bTimeFlag)
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
			file.Seek(LONG(uLength-dwLimit), CFile::begin);
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
//���豸�Է������������Ч�źŽ��ռ������ms
DWORD CSocketBase::GetDeviceHeartBeatTime()
{
	if (m_dwLastReceiveTime == 0)
		return 0;

	DWORD dwTime;
	
	dwTime = ::GetTickCount();
	ASSERT(dwTime >= m_dwLastReceiveTime);
	if (dwTime <= m_dwLastReceiveTime)
		return 0;

	dwTime = dwTime - m_dwLastReceiveTime;
	return dwTime;
}


//}		//namespace rpSocketBase
