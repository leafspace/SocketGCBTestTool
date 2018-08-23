#pragma once

#include "../MBComPort/MBComPort.h"
#include "RasBoardDlg.h"


// CRasBoardThread

class CRasBoardThread : public CWinThread
{
	DECLARE_DYNCREATE(CRasBoardThread)

protected:
	CRasBoardThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CRasBoardThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	float* m_pfReducer_x;				//X减速机
	float* m_pfDollyPerimeter;			//小车周长
	float* m_pfPulsesPerRevolution_x;	//X每圈脉冲

	float* m_pfReducer_y;				//Y减速机
	float* m_pfPulsesPerRevolution_y;	//Y每圈脉冲
	float* m_pfDeferentPerimeter;		//Y导带周长
	float* m_pfDeferentPitch;			//Y丝杆螺距

	float* m_pfReducer_z;				//Z减速机
	float* m_pfPulsesPerRevolution_z;	//Z每圈脉冲
	float* m_pfDeferentPerimeter_z;		//Z导带周长
	float* m_pfDeferentPitch_z;			//Z丝杆螺距

	CCommunicationPort* m_pComPort;

	CRasBoardDlg	**m_ppDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


