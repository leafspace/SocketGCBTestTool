// SFParameterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RB_ComTest.h"
#include "RB_ComTestDlg.h"
#include "SFParameterDlg.h"

// CSFParameterDlg �Ի���

IMPLEMENT_DYNAMIC(CSFParameterDlg, CDialog)

CSFParameterDlg::CSFParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSFParameterDlg::IDD, pParent)
{

}

CSFParameterDlg::~CSFParameterDlg()
{
}

void CSFParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSFParameterDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSFParameterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SBP_SET_PARAMETER_X, &CSFParameterDlg::OnBnClickedButtonSbpSetParameterX)
	ON_BN_CLICKED(IDC_BUTTON_SBP_SET_PARAMETER_Y, &CSFParameterDlg::OnBnClickedButtonSbpSetParameterY)
	ON_BN_CLICKED(IDC_BUTTON_SBP_SET_PARAMETER_Z, &CSFParameterDlg::OnBnClickedButtonSbpSetParameterZ)
	ON_BN_CLICKED(IDC_BUTTON_SBP_GET_PARAMETER_X, &CSFParameterDlg::OnBnClickedButtonSbpGetParameterX)
	ON_BN_CLICKED(IDC_BUTTON_SBP_GET_PARAMETER_Y, &CSFParameterDlg::OnBnClickedButtonSbpGetParameterY)
	ON_BN_CLICKED(IDC_BUTTON_SBP_GET_PARAMETER_Z, &CSFParameterDlg::OnBnClickedButtonSbpGetParameterZ)
END_MESSAGE_MAP()


// CSFParameterDlg ��Ϣ�������

BOOL CSFParameterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str;

	str.Format("%.2f", *m_pfReducer_x);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_REDUCER_X))->SetWindowText(str);

	str.Format("%.2f", *m_pfPulsesPerRevolution_x);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_PULSESPERREVOLUTION_X))->SetWindowText(str);


	str.Format("%.2f", *m_pfDollyPerimeter);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_DOLLYPERIMETER))->SetWindowText(str);

	str.Format("%.2f", *m_pfReducer_y);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_REDUCER_Y))->SetWindowText(str);

	str.Format("%.2f", *m_pfPulsesPerRevolution_y);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_PULSESPERREVOLUTION_Y))->SetWindowText(str);

	str.Format("%.2f", *m_pfDeferentPerimeter);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_DEFERENTPERIMETER))->SetWindowText(str);

	str.Format("%.2f", *m_pfDeferentPitch);
	((CWnd*)GetDlgItem(IDC_EDIT_DEFERENTPITCH))->SetWindowText(str);

	str.Format("%.2f", *m_pfReducer_z);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_REDUCER_Z))->SetWindowText(str);

	str.Format("%.2f", *m_pfPulsesPerRevolution_z);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_PULSESPERREVOLUTION_Z))->SetWindowText(str);

	str.Format("%.2f", *m_pfDeferentPerimeter_z);
	((CWnd*)GetDlgItem(IDC_EDIT_SFP_DEFERENTPERIMETER_Z))->SetWindowText(str);

	str.Format("%.2f", *m_pfDeferentPitch_z);
	((CWnd*)GetDlgItem(IDC_EDIT_DEFERENTPITCH_Z))->SetWindowText(str);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSFParameterDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;
	CDialog::PostNcDestroy();
}

void CSFParameterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;

	((CWnd*)GetDlgItem(IDC_EDIT_SFP_REDUCER_X))->GetWindowText(str);
	*m_pfReducer_x = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFP_PULSESPERREVOLUTION_X))->GetWindowText(str);
	*m_pfPulsesPerRevolution_x = atof(str);


	((CWnd*)GetDlgItem(IDC_EDIT_SFP_DOLLYPERIMETER))->GetWindowText(str);
	*m_pfDollyPerimeter = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFP_REDUCER_Y))->GetWindowText(str);
	*m_pfReducer_y = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFP_PULSESPERREVOLUTION_Y))->GetWindowText(str);
	*m_pfPulsesPerRevolution_y = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFP_DEFERENTPERIMETER))->GetWindowText(str);
	*m_pfDeferentPerimeter = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_DEFERENTPITCH))->GetWindowText(str);
	*m_pfDeferentPitch = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFP_REDUCER_Z))->GetWindowText(str);
	*m_pfReducer_z = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFP_PULSESPERREVOLUTION_Z))->GetWindowText(str);
	*m_pfPulsesPerRevolution_z = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_SFP_DEFERENTPERIMETER_Z))->GetWindowText(str);
	*m_pfDeferentPerimeter_z = atof(str);

	((CWnd*)GetDlgItem(IDC_EDIT_DEFERENTPITCH_Z))->GetWindowText(str);
	*m_pfDeferentPitch_z = atof(str);

	OnOK();

	((CRB_ComTestDlg*)theApp.m_pMainWnd)->SetConfigInfo();
}

void CSFParameterDlg::OnBnClickedButtonSbpSetParameterX()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SF_BOARD_PARAMETER_X sf_board_parameter_x;

	sf_board_parameter_x.m_fReducerX = *m_pfReducer_x;
	sf_board_parameter_x.m_fDollyPerimeter = *m_pfDollyPerimeter;
	sf_board_parameter_x.m_fPulsesPerRevolution_x = *m_pfPulsesPerRevolution_x;

	if (!m_pComPort->servo_set_board_parameter_x(sf_board_parameter_x, CCmdTimeOut::set_firm_parameter, FALSE, m_bThreeAxisServo))
	{
		AfxMessageBox("�����ŷ�����--X����--ʧ��");
	}
	else
	{
		AfxMessageBox("���óɹ�");
	}
}

void CSFParameterDlg::OnBnClickedButtonSbpSetParameterY()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SF_BOARD_PARAMETER_Y sf_board_parameter_y;

	sf_board_parameter_y.m_fReducerY = *m_pfReducer_y;
	sf_board_parameter_y.m_fDeferentPerimeter = *m_pfDeferentPerimeter;
	sf_board_parameter_y.m_fDeferentPitch = *m_pfDeferentPitch;
	sf_board_parameter_y.m_fPulsesPerRevolution_y = *m_pfPulsesPerRevolution_y;

	if (!m_pComPort->servo_set_board_parameter_y(sf_board_parameter_y, CCmdTimeOut::set_firm_parameter, FALSE, m_bThreeAxisServo))
	{
		AfxMessageBox("�����ŷ�����--Y����--ʧ��");
	}
	else
	{
		AfxMessageBox("���óɹ�");
	}
}
void CSFParameterDlg::OnBnClickedButtonSbpSetParameterZ()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SF_BOARD_PARAMETER_Z sf_board_parameter_z;

	if (!m_bThreeAxisServo)
		return;

	sf_board_parameter_z.m_fReducer_Z = *m_pfReducer_z;
	sf_board_parameter_z.m_fDeferentPerimeter_Z = *m_pfDeferentPerimeter_z;
	sf_board_parameter_z.m_fDeferentPitch_Z = *m_pfDeferentPitch_z;
	sf_board_parameter_z.m_fPulsesPerRevolution_Z = *m_pfPulsesPerRevolution_z;

	if (!m_pComPort->servo_set_board_parameter_z(sf_board_parameter_z, CCmdTimeOut::set_firm_parameter, FALSE))
	{
		AfxMessageBox("�����ŷ�����--Y����--ʧ��");
	}
	else
	{
		AfxMessageBox("���óɹ�");
	}
}
void CSFParameterDlg::OnBnClickedButtonSbpGetParameterX()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BZERO(m_pComPort->m_protocolstatus.sf_board_parameter_x);

	if (m_pComPort->servo_get_board_parameter_x(CCmdTimeOut::get_firm_parameter, FALSE, m_bThreeAxisServo))
	{
		*m_pfReducer_x = m_pComPort->m_protocolstatus.sf_board_parameter_x.m_fReducerX;
		*m_pfDollyPerimeter = m_pComPort->m_protocolstatus.sf_board_parameter_x.m_fDollyPerimeter;
		*m_pfPulsesPerRevolution_x = m_pComPort->m_protocolstatus.sf_board_parameter_x.m_fPulsesPerRevolution_x;

		CString str;
		str.Format("��ѯ�ɹ�: ���ٻ�: %.2f С���ܳ�: %.2f ÿȦ����: %.2f", *m_pfReducer_x, *m_pfDollyPerimeter, *m_pfPulsesPerRevolution_x);
		AfxMessageBox(str);
	}
	else
	{
		AfxMessageBox("���ŷ�����--X����--ʧ��");
	}
}

void CSFParameterDlg::OnBnClickedButtonSbpGetParameterY()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BZERO(m_pComPort->m_protocolstatus.sf_board_parameter_y);

	if (m_pComPort->servo_get_board_parameter_y(CCmdTimeOut::get_firm_parameter, FALSE, m_bThreeAxisServo))
	{
		*m_pfReducer_y = m_pComPort->m_protocolstatus.sf_board_parameter_y.m_fReducerY;
		*m_pfDeferentPerimeter = m_pComPort->m_protocolstatus.sf_board_parameter_y.m_fDeferentPerimeter;
		*m_pfDeferentPitch = m_pComPort->m_protocolstatus.sf_board_parameter_y.m_fDeferentPitch;
		*m_pfPulsesPerRevolution_y = m_pComPort->m_protocolstatus.sf_board_parameter_y.m_fPulsesPerRevolution_y;

		CString str;
		str.Format("��ѯ�ɹ�: ���ٻ�: %.2f �����ܳ�: %.2f ˿���ݾ�: %.2f ÿȦ����: %.2f", *m_pfReducer_y, *m_pfDeferentPerimeter, *m_pfDeferentPitch, *m_pfPulsesPerRevolution_y);
		AfxMessageBox(str);
	}
	else
	{
		AfxMessageBox("���ŷ�����--Y����--ʧ��");
	}
}

void CSFParameterDlg::OnBnClickedButtonSbpGetParameterZ()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bThreeAxisServo)
		return;

	BZERO(m_pComPort->m_protocolstatus.sf_board_parameter_z);

	if (m_pComPort->servo_get_board_parameter_z(CCmdTimeOut::get_firm_parameter, FALSE))
	{
		*m_pfReducer_z = m_pComPort->m_protocolstatus.sf_board_parameter_z.m_fReducer_Z;
		*m_pfDeferentPerimeter_z = m_pComPort->m_protocolstatus.sf_board_parameter_z.m_fDeferentPerimeter_Z;
		*m_pfDeferentPitch_z = m_pComPort->m_protocolstatus.sf_board_parameter_z.m_fDeferentPitch_Z;
		*m_pfPulsesPerRevolution_z = m_pComPort->m_protocolstatus.sf_board_parameter_z.m_fPulsesPerRevolution_Z;

		CString str;
		str.Format("��ѯ�ɹ�(Motor_Z): ���ٻ�: %.2f �����ܳ�: %.2f ˿���ݾ�: %.2f ÿȦ����: %.2f", *m_pfReducer_z, *m_pfDeferentPerimeter_z, *m_pfDeferentPitch_z, *m_pfPulsesPerRevolution_z);
		AfxMessageBox(str);
	}
	else
	{
		AfxMessageBox("���ŷ�����--Y����--ʧ��");
	}
}
