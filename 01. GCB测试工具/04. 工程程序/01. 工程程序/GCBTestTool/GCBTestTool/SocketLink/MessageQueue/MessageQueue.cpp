#include "stdafx.h"
#include "MessageQueue.h"

bool MessageQueue::Push_back(const MessageBean beanMessage)
{
	if (this->IsFull()) {
		return false;
	}
	this->beanLst[this->tail] = beanMessage;
	this->tail = (this->tail + 1) % QUEUE_MAX_LEN;
	return true;
}

bool MessageQueue::Push_front(const MessageBean beanMessage)
{
	if (this->IsFull()) {
		return false;
	}

	this->head = (this->head - 1 + QUEUE_MAX_LEN) % QUEUE_MAX_LEN;
	this->beanLst[this->head] = beanMessage;
	return true;
}

bool MessageQueue::Pop_back(MessageBean *beanMessage)
{
	if (this->IsEmpty()) {
		return false;
	}

	this->tail = (this->tail - 1 + QUEUE_MAX_LEN) % QUEUE_MAX_LEN;
	*beanMessage = this->beanLst[this->tail];
	return true;
}

bool MessageQueue::Pop_front(MessageBean *beanMessage)
{
	if (this->IsEmpty()) {
		return false;
	}

	*beanMessage = this->beanLst[this->head];
	this->head = (this->head + 1) % QUEUE_MAX_LEN;
	return true;
}

bool MessageQueue::IsFull(void)
{
	if (this->GetSize() >= QUEUE_MAX_LEN) {
		return true;
	}
	else {
		return false;
	}
}

bool MessageQueue::IsEmpty(void)
{
	if (this->GetSize() == 0) {
		return true;
	}
	else {
		return false;
	}
}

int MessageQueue::GetSize(void)
{
	return (this->tail - this->head + QUEUE_MAX_LEN) % QUEUE_MAX_LEN;
}

void MessageQueue::Clear(void)
{
	this->head = 0;
	this->tail = 0;
}