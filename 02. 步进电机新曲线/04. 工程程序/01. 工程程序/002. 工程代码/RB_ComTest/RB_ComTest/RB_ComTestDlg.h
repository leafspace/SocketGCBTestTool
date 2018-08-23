
// RB_ComTestDlg.h : 头文件
//

#pragma once
#include "MBComPort/MBComPort.h"
#include "afxwin.h"
#include "SFBoardThread.h"
#include "afxcmn.h"
#include "RAS/RasBoardThread.h"
#include "FlatLaser/LDBoardThread.h"

enum FUNCTION_TYPE
{
	functype_servo_4005 = 0,		//伺服板 V4005
	functype_servo_3axis_1001,		//三轴伺服板 V1001
	functype_servo_3axis_1001_new,	//三轴伺服板 V1001 (新曲线)
	functype_raser,					//光栅板 - 数码

	functype_raser_flatlaser,		//光栅板 - 平网蓝光

	functype_servo_param,			//伺服固件参数
	functype_servo_3axis_1001_param,//三轴伺服板固件参数 V1001

	functype_count
};


// CRB_ComTestDlg 对话框
class CRB_ComTestDlg : public CDialog
{
// 构造
public:
	CRB_ComTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RB_COMTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CCommunicationPort m_port;	//串口
	int m_nComNum;					//串口号
	BOOL m_bConnect;				//是否连接

	afx_msg void OnBnClickedButtonOpencom();
	CComboBox m_cbComNum;
	CEdit m_edtComRecive;
	CEdit m_edtComSend;
	
	float m_fReducer_x;				//X减速机
	float m_fDollyPerimeter;		//小车周长
	float m_fPulsesPerRevolution_x;	//X每圈脉冲

	float m_fReducer_y;				//Y减速机
	float m_fPulsesPerRevolution_y;	//Y每圈脉冲
	float m_fDeferentPerimeter;		//Y导带周长
	float m_fDeferentPitch;			//Y丝杆螺距

	float m_fReducer_z;				//Z减速机
	float m_fPulsesPerRevolution_z;	//Z每圈脉冲
	float m_fDeferentPerimeter_z;	//Z导带周长
	float m_fDeferentPitch_z;		//Z丝杆螺距

protected:
	virtual void PostNcDestroy();
public:
	CComboBox m_cbDlgType;
	afx_msg void OnBnClickedButtonOpendlg();

	afx_msg void OnBnClickedButtonClearEdit();

	afx_msg LONG OnComRecive(WPARAM wPALAM, LPARAM lPALAM);
	afx_msg LONG OnComSend(WPARAM wPALAM, LPARAM lPALAM);

	afx_msg void OnBnClickedOk();

	void GetConfigInfo(void);
	void SetConfigInfo(void);
	int m_nPortType;
	DWORD m_dwIP;
	int m_nCom;
	CButton m_openportCtrl;
	void EnableControl(void);
	CStatic m_infoCtrl;

public:
	CWnd *m_pDlgList[functype_count];
	virtual BOOL DestroyWindow();

};
