#pragma once

#include "../MBComPort/MBComPort.h"
#include "LDBoardDlg.h"


// CLDBoardThread

class CLDBoardThread : public CWinThread
{
	DECLARE_DYNCREATE(CLDBoardThread)

protected:
	CLDBoardThread();           // 动态创建所使用的受保护的构造函数
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


