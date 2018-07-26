#include "stdafx.h"
#include "MessageBean.h"

MessageBean::MessageBean(const BYTE *bMessageLst, const uint16_t uMessageSize)
{
	this->SetOrginDataList(bMessageLst, uMessageSize);
}

int MessageBean::FindPackgeHeadPosition()
{
	int nBeIndex = 0;
	if (this->orginDataLst.size() <= 0) {
		return -1;
	}

	for (list<BYTE>::iterator iter = this->orginDataLst.begin();
		iter != this->orginDataLst.end(); iter++, nBeIndex++) {
		list<BYTE>::iterator tempIter = iter;
		tempIter++;
		if (*iter == 0xF0 && tempIter != this->orginDataLst.end()
			&& *tempIter == 0xF1) {
			return nBeIndex;
		}
	}

	return -1;
}

int MessageBean::FindPackgeTailPosition(const int nBeIndex)
{
	int nEnIndex = 0;
	list<BYTE>::iterator iter = this->orginDataLst.begin();
	for (; nEnIndex != nBeIndex; iter++, nEnIndex++) {}

	for (; iter != this->orginDataLst.end(); iter++, nEnIndex++) {
		if (*iter == 0xEC) {
			return nEnIndex;
		}
	}

	return -1;
}

uint16_t MessageBean::MakeUINT16Value(list<BYTE>::iterator iter, list<BYTE>::iterator iterEnd)
{
	BYTE lowByte = 0x00, heiByte = 0x00;
	if (iter == iterEnd) {
		return 0x0000;
	}

	lowByte = (*iter);

	iter++;
	if (iter == iterEnd) {
		return (uint16_t)lowByte;
	}

	heiByte = (*iter);

	return lowByte + heiByte * 0x100;
}

uint32_t MessageBean::MakeUINT32Value(list<BYTE>::iterator iter, list<BYTE>::iterator iterEnd)
{
	uint16_t lowWord = 0x0000, heiWord = 0x0000;

	if (iter == iterEnd) {
		return 0x0000;
	}

	lowWord = MessageBean::MakeUINT16Value(iter, iterEnd);

	MessageBean::JumpUINT16(iter, iterEnd);

	heiWord = MessageBean::MakeUINT16Value(iter, iterEnd);

	return lowWord + heiWord * 0x10000;
}

bool MessageBean::JumpUINT16(list<BYTE>::iterator &iter, list<BYTE>::iterator iterEnd)
{
	for (int nIndex = 0; nIndex < 2; nIndex++) {
		iter++;
		if (iter == iterEnd) {
			return false;
		}
	}

	return true;
}

bool MessageBean::JumpUINT32(list<BYTE>::iterator &iter, list<BYTE>::iterator iterEnd)
{
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		iter++;
		if (iter == iterEnd) {
			return false;
		}
	}

	return true;
}

void MessageBean::SetOrginDataList(list<BYTE> bMessageLst)
{
	this->orginDataLst = bMessageLst;
}

void MessageBean::SetOrginDataList(const BYTE *bMessageLst, const uint16_t uMessageSize)
{
	for (uint16_t uIndex = 0; uIndex < uMessageSize; ++uIndex) {
		this->orginDataLst.push_back(bMessageLst[uIndex]);
	}
}

void MessageBean::SetCMDType(const FRAME_CMD_TYPE cmdType)
{
	this->cmdType = cmdType;
}

void MessageBean::SetParameterSize(const uint16_t uParameterSize)
{
	this->uParameterSize = uParameterSize;
}

void MessageBean::SetParameterList(const list<BYTE> parameterLst)
{
	this->parameterLst = parameterLst;
}

list<BYTE> MessageBean::GetOrginDataList(void)
{
	return this->orginDataLst;
}

FRAME_CMD_TYPE MessageBean::GetCMDType(void)
{
	return this->cmdType;
}

uint16_t MessageBean::GetParameterSize(void)
{
	return this->uParameterSize;
}

list<BYTE> MessageBean::GetParameterList(void)
{
	return this->parameterLst;
}

PROGRAM_STATE_CODE MessageBean::AnalysisOrginDataLst()
{
	if (this->orginDataLst.size() <= 0) {
		return PROGRAM_CANT_ANALYSIS;
	}

	// 检查指令中是否包含0xF0，0xF1关键指令
	int nBeIndex = this->FindPackgeHeadPosition();
	if (nBeIndex < 0) {
		return PROGRAM_CANT_ANALYSIS;
	}

	// 检查指令中是否包含0xEC指令
	int nEnIndex = this->FindPackgeTailPosition(nBeIndex);
	if (nEnIndex < 0) {
		return PROGRAM_CANT_ANALYSIS;
	}

	// 解析指令包中的各个部分
	int nIndex = 0;
	list<BYTE>::iterator iter = this->orginDataLst.begin();
	for (; nIndex != nBeIndex; iter++, nIndex++) {}

	iter++; // 0xF0
	iter++; // 0xF1

	BYTE uCMD = *(iter++);
	uint16_t uSize = MessageBean::MakeUINT16Value(iter, this->orginDataLst.end());
	bool bIsSuccess = MessageBean::JumpUINT16(iter, this->orginDataLst.end());
	if (bIsSuccess == false) {
		return PROGRAM_CANT_ANALYSIS;
	}

	uint16_t uRealSize = 0;
	list<BYTE> parameterLst;
	for (nIndex += 5; nIndex < nEnIndex; ++nIndex, ++uRealSize, ++iter) {
		parameterLst.push_back(*iter);
	}

	if (parameterLst.size() != uRealSize) {
		return PROGRAM_CANT_ANALYSIS;
	}

	this->cmdType = (FRAME_CMD_TYPE)uCMD;
	this->uParameterSize = uRealSize;
	this->parameterLst = parameterLst;


	return PROGRAM_ANALYSIS_ORGIN_DATA;
}

PROGRAM_STATE_CODE MessageBean::AnalysisOrginDataLst(const BYTE *bMessageLst, const uint16_t uMessageSize)
{
	this->SetOrginDataList(bMessageLst, uMessageSize);
	return this->AnalysisOrginDataLst();
}

MessageBean MessageBean::operator=(const MessageBean &messageBean)
{
	this->orginDataLst = messageBean.orginDataLst;
	this->cmdType = messageBean.cmdType;
	this->uParameterSize = messageBean.uParameterSize;
	this->parameterLst = messageBean.parameterLst;
	return *this;
}