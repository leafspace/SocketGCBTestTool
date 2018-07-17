#include "stdafx.h"
#include "../GCBTestTool.h"
#include "../GCBTestToolDlg.h"
#include "GCBMainDlg.h"
#include "GCBDetailFrameDlg.h"


MessageQueue GCBMainDlg::recvMessageQueue;
MessageQueue GCBMainDlg::sendMessageQueue;

// GCBMainDlg 对话框

IMPLEMENT_DYNAMIC(GCBMainDlg, CDialog)

GCBMainDlg::GCBMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GCBMainDlg::IDD, pParent)
{

}

GCBMainDlg::~GCBMainDlg()
{
}

void GCBMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		DDX_Control(pDX, IDC_LIST1 + nIndex, m_List[nIndex]);
	}
}


BEGIN_MESSAGE_MAP(GCBMainDlg, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_DETAIL_BUTTON1, IDC_DETAIL_BUTTON12, OnButtonClick)
END_MESSAGE_MAP()


// GCBMainDlg 消息处理程序

BOOL GCBMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	const int nTableHeadNum = 5, nTableDataNum = 2;
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		CRect rect;
		this->m_List[nIndex].GetClientRect(&rect); //获得当前listcontrol的宽度
		this->m_List[nIndex].SetExtendedStyle(this->m_List[nIndex].GetExtendedStyle() |
			LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		// 插入表头标题		
		for (int nJIndex = 0; nJIndex < nTableHeadNum; ++nJIndex) {
			if ((nJIndex + 1) == nTableHeadNum) {
				this->m_List[nIndex].InsertColumn(nJIndex, _T("数据标题"), LVCFMT_CENTER,
					rect.Width() / nTableHeadNum - 20, nJIndex);
			}
			else {
				this->m_List[nIndex].InsertColumn(nJIndex, _T("数据标题"), LVCFMT_CENTER,
					rect.Width() / nTableHeadNum, nJIndex);
			}
		}

		CString strDataValue;
		for (int nJIndex = 0; nJIndex < nTableDataNum; ++nJIndex) {
			this->m_List[nIndex].InsertItem(nJIndex, _T("数据"));
			for (int nKIndex = 0; nKIndex < nTableHeadNum; ++nKIndex) {
				strDataValue.Format(_T("数据%d"), nKIndex + 1);
				this->m_List[nIndex].SetItemText(nJIndex, nKIndex, strDataValue);
			}
		}
	}

	// 设置表标题文言
	int nLabelSize = GetLabelSize();
	for (int nIndex = 0; nIndex < nLabelSize; ++nIndex) {
		SetDlgItemText(IDC_STATIC_LABEL1 + nIndex, GetLabel(nIndex));
	}
	return TRUE;
}

void GCBMainDlg::OnButtonClick(UINT nID)
{
	int nIndex = nID - IDC_DETAIL_BUTTON1;

	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
	CGCBTestToolDlg* mainDlg = (CGCBTestToolDlg*)CGCBTestToolDlg::FromHandle(hWnd);

	// 添加一个页面
	mainDlg->AddNewFrameTab(nIndex);
}

CListCtrl* GCBMainDlg::JudgeMessageCMDCtrl(MessageBean beanMessage)
{
	switch (beanMessage.GetCMDType())
	{
	case NOZZLE_CARTRIDGE_LEVEL:
		return &this->m_List[0];
	case MODE_LOCKED_SOLENOID_VALVE_WORKING:
		return &this->m_List[1];
	case POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING:
		return &this->m_List[2];
	case INK_SUPPLY_PUMP_WORKING_CONDITION:
		return &this->m_List[3];
	case NOZZLE_CABINET_TEMPERATURE:
		return &this->m_List[4];
	case AIR_NEGATIVE_PRESSURE_VALUE:
		return &this->m_List[5];
	case AIR_POSITIVE_PRESSURE_VALUE:
		return &this->m_List[6];
	case NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE:
		return &this->m_List[7];
	case INK_MOTOR_DELAY_TIME:
		return &this->m_List[8];
	case MODE_PRESSURE_NOZZLE_INK:
		return &this->m_List[9];
	case START_WORKING_STATE_CIRCULATING_MOTOR:
		return &this->m_List[10];
	case MANUALLY_OPEN_MODE_LOCKED_SOLENOID_VALVE_STATUS:
		return &this->m_List[11];
	default:
		return &this->m_List[0];
	}
}

float GCBMainDlg::Make4ByteFloat(BYTE *list)
{
	float retValue;
	memcpy(&retValue, list, sizeof(retValue));
	return retValue;
}

bool GCBMainDlg::WriteLog(MessageBean beanMessage)
{
	ofstream outfile("gcb.log", ios::app);
	if (!outfile) {
		return false;
	}

	struct tm *tm_ptr;
	time_t the_time;
	(void)time(&the_time);
	tm_ptr = gmtime(&the_time);

	outfile << tm_ptr->tm_year << "-" << tm_ptr->tm_mon + 1 << "-" << tm_ptr->tm_mday << " ";
	outfile << tm_ptr->tm_hour << ":" << tm_ptr->tm_min << ":" << tm_ptr->tm_sec << " ";

	outfile << "0x" << hex << beanMessage.GetCMDType() << " :  ";

	if (beanMessage.GetParameterSize() % 4 == 0) {
		list<BYTE> beanLst = beanMessage.GetParameterList();
		list<BYTE>::iterator iter = beanLst.begin();
		for (int nIndex = 0; nIndex < beanMessage.GetParameterSize();) {
			BYTE list[4] = { 0 };
			for (int nJIndex = 0; nJIndex < 4; ++nJIndex, ++nIndex, ++iter) {
				list[nJIndex] = *iter;
			}
			outfile << setiosflags(ios::fixed) << setprecision(2) << this->Make4ByteFloat(list) << "\t";
		}
	}


	outfile << "[";
	list<BYTE> beanLst = beanMessage.GetParameterList();
	list<BYTE>::iterator iter = beanLst.begin();
	for (int nIndex = 0; nIndex < beanMessage.GetParameterSize(); ++nIndex, ++iter) {
		if ((nIndex + 1) == beanMessage.GetParameterSize()) {
			outfile << "0x" << hex << (int)*iter;
		} else {
			outfile << "0x" << hex << (int)*iter << "\t";
		}
	}
	outfile << "]";
	outfile << endl;

	outfile.close();
	return true;
}

void GCBMainDlg::RefreshPage()
{
	// 刷新页面数据
	MessageBean beanMessage;
	while (GCBMainDlg::recvMessageQueue.IsEmpty() == false) {
		GCBMainDlg::recvMessageQueue.Pop_front(&beanMessage);
		
		// 判断这个Message属于哪个组的命令
		CListCtrl *beanList = this->JudgeMessageCMDCtrl(beanMessage);

		this->WriteLog(beanMessage);
		// 将数据放入显示控件中
		CString formatStr;
		list<BYTE> beanLst = beanMessage.GetParameterList();
		list<BYTE>::iterator iter = beanLst.begin();
		beanList->DeleteAllItems();
		for (int nIndex = 0; nIndex < beanMessage.GetParameterSize(); ++nIndex, ++iter) {
			formatStr.Format(_T("0x%d"), (int)*iter);
			beanList->InsertItem(nIndex, formatStr);
		}
	}
}