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

	FRAME_CMD_TYPE cmdType;                                                 // 命令类型
	uint16_t uParameterSize;                                                // 参数大小
	list<BYTE> parameterLst;                                                // 参数列表

	int FindPackgeHeadPosition();                                           // 找到包头的位置
	int FindPackgeTailPosition(const int nBeIndex);                         // 找到包尾的位置
public:
	MessageBean() {}
	MessageBean(const BYTE *bMessageLst, const uint16_t uMessageSize);

	static uint16_t MakeUINT16Value(list<BYTE>::iterator iter, list<BYTE>::iterator iterEnd);                       // 将BYTE流制作一个16bit数据
	static uint32_t MakeUINT32Value(list<BYTE>::iterator iter, list<BYTE>::iterator iterEnd);                       // 将BYTE流制作一个32bit数据

	static bool JumpUINT16(list<BYTE>::iterator &iter, list<BYTE>::iterator iterEnd);                               // BYTE流跳跃16bit
	static bool JumpUINT32(list<BYTE>::iterator &iter, list<BYTE>::iterator iterEnd);                               // BYTE流跳跃32bit

	void SetOrginDataList(list<BYTE> bMessageLst);                          // 设置原始字节流
	void SetOrginDataList(const BYTE *bMessageLst, const uint16_t uMessageSize);
	void SetCMDType(const FRAME_CMD_TYPE cmdType);                          // 设置CMDType
	void SetParameterSize(const uint16_t uParameterSize);                   // 设置参数大小
	void SetParameterList(const list<BYTE> parameterLst);                   // 设置参数流

	list<BYTE> GetOrginDataList(void);                                      // 获取原始字节流
	FRAME_CMD_TYPE GetCMDType(void);                                        // 获取指令类型
	uint16_t GetParameterSize(void);                                        // 获取参数大小
	list<BYTE> GetParameterList(void);                                      // 获取参数流

	PROGRAM_STATE_CODE AnalysisOrginDataLst();                              // 解析原始的数据流
	PROGRAM_STATE_CODE AnalysisOrginDataLst(const BYTE *bMessageLst, const uint16_t uMessageSize);

	MessageBean operator=(const MessageBean &messageBean);
};