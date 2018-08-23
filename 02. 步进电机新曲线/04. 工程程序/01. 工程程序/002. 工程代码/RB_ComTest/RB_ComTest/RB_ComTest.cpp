
// RB_ComTest.cpp : ����Ӧ�ó��������Ϊ��
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


// CRB_ComTestApp ����

CRB_ComTestApp::CRB_ComTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
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


// Ψһ��һ�� CRB_ComTestApp ����

CRB_ComTestApp theApp;


// CRB_ComTestApp ��ʼ��

BOOL CRB_ComTestApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox("Windows �׽��ֳ�ʼ��ʧ�ܡ�");
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
