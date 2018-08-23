
#pragma once

//网口通讯操作类

#ifdef RP_EXT_CLASS
#undef RP_EXT_CLASS
#undef RP_EXT_API
#undef RP_EXT_DATA
#endif

#define RP_EXT_CLASS       
#define RP_EXT_API         
#define RP_EXT_DATA        

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

//namespace rpSocketBase
//{

	//指令发送方式
enum SENDMSG_LEVEL
{
	sml_crucial = 0,	//must send ok (send again if all errors: send-err or handshake-err, untill timeout)
	sml_normal = 1,	//send again if some errors: send err, untill hadshake-err or timeout
	sml_onlyone = 2,	//only Send msg once

	sml_count
};
//发送操作返回值
enum SENDMSG_STATUS
{
	sms_err_send = 0,	//send error

	sms_handshake_ok = 1,	//send ok and handshake return ok

	sms_err_no_ok_fail = 2,	//send ok,but no handshake
	sms_err_fail = 3,	//send ok but handshake return failed

	sms_count
};
//指令发送状态信息
enum Commu_Status_Flags
{
	commu_status_none = 0,	//未作标记
	commu_status_succeed = 1,	//通讯成功
	commu_status_failed = 2,	//通讯失败

	commu_status_count
};


class RP_EXT_CLASS ABaseThread
{
public:
	/*1.构造与系构函数 */
	ABaseThread();
	virtual ~ABaseThread();

	/* 2.创建,结束,复位 */
	virtual bool Create(void* pThreadData);			//创建线程
	virtual bool NeedExit(bool IsNeedExit);			//中断线程
	virtual bool Terminate(DWORD dwTimeOut);		//结束线程（强制）
	virtual bool Resume();							//复位

	virtual bool IsNeedExit() const;
	virtual bool IsCreated() const;					//是否已经创建成功
	virtual bool IsTerminated() const;				//是否已结束线程，当正常运行状态下时返回FALSE

	BOOL SetThreadPriority(int nPriority);			//设置线程优先级
	DWORD SetThreadIdealProcessor(DWORD dwIdealProcessor);	//设置一个线程的首选处理器

protected:
	void*		m_pThreadData;				//关联指针

	HANDLE		m_hThread;					//线程句柄
	DWORD		m_dwThreadId;				//线程号

	volatile bool m_IsTerminated;			//是否已结束线程 
	volatile bool m_IsNeedExit;				//是否需要结束线程

protected:
	/*5.执行线程 */
	virtual void Execute(void) = 0;			//线程执行，在处理函数中必须响应 m_IsNeedExit

	//初始化派生类自定义参数
	//	在 Create(...)时被调用
	virtual bool InitCustomParameter();
	//析构派生类自定义参数
	//	在 Terminate(...)时被调用
	virtual bool ReleaseCustomParameter();

	friend DWORD WINAPI basethread_thread_func(LPVOID lpParam);
};
//串口通讯相关封装类
class RP_EXT_CLASS CSocketBase
{
public:
	CSocketBase();
	virtual ~CSocketBase();

	//	pszIP, uPort:[in]运动控制卡IP地址及操作端口
	//	nBufferSize :[in]初始化接收缓冲区大小
	//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
	//	dwHeartBeatTime	:[in]心跳检测信号发送间隔，ms，为0时不自动发送检测信号
	virtual BOOL Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime);

	virtual BOOL Close(BOOL bReleaseBuffer);

	virtual BOOL IsOpen() const;

protected:
	SOCKADDR_IN m_serverSockAddr;
	SOCKET		m_serverSocket;

	CString		m_strIP;
	short		m_nPort;

	HANDLE      m_hThread;              //线程句柄

	DWORD		m_dwGrowSize;			//接收缓冲区扩容参数
	DWORD		m_dwInitBufferSize;		//初始化接收缓冲区大小

	BYTE		*m_pBuffer;				//接收数据缓冲区
	DWORD		m_dwBufferSize;			//接收数据缓冲区大小
	DWORD		m_dwReceiveSize;		//接收到的有效数据个数

	CString		m_strErrorInfo;

private:
	DWORD		m_dwHeartBeatTime;		//心跳信号自动发送间隔

	DWORD		m_dwLastSendTime;		//最近发送时间
	DWORD		m_dwLastReceiveTime;		//最近接收时间

protected:
	//模拟模式，不使用实际的串口通讯
	BOOL    m_bDemoMode;
public:
	BOOL	IsDemoMode() const;
	BOOL	DemoMode(BOOL bDemoMode);

public:
	//缓冲区同步控制
	void	SyncOpt_Data_Enter();
	void	SyncOpt_Data_Leave();
	//网口发送操作同步
	void	SyncOpt_Send_Enter();
	void	SyncOpt_Send_Leave();
protected:
	CRITICAL_SECTION	m_csDataSync;		//缓冲区同步控制
	CRITICAL_SECTION	m_csSendSync;		//串口发送操作同步

	BOOL	m_sendMsg_handshake_bBreak;		//是否中断当前操作指令串发送状态检测过程

	bool	m_sendMsg_isInLoop;				//当前是否在发送消息时的循环等待中

protected:
	friend DWORD WINAPI SocketBase_RecvThread(LPVOID lpParam);

	int	ReceiveInfo(SOCKET clientSocket, CByteArray &infoArray);

	//	bCopy :[in]重新申请缓冲区时是否要保留原有数据
	BOOL ReAllocBuffer(DWORD dwBufferSize, BOOL bCopy);

	void ReceiveData(const void* pBuf, DWORD InBufferCount);

	//对接收到的数据进行分析
	//操作对象			： m_pBuffer, m_dwReceiveSize
	//返回值：已处理的数据个数（<= m_dwReceiveSize），剩余部分留到下一次数据接收后处理
	//			<= 0时表示未处理（当前数据帧未完全接收），
	//			当当前数据均为无效数据时，也返回 m_dwReceiveSize
	//备注：在实际应用时，此函数一般需重载后使用，
	//	默认情况下是将当前缓冲m_pBuffer内容作为一次有效数据帧处理，返回值为 m_dwReceiveSize
	//	在本函数中必须处理 m_bcommu_status 的信息，否则将影响 SendMsg_handshake(...)对指令有效性的判断
	virtual DWORD ReceiveDataEx();	//直接发送操作指令，一般仅在 ReceiveData 函数中使用

	//发送操作指令
	virtual int Commu_SendMsg_Direct(const void* snd_msg, int msg_len);

	//清接收缓冲区
	virtual void ClearReadBuffer();
public:
	//发送操作指令
	virtual int Commu_SendMsg(const void* snd_msg, int msg_len);

public:
	//初始化相关参数
	virtual void Init(BOOL bReAllocBuffer);

	//重设指令发送状态信息
	virtual void ResetCommu_Status_Flags();

	//中断当前操作指令串发送状态检测过程
	virtual void SendMsg_handshake_Break(BOOL bBreak);

	//当前是否在发送消息时的循环等待中
	virtual bool sendorder_handshake_isinloop() const;

	//得设备对方心跳间隔（有效信号接收间隔），ms
	DWORD GetDeviceHeartBeatTime();

public:
	static BOOL Write_Communication_Info(const char *pszFileName, const BYTE *snd_msg, int msg_len, DWORD sizeLimit, DWORD defaultSize, BOOL bTimeFlag);

public:
	BOOL GetLastErrorInfo(CString &strError) const;
	void ClearErrorInfo();

};

//}		//namespace rpSocketBase