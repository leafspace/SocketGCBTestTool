#pragma once
#include "afxcmn.h"
#include "../CommonType.h"


// GCBDetailFrameDlg 对话框

class GCBDetailFrameDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBDetailFrameDlg)

public:
	GCBDetailFrameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GCBDetailFrameDlg();

	// 对话框数据
	enum { IDD = IDD_DETAIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;

private:
	int nFrameTypeIndex;

public:
	void SetFrameType(int nFrameTypeNum);

};
