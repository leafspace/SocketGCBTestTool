// SFBoardThread.cpp : 实现文件
//

#include "stdafx.h"
#include "RB_ComTest.h"
#include "SFBoardThread.h"


// CSFBoardThread

IMPLEMENT_DYNCREATE(CSFBoardThread, CWinThread)

CSFBoardThread::CSFBoardThread()
{
	m_ppDlg = NULL;
}

CSFBoardThread::~CSFBoardThread()
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

BOOL CSFBoardThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	CSFBoardDlg		*pDlg = NULL;

	if (m_ppDlg == NULL)
		return FALSE;
	pDlg = *m_ppDlg;
	if (pDlg == NULL)
		pDlg = new CSFBoardDlg();
	if (pDlg == NULL)
	{
		AfxMessageBox("out of memory!");
		return FALSE;
	}
	*m_ppDlg = pDlg;

	pDlg->m_pParentThread = this;

	pDlg->m_pfReducer_x = m_pfReducer_x;
	pDlg->m_pfPulsesPerRevolution_x = m_pfPulsesPerRevolution_x;
	pDlg->m_pfDollyPerimeter = m_pfDollyPerimeter;

	pDlg->m_pfReducer_y = m_pfReducer_y;
	pDlg->m_pfPulsesPerRevolution_y = m_pfPulsesPerRevolution_y;
	pDlg->m_pfDeferentPerimeter = m_pfDeferentPerimeter;
	pDlg->m_pfDeferentPitch = m_pfDeferentPitch;

	pDlg->m_pfReducer_z = m_pfReducer_z;
	pDlg->m_pfPulsesPerRevolution_z = m_pfPulsesPerRevolution_z;
	pDlg->m_pfDeferentPerimeter_z = m_pfDeferentPerimeter_z;
	pDlg->m_pfDeferentPitch_z = m_pfDeferentPitch_z;

	pDlg->m_pComPort = m_pComPort;

	pDlg->m_bThreeAxisServo = m_bThreeAxisServo;
	pDlg->bIsNewCurve = bIsNewCurve;

	pDlg->Create(IDD_DIALOG_SFBOARD);
	pDlg->ShowWindow(SW_SHOW);

	//m_pDlg->DoModal();
	return TRUE;
}

int CSFBoardThread::ExitInstance()
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

BEGIN_MESSAGE_MAP(CSFBoardThread, CWinThread)
END_MESSAGE_MAP()


// CSFBoardThread 消息处理程序
