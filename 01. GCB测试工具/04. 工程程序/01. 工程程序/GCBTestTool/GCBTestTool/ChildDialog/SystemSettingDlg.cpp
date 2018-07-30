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
	ON_BN_CLICKED(IDOK, &SystemSettingDlg::OnBnClickedOk)
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
		SetDlgItemText(IDC_EDIT_ADDRESS1 + nIndex, _T("00"));
		SetDlgItemText(IDC_EDIT_NUM1 + nIndex, _T("2"));
		if ((nIndex + 1) == LIST_NUM) {
			SetDlgItemText(IDC_EDIT_NUM1 + nIndex, _T("1"));
		}
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

void SystemSettingDlg::OnBnClickedOk()
{
	this->SetRequestCheckFlag();
	this->SetRequestBeginAddress();
	this->SetRequestDriveNums();
	this->SetTimerGap();
	OnOK();
}

bool SystemSettingDlg::SetRequestCheckFlag()
{
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK1 + nIndex);
		CommunicateCore::SetRequestFlag(nIndex, pButton->GetCheck() == 1);
	}
	return true;
}

bool SystemSettingDlg::SetRequestBeginAddress()
{
	CString strEdit;
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		GetDlgItemText(IDC_EDIT_ADDRESS1 + nIndex, strEdit);
		uint16_t nValude = 0;
		int temp = 0;
		for (int i = 0; i <= strEdit.GetLength();i++) {
			swscanf_s(strEdit.Mid(i,1),_T("%x"),&temp);
			nValude += temp*(int)(pow(16,(strEdit.GetLength()-1.0-i)));
		}
		CommunicateCore::SetRequestBeginAddress(nIndex, nValude);
	}
	return true;
}

bool SystemSettingDlg::SetRequestDriveNums()
{
	CString strEdit;
	CGCBTestToolDlg *pMainDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());
	if (!pMainDlg) {
		return false;
	}
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		GetDlgItemText(IDC_EDIT_NUM1 + nIndex, strEdit);
		CommunicateCore::SetRequestDriveNums(nIndex, _ttoi(strEdit));
		pMainDlg->SetMainPageListHeadNum(nIndex, _ttoi(strEdit));
	}
	return true;
}

bool SystemSettingDlg::SetTimerGap()
{
	return true;
}