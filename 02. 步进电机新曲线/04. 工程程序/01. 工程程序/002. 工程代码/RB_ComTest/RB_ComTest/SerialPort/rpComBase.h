
#pragma once

//串口通讯操作类

//#ifdef RP_EXT_CLASS
//	#undef RP_EXT_CLASS
//	#undef RP_EXT_API
//	#undef RP_EXT_DATA
//#endif

//#ifdef APP_RPPRNBASE
//	#define RP_EXT_CLASS       AFX_CLASS_EXPORT
//	#define RP_EXT_API         AFX_API_EXPORT
//	#define RP_EXT_DATA        AFX_DATA_EXPORT
//#else
//	#define RP_EXT_CLASS       AFX_CLASS_IMPORT
//	#define RP_EXT_API         AFX_API_IMPORT
//	#define RP_EXT_DATA        AFX_DATA_IMPORT
//#endif

namespace rpComBase
{

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

	//串口通讯相关封装类
	class /*RP_EXT_CLASS*/ CComBase
	{
	public:
		CComBase();
		virtual ~CComBase();

		// port initialisation
		//	nBufferSize :[in]初始化接收缓冲区大小
		//	nGrowSize	:[in]当缓冲区大小不够时,指定自动扩容的大小(增幅),当<=0时表示不允许自动扩容
		virtual BOOL Open(UINT portnr, DWORD dwBaud, int nBufferSize, int nGrowSize);

		virtual BOOL Close(BOOL bReleaseBuffer);

		virtual BOOL IsOpen() const;

	protected:
		DWORD		m_dwGrowSize;			//接收缓冲区扩容参数
		DWORD		m_dwInitBufferSize;		//初始化接收缓冲区大小

		BYTE		*m_pBuffer;				//接收数据缓冲区
		DWORD		m_dwBufferSize;			//接收数据缓冲区大小
		DWORD		m_dwReceiveSize;		//接收到的有效数据个数

		void		*m_pComPort;			//串口操作类指针

	protected:
		//当前指令发送是否成功
		//	Commu_Status_Flags
		volatile BYTE	m_bcommu_status;

		//模拟模式，不使用实际的串口通讯
		BOOL    m_bDemoMode;
	public:
		BOOL	IsDemoMode() const;
		BOOL	DemoMode(BOOL bDemoMode);

	public:
		//缓冲区同步控制
		void	SyncOpt_Data_Enter();
		void	SyncOpt_Data_Leave();
		//串口发送操作同步
		void	SyncOpt_Send_Enter();
		void	SyncOpt_Send_Leave();
	protected:
		CRITICAL_SECTION	m_csComBaseSync;		//缓冲区同步控制
		CRITICAL_SECTION	m_csSendSync;			//串口发送操作同步

		BOOL	m_sendMsg_handshake_bBreak;			//是否中断当前操作指令串发送状态检测过程

		bool	m_sendMsg_isInLoop;					//当前是否在发送消息时的循环等待中

	//protected:
	public:
		//	bCopy :[in]重新申请缓冲区时是否要保留原有数据
		BOOL ReAllocBuffer(DWORD dwBufferSize, BOOL bCopy);

		void ReceiveData(const void* pBuf, DWORD InBufferCount);

		//直接发送操作指令，一般仅在 ReceiveData 函数中使用
		virtual BOOL Commu_SendMsg_Direct(const BYTE* snd_msg, int msg_len);

		friend void ComBase_ReceiveData(LPVOID pSender, const void* pBuf, DWORD InBufferCount);

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
		virtual BOOL Commu_SendMsg(const BYTE* snd_msg, int msg_len);

		//操作指令串发送
		//参数定义：
		//snd_msg, s_msg_len	:[in]发送的指令串及长度
		//timeout				:[in]发送指令的总延时值 ms
		//time_repeatsend		:[in]当发送失败时，重发指令口串的时间间隔（累积值在 timeout范围内） ms
		//sendmsg_level			:[in] SENDMSG_LEVEL
		//返回值：SENDMSG_STATUS
		virtual int SendMsg_handshake(BYTE *snd_msg, int s_msg_len,
			int timeout, int time_repeatsend, int sendmsg_level);

		virtual int SendMsg_handshake0(BYTE *snd_msg, int s_msg_len,
			int timeout, int time_repeatsend, int sendmsg_level, BOOL bFlag);

		//操作指令串发送
		struct FLAGCHECK
		{
			BYTE	bFlag;		//函数退出条件
			int		nResult;	//满足条件时对应的返回值
		};
		//snd_msg, s_msg_len	:[in]发送的指令串及长度
		//timeout				:[in]发送指令的总延时值 ms
		//time_repeatsend		:[in]当发送失败时，重发指令口串的时间间隔（累积值在 timeout范围内） ms
		//pFlag					:[in/out]指令发送状态标识（接收到相应信息时被自动赋值），当为NULL时，使用 sml_normal 模式发送
		//pFlagCheck,nCheckCount:[in]指定函数退出条件，除 TimeOut外，仅当  *pFlag==pFlagCheck[i] 时才返回
		//time_check			:[in]函数退出条件(pFlagCheck[i])判断的超时设置，ms
		virtual int SendMsg_handshake(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check);

		virtual int SendMsg_handshake0(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check, BOOL bFlag);
		//清接收缓冲区
		virtual void ClearReadBuffer();


	public:
		//初始化相关参数
		virtual void Init(BOOL bReAllocBuffer);

		//重设指令发送状态信息
		virtual void ResetCommu_Status_Flags();

		//中断当前操作指令串发送状态检测过程
		virtual void SendMsg_handshake_Break(BOOL bBreak);

	public:
		static BOOL Write_Communication_Info(const char *pszFileName, const BYTE *snd_msg, int msg_len, DWORD sizeLimit, DWORD defaultSize, BOOL bTimeFlag);

	};

}		//namespace rpComBase