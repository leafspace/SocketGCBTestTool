
// RB_ComTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Include/rpBasexp.h"
#include "CmdTimeOut.h"

// CRB_ComTestApp:
// �йش����ʵ�֣������ RB_ComTest.cpp
//
class CRB_ComTestApp : public CWinAppEx
{
public:
	CRB_ComTestApp();
	~CRB_ComTestApp();

	// ��д
public:
	virtual BOOL InitInstance();

	// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	CString m_strAppPath;
	CString m_strVersion;

	CString m_strIniFile;

	BOOL	m_bDemoMode;
};

extern CRB_ComTestApp theApp;