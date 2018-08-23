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
		m_pThreadData	= NULL;			//关联指针

		m_hThread		= NULL;			//线程句柄
		m_dwThreadId	= 0;			//线程号

		m_IsTerminated	= TRUE;         //是否已结束线程 
		m_IsNeedExit	= FALSE;        //是否需要结束线程 
	}

	ABaseThread::~ABaseThread()
	{
		Terminate(1000);
		//m_hThread = NULL;
	}

	//初始化派生类自定义参数
	//	在 Create(...)时被调用
	bool ABaseThread::InitCustomParameter()
	{
		return true;
	}
	//析构派生类自定义参数
	//	在 Terminate(...)时被调用
	bool ABaseThread::ReleaseCustomParameter()
	{
		return true;
	}

	// ABaseThread 成员函数
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
		{//已经创建过
			ASSERT(m_dwThreadId != 0);
			ReleaseCustomParameter();
			return true;
		}

		//默认要求处于 CREATE_SUSPENDED 状态
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

	//中断线程
	bool ABaseThread::NeedExit(bool IsNeedExit)
	{
		m_IsNeedExit = IsNeedExit;

		return true;
	}
	bool ABaseThread::IsNeedExit() const
	{
		return m_IsNeedExit;
	}
	//是否已经创建成功
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
	//设置线程优先级
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
	//设置一个线程的首选处理器
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

//心跳检测，用于检测当前网络连接是否正常
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
		{//退出当前线程
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
		{//超过心跳信号发送间隔
			mySocket->Commu_SendMsg(HEARTBEAT_MSG, HEARTBEAT_MSG_LEN);
		}
	}
	mySocket->m_sendMsg_handshake_bBreak = FALSE;
	mySocket->m_hThread = NULL;
	return 0;
} 

CSocketBase::CSocketBase()
{
	m_bDemoMode			= FALSE;	//模拟模式，不使用实际的串口通讯

	::memset(&m_serverSockAddr, 0, sizeof(m_serverSockAddr));
	m_serverSocket	= INVALID_SOCKET;
	m_strIP			= "127.0.0.1";
	m_nPort			= 10000;
	m_strErrorInfo	= "";

	m_hThread		= NULL;

	m_dwGrowSize		= 4*1024;		//接收缓冲区扩容参数
	m_dwInitBufferSize	= 4*1024;		//初始化接收缓冲区大小

	//接收数据缓冲区
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

//	bCopy :[in]重新申请缓冲区时是否要保留原有数据
BOOL CSocketBase::ReAllocBuffer(DWORD dwBufferSize, BOOL bCopy)
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

	return TRUE;
}

//初始化相关参数
void CSocketBase::Init(BOOL bReAllocBuffer)
{
	SendMsg_handshake_Break(FALSE);			//是否中断当前操作指令串发送状态检测过程

	m_sendMsg_isInLoop	= FALSE;			//当前是否在发送消息时的循环等待中

	if (bReAllocBuffer)
	{
		ReAllocBuffer(m_dwInitBufferSize, FALSE);
	}
	
	ClearReadBuffer();
	
	//重设指令发送状态信息
    ResetCommu_Status_Flags();

	::memset(&m_serverSockAddr, 0, sizeof(m_serverSockAddr));
	m_serverSocket	= INVALID_SOCKET;
	m_strIP			= "127.0.0.1";
	m_nPort			= 10000;
	m_hThread		= NULL;

	m_dwHeartBeatTime	= 0;		//心跳信号自动发送间隔

	m_dwLastSendTime	= 0;		//最近发送时间
	m_dwLastReceiveTime	= 0;		//最近接收时间


}
void CSocketBase::ClearReadBuffer()
{
	if (m_dwReceiveSize==0)
		return;

	SyncOpt_Data_Enter();

	m_dwReceiveSize = 0;
	
	SyncOpt_Data_Leave();
}
//重设指令发送状态信息
void CSocketBase::ResetCommu_Status_Flags()
{
	SyncOpt_Data_Enter();

	SendMsg_handshake_Break(FALSE);			//是否中断当前操作指令串发送状态检测过程
	
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


//	pszIP, uPort:[in]运动控制卡IP地址及操作端口
//	nBufferSize :[in]初始化接收缓冲区大小
//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
//	dwHeartBeatTime	:[in]心跳检测信号发送间隔，ms，为0时不自动发送检测信号
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

		//是否中断当前接收线程
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

	//是否中断当前接收线程
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
//直接发送操作指令，一般仅在 ReceiveData 函数中使用
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

		//清接收缓冲区中的指定个数
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

//中断当前操作指令串发送状态检测过程
void CSocketBase::SendMsg_handshake_Break(BOOL bBreak)
{
	m_sendMsg_handshake_bBreak = bBreak;
}
//当前是否在发送消息时的循环等待中
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
//得设备对方心跳间隔（有效信号接收间隔），ms
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
