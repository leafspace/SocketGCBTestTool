#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MBComPort/MBComPort.h"


enum SFBOARD_BOARDTYPE
{
	sf_boardtype_motor_x	= 0,	//单轴-X
	sf_boardtype_motor_y_dd,		//单轴-Y-导带
	sf_boardtype_motor_y_sg,		//单轴-Y-丝杆

	sf_boardtype_count
};
enum SFBOARD_BOARDTYPE_3AXIS
{
	sf_boardtype_motor_3axis_x		= 0,//三轴-X
	sf_boardtype_motor_3axis_y_dd,		//三轴-Y-导带
	sf_boardtype_motor_3axis_y_sg,		//三轴-Y-丝杆

	sf_boardtype_motor_3axis_z_dd,		//三轴-Z-导带
	sf_boardtype_motor_3axis_z_sg,		//三轴-Z-丝杆

	sf_boardtype_3axis_count
};

// SFBoardDlg 对话框

class CSFBoardDlg : public CDialog
{
	DECLARE_DYNAMIC(CSFBoardDlg)

public:
	CSFBoardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSFBoardDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SFBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbBoardType;
	virtual BOOL OnInitDialog();

	float* m_pfReducer_x;				//X减速机
	float* m_pfDollyPerimeter;			//小车周长
	float* m_pfPulsesPerRevolution_x;	//X每圈脉冲

	float* m_pfReducer_y;				//Y减速机
	float* m_pfPulsesPerRevolution_y;	//Y每圈脉冲
	float* m_pfDeferentPerimeter;		//Y导带周长
	float* m_pfDeferentPitch;			//Y丝杆螺距

	float* m_pfReducer_z;				//Z减速机
	float* m_pfPulsesPerRevolution_z;	//Z每圈脉冲
	float* m_pfDeferentPerimeter_z;		//Z导带周长
	float* m_pfDeferentPitch_z;			//Z丝杆螺距
	
	CWinThread *m_pParentThread;

	int MM2Pulse(float fMM);
	float Pulse2MM(int nPulse);

protected:
	virtual void PostNcDestroy();
public:
	CComboBox m_cbSpeedLevel;
	CCommunicationPort* m_pComPort;

	afx_msg void OnBnClickedButtonSfSpeedlevelAdd();
	afx_msg void OnBnClickedButtonSpeedlevelModify();

	BYTE m_bMotoWay;		//SF_MOTOR_WAY
	afx_msg void OnCbnSelchangeComboSfBoardtype();

	afx_msg void OnBnClickedButtonSfCountFullpulse();
	afx_msg void OnEnChangeEditSfMoverelativeMm();
	afx_msg void OnEnChangeEditSfMoverelativeReMm();
	afx_msg void OnEnChangeEditSfMoverelativePulse();
	afx_msg void OnEnChangeEditSfMoverelativeRePulse();
	afx_msg void OnBnClickedButtonSfMoverelative();
	afx_msg void OnEnChangeEditSfMoveabsoluteMm();
	afx_msg void OnEnChangeEditSfMoveabsolutePulse();
	afx_msg void OnBnClickedButtonSfMoveabsolute();
	afx_msg void OnBnClickedButtonSfGetposition();
	afx_msg void OnBnClickedButtonSfSpeedlevelAddOffline();
	afx_msg void OnBnClickedButtonSfMovestop();
	CComboBox m_cbOfflineMoveDir;
	CComboBox m_cbOfflineMoveSpd;
	afx_msg void OnBnClickedButtonSfOfflinemove();
	afx_msg void OnBnClickedButtonSfGetFullpulse();
	CComboBox m_cbSpecialPositionX;
	CComboBox m_cbSpecialPositionY;
	CComboBox m_cbSpecialPositionZ;
	afx_msg void OnBnClickedButtonSfSetSpecialPosition();
	afx_msg void OnEnChangeEditSfSpecialPositionMm();
	afx_msg void OnEnChangeEditSfSpecialPositionPulse();
	afx_msg void OnBnClickedButtonSfGetSpecialPosition();
	afx_msg void OnBnClickedButtonSfMove2SpecialPosition();
	afx_msg void OnBnClickedButtonSfDeleteTimetableData();
	afx_msg void OnBnClickedButtonSfGetTimetableVitalParameter();
	CButton m_ckQueryCom;

	BOOL ExportBoardInfo();
	BOOL ImportBoardInfo();
	afx_msg void OnBnClickedButtonBoardinfoExport();
	afx_msg void OnBnClickedButtonBoardinfoImport();

	BOOL m_bThreeAxisServo;	//三轴伺服板

	int BoardType2MotorWay(int nBoardType);
	BYTE SpecialPositionFromPosType();

	//sf_xy
	CButton m_xy_ckMotorY_SG;
	CComboBox m_xy_cbSpecialPositionX;
	CComboBox m_xy_cbSpecialPositionY;
	BOOL	xy_SpecialPositionFromPosType(BYTE &bPosX, BYTE &bPosY);
	//	bMotoWayy :[in]SFBOARD_BOARDTYPE_3AXIS
	int		xy_MM2Pulse(float fMM, BYTE bMotoWay);
	float	xy_Pulse2MM(int nPulse, BYTE bMotoWay);

	afx_msg void OnBnClickedButtonSfXyMove2SpecialPosition();
	afx_msg void OnBnClickedButtonSfXyMoverelative();
	afx_msg void OnBnClickedButtonSfXyMoveabsolute();
	afx_msg void OnBnClickedButtonSfXyPrintline();
	afx_msg void OnEnChangeEditSfXyMoveXMm();
	afx_msg void OnEnChangeEditSfXyMoveXPulse();
	afx_msg void OnEnChangeEditSfXyMovYMm();
	afx_msg void OnEnChangeEditSfXyMoveYPulse();
	afx_msg void OnEnChangeEditSfXyMoveReXMm();
	afx_msg void OnEnChangeEditSfXyMoveReXPulse();
	afx_msg void OnBnClickedOk();

	bool bIsNewCurve;
protected:
	virtual void OnCancel();
};
