#pragma once
#pragma  comment(lib,"ws2_32.lib")

#define TIMEOUT	500

#include <string>
#include <winsock.h>

using namespace std;

typedef unsigned char BYTE;

class SocketLink
{
private:
	int port;
	SOCKET client;
	WSADATA wsaData;
	struct sockaddr_in server;                                              // 服务器端地址
	string targetIP;

public:
	SocketLink() {
		this->port = 0;
		this->targetIP = "";

	}

	bool initSocket(void);
	bool initSocket(string ip, int port);                                             // 初始化socket
	bool linkServer(void);                                                  // 进行连接测试
	bool sendOrders(const BYTE *orders, const unsigned int size);           // 发送消息
	bool sendOrders(SOCKET socketID, const BYTE *orders, const unsigned int size);              // 发送消息
	bool recvOrders(BYTE *orders, int *iLen, const unsigned int size);
	bool recvOrders(SOCKET socketID, BYTE *orders, int *iLen, const unsigned int size);

	void setParameter(string ip, int port);
	void freeSocket(void);                                                  // 清除socket
};