#include "stdafx.h"
#include "CommunicationCore.h"

CommunicationCore::CommunicationCore()
{
    this->InitCoreData();
}

~CommunicationCore::CommunicationCore()
{
    this->ClearCoreData();
}

void CommunicationCore::InitCoreData(void)
{
    this->ClearCoreData();
}

void CommunicationCore::ClearCoreData(void)
{
    this->CloseSocketLink();
    this->CloseAllThread();
    this->ClearQueueData();
}

void CommunicationCore::CloseSocketLink(void)
{
    this->socketLink.freeSocket();
}

void CommunicationCore::CloseAllThread(void)
{
    CloseHandle(this->hThreadSocketLinkTest);
    CloseHandle(this->hThreadSocketLinkTest);
    CloseHandle(this->hThreadSocketLinkTest);
}
void CommunicationCore::ClearQueueData(void)
{
    this->threadStateTable.ClearThreadAllFlag();
    this->recvMessageQueue.Clear();
    this->sendMessageQueue.Clear();
}

SocketLink* CommunicationCore::GetSocketLink(void)
{
    return &this->socketLink;
}

HANDLE* CommunicationCore::GetThreadLinkTestHandle(void)
{
    return &this->hThreadSocketLinkTest;
}

HANDLE* CommunicationCore::GetThreadLinkRecvHandle(void)
{
    return &this->hThreadSocketLinkRecv;
}

HANDLE* CommunicationCore::GetThreadLinkSendHandle(void)
{
    return &this->hThreadSocketLinkSend;
}

StateTable* CommunicationCore::GetStateTable(void)
{
    return &this->threadStateTable;
}

MessageQueue* CommunicationCore::GetRecvMessageQueue(void)
{
    return &this->recvMessageQueue;
}

MessageQueue* CommunicationCore::GetSendMessageQueue(void)
{
    return &this->sendMessageQueue;
}