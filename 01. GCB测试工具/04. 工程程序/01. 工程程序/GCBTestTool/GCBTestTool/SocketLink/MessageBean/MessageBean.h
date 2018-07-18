#pragma once
#include <list>
#include "../SocketLink.h"
#include "../../CommonType.h"

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;

class MessageBean
{
private:
	list<BYTE> orginDataLst;                                                // 原始接收的数据流

	FRAME_CMD_TYPE cmdType;
	uint16_t uParameterSize;
	list<BYTE> parameterLst;

	uint16_t MakeUINT16Value(list<BYTE>::iterator iter, list<BYTE>::iterator iterEnd);
	uint32_t MakeUINT32Value(list<BYTE>::iterator iter, list<BYTE>::iterator iterEnd);

	bool JumpUINT16(list<BYTE>::iterator &iter, list<BYTE>::iterator iterEnd);
	bool JumpUINT32(list<BYTE>::iterator &iter, list<BYTE>::iterator iterEnd);

	int FindPackgeHeadPosition();
	int FindPackgeTailPosition(const int nBeIndex);
public:
	MessageBean() {}
	MessageBean(const BYTE *bMessageLst, const uint16_t uMessageSize);

	void SetOrginDataList(list<BYTE> bMessageLst);
	void SetOrginDataList(const BYTE *bMessageLst, const uint16_t uMessageSize);
	void SetCMDType(const FRAME_CMD_TYPE cmdType);
	void SetParameterSize(const uint16_t uParameterSize);
	void SetParameterList(const list<BYTE> parameterLst);

	list<BYTE> GetOrginDataList(void);
	FRAME_CMD_TYPE GetCMDType(void);
	uint16_t GetParameterSize(void);
	list<BYTE> GetParameterList(void);

	PROGRAM_STATE_CODE AnalysisOrginDataLst();
	PROGRAM_STATE_CODE AnalysisOrginDataLst(const BYTE *bMessageLst, const uint16_t uMessageSize);

	MessageBean operator=(const MessageBean &messageBean);
};