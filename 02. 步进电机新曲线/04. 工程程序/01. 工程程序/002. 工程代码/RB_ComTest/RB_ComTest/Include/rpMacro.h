///////////////////////////////////////////////////////////////////////////
// rpMacro.h : export's headerfile of rpBase.dll 
//
//	�ṩһЩ���ú궨��
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

//Ԥ������������
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

const DWORD		MAXCHAIN = 0xFFFF;				//�㴮������
const Float64	PI = 3.14159265358979323846;
const Float64	DOUBLEPRECISION = 0.0000000001;			//����С��λ���ȣ�һ������ڱȽ�
const Float64	DOUBLEZERO = 0.0000000001;			//����0�����ֵ��һ������ڱȽ�
const Float32	ANGLEPRECISION = 0.005f;				//�Ƕ�ֵ���ȣ����ȵ�λ

const Float64	TWOPI = 2 * PI;
const Float64	HALFPI = 0.5*PI;
const Float64	HALF3PI = 1.5*PI;

const COLORREF	NULLCOLOR = CLR_INVALID;			//��Ч��ɫֵ
const int		NULLINDEX = 0x7FFFFFFF;			//��Ч����ֵ������������壬<= 8Bits��

const int 		MAXSCROLLSIZE = INT_MAX / 2;			//������������ֵ

//ͼ�񾫶����
const FLOAT		MINRESOLUTION = 0.005f; 				//��С����ֵ��dpmm
const FLOAT		MAXRESOLUTION = 1000.0f; 				//��󾫶�ֵ��dpmm
const int 		MINDPI = 1;					//��С����ֵ��dpi
const int 		MAXDPI = 20000;				//��󾫶�ֵ��dpi


//�ļ���ʽ��ʶ��
//{RainBow ֧�ֵ��ļ���ʽ
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

const WORD AN_RPF = 0x5052; // 'RP'��RainBow RIP �������ļ���ʽ
const WORD RPF_IMAGE = 0x4952; // 'RI'��RainBow RIP RPF�ļ��е�Imageͼ������
const WORD RPF_LAYER = 0x4C52; // 'RL'��RainBow RIP RPF�ļ��е�Layerͼ������
const WORD RPF_FLOWER = 0x4652; // 'RF'��RainBow RIP RPF�ļ��е�Flowerͼ������
const WORD RPF_TEMPLET = 0x4752; // 'RT', RainBow RIP RPF�ļ��е�Templetͼ������
const WORD RPF_LAYOUT = 0x4F52; // 'RO'��RainBow RIP RPF�ļ��е�Layoutͼ������

const WORD CM_BITMAP = 0x4D43; // 'CM'��RainBow RIP �����BMP/TIF�ļ�˽����Ϣ��ʶ

const WORD AN_SEP = 0xFFFF; //������ϵͳ�ڲ����֣�������ͼ���ļ���ʶ��
//}RainBow ֧�ֵ��ļ���ʽ


//��ɢ��ʽ{һ�㣬�ã���ϸ}
enum SEPWAY { swRough = 0, swFine, swSmooth };
//��������{�㴮����������B����������}
enum CURVE { cvPolyline = 0, cvBezier, cvBSpline, cvTrace };
//ѹ����ʽ
//	��ֻʹ���� cpsRGB, cpsPrivate, cpsLZW(TIFF), cpsPackBits(TIFF,1Bits)
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
//ͼ������{��ɫ���Ҷȣ�ɫ������ʣ���Ч}
enum AImageType { imgClrType = 1, imgGrayType, imgClrGrayType, imgRgbType, imgStrangeType };
//���ȵ�λ: ����/Ӣ��,����/˿��,����/����,����/����,����/��
enum AResUnit { ruDefault = 0, ruDPI = 1, ruDPCMM = 2, ruDPMM, ruDPCM, ruDPM };
//�ߴ絥λ ����,˿��,����,����,��,Ӣ��
enum ASizeUnit { suDefault = 0, suPIXEL = 1, suCMM = 2, suMM = 3, suCM, suM, suINCH };
//������䷽ʽ��ͬSDK���壩
enum AFillMode { fmAlternate = ALTERNATE, fmWinding = WINDING };
//��������{ͼ��ͼ��}
//enum APictureType {picImage=0x01, picGraph=0x10};
//ͼ������{��ͨ��������}
//enum AGraphType {grfNormal=1, grfGdeline};
//��ת����{��ʱ�룬˳ʱ��}
enum CLOCKWISE { counterwise = AD_COUNTERCLOCKWISE, clockwise = AD_CLOCKWISE };
// ��Ϸ�ʽ{��ͨ�����ϻ�ϣ�����}
enum ATransferMode { tmNormal = 0, tmDyeMix = 1, tmPhoMix, tmDefault };
// �����ʶ�������趨����ͬ���߳�/���̵Ĳ���Ȩ�ޣ��ܾ��������ͣ�
//	�����ƣ�����ɾ��������д�����ɶ�����ռ
enum AShareFlag { sfDenyNone = 0, sfDenyDelete, sfDenyWrite, sfDenyRead, sfExclusive };

//���Ĵ�С�Ļ���
enum SCALEPOSITION { posNULL = 0, posTopLeft, posBottomLeft, posTopRight, posBottomRight, posLeft, posRight, posTop, posBottom };

//��ʼ��ָ���ṹ��/����
#ifndef BZERO
#define BZERO(st)	(memset(&(st), 0, sizeof(st)))
#endif

//��������������ȡ��
#define ABS(x)		((x) > 0 ? int((x)+0.5) : int((x)-0.5))

#define LOSHORT(l)	((SHORT)(l))
#define HISHORT(l)	((SHORT)(((LONG)(l)>>16) & 0xffff))

//�Ƚ�����POINT�����Ƿ����
#define EQUPOINT(pt1,pt2)	((((pt1).x) == ((pt2).x)) && (((pt1).y) == ((pt2).y)))
//ȷ��rc1�Ƿ���rc2��
#define RECTINRECT(rc1,rc2)	(((rc1).left>=(rc2).left)			\
							&& ((rc1).right<=(rc2).right)		\
							&& ((rc1).top>=(rc2).top)			\
							&& ((rc1).bottom<=(rc2).bottom))

//32λ����
#define ADDTO32(i)			(((DWORD)i+31)/32*4)
//RIP��������д����ʱÿ�����ض�Ӧ���ֽ���				
#define BAND(BitCount)		((BitCount) > 8 ? BYTE(((BitCount)+7)>>3) : 1)
//RGBתGRAY
#define RGBTOGRAY(r,g,b)	((299*(r)+587*(g)+114*(b))/1000)

//��������
#define ROUNDINT(x)			((x) > 0 ? Int32((x)+0.5) : Int32((x)-0.5))

//�Ƚ������������Ƿ����
#define _PRI_EQUDOUBLE(a,b, zero) (((Float64(a)-Float64(b)) < (zero))	\
								&& ((Float64(a)-Float64(b)) > -(zero)))
#define EQUDOUBLE(a,b)	_PRI_EQUDOUBLE(a,b,DOUBLEPRECISION)
#define EQUZERO(a)		_PRI_EQUDOUBLE(a, 0.0, DOUBLEPRECISION)

//�Ƕ����
//���Ƕ�ֵ�����ȣ��޶���TWOPI��
#define BOUNDANGLE(dAngle)\
	if(dAngle<0 || dAngle>=TWOPI)	dAngle -= floor(dAngle/TWOPI)*TWOPI
//���Ƕ�ֵ���Ƕȣ��޶���360����
#define BOUNDANGLE360(dAngle)\
	if(dAngle<0 || dAngle>=360.0)	dAngle -= floor(dAngle/360.0)*360.0
//���Ƕ�ֵ�����ȣ��޶��� PI ��
#define BOUNDPIANGLE(dAngle)\
	if(dAngle<0 || dAngle>=PI)		dAngle -= floor(dAngle/PI)*PI
//���Ƕ�ֵ���Ƕȣ��޶���180����
#define BOUNDPIANGLE180(dAngle)\
	if(dAngle<0 || dAngle>=180.0)	dAngle -= floor(dAngle/180.0)*180.0

//��ֵ��������
#define SwapDouble(a,b) {Float64 c; c=(a); (a)=(b); (b)=c;}
#define SwapInt(a,b)	{Int32 c; c=(a); (a)=(b); (b)=c;}

#define COMMONPRECISION		3		//С�����β��
#define	ZOOMSCALE			LONGLONG(m_ZoomScale >> 16) / int(WORD(m_ZoomScale))
#define	REZOOMSCALE			LONGLONG(WORD(m_ZoomScale)) / int(m_ZoomScale >> 16)

#define	XZOOMSCALE			LONGLONG(m_XZoomScale >> 16) / int(WORD(m_XZoomScale))
#define	YZOOMSCALE			LONGLONG(m_YZoomScale >> 16) / int(WORD(m_YZoomScale))

#define	XOUTSCALE			LONGLONG(m_XOutScale >> 16) / int(WORD(m_XOutScale))
#define	XREOUTSCALE			LONGLONG(WORD(m_XOutScale)) / int(m_XOutScale >> 16)
#define	YOUTSCALE			LONGLONG(m_YOutScale >> 16) / int(WORD(m_YOutScale))
#define	YREOUTSCALE			LONGLONG(WORD(m_YOutScale)) / int(m_YOutScale >> 16)

//���űȽϼ���
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

//�ؼ�ʹ�ܣ������ڶԻ�������ʹ�ã�
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
//���鿽����a:dest, b:src
#define memcpy_array(a,b)	memcpy((a),(b),min(sizeof(a),sizeof(b)))


//��ȡ���ð���״̬
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

//�ṹ���ࣩ������������
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

//�������ļ��ж�ȡָ�����ڵ���ָ��������(value)Ϊ��ʶ�ı�������ֵ
//����ǰ�趨��� strSection, szReg/strValue, strIniFile
//strSection :
//	ָ��������������[CURRENT]
//szReg/strValue	:
//	ָ���������������� char szReg[200]; CString strValue;
//strIniFile:
//	ָ�������������ļ���
//value		:
//	������������Ϊ��Ч�ı�����ʶ��������ʹ��������
//type		:
//	��������������ֵ����ֵ���ͣ�һ����������������ʱ, BYTE, int, short, long ...
//fmt		:
//	��������ĸ�ʽ���ַ������� "%d", "%f"��
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

//��ע����ж�ȡָ�����ڵ���ָ��������(value)Ϊ��ʶ�ı�������ֵ
//����ǰ�趨��� pApp, strSection, strValue
//strSection :
//	ָ��������������[CURRENT]
//strValue	:
//	ָ���������������� CString strValue;
//value		:
//	������������Ϊ��Ч�ı�����ʶ��������ʹ��������
//type		:
//	��������������ֵ����ֵ���ͣ�һ����������������ʱ, BYTE, int, short, long ...
//fmt		:
//	��������ĸ�ʽ���ַ������� "%d", "%f"��

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
//type : ������������, BYTE, int, short, long ...
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
