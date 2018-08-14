#include "stdafx.h"
#include "GCBSettingDlg.h"
#include "../GCBTestToolDlg.h"

IMPLEMENT_DYNAMIC(GCBSettingDlg, CDialog)

GCBSettingDlg::GCBSettingDlg(CWnd* pParent)
	: CDialog(GCBSettingDlg::IDD, pParent)
{
	this->sendOrderNums = 0;
}

GCBSettingDlg::~GCBSettingDlg()
{
}

void GCBSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_SET, mProCtrl);
}


BEGIN_MESSAGE_MAP(GCBSettingDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &GCBSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void GCBSettingDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_DIALOG_SET:
		this->CheckRecv();
		break;
	default:
		KillTimer(nIDEvent);
	}
}

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
		this->sendOrderNums++;
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
		this->sendOrderNums++;
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
		this->sendOrderNums++;
	}

	this->mProCtrl.SetRange(0, 100);
	if (bSuccessFlag) {
		MessageBox(GetStateCodeMessage(PROGRAM_WRITE_QUEUE), LABLE_TIP, MB_ICONINFORMATION | MB_OK);
		SetDlgItemText(IDC_EDIT_LOG, GetStateCodeMessage(PROGRAM_WRITE_QUEUE) + _T("\r\n"));

		CString tempStr;
		this->proStep = (int)(100 / (1 + this->sendOrderNums));
		tempStr.Format(_T("%2d%%"), this->proStep);
		SetDlgItemText(IDC_STATIC_PERCENT, tempStr);

		SetTimer(TIMER_DIALOG_SET, TIMER_GAP, 0);
		this->mProCtrl.SetStep(this->proStep);
		this->mProCtrl.StepIt();
	}
	else {
		MessageBox(GetStateCodeMessage(PROGRAM_CANT_WRITE_QUEUE), LABLE_ERROR, MB_ICONERROR | MB_OK);
		OnOK();
	}
}

void GCBSettingDlg::CheckRecv(void)
{
	if (this->sendOrderNums == 0) {
		OnOK();
	}

	CommunicateCore *communicationCore = NULL;
	CGCBTestToolDlg *pMainDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());
	if (pMainDlg) {
		communicationCore = pMainDlg->GetCommunicateCore();
	}
	if (communicationCore == NULL) {
		return;
	}

	MessageQueue* messageQueue = communicationCore->GetRecvSetMessageQueue();
	while (!messageQueue->IsEmpty() && this->sendOrderNums != 0) {
		MessageBean messageBean;
		messageQueue->Pop_front(&messageBean);

		CString orginStr, tempStr;
		GetDlgItemText(IDC_EDIT_LOG, orginStr);

		list<BYTE> dataList = messageBean.GetOrginDataList();
		for (list<BYTE>::iterator iter = dataList.begin(); iter != dataList.end(); ++iter) {
			tempStr.Format(_T("0x%x "), *iter);
			orginStr = orginStr + tempStr;
		}
		orginStr = orginStr + _T("\r\n");
		SetDlgItemText(IDC_EDIT_LOG, orginStr);
		CEdit *mLogEdit = (CEdit *)GetDlgItem(IDC_EDIT_LOG);
		mLogEdit->SetSel(-1);

		this->sendOrderNums--;

		tempStr.Format(_T("%2d%%"), (int)(100 / (1 + this->sendOrderNums)));
		SetDlgItemText(IDC_STATIC_PERCENT, tempStr);
		this->mProCtrl.SetStep(this->proStep);
		this->mProCtrl.StepIt();
	}
}