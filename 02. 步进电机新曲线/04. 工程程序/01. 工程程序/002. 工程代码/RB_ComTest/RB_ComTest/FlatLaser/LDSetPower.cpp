// LDSetPower.cpp : ʵ���ļ�
//

#include "../stdafx.h"
#include "../RB_ComTest.h"
#include "LDSetPower.h"
#include "LDBoardDlg.h"

// CLDSetPower �Ի���

IMPLEMENT_DYNAMIC(CLDSetPower, CDialog)

CLDSetPower::CLDSetPower(CWnd* pParent /*=NULL*/)
	: CDialog(CLDSetPower::IDD, pParent)
{
	m_fVol = 0;
	m_nPower = 0;
}

CLDSetPower::~CLDSetPower()
{
}

void CLDSetPower::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLDSetPower, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LD_ADD_CHECK_POWER, &CLDSetPower::OnBnClickedButtonLdAddCheckPower)
	ON_BN_CLICKED(IDC_BUTTON_LD_MODIFY_CHECK_POWER, &CLDSetPower::OnBnClickedButtonLdModifyCheckPower)
	ON_BN_CLICKED(IDC_BUTTON_LD_CLOSE_CHECK_POWER, &CLDSetPower::OnBnClickedButtonLdCloseCheckPower)
END_MESSAGE_MAP()


// CLDSetPower ��Ϣ�������

BOOL CLDSetPower::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str;

	str.Format("%.3f", m_fVol);
	((CWnd*)GetDlgItem(IDC_STATIC_LD_SET_VOL))->SetWindowText(str);


	((CEdit*)GetDlgItem(IDC_EDIT_LD_SET_POWER))->CreateSolidCaret(1, 14);
	((CEdit*)GetDlgItem(IDC_EDIT_LD_SET_POWER))->ShowCaret();
	((CEdit*)GetDlgItem(IDC_EDIT_LD_SET_POWER))->SetCaretPos(CPoint(0, 0));
	((CEdit*)GetDlgItem(IDC_EDIT_LD_SET_POWER))->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLDSetPower::OnBnClickedButtonLdAddCheckPower()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str = "";

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_POWER))->GetWindowText(str);

	if (str == "")
	{
		AfxMessageBox("���빦��ֵ����Ϊ��");
		return;
	}
	else
	{
		m_nPower = atoi(str);
	}
	((CLDBoardDlg*)m_pParentWnd)->LD_Check_Power_Add(m_fVol, m_nPower);
}

void CLDSetPower::OnBnClickedButtonLdModifyCheckPower()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str = "";

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_POWER))->GetWindowText(str);

	if (str == "")
	{
		AfxMessageBox("���빦��ֵ����Ϊ��");
		return;
	}
	else
	{
		m_nPower = atoi(str);
	}
	((CLDBoardDlg*)m_pParentWnd)->LD_Check_Power_Modify(m_fVol, m_nPower);
}

void CLDSetPower::OnBnClickedButtonLdCloseCheckPower()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
