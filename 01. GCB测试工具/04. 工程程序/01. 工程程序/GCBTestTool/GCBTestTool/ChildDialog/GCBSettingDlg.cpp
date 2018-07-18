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
	GetDlgItemText(IDC_EDIT1, inputStr);

	list<BYTE> retLsg = this->CreateMessage(NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE, 0x0001, (float)_tstof(inputStr));

	MessageBean tempMessageBean;
	tempMessageBean.SetOrginDataList(retLsg);
	tempMessageBean.AnalysisOrginDataLst();
	GCBMainDlg::sendMessageQueue.Push_back(tempMessageBean);

	MessageBox(_T("成功写入消息队列"), _T("消息"), MB_ICONINFORMATION | MB_OK);
	OnOK();
}


list<BYTE> GCBSettingDlg::CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const float uReadNum)
{
	list<BYTE> retLst;

    // Head
    retLst.push_back(0xF0);
    retLst.push_back(0xF1);

    // CMD
    retLst.push_back(cmdID);

    // Size
	uint16_t uSize = sizeof(uRegisterAddress) + sizeof(uReadNum);
    retLst.push_back(BYTE0(uSize));
	retLst.push_back(BYTE1(uSize));

    // Parameter
    retLst.push_back(BYTE0(uRegisterAddress));
    retLst.push_back(BYTE1(uRegisterAddress));

    retLst.push_back(BYTE0(uReadNum));
    retLst.push_back(BYTE1(uReadNum));
	retLst.push_back(BYTE2(uReadNum));
	retLst.push_back(BYTE3(uReadNum));

    // Tail
    retLst.push_back(0xEC);
    return retLst;
}