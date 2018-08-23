#pragma once


//����ӡ�����ذ壨V4.0������ͨѶ������

#include "../include/rpBasexp.h"
#include "../SerialPort/rpComBase.h"
#include "../include/rpPrnData.h"

#define WORD_MAX					65535
#define servo_download_timevalue_length 242	//�ŷ�����ʱ���ʱ��ֵһ�η��ͳ���
#define TIME_CONVERT_VALUE			1000000
#define TIME_CONVERT(s)				(s*TIME_CONVERT_VALUE)

#define ld_vol_max				5.0
#define ld_power_max			250
#define ld_power_channel_max	32
#define ld_buffer_size			32000
#define ld_data_length			250
#define ld_map_data_max			30
#define ld_light_num			8
#define ld_voltage_max			64

#define WM_COMM_RXCHAR				WM_USER+1
#define WM_COMM_TXCHAR				WM_USER+2
#define WM_SFDOWNLOAD_PROGRESS		WM_USER+3
#define WM_RASSTATE					WM_USER+4
#define WM_LD_PROGRESS				WM_USER+5
#define WM_LD_SET_POWER_STATUS		WM_USER+6

struct OnLinePara
{
	DWORD dist_acce; //��N����Ӧ�ļ��پ��룬cts��DWORD
	DWORD dist_dece; //��N����Ӧ�ļ��پ��룬cts��DWORD
	DWORD speed;     //��N����Ӧ���ٶȣ�cts/us��DWORD
};

struct PositionPara
{
	DWORD pos_x_min; //С����Сλ��
	DWORD pos_x_max; //С�����λ��
	DWORD pos_x_clean;//��ϴ/ɢ��λ��
	DWORD pos_x_moisture;//��ʪλ��
	DWORD pos_y_min;//˿����Сλ�ã��������ͺ��ԣ�
	DWORD pos_y_max;//˿�����λ�ã��������ͺ��ԣ�
};

enum PROTOCOL_MODE
{
	debug_mode = 0,
	print_mode = 1
};

//�豸��λ��ʶ���壬ʵ�ʶ�λֵ���ԡ�С����ʶ �� ˿�˱�ʶ����ʽ��ʶ
enum DEVICE_POSITION
{
	//С��λ�ñ�ʶ����
	pos_x_device_current = 0x01,		//�豸��λ-��ǰλ�ã�С��������
	pos_x_device_min = 0x02,		//�豸��λ-��Сλ�ã��豸ԭ�㣩
	pos_x_device_max = 0x03,		//�豸��λ-���λ��
	pos_x_device_lastpos = 0x04,		//�豸��λ-�ϴ�λ��
	pos_x_device_ready = 0x05,		//�豸��λ-����λ���豸�������Լ���ɺ����λ�ã�
	pos_x_device_check = 0x06,		//�豸��λ-���λ�����ʼ��λ���ݽ�ƽ������ʹ�ã�
	pos_x_device_clean = 0x07,		//�豸��λ-��ϴλ��ƽ�������ݲ���Ҫ����ʹ����ͷ��ϵͳ����Ҫ�� 
	pos_x_device_moisture = 0x08,		//�豸��λ-��ʪλ��ƽ�������ݲ���Ҫ����ʹ����ͷ��ϵͳ����Ҫ��

	//˿��/����λ�ñ�ʶ����
	pos_y_device_current = 0x10,//�豸��λ-��ǰλ�ã�˿��/����������������ʱ�����������⣬��������Y���ʶ����Ч��
	pos_y_device_min = 0x20,		//�豸��λ-��Сλ�ã��豸ԭ�㣬����������Ҫ��
	pos_y_device_max = 0x30,		//�豸��λ-���λ�ã�����������Ҫ��
	pos_y_device_ready = 0x40,		//�豸��λ-����λ���豸�������Լ���ɺ����λ�ã�����������Ҫ��
	pos_y_device_lastpos = 0x50,		//�豸��λ-�ϴ�λ�ã�����������Ҫ��

	pos_zero_x_current_y = pos_x_device_min | pos_y_device_current,

	pos_unknown = 0x00		//δ֪��������ǰλ�á���pos_x/y_device_current������
};
#define POS_X_DEVICE_MASK	0x0F		//��λ��ʶ����-С������
#define POS_Y_DEVICE_MASK	0xF0		//��λ��ʶ����-˿��/��������

//���浱ǰλ��-��ʶ����
enum DEVICE_CURRENT_POSITION_STORE
{
	store_current_pos_x = 0x01,		//����С����X�򣩵����ǰλ��
	store_current_pos_y = 0x02, 		//����˿�ˣ�Y�򣩵����ǰλ��
	store_current_pos_xy = store_current_pos_x | store_current_pos_y		//����X��Y������ǰλ����Ϣ
};

enum COMM_RESULT
{
	comm_result_succeed = 0x00,	//ָ������ִ�����

	comm_result_para_invalid = 0x01,	//��������ʧ��
	comm_result_move_invalid = 0x02,	//С���ƶ������λ���ѵ����ޣ��ƶ�������Ч
	comm_result_download_invalid = 0x03, 	//дFLASH����δ�ɹ�

	comm_result_checking = 0x04,
	comm_result_other = 0xFF	//��������
};

//ָ��ִ��״̬��������������ָ�����ִ�й����еķ�����Ϣ��
//��λ��������Ϣ���壨MB��
enum FLATLASER_COMM_RESULT
{
	mb_comm_result_succeed = 0x00,	//ָ������ִ�����

	mb_comm_result_failed = 0x01,	//��λ���ּ�����ʱͨ��ʧ��
	mb_comm_result_offline = 0x02,	//��λ�������ѻ�״̬

	mb_comm_result_checking = 0x03,
	mb_comm_result_other = 0xFF	//��������
};

//ָ��ִ��״̬��������������ָ�����ִ�й����еķ�����Ϣ��
//��λ����GCB��������Ϣ����
//enum FLATLASER_COMM_RESULT_GCB
//{
//	gcb_comm_result_succeed	= 0x00,	//ָ������ִ�����
//
//	gcb_comm_result_failed	= 0x01,	//ָ���޷�����ִ��
//
//	gcb_comm_result_other		= 0xFF	//��������
//};


enum FLATLASER_ADDRESS_TYPE
{

	addr_none = 0x00,		//��ʼ����δ֪����
	addr_pc = 0x02,		//GCB������

	addr_ld = 0x03,		//LD���������ݰ�

	addr_lp_1 = 0x04,		//LP������������ 1
	addr_lp_2 = 0x05,		//LP������������ 2
	addr_lp_3 = 0x06,		//LP������������ 3
	addr_lp_4 = 0x07,		//LP������������ 4

	addr_ts = 0x08,		//TS��������
	addr_com = 0x09,		//COM��ͨ�Ű�

	address_pd = 0x10,		//PD�����ʼ���

	addr_servo_x = 0x11,		//SERVOX��С������ŷ�������
	addr_servo_y = 0x12,		//SERVOY����������ŷ�������

	addr_power = 0x13,		//POWER����Դ���ư�

	addr_rec_cloth = 0x14,         //REC���ղ���
	addr_sec_cloth = 0x15,         //SEC���Ͳ���
	addr_ink = 0x16,         //INK����ī��
	addr_correct = 0x17,         //CB����ƫ��
	addr_mb = 0x20,         //MB����������װ�
	addr_gcb = 0x21,         //GCB������������İ� 
	addr_ghb1 = 0x22,          //GHB1������������ͷ�� 1
	addr_ghb2 = 0x23,          //GHB2������������ͷ�� 2
	addr_ghb3 = 0x24,          //GHB3������������ͷ�� 3
	addr_ghb4 = 0x25,          //GHB4������������ͷ�� 4
	addr_ghb5 = 0x26,          //GHB5������������ͷ�� 5
	addr_ghb6 = 0x27,         //GHB6������������ͷ�� 6
	addr_ghb7 = 0x28,         //GHB7������������ͷ�� 7
	addr_ghb8 = 0x29,         //GHB8������������ͷ�� 8

	addr_raster = 0x20,         //��դ���ư�

	addr_servo_three_axis = 0x30,	//�����ŷ���

	addr_unknown = 0xFF		//δ֪
};

struct TIME_TABLE_ONLINE_PAMARATER	//�����
{
	WORD			m_wAecSUnit;		//����ÿ�����
	WORD			m_wDecSUnit;		//����ÿ�����
	WORD			m_wTimeUnit;		//��Сʱ�䵥Ԫ
	DWORD			m_dwAecNum;			//���ٸ���
	DWORD			m_dwDecNum;			//���ٸ���
	DWORD			m_dwEvenV;			//ƽ���ٶ�

	BYTE			m_bAecRecordNum;	//���Լ�¼һЩ�����������Ŀ	
	BYTE			m_bDecRecordNum;	//���Լ�¼һЩ�����������Ŀ

	CDWordArray		m_dwAecRecordArray;	//���Լ�¼һЩ���������
	CDWordArray		m_dwDecRecordArray;	//���Լ�¼һЩ���������
public:
	void Init();
};

struct TIME_TABLE_ONLINE
{
	TIME_TABLE_ONLINE_PAMARATER* m_parameterOnline;
	BYTE			m_bSpeedLevelOnline;		//�ٶȵ�λ
	CWordArray		m_wAecTimeArray;	//����ʱ��Word����
	CWordArray		m_wDecTimeArray;	//����ʱ��Word����
};

struct TIME_TABLE_OFFLINE_PAMARATER		//�����
{
	WORD			m_wAecSUnit;			//���ٶ�ÿ�����
	WORD			m_wFast2SlowSUnit;		//�쵽����ÿ�����
	WORD			m_wSlow2StopSUnit;		//����ͣ��ÿ�����
	WORD			m_wTimeUnit;			//��Сʱ�䵥Ԫ
	DWORD			m_dwAecNum;				//���ٸ���
	DWORD			m_dwFast2SlowNum;		//�쵽������
	DWORD			m_dwSlow2StopNum;		//����ͣ����
	DWORD			m_dwFastV;			//�����ٶ�
	DWORD			m_dwSlowV;			//�����ٶ�

	BYTE			m_bAecRecordNum;		//���Լ�¼һЩ�����������Ŀ
	BYTE			m_bFast2SlowRecordNum;	//���Լ�¼һЩ�����������Ŀ
	BYTE			m_bSlow2StopRecordNum;	//���Լ�¼һЩ�����������Ŀ

	CDWordArray		m_dwAecRecordArray;			//���Լ�¼һЩ���������
	CDWordArray		m_dwFast2SlowRecordArray;	//���Լ�¼һЩ���������
	CDWordArray		m_dwSlow2StopRecordArray;	//���Լ�¼һЩ���������
public:
	void Init();
};

struct TIME_TABLE_OFFLINE
{
	TIME_TABLE_OFFLINE_PAMARATER* m_parameterOffline;
	BYTE			m_bSpeedLevelOffline;		//�ٶȵ�λ
	CWordArray		m_wAecTimeArray;			//����ʱ��Word����
	CWordArray		m_wFast2SlowTimeArray;		//���٣��쵽����ʱ��Word����
	CWordArray		m_wSlow2StopTimeArray;		//���٣�����ͣ��ʱ��Word����
};

struct SF_BOARD_PARAMETER_X
{
	float m_fReducerX;				//X���ٻ�
	float m_fDollyPerimeter;		//С���ܳ�
	float m_fPulsesPerRevolution_x;	//ÿȦ����
};

struct SF_BOARD_PARAMETER_Y
{
	float m_fReducerY;				//Y���ٻ�
	float m_fDeferentPerimeter;		//�����ܳ�
	float m_fDeferentPitch;			//˿���ݾ�
	float m_fPulsesPerRevolution_y;	//ÿȦ����
};
struct SF_BOARD_PARAMETER_Z
{
	float m_fReducer_Z;				//Z���ٻ�
	float m_fDeferentPerimeter_Z;	//�����ܳ�
	float m_fDeferentPitch_Z;		//˿���ݾ�
	float m_fPulsesPerRevolution_Z;	//ÿȦ����
};

struct BOARD_CARD_INFO
{
	WORD m_wDeviceVersion;		//���Ӱ汾
	WORD m_wProtocolVersion;	//����汾
	DWORD m_dwSN;	//�忨����
	DWORD m_dwDate;	//��������
	DWORD m_dwProducerID;	//����Ա��
	DWORD m_dwCheckerID;	//����Ա
};

//struct MB_CLEAN_HEAD_PARAMETER
//{
//	BYTE m_bEnable;		
//	WORD m_wPass;
//	WORD m_wPurgeTime;
//	WORD m_wPurgeDelay;
//	BYTE m_bCleanTime;
//};

struct MB_PRINT_START_PARAMETER
{
	BYTE	m_bSpeedLevelX;
	BYTE	m_bSpeedLevelY;
	DWORD	m_dwPositionX;
	DWORD	m_dwPositionY;
	WORD	m_wPurgeTime;
	WORD	m_bSprayTime;
	WORD	m_wSprayFreq;
	WORD	m_bSprayInterval;
	BYTE	m_bPower[8];
	WORD	m_dwPulseWidth[8];
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	WORD	m_wResX;
	WORD	m_wResY;
	BYTE	m_bPrintWay;
};

struct MB_PRINT_LINE_PARAMETER
{
	DWORD	m_dwStartPos;
	DWORD	m_dwWidth;
	WORD	m_wFreqPscM;
	WORD	m_wFreqPscN;
	DWORD	m_dwFreqDistD1;
	DWORD	m_dwFreqDistD2;
	DWORD	m_dwFreqDistD3;
	DWORD	m_dwPulse;
	DWORD	m_dwRePulse;
	DWORD	m_dwYDist;
	BYTE	m_bAction;
	WORD	m_wThousandth;
	//BYTE	m_bSelfN;
	//BYTE	m_bSelfInfo[256];
	DWORD	m_dwPrintHeight;
};

struct RAS_PRINT_START_PARAMETER
{
	BYTE	m_bSpeedLevelX;
	BYTE	m_bSpeedLevelY;
	DWORD	m_dwPositionX;
	DWORD	m_dwPositionY;
	WORD	m_wPurgeTime;
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	WORD	m_wResX;
	WORD	m_wResY;
	BYTE	m_bPrintWay;
};

struct RAS_PRINT_LINE_PARAMETER
{
	DWORD	m_dwStartPos;
	DWORD	m_dwWidth;
	WORD	m_wFreq;
	DWORD	m_dwPulse;
	DWORD	m_dwRePulse;
	DWORD	m_dwYDist;
	BYTE	m_bAction;
	WORD	m_wThousandth;
	DWORD	m_dwPrintHeight;
};


//LD���ѹ����
struct LD_VOLTAGE
{
	BYTE  m_bFeedbackChannel;
	DWORD m_wPC;
	DWORD m_wLastPD;
	DWORD m_wNowPD;
	DWORD m_wLastLD;
	DWORD m_wNowLD;
};

//��Ϣ����״̬
enum PROTOCOL_QUERY_WAY
{
	protocol_queryway_failed = 0,	//ʧ��
	protocol_queryway_checking,			//��Ϣ������
	protocol_queryway_ok,				//�ɹ���MB������Ч��Ϣ��

	protocol_queryway_count
};
//�����趨״̬
enum PROTOCOL_CONFIG_WAY
{
	protocol_configway_failed = 0,	//ʧ��
	protocol_configway_checking,		//�����趨��
	protocol_configway_ok,				//�ɹ���MB������Ч��Ϣ��

	protocol_configway_count
};
//��ӡָ���״̬
enum PROTOCOL_PRINT_WAY
{
	protocol_printway_failed = 0,	//ʧ��
	protocol_printway_checking,			//ָ�����
	protocol_printway_ok,				//�ɹ���MB�ɹ�����������Чȷ����Ϣ��

	protocol_printway_count
};

enum FLATLASER_LD_COMM_RESULT
{
	ld_comm_result_succeed = 0x00,		//ָ������ִ�����

	ld_comm_result_failed = 0x01,		//��λ���ּ�����ʱͨ��ʧ��
	ld_comm_result_offline = 0x02,		//��λ�������ѻ�״̬
	ld_comm_result_power_invalid = 0x03, 	//���⹦�ʲ����
	ld_comm_result_config_invalid = 0x04, 	//�洢��Ԫ�޷���������Ч��
	ld_comm_result_config_invalidata = 0x05, 	//�洢��Ԫ������Ч������д��ʱ����

	ld_comm_result_checking = 0x06,

	ld_comm_result_other = 0xFF		//��������
};

//ϵͳ�쳣��Ϣ���壬������ϵͳ�����޷�άϵ������ӡ��������紥����ӡ��λ��ֹͣ��ӡ���̵ȣ�
//	���¸�״̬��λȡֵ��ͬһ״̬���пɰ�������쳣��Ϣ
enum MB_SYSTEM_STATUS
{
	mb_system_normal = 0x00,		//ȱʡ�����쳣״̬

	mb_system_ink = 0x01,		//Һλ��

	mb_system_unknown = 0x80,		//�����쳣״̬

};

//enum PROTOCOL_VERSION_INFO
//{
//	protocol_version_none		= 0x0000,			//��ʼ��
//
//	protocol_version_4001		= 0x4001,			// V40.01 ��16385��
//	protocol_version_4002		= 0x4002,			// V40.02 ��16386��
//                                                    //	1��protocol_command_print_line �������Ƿ񿪷� DPC ��ѡ�� (bDPCDisable)
//                                                    //	2������֡����ֵ��MB�������������ʧ��ģʽ��PC���շ���ֵʱ�ж���Ӧ��Ϣ��
//	protocol_version_4003		= 0x4003,			// V40.03  (16387)
//                                                    //  ���Ӵ�ӡʱ�˶�����������Ϣ
//
//	protocol_version_unknown	= 0xFFFF			//��Ч�汾��Ϣ
//};

#define _PLUGIN_MAX_COLOR_LIMIT_		20			//���֧�ֵ���ɫͨ����

#define _PLUGIN_MAX_COLOR_DEFAULT_		8			//Ĭ����ɫͨ����

#pragma pack(push)
#pragma pack(1)
//����λ�����صĸ��ֲ�����״̬����Ϣ
struct /*RP_EXT_CLASS*/ Protocol_Control_Statue
{
	// PC-->MB-->PC
	volatile BYTE	bCheckedWay;							//Check ��������״̬��PROTOCOL_QUERY_WAY

	WORD			wProtocolVersion;						//��λ��Э��汾��Ϣ��PROTOCOL_VERSION_INFO
	BYTE			bControlAmplitude[_PLUGIN_MAX_COLOR_LIMIT_];	//��ǰ����ͷ��ѹ��ֵ�趨
	BYTE			bControlPulsewidth[_PLUGIN_MAX_COLOR_LIMIT_];	//��ǰ����ͷ��ѹ�����趨
	WORD			wThicknessPluse;						//��ǰ�Ĳ�������趨ֵ��������
	SPRAYPARAM		sprayparam;								//ɢ�����
	CLEANHEADPARAM	cleanheadparam;							//��ӡ���Զ���ͷ��ϴ����

	MOTOPARAM_GENERAL	motoparam_general;
	MOTOPARAM_X			motoparam_x;
	MOTOPARAM_Y			motoparam_y;
	MOTOPARAM_Z			motoparam_z;

	volatile BYTE	bQueryWay_ProtocolVersion;				//��Ϣ����״̬-��λ��Э��汾��PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_ControlAmplitude;				//��Ϣ����״̬-��ǰ����ͷ��ѹ��ֵ�趨��PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_ControlPulsewidth;			//��Ϣ����״̬-��ǰ����ͷ��ѹ�����趨��PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_ControlThickness;				//��Ϣ����״̬-��ǰ�Ĳ�������趨��PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_sprayparam;					//��Ϣ����״̬-��ǰɢ������趨ֵ��PROTOCOL_QUERY_WAY
	volatile BYTE	bQueryWay_cleanheadparam;				//��Ϣ����״̬-��ǰ�Զ���ϴ�����趨ֵ��PROTOCOL_QUERY_WAY

	volatile BYTE	bQueryWay_motoparam_general;
	volatile BYTE	bQueryWay_motoparam_x;
	volatile BYTE	bQueryWay_motoparam_y;
	volatile BYTE	bQueryWay_motoparam_z;

	volatile BYTE	bConfigWay_ControlAmplitude;			//MB�����������趨״̬-��ǰ��ͷ��ѹ��ֵ�趨��PROTOCOL_CONFIG_WAY
	volatile BYTE	bConfigWay_ControlPulsewidth;			//MB�����������趨״̬-��ǰ��ͷ��ѹ�����趨��PROTOCOL_CONFIG_WAY
	volatile BYTE	bConfigWay_ControlThickness;			//MB�����������趨״̬-��ǰ��������趨��PROTOCOL_CONFIG_WAY
	volatile BYTE	bConfigWay_sprayparam;					//MB�����������趨״̬-��ǰɢ������趨��PROTOCOL_CONFIG_WAY
	volatile BYTE	bConfigWay_cleanheadparam;				//MB�����������趨״̬-��ǰ�Զ���ϴ�����趨��PROTOCOL_CONFIG_WAY

	volatile BYTE	bConfigWay_motoparam_general;
	volatile BYTE	bConfigWay_motoparam_x;
	volatile BYTE	bConfigWay_motoparam_y;
	volatile BYTE	bConfigWay_motoparam_z;

	volatile BYTE	bPrintWay_printstart;					//MB��������ӡָ���״̬-��ӡ׼����PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_printline;					//MB��������ӡָ���״̬-��ӡһ�У�PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_printblock;					//MB��������ӡָ���״̬-��ӡһ�飬PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_printend;						//MB��������ӡָ���״̬-��ӡ������PROTOCOL_PRINT_WAY

	volatile BYTE	bPrintWay_pc_reset;						//MB��������ӡָ���״̬-PC��λ��PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_pc_pause;						//MB��������ӡָ���״̬-PC��ͣ��PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_pc_resume;					//MB��������ӡָ���״̬-PC������PROTOCOL_PRINT_WAY

	volatile BYTE	bPrintWay_print_back2origin;			//MB��������ӡָ���״̬-�ص���ӡԭ�㣬PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_print_cleanhead;				//MB��������ӡָ���״̬-ִ��һ���Զ���ϴ������PROTOCOL_PRINT_WAY
	volatile BYTE	bPrintWay_print_spray;					//MB��������ӡָ���״̬-ִ��һ��ɢ�������PROTOCOL_PRINT_WAY

	// MB-->PC-->MB
	volatile BYTE	bSignal_mb_reset;						//���յ�MB��λָ��
	volatile BYTE	bSignal_mb_pause;						//���յ�MB��ָͣ��
	volatile BYTE	bSignal_mb_resume;						//���յ�MB����ָ��
	volatile BYTE	bSignal_mb_syserror;					//���յ�MBϵͳ����ָ��
	volatile BYTE	bSignal_mb_sysstatus;					//���յ�MBϵͳ�쳣ָ��

	volatile BYTE	bSignal_mb_cleanhead;					//���յ�MB��ͷ��ϴָ��

	//SF
	volatile BYTE	bQueryWay_SF_GetExistentSpeedlevel;
	BYTE bExistSpeedLevel;
	volatile BYTE	bConfigWay_SF_DownloadOnlineTimeTableParameter;
	volatile BYTE	bConfigWay_SF_DownloadTimeTableValue;
	volatile BYTE	bConfigWay_SF_DownloadOfflineTimeTableParameter;
	volatile BYTE	bQueryWay_SF_GetOnlineTimeTableParameter;
	volatile BYTE	bQueryWay_SF_GetOfflineTimeTableParameter;
	volatile BYTE	bPrintWay_SF_CountFullPulse;
	volatile BYTE	bPrintWay_SF_MotorMoveRelative;
	volatile BYTE	bPrintWay_SF_MotorMoveAbsolute;
	volatile BYTE	bQueryWay_SF_MotorGetPosition;
	DWORD dwCurrentPosition;
	volatile BYTE	bPrintWay_SF_MotorMoveSpecialPosition;
	volatile BYTE	bPrintWay_SF_MotorMoveStop;
	volatile BYTE	bPrintWay_SF_MotorMove;
	volatile BYTE	bQueryWay_SF_GetFullPulse;
	DWORD dwFullPulse;
	volatile BYTE	bQueryWay_SF_GetSpecialPosition;
	DWORD dwSpecialPosition;
	volatile BYTE	bConfigWay_SF_SetSpecialPosition;
	volatile BYTE	bConfigWay_SF_DeleteAllTimeTableData;

	volatile BYTE	bConfigWay_SF_SetFirmParameterX;
	volatile BYTE	bConfigWay_SF_SetFirmParameterY;
	volatile BYTE	bConfigWay_SF_SetFirmParameterZ;

	volatile BYTE	bQueryWay_SF_GetFirmParameterX;
	volatile BYTE	bQueryWay_SF_GetFirmParameterY;
	volatile BYTE	bQueryWay_SF_GetFirmParameterZ;

	SF_BOARD_PARAMETER_X sf_board_parameter_x;
	SF_BOARD_PARAMETER_Y sf_board_parameter_y;
	SF_BOARD_PARAMETER_Z sf_board_parameter_z;

	volatile BYTE	bQueryWay_SF_GetTimeTableVitalParameter;
	DWORD dwVitalParaAecPulse;
	DWORD dwVitalParaDecPulse;
	DWORD dwVitalEvenT;

	volatile BYTE	bPrintWay_SF_MotorXY_MoveRelative;
	volatile BYTE	bPrintWay_SF_MotorXY_MoveAbsolute;
	volatile BYTE	bPrintWay_SF_MotorXY_MoveSpecialPosition;
	volatile BYTE	bPrintWay_SF_MotorXY_PrintLine;

	//MB
	volatile BYTE	bConfigWay_MB_OnlineState;

	//volatile BYTE	bConfigWay_MB_CleanHead;
	//volatile BYTE	bQueryWay_MB_CleanHead;
	//volatile BYTE	bPrintWay_MB_CleanHead;
	//MB_CLEAN_HEAD_PARAMETER mb_clean_head_parameter;
	//
	//volatile BYTE	bConfigWay_MB_Thickness;
	//volatile BYTE	bQueryWay_MB_Thickness;
	//WORD wThickness;
	volatile BYTE	bPrintWay_MB_PrintStart;
	volatile BYTE	bPrintWay_MB_PrintLine;
	volatile BYTE	bPrintWay_MB_PrintEnd;
	volatile BYTE	bPrintWay_MB_PrintPause;
	volatile BYTE	bPrintWay_MB_PrintResume;
	volatile BYTE	bPrintWay_MB_PrintStop;
	volatile BYTE	bPrintWay_MB_MoveAbsolutePosition;
	volatile BYTE	bPrintWay_MB_MoveRelativePosition;
	volatile BYTE	bPrintWay_MB_Move2SpecialPosition;
	volatile BYTE	bPrintWay_MB_StoreCurrentPosition;

	//�忨
	volatile BYTE	bConfigWay_BoardCard_SetInfo;
	volatile BYTE	bQueryWay_BoardCard_GetInfo;
	BOARD_CARD_INFO	board_card_info;
	volatile BYTE	bConfigWay_BoardCard_SetAddr;
	volatile BYTE	bQueryWay_BoardCard_GetAddr;
	BYTE	bBoardCardAddr;

	//��ī
	volatile BYTE	bConfigWay_Ink_Check485;
	volatile BYTE	bQueryWay_Ink_CheckState;
	BYTE bInkState;
	WORD wInkState;
	volatile BYTE	bConfigWay_Ink_Force;
	volatile BYTE	bConfigWay_Ink_Press;
	volatile BYTE	bConfigWay_Ink_Scrape;
	volatile BYTE	bConfigWay_Ink_Moisturize;
	volatile BYTE	bConfigWay_Ink_CycleMotor;

	//�ղ�
	volatile BYTE	bQueryWay_RC_CheckState;
	BYTE bRcState;
	volatile BYTE	bConfigWay_RC_Pass;
	volatile BYTE	bConfigWay_RC_PrintStop;
	volatile BYTE	bConfigWay_RC_PrintOver;

	//�Ͳ�
	volatile BYTE	bQueryWay_SC_CheckState;
	//BYTE bScState0;
	//BYTE bScState1;
	volatile BYTE	bConfigWay_SC_BandDry;
	volatile BYTE	bConfigWay_SC_PreDry;
	volatile BYTE	bConfigWay_SC_BandWash;
	volatile BYTE	bConfigWay_SC_PressRoller;
	volatile BYTE	bConfigWay_SC_Motor;
	volatile BYTE	bConfigWay_SC_Stop;
	volatile BYTE	bConfigWay_SC_CancleWrongLabel;

	//��ƫ
	volatile BYTE	bConfigWay_CB_AutoCorrect;
	volatile BYTE	bConfigWay_CB_ManualCorrect;
	volatile BYTE	bConfigWay_CB_ManualCorrectStop;

	//COM
	volatile BYTE	bQueryWay_COM_GetBoardCondition;
	WORD wComBoardCondition;
	volatile BYTE	bPrintWay_COM_Motor_MoveRelative;

	//RAS
	volatile BYTE	bPrintWay_RAS_PrintStart;
	volatile BYTE	bPrintWay_RAS_PrintLine;
	volatile BYTE	bPrintWay_RAS_PrintEnd;
	volatile BYTE	bPrintWay_RAS_Select;

	int nSpeedLevelNum;
	OnLinePara onlinePara[10];
	PositionPara positionPara;

	volatile BYTE bPrintWay_RAS_GetSpeed;
	volatile BYTE bPrintWay_RAS_GetPosition;
	volatile BYTE bPrintWay_RAS_PrintStop;
	volatile BYTE bPrintWay_RAS_Move2Position;

	DWORD dwErrorCode;
	DWORD dwException;

	//�ղ���Э��
	volatile BYTE	bPrintWay_RC_ComState;

	//�Ͳ���Э��
	volatile BYTE	bPrintWay_SC_ComState;
	BYTE bScState0;
	BYTE bScState1;
	DWORD dwTemperature;

	//LD
	WORD			wLDVersion;
	BYTE			bChannelNum;
	BYTE			bPowerChannel[ld_power_channel_max];
	WORD			wVolValue[ld_power_channel_max];
	BYTE			bPowerValue[ld_power_channel_max];
	WORD			wFeedBackCheckVol;

	//LD send
	BYTE			bFeedBackChannel;
	WORD			wFeedBackVol;
	int				nProgress;

	volatile BYTE	bQueryWay_ld_all_power;

	volatile BYTE	bConfigWay_ld_power_off;
	volatile BYTE	bConfigWay_ld_check_power;
	volatile BYTE	bConfigWay_ld_all_power;

	volatile BYTE	bPrintWay_ld_voltage;		//64·����
	volatile BYTE	bPrintWay_ld_check_power;

	volatile BYTE	bConfigWay_ld_write_config;
	volatile BYTE	bConfigWay_ld_write_data;
	volatile BYTE	bConfigWay_ld_read_config;

	volatile BYTE	bConfigWay_ld_light_on;
	volatile BYTE	bConfigWay_ld_light_off;
	volatile BYTE	bConfigWay_ld_light_on_time;

	volatile BYTE	bQueryWay_ld_get_current_power;

	//map data
	volatile BYTE	bConfigWay_map_data;
	volatile BYTE	bQueryWay_map_data;
	BYTE			bMapNum;
	WORD			wMapVolValue[ld_map_data_max];
	BYTE			bMapPowerValue[ld_map_data_max];

	//LD
	LD_VOLTAGE		ldVoltage;  //64·����

public:
	//	bAll :[in]�Ƿ�������в�����FALSEʱ����ʼ�� wProtocolVersion
	void Initialize(BOOL bAll);
};
#pragma pack(pop)

enum SERVO_THREE_AXIS_WAY
{
	sv_three_axis_way_x = 0x01,		//X��
	sv_three_axis_way_y = 0x02,		//Y��
	sv_three_axis_way_z = 0x03,		//Z��

	sv_three_axis_way_unknown = 0x00		//δ֪
};
enum SF_MOTOR_WAY
{
	motor_way_none = 0,

	motor_way_x = 1,
	motor_way_y = 2,

	motor_way_3Axis_x,
	motor_way_3Axis_y,
	motor_way_3Axis_z,

	motor_way_count
};

#include "../SocketPort/CBSocketPort.h"
class /*RP_EXT_CLASS*/ CCommunicationPort : public rpComBase::CComBase
{
public:
	CCommunicationPort(void);
	virtual ~CCommunicationPort(void);

public:
	//��ӡ�豸����ģ�飨�������Ϣ
	//	infoProc		:[in]��ȡ�����Ϣ�ĺ�����ַ
	//	bChannels		:[in]�豸֧�ֵ���ɫͨ������ С�ڵ��� _PLUGIN_MAX_COLOR_LIMIT_
	//void SetPrinterPluginInfo(IS_PRINTER_PLUGIN_PROC infoProc, BYTE bChannels);

	// port initialisation
	//	nBufferSize :[in]��ʼ�����ջ�������С
	//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
	virtual BOOL Open(UINT portnr, DWORD dwBaud, int nBufferSize, int nGrowSize);
	virtual BOOL Close(BOOL bReleaseBuffer);

	virtual BOOL IsOpen() const;

public:
	CWnd* m_pComWnd;

	CString m_strReciveData;
	CString m_strSendData;
	//DWORD m_dwSendThreadID;
	//DWORD m_dwReciveThreadID;

public:
	//ֱ�ӷ��Ͳ���ָ�һ����� ReceiveData ������ʹ��
	virtual BOOL Commu_SendMsg_Direct(const BYTE* snd_msg, int msg_len);
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
	virtual int SendMsg_handshake(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, int sendmsg_level);
	virtual int SendMsg_handshake0(BYTE *snd_msg, int s_msg_len, int timeout, int time_repeatsend, int sendmsg_level, BOOL bFlag);
	//����ָ�����
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

	//����ָ���״̬��Ϣ
	virtual void ResetCommu_Status_Flags();

	//��ʼ����ز���
	virtual void Init(BOOL bReAllocBuffer);

public:
	//�Ƿ����Debug��Ϣ��д�ļ���
	BOOL debug_IsWriteSendInfo() const;
	BOOL debug_IsWriteReceiveInfo() const;
	void debug_WriteSendInfo(BOOL bWrite);
	void debug_WriteReceiveInfo(BOOL bWrite);

protected:
	//�Ƿ����Debug��Ϣ��д�ļ���
	BOOL m_debug_bWriteSendInfo;
	BOOL m_debug_bWriteReceiveInfo;

	//protected:
public:
	Protocol_Control_Statue		m_protocolstatus;		//����λ�����صĸ��ֲ�����״̬����Ϣ-��ǰ

	//��ӡ�豸����ģ�飨�������Ϣ
	//IS_PRINTER_PLUGIN_PROC		m_printer_plugin_proc;
	//�豸����ɫͨ������ С�ڵ��� _PLUGIN_MAX_COLOR_LIMIT_
	BYTE						m_printer_plugin_bChannels;
protected:
	//���� CRC ֵ
	static BYTE CalcCRC(const BYTE *pBuffer, DWORD dwSize);
	// ����֡������������һ����Ч֡��
	//���أ�������Ļ������ݸ���
	DWORD DataFrameAnalyze(const BYTE* pBuffer, DWORD dwBufferSize);
	DWORD DataFrameAnalyze_PrintMode(const BYTE* pBuffer, DWORD dwBufferSize);
	//ת��ԭ��λ�ò���ֵ����ӡ������ --> ͨѶЭ�����
	//	bOriginPosition		:[in]ָ��ԭ��λ�ã�PRINTER::ORIGIN_POSITION
	static BYTE TransOriginPosition(BYTE bOriginPosition);
	//������ʾ����Ҫ���ط�ָ��
	//	resultHandshake		: [in]SENDMSG_STATUS��SendMsg_handshakeָ��ͷ���״̬
	//����ֵ��
	//	==0	�������ط�ָ��
	//	> 0	�������˳�
	//	< 0	���������ط�ָ��
	static int SendMsg_handshake_Failed_and_Resend(int resultHandshake);

protected:
	//������ ReceivedDataEx(...)����
	BYTE	m_receivedataex_bReturnFlag[0xFF];		//�������豸�����ذ壩
	int		m_receivedataex_nReturnFlagLen;			//������Ϣ����

public:
	// ��װָ����Ϣ������֡��
	//	pDstBuffer :[out]��װ�������֡����������������С >= pt_data_len_max
	static int dataframe_transdata(BYTE* pDstBuffer, BYTE bCmd, const BYTE* pCmdData, BYTE bCmdSize);

	static int dataframe_transdata0(BYTE* pDstBuffer, BYTE bSrcAddress, BYTE bDstAddress, BYTE bCmd, BYTE bCmdCount, const BYTE* pCmdData, BYTE bCmdSize);

	//�õ�ǰ��λ�����صĸ��ֲ�����״̬����Ϣ
	const Protocol_Control_Statue *GetControlStatus() const;

	//=================�������=================

	// ����ָ��-�������״̬��PC<-->MBͨѶ�Ƿ�������
	BOOL sendorder_check(int time_check);
	// ����ָ��-��ѯ MB Э��汾
	BOOL sendorder_getverion(int time_check);

	//=================��ѯ=================
	// ע��
	//		���²�ѯ���ָ��� time_check < 0 ʱ��ʾָ��ͳɹ����������أ�
	//		����ⷴ�������Ӳ������ֵ��ʹ�� sml_normal ģʽ���ͣ�
	//
	// ����ָ��-��ѯ��ǰ��ѹ�趨ֵ
	BOOL sendorder_query_controlamplitude(int time_check);
	// ����ָ��-��ѯ��ǰ�����趨ֵ
	BOOL sendorder_query_controlpulsewidth(int time_check);
	// ����ָ��-��ѯ��ǰ��������趨ֵ
	BOOL sendorder_query_controlthickness(int time_check);

	// ����ָ��-��ѯ��ǰɢ�����ֵ
	BOOL sendorder_query_sprayparam(int time_check);
	// ����ָ��-��ѯ��ǰ��ϴ����ֵ
	BOOL sendorder_query_cleanheadparam(int time_check);

	// ����ָ��-�˶�-��ѯ-�������ò���
	BOOL sendorder_query_moto_general(int time_check);
	// ����ָ��-�˶�-��ѯ-X�����ò���
	BOOL sendorder_query_moto_x(int time_check);
	// ����ָ��-�˶�-��ѯ-Y�����ò���
	BOOL sendorder_query_moto_y(int time_check);
	// ����ָ��-�˶�-��ѯ-Z�����ò���
	BOOL sendorder_query_moto_z(int time_check);

	//=================����=================
	// ����ָ��-���õ�ѹ����ֵ
	//	sizeof (...) >= 8
	BOOL sendorder_set_controlamplitude(const BYTE *pbControlAmplitude, int time_check);
	// ����ָ��-�����������ֵ
	//	sizeof (...) >= 8
	BOOL sendorder_set_controlpulsewidth(const BYTE *pbControlPulsewidth, int time_check);
	// ����ָ��-���õ�ǰ��������趨ֵ
	//	nThickness	:�������������ֵ
	//	bTestMode	:�Ƿ���Ҫ��һ�β�����Զ���
	BOOL sendorder_set_controlthickness(int nThickness, BOOL bTestMode, int time_check);

	// ����ָ��-����ɢ�����ֵ
	BOOL sendorder_set_sprayparam(const SPRAYPARAM &sprayparam, int time_check);
	// ����ָ��-������ϴ����ֵ
	BOOL sendorder_set_cleanheadparam(const CLEANHEADPARAM &cleanheadparam, int time_check);

	// ����ָ��-�˶�-�趨-�������ò���
	BOOL sendorder_set_moto_general(const MOTOPARAM_GENERAL &param, int time_check);
	// ����ָ��-�˶�-�趨-X�����ò���
	BOOL sendorder_set_moto_x(const MOTOPARAM_X &param, int time_check);
	// ����ָ��-�˶�-�趨-Y�����ò���
	BOOL sendorder_set_moto_y(const MOTOPARAM_Y &param, int time_check);
	// ����ָ��-�˶�-�趨-Z�����ò���
	BOOL sendorder_set_moto_z(const MOTOPARAM_Z &param, int time_check);

	//=================��ӡ=================

	// ����ָ��-��ӡ׼��
	BOOL sendorder_print_start(const ky_print_info *pPrintInfo, int time_check);
	// ����ָ��-��ӡ׼��
#pragma pack(push)
#pragma pack(1)				//�ֽڶ���
	struct PRINTLINE_INFO
	{
		BYTE	start_io;
		WORD	start_pos;
		DWORD	width;
		BYTE	end_io;
		BYTE	re_start_io;
		WORD	re_start_pos;
		DWORD	re_width;
		BYTE	re_end_io;
		DWORD	ydist;

		BYTE	bDPCDisable;	//�Ƿ�ر�DPC���
	};
#pragma pack(pop)
	//	wProtocolVersion	:[in]//��λ��Э��汾��Ϣ��PROTOCOL_VERSION_INFO��protocol_version_none ��ʾ����Ӧ
	BOOL sendorder_print_line(const PRINTLINE_INFO &lineInfo, WORD wProtocolVersion, int time_check);

	// ����ָ��-�ص���ӡԭ��
	//	bOriginPosition		:[in]ָ��ԭ��λ�ã�PRINTER::ORIGIN_POSITION
	BOOL sendorder_print_back2origin(BYTE bOriginPosition, int time_check);
	// ����ָ��-ִ��һ���Զ���ϴ����
	//	nPurgeTime	:[in]ѹīʱ����ms��
	//	nPurgeDelay	:[in]ѹī����ʱ��ms��
	//	nCleanTime	:[in]�ε�����
	BOOL sendorder_print_cleanhead(int nPurgeTime, int nPurgeDelay, BYTE bCleanTime, int time_check);
	// ����ָ��-ִ��һ��ɢ�����
	//	bSprayTime	:[in]ɢ�����
	BOOL sendorder_print_spray(BYTE bSprayTime, int time_check);

	// ����ָ��-��ӡ����
	//	bOriginPosition		:[in]ָ��ԭ��λ�ã�PRINTER::ORIGIN_POSITION
	BOOL sendorder_print_end(BYTE bOriginPosition, int time_check);

	//=================�쳣=================
	// ����ָ��-��λ
	BOOL sendorder_pc_reset(int time_check);
	// ����ָ��-��ͣ
	BOOL sendorder_pc_pause(int time_check);
	// ����ָ��-����
	BOOL sendorder_pc_resume(int time_check);

	//================ȷ���źŷ���===========

	// ����ȷ��ָ��-MB��λ
	BOOL sendorder_confirmation_mb_reset(void);
	// ����ȷ��ָ��-MB��ͣ
	BOOL sendorder_confirmation_mb_pause(void);
	// ����ȷ��ָ��-MB����
	BOOL sendorder_confirmation_mb_resume(void);
	// ����ȷ��ָ��-MBϵͳ����
	BOOL sendorder_confirmation_mb_syserror(void);
	// ����ȷ��ָ��-MBϵͳ�쳣
	BOOL sendorder_confirmation_mb_status(void);
	// ����ȷ��ָ��-MB��ͷ��ϴ
	BOOL sendorder_confirmation_mb_cleanhead(void);

public:
	//�жϵ�ǰ����ָ�����״̬������
	void sendorder_handshake_Break(BOOL bBreak);
	//��ǰ�Ƿ��ڷ�����Ϣʱ��ѭ���ȴ���
	bool sendorder_handshake_isinloop() const;

	CWnd* m_pOnlineTimeTableDlg;
	CWnd* m_pOfflineTimeTableDlg;

	//SF
	//	bMotoWay :[in]SF_MOTOR_WAY
	BOOL servo_get_existent_speedlevel(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_set_online_timetable_paramater(BYTE bMotoWay, const TIME_TABLE_ONLINE &timetableOnline, int time_check, BOOL bFlag);
	BOOL servo_set_online_timetable_timevalue(BYTE bMotoWay, const TIME_TABLE_ONLINE &timetableOnline, int time_check, BOOL bFlag);

	BOOL servo_set_offline_timetable_paramater(BYTE bMotoWay, const TIME_TABLE_OFFLINE &timetableOffline, int time_check, BOOL bFlag);
	BOOL servo_set_offline_timetable_timevalue(BYTE bMotoWay, const TIME_TABLE_OFFLINE &timetableOffline, int time_check, BOOL bFlag);

	TIME_TABLE_ONLINE_PAMARATER m_parameterOnline;
	TIME_TABLE_OFFLINE_PAMARATER m_parameterOffline;

	BOOL servo_get_online_timetable_paramater(BYTE bMotoWay, BYTE bSpeedLevel, int time_check, BOOL bFlag);
	BOOL servo_get_offline_timetable_paramater(BYTE bMotoWay, BYTE bSpeedLevel, int time_check, BOOL bFlag);

	BOOL servo_count_fullpulse(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_motor_move_relative(BYTE bMotoWay, BYTE bSpeedType, DWORD dwPulse, DWORD dwRePulse, int time_check, BOOL bFlag);
	BOOL servo_motor_move_absolute(BYTE bMotoWay, BYTE bSpeedType, DWORD dwPulse, int time_check, BOOL bFlag);

	BOOL servo_motor_get_position(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_motor_move_special_position(BYTE bMotoWay, BYTE bSpeedType, BYTE bSpecialPosition, int time_check, BOOL bFlag);
	BOOL servo_motor_move_stop(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_motor_move(BYTE bMotoWay, BYTE bMoveType, int time_check, BOOL bFlag);

	BOOL servo_motor_xy_move_relative(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD pulseY, int time_check, BOOL bFlag);
	BOOL servo_motor_xy_move_absolute(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD pulseY, int time_check, BOOL bFlag);
	BOOL servo_motor_xy_move_special_position(BYTE bSpeedTypeX, BYTE bSpeedTypeY, BYTE bPosX, BYTE bPosY, int time_check, BOOL bFlag);
	BOOL servo_motor_xy_print_line(BYTE bSpeedTypeX, BYTE bSpeedTypeY, DWORD pulseX, DWORD re_pulseX, DWORD yDist, int time_check, BOOL bFlag);

	BOOL servo_get_fullpulse(BYTE bMotoWay, int time_check, BOOL bFlag);
	BOOL servo_get_special_position(BYTE bMotoWay, BYTE bSpecialPosition, int time_check, BOOL bFlag);
	BOOL servo_set_special_position(BYTE bMotoWay, BYTE bSpecialPosition, DWORD dwSpecialPosition, int time_check, BOOL bFlag);
	BOOL servo_delete_all_timetable_data(BYTE bMotoWay, int time_check, BOOL bFlag);

	BOOL servo_set_board_parameter_x(SF_BOARD_PARAMETER_X sf_board_parameter_x, int time_check, BOOL bFlag, BOOL bThreeAxisMotor);
	BOOL servo_set_board_parameter_y(SF_BOARD_PARAMETER_Y sf_board_parameter_y, int time_check, BOOL bFlag, BOOL bThreeAxisMotor);
	BOOL servo_set_board_parameter_z(SF_BOARD_PARAMETER_Z sf_board_parameter_z, int time_check, BOOL bFlag);

	BOOL servo_get_board_parameter_x(int time_check, BOOL bFlag, BOOL bThreeAxisMotor);
	BOOL servo_get_board_parameter_y(int time_check, BOOL bFlag, BOOL bThreeAxisMotor);
	BOOL servo_get_board_parameter_z(int time_check, BOOL bFlag);

	BOOL servo_get_timetable_vital_parameter(BYTE bMotoWay, BYTE bSpeedLevel, int time_check, BOOL bFlag);


	//MB
	//BOOL mb_set_online_state(BYTE bOnlineState,int time_check,BOOL bFlag);
	BOOL mb_print_start(MB_PRINT_START_PARAMETER mb_print_start_parameter, int time_check, BOOL bFlag);
	BOOL mb_print_line(MB_PRINT_LINE_PARAMETER mb_print_line_parameter, int time_check, BOOL bFlag);
	BOOL mb_print_end(BYTE bPosition, int time_check, BOOL bFlag);
	BOOL mb_print_pause(int time_check, BOOL bFlag);
	BOOL mb_print_resume(int time_check, BOOL bFlag);
	BOOL mb_print_stop(int time_check, BOOL bFlag);
	BOOL mb_move_absolute_position(DWORD dwPositionX, DWORD dwPositionY, int time_check, BOOL bFlag);
	BOOL mb_move_relative_position(DWORD dwPulseX, DWORD dwPulseY, int time_check, BOOL bFlag);
	BOOL mb_move_to_special_position(BYTE bSpecialPosition, int time_check, BOOL bFlag);
	BOOL mb_store_current_position(BYTE bCurrentPosition, int time_check, BOOL bFlag);
	//BOOL mb_set_clean_head_parameter(MB_CLEAN_HEAD_PARAMETER mb_clean_head_parameter,int time_check,BOOL bFlag);
	//BOOL mb_get_clean_head_parameter(int time_check,BOOL bFlag);
	//BOOL mb_print_clean_head(MB_CLEAN_HEAD_PARAMETER mb_clean_head_parameter,int time_check,BOOL bFlag);

	BOOL mb_confirmation_online_state(void);

	//�忨
	BOOL board_card_set_info(BYTE bDstAddr, BOARD_CARD_INFO board_card_info, int time_check, BOOL bFlag);
	BOOL board_card_get_info(BYTE bDstAddr, int time_check, BOOL bFlag);
	BOOL board_card_set_addr(DWORD dwSN, BYTE bAddr, int time_check, BOOL bFlag);
	BOOL board_card_get_addr(DWORD dwSN, int time_check, BOOL bFlag);

	//��ī
	BOOL ink_check_485(int time_check, BOOL bFlag);
	BOOL ink_check_state(int time_check, BOOL bFlag);
	BOOL ink_force(WORD wForce, int time_check, BOOL bFlag);
	BOOL ink_press(WORD wPress, WORD wPressTime, int time_check, BOOL bFlag);
	BOOL ink_scrape(int time_check, BOOL bFlag);
	BOOL ink_moisturize(BYTE bMoisturize, int time_check, BOOL bFlag);
	BOOL ink_cycle_motor(BYTE bOpen, int time_check, BOOL bFlag);

	//�ղ�
	BOOL rc_check_state(int time_check, BOOL bFlag);
	BOOL rc_pass(BYTE bPass, DWORD dwPass, int time_check, BOOL bFlag);
	BOOL rc_print_stop(BYTE bPrintStop, int time_check, BOOL bFlag);
	BOOL rc_print_over(int time_check, BOOL bFlag);

	//�Ͳ�
	BOOL sc_check_state(int time_check, BOOL bFlag);
	BOOL sc_band_dry(BYTE bBandDry, WORD wBandDry, int time_check, BOOL bFlag);
	BOOL sc_pre_dry(BYTE bPreDry, WORD wPreDry, int time_check, BOOL bFlag);
	BOOL sc_band_wash(BYTE bOpen, int time_check, BOOL bFlag);
	BOOL sc_press_roller(BYTE bOpen, int time_check, BOOL bFlag);
	BOOL sc_motor(BYTE bOpen, int time_check, BOOL bFlag);
	BOOL sc_stop(int time_check, BOOL bFlag);
	BOOL sc_cancel_wrong_label(int time_check, BOOL bFlag);

	//��ƫ
	BOOL cb_auto_correct(DWORD dwPulse, int time_check, BOOL bFlag);
	BOOL cb_manual_correct(BYTE bType, DWORD dwPulse, int time_check, BOOL bFlag);
	BOOL cb_manual_correct_stop(int time_check, BOOL bFlag);

	//COM
	BOOL com_motor_move_relative(BYTE bMotoWay, BYTE bSpeedType, DWORD dwPulse, DWORD dwRePulse, int time_check, BOOL bFlag);
	BOOL com_get_board_condition(int time_check, BOOL bFlag);

	//RAS
	BOOL ras_print_start(RAS_PRINT_START_PARAMETER ras_print_start_parameter, int time_check, BOOL bFlag);
	BOOL ras_print_line(RAS_PRINT_LINE_PARAMETER ras_print_line_parameter, int time_check, BOOL bFlag);
	BOOL ras_print_end(BYTE bPosition, int time_check, BOOL bFlag);
	BOOL ras_select(BYTE bSelect, int time_check, BOOL bFlag);
	BOOL ras_confirmation(BYTE bCmd);
	BOOL ras_get_speed(BYTE bMotoDirection, int time_check, BOOL bFlag);
	BOOL ras_get_position(int time_check, BOOL bFlag);
	BOOL ras_print_stop(int time_check, BOOL bFlag);
	BOOL ras_move2_position(DWORD x, DWORD y, int time_check, BOOL bFlag);

	CWnd* m_pRasDlg;
	int m_nProtocolMode; //����RAS���ӡģʽ

	//���ղ���
	BOOL rc_servo_motor_move_distance(BYTE bSpeedType, DWORD dwPulse, int time_check, BOOL bFlag);
	BOOL rc_up_down_motor_move(BYTE bType, int time_check, BOOL bFlag);
	BOOL rc_servo_motor_move(BYTE bSpeedType, int time_check, BOOL bFlag);
	BOOL rc_heater_con(BYTE bType, int time_check, BOOL bFlag);
	BOOL rc_cloth_motor_con(BYTE bType, int time_check, BOOL bFlag);
	BOOL rc_up_down_motor_reset(int time_check, BOOL bFlag);

	//���Ͳ���
	BOOL sc_new_sendcloth_check_state(int time_check, BOOL bFlag);
	BOOL sc_new_band_dry(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_band_wash(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_pre_dry(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_dd_band_wash(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_get_temperature(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_press_cloth(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_cloth_roller(BYTE bType, DWORD dwType, int time_check, BOOL bFlag);
	BOOL sc_new_cancle_wrong_lable(int time_check, BOOL bFlag);
	BOOL sc_new_manual_correct(BYTE bType, int time_check, BOOL bFlag);
	BOOL sc_new_manual_correct_stop(int time_check, BOOL bFlag);

	//LD
	CWnd* m_pLDWnd;

	BOOL ld_power_off(int time_check, BOOL bFlag);

	BOOL ld_set_check_power(const BYTE bPowerChannel, const WORD wVolValue, int time_check, BOOL bFlag);
	BOOL ld_set_laser_test_power(const BYTE bPowerChannel, const WORD wVolValue, int time_check, BOOL bFlag);
	BOOL ld_set_pc_power(const BYTE bChannel, WORD wVoltage_PC, int time_check, BOOL bFlag);
	BOOL ld_set_ld_power(const BYTE bChannel, WORD wVoltage_LD, int time_check, BOOL bFlag);
	BOOL ld_get_last_power(const BYTE bChannel, int time_check, BOOL bFlag);
	BOOL ld_get_now_power(const BYTE bChannel, int time_check, BOOL bFlag);
	BOOL ld_correct_power(const BYTE bChannel, int time_check, BOOL bFlag);

	//ӳ���
	BOOL ld_set_map_data(const WORD* wMapVolValue, const BYTE* bMapPowerValue, BYTE bMapNum, int time_check, BOOL bFlag);
	BOOL ld_get_map_data(int time_check, BOOL bFlag);

	BOOL ld_set_light_on(const DWORD* dwLightData, int time_check, BOOL bFlag);
	BOOL ld_set_light_off(const DWORD* dwLightData, int time_check, BOOL bFlag);
	BOOL ld_set_light_on_time(const BYTE bChannel, const DWORD dwContinueTime, int time_check, BOOL bFlag);

	//����ȷ��ָ��-LD�м���̷���
	BOOL ld_confirmation_set_power_status(void);

	//���²���/������� TCP ��װͨѶ
public:
	CCBSocketPort_Control	m_socket_Control;
	CCBSocketPort_Data		m_socket_Data;
	BOOL					m_socket_bEnable;
	void					*m_socket_pThread;
private:
	//���������߳�
	bool socket_thread_create();
	//�رմ����߳�
	bool socket_thread_close();
	//�ͷŴ����߳�
	bool socket_thread_delete();

	//�߳��Ƿ��Ѿ������ɹ������������д�ӡ��ش���
	bool socket_thread_isready() const;
	//�߳��Ƿ��Ѿ�����������ת�У�������ӡ�����Ѿ�������
	bool socket_thread_isrunning() const;
	//�ȴ��� Write Thread ��������̹߳ر�
	//	dwTimeOut	:[in]��ʱ������ms, Ϊ0ʱ��ʾ����ʱ�ȴ�
	//ע��һ������ Write Thread �˳�ǰ����ʱ����
	void socket_thread_wait_for_terminated(DWORD dwTimeOut);
public:
	//��ʼ������
	virtual void socket_init(BOOL bReAllocBuffer);
	//	pszIP, uPort:[in]�˶����ƿ�IP��ַ�������˿�
	//	nBufferSize :[in]��ʼ�����ջ�������С
	//	nGrowSize	:[in]����������С����ʱ,ָ���Զ����ݵĴ�С(����),��<=0ʱ��ʾ�������Զ�����
	//	dwHeartBeatTime	:[in]��������źŷ��ͼ����ms��Ϊ0ʱ���Զ����ͼ���ź�
	//	pErrorInfo	:[out]������Ϣ����������ֵΪFALSEʱ��Ч
	virtual BOOL socket_Open(const char *pszIP, int nBufferSize, int nGrowSize, DWORD dwHeartBeatTime, CString *pErrorInfo);
	//���Ͳ���ָ��
	virtual int socket_senddata(const void* snd_msg, int msg_len);
	//�м�����֡����
	static DWORD socket_repeater_config_proc(void *pData, const void *pDataList, int dataSize);
	//�м�����֡����-���������Ϣ
	DWORD socket_AddReceiveData(const void* pBuf, DWORD InBufferCount);
	BOOL socket_IsEnable() const;
	BOOL socket_IsOpen() const;
	void socket_Close(BOOL bReleaseBuffer);

};
