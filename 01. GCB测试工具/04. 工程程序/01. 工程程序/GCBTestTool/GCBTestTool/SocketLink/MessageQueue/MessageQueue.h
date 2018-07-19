#pragma once

#include "../MessageBean/MessageBean.h"
#define QUEUE_MAX_LEN   100

class MessageQueue
{
private:
	int head, tail;
	MessageBean beanLst[QUEUE_MAX_LEN];
public:
	MessageQueue() {
		this->Clear();
	}

	bool Push_back(const MessageBean beanMessage);
	bool Push_front(const MessageBean beanMessage);
	bool Pop_back(MessageBean *beanMessage);
	bool Pop_front(MessageBean *beanMessage);

	bool IsFull(void);
	bool IsEmpty(void);
	int GetSize(void);

	void Clear(void);
};