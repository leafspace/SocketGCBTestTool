#include "stdafx.h"
#include "../GCBTestToolDlg.h"
#include "GCBSettingDlg.h"


// GCBSettingDlg 对话框

IMPLEMENT_DYNAMIC(GCBSettingDlg, CDialog)

GCBSettingDlg::GCBSettingDlg(CWnd* pParent /*=NULL*/)
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


// GCBSettingDlg 消息处理程序

void GCBSettingDlg::OnBnClickedOk()
{
	CString inputStr;
	bool bSuccessFlag = true;

	for (int nIndexID = IDC_EDIT1; nIndexID <= IDC_EDIT5; ++nIndexID) {
		GetDlgItemText(nIndexID, inputStr);
		if (inputStr.GetLength() == 0) {
			continue;
		}

		list<BYTE> retLsg;
		switch (nIndexID)
		{
		case IDC_EDIT1:
			retLsg = CGCBTestToolDlg::CreateMessage(NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE, 0x0001, (float)_tstof(inputStr));
			break;
		case IDC_EDIT2:
			retLsg = CGCBTestToolDlg::CreateMessage(INK_MOTOR_DELAY_TIME, 0x0001, (float)_tstof(inputStr));
			break;
		case IDC_EDIT3:
			retLsg = CGCBTestToolDlg::CreateMessage(MODE_PRESSURE_NOZZLE_INK, 0x0001, (uint16_t)_tstoi(inputStr));
			break;
		case IDC_EDIT4:
			retLsg = CGCBTestToolDlg::CreateMessage(START_WORKING_STATE_CIRCULATING_MOTOR, 0x0001, (uint16_t)_tstoi(inputStr));
			break;
		case IDC_EDIT5:
			retLsg = CGCBTestToolDlg::CreateMessage(MANUALLY_OPEN_MODE_LOCKED_SOLENOID_VALVE_STATUS, 0x0001, (uint16_t)_tstoi(inputStr));
			break;
		default: break;
		}

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

