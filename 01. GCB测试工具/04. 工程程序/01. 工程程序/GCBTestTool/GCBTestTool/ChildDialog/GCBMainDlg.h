#pragma once
#include <math.h>
#include <time.h>
#include <iomanip>
#include <fstream>
#include "afxcmn.h"
#include "../SocketLink/MessageQueue/MessageQueue.h"

#define PICTRUE_SHOW_SIZE	450

// GCBMainDlg 对话框

class GCBMainDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBMainDlg)

public:
	GCBMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GCBMainDlg();

	// 对话框数据
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonClick(UINT nID);
	DECLARE_MESSAGE_MAP()


private:
	CListCtrl* JudgeMessageCMDCtrl(MessageBean beanMessage);
	float Make4ByteFloat(BYTE *list);
	float MakeTurn4ByteFloat(BYTE *list);
	short GetFormatSize(FRAME_CMD_TYPE cmdType);
	bool WriteLog(MessageBean beanMessage, CString *timeStr, list<float> &retValueLst);

	list<float> showValueLst;
	void StartDraw(int ControlID);

	CString GetFormatStrLable(FRAME_CMD_TYPE cmdType, float value);
public:
	static MessageQueue recvMessageQueue;
	static MessageQueue sendMessageQueue;

	CListCtrl m_List[LIST_NUM];
	void RefreshPage(void);
	void CreateTimer(TIMER_TYPE timer);
	void DeleteTimer(TIMER_TYPE timer);

	void ClearAllData(void);
};
