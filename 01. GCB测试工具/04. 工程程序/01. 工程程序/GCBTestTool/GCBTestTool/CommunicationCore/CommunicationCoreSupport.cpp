#include "stdafx.h"
#include "CommunicationCore.h"

list<BYTE> CommunicationCore::CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum)
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

list<BYTE> CommunicationCore::CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const float uReadNum)
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


void CommunicationCore::SendRequestMessage()
{
	// 全部类型发送一遍请求数据

	// 获取组
	list<BYTE> sendMsgLst[LIST_NUM];
	sendMsgLst[0] = CommunicationCore::CreateMessage(NOZZLE_CARTRIDGE_LEVEL, 0x0000, (uint16_t)2);
	sendMsgLst[1] = CommunicationCore::CreateMessage(MODE_LOCKED_SOLENOID_VALVE_WORKING, 0x0000, (uint16_t)2);
	sendMsgLst[2] = CommunicationCore::CreateMessage(POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING, 0x0000, (uint16_t)2);
	sendMsgLst[3] = CommunicationCore::CreateMessage(INK_SUPPLY_PUMP_WORKING_CONDITION, 0x0000, (uint16_t)2);
	sendMsgLst[4] = CommunicationCore::CreateMessage(NOZZLE_CABINET_TEMPERATURE, 0x0000, (uint16_t)2);
	sendMsgLst[5] = CommunicationCore::CreateMessage(AIR_NEGATIVE_PRESSURE_VALUE, 0x0000, (uint16_t)2);
	sendMsgLst[6] = CommunicationCore::CreateMessage(AIR_POSITIVE_PRESSURE_VALUE, 0x0000, (uint16_t)1);

	// 临时设置为7个数
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		MessageBean tempMessageBean;
		tempMessageBean.SetOrginDataList(sendMsgLst[nIndex]);
		tempMessageBean.AnalysisOrginDataLst();
		CommunicationCore::sendMessageQueue.Push_back(tempMessageBean);
	}
}

float CommunicationCore::Make4ByteFloat(BYTE *list)
{
	float retValue;
	memcpy(&retValue, list, sizeof(retValue));
	return retValue;
}

float CommunicationCore::MakeTurn4ByteFloat(BYTE *list)
{
	BYTE newList[4] = { 0 };
	for (int nIndex = 0; nIndex < 4; ++nIndex) {
		newList[nIndex] = list[3 - nIndex];
	}

	float retValue = this->Make4ByteFloat(newList);
	return retValue;
}

short CommunicationCore::GetFormatSize(FRAME_CMD_TYPE cmdType)
{
	switch (cmdType)
	{
	case NOZZLE_CARTRIDGE_LEVEL:
	case MODE_LOCKED_SOLENOID_VALVE_WORKING:
	case POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING:
	case INK_SUPPLY_PUMP_WORKING_CONDITION:
		return 1;
	case NOZZLE_CABINET_TEMPERATURE:
	case AIR_NEGATIVE_PRESSURE_VALUE:
	case AIR_POSITIVE_PRESSURE_VALUE:
		return 4;
	default:
		return 1;
	}
}

bool CommunicationCore::WriteLog(MessageBean beanMessage, CString *timeStr, list<float> &retValueLst)
{
	ofstream outfile("gcb.log", ios::app);
	if (!outfile) {
		return false;
	}

	SYSTEMTIME sys;
	GetLocalTime(&sys);

	timeStr->Format(_T("%4d-%02d-%02d %02d:%02d:%02d "), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	outfile << setw(4) << setfill('0') << sys.wYear << "-";
	outfile << setw(2) << setfill('0') << sys.wMonth << "-";
	outfile << setw(2) << setfill('0') << sys.wDay << " ";
	outfile << setw(2) << setfill('0') << sys.wHour << ":";
	outfile << setw(2) << setfill('0') << sys.wMinute << ":";
	outfile << setw(2) << setfill('0') << sys.wSecond << " ";

	outfile << "0x" << hex << beanMessage.GetCMDType() << " :  ";

	short formatSize = this->GetFormatSize(beanMessage.GetCMDType());
	list<BYTE> beanPLst = beanMessage.GetParameterList();
	list<BYTE>::iterator iterP = beanPLst.begin();

	iterP++;
	iterP++;

	for (int nIndex = 0; nIndex < (beanMessage.GetParameterSize() - 2) / formatSize; ++nIndex) {

		BYTE *list = new BYTE[formatSize];
		for (int nJIndex = 0; nJIndex < formatSize; ++nJIndex, ++iterP) {
			list[nJIndex] = *iterP;
		}

		float retValue = formatSize == 4 ? this->MakeTurn4ByteFloat(list) : list[0];

		delete list;
		outfile << setiosflags(ios::fixed) << setprecision(2) << retValue << "\t";
		retValueLst.push_back(retValue);
	}


	outfile << "[";
	list<BYTE> beanOLst = beanMessage.GetOrginDataList();
	list<BYTE>::iterator iterO = beanOLst.begin();
	for (int nIndex = 0; nIndex < (int)beanOLst.size(); ++nIndex, ++iterO) {
		if ((nIndex + 1) == beanOLst.size()) {
			outfile << "0x" << hex << setw(2) << setfill('0') << (int)*iterO;
		}
		else {
			outfile << "0x" << hex << setw(2) << setfill('0') << (int)*iterO << "\t";
		}
	}
	outfile << "]";
	outfile << endl;

	outfile.close();
	return true;
}

CString CommunicationCore::GetFormatStrLable(FRAME_CMD_TYPE cmdType, float value)
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