#pragma once

#include "../GCBTestTool.h"

class SystemSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(SystemSettingDlg)

public:
	SystemSettingDlg(CWnd* pParent = NULL);
	virtual ~SystemSettingDlg();

	enum { IDD = IDD_SYSTEM_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckAll();
	DECLARE_MESSAGE_MAP()

private:
	bool isCheckAll;
public:

};
