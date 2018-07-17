
// GCBTestToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommonType.h"
#include "GCBTestTool.h"
#include "GCBTestToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

StateTable CGCBTestToolDlg::threadStateTable;

// CGCBTestToolDlg 对话框

CGCBTestToolDlg::CGCBTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCBTestToolDlg::IDD, pParent)
{
	this->m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	this->nCurSelTab = 0;
	this->nDialogLen = 0;

	this->strServerIP = _T("172.16.5.30");
	this->strServerPort = _T("10000");

	this->socketISLinking = false;
}

void CGCBTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRAMETAB, m_FrameTabCtrl);
	DDX_Control(pDX, IDC_SCROLLBAR, m_DlgScrollBar);
}

BEGIN_MESSAGE_MAP(CGCBTestToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_NOTIFY(TCN_SELCHANGE, IDC_FRAMETAB, &CGCBTestToolDlg::OnTcnSelchangeFrametab)
	ON_BN_CLICKED(IDC_BUTTON_LINKTEST, &CGCBTestToolDlg::OnBnClickedButtonLinktest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LINK, &CGCBTestToolDlg::OnBnClickedButtonLink)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CGCBTestToolDlg::OnBnClickedButtonSetting)
END_MESSAGE_MAP()

// CGCBTestToolDlg 消息处理程序

BOOL CGCBTestToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// Tab Control 添加主页面
	this->m_FrameTabCtrl.InsertItem(0, _T("概览"));

	this->mainPage.Create(IDD_MAIN_DIALOG, &this->m_FrameTabCtrl);
	// 设定页面在Tab内显示的范围
	CRect rc;
	this->m_FrameTabCtrl.GetClientRect(rc);
	rc.bottom -= 20;
	this->mainPage.MoveWindow(&rc);

	this->pDialog[0] = &this->mainPage;
	this->pDialog[0]->ShowWindow(SW_SHOW);

	// 滑块移动的位置为0——1000
	this->m_DlgScrollBar.SetScrollRange(SCROLLBAR_MIN, SCROLLBAR_MAX);

	// 设置输入框的默认参数
	SetDlgItemText(IDC_GCB_IPADDRESS, this->strServerIP);
	SetDlgItemText(IDC_GCB_EDIT_PORT, this->strServerPort);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGCBTestToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGCBTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGCBTestToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CGCBTestToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_SOCKET_LINK_CONN:
		this->OnTimerSocketLinkTest();
		break;
	case TIMER_SOCKET_LINK_RECV:
		this->mainPage.RefreshPage();
		break;
	case TIMER_SOCKET_LINK_SEND:
		this->SendRequestMessage();
		break;
	default:
		KillTimer(nIDEvent);
	}
}

BOOL CGCBTestToolDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta < 0)
	{
		OnVScroll(SB_LINEDOWN, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
	}
	else if (zDelta > 0)
	{
		OnVScroll(SB_LINEUP, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CGCBTestToolDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nCurpos = this->m_DlgScrollBar.GetScrollPos();                      // 取得当前Scroll bar 的位置值
	switch (nSBCode)                                                        // 处理滚动消息
	{
	case SB_LINEUP:
		nCurpos = nCurpos - TOPBAR_SIZE;
		break;
	case SB_LINEDOWN:
		nCurpos = nCurpos + TOPBAR_SIZE;
		break;
	case SB_THUMBTRACK:
		nCurpos = nPos;
		break;
	default:
		break;
	}

	if (nCurpos > SCROLLBAR_MAX) {
		nCurpos = SCROLLBAR_MAX;
	}
	else if (nCurpos < SCROLLBAR_MIN) {
		nCurpos = SCROLLBAR_MIN;
	}

	// 设置滚动条位置
	this->m_DlgScrollBar.SetScrollPos(nCurpos);

	// 设置子窗口移动位置
	CRect rc;
	this->m_FrameTabCtrl.GetClientRect(rc);
	rc.top = rc.top - nCurpos;
	rc.bottom = rc.bottom - TOPBAR_SIZE;

	switch (this->nCurSelTab)
	{
	case 0:
		this->mainPage.MoveWindow(&rc);
		break;
	default:
		this->framePage[this->nCurSelTab - 1].MoveWindow(&rc);
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CGCBTestToolDlg::OnTcnSelchangeFrametab(NMHDR *pNMHDR, LRESULT *pResult)
{
	this->pDialog[this->nCurSelTab]->ShowWindow(SW_HIDE);
	this->nCurSelTab = this->m_FrameTabCtrl.GetCurSel();
	this->pDialog[this->nCurSelTab]->ShowWindow(SW_SHOW);

	// 重置页面显示位置
	CRect rc;
	this->m_FrameTabCtrl.GetClientRect(rc);
	rc.bottom = rc.bottom - TOPBAR_SIZE;
	this->pDialog[this->nCurSelTab]->MoveWindow(&rc);

	// 设置滚动条位置
	this->m_DlgScrollBar.SetScrollPos(0);
	*pResult = 0;
}

void CGCBTestToolDlg::OnBnClickedButtonLinktest()
{
	bool bIsSuccess = true;

	// 获取连接的测试IP与端口号
	GetDlgItemText(IDC_GCB_IPADDRESS, this->strServerIP);
	GetDlgItemText(IDC_GCB_EDIT_PORT, this->strServerPort);

	GetDlgItem(IDC_BUTTON_LINKTEST)->EnableWindow(false);

	do {
		wstring wstr(this->strServerIP);
		string strString;
		strString.assign(wstr.begin(), wstr.end());
		bIsSuccess = this->socketLink.initSocket(strString, _ttoi(this->strServerPort));
		if (bIsSuccess == false) {
			break;
		}

	} while (false);

	if (bIsSuccess) {
		// 创建线程用于网络连接测试
		this->hThreadSocketLinkTest = CreateThread(NULL, 0, ThreadSocketLinkConn, &this->socketLink, 0, NULL);

		// 创建定时器定时检测连接线程是否结束
		SetTimer(TIMER_SOCKET_LINK_CONN, TIMER_OUT, 0);
	}
	else {
		GetDlgItem(IDC_BUTTON_LINKTEST)->EnableWindow(true);
		this->ShowMessage(PROGRAM_CANT_LINK_SERVER, PROGRAM_STATE_ERROR);
	}
}

void CGCBTestToolDlg::OnBnClickedButtonLink()
{
	if (this->socketISLinking) {
		this->socketISLinking = false;
		SetDlgItemText(IDC_BUTTON_LINK, _T("连接"));
		CloseHandle(this->hThreadSocketLinkTest);
		CloseHandle(this->hThreadSocketLinkRecv);
		CloseHandle(this->hThreadSocketLinkSend);
		KillTimer(TIMER_SOCKET_LINK_CONN);
		KillTimer(TIMER_SOCKET_LINK_RECV);
		KillTimer(TIMER_SOCKET_LINK_SEND);
	} else {
		this->socketISLinking = true;
		SetDlgItemText(IDC_BUTTON_LINK, _T("停止连接"));
		this->OnBnClickedButtonLinktest();
	}
}

void CGCBTestToolDlg::OnTimerSocketLinkTest()
{
	bool bIsSuccess = true;
	// 检查线程是否结束
	int retState = CGCBTestToolDlg::threadStateTable.GetThreadFinishedFlag(TIMER_SOCKET_LINK_CONN);
	switch (retState)
	{
	case TIMER_STATE_UNRUNNING:
	case TIMER_STATE_RUNNING:
		return;
	case TIMER_STATE_ERROR:
		return;
	case TIMER_STATE_FINISH:
		KillTimer(TIMER_SOCKET_LINK_CONN);
		bIsSuccess = CGCBTestToolDlg::threadStateTable.GetThreadRetValueFlag(TIMER_SOCKET_LINK_CONN) > 0;
		CGCBTestToolDlg::threadStateTable.RemoveThreadFinishedFlag(TIMER_SOCKET_LINK_CONN);
		CGCBTestToolDlg::threadStateTable.RemoveThreadRetValueFlag(TIMER_SOCKET_LINK_CONN);
		break;
	default:
		return;
	}

	if (bIsSuccess) {
		this->ShowMessage(PROGRAM_LINK_SERVER, PROGRAM_STATE_TIP);
	}
	else {
		this->ShowMessage(PROGRAM_CANT_LINK_SERVER, PROGRAM_STATE_ERROR);
		if (this->socketISLinking) {
			GetDlgItem(IDC_BUTTON_LINK)->EnableWindow(true);
		}
	}

	// 如果结束了就设置按钮可以使用且输出状态
	GetDlgItem(IDC_BUTTON_LINKTEST)->EnableWindow(true);

	if (bIsSuccess && this->socketISLinking) {
		this->OnTimerSocketLink();
	}
	else {
		this->socketLink.freeSocket();
	}
}

void CGCBTestToolDlg::OnTimerSocketLink()
{
	// 创建各个线程开始监听
	this->hThreadSocketLinkSend = CreateThread(NULL, 0, ThreadSocketLinkSend, &this->socketLink, 0, NULL);
	this->hThreadSocketLinkRecv = CreateThread(NULL, 0, ThreadSocketLinkRecv, &this->socketLink, 0, NULL);

	// 创建定时器，每隔一秒向GCB板子请求一次数据
	SetTimer(TIMER_SOCKET_LINK_SEND, TIMER_GAP, 0);

	// 创建定时器，检查是否有接收到消息，如果有则显示
	SetTimer(TIMER_SOCKET_LINK_RECV, TIMER_GAP, 0);
}

void CGCBTestToolDlg::ShowMessage(PROGRAM_STATE_CODE stateCode, PROGRAM_STATE_TYPE stateType)
{
	CString strStateMessage = GetStateCodeMessage(stateCode);
	switch (stateType)
	{
	case PROGRAM_STATE_ERROR:
		MessageBox(strStateMessage, _T("错误"), MB_ICONERROR | MB_OK);
		break;
	case PROGRAM_STATE_WARNING:
		MessageBox(strStateMessage, _T("警告"), MB_ICONQUESTION | MB_OK);
		break;
	case PROGRAM_STATE_TIP:
		MessageBox(strStateMessage, _T("消息"), MB_ICONINFORMATION | MB_OK);
		break;
	}
}

bool CGCBTestToolDlg::AddNewFrameTab(const int nIndex)
{
	this->m_FrameTabCtrl.InsertItem(this->nDialogLen + 1, GetLabel(nIndex));
	this->framePage[this->nDialogLen].SetFrameType(nIndex);
	this->framePage[this->nDialogLen].Create(IDD_DETAIL_DIALOG, &this->m_FrameTabCtrl);

	// 设定在Tab内显示的范围
	CRect rc;
	this->m_FrameTabCtrl.GetClientRect(rc);
	rc.bottom = rc.bottom - TOPBAR_SIZE;
	this->framePage[this->nDialogLen].MoveWindow(&rc);

	this->pDialog[this->nDialogLen + 1] = &this->framePage[this->nDialogLen];

	// Tab Control设置当前显示为新页面
	this->m_FrameTabCtrl.SetCurSel(this->nDialogLen + 1);
	this->pDialog[this->nCurSelTab]->ShowWindow(SW_HIDE);
	this->nCurSelTab = this->nDialogLen + 1;
	this->pDialog[this->nCurSelTab]->ShowWindow(SW_SHOW);

	this->nDialogLen++;

	// 设置滚动条位置
	this->m_DlgScrollBar.SetScrollPos(0);
	return true;
}

void CGCBTestToolDlg::SendRequestMessage()
{
	// 全部类型发送一遍请求数据

	// 获取组
	list<BYTE> sendMsgLst[LIST_NUM];
	sendMsgLst[0] = this->CreateMessage(NOZZLE_CARTRIDGE_LEVEL, 0x0001, 2);
	sendMsgLst[1] = this->CreateMessage(MODE_LOCKED_SOLENOID_VALVE_WORKING, 0x0001, 2);
	sendMsgLst[2] = this->CreateMessage(POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING, 0x0001, 2);
	sendMsgLst[3] = this->CreateMessage(INK_SUPPLY_PUMP_WORKING_CONDITION, 0x0001, 2);
	sendMsgLst[4] = this->CreateMessage(NOZZLE_CABINET_TEMPERATURE, 0x0001, 2);
	sendMsgLst[5] = this->CreateMessage(AIR_NEGATIVE_PRESSURE_VALUE, 0x0001, 2);
	sendMsgLst[6] = this->CreateMessage(AIR_POSITIVE_PRESSURE_VALUE, 0x0001, 2);

	// 临时设置为7个数
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		MessageBean tempMessageBean;
		tempMessageBean.SetOrginDataList(sendMsgLst[nIndex]);
		tempMessageBean.AnalysisOrginDataLst();
		GCBMainDlg::sendMessageQueue.Push_back(tempMessageBean);
	}
}
void CGCBTestToolDlg::OnBnClickedButtonSetting()
{
	this->settingPage.DoModal();
}
