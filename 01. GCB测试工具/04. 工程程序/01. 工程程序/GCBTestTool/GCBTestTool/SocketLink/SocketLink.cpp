#include "stdafx.h"
#include "SocketLink.h"

bool SocketLink::initSocket()
{
	if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0) {
		return false;
	}

	int nNetTimeout = TIMEOUT;
	setsockopt(this->client, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	setsockopt(this->client, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//需要连接的服务器地址信息
	this->server.sin_family = AF_INET;                                       //需要连接的服务器的地址信息
	this->server.sin_addr.s_addr = inet_addr(this->targetIP.data());         //将命令行的IP地址转换为二进制表示的网络字节顺序IP地址
	this->server.sin_port = htons(this->port);

	this->client = socket(AF_INET, SOCK_STREAM, 0);                          //建立客户端流套接字
	if (this->client == INVALID_SOCKET) {
		return false;
	}

	return true;
}

bool SocketLink::initSocket(string ip, int port)
{
	this->setParameter(ip, port);
	return this->initSocket();
}

bool SocketLink::linkServer(void)
{
	if (connect(this->client, (struct sockaddr*)&this->server, sizeof(this->server)) == INVALID_SOCKET) {
		return false;
	}
	return true;
}

bool SocketLink::sendOrders(const BYTE *orders, const unsigned int size)
{
	return this->sendOrders(this->client, orders, size);
}

bool SocketLink::sendOrders(SOCKET socketID, const BYTE *orders, const unsigned int size)
{
	int msgLen;
	if ((msgLen = send(socketID, (char*)orders, size, 0)) == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool SocketLink::recvOrders(BYTE *orders, int *iLen, const unsigned int size)
{
	return this->recvOrders(this->client, orders, iLen, size);
}

bool SocketLink::recvOrders(SOCKET socketID, BYTE *orders, int *iLen, const unsigned int size)
{
	*iLen = recv(socketID, (char*)orders, size, 0);
	if (*iLen == SOCKET_ERROR) {
		return false;
	}
	return true;
}

void SocketLink::setParameter(string ip, int port)
{
	this->targetIP = ip;
	this->port = port;
}

void SocketLink::freeSocket(void)
{
	closesocket(this->client);
	WSACleanup();
}
