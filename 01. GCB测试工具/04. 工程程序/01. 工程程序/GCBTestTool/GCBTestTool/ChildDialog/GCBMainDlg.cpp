#include "stdafx.h"
#include <windows.h>
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
	ON_WM_TIMER()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_DETAIL_BUTTON1, IDC_DETAIL_BUTTON12, OnButtonClick)
END_MESSAGE_MAP()


// GCBMainDlg 消息处理程序

BOOL GCBMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	const int nTableHeadNum = 5;
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		CRect rect;
		this->m_List[nIndex].GetClientRect(&rect); //获得当前listcontrol的宽度
		this->m_List[nIndex].SetExtendedStyle(this->m_List[nIndex].GetExtendedStyle() |
			LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		this->m_List[nIndex].InsertColumn(0, _T("时间"), LVCFMT_CENTER,
			rect.Width() / nTableHeadNum, 0);

		// 插入表头标题		
		for (int nJIndex = 1; nJIndex < nTableHeadNum; ++nJIndex) {
			CString labelStr;
			labelStr.Format(_T("数据%d"), nJIndex);
			this->m_List[nIndex].InsertColumn(nJIndex, labelStr, LVCFMT_CENTER,
				(rect.Width() - 20) / nTableHeadNum, nJIndex);
		}
	}

	// 设置表标题文言
	int nLabelSize = GetLabelSize();
	for (int nIndex = 0; nIndex < nLabelSize; ++nIndex) {
		SetDlgItemText(IDC_STATIC_LABEL1 + nIndex, GetLabel(nIndex));
	}
	return TRUE;
}

void GCBMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_DIALOG_DRAW:
		this->StartDraw(IDC_PICTURE1);
		break;
	default:
		KillTimer(nIDEvent);
	}
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
	default:
		return NULL;
	}
}

float GCBMainDlg::Make4ByteFloat(BYTE *list)
{
	float retValue;
	memcpy(&retValue, list, sizeof(retValue));
	return retValue;
}

float GCBMainDlg::MakeTurn4ByteFloat(BYTE *list)
{
	BYTE newList[4] = { 0 };
	for (int nIndex = 0; nIndex < 4; ++nIndex) {
		newList[nIndex] = list[3 - nIndex];
	}

	float retValue = this->Make4ByteFloat(newList);
	return retValue;
}

short GCBMainDlg::GetFormatSize(FRAME_CMD_TYPE cmdType)
{
	switch (cmdType)
	{
	case NOZZLE_CARTRIDGE_LEVEL:
	case MODE_LOCKED_SOLENOID_VALVE_WORKING:
	case POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING:
	case INK_SUPPLY_PUMP_WORKING_CONDITION:
		return 1;
	case NOZZLE_CABINET_TEMPERATURE:
	case AIR_NEGATIVE_PRESSURE_VALUE:
	case AIR_POSITIVE_PRESSURE_VALUE:
		return 4;
	default:
		return 1;
	}
}

bool GCBMainDlg::WriteLog(MessageBean beanMessage, CString *timeStr, list<float> &retValueLst)
{
	ofstream outfile("gcb.log", ios::app);
	if (!outfile) {
		return false;
	}

	SYSTEMTIME sys;
	GetLocalTime(&sys);

	timeStr->Format(_T("%4d-%02d-%02d %02d:%02d:%02d "), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	outfile << setw(4) << setfill('0') << sys.wYear << "-";
	outfile << setw(2) << setfill('0') << sys.wMonth << "-";
	outfile << setw(2) << setfill('0') << sys.wDay << " ";
	outfile << setw(2) << setfill('0') << sys.wHour << ":";
	outfile << setw(2) << setfill('0') << sys.wMinute << ":";
	outfile << setw(2) << setfill('0') << sys.wSecond << " ";

	outfile << "0x" << hex << beanMessage.GetCMDType() << " :  ";

	short formatSize = this->GetFormatSize(beanMessage.GetCMDType());
	list<BYTE> beanPLst = beanMessage.GetParameterList();
	list<BYTE>::iterator iterP = beanPLst.begin();
	iterP++;// 地址位
	iterP++;// 地址位
	for (int nIndex = 0; nIndex < (beanMessage.GetParameterSize() - 2) / formatSize; ++nIndex) {
		BYTE *list = new BYTE[formatSize];
		for (int nJIndex = 0; nJIndex < formatSize; ++nJIndex, ++iterP) {
			list[nJIndex] = *iterP;
		}

		float retValue = formatSize == 4 ? this->MakeTurn4ByteFloat(list) : list[0];

		delete list;
		outfile << setiosflags(ios::fixed) << setprecision(2) << retValue << "\t";
		retValueLst.push_back(retValue);
	}


	outfile << "[";
	list<BYTE> beanOLst = beanMessage.GetOrginDataList();
	list<BYTE>::iterator iterO = beanOLst.begin();
	for (int nIndex = 0; nIndex < (int)beanOLst.size(); ++nIndex, ++iterO) {
		if ((nIndex + 1) == beanOLst.size()) {
			outfile << "0x" << hex << setw(2) << setfill('0') << (int)*iterO;
		}
		else {
			outfile << "0x" << hex << setw(2) << setfill('0') << (int)*iterO << "\t";
		}
	}
	outfile << "]";
	outfile << endl;

	outfile.close();
	return true;
}

void GCBMainDlg::StartDraw(int ControlID)
{
	CRect rc;
	CPaintDC dc(this);
	CWnd *pWnd = GetDlgItem(ControlID);
	pWnd->GetWindowRect(rc);
	CDC *pdc = pWnd->GetDC();

	CPen newPen;                                                            // 用于创建新画笔
	CPen *pOldPen;                                                          // 用于存放旧画笔
	CDC MemDC;                                                              // 首先定义一个显示设备对象
	CBitmap MemBitmap;                                                      // 定义一个位图对象

	int width = rc.Width();
	int height = rc.Height();

	MemDC.CreateCompatibleDC(NULL);                                         // 随后建立与屏幕显示兼容的内存显示设备
	MemBitmap.CreateCompatibleBitmap(pdc, width, height);                   // 下面建立一个与屏幕显示兼容的位图
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);                      // 将位图选入到内存显示设备中//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	MemDC.FillSolidRect(1, 1, width - 2, height - 2, RGB(255, 255, 255));   // 先用背景色将位图清除干净

	newPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));                          // 创建实心画笔，粗度为1，颜色为绿色
	pOldPen = MemDC.SelectObject(&newPen);                                  // 选择新画笔，并将旧画笔的指针保存到pOldPen

	unsigned int pitureWidth = (unsigned int)(width - 1);
	unsigned int pitureHeight = (unsigned int)(height - 1);
	int oldMaxX = 0, oldMaxY = pitureHeight / 2;
	int oldMinX = 0, oldMinY = pitureHeight / 2;

	list<float>::iterator iter = this->showValueLst.begin();
	for (int i = 1; i < (int)min(PICTRUE_SHOW_SIZE, this->showValueLst.size()); ++i, iter++) {
		double showMax = 0, showMin = 0;

		showMax = *iter;
		showMin = *iter;

		// 显示数值
		showMax /= pow((double)2, (double)sizeof(BYTE));
		showMax = (pitureHeight / 2) * showMax;
		showMin /= pow((double)2, (double)sizeof(BYTE));
		showMin = (pitureHeight / 2) * showMin;
		MemDC.MoveTo(oldMaxX, oldMaxY);
		MemDC.LineTo(i, (int)((pitureHeight / 2) - showMax));
		MemDC.MoveTo(oldMinX, oldMinY);
		MemDC.LineTo(i, (int)((pitureHeight / 2) - showMin));
		oldMaxX = i;
		oldMaxY = (int)((pitureHeight / 2) - showMax);
		oldMinX = i;
		oldMinY = (int)((pitureHeight / 2) - showMin);
	}

	MemDC.SelectObject(pOldPen);                                            // 恢复旧画笔
	newPen.DeleteObject();                                                  // 删除新画笔

	newPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));                            // 创建实心画笔，粗度为1，颜色为绿色
	pOldPen = MemDC.SelectObject(&newPen);                                  // 选择新画笔，并将旧画笔的指针保存到pOldPen
	MemDC.MoveTo(0, pitureHeight / 2);
	MemDC.LineTo(pitureWidth, pitureHeight / 2);
	for (unsigned int i = 0; i < pitureWidth; i += (pitureWidth / 10)) {
		MemDC.MoveTo(i, pitureHeight / 2 + 5);
		MemDC.LineTo(i, pitureHeight / 2 - 5);
	}

	pdc->BitBlt(0, 0, width - 2, height - 2, &MemDC, 0, 0, SRCCOPY);        // 将内存中的图拷贝到屏幕上进行显示

	MemBitmap.DeleteObject();                                               // 绘图完成后的清理
	MemDC.DeleteDC();
	CDialog::OnPaint();
}

void GCBMainDlg::RefreshPage()
{
	// 刷新页面数据
	CString timeStr;
	list<float> retValueLst;
	MessageBean beanMessage;
	while (GCBMainDlg::recvMessageQueue.IsEmpty() == false) {
		retValueLst.clear();

		GCBMainDlg::recvMessageQueue.Pop_front(&beanMessage);
		this->WriteLog(beanMessage, &timeStr, retValueLst);

		// 判断这个Message属于哪个组的命令
		CListCtrl *beanList = this->JudgeMessageCMDCtrl(beanMessage);
		if (beanList == NULL) {
			continue;
		}

		// 将数据放入显示控件中
		CString formatStr;
		list<float>::iterator iter = retValueLst.begin();
		beanList->InsertItem(0, timeStr);
		beanList->SetItemText(0, 0, timeStr);

		double sumValue = 0;
		for (int nIndex = 1; nIndex < (int)retValueLst.size() + 1; ++nIndex, ++iter) {
			sumValue += (*iter);
			formatStr.Format(_T("%.2f"), *iter);
			beanList->SetItemText(0, nIndex, formatStr);
		}
		// 计算平均值并写入到绘图数据表中
		if (beanMessage.GetCMDType() == AIR_NEGATIVE_PRESSURE_VALUE) {
			sumValue = sumValue / retValueLst.size();
			this->showValueLst.push_back((float)sumValue);
			if (this->showValueLst.size() > PICTRUE_SHOW_SIZE) {
				this->showValueLst.pop_front();
			}
		}

		// 将数据存入详细页面
		CGCBTestToolDlg *pDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());
		GCBDetailFrameDlg *framePage = pDlg->GetFramePage(beanMessage.GetCMDType());
		if (framePage != NULL) {
			framePage->AddMessageBean(beanMessage);
		}
	}
}

void GCBMainDlg::CreateTimer(TIMER_TYPE timer)
{
	SetTimer(timer, DRAW_GAP, 0);
}

void GCBMainDlg::DeleteTimer(TIMER_TYPE timer)
{
	KillTimer(timer);
}

void GCBMainDlg::ClearAllData(void)
{
	this->showValueLst.clear();
}