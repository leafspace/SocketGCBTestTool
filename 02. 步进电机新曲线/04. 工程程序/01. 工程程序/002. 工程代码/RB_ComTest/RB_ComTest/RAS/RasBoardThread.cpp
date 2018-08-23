// RasBoardThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
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
	// TODO: �ڴ�ִ���������߳�����
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


// CRasBoardThread ��Ϣ�������

int CRasBoardThread::Run()
{
	// TODO: �ڴ����ר�ô����/����û���

	return CWinThread::Run();
}
