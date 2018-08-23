// GridParameter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RB_ComTest.h"
#include "GridParameterDlg.h"


// CGridParameterDlg �Ի���

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


// CGridParameterDlg ��Ϣ�������

BOOL CGridParameterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str;
	
	str.Format("%.2f", *m_pfIOSpace);
	((CWnd*)GetDlgItem(IDC_EDIT_IO_DISTANCE))->SetWindowText(str);
	
	str.Format("%.2f", *m_pfIOBaseGridNum);
	((CWnd*)GetDlgItem(IDC_EDIT_IO_BASEGRIDNUM))->SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGridParameterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_IO_DISTANCE))->GetWindowText(str);
	*m_pfIOSpace = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_IO_BASEGRIDNUM))->GetWindowText(str);
	*m_pfIOBaseGridNum = atof(str);

	OnOK();
}
