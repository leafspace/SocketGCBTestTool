// LDBoardThread.cpp : ʵ���ļ�
//

#include "../stdafx.h"
#include "../RB_ComTest.h"
#include "LDBoardThread.h"



// CLDBoardThread

IMPLEMENT_DYNCREATE(CLDBoardThread, CWinThread)

CLDBoardThread::CLDBoardThread()
{
	m_ppDlg = NULL;
}

CLDBoardThread::~CLDBoardThread()
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

BOOL CLDBoardThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	CLDBoardDlg	*pDlg = NULL;

	if (m_ppDlg == NULL)
		return FALSE;

	pDlg = *m_ppDlg;
	if (pDlg == NULL)
		pDlg = new CLDBoardDlg();
	if (pDlg == NULL)
	{
		AfxMessageBox("out of memory!");
		return FALSE;
	}
	*m_ppDlg = pDlg;

	pDlg->m_pParentThread = this;

	pDlg->m_pComPort = m_pComPort;

	pDlg->Create(IDD_LD_BOARD);
	pDlg->ShowWindow(SW_SHOW);

	return TRUE;
}

int CLDBoardThread::ExitInstance()
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

BEGIN_MESSAGE_MAP(CLDBoardThread, CWinThread)
END_MESSAGE_MAP()


// CLDBoardThread ��Ϣ�������

int CLDBoardThread::Run()
{
	// TODO: �ڴ����ר�ô����/����û���

	return CWinThread::Run();
}
