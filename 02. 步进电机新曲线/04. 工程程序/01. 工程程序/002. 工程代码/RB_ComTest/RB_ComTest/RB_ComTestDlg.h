
// RB_ComTestDlg.h : ͷ�ļ�
//

#pragma once
#include "MBComPort/MBComPort.h"
#include "afxwin.h"
#include "SFBoardThread.h"
#include "afxcmn.h"
#include "RAS/RasBoardThread.h"
#include "FlatLaser/LDBoardThread.h"

enum FUNCTION_TYPE
{
	functype_servo_4005 = 0,		//�ŷ��� V4005
	functype_servo_3axis_1001,		//�����ŷ��� V1001
	functype_servo_3axis_1001_new,	//�����ŷ��� V1001 (������)
	functype_raser,					//��դ�� - ����

	functype_raser_flatlaser,		//��դ�� - ƽ������

	functype_servo_param,			//�ŷ��̼�����
	functype_servo_3axis_1001_param,//�����ŷ���̼����� V1001

	functype_count
};


// CRB_ComTestDlg �Ի���
class CRB_ComTestDlg : public CDialog
{
// ����
public:
	CRB_ComTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RB_COMTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CCommunicationPort m_port;	//����
	int m_nComNum;					//���ں�
	BOOL m_bConnect;				//�Ƿ�����

	afx_msg void OnBnClickedButtonOpencom();
	CComboBox m_cbComNum;
	CEdit m_edtComRecive;
	CEdit m_edtComSend;
	
	float m_fReducer_x;				//X���ٻ�
	float m_fDollyPerimeter;		//С���ܳ�
	float m_fPulsesPerRevolution_x;	//XÿȦ����

	float m_fReducer_y;				//Y���ٻ�
	float m_fPulsesPerRevolution_y;	//YÿȦ����
	float m_fDeferentPerimeter;		//Y�����ܳ�
	float m_fDeferentPitch;			//Y˿���ݾ�

	float m_fReducer_z;				//Z���ٻ�
	float m_fPulsesPerRevolution_z;	//ZÿȦ����
	float m_fDeferentPerimeter_z;	//Z�����ܳ�
	float m_fDeferentPitch_z;		//Z˿���ݾ�

protected:
	virtual void PostNcDestroy();
public:
	CComboBox m_cbDlgType;
	afx_msg void OnBnClickedButtonOpendlg();

	afx_msg void OnBnClickedButtonClearEdit();

	afx_msg LONG OnComRecive(WPARAM wPALAM, LPARAM lPALAM);
	afx_msg LONG OnComSend(WPARAM wPALAM, LPARAM lPALAM);

	afx_msg void OnBnClickedOk();

	void GetConfigInfo(void);
	void SetConfigInfo(void);
	int m_nPortType;
	DWORD m_dwIP;
	int m_nCom;
	CButton m_openportCtrl;
	void EnableControl(void);
	CStatic m_infoCtrl;

public:
	CWnd *m_pDlgList[functype_count];
	virtual BOOL DestroyWindow();

};
