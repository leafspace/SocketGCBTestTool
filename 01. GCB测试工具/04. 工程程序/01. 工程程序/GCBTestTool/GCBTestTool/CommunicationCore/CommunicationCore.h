#pragma once

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))                            // 获取某个数的第一个字节 从最低位开始
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))                      // 获取某个数的第二个字节 从最低位开始
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))                      // 获取某个数的第三个字节 从最低位开始
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))                      // 获取某个数的第四个字节 从最低位开始

#define LIST_NUM			7                                               // 一个GCB板需要监控的表格个数

#include "../SocketLink/SocketLink.h"
#include "../StateTable/StateTable.h"
#include "../SocketLink/MessageQueue/MessageQueue.h"

class CommunicateCore
{
private:
	SocketLink socketLink;                                                  // 建立的Socket连接

	HANDLE hThreadSocketLinkTest;                                           // 线程句柄
	HANDLE hThreadSocketLinkRecv;                                           // 线程句柄
	HANDLE hThreadSocketLinkSend;                                           // 线程句柄

	StateTable threadStateTable;                                            // 线程运行状态表
	MessageQueue recvMessageQueue;                                          // 该Socket连接接收到的消息队列
	MessageQueue sendMessageQueue;                                          // 该Socket连接未发送的消息队列

	static uint16_t uRequestBeginAddress[LIST_NUM];
	static uint16_t uRequestDriveNums[LIST_NUM];
	static bool bRequestFlag[LIST_NUM];
public:
	CommunicateCore(void);
	~CommunicateCore(void);

	static list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum);    // 创建一条要发送的命令
	static list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const float uReadNum);       // 创建一条要发送的命令

	static float Make4ByteFloat(const BYTE *list);                          // 将字节流转换为四个字节的数据
	static float MakeTurn4ByteFloat(const BYTE *list);                      // 将逆转的字节流转换为四个字节的数据
	static CString GetFormatStrLable(const FRAME_CMD_TYPE cmdType, const float value);                              // 根据指令种类，格式化数据最后显示的字符串

	static FRAME_CMD_TYPE GetCMDIdFromIndex(const int nIndex);
	static int GetIndexFromCMDId(const FRAME_CMD_TYPE cmdType);

	static bool SetRequestBeginAddress(int nIndex, uint16_t uAddress);
	static bool SetRequestDriveNums(int nIndex, uint16_t uDriveNums);
	static bool SetRequestFlag(int nIndex, bool bFlag);

	static void GetTimeStr(CString *timeStr);
	static void WriteTimeStr(ofstream &outfile);

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

	bool InisSocketLink(string ipAddress, int port);
	bool InisSocketLink(CString ipAddress, int port);

	bool SendRequestMessage(void);                                          // 发送一次请求消息
	bool SendRequestMessage(const MessageBean beanMessage);                 // 发送一个消息
	bool RecvResponseMessage(MessageBean *beanMessage);

	bool HaveRecvMessageResidual(void);
	bool HaveSednMessageResidual(void);

	short GetFormatSize(const FRAME_CMD_TYPE cmdType);                      // 获取格式化的大小
	bool WriteLog(ofstream &outfile, MessageBean beanMessage, CString *timeStr, list<float> &retValueLst);    // 将消息内容写入日志

	bool CreateSocketLinkTestThread(void);                                  // 创建线程
	bool CreateSocketLinkRecvThread(void);                                  // 创建线程
	bool CreateSocketLinkSendThread(void);                                  // 创建线程

	bool CloseSocketLinkTestThread(void);                                   // 关闭线程
	bool CloseSocketLinkRecvThread(void);                                   // 关闭线程
	bool CloseSocketLinkSendThread(void);                                   // 关闭线程

	int GetSocketLinkTestThreadState(void);                                 // 获取线程状态
	int GetSocketLinkRecvThreadState(void);                                 // 获取线程状态
	int GetSocketLinkSendThreadState(void);                                 // 获取线程状态

	int GetSocketLinkTestThreadRetValue(void);                              // 获取线程返回值
	int GetSocketLinkRecvThreadRetValue(void);                              // 获取线程返回值
	int GetSocketLinkSendThreadRetValue(void);                              // 获取线程返回值

	void RemoveSocketLinkTestThreadSteAndVlu(void);
	void RemoveSocketLinkRecvThreadSteAndVlu(void);
	void RemoveSocketLinkSendThreadSteAndVlu(void);
};
