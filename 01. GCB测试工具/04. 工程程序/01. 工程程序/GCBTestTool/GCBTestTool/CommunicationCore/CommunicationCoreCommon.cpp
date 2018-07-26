#include "stdafx.h"
#include "CommunicationCore.h"

list<BYTE> CommunicateCore::CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum)
{
	list<BYTE> retLst;

	// Head
	retLst.push_back(0xF0);
	retLst.push_back(0xF1);

	// CMD
	retLst.push_back(cmdID);

	// Size
	uint16_t uSize = sizeof(uRegisterAddress) + sizeof(uReadNum);
	retLst.push_back(BYTE0(uSize));
	retLst.push_back(BYTE1(uSize));

	// Parameter
	retLst.push_back(BYTE0(uRegisterAddress));
	retLst.push_back(BYTE1(uRegisterAddress));

	retLst.push_back(BYTE0(uReadNum));
	retLst.push_back(BYTE1(uReadNum));

	// Tail
	retLst.push_back(0xEC);
	return retLst;
}

list<BYTE> CommunicateCore::CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const float uReadNum)
{
	list<BYTE> retLst;

	// Head
	retLst.push_back(0xF0);
	retLst.push_back(0xF1);

	// CMD
	retLst.push_back(cmdID);

	// Size
	uint16_t uSize = sizeof(uRegisterAddress) + sizeof(uReadNum);
	retLst.push_back(BYTE0(uSize));
	retLst.push_back(BYTE1(uSize));

	// Parameter
	retLst.push_back(BYTE0(uRegisterAddress));
	retLst.push_back(BYTE1(uRegisterAddress));

	retLst.push_back(BYTE0(uReadNum));
	retLst.push_back(BYTE1(uReadNum));
	retLst.push_back(BYTE2(uReadNum));
	retLst.push_back(BYTE3(uReadNum));

	// Tail
	retLst.push_back(0xEC);
	return retLst;
}

float CommunicateCore::Make4ByteFloat(const BYTE *list)
{
	float retValue;
	memcpy(&retValue, list, sizeof(retValue));
	return retValue;
}

float CommunicateCore::MakeTurn4ByteFloat(const BYTE *list)
{
	BYTE newList[4] = { 0 };
	for (int nIndex = 0; nIndex < 4; ++nIndex) {
		newList[nIndex] = list[3 - nIndex];
	}

	float retValue = CommunicateCore::Make4ByteFloat(newList);
	return retValue;
}

CString CommunicateCore::GetFormatStrLable(const FRAME_CMD_TYPE cmdType, const float value)
{
	CString formatStr;
	formatStr.Format(_T("%.2f"), value);
	switch (cmdType)
	{
	case NOZZLE_CARTRIDGE_LEVEL:
	{
		switch ((int)value)
		{
		case 0: return _T("正常");
		case 1: return _T("高液位");
		case -1: return _T("低液位");
		default: return _T("未知(") + formatStr + _T(")");

		}
	}
	break;
	case MODE_LOCKED_SOLENOID_VALVE_WORKING:
	case POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING:
	{
		switch ((int)value)
		{
		case 0x00: return _T("默认状态");
		case 0xff: return _T("状态改变");
		default: return _T("未知(") + formatStr + _T(")");

		}
	}
	break;
	case INK_SUPPLY_PUMP_WORKING_CONDITION:
	{
		switch ((int)value)
		{
		case 0x00: return _T("停止状态");
		case 0xff: return _T("工作状态");
		default: return _T("未知(") + formatStr + _T(")");

		}
	}
	break;
	case NOZZLE_CABINET_TEMPERATURE:
	case AIR_NEGATIVE_PRESSURE_VALUE:
	case AIR_POSITIVE_PRESSURE_VALUE:
	{
		return formatStr;
	}
	break;
	default:
		return _T("未知(") + formatStr + _T(")");
	}
}

FRAME_CMD_TYPE CommunicateCore::GetCMDIdFromIndex(const int nIndex)
{
	FRAME_CMD_TYPE retValue = NOZZLE_CARTRIDGE_LEVEL;
	switch (nIndex)
	{
	case 0: retValue = NOZZLE_CARTRIDGE_LEVEL; break;
	case 1: retValue = MODE_LOCKED_SOLENOID_VALVE_WORKING; break;
	case 2: retValue = POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING; break;
	case 3: retValue = INK_SUPPLY_PUMP_WORKING_CONDITION; break;
	case 4: retValue = NOZZLE_CABINET_TEMPERATURE; break;
	case 5: retValue = AIR_NEGATIVE_PRESSURE_VALUE; break;
	case 6: retValue = AIR_POSITIVE_PRESSURE_VALUE; break;
	case 7: retValue = NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE; break;
	case 8: retValue = INK_MOTOR_DELAY_TIME; break;
	case 9: retValue = MODE_PRESSURE_NOZZLE_INK; break;
	case 10: retValue = START_WORKING_STATE_CIRCULATING_MOTOR; break;
	case 11: retValue = MANUALLY_OPEN_MODE_LOCKED_SOLENOID_VALVE_STATUS; break;

	default:
		retValue = NOZZLE_CARTRIDGE_LEVEL; break;
	}

	return retValue;
}

int CommunicateCore::GetIndexFromCMDId(const FRAME_CMD_TYPE cmdType)
{
	int retValue = 0;
	switch (cmdType)
	{
	case NOZZLE_CARTRIDGE_LEVEL: retValue = 0; break;
	case MODE_LOCKED_SOLENOID_VALVE_WORKING: retValue = 1; break;
	case POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING: retValue = 2; break;
	case INK_SUPPLY_PUMP_WORKING_CONDITION: retValue = 3; break;
	case NOZZLE_CABINET_TEMPERATURE: retValue = 4; break;
	case AIR_NEGATIVE_PRESSURE_VALUE: retValue = 5; break;
	case AIR_POSITIVE_PRESSURE_VALUE: retValue = 6; break;
	case NEGATIVE_PRESSURE_OUTPUT_VALUE_NOZZLE: retValue = 7; break;
	case INK_MOTOR_DELAY_TIME: retValue = 8; break;
	case MODE_PRESSURE_NOZZLE_INK: retValue = 9; break;
	case START_WORKING_STATE_CIRCULATING_MOTOR: retValue = 10; break;
	case MANUALLY_OPEN_MODE_LOCKED_SOLENOID_VALVE_STATUS: retValue = 11; break;

	default:
		retValue = 0; break;
	}

	return retValue;
}