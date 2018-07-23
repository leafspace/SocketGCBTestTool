#include "stdafx.h"
#include "../GCBTestToolDlg.h"
#include "GCBSettingDlg.h"

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

	for (int nIndexID = IDC_EDIT1; nIndexID <= IDC_EDIT3; ++nIndexID) {
		GetDlgItemText(nIndexID, inputStr);
		if (inputStr.GetLength() == 0) {
			continue;
		}

		switch (nIndexID)
		{
		case IDC_EDIT1:
			retLsg = CGCBTestToolDlg::CreateMessage(NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE, 0x0000, (float)_tstof(inputStr));
			break;
		case IDC_EDIT2:
			retLsg = CGCBTestToolDlg::CreateMessage(INK_MOTOR_DELAY_TIME, 0x0000, (float)(_tstof(inputStr) * 10));
			break;
		case IDC_EDIT3:
			retLsg = CGCBTestToolDlg::CreateMessage(MODE_PRESSURE_NOZZLE_INK, 0x0000, (uint16_t)(_tstoi(inputStr) - 1));
			break;
		default: break;
		}

		MessageBean tempMessageBean;
		tempMessageBean.SetOrginDataList(retLsg);
		tempMessageBean.AnalysisOrginDataLst();
		bool isSuccess = GCBMainDlg::sendMessageQueue.Push_back(tempMessageBean);
		bSuccessFlag = bSuccessFlag & isSuccess;
	}

	int nCheckID = 0;
	uint16_t uAddress, uValue;

	if ((nCheckID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2)) != 0) {
		uAddress = 0x0000;
		if (nCheckID == IDC_RADIO1) {
			uValue = 0x0000;
		} else {
			uValue = 0xffff;
		}
		retLsg = CGCBTestToolDlg::CreateMessage(START_WORKING_STATE_CIRCULATING_MOTOR, uAddress, uValue);
		MessageBean tempMessageBean;
		tempMessageBean.SetOrginDataList(retLsg);
		tempMessageBean.AnalysisOrginDataLst();
		bool isSuccess = GCBMainDlg::sendMessageQueue.Push_back(tempMessageBean);
		bSuccessFlag = bSuccessFlag & isSuccess;
	}

	if ((nCheckID = GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4)) != 0) {
		if (((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck()) {
			uAddress = 0xffff;
		} else {
			uAddress = 0x0000;
		}
		if (nCheckID == IDC_RADIO3) {
			uValue = 0x0000;
		} else {
			uValue = 0xffff;
		}
		retLsg = CGCBTestToolDlg::CreateMessage(MANUALLY_OPEN_MODE_LOCKED_SOLENOID_VALVE_STATUS, uAddress, uValue);
		MessageBean tempMessageBean;
		tempMessageBean.SetOrginDataList(retLsg);
		tempMessageBean.AnalysisOrginDataLst();
		bool isSuccess = GCBMainDlg::sendMessageQueue.Push_back(tempMessageBean);
		bSuccessFlag = bSuccessFlag & isSuccess;
	}


	if (bSuccessFlag) {
		MessageBox(_T("成功写入消息队列"), _T("消息"), MB_ICONINFORMATION | MB_OK);
	}
	else {
		MessageBox(_T("未成功写入消息"), _T("错误"), MB_ICONERROR | MB_OK);
	}

	OnOK();
}

