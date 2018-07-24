#pragma once
#include "../SocketLink/SocketLink.h"

class CommunicationCore
{
private:
    SocketLink socketLink;                                                  // 建立的Socket连接

    HANDLE hThreadSocketLinkTest;                                           // 线程句柄
	HANDLE hThreadSocketLinkRecv;                                           // 线程句柄
	HANDLE hThreadSocketLinkSend;                                           // 线程句柄

    StateTable threadStateTable;                                            // 线程运行状态表
    MessageQueue recvMessageQueue;                                          // 该Socket连接接收到的消息队列
	MessageQueue sendMessageQueue;                                          // 该Socket连接未发送的消息队列
public:
    CommunicationCore();
    ~CommunicationCore();

	static list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum);    // 创建一条要发送的命令
	static list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const float uReadNum);       // 创建一条要发送的命令

    static float Make4ByteFloat(BYTE *list);                                // 将字节流转换为四个字节的数据
	static float MakeTurn4ByteFloat(BYTE *list);                            // 将逆转的字节流转换为四个字节的数据
    static CString GetFormatStrLable(FRAME_CMD_TYPE cmdType, float value);  // 根据指令种类，格式化数据最后显示的字符串

    static FRAME_CMD_TYPE GetCMDIdFromIndex(int nIndex);
    static int GetIndexFromCMDId(FRAME_CMD_TYPE cmdType);

    void InitCoreData(void);                                                // 初始化核心数据
    void ClearCoreData(void);                                               // 清除核心数据

    void CloseSocketLink(void);                                             // 关闭Socket连接
    void CloseAllThread(void);                                              // 关闭所有的线程
    void ClearQueueData(void);                                              // 清除所有保存的数据


    SocketLink* GetSocketLink(void);                                        // 获取Socket连接对象
    HANDLE* GetThreadLinkTestHandle(void);                                  // 获取线程句柄
    HANDLE* GetThreadLinkRecvHandle(void);                                  // 获取线程句柄
    HANDLE* GetThreadLinkSendHandle(void);                                  // 获取线程句柄
    StateTable* GetStateTable(void);                                        // 获取状态表对象
    MessageQueue* GetRecvMessageQueue(void);                                // 获取消息队列对象
    MessageQueue* GetSendMessageQueue(void);                                // 获取消息队列对象

    void SendRequestMessage(void);                                          // 发送一次请求消息
	short GetFormatSize(FRAME_CMD_TYPE cmdType);                            // 获取格式化的大小
    bool WriteLog(ofstream outfile, MessageBean beanMessage, CString *timeStr, list<float> &retValueLst);           // 将消息内容写入日志
};
