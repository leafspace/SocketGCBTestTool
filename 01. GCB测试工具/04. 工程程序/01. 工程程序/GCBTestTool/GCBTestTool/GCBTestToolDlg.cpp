#include "stdafx.h"
#include "GCBTestToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
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

//=============================================================================

CGCBTestToolDlg::CGCBTestToolDlg(CWnd* pParent)
	: CDialog(CGCBTestToolDlg::IDD, pParent)
{
	this->m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);                   // 加载图标
}

void CGCBTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRAMETAB, m_FrameTabCtrl);
	DDX_Control(pDX, IDC_SCROLLBAR, m_DlgScrollBar);
}

BEGIN_MESSAGE_MAP(CGCBTestToolDlg, CDialog)
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEWHEEL()
	ON_WM_SYSCOMMAND()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_FRAMETAB, &CGCBTestToolDlg::OnTcnSelchangeFrametab)
	ON_BN_CLICKED(IDC_BUTTON_LINK, &CGCBTestToolDlg::OnBnClickedButtonLink)
	ON_BN_CLICKED(IDC_BUTTON_LINKTEST, &CGCBTestToolDlg::OnBnClickedButtonLinktest)
	ON_BN_CLICKED(IDC_BUTTON_GCB_SETTING, &CGCBTestToolDlg::OnBnClickedButtonGcbSetting)
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM_SETTING, &CGCBTestToolDlg::OnBnClickedButtonSystemSetting)
	ON_BN_CLICKED(IDC_BUTTON_CLEARDATA, &CGCBTestToolDlg::OnBnClickedButtonCleardata)
END_MESSAGE_MAP()

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

//当用户拖动最小化窗口时系统调用此函数取得光标显示
HCURSOR CGCBTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGCBTestToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
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
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 窗口初始化代码	
	this->nCurSelTab = 0;
	this->nDialogLen = 0;

	this->socketISLinking = false;

	// Tab Control 添加主页面
	this->m_FrameTabCtrl.InsertItem(0, LABLE_PREVIEW);

	this->mainPage.Create(IDD_MAIN_DIALOG, &this->m_FrameTabCtrl);
	// 设定页面在Tab内显示的范围
	CRect cRect;
	this->m_FrameTabCtrl.GetClientRect(cRect);
	cRect.bottom -= 20;
	this->mainPage.MoveWindow(&cRect);

	this->pDialog[0] = &this->mainPage;
	this->pDialog[0]->ShowWindow(SW_SHOW);

	// 滑块移动的位置为0——1000
	this->m_DlgScrollBar.SetScrollRange(SCROLLBAR_MIN, SCROLLBAR_MAX_MAIN);

	// 设置输入框的默认参数
	SetDlgItemText(IDC_GCB_EDIT_IPADDRESS, GCB_IP_ADDRESS);
	SetDlgItemText(IDC_GCB_EDIT_PORT, GCB_PORT);
	return TRUE;
}

//  如果向对话框添加最小化按钮，则需要下面的代码
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
	case TIMER_SOCKET_LINK_CONN:                                            // 检测Socket的连接状态是否成功
	{
		this->OnTimerSocketLinkTest();
	}
	break;
	case TIMER_SOCKET_LINK_RECV:                                            // 处理接收到的数据并显示在页面上
	{
		this->mainPage.RefreshPage(&this->communicationCore);
	}
	break;
	case TIMER_SOCKET_LINK_SEND:                                            // 向GCB请求一次数据
	{
		this->communicationCore.SendRequestMessage();
	}
	break;
	default:                                                                // 异常处理
		KillTimer(nIDEvent);
		break;
	}
}

BOOL CGCBTestToolDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta < 0) {
		OnVScroll(SB_LINEDOWN, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
	}
	else if (zDelta > 0) {
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

	// 滑轮滚动超过最大值
	int nScrollBarMax = this->nCurSelTab == 0 ? SCROLLBAR_MAX_MAIN : SCROLLBAR_MAX_DETA;
	if (nCurpos > nScrollBarMax) {
		nCurpos = nScrollBarMax;
	}
	else if (nCurpos < SCROLLBAR_MIN) {
		nCurpos = SCROLLBAR_MIN;
	}

	// 设置滚动条位置
	this->m_DlgScrollBar.SetScrollPos(nCurpos);

	// 设置子窗口移动位置
	CRect cRect;
	this->m_FrameTabCtrl.GetClientRect(cRect);
	cRect.top = cRect.top - nCurpos;
	cRect.bottom = cRect.bottom - TOPBAR_SIZE;

	// 根据当前Control Tab显示的页面来移动窗口
	this->pDialog[this->nCurSelTab]->MoveWindow(&cRect);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CGCBTestToolDlg::OnTcnSelchangeFrametab(NMHDR *pNMHDR, LRESULT *pResult)
{
	this->ChangeTabCtrl(this->m_FrameTabCtrl.GetCurSel());

	// 重置页面显示位置
	CRect cRect;
	this->m_FrameTabCtrl.GetClientRect(cRect);
	cRect.bottom = cRect.bottom - TOPBAR_SIZE;
	this->pDialog[this->nCurSelTab]->MoveWindow(&cRect);

	// 设置滚动条位置
	this->m_DlgScrollBar.SetScrollPos(0);
	*pResult = 0;
}

void CGCBTestToolDlg::OnBnClickedButtonLinktest()
{
	bool bIsSuccess = true;

	// 获取连接的测试IP与端口号
	CString strServerIP;
	CString strServerPort;
	GetDlgItemText(IDC_GCB_EDIT_IPADDRESS, strServerIP);
	GetDlgItemText(IDC_GCB_EDIT_PORT, strServerPort);

	GetDlgItem(IDC_BUTTON_LINKTEST)->EnableWindow(false);

	do {
		// 初始化Socket
		bIsSuccess = this->communicationCore.InisSocketLink(strServerIP, _ttoi(strServerPort));
		if (bIsSuccess == false) {
			break;
		}
	} while (false);

	if (bIsSuccess) {
		// 创建线程用于网络连接测试
		this->communicationCore.CreateSocketLinkTestThread();

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
	if (this->socketISLinking) {                                            // 如果当前处于正在连接的状态
		this->socketISLinking = false;
		SetDlgItemText(IDC_BUTTON_LINK, LABLE_LINK);
		KillTimer(TIMER_SOCKET_LINK_CONN);
		KillTimer(TIMER_SOCKET_LINK_RECV);
		KillTimer(TIMER_SOCKET_LINK_SEND);
		this->communicationCore.CloseAllThread();
		this->communicationCore.CloseSocketLink();
		this->mainPage.DeleteTimer(TIMER_DIALOG_DRAW);
		GetDlgItem(IDC_BUTTON_LINKTEST)->EnableWindow(true);
	}
	else {
		this->socketISLinking = true;
		SetDlgItemText(IDC_BUTTON_LINK, LABLE_STOPLINK);
		this->ClearAllData();
		this->mainPage.CreateTimer(TIMER_DIALOG_DRAW, DRAW_GAP);
		this->OnBnClickedButtonLinktest();
	}
}

void CGCBTestToolDlg::OnBnClickedButtonGcbSetting()
{
	if (this->socketISLinking) {
		this->mainPage.ShowSettingDialog();
	}
	else {
		this->ShowMessage(PROGRAM_UNLIKING, PROGRAM_STATE_ERROR);
	}
}

void CGCBTestToolDlg::OnBnClickedButtonSystemSetting()
{
	this->systemSettingPage.DoModal();
}

void CGCBTestToolDlg::OnBnClickedButtonCleardata()
{
	this->ClearAllData();
}


//=============================================================================

void CGCBTestToolDlg::OnTimerSocketLinkTest()
{
	bool bIsSuccess = true;
	// 检查线程是否结束
	// 获取线程状态
	int retState = this->communicationCore.GetSocketLinkTestThreadState();
	switch (retState)
	{
	case TIMER_STATE_UNRUNNING:
	case TIMER_STATE_RUNNING:
		return;
	case TIMER_STATE_ERROR:
		return;
	case TIMER_STATE_FINISH:
	{
		KillTimer(TIMER_SOCKET_LINK_CONN);                              // 取消定时器
		int retValue = this->communicationCore.GetSocketLinkTestThreadRetValue();
		bIsSuccess = (retValue > 0) && (retValue != TIMER_STATE_ERROR);
		this->communicationCore.RemoveSocketLinkTestThreadSteAndVlu();
	}
	break;
	default:
		return;
	}

	if (bIsSuccess) {                                                       // 板卡能够接通
		this->ShowMessage(PROGRAM_LINK_SERVER, PROGRAM_STATE_TIP);
	}
	else {
		this->ShowMessage(PROGRAM_CANT_LINK_SERVER, PROGRAM_STATE_ERROR);
		if (this->socketISLinking) {                                        // 不能接通，且这个检测是由“连接”按钮触发的
			SetDlgItemText(IDC_BUTTON_LINK, LABLE_LINK);
			this->socketISLinking = false;
		}
	}

	if (bIsSuccess && this->socketISLinking) {
		this->OnTimerSocketLink();
	}
	else {
		// 如果结束了就设置按钮可以使用且输出状态
		GetDlgItem(IDC_BUTTON_LINKTEST)->EnableWindow(true);
		this->communicationCore.CloseSocketLink();
	}
}

void CGCBTestToolDlg::OnTimerSocketLink()
{
	// 创建各个线程开始监听
	this->communicationCore.CreateSocketLinkRecvThread();
	this->communicationCore.CreateSocketLinkSendThread();

	// 创建定时器，每隔一秒向GCB板子请求一次数据
	SetTimer(TIMER_SOCKET_LINK_SEND, TIMER_GAP, 0);

	// 创建定时器，检查是否有接收到消息，如果有则显示
	SetTimer(TIMER_SOCKET_LINK_RECV, TIMER_GAP, 0);
}

void CGCBTestToolDlg::ClearAllData(void)
{
	this->communicationCore.ClearQueueData();
	this->mainPage.ClearAllData();
}

//=============================================================================

CTabCtrl* CGCBTestToolDlg::GetTabCtrl(void)
{
	return &this->m_FrameTabCtrl;
}

CommunicateCore* CGCBTestToolDlg::GetCommunicateCore(void)
{
	return &this->communicationCore;
}

void CGCBTestToolDlg::ShowMessage(PROGRAM_STATE_CODE stateCode, PROGRAM_STATE_TYPE stateType)
{
	CString strStateMessage = GetStateCodeMessage(stateCode);
	switch (stateType)
	{
	case PROGRAM_STATE_ERROR:
		MessageBox(strStateMessage, LABLE_ERROR, MB_ICONERROR | MB_OK);
		break;
	case PROGRAM_STATE_WARNING:
		MessageBox(strStateMessage, LABLE_WARNING, MB_ICONQUESTION | MB_OK);
		break;
	case PROGRAM_STATE_TIP:
		MessageBox(strStateMessage, LABLE_TIP, MB_ICONINFORMATION | MB_OK);
		break;
	}
}

bool CGCBTestToolDlg::AddNewFrameTab(CString strLabel)
{
	this->m_FrameTabCtrl.InsertItem(this->nDialogLen + 1, strLabel);
	return true;
}

bool CGCBTestToolDlg::AddNewFrameTab(const GCBDetailFrameDlg *tabPage)
{
	this->nDialogLen++;
	this->pDialog[this->nDialogLen] = (CDialog*)tabPage;

	// Tab Control设置当前显示为新页面
	this->ChangeTabCtrl(this->nDialogLen);

	// 设置滚动条位置
	this->m_DlgScrollBar.SetScrollPos(0);
	return true;
}

bool CGCBTestToolDlg::ChangeTabCtrl(const int newIndex)
{
	this->m_FrameTabCtrl.SetCurSel(newIndex);
	this->pDialog[this->nCurSelTab]->ShowWindow(SW_HIDE);
	this->nCurSelTab = newIndex;
	this->pDialog[this->nCurSelTab]->ShowWindow(SW_SHOW);

	int nScrollBarMax = this->nCurSelTab == 0 ? SCROLLBAR_MAX_MAIN : SCROLLBAR_MAX_DETA;
	this->m_DlgScrollBar.SetScrollRange(SCROLLBAR_MIN, nScrollBarMax);
	return true;
}

bool CGCBTestToolDlg::ChangeTabCtrl(const GCBDetailFrameDlg *tabPage)
{
	for (int nIndex = 0; nIndex <= this->nDialogLen; ++nIndex) {
		if (this->pDialog[nIndex]->GetSafeHwnd() == tabPage->GetSafeHwnd()) {
			this->ChangeTabCtrl(nIndex);
			break;
		}
	}
	return true;
}

void CGCBTestToolDlg::SetMainPageListHeadNum(int nIndex, int nNum)
{
	this->mainPage.SetListHeadNum(nIndex, nNum);
}