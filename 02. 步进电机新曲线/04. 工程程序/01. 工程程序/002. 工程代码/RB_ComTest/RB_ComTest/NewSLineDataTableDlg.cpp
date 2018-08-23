#include "stdafx.h"
#include "RB_ComTest.h"
#include "NewSLineDataTableDlg.h"

// CNewSLineDataTableDlg 对话框
IMPLEMENT_DYNAMIC(CNewSLineDataTableDlg, CDialog)

CNewSLineDataTableDlg::CNewSLineDataTableDlg(CWnd* pParent)
	: CDialog(CNewSLineDataTableDlg::IDD, pParent)
{
}

CNewSLineDataTableDlg::~CNewSLineDataTableDlg()
{
}

void CNewSLineDataTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNewSLineDataTableDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SFONLINE_OPERATION, &CNewSLineDataTableDlg::OnBnClickedButtonSfonlineOperation)
END_MESSAGE_MAP()


// CNewSLineDataTableDlg 消息处理程序

BOOL CNewSLineDataTableDlg::OnInitDialog()
{
	SetDlgItemText(IDC_EDIT_SFONLINE_STARTFREQUE, _T("400"));
	SetDlgItemText(IDC_EDIT_SFONLINE_UNIFOFREQUE, _T("9600"));
	SetDlgItemText(IDC_EDIT_SFONLINE_ACCELPERIOD, _T("600"));
	SetDlgItemText(IDC_EDIT_SFONLINE_SEMEGMENNUM, _T("10"));
	SetDlgItemText(IDC_BUTTON_SFONLINE_OPERATION, _T("新增速度表"));

	(CProgressCtrl*)GetDlgItem(IDC_PROGRESS_SFONLINE_DOWNLOAD)->ShowWindow(false);
	return TRUE;
}
void CNewSLineDataTableDlg::OnBnClickedButtonSfonlineOperation()
{
	CProgressCtrl* m_pgsDownload = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_SFONLINE_DOWNLOAD);
	m_pgsDownload->ShowWindow(true);
	m_pgsDownload->SetRange(0, 100);
	m_pgsDownload->SetPos(0);
}
