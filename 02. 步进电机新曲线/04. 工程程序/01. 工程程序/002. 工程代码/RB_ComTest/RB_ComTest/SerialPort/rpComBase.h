
#pragma once

//����ͨѶ������

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

	//����ͨѶ��ط�װ��
	class /*RP_EXT_CLASS*/ CComBase
	{
	public:
		CComBase();
		virtual ~CComBase();

		// port initialisation
		//	nBufferSize :[in]��ʼ�����ջ�������С
		//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
		virtual BOOL Open(UINT portnr, DWORD dwBaud, int nBufferSize, int nGrowSize);

		virtual BOOL Close(BOOL bReleaseBuffer);

		virtual BOOL IsOpen() const;

	protected:
		DWORD		m_dwGrowSize;			//���ջ��������ݲ���
		DWORD		m_dwInitBufferSize;		//��ʼ�����ջ�������С

		BYTE		*m_pBuffer;				//�������ݻ�����
		DWORD		m_dwBufferSize;			//�������ݻ�������С
		DWORD		m_dwReceiveSize;		//���յ�����Ч���ݸ���

		void		*m_pComPort;			//���ڲ�����ָ��

	protected:
		//��ǰָ����Ƿ�ɹ�
		//	Commu_Status_Flags
		volatile BYTE	m_bcommu_status;

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
		CRITICAL_SECTION	m_csComBaseSync;		//������ͬ������
		CRITICAL_SECTION	m_csSendSync;			//���ڷ��Ͳ���ͬ��

		BOOL	m_sendMsg_handshake_bBreak;			//�Ƿ��жϵ�ǰ����ָ�����״̬������

		bool	m_sendMsg_isInLoop;					//��ǰ�Ƿ��ڷ�����Ϣʱ��ѭ���ȴ���

	//protected:
	public:
		//	bCopy :[in]�������뻺����ʱ�Ƿ�Ҫ����ԭ������
		BOOL ReAllocBuffer(DWORD dwBufferSize, BOOL bCopy);

		void ReceiveData(const void* pBuf, DWORD InBufferCount);

		//ֱ�ӷ��Ͳ���ָ�һ����� ReceiveData ������ʹ��
		virtual BOOL Commu_SendMsg_Direct(const BYTE* snd_msg, int msg_len);

		friend void ComBase_ReceiveData(LPVOID pSender, const void* pBuf, DWORD InBufferCount);

		//�Խ��յ������ݽ��з���
		//��������			�� m_pBuffer, m_dwReceiveSize
		//����ֵ���Ѵ�������ݸ�����<= m_dwReceiveSize����ʣ�ಿ��������һ�����ݽ��պ���
		//			<= 0ʱ��ʾδ������ǰ����֡δ��ȫ���գ���
		//			����ǰ���ݾ�Ϊ��Ч����ʱ��Ҳ���� m_dwReceiveSize
		//��ע����ʵ��Ӧ��ʱ���˺���һ�������غ�ʹ�ã�
		//	Ĭ��������ǽ���ǰ����m_pBuffer������Ϊһ����Ч����֡��������ֵΪ m_dwReceiveSize
		//	�ڱ������б��봦�� m_bcommu_status ����Ϣ������Ӱ�� SendMsg_handshake(...)��ָ����Ч�Ե��ж�
		virtual DWORD ReceiveDataEx();

		//���Ͳ���ָ��
		virtual BOOL Commu_SendMsg(const BYTE* snd_msg, int msg_len);

		//����ָ�����
		//�������壺
		//snd_msg, s_msg_len	:[in]���͵�ָ�������
		//timeout				:[in]����ָ�������ʱֵ ms
		//time_repeatsend		:[in]������ʧ��ʱ���ط�ָ��ڴ���ʱ�������ۻ�ֵ�� timeout��Χ�ڣ� ms
		//sendmsg_level			:[in] SENDMSG_LEVEL
		//����ֵ��SENDMSG_STATUS
		virtual int SendMsg_handshake(BYTE *snd_msg, int s_msg_len,
			int timeout, int time_repeatsend, int sendmsg_level);

		virtual int SendMsg_handshake0(BYTE *snd_msg, int s_msg_len,
			int timeout, int time_repeatsend, int sendmsg_level, BOOL bFlag);

		//����ָ�����
		struct FLAGCHECK
		{
			BYTE	bFlag;		//�����˳�����
			int		nResult;	//��������ʱ��Ӧ�ķ���ֵ
		};
		//snd_msg, s_msg_len	:[in]���͵�ָ�������
		//timeout				:[in]����ָ�������ʱֵ ms
		//time_repeatsend		:[in]������ʧ��ʱ���ط�ָ��ڴ���ʱ�������ۻ�ֵ�� timeout��Χ�ڣ� ms
		//pFlag					:[in/out]ָ���״̬��ʶ�����յ���Ӧ��Ϣʱ���Զ���ֵ������ΪNULLʱ��ʹ�� sml_normal ģʽ����
		//pFlagCheck,nCheckCount:[in]ָ�������˳��������� TimeOut�⣬����  *pFlag==pFlagCheck[i] ʱ�ŷ���
		//time_check			:[in]�����˳�����(pFlagCheck[i])�жϵĳ�ʱ���ã�ms
		virtual int SendMsg_handshake(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check);

		virtual int SendMsg_handshake0(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, volatile BYTE *pFlag, FLAGCHECK *pFlagCheck, int nCheckCount, int time_check, BOOL bFlag);
		//����ջ�����
		virtual void ClearReadBuffer();


	public:
		//��ʼ����ز���
		virtual void Init(BOOL bReAllocBuffer);

		//����ָ���״̬��Ϣ
		virtual void ResetCommu_Status_Flags();

		//�жϵ�ǰ����ָ�����״̬������
		virtual void SendMsg_handshake_Break(BOOL bBreak);

	public:
		static BOOL Write_Communication_Info(const char *pszFileName, const BYTE *snd_msg, int msg_len, DWORD sizeLimit, DWORD defaultSize, BOOL bTimeFlag);

	};

}		//namespace rpComBase