// GridParameter.cpp : 实现文件
//

#include "stdafx.h"
#include "RB_ComTest.h"
#include "GridParameterDlg.h"


// CGridParameterDlg 对话框

IMPLEMENT_DYNAMIC(CGridParameterDlg, CDialog)

CGridParameterDlg::CGridParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridParameterDlg::IDD, pParent)
{

}

CGridParameterDlg::~CGridParameterDlg()
{
}

void CGridParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGridParameterDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGridParameterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGridParameterDlg 消息处理程序

BOOL CGridParameterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString str;
	
	str.Format("%.2f", *m_pfIOSpace);
	((CWnd*)GetDlgItem(IDC_EDIT_IO_DISTANCE))->SetWindowText(str);
	
	str.Format("%.2f", *m_pfIOBaseGridNum);
	((CWnd*)GetDlgItem(IDC_EDIT_IO_BASEGRIDNUM))->SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGridParameterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_IO_DISTANCE))->GetWindowText(str);
	*m_pfIOSpace = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_IO_BASEGRIDNUM))->GetWindowText(str);
	*m_pfIOBaseGridNum = atof(str);

	OnOK();
}
