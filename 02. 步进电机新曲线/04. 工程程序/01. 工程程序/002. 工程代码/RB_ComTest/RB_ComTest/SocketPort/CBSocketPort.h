#pragma once


//����ӡ�����İ�����ͨѶ������
#include "../include/rpBasexp.h"
#include "rpSocketBase.h"

#ifdef RP_EXT_CLASS
#undef RP_EXT_CLASS
#undef RP_EXT_API
#undef RP_EXT_DATA
#endif

#define RP_EXT_CLASS       
#define RP_EXT_API         
#define RP_EXT_DATA     

//#ifdef RP_EXT_CLASS
//#undef RP_EXT_CLASS
//#undef RP_EXT_API
//#undef RP_EXT_DATA
//#endif
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

#pragma pack(push)
#pragma pack(1)

//ָ��ִ��״̬��������������ָ�����ִ�й����еķ�����Ϣ��
//��λ��������Ϣ���壨GCB-->PC����ΪPC����������㣬ʵ����λ�����Ͳ�����ϵ (PC-1)==(GCB)��
// enum COMM_RESULT_GCB_0
// {
// 	gcb_0_comm_result_succeed	= 0x00,	//ָ������ִ�����
// 
// 	gcb_0_comm_result_failed	= 0x01,	//ָ�����ʧ��
// 	gcb_0_comm_result_repeater	= 0x02,	//�¼��忨����ָ��ת�����м̣�ʧ��
// 	gcb_0_comm_result_invalid	= 0x03,	//��Чָ��
// 
// 	gcb_0_comm_result_other		= 0xFF	//��������
// };
enum GCB_COMM_RESULT_GCB
{
	gcb_comm_result_init = 0x00,	//��ʼ����δ�趨״̬

	gcb_comm_result_succeed = 0x01,	//ָ������ִ�����

	gcb_comm_result_failed = 0x02,	//ָ�����ʧ��
	gcb_comm_result_repeater = 0x03,	//�¼��忨����ָ��ת�����м̣�ʧ��
	gcb_comm_result_invalid = 0x04,	//��Чָ��

	gcb_comm_result_other = 0xFF	//��������
};
//��λ��������Ϣ���壨PC-->GCB��
enum GCB_COMM_RESULT_PC
{
	gcb_pc_comm_result_succeed = 0x00,	//ָ������ִ�����

	gcb_pc_comm_result_failed = 0x01,	//ָ�����ʧ��
	gcb_pc_comm_result_invalid = 0x02,	//��Чָ��

	gcb_pc_comm_result_other = 0xFF	//��������
};

//�������ͷ�ʽ��ָ��״̬
enum GCB_PROTOCOL_SEND_STATE
{
	gcb_protocol_send_state_failed = 0,	//ʧ��
	gcb_protocol_send_state_checking,		//������
	gcb_protocol_send_state_ok,				//���ͳɹ�

	gcb_protocol_send_state_count
};

struct GCB_COMMAND_STATE
{
	BYTE sendState;				//ָ��ͼ�ִ��״̬��GCB_PROTOCOL_SEND_STATE
	BYTE commState;				//ָ���״̬��GCB_COMM_RESULT_GCB
};
#define TRANS_COMMSTATE_GCB_TO_PC(gcb)	BYTE(((BYTE)(gcb)==0xFF) ? 0xFF : ((BYTE)(gcb)+1))

enum GCB_TCP_DEFAULT_PORT
{
	tcp_default_port_control = 10000,	//GCB���ü���ӡ����ָ��
	tcp_default_port_data = 10001,	//��ӡ����ָ��

	tcp_default_port_unknown = 0xFFFF
};

enum GCB_PROTOCOL_VERSION_INFO
{
	gcb_protocol_version_none = 0,			//��ʼ��

	gcb_protocol_version_1000 = 1000,			// V1.0.00

	gcb_protocol_version_unknown = 0xFFFF		//��Ч�汾��Ϣ
};

struct RP_EXT_CLASS GCB_Protocol_Control_Statue
{
	// PC --> GCB --> PC
	GCB_COMMAND_STATE	gcb_getversion;				//�汾
	WORD				gcb_getversion_val;

	GCB_COMMAND_STATE	gcb_getbuffersize;			//�û�������С
	DWORD				gcb_getbuffersize_val;

	GCB_COMMAND_STATE	gcb_init;					//�忨��ʼ��

	GCB_COMMAND_STATE	gcb_print_ready;			//��ӡ׼��

	GCB_COMMAND_STATE	gcb_repeater_config;		//�¼��忨����ָ�PC-->GCB-->�忨��

public:
	void Initialize();
};
#pragma pack(pop)

class RP_EXT_CLASS CCBSocketPort_Data : public CSocketBase
{

public:
	CCBSocketPort_Data();
	virtual ~CCBSocketPort_Data();

public:
	//	pszIP, uPort:[in]�˶����ƿ�IP��ַ�������˿�
	//	nBufferSize :[in]��ʼ�����ջ�������С
	//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
	//	dwHeartBeatTime	:[in]��������źŷ��ͼ����ms��Ϊ0ʱ���Զ����ͼ���ź�
	virtual BOOL Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime);

	virtual BOOL Close(BOOL bReleaseBuffer);
	virtual BOOL IsOpen() const;

	//�Խ��յ������ݽ��з���
	//��������			�� m_pBuffer, m_dwReceiveSize
	//����ֵ���Ѵ�������ݸ�����<= m_dwReceiveSize����ʣ�ಿ��������һ�����ݽ��պ���
	//			<= 0ʱ��ʾδ������ǰ����֡δ��ȫ���գ���
	//			����ǰ���ݾ�Ϊ��Ч����ʱ��Ҳ���� m_dwReceiveSize
	//��ע����ʵ��Ӧ��ʱ���˺���һ�������غ�ʹ�ã�
	//	Ĭ��������ǽ���ǰ����m_pBuffer������Ϊһ����Ч����֡��������ֵΪ m_dwReceiveSize
	//	�ڱ������б��봦�� m_bcommu_status ����Ϣ������Ӱ�� SendMsg_handshake(...)��ָ����Ч�Ե��ж�
	virtual DWORD ReceiveDataEx();
};
enum SENDMSG_HANDSHAKE_RESULT
{
	shr_succeed = 0,		//����

	shr_parameter,			//��������

	shr_outofmemory,		//�ڴ治��

	shr_send_failed,		//ָ���ʧ��
	shr_cmd_failed,			//ָ��ִ�з��ش���

	shr_stop,				//�쳣��ֹ��stop, print end, sys error

	shr_timeout,			//ָ��ִ�в������ڹ涨ʱ����δ���յ�������Ϣ��

	shr_count
};

//�м�����֡����
//���أ�������Ļ������ݸ���
//�忨-->GCB-->PC
//	pData				:[in]�ⲿ���ú�������Ĳ�����Ϣ��PC_Repeater_Config_SetProc(...)
//	pDataList,dataSize	:[in]�м�����֡���ݻ����������ݳ���
//���أ��м������е�ǰ��������ݳ��ȣ�<= dataSize
typedef DWORD(*PC_REPEATER_CONFIG_PROC)(void *pData, const void *pDataList, int dataSize);
class RP_EXT_CLASS CCBSocketPort_Control : public CSocketBase
{

public:
	CCBSocketPort_Control();
	virtual ~CCBSocketPort_Control();

protected:
	GCB_Protocol_Control_Statue	*m_pProtocolstatus;		//����λ�����صĸ��ֲ�����״̬����Ϣ-��ǰ

	CBufferManager				m_sendBuffer;			//���ͻ�����

public:
	//	pszIP, uPort:[in]�˶����ƿ�IP��ַ�������˿�
	//	nBufferSize :[in]��ʼ�����ջ�������С
	//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
	//	dwHeartBeatTime	:[in]��������źŷ��ͼ����ms��Ϊ0ʱ���Զ����ͼ���ź�
	virtual BOOL Open(const char *pszIP, short nPort, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime);

	virtual BOOL Close(BOOL bReleaseBuffer);
	virtual BOOL IsOpen() const;

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
	virtual int Commu_SendMsg_Direct(const void* snd_msg, int msg_len);
	virtual int Commu_SendMsg(const void* snd_msg, int msg_len);

	//���Ͳ���ָ���ָ��ʱ����ڼ��ָ�����Ϣ��
	//����ֵ ��SENDMSG_HANDSHAKE_RESULT
	int Commu_SendMsg_handshake(GCB_COMMAND_STATE *pCmdState, const void *snd_msg, int msg_len, int time_check);

protected:
	// ����֡������������һ����Ч֡��
	//���أ�������Ļ������ݸ���
	DWORD DataFrameAnalyze(const BYTE* pBuffer, DWORD dwBufferSize);

	//�м�����֡����
	//���أ�������Ļ������ݸ���
	virtual DWORD DataFrameAnalyze_repeater_config(const BYTE* pBuffer, DWORD dwBufferSize);

	//����ָ���״̬
	//	bResult :[in]GCB_COMM_RESULT_PC
	int Commu_SendMsg_confirmation(BYTE bCmd, BYTE bResult);

public:
	// ��װָ����Ϣ������֡��
	//	pDstBuffer :[out]��װ�������֡����������������С >= pt_data_len_max
	static int dataframe_transdata(BYTE* pDstBuffer, BYTE bCmd, const BYTE* pCmdData, WORD wCmdSize);
	static int dataframe_transdata(BYTE* pDstBuffer, BYTE bCmd, BYTE bResultPC);

	//�õ�ǰ��λ�����صĸ��ֲ�����״̬����Ϣ
	const GCB_Protocol_Control_Statue *GetControlStatus() const;

public:
	//����ֵ �� SENDMSG_HANDSHAKE_RESULT

	// PC --> GCB --> PC
	//�汾
	int sendorder_gcb_getversion(WORD *pwVersion, int time_check);
	//�û�������С
	int sendorder_gcb_getbuffersize(DWORD *pdwBufferSize, int time_check);

	//�忨��ʼ��
	int sendorder_gcb_init(BYTE bInitWay, int time_check);
	//��ӡ׼��
	int sendorder_gcb_print_ready(int time_check);

	//�¼��忨����ָ�PC-->GCB-->�忨��
	int sendorder_gcb_repeater_config(void *pDataList, BYTE cmdSize, int time_check);

	//�¼��忨����ָ��忨-->GCB-->PC��-- PC ����
	int sendorder_confirmation_pc_repeater_config(void *pDataList, BYTE cmdSize);

public:
	//�Ƿ����Debug��Ϣ��д�ļ���
	BOOL debug_IsWriteSendInfo() const;
	BOOL debug_IsWriteReceiveInfo() const;
	//	pszFileName :[in]Debug��Ϣ�ļ���
	void debug_WriteSendInfo(const char *pszFileName, BOOL bWrite);
	void debug_WriteReceiveInfo(const char *pszFileName, BOOL bWrite);

protected:
	//�Ƿ����Debug��Ϣ��д�ļ���
	BOOL m_debug_bWriteSendInfo;
	BOOL m_debug_bWriteReceiveInfo;
	CString m_debug_strWriteSendInfoFile;
	CString m_debug_strWriteReceiveInfoFile;

	//�м����ݷ���
private:
	PC_REPEATER_CONFIG_PROC m_pc_repeater_config_pFunc;
	void *m_pc_repeater_config_pFuncData;
public:
	void PC_Repeater_Config_SetProc(PC_REPEATER_CONFIG_PROC pFunc, void *pData);

};
