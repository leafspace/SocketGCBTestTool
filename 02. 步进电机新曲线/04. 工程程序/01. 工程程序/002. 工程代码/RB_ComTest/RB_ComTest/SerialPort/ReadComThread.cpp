// ReadComThread.cpp : 实现文件
//
#include "../stdafx.h"
#include "ComPort.h"

namespace zmComPort
{

	//CReadComThread

	CReadComThread::CReadComThread()
	{
		this->m_hThread = NULL;
		this->m_pPort = NULL;
		this->IsClose = false;
		::ZeroMemory(&this->m_BreakOverlapped, sizeof(this->m_BreakOverlapped));
		::ZeroMemory(&this->m_ReadOverlapped, sizeof(this->m_ReadOverlapped));

		memset(this->m_InputBuffer, 0, 2048);
		this->m_IsNeedExit = false;
	}

	CReadComThread::~CReadComThread()
	{
		Terminate();
		//this->m_hThread = NULL;
	}

	// CReadComThread 成员函数
	bool CReadComThread::SetReadEvent(OVERLAPPED& overlapped)
	{
	BeginSet:
		if (this->m_pPort == NULL || this->m_pPort->GetSerialPort() == NULL)
			return false;
		memset(this->m_InputBuffer, 0, 2048);
		try
		{
			if (this->m_pPort->GetSerialPort()->Read(this->m_InputBuffer, 2048, overlapped, &this->m_InBufferCount))
			{
				if (!this->HandleData())
					return  false;
				::ResetEvent(this->m_ReadOverlapped.hEvent);
				goto BeginSet;
			}
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
		}
		DWORD error = ::GetLastError();
		if (error == ERROR_IO_PENDING)
		{
			return true;
		}
		return false;
	}


	bool CReadComThread::HandleData() //处理读取数据
	{
		/*
			if (this->m_InBufferCount>0)
			{
			this->m_pBuffer = new byte[this->m_InBufferCount];
			for(int i=0;i<(int)this->m_InBufferCount;i++)
			{
			this->m_pBuffer[i] = this->m_InputBuffer[i];
			}
			//this->m_pSerialPort->
			this->m_pPort->ReceiveData(this->m_pBuffer,this->m_InBufferCount);
			delete[] this->m_pBuffer;
			}
		*/
		try
		{
			this->m_pPort->ReceiveData(this->m_InputBuffer, this->m_InBufferCount);
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
		}
		return true;
	}

	bool CReadComThread::HandleReadEvent(OVERLAPPED& overlapped)
	{
		try
		{
			if (this->m_pPort->GetSerialPort()->GetOverlappedResult(overlapped, this->m_InBufferCount, false))
			{
				return this->HandleData();
			}
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
		}

		DWORD dwError = ::GetLastError();
		if (dwError == ERROR_INVALID_HANDLE)
			return false;

		return true;
	}

	bool CReadComThread::SetBreakEvent(DWORD& dwMask)
	{
	SetBegin:
		if (this->m_pPort == NULL || this->m_pPort->GetSerialPort() == NULL)
			return false;
		try
		{
			if (this->m_pPort->GetSerialPort()->WaitEvent(dwMask, this->m_BreakOverlapped))
			{
				if (!this->HandleBreakEvent(dwMask))
					return false;
				goto SetBegin;
			}
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
			return false;
		}

		DWORD error = ::GetLastError();
		if (error == ERROR_IO_PENDING)
		{
			return true;
		}

		return false;
	}

	bool CReadComThread::HandleBreakEvent(DWORD dwMask)
	{
		DWORD dwReadBytes;
		bool succeed = false;

		try
		{
			succeed = this->m_pPort->GetSerialPort()->GetOverlappedResult(this->m_BreakOverlapped, dwReadBytes, false);
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
			succeed = false;
		}
		if (succeed)
		{
			this->m_pPort->ComBreak(dwMask); //调用处理过程
			return true;
		}
		return false;
	}

	void CReadComThread::Execute()
	{
		try
		{
			if (this->m_pPort->GetReceiveMode() == CComPort::ManualReceiveByQuery)
			{
				this->ExecuteByManualQueryRecvMode();
			}
			else if (this->m_pPort->GetReceiveMode() == CComPort::ManualReceiveByConst)
			{
				this->ExecuteByManualConstRecvMode();
			}
			else if (this->m_pPort->GetReceiveMode() == CComPort::AutoReceiveBySignal)
			{
				this->ExecuteByAutoSignalRecvMode();
			}
			else//中断模式
			{
				this->ExecuteByAutoBreakRecvMode();
			}
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
		}
	}
	void CReadComThread::ExecuteByAutoSignalRecvMode()
	{
		TRACE0("\t\t>> ExecuteByAutoSignalRecvMode() == 1 \n");
		this->m_pPort->SyncOpt_Commu_Enter();

		DWORD dwMask = 0;
		HANDLE WaitHandles[3];	//监听事件数组
		DWORD dwSignaledHandle;
		//DWORD dwStoredFlags = EV_ERR | EV_RLSD | EV_RING;
		DWORD dwStoredFlags = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | \
			EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY;

		WaitHandles[0] = this->m_pPort->GetCloseHandle();
		WaitHandles[1] = this->m_ReadOverlapped.hEvent;
		WaitHandles[2] = this->m_BreakOverlapped.hEvent;

		if (WaitHandles[0] == INVALID_HANDLE_VALUE ||
			WaitHandles[1] == INVALID_HANDLE_VALUE ||
			WaitHandles[2] == INVALID_HANDLE_VALUE)
		{
			this->m_pPort->SyncOpt_Commu_Leave();
			TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == 1 \n");
			return;//Error
		}

		if (this->m_pPort == NULL ||
			this->m_pPort->GetSerialPort() == NULL ||
			this->m_pPort->IsOpen() == false)
		{
			this->m_pPort->SyncOpt_Commu_Leave();
			TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == 1 \n");
			return; //error
		}

		this->m_pPort->GetSerialPort()->SetMask(dwStoredFlags);
		//	if (!SetBreakEvent(dwMask))
		//		goto EndThread;
		//设置读事件
		if (!SetReadEvent(this->m_ReadOverlapped))
		{
			this->m_pPort->SyncOpt_Commu_Leave();
			TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == 1 \n");
			goto EndThread;
		}

		TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == 1 \n");
		this->m_pPort->SyncOpt_Commu_Leave();

		//设置comEvent
		for (;;)
		{
			//dwSignaledHandle=::WaitForMultipleObjects(3,WaitHandles,false,INFINITE);
			dwSignaledHandle = ::WaitForMultipleObjects(3, WaitHandles, false, 500);
			switch (dwSignaledHandle)
			{
			case WAIT_OBJECT_0:
				//close
				goto EndThread;
				break;

			case WAIT_OBJECT_0 + 1:
				TRACE0("\t\t>> ExecuteByAutoSignalRecvMode() == Obj1 \n");
				this->m_pPort->SyncOpt_Commu_Enter();
				if (!this->HandleReadEvent(this->m_ReadOverlapped))
				{
					this->m_pPort->SyncOpt_Commu_Leave();
					TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == Obj1 \n");
					goto EndThread;
				}
				if (!this->SetReadEvent(this->m_ReadOverlapped))
				{
					this->m_pPort->SyncOpt_Commu_Leave();
					TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == Obj1 \n");
					goto EndThread;
				}
				this->m_pPort->SyncOpt_Commu_Leave();
				TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == Obj1 \n");
				break;

			case WAIT_OBJECT_0 + 2:
				TRACE0("\t\t>> ExecuteByAutoSignalRecvMode() == Obj2 \n");
				this->m_pPort->SyncOpt_Commu_Enter();
				if (!this->HandleBreakEvent(dwMask))
				{
					this->m_pPort->SyncOpt_Commu_Leave();
					TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == Obj2 \n");
					goto EndThread;
				}

				if (!this->SetBreakEvent(dwMask))
				{
					this->m_pPort->SyncOpt_Commu_Leave();
					TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == Obj2 \n");
					goto EndThread;
				}
				this->m_pPort->SyncOpt_Commu_Leave();
				TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == Obj2 \n");
				break;
			default:
				if (this->m_IsNeedExit)
				{
					goto EndThread;
				}
				break;
			}
		}

	EndThread:
		TRACE0("\t\t>> ExecuteByAutoSignalRecvMode() == 2 \n");
		this->m_pPort->SyncOpt_Commu_Enter();

		if (this->m_pPort == NULL ||
			this->m_pPort->GetSerialPort() == NULL ||
			this->m_pPort->IsOpen() == false)
		{
		}
		else
		{
			this->m_pPort->GetSerialPort()->Purge(PURGE_RXABORT | PURGE_RXCLEAR);
		}

		if (this->m_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(this->m_ReadOverlapped.hEvent);
			this->m_ReadOverlapped.hEvent = INVALID_HANDLE_VALUE;
		}
		if (this->m_BreakOverlapped.hEvent != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(this->m_BreakOverlapped.hEvent);
			this->m_BreakOverlapped.hEvent = INVALID_HANDLE_VALUE;
		}

		this->m_pPort->SyncOpt_Commu_Leave();
		TRACE0("\t\t<< ExecuteByAutoSignalRecvMode() == 2 \n");
		return;
	}

	void CReadComThread::ExecuteByManualQueryRecvMode()
	{
		DWORD dwMask = 0;
		HANDLE WaitHandles[2]; //监听事件数组
		DWORD dwSignaledHandle;

		WaitHandles[0] = this->m_pPort->GetCloseHandle();

		/*this->m_pPort->GetSerialPort()->SetMask(dwStoredFlags);
		this->m_pPort->GetSerialPort()->SetBreak(); */
		for (;;)
		{
			dwSignaledHandle = ::WaitForMultipleObjects(1, WaitHandles, false, INFINITE);
			switch (dwSignaledHandle)
			{
			case WAIT_OBJECT_0:
				goto EndThread;
				break;

			default:
				//goto EndThread;
				break;
			}
			this->m_pPort->GetSerialPort()->GetMask(dwMask);
			if (dwMask > 0)
			{
				this->m_pPort->ComBreak(dwMask);
			}
		}

	EndThread:
		this->m_pPort->GetSerialPort()->Purge(PURGE_RXABORT | PURGE_RXCLEAR);
		return;
	}

	void CReadComThread::ExecuteByManualConstRecvMode()
	{
		DWORD dwMask = 0;
		HANDLE WaitHandles[2]; //监听事件数组
		DWORD dwSignaledHandle;
		DWORD dwStoredFlags = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | \
			EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY;

		WaitHandles[0] = this->m_pPort->GetCloseHandle();
		WaitHandles[1] = this->m_BreakOverlapped.hEvent;

		this->m_pPort->GetSerialPort()->SetMask(dwStoredFlags);

		if (!SetBreakEvent(dwMask))
			goto EndThread;

		//设置comEvent
		for (;;)
		{
			dwSignaledHandle = ::WaitForMultipleObjects(2, WaitHandles, false, INFINITE);
			switch (dwSignaledHandle)
			{
			case WAIT_OBJECT_0:
				goto EndThread;
				break;

			case WAIT_OBJECT_0 + 1:
				if (!this->HandleBreakEvent(dwMask))
					goto EndThread;
				if (!this->SetBreakEvent(dwMask))
					goto EndThread;
				break;

			default:
				//goto EndThread;
				break;
			}


		}

	EndThread:
		this->m_pPort->GetSerialPort()->Purge(PURGE_RXABORT | PURGE_RXCLEAR);
		::CloseHandle(this->m_ReadOverlapped.hEvent);
		::CloseHandle(this->m_BreakOverlapped.hEvent);
		return;
	}
	void CReadComThread::ExecuteByAutoBreakRecvMode()
	{
		DWORD dwMask = 0;
		HANDLE WaitHandles[2]; //监听事件数组
		DWORD dwSignaledHandle;
		DWORD dwStoredFlags = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | \
			EV_RLSD | EV_RXCHAR | EV_RXFLAG;//??| EV_TXEMPTY 添加后在首次执行时总是接收不到全部数据

		WaitHandles[0] = this->m_pPort->GetCloseHandle();
		WaitHandles[1] = this->m_BreakOverlapped.hEvent;

		this->m_pPort->GetSerialPort()->SetMask(dwStoredFlags);

		//this->m_BreakOverlapped??两个事件同时给一个重叠结果的话??
		if (!SetBreakEvent(dwMask))
			goto EndThread;
		//设置读事件
		if (!SetReadEvent(this->m_BreakOverlapped))
			goto EndThread;

		//设置comEvent
		for (;;)
		{
			dwSignaledHandle = ::WaitForMultipleObjects(2, WaitHandles, false, INFINITE);
			switch (dwSignaledHandle)
			{
			case WAIT_OBJECT_0:
				goto EndThread;
				break;

			case WAIT_OBJECT_0 + 1:
				if ((dwMask&EV_RXCHAR) == EV_RXCHAR)
				{
					if (!this->HandleReadEvent(this->m_BreakOverlapped))
						goto EndThread;
					if (!SetReadEvent(this->m_BreakOverlapped))
						goto EndThread;
				}
				else
				{
					if (!this->HandleBreakEvent(dwMask))
						goto EndThread;
					if (!this->SetBreakEvent(dwMask))
						goto EndThread;
				}
				break;

			default:
				//goto EndThread;
				break;
			}
		}

	EndThread:
		this->m_pPort->GetSerialPort()->Purge(PURGE_RXABORT | PURGE_RXCLEAR);
		::CloseHandle(this->m_ReadOverlapped.hEvent);
		::CloseHandle(this->m_BreakOverlapped.hEvent);
		return;
	}

	DWORD WINAPI ThreadFunc(LPVOID  lpParam)
	{
		CReadComThread* pThread = (CReadComThread*)lpParam;
		ASSERT(pThread);
		pThread->m_IsTerminated = false;
		pThread->Execute();
		pThread->m_IsTerminated = true;
		return 0;
	}

	void CReadComThread::Create()
	{
		this->m_IsTerminated = true;
		m_hThread = CreateThread(
			NULL,                        // no security attributes 
			0,                           // use default stack size  
			ThreadFunc,                  // thread function 
			this,                // argument to thread function 
			CREATE_SUSPENDED,           // use default creation flags 
			&dwThreadId);                // returns the thread identifier 

		//适当提高线程优先级，可以保证通讯畅通
		::SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);	//THREAD_PRIORITY_NORMAL
	}

	void CReadComThread::Terminate()
	{
		if (m_hThread == NULL)
		{
			//char szMsg[80];
			//wsprintf( szMsg, "CreateThread failed." ); 
			//::MessageBox(NULL,szMsg,"ok",0);
		}
		else
		{
			if (!this->IsTerminated())
			{
				Sleep(1000);
			}
			if (!this->IsTerminated())
			{
				Sleep(1000);
				//::TerminateThread(m_hThread,0);
			}
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

	void CReadComThread::Resume()
	{
		if (this->m_hThread)
		{
			this->m_IsTerminated = false;
			ResumeThread(this->m_hThread);
		}
	}

	void CReadComThread::BandSerialPort(CComPort* pPort)
	{
		ASSERT(pPort);
		this->m_pPort = pPort;
		//创建异步读取事件

		if (this->m_pPort->IsOverlapped())
		{
			this->m_ReadOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
			ASSERT(this->m_ReadOverlapped.hEvent);
			this->m_BreakOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
			ASSERT(this->m_BreakOverlapped.hEvent);
		}

	}

	DWORD CReadComThread::ReadInput(void* pBuf, DWORD Count, DWORD dwMilliseconds)
	{
		DWORD dwRead = 0;
		try
		{
			if (!this->m_pPort->GetSerialPort()->Read(pBuf, Count, this->m_ReadOverlapped, &dwRead))
			{
				if (WaitForSingleObject(this->m_ReadOverlapped.hEvent, dwMilliseconds) == WAIT_OBJECT_0)
				{
					this->m_pPort->GetSerialPort()->GetOverlappedResult(this->m_ReadOverlapped, dwRead, false);
				}
			}
		}
		catch (zmSerialPort::CSerialException* pE)
		{
			//AfxMessageBox(pE->GetErrorMessage());
			pE->Delete();
			dwRead = 0;
		}

		return dwRead;

	}

}		//namespace zmComPort
