#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MBComPort/MBComPort.h"

// COfflineTimeTableDlg 对话框
enum OFFLINE_OPERATION_CURVE
{
	offline_operation_curve_none,
	offline_operation_curve_aec,
	offline_operation_curve_fast2slow,
	offline_operation_curve_slow2stop,
};

class COfflineTimeTableDlg : public CDialog
{
	DECLARE_DYNAMIC(COfflineTimeTableDlg)

public:
	COfflineTimeTableDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COfflineTimeTableDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SFOFFLINE_TIMETABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnOperation;
	virtual BOOL OnInitDialog();
	int m_nOperationType;
	afx_msg void OnBnClickedButtonSfofflineOperation();
	CWnd* m_pSFDlg;
	CCommunicationPort* m_pComPort;

	//数值
	float m_fAecS;
	float m_fSlow2StopS;
	float m_fFast2SlowS;
	float m_fFastV;
	float m_fSlowV;
	int m_nAecNum;
	int m_nFast2SlowNum;
	int m_nSlow2StopNum;
	int m_nTimeUnit;

	BOOL GetTimeTableParameter();
	void SetTimeTableParameter();
	
	//曲线
	CRect m_rcView;

	CRect m_rcAec;
	CPoint m_ptCurveAecStart;
	CPoint m_ptCurveAecEnd;

	CPOINTArray m_AecControlPoints;
	CPOINTArray m_AecCurvePoints;

	CRect m_rcFast2Slow;
	CPoint m_ptCurveFast2SlowStart;
	CPoint m_ptCurveFast2SlowEnd;

	CPOINTArray m_Fast2SlowControlPoints;
	CPOINTArray m_Fast2SlowCurvePoints;

	CRect m_rcSlow2Stop;
	CPoint m_ptCurveSlow2StopStart;
	CPoint m_ptCurveSlow2StopEnd;

	CPOINTArray m_Slow2StopControlPoints;
	CPOINTArray m_Slow2StopCurvePoints;

	int m_nOperationCurve;
	int m_nMoveIndex;
	CPoint m_ptMoveLast;
	CPoint m_ptMoveNow;

	void InitCurve();
	int IsInsideControlPoint(POINT& point, int& nOperationCurve);
	void DrawCurveArea(CDC* pDC);
	void DrawCurveAec(CDC* pDC);
	void DrawCurveFast2Slow(CDC* pDC);
	void DrawCurveSlow2Stop(CDC* pDC);
	void DrawAxis(CDC* pDC);
	void DrawCross(RECT rect, POINT point, COLORREF rgbColor, int extent, HDC hDC);
	void DrawCurve(const POINT *point,int count, COLORREF rgbColor,HDC hdc);
	BOOL MoveCurve();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	void CalculateTime();
	afx_msg void OnBnClickedButtonSfofflineCalculatetime();
	CProgressCtrl m_pgsDownload;
	afx_msg LONG OnSFDownloadProgress(WPARAM wPALAM, LPARAM lPALAM);

	//建时间表
	TIME_TABLE_OFFLINE m_timeTableOffline;
	TIME_TABLE_OFFLINE_PAMARATER m_parameterOffline;	
	void InitTimeTable();
	BOOL CreateTimeTable(BYTE bSpeedLevel,BOOL bRemind);
	BYTE m_bSpeedLevel;
	BYTE m_bMotoWay;		//SF_MOTOR_WAY

	BOOL GetSpeedLevelInfo();

	BOOL ExportSpeedLevelInfo(CString strEeportFile);
	afx_msg void OnBnClickedOk();
	BOOL ImportSpeedLevelInfo(CString strImportFile);
	BOOL AddTimeTable(BOOL bRemind);
};
