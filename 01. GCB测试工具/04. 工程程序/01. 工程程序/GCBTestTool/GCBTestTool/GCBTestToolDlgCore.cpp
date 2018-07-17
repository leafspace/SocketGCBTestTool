#include "stdafx.h"
#include "GCBTestToolDlg.h"

list<BYTE> CGCBTestToolDlg::CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum)
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