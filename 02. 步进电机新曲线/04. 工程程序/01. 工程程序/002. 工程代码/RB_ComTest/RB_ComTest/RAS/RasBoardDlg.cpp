// RasBoardDlg.cpp : ʵ���ļ�
//

#include "../stdafx.h"
#include "../RB_ComTest.h"
#include "RasBoardDlg.h"
#include "../MBComPort/Protocol.h"

UINT ThreadProcPrint1(LPVOID pParam)
{
	CRasBoardDlg* pPrintDlg = (CRasBoardDlg*)pParam;

	float fPrintHeight = 0;
	int nThousandth = 0;
	CString  str = "";
	BOOL bFlag = TRUE;
	int nPass = 0;

	if (!pPrintDlg->GetPrintParameter())
	{
		return 0;
	}

	pPrintDlg->m_pgsPrint.ShowWindow(TRUE);
	pPrintDlg->m_staticPrint.ShowWindow(TRUE);
	pPrintDlg->m_pgsPrint.SetRange(0, 1000);
	pPrintDlg->m_staticPrint.SetWindowText("0%");
	pPrintDlg->m_btnPrint.EnableWindow(FALSE);

	pPrintDlg->m_isStopPrint = FALSE;
	pPrintDlg->m_isPausePrint = FALSE;

	if (!pPrintDlg->MBPrintStart())
	{
		goto End;
	}

	while (fPrintHeight <= pPrintDlg->m_fHeight)
	{
		nThousandth = fPrintHeight / pPrintDlg->m_fHeight * 1000;
		pPrintDlg->m_pgsPrint.SetPos(nThousandth);
		str.Format("%d%%", nThousandth / 10);
		pPrintDlg->m_staticPrint.SetWindowText(str);

		if (pPrintDlg->m_isStopPrint == TRUE)
		{
			if (!pPrintDlg->m_pComPort->ras_print_stop(5000, FALSE))
			{
				AfxMessageBox("��ӡ��ֹʧ��");
			}
			goto End;
		}

		if (pPrintDlg->m_isPausePrint == TRUE)
		{
			do
			{
				if (pPrintDlg->m_isPausePrint == FALSE)
				{
					break;
				}
			} while (1);
		}

		bFlag = !bFlag;

		if (!pPrintDlg->MBPrintLine(nThousandth, fPrintHeight, bFlag))
		{
			goto End;
		}
		fPrintHeight += abs(pPrintDlg->m_fYDist);
		nPass++;

		//if (nPass%pPrintDlg->m_nPurgeFreq==0)
		//{
		//	if (!pPrintDlg->m_pComPort->ras_move2_position(pPrintDlg->m_positionPara.pos_x_clean,0xFFFFFFFF,20000,FALSE))
		//	{
		//		AfxMessageBox("������ϴλʧ��");
		//		continue;
		//	}
		//}
	}

End:
	pPrintDlg->MBPrintEnd();

	pPrintDlg->m_pgsPrint.ShowWindow(FALSE);
	pPrintDlg->m_pgsPrint.SetPos(0);
	pPrintDlg->m_staticPrint.ShowWindow(FALSE);
	pPrintDlg->m_btnPrint.EnableWindow(TRUE);

	return 0;
}
// CRasBoardDlg �Ի���

IMPLEMENT_DYNAMIC(CRasBoardDlg, CDialog)

CRasBoardDlg::CRasBoardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRasBoardDlg::IDD, pParent)
{
	m_fIOSpace = 50;	//IO���
	m_fIOBaseGridNum = 25000;	//IO��̶���

	m_pParentThread = NULL;
}

CRasBoardDlg::~CRasBoardDlg()
{
}

void CRasBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RAS_YDIR, m_cbYDir);
	DDX_Control(pDX, IDC_COMBO_RAS_PRINT_WAY, m_cbPrintWay);
	DDX_Control(pDX, IDC_COMBO_RAS_PRINT_ACTION, m_cbAction);
	DDX_Control(pDX, IDC_CHECK_RAS_PRINT_USE_GRID, m_ckUseGrid);
	DDX_Control(pDX, IDC_COMBO_RAS_PRINT_END_POSITION_X, m_cbEndPositionX);
	DDX_Control(pDX, IDC_COMBO_RAS_PRINT_END_POSITION_Y, m_cbEndPositionY);
	DDX_Control(pDX, IDC_PROGRESS_RAS_PRINT, m_pgsPrint);
	DDX_Control(pDX, IDC_STATIC_RAS_PRINT, m_staticPrint);
	DDX_Control(pDX, IDC_BUTTON_RAS_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_LIST_RAS_SPEED_LEVEL_Y, m_lstSpeedLevel_Y);
	DDX_Control(pDX, IDC_LIST_RAS_POSITION_PARA, m_lstPositionPara);
	DDX_Control(pDX, IDC_LIST_RAS_SPEED_LEVEL_X, m_lstSpeedLevel_X);
	DDX_Control(pDX, IDC_COMBO_RAS_SPEED_LEVEL_X, m_cbSpeedLevelX);
	DDX_Control(pDX, IDC_COMBO_RAS_SPEED_LEVEL_Y, m_cbSpeedLevelY);
}


BEGIN_MESSAGE_MAP(CRasBoardDlg, CDialog)
	ON_MESSAGE(WM_RASSTATE, &CRasBoardDlg::OnRasStateConfirm)
	ON_BN_CLICKED(IDC_BUTTON_RAS_PRINT_START, &CRasBoardDlg::OnBnClickedButtonRasPrintStart)
	ON_BN_CLICKED(IDC_BUTTON_RAS_PRINT_LINE, &CRasBoardDlg::OnBnClickedButtonRasPrintLine)
	ON_BN_CLICKED(IDC_BUTTON_RAS_PRINT_END, &CRasBoardDlg::OnBnClickedButtonRasPrintEnd)
	ON_BN_CLICKED(IDC_BUTTON_RAS_PRINT_PAUSE, &CRasBoardDlg::OnBnClickedButtonRasPrintPause)
	ON_BN_CLICKED(IDC_BUTTON_RAS_PRINT, &CRasBoardDlg::OnBnClickedButtonRasPrint)
	ON_BN_CLICKED(IDC_BUTTON_RAS_PRINT_RESUME, &CRasBoardDlg::OnBnClickedButtonRasPrintResume)
	ON_BN_CLICKED(IDC_BUTTON_RAS_PRINT_STOP, &CRasBoardDlg::OnBnClickedButtonRasPrintStop)
	ON_BN_CLICKED(IDC_BUTTON_RAS_SELECT, &CRasBoardDlg::OnBnClickedButtonRasSelect)
	ON_BN_CLICKED(IDC_BUTTON_RAS_GET_POSITION_PARA, &CRasBoardDlg::OnBnClickedButtonRasGetPositionPara)
	ON_EN_CHANGE(IDC_EDIT_RAS_PRINT_FREQ_PSC, &CRasBoardDlg::OnEnChangeEditRasPrintFreqPsc)
	ON_BN_CLICKED(IDC_BUTTON_RAS_GET_SPEED_LEVEL, &CRasBoardDlg::OnBnClickedButtonRasGetSpeedLevel)
	ON_BN_CLICKED(IDC_BUTTON_RAS_MOVE2_POSITION, &CRasBoardDlg::OnBnClickedButtonRasMove2Position)
END_MESSAGE_MAP()


// CRasBoardDlg ��Ϣ�������

BOOL CRasBoardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_cbYDir.ResetContent();
	m_cbYDir.AddString("����");
	m_cbYDir.AddString("˿��");
	m_cbYDir.SetCurSel(0);

	m_cbPrintWay.ResetContent();
	m_cbPrintWay.AddString("˫��");
	m_cbPrintWay.AddString("����");
	m_cbPrintWay.SetCurSel(0);

	m_cbAction.ResetContent();
	m_cbAction.AddString("�޶���");
	m_cbAction.AddString("ɢ��");
	m_cbAction.AddString("��ϴ");
	m_cbAction.SetCurSel(0);

	m_ckUseGrid.SetCheck(TRUE);

	m_cbEndPositionX.ResetContent();
	m_cbEndPositionX.AddString("��ǰλ��");
	m_cbEndPositionX.AddString("��Сλ��");
	m_cbEndPositionX.AddString("���λ��");
	m_cbEndPositionX.AddString("�ϴ�λ��");
	m_cbEndPositionX.AddString("����λ");
	m_cbEndPositionX.AddString("���λ");
	m_cbEndPositionX.AddString("��ϴλ");
	m_cbEndPositionX.AddString("��ʪλ");
	m_cbEndPositionX.SetCurSel(0);

	m_cbEndPositionY.ResetContent();
	m_cbEndPositionY.AddString("��ǰλ��");
	m_cbEndPositionY.AddString("��Сλ��");
	m_cbEndPositionY.AddString("���λ��");
	m_cbEndPositionY.AddString("����λ");
	m_cbEndPositionY.AddString("�ϴ�λ��");
	m_cbEndPositionY.SetCurSel(0);

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_MOVE2POSITION_X))->SetWindowText("500.00");
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_MOVE2POSITION_Y))->SetWindowText("500.00");

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_POSITION_X))->SetWindowText("500.00");
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_POSITION_Y))->SetWindowText("100.00");
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_WIDTH))->SetWindowText("1000.00");
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_HEIGHT))->SetWindowText("10000.00");

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_FREQ_PSC))->SetWindowText("20");

	int dpi = m_fIOBaseGridNum / 20 / m_fIOSpace * 25.4;
	CString str;
	str.Format("%d", dpi);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_RES_X))->SetWindowText(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_RES_Y))->SetWindowText(str);

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_Y_DIST))->SetWindowText("50.00");
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PURGE_TIME))->SetWindowText("10000");
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PURGE_FREQ))->SetWindowText("10");

	m_pgsPrint.SetPos(0);
	m_staticPrint.ShowWindow(FALSE);
	m_pComPort->m_pRasDlg = this;

	DWORD dwStyle = m_lstSpeedLevel_X.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstSpeedLevel_X.SetExtendedStyle(dwStyle);
	m_lstSpeedLevel_X.ModifyStyle(LVS_ICON | LVS_LIST, LVS_REPORT | LVS_SHOWSELALWAYS);

	m_lstSpeedLevel_X.InsertColumn(0, _T("X"), LVCFMT_CENTER, 40);
	m_lstSpeedLevel_X.InsertColumn(1, _T("���پ���"), LVCFMT_CENTER, 80);
	m_lstSpeedLevel_X.InsertColumn(2, _T("���پ���"), LVCFMT_CENTER, 80);
	m_lstSpeedLevel_X.InsertColumn(3, _T("�ٶ�"), LVCFMT_CENTER, 80);

	m_lstSpeedLevel_Y.SetExtendedStyle(dwStyle);
	m_lstSpeedLevel_Y.ModifyStyle(LVS_ICON | LVS_LIST, LVS_REPORT | LVS_SHOWSELALWAYS);

	m_lstSpeedLevel_Y.InsertColumn(0, _T("Y"), LVCFMT_CENTER, 40);
	m_lstSpeedLevel_Y.InsertColumn(1, _T("���پ���"), LVCFMT_CENTER, 80);
	m_lstSpeedLevel_Y.InsertColumn(2, _T("���پ���"), LVCFMT_CENTER, 80);
	m_lstSpeedLevel_Y.InsertColumn(3, _T("�ٶ�"), LVCFMT_CENTER, 80);

	m_lstPositionPara.SetExtendedStyle(dwStyle);
	m_lstPositionPara.ModifyStyle(LVS_ICON | LVS_LIST, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_lstPositionPara.InsertColumn(0, _T("λ��"), LVCFMT_CENTER, 80);
	m_lstPositionPara.InsertColumn(1, _T("����"), LVCFMT_CENTER, 80);
	m_lstPositionPara.InsertColumn(2, _T("����"), LVCFMT_CENTER, 80);

	m_cbSpeedLevelX.EnableWindow(FALSE);
	m_cbSpeedLevelY.EnableWindow(FALSE);
	m_pgsPrint.ShowWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_RES_X))->EnableWindow(FALSE);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_RES_Y))->EnableWindow(FALSE);

	m_nProtocolMode = debug_mode;
	m_pComPort->m_nProtocolMode = m_nProtocolMode;

	if (!m_pComPort->ras_select(0x01, 5000, FALSE))
	{
		AfxMessageBox("�л�������ģʽʧ��");
	}
	OnBnClickedButtonRasGetPositionPara();
	OnBnClickedButtonRasGetSpeedLevel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CRasBoardDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_pParentThread != NULL)
	{
		m_pParentThread->PostThreadMessage(WM_QUIT, 0, 0);
	}
	CDialog::PostNcDestroy();
	delete this;
}

int CRasBoardDlg::MM2GridNum(float fMM)
{
	int nGridNum = fMM * m_fIOBaseGridNum / m_fIOSpace;
	return nGridNum;
}

int CRasBoardDlg::MM2Pulse_X(float fMM)
{
	int nPulse = fMM * (*m_pfReducer_x) / (*m_pfDollyPerimeter)*(*m_pfPulsesPerRevolution_x);
	return nPulse;
}

int CRasBoardDlg::MM2Pulse_Y(float fMM)
{
	int nPulse, nSel = m_cbYDir.GetCurSel();

	switch (nSel)
	{
	case 0:
		nPulse = fMM * (*m_pfReducer_y) / (*m_pfDeferentPerimeter)*(*m_pfPulsesPerRevolution_y);
		break;
	case 1:
		nPulse = fMM * (*m_pfReducer_y) / (*m_pfDeferentPitch)*(*m_pfPulsesPerRevolution_y);
		break;
	}

	return nPulse;
}

float CRasBoardDlg::Pulse2MM_X(int nPulse)
{
	float fMM = ((float)nPulse) / (*m_pfPulsesPerRevolution_x)*(*m_pfDollyPerimeter) / (*m_pfReducer_x);
	return fMM;
}

float CRasBoardDlg::Pulse2MM_Y(int nPulse)
{
	float fMM;
	int nSel = m_cbYDir.GetCurSel();

	switch (nSel)
	{
	case 0:
		fMM = ((float)nPulse) / (*m_pfPulsesPerRevolution_y)*(*m_pfDeferentPerimeter) / (*m_pfReducer_y);
		break;
	case 1:
		fMM = ((float)nPulse) / (*m_pfPulsesPerRevolution_y)*(*m_pfDeferentPitch) / (*m_pfReducer_y);
		break;
	}

	return fMM;
}

BOOL CRasBoardDlg::GetPrintParameter()
{
	int nSel;
	CString str;

	m_nSpeedLevelX = m_cbSpeedLevelX.GetCurSel();
	m_nSpeedLevelY = m_cbSpeedLevelY.GetCurSel();

	if (m_nSpeedLevelX < 0 || m_nSpeedLevelY < 0)
	{
		AfxMessageBox("���Ȼ�ȡ�ٶȵ�λ");
		return FALSE;
	}

	if (m_positionPara.pos_x_min == 0xFFFFFFFF
		|| m_positionPara.pos_x_max == 0xFFFFFFFF
		|| m_positionPara.pos_x_clean == 0xFFFFFFFF)
	{
		AfxMessageBox("X������С��������ϴλ��Ϊ��Чֵ,��������");
		return FALSE;
	}
	nSel = m_cbYDir.GetCurSel();
	if (nSel == 0)
		m_nYDir = 0;
	else
		m_nYDir = 1;

	nSel = m_cbPrintWay.GetCurSel();
	if (nSel == 0)
		m_nPrintWay = 0;
	else
		m_nPrintWay = 1;

	nSel = m_cbAction.GetCurSel();
	if (nSel == 0)
		m_nAction = 0;
	else if (nSel == 1)
		m_nAction = 1;
	else
		m_nAction = 2;

	if (m_ckUseGrid.GetCheck())
		m_isUserGrid = TRUE;
	else
		m_isUserGrid = FALSE;

	BYTE bEndPositionX, bEndPositionY;

	nSel = m_cbEndPositionX.GetCurSel();
	switch (nSel)
	{
	case 0:bEndPositionX = 0x01; break;
	case 1:bEndPositionX = 0x02; break;
	case 2:bEndPositionX = 0x03; break;
	case 3:bEndPositionX = 0x04; break;
	case 4:bEndPositionX = 0x05; break;
	case 5:bEndPositionX = 0x06; break;
	case 6:bEndPositionX = 0x07; break;
	case 7:bEndPositionX = 0x08; break;
	}
	nSel = m_cbEndPositionY.GetCurSel();
	switch (nSel)
	{
	case 0:bEndPositionY = 0x10; break;
	case 1:bEndPositionY = 0x20; break;
	case 2:bEndPositionY = 0x30; break;
	case 3:bEndPositionY = 0x40; break;
	case 4:bEndPositionY = 0x50; break;
	}
	m_bEndPosition = bEndPositionX | bEndPositionY;

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_POSITION_X))->GetWindowText(str);
	m_fPositionX = atof(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_POSITION_Y))->GetWindowText(str);
	m_fPositionY = atof(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PURGE_TIME))->GetWindowText(str);
	m_nPurgeTime = atoi(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_WIDTH))->GetWindowText(str);
	m_fWidth = atof(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_HEIGHT))->GetWindowText(str);
	m_fHeight = atof(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_RES_X))->GetWindowText(str);
	m_fResX = atof(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_RES_Y))->GetWindowText(str);
	m_fResX = atof(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_FREQ_PSC))->GetWindowText(str);
	m_nFreqPsc = atoi(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_Y_DIST))->GetWindowText(str);
	m_fYDist = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PURGE_FREQ))->GetWindowText(str);
	m_nPurgeFreq = atoi(str);

	return TRUE;
}

BOOL CRasBoardDlg::MBPrintStart()
{
	RAS_PRINT_START_PARAMETER print_start_parameter;

	print_start_parameter.m_bSpeedLevelX = m_nSpeedLevelX;
	print_start_parameter.m_bSpeedLevelY = m_nSpeedLevelY;
	print_start_parameter.m_dwPositionX = MM2Pulse_X(m_fPositionX);
	print_start_parameter.m_dwPositionY = MM2Pulse_Y(m_fPositionY);
	print_start_parameter.m_dwWidth = m_fWidth;
	print_start_parameter.m_dwHeight = m_fHeight;
	print_start_parameter.m_wResX = m_fResX;
	print_start_parameter.m_wResY = m_fResY;
	print_start_parameter.m_bPrintWay = m_nPrintWay;

	if (!m_pComPort->ras_print_start(print_start_parameter, 60000, FALSE))
	{
		AfxMessageBox("��ӡ׼��--ʧ��");
		return FALSE;
	}
	return TRUE;
}

BOOL CRasBoardDlg::MBPrintLine(int nThousandth, int nPrintHeight, BOOL bFlag)
{
	RAS_PRINT_LINE_PARAMETER print_line_parameter;
	DWORD dwPulse = 0;

	if (m_isUserGrid == TRUE)
	{
		print_line_parameter.m_dwStartPos = MM2GridNum(2 * m_fIOSpace);	//2��IO
		print_line_parameter.m_dwWidth = MM2GridNum(m_fWidth);
	}
	else
	{
		print_line_parameter.m_dwStartPos = 0;
		print_line_parameter.m_dwWidth = 0;
	}

	if (m_nPrintWay == 0)	//˫��
	{
		print_line_parameter.m_dwPulse = MM2Pulse_X(4 * m_fIOSpace + m_fWidth);	//С�����߳���
		if (print_line_parameter.m_dwPulse % 50 != 0)	//50����
		{
			print_line_parameter.m_dwPulse = (print_line_parameter.m_dwPulse / 50 + 1) * 50;
		}

		if (bFlag == TRUE)
		{
			print_line_parameter.m_dwPulse |= (0x00000001 << 31);
		}
		print_line_parameter.m_dwRePulse = 0;
	}
	else	//����
	{
		print_line_parameter.m_dwPulse = MM2Pulse_X(4 * m_fIOSpace + m_fWidth);	//С�����߳���
		if (print_line_parameter.m_dwPulse % 50 != 0)	//50����
		{
			print_line_parameter.m_dwPulse = (print_line_parameter.m_dwPulse / 50 + 1) * 50;
		}
		print_line_parameter.m_dwRePulse = print_line_parameter.m_dwPulse;
	}

	if (m_fYDist >= 0)
	{
		print_line_parameter.m_dwYDist = MM2Pulse_Y(m_fYDist);
	}
	else
	{
		print_line_parameter.m_dwYDist = (MM2Pulse_Y(0 - m_fYDist) | (0x01 << 31));
	}

	print_line_parameter.m_wFreq = m_nFreqPsc;
	print_line_parameter.m_bAction = m_nAction;
	print_line_parameter.m_wThousandth = nThousandth;
	print_line_parameter.m_dwPrintHeight = nPrintHeight;

	if (!m_pComPort->ras_print_line(print_line_parameter, 60000, bFlag))
	{
		AfxMessageBox("��ӡһ��--����");
		return FALSE;
	}
	return TRUE;
}
BOOL CRasBoardDlg::MBPrintEnd()
{
	if (!m_pComPort->ras_print_end(m_bEndPosition, 60000, FALSE))
	{
		AfxMessageBox("��ӡ����--ʧ��");
		return FALSE;
	}
	return TRUE;
}
void CRasBoardDlg::OnBnClickedButtonRasPrintStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!GetPrintParameter())
	{
		return;
	}
	MBPrintStart();
}

void CRasBoardDlg::OnBnClickedButtonRasPrintLine()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!GetPrintParameter())
	{
		return;
	}
	MBPrintLine(0, 0, FALSE);
}

void CRasBoardDlg::OnBnClickedButtonRasPrintEnd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!GetPrintParameter())
	{
		return;
	}
	MBPrintEnd();
}

void CRasBoardDlg::OnBnClickedButtonRasPrint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pThreadPrint = AfxBeginThread(ThreadProcPrint1, this, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0);
}

void CRasBoardDlg::OnBnClickedButtonRasPrintPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isPausePrint = TRUE;
}

void CRasBoardDlg::OnBnClickedButtonRasPrintResume()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isPausePrint = FALSE;
}

void CRasBoardDlg::OnBnClickedButtonRasPrintStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isStopPrint = TRUE;
}

void CRasBoardDlg::OnBnClickedButtonRasSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nProtocolMode == debug_mode)
	{
		m_nProtocolMode = print_mode;
		m_pComPort->m_nProtocolMode = m_nProtocolMode;
		if (m_pComPort->ras_select(0x00, 5000, FALSE))
		{
			((CWnd*)GetDlgItem(IDC_BUTTON_RAS_SELECT))->SetWindowText("�л�������ģʽ");
		}
		else
		{
			m_nProtocolMode = debug_mode;
			m_pComPort->m_nProtocolMode = m_nProtocolMode;
			AfxMessageBox("�л�����ӡģʽʧ��");
		}
	}
	else
	{
		m_nProtocolMode = debug_mode;
		m_pComPort->m_nProtocolMode = m_nProtocolMode;
		if (m_pComPort->ras_select(0x01, 5000, FALSE))
		{
			((CWnd*)GetDlgItem(IDC_BUTTON_RAS_SELECT))->SetWindowText("�л�����ӡģʽ");
		}
		else
		{
			m_nProtocolMode = print_mode;
			m_pComPort->m_nProtocolMode = m_nProtocolMode;
			AfxMessageBox("�л�������ģʽʧ��");
		}
	}
}

LONG CRasBoardDlg::OnRasStateConfirm(WPARAM wPALAM, LPARAM lPALAM)
{
	BYTE bRasState = (BYTE)wPALAM;

	switch (bRasState)
	{
	case cmd_ras_print_stop:
	case cmd_ras_sys_error:
		m_isStopPrint = TRUE;
		break;
	case cmd_ras_print_pause:
	case cmd_ras_sys_exception:
		m_isPausePrint = TRUE;
		break;
	case cmd_ras_print_resume:
		m_isPausePrint = FALSE;
		break;
	}
	m_pComPort->ras_confirmation(bRasState);
	return 0;
}
void CRasBoardDlg::OnBnClickedButtonRasGetPositionPara()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pComPort->ras_get_position(5000, FALSE))
	{
		DWORD dwPara;
		float fPara;
		CString str;

		m_lstPositionPara.DeleteAllItems();

		dwPara = m_pComPort->m_protocolstatus.positionPara.pos_x_min;
		m_lstPositionPara.InsertItem(0, "");
		m_lstPositionPara.SetItemText(0, 0, "pos_x_min");
		if (dwPara == 0xFFFFFFFF)
		{
			m_lstPositionPara.SetItemText(0, 1, "��Чֵ");
			m_lstPositionPara.SetItemText(0, 2, "��Чֵ");
		}
		else
		{
			str.Format("%d", dwPara);
			m_lstPositionPara.SetItemText(0, 1, str);
			fPara = Pulse2MM_X(dwPara);
			str.Format("%.2f", fPara);
			m_lstPositionPara.SetItemText(0, 2, str);
		}

		dwPara = m_pComPort->m_protocolstatus.positionPara.pos_x_max;
		m_lstPositionPara.InsertItem(1, "");
		m_lstPositionPara.SetItemText(1, 0, "pos_x_max");
		if (dwPara == 0xFFFFFFFF)
		{
			m_lstPositionPara.SetItemText(1, 1, "��Чֵ");
			m_lstPositionPara.SetItemText(1, 2, "��Чֵ");
		}
		else
		{
			str.Format("%d", dwPara);
			m_lstPositionPara.SetItemText(1, 1, str);
			fPara = Pulse2MM_X(dwPara);
			str.Format("%.2f", fPara);
			m_lstPositionPara.SetItemText(1, 2, str);
		}

		dwPara = m_pComPort->m_protocolstatus.positionPara.pos_x_clean;
		m_lstPositionPara.InsertItem(2, "");
		m_lstPositionPara.SetItemText(2, 0, "pos_x_clean");
		if (dwPara == 0xFFFFFFFF)
		{
			m_lstPositionPara.SetItemText(2, 1, "��Чֵ");
			m_lstPositionPara.SetItemText(2, 2, "��Чֵ");
		}
		else
		{
			str.Format("%d", dwPara);
			m_lstPositionPara.SetItemText(2, 1, str);
			fPara = Pulse2MM_X(dwPara);
			str.Format("%.2f", fPara);
			m_lstPositionPara.SetItemText(2, 2, str);
		}

		dwPara = m_pComPort->m_protocolstatus.positionPara.pos_x_moisture;
		m_lstPositionPara.InsertItem(3, "");
		m_lstPositionPara.SetItemText(3, 0, "pos_x_moisture");
		if (dwPara == 0xFFFFFFFF)
		{
			m_lstPositionPara.SetItemText(3, 1, "��Чֵ");
			m_lstPositionPara.SetItemText(3, 2, "��Чֵ");
		}
		else
		{
			str.Format("%d", dwPara);
			m_lstPositionPara.SetItemText(3, 1, str);
			fPara = Pulse2MM_X(dwPara);
			str.Format("%.2f", fPara);
			m_lstPositionPara.SetItemText(3, 2, str);
		}

		dwPara = m_pComPort->m_protocolstatus.positionPara.pos_y_min;
		m_lstPositionPara.InsertItem(4, "");
		m_lstPositionPara.SetItemText(4, 0, "pos_y_min");
		if (dwPara == 0xFFFFFFFF)
		{
			m_lstPositionPara.SetItemText(4, 1, "��Чֵ");
			m_lstPositionPara.SetItemText(4, 2, "��Чֵ");
		}
		else
		{
			str.Format("%d", dwPara);
			m_lstPositionPara.SetItemText(4, 1, str);
			fPara = Pulse2MM_Y(dwPara);
			str.Format("%.2f", fPara);
			m_lstPositionPara.SetItemText(4, 2, str);
		}

		dwPara = m_pComPort->m_protocolstatus.positionPara.pos_y_max;
		m_lstPositionPara.InsertItem(5, "");
		m_lstPositionPara.SetItemText(5, 0, "pos_y_max");
		if (dwPara == 0xFFFFFFFF)
		{
			m_lstPositionPara.SetItemText(5, 1, "��Чֵ");
			m_lstPositionPara.SetItemText(5, 2, "��Чֵ");
		}
		else
		{
			str.Format("%d", dwPara);
			m_lstPositionPara.SetItemText(5, 1, str);
			fPara = Pulse2MM_Y(dwPara);
			str.Format("%.2f", fPara);
			m_lstPositionPara.SetItemText(5, 2, str);
		}
		memcpy(&m_positionPara, &m_pComPort->m_protocolstatus.positionPara, sizeof(PositionPara));
	}
	else
	{
		AfxMessageBox("��λ�ò���ʧ��");
	}
}

void CRasBoardDlg::OnEnChangeEditRasPrintFreqPsc()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_FREQ_PSC))->GetWindowText(str);
	int n = atoi(str);
	int dpi = m_fIOBaseGridNum / n / m_fIOSpace * 25.4;

	str.Format("%d", dpi);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_RES_X))->SetWindowText(str);
	((CWnd*)GetDlgItem(IDC_EDIT_RAS_PRINT_RES_Y))->SetWindowText(str);
}

void CRasBoardDlg::OnBnClickedButtonRasGetSpeedLevel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lstSpeedLevel_X.DeleteAllItems();
	m_lstSpeedLevel_Y.DeleteAllItems();
	m_cbSpeedLevelX.ResetContent();
	m_cbSpeedLevelY.ResetContent();
	m_cbSpeedLevelX.EnableWindow(FALSE);
	m_cbSpeedLevelY.EnableWindow(FALSE);

	if (m_pComPort->ras_get_speed(0x00, 5000, FALSE))
	{
		int nSpeedNum = m_pComPort->m_protocolstatus.nSpeedLevelNum;
		CString str;

		if (nSpeedNum > 0)
		{
			for (int i = 0; i < nSpeedNum; i++)
			{
				str.Format("%d", i + 1);
				m_cbSpeedLevelX.AddString(str);

				int nAec = m_pComPort->m_protocolstatus.onlinePara[i].dist_acce;
				int nDec = m_pComPort->m_protocolstatus.onlinePara[i].dist_dece;
				int nSpeed = m_pComPort->m_protocolstatus.onlinePara[i].speed;

				float fAecS = Pulse2MM_X(nAec);
				float fDecS = Pulse2MM_X(nDec);
				float fEvenV = Pulse2MM_X((float)TIME_CONVERT_VALUE / (float)nSpeed);

				m_lstSpeedLevel_X.InsertItem(i, "");
				m_lstSpeedLevel_X.SetItemText(i, 0, str);

				str.Format("%.2f", fAecS);
				m_lstSpeedLevel_X.SetItemText(i, 1, str);
				str.Format("%.2f", fDecS);
				m_lstSpeedLevel_X.SetItemText(i, 2, str);
				str.Format("%.2f", fEvenV);
				m_lstSpeedLevel_X.SetItemText(i, 3, str);
			}
			m_cbSpeedLevelX.EnableWindow(TRUE);
			m_cbSpeedLevelX.SetCurSel(0);
		}
	}
	else
	{
		AfxMessageBox("��X�����ٶȵ�λʧ��");
	}

	if (m_pComPort->ras_get_speed(0x01, 5000, FALSE))
	{
		int nSpeedNum = m_pComPort->m_protocolstatus.nSpeedLevelNum;
		CString str;

		if (nSpeedNum > 0)
		{
			for (int i = 0; i < nSpeedNum; i++)
			{
				str.Format("%d", i + 1);
				m_cbSpeedLevelY.AddString(str);

				int nAec = m_pComPort->m_protocolstatus.onlinePara[i].dist_acce;
				int nDec = m_pComPort->m_protocolstatus.onlinePara[i].dist_dece;
				int nSpeed = m_pComPort->m_protocolstatus.onlinePara[i].speed;

				float fAecS = Pulse2MM_Y(nAec);
				float fDecS = Pulse2MM_Y(nDec);
				float fEvenV = Pulse2MM_Y((float)TIME_CONVERT_VALUE / (float)nSpeed);

				m_lstSpeedLevel_Y.InsertItem(i, "");
				m_lstSpeedLevel_Y.SetItemText(i, 0, str);

				str.Format("%.2f", fAecS);
				m_lstSpeedLevel_Y.SetItemText(i, 1, str);
				str.Format("%.2f", fDecS);
				m_lstSpeedLevel_Y.SetItemText(i, 2, str);
				str.Format("%.2f", fEvenV);
				m_lstSpeedLevel_Y.SetItemText(i, 3, str);
			}
			m_cbSpeedLevelY.EnableWindow(TRUE);
			m_cbSpeedLevelY.SetCurSel(0);
		}
	}
	else
	{
		AfxMessageBox("��Y�����ٶȵ�λʧ��");
	}
}

void CRasBoardDlg::OnBnClickedButtonRasMove2Position()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_MOVE2POSITION_X))->GetWindowText(str);
	float fx = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_RAS_MOVE2POSITION_Y))->GetWindowText(str);
	float fy = atof(str);

	DWORD dwX = MM2Pulse_X(fx);
	DWORD dwY = MM2Pulse_Y(fy);

	if (!m_pComPort->ras_move2_position(dwX, dwY, 20000, FALSE))
	{
		AfxMessageBox("�ƶ�ʧ��");
	}
}
void CRasBoardDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();
	//	CDialog::OnCancel();
}

void CRasBoardDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();
	//	CDialog::OnOK();
}
