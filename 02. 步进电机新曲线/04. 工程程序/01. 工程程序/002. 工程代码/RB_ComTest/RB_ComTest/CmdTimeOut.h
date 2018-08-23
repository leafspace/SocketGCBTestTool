#pragma once

class CCmdTimeOut
{
public:
	CCmdTimeOut(void);
	~CCmdTimeOut(void);
public:


	static int set_online_timetable_paramater;
	static int set_offline_timetable_paramater;
	static int set_timetable_timevalue;
	static int get_online_timetable_pamarater;
	static int get_offline_timetable_pamarater;
	static int get_existent_speedlevel;

	static int motor_move_relative;
	static int motor_move_absolute;
	static int motor_move_special_position;
	static int motor_move_stop;
	static int motor_move_offline;

	static int motor_xy_move_relative;
	static int motor_xy_move_absolute;
	static int motor_xy_move_special_position;
	static int motor_xy_print_line;

	static int count_fullpulse;
	static int get_fullpulse;

	static int motor_get_position;
	static int get_special_position;
	static int set_special_position;
	static int get_timetable_vital_parameter;
	static int delete_all_timetable_data;
	static int set_firm_parameter;
	static int get_firm_parameter;

	static int print_start;
	static int print_line;
	static int print_end;
	static int print_stop;
	static int print_pause;
	static int print_resume;
	static int move_absolute_position;
	static int move_relative_position;
	static int move_to_special_position;
	static int store_current_position;

	static int check_485;
	static int check_state_ink;
	static int force;
	static int press;
	static int scrape;
	static int moisturize;
	static int cycle_motor;

	static int check_state_rc;
	static int pass;
	static int print_stop_rc;
	static int print_over;

	static int check_state_sc;
	static int band_dry;
	static int pre_dry;
	static int band_wash;
	static int press_roller;
	static int motor;
	static int stop;
	static int cancle_wrong_lable;

	static int auto_correct;
	static int manual_correct;
	static int manual_correct_stop;

public:
	static void LoadFromCfg();
	static void SaveToCfg();
};
