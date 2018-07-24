#pragma once
#include "../SocketLink/SocketLink.h"

class CommunicationCore
{
private:
    SocketLink socketLink;                                                  // 建立的Socket连接

    HANDLE hThreadSocketLinkTest;                                           // 线程句柄
	HANDLE hThreadSocketLinkRecv;                                           // 线程句柄
	HANDLE hThreadSocketLinkSend;                                           // 线程句柄
public:
    CommunicationCore();

    static StateTable threadStateTable;                                     // 线程运行状态表
    static MessageQueue recvMessageQueue;
	static MessageQueue sendMessageQueue;

	static list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum); // 创建一条要发送的命令
	static list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const float uReadNum);


    void SendRequestMessage(void);

    float Make4ByteFloat(BYTE *list);
	float MakeTurn4ByteFloat(BYTE *list);
	short GetFormatSize(FRAME_CMD_TYPE cmdType);
    CString GetFormatStrLable(FRAME_CMD_TYPE cmdType, float value);
    bool WriteLog(MessageBean beanMessage, CString *timeStr, list<float> &retValueLst);
};
