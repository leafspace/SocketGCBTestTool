// OnlineTimeTableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RB_ComTest.h"
#include "OnlineTimeTableDlg.h"
#include "Curve/spline.h"
#include "SFBoardDlg.h"

// COnlineTimeTableDlg 对话框

IMPLEMENT_DYNAMIC(COnlineTimeTableDlg, CDialog)

COnlineTimeTableDlg::COnlineTimeTableDlg(CWnd* pParent /*=NULL*/)
: CDialog(COnlineTimeTableDlg::IDD, pParent)
{
	m_fAecS = 100;
	m_fDecS = 100;
	m_fEvenV = 300;
	m_nAecNum = 200;
	m_nDecNum = 200;
	m_nTimeUnit = 100;
	m_nMoveIndex = -1;
	m_nOperationCurve = online_operation_curve_none;
}

COnlineTimeTableDlg::~COnlineTimeTableDlg()
{
}

void COnlineTimeTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SFONLINE_OPERATION, m_btnOperation);
	DDX_Control(pDX, IDC_PROGRESS_SFONLINE_DOWNLOAD, m_pgsDownload);
}


BEGIN_MESSAGE_MAP(COnlineTimeTableDlg, CDialog)
	ON_MESSAGE(WM_SFDOWNLOAD_PROGRESS, &COnlineTimeTableDlg::OnSFDownloadProgress)
	ON_BN_CLICKED(IDC_BUTTON_SFONLINE_OPERATION, &COnlineTimeTableDlg::OnBnClickedButtonSfonlineOperation)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SFONLINE_CALCULATETIME, &COnlineTimeTableDlg::OnBnClickedButtonSfonlineCalculatetime)
	ON_BN_CLICKED(IDOK, &COnlineTimeTableDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL COnlineTimeTableDlg::AddTimeTable(BOOL bRemind)
{
	// TODO: 在此添加控件通知处理程序代码
	if(!CreateTimeTable(m_bSpeedLevel,bRemind))
	{
		goto End;
	}
	
	if (!m_pComPort->servo_set_online_timetable_paramater(m_bMotoWay,m_timeTableOnline,CCmdTimeOut::set_online_timetable_paramater,FALSE))
	{
		AfxMessageBox("下载联机时间表参数--失败");
		goto End;
	}

	Sleep(1000);
	m_pgsDownload.ShowWindow(TRUE);
	m_pgsDownload.SetRange(0,m_timeTableOnline.m_parameterOnline->m_dwAecNum+m_timeTableOnline.m_parameterOnline->m_dwDecNum);
	m_pgsDownload.SetPos(0);

	if (!m_pComPort->servo_set_online_timetable_timevalue(m_bMotoWay,m_timeTableOnline,CCmdTimeOut::set_timetable_timevalue,FALSE))
	{
		AfxMessageBox("下载联机时间表数据--失败");
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
// COnlineTimeTableDlg 消息处理程序

void COnlineTimeTableDlg::OnBnClickedButtonSfonlineOperation()
{
	// TODO: 在此添加控件通知处理程序代码
	AddTimeTable(TRUE);
}

BOOL COnlineTimeTableDlg::OnInitDialog()
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
	WORD wAecTime,wDecTime;
	CalculateTime(wAecTime,wDecTime);

	m_pComPort = ((CSFBoardDlg*)m_pSFDlg)->m_pComPort;
	m_pComPort->m_pOnlineTimeTableDlg = this;
	
	if (m_nOperationType==1)
	{
		GetSpeedLevelInfo();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void COnlineTimeTableDlg::SetTimeTableParameter()
{
	CString str;

	str.Format("%.2f",m_fAecS);
	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_AECDISTANCE))->SetWindowText(str);

	str.Format("%.2f",m_fDecS);
	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_DECDISTANCE))->SetWindowText(str);

	str.Format("%.2f",m_fEvenV);
	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_EVENSPEED))->SetWindowText(str);

	str.Format("%d",m_nAecNum);
	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_AECNUM))->SetWindowText(str);

	str.Format("%d",m_nDecNum);
	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_DECNUM))->SetWindowText(str);
}

BOOL COnlineTimeTableDlg::GetTimeTableParameter()
{
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_AECDISTANCE))->GetWindowText(str);
	m_fAecS = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_DECDISTANCE))->GetWindowText(str);
	m_fDecS = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_EVENSPEED))->GetWindowText(str);
	m_fEvenV = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_AECNUM))->GetWindowText(str);
	m_nAecNum = atoi(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFONLINE_DECNUM))->GetWindowText(str);
	m_nDecNum = atoi(str);

	return TRUE;
}

void COnlineTimeTableDlg::InitCurve()
{
	CPoint ptControl1,ptControl2;

	((CWnd*)GetDlgItem(IDC_STATIC_SFONLINE_CURVE))->GetWindowRect(&m_rcView);
	ScreenToClient(&m_rcView);

	m_ptCurveAecStart.x = m_rcView.left;
	m_ptCurveAecStart.y = m_rcView.bottom;
	m_ptCurveAecEnd.x = m_rcView.left + 235;
	m_ptCurveAecEnd.y = m_rcView.top;

	m_ptCurveDecStart.x = m_rcView.left + m_rcView.Width() - 235;
	m_ptCurveDecStart.y = m_rcView.top;
	m_ptCurveDecEnd.x = m_rcView.right;
	m_ptCurveDecEnd.y = m_rcView.bottom;

	m_rcAec.top = m_ptCurveAecEnd.y;
	m_rcAec.bottom = m_ptCurveAecStart.y;
	m_rcAec.left = m_ptCurveAecStart.x;
	m_rcAec.right = m_ptCurveAecEnd.x;

	m_rcDec.top = m_ptCurveDecStart.y;
	m_rcDec.bottom = m_ptCurveDecEnd.y;
	m_rcDec.left = m_ptCurveDecStart.x;
	m_rcDec.right = m_ptCurveDecEnd.x;

	//控制点
	ptControl1.x = m_ptCurveAecStart.x + m_rcAec.Width()/3;
	ptControl1.y = m_ptCurveAecStart.y - m_rcAec.Height()/3;

	ptControl2.x = m_ptCurveAecStart.x + m_rcAec.Width()/3*2;
	ptControl2.y = m_ptCurveAecStart.y - m_rcAec.Height()/3*2;

	m_AecControlPoints.RemoveAll();
	m_AecControlPoints.Add(m_ptCurveAecStart);
	m_AecControlPoints.Add(ptControl1);
	m_AecControlPoints.Add(ptControl2);
	m_AecControlPoints.Add(m_ptCurveAecEnd);

	ptControl1.x = m_ptCurveDecStart.x + m_rcDec.Width()/3;
	ptControl1.y = m_ptCurveDecStart.y + m_rcDec.Height()/3;

	ptControl2.x = m_ptCurveDecStart.x + m_rcDec.Width()/3*2;
	ptControl2.y = m_ptCurveDecStart.y + m_rcDec.Height()/3*2;

	m_DecControlPoints.RemoveAll();
	m_DecControlPoints.Add(m_ptCurveDecStart);
	m_DecControlPoints.Add(ptControl1);
	m_DecControlPoints.Add(ptControl2);
	m_DecControlPoints.Add(m_ptCurveDecEnd);
}

int COnlineTimeTableDlg::IsInsideControlPoint(POINT& point, int& nOperationCurve)
{
	int count;

	count = m_AecControlPoints.GetSize();

	if(count>0)	
	{
		for(int i=1; i<count-1; i++)
		{
			if(Distance(m_AecControlPoints[i], point)<5)
			{
				nOperationCurve = online_operation_curve_aec;
				return i;
			}
		}
	}

	count = m_DecControlPoints.GetSize();

	if(count>0)	
	{
		for(int i=1; i<count-1; i++)
		{
			if(Distance(m_DecControlPoints[i], point)<5)
			{
				nOperationCurve = online_operation_curve_dec;
				return i;
			}
		}
	}
	return -1;
}



void COnlineTimeTableDlg::DrawCurveArea(CDC* pDC)
{
	DrawCurveAec(pDC);
	DrawCurveDec(pDC);
}

void COnlineTimeTableDlg::DrawCurveAec(CDC* pDC)
{
	if (m_AecControlPoints.GetSize()>1)
	{	
		Spline spline(m_AecControlPoints.GetData(), m_AecControlPoints.GetSize());
		spline.Generate();

		m_AecCurvePoints.SetSize(spline.GetCurveCount(DLG_CURVE_DIV_FACTOR));
		int PointCount = 0;
		spline.GetCurve(m_AecCurvePoints.GetData(), PointCount, DLG_CURVE_DIV_FACTOR);

		RestrictAecCurve(m_AecCurvePoints.GetData(),PointCount,&m_rcAec, &m_ptMoveNow, &m_ptMoveLast);

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

void COnlineTimeTableDlg::DrawCurveDec(CDC* pDC)
{
	if (m_DecControlPoints.GetSize()>1)
	{
		Spline spline(m_DecControlPoints.GetData(), m_DecControlPoints.GetSize());
		spline.Generate();

		m_DecCurvePoints.SetSize(spline.GetCurveCount(DLG_CURVE_DIV_FACTOR));
		int PointCount = 0;
		spline.GetCurve(m_DecCurvePoints.GetData(), PointCount, DLG_CURVE_DIV_FACTOR);

		RestrictDecCurve(m_DecCurvePoints.GetData(),PointCount, &m_rcDec, &m_ptMoveNow, &m_ptMoveLast);

		//FillRect(pDC->m_hDC, &m_rcDec, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		//pDC->FillSolidRect(&m_rcDec,afxGlobalData.clrBarFace);

		for(int i=1; i<m_DecControlPoints.GetSize()-1; i++)
		{
			DrawCross(m_rcDec, m_DecControlPoints[i], RGB(255,0,0), 4, pDC->m_hDC);
		}

		if (m_DecCurvePoints.GetSize()>1)
		{
			//Polyline(pDC->m_hDC, m_DecCurvePoints.GetData(), m_DecCurvePoints.GetSize());
			DrawCurve(m_DecCurvePoints.GetData(), m_DecCurvePoints.GetSize(),RGB(0,0,0),pDC->m_hDC);
		}
	}
}

void COnlineTimeTableDlg::DrawAxis(CDC* pDC)
{
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

	//匀速
	pDC->MoveTo(m_ptCurveAecEnd);
	pDC->LineTo(m_ptCurveDecStart);

	//右边
	a.x = m_ptCurveDecEnd.x;
	a.y = m_ptCurveDecStart.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveDecEnd);

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

	a.x = m_ptCurveDecStart.x;
	a.y = m_ptCurveDecEnd.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveDecStart);

	a.x = m_ptCurveDecEnd.x;
	a.y = m_ptCurveDecStart.y;
	pDC->MoveTo(a);
	pDC->LineTo(m_ptCurveDecStart);

	pDC->SelectObject(pOldPen);
	cp.DeleteObject();
}

void COnlineTimeTableDlg::DrawCurve(const POINT *point,int count, COLORREF rgbColor,HDC hDC)
{
	HPEN hPen,oldPen;
	hPen = CreatePen(PS_SOLID,1,rgbColor);
	oldPen = (HPEN)SelectObject(hDC,hPen);

	Polyline(hDC, point, count);

	SelectObject(hDC,oldPen);
	DeleteObject(hPen);
}

void COnlineTimeTableDlg::DrawCross(RECT rect, POINT point, COLORREF rgbColor, int extent, HDC hDC)
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

BOOL COnlineTimeTableDlg::MoveCurve()
{
	switch(m_nOperationCurve)
	{
	case online_operation_curve_aec:
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
	case online_operation_curve_dec:
		{
			m_DecControlPoints[m_nMoveIndex] = m_ptMoveNow;

			CRect rcTemp = m_rcDec;
			ClientToScreen(&rcTemp);
			ClipCursor(&rcTemp);

			if (m_DecControlPoints[m_nMoveIndex].x>m_DecControlPoints[m_nMoveIndex+1].x||
				m_DecControlPoints[m_nMoveIndex].x<m_DecControlPoints[m_nMoveIndex-1].x||
				m_DecControlPoints[m_nMoveIndex].y>m_DecControlPoints[m_nMoveIndex+1].y||
				m_DecControlPoints[m_nMoveIndex].y<m_DecControlPoints[m_nMoveIndex-1].y||
				(m_DecControlPoints[m_nMoveIndex].x==m_DecControlPoints[m_nMoveIndex+1].x && m_DecControlPoints[m_nMoveIndex].y==m_DecControlPoints[m_nMoveIndex+1].y)||
				(m_DecControlPoints[m_nMoveIndex].x==m_DecControlPoints[m_nMoveIndex-1].x && m_DecControlPoints[m_nMoveIndex].y==m_DecControlPoints[m_nMoveIndex-1].y))
			{
				m_ptMoveNow = m_ptMoveLast;
				m_DecControlPoints[m_nMoveIndex] = m_ptMoveNow;
				return FALSE;
			}
		}
		break;
	}
	return TRUE;
}
void COnlineTimeTableDlg::OnMouseMove(UINT nFlags, CPoint point)
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
		//pDC->FillSolidRect(&m_rcAec,afxGlobalData.clrBarFace);
		//pDC->FillSolidRect(&m_rcDec,afxGlobalData.clrBarFace);
		FillRect(MemDC.m_hDC, &m_rcAec, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		FillRect(MemDC.m_hDC, &m_rcDec, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

		MoveCurve();

		DrawCurveArea(&MemDC);
		DrawAxis(&MemDC);

		pDC->BitBlt(m_rcView.left,m_rcView.top,m_rcView.Width(),m_rcView.Height(),&MemDC,m_rcView.left,m_rcView.top,SRCCOPY);
		m_ptMoveLast = m_ptMoveNow;
	}
	CDialog::OnMouseMove(nFlags, point);
}

void COnlineTimeTableDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nOperationCurve = online_operation_curve_none;
	m_nMoveIndex = -1;

	int nMoveIndex = IsInsideControlPoint(point, m_nOperationCurve);

	if(nMoveIndex >= 0)
	{
		m_nMoveIndex = nMoveIndex;
		m_ptMoveLast = point;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void COnlineTimeTableDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nMoveIndex>=0)
	{
		CRect rcTemp;

		switch(m_nOperationCurve)
		{
		case online_operation_curve_aec:
			rcTemp = m_rcAec;
			break;
		case online_operation_curve_dec:
			rcTemp = m_rcDec;
			break;
		}
		InvalidateRect(&rcTemp);

		ClipCursor(NULL);
		WORD wAecTime,wDecTime;
		CalculateTime(wAecTime,wDecTime);
		m_nMoveIndex = -1;
		m_nOperationCurve = online_operation_curve_none;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void COnlineTimeTableDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
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
	if(PtInRect(&m_rcDec,point))
	{
		for(int i=1; i<m_DecCurvePoints.GetSize()-1; i++)
		{
			if(Distance(m_DecCurvePoints[i], point)<5)
			{
				ad = m_DecCurvePoints.GetAt(i);

				for (int i=0; i<m_DecControlPoints.GetSize()-1; i++)
				{
					ct1 = m_DecControlPoints.GetAt(i);
					ct2 = m_DecControlPoints.GetAt(i+1);
					if (ad.x>ct1.x && ad.y>ct1.y && ad.x<ct2.x && ad.y<ct2.y)
					{
						if (m_DecControlPoints.GetCount()>=dlg_control_point_max)
						{
							AfxMessageBox("控制点小于等于10个");
							return;
						}
						m_DecControlPoints.InsertAt(i+1, ad);

						InvalidateRect(m_rcDec);
						return;
					}
				}
			}
		}
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void COnlineTimeTableDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
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
	if(PtInRect(&m_rcDec,point))
	{
		for(int i=1; i<m_DecControlPoints.GetSize()-1; i++)
		{
			if(Distance(m_DecControlPoints[i], point)<5)
			{
				m_DecControlPoints.RemoveAt(i);

				InvalidateRect(m_rcDec);
				return;
			}
		}
	}
	CDialog::OnRButtonDblClk(nFlags, point);
}

void COnlineTimeTableDlg::OnPaint()
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
		//pDC->FillSolidRect(&m_rcAec,afxGlobalData.clrBarFace);
		//pDC->FillSolidRect(&m_rcDec,afxGlobalData.clrBarFace);
		FillRect(MemDC.m_hDC, &m_rcAec, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		FillRect(MemDC.m_hDC, &m_rcDec, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

		DrawCurveArea(&MemDC);

		pDC->BitBlt(m_rcView.left,m_rcView.top,m_rcView.Width(),m_rcView.Height(),&MemDC,m_rcView.left,m_rcView.top,SRCCOPY);

		DrawAxis(pDC);
	}
	// 不为绘图消息调用 CDialog::OnPaint()
}

void COnlineTimeTableDlg::CalculateTime(WORD& wAecTime,WORD& wDecTime)
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

	if (!GetTimeTableParameter())
	{
		return;
	}

	if (m_nOperationCurve==online_operation_curve_none || m_nOperationCurve==online_operation_curve_aec)
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
				tableControlPoint.x = m_ptCurveAecStart.x + ((float)(abs(m_AecControlPoints.GetAt(i).x - m_ptCurveAecStart.x)))/((float)(m_rcAec.Width()))*((float)tableRect.Width());
			}

			if (abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y)==0)
			{
				tableControlPoint.y = m_ptCurveAecStart.y;
			}
			else
			{
				tableControlPoint.y = m_ptCurveAecStart.y - ((float)(abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y)))/((float)(m_rcAec.Height()))*((float)tableRect.Height());
			}
			tableControlPointsArray.Add(tableControlPoint);
		}	

		Spline splineAec(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
		splineAec.Generate();

		tableCurvePointsArray.SetSize(splineAec.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
		nPointCount = 0;
		splineAec.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

		RestrictAecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

		//发时间 最小单位 0.1us 最大到 5000us
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

			fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * m_fEvenV;

			if (fV==0)
			{
				nTimeValue = WORD_MAX;
			}
			else
			{
				ASSERT(fV>0 && fV<=m_fEvenV);
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
		((CWnd*)GetDlgItem(IDC_STATIC_SFONLINE_AECTIME))->SetWindowText(str);
		wAecTime = fTotalTime;
	}

	if (m_nOperationCurve==online_operation_curve_none || m_nOperationCurve==online_operation_curve_dec)
	{
		//减速段
		tableControlPointsArray.RemoveAll();
		tableCurvePointsArray.RemoveAll();

		tableRect.left = m_ptCurveDecStart.x;
		tableRect.top = m_ptCurveDecStart.y;
		tableRect.right = m_ptCurveDecStart.x + m_nDecNum - 1;
		tableRect.bottom = m_ptCurveDecStart.y + ((float)(m_rcDec.Height()))/((float)(m_rcDec.Width()))*((float)(m_nDecNum - 1));

		for (int i=0; i<m_DecControlPoints.GetCount(); i++)
		{
			if (abs(m_DecControlPoints.GetAt(i).x - m_ptCurveDecStart.x)==0)
			{
				tableControlPoint.x = m_ptCurveDecStart.x;
			}
			else
			{
				tableControlPoint.x = m_ptCurveDecStart.x + ((float)(abs(m_DecControlPoints.GetAt(i).x - m_ptCurveDecStart.x)))/((float)(m_rcDec.Width()))*((float)tableRect.Width());
			}

			if (abs(m_DecControlPoints.GetAt(i).y - m_ptCurveDecStart.y)==0)
			{
				tableControlPoint.y = m_ptCurveDecStart.y;
			}
			else
			{
				tableControlPoint.y = m_ptCurveDecStart.y + ((float)(abs(m_DecControlPoints.GetAt(i).y - m_ptCurveDecStart.y)))/((float)(m_rcDec.Height()))*((float)tableRect.Height());
			}
			tableControlPointsArray.Add(tableControlPoint);
		}	

		Spline splineDec(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
		splineDec.Generate();

		tableCurvePointsArray.SetSize(splineDec.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
		nPointCount = 0;
		splineDec.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

		RestrictDecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

		//发时间 最小单位 0.1us 最大到5000us
		tableTimeArray.RemoveAll();
		fPulseS = m_fDecS/(float)m_nDecNum;
		nLastX = -1;
		fTotalTime = 0;

		wPulse = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

		for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
		{	
			if (tableCurvePointsArray.GetAt(i).x==nLastX)
			{
				continue;
			}

			fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * m_fEvenV;

			if (fV==0)
			{
				nTimeValue = WORD_MAX;
			}
			else
			{
				ASSERT(fV>0 && fV<=m_fEvenV);
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
		((CWnd*)GetDlgItem(IDC_STATIC_SFONLINE_DECTIME))->SetWindowText(str);
		wDecTime = fTotalTime;
	}
}
void COnlineTimeTableDlg::OnBnClickedButtonSfonlineCalculatetime()
{
	// TODO: 在此添加控件通知处理程序代码
	WORD wAecTime,wDecTime;
	CalculateTime(wAecTime,wDecTime);
}

LONG COnlineTimeTableDlg::OnSFDownloadProgress(WPARAM wPALAM, LPARAM lPALAM)
{
	int nProgress = (int)wPALAM;
	m_pgsDownload.SetPos(nProgress);
	return 0;
}

void COnlineTimeTableDlg::InitTimeTable()
{
	//联机
	m_timeTableOnline.m_bSpeedLevelOnline = 0xFF;

	m_parameterOnline.Init();

	m_timeTableOnline.m_parameterOnline = NULL;
	m_timeTableOnline.m_wAecTimeArray.RemoveAll();
	m_timeTableOnline.m_wDecTimeArray.RemoveAll();
}

BOOL COnlineTimeTableDlg::CreateTimeTable(BYTE bSpeedLevel,BOOL bRemind)
{
	CPOINTArray		tableControlPointsArray;	//虚拟曲线控制点
	CPOINTArray		tableCurvePointsArray;		//虚拟曲线实际点
	CWordArray		tableTimeArray;				//时间表数据
	CDWordArray		tableRecordArray;			//特殊点数据
	CPoint			tableControlPoint;			//特殊点
	CRect			tableRect;					//虚拟矩形
	float			fAecTime = 0,fDecTime = 0;				//总时间
	float			fV;										//速度值	
	float			fPulseS = 0;							//每格距离 mm
	int				nTimeValue,nTimeTemp;					//时间表值
	int				nTimeNum = 0,nPtRecordNum = 0;			//格数
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
	CString strFile = strFolder + "\\OnlineTableValue-" + str + ".txt";
	CStdioFile tableFile(strFile,CFile::modeCreate | CFile::modeWrite | CFile::typeText);

	SetCursor(LoadCursor(NULL,IDC_WAIT));
	
	//联机表
	InitTimeTable();

	m_timeTableOnline.m_bSpeedLevelOnline = bSpeedLevel;
	m_timeTableOnline.m_parameterOnline = &m_parameterOnline;

	m_parameterOnline.m_wTimeUnit = m_nTimeUnit;

	dwSpeedTemp = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(m_fEvenV);
	m_parameterOnline.m_dwEvenV = TIME_CONVERT((float)1/(float)dwSpeedTemp);

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
			tableControlPoint.x = m_ptCurveAecStart.x + ((float)(abs(m_AecControlPoints.GetAt(i).x - m_ptCurveAecStart.x)))/((float)(m_rcAec.Width()))*((float)tableRect.Width());
		}

		if (abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y)==0)
		{
			tableControlPoint.y = m_ptCurveAecStart.y;
		}
		else
		{
			tableControlPoint.y = m_ptCurveAecStart.y - ((float)(abs(m_AecControlPoints.GetAt(i).y - m_ptCurveAecStart.y)))/((float)(m_rcAec.Height()))*((float)tableRect.Height());
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

	//发时间 最小单位 1us 最大到 65535us
	fPulseS = m_fAecS/(float)m_nAecNum;
	nLastX = -1;

	m_parameterOnline.m_wAecSUnit = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

	for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
	{	
		if (tableCurvePointsArray.GetAt(i).x==nLastX)
		{
			continue;
		}

		fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * m_fEvenV;

		if (fV==0)
		{
			nTimeValue = WORD_MAX;
		}
		else
		{
			ASSERT(fV>0 && fV<=m_fEvenV);
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

		fAecTime += (float)tableTimeArray.GetAt(i)/(float)1000 * m_parameterOnline.m_wAecSUnit;
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

	m_parameterOnline.m_dwAecNum = nTimeNum;
	m_parameterOnline.m_bAecRecordNum = nPtRecordNum;

	for (int i=0; i<nTimeNum; i++)
	{
		m_timeTableOnline.m_wAecTimeArray.Add(tableTimeArray.GetAt(i));
	}

	for (int i=0; i<nPtRecordNum; i++)
	{
		m_parameterOnline.m_dwAecRecordArray.Add(tableRecordArray.GetAt(i));
	}

	//减速段
	tableControlPointsArray.RemoveAll();
	tableCurvePointsArray.RemoveAll();

	tableRect.left = m_ptCurveDecStart.x;
	tableRect.top = m_ptCurveDecStart.y;
	tableRect.right = m_ptCurveDecStart.x + m_nDecNum - 1;
	tableRect.bottom = m_ptCurveDecStart.y + ((float)(m_rcDec.Height()))/((float)(m_rcDec.Width()))*((float)(m_nDecNum - 1));

	for (int i=0; i<m_DecControlPoints.GetCount(); i++)
	{
		if (abs(m_DecControlPoints.GetAt(i).x - m_ptCurveDecStart.x)==0)
		{
			tableControlPoint.x = m_ptCurveDecStart.x;
		}
		else
		{
			tableControlPoint.x = m_ptCurveDecStart.x + ((float)(abs(m_DecControlPoints.GetAt(i).x - m_ptCurveDecStart.x)))/((float)(m_rcDec.Width()))*((float)tableRect.Width());
		}

		if (abs(m_DecControlPoints.GetAt(i).y - m_ptCurveDecStart.y)==0)
		{
			tableControlPoint.y = m_ptCurveDecStart.y;
		}
		else
		{
			tableControlPoint.y = m_ptCurveDecStart.y + ((float)(abs(m_DecControlPoints.GetAt(i).y - m_ptCurveDecStart.y)))/((float)(m_rcDec.Height()))*((float)tableRect.Height());
		}
		tableControlPointsArray.Add(tableControlPoint);
	}	

	Spline splineDec(tableControlPointsArray.GetData(), tableControlPointsArray.GetCount());
	splineDec.Generate();

	tableCurvePointsArray.SetSize(splineDec.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
	nPointCount = 0;
	splineDec.GetCurve(tableCurvePointsArray.GetData(), nPointCount, TABLE_CURVE_DIV_FACTOR);

	RestrictDecCurve(tableCurvePointsArray.GetData(), nPointCount, &tableRect, NULL, NULL);

	tableTimeArray.RemoveAll();
	tableRecordArray.RemoveAll();

	//发时间 最小单位 1us 最大到65535us
	fPulseS = m_fDecS/(float)m_nDecNum;
	nLastX = -1;

	m_parameterOnline.m_wDecSUnit = ((CSFBoardDlg*)m_pSFDlg)->MM2Pulse(fPulseS);

	for (int i=0; i<tableCurvePointsArray.GetCount(); i++)
	{
		if (tableCurvePointsArray.GetAt(i).x==nLastX)
		{
			continue;
		}

		fV = ((float)(abs(tableCurvePointsArray.GetAt(i).y - tableRect.bottom)))/(float)tableRect.Height() * m_fEvenV;

		if (fV==0)
		{
			nTimeValue = WORD_MAX;
		}
		else
		{
			ASSERT(fV>0 && fV<=m_fEvenV);
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
		str.Format("DECT: %d\n", tableTimeArray.GetAt(i));
		tableFile.WriteString(str);

		fDecTime += (float)tableTimeArray.GetAt(i)/(float)1000 * m_parameterOnline.m_wDecSUnit;
	}

	ASSERT(nTimeNum==m_nDecNum);

	//特殊点
	if (m_DecControlPoints.GetCount()==2)
	{
		nPtRecordNum = 0;
	}
	else
	{
		for (int i=1;i<m_DecControlPoints.GetCount()-1;i++)
		{
			wLo = abs(m_DecControlPoints.GetAt(i).x - m_ptCurveDecStart.x);
			wHi = abs(m_DecControlPoints.GetAt(i).y - m_ptCurveDecStart.y);

			dwControlPoint = (DWORD(wHi & 0xFFFF)<<16) | (DWORD(wLo & 0xFFFF));
			tableRecordArray.Add(dwControlPoint);
		}
		nPtRecordNum = tableRecordArray.GetCount();
	}

	m_parameterOnline.m_dwDecNum = nTimeNum;
	m_parameterOnline.m_bDecRecordNum = nPtRecordNum;

	for (int i=0; i<nTimeNum; i++)
	{
		m_timeTableOnline.m_wDecTimeArray.Add(tableTimeArray.GetAt(i));
	}

	for (int i=0; i<nPtRecordNum; i++)
	{
		m_parameterOnline.m_dwDecRecordArray.Add(tableRecordArray.GetAt(i));
	}

	SetCursor(LoadCursor(NULL,IDC_ARROW));
	
	if (bSpeedLevel==0xFF)
	{
		str.Format("新增一档联机时间表:\n\n加速时间:\t%.2f ms\n减速时间:\t%.2f ms\n加速档格数:\t%d\n减速档格数:\t%d\n加速档每格脉冲:\t%d\n减速档每格脉冲:\t%d\n匀速发送时间值:\t%d us", 
			fAecTime,fDecTime,m_parameterOnline.m_dwAecNum,m_parameterOnline.m_dwDecNum,m_parameterOnline.m_wAecSUnit,m_parameterOnline.m_wDecSUnit,m_parameterOnline.m_dwEvenV);
	}
	else
	{
		str.Format("修改第 %d 档联机时间表:\n\n加速时间:\t%.2f ms\n减速时间:\t%.2f ms\n加速档格数:\t%d\n减速档格数:\t%d\n加速档每格脉冲:\t%d\n减速档每格脉冲:\t%d\n匀速发送时间值:\t%d us", 
			bSpeedLevel+1,fAecTime,fDecTime,m_parameterOnline.m_dwAecNum,m_parameterOnline.m_dwDecNum,m_parameterOnline.m_wAecSUnit,m_parameterOnline.m_wDecSUnit,m_parameterOnline.m_dwEvenV);
	}

	tableFile.WriteString(str);
	tableFile.Close();
	
	if (bRemind==TRUE)
	{
		if (fAecTime <= 0.0
			|| fDecTime <= 0.0
			|| m_parameterOnline.m_dwAecNum <= 0
			|| m_parameterOnline.m_dwDecNum <= 0
			|| m_parameterOnline.m_wAecSUnit <= 0
			|| m_parameterOnline.m_wDecSUnit <= 0
			|| m_parameterOnline.m_dwEvenV <= 0)
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

BOOL COnlineTimeTableDlg::GetSpeedLevelInfo()
{
	CString str;
	
	m_pComPort->m_parameterOnline.Init();

	if (m_pComPort->servo_get_online_timetable_paramater(m_bMotoWay,m_bSpeedLevel,CCmdTimeOut::get_online_timetable_pamarater,FALSE))
	{
		CPoint c;
		float fAceUnitS = 0,fDceUnitS = 0,fEvenV = 0;

		fAceUnitS = ((CSFBoardDlg*)m_pSFDlg)->Pulse2MM(m_pComPort->m_parameterOnline.m_wAecSUnit);
		fDceUnitS = ((CSFBoardDlg*)m_pSFDlg)->Pulse2MM(m_pComPort->m_parameterOnline.m_wDecSUnit);
		fEvenV = ((CSFBoardDlg*)m_pSFDlg)->Pulse2MM((int)((float)TIME_CONVERT_VALUE/(float)m_pComPort->m_parameterOnline.m_dwEvenV));

		m_fAecS = fAceUnitS*m_pComPort->m_parameterOnline.m_dwAecNum;
		m_fDecS = fDceUnitS*m_pComPort->m_parameterOnline.m_dwDecNum;
		m_fEvenV = fEvenV;
		m_nAecNum = m_pComPort->m_parameterOnline.m_dwAecNum;
		m_nDecNum = m_pComPort->m_parameterOnline.m_dwDecNum;
		m_nTimeUnit = m_pComPort->m_parameterOnline.m_wTimeUnit;

		if (m_fAecS <= 0.00000001)
			m_fAecS = 100;
		if (m_fDecS <= 0.00000001)
			m_fDecS = 100;
		if (m_fEvenV <= 0.00000001)
			m_fEvenV = 300;
		if (m_nAecNum <= 0.00000001)
			m_nAecNum = 200;
		if (m_nDecNum <= 0.00000001)
			m_nDecNum = 200;

		SetTimeTableParameter();

		m_AecControlPoints.RemoveAll();
		m_AecControlPoints.Add(m_ptCurveAecStart);
		for (int i=0; i<m_pComPort->m_parameterOnline.m_bAecRecordNum; i++)
		{
			c.x = m_ptCurveAecStart.x + LOWORD(m_pComPort->m_parameterOnline.m_dwAecRecordArray.GetAt(i));
			c.y = m_ptCurveAecStart.y - HIWORD(m_pComPort->m_parameterOnline.m_dwAecRecordArray.GetAt(i));
			m_AecControlPoints.Add(c);
		}
		m_AecControlPoints.Add(m_ptCurveAecEnd);

		m_DecControlPoints.RemoveAll();
		m_DecControlPoints.Add(m_ptCurveDecStart);

		for (int i=0; i<m_pComPort->m_parameterOnline.m_bDecRecordNum; i++)
		{
			c.x = m_ptCurveDecStart.x + LOWORD(m_pComPort->m_parameterOnline.m_dwDecRecordArray.GetAt(i));
			c.y = m_ptCurveDecStart.y + HIWORD(m_pComPort->m_parameterOnline.m_dwDecRecordArray.GetAt(i));
			m_DecControlPoints.Add(c);
		}
		m_DecControlPoints.Add(m_ptCurveDecEnd);

		InvalidateRect(m_rcAec);
		InvalidateRect(m_rcDec);

		WORD wAecTime,wDecTime;
		CalculateTime(wAecTime,wDecTime);
		return TRUE;
	}
	else
	{
		str.Format("得联机第 %d 档参数--失败",m_bSpeedLevel+1);
		AfxMessageBox(str);
		return FALSE;
	}
}

BOOL COnlineTimeTableDlg::ExportSpeedLevelInfo(CString strEeportFile)
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
	UINT nDecControlX[10];
	UINT nDecControlY[10];

	float fAecS = m_fAecS;
	float fDecS = m_fDecS;
	float fEvenV = m_fEvenV;
	UINT nAecNum = m_nAecNum;
	UINT nDecNum = m_nDecNum;
	UINT nAecControlNum = m_AecControlPoints.GetCount() - 2;
	UINT nDecControlNum = m_DecControlPoints.GetCount() - 2;

	for (int i=0;i<nAecControlNum;i++)
	{
		nAecControlX[i] = abs(m_AecControlPoints.GetAt(i+1).x - m_ptCurveAecStart.x);
		nAecControlY[i] = abs(m_AecControlPoints.GetAt(i+1).y - m_ptCurveAecStart.y);
	}

	for (int i=0;i<nDecControlNum;i++)
	{
		nDecControlX[i] = abs(m_DecControlPoints.GetAt(i+1).x - m_ptCurveDecStart.x);
		nDecControlY[i] = abs(m_DecControlPoints.GetAt(i+1).y - m_ptCurveDecStart.y);
	}

	WRITE_PROFILE_INFO("%.2f",fAecS);
	WRITE_PROFILE_INFO("%.2f",fDecS);
	WRITE_PROFILE_INFO("%.2f",fEvenV);
	WRITE_PROFILE_INFO("%d",nAecNum);
	WRITE_PROFILE_INFO("%d",nDecNum);
	WRITE_PROFILE_INFO("%d",nAecControlNum);
	WRITE_PROFILE_INFO("%d",nDecControlNum);

	WRITE_PROFILE_INFO_ARRAY("%d",nAecControlX,nAecControlNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nAecControlY,nAecControlNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nDecControlX,nDecControlNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nDecControlY,nDecControlNum);

	return TRUE;
}
void COnlineTimeTableDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL COnlineTimeTableDlg::ImportSpeedLevelInfo(CString strImportFile)
{
	CString strFilePath = "",str = "";
	CString strIniFile,strSection;
	char szReg[200];
	CPoint c;

	strIniFile = strImportFile;
	strSection.Format("SpeedInfo_%d",m_bSpeedLevel);

	UINT nAecControlX[10];
	UINT nAecControlY[10];
	UINT nDecControlX[10];
	UINT nDecControlY[10];
	float fAecS = 0,fDecS = 0,fEvenV = 0;
	UINT nAecNum = 0,nDecNum = 0,nAecControlNum = 0,nDecControlNum = 0;

	GET_PROFILE_INFO_FLOAT(fAecS,0,0,500);
	GET_PROFILE_INFO_FLOAT(fDecS,0,0,500);
	GET_PROFILE_INFO_FLOAT(fEvenV,0,0,2000);
	GET_PROFILE_INFO_INT(nAecNum,0,0,2500,UINT);
	GET_PROFILE_INFO_INT(nDecNum,0,0,2500,UINT);
	GET_PROFILE_INFO_INT(nAecControlNum,0,0,10,UINT);
	GET_PROFILE_INFO_INT(nDecControlNum,0,0,10,UINT);

	GET_PROFILE_INFO_ARRAY_INT(nAecControlX,nAecControlNum,0,0,1000,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nAecControlY,nAecControlNum,0,0,1000,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nDecControlX,nDecControlNum,0,0,1000,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nDecControlY,nDecControlNum,0,0,1000,UINT);

	m_fAecS = fAecS;
	m_fDecS = fDecS;
	m_fEvenV = fEvenV;
	m_nAecNum = nAecNum;
	m_nDecNum = nDecNum;

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

	m_DecControlPoints.RemoveAll();
	m_DecControlPoints.Add(m_ptCurveDecStart);
	for (int i=0; i<nDecControlNum; i++)
	{
		c.x = m_ptCurveDecStart.x + nDecControlX[i];
		c.y = m_ptCurveDecStart.y + nDecControlY[i];
		m_DecControlPoints.Add(c);
	}
	m_DecControlPoints.Add(m_ptCurveDecEnd);

	InvalidateRect(m_rcAec);
	InvalidateRect(m_rcDec);

	WORD wAecTime,wDecTime;
	CalculateTime(wAecTime,wDecTime);
	
	m_bSpeedLevel = 0xFF;
	if(!AddTimeTable(FALSE))
	{
		return FALSE;
	}
	return TRUE;
}