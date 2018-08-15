#pragma once
#include "afxcmn.h"

class GCBLinkHistoryDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBLinkHistoryDlg)
public:
	GCBLinkHistoryDlg(CWnd* pParent = NULL);
	virtual ~GCBLinkHistoryDlg();
	enum { IDD = IDD_HISTORY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_list;
public:
};
