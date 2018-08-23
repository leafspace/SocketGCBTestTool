///////////////////////////////////////////////////////////////////////////////////////////////////
//本节定义网络通讯时的数据帧格式，除“打印数据指令”不使用本节定义的帧格式外（直接数据传输），其它网络端口连接均统一使用本节定义。
//数据帧格式	0xF0,0xF1,cmd,cmdSize,[data],[data1],[data2],[…],0xEC
//
//  ●“0xF0, 0xF1”	：数据帧起始符；
//  ●“cmd”			：当前数据帧指令类型，BYTE；
//  ●“cmdSize”		：指令参数长度，WORD，表示其后的所有有效数据（data, data1, …, 不包括0xFC, CRC位）的个数；
//  ●“data, data1, data2,...”	：指令参数，BYTE，根据“cmd”不同，由“cmdSize”指定具体参数个数；
//  ●“0xEC”		：数据帧结束符
//  ●由于TCP本身具有完善的数据校验功能，因此只要数据传输成功即表示当前指令串正常发送，所有指令串均需要按协议格式要求返回操作状态信息（使用原 cmd 参数），以便于主控方（指令发送方）作出相应处理。

// cmdSize及 data 部分参数值遵循以下定义要求。不同的数据类型值传输时必须遵循以下规则：
//  ●所有数据均以连续的8位整型值（BYTE）数据方式进行传输，对于指令串中的16位或32位数值通过分拆/合并进行；
//  ●8位整型值(Byte)		：传输时直接使用参数值；
//  ●16位整型值(Word)	：传输时顺序为[LoByte][HiByte]，即高8位在后，低8位在前，最后合并为一个16位整型数据 Data = [HiByte<<8] | [LoByte]；
//  ●32位整型值(Dword)	：传输时顺序为[LoWord][HiWord]，即高16位在后，低16位在前，最后合并为一个32位整型数据 Data = [HiWord<<16] | [LoWord]，其中每个Word均按 16位整型值方式组合，即完整的传输时顺序为 [LoWord_LoByte][LoWord_HiByte][HiWord_LoByte][HiWord_HiByte]；
//  ●32位浮点型（float）：与32位整型传输格式一样。
//
// modify by hzm 2015-09-27
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//sizeof(pBits) >= 2
#define PROTOCOL_MAKEDATA_WORD(pBits)		WORD(((WORD)((pBits[1]) & 0xFF)<<8) | ((WORD)(pBits[0]) & 0xFF))
//sizeof(pBits) >= 4
#define PROTOCOL_MAKEDATA_DWORD(pBits)		DWORD(((DWORD)((pBits[3]) & 0xFF)<<24) | ((DWORD)((pBits[2]) & 0xFF)<<16) | ((DWORD)((pBits[1]) & 0xFF)<<8) | ((DWORD)((pBits[0]) & 0xFF)))
// dwData --> fData
#define PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(fData,dwData)	\
{	\
	DWORD dwTmpForTrans = (dwData);				\
	(fData) = *(float*)(&dwTmpForTrans);		\
}
// fData --> dwData
#define PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwData,fData)	\
{	\
	float fTmpForTrans = (fData);				\
	(dwData) = *(DWORD*)(&fTmpForTrans);		\
}

//sizeof(pBits) >= 2
#define PROTOCOL_GETDATA_WORD(wData,pBits)		\
{	\
	(pBits[0])	= (BYTE)(wData);				\
	(pBits[1])	= (BYTE)((WORD)(wData)>>8);		\
}
//sizeof(pBits) >= 4
#define PROTOCOL_GETDATA_DWORD(dwData,pBits)	\
{	\
	(pBits[0])	= (BYTE)(((DWORD)(dwData)) & 0xFF);			\
	(pBits[1])	= (BYTE)(((DWORD)(dwData)>>8) & 0xFF);		\
	(pBits[2])	= (BYTE)(((DWORD)(dwData)>>16) & 0xFF);		\
	(pBits[3])	= (BYTE)(((DWORD)(dwData)>>24) & 0xFF);		\
}

// 数据帧格式	0xF0,0xF1,cmd,cmdSize,[data],[data1],[data2],[…],0xEC
enum GCB_PROTOCOL_DATAFRAME_INDEX
{
	gcb_pt_data_start1	= 0,			//0xF0
	gcb_pt_data_start2	= 1,			//0xF1
	gcb_pt_data_cmd,					//cmd
	gcb_pt_data_cmdsize1,				//cmdSize = (cmdsize2<<8) | cmdsize1
	gcb_pt_data_cmdsize2,				//

	gcb_pt_data_startfixcount,			//数据帧起始段固定参数个数
	gcb_pt_data_endfixcount = 1,		//数据帧结束段固定参数个数

	gcb_pt_data_len_min	= gcb_pt_data_startfixcount+gcb_pt_data_endfixcount,			//数据帧最小长度（cmdSize=0，不包括单字节指令）
	gcb_pt_data_len_max = gcb_pt_data_len_min + 0xFFFF,			//数据帧最大长度（cmdSize=0xFFFF）
};
#define gcb_protocol_dataframe_start1				0xF0		//数据帧起始标识1
#define gcb_protocol_dataframe_start2				0xF1		//数据帧起始标识2
#define gcb_protocol_dataframe_end					0xEC		//数据帧结束标识


//======= GCB配置及打印控制指令 =======

// PC --> GCB --> PC
#define cmd_gcb_getversion			0x01		//版本
#define cmd_gcb_getbuffersize		0x02		//得缓冲区大小

#define cmd_gcb_init				0x10		//板卡初始化
#define cmd_gcb_print_ready			0x11		//打印准备

//======= 板卡配置指令 =======
#define cmd_gcb_repeater_config		0x30		//下级板卡配置指令（PC-->GCB-->板卡）
#define cmd_pc_repeater_config		0x31		//下级板卡配置指令（板卡-->GCB-->PC）

//======= 打印数据指令 =======
//无数据帧格式要求