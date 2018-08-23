#include "StdAfx.h"
#include "RB_ComTest.h"
#include "CmdTimeOut.h"

int CCmdTimeOut::set_online_timetable_paramater = 5000;
int CCmdTimeOut::set_offline_timetable_paramater = 5000;
int CCmdTimeOut::set_timetable_timevalue = 5000;
int CCmdTimeOut::get_online_timetable_pamarater = 5000;
int CCmdTimeOut::get_offline_timetable_pamarater = 5000;
int CCmdTimeOut::get_existent_speedlevel = 5000;

int CCmdTimeOut::motor_move_relative = 120000;
int CCmdTimeOut::motor_move_absolute = 120000;
int CCmdTimeOut::motor_move_special_position = 120000;
int CCmdTimeOut::motor_move_stop = 5000;
int CCmdTimeOut::motor_move_offline = 5000;

int CCmdTimeOut::motor_xy_move_relative = 120000;
int CCmdTimeOut::motor_xy_move_absolute = 120000;
int CCmdTimeOut::motor_xy_move_special_position = 120000;
int CCmdTimeOut::motor_xy_print_line	= 120000;

int CCmdTimeOut::count_fullpulse = 500000;
int CCmdTimeOut::get_fullpulse = 5000;

int CCmdTimeOut::motor_get_position = 5000;
int CCmdTimeOut::get_special_position = 5000;
int CCmdTimeOut::set_special_position = 5000;
int CCmdTimeOut::get_timetable_vital_parameter = 5000;
int CCmdTimeOut::delete_all_timetable_data = 5000;
int CCmdTimeOut::set_firm_parameter = 5000;
int CCmdTimeOut::get_firm_parameter = 5000;

int CCmdTimeOut::print_start = 120000;
int CCmdTimeOut::print_line = 120000;
int CCmdTimeOut::print_end = 120000;
int CCmdTimeOut::print_stop = 5000;
int CCmdTimeOut::print_pause = 500000;
int CCmdTimeOut::print_resume = 20000;
int CCmdTimeOut::move_absolute_position = 120000;
int CCmdTimeOut::move_relative_position = 120000;
int CCmdTimeOut::move_to_special_position = 120000;
int CCmdTimeOut::store_current_position = 5000;

int CCmdTimeOut::check_485 = 5000;
int CCmdTimeOut::check_state_ink = 5000;
int CCmdTimeOut::force = 5000;
int CCmdTimeOut::press = 5000;
int CCmdTimeOut::scrape = 5000;
int CCmdTimeOut::moisturize = 5000;
int CCmdTimeOut::cycle_motor = 5000;

int CCmdTimeOut::check_state_rc = 5000;
int CCmdTimeOut::pass = 5000;
int CCmdTimeOut::print_stop_rc = 5000;
int CCmdTimeOut::print_over = 5000;

int CCmdTimeOut::check_state_sc = 5000;
int CCmdTimeOut::band_dry = 5000;
int CCmdTimeOut::pre_dry = 5000;
int CCmdTimeOut::band_wash = 5000;
int CCmdTimeOut::press_roller = 5000;
int CCmdTimeOut::motor = 5000;
int CCmdTimeOut::stop = 5000;
int CCmdTimeOut::cancle_wrong_lable = 5000;

int CCmdTimeOut::auto_correct = 5000;
int CCmdTimeOut::manual_correct = 5000;
int CCmdTimeOut::manual_correct_stop = 5000;

CCmdTimeOut::CCmdTimeOut(void)
{
}

CCmdTimeOut::~CCmdTimeOut(void)
{
}

void CCmdTimeOut::LoadFromCfg()
{
	char szReg[200];
	CString strIniFile,strSection;

	strIniFile = theApp.m_strAppPath + "CmdTimeOut.cfg";

	if (PathFileExists(strIniFile))
	{
		strSection = "SF";

		GET_PROFILE_INFO_INT(set_online_timetable_paramater,0,set_online_timetable_paramater,600000,INT);
		GET_PROFILE_INFO_INT(set_offline_timetable_paramater,0,set_offline_timetable_paramater,600000,INT);
		GET_PROFILE_INFO_INT(set_timetable_timevalue,0,set_timetable_timevalue,600000,INT);
		GET_PROFILE_INFO_INT(get_online_timetable_pamarater,0,get_online_timetable_pamarater,600000,INT);
		GET_PROFILE_INFO_INT(get_offline_timetable_pamarater,0,get_offline_timetable_pamarater,600000,INT);
		GET_PROFILE_INFO_INT(get_existent_speedlevel,0,get_existent_speedlevel,600000,INT);

		GET_PROFILE_INFO_INT(motor_move_relative,0,motor_move_relative,600000,INT);
		GET_PROFILE_INFO_INT(motor_move_absolute,0,motor_move_absolute,600000,INT);
		GET_PROFILE_INFO_INT(motor_move_special_position,0,motor_move_special_position,600000,INT);
		GET_PROFILE_INFO_INT(motor_move_stop,0,motor_move_stop,600000,INT);
		GET_PROFILE_INFO_INT(motor_move_offline,0,motor_move_offline,600000,INT);

		GET_PROFILE_INFO_INT(motor_xy_move_relative,0,motor_xy_move_relative,600000,INT);
		GET_PROFILE_INFO_INT(motor_xy_move_absolute,0,motor_xy_move_absolute,600000,INT);
		GET_PROFILE_INFO_INT(motor_xy_move_special_position,0,motor_xy_move_special_position,600000,INT);
		GET_PROFILE_INFO_INT(motor_xy_print_line,0,motor_xy_print_line,600000,INT);
		
		GET_PROFILE_INFO_INT(count_fullpulse,0,count_fullpulse,600000,INT);
		GET_PROFILE_INFO_INT(get_fullpulse,0,get_fullpulse,600000,INT);

		GET_PROFILE_INFO_INT(motor_get_position,0,motor_get_position,600000,INT);
		GET_PROFILE_INFO_INT(get_special_position,0,get_special_position,600000,INT);
		GET_PROFILE_INFO_INT(set_special_position,0,set_special_position,600000,INT);
		GET_PROFILE_INFO_INT(get_timetable_vital_parameter,0,get_timetable_vital_parameter,600000,INT);
		GET_PROFILE_INFO_INT(delete_all_timetable_data,0,delete_all_timetable_data,600000,INT);
		GET_PROFILE_INFO_INT(set_firm_parameter,0,set_firm_parameter,600000,INT);
		GET_PROFILE_INFO_INT(get_firm_parameter,0,get_firm_parameter,600000,INT);
		
		strSection = "MB";

		GET_PROFILE_INFO_INT(print_start,0,print_start,600000,INT);
		GET_PROFILE_INFO_INT(print_line,0,print_line,600000,INT);
		GET_PROFILE_INFO_INT(print_end,0,print_end,600000,INT);
		GET_PROFILE_INFO_INT(print_stop,0,print_stop,600000,INT);
		GET_PROFILE_INFO_INT(print_pause,0,print_pause,600000,INT);
		GET_PROFILE_INFO_INT(print_resume,0,print_resume,600000,INT);
		GET_PROFILE_INFO_INT(move_absolute_position,0,move_absolute_position,600000,INT);
		GET_PROFILE_INFO_INT(move_relative_position,0,move_relative_position,600000,INT);
		GET_PROFILE_INFO_INT(move_to_special_position ,0,move_to_special_position,600000,INT);
		GET_PROFILE_INFO_INT(store_current_position,0,store_current_position,600000,INT);
		
		strSection = "INK";

		GET_PROFILE_INFO_INT(check_485,0,check_485,600000,INT);
		GET_PROFILE_INFO_INT(check_state_ink,0,check_state_ink,600000,INT);
		GET_PROFILE_INFO_INT(force,0,force,600000,INT);
		GET_PROFILE_INFO_INT(press,0,press,600000,INT);
		GET_PROFILE_INFO_INT(scrape,0,scrape,600000,INT);
		GET_PROFILE_INFO_INT(moisturize,0,moisturize,600000,INT);
		GET_PROFILE_INFO_INT(cycle_motor,0,cycle_motor,600000,INT);
		
		strSection = "RC";

		GET_PROFILE_INFO_INT(check_state_rc,0,check_state_rc,600000,INT);
		GET_PROFILE_INFO_INT(pass,0,pass,600000,INT);
		GET_PROFILE_INFO_INT(print_stop_rc,0,print_stop_rc,600000,INT);
		GET_PROFILE_INFO_INT(print_over,0,print_over,600000,INT);
		
		strSection = "SC";

		GET_PROFILE_INFO_INT(check_state_sc,0,check_state_sc,600000,INT);
		GET_PROFILE_INFO_INT(band_dry,0,band_dry,600000,INT);
		GET_PROFILE_INFO_INT(pre_dry,0,pre_dry,600000,INT);
		GET_PROFILE_INFO_INT(band_wash,0,band_wash,600000,INT);
		GET_PROFILE_INFO_INT(press_roller,0,press_roller,600000,INT);
		GET_PROFILE_INFO_INT(motor,0,motor,600000,INT);
		GET_PROFILE_INFO_INT(stop,0,stop,600000,INT);
		GET_PROFILE_INFO_INT(cancle_wrong_lable,0,cancle_wrong_lable,600000,INT);
		
		strSection = "CB";

		GET_PROFILE_INFO_INT(auto_correct,0,auto_correct,600000,INT);
		GET_PROFILE_INFO_INT(manual_correct,0,manual_correct,600000,INT);
		GET_PROFILE_INFO_INT(manual_correct_stop,0,manual_correct_stop,600000,INT);
	}
}

void CCmdTimeOut::SaveToCfg()
{
	CString strIniFile,strSection,strReg;

	strIniFile = theApp.m_strAppPath + "CmdTimeOut.cfg";
	strSection = "SF";

	WRITE_PROFILE_INFO("%d", set_online_timetable_paramater);
	WRITE_PROFILE_INFO("%d", set_offline_timetable_paramater);
	WRITE_PROFILE_INFO("%d", set_timetable_timevalue);
	WRITE_PROFILE_INFO("%d", get_online_timetable_pamarater);
	WRITE_PROFILE_INFO("%d", get_offline_timetable_pamarater);
	WRITE_PROFILE_INFO("%d", get_existent_speedlevel);

	WRITE_PROFILE_INFO("%d", motor_move_relative);
	WRITE_PROFILE_INFO("%d", motor_move_absolute);
	WRITE_PROFILE_INFO("%d", motor_move_special_position);
	WRITE_PROFILE_INFO("%d", motor_move_stop);
	WRITE_PROFILE_INFO("%d", motor_move_offline);

	WRITE_PROFILE_INFO("%d", motor_xy_move_relative);
	WRITE_PROFILE_INFO("%d", motor_xy_move_absolute);
	WRITE_PROFILE_INFO("%d", motor_xy_move_special_position);
	WRITE_PROFILE_INFO("%d", motor_xy_print_line);

	WRITE_PROFILE_INFO("%d", count_fullpulse);
	WRITE_PROFILE_INFO("%d", get_fullpulse);

	WRITE_PROFILE_INFO("%d", motor_get_position);
	WRITE_PROFILE_INFO("%d", get_special_position);
	WRITE_PROFILE_INFO("%d", set_special_position);
	WRITE_PROFILE_INFO("%d", get_timetable_vital_parameter);
	WRITE_PROFILE_INFO("%d", delete_all_timetable_data);
	WRITE_PROFILE_INFO("%d", set_firm_parameter);
	WRITE_PROFILE_INFO("%d", get_firm_parameter);
	
	strSection = "MB";

	WRITE_PROFILE_INFO("%d", print_start);
	WRITE_PROFILE_INFO("%d", print_line);
	WRITE_PROFILE_INFO("%d", print_end);
	WRITE_PROFILE_INFO("%d", print_stop);
	WRITE_PROFILE_INFO("%d", print_pause);
	WRITE_PROFILE_INFO("%d", print_resume);
	WRITE_PROFILE_INFO("%d", move_absolute_position);
	WRITE_PROFILE_INFO("%d", move_relative_position);
	WRITE_PROFILE_INFO("%d", move_to_special_position);
	WRITE_PROFILE_INFO("%d", store_current_position);
	
	strSection = "INK";

	WRITE_PROFILE_INFO("%d", check_485);
	WRITE_PROFILE_INFO("%d", check_state_ink);
	WRITE_PROFILE_INFO("%d", force);
	WRITE_PROFILE_INFO("%d", press);
	WRITE_PROFILE_INFO("%d", scrape);
	WRITE_PROFILE_INFO("%d", moisturize);
	WRITE_PROFILE_INFO("%d", cycle_motor);
	
	strSection = "RC";

	WRITE_PROFILE_INFO("%d", check_state_rc);
	WRITE_PROFILE_INFO("%d", pass);
	WRITE_PROFILE_INFO("%d", print_stop_rc);
	WRITE_PROFILE_INFO("%d", print_over);
	
	strSection = "SC";

	WRITE_PROFILE_INFO("%d", check_state_sc);
	WRITE_PROFILE_INFO("%d", band_dry);
	WRITE_PROFILE_INFO("%d", pre_dry);
	WRITE_PROFILE_INFO("%d", band_wash);
	WRITE_PROFILE_INFO("%d", press_roller);
	WRITE_PROFILE_INFO("%d", motor);
	WRITE_PROFILE_INFO("%d", stop);
	WRITE_PROFILE_INFO("%d", cancle_wrong_lable);
	
	strSection = "CB";

	WRITE_PROFILE_INFO("%d", auto_correct);
	WRITE_PROFILE_INFO("%d", manual_correct);
	WRITE_PROFILE_INFO("%d", manual_correct_stop);
}

