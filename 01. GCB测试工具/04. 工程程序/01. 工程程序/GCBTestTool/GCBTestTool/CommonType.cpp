#include "stdafx.h"
#include "CommonType.h"

const CString strLabel[] = {
	_T("喷头墨盒液位状态"),
	_T("锁墨电磁阀工作状态"),
	_T("正负压电磁阀工作状态"),
	_T("供墨泵工作状态"),
	_T("喷头机柜温度"),
	_T("空气负压压力数值"),
	_T("空气正压压力数值"),
	_T("喷头负压压力输出值"),
	_T("供墨电机延时时间"),
	_T("模式压喷头墨水"),
	_T("开启循环电机工作状态"),
	_T("手动开启锁墨电磁阀状态"),
};

const CString strStateCodeMessage[] = {
	_T("无法与服务器进行连接"),
	_T("服务器连接成功"),
	_T("未定义的CMD指令"),
	_T("无法解析原始指令包"),
	_T("指令包解析成功"),
	_T("未连接服务器"),
	_T("成功写入消息队列"),
	_T("未成功写入消息"),
};


int GetLabelSize()
{
	return sizeof(strLabel) / sizeof(CString);
}

int GetStateCodeMessageSize()
{
	return sizeof(strStateCodeMessage) / sizeof(CString);
}

CString GetLabel(const unsigned int nIndex)
{
	if (nIndex < 0 || nIndex >(unsigned int) GetLabelSize()) {
		return _T("");
	}
	else {
		return strLabel[nIndex];
	}
}

CString GetStateCodeMessage(const unsigned int nIndex)
{
	if (nIndex < 0 || nIndex >(unsigned int) GetStateCodeMessageSize()) {
		return _T("");
	}
	else {
		return strStateCodeMessage[nIndex];
	}
}