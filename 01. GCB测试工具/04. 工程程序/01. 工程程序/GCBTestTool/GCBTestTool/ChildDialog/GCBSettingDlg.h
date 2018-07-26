#pragma once

#include "../GCBTestTool.h"

class GCBSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBSettingDlg)

public:
	GCBSettingDlg(CWnd* pParent = NULL);
	virtual ~GCBSettingDlg();

	// 对话框数据
	enum { IDD = IDD_GCB_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
};
