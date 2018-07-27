#pragma once

#include <math.h>
#include <time.h>
#include <iomanip>
#include <fstream>
#include "afxcmn.h"
#include <windows.h>
#include "../ChildDialog/GCBSettingDlg.h"
#include "../ChildDialog/GCBDetailFrameDlg.h"
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
	int nDialogLen;                                                         // 本页中打开详情页面的个数
	CListCtrl m_List[LIST_NUM];                                             // 显示数据的列表
	list<float> showValueLst;                                               // 绘图显示的数据列表

	GCBDetailFrameDlg framePage[LIST_NUM];                                  // 详情页面
	GCBSettingDlg gcbSettingPage;                                           // 板卡设置页面
	
	void StartDraw(int ControlID);                                          // 开启一次绘画
public:
	GCBDetailFrameDlg *GetFramePage(FRAME_CMD_TYPE cmdType);                // 按照类型获取详细页面指针

	void RefreshPage(CommunicateCore* communicationCore);                   // 处理一次接收到的消息
	void CreateTimer(TIMER_TYPE timer, int timeGap);                        // 创建绘图定时器
	void DeleteTimer(TIMER_TYPE timer);                                     // 删除绘图定时器

	void ShowSettingDialog(void);                                           // 弹出设置页面
	void ClearAllData(void);                                                // 清除所有数据
};
