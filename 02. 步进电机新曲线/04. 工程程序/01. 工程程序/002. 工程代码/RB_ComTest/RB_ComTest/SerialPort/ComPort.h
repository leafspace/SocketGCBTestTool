#if _MSC_VER > 1000
//#pragma once
#endif // _MSC_VER > 1000

//���պ���
#ifndef COMPORT_H
#define COMPORT_H

typedef void (* FOnReceiveData)(LPVOID,const void*,DWORD);
typedef void (* FOnComBreak)(LPVOID,DWORD,COMSTAT stat);


#include "serialport.h"

// CComPort ����Ŀ��
namespace zmComPort{

class CReadComThread;

class CComPort 
{
public:
	enum ReceiveMode
	{
		ManualReceiveByQuery,  //�ֶ���ѯ����
		ManualReceiveByConst,  //��������
		AutoReceiveBySignal,   //�ź��Զ�����
        AutoReceiveByBreak,	   //�Զ��жϽ���
	};
    //Enums
	enum FlowControl
	{
		NoFlowControl,
		CtsRtsFlowControl,
		CtsDtrFlowControl,
		DsrRtsFlowControl,
		DsrDtrFlowControl,
		XonXoffFlowControl
	};

	enum Parity
	{    
		NoParity	= 0,
		OddParity	= 1,
		EvenParity,
		MarkParity,
		SpaceParity
	};

	enum StopBits
	{
		OneStopBit			= 0,
		OnePointFiveStopBits= 1,
		TwoStopBits
	};

	CComPort();
	virtual ~CComPort();
	
	  //1.��,�رմ��ں���
	bool Open(int nPort,ReceiveMode mode=AutoReceiveBySignal, DWORD dwBaud = 9600, Parity parity = NoParity, BYTE DataBits = 8, 
		  StopBits stopbits = OneStopBit, FlowControl fc = NoFlowControl);
    void Close();

    //2.���ý��պ���,�жϴ����� 
	void SetReceiveFunc(FOnReceiveData pfnOnReceiveData,LPVOID pSender);
    void SetBreakHandleFunc(FOnComBreak pfnOnComBreak);

	//3.��ȡ�������
	int GetCurPortNum() { return this->m_CurPortNum;  }
	zmSerialPort::CSerialPort* GetSerialPort() const; 
	HANDLE GetCloseHandle() const;
	ReceiveMode GetReceiveMode() const;

    //4.(�߳���)֪ͨ���մ�����     
	void ReceiveData(const void* pBuf,DWORD InBufferCount);//�̵߳��õĽ��պ���
	void ComBreak(DWORD dwMask);

	//6.����,�������--����ʵ�ʸ���
	DWORD GetInBufferCount() const;
	DWORD GetInput(void* pBuf,DWORD Count,DWORD dwMilliseconds=1000);
	DWORD Output(void* pBuf,DWORD Count);
	bool IsOverlapped() const { return m_IsOverlapped; }
	
protected:
	zmSerialPort::CSerialPort* m_pPort;                           //�ں�������
	CReadComThread* m_pReadThread;                  //�������߳� 

	LPVOID m_pSender;                               //����ĸ�����ָ��
	int m_CurPortNum;                               //��ǰ�˿ں� 
    FOnReceiveData m_pfnOnReceiveData;              //�����źź���
	FOnComBreak    m_pfnOnComBreak;                 //�����¼�������
    ReceiveMode    m_RecvMode;                      //����ģʽ

	HANDLE         m_hWriteEvent;                    //д�¼�
	OVERLAPPED     m_WriteOverlapped;                //д�ص��ṹ

	bool m_IsOverlapped;                            //�Ƿ��ص��ṹ;
private:
    HANDLE m_hCloseEvent; //E: A event handle to close thread  //Chinese:�����߳��¼�

public:
	// //��ǰ�����Ƿ��
	bool IsOpen(void) const;
protected:
	// ��ǰ�����Ƿ��
	bool m_bIsOpen;

	CRITICAL_SECTION	m_csCommunicationSync;		//����ͨѶͬ��
public:
	//����ͨѶͬ��
	void	SyncOpt_Commu_Enter();
	void	SyncOpt_Commu_Leave();
};

//DWORD WINAPI ThreadFunc(LPVOID  lpParam ); //�̵߳��ú��� 

class CReadComThread
{
public:
    /*1.������ϵ������
	*/
	CReadComThread();
    virtual ~CReadComThread();          
    
	/* 2.����,����,��λ 
	*/
    void Create();                         //�����߳�
    void Terminate();                      //�����߳�
    void Resume();                         //��λ 
	bool IsTerminated()	{return this->m_IsTerminated; }
     
	/*3.�󶨴���,�첽��ȡ
	*/
	void BandSerialPort(CComPort* pPort);  //�󶨴���
	DWORD ReadInput(void* pBuf,DWORD Count,DWORD dwMilliseconds);//�첽��ȡ����

	friend DWORD WINAPI ThreadFunc(LPVOID  lpParam ); 
protected:
	DWORD dwThreadId;//�̺߳�  
    bool IsClose;
    /*4.�����첽��ȡ�¼�,�첽�ж��¼��Լ������¼�
	*/
	bool SetReadEvent(OVERLAPPED& overlapped);//�����������¼�
	bool HandleReadEvent(OVERLAPPED& overlapped);//������¼�
    bool HandleData(); //�����ȡ����
    
	bool SetBreakEvent(DWORD& dwMask);//���ô����ж��¼�,ͨ��DWMask�����ĸı䷵�ؼ���״̬
	bool HandleBreakEvent(DWORD dwMask);//�������ж��¼�

	/*5.�ֶ�ģʽ,�ź�ģʽ,�ж�ģʽִ���߳�
	*/
	void ExecuteByAutoSignalRecvMode();
	void ExecuteByAutoBreakRecvMode();
	void ExecuteByManualQueryRecvMode();
	void ExecuteByManualConstRecvMode();
    void Execute(void);                    //�߳�ִ��
   
private:
	HANDLE m_hThread;           //�߳̾��
	zmComPort::CComPort* m_pPort;          //��������ָ��

    byte  m_InputBuffer[2048];  //���ջ�����
	byte* m_pBuffer;            //ʵ�ʵ��ڴ� 
	DWORD m_InBufferCount;      //���ո��� 

	OVERLAPPED m_ReadOverlapped;     //��ȡ�ص��ṹ
	OVERLAPPED m_BreakOverlapped;    //�����ж��¼��ṹ
    
	bool m_IsTerminated;            //�Ƿ��ѽ����߳� 
public:
	bool m_IsNeedExit;              //�Ƿ���Ҫ�����߳� 
};

}

#endif