#include "stdafx.h"
#include "CommunicationCore.h"

uint16_t CommunicateCore::uRequestBeginAddress[LIST_NUM] = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };
uint16_t CommunicateCore::uRequestDriveNums[LIST_NUM] = { 2, 2, 2, 2, 2, 2, 1 };
bool CommunicateCore::bRequestFlag[LIST_NUM] = { true, true, true, true, true, true, true };

CommunicateCore::CommunicateCore()
{
	this->InitCoreData();
}

CommunicateCore::~CommunicateCore()
{
	this->ClearCoreData();
}

void CommunicateCore::InitCoreData(void)
{
}

void CommunicateCore::ClearCoreData(void)
{
	this->CloseSocketLink();
	this->CloseAllThread();
	this->ClearQueueData();
}

void CommunicateCore::CloseSocketLink(void)
{
	this->socketLink.freeSocket();
}

void CommunicateCore::CloseAllThread(void)
{
	this->CloseSocketLinkTestThread();
	this->CloseSocketLinkRecvThread();
	this->CloseSocketLinkSendThread();
}
void CommunicateCore::ClearQueueData(void)
{
	this->threadStateTable.ClearThreadAllFlag();
	this->recvMessageQueue.Clear();
	this->sendMessageQueue.Clear();
}

SocketLink* CommunicateCore::GetSocketLink(void)
{
	return &this->socketLink;
}

HANDLE* CommunicateCore::GetThreadLinkTestHandle(void)
{
	return &this->hThreadSocketLinkTest;
}

HANDLE* CommunicateCore::GetThreadLinkRecvHandle(void)
{
	return &this->hThreadSocketLinkRecv;
}

HANDLE* CommunicateCore::GetThreadLinkSendHandle(void)
{
	return &this->hThreadSocketLinkSend;
}

StateTable* CommunicateCore::GetStateTable(void)
{
	return &this->threadStateTable;
}

MessageQueue* CommunicateCore::GetRecvMessageQueue(void)
{
	return &this->recvMessageQueue;
}

MessageQueue* CommunicateCore::GetSendMessageQueue(void)
{
	return &this->sendMessageQueue;
}

MessageQueue* CommunicateCore::GetRecvSetMessageQueue(void)
{
	return &this->recvSetMessageQueue;
}