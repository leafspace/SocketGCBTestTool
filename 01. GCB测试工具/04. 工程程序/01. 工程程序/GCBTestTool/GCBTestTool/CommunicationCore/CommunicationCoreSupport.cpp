#include "stdafx.h"
#include "CommunicationCore.h"
#include "../GCBTestToolDlg.h"

bool CommunicateCore::InisSocketLink(string ipAddress, int port)
{
	return this->socketLink.initSocket(ipAddress, port);
}

bool CommunicateCore::InisSocketLink(CString ipAddress, int port)
{
	// CString 转 String
	wstring wstr(ipAddress);
	string strString;
	strString.assign(wstr.begin(), wstr.end());

	return this->InisSocketLink(strString, port);
}

bool CommunicateCore::SendRequestMessage()
{
	// 全部类型发送一遍请求数据
	// 临时设置为7个数(下行)

	bool bIsSuccess = true;
	for (int nIndex = 0; nIndex < LIST_NUM; ++nIndex) {
		if (CommunicateCore::bRequestFlag[nIndex] == false) {
			continue;
		}

		list<BYTE> sendMsg;
		MessageBean beanMessage;

		sendMsg = CommunicateCore::CreateMessage(CommunicateCore::GetCMDIdFromIndex(nIndex),
			CommunicateCore::uRequestBeginAddress[nIndex], CommunicateCore::uRequestDriveNums[nIndex]);

		beanMessage.SetOrginDataList(sendMsg);
		beanMessage.AnalysisOrginDataLst();
		bIsSuccess = bIsSuccess && this->SendRequestMessage(beanMessage);
	}
	return bIsSuccess;
}

bool CommunicateCore::SendRequestMessage(const MessageBean beanMessage)
{
	return this->sendMessageQueue.Push_back(beanMessage);
}

bool CommunicateCore::RecvResponseMessage(MessageBean *beanMessage)
{
	return  this->recvMessageQueue.Pop_front(beanMessage);
}

bool CommunicateCore::HaveRecvMessageResidual(void)
{
	return !this->recvMessageQueue.IsEmpty();
}

bool CommunicateCore::HaveSednMessageResidual(void)
{
	return !this->sendMessageQueue.IsEmpty();
}

short CommunicateCore::GetFormatSize(const FRAME_CMD_TYPE cmdType)
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

bool CommunicateCore::WriteLog(ofstream &outfile, MessageBean beanMessage, CString *timeStr, list<float> &retValueLst)
{
	// 文件未打开
	if (!outfile) {
		return false;
	}

	CommunicateCore::GetTimeStr(timeStr);
	CommunicateCore::WriteTimeStr(outfile);

	outfile << "0x" << hex << beanMessage.GetCMDType() << "\t:\t";

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
		outfile << setiosflags(ios::fixed) << setprecision(2) << retValue << " ";
		retValueLst.push_back(retValue);
	}


	outfile << "\t[";
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

	return true;
}

bool CommunicateCore::CreateSocketLinkTestThread()
{
	this->hThreadSocketLinkTest = CreateThread(NULL, 0, ThreadSocketLinkConn, &this->socketLink, 0, NULL);
	return true;
}

bool CommunicateCore::CreateSocketLinkRecvThread()
{
	this->hThreadSocketLinkRecv = CreateThread(NULL, 0, ThreadSocketLinkRecv, &this->socketLink, 0, NULL);
	return true;
}

bool CommunicateCore::CreateSocketLinkSendThread()
{
	this->hThreadSocketLinkSend = CreateThread(NULL, 0, ThreadSocketLinkSend, &this->socketLink, 0, NULL);
	return true;
}

bool CommunicateCore::CloseSocketLinkTestThread()
{
	CloseHandle(this->hThreadSocketLinkTest);
	return true;
}

bool CommunicateCore::CloseSocketLinkRecvThread()
{
	CloseHandle(this->hThreadSocketLinkRecv);
	return true;
}

bool CommunicateCore::CloseSocketLinkSendThread()
{
	CloseHandle(this->hThreadSocketLinkSend);
	return true;
}

int CommunicateCore::GetSocketLinkTestThreadState()
{
	return this->threadStateTable.GetThreadFinishedFlag(TIMER_SOCKET_LINK_CONN);
}

int CommunicateCore::GetSocketLinkRecvThreadState()
{
	return this->threadStateTable.GetThreadFinishedFlag(TIMER_SOCKET_LINK_RECV);
}

int CommunicateCore::GetSocketLinkSendThreadState()
{
	return this->threadStateTable.GetThreadFinishedFlag(TIMER_SOCKET_LINK_SEND);
}

int CommunicateCore::GetSocketLinkTestThreadRetValue()
{
	return this->threadStateTable.GetThreadRetValueFlag(TIMER_SOCKET_LINK_CONN);
}

int CommunicateCore::GetSocketLinkRecvThreadRetValue()
{
	return this->threadStateTable.GetThreadRetValueFlag(TIMER_SOCKET_LINK_RECV);
}

int CommunicateCore::GetSocketLinkSendThreadRetValue()
{
	return this->threadStateTable.GetThreadRetValueFlag(TIMER_SOCKET_LINK_SEND);
}

void CommunicateCore::RemoveSocketLinkTestThreadSteAndVlu(void)
{
	this->threadStateTable.RemoveThreadFinishedFlag(TIMER_SOCKET_LINK_CONN);
	this->threadStateTable.RemoveThreadRetValueFlag(TIMER_SOCKET_LINK_CONN);
}

void CommunicateCore::RemoveSocketLinkRecvThreadSteAndVlu(void)
{
	this->threadStateTable.RemoveThreadFinishedFlag(TIMER_SOCKET_LINK_RECV);
	this->threadStateTable.RemoveThreadRetValueFlag(TIMER_SOCKET_LINK_RECV);
}

void CommunicateCore::RemoveSocketLinkSendThreadSteAndVlu(void)
{
	this->threadStateTable.RemoveThreadFinishedFlag(TIMER_SOCKET_LINK_SEND);
	this->threadStateTable.RemoveThreadRetValueFlag(TIMER_SOCKET_LINK_SEND);
}