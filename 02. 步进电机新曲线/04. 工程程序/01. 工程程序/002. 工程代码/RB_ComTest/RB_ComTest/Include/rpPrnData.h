///////////////////////////////////////////////////////////////////////////
// rpPrnData.h : �����ӡ�豸��ز�����Ϣ�����ݽṹ�� 
//
// Created by hzm 2010-03-19
//
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "../include/rpBasexp.h"
#include "../include/ColorInfo.h"

//#ifdef /*RP_EXT_CLASS*/
//	#undef /*RP_EXT_CLASS*/
//	#undef RP_EXT_API
//	#undef RP_EXT_DATA
//#endif
//
//#ifdef APP_RPPRNBASE
//	#define /*RP_EXT_CLASS*/       AFX_CLASS_EXPORT
//	#define RP_EXT_API         AFX_API_EXPORT
//	#define RP_EXT_DATA        AFX_DATA_EXPORT
//#else
//	#define /*RP_EXT_CLASS*/       AFX_CLASS_IMPORT
//	#define RP_EXT_API         AFX_API_IMPORT
//	#define RP_EXT_DATA        AFX_DATA_IMPORT
//#endif

#define KY_MAX_GROUP				8									//ϵͳ���֧�ֵ���ͷ����
#define KY_MAX_CHANNELS				8									//ϵͳ���֧�ֵ���ɫͨ����
#define KY_MAX_COLORS				(KY_MAX_GROUP*KY_MAX_CHANNELS)		//ϵͳ���֧�ֵ���ͷ����

#define _MAX_PRINTER_NAME_			100			//ϵͳ���֧�ֵĴ�ӡ�����Ƴ���
#define _MAX_SPEED_INFO_			20			//ϵͳ���֧�ֵĴ�ӡ�ٶ���������
#define _MAX_SPEED_LEVEL_			3			//ϵͳ���֧�ֵĴ�ӡ�ٶȼ�������
#define _MAX_SPEED_LIMIT_			10			//ϵͳ���֧�ֵĴ�ӡ�ٶȼ���ֵ

#define _MAX_XRES_LEVEL_			10			//ϵͳ���֧�ֵ�X�򾫶�������
#define _MAX_YPASS_LEVEL_			20			//ϵͳ���֧�ֵ�Y��Pass������

#ifndef PPDWORD
typedef LPDWORD			*PPDWORD;			//DWORD data[m][n][k]
typedef LPINT			*PPINT;				//int	data[m][n][k]
typedef PUINT			*PPUINT;			//UINT	data[m][n][k]
#endif

#ifndef PZPSTR
#ifndef PSTR
typedef __nullterminated CHAR *NPSTR, *LPSTR, *PSTR;
#endif
typedef __nullterminated PSTR *PZPSTR;		//char	data[m][n][k]
#endif

#pragma pack(push)
#pragma pack(1)


//��ͷ����
#define _PRINTHEAD_ROW_MAX_					16				//������ͷ���������������ֵ��
struct /*RP_EXT_CLASS*/ PRINTHEAD
{
	//��ͷ����
	enum PRINTHEADTYPE
	{
		printhead_none = 0x0000,		//��ʼ��״̬����Ч��ͷ���ͣ�
		printhead_spectra_galxy_80 = 0x0001,		//Spectra Galxy 80
		printhead_spectra_galxy_30 = 0x0002,		//Spectra Galxy 30, default

		printhead_spectra_polaris_pq512 = 0x0003,		//Polaris PQ-512 Printhead

		printhead_spectra_StarFire_sg1024 = 0x0004,		//StarFire SG-1024/M-C printhead

		printhead_Unknown = 0xFFFF		//δ֪��ͷ����
	};

	WORD	nType;					//��ͷ����			: PRINTHEADTYPE

	WORD	nNozzleSize;			//������ͷ��������	: 256
	double	dNozzleDistance;		//������			: 0.254mm��100 dpi��
	BYTE	bRowCount;				//ÿ����ͷ���������� : 1
									//		ÿ�������� = nNozzleSize / bRowCount

	double	dDistance_Hor[_PRINTHEAD_ROW_MAX_];		//ÿ����ͷ�и������м��X��ƫ�ƣ������ֵ��� [0,1,2,...]����mm
													//�����и�ֵ���������������ֵͬ�����ҽ���һ��ֵΪ0����ʾ��ʼ���죨No.0��
													//�� bRowCount<=1ʱ�����Ա�������


	UINT	nColorLevel;			//�Ҷȼ�����Ӧֵ��ʾ�Ҷȼ������� 0x08 ��ʾ֧�� 8 ���Ҷȴ�ӡ
									//		���� 0,1,2 ����ʾ��֧�ֻҶȴ�ӡ����֧�ֵ�ɫ��ӡ
	UINT	nNozzleDropSizeMin;		//��Сī��			: 80pl��������ע�����ī��Ϊ��Сī�� * ���Ҷȼ���-1����

public:
	//�������
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//�����Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();
};

//��ͷ�������������ͷ���� 1*m, 2*m, 3*m, ..., n*m ģʽ��
struct /*RP_EXT_CLASS*/ PRINTHEADGROUP
{
	PRINTHEAD	printhead;									//��ͷ����

	UINT		nHeadNumber;								//�����ͷ����<= KY_MAX_COLORS

	UINT		nGroupCount;								//��ͷ������1, 2, 3, ...��<= KY_MAX_GROUP
															//ÿ����ͷ��ʹ����ɫ�� nColor = nHeadNumber/nGroupCount (<=KY_MAX_CHANNELS)
															//������Լ��������������ɫ��(KY_MAX_COLORS)�������
	//���ж�����ͷ����ͬɫ��ʱ������ͬɫ��ͷ����Ϸ�ʽ
	enum COMBINEWAY
	{
		combineway_normal = 0,		//��1����ͷ(nGroupCount==1)

		//�������ӣ���Ϻ�ʵ������������಻�䣺dNozzleDistance
		combineway_series_connection_normal = 0x01,		//��������-���ص����죬��ͬɫ��ͷY����=(nNozzleSize*dNozzleDistance) * N������
		combineway_series_connection_overlapping_1 = 0x02,		//��������-�ص�1�����죬��ͬɫ��ͷY����=((nNozzleSize-1)*dNozzleDistance) * N��������ʵ����Ч������=nGroupCount*(nZozzleSize-1)+1
		combineway_series_connection_overlapping_2 = 0x03,		//��������-�ص�2�����죬��ͬɫ��ͷY����=((nNozzleSize-2)*dNozzleDistance) * N��������ʵ����Ч������=nGroupCount*(nZozzleSize-2)+2

		//�������ӣ���Ϻ�ʵ��������������ֵ��dNozzleDistance / nGroupCount
		//��������-ͬɫ��ͷ��ֵ����һ��������ȣ���ͬɫ��ͷY����=(m+1/nGroupCount)*dNozzleDistance��mΪ����������ʵ���ߴ�ֵ�ɸ�����ͷ��Passʱ����
		combineway_parallel_connection_normal = 0x10,		//��������-��ͨģʽ(m==0)

	};
	BYTE	bCombineWay;										//ͬɫ��ͷ��Ϸ�ʽ��ע������ nGroupCount > 1 ʱ��Ч

	//����ͬһ���豸��ȷ�����ͣ������²�����ͨ��Ӧ�ó�����и��ģ�����ϻ�е��
	UINT	nFilterIndex[KY_MAX_COLORS];					//��ͷ����ָ࣬��ÿ���е���ͷ��ţ��������ظ�
															//	nHeadIndex = nFilterIndex[nGroupIndex*nColor+nIndex]�����б�ʾ��nGroupIndex���еĵ�nIndex����ͷ��nColorΪһ�����ͷ��������ɫͨ����

	DWORD	nColorTable[KY_MAX_CHANNELS];					//ָ��ÿ����ͷ(nColor)�����ϣ�ky_printer_color����ɫ��
	UINT	nDataFilter[KY_MAX_CHANNELS];					//ָ����ͷ��Ӧ����ͷ��ͨ�����(nColor)

public:
	//�������
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;

	//��ͷ�������Ϣ�Ƿ���Ч
	//  pFilterIndex:[in]��ͷ�������Ϣ��ͬ nFilterIndex ����
	//  nHead		:[in]�ܵ���ͷ��
	//ע��pFilterIndex �е�ÿ��ֵ������ͬ
	static bool IsValidFilter(const UINT *pFilterIndex, int nHead);
	//ÿ����ͷ��������Ϣ�Ƿ���Ч
	//  pColorTable :[in]��ͷ������Ϣ
	//  nColor      :[in]��ɫ��
	//ע����ͷ������Ϣֵ������ͬ
	static bool IsValidColorTable(const DWORD *pColorTable, int nColor);
	//��ͷ��Ӧ��ͷ��ͨ����Ϣ�Ƿ���Ч
	//	pDataFilter	:[in]��ͷ��Ӧ����ͷ��ͨ����Ϣ��ͬ nDataFilter ����
	//	nChannels	:[in]��Чͨ������
	//ע��ͨ�����ֵ������ͬ
	static bool IsValidDataFilter(const UINT *pDataFilter, int nChannels);

	//��������Ӹ���
	//����ֵ��
	//	>= 0 	:������Ӹ���
	//	< 0 	:��Ч��Ϸ�ʽ
	static int GetOverlappingNozzles(BYTE bCombineWay);
	//��ʵ����Ч��������
	//	nNozzles	:[in]������ͷ������
	//	nGroupCount	:[in]��ͷ������ͬɫ��
	//	bCombineWay	:[in]��ͷƴ�ӷ�ʽ
	static int GetTotalNozzles(int nNozzles, int nGroupCount, BYTE bCombineWay);

	//��ͷ������Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();
};
//����š���ɫ�ż���ʵ�ʵ�nFilterIndex���
//	grpindex	:��ţ�< KY_MAX_GROUP
//	clrindex	:��ɫ�ţ�< color
//	color		:ÿ����ɫ���� <= KY_MAX_CHANNELS
//	filterindex	:nFilterIndex��ţ� <= KY_MAX_COLORS��nFilterIndex[filterindex]����Ӧ����ͷ���
#define TransToFilterIndex(grpindex,clrindex,color)					((grpindex)*(color) + (clrindex))
//��nFilterIndex��ż�����š���ɫ��
#define TransFromFilterIndex(grpindex,clrindex,color,filterindex)	{(grpindex)=(filterindex)/(color); (clrindex)=(filterindex) % (color);}
//����Ч����ɫͨ������<= KY_MAX_CHANNELS��
//	headnum		:��ͷ������<= KY_MAX_COLORS
//	grpcount	:��ͷ������<= KY_MAX_GROUP
#define GetValidColorChannels(headnum,grpcount)						((headnum)/(grpcount))

//���Ʋ���
//ɢ��
struct /*RP_EXT_CLASS*/ SPRAYPARAM
{
	BYTE bSprayForIdleEanble;		//�Ƿ��������ʱɢ��
	BYTE bSprayForPrintingEanble;	//�Ƿ������ӡ��ɢ��

	WORD wSprayFrequencyForIdel;	//����ʱɢ��Ƶ�ʣ�Hz
	WORD wSprayPassForPrint;		//ָ����ӡ��ɢ�������������ٸ� Pass �����ɢ��

public:
	//�������
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//�����Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();
};
//��ӡ���Զ���ͷ��ϴ
//	���̣���ͷ�Ƶ���ϴ��-->ѹī-->�ӳ�-->��ϴn��-->�����ϴ
struct /*RP_EXT_CLASS*/ CLEANHEADPARAM
{
	BYTE	bEnable;				//�Ƿ������ӡ�н����Զ���ͷ��ϴ
	WORD	wPass;					//ָ����ӡ����ͷ��ϴ������������ٸ� Pass �����ɢ��
	WORD	wPurgeTime;				//ָ���Զ���ͷ��ϴʱ��ѹīʱ��������
	WORD	wPurgeDelay;			//ָ��ѹī��������ӳ٣�����
	BYTE	bCleanTime;				//ָ���Զ���ͷ��ϴʱ��ϴ���ε�������

public:
	//�������
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//�����Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();
};

//��ӡ�豸�������˶�/��ӡ��أ�
//	�������ò���
struct /*RP_EXT_CLASS*/ MOTOPARAM_GENERAL
{
	WORD	X_MAX_WIDTH;				//x��������ӡ���(mm)
	WORD	PRINT_MAX_FREQ;				//��ӡ���Ƶ��(Hz)
	WORD	X_MAX_SPEED;				//x��������ٶ�(mm/s)
	Float32 X_COEF;						//x�����ٶȽ���ϵ��

public:
	//�������
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//�����Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();
};
//  X�����ò���
struct /*RP_EXT_CLASS*/ MOTOPARAM_X
{
	WORD	SERVE_PER_PULSE;			//���ӳ��ֱȣ����Ӽ��ٻ�����Ϊ�����ٻ���ı�ֵ��
	WORD	X_P_DIS;					//x�����ӡʱ�߼��پ���(mm)
	WORD	X_S_DIS;					//x�����ӡʱ�߼��پ���(mm)
	WORD	IO_SPACE;					//��դIO���(mm) IO_SPACE
	WORD	IO_PULSE;					//��դIO�������� ��IO_PULSE
	DWORD	X_DELAY_S;					//С�������ƶ���ʱʱ�䣨us��,ֵԽ��С���ƶ�Խ��
	DWORD	X_DELAY_F;					//С�������ƶ���ʱʱ�䣨us����ֵԽ��С���ƶ�Խ��
	Float32 SERVE_PER_CYCLE;			//�ŷ����תһ��С���ߵľ��루mm��
public:
	//�������
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//�����Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();
};
//  Y�����ò���
struct /*RP_EXT_CLASS*/ MOTOPARAM_Y
{
	WORD	Y_PER_PULSE;				//Y����ӳ��ֱȣ����Ӽ��ٻ�����Ϊ�����ٻ���ı�ֵ��
	WORD	Y_MAX_SPEED;				//Y��������ٶȣ�mm/s��
	DWORD	Y_DELAY;					//��ӡʱ������ʱʱ�䣨us��
	DWORD	Y_DELAY_S;					//Y�������ƶ���ʱʱ��(us) 
	DWORD	Y_DELAY_F;					//Y������ƶ���ʱʱ��(us) 
	Float32	Y_WHORL;					//Y�����ݾ�(mm) 
public:
	//�������
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//�����Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();
};

//  Z�����ò���
struct /*RP_EXT_CLASS*/ MOTOPARAM_Z
{
	DWORD	Z_DELAY;					//Z�����ƶ���ʱ��us��
	Float32	Z_WHORL;					//Z�����ݾ�(mm) 
	Float32 Z_POSITION_MIN;				//Z������С����(pulse) 
	Float32 Z_POSITION_MAX;				//Z����������(pulse) 
public:
	//�������
	bool GetFromProfile(const char *pszSection, const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszSection, const char *pszProfile) const;
	//�����Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();
};

//��ӡ������
#define _FLATMACHINE_HEIGHT_LIMIT_		50				//ƽ�������С��Ч��ӡ����ֵ��mm
struct /*RP_EXT_CLASS*/ PRINTER
{
	//��ӡ������
	enum PRINTERTYPE
	{
		printer_none = 0x0000,		//��ʼ��״̬����Ч��ӡ�����ͣ�

		printer_RainBow_08SG = 0x0001,		//8��ͷ��1*n���飬Spectra Galxy ��ͷ��PCI9052��Com

		printer_RainBow_64MG = 0x0002,		//m*8 ��ģʽ(m=1,2,4,8)�����8*8��Spectra Galxy��ͷ��PCI9052�� Com

		printer_RainBow_08SS = 0x0003,		//8��ͷ��1*n���飬Spectra SG-1024/M-C ��ͷ��PCI9052��Com

		printer_RainBow_Virtual = 0xFFFD,		//�����ӡ�������������ģ�������

		printer_RainBow_NewDebug = 0xFFFE,		//�����ĵ����ô�ӡ�����ͣ�δȷ����׼���ͣ�

		printer_Unknown = 0xFFFF		//δ֪��ӡ������
	};

	WORD			nType;							//��ӡ������: PRINTERTYPE
	char			szName[_MAX_PRINTER_NAME_ + 1];	//��ӡ�����ƣ��硰RainBow_1816DG��

	double			dMinStartX;						//�豸�ɴ�ӡ����С��ʼλ�ã�mm��X��
	double			dMinStartY;						//�豸�ɴ�ӡ����С��ʼλ�ã�mm��Y�򣩣���ƽ���ʹ�ã��������̶�Ϊ0
	double			dMaxWidth;						//�豸�ɴ�ӡ������ȣ�mm��
	double			dMaxHeight;						//�豸�ɴ�ӡ�����߶ȣ�mm����<= 0 ʱ��ʾ�����ƣ�����������

	PRINTHEADGROUP	printgroup;						//��ͷ�����

	//��ɫģʽ
	UINT	nInkModeLevel[INKMODE_MAX + 1];		//ָ����ǰ��ͷ��֧�ֵ���ɫģʽ�б��� INK_NONE ����
	int		nInkModeCount;						//��Ч����ɫģʽ��

	char	szSpeedLevel[_MAX_SPEED_LEVEL_ + 1][_MAX_SPEED_INFO_];	//֧�ֵĴ�ӡ�ٶȼ�����Ӧ���ٶ��������Կ��ַ�����ʶ����
	BYTE	bSpeedValue[_MAX_SPEED_LEVEL_ + 1];						//ʵ�ʵĴ�ӡ�ٶȼ���ֵ��(0, _MAX_SPEED_LIMIT_]
	int		nSpeedCount;						//��Ч�Ĵ�ӡ�ٶȼ���

	UINT	nXResLevel[_MAX_XRES_LEVEL_ + 1];		//֧�ֵ�X���򾫶�ֵ��dpi��300��400��600��...���� 0 ��ʶ����
	int		nXResCount;							//��Ч��X���򾫶ȼ���

	UINT	nYPassLevel[_MAX_YPASS_LEVEL_ + 1];	//֧�ֵ�Y����Pass����1��2��3��4��...���� 0 ��ʶ����
	enum PASSWAY
	{
		passway_normal = 0,		//��ͨģʽ
		passway_plus,				//Pass+ ģʽ
		passway_count
	};
	BYTE	bYPassWay[_MAX_YPASS_LEVEL_ + 1];		//֧�ֵ�Y����Pass�����
	int		nYPassCount;						//��Ч��Y����Pass������

	BYTE	bSprayEnable;						//�Ƿ�֧��ɢ�������Ӳ�����������ø��ģ�
	BYTE	bCleanHeadEnable;					//�Ƿ�֧���Զ���ͷ��ϴ����

	//����ͬһ���豸��ȷ�����ͣ������²�����ͨ��Ӧ�ó�����и��ģ�����ϻ�е��
	UINT	nInkModeType;						//��ǰʹ�õ���ɫģʽ��< INKMODE_MAX
	UINT	nSpeedType;							//��ǰʹ�õĴ�ӡ�ٶ�ֵ��< _MAX_SPEED_LEVEL_
	UINT	nXResType;							//��ǰʹ�õ�X���򾫶ȣ�< _MAX_XRES_LEVEL_
	UINT	nYPassType;							//��ǰʹ�õ�Y��Pass����< _MAX_YPASS_LEVEL_

	int		nDistance_Hor[_MAX_SPEED_LEVEL_][_MAX_XRES_LEVEL_][KY_MAX_COLORS];			//ָ��ÿ����ͷ��X���ࣨPixel����nDistance_Hor[s][r][m]��m:��ͷ��ţ�nHeadNumber����s:�ٶȣ�r:X�򾫶�
	int		nDistance_Hor_Reversed[_MAX_SPEED_LEVEL_][_MAX_XRES_LEVEL_][KY_MAX_COLORS];	//ָ��ÿ����ͷ��X�����ࣨPixel����nDistance_Hor[s][r][m]��m:��ͷ��ţ�nHeadNumber����s:�ٶȣ�r:X�򾫶�
	double	dDistance_Ver[KY_MAX_COLORS];												//ָ��ÿ����ͷ��Y���ࣨmm����dDistance_Ver[m]��m:��ͷ��ţ�nHeadNumber��

	int		nAdjust_bi_directional_compensation[_MAX_SPEED_LEVEL_][_MAX_XRES_LEVEL_];	//˫�򲹳�ֵ����ӡ�ٶȻ򾫶Ȳ�ͬ������ֵҲ��ͬ��Pixel�����壩
	double	dAdjust_step_compensation[_MAX_YPASS_LEVEL_];								//��������ֵ��1/pixel��ÿ������ƫ��ֵ����Y��Pass����ͬ������ֵҲ��ͬ

	int		fade_nLevel[_MAX_YPASS_LEVEL_];				//���뵭������-Ч�����壬Y��Pass����ͬ������ֵҲ��ͬ

	enum TRACELESSPRINT_WAY
	{
		tracelessprint_way_none = 0,				//��ʹ���޺۴���ֱ���ͣ�

		tracelessprint_way_micro = 1,				//΢����
		tracelessprint_way_small,						//С����
		tracelessprint_way_standard,					//��׼�ͣ���ͨ�ͣ�������С��Ԫ�ߴ��Ӧ
		tracelessprint_way_middle,						//�в���
		tracelessprint_way_large,						//����
		tracelessprint_way_huge,						//�޲���

		tracelessprint_way_limit = 0xFF
	};
	BYTE	bTracelessPrintWay;							//�޺۴�ӡ����-Ч�����壬TRACELESSPRINT_WAY
	double	dTracelessPrintDefWidth;					//�޺۴�ӡ����-��С��ԪĬ�Ͽ�ȣ�mm
	double	dTracelessPrintDefHeight;					//�޺۴�ӡ����-��С��ԪĬ�ϸ߶ȣ�mm

	UINT	nPrintColorLevel;							//ʵ�ʴ�ӡʹ�õĻҶȼ���<= PRINTHEAD::nColorLevel

	SPRAYPARAM		sprayparam;							//ɢ�����

	CLEANHEADPARAM	cleanheadparam;						//��ӡ���Զ���ͷ��ϴ����

	//��ӡ��ʽ
	enum PRINTWAY
	{
		printway_unidirection = 0,		//�����ӡ
		printway_bidirection = 1,		//˫���ӡ

		printway_count                      //��Ч�Ĵ�ӡ��ʽ��
	};
	BYTE	bPrintway;				//��ӡ��ʽ��PRINTWAY

	enum JUMPWAY
	{
		jumpway_normal = 0x00,		//�޿�������ף�����
		jumpway_x = 0x01,		//X��С����������
		jumpway_y = 0x02,		//Y�򣨵���/˿�ˣ�������
		jumpway_all = jumpway_x | jumpway_y,	//����ȫ�����

	};
	BYTE	bJumpWay;			//��������ף�����ʽ
	double	dJumpLimit_x;		//X����С���׿�ȣ�mm
	double	dJumpLimit_y;		//Y��������׳��ȣ�mm

	BYTE	bPrintToFile;		//�����PRN�ļ���ΪFALSEʱֱ���������ӡ��

	BYTE	bVirtualPrinter;	//�Ƿ�Ϊ�����ӡ����������ʵ��Ӳ���豸��

	//ԭ��λ�ö���
	enum ORIGIN_POSITION
	{
		origin_pos_unknown = 0x00,		//δ֪

		origin_pos_x_zero = 0x01,		//X���ӡԭ��
		origin_pos_x_max = 0x02,		//X����ϴ��λ�ã�X�����λ�ã�
		origin_pos_x_current = 0x03,		//X��ǰλ��

		origin_pos_y_zero = 0x10,		//Y���ӡԭ��
		origin_pos_y_max = 0x20,		//Y�����λ��
		origin_pos_y_current = 0x30,		//Y��ǰλ��

		origin_pos_zero_xy = origin_pos_x_zero | origin_pos_y_zero,				//��ӡԭ��
		origin_pos_max_xy = origin_pos_x_max | origin_pos_y_max,					//ƽ�������λ
		origin_pos_zero_x_max_y = origin_pos_x_zero | origin_pos_y_max,
		origin_pos_max_x_zero_y = origin_pos_x_max | origin_pos_y_zero,

		origin_pos_current_xy = origin_pos_x_current | origin_pos_y_current,		//��ǰλ��
		origin_pos_current_x_zero_y = origin_pos_x_current | origin_pos_y_zero,			//˿�˷����ӡԭ�㣨С��������
		origin_pos_zero_x_current_y = origin_pos_x_zero | origin_pos_y_current,			//С�������ӡԭ�㣨˿�˲�����

		origin_pos_max_x_current_y = origin_pos_x_max | origin_pos_y_current,			//ɢ��λ��Ҳ������������λ

		origin_pos_mask_x = 0x0F,		//X��λ�ò�������
		origin_pos_mask_y = 0xF0		//Y��λ�ò�������
	};
	BYTE	bOriginPosition_start;			//��ӡ��ʼǰС��λ�ã�PRINTER::ORIGIN_POSITION
	BYTE	bOriginPosition_end;			//��ӡ����ʱС��λ�ã�PRINTER::ORIGIN_POSITION

	BOOL	bFlatMachine;					//�Ƿ�Ϊƽ������ͣ�FALSEΪ���������ͣ�
	BOOL	bOriginPosition_posXEnable;		//�Ƿ�֧��X����Զ�λ������IO��λ����ģʽ�ݲ�֧�֣�������ΪFALSE

	BOOL	bThicknessEnable;				//�Ƿ�����Զ��岼�����

	int		nPriority;					//��ӡ�����߳����ȼ���Ĭ��Ϊ THREAD_PRIORITY_NORMAL

public:
	BOOL	bOriginPosition_prompt;		//��ӡ��ʼλ�ò��ǡ���ӡԭ�㡱ʱ��ʾ

public:
	//�������
	bool GetFromProfile(const char *pszProfile);
	//д�����
	bool SetToProfile(const char *pszProfile) const;
	//�����Ƿ���Ч
	bool IsValid() const;
	//��ʼ��
	void Initialize();

	BOOL fade_IsValid(int nYPassType) const;	//���뵭������-�Ƿ���Ч

	//������ӡ��ʼǰС��λ�ò���
	//	IsPosXEnable	:[in]�Ƿ�֧��X����Զ�λ������IO��λ����ģʽ�ݲ�֧�֣�������ΪFALSE
	static BYTE Check_OriginPosition_Start(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL IsPosXEnable);
	//������ӡ����ʱС��λ�ò���
	static BYTE Check_OriginPosition_End(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL IsPosXEnable);

	//-------- ���ӡԭ�㶨����صĿ���ѡ���������-----
	//	IsFlatMachine	:[in]�Ƿ�Ϊƽ����ͣ�ΪFALSEʱ��ʾ��������
	//	bStart			:[in]�Ƿ��ӡ��ʼǰ�Ķ�λ״̬��ΪFALSEʱ��ʾ��Դ�ӡ����ʱ��λ״̬����
	//	IsPosXEnable	:[in]�Ƿ�֧��X����Զ�λ������IO��λ����ģʽ�ݲ�֧�֣�������ΪFALSE
	static BYTE TransIndexToOriginPosition(int nIndex, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable);
	static int TransOriginPositionToIndex(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable);
	static void InitOriginPositionList(CComboBox *pList, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable);

	//����Pass��������
	//	pLeapNum	:[out]ÿPass����ֵ��Pixel��
	//	nozzlesize	:[in]������
	//	pass		:[in]PASS��
	//���أ�
	//	���Ĳ���ֵ(Pixel)
	static int step_calcleapnum(DWORD *pLeapNum, int nozzlesize, int pass, int nFadeLevel);
	//���뵭������-�õ������ֵ
	//	blimit	:[in]�Ƿ�������ֵ����ΪFALSEʱ������Ĭ�ϲ���ֵ
	static int fade_calclimit(int nozzlesize, int pass, bool blimit);

	//����ÿ������ռ�õ�����λ��
	//	nPrintColorLevel	:[in]ʵ�ʴ�ӡʹ�õĻҶȼ���<= PRINTHEAD::nColorLevel
	static int ColorLevel_GetStepPixel(UINT nPrintColorLevel);
};

//�ⲿ����״̬��Ӧ�ó�����Դ˽�����Ӧ����
enum ky_printer_command_status
{
	kypcs_mb_none = 0x00000000,	//���������ⲿ����

	kypcs_mb_reset = 0x00000001,	//��λ

	kypcs_mb_pause = 0x00000002,	//��ͣ��ӡ���� kypcs_mb_resume ��ͬʱʹ�ã�
	kypcs_mb_resume = 0x00000004,	//�ָ���ӡ���� kypcs_mb_pause ��ͬʱʹ�ã�

	kypcs_mb_syserror = 0x00000008,	//ϵͳ����

	kypcs_mb_sysstatus = 0x00000010,	//ϵͳ�쳣״̬

	kypcs_mb_cleanhead = 0x00000020,	//��ͷ��ϴ

	kypcs_mb_count
};

// ģ���ⲿ������չ
// UI
//
////��չ�ӿڣ�������أ�ʹ��״̬
enum ky_ui_type
{
	kyuit_offline = 0x80, //�ڷǴ�ӡ״̬���ѻ���δ���� print_start_print ���ߵ��ù� print_end ֮������Ч 

	kyuit_online_inside_printdata = 0x01, //�����ݴ�ӡ״̬������ print_data_print������Ч��һ����Ƿ��ص�ǰ�豸״̬����ʱ�ײ�����Դ�����ģʽ�Ի�����ʽ���������ɹ����������أ� 

	kyuit_online_outside_printdata = 0x02, //�ڷ����ݴ�ӡ״̬�������������� print_data_print ���õ�ǰ������Ч����������Ӧ�ó����еĵ���ʱ�����Ѱ��գ���� 

	kyuit_online_pause = 0x06, //����ͣ״̬����Ч������ kyuit_online_outside_printdata ������ 

	kyuit_online = 0x7F, //������״̬���Գɹ����� print_start_print �� ���� print_end Ϊ�ޣ�����Ч(���� kyuit_online_xxxx��ʽ��״̬�ļ���) 

	kyuit_all = 0xFF	//����״̬�¾���Ч 
};
// hparent :[in]���ô���
//	wParam, lParam :[in]ֱ��ʹ�� ky_ui_command_info ����������
typedef void(_stdcall *ui_command_proc)(HWND hparent, WPARAM wParam, LPARAM lParam);
struct ky_ui_command_info
{
	const char			*ptext;		//����������������ڲ˵�����ʾ
	ui_command_proc		pfncommand;	//ʵ�ʵ����Ӧ�ó���ͨ�� pfncommand(hparent, wParam, lParam)��ʵ�ֹ��ܵ���

	WPARAM				wParam;		//��ز����������ں�������	
									//	command_id : (dwParam & 0x00FFFFFF)����ģ���ڲ�����
									//	ky_ui_type : (dwParam>>28) & 0xFF
	LPARAM				lParam;		//���������������ں�������
};


//��ǰ������ӡ��Ϣ
struct ky_print_info
{
	double	width;			//��� mm
	double	height;			//���� mm

	UINT	nHeadNumber;	//�����ͷ����<= KY_MAX_COLORS
	UINT	nGroupCount;	//��ͷ������1, 2, 3, ...��<= KY_MAX_GROUP
	DWORD	nColorTable[KY_MAX_CHANNELS];				//ָ��ÿ����ͷ�����ϣ�ky_printer_color����ɫ��

	UINT    color_level;    //�Ҷȴ�ӡ��ʽ
	UINT	xres;			//X����ľ���,DPI
	UINT	pass;			//Y�����Passֵ

	BYTE	bPrintway;		//��ӡ��ʽ��PRINTER::PRINTWAY
	UINT	nSpeedType;		//��ǰʹ�õĴ�ӡ�ٶ�ֵ��< _MAX_SPEED_LEVEL_

	BYTE	bOriginPosition_start;	//��ӡ��ʼǰС��λ�ã�PRINTER::ORIGIN_POSITION
	BYTE	bOriginPosition_end;	//��ӡ����ʱС��λ�ã�PRINTER::ORIGIN_POSITION
};

#pragma pack(pop)