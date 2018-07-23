#pragma once
#include "afxcmn.h"
#include "../CommonType.h"

class GCBDetailFrameDlg : public CDialog
{
	DECLARE_DYNAMIC(GCBDetailFrameDlg)

public:
	GCBDetailFrameDlg(CWnd* pParent = NULL);
	virtual ~GCBDetailFrameDlg();

	// 对话框数据
	enum { IDD = IDD_DETAIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	FRAME_CMD_TYPE nFrameType;                                              // 设置当前页面保存的指令类型

public:
	CListCtrl m_List;                                                       // 本页对应的数据列表
	list<MessageBean> histroyLst;                                           // 保存的历史数据

	void SetFrameType(int nFrameTypeNum);                                   // 设置本页的指令类型
	FRAME_CMD_TYPE GetFrameType(void);                                      // 获取本页的指令类型

	void AddMessageBean(MessageBean beanMessage);                           // 添加一个历史消息

	void ClearAllData(void);                                                // 清除所有的数据
};
