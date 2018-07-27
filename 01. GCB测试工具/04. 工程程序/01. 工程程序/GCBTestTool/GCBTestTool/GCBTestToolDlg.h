#pragma once

#define LIST_NUM			7                                               // 一个GCB板需要监控的表格个数
#define GCB_NUM				50                                              // 做多能控制多少个GCB板子
#define TOPBAR_SIZE			20                                              // 顶端标签到边缘的大小
#define SCROLLBAR_MIN		0                                               // Scroll Bar最小值
#define SCROLLBAR_MAX		530                                             // Scroll Bar最大值
#define TIMER_OUT			3000                                            // 超时
#define TIMER_GAP			1000                                            // 时间间隔
#define DRAW_GAP			3000                                            // 绘画的时间间隔
#define ORDER_BUFFER_SIZE   100                                             // 命令字节缓存大小

#define GCB_IP_ADDRESS		_T("172.16.5.30")
#define GCB_PORT			_T("10000")
#define LABLE_LINK			_T("连接")
#define LABLE_STOPLINK		_T("停止连接")
#define LABLE_PREVIEW		_T("概览")
#define LABLE_ERROR			_T("错误")
#define LABLE_WARNING		_T("警告")
#define LABLE_TIP			_T("消息")
#define LABLE_ACCEP_TIME	_T("接收时间")
#define LABLE_ORDER_TYPE	_T("指令类型")
#define LABLE_PARAT_SIZE	_T("参数大小")
#define LABLE_ORDER_DATA	_T("指令数据")
#define LABLE_INK_BOX		_T("墨盒")
#define LABLE_RELAY			_T("继电器")
#define LABLE_INK_PUMP		_T("供墨泵")
#define LABLE_DATA			_T("数据")
#define LABLE_TIME			_T("时间")


#include "afxwin.h"
#include "CommonType.h"
#include "GCBTestTool.h"
#include "StateTable/StateTable.h"
#include "SocketLink/SocketLink.h"
#include "ChildDialog/GCBMainDlg.h"
#include "ThreadHandle/ThreadHandle.h"
#include "ChildDialog/GCBSettingDlg.h"
#include "ChildDialog/SystemSettingDlg.h"
#include "ChildDialog/GCBDetailFrameDlg.h"
#include "CommunicationCore/CommunicationCore.h"

// 主页面 对话框
class CGCBTestToolDlg : public CDialog
{
public:
	CGCBTestToolDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_GCBTESTTOOL_DIALOG };

protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();                                                 // 窗口绘图函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);                                // 定时器相应函数
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);        // 检测滑轮的位置
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);// 滑轮移动的动作
	afx_msg void OnTcnSelchangeFrametab(NMHDR *pNMHDR, LRESULT *pResult);   // 切换Control Tab的页面
	afx_msg void OnBnClickedButtonLinktest();                               // 点击测试连接按钮
	afx_msg void OnBnClickedButtonLink();                                   // 点击连接按钮
	afx_msg void OnBnClickedButtonGcbSetting();                             // 点击上行按钮
	afx_msg void OnBnClickedButtonSystemSetting();                          // 点击系统设置按钮
	DECLARE_MESSAGE_MAP()


private:
	CTabCtrl m_FrameTabCtrl;                                                // Tab Control 控制对象
	CScrollBar m_DlgScrollBar;                                              // Scroll Bar 控制对象

	int nCurSelTab;                                                         // 保存当前显示的是TabControl中的页面号
	int nDialogLen;                                                         // 当前打开的详情页面的个数 数据范围[0,6]
	CDialog* pDialog[GCB_NUM * (LIST_NUM + 1)];                             // 用来保存对话框对象指针

	GCBMainDlg mainPage;                                                    // 概览页面，临时为1个
	GCBDetailFrameDlg framePage[LIST_NUM];                                  // 详情页面
	GCBSettingDlg gcbSettingPage;                                           // 板卡设置页面
	SystemSettingDlg systemSettingPage;                                     // 系统设置页面
	CommunicateCore communicationCore;                                      // 用户Main页面通讯的通讯核心，临时为1个

	bool socketISLinking;                                                   // 当前是否处在持续连接中

	void OnTimerSocketLinkTest(void);                                       // 处理测试连接后的循环事件
	void OnTimerSocketLink(void);                                           // 处理连接按钮后的循环事件

	void ClearAllData(void);
public:

	void ShowMessage(PROGRAM_STATE_CODE stateCode, PROGRAM_STATE_TYPE stateType);     // 跳出提示框
	bool AddNewFrameTab(const int nIndex);                                  // Tab Control 添加一个页面

	GCBDetailFrameDlg *GetFramePage(FRAME_CMD_TYPE cmdType);

	CommunicateCore* GetCommunicateCore(void);
};
