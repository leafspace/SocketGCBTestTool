#include "stdafx.h"
#include "GCBSettingDlg.h"
#include "../GCBTestToolDlg.h"

IMPLEMENT_DYNAMIC(GCBSettingDlg, CDialog)

GCBSettingDlg::GCBSettingDlg(CWnd* pParent)
	: CDialog(GCBSettingDlg::IDD, pParent)
{
}

GCBSettingDlg::~GCBSettingDlg()
{
}

void GCBSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GCBSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &GCBSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void GCBSettingDlg::OnBnClickedOk()
{
	CString inputStr;
	list<BYTE> retLsg;
	bool bSuccessFlag = true;
	CommunicateCore *communicationCore = NULL;
	CGCBTestToolDlg *pMainDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());
	if (pMainDlg) {
		communicationCore = pMainDlg->GetCommunicateCore();
	}
	if (communicationCore == NULL) {
		return;
	}

	for (int nIndexID = IDC_EDIT1; nIndexID <= IDC_EDIT3; ++nIndexID) {
		GetDlgItemText(nIndexID, inputStr);
		if (inputStr.GetLength() == 0) {
			continue;
		}

		switch (nIndexID)
		{
		case IDC_EDIT1:
			retLsg = CommunicateCore::CreateMessage(NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE, 0x0000, (float)_tstof(inputStr));
			break;
		case IDC_EDIT2:
			retLsg = CommunicateCore::CreateMessage(INK_MOTOR_DELAY_TIME, 0x0000, (float)(_tstof(inputStr) * 10));
			break;
		case IDC_EDIT3:
			retLsg = CommunicateCore::CreateMessage(MODE_PRESSURE_NOZZLE_INK, 0x0000, (uint16_t)(_tstoi(inputStr) - 1));
			break;
		default: break;
		}

		MessageBean tempMessageBean;
		tempMessageBean.SetOrginDataList(retLsg);
		tempMessageBean.AnalysisOrginDataLst();
		bool isSuccess = communicationCore->SendRequestMessage(tempMessageBean);
		bSuccessFlag = bSuccessFlag & isSuccess;
	}

	int nCheckID = 0;
	uint16_t uAddress, uValue;

	if ((nCheckID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2)) != 0) {
		uAddress = 0x0000;
		if (nCheckID == IDC_RADIO1) {
			uValue = 0x0000;
		}
		else {
			uValue = 0xffff;
		}
		retLsg = CommunicateCore::CreateMessage(START_WORKING_STATE_CIRCULATING_MOTOR, uAddress, uValue);
		MessageBean tempMessageBean;
		tempMessageBean.SetOrginDataList(retLsg);
		tempMessageBean.AnalysisOrginDataLst();
		bool isSuccess = communicationCore->SendRequestMessage(tempMessageBean);
		bSuccessFlag = bSuccessFlag & isSuccess;
	}

	if ((nCheckID = GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4)) != 0) {
		if (((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck()) {
			uAddress = 0xffff;
		}
		else {
			uAddress = 0x0000;
		}
		if (nCheckID == IDC_RADIO3) {
			uValue = 0x0000;
		}
		else {
			uValue = 0xffff;
		}
		retLsg = CommunicateCore::CreateMessage(MANUALLY_OPEN_MODE_LOCKED_SOLENOID_VALVE_STATUS, uAddress, uValue);
		MessageBean tempMessageBean;
		tempMessageBean.SetOrginDataList(retLsg);
		tempMessageBean.AnalysisOrginDataLst();
		bool isSuccess = communicationCore->SendRequestMessage(tempMessageBean);
		bSuccessFlag = bSuccessFlag & isSuccess;
	}


	if (bSuccessFlag) {
		MessageBox(GetStateCodeMessage(PROGRAM_WRITE_QUEUE), LABLE_TIP, MB_ICONINFORMATION | MB_OK);
	}
	else {
		MessageBox(GetStateCodeMessage(PROGRAM_CANT_WRITE_QUEUE), LABLE_ERROR, MB_ICONERROR | MB_OK);
	}

	OnOK();
}

