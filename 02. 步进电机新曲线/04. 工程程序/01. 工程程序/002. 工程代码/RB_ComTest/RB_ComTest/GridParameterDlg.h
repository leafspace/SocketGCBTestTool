#pragma once


// CGridParameterDlg �Ի���

class CGridParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CGridParameterDlg)

public:
	CGridParameterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGridParameterDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_GRIDPARAMETER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float* m_pfIOSpace;	//IO���
	float* m_pfIOBaseGridNum;	//IO��̶���
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
