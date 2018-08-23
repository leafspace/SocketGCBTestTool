// OfflineTimeTableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RB_ComTest.h"
#include "OfflineTimeTableDlg.h"
#include "Curve/spline.h"
#include "SFBoardDlg.h"

// COfflineTimeTableDlg 对话框

IMPLEMENT_DYNAMIC(COfflineTimeTableDlg, CDialog)

COfflineTimeTableDlg::COfflineTimeTableDlg(CWnd* pParent /*=NULL*/)
: CDialog(COfflineTimeTableDlg::IDD, pParent)
{
	m_fAecS = 50;
	m_fFast2SlowS = 40;
	m_fSlow2StopS = 10;
	m_fFastV = 500;
	m_fSlowV = 200;
	m_nAecNum = 100;
	m_nFast2SlowNum = 80;
	m_nSlow2StopNum = 20;
	m_nTimeUnit = 100;
	m_nMoveIndex = -1;
	m_nOperationCurve = offline_operation_curve_none;
}

COfflineTimeTableDlg::~COfflineTimeTableDlg()
{
}

void COfflineTimeTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SFOFFLINE_OPERATION, m_btnOperation);
	DDX_Control(pDX, IDC_PROGRESS_SFOFFLINE_DOWNLOAD, m_pgsDownload);
}


BEGIN_MESSAGE_MAP(COfflineTimeTableDlg, CDialog)
	ON_MESSAGE(WM_SFDOWNLOAD_PROGRESS, &COfflineTimeTableDlg::OnSFDownloadProgress)
	ON_BN_CLICKED(IDC_BUTTON_SFOFFLINE_OPERATION, &COfflineTimeTableDlg::OnBnClickedButtonSfofflineOperation)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SFOFFLINE_CALCULATETIME, &COfflineTimeTableDlg::OnBnClickedButtonSfofflineCalculatetime)
	ON_BN_CLICKED(IDOK, &COfflineTimeTableDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COfflineTimeTableDlg 消息处理程序

BOOL COfflineTimeTableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_nOperationType==0)
	{
		m_btnOperation.SetWindowText("新增时间表");
	}
	else
	{
		m_btnOperation.SetWindowText("修改时间表");
	}

	m_pgsDownload.ShowWindow(FALSE);

	SetTimeTableParameter();
	InitCurve();
	CalculateTime();

	m_pComPort = ((CSFBoardDlg*)m_pSFDlg)->m_pComPort;
	m_pComPort->m_pOfflineTimeTableDlg = this;
	
	if (m_nOperationType==1)
	{
		GetSpeedLevelInfo();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL COfflineTimeTableDlg::AddTimeTable(BOOL bRemind)
{
	// TODO: 在此添加控件通知处理程序代码
	if(!CreateTimeTable(m_bSpeedLevel,bRemind))
	{
		goto End;
	}
	
	if (!m_pComPort->servo_set_offline_timetable_paramater(m_bMotoWay,m_timeTableOffline,CCmdTimeOut::set_offline_timetable_paramater,FALSE))
	{
		AfxMessageBox("下载脱机时间表参数--失败");
		goto End;
	}

	m_pgsDownload.ShowWindow(TRUE);
	m_pgsDownload.SetRange(0,m_timeTableOffline.m_parameterOffline->m_dwAecNum+m_timeTableOffline.m_parameterOffline->m_dwFast2SlowNum+m_timeTableOffline.m_parameterOffline->m_dwSlow2StopNum);
	m_pgsDownload.SetPos(0);

	if (!m_pComPort->servo_set_offline_timetable_timevalue(m_bMotoWay,m_timeTableOffline,CCmdTimeOut::set_timetable_timevalue,FALSE))
	{
		AfxMessageBox("下载脱机时间表数据--失败");
		goto End;
	}
	
	if (bRemind==TRUE)
	{
		AfxMessageBox("下载时间表--成功");
	}
	m_pgsDownload.ShowWindow(FALSE);
	return TRUE;
End:
	m_pgsDownload.ShowWindow(FALSE);
	return FALSE;
}
void COfflineTimeTableDlg::OnBnClickedButtonSfofflineOperation()
{
	// TODO: 在此添加控件通知处理程序代码
	AddTimeTable(TRUE);
}

BOOL COfflineTimeTableDlg::GetTimeTableParameter()
{
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_AECDISTANCE))->GetWindowText(str);
	m_fAecS = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_F2SDISTANCE))->GetWindowText(str);
	m_fFast2SlowS = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_S2SDISTANCE))->GetWindowText(str);
	m_fSlow2StopS = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_FASTSPEED))->GetWindowText(str);
	m_fFastV = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_SLOWSPEED))->GetWindowText(str);
	m_fSlowV = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_AECNUM))->GetWindowText(str);
	m_nAecNum = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_F2SNUM))->GetWindowText(str);
	m_nFast2SlowNum = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_S2SNUM))->GetWindowText(str);
	m_nSlow2StopNum = atof(str);

	return TRUE;
}

void COfflineTimeTableDlg::SetTimeTableParameter()
{
	CString str;

	str.Format("%.2f",m_fAecS);
	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_AECDISTANCE))->SetWindowText(str);

	str.Format("%.2f",m_fFast2SlowS);
	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_F2SDISTANCE))->SetWindowText(str);

	str.Format("%.2f",m_fSlow2StopS);
	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_S2SDISTANCE))->SetWindowText(str);

	str.Format("%.2f",m_fFastV);
	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_FASTSPEED))->SetWindowText(str);

	str.Format("%.2f",m_fSlowV);
	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_SLOWSPEED))->SetWindowText(str);

	str.Format("%d",m_nAecNum);
	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_AECNUM))->SetWindowText(str);

	str.Format("%d",m_nFast2SlowNum);
	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_F2SNUM))->SetWindowText(str);

	str.Format("%d",m_nSlow2StopNum);
	((CWnd*)GetDlgItem(IDC_EDIT_SFOFFLINE_S2SNUM))->SetWindowText(str);
}

void COfflineTimeTableDlg::InitCurve()
{
	CPoint ptControl1,ptControl2;

	((CWnd*)GetDlgItem(IDC_STATIC_SFOFFLINE_CURVE))->GetWindowRect(&m_rcView);
	ScreenToClient(&m_rcView);

	m_ptCurveAecStart.x = m_rcView.left;
	m_ptCurveAecStart.y = m_rcView.bottom;
	m_ptCurveAecEnd.x = m_rcView.left + 220;
	m_ptCurveAecEnd.y = m_rcView.top;

	m_rcAec.top = m_ptCurveAecEnd.y;
	m_rcAec.bottom = m_ptCurveAecStart.y;
	m_rcAec.left = m_ptCurveAecStart.x;
	m_rcAec.right = m_ptCurveAecEnd.x;

	m_ptCurveFast2SlowStart.x = m_rcView.right - 250;
	m_ptCurveFast2SlowStart.y = m_rcView.top;
	m_ptCurveFast2SlowEnd.x = m_ptCurveFast2SlowStart.x + 132;
	m_ptCurveFast2SlowEnd.y = m_ptCurveFast2SlowStart.y + m_rcView.Height()*3/5;

	m_ptCurveSlow2StopStart.x = m_rcView.right - 90;
	m_ptCurveSlow2StopStart.y = m_ptCurveFast2SlowEnd.y;
	m_ptCurveSlow2StopEnd.x = m_rcView.right;
	m_ptCurveSlow2StopEnd.y = m_rcView.bottom;

	m_rcFast2Slow.top = m_ptCurveFast2SlowStart.y;
	m_rcFast2Slow.bottom = m_ptCurveFast2SlowEnd.y;
	m_rcFast2Slow.left = m_ptCurveFast2SlowStart.x;
	m_rcFast2Slow.right = m_ptCurveFast2SlowEnd.x;

	m_rcSlow2Stop.top = m_ptCurveSlow2StopStart.y;
	m_rcSlow2Stop.bottom = m_ptCurveSlow2StopEnd.y;
	m_rcSlow2Stop.left = m_ptCurveSlow2StopStart.x;
	m_rcSlow2Stop.right = m_ptCurveSlow2StopEnd.x;

	ptControl1.x = m_ptCurveAecStart.x + m_rcAec.Width()/3;
	ptControl1.y = m_ptCurveAecStart.y - m_rcAec.Height()/3;

	ptControl2.x = m_ptCurveAecStart.x + m_rcAec.Width()/3*2;
	ptControl2.y = m_ptCurveAecStart.y - m_rcAec.Height()/3*2;

	m_AecControlPoints.RemoveAll();
	m_AecControlPoints.Add(m_ptCurveAecStart);
	m_AecControlPoints.Add(ptControl1);
	m_AecControlPoints.Add(ptControl2);
	m_AecControlPoints.Add(m_ptCurveAecEnd);

	ptControl1.x = m_ptCurveFast2SlowStart.x + m_rcFast2Slow.Width()/3;
	ptControl1.y = m_ptCurveFast2SlowStart.y + m_rcFast2Slow.Height()/3;

	ptControl2.x = m_ptCurveFast2SlowStart.x + m_rcFast2Slow.Width()/3*2;
	ptControl2.y = m_ptCurveFast2SlowStart.y + m_rcFast2Slow.Height()/3*2;

	m_Fast2SlowControlPoints.RemoveAll();
	m_Fast2SlowControlPoints.Add(m_ptCurveFast2SlowStart);
	m_Fast2SlowControlPoints.Add(ptControl1);
	m_Fast2SlowControlPoints.Add(ptControl2);
	m_Fast2SlowControlPoints.Add(m_ptCurveFast2SlowEnd);

	ptControl1.x = m_ptCurveSlow2StopStart.x + m_rcSlow2Stop.Width()/3;
	ptControl1.y = m_ptCurveSlow2StopStart.y + m_rcSlow2Stop.Height()/3;

	ptControl2.x = m_ptCurveSlow2StopStart.x + m_rcSlow2Stop.Width()/3*2;
	ptControl2.y = m_ptCurveSlow2StopStart.y + m_rcSlow2Stop.Height()/3*2;

	m_Slow2StopControlPoints.RemoveAll();
	m_Slow2StopControlPoints.Add(m_ptCurveSlow2StopStart);
	m_Slow2StopControlPoints.Add(ptControl1);
	m_Slow2StopControlPoints.Add(ptControl2);
	m_Slow2StopControlPoints.Add(m_ptCurveSlow2StopEnd);
}

int COfflineTimeTableDlg::IsInsideControlPoint(POINT& point, int& nOperationCurve)
{
	int count;

	count = m_AecControlPoints.GetSize();

	if(count>0)	
	{
		for(int i=1; i<count-1; i++)
		{
			if(Distance(m_AecControlPoints[i], point)<5)
			{
				nOperationCurve = offline_operation_curve_aec;
				return i;
			}
		}
	}

	count = m_Fast2SlowControlPoints.GetSize();

	if(count>0)	
	{
		for(int i=1; i<count-1; i++)
		{
			if(Distance(m_Fast2SlowControlPoints[i], point)<5)
			{
				nOperationCurve = offline_operation_curve_fast2slow;
				return i;
			}
		}
	}

	count = m_Slow2StopControlPoints.GetSize();

	if(count>0)	
	{
		for(int i=1; i<count-1; i++)
		{
			if(Distance(m_Slow2StopControlPoints[i], point)<5)
			{
				nOperationCurve = offline_operation_curve_slow2stop;
				return i;
			}
		}
	}
	return -1;
}

void COfflineTimeTableDlg::DrawCurveAec(CDC* pDC)
{
	if (m_AecControlPoints.GetSize()>1)
	{	
		Spline spline(m_AecControlPoints.GetData(), m_AecControlPoints.GetSize());
		spline.Generate();

		m_AecCurvePoints.SetSize(spline.GetCurveCount(DLG_CURVE_DIV_FACTOR));
		int PointCount = 0;
		spline.GetCurve(m_AecCurvePoints.GetData(), PointCount, DLG_CURVE_DIV_FACTOR);

		RestrictAecCurve(m_AecCurvePoints.GetData(),PointCount, &m_rcAec, &m_ptMoveNow, &m_ptMoveLast);

		//FillRect(pDC->m_hDC, &m_rcAec, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		//pDC->FillSolidRect(&m_rcAec,afxGlobalData.clrBarFace);

		for(int i=1; i<m_AecControlPoints.GetSize()-1; i++)
		{
			DrawCross(m_rcAec, m_AecControlPoints[i], RGB(255,0,0), 4, pDC->m_hDC);
		}

		if (m_AecCurvePoints.GetSize()>1)
		{
			//Polyline(pDC->m_hDC, m_AecCurvePoints.GetData(), m_AecCurvePoints.GetSize());
			DrawCurve(m_AecCurvePoints.GetData(), m_AecCurvePoints.GetSize(),RGB(0,0,0),pDC->m_hDC);
		}
	}
}

void COfflineTimeTableDlg::DrawCurveFast2Slow(CDC* pDC)
{
	if (m_Fast2SlowControlPoints.GetSize()>1)
	{
		Spline spline(m_Fast2SlowControlPoints.GetData(), m_Fast2SlowControlPoints.GetSize());
		spline.Generate();

		m_Fast2SlowCurvePoints.SetSize(spline.GetCurveCount(DLG_CURVE_DIV_FACTOR));
		int PointCount = 0;
		spline.GetCurve(m_Fast2SlowCurvePoints.GetData(), PointCount, DLG_CURVE_DIV_FACTOR);

		RestrictDecCurve(m_Fast2SlowCurvePoints.GetData(), PointCount, &m_rcFast2Slow, &m_ptMoveNow, &m_ptMoveLast);

		//FillRect(pDC->m_hDC, &m_rcFast2Slow, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		//pDC->FillSolidRect(&m_rcFast2Slow,afxGlobalData.clrBarFace);

		for(int i=1; i<m_Fast2SlowControlPoints.GetSize()-1; i++)
		{
			DrawCross(m_rcFast2Slow, m_Fast2SlowControlPoints[i], RGB(255,0,0), 4, pDC->m_hDC);
		}

		if (m_Fast2SlowCurvePoints.GetSize()>1)
		{
			//Polyline(pDC->m_hDC, m_Fast2SlowCurvePoints.GetData(), m_Fast2SlowCurvePoints.GetSize());
			DrawCurve(m_Fast2SlowCurvePoints.GetData(), m_Fast2SlowCurvePoints.GetSize(),RGB(0,0,0),pDC->m_hDC);
		}
	}
}

void COfflineTimeTableDlg::DrawCurveSlow2Stop(CDC* pDC)
{
	if (m_Slow2StopControlPoints.GetSize()>1)
	{
		Spline spline(m_Slow2StopControlPoints.GetData(), m_Slow2StopControlPoints.GetSize());
		spline.Generate();

		m_Slow2StopCurvePoints.SetSize(spline.GetCurveCount(DLG_CURVE_DIV_FACTOR));
		int PointCount = 0;
		spline.GetCurve(m_Slow2StopCurvePoints.GetData(), PointCount, DLG_CURVE_DIV_FACTOR);

		RestrictDecCurve(m_Slow2StopCurvePoints.GetData(), PointCount, &m_rcSlow2Stop, &m_ptMoveNow, &m_ptMoveLast);

		//FillRect(pDC->m_hDC, &m_rcSlow2Stop, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		//pDC->FillSolidRect(&m_rcSlow2Stop,afxGlobalData.clrBarFace);

		for(int i=1; i<m_Slow2StopControlPoints.GetSize()-1; i++)
		{
			DrawCross(m_rcSlow2Stop, m_Slow2StopControlPoints[i], RGB(255,0,0), 4, pDC->m_hDC);
		}

		if (m_Slow2StopCurvePoints.GetSize()>1)
		{
			//Polyline(pDC->m_hDC, m_Slow2StopCurvePoints.GetData(), m_Slow2StopCurvePoints.GetSize());
			DrawCurve(m_Slow2StopCurvePoints.GetData(), m_Slow2StopCurvePoints.GetSize(),RGB(0,0,0),pDC->m_hDC);
		}
	}
}

void COfflineTimeTableDlg::DrawCurveArea(CDC* pDC)
{
	DrawCurveAec(pDC);
	DrawCurveFast2Slow(pDC);
	DrawCurveSlow2Stop(pDC);
}

void COfflineTimeTableDlg::DrawAxis(CDC* pDC)
{
	//坐标轴
	CPoint a,b;
	a.x = m_rcView.left;
	a.y = m_rcView.bottom;
	b.x = a.x;
	b.y = a.y - m_rcView.Height() - 10;
	pDC->MoveTo(a);
	pDC->LineTo(b);

	a.x = b.x - 5;
	a.y = b.y + 5;
	pDC->MoveTo(b);
	pDC->LineTo(a);

	a.x = b.x + 5;
	a.y = b.y + 5;
	pDC->MoveTo(b);
	pDC->LineTo(a);

	a.x = m_rcView.left;
	a.y = m_rcView.bottom;
	b.x = a.x + m_rcView.Width() + 10;
	b.y = a.y;
	pDC->MoveTo(a);
	pDC->LineTo(b);

	a.x = b.x - 5;
	a.y = b.y - 5;
	pDC->MoveTo(b);
	pDC->LineTo(a);

	a.x = b.x - 5;
	a.y = b.y + 5;
	pDC->MoveTo(b);
	pDC->LineTo(a);

	//快速
	pDC->MoveTo(m_ptCurveAecEnd);
	pDC->LineTo(m_ptCurveFast2SlowStart);

	//慢速
	pDC->MoveTo(m_ptCurveFast2SlowEnd);
	pDC->LineTo(m_ptCurveSlow2StopStart);

	//右边
	a.x = m_ptCurveSlow2StopEnd.x;
	a.y = m_ptCurveFast2SlowStart.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveSlow2StopEnd);

	//虚线
	CPen cp;
	CPen* pOldPen;
	cp.CreatePen(PS_DOT, 0, RGB(0,0,0));
	pOldPen = pDC->SelectObject(&cp);

	a.x = m_ptCurveAecStart.x;
	a.y = m_ptCurveAecEnd.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveAecEnd);

	a.x = m_ptCurveAecEnd.x;
	a.y = m_ptCurveAecStart.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveAecEnd);

	a.x = m_ptCurveFast2SlowStart.x;
	a.y = m_ptCurveSlow2StopEnd.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveFast2SlowStart);

	a.x = m_ptCurveSlow2StopEnd.x;
	a.y = m_ptCurveFast2SlowStart.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveFast2SlowStart);

	a.x = m_ptCurveFast2SlowStart.x;
	a.y = m_ptCurveFast2SlowEnd.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveFast2SlowEnd);

	a.x = m_ptCurveFast2SlowEnd.x;
	a.y = m_ptCurveFast2SlowStart.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveFast2SlowEnd);

	a.x = m_ptCurveSlow2StopStart.x;
	a.y = m_ptCurveSlow2StopEnd.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveSlow2StopStart);

	a.x = m_ptCurveSlow2StopEnd.x;
	a.y = m_ptCurveSlow2StopStart.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveSlow2StopStart);

	pDC->SelectObject(pOldPen);
	cp.DeleteObject();
}

void COfflineTimeTableDlg::DrawCurve(const POINT *point,int count, COLORREF rgbColor,HDC hDC)
{
	HPEN hPen,oldPen;
	hPen = CreatePen(PS_SOLID,1,rgbColor);
	oldPen = (HPEN)SelectObject(hDC,hPen);

	Polyline(hDC, point, count);

	SelectObject(hDC,oldPen);
	DeleteObject(hPen);
}

void COfflineTimeTableDlg::DrawCross(RECT rect, POINT point, COLORREF rgbColor, int extent, HDC hDC)
{

	HPEN hPen,oldPen;
	hPen = CreatePen(PS_SOLID,1,rgbColor);
	oldPen = (HPEN)SelectObject(hDC,hPen);

	//横线
	if (abs(point.x - rect.left)<extent)
	{
		int ex = abs(point.x - rect.left);
		MoveToEx(hDC,point.x - ex, point.y,NULL);
		LineTo(hDC, point.x, point.y);
	}
	else
	{
		MoveToEx(hDC,point.x - extent, point.y,NULL);
		LineTo(hDC, point.x, point.y);
	}

	if (abs(point.x - rect.right)<extent)
	{
		int ex = abs(point.x - rect.right);
		MoveToEx(hDC,point.x + ex, point.y,NULL);
		LineTo(hDC, point.x, point.y);
	}
	else
	{
		MoveToEx(hDC,point.x + extent, point.y,NULL);
		LineTo(hDC, point.x, point.y);
	}

	//竖线
	if (abs(point.y - rect.top)<extent)
	{
		int ex = abs(point.y - rect.top);
		MoveToEx(hDC,point.x, point.y -ex,NULL);
		LineTo(hDC, point.x, point.y);
	}
	else
	{
		MoveToEx(hDC, point.x, point.y - extent,NULL);
		LineTo(hDC,point.x, point.y);
	}

	if (abs(point.y - rect.bottom)<extent)
	{
		int ex = abs(point.y - rect.bottom);
		MoveToEx(hDC,point.x, point.y + ex,NULL);
		LineTo(hDC, point.x, point.y);
	}
	else
	{
		MoveToEx(hDC, point.x, point.y + extent,NULL);
		LineTo(hDC,point.x, point.y);
	}

	SelectObject(hDC,oldPen);
	DeleteObject(hPen);
}

BOOL COfflineTimeTableDlg::MoveCurve()
{
	switch(m_nOperationCurve)
	{
	case offline_operation_curve_aec:
		{
			m_AecControlPoints[m_nMoveIndex] = m_ptMoveNow;

			CRect rcTemp = m_rcAec;
			ClientToScreen(&rcTemp);
			ClipCursor(&rcTemp);

			if (m_AecControlPoints[m_nMoveIndex].x>m_AecControlPoints[m_nMoveIndex+1].x||
				m_AecControlPoints[m_nMoveIndex].x<m_AecControlPoints[m_nMoveIndex-1].x||
				m_AecControlPoints[m_nMoveIndex].y<m_AecControlPoints[m_nMoveIndex+1].y||
				m_AecControlPoints[m_nMoveIndex].y>m_AecControlPoints[m_nMoveIndex-1].y||
				(m_AecControlPoints[m_nMoveIndex].x==m_AecControlPoints[m_nMoveIndex+1].x && m_AecControlPoints[m_nMoveIndex].y==m_AecControlPoints[m_nMoveIndex+1].y)||
				(m_AecControlPoints[m_nMoveIndex].x==m_AecControlPoints[m_nMoveIndex-1].x && m_AecControlPoints[m_nMoveIndex].y==m_AecControlPoints[m_nMoveIndex-1].y))
			{
				m_ptMoveNow = m_ptMoveLast;
				m_AecControlPoints[m_nMoveIndex] = m_ptMoveNow;
				return FALSE;
			}
		}
		break;
	case offline_operation_curve_fast2slow:
		{
			m_Fast2SlowControlPoints[m_nMoveIndex] = m_ptMoveNow;

			CRect rcTemp = m_rcFast2Slow;
			ClientToScreen(&rcTemp);
			ClipCursor(&rcTemp);

			if (m_Fast2SlowControlPoints[m_nMoveIndex].x>m_Fast2SlowControlPoints[m_nMoveIndex+1].x||
				m_Fast2SlowControlPoints[m_nMoveIndex].x<m_Fast2SlowControlPoints[m_nMoveIndex-1].x||
				m_Fast2SlowControlPoints[m_nMoveIndex].y>m_Fast2SlowControlPoints[m_nMoveIndex+1].y||
				m_Fast2SlowControlPoints[m_nMoveIndex].y<m_Fast2SlowControlPoints[m_nMoveIndex-1].y||
				(m_Fast2SlowControlPoints[m_nMoveIndex].x==m_Fast2SlowControlPoints[m_nMoveIndex+1].x && m_Fast2SlowControlPoints[m_nMoveIndex].y==m_Fast2SlowControlPoints[m_nMoveIndex+1].y)||
				(m_Fast2SlowControlPoints[m_nMoveIndex].x==m_Fast2SlowControlPoints[m_nMoveIndex-1].x && m_Fast2SlowControlPoints[m_nMoveIndex].y==m_Fast2SlowControlPoints[m_nMoveIndex-1].y))
			{
				m_ptMoveNow = m_ptMoveLast;
				m_Fast2SlowControlPoints[m_nMoveIndex] = m_ptMoveNow;
				return FALSE;
			}
		}
		break;
	case offline_operation_curve_slow2stop:
		{
			m_Slow2StopControlPoints[m_nMoveIndex] = m_ptMoveNow;

			CRect rcTemp = m_rcSlow2Stop;
			ClientToScreen(&rcTemp);
			ClipCursor(&rcTemp);

			if (m_Slow2StopControlPoints[m_nMoveIndex].x>m_Slow2StopControlPoints[m_nMoveIndex+1].x||
				m_Slow2StopControlPoints[m_nMoveIndex].x<m_Slow2StopControlPoints[m_nMoveIndex-1].x||
				m_Slow2StopControlPoints[m_nMoveIndex].y>m_Slow2StopControlPoints[m_nMoveIndex+1].y||
				m_Slow2StopControlPoints[m_nMoveIndex].y<m_Slow2StopControlPoints[m_nMoveIndex-1].y||
				(m_Slow2StopControlPoints[m_nMoveIndex].x==m_Slow2StopControlPoints[m_nMoveIndex+1].x && m_Slow2StopControlPoints[m_nMoveIndex].y==m_Slow2StopControlPoints[m_nMoveIndex+1].y)||
				(m_Slow2StopControlPoints[m_nMoveIndex].x==m_Slow2StopControlPoints[m_nMoveIndex-1].x && m_Slow2StopControlPoints[m_nMoveIndex].y==m_Slow2StopControlPoints[m_nMoveIndex-1].y))
			{
				m_ptMoveNow = m_ptMoveLast;
				m_Slow2StopControlPoints[m_nMoveIndex] = m_ptMoveNow;
				return FALSE;
			}
		}
		break;
	}
	return TRUE;
}
void COfflineTimeTableDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nMoveIndex>=0)
	{
		m_ptMoveNow = point;

		CDC* pDC = GetDC();
		CDC MemDC;
		CBitmap MemBitmap;

		MemDC.CreateCompatibleDC(pDC);
		MemBitmap.CreateCompatibleBitmap(pDC, m_rcView.left + m_rcView.Width(), m_rcView.top + m_rcView.Height());
		MemDC.SelectObject(&MemBitmap);

		MemDC.FillSolidRect(m_rcView.left,m_rcView.top,m_rcView.Width(),m_rcView.Height(),afxGlobalData.clrBarFace);
		FillRect(MemDC.m_hDC, &m_rcAec, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		FillRect(MemDC.m_hDC, &m_rcFast2Slow, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		FillRect(MemDC.m_hDC, &m_rcSlow2Stop, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

		MoveCurve();

		DrawCurveArea(&MemDC);
		DrawAxis(&MemDC);

		pDC->BitBlt(m_rcView.left,m_rcView.top,m_rcView.Width(),m_rcView.Height(),&MemDC,m_rcView.left,m_rcView.top,SRCCOPY);
		m_ptMoveLast = m_ptMoveNow;
	}
	CDialog::OnMouseMove(nFlags, point);
}

void COfflineTimeTableDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nOperationCurve = offline_operation_curve_none;
	m_nMoveIndex = -1;

	int nMoveIndex = IsInsideControlPoint(point, m_nOperationCurve);

	if(nMoveIndex >= 0)
	{
		m_nMoveIndex = nMoveIndex;
		m_ptMoveLast = point;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void COfflineTimeTableDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nMoveIndex>=0)
	{
		CRect rcTemp;

		switch(m_nOperationCurve)
		{
		case offline_operation_curve_aec:
			rcTemp = m_rcAec;
			break;
		case offline_operation_curve_fast2slow:
			rcTemp = m_rcFast2Slow;
			break;
		case offline_operation_curve_slow2stop:
			rcTemp = m_rcSlow2Stop;
			break;
		}
		InvalidateRect(&rcTemp);

		ClipCursor(NULL);
		CalculateTime();
		m_nMoveIndex = -1;
		m_nOperationCurve = offline_operation_curve_none;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void COfflineTimeTableDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint ad,ct1,ct2;
	CDC* pDC = GetDC();

	if(PtInRect(&m_rcAec,point))
	{
		for(int i=1; i<m_AecCurvePoints.GetSize()-1; i++)
		{
			if(Distance(m_AecCurvePoints[i], point)<5)
			{
				ad = m_AecCurvePoints.GetAt(i);

				for (int i=0; i<m_AecControlPoints.GetSize()-1; i++)
				{
					ct1 = m_AecControlPoints.GetAt(i);
					ct2 = m_AecControlPoints.GetAt(i+1);
					if (ad.x>ct1.x && ad.y<ct1.y && ad.x<ct2.x && ad.y>ct2.y)
					{
						if (m_AecControlPoints.GetCount()>=dlg_control_point_max)
						{
							AfxMessageBox("控制点小于等于10个");
							return;
						}
						m_AecControlPoints.InsertAt(i+1, ad);

						InvalidateRect(m_rcAec);
						return;
					}
				}
			}
		}
	}
	if(PtInRect(&m_rcFast2Slow,point))
	{
		for(int i=1; i<m_Fast2SlowCurvePoints.GetSize()-1; i++)
		{
			if(Distance(m_Fast2SlowCurvePoints[i], point)<5)
			{
				ad = m_Fast2SlowCurvePoints.GetAt(i);

				for (int i=0; i<m_Fast2SlowControlPoints.GetSize()-1; i++)
				{
					ct1 = m_Fast2SlowControlPoints.GetAt(i);
					ct2 = m_Fast2SlowControlPoints.GetAt(i+1);
					if (ad.x>ct1.x && ad.y>ct1.y && ad.x<ct2.x && ad.y<ct2.y)
					{
						if (m_Fast2SlowControlPoints.GetCount()>=dlg_control_point_max)
						{
							AfxMessageBox("控制点小于等于10个");
							return;
						}
						m_Fast2SlowControlPoints.InsertAt(i+1, ad);

						InvalidateRect(m_rcFast2Slow);
						return;
					}
				}
			}
		}
	}
	if(PtInRect(&m_rcSlow2Stop,point))
	{
		for(int i=1; i<m_Slow2StopCurvePoints.GetSize()-1; i++)
		{
			if(Distance(m_Slow2StopCurvePoints[i], point)<5)
			{
				ad = m_Slow2StopCurvePoints.GetAt(i);

				for (int i=0; i<m_Slow2StopControlPoints.GetSize()-1; i++)
				{
					ct1 = m_Slow2StopControlPoints.GetAt(i);
					ct2 = m_Slow2StopControlPoints.GetAt(i+1);
					if (ad.x>ct1.x && ad.y>ct1.y && ad.x<ct2.x && ad.y<ct2.y)
					{
						if (m_Slow2StopControlPoints.GetCount()>=dlg_control_point_max)
						{
							AfxMessageBox("控制点小于等于10个");
							return;
						}
						m_Slow2StopControlPoints.InsertAt(i+1, ad);

						InvalidateRect(m_rcSlow2Stop);
						return;
					}
				}
			}
		}
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void COfflineTimeTableDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC* pDC = GetDC();

	if(PtInRect(&m_rcAec,point))
	{
		for(int i=1; i<m_AecControlPoints.GetSize()-1; i++)
		{
			if(Distance(m_AecControlPoints[i], point)<5)
			{
				m_AecControlPoints.RemoveAt(i);

				InvalidateRect(m_rcAec);
				return;
			}
		}
	}
	if(PtInRect(&m_rcFast2Slow,point))
	{
		for(int i=1; i<m_Fast2SlowControlPoints.GetSize()-1; i++)
		{
			if(Distance(m_Fast2SlowControlPoints[i], point)<5)
			{
				m_Fast2SlowControlPoints.RemoveAt(i);

				InvalidateRect(m_rcFast2Slow);
				return;
			}
		}
	}
	if(PtInRect(&m_rcSlow2Stop,point))
	{
		for(int i=1; i<m_Slow2StopControlPoints.GetSize()-1; i++)
		{
			if(Distance(m_Slow2StopControlPoints[i], point)<5)
			{
				m_Slow2StopControlPoints.RemoveAt(i);

				InvalidateRect(m_rcSlow2Stop);
				return;
			}
		}
	}
	CDialog::OnRButtonDblClk(nFlags, point);
}

void COfflineTimeTableDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	if (m_nMoveIndex==-1)
	{
		CDC* pDC = GetDC();
		CDC MemDC;
		CBitmap MemBitmap;

		MemDC.CreateCompatibleDC(pDC);
		MemBitmap.CreateCompatibleBitmap(pDC, m_rcView.left + m_rcView.Width(), m_rcView.top + m_rcView.Height());
		MemDC.SelectObject(&MemBitmap);

		MemDC.FillSolidRect(m_rcView.left,m_rcView.top,m_rcView.Width(),m_rcView.Height(),afxGlobalData.clrBarFace);
		FillRect(MemDC.m_hDC, &m_rcAec, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		FillRect(MemDC.m_hDC, &m_rcFast2Slow, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		FillRect(MemDC.m_hDC, &m_rcSlow2Stop, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

		DrawCurveArea(&MemDC);

		pDC->BitBlt(m_rcView.left,m_rcView.top,m_rcView.Width(),m_rcView.Height(),&MemDC,m_rcView.left,m_rcView.top,SRCCOPY);

		DrawAxis(pDC);
	}
	// 不为绘图消息调用 CDialog::OnPaint()
}

void COfflineTimeTableDlg::CalculateTime()
{
	CPOINTArray		tableControlPointsArray;
	CPOINTArray		tableCurvePointsArray;
	CWordArray		tableTimeArray;
	CPoint			tableControlPoint;
	CRect			tableRect;
	float fTotalTime;
	float fV;		
	float fPulseS = 0;
	int nLastX,nPointCount = 0;
	int nTimeValue;
	int nTimeNum;
	WORD wPulse;
	DWORD dwTimeTemp;
	int nTimeTemp;
	CString str;

	SetCursor(LoadCursor(NULL,IDC_WAIT));

	if (!GetTimeTableParameter())
	{
		return;
	}

	if (m_nOperationCurve==offline_operation_curve_none || m_nOperationCurve==offline_operation_curve_aec)
	{
		//加速段
		tableControlPointsArray.RemoveAll();
		tableCurvePointsArray.RemoveAll();

		tableRect.left = m_ptCurveAecStart.x;
		tableRect.bottom = m_ptCurveAecStart.y;
		tableRect.right = m_ptCurveAecStart.x + m_nAecNum - 1;
		tableRect.top = m_ptCurveAecStart.y - ((float)(m_rcAec.Height()))/((float)(m_rcAec.Width()))*((float)(m_nAecNum - 1));

		for (int i=0; i<m_AecControlPoints.GetCount(); i++)
		{
			if (abs(m_AecControlPoints.GetAt(i).x - m_ptCurveAecStart.x)==0)
			{
				tableControlPoint.x = m_ptCurveAecStart.x;
			}
			else
			{
				tableControlPoint.x = m_ptCurveAecStart.x + ((float)(abs(m_AecControlPoints.GetAt(i).x - m_ptCurveAecStart.x)))/((float)(m_rcAec.Width()))*tableRect.Width();
			}

			if (abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y)==0)
			{
				tableControlPoint.y = m_ptCurveAecStart.y;
			}
			else
			{
				tableControlPoint.y = m_ptCurveAecStart.y - ((float)(abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y)))/((float)(m_rcAec.Height()))*tableRect.Height();
			}
			tableControlPointsArray.Add(tableControlPoint);
		}	

		Spline splineAec(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
		splineAec.Generate();

		tableCurvePointsArray.SetSize(splineAec.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
		nPointCount = 0;
		splineAec.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

		RestrictAecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

		//发时间 最小单位 0.1us 最大到5000us
		tableTimeArray.RemoveAll();
		fPulseS = m_fAecS/(float)m_nAecNum;
		nLastX = -1;
		fTotalTime = 0;

		wPulse = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

		for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
		{	
			if (tableCurvePointsArray.GetAt(i).x==nLastX)
			{
				continue;
			}

			fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * m_fFastV;

			if (fV==0)
			{
				nTimeValue = WORD_MAX;
			}
			else
			{
				ASSERT(fV>0 && fV<=m_fFastV);
				dwTimeTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fV);
				nTimeValue = TIME_CONVERT((float)1/float(dwTimeTemp));
			}

			if (nTimeValue>WORD_MAX)
			{
				nTimeValue = WORD_MAX;
			}

			tableTimeArray.Add((WORD)nTimeValue);

			nLastX = tableCurvePointsArray.GetAt(i).x;
		}
		nTimeNum = tableTimeArray.GetCount();

		for (int i=0; i<nTimeNum; i++)
		{
			if (tableTimeArray.GetAt(i)<WORD_MAX)
			{
				nTimeTemp = tableTimeArray.GetAt(i);
				break;
			}
		}

		for (int i=0; i<nTimeNum; i++)
		{
			if (tableTimeArray.GetAt(i)>=WORD_MAX)
			{
				tableTimeArray.SetAt(i,nTimeTemp);
			}

			fTotalTime += (float)tableTimeArray.GetAt(i)/(float)1000 * wPulse;
		}
		str.Format("%.2f",fTotalTime);
		((CWnd*)GetDlgItem(IDC_STATIC_SFOFFLINE_AECTIME))->SetWindowText(str);
	}

	if (m_nOperationCurve==offline_operation_curve_none || m_nOperationCurve==offline_operation_curve_fast2slow)
	{
		//减速（快到慢）
		tableControlPointsArray.RemoveAll();
		tableCurvePointsArray.RemoveAll();

		tableRect.left = m_ptCurveFast2SlowStart.x;
		tableRect.top = m_ptCurveFast2SlowStart.y;
		tableRect.right = m_ptCurveFast2SlowStart.x + m_nFast2SlowNum - 1;
		tableRect.bottom = m_ptCurveFast2SlowStart.y + ((float)(m_rcFast2Slow.Height()))/((float)(m_rcFast2Slow.Width()))*((float)(m_nFast2SlowNum - 1));

		for (int i=0; i<m_Fast2SlowControlPoints.GetCount(); i++)
		{
			if (abs(m_Fast2SlowControlPoints.GetAt(i).x - m_ptCurveFast2SlowStart.x)==0)
			{
				tableControlPoint.x = m_ptCurveFast2SlowStart.x;
			}
			else
			{
				tableControlPoint.x = m_ptCurveFast2SlowStart.x + ((float)(abs(m_Fast2SlowControlPoints.GetAt(i).x - m_ptCurveFast2SlowStart.x)))/((float)(m_rcFast2Slow.Width()))*((float)tableRect.Width());
			}

			if (abs(m_Fast2SlowControlPoints.GetAt(i).y - m_ptCurveFast2SlowStart.y)==0)
			{
				tableControlPoint.y = m_ptCurveFast2SlowStart.y;
			}
			else
			{
				tableControlPoint.y = m_ptCurveFast2SlowStart.y + ((float)(abs(m_Fast2SlowControlPoints.GetAt(i).y - m_ptCurveFast2SlowStart.y)))/((float)(m_rcFast2Slow.Height()))*((float)tableRect.Height());
			}
			tableControlPointsArray.Add(tableControlPoint);
		}	

		Spline splineFast2Slow(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
		splineFast2Slow.Generate();

		tableCurvePointsArray.SetSize(splineFast2Slow.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
		nPointCount = 0;
		splineFast2Slow.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

		RestrictDecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

		//发时间 最小单位 0.1us 最大到5000us
		tableTimeArray.RemoveAll();
		fPulseS = m_fFast2SlowS/(float)m_nFast2SlowNum;
		nLastX = -1;
		fTotalTime = 0;

		wPulse = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

		for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
		{
			if (tableCurvePointsArray.GetAt(i).x==nLastX)
			{
				continue;
			}

			fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * (float(m_fFastV - m_fSlowV)) + m_fSlowV;

			ASSERT(fV>=m_fSlowV && fV<=m_fFastV);

			dwTimeTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fV);
			nTimeValue = TIME_CONVERT((float)1/float(dwTimeTemp));

			tableTimeArray.Add((WORD)nTimeValue);

			nLastX = tableCurvePointsArray.GetAt(i).x;
		}
		nTimeNum = tableTimeArray.GetCount();

		for (int i=0; i<nTimeNum; i++)
		{
			fTotalTime += (float)tableTimeArray.GetAt(i)/(float)1000 * wPulse;
		}

		str.Format("%.2f",fTotalTime);
		((CWnd*)GetDlgItem(IDC_STATIC_SFOFFLINE_F2STIME))->SetWindowText(str);
	}

	if (m_nOperationCurve==offline_operation_curve_none || m_nOperationCurve==offline_operation_curve_slow2stop)
	{
		//减速（慢到停）
		tableControlPointsArray.RemoveAll();
		tableCurvePointsArray.RemoveAll();

		tableRect.left = m_ptCurveSlow2StopStart.x;
		tableRect.top = m_ptCurveSlow2StopStart.y;
		tableRect.right = m_ptCurveSlow2StopStart.x + m_nSlow2StopNum - 1;
		tableRect.bottom = m_ptCurveSlow2StopStart.y + ((float)(m_rcSlow2Stop.Height()))/((float)(m_rcSlow2Stop.Width()))*((float)(m_nSlow2StopNum - 1));

		for (int i=0; i<m_Slow2StopControlPoints.GetCount(); i++)
		{
			if (abs(m_Slow2StopControlPoints.GetAt(i).x - m_ptCurveSlow2StopStart.x)==0)
			{
				tableControlPoint.x = m_ptCurveSlow2StopStart.x;
			}
			else
			{
				tableControlPoint.x = m_ptCurveSlow2StopStart.x + ((float)(abs(m_Slow2StopControlPoints.GetAt(i).x - m_ptCurveSlow2StopStart.x)))/((float)(m_rcSlow2Stop.Width()))*((float)tableRect.Width());
			}

			if (abs(m_Slow2StopControlPoints.GetAt(i).y - m_ptCurveSlow2StopStart.y)==0)
			{
				tableControlPoint.y = m_ptCurveSlow2StopStart.y;
			}
			else
			{
				tableControlPoint.y = m_ptCurveSlow2StopStart.y + ((float)(abs(m_Slow2StopControlPoints.GetAt(i).y - m_ptCurveSlow2StopStart.y)))/((float)(m_rcSlow2Stop.Height()))*((float)tableRect.Height());
			}
			tableControlPointsArray.Add(tableControlPoint);
		}	

		Spline splineSlow2Stop(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
		splineSlow2Stop.Generate();

		tableCurvePointsArray.SetSize(splineSlow2Stop.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
		nPointCount = 0;
		splineSlow2Stop.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

		RestrictDecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

		//发时间 最小单位 0.1us 最大到5000us
		tableTimeArray.RemoveAll();
		fPulseS = m_fSlow2StopS/(float)m_nSlow2StopNum;
		nLastX = -1;
		fTotalTime = 0;

		wPulse = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

		for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
		{
			if (tableCurvePointsArray.GetAt(i).x==nLastX)
			{
				continue;
			}

			fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * m_fSlowV;

			if (fV==0)
			{
				nTimeValue = WORD_MAX;
			}
			else
			{
				ASSERT(fV>0 && fV<=m_fSlowV);
				dwTimeTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fV);
				nTimeValue = TIME_CONVERT((float)1/float(dwTimeTemp));
			}

			if (nTimeValue>WORD_MAX)
			{
				nTimeValue = WORD_MAX;
			}

			tableTimeArray.Add((WORD)nTimeValue);

			nLastX = tableCurvePointsArray.GetAt(i).x;
		}
		nTimeNum = tableTimeArray.GetCount();

		for (int i=nTimeNum-1; i>=0; i--)
		{
			if (tableTimeArray.GetAt(i)<WORD_MAX)
			{
				nTimeTemp = tableTimeArray.GetAt(i);
				break;
			}
		}

		for (int i=0; i<nTimeNum; i++)
		{
			if (tableTimeArray.GetAt(i)>=WORD_MAX)
			{
				tableTimeArray.SetAt(i,nTimeTemp);
			}
			fTotalTime += (float)tableTimeArray.GetAt(i)/(float)1000 * wPulse;
		}

		str.Format("%.2f",fTotalTime);
		((CWnd*)GetDlgItem(IDC_STATIC_SFOFFLINE_S2STIME))->SetWindowText(str);
	}
}
void COfflineTimeTableDlg::OnBnClickedButtonSfofflineCalculatetime()
{
	// TODO: 在此添加控件通知处理程序代码
	CalculateTime();
}

LONG COfflineTimeTableDlg::OnSFDownloadProgress(WPARAM wPALAM, LPARAM lPALAM)
{
	int nProgress = (int)wPALAM;
	m_pgsDownload.SetPos(nProgress);
	return 0;
}

void COfflineTimeTableDlg::InitTimeTable()
{
	m_timeTableOffline.m_bSpeedLevelOffline = 0xFF;

	m_parameterOffline.Init();

	m_timeTableOffline.m_parameterOffline = NULL;
	m_timeTableOffline.m_wAecTimeArray.RemoveAll();
	m_timeTableOffline.m_wFast2SlowTimeArray.RemoveAll();
	m_timeTableOffline.m_wSlow2StopTimeArray.RemoveAll();
}

BOOL COfflineTimeTableDlg::CreateTimeTable(BYTE bSpeedLevel,BOOL bRemind)
{
	CPOINTArray		tableControlPointsArray;
	CPOINTArray		tableCurvePointsArray;
	CWordArray		tableTimeArray;
	CDWordArray		tableRecordArray;
	CPoint			tableControlPoint;
	CRect			tableRect;
	float			fAecTime = 0,fF2STime = 0,fS2STime = 0;
	float			fV;		
	float			fPulseS = 0;
	int				nTimeValue,nTimeTemp;
	int				nTimeNum = 0,nPtRecordNum = 0;
	int				nPointCount = 0,nLastX = 0;
	WORD			wLo = 0,wHi = 0;
	DWORD			dwControlPoint = 0;
	DWORD			dwSpeedTemp,dwTimeTemp;
	float			Percentage = 0;
	CString			str;

	if (!GetTimeTableParameter())
	{
		return FALSE;
	}

	CString strFolder;
	strFolder = theApp.m_strAppPath + "TableValue";
	CreateDirectory(strFolder,NULL);

	COleDateTime dateTime = COleDateTime::GetCurrentTime();
	str = dateTime.Format("%Y-%m-%d-%H-%M-%S");
	CString strFile = strFolder + "\\OfflineTableValue-" + str + ".txt";
	CStdioFile tableFile(strFile,CFile::modeCreate | CFile::modeWrite | CFile::typeText);

	SetCursor(LoadCursor(NULL,IDC_WAIT));

	//脱机表
	InitTimeTable();

	m_timeTableOffline.m_bSpeedLevelOffline = bSpeedLevel;
	m_timeTableOffline.m_parameterOffline = &m_parameterOffline;

	m_parameterOffline.m_wTimeUnit = m_nTimeUnit;

	dwSpeedTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(m_fFastV);
	m_parameterOffline.m_dwFastV = TIME_CONVERT((float)1/(float)dwSpeedTemp);

	dwSpeedTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(m_fSlowV);
	m_parameterOffline.m_dwSlowV = TIME_CONVERT((float)1/(float)dwSpeedTemp);

	//加速段
	tableControlPointsArray.RemoveAll();
	tableCurvePointsArray.RemoveAll();

	tableRect.left = m_ptCurveAecStart.x;
	tableRect.bottom = m_ptCurveAecStart.y;
	tableRect.right = m_ptCurveAecStart.x + m_nAecNum - 1;
	tableRect.top = m_ptCurveAecStart.y - ((float)(m_rcAec.Height()))/((float)(m_rcAec.Width()))*((float)(m_nAecNum - 1));

	for (int i=0; i<m_AecControlPoints.GetCount(); i++)
	{
		if (abs(m_AecControlPoints.GetAt(i).x - m_ptCurveAecStart.x)==0)
		{
			tableControlPoint.x = m_ptCurveAecStart.x;
		}
		else
		{
			tableControlPoint.x = m_ptCurveAecStart.x + ((float)(abs(m_AecControlPoints.GetAt(i).x - m_ptCurveAecStart.x)))/((float)(m_rcAec.Width()))*tableRect.Width();
		}

		if (abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y)==0)
		{
			tableControlPoint.y = m_ptCurveAecStart.y;
		}
		else
		{
			tableControlPoint.y = m_ptCurveAecStart.y - ((float)(abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y)))/((float)(m_rcAec.Height()))*tableRect.Height();
		}
		tableControlPointsArray.Add(tableControlPoint);
	}	

	Spline splineAec(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
	splineAec.Generate();

	tableCurvePointsArray.SetSize(splineAec.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
	nPointCount = 0;
	splineAec.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

	RestrictAecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

	tableTimeArray.RemoveAll();
	tableRecordArray.RemoveAll();

	//发时间 最小单位 1us 最大到65535us
	fPulseS = m_fAecS/(float)m_nAecNum;
	nLastX = -1;

	m_parameterOffline.m_wAecSUnit = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

	for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
	{
		if (tableCurvePointsArray.GetAt(i).x==nLastX)
		{
			continue;
		}

		fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * m_fFastV;

		if (fV==0)
		{
			nTimeValue = WORD_MAX;
		}
		else
		{
			ASSERT(fV>0 && fV<=m_fFastV);
			dwTimeTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fV);
			nTimeValue = TIME_CONVERT((float)1/float(dwTimeTemp));
		}

		if (nTimeValue>WORD_MAX)
		{
			nTimeValue = WORD_MAX;
		}

		tableTimeArray.Add((WORD)nTimeValue);

		nLastX = tableCurvePointsArray.GetAt(i).x;
	}
	nTimeNum = tableTimeArray.GetCount();

	for (int i=0; i<nTimeNum; i++)
	{
		if (tableTimeArray.GetAt(i)<WORD_MAX)
		{
			nTimeTemp = tableTimeArray.GetAt(i);
			break;
		}
	}

	for (int i=0; i<nTimeNum; i++)
	{
		if (tableTimeArray.GetAt(i)>=WORD_MAX)
		{
			tableTimeArray.SetAt(i,nTimeTemp);
		}
		str.Format("AECT: %d\n", tableTimeArray.GetAt(i));
		tableFile.WriteString(str);

		fAecTime += (float)tableTimeArray.GetAt(i)/(float)1000 * m_parameterOffline.m_wAecSUnit;
	}

	ASSERT(nTimeNum==m_nAecNum);

	//特殊点
	if (m_AecControlPoints.GetCount()==2)
	{
		nPtRecordNum = 0;
	}
	else
	{
		for (int i=1;i<m_AecControlPoints.GetCount()-1;i++)
		{
			wLo = abs(m_AecControlPoints.GetAt(i).x - m_ptCurveAecStart.x);
			wHi = abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y);

			dwControlPoint = (DWORD(wHi & 0xFFFF)<<16) | (DWORD(wLo & 0xFFFF));
			tableRecordArray.Add(dwControlPoint);
		}
		nPtRecordNum = tableRecordArray.GetCount();
	}

	m_parameterOffline.m_dwAecNum = nTimeNum;
	m_parameterOffline.m_bAecRecordNum = nPtRecordNum;

	for (int i=0; i<nTimeNum; i++)
	{
		m_timeTableOffline.m_wAecTimeArray.Add(tableTimeArray.GetAt(i));
	}

	for (int i=0; i<nPtRecordNum; i++)
	{
		m_parameterOffline.m_dwAecRecordArray.Add(tableRecordArray.GetAt(i));
	}

	//减速（快到慢）
	tableControlPointsArray.RemoveAll();
	tableCurvePointsArray.RemoveAll();

	tableRect.left = m_ptCurveFast2SlowStart.x;
	tableRect.top = m_ptCurveFast2SlowStart.y;
	tableRect.right = m_ptCurveFast2SlowStart.x + m_nFast2SlowNum - 1;
	tableRect.bottom = m_ptCurveFast2SlowStart.y + ((float)(m_rcFast2Slow.Height()))/((float)(m_rcFast2Slow.Width()))*((float)(m_nFast2SlowNum - 1));

	for (int i=0; i<m_Fast2SlowControlPoints.GetCount(); i++)
	{
		if (abs(m_Fast2SlowControlPoints.GetAt(i).x - m_ptCurveFast2SlowStart.x)==0)
		{
			tableControlPoint.x = m_ptCurveFast2SlowStart.x;
		}
		else
		{
			tableControlPoint.x = m_ptCurveFast2SlowStart.x + ((float)(abs(m_Fast2SlowControlPoints.GetAt(i).x - m_ptCurveFast2SlowStart.x)))/((float)(m_rcFast2Slow.Width()))*((float)tableRect.Width());
		}

		if (abs(m_Fast2SlowControlPoints.GetAt(i).y - m_ptCurveFast2SlowStart.y)==0)
		{
			tableControlPoint.y = m_ptCurveFast2SlowStart.y;
		}
		else
		{
			tableControlPoint.y = m_ptCurveFast2SlowStart.y + ((float)(abs(m_Fast2SlowControlPoints.GetAt(i).y - m_ptCurveFast2SlowStart.y)))/((float)(m_rcFast2Slow.Height()))*((float)tableRect.Height());
		}
		tableControlPointsArray.Add(tableControlPoint);
	}	

	Spline splineFast2Slow(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
	splineFast2Slow.Generate();

	tableCurvePointsArray.SetSize(splineFast2Slow.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
	nPointCount = 0;
	splineFast2Slow.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

	RestrictDecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

	tableTimeArray.RemoveAll();
	tableRecordArray.RemoveAll();

	//发时间 最小单位 1us 最大到65535us
	fPulseS = m_fFast2SlowS/(float)m_nFast2SlowNum;
	nLastX = -1;

	m_parameterOffline.m_wFast2SlowSUnit = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

	for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
	{
		if (tableCurvePointsArray.GetAt(i).x==nLastX)
		{
			continue;
		}

		fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * (float(m_fFastV - m_fSlowV)) + m_fSlowV;

		ASSERT(fV>=m_fSlowV && fV<=m_fFastV);
		dwTimeTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fV);
		nTimeValue = TIME_CONVERT((float)1/float(dwTimeTemp));

		tableTimeArray.Add((WORD)nTimeValue);

		nLastX = tableCurvePointsArray.GetAt(i).x;
	}
	nTimeNum = tableTimeArray.GetCount();

	for (int i=0; i<nTimeNum; i++)
	{
		str.Format("F2ST: %d\n", tableTimeArray.GetAt(i));
		tableFile.WriteString(str);

		fF2STime += (float)tableTimeArray.GetAt(i)/(float)1000 * m_parameterOffline.m_wFast2SlowSUnit;
	}

	ASSERT(nTimeNum==m_nFast2SlowNum);

	//特殊点
	if (m_Fast2SlowControlPoints.GetCount()==2)
	{
		nPtRecordNum = 0;
	}
	else
	{
		for (int i=1;i<m_Fast2SlowControlPoints.GetCount()-1;i++)
		{		
			wLo = abs(m_Fast2SlowControlPoints.GetAt(i).x - m_ptCurveFast2SlowStart.x);
			wHi = abs(m_Fast2SlowControlPoints.GetAt(i).y - m_ptCurveFast2SlowStart.y);

			dwControlPoint = (DWORD(wHi & 0xFFFF)<<16) | (DWORD(wLo & 0xFFFF));
			tableRecordArray.Add(dwControlPoint);
		}
		nPtRecordNum = tableRecordArray.GetCount();
	}

	m_parameterOffline.m_dwFast2SlowNum = nTimeNum;
	m_parameterOffline.m_bFast2SlowRecordNum = nPtRecordNum;

	for (int i=0; i<nTimeNum; i++)
	{
		m_timeTableOffline.m_wFast2SlowTimeArray.Add(tableTimeArray.GetAt(i));
	}

	for (int i=0; i<nPtRecordNum; i++)
	{
		m_parameterOffline.m_dwFast2SlowRecordArray.Add(tableRecordArray.GetAt(i));
	}

	//减速（慢到停）
	tableControlPointsArray.RemoveAll();
	tableCurvePointsArray.RemoveAll();

	tableRect.left = m_ptCurveSlow2StopStart.x;
	tableRect.top = m_ptCurveSlow2StopStart.y;
	tableRect.right = m_ptCurveSlow2StopStart.x + m_nSlow2StopNum - 1;
	tableRect.bottom = m_ptCurveSlow2StopStart.y + ((float)(m_rcSlow2Stop.Height()))/((float)(m_rcSlow2Stop.Width()))*((float)(m_nSlow2StopNum - 1));

	for (int i=0; i<m_Slow2StopControlPoints.GetCount(); i++)
	{
		if (abs(m_Slow2StopControlPoints.GetAt(i).x - m_ptCurveSlow2StopStart.x)==0)
		{
			tableControlPoint.x = m_ptCurveSlow2StopStart.x;
		}
		else
		{
			tableControlPoint.x = m_ptCurveSlow2StopStart.x + ((float)(abs(m_Slow2StopControlPoints.GetAt(i).x - m_ptCurveSlow2StopStart.x)))/((float)(m_rcSlow2Stop.Width()))*((float)tableRect.Width());
		}

		if (abs(m_Slow2StopControlPoints.GetAt(i).y - m_ptCurveSlow2StopStart.y)==0)
		{
			tableControlPoint.y = m_ptCurveSlow2StopStart.y;
		}
		else
		{
			tableControlPoint.y = m_ptCurveSlow2StopStart.y + ((float)(abs(m_Slow2StopControlPoints.GetAt(i).y - m_ptCurveSlow2StopStart.y)))/((float)(m_rcSlow2Stop.Height()))*((float)tableRect.Height());
		}
		tableControlPointsArray.Add(tableControlPoint);
	}	

	Spline splineSlow2Stop(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
	splineSlow2Stop.Generate();

	tableCurvePointsArray.SetSize(splineSlow2Stop.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
	nPointCount = 0;
	splineSlow2Stop.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

	RestrictDecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

	tableTimeArray.RemoveAll();
	tableRecordArray.RemoveAll();

	//发时间 最小单位 1us 最大到65535us
	fPulseS = m_fSlow2StopS/(float)m_nSlow2StopNum;
	nLastX = -1;

	m_parameterOffline.m_wSlow2StopSUnit = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

	for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
	{
		if (tableCurvePointsArray.GetAt(i).x==nLastX)
		{
			continue;
		}

		fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * m_fSlowV;

		if (fV==0)
		{
			nTimeValue = WORD_MAX;
		}
		else
		{
			ASSERT(fV>0 && fV<=m_fSlowV);
			dwTimeTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fV);
			nTimeValue = TIME_CONVERT((float)1/float(dwTimeTemp));
		}

		if (nTimeValue>WORD_MAX)
		{
			nTimeValue = WORD_MAX;
		}

		tableTimeArray.Add((WORD)nTimeValue);

		nLastX = tableCurvePointsArray.GetAt(i).x;
	}
	nTimeNum = tableTimeArray.GetCount();

	for (int i=nTimeNum-1; i>=0; i--)
	{
		if (tableTimeArray.GetAt(i)<WORD_MAX)
		{
			nTimeTemp = tableTimeArray.GetAt(i);
			break;
		}
	}

	for (int i=0; i<nTimeNum; i++)
	{
		if (tableTimeArray.GetAt(i)>=WORD_MAX)
		{
			tableTimeArray.SetAt(i,nTimeTemp);
		}
		str.Format("S2ST: %d\n", tableTimeArray.GetAt(i));
		tableFile.WriteString(str);

		fS2STime += (float)tableTimeArray.GetAt(i)/(float)1000 * m_parameterOffline.m_wSlow2StopSUnit;
	}

	ASSERT(nTimeNum==m_nSlow2StopNum);

	//特殊点
	if (m_Slow2StopControlPoints.GetCount()==2)
	{
		nPtRecordNum = 0;
	}
	else
	{
		for (int i=1;i<m_Slow2StopControlPoints.GetCount()-1;i++)
		{
			wLo = abs(m_Slow2StopControlPoints.GetAt(i).x - m_ptCurveSlow2StopStart.x);
			wHi = abs(m_Slow2StopControlPoints.GetAt(i).y - m_ptCurveSlow2StopStart.y);

			dwControlPoint = (DWORD(wHi & 0xFFFF)<<16) | (DWORD(wLo & 0xFFFF));
			tableRecordArray.Add(dwControlPoint);
		}
		nPtRecordNum = tableRecordArray.GetCount();
	}

	m_parameterOffline.m_dwSlow2StopNum = nTimeNum;
	m_parameterOffline.m_bSlow2StopRecordNum = nPtRecordNum;

	for (int i=0; i<nTimeNum; i++)
	{
		m_timeTableOffline.m_wSlow2StopTimeArray.Add(tableTimeArray.GetAt(i));
	}

	for (int i=0; i<nPtRecordNum; i++)
	{
		m_parameterOffline.m_dwSlow2StopRecordArray.Add(tableRecordArray.GetAt(i));
	}

	SetCursor(LoadCursor(NULL,IDC_ARROW));

	if (bSpeedLevel==0xFF)
	{	
		str.Format("新增一档脱机时间表:\n\n加速时间:\t%.2f ms\n快到慢时间:\t%.2f ms\n慢到停时间:\t%.2f ms\n加速档格数:\t%d\n快到慢档格数:\t%d\n慢到停档格数:\t%d\n加速每格脉冲:\t%d\n快到慢每格脉冲:\t%d\n慢到停每格脉冲:\t%d\n快速发送时间值:\t%d us\n慢速发送时间值:\t%d us",  
			fAecTime, fF2STime, fS2STime, m_parameterOffline.m_dwAecNum,m_parameterOffline.m_dwFast2SlowNum,m_parameterOffline.m_dwSlow2StopNum,
			m_parameterOffline.m_wAecSUnit,m_parameterOffline.m_wFast2SlowSUnit,m_parameterOffline.m_wSlow2StopSUnit,m_parameterOffline.m_dwFastV,m_parameterOffline.m_dwSlowV);
	}
	else
	{
		str.Format("修改第 %d 档脱机时间表:\n\n加速时间:\t%.2f ms\n快到慢时间:\t%.2f ms\n慢到停时间:\t%.2f ms\n加速档格数:\t%d\n快到慢档格数:\t%d\n慢到停档格数:\t%d\n加速每格脉冲:\t%d\n快到慢每格脉冲:\t%d\n慢到停每格脉冲:\t%d\n快速发送时间值:\t%d us\n慢速发送时间值:\t%d us", 
			bSpeedLevel+1,fAecTime, fF2STime, fS2STime, m_parameterOffline.m_dwAecNum,m_parameterOffline.m_dwFast2SlowNum,m_parameterOffline.m_dwSlow2StopNum,
			m_parameterOffline.m_wAecSUnit,m_parameterOffline.m_wFast2SlowSUnit,m_parameterOffline.m_wSlow2StopSUnit,m_parameterOffline.m_dwFastV,m_parameterOffline.m_dwSlowV);
	}

	tableFile.WriteString(str);
	tableFile.Close();
	
	if (bRemind==TRUE)
	{	
		if (fAecTime <= 0.0
			|| fS2STime <= 0.0
			|| m_parameterOffline.m_dwAecNum <= 0
			|| m_parameterOffline.m_wFast2SlowSUnit <= 0
			|| m_parameterOffline.m_wSlow2StopSUnit <= 0
			|| m_parameterOffline.m_dwFastV <= 0
			|| m_parameterOffline.m_dwSlowV <= 0)
		{
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
			InitTimeTable();
			DeleteFile(strFile);
			return FALSE;
		}

		if (AfxMessageBox(str,NULL,MB_OKCANCEL)==IDCANCEL)
		{
			InitTimeTable();
			DeleteFile(strFile);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL COfflineTimeTableDlg::GetSpeedLevelInfo()
{
	CString str;
	
	m_pComPort->m_parameterOffline.Init();

	if (m_pComPort->servo_get_offline_timetable_paramater(m_bMotoWay,m_bSpeedLevel,CCmdTimeOut::get_offline_timetable_pamarater,FALSE))
	{
		CPoint c;
		float fAecUnitS = 0,fFast2SlowUnitS = 0,fSlow2StopUnitS = 0,fFastV = 0,fSlowV = 0;

		fAecUnitS = ((CSFBoardDlg*)m_pSFDlg)->Pulse2MM(m_pComPort->m_parameterOffline.m_wAecSUnit);
		fFast2SlowUnitS = ((CSFBoardDlg*)m_pSFDlg)->Pulse2MM(m_pComPort->m_parameterOffline.m_wFast2SlowSUnit);
		fSlow2StopUnitS = ((CSFBoardDlg*)m_pSFDlg)->Pulse2MM(m_pComPort->m_parameterOffline.m_wSlow2StopSUnit);
		fFastV = ((CSFBoardDlg*)m_pSFDlg)->Pulse2MM((int)((float)TIME_CONVERT_VALUE/(float)m_pComPort->m_parameterOffline.m_dwFastV));
		fSlowV = ((CSFBoardDlg*)m_pSFDlg)->Pulse2MM((int)((float)TIME_CONVERT_VALUE/(float)m_pComPort->m_parameterOffline.m_dwSlowV));

		m_fAecS = fAecUnitS*m_pComPort->m_parameterOffline.m_dwAecNum;
		m_fFast2SlowS = fFast2SlowUnitS*m_pComPort->m_parameterOffline.m_dwFast2SlowNum;
		m_fSlow2StopS = fSlow2StopUnitS*m_pComPort->m_parameterOffline.m_dwSlow2StopNum;
		m_fFastV = fFastV;
		m_fSlowV = fSlowV;
		m_nAecNum = m_pComPort->m_parameterOffline.m_dwAecNum;
		m_nFast2SlowNum = m_pComPort->m_parameterOffline.m_dwFast2SlowNum;
		m_nSlow2StopNum = m_pComPort->m_parameterOffline.m_dwSlow2StopNum;
		m_nTimeUnit = m_pComPort->m_parameterOffline.m_wTimeUnit;

		if (m_fAecS <= 0.00000001)
			m_fAecS = 50;
		if (m_fFast2SlowS <= 0.00000001)
			m_fFast2SlowS = 40;
		if (m_fSlow2StopS <= 0.00000001)
			m_fSlow2StopS = 10;
		if (m_fFastV <= 0.00000001)
			m_fFastV = 500;
		if (m_fSlowV <= 0.00000001)
			m_fSlowV = 200;

		SetTimeTableParameter();

		m_AecControlPoints.RemoveAll();
		m_AecControlPoints.Add(m_ptCurveAecStart);
		for (int i=0; i<m_pComPort->m_parameterOffline.m_bAecRecordNum; i++)
		{
			c.x = m_ptCurveAecStart.x + LOWORD(m_pComPort->m_parameterOffline.m_dwAecRecordArray.GetAt(i));
			c.y = m_ptCurveAecStart.y - HIWORD(m_pComPort->m_parameterOffline.m_dwAecRecordArray.GetAt(i));
			m_AecControlPoints.Add(c);
		}
		m_AecControlPoints.Add(m_ptCurveAecEnd);

		m_Fast2SlowControlPoints.RemoveAll();
		m_Fast2SlowControlPoints.Add(m_ptCurveFast2SlowStart);
		for (int i=0; i<m_pComPort->m_parameterOffline.m_bFast2SlowRecordNum; i++)
		{
			c.x = m_ptCurveFast2SlowStart.x + LOWORD(m_pComPort->m_parameterOffline.m_dwFast2SlowRecordArray.GetAt(i));
			c.y = m_ptCurveFast2SlowStart.y + HIWORD(m_pComPort->m_parameterOffline.m_dwFast2SlowRecordArray.GetAt(i));
			m_Fast2SlowControlPoints.Add(c);
		}
		m_Fast2SlowControlPoints.Add(m_ptCurveFast2SlowEnd);

		m_Slow2StopControlPoints.RemoveAll();
		m_Slow2StopControlPoints.Add(m_ptCurveSlow2StopStart);
		for (int i=0; i<m_pComPort->m_parameterOffline.m_bSlow2StopRecordNum; i++)
		{
			c.x = m_ptCurveSlow2StopStart.x + LOWORD(m_pComPort->m_parameterOffline.m_dwSlow2StopRecordArray.GetAt(i));
			c.y = m_ptCurveSlow2StopStart.y + HIWORD(m_pComPort->m_parameterOffline.m_dwSlow2StopRecordArray.GetAt(i));
			m_Slow2StopControlPoints.Add(c);
		}
		m_Slow2StopControlPoints.Add(m_ptCurveSlow2StopEnd);

		InvalidateRect(m_rcAec);
		InvalidateRect(m_rcFast2Slow);
		InvalidateRect(m_rcSlow2Stop);

		CalculateTime();
		return TRUE;
	}
	else
	{
		str.Format("得脱机第 %d 档参数--失败",m_bSpeedLevel+1);
		AfxMessageBox(str);
		return FALSE;
	}

}

BOOL COfflineTimeTableDlg::ExportSpeedLevelInfo(CString strEeportFile)
{
	CString strSection,strReg,strIniFile = strEeportFile;

	if (!GetSpeedLevelInfo())
	{
		DeleteFile(strEeportFile);
		return FALSE;
	}

	strSection.Format("SpeedInfo_%d",m_bSpeedLevel);

	UINT nAecControlX[10];
	UINT nAecControlY[10];
	UINT nFast2SlowControlX[10];
	UINT nFast2SlowControlY[10];
	UINT nSlow2StopControlX[10];
	UINT nSlow2StopControlY[10];

	float fAecS = m_fAecS;
	float fFast2SlowS = m_fFast2SlowS;
	float fSlow2StopS = m_fSlow2StopS;
	float fFastV = m_fFastV;
	float fSlowV = m_fSlowV;
	UINT nAecNum = m_nAecNum;
	UINT nFast2SlowNum = m_nFast2SlowNum;
	UINT nSlow2StopNum = m_nSlow2StopNum;
	UINT nAecControlNum = m_AecControlPoints.GetCount() - 2;
	UINT nFast2SlowControlNum = m_Fast2SlowControlPoints.GetCount() - 2;
	UINT nSlow2StopControlNum = m_Slow2StopControlPoints.GetCount() - 2;

	for (int i=0;i<nAecControlNum;i++)
	{
		nAecControlX[i] = abs(m_AecControlPoints.GetAt(i+1).x - m_ptCurveAecStart.x);
		nAecControlY[i] = abs(m_AecControlPoints.GetAt(i+1).y - m_ptCurveAecStart.y);
	}

	for (int i=0;i<nFast2SlowControlNum;i++)
	{
		nFast2SlowControlX[i] = abs(m_Fast2SlowControlPoints.GetAt(i+1).x - m_ptCurveFast2SlowStart.x);
		nFast2SlowControlY[i] = abs(m_Fast2SlowControlPoints.GetAt(i+1).y - m_ptCurveFast2SlowStart.y);
	}

	for (int i=0;i<nSlow2StopControlNum;i++)
	{
		nSlow2StopControlX[i] = abs(m_Slow2StopControlPoints.GetAt(i+1).x - m_ptCurveSlow2StopStart.x);
		nSlow2StopControlY[i] = abs(m_Slow2StopControlPoints.GetAt(i+1).y - m_ptCurveSlow2StopStart.y);
	}

	WRITE_PROFILE_INFO("%.2f",fAecS);
	WRITE_PROFILE_INFO("%.2f",fFast2SlowS);
	WRITE_PROFILE_INFO("%.2f",fSlow2StopS);
	WRITE_PROFILE_INFO("%.2f",fFastV);
	WRITE_PROFILE_INFO("%.2f",fSlowV);
	WRITE_PROFILE_INFO("%d",nAecNum);
	WRITE_PROFILE_INFO("%d",nFast2SlowNum);
	WRITE_PROFILE_INFO("%d",nSlow2StopNum);
	WRITE_PROFILE_INFO("%d",nAecControlNum);
	WRITE_PROFILE_INFO("%d",nFast2SlowControlNum);
	WRITE_PROFILE_INFO("%d",nSlow2StopControlNum);

	WRITE_PROFILE_INFO_ARRAY("%d",nAecControlX,nAecControlNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nAecControlY,nAecControlNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nFast2SlowControlX,nFast2SlowControlNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nFast2SlowControlY,nFast2SlowControlNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nSlow2StopControlX,nSlow2StopControlNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nSlow2StopControlY,nSlow2StopControlNum);

	return TRUE;
}
void COfflineTimeTableDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL COfflineTimeTableDlg::ImportSpeedLevelInfo(CString strImportFile)
{
	CString strFilePath = "",str = "";
	CString strIniFile,strSection;
	char szReg[200];
	CPoint c;

	strIniFile = strImportFile;
	strSection.Format("SpeedInfo_%d",m_bSpeedLevel);

	UINT nAecControlX[10];
	UINT nAecControlY[10];
	UINT nFast2SlowControlX[10];
	UINT nFast2SlowControlY[10];
	UINT nSlow2StopControlX[10];
	UINT nSlow2StopControlY[10];
	float fAecS = 0,fFast2SlowS = 0,fSlow2StopS=0,fFastV = 0,fSlowV = 0;
	UINT nAecNum = 0,nFast2SlowNum = 0,nSlow2StopNum = 0,nAecControlNum = 0,nFast2SlowControlNum = 0,nSlow2StopControlNum = 0;

	GET_PROFILE_INFO_FLOAT(fAecS,0,0,500);
	GET_PROFILE_INFO_FLOAT(fFast2SlowS,0,0,500);
	GET_PROFILE_INFO_FLOAT(fSlow2StopS,0,0,500);
	GET_PROFILE_INFO_FLOAT(fFastV,0,0,2000);
	GET_PROFILE_INFO_FLOAT(fSlowV,0,0,2000);
	GET_PROFILE_INFO_INT(nAecNum,0,0,2500,UINT);
	GET_PROFILE_INFO_INT(nFast2SlowNum,0,0,2500,UINT);
	GET_PROFILE_INFO_INT(nSlow2StopNum,0,0,2500,UINT);
	GET_PROFILE_INFO_INT(nAecControlNum,0,0,10,UINT);
	GET_PROFILE_INFO_INT(nFast2SlowControlNum,0,0,10,UINT);
	GET_PROFILE_INFO_INT(nSlow2StopControlNum,0,0,10,UINT);

	GET_PROFILE_INFO_ARRAY_INT(nAecControlX,nAecControlNum,0,0,1000,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nAecControlY,nAecControlNum,0,0,1000,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nFast2SlowControlX,nFast2SlowControlNum,0,0,1000,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nFast2SlowControlY,nFast2SlowControlNum,0,0,1000,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nSlow2StopControlX,nSlow2StopControlNum,0,0,1000,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nSlow2StopControlY,nSlow2StopControlNum,0,0,1000,UINT);

	m_fAecS = fAecS;
	m_fFast2SlowS = fFast2SlowS;
	m_fSlow2StopS = fSlow2StopS;
	m_fFastV = fFastV;
	m_fSlowV = fSlowV;
	m_nAecNum = nAecNum;
	m_nFast2SlowNum = nFast2SlowNum;
	m_nSlow2StopNum = nSlow2StopNum;

	SetTimeTableParameter();

	m_AecControlPoints.RemoveAll();
	m_AecControlPoints.Add(m_ptCurveAecStart);
	for (int i=0; i<nAecControlNum; i++)
	{
		c.x = m_ptCurveAecStart.x + nAecControlX[i];
		c.y = m_ptCurveAecStart.y - nAecControlY[i];
		m_AecControlPoints.Add(c);
	}
	m_AecControlPoints.Add(m_ptCurveAecEnd);

	m_Fast2SlowControlPoints.RemoveAll();
	m_Fast2SlowControlPoints.Add(m_ptCurveFast2SlowStart);
	for (int i=0; i<nFast2SlowControlNum; i++)
	{
		c.x = m_ptCurveFast2SlowStart.x + nFast2SlowControlX[i];
		c.y = m_ptCurveFast2SlowStart.y + nFast2SlowControlY[i];
		m_Fast2SlowControlPoints.Add(c);
	}
	m_Fast2SlowControlPoints.Add(m_ptCurveFast2SlowEnd);

	m_Slow2StopControlPoints.RemoveAll();
	m_Slow2StopControlPoints.Add(m_ptCurveSlow2StopStart);
	for (int i=0; i<nSlow2StopControlNum; i++)
	{
		c.x = m_ptCurveSlow2StopStart.x + nSlow2StopControlX[i];
		c.y = m_ptCurveSlow2StopStart.y + nSlow2StopControlY[i];
		m_Slow2StopControlPoints.Add(c);
	}
	m_Slow2StopControlPoints.Add(m_ptCurveSlow2StopEnd);

	InvalidateRect(m_rcAec);
	InvalidateRect(m_rcFast2Slow);
	InvalidateRect(m_rcSlow2Stop);

	CalculateTime();
	
	m_bSpeedLevel = 0xFF;

	if(!AddTimeTable(FALSE))
	{
		return FALSE;
	}
	return TRUE;
}