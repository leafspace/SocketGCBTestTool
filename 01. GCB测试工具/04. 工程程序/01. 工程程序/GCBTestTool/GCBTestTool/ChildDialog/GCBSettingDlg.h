#pragma once

#include "../GCBTestTool.h"
#include "afxcmn.h"

class GCBSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBSettingDlg)
public:
	GCBSettingDlg(CWnd* pParent = NULL);
	virtual ~GCBSettingDlg();
	enum { IDD = IDD_GCB_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog(void);
	afx_msg void OnBnClickedOk();                                           // 点击了设置按钮
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	int sendOrderNums;
	int proStep;
	CProgressCtrl mProCtrl;

	void CheckRecv(void);
};
