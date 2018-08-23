#pragma once


// CLDSetPower 对话框

class CLDSetPower : public CDialog
{
	DECLARE_DYNAMIC(CLDSetPower)

public:
	CLDSetPower(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLDSetPower();

// 对话框数据
	enum { IDD = IDD_LD_SET_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	float m_fVol;
	int m_nPower;
	afx_msg void OnBnClickedButtonLdAddCheckPower();
	afx_msg void OnBnClickedButtonLdModifyCheckPower();
	afx_msg void OnBnClickedButtonLdCloseCheckPower();
};
