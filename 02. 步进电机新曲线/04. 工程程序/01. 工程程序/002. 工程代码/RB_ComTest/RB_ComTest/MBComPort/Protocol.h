///////////////////////////////////////////////////////////////////////////////////////////////////
// ����֡��ʽ����
//
// ����֡��ʽ	0xE0��0xE1��src��dst��cmd��cmdCount��cmdSize��[data]��[data1]��[data2]��[��]��0xFC��CRCλ
// ���ֽ�ָ��	ָ����		cmd		˵��
//				Check֡		0xE2	��ǰϵͳ�Ƿ��������ӣ�PC <-->MB��
//				OK֡		0xE3	��ǰ���յ���ָ�������֡����ʽ������
//				Fail֡		0xE4	��ǰ���յ���ָ�������֡����ʽ��������ݲ�ȫ��
// 
// ��0xE0, 0xE1��	������֡��ʼ����
// ��src, dst��     	��ָ��ָ�Դ��ַ��Ŀ���ַ����ǰ�忨������ dst ָ���Լ���ָ��
// ��cmd��			����ǰ����ָ֡�����ͣ�
// ��cmdCount��   : ��ǰָ��ʹ���
// ��cmdSize��		��ָ��������ȣ���ʾ�����������ݣ�data, data1, ��, ������0xFC, CRCλ���ĸ�����
// ��data, data1, data2,...��	��ָ����������ݡ�cmd����ͬ���ɡ�cmdSize��ָ���������������
// ��0xFC��		������֡������
// ��CRCλ��		��Ϊ��CRCУ���ֵ������0xE0, 0xE1, ��, 0xFC ���������ݼ���Ӻ�ֵ
//						�������ڵ��ֽ����㣬��Ӻ��糬��0xFF���ȡ���ֽ����ݡ�
//						������0xE0+0xE1+0x01+0x00+0xFC��= 0x2BE --> CRC=0xBE����
// ������֡�������ֽ�ָ���⣩ͨѶ���̷��ͺ��Ҫ�ȴ���OK����ͨѶ�ɹ�����Fail����ͨѶʧ�ܣ�ȷ�ϣ��������ư巵�أ����ֽ�ָ���
//		ͬʱ�ڽ��շ����ղ�������ɺ������ԭָ�cmd����Ϊ�����źű�ʶ���͸����ͷ�����ȷ�ϡ�
// ���ڽ��յ���OK֡�� Fail ֡��ʱ�����շ�����Ҫ���з����������յ������ĵ��ֽ�ָ��� Check֡��ʱ��ֱ�ӷ�����ͬ�ĵ��ֽ�ָ��ɣ�
//
//�������ж������ʮ�����Ʊ�ʾ��������֡˵���н����� ��cmd, cmdSize, [data], [data1], ...������ͬ����������ֵ����ʱ������ѭ���¹���
//  ���������ݾ���������8λ����ֵ��BYTE�����ݷ�ʽ���д��䣬����ָ��е�16λ��32λ��ֵͨ���ֲ�/�ϲ����У�
//  ��8λ����ֵ(Byte)	������ʱֱ��ʹ�ò���ֵ��
//  ��16λ����ֵ(Word)	������ʱ˳��Ϊ[LoByte][HiByte]������8λ�ں󣬵�8λ��ǰ��
//						���ϲ�Ϊһ��16λ�������� Data = [HiByte<<8] | [LoByte]��
//  ��32λ����ֵ(Dword)	������ʱ˳��Ϊ[LoWord][HiWord]������16λ�ں󣬵�16λ��ǰ��
//						���ϲ�Ϊһ��32λ�������� Data = [HiWord<<16] | [LoWord]��
//						����ÿ��Word���� 16λ����ֵ��ʽ��ϣ��������Ĵ���ʱ˳��Ϊ 
//						[LoWord_LoByte][LoWord_HiByte][HiWord_LoByte][HiWord_HiByte]��
//  ��32λ�����ͣ�float������32λ���ʹ����ʽһ����
//
//	�����ϸ����ĵ�����
//
// Created by hzm 2011-03-22
// Modified by hzm 2013-02-17
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#define PLUGIN_MAX_COLOR		8		//�����ɫͨ������

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
// PC����������
//=============================================================
#define protocol_command_ok						0xE3		//OK֡�����ֽ�ָ���ǰ���յ���ָ�������֡����ʽ������
#define protocol_command_fail					0xE4		//Fail֡�����ֽ�ָ���ǰ���յ���ָ�������֡����ʽ��������ݲ�ȫ��

// ����֡��ʽ	0xE0��0xE1��cmd��cmdSize��[data]��[data1]��[data2]��[��]��0xFC��CRCλ
enum PROTOCOL_DATAFRAME_INDEX
{
	pt_data_start0	= 0,			//0xE0
	pt_data_start1	= 1,			//0xE1
	pt_data_add_src,				//src
	pt_data_add_dst,				//dst
	pt_data_cmd,					//cmd
	pt_data_cmdcount,				//cmdCount
	pt_data_cmdsize,				//cmdSize

	pt_data_startfixcount,			//����֡��ʼ�ι̶���������
	pt_data_endfixcount = 2,		//����֡�����ι̶���������

	pt_data_len_min	= pt_data_startfixcount+pt_data_endfixcount,			//����֡��С���ȣ�cmdSize=0�����������ֽ�ָ�
	pt_data_len_max = pt_data_len_min + 0xFF,			//����֡��󳤶ȣ�cmdSize=0xFF��
};
#define protocol_dataframe_start1				0xE0		//����֡��ʼ��ʶ1
#define protocol_dataframe_start2				0xE1		//����֡��ʼ��ʶ2
#define protocol_dataframe_end					0xFC		//����֡������ʶ

#define protocol_command_check					0xE2		//Check֡�����ֽ�ָ��

//=================== ����֡ ========================
#define protocol_command_getversion				0x01		//�汾

#define protocol_command_getconfig_amplitude	0x11		//��ѯ-��ѹ�趨ֵ
#define protocol_command_getconfig_pulsewidth	0x12		//��ѯ-�����趨ֵ

// #define protocol_command_getstatus_amplitude	0x14		//��ѯ-��ѹ״ֵ̬
// #define protocol_command_getstatus_pulsewidth	0x15		//��ѯ-����״ֵ̬

#define protocol_command_getconfig_spray		0x16		//��ѯ-ɢ�����ֵ
#define protocol_command_getconfig_cleanhead	0x17		//��ѯ-��ϴ����ֵ

#define protocol_command_getconfig_thickness	0x18		//��ѯ-�������ֵ

#define protocol_command_setconfig_amplitude	0x41		//�趨-��ѹ����ֵ
#define protocol_command_setconfig_pulsewidth	0x42		//�趨-�������ֵ

#define protocol_command_setconfig_spray		0x43		//�趨-ɢ�����ֵ
#define protocol_command_setconfig_cleanhead	0x44		//�趨-��ϴ����ֵ

#define protocol_command_setconfig_thickness	0x45		//�趨-�������ֵ�������Թ��ܣ�

#define protocol_command_print_start			0x50		//��ӡ-��ӡ׼��
#define protocol_command_print_line				0x51		//��ӡ-��ӡһ��
#define protocol_command_print_end				0x52		//��ӡ-��ӡ����

#define protocol_command_pc_reset				0x53		//��ӡ-��λ
#define protocol_command_pc_pause				0x54		//��ӡ-��ͣ
#define protocol_command_pc_resume				0x55		//��ӡ-����

#define protocol_command_print_back2origin		0x56		//��ӡ-�ص���ӡԭ��
#define protocol_command_print_cheanhead		0x57		//��ӡ-ִ��һ���Զ���ϴ����
#define protocol_command_print_spray			0x58		//��ӡ-ִ��һ��ɢ�����

#define protocol_command_setconfig_moto_general	0x70		//�˶�-�趨-�������ò���
#define protocol_command_setconfig_moto_x		0x71		//�˶�-����-X�����ò���
#define protocol_command_setconfig_moto_y		0x72		//�˶�-����-Y�����ò���
#define protocol_command_setconfig_moto_z		0x73		//�˶�-����-�������ò���

#define protocol_command_getconfig_moto_general	0x74		//�˶�-��ѯ-�������ò���
#define protocol_command_getconfig_moto_x		0x75		//�˶�-��ѯ-X�����ò���
#define protocol_command_getconfig_moto_y		0x76		//�˶�-��ѯ-Y�����ò���
#define protocol_command_getconfig_moto_z		0x77		//�˶�-��ѯ-�������ò���


//========================================================
// MB��������
//========================================================
#define protocol_command_mb_reset				0x60		//�쳣-��λ
#define protocol_command_mb_pause				0x61		//�쳣-��ͣ
#define protocol_command_mb_resume				0x62		//�쳣-����

#define protocol_command_mb_syserror			0x64		//�쳣-ϵͳ����
#define protocol_command_mb_sysstatus			0x65		//�쳣-ϵͳ�쳣״̬

#define protocol_command_mb_cleanhead			0x66		//�쳣-��ͷ��ϴ


//PC-SF


#define cmd_sf_motor_move_relative				0x01	//����Ծ���
#define cmd_sf_motor_move_absolute				0x02	//�ƶ�������λ��
#define cmd_sf_motor_get_position				0x03	//��ȡ��ǰλ��
#define cmd_sf_motor_move_stop					0x04	//ֹͣ�ƶ�	
#define cmd_sf_motor_move_special_position		0x05	//�ƶ�������λ��
#define cmd_sf_motor_move_offline				0x06	//�ѻ��ƶ�
#define cmd_sf_download_on_timetable_paramater	0x07	//��������ʱ������
#define	cmd_sf_get_on_timetable_pamarater		0x08	//��ȡ����ʱ������
#define cmd_sf_download_timetable_timevalue		0x09	//����ʱ�������
#define cmd_sf_get_existent_speedlevel			0x0A	//��ȡ��λ����
#define cmd_sf_count_fullpulse					0x0B	//ȫ���������
#define	cmd_sf_get_fullpulse					0x0C	//��ȫ��������

#define cmd_sf_get_special_position				0x0E	//��ȡ����λ��
#define cmd_sf_set_special_position				0x0F	//��������λ��

#define cmd_sf_get_timetable_vital_parameter	0x12	//��ʱ�����Ҫ����
#define cmd_sf_delete_all_timetable_data		0x13	//��յ�λ��Ϣ

#define cmd_sf_download_off_timetable_paramater 0x15	//�����ѻ�ʱ������
#define cmd_sf_get_off_timetable_pamarater		0x16	//��ȡ�ѻ�ʱ������
#define cmd_sf_set_firm_parameter				0x17	//���ù̼�����
#define cmd_sf_get_firm_parameter				0x18	//��ȡ�̼�����

#define cmd_sf_motor_xy_move_relative			0x20	//XY���� - �ƶ���Ծ���
#define cmd_sf_motor_xy_move_absolute			0x21	//XY���� - �ƶ�������λ��	
#define cmd_sf_motor_xy_move_special_position	0x22	//XY���� - �ƶ�������λ��
#define cmd_sf_motor_xy_print_line				0x23	//XY���� - ��ӡһ��

//MB
//#define cmd_mb_online_state	0x02	//����PC������״̬

//#define cmd_mb_getconfig_cleanhead	0x17	//��ѯ--��ϴ
//#define cmd_mb_setconfig_cleanhead	0x44	//����--��ϴ
//#define cmd_mb_print_cleanhead	0x57	//ִ��--��ϴ
//
//#define cmd_mb_getconfig_thickness 0x18	//��ѯ--����
//#define cmd_mb_setconfig_thickness 0x45	//����--����
//
//#define cmd_mb_getconfig_spray	0x19	//��ѯ--ɢ��
//#define cmd_mb_setconfig_spray	0x46	//����--ɢ��

#define cmd_mb_print_start 0x20		//��ӡ׼��
#define cmd_mb_print_line 0x21		//��ӡһ��
#define cmd_mb_print_end 0x22		//��ӡ����
#define cmd_mb_print_stop_gcb	0x27	//��ӡ��ֹ
#define cmd_mb_print_pause_gcb	0x28	//��ӡ��ͣ
#define	cmd_mb_print_resume_gcb	0x29	//��ӡ����
#define cmd_mb_move_absolute_position	0x23	//�ƶ�������λ��
#define cmd_mb_move_relative_position	0x24	//�ƶ����λ��
#define cmd_mb_move_to_special_position	0x25	//�ƶ�������λ��
#define cmd_mb_store_current_position	0x26	//���浱ǰλ��

#define cmd_gcb_online_state	0x02	//��������״̬

//�忨
#define cmd_card_setinfo	0xB1	//���ð忨����
#define cmd_card_queryinfo	0xB2	//��ѯ�忨����
#define cmd_card_setaddr	0xB3	//���ð忨��ַ
#define	cmd_card_query		0xB4	//��ѯ�忨��ַ

//��ī �ղ� �Ͳ� ��ƫ
#define	cmd_ink_check_485	0x12	//ͨѶ�Լ�
#define cmd_ink_check_state	0x13	//��ѯ״̬
#define cmd_ink_force		0x14	//ǿ�ƹ�ī
#define cmd_ink_press		0x15	//ѹī
#define cmd_ink_scrape		0x16	//��ī
#define cmd_ink_moisturize  0x17	//��ʪ
#define cmd_ink_cycle_motor	0x18	//ѭ�����

#define cmd_rc_check_state	0x13	//��ѯ״̬
#define cmd_rc_pass			0x20	//Pass����
#define cmd_rc_print_stop	0x21	//��ӡ��ͣ
#define cmd_rc_print_over	0x22	//��ӡ����

#define cmd_sc_check_state	0x13	//��ѯ״̬
#define cmd_sc_band_dry		0x20	//�������
#define cmd_sc_pre_dry		0x21	//��Ԥ��
#define cmd_sc_band_wash	0x22	//������ϴ
#define cmd_sc_press_roller	0x23	//ѹ����
#define cmd_sc_motor		0x24	//�Ͳ��������
#define cmd_sc_stop			0x25	//�Ͳ���ͣ
#define cmd_sc_cancle_wrong_lable 0x26	//ȡ������λ

#define cmd_cb_auto_correct	0x20	//�Զ���ƫ
#define cmd_cb_manual_correct	0x21	//�ֶ���ƫ
#define cmd_cb_manual_correct_stop 0x23	//�ֶ���ƫֹͣ

//COM
#define cmd_com_motor_move_relative 0x30	//�ƶ���Ծ���
#define cmd_com_get_board_condition 0x09	//��ѯ���忨�������

//RAS
#define cmd_ras_print_start 0x20		//��ӡ׼��
#define cmd_ras_print_line 0x21		//��ӡһ��
#define cmd_ras_print_end 0x22		//��ӡ����
#define cmd_ras_select 0xA1		//PC�л�
#define cmd_ras_print_stop 0x2A
#define cmd_ras_print_pause 0x2B
#define cmd_ras_print_resume 0x2C
#define cmd_ras_sys_error	0x2D
#define cmd_ras_sys_exception	0x2E

#define cmd_ras_getspeed 0xA2
#define cmd_ras_getposition 0xA3
#define cmd_ras_print_stop_gcb 0x27
#define cmd_ras_move_absolute_position 0x23

//�ղ���Э��
#define cmd_rc_servo_motor_move_distance 0x80 //�ŷ�����˶�(�����о���)
#define cmd_rc_up_down_motor_move 0x81//��������˶�
#define cmd_rc_servo_motor_move 0x82//�ŷ�����˶�(��������)
#define cmd_rc_heater_con	0x83//���ȹܿ���
#define cmd_rc_cloth_motor_con	0x84//�ղ��������
#define cmd_rc_up_down_motor_reset  0x85//���������λ

//�Ͳ���Э��
#define cmd_sc_new_sendcloth_check_state 0x13 //��ѯ״̬
#define cmd_sc_new_band_dry 0x52 //�������
#define cmd_sc_new_band_wash 0x51 //��ˮ��
#define cmd_sc_new_pre_dry 0x53 //��Ԥ��
#define cmd_sc_new_dd_band_wash 0x54 //������ϴ
#define cmd_sc_new_get_temperature 0x56 //��ȡ�¶�
#define cmd_sc_new_press_cloth 0x5A //ѹ���������ƶ�
#define cmd_sc_new_cloth_roller 0x50 //ѹ����ǰ���ƶ�
#define cmd_sc_new_cancle_wrong_lable 0x57 //ȡ������λ
#define cmd_sc_new_manual_correct 0x5B //�ֶ���ƫ
#define cmd_sc_new_manual_correct_stop 0x5C //�ֶ���ƫֹͣ

//LD
#define cmd_ld_set_check_power				0x09		//���õ�һ���⹦��
#define cmd_ld_set_laser_test_power			0x18		//����΢�⹦�ʼ���ѹ
// #define cmd_ld_set_all_power				0x11		//�������м��⹦��
// #define cmd_ld_get_all_power				0x12		//��ѯ���⵱ǰ����
// #define cmd_ld_print_check_power			0x13		//ִ�м��⹦�ʼ��

#define	cmd_pc_set_power_status				0x16		//ͨ�����⹦������״̬����

#define cmd_ld_power_set_pc					0x15
#define cmd_ld_power_set_ld					0x16
#define cmd_ld_power_get_last				0x12
#define cmd_ld_power_get_now				0x13
#define cmd_ld_power_correct				0x14

#define cmd_ld_light_on						0xc0		//ָ����·��
#define cmd_ld_light_off					0xc1		//ָ����·��
#define cmd_ld_light_on_time				0x17

#define cmd_ld_get_current_power			0x50		//��ѯ��ǰ����ʵ�ʹ���

//PD
#define cmd_get_pd_mapdata					0x07		//��ӳ���
#define cmd_set_pd_mapdata					0x08		//��ӳ���
