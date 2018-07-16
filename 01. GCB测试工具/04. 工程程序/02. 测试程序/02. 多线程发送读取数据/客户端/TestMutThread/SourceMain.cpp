#include "SocketLink/SocketLink.h"
#include <iostream>
using namespace std;

#define TIMER_GAP 3000
#define ORDER_BUFFER_SIZE 100

SocketLink socketLink;
HANDLE hMutexSocketLinkRecv;                                                // 接收数据线程的信号
HANDLE hMutexSocketLinkSend;                                                // 发送数据线程的信号


DWORD WINAPI ThreadSocketLinkRecv(LPVOID lpParamter)
{
	WaitForSingleObject(hMutexSocketLinkRecv, INFINITE);
	do {
		int nLen = 0;
		BYTE rOrders[ORDER_BUFFER_SIZE] = { 0 };
		bool bIsSuccess = socketLink.recvOrders(rOrders, &nLen, ORDER_BUFFER_SIZE);
		cout << "接收到数据" << endl;
		Sleep(1);
	} while (true);

	ReleaseMutex(hMutexSocketLinkRecv);

	return 0;
}

DWORD WINAPI ThreadSocketLinkSend(LPVOID lpParamter)
{
	WaitForSingleObject(hMutexSocketLinkSend, INFINITE);
	const BYTE sOrders[] = { 0xF0, 0XF1, 0x41, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0xEC };
	do
	{
		socketLink.sendOrders(sOrders, sizeof(sOrders));
		cout << "发送了数据" << endl;
		Sleep(1);
	} while(true);

	ReleaseMutex(hMutexSocketLinkSend);
	return 0;
}

int main()
{
	socketLink.initSocket("127.0.0.1", 10000);
	socketLink.linkServer();

	CreateThread(NULL, 0, ThreadSocketLinkSend, NULL, 0, NULL);
	CreateThread(NULL, 0, ThreadSocketLinkRecv, NULL, 0, NULL);

	Sleep(30000);
	return 0;
}