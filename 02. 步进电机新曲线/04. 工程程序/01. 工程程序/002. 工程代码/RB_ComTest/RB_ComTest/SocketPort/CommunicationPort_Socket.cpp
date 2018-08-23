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

//打印输出
class CSocketDataThread : public ABaseThread
{
public:
	/*1.构造与系构函数 */
	CSocketDataThread();
	virtual ~CSocketDataThread();

protected:
	/*5.执行线程 */
	void Execute(void);						//线程执行

	friend class CCommunicationPort;
};

//CSocketDataThread
CSocketDataThread::CSocketDataThread()
{
}

CSocketDataThread::~CSocketDataThread()
{
}

// CSocketDataThread 成员函数
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


//创建处理线程
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
//关闭处理线程
bool CCommunicationPort::socket_thread_close()
{
	socket_thread_wait_for_terminated(5000);
	return true;
}
//释放处理线程
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

//线程是否已经创建成功（可正常进行打印相关处理）
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
//线程是否已经处理正常运转中（正常打印过程已经启动）
bool CCommunicationPort::socket_thread_isrunning() const
{
	CSocketDataThread* pThread;

	pThread = (CSocketDataThread*)m_socket_pThread;
	if (pThread == NULL)
		return false;

	if (pThread->IsTerminated())
		return false;

	//只要有一个线程还处于正常运行状态，即返回TRUE
	return TRUE;
}
//等待除 Write Thread 外的其它线程关闭
//	dwTimeOut	:[in]超时参数，ms, 为0时表示不限时等待
//注：一般用于 Write Thread 退出前的延时处理
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
	//等待线程处理完成
	dwStart = GetTickCount();
	while (1)
	{
		if (pThread->IsTerminated())
			return;

		if (dwTimeOut > 0 && (::GetTickCount() - dwStart) > dwTimeOut)
			break;		//超时，强制结束线程
		Sleep(1);
	}

	//强制结束线程处理过程
	if (pThread != NULL)
	{
		pThread->Terminate(2000);
	}
	return;
}

//强制初始化参数
void CCommunicationPort::socket_init(BOOL bReAllocBuffer)
{
	socket_Close(bReAllocBuffer);
}
//	pszIP, uPort:[in]运动控制卡IP地址及操作端口
//	nBufferSize :[in]初始化接收缓冲区大小
//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
//	dwHeartBeatTime	:[in]心跳检测信号发送间隔，ms，为0时不自动发送检测信号
//	pErrorInfo	:[out]错误信息，仅当返回值为FALSE时有效
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
//发送操作指令
int CCommunicationPort::socket_senddata(const void* snd_msg, int msg_len)
{
	if (!socket_IsEnable())
		return 0;

	if (m_socket_Data.IsOpen())
		return m_socket_Data.Commu_SendMsg(snd_msg, msg_len);

	return 0;
}
//中继数据帧分析
DWORD CCommunicationPort::socket_repeater_config_proc(void *pData, const void *pDataList, int dataSize)
{
	CCommunicationPort *pPort = (CCommunicationPort*)pData;
	DWORD dwResult = 0;

	dwResult = pPort->socket_AddReceiveData(pDataList, dataSize);

	return dwResult;
}
//中继数据帧分析-添加数据信息
DWORD CCommunicationPort::socket_AddReceiveData(const void* pBuf, DWORD InBufferCount)
{
	if (!socket_IsEnable())
		return 0;

	if (!m_socket_Control.IsOpen())
		return 0;

	ReceiveData(pBuf, InBufferCount);

	return InBufferCount;
}