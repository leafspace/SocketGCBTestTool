#pragma once
#include "afxcmn.h"
#include "../SocketLink/MessageQueue/MessageQueue.h"

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
	afx_msg void OnButtonClick(UINT nID);
	DECLARE_MESSAGE_MAP()


private:
	CListCtrl* JudgeMessageCMDCtrl(MessageBean beanMessage);

public:
	static MessageQueue recvMessageQueue;
	static MessageQueue sendMessageQueue;
	

	CListCtrl m_List[LIST_NUM];
	void RefreshPage();
};
