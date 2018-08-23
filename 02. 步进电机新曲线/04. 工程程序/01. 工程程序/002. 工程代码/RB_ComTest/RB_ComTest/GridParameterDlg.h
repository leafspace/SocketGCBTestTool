#pragma once


// CGridParameterDlg 对话框

class CGridParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CGridParameterDlg)

public:
	CGridParameterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGridParameterDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_GRIDPARAMETER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float* m_pfIOSpace;	//IO间距
	float* m_pfIOBaseGridNum;	//IO间刻度数
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
