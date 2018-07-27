#pragma once

#include <math.h>
#include <time.h>
#include <iomanip>
#include <fstream>
#include "afxcmn.h"
#include <windows.h>
#include "../CommunicationCore/CommunicationCore.h"
#include "../SocketLink/MessageQueue/MessageQueue.h"

#define PICTRUE_SHOW_SIZE	450                                             // 图片绘图的宽度

class GCBMainDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBMainDlg)
public:
	GCBMainDlg(CWnd* pParent = NULL);
	virtual ~GCBMainDlg();
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
public:

	CListCtrl m_List[LIST_NUM];
	void RefreshPage(CommunicateCore* communicationCore);                   // 处理一次接收到的消息
	void CreateTimer(TIMER_TYPE timer, int timeGap);                        // 创建绘图定时器
	void DeleteTimer(TIMER_TYPE timer);                                     // 删除绘图定时器

	void ClearAllData(void);
};
