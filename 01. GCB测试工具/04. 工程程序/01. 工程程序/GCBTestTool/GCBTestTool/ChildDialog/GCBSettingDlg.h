﻿#pragma once
#include "../CommonType.h"
#include "../SocketLink/MessageBean/MessageBean.h"

// GCBSettingDlg 对话框

class GCBSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBSettingDlg)

public:
	GCBSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GCBSettingDlg();

// 对话框数据
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

private:
	list<BYTE> CreateMessage(const BYTE cmdID, const uint16_t uRegisterAddress, const float uReadNum);
};
