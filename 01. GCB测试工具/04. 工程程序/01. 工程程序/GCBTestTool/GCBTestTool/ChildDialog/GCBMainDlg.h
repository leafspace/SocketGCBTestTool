#pragma once

#include <math.h>
#include <time.h>
#include <iomanip>
#include <fstream>
#include "afxcmn.h"
#include <windows.h>
#include "../CommunicationCore/CommunicationCore.h"
#include "../SocketLink/MessageQueue/MessageQueue.h"

#define PICTRUE_SHOW_SIZE	450

class GCBMainDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBMainDlg)

public:
	GCBMainDlg(CWnd* pParent = NULL);
	virtual ~GCBMainDlg();

	// 对话框数据
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonClick(UINT nID);
	DECLARE_MESSAGE_MAP()


private:
	list<float> showValueLst;
	void StartDraw(int ControlID);
	CListCtrl* JudgeMessageCMDCtrl(MessageBean beanMessage);
public:

	CListCtrl m_List[LIST_NUM];
	void RefreshPage(CommunicateCore* communicationCore);
	void CreateTimer(TIMER_TYPE timer);
	void DeleteTimer(TIMER_TYPE timer);

	void ClearAllData(void);
};
