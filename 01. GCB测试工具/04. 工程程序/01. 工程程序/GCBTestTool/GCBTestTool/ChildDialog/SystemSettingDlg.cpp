#include "stdafx.h"
#include "SystemSettingDlg.h"
#include "../GCBTestToolDlg.h"

IMPLEMENT_DYNAMIC(SystemSettingDlg, CDialog)

SystemSettingDlg::SystemSettingDlg(CWnd* pParent)
	: CDialog(SystemSettingDlg::IDD, pParent)
{
}

SystemSettingDlg::~SystemSettingDlg()
{
}

void SystemSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SystemSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ALL, &SystemSettingDlg::OnBnClickedCheckAll)
END_MESSAGE_MAP()

BOOL SystemSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 设置标题文言
	int nLabelSize = GetLabelSize();
	for (int nIndex = 0; nIndex < nLabelSize; ++nIndex) {
		SetDlgItemText(IDC_STATIC_LABEL1 + nIndex, GetLabel(nIndex));
	}

	this->isCheckAll = false;
	this->OnBnClickedCheckAll();
	((CButton*)GetDlgItem(IDC_CHECK_ALL))->SetCheck(true);

	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		SetDlgItemText(IDC_EDIT_NUM1 + nIndex, _T("2"));
	}

	SetDlgItemText(IDC_EDIT_TIMEGAP, _T("1000"));
	SetDlgItemText(IDC_EDIT_DRAWGAP, _T("3000"));
	return TRUE;
}

void SystemSettingDlg::OnBnClickedCheckAll()
{
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK1 + nIndex);
		pButton->SetCheck(!this->isCheckAll);
	}

	this->isCheckAll = !this->isCheckAll;
}
