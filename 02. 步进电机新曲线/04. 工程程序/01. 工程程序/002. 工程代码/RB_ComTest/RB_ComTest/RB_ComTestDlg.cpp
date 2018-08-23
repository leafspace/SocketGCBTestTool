
// RB_ComTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RB_ComTest.h"
#include "RB_ComTestDlg.h"
#include "MBComPort/MBComPort.h"
//#include "SFBoardDlg.h"
#include "SFParameterDlg.h"
#include "ras/RasBoardDlg.h"

#include "FlatLaser/LDBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define _COM_NUM_MAX_		16

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
public:
	CString m_strVersion;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_strVersion = theApp.m_strVersion;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRB_ComTestDlg 对话框

CRB_ComTestDlg::CRB_ComTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRB_ComTestDlg::IDD, pParent)
	, m_nPortType(0)
	, m_dwIP(0)
	, m_nCom(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bConnect = FALSE;

	m_fReducer_x = 5;				//X减速机
	m_fDollyPerimeter = 180;		//小车周长
	m_fPulsesPerRevolution_x = 300;	//每圈脉冲

	m_fReducer_y = 45;					//Y减速机
	m_fPulsesPerRevolution_y = 1800;	//Y每圈脉冲
	m_fDeferentPerimeter = 640;			//Y导带周长
	m_fDeferentPitch = 10;				//Y丝杆螺距

	m_fReducer_z = 45;					//Z减速机
	m_fPulsesPerRevolution_z = 1800;	//Z每圈脉冲
	m_fDeferentPerimeter_z = 640;		//Z导带周长
	m_fDeferentPitch_z = 10;			//Z丝杆螺距

	BZERO(m_pDlgList);
	CCmdTimeOut::LoadFromCfg();

}

void CRB_ComTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMNUM, m_cbComNum);
	DDX_Control(pDX, IDC_EDIT_COMRECIVE, m_edtComRecive);
	DDX_Control(pDX, IDC_EDIT_COMSEND, m_edtComSend);
	DDX_Control(pDX, IDC_COMBO_RB_DLGTYPE, m_cbDlgType);
	DDX_Radio(pDX, IDC_RADIO_USECOM, m_nPortType);
	DDX_IPAddress(pDX, IDC_IPADDRESS_SOCKETPORT, m_dwIP);
	DDX_CBIndex(pDX, IDC_COMBO_COMNUM, m_nCom);
	DDX_Control(pDX, IDC_BUTTON_OPENCOM, m_openportCtrl);
	DDX_Control(pDX, IDC_STATIC_COMSTATE, m_infoCtrl);
}

BEGIN_MESSAGE_MAP(CRB_ComTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COMM_RXCHAR, &CRB_ComTestDlg::OnComRecive)
	ON_MESSAGE(WM_COMM_TXCHAR, &CRB_ComTestDlg::OnComSend)
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, &CRB_ComTestDlg::OnBnClickedButtonOpencom)
	ON_BN_CLICKED(IDC_BUTTON_OPENDLG, &CRB_ComTestDlg::OnBnClickedButtonOpendlg)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_EDIT, &CRB_ComTestDlg::OnBnClickedButtonClearEdit)
	ON_BN_CLICKED(IDOK, &CRB_ComTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRB_ComTestDlg 消息处理程序

BOOL CRB_ComTestDlg::OnInitDialog()
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

	//ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	int i;
	CString strTmp;

	GetConfigInfo();

	m_cbComNum.ResetContent();
	for (i=0; i< _COM_NUM_MAX_; i++)
	{
		strTmp.Format("COM%d", i+1);
		m_cbComNum.AddString(strTmp);
	}
	if (m_nCom < 0 || m_nCom >= _COM_NUM_MAX_)
		m_nCom = 0;
	m_cbComNum.SetCurSel(m_nCom);
	
	m_port.m_pComWnd = this;
	m_port.DemoMode(theApp.m_bDemoMode);

	m_cbDlgType.ResetContent();
	m_cbDlgType.AddString("伺服板 V4005");
	m_cbDlgType.AddString("三轴伺服板 V1001");
	m_cbDlgType.AddString("三轴伺服板 V1001 (新S曲线)");
	m_cbDlgType.AddString("光栅板 - 数码");
	m_cbDlgType.AddString("光栅板 - 平网蓝光");

	m_cbDlgType.AddString("伺服固件参数");
	m_cbDlgType.AddString("三轴伺服板固件参数 V1001");


	m_cbDlgType.SetCurSel(functype_servo_4005);
	
#ifdef _CONFIG_CLIENT_
	CRect rectLarge;
	CRect rectSmall;
	CRect rectSeparator;

	GetWindowRect(&rectLarge);
	GetDlgItem(IDC_EDIT_COMRECIVE)->GetWindowRect(&rectSeparator);

	rectSmall.left	= rectLarge.left;
	rectSmall.top	= rectLarge.top;
	rectSmall.right	= rectSeparator.left;
	rectSmall.bottom= rectLarge.bottom;

	SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),SWP_NOMOVE | SWP_NOZORDER);

	m_cbDlgType.SetCurSel(functype_raser_flatlaser);
#endif

	SetWindowText(theApp.m_strVersion);

	EnableControl();

	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRB_ComTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRB_ComTestDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRB_ComTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRB_ComTestDlg::OnBnClickedButtonOpencom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bConnect==FALSE)
	{
		switch(m_nPortType)
		{
		default:
		case 0:
			//com
			if (m_port.Open(m_nCom+1,9600,4096,1024))
			{
				m_port.m_pComWnd = this;
				m_bConnect = TRUE;
			}
			else
			{
				AfxMessageBox("打开端口失败!");
			}
			break;
		case 1:
			//tcp
			BYTE a,b,c,d;
			CString strIP, strError;
			a = HIBYTE(HIWORD(m_dwIP));
			b = LOBYTE(HIWORD(m_dwIP));
			c = HIBYTE(LOWORD(m_dwIP));
			d = LOBYTE(LOWORD(m_dwIP));
			strIP.Format("%d.%d.%d.%d", a,b,c,d);
			if (m_port.socket_Open(strIP, 2048, 1024, 1000, &strError))
			{
				m_port.m_pComWnd = this;
				m_bConnect = TRUE;
			}
			else
			{
				if (strError.IsEmpty())
					strError = "Failed to Open TCP Port!";
				AnxDialogMsg(strError);
			}
			break;
		}
	}
	else
	{
		if (m_port.Close(TRUE))
		{
			m_bConnect = FALSE;
		}
		else
		{
			AfxMessageBox("关闭端口失败!");
		}
	}
	EnableControl();

}

void CRB_ComTestDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::PostNcDestroy();
}


void CRB_ComTestDlg::OnBnClickedButtonOpendlg()
{
	// TODO: 在此添加控件通知处理程序代码
	int		nSel = m_cbDlgType.GetCurSel();
	ULONG64 ulTimeStart, ulTimeout = 20000;

#ifdef _CONFIG_CLIENT_
	nSel = functype_raser_flatlaser;		//光栅板 - 平网蓝光
#endif

	switch(nSel)
	{
	case functype_servo_4005:
		{//伺服板 V4005
			CSFBoardThread		*pSFBoardhread = NULL;
			CSFBoardDlg			**ppDlg = NULL;

			ppDlg = (CSFBoardDlg**)(&m_pDlgList[nSel]);
			ASSERT(ppDlg != NULL);
			if (ppDlg != NULL && (*ppDlg) != NULL && ::IsWindow((*ppDlg)->m_hWnd))
			{
				if ((*ppDlg)->m_bThreeAxisServo)
				{
					AfxMessageBox("已打开三轴伺服配置模式，需先关闭！");
				}
				(*ppDlg)->ShowWindow(SW_SHOW);
				break;
			}

			pSFBoardhread = (CSFBoardThread*)AfxBeginThread(RUNTIME_CLASS(CSFBoardThread),THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
			if (pSFBoardhread==NULL)
			{
				AfxMessageBox("Failed to create thread.");
				break;
			}
			pSFBoardhread->m_pfReducer_x				= &m_fReducer_x;
			pSFBoardhread->m_pfPulsesPerRevolution_x	= &m_fPulsesPerRevolution_x;
			pSFBoardhread->m_pfDollyPerimeter			= &m_fDollyPerimeter;

			pSFBoardhread->m_pfReducer_y				= &m_fReducer_y;
			pSFBoardhread->m_pfPulsesPerRevolution_y	= &m_fPulsesPerRevolution_y;	
			pSFBoardhread->m_pfDeferentPerimeter		= &m_fDeferentPerimeter;
			pSFBoardhread->m_pfDeferentPitch			= &m_fDeferentPitch;

			pSFBoardhread->m_pfReducer_z				= &m_fReducer_z;
			pSFBoardhread->m_pfPulsesPerRevolution_z	= &m_fPulsesPerRevolution_z;	
			pSFBoardhread->m_pfDeferentPerimeter_z		= &m_fDeferentPerimeter_z;
			pSFBoardhread->m_pfDeferentPitch_z			= &m_fDeferentPitch_z;

			pSFBoardhread->m_pComPort					= &m_port;
			pSFBoardhread->m_bThreeAxisServo			= FALSE;

			pSFBoardhread->m_ppDlg						= ppDlg;
			pSFBoardhread->bIsNewCurve					= false;

			ulTimeStart = ::GetTickCount64();
			pSFBoardhread->ResumeThread();
			while (::GetTickCount64() < (ulTimeStart + ulTimeout))
			{
				if ((*ppDlg) != NULL)
				{
					if (::IsWindow((*ppDlg)->m_hWnd))
					{
						break;
					}
				}
			}
			ASSERT((CWnd**)ppDlg == &(m_pDlgList[nSel]));
			if (m_pDlgList[nSel]==NULL || m_pDlgList[nSel]->m_hWnd==NULL)
			{
				AfxMessageBox("Failed to open dialog.");
				break;
			}

			break;
		}
	case functype_raser:
		{//光栅板 - 数码
			CRasBoardThread		*pRasThread;
			CRasBoardDlg		**ppDlg = NULL;

			ppDlg = (CRasBoardDlg**)&(m_pDlgList[nSel]);
			ASSERT(ppDlg != NULL);
			if (ppDlg != NULL && (*ppDlg) != NULL && ::IsWindow((*ppDlg)->m_hWnd))
			{
				(*ppDlg)->ShowWindow(SW_SHOW);
				break;
			}

			pRasThread = (CRasBoardThread*)AfxBeginThread(RUNTIME_CLASS(CRasBoardThread),THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
			if (pRasThread==NULL)
			{
				AfxMessageBox("Failed to create thread.");
				break;
			}

			pRasThread->m_pfReducer_x				= &m_fReducer_x;
			pRasThread->m_pfPulsesPerRevolution_x	= &m_fPulsesPerRevolution_x;
			pRasThread->m_pfDollyPerimeter			= &m_fDollyPerimeter;

			pRasThread->m_pfReducer_y				= &m_fReducer_y;
			pRasThread->m_pfPulsesPerRevolution_y	= &m_fPulsesPerRevolution_y;	
			pRasThread->m_pfDeferentPerimeter		= &m_fDeferentPerimeter;
			pRasThread->m_pfDeferentPitch			= &m_fDeferentPitch;

			pRasThread->m_pfReducer_z				= &m_fReducer_z;
			pRasThread->m_pfPulsesPerRevolution_z	= &m_fPulsesPerRevolution_z;	
			pRasThread->m_pfDeferentPerimeter_z		= &m_fDeferentPerimeter_z;
			pRasThread->m_pfDeferentPitch_z			= &m_fDeferentPitch_z;

			pRasThread->m_pComPort					= &m_port;
			pRasThread->m_ppDlg						= ppDlg;

			ulTimeStart = ::GetTickCount64();
			pRasThread->ResumeThread();
			while (::GetTickCount64() < (ulTimeStart + ulTimeout))
			{
				if ((*ppDlg) != NULL)
				{
					if (::IsWindow((*ppDlg)->m_hWnd))
					{
						break;
					}
				}
			}
			ASSERT((CWnd**)ppDlg == &(m_pDlgList[nSel]));
			if (m_pDlgList[nSel]==NULL || m_pDlgList[nSel]->m_hWnd==NULL)
			{
				AfxMessageBox("Failed to open dialog.");
				break;
			}


			break;
		}
	case functype_servo_param:
		{//伺服固件参数
			CSFParameterDlg		**ppDlg = NULL;

			ppDlg = (CSFParameterDlg**)(&m_pDlgList[nSel]);
			ASSERT(ppDlg != NULL);
			if (ppDlg != NULL && (*ppDlg) != NULL && ::IsWindow((*ppDlg)->m_hWnd))
			{
				(*ppDlg)->ShowWindow(SW_SHOW);
				break;
			}
			(*ppDlg) = new CSFParameterDlg();
			if ((*ppDlg) == NULL)
			{
				AfxMessageBox("Out of memory!");
				break;
			}

			(*ppDlg)->m_pfReducer_x				= &m_fReducer_x;
			(*ppDlg)->m_pfPulsesPerRevolution_x	= &m_fPulsesPerRevolution_x;
			(*ppDlg)->m_pfDollyPerimeter		= &m_fDollyPerimeter;

			(*ppDlg)->m_pfReducer_y				= &m_fReducer_y;
			(*ppDlg)->m_pfPulsesPerRevolution_y	= &m_fPulsesPerRevolution_y;	
			(*ppDlg)->m_pfDeferentPerimeter		= &m_fDeferentPerimeter;
			(*ppDlg)->m_pfDeferentPitch			= &m_fDeferentPitch;

			(*ppDlg)->m_pfReducer_z				= &m_fReducer_z;
			(*ppDlg)->m_pfPulsesPerRevolution_z	= &m_fPulsesPerRevolution_z;	
			(*ppDlg)->m_pfDeferentPerimeter_z	= &m_fDeferentPerimeter_z;
			(*ppDlg)->m_pfDeferentPitch_z		= &m_fDeferentPitch_z;


			(*ppDlg)->m_pComPort				= &m_port;
			(*ppDlg)->m_bThreeAxisServo			= FALSE;

			(*ppDlg)->Create(IDD_DIALOG_SFPARAMATER, this);
			(*ppDlg)->ShowWindow(SW_SHOW);

			ASSERT((CWnd**)ppDlg == &(m_pDlgList[nSel]));

			break;
		}
	case functype_servo_3axis_1001:
	case functype_servo_3axis_1001_new:
		{//三轴伺服板 V1001
			CSFBoardThread		*pSFBoardhread = NULL;
			CSFBoardDlg			**ppDlg = NULL;

			ppDlg = (CSFBoardDlg**)(&m_pDlgList[nSel]);
			ASSERT(ppDlg != NULL);
			if (ppDlg != NULL && (*ppDlg) != NULL && ::IsWindow((*ppDlg)->m_hWnd))
			{
				if (!(*ppDlg)->m_bThreeAxisServo)
				{
					AfxMessageBox("已打开单轴伺服配置模式，需先关闭！");
				}
				(*ppDlg)->ShowWindow(SW_SHOW);
				break;
			}

			pSFBoardhread = (CSFBoardThread*)AfxBeginThread(RUNTIME_CLASS(CSFBoardThread),THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
			if (pSFBoardhread==NULL)
			{
				AfxMessageBox("Failed to create thread.");
				break;
			}

			//pSFBoardhread->m_bAutoDelete = FALSE;
			pSFBoardhread->m_pfReducer_x				= &m_fReducer_x;
			pSFBoardhread->m_pfPulsesPerRevolution_x	= &m_fPulsesPerRevolution_x;
			pSFBoardhread->m_pfDollyPerimeter			= &m_fDollyPerimeter;

			pSFBoardhread->m_pfReducer_y				= &m_fReducer_y;
			pSFBoardhread->m_pfPulsesPerRevolution_y	= &m_fPulsesPerRevolution_y;	
			pSFBoardhread->m_pfDeferentPerimeter		= &m_fDeferentPerimeter;
			pSFBoardhread->m_pfDeferentPitch			= &m_fDeferentPitch;

			pSFBoardhread->m_pfReducer_z				= &m_fReducer_z;
			pSFBoardhread->m_pfPulsesPerRevolution_z	= &m_fPulsesPerRevolution_z;	
			pSFBoardhread->m_pfDeferentPerimeter_z		= &m_fDeferentPerimeter_z;
			pSFBoardhread->m_pfDeferentPitch_z			= &m_fDeferentPitch_z;

			pSFBoardhread->m_pComPort					= &m_port;
			pSFBoardhread->m_bThreeAxisServo			= TRUE;

			pSFBoardhread->m_ppDlg						= ppDlg;
			pSFBoardhread->bIsNewCurve					= nSel == functype_servo_3axis_1001_new;

			ulTimeStart = ::GetTickCount64();
			pSFBoardhread->ResumeThread();
			while (::GetTickCount64() < (ulTimeStart + ulTimeout))
			{
				if ((*ppDlg) != NULL)
				{
					if (::IsWindow((*ppDlg)->m_hWnd))
					{
						break;
					}
				}
			}
			ASSERT((CWnd**)ppDlg == &(m_pDlgList[nSel]));
			if (m_pDlgList[nSel]==NULL || m_pDlgList[nSel]->m_hWnd==NULL)
			{
				AfxMessageBox("Failed to open dialog.");
				break;
			}

			break;
		}
	case functype_servo_3axis_1001_param:
		{//三轴伺服板固件参数 V1001
			CSFParameterDlg		**ppDlg = NULL;

			ppDlg = (CSFParameterDlg**)(&m_pDlgList[nSel]);
			ASSERT(ppDlg != NULL);
			if (ppDlg != NULL && (*ppDlg) != NULL && ::IsWindow((*ppDlg)->m_hWnd))
			{
				(*ppDlg)->ShowWindow(SW_SHOW);
				break;
			}
			(*ppDlg) = new CSFParameterDlg();
			if ((*ppDlg) == NULL)
			{
				AfxMessageBox("Out of memory!");
				break;
			}

			(*ppDlg)->m_pfReducer_x				= &m_fReducer_x;
			(*ppDlg)->m_pfPulsesPerRevolution_x	= &m_fPulsesPerRevolution_x;
			(*ppDlg)->m_pfDollyPerimeter		= &m_fDollyPerimeter;

			(*ppDlg)->m_pfReducer_y				= &m_fReducer_y;
			(*ppDlg)->m_pfPulsesPerRevolution_y	= &m_fPulsesPerRevolution_y;	
			(*ppDlg)->m_pfDeferentPerimeter		= &m_fDeferentPerimeter;
			(*ppDlg)->m_pfDeferentPitch			= &m_fDeferentPitch;

			(*ppDlg)->m_pfReducer_z				= &m_fReducer_z;
			(*ppDlg)->m_pfPulsesPerRevolution_z	= &m_fPulsesPerRevolution_z;	
			(*ppDlg)->m_pfDeferentPerimeter_z	= &m_fDeferentPerimeter_z;
			(*ppDlg)->m_pfDeferentPitch_z		= &m_fDeferentPitch_z;

			(*ppDlg)->m_pComPort				= &m_port;
			(*ppDlg)->m_bThreeAxisServo			= TRUE;

			(*ppDlg)->Create(IDD_DIALOG_SFPARAMATER, this);
			(*ppDlg)->ShowWindow(SW_SHOW);

			ASSERT((CWnd**)ppDlg == &(m_pDlgList[nSel]));

			break;
		}
	case functype_raser_flatlaser:
		{//光栅板 - 平网蓝光
  			CLDBoardThread		*pLDThread;
  			CLDBoardDlg			**ppDlg = NULL;
  
  			ppDlg = (CLDBoardDlg**)&(m_pDlgList[nSel]);
  			ASSERT(ppDlg != NULL);
  			if (ppDlg != NULL && (*ppDlg) != NULL && ::IsWindow((*ppDlg)->m_hWnd))
  			{
  				(*ppDlg)->ShowWindow(SW_SHOW);
  				break;
  			}
  
  			pLDThread = (CLDBoardThread*)AfxBeginThread(RUNTIME_CLASS(CLDBoardThread),THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
  			if (pLDThread==NULL)
  			{
  				AfxMessageBox("Failed to create thread.");
  				break;
  			}
   
  			pLDThread->m_pComPort		= &m_port;
  			pLDThread->m_ppDlg			= ppDlg;
  
  			ulTimeStart = ::GetTickCount64();
  			pLDThread->ResumeThread();
  			while (::GetTickCount64() < (ulTimeStart + ulTimeout))
  			{
  				if ((*ppDlg) != NULL)
  				{
  					if (::IsWindow((*ppDlg)->m_hWnd))
  					{
  						break;
  					}
  				}
  			}
  			ASSERT((CWnd**)ppDlg == &(m_pDlgList[nSel]));
  			if (m_pDlgList[nSel]==NULL || m_pDlgList[nSel]->m_hWnd==NULL)
  			{
  				AfxMessageBox("Failed to open dialog.");
  				break;
  			}
			break;
		}
	}
}

void CRB_ComTestDlg::OnBnClickedButtonClearEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	m_edtComRecive.SetWindowText("");
	m_edtComSend.SetWindowText("");
}

LONG CRB_ComTestDlg::OnComRecive(WPARAM wPALAM, LPARAM lPALAM)
{
	CString* str = (CString*)wPALAM;
	m_edtComRecive.SetSel(-1,-1);
	m_edtComRecive.ReplaceSel(*str);
	return 0;
}

LONG CRB_ComTestDlg::OnComSend(WPARAM wPALAM, LPARAM lPALAM)
{
	CString* str = (CString*)wPALAM;
	m_edtComSend.SetSel(-1,-1);
	m_edtComSend.ReplaceSel(*str);
	return 0;
}
void CRB_ComTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	SetConfigInfo();

	OnOK();
}

void CRB_ComTestDlg::SetConfigInfo(void)
{
	CString strIniFile,strSection,strReg;

	strIniFile = theApp.m_strIniFile;
	
	strSection = "PORT";
	WRITE_PROFILE_INFO("%d", m_nCom);
	WRITE_PROFILE_INFO("%d", m_nPortType);
	WRITE_PROFILE_INFO("%ul", m_dwIP);

	strSection = "MOTOR_X";
	WRITE_PROFILE_INFO("%f", m_fReducer_x);
	WRITE_PROFILE_INFO("%f", m_fDollyPerimeter);
	WRITE_PROFILE_INFO("%f", m_fPulsesPerRevolution_x);

	strSection = "MOTOR_Y";
	WRITE_PROFILE_INFO("%f", m_fReducer_y);
	WRITE_PROFILE_INFO("%f", m_fPulsesPerRevolution_y);
	WRITE_PROFILE_INFO("%f", m_fDeferentPerimeter);
	WRITE_PROFILE_INFO("%f", m_fDeferentPitch);

	strSection = "MOTOR_Z";
	WRITE_PROFILE_INFO("%f", m_fReducer_z);
	WRITE_PROFILE_INFO("%f", m_fPulsesPerRevolution_z);
	WRITE_PROFILE_INFO("%f", m_fDeferentPerimeter_z);
	WRITE_PROFILE_INFO("%f", m_fDeferentPitch_z);

}

void CRB_ComTestDlg::GetConfigInfo(void)
{
	char szReg[200];
	CString strIniFile,strSection;

	strIniFile = theApp.m_strIniFile;

	strSection = "PORT";
	GET_PROFILE_INFO_INT(m_nCom, 0, 0, _COM_NUM_MAX_, int);
	GET_PROFILE_INFO_INT(m_nPortType, 0, 0, 1, int);
	GET_PROFILE_INFO_UINT(m_dwIP, 0, m_dwIP, 0xFFFFFFFF);

	strSection = "MOTOR_X";
	GET_PROFILE_INFO_FLOAT(m_fReducer_x, 0.01, m_fReducer_x, 10000.0);
	GET_PROFILE_INFO_FLOAT(m_fDollyPerimeter, 1, m_fDollyPerimeter, 10000.0);
	GET_PROFILE_INFO_FLOAT(m_fPulsesPerRevolution_x, 1, m_fPulsesPerRevolution_x, 1000000000);

	strSection = "MOTOR_Y";
	GET_PROFILE_INFO_FLOAT(m_fReducer_y, 0.01, m_fReducer_y, 10000.0);
	GET_PROFILE_INFO_FLOAT(m_fPulsesPerRevolution_y, 1, m_fPulsesPerRevolution_y, 1000000000);
	GET_PROFILE_INFO_FLOAT(m_fDeferentPerimeter, 1, m_fDeferentPerimeter, 10000.0);
	GET_PROFILE_INFO_FLOAT(m_fDeferentPitch, 0.01, m_fDeferentPitch, 10000.0);

	strSection = "MOTOR_Z";
	GET_PROFILE_INFO_FLOAT(m_fReducer_z, 0.01, m_fReducer_z, 10000.0);
	GET_PROFILE_INFO_FLOAT(m_fPulsesPerRevolution_z, 1, m_fPulsesPerRevolution_z, 1000000000);
	GET_PROFILE_INFO_FLOAT(m_fDeferentPerimeter_z, 1, m_fDeferentPerimeter_z, 10000.0);
	GET_PROFILE_INFO_FLOAT(m_fDeferentPitch_z, 0.01, m_fDeferentPitch_z, 10000.0);

}

void CRB_ComTestDlg::EnableControl(void)
{
	m_cbComNum.EnableWindow(!m_bConnect);
	m_cbDlgType.EnableWindow(m_bConnect);
	
	ENABLECONTROL(IDC_RADIO_USECOM, !m_bConnect);
	ENABLECONTROL(IDC_RADIO_USETCP, !m_bConnect);
	ENABLECONTROL(IDC_IPADDRESS_SOCKETPORT, !m_bConnect);
	ENABLECONTROL(IDC_BUTTON_OPENDLG, m_bConnect);

	m_openportCtrl.SetWindowText(m_bConnect ? "关闭端口" : "打开端口");
	m_infoCtrl.SetWindowText(m_bConnect ? "已连接" : "未连接");


#ifdef _CONFIG_CLIENT_
	ENABLECONTROL(IDC_COMBO_RB_DLGTYPE, FALSE);
#endif

}

BOOL CRB_ComTestDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CCmdTimeOut::SaveToCfg();

	int i;
	for (i=0; i<functype_count; i++)
	{
		if (m_pDlgList[i] != NULL)
		{
			m_pDlgList[i]->DestroyWindow();
			m_pDlgList[i] = NULL;
		}
	}

	return CDialog::DestroyWindow();
}
