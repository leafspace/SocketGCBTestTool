#pragma once


// CLDSetPower �Ի���

class CLDSetPower : public CDialog
{
	DECLARE_DYNAMIC(CLDSetPower)

public:
	CLDSetPower(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLDSetPower();

// �Ի�������
	enum { IDD = IDD_LD_SET_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	float m_fVol;
	int m_nPower;
	afx_msg void OnBnClickedButtonLdAddCheckPower();
	afx_msg void OnBnClickedButtonLdModifyCheckPower();
	afx_msg void OnBnClickedButtonLdCloseCheckPower();
};
