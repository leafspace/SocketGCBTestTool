#define GCB_IP "172.16.5.30"
#define BUFFER_SIZE 100

#include <iomanip>
#include <iostream>
#include "SocketLink/SocketLink.h"
using namespace std;

int main()
{
	bool isSuccess = true;
	int nRunTime = 3;
	SocketLink *socketLink = new SocketLink();
	const BYTE sOrders[] = { 0xF0, 0XF1, 0x41, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0xEC };
	const BYTE sOrdersB[] = { 0xF0, 0XF1, 0x20, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0xEC };
	BYTE rOrders[BUFFER_SIZE] = { 0 };
	do {
		isSuccess = socketLink->initSocket(GCB_IP);
		if (isSuccess == false) {
			cout << "ERROR : InitSoket failure !" << endl;
			break;
		}
		isSuccess = socketLink->linkServer();
		if (isSuccess == false) {
			cout << "ERROR : LinkSoket failure !" << endl;
			break;
		}

		cout << "TIP : Send Orders , size = " << sizeof(sOrders) << " ... " << endl;
		isSuccess = socketLink->sendOrders(sOrders, sizeof(sOrders));
		if (isSuccess == false) {
			cout << "ERROR : Send orders failure !" << endl;
			break;
		}

		cout << "TIP : Receive orders ..." << endl;
		int nLen = 0;
		isSuccess = socketLink->recvOrders(rOrders, &nLen, BUFFER_SIZE);
		if (isSuccess == false) {
			cout << "ERROR : Receive order failure !" << endl;
			break;
		} else {
			cout << "Receive order list , size = " << nLen << " : " << endl;
			cout.setf(ios::hex);
			for (int nIndex = 0; nIndex < nLen; ++nIndex) {
				cout << "0x" << hex << (int)rOrders[nIndex] << "\t";
			}
			cout << endl;
			cout.unsetf(ios::hex);
		}

		socketLink->freeSocket();
		Sleep(2000);
	} while(nRunTime--);

	delete socketLink;
	return 0;
}