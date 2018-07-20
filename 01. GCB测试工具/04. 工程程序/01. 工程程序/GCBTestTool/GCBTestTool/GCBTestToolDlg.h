#pragma once

#define LIST_NUM			7
#define TOPBAR_SIZE			20                                              // 顶端标签到边缘的大小
#define SCROLLBAR_MIN		0                                               // Scroll Bar最小值
#define SCROLLBAR_MAX		530                                             // Scroll Bar最大值
#define TIMER_OUT			3000                                            // 超时
#define TIMER_GAP			1000                                            // 时间间隔
#define DRAW_GAP			3000                                            // 绘画的时间间隔
#define ORDER_BUFFER_SIZE   100                                             // 命令字节缓存大小

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))                            // 获取某个数的第一个字节 从最低位开始
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))                      // 获取某个数的第二个字节 从最低位开始
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))                      // 获取某个数的第三个字节 从最低位开始
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))                      // 获取某个数的第四个字节 从最低位开始

#include "afxwin.h"
#include "GCBTestTool.h"
#include "StateTable/StateTable.h"
#include "SocketLink/SocketLink.h"
#include "ChildDialog/GCBMainDlg.h"
#include "ThreadHandle/ThreadHandle.h"
#include "ChildDialog/GCBSettingDlg.h"
#include "ChildDialog/GCBDetailFrameDlg.h"


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
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTcnSelchangeFrametab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLinktest();
	afx_msg void OnBnClickedButtonLink();
	afx_msg void OnBnClickedButtonSetting();
	DECLARE_MESSAGE_MAP()


private:
	CTabCtrl m_FrameTabCtrl;                                                // Tab Control 控制对象
	CScrollBar m_DlgScrollBar;                                              // Scroll Bar 控制对象

	int nCurSelTab;                                                         // 保存当前显示的是TabControl中的页面号
	int nDialogLen;                                                         // 当前打开的详情页面的个数 数据范围[0,11]
	CDialog* pDialog[LIST_NUM + 1];                                         // 用来保存对话框对象指针

	GCBMainDlg mainPage;                                                    // 概览页面
	GCBDetailFrameDlg framePage[LIST_NUM];                                  // 详情页面
	GCBSettingDlg settingPage;                                              // 设置页面

	CString strServerIP;                                                    // 服务器的IP
	CString strServerPort;                                                  // 通讯的端口号

	SocketLink socketLink;                                                  // 建立的Socket连接
	bool socketISLinking;                                                   // 当前是否处在持续连接中

	HANDLE hThreadSocketLinkTest;                                           // 线程句柄
	HANDLE hThreadSocketLinkRecv;                                           // 线程句柄
	HANDLE hThreadSocketLinkSend;                                           // 线程句柄

	void OnTimerSocketLinkTest(void);                                       // 处理测试连接后的循环事件
	void OnTimerSocketLink(void);                                           // 处理连接按钮后的循环事件

	int GetFrameTabIndex(const int nIndex);                                 // 获取按钮为nIndex的tab号

	void ClearAllData(void);
public:
	static StateTable threadStateTable;

	static list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum); // 创建一条要发送的命令
	static list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const float uReadNum);

	void ShowMessage(PROGRAM_STATE_CODE stateCode, PROGRAM_STATE_TYPE stateType);     // 跳出提示框
	bool AddNewFrameTab(const int nIndex);                                  // Tab Control 添加一个页面
	void SendRequestMessage(void);

	GCBDetailFrameDlg *GetFramePage(FRAME_CMD_TYPE cmdType);
};
