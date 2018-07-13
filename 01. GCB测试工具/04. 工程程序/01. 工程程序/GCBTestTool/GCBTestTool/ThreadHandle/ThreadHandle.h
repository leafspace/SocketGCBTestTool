#pragma once

#include <windows.h>

DWORD WINAPI ThreadSocketLinkConn(LPVOID lpParamter);                       // 连接测试线程的信号
DWORD WINAPI ThreadSocketLinkRecv(LPVOID lpParamter);                       // 接收数据线程的信号
DWORD WINAPI ThreadSocketLinkSend(LPVOID lpParamter);                       // 发送数据线程的信号
