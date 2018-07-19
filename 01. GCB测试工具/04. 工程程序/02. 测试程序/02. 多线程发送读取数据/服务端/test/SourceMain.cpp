﻿#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib") //或者在项目属性的连接器的输入的附加依赖项中添加ws2_32.lib
#define  PORT 10000

HANDLE hMutexSocketLinkRecv;                                                // 接收数据线程的信号
HANDLE hMutexSocketLinkSend;                                                // 发送数据线程的信号

HANDLE hThreadSocketLinkRecv;                                           // 线程句柄
HANDLE hThreadSocketLinkSend;                                           // 线程句柄

SOCKET sAccept;

int index;
unsigned char cmdType;

DWORD WINAPI ThreadSocketLinkRecv(LPVOID lpParamter)
{
	WaitForSingleObject(hMutexSocketLinkRecv, INFINITE);
	do {
		char* msg = new char[500];
		int msgLen = recv(sAccept, msg, 500, 0);
		if (msg > 0) {
			printf("服务器接收到了消息[%d]\n", ++index);
			cmdType = msg[2];
		}
	} while (true);

	ReleaseMutex(hMutexSocketLinkRecv);

	return 0;
}

DWORD WINAPI ThreadSocketLinkSend(LPVOID lpParamter)
{
	WaitForSingleObject(hMutexSocketLinkSend, INFINITE);
	do
	{
		//char buf[] = { 0xF0, 0xF1, 0x41, 0x0A, 0x00, 0x00, 0x00, 0x3e, 0xd6, 0x2b, 0xdd, 0x3f, 0x52, 0xec };
		//buf[2] = cmdType;
		//int iSend = send(sAccept, buf, sizeof(buf), 0);
		printf("服务器发送了消息[%d]\n", ++index);
	} while(true);

	ReleaseMutex(hMutexSocketLinkSend);
	return 0;
}

void main()
{
	int port = PORT;
	WSADATA wsaData;
	SOCKET sListen;
	index = 0;

	int iLen;  //客户地址长度
	int iSend;  //发送数据长度
	struct sockaddr_in serv, client;//服务器、客户的地址
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Winsock load failed\n");
		return;
	}

	sListen = socket(AF_INET, SOCK_STREAM, 0);//创建套接字
	if (sListen == INVALID_SOCKET)
	{
		//创建套接字失败
		printf("socket failed:%d\n", WSAGetLastError());//输出错误
		return;
	}

	//建立服务器地址
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);//把一个双字节主机字节顺序的数据转换为网络字节顺序
	serv.sin_addr.s_addr = htonl(INADDR_ANY);//把四字节主机字节顺序转换为网络字节顺序，INADDR_ANY为系统指定的IP地址

	//绑定
	if (bind(sListen, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
	{
		//绑定失败
		printf("bind() failed:%d\n", WSAGetLastError());
		return;
	}

	//进入监听状态
	if (listen(sListen, 5) == SOCKET_ERROR)//正在等待连接的最大个数是5
	{
		//侦听出错
		printf("listen() failed:%d\n", WSAGetLastError());
		return;
	}

	iLen = sizeof(client);//初始化客户地址长度

	//进入循环等待客户端连接请求
	while(true)
	{
		sAccept = accept(sListen, (struct sockaddr*)&client, &iLen);
		if (sAccept == INVALID_SOCKET)
		{
			printf("accept() failed:%d\n", WSAGetLastError());
			exit(1);
		}

		
		int i = 0;
		do 
		{
			char* msg = new char[500];
			int msgLen = recv(sAccept, msg, 500, 0);
			if (msgLen > 0) {
				printf("服务器接收到了消息[%d]\n", i);

				/*for (int nIndex = 0; nIndex < msgLen; ++nIndex) {
					if (msg[nIndex] == 0xF0) {
						
					}
				}*/
				char buf[] = { 0xF0, 0xF1, 0x41, 0x0a, 0x00, 0x3e, 0xd8, 0x94, 0xc0, 0x3f, 0x5d, 0x8b, 0x6e, 0x00, 0x00, 0xec };
				buf[2] = msg[2];
				int iSend = send(sAccept, buf, sizeof(buf), 0);
				printf("服务器发送了消息[%d]\n", i);
				++i;
			} else {
				break;
			}

		} while (true);
		
	}

	closesocket(sListen);
	WSACleanup();
}