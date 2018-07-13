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

	bool InsertThreadFinishedFlag(const TIMER_TYPE key, const int value);
	bool InsertThreadRetValueFlag(const TIMER_TYPE key, const int value);

	bool SetThreadFinishedFlag(const TIMER_TYPE key, const int value);
	bool SetThreadRetValueFlag(const TIMER_TYPE key, const int value);

	int GetThreadFinishedFlag(const TIMER_TYPE key);
	int GetThreadRetValueFlag(const TIMER_TYPE key);

	bool RemoveThreadFinishedFlag(const TIMER_TYPE key);
	bool RemoveThreadRetValueFlag(const TIMER_TYPE key);
};