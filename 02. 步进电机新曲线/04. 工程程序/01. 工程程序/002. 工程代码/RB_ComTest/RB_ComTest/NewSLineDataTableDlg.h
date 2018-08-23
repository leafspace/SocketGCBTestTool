#pragma once
#include "afxcmn.h"

class CNewSLineDataTableDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewSLineDataTableDlg)
public:
	CNewSLineDataTableDlg(CWnd* pParent = NULL);
	virtual ~CNewSLineDataTableDlg();
	enum { IDD = IDD_DIALOG_NEWSLINE_DATATABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonSfonlineOperation();
};
