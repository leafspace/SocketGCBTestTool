///////////////////////////////////////////////////////////////////////////
// rpMacro.h : export's headerfile of rpBase.dll 
//
//	提供一些常用宏定义
//
// Created by hzm 2012-02-09
//
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "afxstatusbar.h"
#include "afxdialogex.h"

#ifdef RP_EXT_CLASS
#undef RP_EXT_CLASS
#undef RP_EXT_API
#undef RP_EXT_DATA
#endif

#ifdef APP_RPBASE
#define RP_EXT_CLASS       AFX_CLASS_EXPORT
#define RP_EXT_API         AFX_API_EXPORT
#define RP_EXT_DATA        AFX_DATA_EXPORT
#else
#define RP_EXT_CLASS       AFX_CLASS_IMPORT
#define RP_EXT_API         AFX_API_IMPORT
#define RP_EXT_DATA        AFX_DATA_IMPORT
#endif

//预定义数据类型
typedef	double			Float64;
typedef	float			Float32;
typedef	short			Int16;
typedef	long			Int32;
typedef	__int64			Int64;
typedef	ULONGLONG		DDWORD;
typedef DDWORD*			LPDDWORD;

typedef CArray<RGBQUAD, RGBQUAD>	CRGBArray;
typedef CArray<POINT, POINT>		CPOINTArray;
typedef CArray<BOOL, BOOL>			CBOOLArray;
typedef CArray<RECT, RECT>			CRECTArray;
typedef CArray<int, int>			CintArray;
typedef CArray<double, double>		CDoubleArray;
typedef CArray<LONGLONG, LONGLONG>		CLONGLONGArray;
typedef CArray<ULONGLONG, ULONGLONG>	CULONGLONGArray;
typedef CDWordArray					CDWORDArray;
typedef CDWordArray					CClrArray;

const DWORD		MAXCHAIN = 0xFFFF;				//点串最大个数
const Float64	PI = 3.14159265358979323846;
const Float64	DOUBLEPRECISION = 0.0000000001;			//浮点小数位精度，一般仅用于比较
const Float64	DOUBLEZERO = 0.0000000001;			//浮点0的最大值，一般仅用于比较
const Float32	ANGLEPRECISION = 0.005f;				//角度值精度，弧度单位

const Float64	TWOPI = 2 * PI;
const Float64	HALFPI = 0.5*PI;
const Float64	HALF3PI = 1.5*PI;

const COLORREF	NULLCOLOR = CLR_INVALID;			//无效颜色值
const int		NULLINDEX = 0x7FFFFFFF;			//无效索引值（相对于索引稿，<= 8Bits）

const int 		MAXSCROLLSIZE = INT_MAX / 2;			//最大滚动条计数值

//图像精度相关
const FLOAT		MINRESOLUTION = 0.005f; 				//最小精度值，dpmm
const FLOAT		MAXRESOLUTION = 1000.0f; 				//最大精度值，dpmm
const int 		MINDPI = 1;					//最小精度值，dpi
const int 		MAXDPI = 20000;				//最大精度值，dpi


//文件格式标识符
//{RainBow 支持的文件格式
const WORD AN_VERSION = 0x400;
const WORD WN_BITMAP = 0x4D42; // Must be 'BM'
const WORD AN_BMPVER = 0x40;
const WORD AN_APF = 0x4D52; // 'RM'
const WORD AN_BITMAP = 0x4D41; // 'AM'
const WORD AN_MACTIFF = 0x4D4D; // 'MM'
const WORD AN_TIFF = 0x4949; // 'II'
const WORD AN_JPEG = 0x4A50; // 'JP'
const WORD AN_TIFFVER = 0x40;
const WORD AN_PCX = 0x0A;
const WORD AN_AIF = 0x4941; // 'AI'
const WORD AN_LAYER = 0x4C41; // 'AL'
const WORD AN_FLOWER = 0x4641; // 'AF'
const WORD AN_IMAGE = 0x4D49; // 'IM'
const WORD AN_GRAPH = 0x4741; // 'AG'
const WORD AN_GRFVER = 0x40;
const WORD AN_GRFNORMAL = 0x4E47; // 'GN'
const WORD AN_GRFGDELINE = 0x4C47; // 'GL'
const WORD AN_COMPOUND = 0x4443; // 'CD'
const WORD AN_CPDVER = 0x40;
const WORD AN_STANDARD = 0x4E00; // added by Cdq : all other standard picture: JPG,GIF,PNG ...
const WORD AN_PRIVATE = 0x5250; // 'PR'
const WORD AN_UNKNOWNFILE = 0x5858; // 'XX'

const WORD AN_RPF = 0x5052; // 'RP'，RainBow RIP 软件输出文件格式
const WORD RPF_IMAGE = 0x4952; // 'RI'，RainBow RIP RPF文件中的Image图像数据
const WORD RPF_LAYER = 0x4C52; // 'RL'，RainBow RIP RPF文件中的Layer图像数据
const WORD RPF_FLOWER = 0x4652; // 'RF'，RainBow RIP RPF文件中的Flower图像数据
const WORD RPF_TEMPLET = 0x4752; // 'RT', RainBow RIP RPF文件中的Templet图像数据
const WORD RPF_LAYOUT = 0x4F52; // 'RO'，RainBow RIP RPF文件中的Layout图像数据

const WORD CM_BITMAP = 0x4D43; // 'CM'，RainBow RIP 输出的BMP/TIF文件私有信息标识

const WORD AN_SEP = 0xFFFF; //仅用于系统内部区分，并不是图像文件标识符
//}RainBow 支持的文件格式


//离散方式{一般，好，精细}
enum SEPWAY { swRough = 0, swFine, swSmooth };
//曲线类型{点串，贝塞尔，B样条，跟踪}
enum CURVE { cvPolyline = 0, cvBezier, cvBSpline, cvTrace };
//压缩方式
//	暂只使用了 cpsRGB, cpsPrivate, cpsLZW(TIFF), cpsPackBits(TIFF,1Bits)
enum ACompressMode
{
	cpsRGB = BI_RGB,
	cpsRLE8 = BI_RLE8,
	cpsRLE4 = BI_RLE4,
	cpsBITFIELDS = BI_BITFIELDS,
	cpsJPEG = 10,
	cpsLZW = 11,
	cpsPrivate = 12,
	cpsPackBits = 13,
	cpsZIP = 14,
	cpsADOBEZIP = 15
};
//图像类型{彩色，灰度，色调，真彩，无效}
enum AImageType { imgClrType = 1, imgGrayType, imgClrGrayType, imgRgbType, imgStrangeType };
//精度单位: 像素/英寸,像素/丝米,像素/毫米,像素/厘米,像素/米
enum AResUnit { ruDefault = 0, ruDPI = 1, ruDPCMM = 2, ruDPMM, ruDPCM, ruDPM };
//尺寸单位 像素,丝米,毫米,厘米,米,英寸
enum ASizeUnit { suDefault = 0, suPIXEL = 1, suCMM = 2, suMM = 3, suCM, suM, suINCH };
//区域填充方式（同SDK定义）
enum AFillMode { fmAlternate = ALTERNATE, fmWinding = WINDING };
//花样类型{图像，图形}
//enum APictureType {picImage=0x01, picGraph=0x10};
//图形类型{普通，辅助线}
//enum AGraphType {grfNormal=1, grfGdeline};
//旋转方向{逆时针，顺时针}
enum CLOCKWISE { counterwise = AD_COUNTERCLOCKWISE, clockwise = AD_CLOCKWISE };
// 混合方式{普通，颜料混合，光混合}
enum ATransferMode { tmNormal = 0, tmDyeMix = 1, tmPhoMix, tmDefault };
// 共享标识，用于设定其它同步线程/进程的操作权限（拒绝操作类型）
//	无限制，不可删除，不可写，不可读，独占
enum AShareFlag { sfDenyNone = 0, sfDenyDelete, sfDenyWrite, sfDenyRead, sfExclusive };

//更改大小的基点
enum SCALEPOSITION { posNULL = 0, posTopLeft, posBottomLeft, posTopRight, posBottomRight, posLeft, posRight, posTop, posBottom };

//初始化指定结构体/数组
#ifndef BZERO
#define BZERO(st)	(memset(&(st), 0, sizeof(st)))
#endif

//浮点数四舍五入取整
#define ABS(x)		((x) > 0 ? int((x)+0.5) : int((x)-0.5))

#define LOSHORT(l)	((SHORT)(l))
#define HISHORT(l)	((SHORT)(((LONG)(l)>>16) & 0xffff))

//比较两个POINT变量是否相等
#define EQUPOINT(pt1,pt2)	((((pt1).x) == ((pt2).x)) && (((pt1).y) == ((pt2).y)))
//确认rc1是否在rc2中
#define RECTINRECT(rc1,rc2)	(((rc1).left>=(rc2).left)			\
							&& ((rc1).right<=(rc2).right)		\
							&& ((rc1).top>=(rc2).top)			\
							&& ((rc1).bottom<=(rc2).bottom))

//32位对齐
#define ADDTO32(i)			(((DWORD)i+31)/32*4)
//RIP缓冲区读写操作时每个像素对应的字节数				
#define BAND(BitCount)		((BitCount) > 8 ? BYTE(((BitCount)+7)>>3) : 1)
//RGB转GRAY
#define RGBTOGRAY(r,g,b)	((299*(r)+587*(g)+114*(b))/1000)

//四舍五入
#define ROUNDINT(x)			((x) > 0 ? Int32((x)+0.5) : Int32((x)-0.5))

//比较两个浮点数是否相等
#define _PRI_EQUDOUBLE(a,b, zero) (((Float64(a)-Float64(b)) < (zero))	\
								&& ((Float64(a)-Float64(b)) > -(zero)))
#define EQUDOUBLE(a,b)	_PRI_EQUDOUBLE(a,b,DOUBLEPRECISION)
#define EQUZERO(a)		_PRI_EQUDOUBLE(a, 0.0, DOUBLEPRECISION)

//角度相关
//将角度值（弧度）限定到TWOPI内
#define BOUNDANGLE(dAngle)\
	if(dAngle<0 || dAngle>=TWOPI)	dAngle -= floor(dAngle/TWOPI)*TWOPI
//将角度值（角度）限定到360度内
#define BOUNDANGLE360(dAngle)\
	if(dAngle<0 || dAngle>=360.0)	dAngle -= floor(dAngle/360.0)*360.0
//将角度值（弧度）限定到 PI 内
#define BOUNDPIANGLE(dAngle)\
	if(dAngle<0 || dAngle>=PI)		dAngle -= floor(dAngle/PI)*PI
//将角度值（角度）限定到180度内
#define BOUNDPIANGLE180(dAngle)\
	if(dAngle<0 || dAngle>=180.0)	dAngle -= floor(dAngle/180.0)*180.0

//数值交换定义
#define SwapDouble(a,b) {Float64 c; c=(a); (a)=(b); (b)=c;}
#define SwapInt(a,b)	{Int32 c; c=(a); (a)=(b); (b)=c;}

#define COMMONPRECISION		3		//小数点后尾数
#define	ZOOMSCALE			LONGLONG(m_ZoomScale >> 16) / int(WORD(m_ZoomScale))
#define	REZOOMSCALE			LONGLONG(WORD(m_ZoomScale)) / int(m_ZoomScale >> 16)

#define	XZOOMSCALE			LONGLONG(m_XZoomScale >> 16) / int(WORD(m_XZoomScale))
#define	YZOOMSCALE			LONGLONG(m_YZoomScale >> 16) / int(WORD(m_YZoomScale))

#define	XOUTSCALE			LONGLONG(m_XOutScale >> 16) / int(WORD(m_XOutScale))
#define	XREOUTSCALE			LONGLONG(WORD(m_XOutScale)) / int(m_XOutScale >> 16)
#define	YOUTSCALE			LONGLONG(m_YOutScale >> 16) / int(WORD(m_YOutScale))
#define	YREOUTSCALE			LONGLONG(WORD(m_YOutScale)) / int(m_YOutScale >> 16)

//缩放比较计算
#define	PAINTZOOMSCALE(dwZoomScale)			LONGLONG((dwZoomScale) >> 16) / int(WORD(dwZoomScale))
#define	REPAINTZOOMSCALE(dwZoomScale)		LONGLONG(WORD(dwZoomScale)) / int((dwZoomScale) >> 16)

#define INDEXCOLOR(clrRef)	(((clrRef) & 0xFF000000) == 0x01000000)

#define vfScale(ZoomScale)	(double(max(1.0, HIWORD(ZoomScale)) / max (1.0, LOWORD(ZoomScale))))
#define fvScale(ZoomScale)	(double(max(1.0, LOWORD(ZoomScale)) / max (1.0, HIWORD(ZoomScale))))

#define DECIMAL_TIMES			10000.0
#define INTEGER_TO_DWORD(a)		(::AnxFloatToInt((a),FALSE))
#define DECIMAL_TO_DWORD(a)		(::AnxFloatToInt(((a)-INTEGER_TO_DWORD(a))*DECIMAL_TIMES,FALSE))
#define DWORD_TO_DECIMAL(a)		(((a)/DECIMAL_TIMES)-INTEGER_TO_DWORD((a)/DECIMAL_TIMES))

#define DECIMAL_TO_DWORD_TIMES(a,dTimes)		(::AnxFloatToInt(((a)-INTEGER_TO_DWORD(a))*double(dTimes),FALSE))
#define DWORD_TO_DECIMAL_TIMES(a,dTimes)		(((a)/double(dTimes))-INTEGER_TO_DWORD((a)/double(dTimes)))

#ifndef MAKEDWORD
#define MAKEDWORD(a, b)			((DWORD)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#endif
#ifndef LODWORD
#define LODWORD(ul)				((DWORD)(((ULONGLONG)(ul)) & 0xffffffff))
#define HIDWORD(ul)				((DWORD)((((ULONGLONG)(ul)) >> 32) & 0xffffffff))
#endif
#ifndef MAKEULONGLONG
#define MAKEULONGLONG(a, b)		((ULONGLONG)(((DWORD)(((ULONGLONG)(a)) & 0xffffffff)) | ((ULONGLONG)((DWORD)(((ULONGLONG)(b)) & 0xffffffff))) << 32))
#endif

//控件使能（必须在对话框类中使用）
#define ENABLECONTROL(nID,bEnable)	\
{	\
	CWnd *pWnd = GetDlgItem(nID);			\
	ASSERT(pWnd != NULL);					\
	ASSERT(::IsWindow(pWnd->m_hWnd));		\
	pWnd->EnableWindow(bEnable);			\
}
#define SHOWCONTROL(uID,bShow)	\
{\
	CWnd *pWnd;	\
	pWnd = (uID) ? GetDlgItem(uID) : NULL;		\
	if (pWnd) pWnd->ShowWindow((bShow)?SW_SHOW:SW_HIDE);	\
}
//数组拷贝，a:dest, b:src
#define memcpy_array(a,b)	memcpy((a),(b),min(sizeof(a),sizeof(b)))


//获取常用按键状态
#define Ctrl_State			((GetKeyState(VK_CONTROL) & 0x8000)? TRUE : FALSE)
#define Shift_State			((GetKeyState(VK_SHIFT) & 0x8000) ? TRUE : FALSE)
#define Alt_State			((GetKeyState(VK_MENU) & 0x8000) ? TRUE : FALSE)
#define NumLock_State		((GetKeyState(VK_NUMLOCK) & 0x0001) ? TRUE : FALSE)
#define ScrollLock_State	((GetKeyState(VK_SCROLL) & 0x0001) ? TRUE : FALSE)
#define Key_State(nChar)	((GetKeyState(nChar) & 0x8000) ? TRUE : FALSE)
#define Key_State_none		(!(Ctrl_State|| Shift_State|| Alt_State))
#define Key_State_OnlyCtrl	(Ctrl_State && !Shift_State && !Alt_State)
#define Key_State_OnlyAlt	(!Ctrl_State && !Shift_State && Alt_State)
#define Key_State_OnlyShift (!Ctrl_State && Shift_State && !Alt_State)
#define Key_State_CtrlAlt	(Ctrl_State && !Shift_State && Alt_State)
#define Key_State_CtrlShift (Ctrl_State && Shift_State && !Alt_State)
#define Key_State_AltShift	(!Ctrl_State && Shift_State && Alt_State)

#define KEY_STATE_GETALL(nFlags)	\
			BOOL t_IsCtrlKeyDown,t_IsShiftKeyDown,t_IsAltKeyDown;	\
			t_IsCtrlKeyDown = Ctrl_State;		\
			t_IsShiftKeyDown = Shift_State;		\
			t_IsAltKeyDown = (nFlags==0 ? Alt_State : ((nFlags)&KF_ALTDOWN))
#define KEY_STATE_GETCTRLANDSHIFT(nFlags)		\
			BOOL t_IsCtrlKeyDown,t_IsShiftKeyDown;	\
			t_IsCtrlKeyDown = Ctrl_State;			\
			t_IsShiftKeyDown = Shift_State;
#define KEY_STATE_GETCTRLANDALT(nFlags)	\
			BOOL t_IsCtrlKeyDown,t_IsAltKeyDown;	\
			t_IsCtrlKeyDown = Ctrl_State;		\
			t_IsAltKeyDown = (nFlags==0 ? Alt_State : ((nFlags)&KF_ALTDOWN))
#define KEY_STATE_GETSHIFTANDALT(nFlags)	\
			BOOL t_IsShiftKeyDown,t_IsAltKeyDown;	\
			t_IsShiftKeyDown = Shift_State;	\
			t_IsAltKeyDown = (nFlags==0 ? Alt_State : ((nFlags)&KF_ALTDOWN))

#define NOONEDOWN	(!(t_IsCtrlKeyDown || t_IsShiftKeyDown || t_IsAltKeyDown))
#define ONLYCTRL	(t_IsCtrlKeyDown && !t_IsShiftKeyDown && !t_IsAltKeyDown)
#define ONLYALT		(!t_IsCtrlKeyDown && !t_IsShiftKeyDown && t_IsAltKeyDown)
#define ONLYSHIFT	(!t_IsCtrlKeyDown && t_IsShiftKeyDown && !t_IsAltKeyDown)
#define CTRLALT		(t_IsCtrlKeyDown && !t_IsShiftKeyDown && t_IsAltKeyDown)
#define CTRLSHIFT	(t_IsCtrlKeyDown && t_IsShiftKeyDown && !t_IsAltKeyDown)
#define ALTSHIFT	(!t_IsCtrlKeyDown && t_IsShiftKeyDown && t_IsAltKeyDown)

//结构（类）变量参数拷贝
#define STRUCT_SETVALUE_PDST(pdst,src,value)	\
{	\
	(pdst)->value = (src).value;	\
}
#define STRUCT_SETVALUE_ARRAY_PDST(pdst,src,value)	\
{	\
	memcpy((pdst)->value, (src).value, sizeof((pdst)->value));		\
}
#define STRUCT_SETVALUE(dst,src,value)	\
{	\
	(dst)->value = (src)->value;	\
}
#define STRUCT_SETVALUE_ARRAY(dst,src,value)	\
{	\
	memcpy((dst)->value, (src)->value, sizeof((dst)->value));		\
}

//从配置文件中读取指定段内的以指定变量名(value)为标识的变量参数值
//调用前需定义好 strSection, szReg/strValue, strIniFile
//strSection :
//	指定操作段名，例[CURRENT]
//szReg/strValue	:
//	指定操作缓冲区，例 char szReg[200]; CString strValue;
//strIniFile:
//	指定操作的配置文件名
//value		:
//	变量名，必须为有效的变量标识符，不能使用立即数
//type		:
//	定义所操作变量值的数值类型，一般用于整型数操作时, BYTE, int, short, long ...
//fmt		:
//	定义操作的格式化字符串，例 "%d", "%f"等
#define GET_PROFILE_INFO_STRING(value)	\
{	\
	szReg[0] = '\0';	\
	(value) = szReg;	\
	if (GetPrivateProfileString (strSection, #value, value, szReg, _countof(szReg), strIniFile))	\
	{	\
		if (szReg[0] != '\0')	\
			(value) = szReg;		\
	}	\
}
#define GET_PROFILE_INFO_STRINGEX(value)	\
{	\
    szReg[0] = '\0';	\
    if (GetPrivateProfileString (strSection, #value, value, szReg, _countof(szReg), strIniFile))	\
    {	\
        if (szReg[0] != '\0')	\
            strcpy_s(value, szReg);	\
    }	\
}
#define GET_PROFILE_INFO_DOUBLE(value,minvalue,defvalue,maxvalue)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
	(value) = (defvalue);		\
	szReg[0] = '\0';	\
	if (GetPrivateProfileString (strSection, #value, NULL, szReg, _countof(szReg), strIniFile))	\
	{	\
		if (szReg[0] != '\0')	\
			(value) = atof(szReg);	\
	}	\
	if ((value) < (minvalue))	\
		(value)	= (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
	else if ((value) > (maxvalue)) \
		(value) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
}
#define GET_PROFILE_INFO_FLOAT(value,minvalue,defvalue,maxvalue)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
	(value) = (defvalue);		\
	szReg[0] = '\0';	\
	if (GetPrivateProfileString (strSection, #value, NULL, szReg, _countof(szReg), strIniFile))	\
	{	\
		if (szReg[0] != '\0')	\
			(value) = (float)atof(szReg);	\
	}	\
	if ((value) < (minvalue))	\
		(value)	= (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
	else if ((value) > (maxvalue)) \
		(value) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
}
#define GET_PROFILE_INFO_INT(value,minvalue,defvalue,maxvalue,type)		\
{	\
	ASSERT(minvalue <= maxvalue);	\
	long	lTmp = 0;		\
	lTmp = (long)GetPrivateProfileInt (strSection, #value, INT(value), strIniFile);		\
	if (lTmp < (minvalue))	\
		(value)	= (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
	else if (lTmp > (maxvalue)) \
		(value) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
	else	\
		(value) = (type)lTmp;	\
}
#define GET_PROFILE_INFO_UINT(value,minvalue,defvalue,maxvalue)		\
{	\
	ASSERT(minvalue <= maxvalue);	\
    unsigned long	lTmp = 0;		\
    lTmp = (unsigned long)GetPrivateProfileInt (strSection, #value, INT(value), strIniFile);		\
	if (lTmp < (minvalue))	\
		(value)	= (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
	else if (lTmp > (maxvalue)) \
		(value) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
    else	\
        (value) = lTmp;	\
}
#define WRITE_PROFILE_INFO(fmt,value)	\
{	\
	strReg.Format((fmt), (value));		\
	WritePrivateProfileString (strSection, #value, strReg, strIniFile);	\
}

#define GET_PROFILE_INFO_ARRAY_INT(value,nsize,minvalue,defvalue,maxvalue,type)	\
{	\
    CString strEntry;   \
	long	lTmp = 0;		\
	ASSERT(minvalue <= maxvalue);	\
    for(int i=0; i<(nsize);i++)   \
    {   \
        strEntry.Format(#value"[%d]", i);  \
        lTmp	= (long)GetPrivateProfileInt (strSection, strEntry, (value[i]), strIniFile);		\
		if (lTmp < (minvalue))	\
			(value[i])	= (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
		else if (lTmp > (maxvalue)) \
			(value[i]) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
		else	\
			(value[i]) = (type)lTmp;	\
    }   \
}
#define GET_PROFILE_INFO_ARRAY_UINT(value,nsize,minvalue,defvalue,maxvalue)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
    CString strEntry;   \
    unsigned long	lTmp = 0;		\
    for(int i=0; i<(nsize);i++)   \
    {   \
		strEntry.Format(#value"[%d]", i);  \
		lTmp	= (unsigned long)GetPrivateProfileInt (strSection, strEntry, (value[i]), strIniFile);		\
		if (lTmp < (minvalue))	\
			(value[i]) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
		else if (lTmp > (maxvalue)) \
			(value[i]) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
		else	\
			(value[i]) = lTmp;	\
    }   \
}
#define GET_PROFILE_INFO_ARRAY_DOUBLE(value,nsize,minvalue,defvalue,maxvalue)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
    CString strEntry;   \
    for(int i=0; i<(nsize);i++)   \
    {   \
        szReg[0] = '\0';	\
		(value[i]) = (defvalue);		\
        strEntry.Format(#value"[%d]", i);  \
        if (GetPrivateProfileString (strSection, strEntry, NULL, szReg, _countof(szReg), strIniFile))	\
        {	\
            if (szReg[0] != '\0')	\
                (value[i]) = atof(szReg);	\
        }	\
		if ((value[i]) < (minvalue))	\
			(value[i]) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
		else if ((value[i]) > (maxvalue)) \
			(value[i]) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
    }   \
}
#define GET_PROFILE_INFO_ARRAY_CSTRING(value,nsize,defvalue)	\
{	\
    CString strEntry;   \
    for(int i=0; i<(nsize);i++)   \
    {   \
		szReg[0] = '\0';	\
		(value[i]) = (defvalue);		\
		strEntry.Format(#value"[%d]", i);  \
		if (GetPrivateProfileString (strSection, strEntry, NULL, szReg, _countof(szReg), strIniFile))	\
		{	\
			if (szReg[0] != '\0')	\
				(value[i]) = szReg;	\
		}	\
		if (strlen(value[i]) <= 0)	\
			(value[i])	= (defvalue);	\
    }   \
}
#define GET_PROFILE_INFO_ARRAY_STRINGEX(value,nsize,defvalue)	\
{	\
    CString strEntry;   \
    for(int i=0; i<(nsize);i++)   \
    {   \
        szReg[0] = '\0';	\
		strcpy(value[i], (defvalue));	\
        strEntry.Format(#value"[%d]", i);  \
        if (GetPrivateProfileString (strSection, strEntry, NULL, szReg, _countof(szReg), strIniFile))	\
        {	\
            if (szReg[0] != '\0')	\
                strcpy(value[i], szReg);	\
        }	\
        if (strlen(value[i]) <= 0)	\
            strcpy(value[i], (defvalue));	\
    }   \
}
#define GET_PROFILE_INFO_ARRAYEX_INT(value,nsize1,nsize2,minvalue,defvalue,maxvalue,type)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
    CString strEntry;   \
    long	lTmp = 0;		\
    for(int i=0; i<(nsize1);i++)   \
    {   \
        for(int j=0; j<(nsize2);j++)    \
        {\
            strEntry.Format(#value"[%d][%d]", i,j);  \
            lTmp	= (long)GetPrivateProfileInt (strSection, strEntry, (value[i][j]), strIniFile);		\
			if (lTmp < (minvalue))	\
				(value[i][j]) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
			else if (lTmp > (maxvalue)) \
				(value[i][j]) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
            else	\
                (value[i][j]) = (type)lTmp;	\
        }\
    }   \
}
#define GET_PROFILE_INFO_ARRAYEX_UINT(value,nsize1,nsize2,minvalue,defvalue,maxvalue,type)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
    CString strEntry;   \
    unsigned long	lTmp = 0;		\
    for(int i=0; i<int(nsize1);i++)   \
    {   \
        for(int j=0; j<int(nsize2);j++)    \
        {\
            strEntry.Format(#value"[%d][%d]", i,j);  \
            lTmp	= (unsigned long)GetPrivateProfileInt (strSection, strEntry, (value[i][j]), strIniFile);		\
			if (lTmp < (minvalue))	\
				(value[i][j]) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
			else if (lTmp > (maxvalue)) \
				(value[i][j]) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
            else	\
                (value[i][j]) = (type)lTmp;	\
        }\
    }   \
}
#define GET_PROFILE_INFO_ARRAYEX_FLOAT(value,nsize1,nsize2,minvalue,defvalue,maxvalue)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
    CString strEntry;   \
    for(int i=0; i<int(nsize1);i++)   \
    {   \
        for(int j=0; j<int(nsize2);j++)    \
        {\
            szReg[0] = '\0';	\
			(value[i][j]) = (defvalue);		\
            strEntry.Format(#value"[%d][%d]", i, j);  \
            if (GetPrivateProfileString (strSection, strEntry, NULL, szReg, _countof(szReg), strIniFile))	\
            {	\
                if (szReg[0] != '\0')	\
                    (value[i][j]) = atof(szReg);	\
            }	\
			if (lTmp < (minvalue))	\
				(value[i][j]) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
			else if (lTmp > (maxvalue)) \
				(value[i][j]) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
        }   \
    }   \
}
#define WRITE_PROFILE_INFO_ARRAY(fmt,value,nsize)	\
{	\
    CString strEntry;   \
    for(int i=0; i<int(nsize);i++)   \
    {   \
        strEntry.Format(#value"[%d]", i);      \
        strReg.Format((fmt), (value[i]));		\
        WritePrivateProfileString (strSection, strEntry, strReg, strIniFile);	\
    }   \
}
#define WRITE_PROFILE_INFO_ARRAYEX(fmt,value,nsize1,nsize2)	\
{	\
    CString strEntry;   \
    for(int i=0; i<int(nsize1);i++)   \
    {   \
        for(int j=0; j<int(nsize2);j++)    \
        {\
            strEntry.Format(#value"[%d][%d]", i, j);      \
            strReg.Format((fmt), (value[i][j]));		\
            WritePrivateProfileString (strSection, strEntry, strReg, strIniFile);	\
        }   \
    }   \
}
#define WRITE_PROFILE_INFO_ARRAY_CSTRING(value,nsize)	\
{	\
    CString strEntry;   \
    for(int i=0; i<int(nsize);i++)   \
    {   \
		strEntry.Format(#value"[%d]", i);      \
		strReg  = (value[i]);		\
		WritePrivateProfileString (strSection, strEntry, strReg, strIniFile);	\
    }   \
}

//从注册表中读取指定段内的以指定变量名(value)为标识的变量参数值
//调用前需定义好 pApp, strSection, strValue
//strSection :
//	指定操作段名，例[CURRENT]
//strValue	:
//	指定操作缓冲区，例 CString strValue;
//value		:
//	变量名，必须为有效的变量标识符，不能使用立即数
//type		:
//	定义所操作变量值的数值类型，一般用于整型数操作时, BYTE, int, short, long ...
//fmt		:
//	定义操作的格式化字符串，例 "%d", "%f"等

#define GET_REGISTER_INFO_STRING(value)	{(value) = pApp->GetProfileString(strSection, #value, value);}
#define GET_REGISTER_INFO_FLOAT(value,minvalue,defvalue,maxvalue)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
	(value) = (defvalue);	\
	strValue = pApp->GetProfileString(strSection,#value, NULL);	\
	if (!strValue.IsEmpty())	\
	{	\
		(value) = atof(strValue);	\
	}	\
	if ((value) < (minvalue))	\
		(value) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
	else if ((value) > (maxvalue)) \
		(value) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
}
//type : 所需数据类型, BYTE, int, short, long ...
#define GET_REGISTER_INFO_INT(value,minvalue,defvalue,maxvalue,type)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
	long	lTmp = (long)(defvalue);		\
	strValue = pApp->GetProfileString(strSection,#value, NULL);	\
	if (!strValue.IsEmpty())	\
	{	\
		lTmp = atol(strValue);		\
	}	\
	if (lTmp < (minvalue))	\
		(value) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
	else if (lTmp > (maxvalue)) \
		(value) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
	else						\
		(value) = (type)lTmp;	\
}
#define WRITE_REGISTER_INFO_STRING(value)	{pApp->WriteProfileString (strSection, #value, (value));}
#define WRITE_REGISTER_INFO(fmt,value)	\
{	\
	strValue.Format((fmt), (value));		\
	pApp->WriteProfileString (strSection,#value, strValue);	\
}

#define GET_REGISTER_INFO_ARRAY_INT(value,nsize,minvalue,defvalue,maxvalue,type)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
    CString strEntry;   \
	long	lTmp = 0;		\
    for(int i=0; i<(nsize);i++)   \
    {   \
		lTmp = (long)(defvalue);		\
        strEntry.Format(#value"[%d]", i);  \
		strValue = pApp->GetProfileString(strSection,strEntry, NULL);	\
		if (!strValue.IsEmpty())	\
		{	\
			lTmp = atol(strValue);		\
		}	\
		if (lTmp < (minvalue))	\
			(value) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
		else if (lTmp > (maxvalue)) \
			(value) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
		else						\
			(value) = (type)lTmp;	\
    }   \
}
#define GET_REGISTER_INFO_ARRAY_FLOAT(value,nsize,minvalue,defvalue,maxvalue)	\
{	\
	ASSERT(minvalue <= maxvalue);	\
    CString strEntry;   \
    for(int i=0; i<(nsize);i++)   \
    {   \
		(value[i]) = (defvalue);	\
        strEntry.Format(#value"[%d]", i);  \
		strValue = pApp->GetProfileString(strSection,strEntry, NULL);	\
		if (!strValue.IsEmpty())	\
		{	\
			(value[i]) = atof(strValue);		\
		}	\
		if ((value[i]) < (minvalue))	\
			(value[i]) = (defvalue) < (minvalue) ? (minvalue) : (defvalue);	\
		else if ((value[i]) > (maxvalue)) \
			(value[i]) = (defvalue) > (maxvalue) ? (maxvalue) : (defvalue);   \
    }   \
}
#define GET_REGISTER_INFO_ARRAY_CSTRING(value,nsize,defvalue)	\
{	\
    CString strEntry;   \
    for(int i=0; i<(nsize);i++)   \
    {   \
		(value[i]) = (defvalue);	\
		strEntry.Format(#value"[%d]", i);  \
		strValue = pApp->GetProfileString(strSection,strEntry, NULL);	\
		if (!strValue.IsEmpty())	\
        {	\
			(value[i]) = strValue;	\
        }	\
        if (strlen(value[i]) <= 0)	\
			(value[i])	= (defvalue);	\
    }   \
}

#define WRITE_REGISTER_INFO_ARRAY(fmt,value,nsize)	\
{	\
    CString strEntry;   \
    for(int i=0; i<(nsize);i++)   \
    {   \
        strEntry.Format(#value"[%d]", i);      \
        strValue.Format((fmt), (value[i]));		\
		pApp->WriteProfileString (strSection,strEntry, strValue);	\
    }   \
}
#define WRITE_REGISTER_INFO_ARRAY_CSTRING(value,nsize)	\
{	\
    CString strEntry;   \
    for(int i=0; i<(nsize);i++)   \
    {   \
		strEntry.Format(#value"[%d]", i);      \
		strValue = (value[i]);		\
		pApp->WriteProfileString (strSection,strEntry, strValue);	\
    }   \
}
