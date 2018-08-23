// ComPort.cpp : 实现文件
//
#include "../stdafx.h"
#include "ComPort.h"

namespace zmComPort
{

	// CComPort

	CComPort::CComPort()
		: m_bIsOpen(false)
	{
		this->m_pfnOnReceiveData = NULL;
		this->m_pfnOnComBreak = NULL;
		this->m_pPort = NULL;
		this->m_pReadThread = NULL;
		::ZeroMemory(&this->m_WriteOverlapped, sizeof(this->m_WriteOverlapped));
		this->m_hWriteEvent = NULL;
		this->m_bIsOpen = false;

		InitializeCriticalSection(&m_csCommunicationSync);
	}

	CComPort::~CComPort()
	{
		DeleteCriticalSection(&m_csCommunicationSync);
		if (this->m_pPort)
		{
			if (this->m_pPort->IsOpen())
			{
				this->Close();
			}
		}

		if (this->m_pPort)
		{
			delete this->m_pPort;
			this->m_pPort = NULL;
		}
	}

	// CComPort 成员函数

	//当前串口是否打开
	bool CComPort::IsOpen(void) const
	{
		return this->m_bIsOpen;
	}

	bool CComPort::Open(int nPort, ReceiveMode mode, DWORD dwBaud, Parity parity, BYTE DataBits,
		StopBits stopbits, FlowControl fc)
	{
		Close();

		//1.新建串口
		if (this->m_pPort)
			delete this->m_pPort;
		this->m_pPort = new zmSerialPort::CSerialPort();
		if (this->m_pPort == NULL)
			return false;

		this->m_bIsOpen = false;

		//2.判断收发模式
		if (mode == zmComPort::CComPort::ManualReceiveByQuery)
		{
			this->m_IsOverlapped = false;
		}
		else
		{
			this->m_IsOverlapped = true;
		}
		this->m_RecvMode = mode;

		//3.转换参数,打开串口
		int index;
		index = parity - CComPort::EvenParity;
		zmSerialPort::CSerialPort::Parity spParity = (zmSerialPort::CSerialPort::Parity)(zmSerialPort::CSerialPort::EvenParity + index);
		index = stopbits - CComPort::OneStopBit;
		zmSerialPort::CSerialPort::StopBits spStopbits = (zmSerialPort::CSerialPort::StopBits)(zmSerialPort::CSerialPort::OneStopBit + index);
		index = fc - CComPort::NoFlowControl;
		zmSerialPort::CSerialPort::FlowControl spFC = (zmSerialPort::CSerialPort::FlowControl)(zmSerialPort::CSerialPort::NoFlowControl + index);

		try
		{
			this->m_pPort->Open(nPort, dwBaud, spParity, DataBits, spStopbits, spFC, m_IsOverlapped);
			ASSERT(this->m_pPort->IsOpen());

			//it is important!!
			COMMTIMEOUTS timeouts;
			this->m_pPort->GetTimeouts(timeouts);
			timeouts.ReadIntervalTimeout = 100;
			//timeouts.WriteTotalTimeoutConstant = 1000;
			this->m_pPort->SetTimeouts(timeouts);

			this->m_pPort->Purge(PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			this->m_pPort->Setup(4096, 4096);
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
			Close();
			return false;
		}

		this->m_CurPortNum = nPort;
		//创建关闭事件
		this->m_hCloseEvent = CreateEvent(NULL, true, false, NULL);
		if (this->m_hCloseEvent == NULL)
		{
			Close();
			return false;
		}
		//4.创建线程类

		this->m_pReadThread = new CReadComThread();
		if (this->m_pReadThread == NULL)
		{
			Close();
			return false;
		}
		this->m_pReadThread->BandSerialPort(this);
		this->m_pReadThread->Create(); //Suspend
		this->m_pReadThread->m_IsNeedExit = false;
		this->m_pReadThread->Resume();

		if (this->IsOverlapped())
		{
			this->m_hWriteEvent = ::CreateEvent(NULL, true, false, NULL);
			if (this->m_hWriteEvent == NULL)
			{
				Close();
				return false;
			}
			this->m_WriteOverlapped.hEvent = this->m_hWriteEvent;
		}

		this->m_bIsOpen = true;
		return true;
	}

	void CComPort::Close()
	{
		//1.串口
		if (this->m_pPort == NULL)
			return;
		if (!this->m_pPort->IsOpen())
			return;

		CReadComThread	*pReadThread = NULL;
		zmSerialPort::CSerialPort *pPort = NULL;

		TRACE0(">> CComPort::Close() == 1 \n");
		this->SyncOpt_Commu_Enter();

		pPort = this->m_pPort;
		pReadThread = this->m_pReadThread;

		//预设置为“已关闭”状态
		this->m_bIsOpen = false;
		this->m_pPort = NULL;
		this->m_pReadThread = NULL;
		this->SyncOpt_Commu_Leave();
		TRACE0("<< CComPort::Close() == 1 \n");

		//2.事件
		if (pReadThread != NULL)
		{
			TRACE0(">> CComPort::Close() == 2 \n");
			this->SyncOpt_Commu_Enter();
			pReadThread->m_IsNeedExit = true;
			::SetEvent(this->m_hCloseEvent);		//通知关闭系统
			this->SyncOpt_Commu_Leave();
			TRACE0("<< CComPort::Close() == 2 \n");

			DWORD dwstart = GetTickCount() + 2000;
			while (!(pReadThread->IsTerminated()))
			{
				Sleep(100);
				if (GetTickCount() > dwstart)
				{
					TRACE0("\n -- Terminate in CComPort::Close() -- \n\n");
					break;
				}
			}
		}

		TRACE0(">> CComPort::Close() == 3 \n");
		this->SyncOpt_Commu_Enter();

		//3.结束读线程
		try
		{
			if (pReadThread != NULL)
			{
				pReadThread->Terminate();
				delete pReadThread;
				pReadThread = NULL;
			}
		}
		catch (char e[150])
		{
			::AfxMessageBox(e);
		}

		//4.结束关闭线程
		::CloseHandle(this->m_hCloseEvent);
		if (pPort != NULL)
		{
			try
			{
				pPort->Purge(PURGE_RXABORT | PURGE_RXCLEAR);
			}
			catch (zmSerialPort::CSerialException* pE)
			{
				//AfxMessageBox(pE->GetErrorMessage());
				pE->Delete();
			}
			pPort->Close();
		}

		//5.结束写事件
		if (this->m_hWriteEvent)
		{
			::CloseHandle(this->m_hWriteEvent);
			this->m_hWriteEvent = NULL;
		}
		//6.释放串口对象
		if (pPort != NULL)
		{
			delete pPort;
			pPort = NULL;
		}

		this->SyncOpt_Commu_Leave();
		TRACE0("<< CComPort::Close() == 3 \n");

	}

	void CComPort::ReceiveData(const void* pBuf, DWORD InBufferCount)
	{
		if (this->m_pfnOnReceiveData)
			this->m_pfnOnReceiveData(this->m_pSender, pBuf, InBufferCount);
	}

	void CComPort::SetReceiveFunc(FOnReceiveData pfnOnReceiveData, LPVOID pSender)
	{
		this->m_pfnOnReceiveData = pfnOnReceiveData;
		this->m_pSender = pSender;
	}

	void CComPort::ComBreak(DWORD dwMask)
	{
		if (this->m_pfnOnComBreak)
		{
			COMSTAT stat;
			try
			{
				this->m_pPort->GetStatus(stat);
			}
			catch (zmSerialPort::CSerialException* pE)
			{
				//AfxMessageBox(pE->GetErrorMessage());
				pE->Delete();
			}
			this->m_pfnOnComBreak(this->m_pPort, dwMask, stat);
		}
	}

	void CComPort::SetBreakHandleFunc(FOnComBreak pfnOnComBreak)
	{
		this->m_pfnOnComBreak = pfnOnComBreak;
	}

	CComPort::ReceiveMode CComPort::GetReceiveMode() const
	{
		return this->m_RecvMode;
	}

	DWORD CComPort::GetInBufferCount() const
	{
		if (this->IsOverlapped())
		{
			::AfxMessageBox("this methord is only used for ManualQueryMode!");
			return 0;
		}
		COMSTAT stat;
		::ZeroMemory(&stat, sizeof(stat));

		try
		{
			this->m_pPort->GetStatus(stat);
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
		}
		return stat.cbInQue;
	}


	DWORD CComPort::GetInput(void* pBuf, DWORD Count, DWORD dwMilliseconds)
	{
		//不能在自动模式下getinput
		if (this->GetReceiveMode() == CComPort::AutoReceiveByBreak ||
			this->GetReceiveMode() == CComPort::AutoReceiveBySignal)
		{
			::AfxMessageBox("Can't use GetInput methord in this mode!");
			return 0;
		}

		try
		{
			if (this->IsOverlapped())
			{
				ASSERT(this->m_pReadThread);
				DWORD dwBytes = this->m_pReadThread->ReadInput(pBuf, Count, dwMilliseconds);
				this->m_pPort->TerminateOutstandingReads();
				return dwBytes;
			}
			else
				return this->m_pPort->Read(pBuf, Count);
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
			return 0;
		}
	}

	DWORD CComPort::Output(void* pBuf, DWORD Count)
	{
		DWORD dwWriteBytes = 0;
		try
		{
			if (this->IsOverlapped())//异步模式
			{
				TRACE0(">> CComPort::Output() == 1 \n");
				this->SyncOpt_Commu_Enter();
				this->m_pPort->Write(pBuf, Count, this->m_WriteOverlapped);
				this->SyncOpt_Commu_Leave();
				TRACE0("<< CComPort::Output() == 1 \n");
				if (WaitForSingleObject(this->m_WriteOverlapped.hEvent, INFINITE) == WAIT_OBJECT_0)
				{
					this->m_pPort->GetOverlappedResult(this->m_WriteOverlapped, dwWriteBytes, false);
				}
			}
			else
			{
				TRACE0(">> CComPort::Output() == 2 \n");
				this->SyncOpt_Commu_Enter();
				// 		//for use in win98
				// 		for(DWORD i=0;i<Count;i++)
				// 		{
				// 		dwWriteBytes+= this->m_pPort->Write(pBuf,1);
				// 		} 

				dwWriteBytes = this->m_pPort->Write(pBuf, Count);
				this->SyncOpt_Commu_Leave();
				TRACE0("<< CComPort::Output() == 2 \n");
			}
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
			dwWriteBytes = 0;
		}
		return dwWriteBytes;
	}

	zmSerialPort::CSerialPort* CComPort::GetSerialPort() const
	{
		//	ASSERT(m_pPort);
		return m_pPort;
	}
	HANDLE CComPort::GetCloseHandle() const
	{
		ASSERT(this->m_hCloseEvent);
		return this->m_hCloseEvent;
	}

	void CComPort::SyncOpt_Commu_Enter()
	{
		EnterCriticalSection(&m_csCommunicationSync);
		TRACE3("\t\t\tSyncOpt_Commu_Enter_%d2,%2d, %.8X\n",
			m_csCommunicationSync.LockCount,
			m_csCommunicationSync.RecursionCount,
			m_csCommunicationSync.OwningThread);
	}
	void CComPort::SyncOpt_Commu_Leave()
	{
		TRACE3("\t\t\tSyncOpt_Commu_Leave_%2d,%2d,%.8X\n",
			m_csCommunicationSync.LockCount,
			m_csCommunicationSync.RecursionCount,
			m_csCommunicationSync.OwningThread);
		LeaveCriticalSection(&m_csCommunicationSync);
	}


}		//namespace zmComPort
