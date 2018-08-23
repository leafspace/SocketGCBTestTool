#pragma once
#include "../MBComPort/MBComPort.h"
#include "afxwin.h"
#include "afxcmn.h"

// CRasBoardDlg 对话框

class CRasBoardDlg : public CDialog
{
	DECLARE_DYNAMIC(CRasBoardDlg)

public:
	CRasBoardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRasBoardDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RASBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

public:
	CCommunicationPort* m_pComPort;
	int m_nProtocolMode;

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

	CWinThread *m_pParentThread;

	float m_fIOSpace;	//IO间距
	float m_fIOBaseGridNum;	//IO间刻度数
	
	//参数
	int m_nYDir;
	int m_nPrintWay;
	int m_nAction;
	BOOL m_isUserGrid;
	BYTE m_bEndPosition;
	int m_nSpeedLevelX;
	int m_nSpeedLevelY;
	float m_fPositionX;
	float m_fPositionY;
	int m_nPurgeTime;
	float m_fWidth;
	float m_fHeight;
	float m_fResX;
	float m_fResY;
	int m_nFreqPsc;
	float m_fYDist;
	int m_nPurgeFreq;
	PositionPara m_positionPara;
	
	int MM2Pulse_X(float fMM);
	int MM2Pulse_Y(float fMM);
	float Pulse2MM_X(int nPulse);
	float Pulse2MM_Y(int nPulse);
	int MM2GridNum(float fMM);
	BOOL GetPrintParameter();
	BOOL MBPrintStart();
	BOOL MBPrintLine(int nThousandth,int nPrintHeight,BOOL bFlag);
	BOOL MBPrintEnd();
	
	CWinThread* m_pThreadPrint;
	BOOL m_isStopPrint;
	BOOL m_isPausePrint;

protected:
	virtual void PostNcDestroy();
public:
	CComboBox m_cbYDir;
	CComboBox m_cbPrintWay;
	CComboBox m_cbAction;
	CButton m_ckUseGrid;
	CComboBox m_cbEndPositionX;
	CComboBox m_cbEndPositionY;
	CProgressCtrl m_pgsPrint;
	CStatic m_staticPrint;
	afx_msg void OnBnClickedButtonRasPrintStart();
	afx_msg void OnBnClickedButtonRasPrintLine();
	afx_msg void OnBnClickedButtonRasPrintEnd();
	afx_msg void OnBnClickedButtonRasPrintPause();
	afx_msg void OnBnClickedButtonRasPrint();
	CButton m_btnPrint;
	afx_msg void OnBnClickedButtonRasPrintResume();
	afx_msg void OnBnClickedButtonRasPrintStop();
	afx_msg void OnBnClickedButtonRasSelect();

	afx_msg LONG OnRasStateConfirm(WPARAM wPALAM, LPARAM lPALAM);
	CListCtrl m_lstSpeedLevel_Y;
	CListCtrl m_lstPositionPara;
	CListCtrl m_lstSpeedLevel_X;
	afx_msg void OnBnClickedButtonRasGetPositionPara();
	CComboBox m_cbSpeedLevelX;
	CComboBox m_cbSpeedLevelY;
	afx_msg void OnEnChangeEditRasPrintFreqPsc();
	afx_msg void OnBnClickedButtonRasGetSpeedLevel();
	afx_msg void OnBnClickedButtonRasMove2Position();
protected:
	virtual void OnCancel();
	virtual void OnOK();
};
