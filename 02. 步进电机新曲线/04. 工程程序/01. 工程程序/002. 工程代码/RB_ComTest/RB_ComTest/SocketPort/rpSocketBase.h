
#pragma once

//����ͨѶ������

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

	//ָ��ͷ�ʽ
enum SENDMSG_LEVEL
{
	sml_crucial = 0,	//must send ok (send again if all errors: send-err or handshake-err, untill timeout)
	sml_normal = 1,	//send again if some errors: send err, untill hadshake-err or timeout
	sml_onlyone = 2,	//only Send msg once

	sml_count
};
//���Ͳ�������ֵ
enum SENDMSG_STATUS
{
	sms_err_send = 0,	//send error

	sms_handshake_ok = 1,	//send ok and handshake return ok

	sms_err_no_ok_fail = 2,	//send ok,but no handshake
	sms_err_fail = 3,	//send ok but handshake return failed

	sms_count
};
//ָ���״̬��Ϣ
enum Commu_Status_Flags
{
	commu_status_none = 0,	//δ�����
	commu_status_succeed = 1,	//ͨѶ�ɹ�
	commu_status_failed = 2,	//ͨѶʧ��

	commu_status_count
};


class RP_EXT_CLASS ABaseThread
{
public:
	/*1.������ϵ������ */
	ABaseThread();
	virtual ~ABaseThread();

	/* 2.����,����,��λ */
	virtual bool Create(void* pThreadData);			//�����߳�
	virtual bool NeedExit(bool IsNeedExit);			//�ж��߳�
	virtual bool Terminate(DWORD dwTimeOut);		//�����̣߳�ǿ�ƣ�
	virtual bool Resume();							//��λ

	virtual bool IsNeedExit() const;
	virtual bool IsCreated() const;					//�Ƿ��Ѿ������ɹ�
	virtual bool IsTerminated() const;				//�Ƿ��ѽ����̣߳�����������״̬��ʱ����FALSE

	BOOL SetThreadPriority(int nPriority);			//�����߳����ȼ�
	DWORD SetThreadIdealProcessor(DWORD dwIdealProcessor);	//����һ���̵߳���ѡ������

protected:
	void*		m_pThreadData;				//����ָ��

	HANDLE		m_hThread;					//�߳̾��
	DWORD		m_dwThreadId;				//�̺߳�

	volatile bool m_IsTerminated;			//�Ƿ��ѽ����߳� 
	volatile bool m_IsNeedExit;				//�Ƿ���Ҫ�����߳�

protected:
	/*5.ִ���߳� */
	virtual void Execute(void) = 0;			//�߳�ִ�У��ڴ������б�����Ӧ m_IsNeedExit

	//��ʼ���������Զ������
	//	�� Create(...)ʱ������
	virtual bool InitCustomParameter();
	//�����������Զ������
	//	�� Terminate(...)ʱ������
	virtual bool ReleaseCustomParameter();

	friend DWORD WINAPI basethread_thread_func(LPVOID lpParam);
};
//����ͨѶ��ط�װ��
class RP_EXT_CLASS CSocketBase
{
public:
	CSocketBase();
	virtual ~CSocketBase();

	//	pszIP, uPort:[in]�˶����ƿ�IP��ַ�������˿�
	//	nBufferSize :[in]��ʼ�����ջ�������С
	//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
	//	dwHeartBeatTime	:[in]��������źŷ��ͼ����ms��Ϊ0ʱ���Զ����ͼ���ź�
	virtual BOOL Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime);

	virtual BOOL Close(BOOL bReleaseBuffer);

	virtual BOOL IsOpen() const;

protected:
	SOCKADDR_IN m_serverSockAddr;
	SOCKET		m_serverSocket;

	CString		m_strIP;
	short		m_nPort;

	HANDLE      m_hThread;              //�߳̾��

	DWORD		m_dwGrowSize;			//���ջ��������ݲ���
	DWORD		m_dwInitBufferSize;		//��ʼ�����ջ�������С

	BYTE		*m_pBuffer;				//�������ݻ�����
	DWORD		m_dwBufferSize;			//�������ݻ�������С
	DWORD		m_dwReceiveSize;		//���յ�����Ч���ݸ���

	CString		m_strErrorInfo;

private:
	DWORD		m_dwHeartBeatTime;		//�����ź��Զ����ͼ��

	DWORD		m_dwLastSendTime;		//�������ʱ��
	DWORD		m_dwLastReceiveTime;		//�������ʱ��

protected:
	//ģ��ģʽ����ʹ��ʵ�ʵĴ���ͨѶ
	BOOL    m_bDemoMode;
public:
	BOOL	IsDemoMode() const;
	BOOL	DemoMode(BOOL bDemoMode);

public:
	//������ͬ������
	void	SyncOpt_Data_Enter();
	void	SyncOpt_Data_Leave();
	//���ڷ��Ͳ���ͬ��
	void	SyncOpt_Send_Enter();
	void	SyncOpt_Send_Leave();
protected:
	CRITICAL_SECTION	m_csDataSync;		//������ͬ������
	CRITICAL_SECTION	m_csSendSync;		//���ڷ��Ͳ���ͬ��

	BOOL	m_sendMsg_handshake_bBreak;		//�Ƿ��жϵ�ǰ����ָ�����״̬������

	bool	m_sendMsg_isInLoop;				//��ǰ�Ƿ��ڷ�����Ϣʱ��ѭ���ȴ���

protected:
	friend DWORD WINAPI SocketBase_RecvThread(LPVOID lpParam);

	int	ReceiveInfo(SOCKET clientSocket, CByteArray &infoArray);

	//	bCopy :[in]�������뻺����ʱ�Ƿ�Ҫ����ԭ������
	BOOL ReAllocBuffer(DWORD dwBufferSize, BOOL bCopy);

	void ReceiveData(const void* pBuf, DWORD InBufferCount);

	//�Խ��յ������ݽ��з���
	//��������			�� m_pBuffer, m_dwReceiveSize
	//����ֵ���Ѵ�������ݸ�����<= m_dwReceiveSize����ʣ�ಿ��������һ�����ݽ��պ���
	//			<= 0ʱ��ʾδ������ǰ����֡δ��ȫ���գ���
	//			����ǰ���ݾ�Ϊ��Ч����ʱ��Ҳ���� m_dwReceiveSize
	//��ע����ʵ��Ӧ��ʱ���˺���һ�������غ�ʹ�ã�
	//	Ĭ��������ǽ���ǰ����m_pBuffer������Ϊһ����Ч����֡��������ֵΪ m_dwReceiveSize
	//	�ڱ������б��봦�� m_bcommu_status ����Ϣ������Ӱ�� SendMsg_handshake(...)��ָ����Ч�Ե��ж�
	virtual DWORD ReceiveDataEx();	//ֱ�ӷ��Ͳ���ָ�һ����� ReceiveData ������ʹ��

	//���Ͳ���ָ��
	virtual int Commu_SendMsg_Direct(const void* snd_msg, int msg_len);

	//����ջ�����
	virtual void ClearReadBuffer();
public:
	//���Ͳ���ָ��
	virtual int Commu_SendMsg(const void* snd_msg, int msg_len);

public:
	//��ʼ����ز���
	virtual void Init(BOOL bReAllocBuffer);

	//����ָ���״̬��Ϣ
	virtual void ResetCommu_Status_Flags();

	//�жϵ�ǰ����ָ�����״̬������
	virtual void SendMsg_handshake_Break(BOOL bBreak);

	//��ǰ�Ƿ��ڷ�����Ϣʱ��ѭ���ȴ���
	virtual bool sendorder_handshake_isinloop() const;

	//���豸�Է������������Ч�źŽ��ռ������ms
	DWORD GetDeviceHeartBeatTime();

public:
	static BOOL Write_Communication_Info(const char *pszFileName, const BYTE *snd_msg, int msg_len, DWORD sizeLimit, DWORD defaultSize, BOOL bTimeFlag);

public:
	BOOL GetLastErrorInfo(CString &strError) const;
	void ClearErrorInfo();

};

//}		//namespace rpSocketBase