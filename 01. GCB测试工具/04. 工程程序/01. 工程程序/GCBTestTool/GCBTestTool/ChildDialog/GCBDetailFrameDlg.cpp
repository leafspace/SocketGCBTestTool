#include "stdafx.h"
#include "../GCBTestTool.h"
#include "../GCBTestToolDlg.h"
#include "GCBDetailFrameDlg.h"

IMPLEMENT_DYNAMIC(GCBDetailFrameDlg, CDialog)

GCBDetailFrameDlg::GCBDetailFrameDlg(CWnd* pParent)
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


BOOL GCBDetailFrameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_STATIC, GetLabel(this->nFrameType));

	CRect rect;
	this->m_List.GetClientRect(&rect); //获得当前listcontrol的宽度
	this->m_List.SetExtendedStyle(this->m_List.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	const int nTableHeadNum = 4;

	this->m_List.InsertColumn(0, _T("接收时间"), LVCFMT_CENTER,
		120, 0);
	this->m_List.InsertColumn(1, _T("指令类型"), LVCFMT_CENTER,
		60, 1);
	this->m_List.InsertColumn(2, _T("参数大小"), LVCFMT_CENTER,
		60, 2);
	this->m_List.InsertColumn(3, _T("指令数据"), LVCFMT_CENTER,
		(rect.Width() - 260), 3);

	return TRUE;
}

void GCBDetailFrameDlg::SetFrameType(int nFrameTypeNum)
{
	this->nFrameType = NOZZLE_CARTRIDGE_LEVEL;

	switch (nFrameTypeNum)
	{
	case 0: this->nFrameType = NOZZLE_CARTRIDGE_LEVEL; break;
	case 1: this->nFrameType = MODE_LOCKED_SOLENOID_VALVE_WORKING; break;
	case 2: this->nFrameType = POSITIVE_NEGATIVE_PRESSURE_SOLENOID_VALVE_WORKING; break;
	case 3: this->nFrameType = INK_SUPPLY_PUMP_WORKING_CONDITION; break;
	case 4: this->nFrameType = NOZZLE_CABINET_TEMPERATURE; break;
	case 5: this->nFrameType = AIR_NEGATIVE_PRESSURE_VALUE; break;
	case 6: this->nFrameType = AIR_POSITIVE_PRESSURE_VALUE; break;
	default: this->nFrameType = NOZZLE_CARTRIDGE_LEVEL; break;
	}
	return;
}

FRAME_CMD_TYPE GCBDetailFrameDlg::GetFrameType(void)
{
	return this->nFrameType;
}

void GCBDetailFrameDlg::AddMessageBean(MessageBean beanMessage)
{
	// 将数据存入列表中
	this->histroyLst.push_back(beanMessage);

	CString tempStr;

	this->m_List.InsertItem(0, tempStr);                                    // 插入一行数据

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	tempStr.Format(_T("%4d-%02d-%02d %02d:%02d:%02d "), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	this->m_List.SetItemText(0, 0, tempStr);

	tempStr.Format(_T("0x%x"), beanMessage.GetCMDType());
	this->m_List.SetItemText(0, 1, tempStr);

	tempStr.Format(_T("%d"), beanMessage.GetParameterSize());
	this->m_List.SetItemText(0, 2, tempStr);

	tempStr = _T("");
	list<BYTE> beanLst = beanMessage.GetOrginDataList();
	list<BYTE>::iterator iter = beanLst.begin();
	for (int nIndex = 0; nIndex < (int)beanLst.size(); ++nIndex, ++iter) {
		CString tStr;
		tStr.Format(_T("0x%02x"), (int)*iter);
		tempStr += tStr;
		tempStr += _T(" ");
	}

	this->m_List.SetItemText(0, 3, tempStr);
}

void GCBDetailFrameDlg::ClearAllData(void)
{
	this->histroyLst.clear();
	this->m_List.DeleteAllItems();
}