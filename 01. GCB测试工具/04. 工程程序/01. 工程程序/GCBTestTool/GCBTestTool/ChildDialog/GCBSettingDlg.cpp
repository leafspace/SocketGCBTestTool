// E:\杭州开源电脑技术有限公司\01. GCB测试工具\04. 工程程序\01. 工程程序\GCBTestTool\GCBTestTool\ChildDialog\GCBSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../GCBTestTool.h"
#include "GCBSettingDlg.h"


// GCBSettingDlg 对话框

IMPLEMENT_DYNAMIC(GCBSettingDlg, CDialog)

GCBSettingDlg::GCBSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GCBSettingDlg::IDD, pParent)
{

}

GCBSettingDlg::~GCBSettingDlg()
{
}

void GCBSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GCBSettingDlg, CDialog)
END_MESSAGE_MAP()


// GCBSettingDlg 消息处理程序
