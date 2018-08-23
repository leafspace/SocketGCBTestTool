///////////////////////////////////////////////////////////////////////////////////////////////////
// 数据帧格式定义
//
// 数据帧格式	0xE0，0xE1，src，dst，cmd，cmdCount，cmdSize，[data]，[data1]，[data2]，[…]，0xFC，CRC位
// 单字节指令	指令名		cmd		说明
//				Check帧		0xE2	当前系统是否正常连接（PC <-->MB）
//				OK帧		0xE3	当前接收到的指令串（数据帧）格式正常。
//				Fail帧		0xE4	当前接收到的指令串（数据帧）格式错误或数据不全。
// 
// ●“0xE0, 0xE1”	：数据帧起始符；
// ●“src, dst”     	：指定指令串源地址、目标地址，当前板卡仅处理 dst 指向自己的指令
// ●“cmd”			：当前数据帧指令类型；
// ●“cmdCount”   : 当前指令发送次数
// ●“cmdSize”		：指令参数长度，表示其后的所有数据（data, data1, …, 不包括0xFC, CRC位）的个数；
// ●“data, data1, data2,...”	：指令参数，根据“cmd”不同，由“cmdSize”指定具体参数个数；
// ●“0xFC”		：数据帧结束符
// ●“CRC位”		：为简单CRC校验和值，包括0xE0, 0xE1, …, 0xFC 的所有数据简单相加和值
//						（仅限于单字节运算，相加后如超过0xFF则仅取低字节数据。
//						例：“0xE0+0xE1+0x01+0x00+0xFC”= 0x2BE --> CRC=0xBE）；
// ●数据帧（除单字节指令外）通讯过程发送后均要等待“OK”（通讯成功）或“Fail”（通讯失败）确认（由主控制板返回，单字节指令），
//		同时在接收方接收并处理完成后必须以原指令“cmd”作为反馈信号标识发送给发送方进行确认。
// ●在接收到“OK帧， Fail 帧”时，接收方不需要进行反馈，而接收到其它的单字节指令（如 Check帧）时，直接返回相同的单字节指令即可；
//
//以下所有定义均以十六进制表示，各数据帧说明中仅包含 “cmd, cmdSize, [data], [data1], ...”。不同的数据类型值传输时必须遵循以下规则：
//  ●所有数据均以连续的8位整型值（BYTE）数据方式进行传输，对于指令串中的16位或32位数值通过分拆/合并进行；
//  ●8位整型值(Byte)	：传输时直接使用参数值；
//  ●16位整型值(Word)	：传输时顺序为[LoByte][HiByte]，即高8位在后，低8位在前，
//						最后合并为一个16位整型数据 Data = [HiByte<<8] | [LoByte]；
//  ●32位整型值(Dword)	：传输时顺序为[LoWord][HiWord]，即高16位在后，低16位在前，
//						最后合并为一个32位整型数据 Data = [HiWord<<16] | [LoWord]，
//						其中每个Word均按 16位整型值方式组合，即完整的传输时顺序为 
//						[LoWord_LoByte][LoWord_HiByte][HiWord_LoByte][HiWord_HiByte]；
//  ●32位浮点型（float）：与32位整型传输格式一样。
//
//	详见详细设计文档内容
//
// Created by hzm 2011-03-22
// Modified by hzm 2013-02-17
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#define PLUGIN_MAX_COLOR		8		//最大颜色通道个数

//sizeof(pBits) >=1
#define PROTOCOL_MAKEDATA_BYTE(pBits)		((BYTE)(pBits[0]))
//sizeof(pBits) >= 2
#define PROTOCOL_MAKEDATA_WORD(pBits)		(((WORD)((pBits[1]) & 0xFF)<<8) | ((WORD)(pBits[0]) & 0xFF))
//sizeof(pBits) >= 4
#define PROTOCOL_MAKEDATA_DWORD(pBits)		(((DWORD)((pBits[3]) & 0xFF)<<24) | ((DWORD)((pBits[2]) & 0xFF)<<16) | ((DWORD)((pBits[1]) & 0xFF)<<8) | ((DWORD)((pBits[0]) & 0xFF)))
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

//=============================================================
// PC端主动发送
//=============================================================
#define protocol_command_ok						0xE3		//OK帧，单字节指令，当前接收到的指令串（数据帧）格式正常。
#define protocol_command_fail					0xE4		//Fail帧，单字节指令，当前接收到的指令串（数据帧）格式错误或数据不全。

// 数据帧格式	0xE0，0xE1，cmd，cmdSize，[data]，[data1]，[data2]，[…]，0xFC，CRC位
enum PROTOCOL_DATAFRAME_INDEX
{
	pt_data_start0	= 0,			//0xE0
	pt_data_start1	= 1,			//0xE1
	pt_data_add_src,				//src
	pt_data_add_dst,				//dst
	pt_data_cmd,					//cmd
	pt_data_cmdcount,				//cmdCount
	pt_data_cmdsize,				//cmdSize

	pt_data_startfixcount,			//数据帧起始段固定参数个数
	pt_data_endfixcount = 2,		//数据帧结束段固定参数个数

	pt_data_len_min	= pt_data_startfixcount+pt_data_endfixcount,			//数据帧最小长度（cmdSize=0，不包括单字节指令）
	pt_data_len_max = pt_data_len_min + 0xFF,			//数据帧最大长度（cmdSize=0xFF）
};
#define protocol_dataframe_start1				0xE0		//数据帧起始标识1
#define protocol_dataframe_start2				0xE1		//数据帧起始标识2
#define protocol_dataframe_end					0xFC		//数据帧结束标识

#define protocol_command_check					0xE2		//Check帧，单字节指令

//=================== 数据帧 ========================
#define protocol_command_getversion				0x01		//版本

#define protocol_command_getconfig_amplitude	0x11		//查询-电压设定值
#define protocol_command_getconfig_pulsewidth	0x12		//查询-脉宽设定值

// #define protocol_command_getstatus_amplitude	0x14		//查询-电压状态值
// #define protocol_command_getstatus_pulsewidth	0x15		//查询-脉宽状态值

#define protocol_command_getconfig_spray		0x16		//查询-散喷参数值
#define protocol_command_getconfig_cleanhead	0x17		//查询-清洗参数值

#define protocol_command_getconfig_thickness	0x18		//查询-布厚参数值

#define protocol_command_setconfig_amplitude	0x41		//设定-电压控制值
#define protocol_command_setconfig_pulsewidth	0x42		//设定-脉宽控制值

#define protocol_command_setconfig_spray		0x43		//设定-散喷控制值
#define protocol_command_setconfig_cleanhead	0x44		//设定-清洗控制值

#define protocol_command_setconfig_thickness	0x45		//设定-布厚参数值（含测试功能）

#define protocol_command_print_start			0x50		//打印-打印准备
#define protocol_command_print_line				0x51		//打印-打印一行
#define protocol_command_print_end				0x52		//打印-打印结束

#define protocol_command_pc_reset				0x53		//打印-复位
#define protocol_command_pc_pause				0x54		//打印-暂停
#define protocol_command_pc_resume				0x55		//打印-继续

#define protocol_command_print_back2origin		0x56		//打印-回到打印原点
#define protocol_command_print_cheanhead		0x57		//打印-执行一次自动清洗操作
#define protocol_command_print_spray			0x58		//打印-执行一次散喷操作

#define protocol_command_setconfig_moto_general	0x70		//运动-设定-公共配置参数
#define protocol_command_setconfig_moto_x		0x71		//运动-设置-X向配置参数
#define protocol_command_setconfig_moto_y		0x72		//运动-设置-Y向配置参数
#define protocol_command_setconfig_moto_z		0x73		//运动-设置-公共配置参数

#define protocol_command_getconfig_moto_general	0x74		//运动-查询-公共配置参数
#define protocol_command_getconfig_moto_x		0x75		//运动-查询-X向配置参数
#define protocol_command_getconfig_moto_y		0x76		//运动-查询-Y向配置参数
#define protocol_command_getconfig_moto_z		0x77		//运动-查询-公共配置参数


//========================================================
// MB主动发送
//========================================================
#define protocol_command_mb_reset				0x60		//异常-复位
#define protocol_command_mb_pause				0x61		//异常-暂停
#define protocol_command_mb_resume				0x62		//异常-继续

#define protocol_command_mb_syserror			0x64		//异常-系统错误
#define protocol_command_mb_sysstatus			0x65		//异常-系统异常状态

#define protocol_command_mb_cleanhead			0x66		//异常-喷头清洗


//PC-SF


#define cmd_sf_motor_move_relative				0x01	//移相对距离
#define cmd_sf_motor_move_absolute				0x02	//移动到绝对位置
#define cmd_sf_motor_get_position				0x03	//获取当前位置
#define cmd_sf_motor_move_stop					0x04	//停止移动	
#define cmd_sf_motor_move_special_position		0x05	//移动到特殊位置
#define cmd_sf_motor_move_offline				0x06	//脱机移动
#define cmd_sf_download_on_timetable_paramater	0x07	//下载联机时间表参数
#define	cmd_sf_get_on_timetable_pamarater		0x08	//获取联机时间表参数
#define cmd_sf_download_timetable_timevalue		0x09	//下载时间表数据
#define cmd_sf_get_existent_speedlevel			0x0A	//获取档位总数
#define cmd_sf_count_fullpulse					0x0B	//全程脉冲计数
#define	cmd_sf_get_fullpulse					0x0C	//得全程脉冲数

#define cmd_sf_get_special_position				0x0E	//获取特殊位置
#define cmd_sf_set_special_position				0x0F	//设置特殊位置

#define cmd_sf_get_timetable_vital_parameter	0x12	//得时间表重要参数
#define cmd_sf_delete_all_timetable_data		0x13	//清空档位信息

#define cmd_sf_download_off_timetable_paramater 0x15	//下载脱机时间表参数
#define cmd_sf_get_off_timetable_pamarater		0x16	//获取脱机时间表参数
#define cmd_sf_set_firm_parameter				0x17	//设置固件参数
#define cmd_sf_get_firm_parameter				0x18	//获取固件参数

#define cmd_sf_motor_xy_move_relative			0x20	//XY联动 - 移动相对距离
#define cmd_sf_motor_xy_move_absolute			0x21	//XY联动 - 移动到绝对位置	
#define cmd_sf_motor_xy_move_special_position	0x22	//XY联动 - 移动到特殊位置
#define cmd_sf_motor_xy_print_line				0x23	//XY联动 - 打印一行

//MB
//#define cmd_mb_online_state	0x02	//设置PC的联机状态

//#define cmd_mb_getconfig_cleanhead	0x17	//查询--清洗
//#define cmd_mb_setconfig_cleanhead	0x44	//设置--清洗
//#define cmd_mb_print_cleanhead	0x57	//执行--清洗
//
//#define cmd_mb_getconfig_thickness 0x18	//查询--布厚
//#define cmd_mb_setconfig_thickness 0x45	//设置--布厚
//
//#define cmd_mb_getconfig_spray	0x19	//查询--散喷
//#define cmd_mb_setconfig_spray	0x46	//设置--散喷

#define cmd_mb_print_start 0x20		//打印准备
#define cmd_mb_print_line 0x21		//打印一行
#define cmd_mb_print_end 0x22		//打印结束
#define cmd_mb_print_stop_gcb	0x27	//打印终止
#define cmd_mb_print_pause_gcb	0x28	//打印暂停
#define	cmd_mb_print_resume_gcb	0x29	//打印继续
#define cmd_mb_move_absolute_position	0x23	//移动到绝对位置
#define cmd_mb_move_relative_position	0x24	//移动相对位置
#define cmd_mb_move_to_special_position	0x25	//移动到特殊位置
#define cmd_mb_store_current_position	0x26	//保存当前位置

#define cmd_gcb_online_state	0x02	//设置联机状态

//板卡
#define cmd_card_setinfo	0xB1	//设置板卡类型
#define cmd_card_queryinfo	0xB2	//查询板卡类型
#define cmd_card_setaddr	0xB3	//设置板卡地址
#define	cmd_card_query		0xB4	//查询板卡地址

//供墨 收布 送布 纠偏
#define	cmd_ink_check_485	0x12	//通讯自检
#define cmd_ink_check_state	0x13	//查询状态
#define cmd_ink_force		0x14	//强制供墨
#define cmd_ink_press		0x15	//压墨
#define cmd_ink_scrape		0x16	//刮墨
#define cmd_ink_moisturize  0x17	//保湿
#define cmd_ink_cycle_motor	0x18	//循环电机

#define cmd_rc_check_state	0x13	//查询状态
#define cmd_rc_pass			0x20	//Pass命令
#define cmd_rc_print_stop	0x21	//打印暂停
#define cmd_rc_print_over	0x22	//打印结束

#define cmd_sc_check_state	0x13	//查询状态
#define cmd_sc_band_dry		0x20	//导带烘干
#define cmd_sc_pre_dry		0x21	//布预烘
#define cmd_sc_band_wash	0x22	//导带清洗
#define cmd_sc_press_roller	0x23	//压布辊
#define cmd_sc_motor		0x24	//送布电机开关
#define cmd_sc_stop			0x25	//送布暂停
#define cmd_sc_cancle_wrong_lable 0x26	//取消错误位

#define cmd_cb_auto_correct	0x20	//自动纠偏
#define cmd_cb_manual_correct	0x21	//手动纠偏
#define cmd_cb_manual_correct_stop 0x23	//手动纠偏停止

//COM
#define cmd_com_motor_move_relative 0x30	//移动相对距离
#define cmd_com_get_board_condition 0x09	//查询各板卡工作情况

//RAS
#define cmd_ras_print_start 0x20		//打印准备
#define cmd_ras_print_line 0x21		//打印一行
#define cmd_ras_print_end 0x22		//打印结束
#define cmd_ras_select 0xA1		//PC切换
#define cmd_ras_print_stop 0x2A
#define cmd_ras_print_pause 0x2B
#define cmd_ras_print_resume 0x2C
#define cmd_ras_sys_error	0x2D
#define cmd_ras_sys_exception	0x2E

#define cmd_ras_getspeed 0xA2
#define cmd_ras_getposition 0xA3
#define cmd_ras_print_stop_gcb 0x27
#define cmd_ras_move_absolute_position 0x23

//收布新协议
#define cmd_rc_servo_motor_move_distance 0x80 //伺服电机运动(带运行距离)
#define cmd_rc_up_down_motor_move 0x81//升降电机运动
#define cmd_rc_servo_motor_move 0x82//伺服电机运动(不带距离)
#define cmd_rc_heater_con	0x83//加热管控制
#define cmd_rc_cloth_motor_con	0x84//收布电机控制
#define cmd_rc_up_down_motor_reset  0x85//升降电机复位

//送布新协议
#define cmd_sc_new_sendcloth_check_state 0x13 //查询状态
#define cmd_sc_new_band_dry 0x52 //导带烘干
#define cmd_sc_new_band_wash 0x51 //供水阀
#define cmd_sc_new_pre_dry 0x53 //布预烘
#define cmd_sc_new_dd_band_wash 0x54 //导带清洗
#define cmd_sc_new_get_temperature 0x56 //获取温度
#define cmd_sc_new_press_cloth 0x5A //压布辊上下移动
#define cmd_sc_new_cloth_roller 0x50 //压布辊前后移动
#define cmd_sc_new_cancle_wrong_lable 0x57 //取消错误位
#define cmd_sc_new_manual_correct 0x5B //手动纠偏
#define cmd_sc_new_manual_correct_stop 0x5C //手动纠偏停止

//LD
#define cmd_ld_set_check_power				0x09		//设置单一激光功率
#define cmd_ld_set_laser_test_power			0x18		//设置微光功率检测电压
// #define cmd_ld_set_all_power				0x11		//设置所有激光功率
// #define cmd_ld_get_all_power				0x12		//查询激光当前功率
// #define cmd_ld_print_check_power			0x13		//执行激光功率检测

#define	cmd_pc_set_power_status				0x16		//通道激光功率设置状态反馈

#define cmd_ld_power_set_pc					0x15
#define cmd_ld_power_set_ld					0x16
#define cmd_ld_power_get_last				0x12
#define cmd_ld_power_get_now				0x13
#define cmd_ld_power_correct				0x14

#define cmd_ld_light_on						0xc0		//指定光路开
#define cmd_ld_light_off					0xc1		//指定光路关
#define cmd_ld_light_on_time				0x17

#define cmd_ld_get_current_power			0x50		//查询当前激光实际功率

//PD
#define cmd_get_pd_mapdata					0x07		//得映射表
#define cmd_set_pd_mapdata					0x08		//设映射表
