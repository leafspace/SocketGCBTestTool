#pragma once


// GCBSettingDlg �Ի���

class GCBSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBSettingDlg)

public:
	GCBSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GCBSettingDlg();

// �Ի�������
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
