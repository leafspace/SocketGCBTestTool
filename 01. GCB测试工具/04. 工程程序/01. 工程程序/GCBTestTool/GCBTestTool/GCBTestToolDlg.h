
// GCBTestToolDlg.h : 头文件
//

#pragma once

#define LIST_NUM			12
#define TOPBAR_SIZE			20                                              // 顶端标签到边缘的大小
#define SCROLLBAR_MIN		0                                               // Scroll Bar最小值
#define SCROLLBAR_MAX		1000                                            // Scroll Bar最大值
#define TIMER_OUT			3000                                            // 超时
#define TIMER_GAP			1000                                            // 时间间隔
#define ORDER_BUFFER_SIZE   100                                             // 命令字节缓存大小

#include "afxwin.h"
#include "GCBTestTool.h"
#include "StateTable/StateTable.h"
#include "SocketLink/SocketLink.h"
#include "ChildDialog/GCBMainDlg.h"
#include "ThreadHandle/ThreadHandle.h"
#include "ChildDialog/GCBDetailFrameDlg.h"


// CGCBTestToolDlg 对话框
class CGCBTestToolDlg : public CDialog
{
	// 构造
public:
	CGCBTestToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GCBTESTTOOL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	DECLARE_MESSAGE_MAP()


private:
	CTabCtrl m_FrameTabCtrl;                                                // Tab Control 控制对象
	CScrollBar m_DlgScrollBar;                                              // Scroll Bar 控制对象

	int nCurSelTab;                                                         // 保存当前显示的是TabControl中的页面号
	int nDialogLen;                                                         // 当前打开的详情页面的个数 数据范围[0,11]
	CDialog* pDialog[LIST_NUM + 1];                                         // 用来保存对话框对象指针

	GCBMainDlg mainPage;                                                    // 概览页面
	GCBDetailFrameDlg framePage[LIST_NUM];                                  // 详情页面

	CString strServerIP;                                                    // 服务器的IP
	CString strServerPort;                                                  // 通讯的端口号

	SocketLink socketLink;                                                  // 建立的Socket连接
	bool socketISLinking;                                                   // 当前是否处在持续连接中

	HANDLE hThreadSocketLinkTest;                                           // 线程句柄
	HANDLE hThreadSocketLinkRecv;                                           // 线程句柄
	HANDLE hThreadSocketLinkSend;                                           // 线程句柄

	void OnTimerSocketLinkTest();                                           // 处理测试连接后的循环事件
	void OnTimerSocketLink();                                               // 处理连接按钮后的循环事件

	list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const uint16_t uReadNum); // 创建一条要发送的命令
public:
	static StateTable threadStateTable;	

	void ShowMessage(PROGRAM_STATE_CODE stateCode, PROGRAM_STATE_TYPE stateType);     // 跳出提示框
	bool AddNewFrameTab(const int nIndex);                                  // Tab Control 添加一个页面
	void SendRequestMessage();
};
