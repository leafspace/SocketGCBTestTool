#include "stdafx.h"
#include "../GCBTestTool.h"
#include "../GCBTestToolDlg.h"
#include "GCBMainDlg.h"
#include "GCBDetailFrameDlg.h"

IMPLEMENT_DYNAMIC(GCBMainDlg, CDialog)

GCBMainDlg::GCBMainDlg(CWnd* pParent)
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

	this->nDialogLen = 0;
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		this->SetListHeadNum(nIndex, 4);
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

//=============================================================================

void GCBMainDlg::OnButtonClick(UINT nID)
{
	int nIndex = nID - IDC_DETAIL_BUTTON1;
	CGCBTestToolDlg *pMainDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());

	int newTabIndex = -1;

	FRAME_CMD_TYPE cmdType = CommunicateCore::GetCMDIdFromIndex(nIndex);
	for (int nJndex = 0; nJndex < this->nDialogLen; ++nJndex) {
		if (this->framePage[nJndex].GetFrameType() == cmdType) {
			newTabIndex = nJndex;
			break;
		}
	}

	if (newTabIndex < 0) {
		// 添加一个页面
		pMainDlg->AddNewFrameTab(GetLabel(nIndex));
		this->framePage[this->nDialogLen].SetFrameType(nIndex);             // 设置页面窗口的属性
		this->framePage[this->nDialogLen].Create(IDD_DETAIL_DIALOG, pMainDlg->GetTabCtrl());

		// 设定在Tab内显示的范围
		CRect cRect;
		pMainDlg->GetTabCtrl()->GetClientRect(cRect);
		cRect.bottom = cRect.bottom - TOPBAR_SIZE;
		this->framePage[this->nDialogLen].MoveWindow(&cRect);

		pMainDlg->AddNewFrameTab(&this->framePage[this->nDialogLen]);
		this->nDialogLen++;
	} else {
		pMainDlg->ChangeTabCtrl(&this->framePage[newTabIndex]);
	}
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

void GCBMainDlg::ClearListHead(int nIndex)
{
	CHeaderCtrl* pHeaderCtrl = this->m_List[nIndex].GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		int nColumnCount = pHeaderCtrl->GetItemCount();
		for (int i=0; i<nColumnCount; i++)
		{
			this->m_List[nIndex].DeleteColumn(0);
		}
	}
}

//=============================================================================

GCBDetailFrameDlg *GCBMainDlg::GetFramePage(FRAME_CMD_TYPE cmdType)
{
	for (int nIndex = 0; nIndex < this->nDialogLen; ++nIndex) {
		if (this->framePage[nIndex].GetFrameType() == cmdType) {
			return &this->framePage[nIndex];
		}
	}
	return NULL;
}

void GCBMainDlg::RefreshPage(CommunicateCore* communicationCore)
{
	// 刷新页面数据
	CString timeStr;
	list<float> retValueLst;
	MessageBean beanMessage;
	while (communicationCore->HaveRecvMessageResidual()) {
		retValueLst.clear();

		ofstream outfile("gcb.log", ios::app);
		communicationCore->RecvResponseMessage(&beanMessage);
		communicationCore->WriteLog(outfile, beanMessage, &timeStr, retValueLst);
		outfile.close();

		// 判断这个Message属于哪个组的命令
		int nCMDID = CommunicateCore::GetIndexFromCMDId(beanMessage.GetCMDType());
		CListCtrl *beanList = &this->m_List[nCMDID];
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
			formatStr = CommunicateCore::GetFormatStrLable(beanMessage.GetCMDType(), *iter);
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
		GCBDetailFrameDlg *framePage = this->GetFramePage(beanMessage.GetCMDType());
		if (framePage != NULL) {
			framePage->AddMessageBean(beanMessage);
		}
	}
}

void GCBMainDlg::CreateTimer(TIMER_TYPE timer, int timeGap)
{
	SetTimer(timer, timeGap, 0);
}

void GCBMainDlg::DeleteTimer(TIMER_TYPE timer)
{
	KillTimer(timer);
}

void GCBMainDlg::ShowSettingDialog(void)
{
	this->gcbSettingPage.DoModal();
}

void GCBMainDlg::SetListHeadNum(int nIndex, int nNum)
{
	const CString strTableHeadLable[] = {
		LABLE_INK_BOX,
		LABLE_RELAY,
		LABLE_RELAY,
		LABLE_INK_PUMP,
		LABLE_DATA,
		LABLE_DATA,
		LABLE_DATA
	};
	int nTableHeadNum = nNum + 1;
	CRect rect;
	this->m_List[nIndex].DeleteAllItems();
	this->ClearListHead(nIndex);
	this->m_List[nIndex].GetClientRect(&rect); //获得当前listcontrol的宽度
	this->m_List[nIndex].SetExtendedStyle(this->m_List[nIndex].GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	this->m_List[nIndex].InsertColumn(0, LABLE_TIME, LVCFMT_CENTER,
		rect.Width() / nTableHeadNum + 10, 0);

	// 插入表头标题		
	for (int nJIndex = 1; nJIndex < nTableHeadNum; ++nJIndex) {
		CString labelStr = strTableHeadLable[nIndex];
		labelStr.Format(_T("%s%d"), labelStr, nJIndex);
		this->m_List[nIndex].InsertColumn(nJIndex, labelStr, LVCFMT_CENTER,
			(rect.Width() - 40) / nTableHeadNum, nJIndex);
	}
}

void GCBMainDlg::ClearAllData(void)
{
	this->showValueLst.clear();
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		this->m_List[nIndex].DeleteAllItems();
	}

	for (int nIndex = 0; nIndex < nDialogLen; ++nIndex) {
		this->framePage[nIndex].ClearAllData();
	}
}
