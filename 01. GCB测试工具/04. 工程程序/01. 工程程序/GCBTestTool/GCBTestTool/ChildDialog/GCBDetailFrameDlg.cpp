#include "stdafx.h"
#include "../GCBTestTool.h"
#include "../GCBTestToolDlg.h"
#include "GCBDetailFrameDlg.h"


// GCBDetailFrameDlg 对话框

IMPLEMENT_DYNAMIC(GCBDetailFrameDlg, CDialog)

GCBDetailFrameDlg::GCBDetailFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GCBDetailFrameDlg::IDD, pParent)
{
	this->SetFrameType(0);
}

GCBDetailFrameDlg::~GCBDetailFrameDlg()
{
}

void GCBDetailFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTMODEL, m_List);
}


BEGIN_MESSAGE_MAP(GCBDetailFrameDlg, CDialog)
END_MESSAGE_MAP()


// GCBDetailFrameDlg 消息处理程序

BOOL GCBDetailFrameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_STATIC, GetLabel(this->nFrameTypeIndex));

	CRect rect;
	this->m_List.GetClientRect(&rect); //获得当前listcontrol的宽度
	this->m_List.SetExtendedStyle(this->m_List.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	const int nTableHeadNum = 5, nTableDataNum = 2;
	// 插入表头标题		
	for (int nJIndex = 0; nJIndex < nTableHeadNum; ++nJIndex) {
		if ((nJIndex + 1) == nTableHeadNum) {
			this->m_List.InsertColumn(nJIndex, _T("数据标题"), LVCFMT_CENTER,
				rect.Width() / nTableHeadNum - 20, nJIndex);
		}
		else {
			this->m_List.InsertColumn(nJIndex, _T("数据标题"), LVCFMT_CENTER,
				rect.Width() / nTableHeadNum, nJIndex);
		}
	}

	CString strDataValue;
	for (int nJIndex = 0; nJIndex < nTableDataNum; ++nJIndex) {
		this->m_List.InsertItem(nJIndex, _T("数据"));
		for (int nKIndex = 0; nKIndex < nTableHeadNum; ++nKIndex) {
			strDataValue.Format(_T("数据%d"), nKIndex + 1);
			this->m_List.SetItemText(nJIndex, nKIndex, strDataValue);
		}
	}
	return TRUE;
}

void GCBDetailFrameDlg::SetFrameType(int nFrameTypeNum)
{
	this->nFrameTypeIndex = nFrameTypeNum;
}