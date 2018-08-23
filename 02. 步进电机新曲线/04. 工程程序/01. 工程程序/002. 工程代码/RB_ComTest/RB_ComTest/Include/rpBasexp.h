///////////////////////////////////////////////////////////////////////////
// rpBasexp.h : export's headerfile of rpBase.dll 
//
//	提供最基本的操作定义，包括常量定义、压缩算法、MFC类库的扩展等
//
// Created by hzm 2009-11-30
//
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "afxstatusbar.h"
#include "afxdialogex.h"

#include "rpMacro.h"
#include "accctrl.h"

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

//	字符串资源读取函数
//注：最大字符个数为 _ANX_LOADSTRING_MAX_
#define _ANX_LOADSTRING_MAX_		1000
RP_EXT_API CString AnxLoadString(HINSTANCE hInstance, UINT uID);

// 不定参数的对话框及状态栏提示函数
//	hInstance			:[in]指定相应字符串ID(idHeadStr,idEndStr)对应的工程句柄，
//							不能为NULL，当为NULL时函数调用错误。 
//	idHeadStr, idEndStr :[in]指定提示字符串ID，可以为0（表示不需要相应字符提示信息）
//	lpFormat,...		:[in]中间字符串，可使用格式化字符串及相应不定参数，可参考 printf
//	fuStyle				:[in]对话框风格。
//注：函数最终输出形式：[idHeadStr + ] lpFormat [+ idEndStr]
//
//对话框显示
// 参数说明可参阅 AfxMessageBox 函数Help。
RP_EXT_API int AFX_CDECL AnxDialogMsg (LPCTSTR lpFormat, ...);	// Used in debug
RP_EXT_API int AFX_CDECL AnxDialogMsg (HINSTANCE hInstance, UINT idHeadStr, UINT idEndStr=0, UINT fuStyle=MB_OK, LPCTSTR lpFormat=NULL, ...);
//	状态栏显示
RP_EXT_API void AFX_CDECL AnxStatusMsg (LPCTSTR lpFormat, ...); // Used in debug
RP_EXT_API void AFX_CDECL AnxStatusMsg (HINSTANCE hInstance, UINT idHeadStr, UINT idEndStr=0, LPCTSTR lpFormat=NULL, ...);

//该宏仅仅用于 Debug 版，用对话框/状态栏来简单提示错误信息。对Release 版定义为NULL。
#ifdef	_DEBUG
	#define	DEBUGDLGPROMPT		::AnxDialogMsg
	#define	DEBUGSTATUSPROMPT	::AnxStatusMsg
#else
	#define	DEBUGDLGPROMPT		__noop
	#define	DEBUGSTATUSPROMPT	__noop
#endif
//调用密码输入框
//	strSrc				:[out]原密码输入值
//	pstrDst				:[out]新密码输入值，当 bChangePassword==TRUE时有效
//	bChangePassword		:[in]是否调用密码修改界面，为FALSE时仅调用密码输入界面
//	bCanEmpty			:[in]新密码是否允许为空
RP_EXT_API BOOL AFX_CDECL AnxPasswordDlg(CString &strSrc, CString *pstrDst, BOOL bChangePassword, BOOL bCanEmpty);


//缩放比例计算
// dRatio	:[in/out]开始值为当前级别的比例值。结束时改为下一缩小/放大级别的比例值//
//
// 当前比例缩小一级
RP_EXT_API void AnxZoomOutRatio(double& dRatio);
//	当前比例放大一级
RP_EXT_API void AnxZoomInRatio(double& dRatio);
//	调整当前比例值为整数倍（放大或缩小）
// return dRatio==(int)x or dRatio==1.0/(int)x
RP_EXT_API void AnxZoomNormalRatio(double& dRatio);

// 求浮点数精度值，nPrecision:小数点位数，bRound : 是否四舍五入
//注：AnxFloatPrecision(1.1234567, 4, TRUE) 返回 1.1235
RP_EXT_API double AnxFloatPrecision(double dValue, int nPrecision, BOOL bRound=TRUE);

// 求浮点数精度值(字符串)，nPrecision:小数点位数，bRound : 是否四舍五入
//注：AnxFloatPrecisionString(1.1234567, 4, TRUE) 返回 "1.1235"
#define MAXPRECISION	10			//最大浮点数精度位数
RP_EXT_API CString AnxFloatPrecisionString(double dValue, int nPrecision, BOOL bRound=TRUE);

#define MAX_LIMITWIDTH	8			//十六进制数最大字符个数
//bLimit : 仅十六进制时有效，表示转换后的字符个数，为0时表示自动，最大为MAX_LIMITWIDTH
RP_EXT_API CString AnxIntString(int nValue, BOOL bHex, BYTE bLimit=0);
RP_EXT_API CString AnxDWORDString(DWORD dwValue, BOOL bHex, BYTE bLimit=0);

//浮点数转整型值
RP_EXT_API int AnxFloatToInt(double dValue, BOOL bRound=TRUE);
RP_EXT_API DWORD AnxFloatToDWORD(double dValue, BOOL bRound=TRUE);


//实现不同窗口进的坐标点转换
//参数：
//	msg: 窗口消息（可由 PeekMessage(...)得到）msg.message的值必须在 WM_MOUSEFIRST <= msg.message <= WM_MOUSELAST 范围内
//	pDstWnd : 目的窗口指针，为空时不转换（源窗口指针在 msg 中可以得到，不必传入）
//	pDstPt : 坐标转换后的坐标点值（当未进行过转换时也计算），默认为NULL表示不用计算
//返回：
//	经过坐标转换后窗口消息
RP_EXT_API MSG AnxMapPoint (MSG msg, CWnd* pDstWnd, POINT* pDstPt = NULL);

//当前指针（CObject*）是否有效
RP_EXT_API BOOL AnxValidObject(const CObject* pOb);


//单位转换函数
//

//精度单位	：AResUnit，长度单位：ASizeUnit，g_Flags为全局参数变量
// resUnit	：dRes 的精度单位，可以为ruDefault
// szUnit	：dValue的长度单位，可以为suDefault
// 精度及长度转化,
//		单位为resUnit的精度dRes转化为DPI、DPMM，
//		单位为szUnit的长度dValue转化为PIXEL、MM、INCH
// nBufferMax	：字符串最大长度
// bRound		：是否四舍五入取整
// 注：ruDefault/suDefault指的是当前系统使用的精度/长度（g_Flags.ResUnit / g_Flags. SizeUnit）
//
//精度相关
RP_EXT_API int ToNDPI (double dRes, AResUnit resUnit); 
RP_EXT_API double ToDPI (double dRes, AResUnit resUnit);
RP_EXT_API double ToDPMM (double dRes, AResUnit resUnit);
RP_EXT_API double ConvertRes(double dRes, AResUnit srcUnit, AResUnit dstUnit, BOOL bRound);

//以下函数判断精度是否相等
RP_EXT_API BOOL EqualDPI (double dRes1, double dRes2);
RP_EXT_API BOOL EqualDPMM (double dRes1, double dRes2);
RP_EXT_API BOOL EqualRes (double dRes1, double dRes2, AResUnit resUnit, BOOL bRound);
//dRes1,dRes2四舍五入取整后判断
RP_EXT_API BOOL EqualNDPI (double dRes1, double dRes2);

//返回不同分辨率的精度范围
RP_EXT_API double DefResPrecision (AResUnit resUnit);

//修正输入精度值为有效可用精度
RP_EXT_API double RightRes(double dRes, AResUnit resUnit);

//当前精度是否有效
RP_EXT_API BOOL IsValidRes(double dRes, AResUnit resUnit);

//将精度单位转为字符串表示 ("dpi, dpmm, ...")
// nBufferMax:字符串最大长度
RP_EXT_API int ResUnitString (LPTSTR lpBuffer, int nBufferMax, AResUnit resUnit);
RP_EXT_API int ResUnitString (CString& str, AResUnit resUnit);

//长度相关 
RP_EXT_API int ToNPIXEL (double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
RP_EXT_API double ToPIXEL (double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
RP_EXT_API double ToMM (double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
RP_EXT_API double ToINCH (double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
RP_EXT_API double ConvertSize(double dValue, ASizeUnit srcUnit, ASizeUnit dstUnit, double dRes, AResUnit resUnit, BOOL bRound);

//将尺寸单位转为字符串表示 ("inch, mm, pixel, ...")
// nBufferMax:字符串最大长度
RP_EXT_API int SizeUnitString (LPTSTR lpBuffer, int nBufferMax, ASizeUnit szUnit);
RP_EXT_API int SizeUnitString (CString& str, ASizeUnit szUnit);

//长度单位转换
class RP_EXT_CLASS CUnitTrans  
{
public:
	CUnitTrans();
	~CUnitTrans();

public:
	BOOL SetParams(double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
	BOOL SetRes(double dRes, AResUnit resUnit);
public:
	//bNPixel : 当为TRUE且szUnit==suPIXEL时，表示返回四舍五入的像素值
	double	GetValue(ASizeUnit szUnit, int nPrecision=COMMONPRECISION, BOOL bNPixel=TRUE) const;
	BOOL IsValid() const;

protected:
	double		m_dMMVal; //MM
	double		m_dMMRes; //dpMM;
};


//POINTD ： 浮点操作类
struct RP_EXT_CLASS POINTD
{
	Float64 x, y;

	POINTD() {x = y = 0.0;}
	POINTD (Float64 dx, Float64 dy) {x=dx; y=dy;}
	POINTD(const POINT& pt) {x = pt.x;	y = pt.y;}
	POINTD(const POINTF& pt) {x = pt.x;	y = pt.y;}
	POINTD(const POINTD& pt) {x = pt.x;	y = pt.y;}
	
	operator POINT() const 
	{
		POINT pt; 
		pt.x= (Int32)(x + (x < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		pt.y= (Int32)(y + (y < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		return pt; 
	}
	POINT GPOINT() const 
	{
		POINT	pt;
		Float64 d;
		d= x + (x < 0 ? -0.5 : 0.5);
		pt.x= (Int32)(d + (d < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		d= y + (y < 0 ? -0.5 : 0.5);
		pt.y= (Int32)(d + (d < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		return pt; 
	}

	BOOL operator == (const POINTD& pt) const { return (EQUDOUBLE(x,pt.x) && EQUDOUBLE(y,pt.y)); }
	BOOL operator == (const POINT& pt) const { return (EQUDOUBLE(x,pt.x) && EQUDOUBLE(y,pt.y)); }
	BOOL operator != (const POINTD& pt) const { return (!EQUDOUBLE(x,pt.x) || !EQUDOUBLE(y,pt.y)); }
	BOOL operator != (const POINT& pt) const { return (!EQUDOUBLE(x,pt.x) || !EQUDOUBLE(y,pt.y)); }
	POINTD operator + (const POINTD& pt) const { POINTD ptRet(*this); ptRet.x += pt.x; ptRet.y += pt.y; return ptRet; }
	POINTD operator - (const POINTD& pt) const { POINTD ptRet(*this); ptRet.x -= pt.x; ptRet.y -= pt.y; return ptRet; }
	POINTD operator += (const POINTD& pt) { x += pt.x; y += pt.y; return *this; }
	POINTD operator -= (const POINTD& pt) { x -= pt.x; y -= pt.y; return *this; }
};
typedef POINTD* LPPOINTD;
typedef	const POINTD* LPCPOINTD;
typedef CArray<POINTD,POINTD>	CPOINTDArray;

//RECTD ： 浮点矩形操作类定
class RP_EXT_CLASS RECTD 
{
public:
	Float64 left;
	Float64 top;
	Float64 right;
	Float64 bottom;

public:
	RECTD () {left=top=right=bottom=0.0;}
	RECTD (Float64 dl, Float64 dt, Float64 dr, Float64 db) 
	{ left = dl;	top = dt;	right = dr;		bottom = db; }
	RECTD (const RECT& rect) {left=rect.left; top=rect.top; right=rect.right; bottom=rect.bottom;}
	RECTD (LPCPOINTD lpPt, Int32 nNumPt) { GetBorderRect (lpPt, nNumPt); }
	
	RECT GRECT() const 
	{
		RECT rc; Float64 d;
		d= left + (left < 0 ? -0.5 : 0.5);
		rc.left= (Int32)(d + (d < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		d= top + (top < 0 ? -0.5 : 0.5);
		rc.top= (Int32)(d + (d < 0 ? -DOUBLEZERO : DOUBLEZERO));
		d= right + (right < 0 ? -0.5 : 0.5);
		rc.right= (Int32)(d + (d < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		d= bottom + (bottom < 0 ? -0.5 : 0.5);
		rc.bottom= (Int32)(d + (d < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		return rc; 
	}

	operator RECT() const 
	{
		RECT rc;
		rc.left	 = (Int32)(left  + (left  < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		rc.top	 = (Int32)(top	 + (top   < 0 ? -DOUBLEZERO : DOUBLEZERO));
		rc.right = (Int32)(right + (right < 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		rc.bottom= (Int32)(bottom+ (bottom< 0 ? -DOUBLEZERO : DOUBLEZERO)); 
		return rc; 
	}
	
	BOOL operator == (const RECTD& rtDst) const 
	{ return (left==rtDst.left && top==rtDst.top && right==rtDst.right && bottom==rtDst.bottom); }
	BOOL operator != (const RECTD& rtDst) const 
	{ return (left!=rtDst.left || top!=rtDst.top || right!=rtDst.right || bottom!=rtDst.bottom); }
	BOOL operator == (const RECT& rtDst) const 
	{ return (left==rtDst.left && top==rtDst.top && right==rtDst.right && bottom==rtDst.bottom); }
	BOOL operator != (const RECT& rtDst) const 
	{ return (left!=rtDst.left || top!=rtDst.top || right!=rtDst.right || bottom!=rtDst.bottom); }

	Float64 Width () const {return (right - left);}
	Float64 Height () const {return (bottom - top);}
	POINTD CenterPoint () const {POINTD ptRet((left+right)/2.0, (top+bottom)/2.0); return ptRet;}
	POINTD TopLeft() const {POINTD ptRet(left,top); return ptRet;}
	POINTD BottomRight() const {POINTD ptRet(right,bottom); return ptRet;}
	void NormalizeRect () 
	{
		if (right < left) SwapDouble (right, left);
		if (bottom < top) SwapDouble (bottom, top);
	}
	void NormalizeBorder() 
	{
		NormalizeRect ();
		if (Width() == 0) right = left + 1;
		if (Height() == 0) bottom = top + 1;
		Float64 dW = max (Width(), Height());
		right = left + dW;
		bottom = top + dW;
	}
	
	void SetRect (Float64 dl, Float64 dt, Float64 dr, Float64 db) 
	{ left = dl;	top = dt;	right = dr;		bottom = db; }
	void SetRectEmpty() {left=right=top=bottom=0;}
	BOOL IsRectEmpty() const {return (left==right || top==bottom);}
	BOOL IsRectNull() const {return (left==right && top==bottom && left==top && left==0);}
	void UnionRect (const RECTD& rcSrc, const RECTD& rcUnion) 
	{ 
		left	= min (rcSrc.left, rcUnion.left);
		right 	= max (rcSrc.right, rcUnion.right);
		top 	= min (rcSrc.top, rcUnion.top);
		bottom 	= max (rcSrc.bottom, rcUnion.bottom);
	}
	void OffsetRect (Float64 dx, Float64 dy) 
	{
		left += dx;	right += dx;
		top += dy;	bottom += dy;
	}
	void GetBorderRect (LPCPOINTD lpPt, Int32 nNumPt) 
	{
		if (lpPt==NULL || nNumPt <= 0)
			SetRectEmpty();
		else 
		{
			Int32 i;
			left = lpPt[0].x;	right = lpPt[0].x;
			top = lpPt[0].y; 	bottom = lpPt[0].y;
			for (i=1; i< nNumPt; i++) 
			{
				if (lpPt[i].x < left)
					left = lpPt[i].x;
				else if (lpPt[i].x > right)
					right = lpPt[i].x;
				if (lpPt[i].y < top)
					top = lpPt[i].y;
				else if (lpPt[i].y > bottom)
					bottom = lpPt[i].y;
			}
		}
	}
	BOOL PtInRect (const POINTD point) const 
	{ return (point.x < right && point.x >= left && point.y < bottom && point.y >= top); }
};
typedef RECTD FAR* LPRECTD;

//精确度处理类（四舍五入等）
//	把双精度类型转化精确的整数类型
class RP_EXT_CLASS CPrecise
{
public:
	//把双精度类型转化精确的整数类型
	// 返回转化后的整数值
	//	d	: [in]被转化的双精度值
	//	dp	: [in]转化精度取[0, 1.0)之间
	static int PreciseInt(double d, double dp=0.5)
	{ return d >= 0 ? AnxFloatToInt(d+dp,FALSE) : AnxFloatToInt(d-dp,FALSE); }
	
	//把双精度类型转化精确的整数类型
	//	dx, dy	: [in]被转化的双精度值
	//	nx, ny	: [in]接收转化后的整数值
	//	dp		: [in]转化精度取[0, 1.0)之间
	static void PrecisePoint(double dx, double dy, int& nX, int& nY, double dp=0.5)
	{	
		nX = dx >= 0 ? AnxFloatToInt(dx+dp,FALSE) : AnxFloatToInt(dx-dp,FALSE); 
		nY = dy >= 0 ? AnxFloatToInt(dy+dp,FALSE) : AnxFloatToInt(dy-dp,FALSE); 
	}
	static void PrecisePoint(double dx, double dy, long& nX, long& nY, double dp=0.5)
	{	
		nX = dx >= 0 ? AnxFloatToInt(dx+dp,FALSE) : AnxFloatToInt(dx-dp,FALSE); 
		nY = dy >= 0 ? AnxFloatToInt(dy+dp,FALSE) : AnxFloatToInt(dy-dp,FALSE); 
	}
	// 把双精度类型的 POINTD 结构转化精确的POINT 结构
	//	ptd	: [in]被转化的双精度POINTD 结构
	//	pt	: [in]接收转化后的POINT 结构
	//	dp	: [in]转化精度,取[0, 1.0)之
	static void PrecisePoint(POINTD ptd, POINT& pt, double dp=0.5)
	{	PrecisePoint(ptd.x, ptd.y, pt.x, pt.y, dp);	}
};

//////////////////////////////////////////////////////////////////
// Matrix	(T)					Identity Matrix		(I)
//	eM11	eM12	0					1	0	0
//	eM21	eM22	0					0	1	0
//	eDx		eDy		1					0	0	1
//
//	Source
//		[x', y', 1] = [x, y, 1] * T
// 本矩阵为一 3*3 的方阵
struct RP_EXT_CLASS TransMatrix
{
public:
	double eM11, eM12;
	double eM21, eM22;
	double eDx, eDy;
public:
	TransMatrix ();
	TransMatrix (const XFORM& xform);
	TransMatrix (const TransMatrix& trMatrix);

	//矩阵求逆
	BOOL InverseMatrix ();
	BOOL InverseMatrix (const TransMatrix& trsrc)
	{ LoadMatrix(trsrc); return InverseMatrix(); }
	
	//设矩阵
	//dMatrix is a 3*3  Matrix
	void LoadMatrix (const TransMatrix& trMatrix);
	void SetTransMatrix (double* dMatrix);
	//得矩阵数组
	void GetTransMatrix (double* dMatrix) const;

	//是否为单位矩阵
	BOOL IdentityMatrix (void) const;
	//设为单位矩阵
	void LoadIdentity () 
	{ eM11 = 1;	eM12 = 0; eM21 = 0;	eM22 = 1; eDx = 0; eDy = 0; }

	BOOL OnlyScaleOffset() const;

	//两个变换矩阵是否仅有移动操作
	//return 0 : have other trans
	//		-1 : equal
	//		 1 : only offset
	int OnlyOffset (const TransMatrix& transOld, POINTD* pptdOffset) const;

	//XFORM 类型操作符, 将 ATransform class 转化为 XFORM struct (SDK定义)
	operator XFORM () const;
public:
	//矩阵求逆
	// dMatrix is a n*n Matrix
	static BOOL InverseMatrix (double* dMatrix, int n=3);
	static BOOL InverseMatrix (double* dMatrixDst, const double* dMatrixSrc, int n=3);
	static BOOL InverseMatrix (TransMatrix& trdest, const TransMatrix& trsrc)
	{ return trdest.InverseMatrix(trsrc); }
};

class RP_EXT_CLASS ATransform : public TransMatrix
{
public:
	ATransform(const TransMatrix& trMatrix);
	ATransform(const XFORM& form);
	ATransform();
	virtual ~ATransform();

public:
	void MultMatrix (const ATransform& trform);
	static void MultMatrix (ATransform& formdst, const ATransform& formsrc1, const ATransform& formsrc2);
	
public:
	ATransform operator *(const ATransform& trform) const
	{ ATransform tr(*this); tr.MultMatrix(trform); return tr; }
	ATransform operator *(const TransMatrix& trMatrix) const
	{ ATransform tr1(*this),tr2(trMatrix); tr1.MultMatrix(tr2); return tr1; }
	const BOOL operator == (const ATransform& trform) const
	{ 
		if (!EQUDOUBLE(eDx,trform.eDx) || !EQUDOUBLE(eDy,trform.eDy)
			|| !EQUDOUBLE(eM11,trform.eM11) || !EQUDOUBLE(eM22,trform.eM22)
			|| !EQUDOUBLE(eM12,trform.eM12) || !EQUDOUBLE(eM21,trform.eM21)) 
			return FALSE;
		return TRUE;
	}

	friend RP_EXT_API ATransform operator *(const TransMatrix& trMatrix, const ATransform& trform)
	{ ATransform tr(trMatrix); tr.MultMatrix(trform); return tr; }
public:
	//以下点 ptTrans(dx, dy) : 矩阵变换的相对坐标点

	//平移
	//	平移 dx, dy
	//	eDx = dx, eDy = dy
	void Translation (double dx, double dy);
	void Translation (const ATransform& trform, double dx, double dy)
	{ LoadMatrix (trform); Translation (dx, dy); }
	//平移-->比例-->平移
	//	相对于点 ptTrans 比例变换 sx, sy
	//	eM11 = sx, eM22 = sy
	void Scaling (double sx, double sy, double dx=0.0, double dy=0.0);
	void Scaling (const ATransform& trform, double sx, double sy, double dx=0.0, double dy=0.0)
	{ LoadMatrix (trform); Scaling (sx, sy, dx, dy); }
	//平移-->旋转-->平移
	//	相对于点 ptTrans 旋转 fangle
	//	eM11 = cos(fangle), eM12 = sin(fangle)
	//	eM21 = -sin(fangle), eM22 = cos(fangle)
	void Rotate (double fangle, double dx=0.0, double dy=0.0);
	void Rotate (const ATransform& trform, double fangle, double dx=0.0, double dy=0.0)
	{ LoadMatrix (trform); Rotate (fangle, dx, dy); }
	//平移-->错切-->平移
	//	相对于点 ptTrans 错切 sx, sy
	//	eM12 = sx, eM21 = sy
	void Shear (double sx, double sy, double dx=0.0, double dy=0.0);
	void Shear (const ATransform& trform, double sx, double sy, double dx=0.0, double dy=0.0)
	{ LoadMatrix (trform); Shear (sx, sy, dx, dy); }
	//平移-->旋转-->对称-->旋转-->平移
	// 关于过点 (dx, dy)、角度为 fangle 的直线对称
	// if not (0 <= fangle < PI) 则关于(dx, dy)对称
	void Reflection (double fangle=0.0, double dx=0.0, double dy=0.0);
	void Reflection (const ATransform& trform, double fangle=0.0, double dx=0.0, double dy=0.0) 
	{ LoadMatrix (trform); Reflection (fangle, dx,dy); }

public:
	//==============================================================
	//以下为根据矩阵正求坐标
	// dp : [in]point precise (0 <= dp < 1.0)

	//单个点正求坐标
	void CalcPoint (double& dx, double& dy) const;
	void CalcPoint(float & fx, float & fy) const 
	{ 
		double dx = (double)fx, dy = (double)fy; CalcPoint (dx, dy); 
		fx = (float)dx; fy = (float)dy; 
	}
	void CalcPoint (int& x, int& y, double dp=0.5) const 
	{
		double dx = (double)x, dy = (double)y; CalcPoint (dx, dy);
		CPrecise::PrecisePoint(dx, dy, x, y, dp);
	}
	void CalcPoint(long& lx, long& ly, double dp=0.5) const 
	{
		double dx = (double)lx, dy = (double)ly; CalcPoint(dx, dy); 
		CPrecise::PrecisePoint(dx, dy, lx, ly, dp);
	}
	void CalcPoint(POINT& pt, double dp=0.5) const { CalcPoint(pt.x, pt.y, dp); }
	void CalcPoint(POINTF& pt) const { CalcPoint(pt.x, pt.y); }
	void CalcPoint(POINTD& pt) const { CalcPoint(pt.x, pt.y); }

	//Poly 形式点串正求坐标
	void CalcPoints(LPPOINT lpPt, int nNumPt, double dp=0.5) const
	{ 
		ASSERT(lpPt && nNumPt>0); 
		for (int i = 0; i < nNumPt; i++) CalcPoint(lpPt[i].x, lpPt[i].y, dp); 
	}
	void CalcPoints(LPPOINTF lpPt, int nNumPt) const
	{ 
		ASSERT(lpPt && nNumPt>0); 
		for (int i = 0; i < nNumPt; i++) CalcPoint(lpPt[i].x, lpPt[i].y); 
	}
	void CalcPoints(LPPOINTD lpPt, int nNumPt) const
	{ 
		ASSERT(lpPt && nNumPt>0); 
		for (int i = 0; i < nNumPt; i++) CalcPoint(lpPt[i].x, lpPt[i].y); 
	}

	//PolyPoly 形式点串正求坐标
	void CalcPointPoints(LPPOINT lpPt, LPINT pCount, int nCount, double dp=0.5) const
	{
		ASSERT(lpPt && pCount && nCount>0); 
		LPPOINT lpTmp = lpPt;
		int		i, j;
		for (i = 0; i < nCount; i++) 
		{
			ASSERT(pCount[i]>0);
			if (pCount[i]<=0) continue;
			for (j = 0; j < pCount[i]; j++,lpTmp++) CalcPoint(lpTmp[j].x, lpTmp[j].y, dp);
		}
	}
	void CalcPointPoints(LPPOINTF lpPt, LPINT pCount, int nCount) const
	{
		ASSERT(lpPt && pCount && nCount>0);
		LPPOINTF lpTmp = lpPt;
		int		i, j;
		for (i = 0; i < nCount; i++) 
		{
			ASSERT(pCount[i]>0);
			if (pCount[i]<=0) continue;
			for (j = 0; j < pCount[i]; j++,lpTmp++) CalcPoint(lpTmp[j].x, lpTmp[j].y);
		}
	}
	void CalcPointPoints(LPPOINTD lpPt, LPINT pCount, int nCount) const
	{
		ASSERT(lpPt && pCount && nCount>0);
		LPPOINTD lpTmp = lpPt;
		int		i, j;
		for (i = 0; i < nCount; i++) {
			ASSERT(pCount[i]>0);
			if (pCount[i]<=0) continue;
			for (j = 0; j < pCount[i]; j++,lpTmp++) CalcPoint(lpTmp[j].x, lpTmp[j].y);
		}
	}
	//长度正求坐标
	void CalcDist (double & fDist) const;

	//==============================================================
	//以下为根据矩阵坐标求逆
	//	在使用 InverseCalPoint (...) 函数前请先使用 CanInverseCalc().
	//
	inline BOOL CanInverseCalc () const;		//是否可以进行坐标求逆操作

	//单个点坐标求逆
	void InverseCalcPoint(double & dx, double & dy) const;
	void InverseCalcPoint(float & fx, float & fy) const 
	{
		double dx = (double)fx, dy = (double)fy; InverseCalcPoint (dx, dy);
		fx = (float)dx;		fy = (float)dy;
	}
	void InverseCalcPoint (int& x, int& y, double dp=0.5) const
	{
		double dx = (double)x, dy = (double)y; InverseCalcPoint (dx, dy);
		CPrecise::PrecisePoint(dx, dy, x, y, dp);
	}
	void InverseCalcPoint(long& lx, long& ly, double dp=0.5) const
	{
		double dx = (double)lx, dy = (double)ly; InverseCalcPoint(dx, dy); 
		CPrecise::PrecisePoint(dx, dy, lx, ly, dp);
	}
	void InverseCalcPoint(POINT& pt, double dp=0.5) const { InverseCalcPoint(pt.x, pt.y, dp); }
	void InverseCalcPoint(POINTF& pt) const { InverseCalcPoint(pt.x, pt.y); }
	void InverseCalcPoint(POINTD& pt) const { InverseCalcPoint(pt.x, pt.y); }

	//Poly点串形式坐标求逆
	void InverseCalcPoints(LPPOINT lpPt, int nNumPt, double dp=0.5)	const
	{
		ATransform trform(*this);
		if (!trform.InverseMatrix()) return;
		trform.CalcPoints (lpPt, nNumPt, dp);
	}
	void InverseCalcPoints(LPPOINTF lpPt, int nNumPt) const
	{
		ATransform trform(*this);
		if (!trform.InverseMatrix()) return;
		trform.CalcPoints (lpPt, nNumPt);
	}
	void InverseCalcPoints(LPPOINTD lpPt, int nNumPt) const
	{
		ATransform trform(*this);
		if (!trform.InverseMatrix()) return;
		trform.CalcPoints (lpPt, nNumPt);
	}

	//PolyPoly点串形式坐标求逆
	void InverseCalcPointPoints(LPPOINT lpPt, LPINT pCount, int nCount, double dp=0.5) const
	{
		ATransform trform(*this);
		if (!trform.InverseMatrix()) return;
		trform.CalcPointPoints  (lpPt, pCount, nCount, dp);
	}
	void InverseCalcPointPoints(LPPOINTF lpPt, LPINT pCount, int nCount) const
	{
		ATransform trform(*this);
		if (!trform.InverseMatrix()) return;
		trform.CalcPointPoints (lpPt, pCount, nCount);
	}
	void InverseCalcPointPoints(LPPOINTD lpPt, LPINT pCount, int nCount) const
	{
		ATransform trform(*this);
		if (!trform.InverseMatrix()) return;
		trform.CalcPointPoints (lpPt, pCount, nCount);
	}
	
	//长度值坐标求逆
	void InverseCalcDist (double & fDist) const;

	//矩阵分解，将矩阵分解为“比例”、“平移”、“旋转”三个不同矩阵
	// ptS	: [in]Scaling Center;
	// ptR	: [in]Rotate Center;
	// pTrScale, pTrMove, pTrRotate : Receive resoluted transform
	//注：此函数还未完善，暂不使用
	// Resolution Transform Let Transform = pTrScale * pTrMove * pTrRotate;
	BOOL Resolution(const POINTD& ptS, const POINTD& ptR, ATransform* pTrScale = NULL, ATransform* pTrMove = NULL, ATransform* pTrRotate = NULL) const;

public:
	//	DWORD	ZoomScale; // Device:Logic = HiWord:LoWord

	// pPtOffFV is Flower (0, 0) trans to View coordinate.
	//设单位矩阵的变化关系 (Trans 'Flower To Flower' To 'Flower To View'关系)
	void IdentityFView (DWORD ZoomScale, const POINT* pPtOffFV);
	// Trans 'Flower to Flower' to 'View to View'
	void FFlowerToVView (const ATransform& FFform, DWORD ZoomScale, const POINT* pPtOffFV);
	// Trans 'Flower to Flower' to 'Flower to View'
	void FFlowerToFView (const ATransform& FFform, DWORD ZoomScale, const POINT* pPtOffFV);
	// Trans 'Flower to View' to 'Flower to Flower'
	void FViewToFFlower (const ATransform& FVform, DWORD ZoomScale, const POINT* pPtOffFV);
	// Trans 'Flower to View' to 'View To View'
	void FViewToVView (const ATransform& FVform, DWORD ZoomScale, const POINT* pPtOffFV);
	
	// ptOffset is View (0, 0) trans to Flower coordinate.
	//设单位矩阵的变化关系 (Trans 'View To View' To 'View To Flower'关系)
	void IdentityVFlower (DWORD ZoomScale, const POINT* pPtOffVF);
	// Trans 'View To View' to 'Flower to Flower'
	void VViewToFFlower (const ATransform& VVform, DWORD ZoomScale, const POINT* pPtOffVF);
	// Trans 'View To View' to 'View to Flower'
	void VViewToVFlower (const ATransform& VVform, DWORD ZoomScale, const POINT* pPtOffVF);
	// Trans 'View To Flower' to 'View to View'
	void VFlowerToVView (const ATransform& VFform, DWORD ZoomScale, const POINT* pPtOffVF);
	// Trans 'View To Flower' to 'Flower To Flower'
	void VFlowerToFFlower (const ATransform& VFform, DWORD ZoomScale, const POINT* pPtOffVF);

};

//BITMAP 扩展类
class RP_EXT_CLASS ABitmap : public CBitmap
{
	DECLARE_DYNAMIC(ABitmap)
		
public:
	// Constructors
	ABitmap();
	virtual ~ABitmap();
	
public:
	BOOL Copy(HBITMAP hBitmap);
	BOOL GetRgbBits(BYTE* pBits);		// 24-bits
	BOOL SetRgbBits(const BYTE* pBits); // 24-bits
};

/////////////////////////////////////////////////////////////////////////////
// CMenuWnd window
//在指定位置上弹出菜单操作时用，一般需对其进行重载
class RP_EXT_CLASS CMenuWnd : public CWnd
{
	DECLARE_DYNCREATE(CMenuWnd)
		
		// Construction
public:
	CMenuWnd();
	
	// Attributes
public:
	void* m_pData;		//用于派生类操作时定义相应参数变量，基类中默认为NULL

	// Operations
public:
	//在指定位置创建弹出菜单，并在选择后退出
	//	hInstance	:[in]nIDMenu相应的实例句柄，此值必须正常，否则有可能创建出错
	//	nIDMenu		:[in]ID of menu, if uIDMenu==0 then failed
	//	point		:[in]screen coord
	//	nFlags		:[in]指定屏幕位置标识和鼠标键标识，参见：CMenu::TrackPopupMenu
	//	pData		:[in]用于派生类操作时定义相应参数变量，基类中默认为NULL
	//	nSubMenu	:[in]nIDMenu 的子菜单项值，-1时表示无子菜单
	//	pParentWnd	:[in]parent windows, if NULL the AfxGetMainWnd()
	//注：以下两个函数在相应的派生类中应作相应的重载，其中 OnInitMenuPopup 也可以没有
	//	MenuCommand (UINT uId);
	//	InitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	LRESULT Create(HINSTANCE hInstance, UINT nIDMenu, CPoint point, UINT nFlags, int nSubMenu=-1, CWnd* pParentWnd=NULL);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuWnd)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CMenuWnd();
	
	// Generated message map functions
	//{{AFX_MSG(CMenuWnd)
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//UINT uId : 表示选中的菜单项ID，uId==0 表示没有选中的菜单项
	//注：此函数必须重载
	virtual LRESULT MenuCommand (UINT uId);
	//对菜单项进行必要的初始化操作（可不重载）
	virtual void InitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	
	//在指定位置创建弹出菜单，并在选择后退出
	//	hInstance	:[in]nIDMenu相应的实例句柄，此值必须正常，否则有可能创建出错
	//	nIDMenu		:[in]ID of menu, if uIDMenu==0 then failed
	//	point		:[in]screen coord
	//	nFlags		:[in]指定屏幕位置标识和鼠标键标识，参见：CMenu::TrackPopupMenu
	//	pData		:[in]用于派生类操作时定义相应参数变量，基类中默认为NULL
	//	nSubMenu	:[in]nIDMenu 的子菜单项值，-1时表示无子菜单
	//	pParentWnd	:[in]parent windows, if NULL the AfxGetMainWnd()
	//注：以下两个函数在相应的派生类中应作相应的重载，其中 OnInitMenuPopup 也可以没有
	//	MenuCommand (UINT uId);
	//	InitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//	具体可参见 CMenuWnd::Create() 的实现
	static LRESULT CreateMenu(HINSTANCE hInstance, 
							UINT nIDMenu, CPoint point, 
							UINT nFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON,
							void* pData = NULL,
							int nSubMenu=-1, 
							CWnd* pParentWnd=NULL);
};
//主窗口状态条显示扩展
// AStatusBar

class RP_EXT_CLASS AStatusBar : public CMFCStatusBar
{
	DECLARE_DYNAMIC(AStatusBar)

public:
	AStatusBar();
	virtual ~AStatusBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	bool CreateBar(CWnd* pParent);
	BOOL SelectSizeUnit(CPoint point);
	void SetSizeUnitFlag();
	void InitShowFlags();

    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	// User message
	afx_msg LRESULT OnSysInfoUpdate(WPARAM wParam, LPARAM lParam); 
	virtual void DoPaint(CDC* pDC);
};

//MFC资源ID协调类
// 注：本类析构时会自动调用Unlock()恢复原始（Lock前）的状态，但程序员要养成手动添加Unlock的习惯
class RP_EXT_CLASS CResourceHandle
{
private:
	//原始资源ID所在模块句柄
	HINSTANCE m_hInstanceOld;

public:
	CResourceHandle(void);
	virtual ~CResourceHandle(void);

	//锁定资源操作句柄
	//注：重复调用时不影响原始句柄值，仅改变当前模块句柄
	void Lock(HINSTANCE hInstance);

	//解锁资源操作句柄
	void Unlock();
};

//功能概要: 用于窗口处理时额外的定制处理(系统状态转变), 主要是在并行刷新时,窗口的操作(如移动等)
//  自动和并行刷新同步,使CDialogPaint的派生类可不必考虑和系统的刷新同步,减少系统复杂度.
//  CDialogPaint和系统状态(全局变量g_Flags)有关
//  CDialogPaint是一个基类,原则上系统中的所有独立窗口都要从CDialogPaint派生,不然会存在刷新问题.
class RP_EXT_CLASS CDialogPaint
{
public:
	CDialogPaint();
	~CDialogPaint();
private:
	int		m_nTimerPaint;

	void	CustomPaintStart();
	void	CustomPaintEnd();

public:
	//*pnReturn = 0  : 消息未处理
	//*pnReturn < 0 : 消息已处理，一般在其后不再跟消息处理函数
	//*pnReturn > 0 : 消息已处理，一般在其后可再跟消息处理函数
	virtual LRESULT CustomPaintStart(UINT message, WPARAM wParam, LPARAM lParam, int* pnReturn=NULL);
	virtual LRESULT CustomPaintEnd(UINT message, WPARAM wParam, LPARAM lParam, int* pnReturn=NULL);
};

class RP_EXT_CLASS CAnDialog : public CDialogEx, public CDialogPaint
{
	DECLARE_DYNAMIC (CAnDialog);
public:
	CAnDialog();
	CAnDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CAnDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CAnDialog();

protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnDialog)
public:
	virtual int DoModal();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Implementation
public:
	// Generated message map functions
protected:
	//{{AFX_MSG(CAnDialog)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//功能概要: 系统视类,负责图象样稿显示的管理
class RP_EXT_CLASS AScrollView : public CScrollView, public CDialogPaint
{
protected:
	AScrollView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(AScrollView)

// Attributes
public:
	//得到View的屏幕坐标,即View坐标原点(花样的左上角,不是屏幕窗口)
	//	pPoint: [out]存放坐标值
	//注：系统中的View坐标,加上该坐标原点后即是基于花样的左上角的坐标,
	//		这时才可进行坐标转换(如ViewToFlower的内部实现),因系统中各种坐标系
	//		最终都基于花样的左上角(Flower原点在左下角,但从View坐标转换到Flower
	//		时仍然是从左上角开始的)
	void GetViewPosition(POINT* pPoint) const;

	//同GetViewPosition，只是返回的是X坐标
	int GetViewLeftPos() const;
	//同GetViewPosition，只是返回的是Y坐标
	int GetViewTopPos() const;

public:
	//得有效图像区域的左上角坐标 (Left, Top)
	void GetPaintLeftTop(POINT* pPoint) const;
	//得有效图像区域大小（Pixel）
	void GetPaintSize(SIZE* pSize) const;
	//得有效图像区域大小
	void GetPaintRect(RECT* pRect) const;

protected:
	POINT	m_ptPaint; // Paint's left-top in view
	SIZE	m_szPaint; // Paint's size in view

public:
	int		m_nReserved0;	//保留参数，可作为临时调试用
	int		m_nReserved1;
	double	m_dReserved0;
	double	m_dReserved1;

public:
	//滚动视窗到指定位置(逻辑坐标)
	//备注：
	//	有关定义参见MSDN中有关 CScrollView::ScrollToPosition 的说明
	virtual void AScrollToPosition(POINT pt);		     // replace ScrollToPosition 
	//滚动视窗到指定位置(设备坐标)
	virtual void AScrollToDevicePosition(POINT ptDev);   // replace ScrollToDevicePosition
	//视窗相对滚动一定距离
	//参数：
	//	xAmount / yAmount : x / y 方向的距离
	//备注：
	//	有关定义参见MSDN中有关 CWnd::ScrollWindow的说明
	virtual void AScrollWindow(int xAmount, int yAmount);// replace ScrollWindow
	
	//在 OnScroll 的重载函数中获得当前的滚动条位置
    //注：大文件操作，拉动滚动条进行定位时出错（View定位点超过 33000以上时）
	int OnScrollGetPostion(UINT nScrollCode, UINT nPos, BOOL bDoScroll); 

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AScrollView)
	protected:
	virtual void OnDraw(CDC* pDC);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~AScrollView();

	// Generated message map functions
	//{{AFX_MSG(AScrollView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//*pnReturn = 0  : 消息未处理
	//*pnReturn < 0 : 消息已处理，一般在其后不再跟消息处理函数
	//*pnReturn > 0 : 消息已处理，一般在其后可再跟消息处理函数
	LRESULT CustomPaintStart(UINT message, WPARAM wParam, LPARAM lParam, int* pnReturn=NULL);

public:
	//仅当 pWnd 是 AScrollView 的派生类时才有值，否则为 0x00010001
	// ZoomScale = HIWORD(m_ZoomScale) / LOWORD(m_ZoomScale)
	virtual DWORD ZoomScale() const;

protected:
	static AScrollView* m_pActiveView;	//系统当前激活View指针

public:
	//得当前激活View
	static AScrollView* GetActiveView();
	//设当前激活View
	//返回：前一次的激活View指针
	static AScrollView* SetActiveView(AScrollView* pView);

public:
	//虚屏显示
	enum VIEWTOVIRTUALTYPE { vvtNull=0, vvtVBitmapToView=0x01, vvtViewToVBitmap=0x02 };
	BOOL ViewToVBitmap() const;	
	BOOL ViewToVBitmap(BOOL bViewToVBitmap);	
	BOOL VBitmapToView() const;	
	BOOL VBitmapToView(BOOL bVBitmapToView);	

protected:
	int				m_nViewToVBitmap;	//虚屏状态, VIEWTOVIRTUALTYPE 的合集

public:
	//相关后台刷新线程（与花样数据读取相关）的允许调用标记
	//此标记一般仅在进行对图像数据独享（在处理里不须进行后台的其它操作功能）时进行设置，默认值为 normal
	enum BACKGROUND_PAINTFLAG
	{
		backpaint_disable   = 0x00,     //不允许所有的后台刷新功能

		backpaint_Display   = 0x01,     //允许花样叠加数据显示的刷新
		backpaint_Thumb     = 0x02,     //允许花回/层对话框中的小图标刷新
		backpaint_Navigation= 0x03,     //允许导航属性页中导航图刷新

		backpaint_normal    = 0xFF      //正常，允许所有数据的刷新
	};
	BYTE			m_bBackground_paintFlag;      //BACKGROUND_PAINTFLAG
};

//功能概要: 进度条类, 用于长时间操作时,在状态栏指示操作的进度. 一般和中途退出功能(按Esc)一起实现的
//用法 1:
//		1, 预调用			:	AProgress::GetStepSize(total_num,&nStep, &nStepSize);
//		2, 定  义			:	AProgress	pro(nStepSize);
//		3, 设置显示窗口		:	pro.SetPaintWnd(...)
//		4, 进度条标识字符串	:	strOld = pro.ProgressName(...);
//		5, 显示进度			:	for(..; nCount<..; nCount++) 
//								{ ...
//									if (pro.Step()==VK_ESCAPE)
//									{	...;  break;	}
//								}
//		6, 复位进度条标识	:	pro.ProgressName(strOld);
//		7, 结束进度显示		:	pro.End();
//
//用法 2 （用于计数值较大情况，一般超过 total_num > 2000 以上的可以考虑使用）:
//		1, 预调用			:	AProgress::GetStepSize(total_num,&nStep, &nStepSize);
//		2, 定  义			:	pro.Rang(nStepSize)
//		3, 进度条标识字符串	:	strOld = pro.ProgressName(...);
//		5, 显示进度			:	for(..; nCount<..; nCount++) 
//								{ ...
//									if ((nCount%nStep)==0 && pro.Step()==VK_ESCAPE)
//									{	...;  break;	}
//								}
//		6, 复位进度条标识	:	pro.ProgressName(strOld);
//		7, 结束进度显示		:	pro.End();
//
//参考：AStatusProgress
class RP_EXT_CLASS AProgress
{
public:
	// 构造函数。
	//	nEnd	:[in]结束时的指示绝对值。
	//	nStart	:[in]开始时的指示绝对值。
	AProgress(int nEnd=100, int nStart=0);
	virtual ~AProgress();

public:
	// 设置进度条显示位置
	//	pWnd	:[in]指定进度条所在窗口句柄
	//	prcPaint:[in]指定进度条在窗口中的显示区域，当为NULL时，表示使用整个窗口
	//
	//注：如果想在状态条位置进行进度显示，请使用g_progress进行操作
	BOOL SetPaintWnd(CWnd* pWnd, const RECT* prcPaint=NULL);

	// 设置范围
	void Range(int nEnd, int nStart=0);

	// 给定当前指示的绝对值
	//返回值：进度显示过程中的按键状态，同 WM_KEYDOWN 定义中的 msg.wParam
	WPARAM Current(int nPos);
	// 给定指示器走动步长的绝对值。
	void Step(int nStep);
	// 让指示器前进一步
	WPARAM Step() {return Current(m_nPos+m_nStep);}
	
	//当前操作类型名（显示在进度条中）
	CString ProgressName() const {return m_strProgressName; }
	//返回值：前一次的操作类型名
	CString ProgressName(const char *pszProgressName);

	//结束进度条操作
	void End();

public:	
	//当前是否有进度条操作
	//如果存在，则有关进度条位置上的操作都应该暂时停止（如显示当前内存状态）
	BOOL IsRuning();
	//更新刷新区域
	void UpdateRect(const RECT* prcPaint);
	//重刷新进度信息
	void Repaint();

public:
	//计算最优的进度条控制参数值
	//	nTotalSize 			：[in]总进度要求
	//	nStep、nStepCount	：[out]计算后的步长（用于取余计算）及进度条参数（总进度）
	//示例：
	// AProgress::GetStepSize(nTotalSize,nStep,nStepCount);
	// AProgress proc(nStepCount);
	// if ((nCurrentPos % nStep)==0 && proc.Step()==VK_ESCAPE) break;
	static void GetStepSize(int nTotalSize, int* nStep, int* nStepCount);

private:
	BOOL CreateMemDC();
	BOOL ReleaseMemDC();

	//	nCurrentPos :[in]进度值，<= _PROGRESS_PERCENT_SIZE_
	BOOL DrawProgressInfo(int nCurrentPos);

protected:
	CWnd*	m_pWnd;					//进度条所在位置
	CRect	m_rcPaint;				//进度显示区域
	int		m_nEnd, m_nStart;		//指定进度条显示范围
	int		m_nStep;				//进度步长
	CString m_strProgressName;		//当前操作类型名（显示在进度条中）

protected:
	int		m_nPos;					//当前进度值
	int		m_nCurrentOld;			//前一次进度值
	int		m_nLength;				//总进度值
	BOOL	m_bUp;					//是否反向计数
	HDC		m_hDC;					//显示用HDC

private:
	HDC			m_hMemDC;			//内存缓冲，进度显示过程： drawtext --> m_hMemDC(m_hMemBitmap) --> m_hDC
	HBITMAP		m_hMemBitmap;		//当前 Bitmap 信息（相对于 m_hMemDC）
	HBITMAP		m_hOldBitmap;		//保存上次的 Bitmap 信息
	HFONT		m_hOldFont;			//保存上次的 Font 信息

	friend class AStatusProgress;
};

//功能概要: 状态条位置的进度条类, 用于长时间操作时,在状态栏指示操作的进度. 一般和中途退出功能(按Esc)一起实现的
//用法:
//		1, 开始进度显示		:	bProgress = g_progress.Begin(bSubMode, pszProgressName);	必须有此部分代码
//		2, 定  义			:	if (bProgress) g_progress.Rang(nStepSize);
//		3, 显示进度			:	for(..; nCount<..; nCount++) 
//								{ ...
//									if (bProgress&& g_progress.Step()==VK_ESCAPE)	或使用 g_progress.Current(i) 指定绝对位置
//									{	...;  break;	}
//								}
//		4, 结束进度显示		:	if (bProgress) g_progress.End();			必须有此部分代码
//
class CStatusProgressSubInfo;
class RP_EXT_CLASS AStatusProgress
{
public:
	// 构造函数。
	AStatusProgress();
	virtual ~AStatusProgress();

public:
	//开始进度条操作
	//	bSubMode		:[in]是否为细分模式，在细分模式下，可以实现对已有进度条中某一个 nStep 区域进行细分进度显示
	//	pszProgressName	:[in]设置当前操作类型名（显示在进度条中），
	//						pszProgressName == NULL，则使用上一次的显示信息
	//						pszProgressName[0]=='\0'，则清空当前类型名
	//注：此操作将保存当前进度条信息，当End()时自动恢复
	BOOL Begin(BOOL bSubMode, const char *pszProgressName);
	//结束进度条操作
	//注：此操作将恢复 Begin时保存的上一次进度条信息
	void End();

	// 设置范围
	//	nEnd	:[in]结束时的指示绝对值。
	//	nStart	:[in]开始时的指示绝对值。
	void Range(int nEnd, int nStart=0);
	// 给定当前指示的绝对值
	//返回值：进度显示过程中的按键状态，同 WM_KEYDOWN 定义中的 msg.wParam
	WPARAM Current(int nPos);
	// 给定指示器走动步长的绝对值。
	void Step(int nStep);
	// 让指示器前进一步
	WPARAM Step();

public:	
	// 设置进度条显示位置
	//	pWnd	:[in]指定进度条所在窗口句柄
	//	prcPaint:[in]指定进度条在窗口中的显示区域，当为NULL时，表示使用整个窗口
	//
	//注：如果想在状态条位置进行进度显示，请使用g_progress进行操作
	BOOL SetPaintWnd(CWnd* pWnd, const RECT* prcPaint=NULL);
	//当前是否有进度条操作
	//如果存在，则有关进度条位置上的操作都应该暂时停止（如显示当前内存状态）
	BOOL IsRuning();
	//更新刷新区域
	void UpdateRect(const RECT* prcPaint);
	//重刷新进度信息
	void Repaint();

protected:
	//备份当前信息并初始化一个新的 SubInfo
	BOOL BackupProgressInfo();
	//恢复上一次操作信息，删除当前 SubInfo
	BOOL RestoreProgressInfo();

	//根据给定的进度值（绝对值，相对于 Range）得进度百分比值（(0.0-1.0)）
	//	bGlobal :[in]是否返回全局进度值，为FALSE时仅返回细分进度百分比值
	double GetPercent(int nPos, BOOL bGlobal) const;
	//根据给定的进度百分比值得进度条绝对值（相对于 m_progress.Range）
	//	bGlobal :[in]指定dPercent是否为全局进度值，FALSE表示为细分进度百分比值
	int GetPos(double dPercent, BOOL bGlobal) const;
	//根据给定的进度百分比值得进度条绝对值（相对于 Range）
	//	bGlobal :[in]指定dPercent是否为全局进度值，FALSE表示为细分进度百分比值
	int GetSubPos(double dPercent, BOOL bGlobal) const;

private:
	CStatusProgressSubInfo	*m_pSubInfo;	//细分操作数据缓冲，指向当前的操作数据
	AProgress				m_progress;		//进度条控制

};

class RP_EXT_CLASS APtrList : public CPtrList
{
public:
	APtrList(int nBlockSize = 10);
	~APtrList();
	
public:
	// If no find, return NULLINDEX.
	int FindPosition(POSITION pos) const; 
	//添加相应结点
	//	nIdxPos		:[in]指定插入位置，
	//				< 0		: 添加到表头
	//				> m_nCount : 添加到表尾
	//				other	:插到指定位置
	POSITION InsertAt(int nIdxPos, void* pNew);
};

//整形数值控件
class RP_EXT_CLASS CIntEdit : public CEdit
{
	DECLARE_DYNAMIC (CIntEdit);
// Construction
public:
	CIntEdit();

// Attributes
public:

// Operations
public:
	operator int() const;
	//是否允许输入“-”号
	void Negative(BOOL bEnable=TRUE);

	void SetRange (int nMin=INT_MIN, int nMax=INT_MAX);
	void GetRange (int &nMin, int &nMax) const;
	void Hexadecimal(BOOL bHex);
	void CheckValue(int nData, BOOL bForce, BOOL bMsg=TRUE);
	//设置输出时的字符个数
	//bLimit : 仅十六进制时有效，表示转换后的字符个数，为0时表示自动，最大为MAX_LIMITWIDTH
	void SetLimitWidth(BYTE bLimit);

	static int ToInt(CString number, BOOL bHex);
	// if return FALSE then error for lpszText with pData
	// lpszText== "-0"时 判断有误，但为求与 MFC 统一，不作修改
	static BOOL SimpleScanf(LPCTSTR lpszText, int& nData, BOOL bNegative, BOOL bHex);
	//输入字符串是否正确
	// bHex : 是否为十六进制输入值
	static BOOL IsStrFormatOK(LPCTSTR lpszText, BOOL bHex);
protected:
	BOOL	m_bNegative;	//是否允许输入“-”号
	int		m_nMin, m_nMax;
	BOOL	m_bHex;			//是否为十六进制输入值
	BYTE	m_bLimit;		//输入时的字符个数限制（仅十六进制方式有效）
	
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIntEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIntEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
class RP_EXT_API CDWORDEdit : public CEdit
{
	DECLARE_DYNAMIC (CDWORDEdit);
// Construction
public:
	CDWORDEdit();

// Attributes
public:

// Operations
public:
	operator DWORD() const;
	void SetRange (DWORD dwMin, DWORD dwMax=UINT_MAX);
	void GetRange (DWORD &dwMin, DWORD &dwMax) const;
	void Hexadecimal(BOOL bHex);
	void CheckValue(DWORD dwData, BOOL bForce, BOOL bMsg=TRUE);
	//设置输出时的字符个数
	//bLimit : 仅十六进制时有效，表示转换后的字符个数，为0时表示自动，最大为MAX_LIMITWIDTH
	void SetLimitWidth(BYTE bLimit);

	static DWORD ToDWORD(CString number, BOOL bHex);
	// if return FALSE then error for lpszText with pData
	// lpszText== "-0"时 判断有误，但为求与 MFC 统一，不作修改
	static BOOL SimpleScanf(LPCTSTR lpszText, DWORD &dwData, BOOL bHex);
	//当前输入字符串是否正确
	static BOOL IsStrFormatOK(LPCTSTR lpszText, BOOL bHex);
protected:
	DWORD	m_dwMin;
	DWORD	m_dwMax;
	BOOL	m_bHex;			//是否为十六进制输入值
	BYTE	m_bLimit;		//输入时的字符个数限制（仅十六进制方式有效）
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDWORDEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDWORDEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDWORDEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class RP_EXT_CLASS CFloatEdit : public CEdit
{
	DECLARE_DYNAMIC (CFloatEdit);
// Construction
public:
	CFloatEdit();

// Attributes
public:

// Operations
public:
	operator double() const;

	//求浮点数精度值，nPrecision:小数点位数，bRound : 是否四舍五入
	void SetPrecision (int nPrecision=COMMONPRECISION, BOOL bRound=TRUE);
	void GetPrecision (int &nPrecision, BOOL& bRound) const;

	void Negative(BOOL bEnable=TRUE);
	void SetRange (double dMin=DBL_MIN, double dMax=DBL_MAX);
	void GetRange (double &dMin, double &dMax) const;

	void CheckValue(double dData, BOOL bForce, BOOL bMsg=TRUE);

	static double ToDouble(CString number);
	// if return FALSE then error for lpszText with d
	// lpszText== "-0.0"时 判断有误，但为求与 MFC 统一，不作修改
	static BOOL SimpleFloatParse(LPCTSTR lpszText, void* pData, BOOL bNegative, BOOL bDouble);
	static BOOL IsStrFormatOK(LPCTSTR lpszText);

protected:
	BOOL	m_bNegative;		//是否允许输入“-”号
	double	m_dMin, m_dMax;

	int		m_nPrecision;		//小数点位数
	BOOL	m_bRound;			//是否允许四舍五入
protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFloatEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFloatEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#define  GRIDCELL_SET_ITEMVALUE		(WM_USER + 201)			//当参数输入完成（控件失去焦点或确认）后发送给主窗口
															//WPARAM	: MAKELONG(m_Row, m_Col)
															//LPARAM	: this->m_hWnd
#define  GRIDCELL_SET_ACTIVEITEM	(WM_USER + 202)			//当前选中荐改变后发送给主窗口
															//WPARM		: m_nSelect
															//LPARAM	: this->m_hWnd
									

//CListCtrl中自定义数值（浮点数）输入控件
class RP_EXT_CLASS CListCtrlEdit : public CEdit
{
	DECLARE_DYNAMIC (CListCtrlEdit);
public:
	CListCtrlEdit();
	~CListCtrlEdit();
public:
	//	rowIndex, colIndex :[in]列表框中的定位参数
	void Edit(int rowIndex,int colIndex);

	BOOL ExitWithoutSetValue(){ return m_bExitWithoutSetValue;}

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void EndEdit();		//操作完成后向父窗口发送 GRIDCELL_SET_ITEMVALUE 消息
	virtual void PostNcDestroy();
protected:
	//{{AFX_MSG(CListCtrlEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT    m_nLastChar;
	BOOL    m_bExitWithoutSetValue;
	int 	m_nRowIndex, m_nColIndex;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
//CListCtrl中自定义数值（浮点数）输入控件
class RP_EXT_CLASS CListCtrlFloatEdit : public CFloatEdit
{
	DECLARE_DYNAMIC (CListCtrlFloatEdit);
public:
	CListCtrlFloatEdit();
	~CListCtrlFloatEdit();
public:
	//	rowIndex, colIndex :[in]列表框中的定位参数
	void Edit(int rowIndex,int colIndex);

	BOOL ExitWithoutSetValue(){ return m_bExitWithoutSetValue;}

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void EndEdit();		//操作完成后向父窗口发送 GRIDCELL_SET_ITEMVALUE 消息
	virtual void PostNcDestroy();
protected:
	//{{AFX_MSG(CListCtrlFloatEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT    m_nLastChar;
	BOOL    m_bExitWithoutSetValue;
	int 	m_nRowIndex, m_nColIndex;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

//CListCtrl中自定义CComboBox控件
class RP_EXT_CLASS CListCtrlComboBox : public CComboBox
{
	DECLARE_DYNAMIC (CListCtrlComboBox);
public:
	CListCtrlComboBox();
	~CListCtrlComboBox();
public:
	//	rowIndex, colIndex :[in]列表框中的定位参数
	void Edit(int rIndex,int cIndex);
	BOOL ExitWithoutSetValue(){ return m_bExitWithoutSetValue;}

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void EndEdit();			//操作完成后向父窗口发送 GRIDCELL_SET_ITEMVALUE 消息
	virtual void PostNcDestroy();
protected:
	//{{AFX_MSG(CListCtrlComboBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT    m_nLastChar;
	BOOL    m_bExitWithoutSetValue;
	int 	m_Row,m_Col;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


//CSortListCtrl
//CListCtrl 扩展，实现可排序列表功能

//自定义排序函数
//	pszText		:[in]当前表项数据（字符串）
//	nSubItem	:[in]当前表项数据对应的列序号（起始值为 0）
//	strText		:[out]存放输出字符串，函数内部根据传入参数信息处理后返回此字符串以便与其它表项进行比较
typedef LRESULT (CALLBACK *CHECKITEMTEXTPROC)(CString &strText, const char *pszText, int nSubItem);
class RP_EXT_CLASS CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();

// Attributes
public:
	//[out]左键双击时的光标所在项
	struct DBL_LOCATION
	{
		int nItem;
		int nSubItem;
	};	
	DBL_LOCATION m_dblLoc;

protected:
	//排序用参数
	BOOL m_bRevert;
	int	m_nSortItem;
	BOOL m_bSortEnable;

	CHECKITEMTEXTPROC m_checkItemTextFunc;

public:
	//得随机的行标识数据
	static DWORD GetItemFlag();
	//得有效列数
	static int GetColumnCount(const CListCtrl *pListCtrl);
	//得第一个被选中行
	static int GetFirstSelected(const CListCtrl *pListCtrl);
	//查找指定字符串 （按行序-列序）
	//	pszText			:[in]字符串
	//	nStart			:[in]起始行序号（含），当 nStart < 0时，表示从第一个被选中的行开始查找
	//	bMatchWholeWord	:[in]匹配整个字符串
	//	bMatchCase		:[in]匹配大小写
	//返回：
	//	< 0	: 未找到指定字符串
	//	>= 0: 与字符串匹配的行序号（起始值为 0）
	//
	//注：本函数自动支持循环查找，即返回值 nIndex 有可能会小于 nStart，
	//	此时表示在 nStart 以前的行内有匹配的字符串存在。
	static int SearchTextFromListCtrl(const CListCtrl *pListCtrl, const char *pszText, 
										int nStart, BOOL bMatchWholeWord, BOOL bMatchCase);

	//设置排序参数
	void SetSortParam(BOOL bRevert, int nInitSortItem, CHECKITEMTEXTPROC checkItemTextFunc);
	//执行排序操作
	BOOL SortItemsEx();
	//设置行标识数据
	BOOL SetItemDataEx(int nItem);
	
	//允许排序
	BOOL IsSortEnable() const;
	BOOL SortEnable(BOOL bEnable);

	//有效列数
	int GetColumnCount() const;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSortListCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	BOOL GetIndex(CPoint point,int& nItem,int& nSubItem);

	friend int CALLBACK ListCtrlExCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
};

/////////////////////////////////////////////////////////////
// CUndoCell
enum DISCARDTYPE 
{
	dis_normal	= 0x0,	//普通，无特殊含义
	dis_imgdata	= 0x1,	//AImageData相关
	dis_image,			//AImage	相关
	dis_layer,			//ALayer	相关
	dis_flower,			//AFlower	相关
	dis_layout,			//ALayout	相关
	dis_node,
	dis_count
};
struct DISCARDDATA
{
	int		nType;	// DISCARDTYPE, Mask=0x0FFF
	void*	pData;	// for example: pImageData, pImage, pLayer, pFlower, pLayout, ..., decided by nType
};

//UNDO的最小单元，定义指定单步的UNDO操作
//	应用程序中通过对CUndoCell类的派生来实现自定义UNDO需求
class RP_EXT_CLASS CUndoCell : public CObject
{
	DECLARE_DYNCREATE(CUndoCell)
public:
	CUndoCell();
	virtual ~CUndoCell();

// Operators
public:
	//系统执行一次Undo操作，要求在此函数实现时考虑Redo数据的准备
	virtual void Undo() {}
	//当前Undo单元是否可以被丢弃
	virtual BOOL CanDiscard(DISCARDDATA* pDiscard);
	//得Undo缓冲文件名（当Undo数据被存放于磁盘文件时有效
	//返回值：m_uUnique
	virtual UINT GetTempFileName(LPTSTR szName);
	//当前Undo单元是否已经创建成功
	// Default is True;
	virtual BOOL CreateSuccessed() { return m_bSuccess; }
	//当前Undo单元占用的缓冲区大小
	virtual DWORD WorkSpace() const;

protected:
	PVOID		m_pDiscard;		//Undo单元所属类别，一般用于CanDiscard的判断
	UINT		m_uUnique;		// unique temp file name，同 GetTempFileName 返回值含义
	BOOL		m_bInFile;		// if m_bInFile then m_uUnique != 0
	BOOL		m_bSuccess;		//Undo单元是否创建成功
	DWORD		m_dwWorkSpace;	//Undo单元占用的缓冲区大小
	BOOL		m_bCanDiscard;	//Undo单元是否可被 Discard
};
/////////////////////////////////////////////////////////////
//Undo单元列表，通过它可实现一组Undo操作
//注：系统需要将一组Undo作为一次Undo来进行处理
class RP_EXT_CLASS CUndoObList : public CObList
{
	DECLARE_DYNCREATE(CUndoObList)
public:
	CUndoObList(int nBlockSize = 10);
	~CUndoObList();

	//Undo操作类型定义(Undo名，如："Create line...", "Erase..."...
	void SetUndoName(HINSTANCE hInstance, UINT uID);
	void SetUndoName(LPCSTR lpszUndoName);
	void GetUndoName(CString& strUndoName) const;
public:
	CString m_strUndoName;		//Undo操作类型名
	DWORD	m_dwWorkSpace;		//Undo单元列表占用的缓冲区大小

public:
	//Undo单元列表所占缓冲区大小
	DWORD WorkSpace() const;
	DWORD CalcWorkSpace();
};
typedef CTypedPtrList<CUndoObList, CObject*>	CUndoCellList;
typedef CTypedPtrList<CPtrList, CUndoCellList*>	CUndoList;
//Undo操作封装类，实现系统UNDO机制封装，系统可基于AUndo实现全局UNDO功能
//注：一般操作方式
//		NewSeries(...)
//		Backup(pCell)
//		NewSeries()
class RP_EXT_CLASS AUndo : public CObject
{
	DECLARE_DYNCREATE(AUndo)
public:
	AUndo();
	virtual ~AUndo();

// Operator
public:
	//Undo / Redo 实现
	void Undo();
	void Redo();

	//新建一次Undo操作(Undo单元列表）
	void NewSeries(LPCSTR lpszUndoName=NULL);
	
	//添加Undo单元
	BOOL Backup(CUndoCell* pCell);

	//丢弃（释放）指定的Undo数据
	void Discard(DISCARDDATA* pDiscard);

	//撤消当前新添加的Undo单元列表信息
	//注：在应用程序中，由于操作失败，需要撤消由于此操作而创建的Undo信息
	void RollBackCurList();

	//得最后一次的Undo单元数据
	const CUndoCell* GetLastUndo() const;

	//是否可以进行Undo/Redo操作
	BOOL CanUndo() const;
	BOOL CanRedo() const;
	//清空缓冲数据
	void Empty();
	
	//Undo次数限制
	// nOrder <= -1 means no limite
	void SetUndoOrder(int nOrder);
	int GetUndoOrder() const;
	BOOL IsUndoLimited() const;
	BOOL IsNeedUndo() const;
	//是否可创建Undo
	BOOL IsNotCreateUndo() const;

	//当前Undo操作类型名（Undo单元列表）
	BOOL GetUndoName (CString& strUndoName) const;
	BOOL GetRedoName (CString& strUndoName) const;
	BOOL SetUndoName (LPCSTR lpszUndoName=NULL);

	//系统Undo占用的缓冲区大小
	DWORD WorkSpace() const;

	//系统是否支持Undo操作
	BOOL IsBackuped() const;
	void IsBackuped(BOOL bBackuped);

	//限制进行Discard操作
	//	BeginDiscardLimit 与 EndDiscardLimit必须成对使用
	//返回值：前一次操作的Discard标识
	BOOL BeginDiscardLimit(BOOL bDiscardLimit = TRUE);
	void EndDiscardLimit(BOOL bDiscardLimit = FALSE);
	//设置自动清空标志
	//返回值：前一次操作的Emtpy标识
	BOOL SetEmptyFlag(BOOL bEmtpy);

	//得当前Undo单元列表
	const CUndoList* GetUndoList() const;

private:
	void DeleteTail(const POSITION& pos);
	void DeleteHead(const POSITION& pos);
	void DeleteHead();

private:
	int			m_nUndoOrder;		//Undo次数限制，< 0 表示无限制
	POSITION	m_UndoPos;			//当前Undo位置
	CUndoCellList*	m_pCellList;	//当前Undo单元列表
	CUndoList	m_UndoList;			//所有Undo数据（多个Undo单元列表）

	CString m_strUndoName;			//当前Undo操作类型名

	DWORD	m_dwWorkSpace;			//系统Undo所占用的缓冲区大小

	BOOL	m_IsBackup;				//是否允许Undo

	BOOL    m_bDiscardLimitFlag;	//限制进行Discard操作标识，当为TRUE时，Undo的Discard操作不执行
									//注：当Move层后再delete 层会造成两面二次进入Discard;
	BOOL    m_bEmptyFlag;			//自动清空Undo操作标识，当为TRUE时，表示添加新的Undo单元前自动清空所有的Undo数据
	
private:
	//Undo Limit flag: 
	//	now only for backup graph object flag, 
	//	if (m_dwBackupLimitFlags & bkGraph) then can't create graph undo.
	DWORD	m_dwBackupLimitFlags;
public:
	enum BACKUPLIMIT{ bkNull=0x0000, bkGraph=0x0001 };
	//是否可设置图形目标的恢复操作
	//操作后必须恢复原状态
	bool BackupGraphLimit(bool bBackupGraph);
	bool BackupGraphLimit() const;

};


//功能概要: 系统部分信息的存放路径, 如当前系统的执行文件路径,FMT路径等
class RP_EXT_CLASS CAppPath
{
public:
	CString	m_strApp;			// Applications path
	CString m_strFMT;			// ".\FMT\", file format path, rpFXXX.fmt
	CString m_strDrv;			// ".\DRV\", printer device drivers path, rpPXXX.drv
	CString m_strLib;			// ".\LIB\", rpLXXX.lib
	CString m_strRPT;			// ".\RPT\", tools plugin path, rpTXXX.rpt
	CString m_strProfile;		// ".\Profile\", 色彩校正曲线文件根目录
	CString m_strConfig;		// ".\Config\", 各种RIP参数（配置信息）存放目录
	CString m_strChart;			// "./Chart/"
	CString m_strTemplate;		// "./Template/"，平板机模板库存放目录

	CString m_strDebug;			// "./Debug/"，Debug信息存放目录

public:
	CString m_strProfile_Input_RGB;		// ".\Profile\Input\"，“输入RGB”曲线文件目录
	CString m_strProfile_Input_CMYK;	// ".\Profile\InputCMYK\"，“输入CMYK”曲线文件目录
	CString m_strProfile_Monitor;		// ".\Profile\Monitor\"，“显示”曲线文件目录
	CString m_strProfile_Output;		// ".\Profile\Output\"，“输出”（打印）曲线文件目录
	CString m_strProfile_strCurve;		// ".\Profile\Curve\", 密度曲线文件目录
	CString m_strProfile_strNameColor;	// ".\Profile\NameColor\",特殊颜色替换文件目录

public:
	CString m_strChart_ChartLine;	// "./Chart/ChartLine/"
	CString m_strChart_ChartICC;	// "./Chart/ChartICC/"
	CString m_strChart_ChartProof;	// "./Chart/ChartProof/"

public:
	BOOL Init(HINSTANCE hAppInst);
};

class RP_EXT_CLASS AEnvObj
{
public:
	AEnvObj();
public:
	int				m_nBitsPixel;	// Number of adjacent color bits for each pixel.
	int				m_nWidth;		// Width of the display (in pixels).
	int				m_nHeight;		// Height of the display (in pixels).
	int				m_nXRes;		// resolution of the display (dpi)
	int				m_nYRes;		// resolution of the display (dpi)

	int				m_nWidth_MM;	// Horizontal size in millimeters 
	int				m_nHeight_MM;	// Vertical size in millimeters

	OSVERSIONINFO	m_osVerInfo;	// Identifies the operating system version info

	SYSTEM_INFO		m_systemInfo;	// Contains information about the current computer system

	double			m_dXRes_Real;	// real resolution of the display (dpi)
	double			m_dYRes_Real;	// real resolution of the display (dpi)

	CString			m_strDeviceSN;	//加密锁信息

public:
	BOOL Init(BOOL bCheckDeviceSN);

	void CheckDeviceSN();
};

class RP_EXT_CLASS ACommonWnd
{
private:
	static void* m_pSplashHandle;	//启动画面

public:
	static BOOL SetSplashHandle(void *pSplashHandle);

	//显示Splash 窗口
	//	pParentWnd		:[in]父窗口句柄，为NULL时表示主窗串口
	//	pszAppVerion	:[in]软件版本信息
	//	pszLogoFileName	:[in]自定义的Logo图像文件名
	//	dwTimeOut		:[in]定义超时自动退出，为0时表示无超时，单位为秒
	//	bClickToExit	:[in]是否允许在窗体上点击退出
	//返回：
	//	相应的Splash操作句柄，必须由 SplashScreen_Exit进行释放，当为NULL时表示操作不成功
	//注：
	//	当 dwTimeOut 或 bClickToExit 有效时，返回值仅用于判断是否成功调用，
	//	但不能用于 SplashScreen_ProgressInfo 及 SplashScreen_Exit 中，相应条件满足时系统会自动析构相关资源
	static void* SplashScreen_Show(CWnd* pParentWnd, const char *pszAppVeriosn, const char *pszLogoFileName, DWORD dwTimeOut, BOOL bClickToExit);
	//显示相应的进度信息
	//	pSplashHandle	:[in]Splash操作句柄
	//	pszInfo			:[in]进度信息
	static BOOL SplashScreen_ProgressInfo(void* pSplashHandle, const char *pszInfo);
	//显示/隐藏窗口
	static BOOL SplashScreen_Show(void* pSplashHandle, BOOL bShow);
	//退出 Splash 窗口
	//	pSplashHandle	:[in]Splash操作句柄
	static BOOL SplashScreen_Exit(void* pSplashHandle);
};

//为速算要求，定义一些常量/常量表，程序可以调用，但不得进行更改
#define _ACV_DBFF_SIZE	(255*255+1)
class RP_EXT_CLASS ACV	//	AConstValue
{
public:
	enum LyChange {lyNew=1, lyAdd=2, lyDelete=4, lyActive=8, lyRename=16, lyLinked=32, lyThumbUpdated=64};
	enum AColorMuster {foreColor=0, backColor=1, protColor=2, tranColor=3, defColor};
public:
	static WORD	M[256][256];			//速算表，其中M[i][j] = i*j
	static BYTE Dbff[_ACV_DBFF_SIZE];	//速算表，其中Dbff[n] = (BYTE)(n/0xFF) 
public:
	static BOOL Init();
	static void Exit();
};
//功能概要: 消息处理函数,功能用Windows的相应函数
//  AMsgSend实际上就是调用Windows的消息函数实现的,写AMsgSend的目的是在处理消息时,可增加一些自己的额外处理
class RP_EXT_CLASS AMsgSend
{
public:
	// Send to MainFrame
	//
	//发送消息到主窗口并等待处理完成,参数同SDK中的 SendMessage函数
	static LRESULT ToMain(UINT message, WPARAM wParam=0, LPARAM lParam=0);
	//发送消息到主窗口并直接返回（不必等主窗口处理完成）,参数同SDK中的 PostMessage函数
	static BOOL PostToMain(UINT message, WPARAM wParam=0, LPARAM lParam=0);
};


//定义与墨水（颜料）相关的数据结构
#include "../include/ColorInfo.h"


//文件操作类封装
#ifndef BIF_USENEWUI
	#define	BIF_NEWDIALOGSTYLE	0x0040   
	#define	BIF_USENEWUI		(BIF_NEWDIALOGSTYLE | BIF_EDITBOX) 
#endif
class RP_EXT_CLASS ADocObj : public CObject
{
protected:
	ADocObj();
	DECLARE_DYNCREATE(ADocObj)

public:
	~ADocObj();

protected:
	//文件操作时的错误信息处理
	//	bPrompt :[in]指定操作方式
	//			TRUE ： 显示对话框
	//			FALSE：仅当错误信息添加到 g_Error 中
	static void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault, BOOL bPrompt);

public:
	//删除指定文件
	//	bDelete		: [in]是否真正的删除文件，
	//		为FALSE时表示用	g_Flags.bDirectDelete 参数来决定是否直接删除文件(文件替换时)
	//	bMultifiles	: [in]是否指定多文件名
	//		当为TRUE时，每个文件名必须以一个'\0'结束，字符串最后应以两个 '\0'(即 "\0\0")结束
	//		具体定义可参见 SHFILEOPSTRUCT 定义
	static BOOL Remove(LPCTSTR lpszFileName, BOOL bDelete, BOOL bMultifiles=FALSE);
	//文件重命名
	//	lpszFileNameSrc --> lpszFileNameDst
	//	bDelete		: [in]在lpszFileNameDst 已存在时有效，含义同上
	static BOOL Rename (LPCTSTR lpszFileNameSrc, LPCTSTR lpszFileNameDst, BOOL bDelete);
	//创建指定目录
	//	lpszPath	:[in]指定要创建的目录名
	//备注：
	//	当目录已经存在时，返回值仍为TRUE
	static BOOL CreateDirectory(LPCTSTR lpszPath);
	//删除指定目录
	//	lpszPath	:[in]指定要删除的目录名
	//备注：
	//	当目录不存在时，返回值仍为TRUE
	static BOOL DeleteDirectory(LPCTSTR lpszPath);
	//通过文件名得目录名
	//	strPathName	:[out]存放输出的目录
	//	lpszFileName:[in]指定文件名
	static BOOL GetDirectory(CString& strPathName, LPCTSTR lpszFileName);
	//打开文件进行读操作
	//	lpszPathName:[in]文件名
	//	bPrompt		:[in]出错时是否有消息对话框弹出
	static BOOL OpenReadFile(LPCTSTR lpszPathName, CFile& file, BOOL bPrompt=TRUE); 
	//打开文件进行读写操作
	//	lpszPathName:[in]文件名
	//	bPrompt		:[in]出错时是否有消息对话框弹出
	static BOOL OpenWriteFile(LPCTSTR lpszPathName, CFile& file, BOOL bPrompt=TRUE); 
	//关闭文件操作
	//注：与 OpenXXXFile对应，同CFile::Close()功能
	static void CloseFile(CFile& file);

	//	IDPDefault	:[in]默认的出错信息ID，AFX_ 起始（如 AFX_IDP_FAILED_INVALID_PATH，afxres.h）
	//得文件操作时的错误信息
	static CString GetSaveLoadExceptionInfo(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault);
	//显示文件操作时的错误信息（对话框）
	static void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault);

	// 当前操作系统能否打开相应宽高的文件
	// int nWidth, nHeight : Width and Height of image.
	// int nPrompt : 0 : no prompt, 1: prompt on status, 2 : prompt on dialog
	static BOOL CanOpenFile(int nWidth, int nHeight, int nPrompt=0);

	//分解文件名
	// 	path	:[in]Full path. 
	// 	drive	:[out]Optional drive letter, followed by a colon (:). 
	// 	dir		:[out]Optional directory path, including trailing slash. Forward slashes ( / ), backslashes ( \ ), or both may be used. 
	// 	fname	:[out]Optional base filename (no extension). 
	// 	ext		:[out]Optional filename extension, including leading period (.). 
	//注：参考 _splitpath_s 函数
	static BOOL SplitPath(const char *path, CString *drive, CString *dir, CString *fname, CString *ext);

	//文件复制
	//	pszDstFileName	:[in]目标文件名，如果为目录的话，请以"\\"结尾
	//	pszSrcFileName	:[in]源文件名，必须为有效的单个文件
	//	bAutoRename		:[in]当目标位置已存在同名文件时，是否自动更名
	//	bPrompt		:[in]出错时是否有消息对话框弹出
	static BOOL CopyFile(CString &strDstFileName, const char *pszSrcFileName, BOOL bAutoRename, BOOL bPrompt);

	// 启用某个账户对某个文件(夹)或注册表项的操作权限
	//	pszAccount			:[in]账户名称，如 Users
	//	ObjectType			:[in]目标类型，SE_OBJECT_TYPE
	//							SE_FILE_OBJECT			文件操作
	//							SE_REGISTRY_KEY			注册表
	//							SE_REGISTRY_WOW64_32KEY	32位软件在64位系统上的注册表项
	//	pszPath				:[in]文件(夹)路径或注册表项名，如 C:\Temp\，
	// 							SE_FILE_OBJECT 
	// 								Indicates a file or directory. The name string that identifies a file or directory object can be in one of the following formats:
	// 								*  A relative path, such as FileName.dat or ..\FileName
	// 								*  An absolute path, such as FileName.dat, C:\DirectoryName\FileName.dat, or G:\RemoteDirectoryName\FileName.dat.
	// 								*  A UNC name, such as \\ComputerName\ShareName\FileName.dat.
	// 							SE_REGISTRY_KEY 
	// 								Indicates a registry key. A registry key object can be in the local registry, such as CLASSES_ROOT\SomePath or in a remote registry, such as \\ComputerName\CLASSES_ROOT\SomePath.
	// 								The names of registry keys must use the following literal strings to identify the predefined registry keys: "CLASSES_ROOT", "CURRENT_USER", "MACHINE", and "USERS".
	// 							SE_REGISTRY_WOW64_32KEY 
	// 								Indicates an object for a registry entry under WOW64.
	//	AccessPermissions	:[in]访问权限
	// 							GENERIC_READ
	// 							GENERIC_WRITE
	// 							GENERIC_EXECUTE
	// 							GENERIC_ALL
	static BOOL EnableAccountPrivilege (PCTSTR pszAccount, SE_OBJECT_TYPE ObjectType, PCTSTR pszPath, DWORD AccessPermissions=GENERIC_READ |GENERIC_WRITE |GENERIC_EXECUTE);
public:
	//系统缓存路径
	//以下函数参数及返回值与 GetTempPath(...)、GetTempFileName (...) 同
	static DWORD GetSysTempPath(int nBufferLength, LPTSTR lpBuffer);
	static UINT GetSysTempFileName (LPTSTR szTmpName, LPCTSTR lpPrefixString, UINT uUnique);

public:
	//选择文件夹
	//	strPath		:[out]当前选择的文件夹，以'\'结束，如“C:\”，“C:\Temp\”
	//	pParentWnd	:[in]操作窗口
	//	pszDirectory:[in]初始文件夹
	//	pszTitle	:[in]对话框标题
	//	ulFlag		:[in]对话框操作属性，如 BIF_NEWDIALOGSTYLE
	static BOOL GetSpecialFolder(CString &strPath, HWND pParentWnd, const char *pszDirectory, const char *pszTitle, UINT ulFlags);

	//以下定义仅为后续扩展需要
public:
	virtual const CString& GetPathName() const;
	virtual void SetPathName(LPCTSTR lpszPathName);
	virtual BOOL IsModified() const;
	virtual void SetModifiedFlag(BOOL bModified = TRUE);

protected:
	CString		m_strPathName;		//文件名（包括全路径信息）
	BOOL		m_bModified;		//是否已经编辑过了（未保存）
};


//预定义缓冲区管理类
//注：通过本类可以预定义一个全局（或局部）缓冲区，并通过它进行缓冲区申请、释放等操作
class RP_EXT_CLASS CBufferManager
{
public:
	CBufferManager();
	virtual ~CBufferManager();
	
private:
	//预定义缓冲区m_pBuffer大小，初始大小由 InitBuffer() 指定;
	DWORD	m_dwBufferSize;
	
	//预定义缓冲区地址: 通过LockBuffer调用,通过m_bUseFlag（m_bufferCritical）判断是否已经被使用
	LPBYTE	m_pBuffer;
	
	//临界操作，用于确保不被同进程的不同线程同时调用
	CRITICAL_SECTION	m_bufferCritical;	
	//缓冲区 m_pBuffer 使用标记，配合m_bufferCritical使用
	//	用于确保不被同线程的不同函数同时调用
	BOOL	m_bUseFlag;
	
public:
	//	pdwDstSize	:[out]实际可使用缓冲区的大小（可以为NULL），有可能不等于（大于/小于）所需缓冲区大小
	//	dwSize		:[in]初始化预定义缓冲区大小，当为0时表示释放当前缓冲区空间
	//返回值：当前操作是否成功
	//注：无论返回值是否为TRUE，*pdwDstSize均存放实际可操作的缓冲区大小
	BOOL InitBuffer(DWORD *pdwDstSize, DWORD dwSize);
	//	dwSize		:[in]需要申请缓冲区大小
	//	Alloc	:[in]是否进行缓冲区额外申请，
	//返回值：缓冲区首址，为NULL时表示操作不成功
	//	如dwSize小于等于预定义缓冲区的大小，并且预定义缓冲区没有被其他地方使用，返回预定义缓冲区的指针，
	//	否则根据bAlloc的参数来确定以下两种处理方式:
	//		bAlloc==FALSE	：返回NULL，此时则需由用户自己申请所需的内存
	//		bAlloc==TRUE	：当预定义缓冲区无法锁定时，函数强制申请额外的缓冲区并返回，否则返回NULL
	//注：当 LockBuffer 返回值不为NULL的情况下，在使用完缓冲区后，一定要调用 UnlockBuffer() 函数进行资源释放
	LPBYTE LockBuffer(DWORD dwSize, BOOL bAlloc);
	
	//释放由用户占用的预定义缓冲区,以便可以让其他地方使用;
	//注：在使用 LockBuffer 函数,并且返回不为NULL的情况下,在使用完缓冲区后,一定要调用此函数
	BOOL UnlockBuffer(LPBYTE pBuffer);
	
	//得到缓冲区的长度
	DWORD GetBufferLength() const;
	
};
//缓冲区操作（申请、释放）封装类
typedef CMap<DWORD, DWORD, int, int&> CDWORDintMap;
class RP_EXT_CLASS AGV : public CBufferManager	//	AGlobalValue
{
public:
	AGV();
	virtual ~AGV();
	
protected:
	//以下Init()/Exit()仅涉及 static 变量的操作
	static BOOL Init();
	static void Exit();

	friend class CrpBaseApp;

public:
	//以下个函数功能分别对应各自的API函数,是为了便于内存管理而做,
	//在应用程序中不能直接使用有关内存操作的API函数,应使用相映的AGV函数的#define定义
	//使用方式: AGV::A*******(参数列表);
	static HGLOBAL _RAGlobalAlloc(UINT uFlags,DWORD dwBytes);
	static HGLOBAL _RAGlobalReAlloc(HGLOBAL hMem,DWORD dwBytes,UINT uFlags);
	static HGLOBAL _RAGlobalFree(HGLOBAL hMem);
	

	static LPVOID _RAGlobalAllocPtr(UINT uFlags, DWORD dwBytes);
	static LPVOID _RAGlobalReAllocPtr(LPCVOID lp, DWORD dwNewBytes, UINT uFlags);
	static BOOL _RAGlobalFreePtr(LPCVOID lp);
	
	static LPVOID _RAGlobalLock(HGLOBAL hMem);
	static BOOL _RAGlobalUnlock(HGLOBAL hMem);
	
	static LPVOID _RAVirtualAlloc(LPVOID lpAddress,DWORD dwSize,DWORD flAllocationType,DWORD flProtect);
	static BOOL _RAVirtualFree(LPVOID lpAddress,DWORD dwSize,DWORD dwFreeType);
	
	static BOOL _RAVirtualLock(LPVOID lpAddress,DWORD dwSize);
	static BOOL _RAVirtualUnlock(LPVOID lpAddress,DWORD dwSize);
	
public:
	//注意:修改以下各函数也应修改对应的非DEBUG版本
	static HGLOBAL _DAGlobalAlloc(UINT uFlags,DWORD dwBytes,LPCSTR lpszFileName, int nLine);
	static HGLOBAL _DAGlobalReAlloc(HGLOBAL hMem,DWORD dwBytes,UINT uFlags,LPCSTR lpszFileName, int nLine);
	static HGLOBAL _DAGlobalFree(HGLOBAL hMem,LPCSTR lpszFileName, int nLine);
	
	static LPVOID _DAGlobalAllocPtr(UINT uFlags, DWORD dwBytes,LPCSTR lpszFileName, int nLine);
	static LPVOID _DAGlobalReAllocPtr(LPCVOID lp, DWORD dwNewBytes, UINT uFlags,LPCSTR lpszFileName, int nLine);
	static BOOL _DAGlobalFreePtr(LPCVOID lp,LPCSTR lpszFileName, int nLine);
	
	static LPVOID _DAGlobalLock(HGLOBAL hMem,LPCSTR lpszFileName, int nLine);
	static BOOL _DAGlobalUnlock(HGLOBAL hMem,LPCSTR lpszFileName, int nLine);
	
	static LPVOID _DAVirtualAlloc(LPVOID lpAddress,DWORD dwSize,DWORD flAllocationType,DWORD flProtect,LPCSTR lpszFileName, int nLine);
	static BOOL _DAVirtualFree(LPVOID lpAddress,DWORD dwSize,DWORD dwFreeType,LPCSTR lpszFileName, int nLine);
	
	static BOOL _DAVirtualLock(LPVOID lpAddress,DWORD dwSize,LPCSTR lpszFileName, int nLine);
	static BOOL _DAVirtualUnlock(LPVOID lpAddress,DWORD dwSize,LPCSTR lpszFileName, int nLine);

private:
	static CDWORDintMap	m_VAAdrIndexMap;
	static CDWORDintMap	m_GAAdrIndexMap;
	static CStringArray	m_strVAArray;
	static CStringArray	m_strGAArray;

private:
	static void AddGlobalAllocInfo(DWORD dwAddress, LPCSTR lpszFileName, int nLine);
	static void DeleteGlobalAllocInfo(DWORD dwAddress, LPCSTR lpszFileName, int nLine);
	static void AddVirtualAllocInfo(DWORD dwAddress, LPCSTR lpszFileName, int nLine);
	static void DeleteVirtualAllocInfo(DWORD dwAddress, LPCSTR lpszFileName, int nLine);
};

#ifdef _DEBUG
#define AGlobalAlloc(uFlags,dwBytes)			_DAGlobalAlloc(uFlags,dwBytes,THIS_FILE, __LINE__)
#define AGlobalReAlloc(hMem,dwBytes,uFlags)		_DAGlobalReAlloc(hMem,dwBytes,uFlags,THIS_FILE, __LINE__)
#define AGlobalFree(hMem)						_DAGlobalFree(hMem,THIS_FILE, __LINE__)

#define AGlobalAllocPtr(uFlags,dwBytes)			_DAGlobalAllocPtr(uFlags,dwBytes,THIS_FILE, __LINE__)
#define AGlobalReAllocPtr(lp,dwNewBytes,uFlags)	_DAGlobalReAllocPtr(lp,dwNewBytes,uFlags,THIS_FILE, __LINE__)
#define AGlobalFreePtr(lp)						_DAGlobalFreePtr(lp,THIS_FILE, __LINE__)

#define AGlobalLock(hMem)						_DAGlobalLock(hMem,THIS_FILE, __LINE__)
#define AGlobalUnlock(hMem)						_DAGlobalUnlock(hMem,THIS_FILE, __LINE__)

#define AVirtualAlloc(lpAddress,dwSize,flAllocationType,flProtect)	_DAVirtualAlloc(lpAddress,dwSize,flAllocationType,flProtect,THIS_FILE, __LINE__)
#define AVirtualFree(lpAddress,dwSize,dwFreeType)					_DAVirtualFree(lpAddress,dwSize,dwFreeType,THIS_FILE, __LINE__)

#define AVirtualLock(lpAddress,dwSize)								_DAVirtualLock(lpAddress,dwSize,THIS_FILE, __LINE__)
#define AVirtualUnlock(lpAddress,dwSize)							_DAVirtualUnlock(lpAddress,dwSize,THIS_FILE, __LINE__)

#else
#define AGlobalAlloc(uFlags,dwBytes)			_RAGlobalAlloc(uFlags,dwBytes)
#define AGlobalReAlloc(hMem,dwBytes,uFlags)		_RAGlobalReAlloc(hMem,dwBytes,uFlags)
#define AGlobalFree(hMem)						_RAGlobalFree(hMem)

#define AGlobalAllocPtr(uFlags,dwBytes)			_RAGlobalAllocPtr(uFlags,dwBytes)
#define AGlobalReAllocPtr(lp,dwNewBytes,uFlags)	_RAGlobalReAllocPtr(lp,dwNewBytes,uFlags)
#define AGlobalFreePtr(lp)						_RAGlobalFreePtr(lp)

#define AGlobalLock(hMem)						_RAGlobalLock(hMem)
#define AGlobalUnlock(hMem)						_RAGlobalUnlock(hMem)

#define AVirtualAlloc(lpAddress,dwSize,flAllocationType,flProtect)	_RAVirtualAlloc(lpAddress,dwSize,flAllocationType,flProtect)
#define AVirtualFree(lpAddress,dwSize,dwFreeType)					_RAVirtualFree(lpAddress,dwSize,dwFreeType)

#define AVirtualLock(lpAddress,dwSize)								_RAVirtualLock(lpAddress,dwSize)
#define AVirtualUnlock(lpAddress,dwSize)							_RAVirtualUnlock(lpAddress,dwSize)
#endif


class RP_EXT_CLASS ACompress
{
private:
	static BYTE		s_BUF[256*8];
	static LPBYTE	s_Loc[128];
protected:
	static BOOL Init();
	static void Exit();
	friend class CrpBaseApp;

public:
	//----- MemMode(UnPackMode) to/from FileMode(BMP) ---------
	//	紧凑格式压缩
	//注：一般用于BMP图像保存，仅适用于 nBitCount < 8模式（1,2,4）
	//
	//紧凑格式压缩
	//	pData		:[in/out]存放原始图像数据（未压缩，1个字节对应1个像素）
	//					当 pFileData==NULL时，也用于存放压缩后的数据
	//	dwReadStart	:[in]起始位置（Pixel)，[0, 8/nBitCount)
	//	dwWidth		:[in]原始图像数据长度（像素点个数）
	//	nBitCount	:[in]原始图像数据比特数，仅限于1,2,4
	//	pFileData	:[out]存放压缩后的图像数据，当为NULL时，压缩后数据存放于 pData中
	//注：当nBitCount时，函数直接返回TRUE，pFileData数据无效
	static BOOL ShrinkBit(BYTE* pData, DWORD dwReadStart, DWORD dwWidth, int nBitCount, BYTE* pFileData=NULL);
	//紧凑格式解压缩
	//	pFileData	:[in]原始紧凑格式图像数据
	//	dwWriteStart:[in]起始位置（Pixel)
	//	dwWidth		:[in]原始紧凑格式图像数据长度（像素点个数）	
	//	nBitCount	:[in]原始图像数据比特数，仅限于1,2,4
	//	pData		:[out]存放解压缩后的图像数据
	static BOOL ExpandBit(const BYTE* pFileData, DWORD dwWriteStart, DWORD dwWidth, int nBitCount, BYTE* pData);

	//----- MemMode(UnPackMode) Pack to/from MemMode(PackMode)---------
	// 压缩：[pData, dwData] --> [pComp, dwComp]
	// 解压：[pComp, dwComp] --> [pData, dwData]
	//压缩：
	//	nBitCount		:[in]指定图像比特数
	//	pData, dwData	:[in]指定原始图像数据及长度（字节数）
	//	pComp, dwComp	:[out]指定目标图像数据及长度（字节数）
	//	dwUp			:[in]指定目标缓冲区最大尺寸（字节数）
	static BOOL Pack(const BYTE* pData, DWORD dwData, int nBitCount, BYTE* pComp, DWORD dwUp, DWORD& dwComp);
	//解压缩：
	//	nBitCount		:[in]指定图像比特数
	//	pComp, dwComp	:[in]指定原始压缩图像数据及长度（字节数）
	//	pData			:[out]指定解压缩后的目标图像数据缓冲区
	//	dwWidth			:[in]指定目标图像数据大小（字节数）
	//注：正常的压缩数据解压缩后的数据大小 == dwWidth
	static BOOL UnPack(const BYTE* pComp, DWORD dwComp,DWORD dwWidth, int nBitCount,BYTE* pData);
	//解压缩指定区域的数据
	//	nBitCount		:[in]指定图像比特数
	//	pComp, dwComp	:[in]指定原始压缩图像数据及长度（字节数）
	//	pData			:[out]指定解压缩后的目标图像数据缓冲区
	//	dwWidth			:[in]指定目标图像缓冲区大小（像素点）
	//	Start, Length	:[in]Pixel，指定所需处理的图像数据区域，Start+Length <= dwWidth
	static BOOL UnPack(const BYTE* pComp, DWORD dwComp, 
							 DWORD dwWidth/*pixel*/, int nBitCount,
							 DWORD Start/*pixel*/, DWORD Length/*pixel*/,
							 BYTE* pData);

public:	// Block Data Compress
	//-- this fund is used in Image's Expand(Block Compress Mode)-----
	//-- szBlock	: size of Block
	//-- posInBlock	: start point'coord in szBlock;
	//-- RECT(posInData,szMap): zoomscale rect(即考虑ZoomScale), pXMap and pYMap are datas of ZoomScale
	//-- while pXMap and pYMap are NULL, noZoomScale(1:1), and szMap is size of UnPack rgn
	//		at this time, RECT(posInBlock,szMap) is also UnPack rect.
	//---* all is pixel unit ( except nWidthBytes, pComp and pData) *----//
	static BOOL UnPack( const SIZE* szBlock, int nBitCount,
		const BYTE* pComp, DWORD dwComp, 
		BYTE* pData, int nRowBytes,
		const POINT* posInBlock, const SIZE* szMap,
		const DWORD* pXMap, const DWORD* pYMap);
	
	static bool UnPackFlagBits( const SIZE *szBlock, BYTE BitCount, 
		const BYTE* pComp, DWORD dwComp, 
		const BYTE* pFlagVal, int nFlagVal, BYTE validVal,
		BYTE* pBits, DWORD rowBytes,
		const POINT* posInBlock,const SIZE* szInBlock);
	
	//计算指定缓冲区中 *pVal 所点的个数（像素点数）
	//注：主要用于计算黑白比
	static DWORD CalcMono(BYTE BitCount, int nCompMode, int nWidth, int nHeight, 
		const BYTE* pBits, DWORD dwBits, const BYTE* pVal);

	
public:
	// 压缩：[pData, dwData] --> [pComp, dwComp]
	// 解压：[pComp, dwComp] --> [pData, dwData]
	
	// LZW Compress
	static BOOL Comp_LZW (const BYTE* pData, DWORD dwData, BYTE* pComp, DWORD BufSize, DWORD& dwComp);
	static BOOL UnComp_LZW(const BYTE* pComp, DWORD dwComp, BYTE* pData, DWORD BufSize, DWORD& dwData);
	
	// Compress/UnCompress one line
	static BOOL Comp_RLE(const BYTE* pData, DWORD dwData, BYTE* pComp, DWORD BufSize, DWORD& dwComp);
	static BOOL UnComp_RLE(const BYTE* pComp, DWORD dwComp, BYTE* pData, DWORD BufSize, DWORD& dwData);
	
	// LZW Compress(Private)
	static BOOL Pack_LZW(const BYTE* pData, DWORD dwData, WORD* pwComp, DWORD BufSize, DWORD& dwComp);
	static BOOL UnPack_LZW(const WORD* pwComp, DWORD dwCount, BYTE* pData, DWORD BufSize, DWORD& dwData);
	
	// LZW Compress --> LZW Compress(Private)
	// pwComp's size is equal to dwSize
	static BOOL ResumeDataBit(const BYTE* pBuf, DWORD dwSize, WORD* pwComp, DWORD& dwCount);
	// LZW Compress(Private) --> LZW Compress
	static BOOL TransformDataBits(const WORD* pwComp, DWORD dwComp, BYTE* pData, DWORD BufSize, DWORD& dwData);
	
public:
	// 初始化一行数据
	//	nWidth		：[in]长度（像素点）
	//	wBitCout	：[in]比特数
	//	bCompress	：[in]初始化为压缩行
	//	pValue		：[in]初始化值
	//	pDstBits	：[out]由调用者申请,并由调用者释放
	// 返回值：==0 时函数调用失败，否则当初始化压缩行时为pDstBits字节数，否则为nWidth
	static DWORD InitSpaceBits (LPBYTE pDstBits, int nWidth, WORD wBitCount, 
								const BYTE* pValue, BOOL bCompress);
	
	//图像数据简单缩放操作
	//	wBitCount	:[in]比特数
	//	pData		:[in]原始图像数据
	//	nWidthPre, nHeightPre	:[in]原始图像大小
	//	pBits		:[out]目标图像数据缓冲区
	//	nWidth, nHeight			:[in]目标图像大小
	static BOOL ZoomBits(int nWidthPre, int nHeightPre, WORD wBitCount, const BYTE* pData, 
						int nWidth, int nHeight, BYTE* pBits);

	//根据圆整映射表进行图像数据缩放操作
	//	nBitCount	:[in]比特数
	//	pSrcBits, nWidthOld	:[in]指定原始图像数据及其宽度
	//	pDstBits, nWidthNew	:[in]指定目标图像数据缓冲区及其宽度
	//	pMap		:[in]指定nWidthNew-->nWidthOld的映射表，当为NULL时，函数内部自动生成
	//					参考 AMathOpt::ResampleLineMap(nWidthNew, nWidthOld, pMap) 函数
	//注：pSrcBits != pBitDst
	static BOOL ResampleLine(const BYTE* pSrcBits, int nBitCount, int nWidthOld,
							BYTE* pDstBits, int nWidthNew, const DWORD* pMap);

protected:
	// return TRUE:	Str In List. or not in List.
	static BOOL IsStrInList(WORD preCode,BYTE valid, const BYTE* pList, WORD StartFind, WORD* Code, WORD* pEndFind);

protected:
	static BOOL Pack_Value(BYTE Value, DWORD dwCount, int nBitCount,BYTE* pComp/*length=10*/, DWORD& dwComp);
	//for 1 Bits
	static BOOL ShrinkBit1(const BYTE* pRead, DWORD dwReadStart,DWORD ReadLength, BYTE* pWrite);
	static BOOL ExpandBit1(const BYTE* pRead, BYTE* pWrite, DWORD dwWriteStart,DWORD WriteLength);
	static BOOL Pack1(const BYTE* pData, DWORD dwData, BYTE* pComp, DWORD dwUp, DWORD& dwComp);
	static BOOL UnPack1(const BYTE* pComp, DWORD dwComp,DWORD dwWidth, BYTE* pData);
	static DWORD UnPackSeg1(const BYTE* pComp, DWORD dwStart, DWORD dwLength, BYTE& val,DWORD& dwCount, BOOL bForward);
	static BOOL GetLoc_RLE1(DWORD FindLoc, DWORD dwWidth, const BYTE* pComp, DWORD dwComp,
							 DWORD* Start_Comp=NULL, DWORD* Comp_Length=NULL, 
							 DWORD* sub_loc=NULL, DWORD* dwCount=NULL);
	static bool UnPackFlagBits1( const SIZE* szBlock, const BYTE* pComp, DWORD dwComp, 
		BYTE flagVal, BYTE validVal, BYTE*	pData, int nRowBytes,
		const POINT* posInBlock,const SIZE* szInBlock);

	//for 2 Bits
	static BOOL ExpandBit2(const BYTE* pRead, BYTE* pWrite, DWORD dwWriteStart,DWORD WriteLength);
	static BOOL ShrinkBit2(const BYTE* pRead, DWORD dwReadStart,DWORD ReadLength, BYTE* pWrite);
	static BOOL UnPack2(const BYTE* pComp, DWORD dwComp, DWORD dwWidth,BYTE* pData);
	static DWORD UnPackSeg2(const BYTE* pComp, DWORD dwStart, DWORD dwLength, BYTE& val,DWORD& dwCount, BOOL bForward);
	static BOOL Pack2(const BYTE* pData, DWORD dwData, BYTE* pComp, DWORD dwUp, DWORD& dwComp);
	static BOOL GetLoc_RLE2(DWORD FindLoc, DWORD dwWidth, const BYTE* pComp, DWORD dwComp,
							 DWORD* Start_Comp, DWORD* Comp_Length, 
							 DWORD* sub_loc, DWORD* dwCount);

	//for 4 Bits
	static BOOL ExpandBit4(const BYTE* pRead, BYTE* pWrite, DWORD dwWriteStart,DWORD WriteLength);
	static BOOL ShrinkBit4(const BYTE* pRead, DWORD dwReadStart,DWORD ReadLength, BYTE* pWrite);
	static BOOL Comp_RLE4(const BYTE* pData, DWORD dwData, BYTE* pComp, DWORD BufSize, DWORD& dwComp);
	static BOOL UnComp_RLE4(const BYTE* pComp, DWORD dwComp,
							 DWORD	Start, DWORD Length, DWORD dwWidth, BYTE* pData);
	static BOOL GetLoc_RLE4(DWORD FindLoc, DWORD dwWidth, const BYTE* pComp, DWORD dwComp,
							 DWORD* Start_UnComp, DWORD* LLength);

	//for 8 Bits
	static BOOL Comp_RLE8(const BYTE* pData, DWORD dwData, BYTE* pComp, DWORD BufSize, DWORD& dwComp);
	static BOOL UnComp_RLE8(const BYTE* pComp, DWORD dwComp, DWORD dwWidth,
							 /* UnComp Locatin */DWORD	Start, DWORD Length,
							 BYTE* pData);
	static BOOL GetLoc_RLE8( DWORD Loc_UnComp, DWORD dwWidth, const BYTE* pComp, DWORD dwComp,
							DWORD* Start_Comp, DWORD* Start_UnComp, DWORD* Length );
	static bool UnPackFlagBits8( const SIZE* szBlock, const BYTE* pComp, DWORD dwComp, 
		const BYTE* pFlagVal, int nFlagVal, BYTE validVal, 
		BYTE*	pData, int nRowBytes,
		const POINT* posInBlock,const SIZE* szInBlock);

	//for 24 Bits
	static BOOL Comp_RLE24(const BYTE* pData, DWORD dwData, BYTE* pComp, DWORD BufSize, DWORD& dwComp);
	static BOOL UnComp_RLE24(const BYTE* pComp, DWORD dwComp,
							 /* UnComp Locatin */DWORD	Start, DWORD Length, DWORD dwWidth,
							 BYTE* pData);
	static BOOL GetLoc_RLE24(DWORD FindLoc, DWORD dwWidth, const BYTE* pComp, DWORD dwComp,
							 DWORD* Start_Comp, DWORD* Start_UnComp);

protected:	// Block Data Compress
	static BOOL UnPack1( const SIZE* szBlock, const BYTE* pComp, DWORD dwComp, 
							BYTE*	pData, int nRowBytes,
							const POINT* posInBlock,const SIZE* szMap,
							const DWORD* pXMap, const DWORD* pYMap);
	static BOOL UnPack8( const SIZE* szBlock,const BYTE* pComp, DWORD dwComp, 
							BYTE*	pData, int nRowBytes,
							const POINT* posInBlock,const SIZE* szMap,
							const DWORD* pXMap, const DWORD* pYMap);
	static BOOL UnPack24( const SIZE* szBlock, const BYTE* pComp, DWORD dwComp, 
							BYTE*	pData, int nRowBytes,
							const POINT* posInBlock,const SIZE* szMap,
							const DWORD* pXMap, const DWORD* pYMap);

	static DWORD CalcMono1(int nCompMode, int nWidth, int nHeight, 
							const BYTE* pBits, DWORD dwBits, BYTE val);
	static DWORD CalcMono8(int nCompMode, int nLength, 
							const BYTE* pBits, DWORD dwBits, BYTE val);
	static DWORD CalcMono24(int nCompMode, int nLength, 
							const BYTE* pBits, DWORD dwBits, const BYTE* pVal);

public:
	//Base64编码/译码
	//	src		:[in]源字符串
	//返回：编码/解码后目标缓冲区（字符串缓冲区）
	static char* Base64_Encode(const char *src);
	static char* Base64_Decode(const char *src);
	//释放 Base64_Encode / Base64_Decode 返回的
	static void Base64_Free(char **ppBuf);

};

//曲线离散操作类
class RP_EXT_CLASS SEPARATE
{
public:
	SEPARATE();
	~SEPARATE();

	//-***  cv= cvBezier or cvBSpline ****
	//得曲线外接矩形
	static BOOL GetCurveBorder(RECTD& rcDst, const POINTD* pCtrl, int nCtrl, CURVE cv);

	//-***  cv= cvBezier, cvBSpline, cvPolyline or cvTrace ****
	//Poly曲线离散
	static BOOL Separate( CPOINTDArray& ptArray,  const POINTD* pCtrlPt, int nCtrlPt, CURVE cv, BOOL bClose= FALSE, SEPWAY way=swRough);
	static BOOL Separate( CPOINTArray& ptArray,  const POINTD* pCtrlPt, int nCtrlPt, CURVE cv, BOOL bClose= FALSE, SEPWAY way=swSmooth);
	static BOOL Separate( CPOINTArray& ptArray,  const POINT* pCtrlPt, int nCtrlPt, CURVE cv, BOOL bClose= FALSE, SEPWAY way=swSmooth );
	//PolyPoly曲线离散
	static BOOL Separate( CPOINTArray& SprtArray, CintArray& CountArray, const POINTD* lpCtrl, const int* lpCount, int nPoly, CURVE cv, BOOL bClose= FALSE,SEPWAY way=swSmooth);
	static BOOL Separate( CPOINTArray& SprtArray, CintArray& CountArray, const POINT* lpCtrl, const int* lpCount, int nPoly, CURVE cv, BOOL bClose= FALSE, SEPWAY way=swSmooth);


	//BSpline <--> Bezier
	//	lpCtrl1, nNumPoint	:[in]原始点串
	//	lpCtrl2, nNumPoint2	:[out]目标点串
	//	bBStoBe				:[in]是否为BSpline-->Bezier，当为FALSE时，表示Bezier-->BSpline
	static BOOL BSplineToBezier( const POINT* lpCtrl1,  int nNumPoint, POINT* lpCtrl2, int& nNumPoint2, BOOL bBStoBe=TRUE);
	static BOOL BSplineToBezier( const POINTD* lpCtrl1, int nNumPoint, POINTD* lpCtrl2, int& nNumPoint2, BOOL bBStoBe=TRUE);

protected:
	//
	static BOOL LineInsert( const POINTD* pPointF, int nNumPtF, LPPOINT lpPointD, int nBufSize, int& nNumPtD );
	static BOOL LineInsert( const POINTD* pPointF, int nNumPtF, POINTD* lpPointD, int nBufSize, int& nNumPtD );
	//
	static BOOL Insert(const POINTD* lpPoint, int nPoint, POINT*  pPointD,int nBufSize,int& nNumD, double dDest=5.0 );
	static BOOL Insert(const POINT* lpPoint, int nPoint, POINT*  pPointD,int nBufSize,int& nNumD, double dDest=5.0 );
	static BOOL Insert(const POINT* lpPoint, int nPoint, POINTD* lpPtF,  int nBufSize, int& nNumPt, double dDest=5.0);
	//暂未实现
	static BOOL Insert(const POINTD* lpPoint, int nPoint, POINTD*  pPointD,int nBufSize,int& nNumD, double dDest=5.0);

	static BOOL SeparateChar( CPOINTArray& ptArray,  const POINTD* pCtrlPt, int nCtrlPt, BOOL bClose= FALSE, SEPWAY way=swSmooth );

private:
	static RECTD GetOneSegBorder( const POINTD* pCtrl /*nCtrl=4*/);
	static BOOL SeparateOneSeg(LPPOINT pSprt, int nBufSize, int& nSprt, const double* dCtrlX, const double* dCtrlY/*nCtrl=4*/, int nPreNum);
	static BOOL SeparateOneSeg(POINTD* pSprt, int nBufSize, int& nSprt, const double* dCtrlX, const double* dCtrlY/*nCtrl=4*/, int nPreNum);
	static BOOL GetEnlg3(const POINT* pPt, POINTD* pPtF,double *t1,double *t2);
	//复合俩Curve
	static BOOL Fuhe(POINTD* pCtrl1, double t1, double t2, POINTD* pCtrl2, double d1, double d2, POINTD *lpPtF,int nBufSize, int& nNumPt);
	static BOOL Enlg3Insert(const POINT* lpPoint, int nPoint, POINTD* lpPtF, int nBufSize, int& nNumPt);
	static BOOL GetMyBSCtrl(POINTD* pCtrl, int nCtrl);
	
	static BOOL GetMyEBSCtrl(POINTD* pCtrl, int nCtrl);
	static BOOL SeparateEBSOneSeg(LPPOINT pSprt, int nBufSize, int& nSprt, const double* dCtrlX, const double* dCtrlY/*nCtrl=4*/, int nPreNum);
};

class CRstLines;
class RP_EXT_CLASS AMathOpt
{
public:
	//角度类型：普通，水平，垂直
	enum ANGLESYTLE {ANGLE_NORMAL=0, ANGLE_HOR, ANGLE_VER};

private:
	static CRITICAL_SECTION	m_section_IsBadWritePtr;		//IsBadWritePt 函数临界标识	
protected:
	// AMathOpt相关参数初始化/释放操作，CrpBaseApp调用一次并且仅允许调用一次。
	static void Init();
	static void Exit();
	friend class CrpBaseApp;

public:
	//当前缓冲区空间是否可写（同SDK中的 IsBadWritePtr 定义）
	//	lp	:[in]内存地址
	//	ucb	:[in]内存大小
	//注：SDK中函数不支持并发处理，本函数通过添加必须的临界标识来进行改进
	static BOOL MathOpt_IsBadWritePtr(LPVOID lp, UINT_PTR ucb);

public:
	//去除相邻并且相同的坐标点
	//	lpDst			:[out]目标缓冲区
	//	lpSrc, nCount	:[in]源点串缓冲区（点串）及缓冲区大小（点子数）
	//	返回值：目标点串长度
	//注：允许lpDst, lpSrc 使用相同指针
	static int SprayPoints(LPPOINT lpDst, const POINT* lpSrc, int nCount);
	//去除多段曲线中的相邻并且相同的坐标点
	//	lpPt	:[in/out]指定点串（多段）缓冲区
	//	lpCount	:[in/out]指定多段点串中每段的大小（点子数）
	//	nCount	:[in/out]指定点串段数
	//注：仅当存在某一段曲线中无有效坐标点时才有可能改变 nCount的值
	static int SprayPoints (LPPOINT lpPt, LPINT lpCount, int& nCount);

	//直线离散
	//	lpPt	:[in/out]输入时存放原始坐标点串，输出时存放离散后的目标点串
	//	dwNumPt	:[in]原始点串长度
	//	dwNumMax:[in]目标坐标缓冲区大小（可存放的最大点子数，POINT）
	//返回值：离散后的点子数
	static DWORD StepPolyline (LPPOINT lpPt, DWORD dwNumPt, DWORD dwNumMax);

	//得指定点串的外接矩形
	static RECT GetBorderRect (const POINT* lpPt, DWORD dwNumPt);
	static RECTD GetBorderRect (const POINTD* lpPt, DWORD dwNumPt);

	//点串移动
	static VOID OffsetPolygon (LPPOINT lpPoly, DWORD dwNum, int nXOffset, int nYOffset);
	static VOID OffsetPolygon (LPPOINT lpDst, const POINT* lpSrc, DWORD dwNum, int nXOffset, int nYOffset);

	//比例缩放点串坐标
	//	rcFrame :[in]矩形区域，指定点串经过一定的比例缩放后其外接矩形与此值相同
	static VOID FitPolyToRect (LPPOINT lpPoly, DWORD dwNumPt, RECT rcFrame);
	
	//点串旋转处理
	//	pCenter	:[in]指定旋转中心点，如果 pCenter==NULL，则旋转中心点取点串外接矩形中心
	static VOID RotatePoints (LPPOINT lpPt, DWORD dwNumPt, double fAngle, const POINT* pCenter=NULL);
	static VOID RotatePoints (LPPOINTD lpPt, DWORD dwNumPt, double fAngle, const POINTD* pCenter=NULL);

	//Bezier曲线离散
	//	lpPoints:[out]存放离散后的点串信息，当为NULL时，函数仅返回离散后点子数
	//	ptBCP	:[in]Bezier曲线的控制点（4个点）
	//	bSepWay	:[in]曲线离散方式，SEPWAY (swRough, swFine, swSmooth)
	//	dwNumMax:[in]目标坐标缓冲区大小（可存放的最大点子数，POINT）
	//返回值：曲线离散后点子数
	//------------------------------------------------------------------------
	static DWORD CreateBezierPoints (LPPOINT lpPoints, LPPOINT ptBCP, BYTE bSepWay, DWORD dwNumMax); 

public:
	//计算最小公约数
	static DWORD CommonMultiple(DWORD dwNum1, DWORD dwNum2);
	//浮点数转分子式
	//	dRatio	:[in]指定小数值（浮点数）
	//	wSize	:[in]指定分式中的分母最大值，(10, 0xFFFF)
	//返回值：dwScale，分子表达式，dRatio ≈ HIWORD(dwScale)/LOWORD(dwScale)
	static DWORD DoubleToFraction(double dRatio, WORD wSize); // wSize >= 10
	//简化分子式
	// HIWORD/LOWORD
	static DWORD SimpleFraction(DWORD dwFraction);
	//比例缩放
	// HIWORD(dwScale1)*HIWORD(dwScale2) / (LOWORD(dwScale1)*LOWORD(dwScale2))
	static DWORD ScaleMultiply(DWORD dwScale1, DWORD dwScale2);

	// 0 <= dwPosition < dwOld
	// return dwPosition 从 dwOld 到 dwNew 的位置
	static DWORD ResamplePosition (DWORD dwOld, DWORD dwNew, DWORD dwPosition);
	// 0 <= dwPosition < dwOld
	// pMap : dwPosition 从 dwOld 到 dwNew 的位置
	// return dwPosition 从 dwOld 到 dwNew 时的个数
	static DWORD ResamplePosition (DWORD dwOld, DWORD dwNew, DWORD dwPosition, DWORD* pMap);
	//	dwOld:		原始长度。
	//	dwNew:		圆整后的长度。
	//	dwStart, dwEnd : 得从 dwStart 到 dwEnd 之间的各点对应值, dwStart < dwEnd
	//	pCount:		原始的每个点的重复数(不包括dwEnd), pCount size is dwEnd-dwStart
	static void ResampleSegCount(DWORD dwOld, DWORD dwNew, DWORD dwStart, DWORD dwEnd, DWORD* pCount);
	//	dwOld:		原始长度。
	//	dwNew:		圆整后的长度。
	//	dwStart, dwEnd : 得从 dwStart 到 dwEnd 之间的各点对应值, dwStart < dwEnd
	//	pMap:		原始的每个点的对应值(不包括dwEnd), pMap size is dwEnd-dwStart
	static void ResampleSegMap(DWORD dwOld, DWORD dwNew, DWORD dwStart, DWORD dwEnd, DWORD* pMap);
	//	pCount : Repeat's count of every point on dwOld (pCount's size is dwOld)
	//	dwOld:		原始长度。
	//	dwNew:		圆整后的长度。
	//	pCount:		原始的每个点的重复数(不包括dwEnd), (pCount's size is dwOld)
	static void ResampleLine (DWORD dwOld, DWORD dwNew, DWORD* pCount);
	//	pMap : Map coordinates of dwOld to dwNew (pMap's size is dwOld+1)
	//	dwOld:		原始长度。
	//	dwNew:		圆整后的长度。
	//	pMap:		原始的每个点的对应值(不包括dwOld), (pMap's size is dwOld)
	static void ResampleLineMap (DWORD dwOld, DWORD dwNew, DWORD* pMap);

public:
	//两个角度值（弧度）是否相等
	static BOOL EquAngle (double angle1, double angle2);
	//判断指定角度值（弧度）状态（水平或垂直）
	static AMathOpt::ANGLESYTLE AngleStyle(double dbAngle);
	//计算直线角度
	//	ptFirst, ptCursor	:[in]直线的两个端点
	//	bRadian				:[in]是否需返回弧度值，为FALSE返回角度值
	static double GetAngle (POINTD ptFirst, POINTD ptCursor, BOOL bRadian);
	static double GetAngle (POINT ptFirst, POINT ptCursor, BOOL bRadian) 
	{ return GetAngle (POINTD(ptFirst), POINTD(ptCursor), bRadian); }

public:
	//得点到直线距离
	//	pt				:[in]点
	//	pt1, pt2		:[in]直线参数（pt1, pt2 指定直线的两个端点）
	static double DistPtToLine (POINTD pt, const POINTD& pt1, const POINTD& pt2);
	//得点到直线距离
	//	pt				:[in]点
	//	ptLine, dbAngle	:[in]直线参数（通过ptLine，角度为dbAngle）
	static double DistPtToLine (POINT pt, POINT ptLine, double dbAngle);
	//得点到直线距离
	//	pt				:[in]点
	//	pPtLine[2]		:[in]直线参数（pPtLine[0], pPtLine[1]指定直线的两个端点）
	static double DistPtToLine (POINT pt, const POINT* pPtLine);
	
	//得两点间的距离
	//	pt1, pt2		:[in]点
	//返回值：pt1,pt2之间的距离
	static double NearPoint (const POINT& pt1, const POINT& pt2);
	//两点间距离是否小于 dDist
	//	pt1, pt2		:[in]点
	//	dDist			:[in]判定距离
	//返回值：pt1,pt2之间的距离 <= dDist 时返回TRUE，否则返回FALSE
	static BOOL NearPoint (const POINT& pt1, const POINT& pt2, double dDist);
	//得点到线上点（点串）的最短距离
	//	pt				:[in]点
	//	lpPt, uNum		:[in]点串坐标
	//	pdRet			:[out]存放最短距离值
	//返回值：最短距离值对应的点在点串中的索引
	static UINT NearPoint(const POINT* lpPt , UINT uNum, const POINT & pt, double* pdRet=NULL);

public:
	//两矩形是否相同
	static BOOL EquRect (const RECT& rc1, const RECT& rc2)
	{
		return (BOOL)(rc1.left == rc2.left
			&& rc1.top == rc2.top
			&& rc1.right == rc2.right
			&& rc1.bottom == rc2.bottom
			);
	}
	//矩形合并（参考 SDK 的 UnionRect 函数）
	static RECT UnionRect (const RECT& rcSrc1, const RECT& rcSrc2);
	//得矩形四点坐标
	//	topleft, topright, bottomright, bottomleft
	static void GetRectPoints (POINT* pPts, const RECT& rect);

public:
	//计算两条直线段的交点坐标
	//  P1P2 Line: pt1, pt2; P3P4 Line: pt3, pt4
	//	lpPt	:[out]存放交点坐标
	//返回值：交点状态
	//	-1	:无交点，即两直线段为平行线
	//	0	:交点在直线段外（非有效交点）
	//	1	:交点在直线段内，为有效交点
	static int IntersectLine(LPPOINT lpPt, const POINT& pt1, const POINT& pt2, const POINT& pt3, const POINT& pt4);
	static int IntersectLine (LPPOINTD lpPt, const POINTD& pt1, const POINTD& pt2, const POINTD& pt3, const POINTD& pt4);
	//两直线交点
	//	ptInter			:[out]交点坐标
	//	ptd1, dbAngle1	:[in]第一条直线参数
	//	ptd2, dbAngle2	:[in]第二条直线参数
	static BOOL IntersectLine (POINTD& ptInter, const POINTD& ptd1, double dbAngle1, const POINTD& ptd2, double dbAngle2);
	
	//求直线与矩形的交点
	//	rc			:[in]源矩形
	//	pt, dbAngle	:[in]直线参数（经过pt，角度为dbAngle的直线）
	//	pRet		:[out]交点坐标缓冲区，缓冲区大小需 >= 4
	//返回值：交点个数，最多为4个点
	static int IntersectRect (RECT rc, POINT pt, double dbAngle=0.0, POINT* pRet = NULL);
	//两圆交点，返回值为交点点数
	//	ptInterArray	:[out]交点坐标数组
	//	ptd1, dbDiameter1	:[in]第一圆参数（以ptd1为中心，直径为dbDiameter1的圆）
	//	ptd2, dbDiameter2	:[in]第二圆参数（以ptd2为中心，直径为dbDiameter2的圆）
	static int IntersectCircle (CPOINTDArray& ptInterArray, const POINTD& ptd1, double dbDiameter1, const POINTD& ptd2, double dbDiameter2);
	//圆与直线交点，返回值为交点点数
	//	ptInterArray		:[out]交点坐标数组
	//	ptd1, dbDiameter1	:[in]圆参数（以ptd1为中心，直径为dbDiameter1的圆）
	//	pt2, dbAngle2		:[in]直线参数（经过ptd2，角度为dbAngle2的直线）
	static int IntersectCircleLine (CPOINTDArray& ptInterArray, const POINTD& ptd1, double dbDiameter1, const POINTD& ptd2, double dbAngle2);
	
public:
	//三点得圆
	//	pt1, pt2, pt3	:[in]圆上的三点
	//	ptCenter		:[out]计算得出的圆中心点
	//返回值：
	//	计算得出的圆半径 radius，当 radius < 0 时表示参数错误
	static double GetCircleFromThreePoints (POINTD& ptCenter, const POINTD& pt1, const POINTD& pt2, const POINTD& pt3);

public:
	//曲线外接矩形
	static RECT GetBox(const POINT* pPts, const int* pCount, UINT uCount, const ATransform * ptrform=NULL, CURVE curve=cvBezier);
	static RECT GetBox (const POINT* pPts, UINT uNumPts, const ATransform* ptrform=NULL, CURVE curve=cvBezier);
	//求弧度, ptc: center point
	static BOOL ArcAngle(const POINTD& pt, const POINTD& ptc, double& dAngle);
	//是否为逆时针方向
	static BOOL IsCounterClockwise(double dAngStart, double dAngMid, double dAngEnd);

public:
	//内存拷贝
	//将指定区域内的原数据拷贝到目标缓冲区中
	//	pDst				:[out]目标缓冲区，缓冲区宽度为 nDstRowBytes
	//	nDstRowBytes		:[in]目标缓冲区宽度
	//	pSrc, pSrcRowBytes	:[in]原始缓冲区及每行宽度
	//	nWidth, nHeight		:[in]指定所需拷贝的区域大小
	static BOOL myMemcpy(BYTE* pDst, int nDstRowBytes, BYTE BitCount,
				const BYTE* pSrc, int nSrcRowBytes, int nWidth, int nHeight);
	//	pRstInBlock			:[in]指定所需拷贝的区域大小
	//							pRstInBlock'(left,top) be equal to pSrc'start position
	//							pRstInBlock is protect Rgn of pDst
	//							pDst : Start write position ==> top*nDstRowBytes+left
	//							pSrc : Start read  position ==> 0
	static BOOL myMemcpy(BYTE* pDst, int nDstRowBytes, BYTE BitCount,
				const BYTE* pSrc, int nSrcRowBytes, const CRstLines* pRstInBlock);
	//内存初始化操作
	//	pDst		:[out]目标缓冲区，缓冲区大小为 nItem * nCount
	//	pSrc		:[in]原始缓冲区
	//	nItem		:[in]指定原始数据单元的大小（即pSrc缓冲区大小），如 sizeof(POINT)
	//	nCount		:[in]指定所需初始化操作的单元个数
	static void myMemset(BYTE* pDst, const BYTE* pSrc, int nItem, int nCount);

	//功能概要:求rcSrc1不在rcSrc2中的部分,结果保存与rcDstArray中
	//参数定义:
	//	rcSrc1, rcSrc2	:[in]
	//	rcDstArray		:[out]
	//返回值:
	//	rcDstArray数组的长度,假如为0,则全部在rcSrc2中
	static int DifferRect(const RECT rcSrc1, const RECT rcSrc2, CRECTArray& rcDstArray);

	//功能概要:求rcSrcArray不在rcSrc中的部分,结果保存与rcDstArray中
	//参数定义:
	//	rcSrcArray, rcSrc	:[in]
	//	rcDstArray			:[out]
	//返回值:
	//	rcDstArray数组的长度,假如为0,则全部在rcSrc中
	static int DifferRect(const CRECTArray& rcSrcArray, const RECT rcSrc, CRECTArray& rcDstArray);

	//功能概要:求rcSrc不在rcSrcArray中的部分,结果保存与rcDstArray中
	//参数定义:
	//	rcSrc, rcSrcArray	:[in]
	//	rcDstArray			:[out]
	//返回值:
	//	rcDstArray数组的长度,假如为0,则全部在rcSrcArray中
	static int DifferRect(const RECT rcSrc, const CRECTArray& rcSrcArray, CRECTArray& rcDstArray);
	
	//功能概要:求rcSrcArray1不在rcSrcArray2中的部分,结果保存与rcDstArray中
	//参数定义:
	//	rcSrcArray1, rcSrcArray2	:[in]
	//	rcDstArray					:[out]
	//返回值:
	//	rcDstArray数组的长度,假如为0,则全部在rcSrcArray2中
	static int DifferRect(const CRECTArray& rcSrcArray1, const CRECTArray& rcSrcArray2, CRECTArray& rcDstArray);

public:
	// 功能	   : 求 代数方程 A[0] + A[1]*x +A[2]*x^2 + A[3]*x^3 + A[4]^4 = 0 的实根(最高4次)
	// 输入条件：nA>=2 && nA<=5. x 长度>= nA-1.
	// 方程次数: nA-1.
	// 返回    : if return TRUE:	n 为实根个数. x 存放实根
	//			 if return FALSE:   data error.
	static BOOL Equation( const double* A, int nA, double* x, int& n);
	//	5 次代数方程求实根, 返回同上.
	static BOOL Equation5(const double* A/*nA=6*/, double x0, double x1, double* x, int& n, double eps, BYTE bUp);
	
protected:
	// u + v*i = (x + y*i)^(1/n) : i= sqrt(-1)
	static void Ontrt(double x, double y, int n, double* u, double* v);
	static BOOL ddhrt(const double* A, double a,double b, double* x, double eps);

public:
	//缓冲区数据排序
	//	base		:[in/out]输入时存放原始需排序数据，输出时存放排序好后的数据
	//	width,length:[in]指定缓冲区大小，size=width*lenght
	//				width :指定缓冲区数据成员的大小，如 sizeof(POINT)
	//				length:指定缓冲区数组中成员个数
	//	p_buffer	:[in/out]为辅助空间, 长度和base一样, p_buffer != base
	//				p_buffer中仅存放临时数据
	//	comp		:[in]自定义比较函数，具体含义同 SDK 中的 qsort
	//				comp 函数返回值含义：
	//					< 0	: elem1 <  elem2 
	//					0	: elem1 == elem2 
	//					> 0 : elem1 >  elem2 
	//例：
	//	Int32 __cdecl pointCompProc(const void *elem1, const void *elem2)
	// 	{
	// 		ASSERT(elem1 != NULL);
	// 		ASSERT(elem2 != NULL);
	//		POINT pt1, pt2;
	//		pt1 = *(POINT*)elem1;	
	//		pt2 = *(POINT*)elem2;
	// 		if (pt1.x > pt2.x) return 1;
	// 		if (pt1.x < pt2.x) return -1;
	// 		return (pt1.y - pt2.y);
	// 	}
	//	AMathOpt::merger_sort(pPoints, nCount, sizeof(POINT), pPtTmp, pointCompProc);
	static void merger_sort(
		void *base, Int32 length, Int32 width,
		void *p_buffer,
		Int32 (__cdecl *comp)(const void * elem1, const void *elem2));

protected:
	//对base[s..t] (不包括t)进行归并排序
	//p_buffer为辅助空间, 长度和base一样, p_buffer != base
	static void _pri_merger_sort(
		void *base, Int32 width, Int32 s,	Int32 t,
		void *p_buffer,
		Int32 (__cdecl *comp)(const void * elem1, const void *elem2));
	//将SR[i..m]和SR[m..n] 归并为有序的SR[i..n]
	static void _pri_Merge(void* SR, Int32 width,
		Int32 i, Int32 m, Int32 n, 
		void *p_buffer,
		Int32 (__cdecl *comp)(const void * elem1, const void *elem2));

public:
	//在缓冲区(pBuffer)中查找指定数据串(pData)
	//返回：指令串在缓冲区的偏移，当为 0xFFFFFFFF 时表示未查找到符合项
	static DWORD FindData(const BYTE *pBuffer, DWORD dwBufferSize, const BYTE *pData, DWORD dwDataSize);
	static DWORD FindData(const BYTE *pBuffer, DWORD dwBufferSize, BYTE bData);
	//在缓冲区(pBuffer)中查找指定数据(bData)前的数值字符串('0'-'9')
	//返回：指令串在缓冲区的偏移，当为 0xFFFFFFFF 时表示未查找到符合项
	static DWORD FindData_Int(const BYTE *pBuffer, DWORD dwBufferSize, BYTE bData);
	//在缓冲区(pBuffer)中反向查找指定数据串(pData)
	//返回：指令串在缓冲区的偏移（反向），当为 0xFFFFFFFF 时表示未查找到符合项
	static DWORD ReverseFindData(const BYTE *pBuffer, DWORD dwBufferSize, const BYTE *pData, DWORD dwDataSize);
	static DWORD ReverseFindData(const BYTE *pBuffer, DWORD dwBufferSize, BYTE bData);

};

class RP_EXT_CLASS ALocation : public CPoint
{
public:
	ALocation() : CPoint(0, 0) {m_fAngle=0.0f;}
	ALocation(int nX, int nY, float fAngle) : CPoint(nX, nY) {m_fAngle = fAngle;}
	ALocation(POINT pt, float fAngle) : CPoint(pt) {m_fAngle = fAngle;}
	ALocation(SIZE size, float fAngle) : CPoint(size) {m_fAngle = fAngle;}
	ALocation(DWORD dwPoint, float fAngle) : CPoint(dwPoint) {m_fAngle = fAngle;}
	
public:
	//	Operators
	ALocation& operator = (const ALocation& src);
	BOOL operator == (const ALocation& src);

public:
	float	m_fAngle;	//	Rotation angle : source relative to destination;
};

//	Key word:
//		ANLOOP:
//			is consist of several polygons, first is outside loop and
//			the others are inside loop.
//		ANLOOPGRP:
//			LOOP group, consist of several LOOPs.
struct ANLOOP {
	Int32		nNumPoly;
	Int32		nNumPoint;
	LPINT		lpCount;	//	Point's count of every polygon
	LPPOINT		lpPoint;	//	All points
	RECT		rcBox;
}; 
typedef ANLOOP FAR* LPANLOOP;

struct ANLOOPGRP {
	Int32			nNumLoop;
	HGLOBAL		hMemLoop;	//	Lock by (LPANLOOP)lpLoop
	RECT		rcBox;
};
typedef ANLOOPGRP FAR* LPANLOOPGRP;


//接回头参数定义
struct CYCLEPARAMS
{
	Int32		m_cycWidth;		//	Pixels
	Int32		m_cycHeight;	//	Pixels
	Int32		m_cycParam;		//	Offset relative to width or height (unit: pixel)
};
struct RP_EXT_CLASS CYCLEINFO : public CYCLEPARAMS
{
	enum CycWay {horCycle=1, verCycle=2};
	BOOL		m_cycOn;	//On=TRUE, Off= FALSE	
	Int32		m_cycWay;	//horCycle=1, verCycle=2

public:
	CYCLEINFO();
	void Init();
	BOOL IsValid() const;
};

//接回头参数定义-浮点型
struct CYCLEPARAMS_FLOAT
{
	double		m_float_cycWidth;		//	Pixels
	double		m_float_cycHeight;		//	Pixels
	double		m_float_cycRatio;		//	跳接比例，以分式形式表示，如 1/2，1/3，……，必须小于等于1（0相当于1，表示平接）
};
struct RP_EXT_CLASS CYCLEINFO_FLOAT : public CYCLEPARAMS_FLOAT
{
	BOOL		m_float_cycOn;	//On=TRUE, Off= FALSE	
	Int32		m_float_cycWay;	//horCycle=1, verCycle=2

public:
	CYCLEINFO_FLOAT();
	void Init();
	BOOL IsValid() const;
	BOOL GetCycleInfo(CYCLEINFO &cycInfo) const;
	void SetCycleInfo(const CYCLEINFO &cycInfo);
};

struct APTLENGTH
{
	int	nX;
	int	nY;
	int	nLength;
};
typedef CArray<APTLENGTH,APTLENGTH>	APTLENGTHArray;



//g_Flags存放全局状态信息, 部分内容和注册表有关(程序启动时读注册表,关闭时写注册表)
#define DEFAULT_RESUNIT		ruDPI		//缺省精度单位
#define DEFAULT_SIZEUNIT	suMM		//缺省尺寸单位
#define DEFAULT_RES 		600			//ruDPI
#define DEFAULT_WIDTH		1800		//suMM
#define DEFAULT_HEIGHT		10000		//suMM
#define RULER_WIDTH			16			//标尺宽度(Pixel)
struct RP_EXT_CLASS AGFLAGS
{
	AResUnit	ResUnit;				//精度单位: 像素/英寸,像素/丝米,像素/毫米,像素/厘米,像素/米
	ASizeUnit	SizeUnit;				//尺寸单位 像素,丝米,毫米,厘米,米,英寸
	
	double		dDefRes;				//新建文件的缺省精度, AResUnit
	double		dDefWidth;				//新建文件的缺省宽度, suMM
	double		dDefHeight;				//新建文件的缺省高度, suMM

	BOOL		ShowRuler;				//显示标尺

	BOOL		ShowGridline;			//显示网格线

	int			nLRSpace;				//当非接回头状态时，左右两边的空隙（屏幕坐标大小）
	int			nTBSpace;				//当非接回头状态时，上下两边的空隙（屏幕坐标大小）

	BOOL		bDirectDelete;			//是否直接删除文件(文件替换时)，为 FALSE 时为放入垃圾箱中

	CString 	strCacheDirectory; 		//系统缓存路径

	BOOL		bShowColorGroup;		//色组对话框是否以全图（显示标志色和成员色）方式显示

	BOOL		bEnableToolTip;			//是否显示工具提示

	UINT		nApplicationLook;		//应用程序界面风格ID（仅用于 CMainFrame）

	CString		strPrintDeviceModule;	//打印设备驱动模块名称，如 "rp16DGU.drv"

	BOOL		bThumbWithMono;			//层列表-缩略图-单色稿/灰度稿以单色模式显示
	
	BOOL		bScaleLimit;			//在进行图像大小改变相关的操作时限制X/Y方向等比缩放

	enum DYEMIX_WAY
	{
		dyemix_way_anseries = 0,		//AnSeries 模式，透明度应用时不考虑复色，全局影响
		dyemix_way_opacity	= 1,		//透明度模式，应用时考虑复色，仅有复色部分进行混色

		dyemix_way_count
	};
	BYTE		bDyeMixWay;								//颜料混合模式，DYEMIX_WAY
	int			nDefaultOpacity[dyemix_way_count];		//默认透明度

	enum DYEMIX_OPACITY_WAY
	{
		dyemix_opacity_way_normal = 0,	//普通方式（类似于变色龙的颜料混合模式）， R=R1*R2/255
		dyemix_opacity_way_addup,		//直接累加模式，C=C1+C2，有一通道溢出则所有通道同比压缩

		dyemix_opacity_way_count
	};
	BYTE		bDyeMixOpacityWay;		//透明度模式下的混合计算方式，仅针对 dyemix_way_opacity 模式

	BOOL		bLoadCMYKFileToRGB;		//是否将CMYK格式的TIF/JPG文件当作RGB图处理，为FALSE时读入时作为4个专色层处理
	BOOL		bUsedEmbedICC;			//是否使用图像文件内嵌ICC进行进行导入处理（针对CMYK格式，当bLoadCMYKFileToRGB==TRUE时有效）

	BOOL		bNeedCloth;				//是否显示背景图案

	BOOL		bBackgroundPaintEnable;	//是否允许后台刷新模式
	int			nPaintSize;				//后台刷新区域大小（分块）

public:
	BOOL		bStatusShowColorInfo;	//是否在状态栏上显示颜色信息（临时，不存放于配置信息中）
	BOOL		bTimerPaint;			//延时刷新标记，为FALSE时屏幕实时刷新，仅bBackgroundPaintEnable==TRUE时有效

public:
	//以下参数与实际RIP时使用的 ClrSepParam 值应该一致，
	//	在此处定义仅为保证所有底层库均能进行调用，但仅允许在 RainBow界面中进行修改
	CString			strInIccProfile;		//输入 icc profile （RGB）
	CString			strInCMYKIccProfile;	//输入 icc profile （CMYK）
	CString			strMoniIccProfile;		//显示器 icc profile
	CString			strOutIccProfile;		//输出 icc profile
	enum INTENT_WAY
	{
		intent_Perceptual	= 0,		//可感知	，Perceptual
		intent_Relative		= 1,		//相对比色	，Relative colorimetric
		intent_Saturation	= 2,		//饱和度	，Saturation
		intent_Absolute		= 3,		//绝对比色	，Absolute colormietirc

		intent_count
	};
	UINT32			nIntent;			//匹配模式，INTENT_WAY

	BOOL			bMapDataFileEnable;			//是否允许自动生成映射数据文件
	CString			strMapDataCacheDirectory;	// RGB->CMYK的快速映射数据目录，可自定义

	BOOL			bShowSpashScreen;			//启动时是否显示SpashScreen

	DWORD 			dwPrnFileSizeLimit;			//单个PRN文件大小限制，当为0时表示不作限制，单位：MB

	BOOL			bMouseWheelForZoomScale;	//鼠标滚轮缩放，为FALSE时滚轮进行上下/左右移动
	
	BOOL			bProofDisplay;				//是否默认为屏幕软校样模式（显示）

	CString			strRIPDataCacheDirectory;	// RIP处理时数据缓存目录，可自定义

	BOOL			page_bLayoutLimit;			//选中图案定位时限制在版面内
	BOOL			page_bSpaceLimit_Enable;	//选中图案拼接时是否有间隔限制
	double			page_dSpaceLimit_hor;		//图案水平间隔（最小值），mm，< 0 时表示亦不限制
	double			page_dSpaceLimit_ver;		//图案垂直间隔（最小值），mm，< 0 时表示亦不限制

	BOOL			page_bDefaultToCustomCut;	//默认为简单剪裁，FALSE表示默认为缩放

	CString			strDefaultFontName;			//标题使用的默认字体名

public:
	//初始化全局参数值，对g_Flags操作
	BOOL Init();

	//从配置文件/注册表中读取信息
	BOOL LoadFromProfile();
	BOOL LoadFromProfile(const char *pszIniFile);
	//将参数写入配置文件/注册表中
	void SaveToProfile();
	BOOL SaveToProfile(const char *pszIniFile);
};

//功能概要: 回头操作类,提供接回头的算法(回头分割,回头定位等), 该类是纯算法类
//注：
//	有关接回头计算均基于 CYCLEINFO 进行，仅当需对回头参数进行缩放等操作时才采用 CYCLEINFO_FLOAT
class RP_EXT_CLASS ACycle : protected CYCLEINFO, protected CYCLEINFO_FLOAT
{
public:
	ACycle();
	~ACycle();

public:
	void CalLocalPoint (POINT& pt) const;
	void CalLocalPoint (int* pX, int* pY) const {POINT pt={*pX, *pY}; CalLocalPoint(pt); *pX=pt.x; *pY=pt.y;}
	POINT CalLocalPoint (int nX, int nY) const {POINT pt={nX, nY}; CalLocalPoint(pt); return pt;}
	void CalLocalPoints (POINT* pPoints, int nCount) const;
	int CalCycleLine (const APTLENGTH& srcSect, APTLENGTH* pSect, int* pX) const;
	int CalCycleRect (const RECT& rcSrc, LPRECT lprcBlk, LPPOINT lpptBlk) const;
	BOOL IsIntersectant(const RECT* pRect1, const RECT* pRect2) const; // if pRect2 is NULL, entire flower's rect
	// Calculate entire flower's rectangle
	int CycleSituation (POINT* pptSrc) const; // Only return 1 or 0
	int CycleSituation (const APTLENGTH* pSrcSect, POINT* pptSit) const;
	int CycleSituation (const RECT* prcSrc, POINT* pptSit) const;
	// If prcDst is NULL, entire flower's rectangle (same as above)
	int CycleSituation (POINT* pptSrc, const RECT* prcDst) const; // Only return 1 or 0
	int CycleSituation (const POINT* pptSrc, CPOINTArray& ptSit, const RECT* prcDst) const;
	int CycleSituation (const APTLENGTH* pSrcSect, CPOINTArray& ptSit, const RECT* prcDst) const;
	int CycleSituation (const RECT* prcSrc, CPOINTArray& ptSit, const RECT* prcDst) const;
	// If pIntArray is NULL, don't calculate this item 
	int CycleSituation (const APTLENGTH* pSrcSect, APTLENGTHArray* pScArray, CintArray* pIntArray, const RECT* prcDst) const;
	// If pPtArray is NULL, don't calculate this item 
	int CycleSituation (const RECT* prcSrc, CRECTArray* pRcArray, CPOINTArray* pPtArray, const RECT* prcDst) const;

	void BoundLeft (APTLENGTH& sc) const;
	void BoundMiddle (APTLENGTH& sc) const;
	void BoundMiddle (RECT& rc) const;
	void BoundLeftLower (RECT& rc) const;
	void BoundLeftUpper (RECT& rc) const;

public:
	void CalLocalPointD (POINTD& pt) const;

public:
	BOOL CycleOnOff() const {return m_cycOn;}
	int CycleWay() const {return m_cycWay;}
	int CycleParam() const {return m_cycParam;}
	int CycleWidth() const {return m_cycWidth;}
	int CycleHeight() const {return m_cycHeight;}
	void GetCycleParams(CYCLEPARAMS* pParams) const;
	void GetCycleInfo(CYCLEINFO* pInfo) const;

public:
	virtual void SetCycleOnOff(BOOL cycOn) {m_cycOn=cycOn;}
	virtual void SetCycleWay(int cycWay) {m_cycWay=cycWay;}
	virtual void SetCycleParam(int nParam) {m_cycParam=nParam;}
	virtual void SetCycleWidth(int nWidth) {m_cycWidth=nWidth;}
	virtual void SetCycleHeight(int nHeight) {m_cycHeight=nHeight;}
	virtual BOOL SetCycleParams(int nWidth, int nHeight, int nParam);
	virtual BOOL SetCycleParams(const CYCLEPARAMS* pParams);
	virtual BOOL SetCycleInfo(const CYCLEINFO* pInfo);

	// pOldParams: used to preserve old params; if NULL, no preserve 
	virtual BOOL ScaleCycleParams(DWORD dwScale, CYCLEPARAMS* pOldParams, BOOL bNegative=FALSE); //	XScale = YScale = dwScale
	virtual BOOL ScaleCycleParams(DWORD dwXScale, DWORD dwYScale, CYCLEPARAMS* pOldParams, BOOL bNegative=FALSE);

public:
	// Note!: after temporarily change params, must restore
	// pOldParams: used to preserve old params; if NULL, no preserve 
	BOOL TScaleParams(DWORD dwScale, CYCLEPARAMS* pOldParams, BOOL bNegative=FALSE); //	XScale = YScale = dwScale
	BOOL TScaleParams(DWORD dwXScale, DWORD dwYScale, CYCLEPARAMS* pOldParams, BOOL bNegative=FALSE);
	void TRestoreParams(const CYCLEPARAMS* pParams);

public:
	static BOOL InterLineRect (APTLENGTH* pDstSect, const APTLENGTH* pSrcSect, const RECT* pRect);
	static void GCalLocalPointD (POINTD& pt, const CYCLEINFO* pCycInfo);
	static void GCalLocalPoint (int* pX, int* pY,const CYCLEINFO* pCycInfo);
	static POINT GCalLocalPoint (int nX, int nY, const CYCLEINFO* pCycInfo);
	static void GCalLocalPoint (POINT& pt, const CYCLEINFO* pCycInfo);
	static void GCalLocalPoints (POINT* pPoints, int nCount, const CYCLEINFO* pCycInfo);
	static int GCalCycleLine (const APTLENGTH& srcSect, APTLENGTH* pSect, int* pX,const CYCLEINFO* pCycInfo);
	static int GCalCycleRect (const RECT& rcSrc, LPRECT lprcBlk, LPPOINT lpptBlk, const CYCLEINFO* pCycInfo);
	static int GCycleSituation (POINT* pptSrc, const CYCLEINFO* pCycInfo);// Only return 1 or 0
	static int GCycleSituation (POINT* pptSrc, const RECT* prcDst, const CYCLEINFO* pCycInfo); // Only return 1 or 0
	static int GCycleSituation (const POINT* pptSrc, CPOINTArray& ptSit, const RECT* prcDst, const CYCLEINFO* pCycInfo);
	static int GCycleSituation (const APTLENGTH* pSrcSect, POINT* pptSit, const CYCLEINFO* pCycInfo);
	static int GCycleSituation (const APTLENGTH* pSrcSect, CPOINTArray& ptSit, const RECT* prcDst, const CYCLEINFO* pCycInfo);
	static int GCycleSituation (const APTLENGTH* pSrcSect, APTLENGTHArray* pScArray, CintArray* pIntArray, const RECT* prcDst, const CYCLEINFO* pCycInfo);
	static int GCycleSituation (const RECT* prcSrc, POINT* pptSit, const CYCLEINFO* pCycInfo);
	static int GCycleSituation(const RECT* prcSrc, CPOINTArray& ptSit, const RECT* prcDst, const CYCLEINFO* pCycInfo);
	static int GCycleSituation (const RECT* prcSrc, CRECTArray* pRcArray, CPOINTArray* pPtArray, const RECT* prcDst, const CYCLEINFO* pCycInfo);
	static BOOL GScaleCycleParams(CYCLEPARAMS* pDst, const CYCLEPARAMS* pSrc, DWORD dwScale, BOOL bNegative);
	static BOOL GScaleCycleParams(CYCLEPARAMS* pDst, const CYCLEINFO* pSrc, DWORD dwXScale, DWORD dwYScale, BOOL bNegative);
};

//	Coordination transform class 
class RP_EXT_CLASS ACalCoord : public ACycle
{
public:
	ACalCoord();
	~ACalCoord();

public:
	//	If pView is NULL, activated view is used default
	void ViewToFlower(POINT& pt, AScrollView* pView) const;
	void FlowerToView(POINT& pt, AScrollView* pView) const;
	int  FlowerToView(POINT& pt, CPOINTArray& ptOffset, AScrollView* pView) const;
	
	void ViewToMatchFlower(POINT& pt, AScrollView* pView) const;
	void ViewToMatchFlower(RECT& rcPaint, AScrollView* pView) const;
	void ViewToMatchFlower(CRstLines& rstLines, AScrollView* pView) const;
	void FlowerToView (CRstLines& rstLines, AScrollView* pView) const;
	void ViewToFlower (CRstLines& rstLines, AScrollView* pView) const;

	void ViewToFlower(LPPOINT lpPts, int nCount, AScrollView* pView) const;
	void FlowerToView(LPPOINT lpPts, int nCount, AScrollView* pView) const;
	int	 FlowerToView(LPPOINT lpPts, int nCount, CPOINTArray& ptOffset, AScrollView* pView) const;

	void ViewToFlower(int& nValue) const;
	void FlowerToView(int& nValue) const;

	void ViewToFlower(SIZE& size) const;
	void FlowerToView(SIZE& size) const;

	void ViewToFlower(RECT& Rect, AScrollView* pView) const;
	void FlowerToView(RECT& Rect, AScrollView* pView) const;
	int  FlowerToView(RECT& Rect, CPOINTArray& ptOffset, AScrollView* pView) const;

	virtual void FlowerToSelf(POINT& pt, const POINT* pPoint) const;
	virtual void SelfToFlower(POINT& pt, const POINT* pPoint) const;
	virtual void FlowerToSelf(LPPOINT lpPts, int nCount) const;
	virtual void SelfToFlower(LPPOINT lpPts, int nCount) const;
	virtual void FlowerToSelfX(int& nWidth) const;
	virtual void SelfToFlowerX(int& nWidth) const;
	virtual void FlowerToSelfY(int& nHeight) const;
	virtual void SelfToFlowerY(int& nHeight) const;
	virtual void FlowerToSelf(SIZE& size, const SIZE* pSize) const;
	virtual void SelfToFlower(SIZE& size, const SIZE* pSize) const;
	virtual void FlowerToSelf(APTLENGTH& sc, const APTLENGTH* pSect) const;
	virtual void SelfToFlower(APTLENGTH& sc, const APTLENGTH* pSect) const;
	virtual void FlowerToSelf(RECT& Rect, const RECT* pRect) const;
	virtual void SelfToFlower(RECT& Rect, const RECT* pRect) const;

	// double type Transform
	void ViewToFlower(POINTD& pt, AScrollView* pView) const;
	void FlowerToView(POINTD& pt, AScrollView* pView) const;

	void ViewToFlower(LPPOINTD lpPts, int nCount, AScrollView* pView) const;
	void FlowerToView(LPPOINTD lpPts, int nCount, AScrollView* pView) const;

	void ViewToFlower(double& fValue) const;
	void FlowerToView(double& fValue) const;
	// End double type

	void FFlowerToVView(ATransform* pDstTrans, const ATransform* pSrcTrans, AScrollView* pView) const;
	int  FFlowerToVView(ATransform* pDstTrans, CPOINTArray& ptOffset, const ATransform* pSrcTrans, AScrollView* pView) const;
	void FFlowerToFView(ATransform* pDstTrans, const ATransform* pSrcTrans, AScrollView* pView) const;
	int  FFlowerToFView(ATransform* pDstTrans, CPOINTArray& ptOffset, const ATransform* pSrcTrans, AScrollView* pView) const;
	void FViewToFFlower(ATransform* pDstTrans, const ATransform* pSrcTrans, AScrollView* pView) const; // Only one
	void FViewToVView(ATransform* pDstTrans, const ATransform* pSrcTrans, AScrollView* pView) const; 
	int  FViewToVView(ATransform* pDstTrans, CPOINTArray& ptOffset, const ATransform* pSrcTrans, AScrollView* pView) const;
	void VViewToFFlower(ATransform* pDstTrans, const ATransform* pSrcTrans, AScrollView* pView) const; // Only one
	void VViewToVFlower(ATransform* pDstTrans, const ATransform* pSrcTrans, AScrollView* pView) const; 
	void VFlowerToVView(ATransform* pDstTrans, const ATransform* pSrcTrans, AScrollView* pView) const;
	void VFlowerToFFlower(ATransform* pDstTrans, const ATransform* pSrcTrans, AScrollView* pView) const; // Only one

	void VSameCellLeftTop(POINT& ptLT, const POINT& pt0, AScrollView* pView) const;
	void VSameCellLeftBottom(POINT& ptLB, const POINT& pt0, AScrollView* pView) const;
	void VSameCellPoint(POINT& ptDst, const POINT& pt0, AScrollView* pView) const;
	void VSameCellPointAndLT(POINT& ptDst, POINT& ptLT, const POINT& pt0, AScrollView* pView) const;
	void VSameCellPointAndLB(POINT& ptDst, POINT& ptLB, const POINT& pt0, AScrollView* pView) const;

	void ViewLeftAlign(int& nValue, AScrollView* pView) const;
	void ViewTopAlign(int& nValue, AScrollView* pView) const;
	void ViewRightAlign(int& nValue, AScrollView* pView) const;
	void ViewBottomAlign(int& nValue, AScrollView* pView) const;
	void ViewInflateAlign(RECT& rc, AScrollView* pView) const;

public:
	//	Coordination is relative to layout
	//	If prcPaint is NULL, paint on all area of related views.
	virtual void Repaint(AScrollView* pView, const RECT* prcPaint=NULL);

	//scale parameter, zoom, out, ...
public:
	DWORD ZoomScale() const {return m_ZoomScale;}
	DWORD XOutScale() const {return m_XOutScale;}
	DWORD YOutScale() const {return m_YOutScale;}
	DWORD XZoomScale() const {return m_XZoomScale;}
	DWORD YZoomScale() const {return m_YZoomScale;}
	virtual void SetZoomScale(DWORD dwScale);
	virtual void SetXOutScale(double fScale);
	virtual void SetXOutScale(DWORD dwScale);
	virtual void SetYOutScale(double fScale);
	virtual void SetYOutScale(DWORD dwScale);
	virtual void SetOutScale(double fXScale, double fYScale);
	virtual void SetOutScale(DWORD dwXScale, DWORD dwYScale);
	virtual void XOutScaleChanged();
	virtual void YOutScaleChanged();
	virtual void OutScaleChanged();

protected:
	DWORD	m_ZoomScale;	// View:Flower = HiWord:LoWord
	DWORD	m_XOutScale;	// Flower:Self = XOutRes:XRes
	DWORD	m_YOutScale;	// Flower:Self = YOutRes:YRes
	DWORD	m_XZoomScale;	// m_ZoomScale*m_XOutScale
	DWORD	m_YZoomScale;	// m_ZoomScale*m_YOutScale

	//set cycle info
public:
	void SetCycleOnOff(BOOL cycOn);
	void SetCycleWay(int cycWay);
	void SetCycleParam(int nParam);
	void SetCycleWidth(int nWidth);
	void SetCycleHeight(int nHeight);
	BOOL SetCycleParams(int nWidth, int nHeight, int nParam);
	BOOL SetCycleParams(const CYCLEPARAMS* pParams);
	BOOL SetCycleInfo(const CYCLEINFO* pInfo);
	BOOL ScaleCycleParams(DWORD dwScale, CYCLEPARAMS* pOldParams, BOOL bNegative=FALSE); //	XScale = YScale = dwScale
	BOOL ScaleCycleParams(DWORD dwXScale, DWORD dwYScale, CYCLEPARAMS* pOldParams, BOOL bNegative=FALSE);

	//location
public:
	const ALocation* Location() const {return &m_posSelf;}
	const POINTD* Position() const {return &m_ptdSelf;}
	virtual void SetLocation(const ALocation* pPos);
	virtual void SetLocation(const POINTD* pPos);
protected:
	ALocation	m_posSelf;	//Self's position in Flower
	POINTD		m_ptdSelf;	//same as m_posSelf, but it's float coord 

private:
	void LocalView(POINT& pt, AScrollView* pView) const;
	void LocalView(LPPOINT lpPts, int nCount, AScrollView* pView) const;
};

// Rasterline opt
struct RSRLINE
{
	CPOINTArray m_ptArray;
	CintArray	m_nLinesArray;
	CRect		m_rcBox;
};
typedef RSRLINE*					LPRSRLINE;
typedef CArray<LPRSRLINE,LPRSRLINE>	RSRLINEArray;
typedef RSRLINEArray*				LPRSRLINEArray;

class CLoopGrpOpt;
// 注：反串操作( RasterArray -> PolyPolygon) 在CLoopGrpOpt类
class RP_EXT_CLASS CRstLines
{
public:
	enum COMBINE_MODE { COMBINE_AND=5, COMBINE_OR, COMBINE_DIFF, COMBINE_XOR};
	enum DATA_TYPE { DATA_OK, DATA_NULL, DATA_ERROR,DATA_EMPTY};	//DATA_EMPTY剪不到时delete

private:
	RSRLINEArray*  m_lpRasterArray;
	RECT		   m_rcBox;
	int			   m_nNeedDelete;

public:
	CRstLines();
	virtual ~CRstLines();
 
	BOOL  Create(const POINT* lpPoint, int nPoint,AFillMode fillMode= fmAlternate);
	BOOL  Create(const POINT* lpPoint, const int* lpCount, int nPoly, AFillMode fillMode= fmAlternate,BOOL bTextType=FALSE);
	BOOL  Create(const RECT& rcRgn, BOOL bRect=TRUE, double dAngle=0);
	BOOL  Create(const ANLOOPGRP* lpAnloopGrp);

	BOOL  Copy(const RSRLINEArray* lpRasterArray);//Copy lpRasterArray'data to the Obj
	BOOL  Copy(const CRstLines* pRstLines);  // Copy an CRstLines obj

	VOID  Update();	// GetBox(m_lpRasterArray)==>m_rcBox

	VOID  CalLocal(const ACycle* pCycle);
	BOOL  Cycle(const ACycle* pCycle, const RECT* pRect=NULL);

	RECT		GetBox() const		{ return m_rcBox;	}
	RSRLINEArray* GetData() const	{ return m_lpRasterArray; }

	void  SetData(LPRSRLINEArray* ppRasterArray);// Set lpRasterArray to the Obj
	void  SetData(RSRLINEArray* lpRasterArray);// Move lpRasterArray'data to the Obj
	void  MoveData(CRstLines* pReMove);

	int   Combine(const CRstLines* pRstLines1, const CRstLines* pRstLines2, int nCombineMode, const ACycle* pCycle=NULL);

	int   Offset(int nXOffset, int nYOffset);
	BOOL  PtInRstLines(const POINT& pt, const ACycle* pCycle=NULL)const;
	BOOL  RectInRstLines(RECT* rc, const ACycle* pCycle=NULL)const;

	BOOL  IsEmpty() const;	
	VOID  SetEmpty();

	int   Add(const RSRLINEArray* lpRasterArray,BOOL bAdd=TRUE, const ACycle* pCycle=NULL);
	int   Add(const RSRLINE* lpRaster, BOOL bAdd= TRUE, const ACycle* pCycle=NULL);

public:
//////////////////////////////////////////////////////////////////////////////////
//------------------------------	ststic function ----------------------------//
//////////////////////////////////////////////////////////////////////////////////
	//在接回头时合并回头间的扫描线组,返回合并结果。
	//lpSrcRstLines  : 源扫描线指针
	//lpDstRstLines	 : 目的扫描线指针
	//return -1 : no changed, 0 : failed , 1 : succeed
	static int  CombineCycleRst(CRstLines* lpDstRstLines,const CRstLines* lpSrcRstLines, const ACycle* pCycle=NULL);//add by lcw 2003-7-30

	static int  Combine(const RSRLINEArray* lpRasterArray1, const RSRLINEArray* lpRasterArray2, int nCombineMode, LPRSRLINEArray& pResult,const ACycle* pCycle=NULL);

	//only set pRstArray to m_pRasterArray,not copy. while release CRstLines, not delete pRstArray
	static CRstLines* FromHand(const RSRLINEArray* pRstArray);
	static void FromHand(CRstLines* pObj, const RSRLINEArray* pRstArray);

	// return RasterArray count . after useing , need Release!
	static int DividebyLayer(const RSRLINEArray* lpRst, const RECT& rcLayer, LPRSRLINEArray* pResult, const ACycle* pCycle=NULL);

	//  属性操作
	static BOOL  PtInRaster( const RSRLINE* lpRaster,const POINT& pt);
	static BOOL  PtInRasterArray(const RSRLINEArray* pRst, const POINT& pt, const ACycle* pCycle=NULL);
	static BOOL  RectInRaster( const RSRLINE* pRst,RECT* rc, const ACycle* pCycle=NULL);
	static BOOL  RectInRasterArray(const RSRLINEArray* pRst,RECT* rc, const ACycle* pCycle=NULL);
	static BOOL  IsEqual(const RSRLINEArray* pRst1,const RSRLINEArray* pRst2);
	static BOOL  IsEqual(const RSRLINE* pRst1,const RSRLINE* pRst2);
	static BOOL  InSide( const RSRLINEArray* pRst1,const RSRLINEArray* pRst2);
	static BOOL  IsIntersect(const RSRLINE* pRst1,const RSRLINE* pRst2);
	static BOOL  IsIntersect(const RSRLINEArray* pRst1,const RSRLINEArray* pRst2);
	static BOOL  IsEmpty(const RSRLINEArray* pRst);
    static BOOL  OutLineInsideRstline(const LPRSRLINE pRstLines1,const LPRSRLINE pRstLines2);//add by sjm 2001-8-28
	static BOOL  ReviseRasterOnlyText(LPRSRLINE lpRstline1,LPRSRLINE lpRstline2);//add by sjm 2001-8-30
	static BOOL  NormalizeRasterOnlyText(LPRSRLINE lpRaster);//add by sjm 2001-8-30
	// 集合运算
	static BOOL Combine(RSRLINEArray* pDst, const RECT& rcSrc, const RSRLINE* pRaster, int nCombineMode);
	static BOOL ANDPolylines (const RSRLINE* lpRaster1, const RSRLINE* lpRaster2,LPRSRLINEArray lpRasterArray);
	static BOOL ORPolylines  (const RSRLINE* lpRaster1, const RSRLINE* lpRaster2,LPRSRLINEArray lpRasterArray);
	static BOOL DIFFPolylines(const RSRLINE* lpRaster1, const RSRLINE* lpRaster2,LPRSRLINEArray lpRasterArray);
	static BOOL XORPolylines (const RSRLINE* lpRaster1, const RSRLINE* lpRaster2,LPRSRLINEArray lpRasterArray);

	// 创建Raster
	static LPRSRLINE CreateRaster(const RSRLINE*);
	static LPRSRLINE CreateRaster(const RECT& rcRgn, BOOL bRect=TRUE,double dAngle=0,POINT* ptCent=NULL);
	static LPRSRLINE CreateRaster(const RECTD* rcRgn, const ATransform* Trans);
	static LPRSRLINE CreateRaster(const POINT* lpPt, int nCount, int nFillMode=ALTERNATE);
	static LPRSRLINE CreateRaster(const ANLOOP*, int nFillMode=ALTERNATE);

	// 创建RasterArray
	static LPRSRLINEArray CreateRasterArray(const RSRLINE* lpRaster);
	static LPRSRLINEArray CreateRasterArray(const RECT& rcRgn, BOOL bRect=TRUE, double dAngle=0);
	static LPRSRLINEArray CreateRasterArray(const POINT* lpPoint, int nPoint, int nFillMode=ALTERNATE);
	static LPRSRLINEArray CreateRasterArray(const POINT* lpPoint,  int nPoint, DWORD dwWidth);
	static LPRSRLINEArray CreateRasterArray(const POINT* lpPoint, const int* lpCount, int nPoly, int nFillMode=ALTERNATE,BOOL bTextType=FALSE);//modify by sjm 2001-8-31
	static LPRSRLINEArray CreateRasterArray(const RSRLINE* lpRaster1,const RSRLINE* lpRaster2,int nCombineMode);
	static LPRSRLINEArray CreateRasterArray(const RSRLINEArray*);
	static LPRSRLINEArray CreateRasterArray(const RSRLINE* lpRaster1,const RSRLINEArray*,int nCombineMode);
	static LPRSRLINEArray CreateRasterArray(const RECT& rcSrc, const RSRLINEArray*,int nCombineMode);
	static LPRSRLINEArray CreateRasterArray(const RSRLINEArray*,const RSRLINEArray*,int nCombineMode);
	static LPRSRLINEArray CreateRasterArray(const ANLOOPGRP*, int nFillMode=ALTERNATE);
	static LPRSRLINEArray CreateRasterArray(const BYTE* pMask, const RECT& rcBox,int Valid, int Invalid);
	static LPRSRLINEArray CreateRasterArrayText(const  RSRLINE* lpRaster,RSRLINEArray* pDstRasterArray,int nCombineMode);///add by sjm 2001-8-28

	static BOOL Create(RSRLINE* pRaster, const RECTD* rcRgn, const ATransform* Trans);
	static BOOL Create(RSRLINE* pRaster, const RECT& rcRgn, BOOL bRect=TRUE,double dAngle=0,POINT* ptCent=NULL);
	static BOOL Create(RSRLINE* pRaster, const POINT* lpPt, int nCount, int nFillMode=ALTERNATE);
	static BOOL Create(RSRLINEArray* pDst, const RSRLINE* lpRaster1,const RSRLINE* lpRaster2,int nCombineMode);
	static BOOL Create(RSRLINEArray* pDst, const RSRLINE* lpRaster1,const RSRLINEArray*,int nCombineMode);
	static BOOL Create(RSRLINEArray* pDst, const RSRLINEArray*,const RSRLINEArray*,int nCombineMode,const ACycle* pCycle);
	static BOOL Combine(RSRLINEArray* pDst, const RECT& rcSrc, const RSRLINEArray* pSrc,int nCombineMode);

	static BOOL IsValid(const RSRLINE* lpRaster);   // if lpRaster==NULL , return FALSE
	static BOOL IsValid(const RSRLINEArray* lpRst); // if lpRst==NULL , return FALSE

	// 分割扫描线, 返回的扫描线(RSRLINEArray内包含的RSRLINE) 与 Windows 的 Polygon 对应.
	static LPRSRLINEArray DivideUp(const RSRLINEArray* lpRst);
	static BOOL DivideUp(const RSRLINE* lpRaster,RSRLINEArray& RstDiv);

	//-- pSrc AND RECTs of one line, return Array of RSRLINEArray. (used in AImage)
	static int DivideUp(LPRSRLINEArray* pDst, const RSRLINEArray* pSrc, int nHeight, const POINT* ptStart, int* pWidth, int nLength);

	// be DivideUp in one flower(Have OR opt). coord must be flower'coord.
	static LPRSRLINEArray Cycle( const RSRLINEArray* pSrc, const ACycle* pCycle, const RECT* pRect=NULL);
	static BOOL Cycle( RSRLINEArray* pReturn, const RSRLINEArray* lpRstArray, const ACycle* pCycle, const RECT* pRect=NULL);

	// be DivideUp in one flower(Have NO OR opt).
	// input : pSrc   -- flower coord
	// output: pReturn-- all RSRLINEArray are in one flower, and are flower coord
	//         pSit   -- position in pSrc'Box
	// return count of pReturn and pSit. while pReturn==NULL and pSit==NULL, return max number needed.
	static int Cycle( LPRSRLINEArray* pReturn, POINT* pSit, const RSRLINEArray* pSrc, const ACycle* pCycle, const RECT* pRect=NULL);

	//--- pXMap, pYMap'start positions are pSrc'(left,top). pXMap and pYMap are new position of old point
	static BOOL Resample(CRstLines* pDst, const CRstLines* pSrc, const SIZE* szMap, const DWORD* pXMap, const DWORD* pYMap);

	static RECT GetBox(const RSRLINEArray* lpRasterArray);
	static VOID CopyRaster(LPRSRLINE lpRasterD, const RSRLINE* lpRasterS);
	static VOID CopyRasterArray(LPRSRLINEArray lpRasterArrayD, const RSRLINEArray* lpRasterArrayS);
	static VOID Offset(LPRSRLINE lpRaster,int nXOffset, int nYOffset);
	static VOID Offset(LPRSRLINEArray lpRstArray,int nXOffset, int nYOffset);

	static VOID Release(LPRSRLINEArray lpRSTArray);

	static VOID Delete(LPRSRLINE&);
	static VOID Delete(LPRSRLINEArray&);

	// be moved in flower0( Up_Left in flower0 ). coord must be flower'coord.
	static VOID  CalLocal(LPRSRLINEArray lpRasterArray, const ACycle* pCycle=NULL);

	static BOOL Check(const RSRLINEArray*);
	static BOOL SetRasterMaskInBuf(LPBYTE pMask,const RECT& rcBox, LPRSRLINE lpRaster,int valid);

private:
	static void SetRasterBox(LPRSRLINE lpRaster,int nNumPoint, int nNumLines,int Ln);
	static BOOL JudgeDirectionFromX(LPPOINT lpPtAry, int nSize);

public:
	//用于判断闭和多边形的绘制方向,返回TRUE为顺时针方向，反之为逆时针方向；
	static BOOL JudgeDirection(const POINT* lpPt, int nCount);

public:
	//将 pRsrLine 相对于 rcBox 的中线左右镜向
	static void HorMirror(LPRSRLINE pRsrLine, const CRect& rcBox);
	//将 pRsrLine 相对于 rcBox 的中线上下镜向
	static void VerMirror(LPRSRLINE pRsrLine, const CRect& rcBox);
	//将 pRasterArray 相对于 rcBox 的中线左右镜向
	static void HorMirror(LPRSRLINEArray pRasterArray, const CRect& rcBox);
	//将 pRasterArray 相对于 rcBox 的中线上下镜向
	static void VerMirror(LPRSRLINEArray pRasterArray, const CRect& rcBox);

	//扫描线组相对于自身的外接矩形 m_rcBox 的中线镜向
	void HorMirror();
	void VerMirror();

public:
	BOOL GetBaryCenter(POINTD &PointBC);
	static BOOL GetBaryCenter(LPRSRLINEArray lpRsrAry , POINTD  & point);
	//扫描线缩扩点
	static LPRSRLINEArray ExpandRaster(const LPRSRLINEArray lpSrc, int nAdd);
	static LPRSRLINEArray ContractRaster(const LPRSRLINEArray lpSrc, int nSub);
};
typedef CRstLines* PRstLines;

// 反串操作( RasterArray -> PolyPolygon)
class RP_EXT_CLASS CLoopGrpOpt
{
private:
	LPANLOOPGRP m_lpAnloopGrp;
public:
	CLoopGrpOpt();
	~CLoopGrpOpt();
	
	BOOL  Create(const POINT* lpPoint, const int* lpCount, int nPoly);
	BOOL  Create(const RECT& rcRgn, BOOL bRect=TRUE, double dAngle=0);
	BOOL  Create(const RSRLINEArray* lpRasterArray);
	BOOL  Create(const RSRLINE* lpRaster);
	BOOL  Create(LPANLOOPGRP& lpAnloopGrp);
	BOOL  Create(const CRstLines* pRstLines);
	
	RECT  GetBox() const { return m_lpAnloopGrp->rcBox;}
	LPANLOOPGRP GetData() const { return m_lpAnloopGrp ;}
	
	int   Combine(const CLoopGrpOpt* pLoopGrp1, const CLoopGrpOpt* pLoopGrp2, int nCombineMode);
	BOOL  PtInAnloopGrp(POINT pt);
	int   Copy(CLoopGrpOpt* pLoopGrp);
	int   Offset(int nXOffset, int nYOffset);
	BOOL  Draw(CDC* pDC);
	
public:
	static LPANLOOP CreateAnloop();
	static LPANLOOP CreateAnloop(const RSRLINE*);
	static LPANLOOP CreateAnloop(LPANLOOP lpAnloop);
	static LPANLOOP CreateAnloop(const RECT& rcRgn, BOOL bRect=TRUE, double dAngle=0);
	static LPANLOOP CreateAnloop(const POINT* lpPoint, const int* lpCount, int nNumPoly);
	static LPANLOOPGRP CreateAnloopGrp();
	static LPANLOOPGRP CreateAnloopGrp(const ANLOOPGRP*);
	static LPANLOOPGRP CreateAnloopGrp(const POINT* lpPt1,int nCount1,const POINT* lpPt2,int nCount2,int nCombineMode);
	static LPANLOOPGRP CreateAnloopGrp(const RSRLINEArray* lpRstArray);
	static LPANLOOPGRP CreateAnloopGrp(const ANLOOP*, const ANLOOP*, int nCombineMode);
	static LPANLOOPGRP CreateAnloopGrp(const ANLOOPGRP*, const ANLOOPGRP*, int nCombineMode);
	
	// 反串操作		IntArray.GetSize() == nNumPolygon
	static BOOL RasterToPts( const RSRLINE* lpRaster,LPPOINT& lpPt,int& nNumPoint,LPINT& lpCount,int& nPoly);
	static BOOL GetPolyPolygon( const RSRLINEArray* lpRasterArray, CPOINTArray& ptArray, CintArray& intArray);
	static BOOL GetPolyPolygon( const ANLOOPGRP* lpAnloopGrp, CPOINTArray& ptArray, CintArray& intArray );
	static BOOL GetPolyPolygon( const ANLOOPGRP* lpAnloopGrp,LPPOINT& lpPoint, LPINT& lpCount, int& nNumPoly);
	
	static BOOL IsEmpty( const ANLOOPGRP* lpAnloopGrp );
	static BOOL IsEqual( const ANLOOPGRP* lpAnloopGrp1, const ANLOOPGRP* lpAnloopGrp2 );
	static BOOL PtInAnloop(const ANLOOP* lpAnloop,POINT pt);
	static BOOL PtInAnloopGrp(const ANLOOPGRP* lpAnloopGrp,POINT pt);
	
	static VOID CopyAnloop(LPANLOOP lpAnloop,const ANLOOP* lpAnloopS);
	static VOID MoveLoopGrp(HGLOBAL hLoopGrp, int nX, int nY);
	static VOID MoveAnloopGrp(LPANLOOPGRP ,int nXOffset, int nYOffset);
	static VOID CopyAnloopGrp(LPANLOOPGRP lpAnloopGrpD,const ANLOOPGRP* lpAnloopGrpS);
	
	// before drawing, select GDI object into DC
	static VOID DrawAnloop(CDC* pDC,LPANLOOP lpAnloop);
	static VOID DrawAnloopGrp(CDC* pDC,const ANLOOPGRP* lpAnloopGrp);
	
	static VOID ReleaseAnloop(LPANLOOP lpAnloop);
	static VOID ReleaseAnloopGrp(LPANLOOPGRP);
	
	static VOID DeleteAnloop(LPANLOOP& lpAnloop);
	static VOID DeleteAnloopGrp(LPANLOOPGRP&);
	
protected:
	static LPANLOOPGRP CreateAnloopGrp(const LPANLOOP lpAnloop, BOOL bCopy=TRUE);
};

//功能概要: 颜色组管理类。
//		AClrGroup管理颜色数组，如查找、插入、删除颜色等，及将数据保存到磁盘文件中。
//注意：
//   AClrGroup管理的是真彩颜色组（32位的COLORREF），对Index颜色，请用AByteArray。
//	 AClrGroup几乎未经过性能的优化，如颜色查找是按顺序查找的。它仅提供使用的方便性，
//	因此不适合用于性能要求高的场合（此时应自己优化）。
//
//色组状态{普通，取反，无效}
enum AClrGrpState {cgsNormal=0, cgsInverse, cgsDisable};
class RP_EXT_CLASS AClrGroup : public CObject
{
	DECLARE_SERIAL(AClrGroup)
public:
	AClrGroup();
	~AClrGroup();
	AClrGroup& operator = (const AClrGroup& src);

public:
	//色组状态
	void State(AClrGrpState nState) {m_nState=nState;}
	AClrGrpState State() const {return m_nState;}

public:
	//标志色
	COLORREF FlagColor() const;
	//设定标志色
	//	clr		:[in]标志色颜色值
	//	bAdd	:[in]是否添加到色组中
	//返回：
	//	当clr在当前色组中时，返回其在色组中的序号
	//	当clr不在当前色组中时，根据bAdd参数进行操作
	//		bAdd==TRUE : 添加clr到当前色组末尾
	//		bAdd==FALSE: 用clr替换当前标志色，当不存在当前标志色时，添加到色组末尾
	int FlagColor(COLORREF clr, BOOL bAdd=FALSE);
	//标志色序号
	// Zero-base index in color-array.
	int FlagPosition() const {return m_nFlag;}
	BOOL FlagPosition(int nPos);

	//返回色组中的颜色序列
	const CClrArray& ColorArray() const {return m_Color;}
	//主要接口：[in], 用另一颜色组替换当前颜色组(拷贝一份)
	void ColorArray(const CClrArray& clrArray) {m_Color.Copy(clrArray);}
	//主要接口：[out],得到颜色组的长度。
	int GetCount() const {return m_Color.GetSize();}
	//主要接口：[out],得到颜色组的指针。
	const COLORREF* GetData() const {return m_Color.GetData();}
	//主要接口：[out],得到颜色组的第nIndex号颜色。
	//    nIndex: [in], 指明需要的颜色在颜色组中的序号
	//    return: COLORREF类型的颜色
	COLORREF GetAt(int nIndex) const {return m_Color.GetAt(nIndex);}
	//主要接口：[in], 将颜色设置到颜色组中的第nIndex号位置中。
	//    nIndex: [in], 指明颜色组中的位置
	void SetAt(int nIndex, COLORREF newClr) {m_Color.SetAt(nIndex, newClr);}
	//主要接口：[out], 查找指定颜色在颜色组中的位置。
	//    clr: [in], 指定颜色
	//	  nStart: [in], 开始查找的位置
	// return: [out], 在颜色组中的位置。若找不到，返回NULLINDEX
	int	 Find(COLORREF clr, int nStart=0) const; 
	//主要接口：[out], 查找指定颜色在颜色组中的位置。
	//    pBytes: [in], 指定颜色(用RGB模式表示：B=pBytes[0]，G=pBytes[1]， R= pBytes[2])
	//	  nStart: [in], 开始查找的位置
	// return: [out], 在颜色组中的位置。若找不到，返回NULLINDEX
	int  Find(const BYTE* pBytes, int nStart) const;

	//主要接口：[out], 判断指定颜色是不是在颜色组中.
	// clr:[in], 指定的颜色
	// return: 返回FALSE(TRUE)表示在(不在)颜色组中.
	//注意:
	//	当m_nState==cgsNormal时m_Color就是是颜色组, 
	//	当m_nState==cgsInverse,m_Color以外的颜色是颜色组
	//	当m_nState==cgsDisable，始终返回TRUE
	BOOL NoClaim(COLORREF clr) const; 
	//主要接口：[out], 同NoClaim(COLORREF clr),只是用BYTE数组表示颜色
	BOOL NoClaim(const BYTE* pBytes) const;

	//在色组内部进行颜色值（序号间）移动
	//	nFromIndex	:[in]指定源序号
	//	nToIndex	:[in]指定目的序号
	//注：操作后，nFromIndex所对应的颜色将移动并插入到 nToIndex位置
	BOOL Migrate(int nFromIndex, int nToIndex);
	//主要接口: 向颜色组增加颜色
	//  pColor: [in], 指向被增加的颜色的数据块
	//  nCount: [in], pColor的长度
	//  return: 原颜色组的长度,若失败返回-1
	int  Add(const COLORREF* pColor, int nCount=1);
	//主要接口: 向颜色组插入颜色
	//  nIndex: [in], 插入的位置
	//  pColor: [in], 指向被增加的颜色的数据块
	//  nCount: [in], pColor的长度
	//  return: 实际插入的颜色数
	int  InsertAt(int nIndex, const COLORREF* pColor, int nCount=1);
	//主要接口: 从颜色组中移走颜色
	//  pColor: [in], 要移走的颜色
	//  nCount: [in], pColor的长度
	//  return: 实际移走的颜色数
	int  Remove(const COLORREF* pColor, int nCount=1);
	//主要接口: 从位置nIndex开始,从颜色组中移走nCount个颜色
	//  nIndex: [in], 要移走的颜色起始位置
	//  nCount: [in], 要移走的颜色数
	BOOL RemoveAt(int nIndex, int nCount=1);
	//主要接口: 从颜色组中移走所有颜色
	BOOL RemoveAll();

protected:
	int				m_nFlag;
	CClrArray		m_Color;
	AClrGrpState	m_nState;

public:
	//主要接口: 从文件中读入颜色组
	// pFile: [in], 指向操作的文件, ReadFile直接使用pFile, 因此需要调用者先将pFile打开文件(pFile->Open())
	BOOL ReadFile(CFile* pFile);
	//主要接口: 将颜色组写到文件中, 参数意义同ReadFile(CFile* pFile)
	BOOL SaveFile(CFile* pFile) const;
};

#define MAP_MAXNUM	256
//功能概要: BYTE数组管理类, 主要用于Index颜色组的管理, 该类进行过优化,可用于高性能要求的场合
//  AByteArray管理的最大颜色数为256
class RP_EXT_CLASS AByteArray
{

public:
	AByteArray();

public:
	//主要接口: [in], 将AClrGroup颜色组经转化后复制到当前组中
	AByteArray& operator = (const AClrGroup& src);
	//主要接口: [in], 将AByteArray颜色组复制到当前组中
	AByteArray& operator = (const AByteArray& src);
	//主要接口: [in], 设置颜色组状态(正常模式\取反模式\失效模式)
	void State(AClrGrpState nState) {m_nState=nState;}
	//主要接口: [out], 得到当前的颜色组状态
	AClrGrpState State() const {return m_nState;}
	//主要接口: [out], 得到当前颜色组的一份拷贝, 放在clrGroup中
	int GetClrGrp(AClrGroup& clrGroup) const;

public:
	//主要接口: 向颜色组插入颜色
	//  pByte : [in], 插入的颜色
	//  nCount: [in], pByte的长度(即颜色数)
	void Insert(const BYTE* pByte, int nCount=1);
	//主要接口: 更改颜色组中的颜色
	//  pByte : [in], 插入的颜色
	//  nCount: [in], pByte的长度(即颜色数)
	void Modify(const BYTE* pByte, const BYTE* pNewByte);
	//主要接口: 从颜色组移走颜色
	//  pByte : [in], 要移走的颜色
	//  nCount: [in], pByte的长度(即颜色数)
	void Remove(const BYTE* pByte, int nCount=1);
	//主要接口: 清空颜色组中的颜色
	void EmptyAll();
	BOOL IsNull() const;
	BOOL IsAll() const;
	//主要接口：[out], 判断指定颜色是不是在颜色组中.
	// pByte:[in], 指定的颜色
	// return: 返回FALSE(TRUE)表示在(不在)颜色组中.
	//注意:
	//	当m_nState==cgsNormal时m_Color就是是颜色组, 
	//	当m_nState==cgsInverse,m_Color以外的颜色是颜色组
	//	当m_nState==cgsDisable，始终返回TRUE
	BOOL NoClaim(const BYTE* pByte) const; //Protected, Transparent, ... 
	
	//返回当前的BYTE色组缓冲区（为方便操作，请谨慎使用）
	const BYTE* IndexMapPointer(WORD* pSize=NULL) const
	{ if (pSize) *pSize=m_nSize; return m_bIndexMap; }
protected:
	AClrGrpState	m_nState;
	BYTE			m_bIndexMap[MAP_MAXNUM];	// size is 256, Only set 0 or 1.
	WORD			m_nSize;					// max is 256, m_bIndexMap 中的有效位（在映射表中有效）个数
};

typedef CArray<BGRVALUE, BGRVALUE>	CBgrArray;	

//功能概要: 颜色组管理类, 主要用于CBgrArray颜色组的管理, 该类进行过优化,可用于高性能要求的场合
// 功能类似于AClrGroup,AByteArray
class RP_EXT_CLASS ABgrArray : protected CBgrArray
{
public:
	ABgrArray() {m_nState=cgsNormal;}

public:
	ABgrArray& operator = (const AClrGroup& src);
	ABgrArray& operator = (const ABgrArray& src);
	void State(AClrGrpState nState) {m_nState=nState;}
	AClrGrpState State() const {return m_nState;}
	int GetClrGrp(AClrGroup& clrGroup) const;

public:
	void Insert(const BGRVALUE* pBgr, int nCount=1);
	void Modify(const BGRVALUE* pBgr, const BGRVALUE* pNewBgr);
	void Remove(const BGRVALUE* pBgr, int nCount=1);
	void EmptyAll() {RemoveAll();}
	BOOL IsNull() const {return m_nState==cgsDisable || (m_nState==cgsNormal && m_nSize==0);}
	BOOL IsAll() const {return m_nState==cgsInverse && m_nSize==0;}
	//主要接口：[out], 判断指定颜色是不是在颜色组中.
	// pBgr:[in], 指定的颜色(RGB)
	// return: 返回FALSE(TRUE)表示在(不在)颜色组中.
	//注意:
	//	当m_nState==cgsNormal时m_Color就是是颜色组, 
	//	当m_nState==cgsInverse,m_Color以外的颜色是颜色组
	//	当m_nState==cgsDisable，始终返回TRUE
	BOOL NoClaim(const void* pBgr) const; //Protected, Transparent, ... 

protected:
	void* Search(const void* pBgr) const;	
	int Find(BGRVALUE bgr, int nStart=0) const; // No find, return NULLINDEX.

protected:
	AClrGrpState	m_nState;
};

// Following structs speciel for AnSeries / RIP file
struct ANPRIHEADER
{
    DWORD		abhSize; //  Specifies the number of bytes required by this struct
	CYCLEINFO	cycInfo;
	POINTD		ptdSelf;
	ANPRIHEADER() {abhSize=sizeof(ANPRIHEADER); ptdSelf.x=ptdSelf.y=0.0;}
};
//图像顺序
enum  IMAGE_ORDER	
{
	IMAGE_UNKNOWN		= 0,	//未知顺序
	IMAGE_LINE_DOWNTOUP,		//从下到上行序
	IMAGE_LINE_UPTODOWN,		//从上到下行序
	IMAGE_ROW_LEFTTORIGHT,		//从左到右列序
	IMAGE_ROW_RIGHTTOLEFT,		//从右到左列序
	IMAGE_BLOCK_DOWNTOUP,		//从下到上块序
	IMAGE_BLOCK_UPTODOWN		//从上到下块序
};
//图像文件信息，主要用于文件操作
//注：
//	修改本数据结构时，仅允许在结构尾部添加变量，且不允许修改或删除 Reserve 以前的参数类型
//	相关：AFile, inside.h

struct RP_EXT_CLASS AIMAGEINFO
{
public:
	enum TYPE
	{
		UNKNOWN=0,
		INDEX,		/* Index */
		GRAY,		/* monochrome */
		RGB,		/* red/green/blue */
		YCbCr,		/* Y/Cb/Cr (also known as YUV) */
		CMYK,		/* C/M/Y/K */
		YCCK,		/* Y/Cb/Cr/K */
		Lab
	};
public: //Init
	AIMAGEINFO();
	~AIMAGEINFO();
	
public:
	Int32			Width,Height;		//Pixel
	WORD			BitCount;
	WORD			Compression;		//压缩方式
	
	Float64			XRes, YRes; 		//dpmm
	
	PALETTEENTRY	Palette[256]; 		//BYTE:    peRed,peGreen,peBlue,peFlags;
	WORD			palNumUsed;			// 1--256
	
	TYPE			Type;				//Image类型, 如RGB,GRAY,INDEX,CMYK等
	BYTE			BitChannel; 		//8或16位/通道
	
	DWORD			Reserve;			//保留
	//---------------以上的参数类型及参数个数均不得修改 --------------------------//

	//扩展调色板信息
	KY_EXPANDPALETTE	m_expandPalette;	//扩展调色板信息

};
//图像信息，主要用于系统图像处理，可从AIMAGEINFO中转换得到
#pragma pack(push)
#pragma pack(1)
struct RP_EXT_CLASS APictureInfo
{
	// 宽高: Pixel
	LONG		Width; 
	LONG		Height;
	WORD		BitCount;

	// 精度值：单位为 g_Flags.ResUnit;
	double		XRes, YRes; 
	double		XOutRes, YOutRes;	//	Used by paint, transform graph to image, screen etc.
	
	CPalette*	pPalette;
	WORD		Compression; 

	KY_EXPANDPALETTE	m_expandPalette;	//扩展调色板信息

public:
	APictureInfo();	
	~APictureInfo();
	virtual void Copy(const APictureInfo* pSrc);
	virtual void SetDefaultValue();
	virtual BOOL DeleteContents();

	//AIMAGEINFO --> APictureInfo
	BOOL Transform(const AIMAGEINFO* pImageInfo);
	//APictureInfo --> AIMAGEINFO
	void GetIMAGEINFO (AIMAGEINFO* pImageInfo) const;
};
#pragma pack(pop)

class RP_EXT_CLASS APicture : public ADocObj, public ACalCoord
{
//	Picture operations : select, rotate, move, resize etc.
public:
	APicture();
	~APicture();

public:
//	Operators
	APicture& operator = (const APicture& src);

public:
	//内部是对 pInfo 的引用操作，仅当返回值为FALSE时需要外部函数释放 pInfo 资源
	virtual BOOL Create(const APictureInfo* pInfo=NULL);
	virtual BOOL Delete();

public:
	virtual BOOL Resize(int nWidth, int nHeight); // If nWidth or nHeight < 0 that resize along negative direction
	
	virtual BOOL Scale(DWORD dwXScale, DWORD dwYScale);
	virtual BOOL Resample(DWORD dwXScale, DWORD dwYScale);
	virtual BOOL SyntonizeRes(DWORD dwXScale, DWORD dwYScale);

	virtual BOOL ScaleTo(int nWidth, int nHeight);
	virtual BOOL ResampleTo(int nWidth, int nHeight);
	virtual BOOL SyntonizeResTo(int nWidth, int nHeight);

public:
	virtual const APictureInfo* Info() const {return &m_Info;}
	int Width () const {return m_Info.Width;}
	int Height () const {return m_Info.Height;}
	virtual WORD BitCount () const {return m_Info.BitCount;}
	
	// 精度值：单位为 g_Flags.ResUnit;
	double XRes () const {return m_Info.XRes;}
	double YRes () const {return m_Info.YRes;}
	double XOutRes () const {return m_Info.XOutRes;}
	double YOutRes () const {return m_Info.YOutRes;}

	WORD Compress () const {return m_Info.Compression;}

	virtual void SetWidth (int nWidth);
	virtual void SetHeight (int nHeight);
	virtual void SetBitCount (WORD wBitCount);

	// 精度值：单位为 g_Flags.ResUnit;
	virtual BOOL SetXRes (double fXRes);
	virtual BOOL SetYRes (double fYRes);
	virtual BOOL SetXOutRes (double fXRes);
	virtual BOOL SetYOutRes (double fYRes);
	virtual BOOL SetRes (double fXRes, double fYRes);
	virtual BOOL SetOutRes (double fXRes, double fYRes);
	
	virtual void SetCompress (WORD wCompress);

	virtual void XOutScaleChanged();
	virtual void YOutScaleChanged();

public:
	int CalSelfCycle (POINT* pptSrc) const; // Only return 1 or 0
	int CalSelfCycle (const APTLENGTH* pscSrc, APTLENGTHArray* pScArray, CintArray* pIntArray) const;
	int CalSelfCycle (const RECT* prcSrc, CRECTArray* pRcArray, CPOINTArray* pPtArray) const;
	//--- return count of prstArray and pSit. while prstArray and pSit are NULL, return max count
	int CalSelfCycle (const CRstLines* prstFlw, PRstLines* prstArray, POINT* pSit) const;

protected:
	APictureInfo	m_Info;

public:
	void AddView(CView* pView);
	void RemoveView(CView* pView);
	const CPtrList& GetViewList() const {return m_viewList;}
	void SetViewList(const CPtrList& viewList, BOOL bNotify=TRUE);

protected:			 				 
	virtual POSITION GetFirstViewPosition() const;
	virtual CView* GetNextView(POSITION& rPosition) const;
	// Update Views (simple update - DAG only)
	void UpdateAllViews(CView* pSender);
	// Special notifications
	virtual void OnChangedViewList(); // after Add or Remove view
	virtual void UpdateFrameCounts();
protected:			 
	CPtrList	m_viewList;

public:
	//只设置相应的精度参数，不改变其它属性
	BOOL SetSameRes (const APictureInfo* pInfo);

public:
	//	Coordination is relative to layout
	//	If prcPaint is NULL, paint on all area of related views.
	//	If pView is NULL, paint for all view
	virtual void Repaint(AScrollView* pView, const RECT* prcPaint);

public:
	virtual const CPalette* Palette() const;
	virtual DWORD ColorUsed () const;

	//设置调色板
	//bResetExpandPalette :[in]是否清除扩展调色板信息
	virtual UINT SetPalEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPalEntry, BOOL bResetExpandPalette);
	virtual BOOL SetPalette(const CPalette *pPalette, BOOL bResetExpandPalette);
	//RGB调色板改变
	virtual void PaletteModified();
	//扩展调色板改变
	virtual void ExpandPaletteModified();

	//得扩展调色板
	//	bForce	:[in]是否强制返回内部扩展调色板参数，
	//			当为FALSE时，将对信息的有效性进行判断，无有效扩展调色板信息时返回 NULL
	virtual const KY_EXPANDPALETTE* GetExpandPalette(BOOL bForce) const;
	//得扩展调色板
	//	bForce	:[in]是否强制返回内部扩展调色板参数，
	//			当为FALSE时，将对信息的有效性进行判断，无有效扩展调色板信息时返回 0
	//返回：调色板颜色数
	virtual int GetExpandPalette(KY_EXPANDPALETTE &expandPalette, BOOL bForce) const;
	//设扩展调色板
	//	bGray :[in]是否为灰度稿/色调稿，当为TRUE时忽略nClrUsed，仅取第一个通道色信息
	virtual BOOL SetExpandPalette(const KY_EXPANDPALETTE *pExpandPalette, int nClrUsed, BOOL bGray);
	//扩展调色板使能
	virtual BOOL ExpandPaletteEnable() const;
	virtual BOOL ExpandPaletteEnable(BOOL bEnable);
	//是否支持扩展调色板操作
	virtual BOOL SupportExpandPalette() const;

protected:
	BOOL m_bExpandPaletteEnable;	//是否使用扩展调色板
};

//////////////////////////////////////////////////////////
//	ELLPARAM结构: 表示椭圆一般二次方程的系数
//	椭圆一般二次方程: dPA*X*X + dPB*X*Y + dPC*Y*Y + dPD*X + dPE*Y + dPF = 0
//	所有的系数均用双精度类型表示
//	dPA: X的二次项系数
//	dPB: X*Y的二次项系数
//	dPC: Y的二次项系数
//	dPD: X的一次项系数
//	dPE: Y的一次项系数
//	dPF: 常数项系数
//
struct RP_EXT_CLASS ELLPARAM
{
	ELLPARAM();
	ELLPARAM(const ELLPARAM& eParam);

	double dPA, dPB, dPC, dPD, dPE, dPF;
};

// ELLBOUND结构: 表示椭圆的外形参数
// ptBound: 椭圆的两条水平切线与两条垂直切线
//			与椭圆相切的四个切点
// pptBound[0] : 上切点
// pptBound[1] : 左切点
// pptBound[2] : 下切点
// pptBound[3] : 右切点
// ptCenter: 椭圆的中心点
// rcBorder: 椭圆的外接矩形
//
struct RP_EXT_CLASS ELLBOUND
{
	POINTD	ptBound[4];
	POINTD	ptCenter;
	CRect	rcBorder;
};

//椭圆离散时使用的函数类型
typedef BOOL (*ELLSEPPROC)(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);

//////////////////////////////////////////////////////
// CEllipse类: 采用中点法离散椭圆,矩形,直线
//
class RP_EXT_CLASS CEllipse
{
public:
	// Get seperate point
	//直线离散函数
	//	ptStart, ptEnd: [in]两点表示被离散的直线
	//	ptArray       : [out]POINT结构数组，接收直线的离散点
	static void MidPointLine(POINT ptStart, POINT ptEnd, CPOINTArray& ptArray);

	//矩形离散函数
	//	rect   : [in]表示被离散的矩形
	//	dAngle : [in]矩形的自转角度，用弧度表示
	//	ptArray: [out]POINT结构数组，接收矩形的离散点
	static void MidPointRect(const RECT& rect, double dAngle, CPOINTArray& ptArray)
	{ BOUNDANGLE(dAngle); POINT pt[4]; RotateRect(rect, dAngle, pt); MidPointParalgram(pt, ptArray); }
	static void MidPointRect(const RECT& rect, const ATransform& Trans, CPOINTArray& ptArray)
	{ POINT pt[4]; RectVertex(rect, pt); Trans.CalcPoints(pt, 4); MidPointParalgram(pt, ptArray); }

	// 四边形离散函数
	//	pt     : [in]表示被离散的四边形的四个顶点
	//	ptArray: [out]POINT结构数组，接收四边形的离散点
	static void MidPointParalgram(const POINT pt[4], CPOINTArray& ptArray);


	// 圆离散函数
	//	rc     : [in]表示圆的外接矩形
	//	ptArray: [out]POINT结构数组，接收圆的离散点
	static void MidPointCircle(const CRect& rc, CPOINTArray&);
	// 椭圆离散函数
	//	rect   : [in]表示被离散的正椭圆的外接矩形
	//	dAngle : [in]正椭圆的自转角度，用弧度表示
	//	Trans  : [in]正椭圆的的变换矩阵
	//	ptArray: [out]POINT结构数组，接收椭圆的离散点
	static void MidPointEllipse(const RECT& rc, double dAngle, CPOINTArray&);
	static void MidPointEllipse(const RECT& rc, const ATransform& Trans, CPOINTArray& ptArray)
	{ RECTD rcd(rc); MidPointEllipse(rcd, Trans, ptArray); }
	static void MidPointEllipse(const RECTD& rc, const ATransform& Trans, CPOINTArray&);

	// 椭圆弧离散函数
	//	rect   : [in]表示被离散的正椭圆的外接矩形
	//	Trans  : [in]正椭圆的变换矩阵
	//	ptArray: [out]POINT结构数组，接收椭圆的离散点
	//	dAngStart, dAngEnd: [in]相对于正椭圆的圆弧的起始角度和终止角度，角度为逆时针方向
	static void MidPointArc(const RECTD& rc, const ATransform& Trans, double dAngStart, double dAngEnd, CPOINTArray&);
	static void MidPointArc(const RECT& rc, const ATransform& Trans, double dAngStart, double dAngEnd, CPOINTArray& ptArray)
	{ RECTD rcd(rc); MidPointArc(rcd, Trans, dAngStart, dAngEnd, ptArray); }

	// 求椭圆上一点的函数
	//	eParam	: [in]表示椭圆的一般二次方程的系数
	//	pptCenter, dAngle: [in]表示所求点与pptCenter点所构成的直线与X轴的夹角为dAngle
	//						如果指针pptCenter为空，则用椭圆的中心点代替。
	//	point	: [out]POINT/POINTD结构，接收所求的点
	static BOOL EllipsePoint(const ELLPARAM& eParam, const POINT* pptCenter, double dAngle, POINT& point);
	static BOOL EllipsePoint(const ELLPARAM& eParam, const POINTD* pptCenter, double dAngle, POINTD& point);

	// 求椭圆上一点的函数
	//	rect	: [in]表示被离散的正椭圆的外接矩形
	//	Trans	: [in]正椭圆的变换矩阵
	//	point	: [out]POINT/POINTD结构，接收所求的点
	//	pptCenter, dAngle: [in]表示所求点与pptCenter点所构成的直线与X轴的夹角为dAngle
	//						如果指针pptCenter为空，则用椭圆的中心点代替。
	static BOOL EllipsePoint(const RECTD& rc, const ATransform& Trans, const POINT* pptCenter, double dAngle, POINT& point);
	static BOOL EllipsePoint(const RECT& rc, const ATransform& Trans, const POINT* pptCenter, double dAngle, POINT& point)
	{ RECTD rcd(rc); return EllipsePoint(rcd, Trans, pptCenter, dAngle, point); }
	static BOOL EllipsePoint(const RECTD& rc, const ATransform& Trans, const POINTD* pptCenter, double dAngle, POINTD& point);
	static BOOL EllipsePoint(const RECT& rc, const ATransform& Trans, const POINTD* pptCenter, double dAngle, POINTD& point)
	{ RECTD rcd(rc); return EllipsePoint(rcd, Trans, pptCenter, dAngle, point); }

	// 求椭圆上一点的函数
	//	eParam	: [in]表示椭圆的一般二次方程的系数
	//	eBound	: [in]表示椭圆的外形参数
	//	dAngle	: [in]所求的点与椭圆的中心点所构成的直线与X轴的夹角
	//	point	: [out]POINT/POINTD结构，接收所求的点
	static BOOL EllipsePoint(const ELLPARAM& eParam, const ELLBOUND& eBound, double dAngle, POINT& point)
	{ POINTD pt; BOOL bRet = EllipsePoint(eParam, &eBound.ptCenter, dAngle, pt); CPrecise::PrecisePoint(pt, point); return bRet; }
	static BOOL EllipsePoint(const ELLPARAM& eParam, const ELLBOUND& eBound, double dAngle, POINTD& point)
	{ return EllipsePoint(eParam, &eBound.ptCenter, dAngle, point); }

public:	//扫描线操作
	//
	// 求直线的行扫描线函数
	//	ptStart, ptEnd: [in]两点表示被离散的直线
	//	ptRelate: [in]根据点ptRelate处于直线的上半区还是下半区，来决定行扫描线相对于直线的上半区还是下半区
	//	nXArray : [out]整型数组，接收所求行扫描线X坐标， nXArray的元素个数表示行扫描线的数目
	//			行扫描的Y坐标从ptStart的Y坐标开始到ptEnd的Y坐标结束
	static void MidPointLine(POINT ptStart, POINT ptEnd, POINT ptRelate, CintArray& nXArray);

	// 求矩形的行扫描线函数
	//	rect   : [in]表示被离散的矩形
	//	dAngle : [in]矩形的自转角度，用弧度表示
	//	pRect  : [in]用于接收外接矩形
	//	Trans  : [in]矩形的变换矩阵
	//	ptArray: [out]POINT结构数组，接收所求行扫描线X坐标， ptArray的元素个数表示行扫描线的数目
	//			ptArray中元素的X分量表示行扫描线左端点的X坐标，ptArray中元素的Y分量表示行扫描线右端点的X坐标
	//	rsrLine: [out]RSRLINE结构，接收所求扫描线
	// 返回值 : 第一条行扫描线的Y坐标值
	static int RectRasterLine(const RECT& rect, double dAngle, CPOINTArray& ptArray, LPRECT pRect = NULL);
	static int RectRasterLine(const RECT& rect, const ATransform& Trans, CPOINTArray& ptArray, LPRECT pRect = NULL);
	static int RectRasterLine(const RECT& rect, double dAngle, RSRLINE& rsrLine);
	static int RectRasterLine(const RECT& rect, const ATransform& Trans, RSRLINE& rsrLine);

	// 求椭圆的行扫描线函数
	//	rect   : [in]表示被离散的椭圆
	//	dAngle : [in]椭圆的自转角度，用弧度表示
	//	Trans  : [in]椭圆的变换矩阵
	//	ptArray: [out]POINT结构数组，接收所求行扫描线X坐标， ptArray的元素个数表示行扫描线的数目
	//			ptArray中元素的X分量表示行扫描线左端点的X坐标，ptArray中元素的Y分量表示行扫描线右端点的X坐标
	//	pRect  : [out]用于接收外接矩形
	//	rsrLine: [out]RSRLINE结构，接收所求扫描线
	// 返回值 : 第一条行扫描线的Y坐标值
	static int EllipseRasterLine(const CRect& rect, double dAngle, CPOINTArray&, LPRECT pRect = NULL);
	static int EllipseRasterLine(const RECTD& rect, const ATransform& Trans, CPOINTArray&, LPRECT pRect = NULL);
	static int EllipseRasterLine(const RECT& rect, const ATransform& Trans, CPOINTArray& ptArray, LPRECT pRect = NULL)
	{ RECTD rcd(rect); return EllipseRasterLine(rcd, Trans, ptArray, pRect); }
	static int EllipseRasterLine(const RECT& rect, double dAngle, RSRLINE& rsrLine);
	static int EllipseRasterLine(const RECT& rect, const ATransform& Trans, RSRLINE& rsrLine);
	static int EllipseRasterLine(const RECTD& rect, const ATransform& Trans, RSRLINE& rsrLine);

public:	// Get Ellipse's parameter
	// 求只有角度变化的椭圆的一般二次方程的系数
	//	只有角度变化的椭圆的一般二次方程:dPA * X * X + dPB * Y * Y + dPC * X * Y - dPD = 0
	//	rect	: [in]表示正椭圆的外接矩形
	//	dAngle	: [in]椭圆的自转角度，用弧度表示
	//	Trans	: [in]椭圆的变换矩阵
	//	dPA, dPB, dPC, dPD: [out]用于接收椭圆的一般二次方程的系数
	//	eParam	: [out]用于接收椭圆的一般二次方程的系数
	static BOOL GetEllipseParam(const RECT& rect, double dAngle, double&, double&, double&, double&);
	static BOOL GetEllipseParam(const RECTD& rect, const ATransform& Trans,
		double&, double&, double&, double&, double&, double&);
	static BOOL GetEllipseParam(const RECT& rect, const ATransform& Trans,
		double& dPA, double& dPB, double& dPC, double& dPD, double& dPE, double& dPF)
	{ RECTD rcd(rect); return GetEllipseParam(rcd, Trans, dPA, dPB, dPC, dPD, dPE, dPF); }
	static BOOL GetEllipseParam(const RECT& rect, const ATransform& Trans, ELLPARAM& eParam)
	{ return GetEllipseParam(rect, Trans, eParam.dPA, eParam.dPB, eParam.dPC, eParam.dPD, eParam.dPE, eParam.dPF); }
	static BOOL GetEllipseParam(const RECTD& rect, const ATransform& Trans, ELLPARAM& eParam)
	{ return GetEllipseParam(rect, Trans, eParam.dPA, eParam.dPB, eParam.dPC, eParam.dPD, eParam.dPE, eParam.dPF); }

public:	// Get Border Rect
	// 求矩形的外接矩形
	//	rcBox   : [in]表示正的矩形
	//	dAngle  : [in]矩形的自转角度，用弧度表示
	//	Trans   : [in]矩形的变换矩阵
	//	rcBorder: [out]用于接收所求的外接矩形
	static void GetRectBorder(const RECT& rcBox, double dAngle, RECT& rcBorder);
	static void GetRectBorder(const RECT& rcBox, const ATransform& Trans, RECT& rcBorder);

	// 求椭圆的外接矩形
	//	rcBox   : [in]表示正椭圆的外接矩形
	//	dAngle  : [in]椭圆的自转角度，用弧度表示
	//	eParam  : [in]表示椭圆的一般二次方程的系数
	//	Trans   : [in]椭圆的变换矩阵
	//	rcBorder: [out]用于接收所求的外接矩形
	static void GetEllipseBorder(const RECT& rcBox, double dAngle, RECT& rcBorder);
	static BOOL GetEllipseBorder(const ELLPARAM& eParam, RECT& rcBorder);
	static BOOL GetEllipseBorder(const RECT& rcBox, const ATransform& Trans, RECT& rcBorder)
	{ 
		ELLPARAM eParam; ATransform TransEll; TransEll.InverseMatrix(Trans);
		if (GetEllipseParam(rcBox, TransEll, eParam)) 
			return GetEllipseBorder(eParam, rcBorder); 
		return FALSE; 
	}
	static BOOL GetEllipseBorder(const RECTD& rcBox, const ATransform& Trans, RECT& rcBorder)
	{ 
		ELLPARAM eParam; ATransform TransEll; TransEll.InverseMatrix(Trans);
		if (GetEllipseParam(rcBox, TransEll, eParam)) 
			return GetEllipseBorder(eParam, rcBorder); 
		return FALSE; 
	}

public:	// Get boundary attributes of ellipse
	// 求椭圆外形参数的函数, 用POINT/POINTD结构表示点
	//	rcBox   : [in]表示正椭圆的外接矩形
	//	Trans	: [in]椭圆的变换矩阵
	//	eParam      : [in]椭圆的一般二次方程的系数
	//	pptBound    : [out]接收椭圆的两条水平切线与两条垂直切线与椭圆相切的四个切点
	// 		pptBound[0] : 上切点
	// 		pptBound[1] : 左切点
	// 		pptBound[2] : 下切点
	// 		pptBound[3] : 右切点
	//	prBorder    : [out]接收椭圆的外接矩形
	//	pptCenter   : out]接收椭圆的中心点
	static BOOL GetEllBoundary(const ELLPARAM& eParam, LPPOINT pptBound, LPRECT prBorder=NULL, LPPOINT pptCenter=NULL);
	static BOOL GetEllBoundary(const ELLPARAM& eParam, LPPOINTD pptBound, LPRECT prBorder=NULL, LPPOINTD pptCenter=NULL);
	static BOOL GetEllBoundary(const RECT& rect, const ATransform& Trans, LPPOINT pptBound, LPRECT prBorder=NULL, LPPOINT pptCenter=NULL)
	{ ELLPARAM eParam; ATransform TransEll; TransEll.InverseMatrix(Trans); if (GetEllipseParam(rect, TransEll, eParam))
		return GetEllBoundary(eParam, pptBound, prBorder, pptCenter); else return FALSE; }
	static BOOL GetEllBoundary(const RECTD& rect, const ATransform& Trans, LPPOINT pptBound, LPRECT prBorder=NULL, LPPOINT pptCenter=NULL)
	{ ELLPARAM eParam; ATransform TransEll; TransEll.InverseMatrix(Trans); if (GetEllipseParam(rect, TransEll, eParam))
		return GetEllBoundary(eParam, pptBound, prBorder, pptCenter); else return FALSE; }
	static BOOL GetEllBoundary(const RECT& rect, const ATransform& Trans, LPPOINTD pptBound, LPRECT prBorder=NULL, LPPOINTD pptCenter=NULL)
	{ ELLPARAM eParam; ATransform TransEll; TransEll.InverseMatrix(Trans); if (GetEllipseParam(rect, TransEll, eParam))
		return GetEllBoundary(eParam, pptBound, prBorder, pptCenter); else return FALSE; }
	static BOOL GetEllBoundary(const RECTD& rect, const ATransform& Trans, LPPOINTD pptBound, LPRECT prBorder=NULL, LPPOINTD pptCenter=NULL)
	{ ELLPARAM eParam; ATransform TransEll; TransEll.InverseMatrix(Trans); if (GetEllipseParam(rect, TransEll, eParam))
		return GetEllBoundary(eParam, pptBound, prBorder, pptCenter); else return FALSE; }
	static BOOL GetEllBoundary(const RECT& rect, const ATransform& Trans, ELLBOUND& eBound)
	{ return GetEllBoundary(rect, Trans, eBound.ptBound, &eBound.rcBorder, &eBound.ptCenter); }
	static BOOL GetEllBoundary(const RECTD& rect, const ATransform& Trans, ELLBOUND& eBound)
	{ return GetEllBoundary(rect, Trans, eBound.ptBound, &eBound.rcBorder, &eBound.ptCenter); }
	static BOOL GetEllBoundary(const ELLPARAM& eParam, ELLBOUND& eBound)
	{ return GetEllBoundary(eParam, eBound.ptBound, &eBound.rcBorder, &eBound.ptCenter); }

	// 把矩形转换为四个顶点表示的函数
	//	rc	: [in]被转换的矩形
	//	pt	: [out]接受四个顶点的数组
	static void RectVertex(const RECT& rc, POINT pt[4])
	{
		pt[1].x = pt[0].x = rc.left;
		pt[3].y = pt[0].y = rc.top;
		pt[1].y = pt[2].y = rc.bottom;
		pt[2].x = pt[3].x = rc.right;
	}

private:
	// Raster line
	static void MidPointCircle(int nRadius, CintArray&);
	static void MidPointEllipse(int nA, int nB, CintArray&);
	static int MidPointEllipseLine(const CRect& rc, double dAngle, CPOINTArray& ptArray, LPRECT pRect = NULL);
	// Separate point
	static void MidPointEllipse(const CRect& rc, CPOINTArray&);

	static int	RotateRect(const RECT& rect, double dAngle, POINT pt[4]);
	static void PlgRasterLine(const POINT pt[4], const POINT& ptC, int nLeft, int nRight, int nTop, int nBottom, CPOINTArray& ptArray);

	static BOOL GetRightPoint(double dPA, double dPB, double dPC, double dPD, POINT& pt);
	static BOOL GetTopPoint(double dPA, double dPB, double dPC, double dPD, POINT& pt);
	static BOOL GetTopPoint(double dPA, double dPB, double dPC, double dPD, double dPE, double dPF, POINT& pt);
	static BOOL GetTopPoint(const ELLPARAM& eParam, POINT& pt)
	{ return GetTopPoint(eParam.dPA, eParam.dPB, eParam.dPC, eParam.dPD, eParam.dPE, eParam.dPF, pt); }
	static void PrecisePoint(double dx, double dy, POINT& pt);

	static int	EllPointSite(const ELLPARAM& eParam, const POINTD& point);
	static int	EllPointSite(const ELLPARAM& eParam, const POINT& point)
	{ POINTD pt(point); return EllPointSite(eParam, pt); }

	static BOOL EllSep0(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);
	static BOOL EllSep1(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);
	static BOOL EllSep2(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);
	static BOOL EllSep3(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);
	static BOOL EllSep4(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);
	static BOOL EllSep5(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);
	static BOOL EllSep6(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);
	static BOOL EllSep7(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);

	static ELLSEPPROC m_EllSepProc[8];
};
//精确的耗时计算
//操作:
// CCostTime costTime;
// double dCostTime, dMaxTime, dMinTime, dCurrentTime;
// costTime.Init();
// costTime.Time_Init();
// costTime.Time_Exit();
// dCostTime = costTime.GetCostTime(&dMaxTime, &dMinTime, &dCurrentTime); //xxxx.xx ms
class RP_EXT_CLASS CCostTime
{
private:
	LONGLONG	m_lFrequency;				//当为0 时，用 GetTickCount()取值
	LONGLONG	m_lPerformanceCountStart;

protected:
	LONGLONG	m_lTotalCount;
	LONGLONG	m_lMin, m_lMax;
	LONGLONG	m_lCurrent;
	int			m_nSize;
	DWORD		m_dwMinFlag, m_dwMaxFlag, m_dwCurrentFlag;

public:
	CCostTime(void);
	~CCostTime(void);
public:
	void Init();
	//dwFlag:可以增加特殊的标记，表示当前计时的状态
	void Time_Init(DWORD dwCurrentFlag);
	void Time_Exit();
	double GetCostTime(double *pMaxTime, double *pMinTime, double *pCurrentTime) const; //xxxx.xx ms
	BOOL GetCostTimeFlag(DWORD *pdwMaxTimeFlag, DWORD *pdwMinTimeFlag, DWORD *pdwCurrentFlag) const;
	//得总时间
	double GetTotoalTime() const;	//xxxx.xx ms
};

class RP_EXT_CLASS CErrorInfo
{
public:
	CErrorInfo();
	~CErrorInfo();

protected:
	BOOL				m_bError;		//错误状态，FALSE表示无错误
	CString				m_strError;		//错误信息

	//临界操作，用于确保不被同进程的不同线程同时调用
	CRITICAL_SECTION	m_critical;	
public:
	//得最后的错误信息
	//返回：
	//	TRUE ：存在错误，strError 有效
	//	FALSE：无错误，忽略 strError
	BOOL IsError() const;
	BOOL GetLastError(CString &strError) const;

	//设置最后的错误信息
	//	hInstance, nID	:[in]错误ID所在的资源实例句柄，内部根据此信息自动获取字符串值
	//						注：当无法正常获取字符串信息时，返回FALSE
	//	lpFormat,...	:[in]错误信息字符串（可指定格式）
	//注：
	//	当参数值为NULL时，自动清除错误信息参数值，即 ClearError()
	BOOL SetLastError(HINSTANCE hInstance, UINT nID);
	BOOL SetLastError(LPCTSTR lpFormat, ...);
	
	//根据系统错误代码设置错误信息
	//	dwErrCode		:[in]全局错误代码，同 Windows SDK中的::GetLastError()定义，如 ERROR_OUTOFMEMORY (WinError.h)
	BOOL SetLastErrorFromSystem(DWORD dwErrCode);
	BOOL SetLastErrorFromSystem();

	//清除所有错误信息
	void ClearError();

public:
	//转换 windows SDK 中的系统错误代码（::GetLastError()）到字符串
	static CString TransSystemErrorCodeToString(DWORD dwErrCode);

};


class RP_EXT_CLASS CInkCountInfo
{
public:
	CInkCountInfo();
	~CInkCountInfo();

protected:
	static BYTE			m_bMap[256];

	//临界操作，用于确保不被同进程的不同线程同时调用
	CRITICAL_SECTION	m_critical;	

	ULONGLONG			m_ulTotalCount;		//数据总量
	ULONGLONG			m_ulValidCount;		//有效数据总量，<= m_ulTotalCount

public:
	static void InitMap();

	//初始化计数值
	void Init();

	//得当前统计信息
	ULONGLONG GetValidCount(ULONGLONG *pulTotalCount) const;

	//添加信息行
	//	pLineData		:[in]数据行缓冲区
	//	width			:[in]数据行缓冲区对应的点子数，Pixel
	//	dwValidCount	:[in]有效点子数（为1）
	BOOL AddLineInfo_Mono(BYTE *pLineData, DWORD width);
	BOOL AddLineInfo_Index(BYTE *pLineData, DWORD width);
	BOOL AddLineInfo(DWORD dwValidCount, DWORD width);

public:
	static DWORD GetValidDataCount_Mono(BYTE *pLineData, DWORD width);
	static DWORD GetValidDataCount_Index(BYTE *pLineData, DWORD width);

};

//以下全局变量实体请在应用程序中定义
extern RP_EXT_DATA AGFLAGS			g_Flags;				//全局参数
extern RP_EXT_DATA AStatusProgress	g_progress;				//进度条显示，主程序中需通过 SetPaintWnd 来设定全局进度条显示位置
extern RP_EXT_DATA AEnvObj			g_EnvObj;				//环境参数
extern RP_EXT_DATA CAppPath			g_AppPath;				//系统路径
extern RP_EXT_DATA AGV				g_AGV;					//缓冲区操作（申请、释放）封装类
extern RP_EXT_DATA AUndo			g_Undo;					//全局UNDO，仅适用于RIP应用程序中

extern RP_EXT_DATA HCURSOR			g_hCursor;				//全局光标

extern RP_EXT_DATA ARGBToCMYK		g_RGBtoCMYK;			//全局输出 RGB<-->CMYK、RGB<-->LAB之间的转换封装类（基于BYTE）
extern RP_EXT_DATA ARGBToCMYK		g_InputCMYK;			//全局输入 RGB<-->CMYK、RGB<-->LAB之间的转换封装类（基于BYTE）

extern RP_EXT_DATA CErrorInfo		g_Error;				//全局错误信息

extern RP_EXT_DATA CString			g_strAppVersion;		//应用程序版本信息（全局）

