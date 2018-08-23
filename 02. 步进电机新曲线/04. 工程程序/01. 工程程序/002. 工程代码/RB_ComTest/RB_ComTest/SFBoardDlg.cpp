// SFBoardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RB_ComTest.h"
#include "SFBoardDlg.h"
#include "OnlineTimeTableDlg.h"
#include "OfflineTimeTableDlg.h"

void RestrictAecCurve(POINT points[], int nPointCount, CRect* pRect, CPoint* pPtMoveNow, CPoint* pPtMoveLast)
{
	int ax,ay;

	if (pPtMoveNow==NULL && pPtMoveLast==NULL)
	{
		ax = 0;
		ay = 0;
	}
	else
	{
		ax = pPtMoveNow->x - pPtMoveLast->x;
		ay = pPtMoveNow->y - pPtMoveLast->y;
	}

	if (ay<=0)
	{
		for(int i=nPointCount-1; i>0; i--)
		{
			if (points[i-1].y<points[i].y)
			{
				points[i-1].y = points[i].y;
			}
		}

	}

	if (ay>0)
	{
		for(int i=0; i<nPointCount-1; i++)
		{
			if (points[i+1].y>points[i].y)
			{
				points[i+1].y = points[i].y;
			}
		}
	}

	if (ax>=0)
	{
		for(int i=nPointCount-1; i>0; i--)
		{
			if (points[i-1].x>points[i].x)
			{
				points[i-1].x = points[i].x;
			}
		}
	}

	if (ax<0)
	{
		for(int i=0; i<nPointCount-1; i++)
		{
			if (points[i+1].x<points[i].x)
			{
				points[i+1].x = points[i].x;
			}
		}
	}

	for(int i=nPointCount-1; i>=0; i--)
	{
		if (points[i].y>pRect->bottom)
		{
			points[i].y=pRect->bottom;
		}

		if (points[i].x<pRect->left)
		{
			points[i].x=pRect->left;
		}

		if (points[i].y<pRect->top)
		{
			points[i].y=pRect->top;
		}

		if (points[i].x>pRect->right)
		{
			points[i].x=pRect->right;
		}
	}
}

void RestrictDecCurve(POINT points[], int nPointCount, CRect* pRect, CPoint* pPtMoveNow, CPoint* pPtMoveLast)
{
	int ax,ay;

	if (pPtMoveNow==NULL && pPtMoveLast==NULL)
	{
		ax = 0;
		ay = 0;
	}
	else
	{
		ax = pPtMoveNow->x - pPtMoveLast->x;
		ay = pPtMoveNow->y - pPtMoveLast->y;
	}

	if (ay>0)
	{
		for(int i=nPointCount-1; i>0; i--)
		{
			if (points[i-1].y>points[i].y)
			{
				points[i-1].y = points[i].y;
			}
		}

	}

	if (ay<=0)
	{
		for(int i=0; i<nPointCount-1; i++)
		{
			if (points[i+1].y<points[i].y)
			{
				points[i+1].y = points[i].y;
			}
		}
	}

	if (ax>0)
	{
		for(int i=nPointCount-1; i>0; i--)
		{
			if (points[i-1].x>points[i].x)
			{
				points[i-1].x = points[i].x;
			}
		}
	}

	if (ax<=0)
	{
		for(int i=0; i<nPointCount-1; i++)
		{
			if (points[i+1].x<points[i].x)
			{
				points[i+1].x = points[i].x;
			}
		}
	}

	for(int i=nPointCount-1; i>=0; i--)
	{
		if (points[i].y>pRect->bottom)
		{
			points[i].y=pRect->bottom;
		}

		if (points[i].x<pRect->left)
		{
			points[i].x=pRect->left;
		}

		if (points[i].y<pRect->top)
		{
			points[i].y=pRect->top;
		}

		if (points[i].x>pRect->right)
		{
			points[i].x=pRect->right;
		}
	}
}

double Distance(const POINT& p1, const POINT& p2)
{
	int dx = abs(p1.x - p2.x);
	int dy = abs(p1.y - p2.y);

	return (double)sqrt((double)(dx*dx + dy*dy));
}

// SFBoardDlg 对话框

IMPLEMENT_DYNAMIC(CSFBoardDlg, CDialog)

CSFBoardDlg::CSFBoardDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSFBoardDlg::IDD, pParent)
{
	m_pParentThread = NULL;
	bIsNewCurve = false;
}

CSFBoardDlg::~CSFBoardDlg()
{
}

void CSFBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SF_BOARDTYPE, m_cbBoardType);
	DDX_Control(pDX, IDC_COMBO_SF_SPEEDLEVEL, m_cbSpeedLevel);
	DDX_Control(pDX, IDC_COMBO_SF_OFFLINEMOVE_DIRECTION, m_cbOfflineMoveDir);
	DDX_Control(pDX, IDC_COMBO_SF_OFFLINEMOVE_SPEED, m_cbOfflineMoveSpd);
	DDX_Control(pDX, IDC_COMBO_SF_SPECIAL_POSITION_X, m_cbSpecialPositionX);
	DDX_Control(pDX, IDC_COMBO_SF_SPECIAL_POSITION_Y, m_cbSpecialPositionY);
	DDX_Control(pDX, IDC_COMBO_SF_SPECIAL_POSITION_Z, m_cbSpecialPositionZ);
	DDX_Control(pDX, IDC_COMBO_SF_XY_SPECIAL_POSITION_X, m_xy_cbSpecialPositionX);
	DDX_Control(pDX, IDC_COMBO_SF_XY_SPECIAL_POSITION_Y, m_xy_cbSpecialPositionY);
	DDX_Control(pDX, IDC_CHECK_SF_QUERY_COM, m_ckQueryCom);
	DDX_Control(pDX, IDC_EDIT_SF_XY_MOTORY_SG, m_xy_ckMotorY_SG);
}


BEGIN_MESSAGE_MAP(CSFBoardDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SF_SPEEDLEVEL_ADD, &CSFBoardDlg::OnBnClickedButtonSfSpeedlevelAdd)
	ON_BN_CLICKED(IDC_BUTTON_SPEEDLEVEL_MODIFY, &CSFBoardDlg::OnBnClickedButtonSpeedlevelModify)
	ON_CBN_SELCHANGE(IDC_COMBO_SF_BOARDTYPE, &CSFBoardDlg::OnCbnSelchangeComboSfBoardtype)
	ON_BN_CLICKED(IDC_BUTTON_SF_COUNT_FULLPULSE, &CSFBoardDlg::OnBnClickedButtonSfCountFullpulse)
	ON_EN_CHANGE(IDC_EDIT_SF_MOVERELATIVE_MM, &CSFBoardDlg::OnEnChangeEditSfMoverelativeMm)
	ON_EN_CHANGE(IDC_EDIT_SF_MOVERELATIVE_RE_MM, &CSFBoardDlg::OnEnChangeEditSfMoverelativeReMm)
	ON_EN_CHANGE(IDC_EDIT_SF_MOVERELATIVE_PULSE, &CSFBoardDlg::OnEnChangeEditSfMoverelativePulse)
	ON_EN_CHANGE(IDC_EDIT_SF_MOVERELATIVE_RE_PULSE, &CSFBoardDlg::OnEnChangeEditSfMoverelativeRePulse)
	ON_BN_CLICKED(IDC_BUTTON_SF_MOVERELATIVE, &CSFBoardDlg::OnBnClickedButtonSfMoverelative)
	ON_EN_CHANGE(IDC_EDIT_SF_MOVEABSOLUTE_MM, &CSFBoardDlg::OnEnChangeEditSfMoveabsoluteMm)
	ON_EN_CHANGE(IDC_EDIT_SF_MOVEABSOLUTE_PULSE, &CSFBoardDlg::OnEnChangeEditSfMoveabsolutePulse)
	ON_BN_CLICKED(IDC_BUTTON_SF_MOVEABSOLUTE, &CSFBoardDlg::OnBnClickedButtonSfMoveabsolute)
	ON_BN_CLICKED(IDC_BUTTON_SF_GETPOSITION, &CSFBoardDlg::OnBnClickedButtonSfGetposition)
	ON_BN_CLICKED(IDC_BUTTON_SF_SPEEDLEVEL_ADD_OFFLINE, &CSFBoardDlg::OnBnClickedButtonSfSpeedlevelAddOffline)
	ON_BN_CLICKED(IDC_BUTTON_SF_MOVESTOP, &CSFBoardDlg::OnBnClickedButtonSfMovestop)
	ON_BN_CLICKED(IDC_BUTTON_SF_OFFLINEMOVE, &CSFBoardDlg::OnBnClickedButtonSfOfflinemove)
	ON_BN_CLICKED(IDC_BUTTON_SF_GET_FULLPULSE, &CSFBoardDlg::OnBnClickedButtonSfGetFullpulse)
	ON_BN_CLICKED(IDC_BUTTON_SF_SET_SPECIAL_POSITION, &CSFBoardDlg::OnBnClickedButtonSfSetSpecialPosition)
	ON_EN_CHANGE(IDC_EDIT_SF_SPECIAL_POSITION_MM, &CSFBoardDlg::OnEnChangeEditSfSpecialPositionMm)
	ON_EN_CHANGE(IDC_EDIT_SF_SPECIAL_POSITION_PULSE, &CSFBoardDlg::OnEnChangeEditSfSpecialPositionPulse)
	ON_BN_CLICKED(IDC_BUTTON_SF_GET_SPECIAL_POSITION, &CSFBoardDlg::OnBnClickedButtonSfGetSpecialPosition)
	ON_BN_CLICKED(IDC_BUTTON_SF_MOVE2_SPECIAL_POSITION, &CSFBoardDlg::OnBnClickedButtonSfMove2SpecialPosition)
	ON_BN_CLICKED(IDC_BUTTON_SF_DELETE_TIMETABLE_DATA, &CSFBoardDlg::OnBnClickedButtonSfDeleteTimetableData)
	ON_BN_CLICKED(IDC_BUTTON_SF_GET_TIMETABLE_VITAL_PARAMETER, &CSFBoardDlg::OnBnClickedButtonSfGetTimetableVitalParameter)
	ON_BN_CLICKED(IDC_BUTTON_BOARDINFO_EXPORT, &CSFBoardDlg::OnBnClickedButtonBoardinfoExport)
	ON_BN_CLICKED(IDC_BUTTON_BOARDINFO_IMPORT, &CSFBoardDlg::OnBnClickedButtonBoardinfoImport)
	ON_BN_CLICKED(IDC_BUTTON_SF_XY_MOVE2_SPECIAL_POSITION, &CSFBoardDlg::OnBnClickedButtonSfXyMove2SpecialPosition)
	ON_BN_CLICKED(IDC_BUTTON_SF_XY_MOVERELATIVE, &CSFBoardDlg::OnBnClickedButtonSfXyMoverelative)
	ON_BN_CLICKED(IDC_BUTTON_SF_XY_MOVEABSOLUTE, &CSFBoardDlg::OnBnClickedButtonSfXyMoveabsolute)
	ON_BN_CLICKED(IDC_BUTTON_SF_XY_PRINTLINE, &CSFBoardDlg::OnBnClickedButtonSfXyPrintline)
	ON_EN_CHANGE(IDC_EDIT_SF_XY_MOVE_X_MM, &CSFBoardDlg::OnEnChangeEditSfXyMoveXMm)
	ON_EN_CHANGE(IDC_EDIT_SF_XY_MOVE_X_PULSE, &CSFBoardDlg::OnEnChangeEditSfXyMoveXPulse)
	ON_EN_CHANGE(IDC_EDIT_SF_XY_MOVE_Y_MM, &CSFBoardDlg::OnEnChangeEditSfXyMovYMm)
	ON_EN_CHANGE(IDC_EDIT_SF_XY_MOVE_Y_PULSE, &CSFBoardDlg::OnEnChangeEditSfXyMoveYPulse)
	ON_EN_CHANGE(IDC_EDIT_SF_XY_MOVE_RE_X_MM, &CSFBoardDlg::OnEnChangeEditSfXyMoveReXMm)
	ON_EN_CHANGE(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE, &CSFBoardDlg::OnEnChangeEditSfXyMoveReXPulse)
	ON_BN_CLICKED(IDOK, &CSFBoardDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SFBoardDlg 消息处理程序

BOOL CSFBoardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString str;
	int		i;
	float	fMM;

	m_cbBoardType.ResetContent();

	if (m_bThreeAxisServo)
	{//三轴伺服板
		m_cbBoardType.AddString("三轴-X");
		m_cbBoardType.AddString("三轴-Y-导带");
		m_cbBoardType.AddString("三轴-Y-丝杆");
		m_cbBoardType.AddString("三轴-Z-导带");
		m_cbBoardType.AddString("三轴-Z-丝杆");
	}
	else
	{
		m_cbBoardType.AddString("小车");
		m_cbBoardType.AddString("导带");
		m_cbBoardType.AddString("丝杆");
	}

	m_cbBoardType.SetCurSel(0);
	m_bMotoWay = BoardType2MotorWay(0);

	if (AfxMessageBox("使用光栅控制板需切换至调试模式\n是否切换?",MB_YESNO)==IDYES)
	{
		if (!m_pComPort->ras_select(0x01,5000,FALSE))
		{
			AfxMessageBox("切换至调试模式失败");
		}
	}

	m_pComPort->m_protocolstatus.bExistSpeedLevel = 0;
	m_cbSpeedLevel.ResetContent();
	m_cbSpeedLevel.EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(FALSE);

	if (m_pComPort->servo_get_existent_speedlevel(m_bMotoWay,CCmdTimeOut::get_existent_speedlevel,FALSE))
	{
		int nExistSpeedLevel = m_pComPort->m_protocolstatus.bExistSpeedLevel;
		if (nExistSpeedLevel>0)
		{
			m_cbSpeedLevel.EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(TRUE);

			for (i=0;i<nExistSpeedLevel;i++)
			{
				str.Format("%d",i+1);
				m_cbSpeedLevel.AddString(str);
			}
			m_cbSpeedLevel.SetCurSel(0);

			if (nExistSpeedLevel>=2)
			{
				((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(TRUE);
			}
		}
	}	
	else
	{
		AfxMessageBox("获取档位总数--失败");
	}
	
	if (m_pComPort->IsDemoMode())
	{
		((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(TRUE);
	}

	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_PULSE))->SetWindowText("10000");
	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_PULSE))->SetWindowText("10000");
	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_PULSE))->SetWindowText("10000");
	((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_PULSE))->SetWindowText("10000");

	fMM = Pulse2MM(10000);
	str.Format("%.2f",fMM);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_MM))->SetWindowText(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_MM))->SetWindowText(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_MM))->SetWindowText(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_MM))->SetWindowText(str);

	m_cbOfflineMoveDir.ResetContent();
	m_cbOfflineMoveDir.AddString("正向");
	m_cbOfflineMoveDir.AddString("反向");
	m_cbOfflineMoveDir.SetCurSel(0);

	m_cbOfflineMoveSpd.ResetContent();
	m_cbOfflineMoveSpd.AddString("慢速");
	m_cbOfflineMoveSpd.AddString("快速");
	m_cbOfflineMoveSpd.SetCurSel(0);

	m_cbSpecialPositionX.ResetContent();
	m_cbSpecialPositionX.AddString("当前位置");
	m_cbSpecialPositionX.AddString("最小位置");
	m_cbSpecialPositionX.AddString("最大位置");
	m_cbSpecialPositionX.AddString("上次位置");
	m_cbSpecialPositionX.AddString("待机位");
	m_cbSpecialPositionX.AddString("检测位");
	m_cbSpecialPositionX.AddString("清洗位");
	m_cbSpecialPositionX.AddString("保湿位");
	m_cbSpecialPositionX.SetCurSel(0);

	m_cbSpecialPositionY.ResetContent();
	m_cbSpecialPositionY.AddString("当前位置");
	m_cbSpecialPositionY.AddString("最小位置");
	m_cbSpecialPositionY.AddString("最大位置");
	m_cbSpecialPositionY.AddString("待机位");
	m_cbSpecialPositionY.AddString("上次位置");
	m_cbSpecialPositionY.SetCurSel(0);

	m_cbSpecialPositionZ.ResetContent();
	m_cbSpecialPositionZ.AddString("当前位置");
	m_cbSpecialPositionZ.AddString("最小位置");
	m_cbSpecialPositionZ.AddString("最大位置");
	m_cbSpecialPositionZ.AddString("待机位");
	m_cbSpecialPositionZ.AddString("上次位置");
	m_cbSpecialPositionZ.SetCurSel(0);

	m_cbSpecialPositionX.EnableWindow(TRUE);
	m_cbSpecialPositionY.EnableWindow(FALSE);
	m_cbSpecialPositionZ.EnableWindow(FALSE);


	m_xy_cbSpecialPositionX.ResetContent();
	m_xy_cbSpecialPositionX.AddString("当前位置");
	m_xy_cbSpecialPositionX.AddString("最小位置");
	m_xy_cbSpecialPositionX.AddString("最大位置");
	m_xy_cbSpecialPositionX.AddString("上次位置");
	m_xy_cbSpecialPositionX.AddString("待机位");
	m_xy_cbSpecialPositionX.AddString("检测位");
	m_xy_cbSpecialPositionX.AddString("清洗位");
	m_xy_cbSpecialPositionX.AddString("保湿位");
	m_xy_cbSpecialPositionX.SetCurSel(0);

	m_xy_cbSpecialPositionY.ResetContent();
	m_xy_cbSpecialPositionY.AddString("当前位置");
	m_xy_cbSpecialPositionY.AddString("最小位置");
	m_xy_cbSpecialPositionY.AddString("最大位置");
	m_xy_cbSpecialPositionY.AddString("待机位");
	m_xy_cbSpecialPositionY.AddString("上次位置");
	m_xy_cbSpecialPositionY.SetCurSel(0);

	ENABLECONTROL(IDC_BUTTON_SF_XY_MOVE2_SPECIAL_POSITION, m_bThreeAxisServo);
	ENABLECONTROL(IDC_BUTTON_SF_XY_MOVERELATIVE, m_bThreeAxisServo);
	ENABLECONTROL(IDC_BUTTON_SF_XY_MOVEABSOLUTE, m_bThreeAxisServo);
	ENABLECONTROL(IDC_BUTTON_SF_XY_PRINTLINE, m_bThreeAxisServo);

	ENABLECONTROL(IDC_EDIT_SF_XY_MOTORY_SG, m_bThreeAxisServo);
	ENABLECONTROL(IDC_COMBO_SF_XY_SPECIAL_POSITION_X, m_bThreeAxisServo);
	ENABLECONTROL(IDC_COMBO_SF_XY_SPECIAL_POSITION_Y, m_bThreeAxisServo);

	ENABLECONTROL(IDC_EDIT_SF_XY_SPEED_X, m_bThreeAxisServo);
	ENABLECONTROL(IDC_EDIT_SF_XY_SPEED_Y, m_bThreeAxisServo);
	ENABLECONTROL(IDC_EDIT_SF_XY_MOVE_X_PULSE, m_bThreeAxisServo);
	ENABLECONTROL(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE, m_bThreeAxisServo);
	ENABLECONTROL(IDC_EDIT_SF_XY_MOVE_Y_PULSE, m_bThreeAxisServo);
	ENABLECONTROL(IDC_EDIT_SF_XY_MOVE_X_MM, m_bThreeAxisServo);
	ENABLECONTROL(IDC_EDIT_SF_XY_MOVE_Y_MM, m_bThreeAxisServo);
	ENABLECONTROL(IDC_EDIT_SF_XY_MOVE_RE_X_MM, m_bThreeAxisServo);

	i = 0;
	str.Format("%d", i);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_X))->SetWindowText(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_Y))->SetWindowText(str);

	i = 10000;
	str.Format("%d", i);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_PULSE))->SetWindowText(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE))->SetWindowText(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_PULSE))->SetWindowText(str);

	fMM = xy_Pulse2MM(i, sf_boardtype_motor_3axis_x);
	str.Format("%.2f",fMM);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_MM))->SetWindowText(str);

	m_xy_ckMotorY_SG.SetCheck(0);
	fMM = xy_Pulse2MM(i, sf_boardtype_motor_3axis_y_dd);
	str.Format("%.2f",fMM);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_MM))->SetWindowText(str);

	fMM = xy_Pulse2MM(i, sf_boardtype_motor_3axis_x);
	str.Format("%.2f",fMM);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_MM))->SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSFBoardDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_pParentThread != NULL)
	{
		m_pParentThread->PostThreadMessage(WM_QUIT, 0, 0);
	}
	CDialog::PostNcDestroy();
	delete this;
}

int CSFBoardDlg::MM2Pulse(float fMM)
{
	int nPulse,nSel = m_cbBoardType.GetCurSel();

	if (m_bThreeAxisServo)
	{
		switch(nSel)
		{
		case sf_boardtype_motor_3axis_x:
			nPulse = fMM * (*m_pfReducer_x)/(*m_pfDollyPerimeter)*(*m_pfPulsesPerRevolution_x);
			break;
		case sf_boardtype_motor_3axis_y_dd:
			nPulse = fMM * (*m_pfReducer_y)/(*m_pfDeferentPerimeter)*(*m_pfPulsesPerRevolution_y);
			break;
		case sf_boardtype_motor_3axis_y_sg:
			nPulse = fMM * (*m_pfReducer_y)/(*m_pfDeferentPitch)*(*m_pfPulsesPerRevolution_y);
			break;
		case sf_boardtype_motor_3axis_z_dd:
			nPulse = fMM * (*m_pfReducer_z)/(*m_pfDeferentPerimeter_z)*(*m_pfPulsesPerRevolution_z);
			break;
		case sf_boardtype_motor_3axis_z_sg:
			nPulse = fMM * (*m_pfReducer_z)/(*m_pfDeferentPitch_z)*(*m_pfPulsesPerRevolution_z);
			break;
		}
	}
	else
	{
		switch(nSel)
		{
		case sf_boardtype_motor_x:
			nPulse = fMM * (*m_pfReducer_x)/(*m_pfDollyPerimeter)*(*m_pfPulsesPerRevolution_x);
			break;
		case sf_boardtype_motor_y_dd:
			nPulse = fMM * (*m_pfReducer_y)/(*m_pfDeferentPerimeter)*(*m_pfPulsesPerRevolution_y);
			break;
		case sf_boardtype_motor_y_sg:
			nPulse = fMM * (*m_pfReducer_y)/(*m_pfDeferentPitch)*(*m_pfPulsesPerRevolution_y);
			break;
		}
	}

	return nPulse;
}

float CSFBoardDlg::Pulse2MM(int nPulse)
{
	float fMM;
	int nSel = m_cbBoardType.GetCurSel();

	if (m_bThreeAxisServo)
	{
		switch(nSel)
		{
		case sf_boardtype_motor_3axis_x:
			fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_x)*(*m_pfDollyPerimeter)/(*m_pfReducer_x);
			break;
		case sf_boardtype_motor_3axis_y_dd:
			fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_y)*(*m_pfDeferentPerimeter)/(*m_pfReducer_y);
			break;
		case sf_boardtype_motor_3axis_y_sg:
			fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_y)*(*m_pfDeferentPitch)/(*m_pfReducer_y);
			break;
		case sf_boardtype_motor_3axis_z_dd:
			fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_z)*(*m_pfDeferentPerimeter_z)/(*m_pfReducer_z);
			break;
		case sf_boardtype_motor_3axis_z_sg:
			fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_z)*(*m_pfDeferentPitch_z)/(*m_pfReducer_z);
			break;
		}
	}
	else
	{
		switch(nSel)
		{
		case sf_boardtype_motor_x:
			fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_x)*(*m_pfDollyPerimeter)/(*m_pfReducer_x);
			break;
		case sf_boardtype_motor_y_dd:
			fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_y)*(*m_pfDeferentPerimeter)/(*m_pfReducer_y);
			break;
		case sf_boardtype_motor_y_sg:
			fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_y)*(*m_pfDeferentPitch)/(*m_pfReducer_y);
			break;
		}

	}

	return fMM;
}

void CSFBoardDlg::OnBnClickedButtonSfSpeedlevelAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;

	//if (m_cbSpeedLevel.GetCount()<2)
	//{
	//	COfflineTimeTableDlg dlg(this);
	//	dlg.m_nOperationType = 0;
	//	dlg.m_pSFDlg = this;
	//	dlg.m_bSpeedLevel = 0xFF;
	//	dlg.m_bMotoWay = m_bMotoWay;
	//	dlg.DoModal();
	//}
	//else
	//{
		COnlineTimeTableDlg dlg(this);
		dlg.m_nOperationType = 0;
		dlg.m_pSFDlg = this;
		dlg.m_bSpeedLevel = 0xFF;
		dlg.m_bMotoWay = m_bMotoWay;
		dlg.DoModal();
	//}
	
	if (m_pComPort->servo_get_existent_speedlevel(m_bMotoWay,CCmdTimeOut::get_existent_speedlevel,FALSE))
	{
		int nExistSpeedLevel = m_pComPort->m_protocolstatus.bExistSpeedLevel;
		if (nExistSpeedLevel>0)
		{
			m_cbSpeedLevel.EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(TRUE);

			m_cbSpeedLevel.ResetContent();
			for (int i=0;i<nExistSpeedLevel;i++)
			{
				str.Format("%d",i+1);
				m_cbSpeedLevel.AddString(str);
			}
			m_cbSpeedLevel.SetCurSel(0);
		}
	}	
	else
	{
		AfxMessageBox("获取档位总数--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSpeedlevelModify()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSpeedLevel = m_cbSpeedLevel.GetCurSel();
	if (nSpeedLevel<2)
	{
		COfflineTimeTableDlg dlg(this);
		dlg.m_nOperationType = 1;
		dlg.m_pSFDlg = this;
		dlg.m_bSpeedLevel = nSpeedLevel;
		dlg.m_bMotoWay = m_bMotoWay;
		dlg.DoModal();
	}
	else
	{
		COnlineTimeTableDlg dlg(this);
		dlg.m_nOperationType = 1;
		dlg.m_pSFDlg = this;
		dlg.m_bSpeedLevel = nSpeedLevel;
		dlg.m_bMotoWay = m_bMotoWay;
		dlg.DoModal();
	}
}

void CSFBoardDlg::OnCbnSelchangeComboSfBoardtype()
{
	// TODO: 在此添加控件通知处理程序代码
	int		nSel = m_cbBoardType.GetCurSel();
	CString str;
	BOOL	bEnableX, bEnableY, bEnableZ;

	bEnableX = bEnableY = bEnableZ = FALSE;
	if (m_bThreeAxisServo)
	{
		switch(nSel)
		{
		case sf_boardtype_motor_3axis_x:
			bEnableX = TRUE;
			break;
		case sf_boardtype_motor_3axis_y_dd:
		case sf_boardtype_motor_3axis_y_sg:
			bEnableY = TRUE;
			break;
		case sf_boardtype_motor_3axis_z_dd:
		case sf_boardtype_motor_3axis_z_sg:
			bEnableZ = TRUE;
			break;
		}
	}
	else
	{
		switch(nSel)
		{
		case sf_boardtype_motor_x:
			bEnableX = TRUE;
			break;
		case sf_boardtype_motor_y_dd:
		case sf_boardtype_motor_y_sg:
			bEnableY = TRUE;
			break;
		}
	}
	
	m_cbSpecialPositionX.EnableWindow(bEnableX);
	m_cbSpecialPositionY.EnableWindow(bEnableY);
	m_cbSpecialPositionZ.EnableWindow(bEnableZ);

	m_bMotoWay = BoardType2MotorWay(nSel);

	m_cbSpeedLevel.ResetContent();
	m_cbSpeedLevel.EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(FALSE);
	
	m_pComPort->m_protocolstatus.bExistSpeedLevel = 0;
	if (m_pComPort->servo_get_existent_speedlevel(m_bMotoWay,CCmdTimeOut::get_existent_speedlevel,FALSE))
	{
		int nExistSpeedLevel = m_pComPort->m_protocolstatus.bExistSpeedLevel;
		if (nExistSpeedLevel>0)
		{
			m_cbSpeedLevel.EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(TRUE);

			for (int i=0;i<nExistSpeedLevel;i++)
			{
				str.Format("%d",i+1);
				m_cbSpeedLevel.AddString(str);
			}
			m_cbSpeedLevel.SetCurSel(0);

			if (nExistSpeedLevel>=2)
			{
				((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(TRUE);
			}
		}
	}	
	else
	{
		AfxMessageBox("获取档位总数--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfCountFullpulse()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pComPort->servo_count_fullpulse(m_bMotoWay,CCmdTimeOut::count_fullpulse,FALSE))
	{
		AfxMessageBox("全程脉冲计数--失败");
	}

}

void CSFBoardDlg::OnEnChangeEditSfMoverelativeMm()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_MM))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_MM))->GetWindowText(str);
		fMM = atof(str);
		nPulse = MM2Pulse(fMM);
		str.Format("%d",nPulse);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_PULSE))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnEnChangeEditSfMoverelativeReMm()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_MM))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_MM))->GetWindowText(str);
		fMM = atof(str);
		nPulse = MM2Pulse(fMM);
		str.Format("%d",nPulse);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_PULSE))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnEnChangeEditSfMoverelativePulse()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_PULSE))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_PULSE))->GetWindowText(str);
		nPulse = atoi(str);
		fMM = Pulse2MM(nPulse);
		str.Format("%.2f",fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_MM))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnEnChangeEditSfMoverelativeRePulse()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_PULSE))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_PULSE))->GetWindowText(str);
		nPulse = atoi(str);
		fMM = Pulse2MM(nPulse);
		str.Format("%.2f",fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_MM))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnBnClickedButtonSfMoverelative()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPulse,nRePulse,nSpeedType;
	DWORD dwPulse;
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_PULSE))->GetWindowText(str);
	nPulse = atoi(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVERELATIVE_RE_PULSE))->GetWindowText(str);
	nRePulse = atoi(str);

	if (nPulse<0)
	{
		dwPulse = ((0 - nPulse)|0x01<<31);
	}
	else
	{
		dwPulse = nPulse;
	}
	nSpeedType = m_cbSpeedLevel.GetCurSel();

	if (nSpeedType==-1)
	{
		AfxMessageBox("请先下载时间表");
		return;
	}

	if (nSpeedType<2)
	{
		AfxMessageBox("请选择联机(第三档以后)速度档");
		return;
	}

	if (!m_pComPort->servo_motor_move_relative(m_bMotoWay,nSpeedType,dwPulse,nRePulse,CCmdTimeOut::motor_move_relative,FALSE))
	{
		AfxMessageBox("移动相对位置--失败");
	}

	if (m_ckQueryCom.GetCheck())
	{
		BOOL bFlag = TRUE;
		while(1)
		{
			m_pComPort->m_protocolstatus.wComBoardCondition = 0;

			bFlag = !bFlag;

			if (!m_pComPort->com_get_board_condition(5000,bFlag))
			{
				AfxMessageBox("查询各板卡工作情况--出错");
				break;
			}

			WORD wCondition = m_pComPort->m_protocolstatus.wComBoardCondition;

			if ((wCondition&0x0001)==0x0001)
			{
				AfxMessageBox("强制取消打印");
				break;
			}
			else
			{
				if (m_bMotoWay==addr_servo_x)
				{
					if ((wCondition&(0x0001<<1))==0)
					{
						AfxMessageBox("X向移动停止");
						break;
					}
					else
						continue;
				}
				else
				{
					if ((wCondition&(0x0001<<2))==0)
					{
						AfxMessageBox("Y向移动停止");
						break;
					}
					else
						continue;
				}
			}
		}
	}
}

void CSFBoardDlg::OnEnChangeEditSfMoveabsoluteMm()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_MM))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_MM))->GetWindowText(str);
		fMM = atof(str);
		nPulse = MM2Pulse(fMM);
		str.Format("%d",nPulse);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_PULSE))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnEnChangeEditSfMoveabsolutePulse()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_PULSE))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_PULSE))->GetWindowText(str);
		nPulse = atoi(str);
		fMM = Pulse2MM(nPulse);
		str.Format("%.2f",fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_MM))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnBnClickedButtonSfMoveabsolute()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPulse,nSpeedType;
	DWORD dwPulse;
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_PULSE))->GetWindowText(str);
	nPulse = atoi(str);

	if (nPulse<0)
	{
		dwPulse = ((0 - nPulse)|0x01<<31);
	}
	else
	{
		dwPulse = nPulse;
	}
	nSpeedType = m_cbSpeedLevel.GetCurSel();

	if (nSpeedType==-1)
	{
		AfxMessageBox("请先下载时间表");
		return;
	}
	if (nSpeedType<2)
	{
		AfxMessageBox("请选择联机(第三档以后)速度档");
		return;
	}

	if (!m_pComPort->servo_motor_move_absolute(m_bMotoWay,nSpeedType,dwPulse,CCmdTimeOut::motor_move_absolute,FALSE))
	{
		AfxMessageBox("移动相对位置--失败");
	}

	if (m_ckQueryCom.GetCheck())
	{
		BOOL bFlag = TRUE;
		while(1)
		{
			m_pComPort->m_protocolstatus.wComBoardCondition = 0;

			bFlag = !bFlag;

			if (!m_pComPort->com_get_board_condition(5000,bFlag))
			{
				AfxMessageBox("查询各板卡工作情况--出错");
				break;
			}

			WORD wCondition = m_pComPort->m_protocolstatus.wComBoardCondition;

			if ((wCondition&0x0001)==0x0001)
			{
				AfxMessageBox("强制取消打印");
				break;
			}
			else
			{
				if (m_bMotoWay==addr_servo_x)
				{
					if ((wCondition&(0x0001<<1))==0)
					{
						AfxMessageBox("X向移动停止");
						break;
					}
					else
						continue;
				}
				else
				{
					if ((wCondition&(0x0001<<2))==0)
					{
						AfxMessageBox("Y向移动停止");
						break;
					}
					else
						continue;
				}
			}
		}
	}
}

void CSFBoardDlg::OnBnClickedButtonSfGetposition()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pComPort->m_protocolstatus.dwCurrentPosition = 0;

	if (m_pComPort->servo_motor_get_position(m_bMotoWay,CCmdTimeOut::motor_get_position,FALSE))
	{
		CString str;
		float fMM;
		int nPulse = m_pComPort->m_protocolstatus.dwCurrentPosition;

		fMM = Pulse2MM(nPulse);
		str.Format("当前位置: %.2f mm %d pulse",fMM,nPulse);
		AfxMessageBox(str);

		str.Format("%d", nPulse);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_PULSE))->SetWindowText(str);
		str.Format("%.2f",fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_MOVEABSOLUTE_MM))->SetWindowText(str);
	}
	else
	{
		AfxMessageBox("得当前位置--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfSpeedlevelAddOffline()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;

	COfflineTimeTableDlg dlg(this);
	dlg.m_nOperationType = 0;
	dlg.m_pSFDlg = this;
	dlg.m_bSpeedLevel = 0xFF;
	dlg.m_bMotoWay = m_bMotoWay;
	dlg.DoModal();
	
	if (m_pComPort->servo_get_existent_speedlevel(m_bMotoWay,CCmdTimeOut::get_existent_speedlevel,FALSE))
	{
		int nExistSpeedLevel = m_pComPort->m_protocolstatus.bExistSpeedLevel;
		if (nExistSpeedLevel>0)
		{
			m_cbSpeedLevel.EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(TRUE);

			m_cbSpeedLevel.ResetContent();
			for (int i=0;i<nExistSpeedLevel;i++)
			{
				str.Format("%d",i+1);
				m_cbSpeedLevel.AddString(str);
			}
			m_cbSpeedLevel.SetCurSel(0);

			if (nExistSpeedLevel>=2)
			{
				((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(TRUE);
			}
		}
	}	
	else
	{
		AfxMessageBox("获取档位总数--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfMovestop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pComPort->servo_motor_move_stop(m_bMotoWay,CCmdTimeOut::motor_move_stop,FALSE))
	{
		AfxMessageBox("停止移动--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfOfflinemove()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE bMoveType = 0;

	if (m_cbOfflineMoveDir.GetCurSel()==1)
	{
		bMoveType |= 0x01;
	}
	if (m_cbOfflineMoveSpd.GetCurSel()==1)
	{
		bMoveType |= 0x02;
	}
	
	if (!m_pComPort->servo_motor_move(m_bMotoWay,bMoveType,CCmdTimeOut::motor_move_offline,FALSE))
	{
		AfxMessageBox("脱机移动--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfGetFullpulse()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pComPort->m_protocolstatus.dwFullPulse = 0;

	if (m_pComPort->servo_get_fullpulse(m_bMotoWay,CCmdTimeOut::get_fullpulse,FALSE))
	{
		int nPulse = m_pComPort->m_protocolstatus.dwFullPulse;
		CString str;
		float fMM = Pulse2MM(nPulse);

		str.Format("全程脉冲数: %d pulse %.2f mm",nPulse,fMM);
		AfxMessageBox(str);
	}
	else
	{
		AfxMessageBox("得全程脉冲数--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfSetSpecialPosition()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE		bSpecialPosition;
	DWORD		dwSpecialPosition;
	CString		str;

	bSpecialPosition = SpecialPositionFromPosType();

	((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_PULSE))->GetWindowText(str);
	dwSpecialPosition = atoi(str);
	
	if (!m_pComPort->servo_set_special_position(m_bMotoWay,bSpecialPosition,dwSpecialPosition,CCmdTimeOut::set_special_position,FALSE))
	{
		AfxMessageBox("设置特殊位置--失败");
	}
}

void CSFBoardDlg::OnEnChangeEditSfSpecialPositionMm()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_MM))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_MM))->GetWindowText(str);
		fMM = atof(str);
		nPulse = MM2Pulse(fMM);
		str.Format("%d",nPulse);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_PULSE))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnEnChangeEditSfSpecialPositionPulse()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_PULSE))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_PULSE))->GetWindowText(str);
		nPulse = atoi(str);
		fMM = Pulse2MM(nPulse);
		str.Format("%.2f",fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_MM))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnBnClickedButtonSfGetSpecialPosition()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE bSpecialPosition;
	DWORD dwSpecialPosition;
	CString str;

	bSpecialPosition = SpecialPositionFromPosType();

	m_pComPort->m_protocolstatus.dwSpecialPosition = 0;
	if (m_pComPort->servo_get_special_position(m_bMotoWay,bSpecialPosition,CCmdTimeOut::get_special_position,FALSE))
	{
		dwSpecialPosition = m_pComPort->m_protocolstatus.dwSpecialPosition;
		float fMM = Pulse2MM(dwSpecialPosition);

		str.Format("特殊位置值: %.2f mm %d pulse",fMM,dwSpecialPosition);
		AfxMessageBox(str);

		str.Format("%.2f", fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_MM))->SetWindowText(str);

		str.Format("%d", dwSpecialPosition);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_SPECIAL_POSITION_PULSE))->SetWindowText(str);
	}
	else
	{
		AfxMessageBox("获取特殊位置--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfMove2SpecialPosition()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE bSpecialPosition;
	int nSpeedType;
	CString str;
	
	nSpeedType = m_cbSpeedLevel.GetCurSel();

	if (nSpeedType==-1)
	{
		AfxMessageBox("请先下载时间表");
		return;
	}
	bSpecialPosition = SpecialPositionFromPosType();
	
	if (!m_pComPort->servo_motor_move_special_position(m_bMotoWay,nSpeedType,bSpecialPosition,CCmdTimeOut::motor_move_special_position,FALSE))
	{
		AfxMessageBox("移至特殊位置--失败");
	}

	if (m_ckQueryCom.GetCheck())
	{
		BOOL bFlag = TRUE;
		while(1)
		{
			m_pComPort->m_protocolstatus.wComBoardCondition = 0;

			bFlag = !bFlag;

			if (!m_pComPort->com_get_board_condition(5000,bFlag))
			{
				AfxMessageBox("查询各板卡工作情况--出错");
				break;
			}

			WORD wCondition = m_pComPort->m_protocolstatus.wComBoardCondition;

			if ((wCondition&0x0001)==0x0001)
			{
				AfxMessageBox("强制取消打印");
				break;
			}
			else
			{
				if (m_bMotoWay==addr_servo_x)
				{
					if ((wCondition&(0x0001<<1))==0)
					{
						AfxMessageBox("X向移动停止");
						break;
					}
					else
						continue;
				}
				else
				{
					if ((wCondition&(0x0001<<2))==0)
					{
						AfxMessageBox("Y向移动停止");
						break;
					}
					else
						continue;
				}
			}
		}
	}
}

void CSFBoardDlg::OnBnClickedButtonSfDeleteTimetableData()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pComPort->servo_delete_all_timetable_data(m_bMotoWay,CCmdTimeOut::delete_all_timetable_data,FALSE))
	{
		AfxMessageBox("清空档位信息--失败");
	}

	CString str;
	m_pComPort->m_protocolstatus.bExistSpeedLevel = 0;
	m_cbSpeedLevel.ResetContent();
	m_cbSpeedLevel.EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(FALSE);

	if (m_pComPort->servo_get_existent_speedlevel(m_bMotoWay,CCmdTimeOut::get_existent_speedlevel,FALSE))
	{
		int nExistSpeedLevel = m_pComPort->m_protocolstatus.bExistSpeedLevel;
		if (nExistSpeedLevel>0)
		{
			m_cbSpeedLevel.EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(TRUE);

			for (int i=0;i<nExistSpeedLevel;i++)
			{
				str.Format("%d",i+1);
				m_cbSpeedLevel.AddString(str);
			}
			m_cbSpeedLevel.SetCurSel(0);

			if (nExistSpeedLevel>=2)
			{
				((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(TRUE);
			}
		}
	}	
	else
	{
		AfxMessageBox("获取档位总数--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfGetTimetableVitalParameter()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSpeedLevel = m_cbSpeedLevel.GetCurSel();
	if (nSpeedLevel<2)
	{
		AfxMessageBox("不是联机速度档位");
		return;
	}
	
	if (m_pComPort->servo_get_timetable_vital_parameter(m_bMotoWay,nSpeedLevel,CCmdTimeOut::get_timetable_vital_parameter,FALSE))
	{
		float fAecS,fDecS,fEvenV;
		CString str;

		fAecS = Pulse2MM(m_pComPort->m_protocolstatus.dwVitalParaAecPulse);
		fDecS = Pulse2MM(m_pComPort->m_protocolstatus.dwVitalParaDecPulse);
		fEvenV = Pulse2MM((float)TIME_CONVERT_VALUE/(float)m_pComPort->m_protocolstatus.dwVitalEvenT);

		str.Format("联机参数: 加速距离: %.2f mm 减速距离: %.2f mm 匀速速度: %.2f mm/s",fAecS,fDecS,fEvenV);
		AfxMessageBox(str);
	}
	else
	{
		AfxMessageBox("得联机速度档重要参数--失败");
	}

}

BOOL CSFBoardDlg::ExportBoardInfo()
{	
	COleDateTime ExportTime = COleDateTime::GetCurrentTime();
	CString		strIniFile,strSection,strReg,str = "";
	CString		strFileTitle = "",strFilePath = "";
	int			nBoardType	= m_cbBoardType.GetCurSel();

	if (m_bThreeAxisServo)
	{
		switch(nBoardType)
		{
		case sf_boardtype_motor_3axis_x:
			str.Format("SFBoardInfo-Moto_3Axis_X_XC");
			break;
		case sf_boardtype_motor_3axis_y_dd:
			str.Format("SFBoardInfo-Moto_3Axis_Y_DD");
			break;
		case sf_boardtype_motor_3axis_y_sg:
			str.Format("SFBoardInfo-Moto_3Axis_Y_SG");
			break;
		case sf_boardtype_motor_3axis_z_dd:
			str.Format("SFBoardInfo-Moto_3Axis_Z_DD");
			break;
		case sf_boardtype_motor_3axis_z_sg:
			str.Format("SFBoardInfo-Moto_3Axis_Z_SG");
			break;
		}
	}
	else
	{
		switch(nBoardType)
		{
		case sf_boardtype_motor_x:
			str.Format("SFBoardInfo-Moto_X_XC");
			break;
		case sf_boardtype_motor_y_dd:
			str.Format("SFBoardInfo-Moto_Y_DD");
			break;
		case sf_boardtype_motor_y_sg:
			str.Format("SFBoardInfo-Moto_Y_SG");
			break;
		}
	}

	strFileTitle = str;
	str			= ExportTime.Format("-%Y%m%d%H%M%S.SFCFG");
	strFileTitle = strFileTitle + str;

	CFileDialog dlg(FALSE,"*.SFCFG", strFileTitle, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, 
		"伺服配置文件 (*.SFCFG)|*.SFCFG|All Files (*.*)|*.*||", this);

	dlg.m_ofn.lpstrTitle = "导出伺服板信息";

	if (dlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	strFilePath = dlg.GetPathName();

	SetCursor(LoadCursor(NULL,IDC_WAIT));
	
	strIniFile = strFilePath;

	strSection = "BoardInfo";
	WRITE_PROFILE_INFO("%d",m_bMotoWay);
	
	int nSpeedNum = m_cbSpeedLevel.GetCount();

	if (nSpeedNum<=0)
	{
		DeleteFile(strFilePath);
		return FALSE;
	}
	else
	{
		strSection = "SpeedNum";
		WRITE_PROFILE_INFO("%d",nSpeedNum);
		
		for (int i=0;i<nSpeedNum;i++)
		{
			if (i<2)
			{
				//Offline
				COfflineTimeTableDlg dlg(this);
				dlg.m_nOperationType = 0;
				dlg.m_pSFDlg = this;
				dlg.m_bSpeedLevel = i;
				dlg.m_bMotoWay = m_bMotoWay;
				dlg.m_pComPort = m_pComPort;

				dlg.Create(IDD_DIALOG_SFOFFLINE_TIMETABLE,this);
				dlg.ShowWindow(FALSE);

				if (!dlg.ExportSpeedLevelInfo(strFilePath))
				{
					DeleteFile(strFilePath);
					return FALSE;
				}
			}
			else
			{
				//Online
				COnlineTimeTableDlg dlg(this);
				dlg.m_nOperationType = 0;
				dlg.m_pSFDlg = this;
				dlg.m_bSpeedLevel = i;
				dlg.m_bMotoWay = m_bMotoWay;
				dlg.m_pComPort = m_pComPort;

				dlg.Create(IDD_DIALOG_SFONLINE_TIMETABLE,this);
				dlg.ShowWindow(FALSE);

				if (!dlg.ExportSpeedLevelInfo(strFilePath))
				{
					DeleteFile(strFilePath);
					return FALSE;
				}
			}
		}
	}
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	return TRUE;
}
void CSFBoardDlg::OnBnClickedButtonBoardinfoExport()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!ExportBoardInfo())
	{
		AfxMessageBox("导出失败");
	}
	else
	{
		AfxMessageBox("导出成功");
	}
}

BOOL CSFBoardDlg::ImportBoardInfo()
{
	CFileDialog dlg(TRUE,"*.SFCFG", "", 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, 
		"伺服配置文件 (*.SFCFG)|*.SFCFG|All Files (*.*)|*.*||", this);

	dlg.m_ofn.lpstrTitle = "导入伺服板信息";

	CString strFilePath = "",str = "";
	CString strIniFile,strSection;
	char szReg[200];

	if (dlg.DoModal() != IDOK)
	{
		return FALSE;
	}
	strFilePath = dlg.GetPathName();
	
	SetCursor(LoadCursor(NULL,IDC_WAIT));

	if (!PathFileExists(strFilePath))
	{
		str.Format("文件 %s 不存在",strFilePath);
		AfxMessageBox(str);
		return FALSE;
	}

	strIniFile = strFilePath;
	strSection = "BoardInfo";

	int		nBoardType = 0;
	int		nBoardType_Now = m_cbBoardType.GetCurSel();
	BYTE	bMotorWay = motor_way_none;

	GET_PROFILE_INFO_INT(bMotorWay,0,0,3,UINT);
	if (bMotorWay != m_bMotoWay)
	{
		AfxMessageBox("请选择正确的伺服板类型(小车 导带 丝杆)");
		return FALSE;
	}
	
	if (AfxMessageBox("即将导入伺服板信息 将会覆盖现有伺服板信息 是否导入？",MB_YESNO)==IDNO)
	{
		return FALSE;
	}
	
	OnBnClickedButtonSfDeleteTimetableData();

	strSection = "SpeedNum";
	int nSpeedNum = 0;
	GET_PROFILE_INFO_INT(nSpeedNum,0,0,255,UINT);
	
	for (int i=0;i<nSpeedNum;i++)
	{
		if (i<2)
		{
			COfflineTimeTableDlg dlg(this);
			dlg.m_nOperationType = 0;
			dlg.m_pSFDlg = this;
			dlg.m_bSpeedLevel = i;
			dlg.m_bMotoWay = m_bMotoWay;
			dlg.m_pComPort = m_pComPort;

			dlg.Create(IDD_DIALOG_SFOFFLINE_TIMETABLE,this);
			dlg.ShowWindow(FALSE);

			if (!dlg.ImportSpeedLevelInfo(strFilePath))
			{
				return FALSE;
			}
		}
		else
		{
			COnlineTimeTableDlg dlg(this);
			dlg.m_nOperationType = 0;
			dlg.m_pSFDlg = this;
			dlg.m_bSpeedLevel = i;
			dlg.m_bMotoWay = m_bMotoWay;
			dlg.m_pComPort = m_pComPort;

			dlg.Create(IDD_DIALOG_SFONLINE_TIMETABLE,this);
			dlg.ShowWindow(FALSE);

			if (!dlg.ImportSpeedLevelInfo(strFilePath))
			{
				return FALSE;
			}
		}
	}

	m_pComPort->m_protocolstatus.bExistSpeedLevel = 0;
	m_cbSpeedLevel.ResetContent();
	m_cbSpeedLevel.EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(FALSE);

	if (m_pComPort->servo_get_existent_speedlevel(m_bMotoWay,CCmdTimeOut::get_existent_speedlevel,FALSE))
	{
		int nExistSpeedLevel = m_pComPort->m_protocolstatus.bExistSpeedLevel;
		if (nExistSpeedLevel>0)
		{
			m_cbSpeedLevel.EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_BUTTON_SPEEDLEVEL_MODIFY))->EnableWindow(TRUE);

			for (int i=0;i<nExistSpeedLevel;i++)
			{
				str.Format("%d",i+1);
				m_cbSpeedLevel.AddString(str);
			}
			m_cbSpeedLevel.SetCurSel(0);

			if (nExistSpeedLevel>=2)
			{
				((CWnd*)GetDlgItem(IDC_BUTTON_SF_SPEEDLEVEL_ADD))->EnableWindow(TRUE);
			}
		}
	}	
	else
	{
		AfxMessageBox("获取档位总数--失败");
		return FALSE;
	}
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	return TRUE;
}

void CSFBoardDlg::OnBnClickedButtonBoardinfoImport()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!ImportBoardInfo())
	{
		AfxMessageBox("导入失败");
	}
	else
	{
		AfxMessageBox("导入成功");
	}
}

int CSFBoardDlg::BoardType2MotorWay(int nBoardType)
{
	int nMotorWay = motor_way_none;
	if (m_bThreeAxisServo)
	{
		switch(nBoardType)
		{
		case sf_boardtype_motor_3axis_x:
			nMotorWay = motor_way_3Axis_x;
			break;
		case sf_boardtype_motor_3axis_y_dd:
		case sf_boardtype_motor_3axis_y_sg:
			nMotorWay = motor_way_3Axis_y;
			break;
		case sf_boardtype_motor_3axis_z_dd:
		case sf_boardtype_motor_3axis_z_sg:
			nMotorWay = motor_way_3Axis_z;
			break;
		}
	}
	else
	{
		switch(nBoardType)
		{
		case sf_boardtype_motor_x:
			nMotorWay = motor_way_x;
			break;
		case sf_boardtype_motor_y_dd:
		case sf_boardtype_motor_y_sg:
			nMotorWay = motor_way_y;
			break;
		}	

	}
	return nMotorWay;
}

BYTE CSFBoardDlg::SpecialPositionFromPosType()
{
	BYTE	bSpecialPosition = pos_unknown;
	int		nPosType, nBoardType;

	nBoardType = m_cbBoardType.GetCurSel();

	BOOL	bEnableX, bEnableY, bEnableZ;

	bEnableX = bEnableY = bEnableZ = FALSE;
	if (m_bThreeAxisServo)
	{
		switch(nBoardType)
		{
		case sf_boardtype_motor_3axis_x:
			bEnableX = TRUE;
			break;
		case sf_boardtype_motor_3axis_y_dd:
		case sf_boardtype_motor_3axis_y_sg:
			bEnableY = TRUE;
			break;
		case sf_boardtype_motor_3axis_z_dd:
		case sf_boardtype_motor_3axis_z_sg:
			bEnableZ = TRUE;
			break;
		}
	}
	else
	{
		switch(nBoardType)
		{
		case sf_boardtype_motor_x:
			bEnableX = TRUE;
			break;
		case sf_boardtype_motor_y_dd:
		case sf_boardtype_motor_y_sg:
			bEnableY = TRUE;
			break;
		}
	}
	if (bEnableX)
	{
		nPosType = m_cbSpecialPositionX.GetCurSel();
		switch (nPosType)
		{
		case 0:
			bSpecialPosition = pos_x_device_current;
			break;
		case 1:
			bSpecialPosition = pos_x_device_min;
			break;
		case 2:
			bSpecialPosition = pos_x_device_max;
			break;
		case 3:
			bSpecialPosition = pos_x_device_lastpos;
			break;
		case 4:
			bSpecialPosition = pos_x_device_ready;
			break;
		case 5:
			bSpecialPosition = pos_x_device_check;
			break;
		case 6:
			bSpecialPosition = pos_x_device_clean;
			break;
		case 7:
			bSpecialPosition = pos_x_device_moisture;
			break;
		}
	}

	if (bEnableY)
	{
		nPosType = m_cbSpecialPositionY.GetCurSel();
		switch (nPosType)
		{
		case 0:
			bSpecialPosition = pos_y_device_current;
			break;
		case 1:
			bSpecialPosition = pos_y_device_min;
			break;
		case 2:
			bSpecialPosition = pos_y_device_max;
			break;
		case 3:
			bSpecialPosition = pos_y_device_ready;
			break;
		case 4:
			bSpecialPosition = pos_y_device_lastpos;
			break;
		}
	}

	if (bEnableZ)
	{
		nPosType = m_cbSpecialPositionZ.GetCurSel();
		switch (nPosType)
		{
		case 0:
			bSpecialPosition = pos_y_device_current;
			break;
		case 1:
			bSpecialPosition = pos_y_device_min;
			break;
		case 2:
			bSpecialPosition = pos_y_device_max;
			break;
		case 3:
			bSpecialPosition = pos_y_device_ready;
			break;
		case 4:
			bSpecialPosition = pos_y_device_lastpos;
			break;
		}
	}
	return bSpecialPosition;
}

BOOL CSFBoardDlg::xy_SpecialPositionFromPosType(BYTE &bPosX, BYTE &bPosY)
{
	int		nPosType;

	bPosX = bPosY = pos_unknown;

	nPosType = m_xy_cbSpecialPositionX.GetCurSel();
	switch (nPosType)
	{
	case 0:
		bPosX = pos_x_device_current;
		break;
	case 1:
		bPosX = pos_x_device_min;
		break;
	case 2:
		bPosX = pos_x_device_max;
		break;
	case 3:
		bPosX = pos_x_device_lastpos;
		break;
	case 4:
		bPosX = pos_x_device_ready;
		break;
	case 5:
		bPosX = pos_x_device_check;
		break;
	case 6:
		bPosX = pos_x_device_clean;
		break;
	case 7:
		bPosX = pos_x_device_moisture;
		break;
	}


	nPosType = m_cbSpecialPositionY.GetCurSel();
	switch (nPosType)
	{
	case 0:
		bPosY = pos_y_device_current;
		break;
	case 1:
		bPosY = pos_y_device_min;
		break;
	case 2:
		bPosY = pos_y_device_max;
		break;
	case 3:
		bPosY = pos_y_device_ready;
		break;
	case 4:
		bPosY = pos_y_device_lastpos;
		break;
	}

	return TRUE;
}
//	bMotoWayy :[in]SFBOARD_BOARDTYPE_3AXIS
int CSFBoardDlg::xy_MM2Pulse(float fMM, BYTE bMotoWay)
{
	int		nPulse = 0;

	switch(bMotoWay)
	{
	case sf_boardtype_motor_3axis_x:
		nPulse = fMM * (*m_pfReducer_x)/(*m_pfDollyPerimeter)*(*m_pfPulsesPerRevolution_x);
		break;
	case sf_boardtype_motor_3axis_y_dd:
		nPulse = fMM * (*m_pfReducer_y)/(*m_pfDeferentPerimeter)*(*m_pfPulsesPerRevolution_y);
		break;
	case sf_boardtype_motor_3axis_y_sg:
		nPulse = fMM * (*m_pfReducer_y)/(*m_pfDeferentPitch)*(*m_pfPulsesPerRevolution_y);
		break;
	default:
		nPulse = 0;
		break;
	}
	return nPulse;
}

float CSFBoardDlg::xy_Pulse2MM(int nPulse, BYTE bMotoWay)
{
	float fMM = 0.0;
	
	switch(bMotoWay)
	{
	case sf_boardtype_motor_3axis_x:
		fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_x)*(*m_pfDollyPerimeter)/(*m_pfReducer_x);
		break;
	case sf_boardtype_motor_3axis_y_dd:
		fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_y)*(*m_pfDeferentPerimeter)/(*m_pfReducer_y);
		break;
	case sf_boardtype_motor_3axis_y_sg:
		fMM = ((float)nPulse)/(*m_pfPulsesPerRevolution_y)*(*m_pfDeferentPitch)/(*m_pfReducer_y);
		break;
	default:
		fMM = 0.0;
		break;
	}
	
	return fMM;
}
void CSFBoardDlg::OnBnClickedButtonSfXyMove2SpecialPosition()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE	bPosX, bPosY;
	BYTE	bSpeedTypeX, bSpeedTypeY;
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_X))->GetWindowText(str);
	bSpeedTypeX = (BYTE)atoi(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_Y))->GetWindowText(str);
	bSpeedTypeY = (BYTE)atoi(str);

	xy_SpecialPositionFromPosType(bPosX, bPosY);	
	if (!m_pComPort->servo_motor_xy_move_special_position(bSpeedTypeX, bSpeedTypeY, bPosX, bPosY,CCmdTimeOut::motor_xy_move_special_position,FALSE))
	{
		AfxMessageBox("移动到指定特殊位置--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfXyMoverelative()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD	pulseX, pulseY, re_pulseX;
	BYTE	bSpeedTypeX, bSpeedTypeY;
	int		nX, nY;
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_X))->GetWindowText(str);
	bSpeedTypeX = (BYTE)atoi(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_Y))->GetWindowText(str);
	bSpeedTypeY = (BYTE)atoi(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_PULSE))->GetWindowText(str);
	nX = atoi(str);
	if (nX < 0)
	{
		pulseX = -nX;
		pulseX |= (0x01<<31);
	}
	else
	{
		pulseX = nX;
	}
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE))->GetWindowText(str);
	re_pulseX = labs(atoi(str));

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_PULSE))->GetWindowText(str);
	nY = atoi(str);
	if (nY < 0)
	{
		pulseY = -nY;
		pulseY |= (0x01<<31);
	}
	else
	{
		pulseY = nY;
	}

	if (!m_pComPort->servo_motor_xy_move_relative(bSpeedTypeX, bSpeedTypeY, pulseX, pulseY,CCmdTimeOut::motor_xy_move_relative,FALSE))
	{
		AfxMessageBox("移动相对距离--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfXyMoveabsolute()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD	pulseX, pulseY, re_pulseX;
	BYTE	bSpeedTypeX, bSpeedTypeY;
	int		nX, nY;
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_X))->GetWindowText(str);
	bSpeedTypeX = (BYTE)atoi(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_Y))->GetWindowText(str);
	bSpeedTypeY = (BYTE)atoi(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_PULSE))->GetWindowText(str);
	nX = atoi(str);
	if (nX < 0)
	{
		AfxMessageBox("无效的定位参数 :X < 0");
		return;
	}
	pulseX = nX;

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE))->GetWindowText(str);
	re_pulseX = labs(atoi(str));

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_PULSE))->GetWindowText(str);
	nY = atoi(str);
	if (nY < 0)
	{
		AfxMessageBox("无效的定位参数 :Y < 0");
		return;
	}

	pulseY = nY;

	if (!m_pComPort->servo_motor_xy_move_absolute(bSpeedTypeX, bSpeedTypeY, pulseX, pulseY,CCmdTimeOut::motor_xy_move_absolute,FALSE))
	{
		AfxMessageBox("移动到绝对位置--失败");
	}
}

void CSFBoardDlg::OnBnClickedButtonSfXyPrintline()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD	pulseX, pulseY, re_pulseX;
	BYTE	bSpeedTypeX, bSpeedTypeY;
	int		nX, nY;
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_X))->GetWindowText(str);
	bSpeedTypeX = (BYTE)atoi(str);
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_SPEED_Y))->GetWindowText(str);
	bSpeedTypeY = (BYTE)atoi(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_PULSE))->GetWindowText(str);
	nX = atoi(str);
	if (nX < 0)
	{
		pulseX = -nX;
		pulseX |= (0x01<<31);
	}
	else
	{
		pulseX = nX;
	}
	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE))->GetWindowText(str);
	re_pulseX = labs(atoi(str));

	((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_PULSE))->GetWindowText(str);
	nY = atoi(str);
	if (nY < 0)
	{
		pulseY = -nY;
		pulseY |= (0x01<<31);
	}
	else
	{
		pulseY = nY;
	}

	if (!m_pComPort->servo_motor_xy_print_line(bSpeedTypeX, bSpeedTypeY, pulseX, re_pulseX, pulseY,CCmdTimeOut::motor_xy_print_line,FALSE))
	{
		AfxMessageBox("打印一行--失败");
	}
}

void CSFBoardDlg::OnEnChangeEditSfXyMoveXMm()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_MM))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_MM))->GetWindowText(str);
		fMM		= atof(str);
		nPulse	= xy_MM2Pulse(fMM, sf_boardtype_motor_3axis_x);
		str.Format("%d",nPulse);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_PULSE))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnEnChangeEditSfXyMoveXPulse()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_PULSE))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_PULSE))->GetWindowText(str);
		nPulse	= atoi(str);
		fMM		= xy_Pulse2MM(nPulse, sf_boardtype_motor_3axis_x);
		str.Format("%.2f",fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_X_MM))->SetWindowText(str);
	}

}

void CSFBoardDlg::OnEnChangeEditSfXyMovYMm()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_MM))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;
		BYTE bMotoWay = m_xy_ckMotorY_SG.GetCheck() ? sf_boardtype_motor_3axis_y_sg : sf_boardtype_motor_3axis_y_dd;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_MM))->GetWindowText(str);
		fMM		= atof(str);
		nPulse	= xy_MM2Pulse(fMM, bMotoWay);
		str.Format("%d",nPulse);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_PULSE))->SetWindowText(str);
	}

}

void CSFBoardDlg::OnEnChangeEditSfXyMoveYPulse()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_PULSE))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;
		BYTE bMotoWay = m_xy_ckMotorY_SG.GetCheck() ? sf_boardtype_motor_3axis_y_sg : sf_boardtype_motor_3axis_y_dd;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_PULSE))->GetWindowText(str);
		nPulse	= atoi(str);
		fMM		= xy_Pulse2MM(nPulse, bMotoWay);
		str.Format("%.2f",fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_Y_MM))->SetWindowText(str);
	}

}

void CSFBoardDlg::OnEnChangeEditSfXyMoveReXMm()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_MM))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_MM))->GetWindowText(str);
		fMM		= atof(str);
		nPulse	= xy_MM2Pulse(fMM, sf_boardtype_motor_3axis_x);
		str.Format("%d",nPulse);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE))->SetWindowText(str);
	}
}

void CSFBoardDlg::OnEnChangeEditSfXyMoveReXPulse()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	if (((CEdit*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE))->GetModify())
	{
		int nPulse;
		float fMM;
		CString str;

		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_PULSE))->GetWindowText(str);
		nPulse	= atoi(str);
		fMM		= xy_Pulse2MM(nPulse, sf_boardtype_motor_3axis_x);
		str.Format("%.2f",fMM);
		((CWnd*)GetDlgItem(IDC_EDIT_SF_XY_MOVE_RE_X_MM))->SetWindowText(str);
	}

}

void CSFBoardDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	OnOK();

	DestroyWindow();
}

void CSFBoardDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	//	CDialog::OnCancel();
	DestroyWindow();

}
