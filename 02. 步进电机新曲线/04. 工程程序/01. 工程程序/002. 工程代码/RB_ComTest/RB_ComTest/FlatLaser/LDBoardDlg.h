
#pragma once

#include "../MBComPort/MBComPort.h"

// CLDBoardDlg �Ի���

#define LD_MAX_VOLUME		5.0
#define SELF_INFO_MAX		200
#define LD_CURVE_RATE_X		50
#define VP_NUM_MAX			60000
#define CFG_MAX_POWER		255


enum LDVOLTAGELIST
{
	ldvoltagelist_ch		= 0,	//ͨ��
	ldvoltagelist_pc		= 1,	//���õ�ѹ
	ldvoltagelist_lastPD,			//�ϴ�ʵ���ѹ
	ldvoltagelist_nowPD,			//��ǰʵ���ѹ
	ldvoltagelist_lastLD,			//�ϴε�Դ���ѹ
	ldvoltagelist_nowLD,			//��ǰ��Դ���ѹ
	ldvoltagelist_setpower,			//���ù���
	ldvoltagelist_power,			//ʵ�ʹ���
	ldvoltagelist_state,			//״̬

	ldvoltagelist_count
};

//����ģ�����·��
enum FLATLASER_MODAL
{
	flatlaser_modal_32 = 0,
	flatlaser_modal_48,
	flatlaser_modal_64,

	flatlaser_modal_16,

	flatlaser_modal_24,

	flatlaser_modal_count
};
#define flatlaser_modal_default		flatlaser_modal_64

class CLDBoardDlg : public CDialog
{
	DECLARE_DYNAMIC(CLDBoardDlg)

public:
	CLDBoardDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLDBoardDlg();

// �Ի�������
	enum { IDD = IDD_LD_BOARD };

private:
	CRect m_rectLarge;
	CRect m_rectSmall;

	int m_nLightNum;	//��·ģ���Ӧ��ͨ������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLdSetCheckPower();

	CListCtrl m_lstLdPower;

	float m_fSetCheckVol;

	int	m_nLaserModal;			//��·ģ������

	CIntEdit	m_nEditSetPCPower;
	CFloatEdit	m_fEditSetVol;
	CFloatEdit	m_fEditSetCheckVol;

	void LDParameterSaveToCfg64(const char *pszProfile);
	void LDParameterLoadFromCfg64(const char *pszProfile);
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CListCtrl m_lstLdCheckPower;
	CComboBox m_cbLdVolCheck;

	afx_msg void OnBnClickedButtonLdCheckPowerDelete();
	afx_msg void OnBnClickedButtonLdCheckPowerClearall();
	
	//��ֵ
	CPOINTArray m_VolPowerControlPoints;
	CPOINTArray m_VolPowerCurvePoints;
	BOOL LDCreateVPTable();
	BOOL LDCreateVPCurve();

	void LDSaveVPCurve(const char *pszProfile);
	void LDLoadVPCurve(const char *pszProfile);

	void LD_Check_Power_Add(float fVol,int nPower);
	void LD_Check_Power_Modify(float fVol,int nPower);
	BOOL IsVPTableResonable();
	void RestrictTableAec();

	afx_msg void OnBnClickedCheckSelectAllChannel();
	CButton m_ckSelectAllChannel;

	afx_msg void OnBnClickedButtonLdCheckPowerExport();
	afx_msg void OnBnClickedButtonLdCheckPowerImport();
	afx_msg void OnBnClickedButtonLdSetcheck();

	CEdit m_edtSetPowerDelay;

	afx_msg void OnBnClickedButtonLdExport();
	afx_msg void OnBnClickedButtonLdImport();
	CListCtrl m_lstLdVoltage;

	afx_msg void OnBnClickedButtonExpand();
	CStatic		m_pgsLd64Status;
	afx_msg void OnBnClickedButtonLdGetLast();

	afx_msg void OnBnClickedButtonLdGetNow();
	afx_msg void OnBnClickedButtonLdCorrect();
	afx_msg void OnBnClickedButtonLdSetLd();
	afx_msg void OnBnClickedButtonLdSetPc();
	afx_msg void OnBnClickedButtonLpLighton();
	afx_msg void OnBnClickedButtonLpLightoff();
	afx_msg void OnBnClickedButtonLpLightonTime();

	//������Ϣ
	void ShowErrorInfo(BYTE bErrorCode);
	void EnableControl(void);

public:
	CCommunicationPort* m_pComPort;

	CWinThread *m_pParentThread;

public:
	CString		m_strIniFile;
protected:
	virtual void PostNcDestroy();
public:
	CComboBox m_lasermodalCtrl;
	afx_msg void OnCbnSelchangeComboLdLasermodal();
	void ResetListForLightNum(void);
};


//����Ĭ�ϼ���ģ���·��
int GetLaserNumber_Default(BYTE bLaserModal);
UINT LDGetCurrentPowerThreadProc(LPVOID pParam);
