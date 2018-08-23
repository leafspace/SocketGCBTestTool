// RasBoardThread.cpp : 实现文件
//

#include "../stdafx.h"
#include "../RB_ComTest.h"
#include "RasBoardThread.h"


// CRasBoardThread

IMPLEMENT_DYNCREATE(CRasBoardThread, CWinThread)

CRasBoardThread::CRasBoardThread()
{
	m_ppDlg = NULL;
}

CRasBoardThread::~CRasBoardThread()
{
	if (m_ppDlg != NULL && (*m_ppDlg) != NULL)
	{
		if (IsWindow((*m_ppDlg)->m_hWnd))
		{
			(*m_ppDlg)->DestroyWindow();
		}
		*m_ppDlg = NULL;
	}
}

BOOL CRasBoardThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	CRasBoardDlg	*pDlg = NULL;

	if (m_ppDlg == NULL)
		return FALSE;

	pDlg = *m_ppDlg;
	if (pDlg == NULL)
		pDlg = new CRasBoardDlg();
	if (pDlg==NULL)
	{
		AfxMessageBox("out of memory!");
		return FALSE;
	}
	*m_ppDlg = pDlg;

	pDlg->m_pParentThread = this;

	pDlg->m_pfReducer_x				= m_pfReducer_x;
	pDlg->m_pfReducer_y				= m_pfReducer_y;
	pDlg->m_pfPulsesPerRevolution_x = m_pfPulsesPerRevolution_x;
	pDlg->m_pfPulsesPerRevolution_y = m_pfPulsesPerRevolution_y;
	pDlg->m_pfDollyPerimeter		= m_pfDollyPerimeter;
	pDlg->m_pfDeferentPerimeter		= m_pfDeferentPerimeter;
	pDlg->m_pfDeferentPitch			= m_pfDeferentPitch;
	pDlg->m_pComPort				= m_pComPort;

	pDlg->Create(IDD_DIALOG_RASBOARD);
	pDlg->ShowWindow(SW_SHOW);

	return TRUE;
}

int CRasBoardThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	if (m_ppDlg != NULL && (*m_ppDlg) != NULL)
	{
		if (IsWindow((*m_ppDlg)->m_hWnd))
		{
			(*m_ppDlg)->DestroyWindow();
		}
		*m_ppDlg = NULL;
	}
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRasBoardThread, CWinThread)
END_MESSAGE_MAP()


// CRasBoardThread 消息处理程序

int CRasBoardThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWinThread::Run();
}
