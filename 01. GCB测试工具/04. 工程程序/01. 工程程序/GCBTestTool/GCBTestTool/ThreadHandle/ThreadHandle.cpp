#include "stdafx.h"
#include "ThreadHandle.h"

HANDLE hMutexSocketLinkConn;                                                // 连接测试线程的信号
HANDLE hMutexSocketLinkRecv;                                                // 接收数据线程的信号
HANDLE hMutexSocketLinkSend;                                                // 发送数据线程的信号

DWORD WINAPI ThreadSocketLinkConn(LPVOID lpParamter)
{
	CommunicateCore *communicationCore = NULL;
	CGCBTestToolDlg *pMainDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());
	if (pMainDlg) {
		communicationCore = pMainDlg->GetCommunicateCore();
	}
	if (communicationCore == NULL) {
		return 0;
	}

	communicationCore->GetStateTable()->InsertThreadFinishedFlag(TIMER_SOCKET_LINK_CONN, TIMER_STATE_UNRUNNING);
	WaitForSingleObject(hMutexSocketLinkConn, INFINITE);
	communicationCore->GetStateTable()->SetThreadFinishedFlag(TIMER_SOCKET_LINK_CONN, TIMER_STATE_RUNNING);
	SocketLink *socketLink = (SocketLink*)lpParamter;
	bool bIsSuccess = socketLink->linkServer();
	// 将结果返回到状态表中
	communicationCore->GetStateTable()->InsertThreadRetValueFlag(TIMER_SOCKET_LINK_CONN, (int)bIsSuccess);

	ReleaseMutex(hMutexSocketLinkConn);
	communicationCore->GetStateTable()->SetThreadFinishedFlag(TIMER_SOCKET_LINK_CONN, TIMER_STATE_FINISH);
	return 0;
}

DWORD WINAPI ThreadSocketLinkRecv(LPVOID lpParamter)
{
	CommunicateCore *communicationCore = NULL;
	CGCBTestToolDlg *pMainDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());
	if (pMainDlg) {
		communicationCore = pMainDlg->GetCommunicateCore();
	}
	if (communicationCore == NULL) {
		return 0;
	}

	communicationCore->GetStateTable()->InsertThreadFinishedFlag(TIMER_SOCKET_LINK_RECV, TIMER_STATE_UNRUNNING);
	WaitForSingleObject(hMutexSocketLinkRecv, INFINITE);
	communicationCore->GetStateTable()->SetThreadFinishedFlag(TIMER_SOCKET_LINK_RECV, TIMER_STATE_RUNNING);

	// 接收数据循环操作
	SocketLink *socketLink = (SocketLink*)lpParamter;
	int nLen = 0;
	int threadRetValue = TIMER_STATE_FINISH;
	BYTE rOrders[ORDER_BUFFER_SIZE] = { 0 };
	do {
		bool bIsSuccess = socketLink->recvOrders(rOrders, &nLen, ORDER_BUFFER_SIZE);
		if (bIsSuccess) {
			do {} while (communicationCore->GetRecvMessageQueue()->IsFull());

			// 将接收到的原始数据写入log
			ofstream outfile("orgin.log", ios::app);
			CommunicateCore::WriteTimeStr(outfile);
			for (int nIndex = 0; nIndex < nLen; ++nIndex) {
				outfile << "0x" << hex << setw(2) << setfill('0') << (int)rOrders[nIndex] << " ";
			}
			outfile << endl;
			outfile.close();


			// rOrders可能存在多个返回的指令集，所以在此对返回码进行拆分
			for (int nBeIndex = 0, nEnIndex = 0, nIndex = 1; nIndex < nLen; ++nIndex) {
				if (rOrders[nIndex] == 0xF1 && rOrders[nIndex - 1] == 0xF0) {
					nBeIndex = nIndex - 1;
				}

				if (rOrders[nIndex] == 0xEC) {
					nEnIndex = nIndex;

					// 能够接收的消息
					if (rOrders[nBeIndex + 2] < NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE) {
						// 将原始指令数据封装成对象
						MessageBean recvedMessageBean;
						recvedMessageBean.SetOrginDataList(&rOrders[nBeIndex], nEnIndex - nBeIndex + 1);
						PROGRAM_STATE_CODE retStateCode = recvedMessageBean.AnalysisOrginDataLst();
						if (retStateCode == PROGRAM_ANALYSIS_ORGIN_DATA) {
							communicationCore->GetRecvMessageQueue()->Push_back(recvedMessageBean);
						}
					}

					nBeIndex = 0;
					nEnIndex = 0;
				}
			}

			Sleep(TIMER_GAP);
		}
		else {
			threadRetValue = TIMER_STATE_ERROR_TYPE_BREAKNET;
		}
	} while (true);

	// 将结果返回到状态表中
	communicationCore->GetStateTable()->InsertThreadRetValueFlag(TIMER_SOCKET_LINK_RECV, threadRetValue);

	ReleaseMutex(hMutexSocketLinkRecv);
	communicationCore->GetStateTable()->SetThreadFinishedFlag(TIMER_SOCKET_LINK_RECV, TIMER_STATE_FINISH);

	return 0;
}

DWORD WINAPI ThreadSocketLinkSend(LPVOID lpParamter)
{
	CommunicateCore *communicationCore = NULL;
	CGCBTestToolDlg *pMainDlg = (CGCBTestToolDlg*)(AfxGetApp()->GetMainWnd());
	if (pMainDlg) {
		communicationCore = pMainDlg->GetCommunicateCore();
	}
	if (communicationCore == NULL) {
		return 0;
	}

	communicationCore->GetStateTable()->InsertThreadFinishedFlag(TIMER_SOCKET_LINK_SEND, TIMER_STATE_UNRUNNING);
	WaitForSingleObject(hMutexSocketLinkSend, INFINITE);
	communicationCore->GetStateTable()->SetThreadFinishedFlag(TIMER_SOCKET_LINK_SEND, TIMER_STATE_RUNNING);

	// 发送数据循环操作
	SocketLink *socketLink = (SocketLink*)lpParamter;
	int threadRetValue = TIMER_STATE_FINISH;
	do
	{
		do {} while (communicationCore->GetSendMessageQueue()->IsEmpty());
		do {
			MessageBean sendedMessageBean;
			communicationCore->GetSendMessageQueue()->Pop_front(&sendedMessageBean);
			list<BYTE> sendOrginDataLst = sendedMessageBean.GetOrginDataList();
			BYTE *sOrders = new BYTE[sendOrginDataLst.size()];

			ofstream outfile("send.log", ios::app);
			list<BYTE>::iterator iter = sendOrginDataLst.begin();
			CommunicateCore::WriteTimeStr(outfile);
			for (int nIndex = 0; nIndex < (int)sendOrginDataLst.size(); ++nIndex, ++iter) {
				sOrders[nIndex] = *iter;
				outfile << "0x" << hex << setw(2) << setfill('0') << (int)sOrders[nIndex] << " ";
			}
			outfile << endl;
			outfile.close();

			socketLink->sendOrders(sOrders, sendOrginDataLst.size());

			delete sOrders;
		} while (!communicationCore->GetSendMessageQueue()->IsEmpty());

		Sleep(TIMER_GAP);
	} while (true);


	// 将结果返回到状态表中
	communicationCore->GetStateTable()->InsertThreadRetValueFlag(TIMER_SOCKET_LINK_SEND, threadRetValue);

	ReleaseMutex(hMutexSocketLinkSend);
	communicationCore->GetStateTable()->SetThreadFinishedFlag(TIMER_SOCKET_LINK_SEND, TIMER_STATE_FINISH);
	return 0;
}
