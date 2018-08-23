#pragma once

#include "../MBComPort/MBComPort.h"
#include "LDBoardDlg.h"


// CLDBoardThread

class CLDBoardThread : public CWinThread
{
	DECLARE_DYNCREATE(CLDBoardThread)

protected:
	CLDBoardThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLDBoardThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	
	CCommunicationPort* m_pComPort;

	CLDBoardDlg	**m_ppDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


