#include "stdafx.h"
#include "GCBTestToolDlg.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))

list<BYTE> CGCBTestToolDlg::CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum)
{
    list<BYTE> retLst;

    // Head
    retLst.push_back(0xF0);
    retLst.push_back(0xF1);

    // CMD
    retLst.push_back(cmdID);

    // Size
    retLst.push_back((BYTE) (sizeof(uRegisterAddress) + sizeof(uReadNum)));

    // Parameter
    retLst.push_back(BYTE0(uRegisterAddress));
    retLst.push_back(BYTE1(uRegisterAddress));

    retLst.push_back(BYTE0(uReadNum));
    retLst.push_back(BYTE1(uReadNum));

    // Tail
    retLst.push_back(0xEC);
    return retLst;
}