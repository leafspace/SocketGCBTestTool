#pragma once
#include <hash_map>
#include "../CommonType.h"

using namespace std;
using namespace stdext;

class StateTable
{
private:
	hash_map<TIMER_TYPE, int> threadFinishedFlag;
	hash_map<TIMER_TYPE, int> threadRetValueFlag;
public:
	StateTable() {}

	bool InsertThreadFinishedFlag(const TIMER_TYPE key, const int value);   // 插入线程已经结束的状态
	bool InsertThreadRetValueFlag(const TIMER_TYPE key, const int value);   // 插入线程返回的数值

	bool SetThreadFinishedFlag(const TIMER_TYPE key, const int value);      // 设置线程已经结束的状态
	bool SetThreadRetValueFlag(const TIMER_TYPE key, const int value);      // 设置线程返回的数值

	int GetThreadFinishedFlag(const TIMER_TYPE key);                        // 获取线程已经结束的状态
	int GetThreadRetValueFlag(const TIMER_TYPE key);                        // 获取线程返回的数值

	bool RemoveThreadFinishedFlag(const TIMER_TYPE key);                    // 删除线程已经结束的状态
	bool RemoveThreadRetValueFlag(const TIMER_TYPE key);                    // 删除线程返回的数值

	void ClearThreadFinishedFlag(void);                                     // 清除所有的结束符标签
	void ClearThreadRetValueFlag(void);                                     // 清除所有的接收数据标签
	void ClearThreadAllFlag(void);                                          // 清除所有的标签
};