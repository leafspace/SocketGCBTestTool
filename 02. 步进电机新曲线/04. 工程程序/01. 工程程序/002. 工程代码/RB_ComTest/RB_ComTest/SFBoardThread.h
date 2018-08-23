#pragma once
#include "MBComPort/MBComPort.h"
#include "SFBoardDlg.h"

// CSFBoardThread

class CSFBoardThread : public CWinThread
{
	DECLARE_DYNCREATE(CSFBoardThread)

public:
	CSFBoardThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSFBoardThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	float* m_pfReducer_x;				//X���ٻ�
	float* m_pfDollyPerimeter;			//С���ܳ�
	float* m_pfPulsesPerRevolution_x;	//XÿȦ����

	float* m_pfReducer_y;				//Y���ٻ�
	float* m_pfPulsesPerRevolution_y;	//YÿȦ����
	float* m_pfDeferentPerimeter;		//Y�����ܳ�
	float* m_pfDeferentPitch;			//Y˿���ݾ�

	float* m_pfReducer_z;				//Z���ٻ�
	float* m_pfPulsesPerRevolution_z;	//ZÿȦ����
	float* m_pfDeferentPerimeter_z;		//Z�����ܳ�
	float* m_pfDeferentPitch_z;			//Z˿���ݾ�

	CCommunicationPort* m_pComPort;
	CSFBoardDlg	**m_ppDlg;

	BOOL m_bThreeAxisServo;	//�����ŷ���
	BOOL bIsNewCurve;

protected:
	DECLARE_MESSAGE_MAP()
};


