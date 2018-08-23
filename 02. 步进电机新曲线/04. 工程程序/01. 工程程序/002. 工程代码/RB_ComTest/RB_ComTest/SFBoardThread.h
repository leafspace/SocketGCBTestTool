#pragma once
#include "MBComPort/MBComPort.h"
#include "SFBoardDlg.h"

// CSFBoardThread

class CSFBoardThread : public CWinThread
{
	DECLARE_DYNCREATE(CSFBoardThread)

public:
	CSFBoardThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CSFBoardThread();

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
	CSFBoardDlg	**m_ppDlg;

	BOOL m_bThreeAxisServo;	//三轴伺服板
	BOOL bIsNewCurve;

protected:
	DECLARE_MESSAGE_MAP()
};


