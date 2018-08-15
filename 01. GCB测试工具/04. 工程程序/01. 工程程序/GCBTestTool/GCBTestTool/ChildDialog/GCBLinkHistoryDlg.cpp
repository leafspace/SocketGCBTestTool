#include "stdafx.h"
#include <string>
#include <fstream>
#include "../GCBTestTool.h"
#include "GCBLinkHistoryDlg.h"
#include "../GCBTestToolDlg.h"
using namespace std;

IMPLEMENT_DYNAMIC(GCBLinkHistoryDlg, CDialog)

GCBLinkHistoryDlg::GCBLinkHistoryDlg(CWnd* pParent)
	: CDialog(GCBLinkHistoryDlg::IDD, pParent)
{

}

GCBLinkHistoryDlg::~GCBLinkHistoryDlg()
{
}

void GCBLinkHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BOOL GCBLinkHistoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	this->m_list.GetClientRect(&rect);
	this->m_list.InsertColumn(0, _T("IP"), LVCFMT_CENTER, rect.Width(), 0);

	ifstream infile;
	infile.open(_T("history.log"));
	if (!infile.is_open()) {
		return FALSE;
	}

	string ipStr;
	while (getline(infile, ipStr)) {
		USES_CONVERSION;
		CString cIpStr(ipStr.c_str());

		this->m_list.InsertItem(0, _T(""));
		this->m_list.SetItemText(0, 0, cIpStr);
	}
	infile.close();

	return TRUE;
}

void GCBLinkHistoryDlg::OnBnClickedOk()
{
	POSITION positon = m_list.GetFirstSelectedItemPosition();
	int nIndex = m_list.GetNextSelectedItem(positon);

	if (nIndex == -1) {
		nIndex = 0;
	}
	CString itemStr = this->m_list.GetItemText(nIndex, 0);

	CGCBTestToolDlg *pMainDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());
	pMainDlg->SetIpAddress(itemStr);
	OnOK();
}

void GCBLinkHistoryDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	this->OnBnClickedOk();
	*pResult = 0;
}


BEGIN_MESSAGE_MAP(GCBLinkHistoryDlg, CDialog)
	ON_BN_CLICKED(IDOK, &GCBLinkHistoryDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &GCBLinkHistoryDlg::OnNMDblclkList1)
END_MESSAGE_MAP()


// GCBLinkHistoryDlg 消息处理程序