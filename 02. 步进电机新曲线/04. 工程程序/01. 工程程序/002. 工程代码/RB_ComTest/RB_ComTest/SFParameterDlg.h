#pragma once

#include "MBComPort/MBComPort.h"
// CSFParameterDlg �Ի���

class CSFParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CSFParameterDlg)

public:
	CSFParameterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSFParameterDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SFPARAMATER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedOk();

	float* m_pfReducer_x;				//X���ٻ�
	float* m_pfDollyPerimeter;			//С���ܳ�
	float* m_pfPulsesPerRevolution_x;	//XÿȦ����

	float* m_pfReducer_y;				//Y���ٻ�
	float* m_pfPulsesPerRevolution_y;	//YÿȦ����
	float* m_pfDeferentPerimeter;		//Y�����ܳ�
	float* m_pfDeferentPitch;			//Y˿���ݾ�

	float* m_pfReducer_z;				//Z���ٻ�
	float* m_pfPulsesPerRevolution_z;	//ZÿȦ����
	float* m_pfDeferentPerimeter_z;		//Z�����ܳ�
	float* m_pfDeferentPitch_z;			//Z˿���ݾ�

	afx_msg void OnBnClickedButtonSbpSetParameterX();
	afx_msg void OnBnClickedButtonSbpSetParameterY();
	afx_msg void OnBnClickedButtonSbpSetParameterZ();

	afx_msg void OnBnClickedButtonSbpGetParameterX();
	afx_msg void OnBnClickedButtonSbpGetParameterY();
	afx_msg void OnBnClickedButtonSbpGetParameterZ();

	CCommunicationPort* m_pComPort;
	BOOL m_bThreeAxisServo;	//�����ŷ���
};
