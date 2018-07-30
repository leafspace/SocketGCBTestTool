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
	virtual BOOL OnInitDialog(void);
	afx_msg void OnBnClickedCheckAll(void);
	afx_msg void OnBnClickedOk(void);
	DECLARE_MESSAGE_MAP()

private:
	bool isCheckAll;

	bool SetRequestCheckFlag(void);
	bool SetRequestBeginAddress(void);
	bool SetRequestDriveNums(void);
	bool SetTimerGap(void);
};
