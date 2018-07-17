#include "stdafx.h"
#include "ThreadHandle.h"
#include "../GCBTestToolDlg.h"
#include "../SocketLink/SocketLink.h"

HANDLE hMutexSocketLinkConn;                                                // 连接测试线程的信号
HANDLE hMutexSocketLinkRecv;                                                // 接收数据线程的信号
HANDLE hMutexSocketLinkSend;                                                // 发送数据线程的信号
HANDLE hMutexSocketLinkHand;                                                // 处理指令线程的信号

DWORD WINAPI ThreadSocketLinkConn(LPVOID lpParamter)
{
	CGCBTestToolDlg::threadStateTable.InsertThreadFinishedFlag(TIMER_SOCKET_LINK_CONN, TIMER_STATE_UNRUNNING);
	WaitForSingleObject(hMutexSocketLinkConn, INFINITE);
	CGCBTestToolDlg::threadStateTable.SetThreadFinishedFlag(TIMER_SOCKET_LINK_CONN, TIMER_STATE_RUNNING);
	SocketLink *socketLink = (SocketLink*)lpParamter;
	bool bIsSuccess = socketLink->linkServer();
	// 将结果返回到状态表中
	CGCBTestToolDlg::threadStateTable.InsertThreadRetValueFlag(TIMER_SOCKET_LINK_CONN, (int)bIsSuccess);

	ReleaseMutex(hMutexSocketLinkConn);
	CGCBTestToolDlg::threadStateTable.SetThreadFinishedFlag(TIMER_SOCKET_LINK_CONN, TIMER_STATE_FINISH);
	return 0;
}

DWORD WINAPI ThreadSocketLinkRecv(LPVOID lpParamter)
{
	CGCBTestToolDlg::threadStateTable.InsertThreadFinishedFlag(TIMER_SOCKET_LINK_RECV, TIMER_STATE_UNRUNNING);
	WaitForSingleObject(hMutexSocketLinkRecv, INFINITE);
	CGCBTestToolDlg::threadStateTable.SetThreadFinishedFlag(TIMER_SOCKET_LINK_RECV, TIMER_STATE_RUNNING);

	// 接收数据循环操作
	SocketLink *socketLink = (SocketLink*)lpParamter;
	int nLen = 0;
	int threadRetValue = TIMER_STATE_FINISH;
	BYTE rOrders[ORDER_BUFFER_SIZE] = { 0 };
	do {
		bool bIsSuccess = socketLink->recvOrders(rOrders, &nLen, ORDER_BUFFER_SIZE);
		if (bIsSuccess) {
			do {} while(GCBMainDlg::recvMessageQueue.IsFull());

			MessageBean recvedMessageBean;
			recvedMessageBean.SetOrginDataList(rOrders, nLen);
			PROGRAM_STATE_CODE retStateCode = recvedMessageBean.AnalysisOrginDataLst();
			if (retStateCode == PROGRAM_ANALYSIS_ORGIN_DATA) {
				GCBMainDlg::recvMessageQueue.Push_back(recvedMessageBean);
			}
			
			Sleep(TIMER_GAP);
		} else {
			threadRetValue = TIMER_STATE_ERROR_TYPE_BREAKNET;
		}
	} while (true);

	// 将结果返回到状态表中
	CGCBTestToolDlg::threadStateTable.InsertThreadRetValueFlag(TIMER_SOCKET_LINK_RECV, threadRetValue);

	ReleaseMutex(hMutexSocketLinkRecv);
	CGCBTestToolDlg::threadStateTable.SetThreadFinishedFlag(TIMER_SOCKET_LINK_RECV, TIMER_STATE_FINISH);

	return 0;
}

DWORD WINAPI ThreadSocketLinkSend(LPVOID lpParamter)
{
	CGCBTestToolDlg::threadStateTable.InsertThreadFinishedFlag(TIMER_SOCKET_LINK_SEND, TIMER_STATE_UNRUNNING);
	WaitForSingleObject(hMutexSocketLinkSend, INFINITE);
	CGCBTestToolDlg::threadStateTable.SetThreadFinishedFlag(TIMER_SOCKET_LINK_SEND, TIMER_STATE_RUNNING);

	// 发送数据循环操作
	SocketLink *socketLink = (SocketLink*)lpParamter;
	int threadRetValue = TIMER_STATE_FINISH;
	do
	{
		do {} while(GCBMainDlg::sendMessageQueue.IsEmpty());
		do {
			MessageBean sendedMessageBean;
			GCBMainDlg::sendMessageQueue.Pop_front(&sendedMessageBean);
			list<BYTE> sendOrginDataLst = sendedMessageBean.GetOrginDataList();
			BYTE *sOrders = new BYTE[sendOrginDataLst.size()];

			list<BYTE>::iterator iter = sendOrginDataLst.begin();
			for (int nIndex = 0; nIndex < (int) sendOrginDataLst.size(); ++nIndex, ++iter) {
				sOrders[nIndex] = *iter;
			}
			socketLink->sendOrders(sOrders, sendOrginDataLst.size());
			delete sOrders;
		} while(!GCBMainDlg::sendMessageQueue.IsEmpty());

		Sleep(TIMER_GAP);
	} while(true);


	// 将结果返回到状态表中
	CGCBTestToolDlg::threadStateTable.InsertThreadRetValueFlag(TIMER_SOCKET_LINK_SEND, threadRetValue);

	ReleaseMutex(hMutexSocketLinkSend);
	CGCBTestToolDlg::threadStateTable.SetThreadFinishedFlag(TIMER_SOCKET_LINK_SEND, TIMER_STATE_FINISH);
	return 0;
}
