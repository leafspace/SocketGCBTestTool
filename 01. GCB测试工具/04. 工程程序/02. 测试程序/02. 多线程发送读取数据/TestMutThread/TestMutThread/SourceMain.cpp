#include "SocketLink/SocketLink.h"
#include <iostream>
using namespace std;

#define TIMER_GAP 3000
#define ORDER_BUFFER_SIZE 100

SocketLink socketLink;
HANDLE hMutexSocketLinkRecv;                                                // ���������̵߳��ź�
HANDLE hMutexSocketLinkSend;                                                // ���������̵߳��ź�


DWORD WINAPI ThreadSocketLinkRecv(LPVOID lpParamter)
{
	WaitForSingleObject(hMutexSocketLinkRecv, INFINITE);
	do {
		int nLen = 0;
		BYTE rOrders[ORDER_BUFFER_SIZE] = { 0 };
		bool bIsSuccess = socketLink.recvOrders(rOrders, &nLen, ORDER_BUFFER_SIZE);
		cout << "���յ�����" << endl;
	} while (true);

	ReleaseMutex(hMutexSocketLinkRecv);

	return 0;
}

DWORD WINAPI ThreadSocketLinkSend(LPVOID lpParamter)
{
	WaitForSingleObject(hMutexSocketLinkSend, INFINITE);
	do
	{
		const BYTE sOrders[] = { 0xF0, 0XF1, 0x41, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0xEC };
		socketLink.sendOrders(sOrders, sizeof(sOrders));
		cout << "����������" << endl;
		Sleep(TIMER_GAP);
	} while(true);

	ReleaseMutex(hMutexSocketLinkSend);
	return 0;
}

int main()
{
	socketLink.initSocket("172.16.5.30", 10000);
	socketLink.linkServer();

	return 0;
}