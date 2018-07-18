#pragma once
#include <afx.h>

enum FRAME_CMD_TYPE
{
	NOZZLE_CARTRIDGE_LEVEL = 0x20,
	MODE_LOCKED_SOLENOID_VALVE_WORKING = 0x21,
	POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING = 0x22,
	INK_SUPPLY_PUMP_WORKING_CONDITION = 0x23,
	NOZZLE_CABINET_TEMPERATURE = 0x40,
	AIR_NEGATIVE_PRESSURE_VALUE = 0x41,
	AIR_POSITIVE_PRESSURE_VALUE = 0x42,
	NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE = 0x60,
	INK_MOTOR_DELAY_TIME = 0x61,
	MODE_PRESSURE_NOZZLE_INK = 0x50,
	START_WORKING_STATE_CIRCULATING_MOTOR = 0x51,
	MANUALLY_OPEN_MODE_LOCKED_SOLENOID_VALVE_STATUS = 0x52,
};


enum COMM_RESULT_GCB
{
	gcb_comm_result_successed = 0x00,	                // 指令正常执行完成
	gcb_comm_result_failed = 0x01,            			// 指令操作失败
	gcb_comm_result_invalid = 0x02,			            // 无效命令
	gcb_comm_result_data = 0x03,            			// 参数数据错误
	gcb_comm_result_other = 0xFF,		            	// 其它错误，如系统正忙，如操作步骤错误
};

enum PROGRAM_STATE_CODE
{
	PROGRAM_CANT_LINK_SERVER = 0x00,							// 【TIMER_SOCKET_LINK_TEST】无法连接服务器
	PROGRAM_LINK_SERVER,										// 【TIMER_SOCKET_LINK_TEST】连接服务器成功
	PROGRAM_UNDEFINE_CMD,										// 未定义的CMD类型
	PROGRAM_CANT_ANALYSIS,										// 无法解析原始命令码
	PROGRAM_ANALYSIS_ORGIN_DATA,								// 解析原始命令码成功
};

enum PROGRAM_STATE_TYPE                                 // MessageBox的三种状态
{
	PROGRAM_STATE_ERROR = 0x00,
	PROGRAM_STATE_WARNING,
	PROGRAM_STATE_TIP,
};

enum TIMER_TYPE
{
	TIMER_SOCKET_LINK_CONN = 0x00,                      // 进行与服务器的连接测试
	TIMER_SOCKET_LINK_RECV,
	TIMER_SOCKET_LINK_SEND,


	TIMER_DIALOG_DRAW,
};

enum TIMER_STATE_TYPE
{
	TIMER_STATE_UNRUNNING = 0x00,                       // 线程尚未启动
	TIMER_STATE_RUNNING,                                // 线程正在运行
	TIMER_STATE_FINISH,                                 // 线程运行结束
	TIMER_STATE_ERROR = 0xFF,                           // 线程出现错误
};

enum TIMER_STATE_ERROR_TYPE
{
	TIMER_STATE_ERROR_TYPE_BREAKNET = 0x0100,

};

extern const CString strLabel[];                                            // 12种不同测试对象的文言
extern const CString strStateCodeMessage[];                                 // 系统状态对应的错误信息或提示信息

int GetLabelSize();
int GetStateCodeMessageSize();

CString GetLabel(const unsigned int nIndex);
CString GetStateCodeMessage(const unsigned int nIndex);