#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MBComPort/MBComPort.h"

// COnlineTimeTableDlg 对话框
enum ONLINE_OPERATION_CURVE
{
	online_operation_curve_none,
	online_operation_curve_aec,
	online_operation_curve_dec,
};

class COnlineTimeTableDlg : public CDialog
{
	DECLARE_DYNAMIC(COnlineTimeTableDlg)

public:
	COnlineTimeTableDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COnlineTimeTableDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SFONLINE_TIMETABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnOperation;
	afx_msg void OnBnClickedButtonSfonlineOperation();
	virtual BOOL OnInitDialog();
	int m_nOperationType;
	CWnd* m_pSFDlg;
	CCommunicationPort* m_pComPort;

	//数值
	float m_fAecS;
	float m_fDecS;
	float m_fEvenV;
	int m_nAecNum;
	int m_nDecNum;
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

	CRect m_rcDec;
	CPoint m_ptCurveDecStart;
	CPoint m_ptCurveDecEnd;
	CPOINTArray m_DecControlPoints;
	CPOINTArray m_DecCurvePoints;
	
	int m_nOperationCurve;
	int m_nMoveIndex;
	CPoint m_ptMoveLast;
	CPoint m_ptMoveNow;

	void InitCurve();
	int IsInsideControlPoint(POINT& point, int& nOperationCurve);
	void DrawCurveArea(CDC* pDC);
	void DrawCurveAec(CDC* pDC);
	void DrawCurveDec(CDC* pDC);
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
	
	void CalculateTime(WORD& wAecTime,WORD& wDecTime);
	afx_msg void OnBnClickedButtonSfonlineCalculatetime();
	CProgressCtrl m_pgsDownload;

	afx_msg LONG OnSFDownloadProgress(WPARAM wPALAM, LPARAM lPALAM);

	//建时间表
	TIME_TABLE_ONLINE m_timeTableOnline;
	TIME_TABLE_ONLINE_PAMARATER m_parameterOnline;
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
