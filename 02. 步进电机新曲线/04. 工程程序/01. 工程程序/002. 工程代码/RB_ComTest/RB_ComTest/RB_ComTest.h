
// RB_ComTest.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Include/rpBasexp.h"
#include "CmdTimeOut.h"

// CRB_ComTestApp:
// 有关此类的实现，请参阅 RB_ComTest.cpp
//
class CRB_ComTestApp : public CWinAppEx
{
public:
	CRB_ComTestApp();
	~CRB_ComTestApp();

	// 重写
public:
	virtual BOOL InitInstance();

	// 实现

	DECLARE_MESSAGE_MAP()

public:
	CString m_strAppPath;
	CString m_strVersion;

	CString m_strIniFile;

	BOOL	m_bDemoMode;
};

extern CRB_ComTestApp theApp;