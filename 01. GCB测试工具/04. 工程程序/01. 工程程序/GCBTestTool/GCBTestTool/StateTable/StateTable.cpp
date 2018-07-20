#include "stdafx.h"
#include "StateTable.h"

bool StateTable::InsertThreadFinishedFlag(const TIMER_TYPE key, const int value)
{
	return this->SetThreadFinishedFlag(key, value);
}

bool StateTable::InsertThreadRetValueFlag(const TIMER_TYPE key, const int value)
{
	return this->SetThreadRetValueFlag(key, value);
}


bool StateTable::SetThreadFinishedFlag(const TIMER_TYPE key, const int value)
{
	this->threadFinishedFlag[key] = value;
	return true;
}

bool StateTable::SetThreadRetValueFlag(const TIMER_TYPE key, const int value)
{
	this->threadRetValueFlag[key] = value;
	return true;
}


int StateTable::GetThreadFinishedFlag(const TIMER_TYPE key)
{
	if (this->threadFinishedFlag.find(key) != this->threadFinishedFlag.end()) {
		int retValue = this->threadFinishedFlag[key];
		return retValue;
	}
	else {
		return TIMER_STATE_ERROR;
	}
}

int StateTable::GetThreadRetValueFlag(const TIMER_TYPE key)
{
	if (this->threadRetValueFlag.find(key) != this->threadRetValueFlag.end()) {
		int retValue = this->threadRetValueFlag[key];
		return retValue;
	}
	else {
		return TIMER_STATE_ERROR;
	}
}


bool StateTable::RemoveThreadFinishedFlag(const TIMER_TYPE key)
{
	return true;
}

bool StateTable::RemoveThreadRetValueFlag(const TIMER_TYPE key)
{
	return true;
}

void StateTable::ClearThreadFinishedFlag(void)
{
	this->threadFinishedFlag.clear();
}

void StateTable::ClearThreadRetValueFlag(void)
{
	this->threadRetValueFlag.clear();
}

void StateTable::ClearThreadAllFlag(void)
{
	this->ClearThreadFinishedFlag();
	this->ClearThreadRetValueFlag();
}