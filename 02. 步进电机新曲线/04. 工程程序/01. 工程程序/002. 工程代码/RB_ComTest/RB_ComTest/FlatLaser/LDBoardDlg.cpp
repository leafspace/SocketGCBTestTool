// LDBoardDlg.cpp : ʵ���ļ�
//

#include "../stdafx.h"
#include "../RB_ComTest.h"

#include "LDBoardDlg.h"
#include "LDSetPower.h"

#include "../Curve/spline.h"


//����Ĭ�ϼ���ģ���·��
int GetLaserNumber_Default(BYTE bLaserModal)
{
	//����Ĭ�ϼ���ģ���·��
	int nLaserNumberTable[flatlaser_modal_count] = 
	{
		32,				//flatlaser_modal_32
		48,				//flatlaser_modal_48
		64,				//flatlaser_modal_64
		16,				//flatlaser_modal_16
		24				//flatlaser_modal_24
	};

	if (bLaserModal >= flatlaser_modal_count)
		bLaserModal = flatlaser_modal_default;

	return nLaserNumberTable[bLaserModal];
}
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pList = (CListCtrl*)lParamSort;
	int nItem1,nItem2;

	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = lParam1;
	nItem1 = pList->FindItem(&FindInfo, -1);
	FindInfo.lParam = lParam2;
	nItem2 = pList->FindItem(&FindInfo, -1);

	if((nItem1 == -1) || (nItem2 == -1)) 
	{ 
		return 0; 
	}

	CString str1, str2;
	str1 = pList->GetItemText(nItem1, 1);
	str2 = pList->GetItemText(nItem2, 1);

	int iCompRes = 0;

	double i1,i2;
	i1 = atof(str1);
	i2 = atof(str2);

	if( i1 > i2) 
		iCompRes = 1; 
	else if( i1 == i2 ) 
		iCompRes = 0; 
	else 
		iCompRes = -1;

	return iCompRes;
}
// CLDBoardDlg �Ի���

IMPLEMENT_DYNAMIC(CLDBoardDlg, CDialog)

CLDBoardDlg::CLDBoardDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLDBoardDlg::IDD, pParent)
, m_nLaserModal(0)
{
	m_pComPort = NULL;

	m_fSetCheckVol = 0;
	
	m_nLaserModal	= flatlaser_modal_default;
	m_nLightNum		= GetLaserNumber_Default(m_nLaserModal);

	m_pParentThread = NULL;

	m_rectSmall.SetRectEmpty();
	m_rectLarge.SetRectEmpty();

}

CLDBoardDlg::~CLDBoardDlg()
{
	m_pComPort = NULL;
}

void CLDBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LD_CHECK_POWER, m_lstLdCheckPower);
	DDX_Control(pDX, IDC_COMBO_LD_VOL_CHECK, m_cbLdVolCheck);
	DDX_Control(pDX, IDC_CHECK_SELECT_ALL_CHANNEL, m_ckSelectAllChannel);
	//	DDX_Control(pDX, IDC_EDIT_LD_SET_DELAY, m_edtSetPowerDelay);
	DDX_Control(pDX, IDC_LIST_LD_VOLTAGE, m_lstLdVoltage);
	DDX_Control(pDX, IDC_PROGRESS_LD_64_STATUS, m_pgsLd64Status);
	DDX_Control(pDX, IDC_EDIT_LD_SET_PC, m_nEditSetPCPower);
	DDX_Control(pDX, IDC_EDIT_LD_SET_LD, m_fEditSetVol);
	DDX_Control(pDX, IDC_EDIT_LD_VOL_CHECK, m_fEditSetCheckVol);
	DDX_Control(pDX, IDC_COMBO_LD_LASERMODAL, m_lasermodalCtrl);
	DDX_CBIndex(pDX, IDC_COMBO_LD_LASERMODAL, m_nLaserModal);
}


BEGIN_MESSAGE_MAP(CLDBoardDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LD_SET_CHECK_POWER, &CLDBoardDlg::OnBnClickedButtonLdSetCheckPower)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CLDBoardDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLDBoardDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LD_CHECK_POWER_DELETE, &CLDBoardDlg::OnBnClickedButtonLdCheckPowerDelete)
	ON_BN_CLICKED(IDC_BUTTON_LD_CHECK_POWER_CLEARALL, &CLDBoardDlg::OnBnClickedButtonLdCheckPowerClearall)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL_CHANNEL, &CLDBoardDlg::OnBnClickedCheckSelectAllChannel)
	ON_BN_CLICKED(IDC_BUTTON_LD_CHECK_POWER_EXPORT, &CLDBoardDlg::OnBnClickedButtonLdCheckPowerExport)
	ON_BN_CLICKED(IDC_BUTTON_LD_CHECK_POWER_IMPORT, &CLDBoardDlg::OnBnClickedButtonLdCheckPowerImport)
	ON_BN_CLICKED(IDC_BUTTON_LD_SETCHECK, &CLDBoardDlg::OnBnClickedButtonLdSetcheck)
	ON_BN_CLICKED(IDC_BUTTON_LD_EXPORT, &CLDBoardDlg::OnBnClickedButtonLdExport)
	ON_BN_CLICKED(IDC_BUTTON_LD_IMPORT, &CLDBoardDlg::OnBnClickedButtonLdImport)
	ON_BN_CLICKED(IDC_BUTTON_EXPAND, &CLDBoardDlg::OnBnClickedButtonExpand)
	ON_BN_CLICKED(IDC_BUTTON_LD_GET_LAST, &CLDBoardDlg::OnBnClickedButtonLdGetLast)
	ON_BN_CLICKED(IDC_BUTTON_LD_GET_NOW, &CLDBoardDlg::OnBnClickedButtonLdGetNow)
	ON_BN_CLICKED(IDC_BUTTON_LD_CORRECT, &CLDBoardDlg::OnBnClickedButtonLdCorrect)
	ON_BN_CLICKED(IDC_BUTTON_LD_SET_LD, &CLDBoardDlg::OnBnClickedButtonLdSetLd)
	ON_BN_CLICKED(IDC_BUTTON_LD_SET_PC, &CLDBoardDlg::OnBnClickedButtonLdSetPc)
	ON_BN_CLICKED(IDC_BUTTON_LP_LIGHTON, &CLDBoardDlg::OnBnClickedButtonLpLighton)
	ON_BN_CLICKED(IDC_BUTTON_LP_LIGHTOFF, &CLDBoardDlg::OnBnClickedButtonLpLightoff)
	ON_BN_CLICKED(IDC_BUTTON_LP_LIGHTON_TIME, &CLDBoardDlg::OnBnClickedButtonLpLightonTime)
	ON_CBN_SELCHANGE(IDC_COMBO_LD_LASERMODAL, &CLDBoardDlg::OnCbnSelchangeComboLdLasermodal)
END_MESSAGE_MAP()


// CLDBoardDlg ��Ϣ�������


BOOL CLDBoardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CString str;
	int i;
	CString strIniFile, strSection, strReg;

	strIniFile = theApp.m_strIniFile;
	strSection = "LDParameter";
	GET_PROFILE_INFO_INT(m_nLaserModal, -1, -1, flatlaser_modal_count, int);
	if (m_nLaserModal < 0 || m_nLaserModal >= flatlaser_modal_count)
	{
		m_nLaserModal = flatlaser_modal_default;
		WRITE_PROFILE_INFO("%d", m_nLaserModal);
	}

	m_lasermodalCtrl.ResetContent();
	for (i=0; i<flatlaser_modal_count; i++)
	{
		str.Format("%2d", GetLaserNumber_Default(i));
		m_lasermodalCtrl.AddString(str);
	}
	if (m_nLaserModal >= m_lasermodalCtrl.GetCount())
		m_nLaserModal = 0;
	m_lasermodalCtrl.SetCurSel(m_nLaserModal);


	m_nEditSetPCPower.SetRange(0,120);
	m_fEditSetVol.SetPrecision(3,1);
	m_fEditSetVol.SetRange(0.000, 5.000);
	m_fEditSetCheckVol.SetPrecision(3,1);
	m_fEditSetCheckVol.SetRange(0.000, 5.000);
	m_nEditSetPCPower.SetWindowText("6");
	m_fEditSetVol.SetWindowText("3.000");
	m_fEditSetVol.SetWindowText("3.000");

	DWORD dwStyle;
	dwStyle = m_lstLdCheckPower.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstLdCheckPower.SetExtendedStyle(dwStyle);
	m_lstLdCheckPower.ModifyStyle(LVS_ICON | LVS_LIST,LVS_REPORT | LVS_SHOWSELALWAYS|LVS_SINGLESEL);
	m_lstLdCheckPower.InsertColumn(0,_T("���"),LVCFMT_LEFT,50); 
	m_lstLdCheckPower.InsertColumn(1,_T("��ѹ(��λ:��)"),LVCFMT_LEFT,80);
	m_lstLdCheckPower.InsertColumn(2,_T("����(��λ:����"),LVCFMT_LEFT,80);

	m_cbLdVolCheck.ResetContent();

	if(m_rectLarge.IsRectNull())
	{
		CRect rectSeparator;
		GetWindowRect(&m_rectLarge);
		GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);

		m_rectSmall.left=m_rectLarge.left;
		m_rectSmall.top=m_rectLarge.top;
		m_rectSmall.right=rectSeparator.right;
		m_rectSmall.bottom=m_rectLarge.bottom;
	}
	SetDlgItemText(IDC_BUTTON_EXPAND,"��>>");
	SetWindowPos(NULL,0,0,m_rectSmall.Width(),m_rectSmall.Height(),
		SWP_NOMOVE | SWP_NOZORDER);

	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_CHECKBOXES;
	m_lstLdVoltage.SetExtendedStyle(dwStyle);
	m_lstLdVoltage.SetExtendedStyle(dwStyle);
	m_lstLdVoltage.ModifyStyle(LVS_ICON | LVS_LIST,LVS_REPORT | LVS_SHOWSELALWAYS);

	const char TITLE_LABEL[ldvoltagelist_count][40] = 
	{
		_T("ͨ��"),
		_T("���õ�ѹ"),
		_T("�ϴ�ʵ���ѹ"),
		_T("��ǰʵ���ѹ"),
		_T("�ϴε�Դ���ѹ"),
		_T("��ǰ��Դ���ѹ"),
		_T("���ù���"),
		_T("ʵ�ʹ���"),
		_T("״̬")
	};

	CString strInfo;

	for (i=0; i<ldvoltagelist_count; i++)
	{
		m_lstLdVoltage.InsertColumn(i, TITLE_LABEL[i], LVCFMT_CENTER, 60);
	}

	// Sets the width of a column in a report-mode control.
	m_lstLdVoltage.SetColumnWidth(ldvoltagelist_ch, 40);
	m_lstLdVoltage.SetColumnWidth(ldvoltagelist_state, 70);
// #ifdef _CONFIG_CLIENT_
// 	m_lstLdVoltage.SetColumnWidth(ldvoltagelist_pc, 0);
// 	m_lstLdVoltage.SetColumnWidth(ldvoltagelist_lastPD, 0);
// 	m_lstLdVoltage.SetColumnWidth(ldvoltagelist_nowPD, 0);
// 	m_lstLdVoltage.SetColumnWidth(ldvoltagelist_lastLD, 0);
// 	m_lstLdVoltage.SetColumnWidth(ldvoltagelist_nowLD, 0);
// #endif

	ResetListForLightNum();

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->SetWindowText("500");
// 	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_LD))->SetWindowText("0.000");
// 	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_PC))->SetWindowText("0");
	((CWnd*)GetDlgItem(IDC_EDIT_LIGHTON_TIME))->SetWindowText("1000");
	
	if (PathFileExists(theApp.m_strIniFile))
	{
		LDParameterLoadFromCfg64(theApp.m_strIniFile);
	}
	LDCreateVPTable();

	UpdateData(FALSE);

	EnableControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLDBoardDlg::RestrictTableAec()
{
	CString str = "";
	int num = m_lstLdCheckPower.GetItemCount(); 

	while(num--)
		m_lstLdCheckPower.SetItemData(num, num);  

	m_lstLdCheckPower.SortItems(CompareFunc, ((LPARAM)&m_lstLdCheckPower)); 

	for (int i=0;i<m_lstLdCheckPower.GetItemCount();i++)
	{
		str.Format("%d",i);
		m_lstLdCheckPower.SetItemText(i,0,str);
	}
}
void CLDBoardDlg::OnBnClickedButtonLdSetCheckPower()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bResult = FALSE;
	CString str = "";
	BYTE bChannel;
	WORD wVol;

	bChannel = m_cbLdVolCheck.GetCurSel()+1;

	m_fSetCheckVol = double(m_fEditSetCheckVol);

	if (m_fSetCheckVol<0 || m_fSetCheckVol>LD_MAX_VOLUME)
	{
		str.Format("��ѹֵ��Χ������ 0 - %.3fV ֮��",LD_MAX_VOLUME);
		AfxMessageBox(str);
		return;
	}

	wVol = m_fSetCheckVol*1000;

	m_pComPort->m_protocolstatus.wFeedBackCheckVol = 0;

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);

	bResult = m_pComPort->ld_set_laser_test_power(bChannel, wVol, atoi(str), FALSE);

	if (bResult==TRUE)
	{
		if (m_pComPort->m_protocolstatus.bPrintWay_ld_voltage==ld_comm_result_succeed
			&& m_pComPort->m_protocolstatus.bFeedBackChannel == bChannel)
		{		
			if (m_fSetCheckVol>0)
			{
				CLDSetPower dlg(this);
				const int size = ld_voltage_max/(8*sizeof(DWORD));
				DWORD dwLightData[size];

				memset(dwLightData, 0, sizeof(DWORD)*size);
				dlg.m_fVol = (float)((float)m_pComPort->m_protocolstatus.ldVoltage.m_wNowPD/1000.0);
				dlg.DoModal();

				m_pComPort->ld_set_light_off(dwLightData, atoi(str), FALSE);
			}
		}
		else
		{
			((CWnd *)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
		}
	}
	else
	{
		((CWnd *)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
	}
}
void CLDBoardDlg::LDSaveVPCurve(const char *pszProfile)
{
	//��ѹ���ʹ�ϵ��
	CString strIniFile,strSection,strReg,str;

	strIniFile = pszProfile;
	strSection = "LDVolPowerTable";
	
	//ɾ��ԭ����
	WritePrivateProfileString(strSection,NULL,NULL,strIniFile);

	int nVpNo[VP_NUM_MAX];
	float fVpVol[VP_NUM_MAX];
	int nVpPower[VP_NUM_MAX];
	int nVpNum = 0;

	for (int i=0;i<m_lstLdCheckPower.GetItemCount();i++)
	{
		if (i>=VP_NUM_MAX)
		{
			break;
		}
		str = m_lstLdCheckPower.GetItemText(i,0);
		nVpNo[i] = atoi(str);
		str = m_lstLdCheckPower.GetItemText(i,1);
		fVpVol[i] = atof(str);
		str = m_lstLdCheckPower.GetItemText(i,2);

		if (str=="")
		{
			nVpPower[i] = -1;
		}
		else
		{
			nVpPower[i] = atoi(str);
		}
		nVpNum++;
	}
	WRITE_PROFILE_INFO("%d",nVpNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nVpNo,nVpNum);
	WRITE_PROFILE_INFO_ARRAY("%.3f",fVpVol,nVpNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nVpPower,nVpNum);
}

void CLDBoardDlg::LDLoadVPCurve(const char *pszProfile)
{
	//��ѹ���ʹ�ϵ��
	m_lstLdCheckPower.DeleteAllItems();

	CString strIniFile,strSection,str;
	char szReg[200];

	strIniFile = pszProfile;
	strSection = "LDVolPowerTable";

	int nVpNo[VP_NUM_MAX];
	float fVpVol[VP_NUM_MAX];
	int nVpPower[VP_NUM_MAX];
	int nVpNum = 0;

	GET_PROFILE_INFO_INT(nVpNum,0,0,VP_NUM_MAX,UINT);
	GET_PROFILE_INFO_ARRAY_INT(nVpNo,nVpNum,0,0,VP_NUM_MAX,UINT);
	GET_PROFILE_INFO_ARRAY_DOUBLE(fVpVol,nVpNum,0,0,LD_MAX_VOLUME);
	GET_PROFILE_INFO_ARRAY_INT(nVpPower,nVpNum,-1,0,CFG_MAX_POWER,INT);

	for (int i=0;i<nVpNum;i++)
	{
		m_lstLdCheckPower.InsertItem(i,"");
		str.Format("%d",nVpNo[i]);
		m_lstLdCheckPower.SetItemText(i,0,str);
		str.Format("%.3f",fVpVol[i]);
		m_lstLdCheckPower.SetItemText(i,1,str);

		if (nVpPower[i]==-1)
		{
			str = "";
		}
		else
		{
			str.Format("%d",nVpPower[i]);
		}
		m_lstLdCheckPower.SetItemText(i,2,str);
	}
	RestrictTableAec();

	LDCreateVPTable();
}


void CLDBoardDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	LDParameterSaveToCfg64(theApp.m_strIniFile);
	CDialog::OnClose();
}

void CLDBoardDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	LDParameterSaveToCfg64(theApp.m_strIniFile);
	OnOK();
}

void CLDBoardDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LDParameterSaveToCfg64(theApp.m_strIniFile);
	OnCancel();
}

void CLDBoardDlg::LD_Check_Power_Add(float fVol,int nPower)
{
	CString str = "";

	for (int i=0;i<m_lstLdCheckPower.GetItemCount();i++)
	{
		str = m_lstLdCheckPower.GetItemText(i,1);
		float fV = atof(str);

		if (fV==fVol)
		{
			str.Format("�����Ѵ��ڵ�ѹֵΪ %.3f ����Ƿ��޸ģ�",fVol);
			if (AfxMessageBox(str,MB_YESNO)==IDYES)
			{
				LD_Check_Power_Modify(fVol,nPower);
			}
			return;
		}
	}

	m_lstLdCheckPower.InsertItem(0,"");

	str.Format("%.3f",fVol);
	m_lstLdCheckPower.SetItemText(0,1,str);

	str.Format("%d",nPower);
	m_lstLdCheckPower.SetItemText(0,2,str);

	m_lstLdCheckPower.SetFocus();
	m_lstLdCheckPower.SetItemState(0, LVIS_FOCUSED, LVIS_FOCUSED);
	m_lstLdCheckPower.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

	RestrictTableAec();

	if(!IsVPTableResonable())
	{	
		AfxMessageBox("��ȷ����ѹ���ʹ�ϵ���еĹ���ֵ�Ǵ�С����˳�����е�",MB_ICONERROR);

		for(int i=0;i<m_lstLdCheckPower.GetItemCount();i++)
		{
			CString s = m_lstLdCheckPower.GetItemText(i,2);
			int p = atoi(s);

			if (p==nPower)
			{
				m_lstLdCheckPower.DeleteItem(i);
				RestrictTableAec();
				LDCreateVPTable();
				break;
			}
		}
	}
	else
	{
		LDCreateVPTable();
	}
}

void CLDBoardDlg::LD_Check_Power_Modify(float fVol,int nPower)
{
	CString str = "";
	int nItem = -1;
	int nP;
	float fV;

	for (int i=0;i<m_lstLdCheckPower.GetItemCount();i++)
	{
		str = m_lstLdCheckPower.GetItemText(i,1);
		fV = atof(str);
		str = m_lstLdCheckPower.GetItemText(i,2);
		nP = atoi(str);

		if (fV==fVol)
		{
			nItem = i;
			break;
		}
	}

	if (nItem==-1)
	{
		str.Format("�����޵�ѹֵΪ %.3f ��������Ƿ���ӣ�",fVol);
		if (AfxMessageBox(str,MB_YESNO)==IDYES)
		{
			LD_Check_Power_Add(fVol,nPower);
		}
		return;
	}

	str.Format("%d",nPower);
	m_lstLdCheckPower.SetItemText(nItem,2,str);

	m_lstLdCheckPower.SetFocus();
	m_lstLdCheckPower.SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);
	m_lstLdCheckPower.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);

	if(!IsVPTableResonable())
	{
		AfxMessageBox("��ȷ����ѹ���ʹ�ϵ���еĹ���ֵ�Ǵ�С����˳�����е�",MB_ICONERROR);

		str.Format("%.3f",fV);
		m_lstLdCheckPower.SetItemText(nItem,1,str);
		str.Format("%d",nP);
		m_lstLdCheckPower.SetItemText(nItem,2,str);
		LDCreateVPTable();
	}
	else
	{
		LDCreateVPTable();
	}
}

void CLDBoardDlg::OnBnClickedButtonLdCheckPowerDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str = "";

	POSITION pos = m_lstLdCheckPower.GetFirstSelectedItemPosition();
	int nItem = -1;

	if (pos!=NULL)
	{
		while(pos)
		{
			nItem = m_lstLdCheckPower.GetNextSelectedItem(pos);
			m_lstLdCheckPower.DeleteItem(nItem);
		}
	}
	for (int i=0;i<m_lstLdCheckPower.GetItemCount();i++)
	{
		str.Format("%d",i+1);
		m_lstLdCheckPower.SetItemText(i,0,str);////
	}
	LDCreateVPTable();

}

void CLDBoardDlg::OnBnClickedButtonLdCheckPowerClearall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lstLdCheckPower.DeleteAllItems();
	LDCreateVPTable();
}

BOOL CLDBoardDlg::LDCreateVPTable()
{
	m_VolPowerControlPoints.RemoveAll();

	CString str = "";
	int x =0,y = 0;
	CPoint ptControl(0,0);

	for(int i=0;i<m_lstLdCheckPower.GetItemCount();i++)
	{
		str = m_lstLdCheckPower.GetItemText(i,2);

		if (str=="")
		{
			continue;
		}
		x = atof(str) * LD_CURVE_RATE_X;
		str = m_lstLdCheckPower.GetItemText(i,1);
		y = atof(str) * 1000;

		ptControl.x = x;
		ptControl.y = 0 - y;

		m_VolPowerControlPoints.Add(ptControl);
	}
	return TRUE;
}

BOOL CLDBoardDlg::LDCreateVPCurve()
{	
	m_VolPowerCurvePoints.RemoveAll();

	if (m_VolPowerControlPoints.GetCount()>1)
	{
		Spline spline(m_VolPowerControlPoints.GetData(), m_VolPowerControlPoints.GetSize());
		spline.Generate();

		m_VolPowerCurvePoints.SetSize(spline.GetCurveCount(TABLE_CURVE_DIV_FACTOR));
		int PointCount = 0;
		spline.GetCurve(m_VolPowerCurvePoints.GetData(), PointCount, TABLE_CURVE_DIV_FACTOR);

		return TRUE;
	}

	return FALSE;
}

BOOL CLDBoardDlg::IsVPTableResonable()
{
	CString str = "";
	int nTemp,nValue;

	for (int i=0;i<m_lstLdCheckPower.GetItemCount();i++)
	{
		str = m_lstLdCheckPower.GetItemText(i,2);
		nValue = atoi(str);
		int nRow = i;

		for (int i=nRow+1;i<=m_lstLdCheckPower.GetItemCount()-1;i++)
		{
			str = m_lstLdCheckPower.GetItemText(i,2);
			nTemp = atoi(str);

			if (nTemp<nValue)
			{
				return FALSE;
			}
		}

		for (int i=0;i<=nRow-1;i++)
		{
			str = m_lstLdCheckPower.GetItemText(i,2);
			nTemp = atoi(str);
			if (nTemp>nValue)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CLDBoardDlg::OnBnClickedCheckSelectAllChannel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_ckSelectAllChannel.GetCheck())
	{
		for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
		{
			m_lstLdVoltage.SetCheck(i,TRUE);
		}
	}
	else
	{
		for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
		{
			m_lstLdVoltage.SetCheck(i,FALSE);
		}
	}
}
void CLDBoardDlg::OnBnClickedButtonLdCheckPowerExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (AfxMessageBox("���ú�Ḳ�ǰ忨�д�Ĺ�ϵ��ȷ��Ҫд��忨��",MB_YESNO|MB_ICONWARNING)==IDNO)
		return;

	BOOL bResult = FALSE;
	CString str = "";

	BZERO(m_pComPort->m_protocolstatus.wMapVolValue);
	BZERO(m_pComPort->m_protocolstatus.bMapPowerValue);

	m_pComPort->m_protocolstatus.bMapNum = m_lstLdCheckPower.GetItemCount();

	for (int i=0;i<m_pComPort->m_protocolstatus.bMapNum;i++)
	{
		str = m_lstLdCheckPower.GetItemText(i,1);
		m_pComPort->m_protocolstatus.wMapVolValue[i] = atof(str) * 1000;

		str = m_lstLdCheckPower.GetItemText(i,2);
		m_pComPort->m_protocolstatus.bMapPowerValue[i] = atoi(str);
	}

	bResult = m_pComPort->ld_set_map_data(m_pComPort->m_protocolstatus.wMapVolValue, m_pComPort->m_protocolstatus.bMapPowerValue, m_pComPort->m_protocolstatus.bMapNum, 20000, FALSE);

	if (bResult==TRUE)
	{
		if (m_pComPort->m_protocolstatus.bConfigWay_map_data==ld_comm_result_succeed)
		{
			((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ɹ�");
		}
		else
		{
			((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
		}	
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
	}
}

void CLDBoardDlg::OnBnClickedButtonLdCheckPowerImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (AfxMessageBox("��ѯ��Ḳ���Ϸ�������ݣ�ȷ��Ҫ��ȡ�忨��",MB_YESNO|MB_ICONWARNING)==IDNO)
		return;

	BOOL bResult = FALSE;
	CString str = "";

	BZERO(m_pComPort->m_protocolstatus.wMapVolValue);
	BZERO(m_pComPort->m_protocolstatus.bMapPowerValue);
	m_pComPort->m_protocolstatus.bMapNum = 0;

	bResult = m_pComPort->ld_get_map_data(20000, FALSE);

	if (bResult==TRUE)
	{
		if (m_pComPort->m_protocolstatus.bQueryWay_map_data==ld_comm_result_succeed)
		{
			m_lstLdCheckPower.DeleteAllItems();

			for (int i=0; i<m_pComPort->m_protocolstatus.bMapNum; i++)
			{
				m_lstLdCheckPower.InsertItem(i,"");

				str.Format("%d",i+1);
				m_lstLdCheckPower.SetItemText(i, 0, str);
				str.Format("%.3f",(float)m_pComPort->m_protocolstatus.wMapVolValue[i]/1000.0);
				m_lstLdCheckPower.SetItemText(i, 1, str);
				str.Format("%d",m_pComPort->m_protocolstatus.bMapPowerValue[i]);
				m_lstLdCheckPower.SetItemText(i, 2, str);
			}

			((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("��ȡ�ɹ�");
			LDCreateVPTable();
		}
		else
		{
			((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("��ȡʧ��");
		}	
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
	}
}

void CLDBoardDlg::OnBnClickedButtonLdSetcheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_lstLdVoltage.GetFirstSelectedItemPosition();

	if (pos!=NULL)
	{
		while(pos)
		{
			int nItem = m_lstLdVoltage.GetNextSelectedItem(pos);
			m_lstLdVoltage.SetCheck(nItem,TRUE);
		}
	}
}

void CLDBoardDlg::OnBnClickedButtonLdExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COleDateTime ExportTime = COleDateTime::GetCurrentTime();
	CString str = "",strFilePath;

	str = ExportTime.Format("LDBoardInfo-%Y%m%d%H%M%S.LDCFG");

	CFileDialog dlg(FALSE,"*.LDCFG", str, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		"�����ļ� (*.LDCFG)|*.LDCFG|All Files (*.*)|*.*||", this);

	dlg.m_ofn.lpstrTitle = "��������Ϣ";

	if (dlg.DoModal() == IDOK)
	{
		strFilePath = dlg.GetPathName();

		LDParameterSaveToCfg64(strFilePath);

		str.Format("�ѳɹ������� %s",strFilePath);

		AfxMessageBox(str);
	}
}

void CLDBoardDlg::OnBnClickedButtonLdImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,"*.LDCFG", "", 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		"�����ļ� (*.LDCFG)|*.LDCFG|All Files (*.*)|*.*||", this);

	dlg.m_ofn.lpstrTitle = "�������Ϣ";

	CString strFilePath = "",str = "";

	if (dlg.DoModal() == IDOK)
	{
		strFilePath = dlg.GetPathName();

		if (!PathFileExists(strFilePath))
		{
			str.Format("%s ������",strFilePath);
			AfxMessageBox(str);
			return;
		}

		m_lstLdCheckPower.DeleteAllItems();

		LDParameterLoadFromCfg64(strFilePath);

		AfxMessageBox("����ɹ�!");
	}
}

void CLDBoardDlg::OnBnClickedButtonExpand()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	if(GetDlgItemText(IDC_BUTTON_EXPAND,str),str=="��>>")
	{
		SetDlgItemText(IDC_BUTTON_EXPAND,"��<<");
		SetWindowPos(NULL,0,0,m_rectLarge.Width(),m_rectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_EXPAND,"��>>");
		SetWindowPos(NULL,0,0,m_rectSmall.Width(),m_rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
}

void CLDBoardDlg::OnBnClickedButtonLdGetLast()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bResult = FALSE;
	CString str = "";
	int nChannel = 0,j=0;
	
	for(int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			nChannel++;
		}
	}

	if (nChannel==0)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("�빴ѡҪ��ѯ��ͨ��!");
		return;
	}
	
	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"");
	}
	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_lastPD,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_lastLD,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"���ڲ�ѯ");
		}
	}

	AProgress prog;

	prog.SetPaintWnd(&m_pgsLd64Status, NULL);
	prog.Range(nChannel, 0);

	prog.Current(0);
	
	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			m_lstLdVoltage.EnsureVisible(i,TRUE);
			m_lstLdVoltage.UpdateWindow();

			BZERO(m_pComPort->m_protocolstatus.ldVoltage);
			((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);

			bResult = m_pComPort->ld_get_last_power(i+1,atoi(str), FALSE);

			if (bResult==TRUE)
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("");
				if (m_pComPort->m_protocolstatus.bPrintWay_ld_voltage==ld_comm_result_succeed
					&& m_pComPort->m_protocolstatus.bFeedBackChannel == i+1)
				{
					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wPC/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc, str);

					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wLastPD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_lastPD, str);

					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wLastLD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_lastLD, str);

					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "��ѯ�ɹ�");
				}
				else
				{
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "��ѯʧ��");
				}
				
				if (VK_ESCAPE == prog.Current(++j))
				{
					break;
				}
				m_lstLdVoltage.UpdateWindow();
			}
			else
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
				break;
			}
		}
	}
	prog.End();
}

void CLDBoardDlg::OnBnClickedButtonLdGetNow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bResult = FALSE;
	CString str = "";
	int nChannel = 0,j=0;

	for(int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			nChannel++;
		}
	}

	if (nChannel==0)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("�빴ѡҪ��ѯ��ͨ��!");
		return;
	}

	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"");
	}
	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_power,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"���ڲ�ѯ");
		}
	}

	AProgress prog;

	prog.SetPaintWnd(&m_pgsLd64Status, NULL);
	prog.Range(nChannel, 0);

	prog.Current(0);
	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			m_lstLdVoltage.EnsureVisible(i,TRUE);
			m_lstLdVoltage.UpdateWindow();

			BZERO(m_pComPort->m_protocolstatus.ldVoltage);
			((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);

			bResult = m_pComPort->ld_get_now_power(i+1,atoi(str), FALSE);

			if (bResult==TRUE)
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("");
				if (m_pComPort->m_protocolstatus.bPrintWay_ld_voltage==ld_comm_result_succeed
					&& m_pComPort->m_protocolstatus.bFeedBackChannel == (i+1))
				{
					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wPC/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc, str);

					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wNowPD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD, str);

					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wNowLD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD, str);

					if(LDCreateVPCurve())
					{
						//��ǰʵ�ʹ����Ե�ǰʵ���ѹΪ��׼
						float fVol = float(float(m_pComPort->m_protocolstatus.ldVoltage.m_wNowPD)/1000.0);	//m_wPC

						int nVol = fVol*1000;
						int nVolMin = 0 - m_VolPowerControlPoints.GetAt(0).y;
						int nVolMax = 0 - m_VolPowerControlPoints.GetAt((m_VolPowerControlPoints.GetCount()-1)).y;
						int nSetPower = 0;

						if (nVol>nVolMax)
						{
							nSetPower = m_VolPowerControlPoints.GetAt((m_VolPowerControlPoints.GetCount()-1)).x;
						}
						else if (nVol<nVolMin)
						{
							nSetPower = m_VolPowerControlPoints.GetAt(0).x;
						}
						else
						{
							for (int i=0;i<m_VolPowerCurvePoints.GetSize();i++)
							{
								int nSetVol = 0 - m_VolPowerCurvePoints.GetAt(i).y;
								if (nSetVol==nVol)
								{
									nSetPower = m_VolPowerCurvePoints.GetAt(i).x;
									break;
								}
							}
						}
						nSetPower = nSetPower/LD_CURVE_RATE_X;
						str.Format("%d",nSetPower);
						m_lstLdVoltage.SetItemText(i, ldvoltagelist_power, str);
					}


					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "��ѯ�ɹ�");
				}
				else
				{
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "��ѯʧ��");
				}
				if (VK_ESCAPE == prog.Current(++j))
				{
					break;
				}
				m_lstLdVoltage.UpdateWindow();
			}
			else
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
				break;
			}
		}
	}
	prog.End();
}

void CLDBoardDlg::OnBnClickedButtonLdCorrect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bResult = FALSE;
	CString str = "";
	int nChannel = 0,j=0;

	LDCreateVPCurve();

	for(int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			nChannel++;
		}
	}

	if (nChannel==0)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("�빴ѡҪУ����ͨ��!");
		return;
	}

	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"");
	}
	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_power,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"����У��");
		}
	}

	AProgress prog;

	prog.SetPaintWnd(&m_pgsLd64Status, NULL);
	prog.Range(nChannel, 0);

	prog.Current(0);

	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("");
		if (m_lstLdVoltage.GetCheck(i))
		{
			m_lstLdVoltage.EnsureVisible(i,TRUE);
			m_lstLdVoltage.UpdateWindow();

			BZERO(m_pComPort->m_protocolstatus.ldVoltage);
			((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);

			bResult = m_pComPort->ld_correct_power(i+1,atoi(str), FALSE);

			if (bResult==TRUE)
			{
				if (m_pComPort->m_protocolstatus.bPrintWay_ld_voltage==ld_comm_result_succeed
					&& m_pComPort->m_protocolstatus.bFeedBackChannel == (i+1))
				{
					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wPC/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc, str);

					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wNowPD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD, str);

					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wNowLD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD, str);

					if(LDCreateVPCurve())
					{
						//��ǰʵ�ʹ����Ե�ǰʵ���ѹΪ��׼
						float fVol = float(float(m_pComPort->m_protocolstatus.ldVoltage.m_wNowPD)/1000.0);	//m_wPC

						int nVol = fVol*1000;
						int nVolMin = 0 - m_VolPowerControlPoints.GetAt(0).y;
						int nVolMax = 0 - m_VolPowerControlPoints.GetAt((m_VolPowerControlPoints.GetCount()-1)).y;
						int nSetPower = 0;

						if (nVol>nVolMax)
						{
							nSetPower = m_VolPowerControlPoints.GetAt((m_VolPowerControlPoints.GetCount()-1)).x;
						}
						else if (nVol<nVolMin)
						{
							nSetPower = m_VolPowerControlPoints.GetAt(0).x;
						}
						else
						{
							for (int i=0;i<m_VolPowerCurvePoints.GetSize();i++)
							{
								int nSetVol = 0 - m_VolPowerCurvePoints.GetAt(i).y;
								if (nSetVol==nVol)
								{
									nSetPower = m_VolPowerCurvePoints.GetAt(i).x;
									break;
								}
							}
						}
						nSetPower = nSetPower/LD_CURVE_RATE_X;
						str.Format("%d",nSetPower);
						m_lstLdVoltage.SetItemText(i, ldvoltagelist_power, str);
					}

					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "У���ɹ�");
				}
				else
				{
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "У��ʧ��");
				}
				if (VK_ESCAPE == prog.Current(++j))
				{
					break;
				}
				m_lstLdVoltage.UpdateWindow();
			}
			else
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
				break;
			}
		}
	}

	prog.End();
}

void CLDBoardDlg::OnBnClickedButtonLdSetLd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bResult = FALSE;
	CString str = "";
	int nChannel = 0,j=0,nSetV = 0;
	float fTmp;
	
	for(int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			nChannel++;
		}
	}

	if (nChannel==0)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("�빴ѡҪ���õ�ͨ��!");
		return;
	}
	
	fTmp = double(m_fEditSetVol);
	nSetV = fTmp*1000;

// 	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
// 	{
// 		m_lstLdVoltage.SetItemText(i+1, ldvoltagelist_state,"");
// 	}
	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
//			m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD,str);
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"��������");
		}
	}

	AProgress prog;

	prog.SetPaintWnd(&m_pgsLd64Status, NULL);
	prog.Range(nChannel, 0);

	prog.Current(0);

	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			m_lstLdVoltage.EnsureVisible(i,TRUE);
			m_lstLdVoltage.UpdateWindow();

			BZERO(m_pComPort->m_protocolstatus.ldVoltage);
			((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);

			bResult = m_pComPort->ld_set_ld_power(i+1,nSetV,atoi(str), FALSE);

			if (bResult==TRUE)
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("");
				if (m_pComPort->m_protocolstatus.bPrintWay_ld_voltage==ld_comm_result_succeed
					&& m_pComPort->m_protocolstatus.bFeedBackChannel == i+1)
				{
					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wNowPD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD, str);

					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wNowLD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD, str);

					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "���óɹ�");
				}
				else
				{
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "����ʧ��");
				}
				if (VK_ESCAPE==prog.Current(++j))
				{
					break;
				}
				m_lstLdVoltage.UpdateWindow();
			}
			else
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
				break;
			}
		}
	}
	prog.End();
}

void CLDBoardDlg::OnBnClickedButtonLdSetPc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bResult = FALSE;
	CString str = "";
	int nChannel = 0,j=0,nSetPower = 0;
	float fVol = 0;

	for(int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			nChannel++;
		}
	}

	if (nChannel==0)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("�빴ѡҪ���õ�ͨ��!");
		return;
	}
	
	if (m_VolPowerControlPoints.GetCount()<2)
	{
		AfxMessageBox("���������2����������ѹ���ʹ�ϵ��");
		return;
	}
	if(!IsVPTableResonable())
	{	
		AfxMessageBox("��ȷ����ѹ���ʹ�ϵ���еĹ���ֵ�Ǵ�С����˳�����е�",MB_ICONERROR);
		return;
	}


	LDCreateVPCurve();

	nSetPower = int(m_nEditSetPCPower);
	
	int nMin = m_VolPowerControlPoints.GetAt(0).x/LD_CURVE_RATE_X;
	int nMax = m_VolPowerControlPoints.GetAt(m_VolPowerControlPoints.GetCount()-1).x/LD_CURVE_RATE_X;

	if (nSetPower>nMax)
	{
		nSetPower = nMax;
	}

	if (nSetPower<nMin)
	{
		nSetPower = nMin;
	}

	for (int j=0;j<m_VolPowerCurvePoints.GetSize();j++)
	{
		int nPowrX = m_VolPowerCurvePoints.GetAt(j).x;
		if (nPowrX==nSetPower*LD_CURVE_RATE_X)
		{
			fVol = (float(0 - m_VolPowerCurvePoints.GetAt(j).y))/1000.0;
			break;
		}
	}

	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"");
	}
	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			str.Format("%.3f",fVol);
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc,str);
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD,"");
			str.Format("%d",nSetPower);
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_setpower,str);
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_power,"");
			m_lstLdVoltage.SetItemText(i, ldvoltagelist_state,"��������");
		}
	}

	AProgress prog;

	prog.SetPaintWnd(&m_pgsLd64Status, NULL);
	prog.Range(nChannel, 0);

	prog.Current(0);

	for (int i=0;i<m_lstLdVoltage.GetItemCount();i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			m_lstLdVoltage.EnsureVisible(i,TRUE);
			m_lstLdVoltage.UpdateWindow();

			BZERO(m_pComPort->m_protocolstatus.ldVoltage);
			((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);

			bResult = m_pComPort->ld_set_pc_power(i+1,fVol*1000,atoi(str), FALSE);

			if (bResult==TRUE)
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("");
				if (m_pComPort->m_protocolstatus.bPrintWay_ld_voltage==ld_comm_result_succeed
					&& m_pComPort->m_protocolstatus.bFeedBackChannel == (i+1))
				{
					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wPC/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc, str);

					//��ǰʵ�ʹ����Ե�ǰʵ���ѹΪ��׼
					float fVol = float(float(m_pComPort->m_protocolstatus.ldVoltage.m_wNowPD)/1000.0);	//m_wPC

					int nVol = fVol*1000;
					int nVolMin = 0 - m_VolPowerControlPoints.GetAt(0).y;
					int nVolMax = 0 - m_VolPowerControlPoints.GetAt((m_VolPowerControlPoints.GetCount()-1)).y;
					int nSetPower = 0;

					if (nVol>nVolMax)
					{
						nSetPower = m_VolPowerControlPoints.GetAt((m_VolPowerControlPoints.GetCount()-1)).x;
					}
					else if (nVol<nVolMin)
					{
						nSetPower = m_VolPowerControlPoints.GetAt(0).x;
					}
					else
					{
						for (int i=0;i<m_VolPowerCurvePoints.GetSize();i++)
						{
							int nSetVol = 0 - m_VolPowerCurvePoints.GetAt(i).y;
							if (nSetVol==nVol)
							{
								nSetPower = m_VolPowerCurvePoints.GetAt(i).x;
								break;
							}
						}
					}
					nSetPower = nSetPower/LD_CURVE_RATE_X;
					str.Format("%d",nSetPower);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_power, str);
					
					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wNowPD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD, str);

					str.Format("%.3f",(double)m_pComPort->m_protocolstatus.ldVoltage.m_wNowLD/(double)1000);
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD, str);

					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "���óɹ�");
				}
				else
				{
					m_lstLdVoltage.SetItemText(i, ldvoltagelist_state, "����ʧ��");
				}
				if (VK_ESCAPE == prog.Current(++j))
					break;
				m_lstLdVoltage.UpdateWindow();
			}
			else
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("����ʧ��");
				break;
			}
		}
	}
	prog.End();
}

void CLDBoardDlg::LDParameterSaveToCfg64(const char *pszProfile)
{
	CString strIniFile,strSection,strReg,str;

	strIniFile = pszProfile;
	strSection = "LDParameter";

	WRITE_PROFILE_INFO("%d", m_nLaserModal);

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_PC))->GetWindowText(str);
	int nSetPcPower = atoi(str);
	WRITE_PROFILE_INFO("%d", nSetPcPower)

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_LD))->GetWindowText(str);
	float fSetLdVol = atof(str);
	WRITE_PROFILE_INFO("%.3f", fSetLdVol);

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);
	int nSetTimeOut = atoi(str);
	WRITE_PROFILE_INFO("%d", nSetTimeOut);

	((CWnd*)GetDlgItem(IDC_EDIT_LD_VOL_CHECK))->GetWindowText(str);
	float fSetCheckVol = atof(str);
	WRITE_PROFILE_INFO("%.3f", fSetCheckVol);

	float fSetPcV[ld_voltage_max];
	float fSetPdLastV[ld_voltage_max];
	float fSetLdLastV[ld_voltage_max];
	int nSetPcP[ld_voltage_max];

	for (int i=0;i<m_nLightNum;i++)
	{
		str = m_lstLdVoltage.GetItemText(i, ldvoltagelist_pc);
		fSetPcV[i] = atof(str);
		str = m_lstLdVoltage.GetItemText(i, ldvoltagelist_lastPD);
		fSetPdLastV[i] = atof(str);
		str = m_lstLdVoltage.GetItemText(i, ldvoltagelist_lastLD);
		fSetLdLastV[i] = atof(str);
		str = m_lstLdVoltage.GetItemText(i, ldvoltagelist_setpower);
		nSetPcP[i] = atoi(str);
	}

	WRITE_PROFILE_INFO_ARRAY("%f",fSetPcV,m_nLightNum);
	WRITE_PROFILE_INFO_ARRAY("%f",fSetPdLastV,m_nLightNum);
	WRITE_PROFILE_INFO_ARRAY("%f",fSetLdLastV,m_nLightNum);
	WRITE_PROFILE_INFO_ARRAY("%d",nSetPcP,m_nLightNum);

	LDSaveVPCurve(strIniFile);
}

void CLDBoardDlg::LDParameterLoadFromCfg64(const char *pszProfile)
{
	CString strIniFile,strSection,str;
	char szReg[200];

	strIniFile = pszProfile;
	strSection = "LDParameter";

	int nSetPcPower = 0;
	GET_PROFILE_INFO_INT(nSetPcPower,0,0,CFG_MAX_POWER,UINT);
	str.Format("%d",nSetPcPower);
	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_PC))->SetWindowText(str);

	float fSetLdVol = 0;
	GET_PROFILE_INFO_FLOAT(fSetLdVol,0,0,LD_MAX_VOLUME);
	str.Format("%.3f",fSetLdVol);
	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_LD))->SetWindowText(str);

	int nSetTimeOut = 0;
	GET_PROFILE_INFO_INT(nSetTimeOut,0,0,10000000,UINT);
	str.Format("%d",nSetTimeOut);
	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->SetWindowText(str);

	float fSetCheckVol = 0;
	GET_PROFILE_INFO_FLOAT(fSetCheckVol,0,0,LD_MAX_VOLUME);
	str.Format("%.3f",fSetCheckVol);
	((CWnd*)GetDlgItem(IDC_EDIT_LD_VOL_CHECK))->SetWindowText(str);

	float fSetPcV[ld_voltage_max];
	float fSetPdLastV[ld_voltage_max];
	float fSetLdLastV[ld_voltage_max];
	int nSetPcP[ld_voltage_max];
	
	GET_PROFILE_INFO_ARRAY_DOUBLE(fSetPcV,m_nLightNum,0,0,LD_MAX_VOLUME);
	GET_PROFILE_INFO_ARRAY_DOUBLE(fSetPdLastV,m_nLightNum,0,0,LD_MAX_VOLUME);
	GET_PROFILE_INFO_ARRAY_DOUBLE(fSetLdLastV,m_nLightNum,0,0,LD_MAX_VOLUME);
	GET_PROFILE_INFO_ARRAY_INT(nSetPcP,m_nLightNum,0,0,CFG_MAX_POWER,UINT);
	
	for (int i=0;i<m_nLightNum;i++)
	{
		str.Format("%.3f",fSetPcV[i]);
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc,str);
		str.Format("%.3f",fSetPdLastV[i]);
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_lastPD,str);
		str.Format("%.3f",fSetLdLastV[i]);
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_lastLD,str);
		str.Format("%d",nSetPcP[i]);
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_setpower,str);
	}

	LDLoadVPCurve(strIniFile);
}

void CLDBoardDlg::OnBnClickedButtonLpLighton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BOOL bResult;
	int i,nChannel;
	CString str;
	const int size = ld_voltage_max/(8*sizeof(DWORD));
	DWORD dwLightData[size];

	nChannel = 0;
	((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("");
	memset(dwLightData, 0, size*sizeof(DWORD));
	for (i = 0; i<m_lstLdVoltage.GetItemCount(); i++)
	{
		ASSERT(i< m_nLightNum);
		if (m_lstLdVoltage.GetCheck(i))
		{
			DWORD temp = 0x01;
			dwLightData[i/32] |= temp<<(i%32);
			nChannel++;
		}
	}

	if (nChannel == 0)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("��ѡ��Ҫ�򿪵Ĺ�·��\n");
		return;
	}

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);
	bResult = m_pComPort->ld_set_light_on(dwLightData, atoi(str), FALSE);

	if (bResult == TRUE)
	{
		if (m_pComPort->m_protocolstatus.bConfigWay_ld_light_on == ld_comm_result_succeed)
		{
			((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("Success: �򿪹�·�ɹ�!\n");
		}
		else
		{
			ShowErrorInfo(m_pComPort->m_protocolstatus.bConfigWay_ld_light_on);
		}
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("Failed: ��������ʧ�ܣ�\n");
	}
}

void CLDBoardDlg::OnBnClickedButtonLpLightoff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BOOL bResult;
	int i,nChannel;
	CString str;
	const int size = ld_voltage_max/(8*sizeof(DWORD));
	DWORD dwLightData[size];

	nChannel = 0;
	((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("");
	memset(dwLightData, 0, ld_light_num*sizeof(BYTE));
	for (i = 0; i<m_lstLdVoltage.GetItemCount(); i++)
	{
		ASSERT(i< m_nLightNum);
		if (m_lstLdVoltage.GetCheck(i))
		{
			DWORD temp = 0x01;
			dwLightData[i/32] |= temp<<(i%32);
			nChannel++;
		}
	}

	if (nChannel == m_nLightNum)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("��ѡ��Ҫ�رյĹ�·��\n");
		return;
	}

	((CWnd*)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);
	bResult = m_pComPort->ld_set_light_off(dwLightData, atoi(str), FALSE);

	if (bResult == TRUE)
	{
		if (m_pComPort->m_protocolstatus.bConfigWay_ld_light_off == ld_comm_result_succeed)
		{
			((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("Success: �رչ�·�ɹ�!\n");
		}
		else
		{
			ShowErrorInfo(m_pComPort->m_protocolstatus.bConfigWay_ld_light_off);
		}
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("Failed: ��������ʧ�ܣ�\n");
	}
}

void CLDBoardDlg::OnBnClickedButtonLpLightonTime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BOOL bResult;
	int i,nChannel;
	CString str;
	const int size = ld_voltage_max/(8*sizeof(DWORD));
	DWORD dwLightData[size];
	DWORD dwContinueTime;

	nChannel = 0;
	memset(dwLightData, 0, ld_light_num*sizeof(BYTE));
	((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("");
	for (i = 0; i<m_lstLdVoltage.GetItemCount(); i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			nChannel++;
		}
	}

	if (nChannel == 0)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("��ѡ��Ҫ�򿪵Ĺ�·��\n");
		return;
	}

	((CWnd *)GetDlgItem(IDC_EDIT_LIGHTON_TIME))->GetWindowText(str);
	sscanf_s(str, "%d", &dwContinueTime);
	((CWnd *)GetDlgItem(IDC_EDIT_LD_SET_TIMEOUT))->GetWindowText(str);
	for (i =0; i<m_lstLdVoltage.GetItemCount(); i++)
	{
		if (m_lstLdVoltage.GetCheck(i))
		{
			bResult = m_pComPort->ld_set_light_on_time(i+1, dwContinueTime, atoi(str), FALSE);

			if (bResult == TRUE)
			{
				if (m_pComPort->m_protocolstatus.bConfigWay_ld_light_on_time == ld_comm_result_succeed)
				{
					str.Format("Success: �򿪵�%d·��ɹ�!\n", i+1);
					((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText(str);
				}
				else
				{
					ShowErrorInfo(m_pComPort->m_protocolstatus.bConfigWay_ld_light_on_time);
				}
			}
			else
			{
				((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText("Failed: �����ʧ�ܣ�\n");
			}
		}

	}

}

void CLDBoardDlg::ShowErrorInfo(BYTE bErrorCode)
{
	CString str = "";

	switch(bErrorCode)
	{
	case ld_comm_result_failed:
		str = "Fail ��λ���ּ�����ʱͨ��ʧ��";
		break;
	case ld_comm_result_offline:
		str = "Fail ��λ�������ѻ�״̬";
		break;
	case ld_comm_result_power_invalid:
		str = "Fail ���⹦�ʲ����";
		break;
	case ld_comm_result_other:
		str = "Fail ��������";
		break;
	default:
		str.Format("Fail δ�ܵ�ȷ����Ϣ���߷�����Ϣ ���յ�E4 (ErrorCode 0x%x)", bErrorCode);
		break;
	}
	((CWnd*)GetDlgItem(IDC_STATIC_LDBORD_FEEDBACK))->SetWindowText(str);
}
void CLDBoardDlg::EnableControl(void)
{
// #ifdef _CONFIG_CLIENT_
// 
// 	ENABLECONTROL(IDC_BUTTON_LP_LIGHTON, FALSE);
// 	ENABLECONTROL(IDC_BUTTON_LP_LIGHTOFF, FALSE);
// 	ENABLECONTROL(IDC_BUTTON_LP_LIGHTON_TIME, FALSE);
// 	ENABLECONTROL(IDC_EDIT_LIGHTON_TIME, FALSE);
// 
// 	ENABLECONTROL(IDC_BUTTON_LD_GET_LAST, FALSE);
// 
// 	ENABLECONTROL(IDC_BUTTON_LD_SET_LD, FALSE);
// 	ENABLECONTROL(IDC_EDIT_LD_SET_LD, FALSE);
// 
// 	ENABLECONTROL(IDC_BUTTON_EXPAND, FALSE);
// 	SHOWCONTROL(IDC_BUTTON_EXPAND, FALSE);
// 
// 	ENABLECONTROL(IDC_BUTTON_LD_CHECK_POWER_DELETE, FALSE);
// 	ENABLECONTROL(IDC_BUTTON_LD_CHECK_POWER_CLEARALL, FALSE);
// 	ENABLECONTROL(IDC_COMBO_LD_VOL_CHECK, FALSE);
// 	ENABLECONTROL(IDC_EDIT_LD_VOL_CHECK, FALSE);
// 	ENABLECONTROL(IDC_BUTTON_LD_SET_CHECK_POWER, FALSE);
// 	ENABLECONTROL(IDC_BUTTON_LD_CHECK_POWER_EXPORT, FALSE);
// #endif
}

void CLDBoardDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_pParentThread != NULL)
	{
		m_pParentThread->PostThreadMessage(WM_QUIT, 0, 0);
	}
	CDialog::PostNcDestroy();
	delete this;
}

void CLDBoardDlg::OnCbnSelchangeComboLdLasermodal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nLaserModal = m_nLaserModal;
	UpdateData(TRUE);
	if (nLaserModal==m_nLaserModal)
		return;

	ResetListForLightNum();
}
void CLDBoardDlg::ResetListForLightNum(void)
{
	m_nLightNum		= GetLaserNumber_Default(m_nLaserModal);

	int		i;
	CString str;

	m_cbLdVolCheck.ResetContent();
	for(i=0;i<m_nLightNum;i++)
	{
		str.Format("%d",i+1);
		m_cbLdVolCheck.AddString(str);
	}
	m_cbLdVolCheck.SetCurSel(0);

	m_lstLdVoltage.DeleteAllItems();
	for(i=0;i<m_nLightNum;i++)
	{
		m_lstLdVoltage.InsertItem(i,"");
		str.Format("%d",i+1);
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_ch,str);
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_pc,"0.000");
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_lastPD,"0.000");
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowPD,"-");
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_lastLD,"0.000");
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_nowLD,"-");
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_setpower,"0");
		m_lstLdVoltage.SetItemText(i, ldvoltagelist_power,"-");
	}

}
