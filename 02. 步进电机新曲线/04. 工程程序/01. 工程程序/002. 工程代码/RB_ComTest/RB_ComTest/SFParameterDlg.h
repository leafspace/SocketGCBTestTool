#pragma once

#include "MBComPort/MBComPort.h"
// CSFParameterDlg 对话框

class CSFParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CSFParameterDlg)

public:
	CSFParameterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSFParameterDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SFPARAMATER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedOk();

	float* m_pfReducer_x;				//X减速机
	float* m_pfDollyPerimeter;			//小车周长
	float* m_pfPulsesPerRevolution_x;	//X每圈脉冲

	float* m_pfReducer_y;				//Y减速机
	float* m_pfPulsesPerRevolution_y;	//Y每圈脉冲
	float* m_pfDeferentPerimeter;		//Y导带周长
	float* m_pfDeferentPitch;			//Y丝杆螺距

	float* m_pfReducer_z;				//Z减速机
	float* m_pfPulsesPerRevolution_z;	//Z每圈脉冲
	float* m_pfDeferentPerimeter_z;		//Z导带周长
	float* m_pfDeferentPitch_z;			//Z丝杆螺距

	afx_msg void OnBnClickedButtonSbpSetParameterX();
	afx_msg void OnBnClickedButtonSbpSetParameterY();
	afx_msg void OnBnClickedButtonSbpSetParameterZ();

	afx_msg void OnBnClickedButtonSbpGetParameterX();
	afx_msg void OnBnClickedButtonSbpGetParameterY();
	afx_msg void OnBnClickedButtonSbpGetParameterZ();

	CCommunicationPort* m_pComPort;
	BOOL m_bThreeAxisServo;	//三轴伺服板
};
