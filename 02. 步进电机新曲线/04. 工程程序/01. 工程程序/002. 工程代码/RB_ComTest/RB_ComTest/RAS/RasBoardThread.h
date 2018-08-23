#pragma once

#include "../MBComPort/MBComPort.h"
#include "RasBoardDlg.h"


// CRasBoardThread

class CRasBoardThread : public CWinThread
{
	DECLARE_DYNCREATE(CRasBoardThread)

protected:
	CRasBoardThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CRasBoardThread();

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

	CRasBoardDlg	**m_ppDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


