
// RB_ComTest.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxsock.h"

#include "RB_ComTest.h"
#include "RB_ComTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRB_ComTestApp

BEGIN_MESSAGE_MAP(CRB_ComTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRB_ComTestApp 构造

CRB_ComTestApp::CRB_ComTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_strVersion = "RainBow Com Test V1.0.20180815";
	m_strAppPath = "";
	m_strIniFile = "Config.cfg";

	m_bDemoMode	 = FALSE;

}
CRB_ComTestApp::~CRB_ComTestApp()
{
	{
		CString strIniFile,strSection,strReg;

		strIniFile = theApp.m_strIniFile;
		strSection = "DEBUG";
		WRITE_PROFILE_INFO("%d", m_bDemoMode);
	}

}


// 唯一的一个 CRB_ComTestApp 对象

CRB_ComTestApp theApp;


// CRB_ComTestApp 初始化

BOOL CRB_ComTestApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox("Windows 套接字初始化失败。");
		return FALSE;
	}

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("RainBow ComTest"));

	{
		char	sPath[MAX_PATH+100];

		::GetModuleFileName(m_hInstance, sPath, _countof(sPath));

		char* pStr = ::strrchr(sPath, '\\');
		if (pStr != NULL)
		{
			*(pStr+1) = '\0';
		}
		else
		{
			ASSERT(0);
			strcat_s(sPath, MAX_PATH+100,"\\");
		}

		m_strAppPath	= sPath;
	}

	m_strIniFile = theApp.m_strAppPath + "Config.cfg";

	{
		char szReg[200];
		CString strIniFile,strSection;

		strIniFile = theApp.m_strIniFile;
		strSection = "DEBUG";
		GET_PROFILE_INFO_INT(m_bDemoMode, 0, m_bDemoMode, 1, BOOL);
	}

	CRB_ComTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
