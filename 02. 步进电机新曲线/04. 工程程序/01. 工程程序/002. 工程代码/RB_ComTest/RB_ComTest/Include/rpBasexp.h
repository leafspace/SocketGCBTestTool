///////////////////////////////////////////////////////////////////////////
// rpBasexp.h : export's headerfile of rpBase.dll 
//
//	�ṩ������Ĳ������壬�����������塢ѹ���㷨��MFC������չ��
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

//	�ַ�����Դ��ȡ����
//ע������ַ�����Ϊ _ANX_LOADSTRING_MAX_
#define _ANX_LOADSTRING_MAX_		1000
RP_EXT_API CString AnxLoadString(HINSTANCE hInstance, UINT uID);

// ���������ĶԻ���״̬����ʾ����
//	hInstance			:[in]ָ����Ӧ�ַ���ID(idHeadStr,idEndStr)��Ӧ�Ĺ��̾����
//							����ΪNULL����ΪNULLʱ�������ô��� 
//	idHeadStr, idEndStr :[in]ָ����ʾ�ַ���ID������Ϊ0����ʾ����Ҫ��Ӧ�ַ���ʾ��Ϣ��
//	lpFormat,...		:[in]�м��ַ�������ʹ�ø�ʽ���ַ�������Ӧ�����������ɲο� printf
//	fuStyle				:[in]�Ի�����
//ע���������������ʽ��[idHeadStr + ] lpFormat [+ idEndStr]
//
//�Ի�����ʾ
// ����˵���ɲ��� AfxMessageBox ����Help��
RP_EXT_API int AFX_CDECL AnxDialogMsg (LPCTSTR lpFormat, ...);	// Used in debug
RP_EXT_API int AFX_CDECL AnxDialogMsg (HINSTANCE hInstance, UINT idHeadStr, UINT idEndStr=0, UINT fuStyle=MB_OK, LPCTSTR lpFormat=NULL, ...);
//	״̬����ʾ
RP_EXT_API void AFX_CDECL AnxStatusMsg (LPCTSTR lpFormat, ...); // Used in debug
RP_EXT_API void AFX_CDECL AnxStatusMsg (HINSTANCE hInstance, UINT idHeadStr, UINT idEndStr=0, LPCTSTR lpFormat=NULL, ...);

//�ú�������� Debug �棬�öԻ���/״̬��������ʾ������Ϣ����Release �涨��ΪNULL��
#ifdef	_DEBUG
	#define	DEBUGDLGPROMPT		::AnxDialogMsg
	#define	DEBUGSTATUSPROMPT	::AnxStatusMsg
#else
	#define	DEBUGDLGPROMPT		__noop
	#define	DEBUGSTATUSPROMPT	__noop
#endif
//�������������
//	strSrc				:[out]ԭ��������ֵ
//	pstrDst				:[out]����������ֵ���� bChangePassword==TRUEʱ��Ч
//	bChangePassword		:[in]�Ƿ���������޸Ľ��棬ΪFALSEʱ�����������������
//	bCanEmpty			:[in]�������Ƿ�����Ϊ��
RP_EXT_API BOOL AFX_CDECL AnxPasswordDlg(CString &strSrc, CString *pstrDst, BOOL bChangePassword, BOOL bCanEmpty);


//���ű�������
// dRatio	:[in/out]��ʼֵΪ��ǰ����ı���ֵ������ʱ��Ϊ��һ��С/�Ŵ󼶱�ı���ֵ//
//
// ��ǰ������Сһ��
RP_EXT_API void AnxZoomOutRatio(double& dRatio);
//	��ǰ�����Ŵ�һ��
RP_EXT_API void AnxZoomInRatio(double& dRatio);
//	������ǰ����ֵΪ���������Ŵ����С��
// return dRatio==(int)x or dRatio==1.0/(int)x
RP_EXT_API void AnxZoomNormalRatio(double& dRatio);

// �󸡵�������ֵ��nPrecision:С����λ����bRound : �Ƿ���������
//ע��AnxFloatPrecision(1.1234567, 4, TRUE) ���� 1.1235
RP_EXT_API double AnxFloatPrecision(double dValue, int nPrecision, BOOL bRound=TRUE);

// �󸡵�������ֵ(�ַ���)��nPrecision:С����λ����bRound : �Ƿ���������
//ע��AnxFloatPrecisionString(1.1234567, 4, TRUE) ���� "1.1235"
#define MAXPRECISION	10			//��󸡵�������λ��
RP_EXT_API CString AnxFloatPrecisionString(double dValue, int nPrecision, BOOL bRound=TRUE);

#define MAX_LIMITWIDTH	8			//ʮ������������ַ�����
//bLimit : ��ʮ������ʱ��Ч����ʾת������ַ�������Ϊ0ʱ��ʾ�Զ������ΪMAX_LIMITWIDTH
RP_EXT_API CString AnxIntString(int nValue, BOOL bHex, BYTE bLimit=0);
RP_EXT_API CString AnxDWORDString(DWORD dwValue, BOOL bHex, BYTE bLimit=0);

//������ת����ֵ
RP_EXT_API int AnxFloatToInt(double dValue, BOOL bRound=TRUE);
RP_EXT_API DWORD AnxFloatToDWORD(double dValue, BOOL bRound=TRUE);


//ʵ�ֲ�ͬ���ڽ��������ת��
//������
//	msg: ������Ϣ������ PeekMessage(...)�õ���msg.message��ֵ������ WM_MOUSEFIRST <= msg.message <= WM_MOUSELAST ��Χ��
//	pDstWnd : Ŀ�Ĵ���ָ�룬Ϊ��ʱ��ת����Դ����ָ���� msg �п��Եõ������ش��룩
//	pDstPt : ����ת����������ֵ����δ���й�ת��ʱҲ���㣩��Ĭ��ΪNULL��ʾ���ü���
//���أ�
//	��������ת���󴰿���Ϣ
RP_EXT_API MSG AnxMapPoint (MSG msg, CWnd* pDstWnd, POINT* pDstPt = NULL);

//��ǰָ�루CObject*���Ƿ���Ч
RP_EXT_API BOOL AnxValidObject(const CObject* pOb);


//��λת������
//

//���ȵ�λ	��AResUnit�����ȵ�λ��ASizeUnit��g_FlagsΪȫ�ֲ�������
// resUnit	��dRes �ľ��ȵ�λ������ΪruDefault
// szUnit	��dValue�ĳ��ȵ�λ������ΪsuDefault
// ���ȼ�����ת��,
//		��λΪresUnit�ľ���dResת��ΪDPI��DPMM��
//		��λΪszUnit�ĳ���dValueת��ΪPIXEL��MM��INCH
// nBufferMax	���ַ�����󳤶�
// bRound		���Ƿ���������ȡ��
// ע��ruDefault/suDefaultָ���ǵ�ǰϵͳʹ�õľ���/���ȣ�g_Flags.ResUnit / g_Flags. SizeUnit��
//
//�������
RP_EXT_API int ToNDPI (double dRes, AResUnit resUnit); 
RP_EXT_API double ToDPI (double dRes, AResUnit resUnit);
RP_EXT_API double ToDPMM (double dRes, AResUnit resUnit);
RP_EXT_API double ConvertRes(double dRes, AResUnit srcUnit, AResUnit dstUnit, BOOL bRound);

//���º����жϾ����Ƿ����
RP_EXT_API BOOL EqualDPI (double dRes1, double dRes2);
RP_EXT_API BOOL EqualDPMM (double dRes1, double dRes2);
RP_EXT_API BOOL EqualRes (double dRes1, double dRes2, AResUnit resUnit, BOOL bRound);
//dRes1,dRes2��������ȡ�����ж�
RP_EXT_API BOOL EqualNDPI (double dRes1, double dRes2);

//���ز�ͬ�ֱ��ʵľ��ȷ�Χ
RP_EXT_API double DefResPrecision (AResUnit resUnit);

//�������뾫��ֵΪ��Ч���þ���
RP_EXT_API double RightRes(double dRes, AResUnit resUnit);

//��ǰ�����Ƿ���Ч
RP_EXT_API BOOL IsValidRes(double dRes, AResUnit resUnit);

//�����ȵ�λתΪ�ַ�����ʾ ("dpi, dpmm, ...")
// nBufferMax:�ַ�����󳤶�
RP_EXT_API int ResUnitString (LPTSTR lpBuffer, int nBufferMax, AResUnit resUnit);
RP_EXT_API int ResUnitString (CString& str, AResUnit resUnit);

//������� 
RP_EXT_API int ToNPIXEL (double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
RP_EXT_API double ToPIXEL (double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
RP_EXT_API double ToMM (double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
RP_EXT_API double ToINCH (double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
RP_EXT_API double ConvertSize(double dValue, ASizeUnit srcUnit, ASizeUnit dstUnit, double dRes, AResUnit resUnit, BOOL bRound);

//���ߴ絥λתΪ�ַ�����ʾ ("inch, mm, pixel, ...")
// nBufferMax:�ַ�����󳤶�
RP_EXT_API int SizeUnitString (LPTSTR lpBuffer, int nBufferMax, ASizeUnit szUnit);
RP_EXT_API int SizeUnitString (CString& str, ASizeUnit szUnit);

//���ȵ�λת��
class RP_EXT_CLASS CUnitTrans  
{
public:
	CUnitTrans();
	~CUnitTrans();

public:
	BOOL SetParams(double dValue, double dRes, AResUnit resUnit, ASizeUnit szUnit);
	BOOL SetRes(double dRes, AResUnit resUnit);
public:
	//bNPixel : ��ΪTRUE��szUnit==suPIXELʱ����ʾ�����������������ֵ
	double	GetValue(ASizeUnit szUnit, int nPrecision=COMMONPRECISION, BOOL bNPixel=TRUE) const;
	BOOL IsValid() const;

protected:
	double		m_dMMVal; //MM
	double		m_dMMRes; //dpMM;
};


//POINTD �� ���������
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

//RECTD �� ������β����ඨ
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

//��ȷ�ȴ����ࣨ��������ȣ�
//	��˫��������ת����ȷ����������
class RP_EXT_CLASS CPrecise
{
public:
	//��˫��������ת����ȷ����������
	// ����ת���������ֵ
	//	d	: [in]��ת����˫����ֵ
	//	dp	: [in]ת������ȡ[0, 1.0)֮��
	static int PreciseInt(double d, double dp=0.5)
	{ return d >= 0 ? AnxFloatToInt(d+dp,FALSE) : AnxFloatToInt(d-dp,FALSE); }
	
	//��˫��������ת����ȷ����������
	//	dx, dy	: [in]��ת����˫����ֵ
	//	nx, ny	: [in]����ת���������ֵ
	//	dp		: [in]ת������ȡ[0, 1.0)֮��
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
	// ��˫�������͵� POINTD �ṹת����ȷ��POINT �ṹ
	//	ptd	: [in]��ת����˫����POINTD �ṹ
	//	pt	: [in]����ת�����POINT �ṹ
	//	dp	: [in]ת������,ȡ[0, 1.0)֮
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
// ������Ϊһ 3*3 �ķ���
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

	//��������
	BOOL InverseMatrix ();
	BOOL InverseMatrix (const TransMatrix& trsrc)
	{ LoadMatrix(trsrc); return InverseMatrix(); }
	
	//�����
	//dMatrix is a 3*3  Matrix
	void LoadMatrix (const TransMatrix& trMatrix);
	void SetTransMatrix (double* dMatrix);
	//�þ�������
	void GetTransMatrix (double* dMatrix) const;

	//�Ƿ�Ϊ��λ����
	BOOL IdentityMatrix (void) const;
	//��Ϊ��λ����
	void LoadIdentity () 
	{ eM11 = 1;	eM12 = 0; eM21 = 0;	eM22 = 1; eDx = 0; eDy = 0; }

	BOOL OnlyScaleOffset() const;

	//�����任�����Ƿ�����ƶ�����
	//return 0 : have other trans
	//		-1 : equal
	//		 1 : only offset
	int OnlyOffset (const TransMatrix& transOld, POINTD* pptdOffset) const;

	//XFORM ���Ͳ�����, �� ATransform class ת��Ϊ XFORM struct (SDK����)
	operator XFORM () const;
public:
	//��������
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
	//���µ� ptTrans(dx, dy) : ����任����������

	//ƽ��
	//	ƽ�� dx, dy
	//	eDx = dx, eDy = dy
	void Translation (double dx, double dy);
	void Translation (const ATransform& trform, double dx, double dy)
	{ LoadMatrix (trform); Translation (dx, dy); }
	//ƽ��-->����-->ƽ��
	//	����ڵ� ptTrans �����任 sx, sy
	//	eM11 = sx, eM22 = sy
	void Scaling (double sx, double sy, double dx=0.0, double dy=0.0);
	void Scaling (const ATransform& trform, double sx, double sy, double dx=0.0, double dy=0.0)
	{ LoadMatrix (trform); Scaling (sx, sy, dx, dy); }
	//ƽ��-->��ת-->ƽ��
	//	����ڵ� ptTrans ��ת fangle
	//	eM11 = cos(fangle), eM12 = sin(fangle)
	//	eM21 = -sin(fangle), eM22 = cos(fangle)
	void Rotate (double fangle, double dx=0.0, double dy=0.0);
	void Rotate (const ATransform& trform, double fangle, double dx=0.0, double dy=0.0)
	{ LoadMatrix (trform); Rotate (fangle, dx, dy); }
	//ƽ��-->����-->ƽ��
	//	����ڵ� ptTrans ���� sx, sy
	//	eM12 = sx, eM21 = sy
	void Shear (double sx, double sy, double dx=0.0, double dy=0.0);
	void Shear (const ATransform& trform, double sx, double sy, double dx=0.0, double dy=0.0)
	{ LoadMatrix (trform); Shear (sx, sy, dx, dy); }
	//ƽ��-->��ת-->�Գ�-->��ת-->ƽ��
	// ���ڹ��� (dx, dy)���Ƕ�Ϊ fangle ��ֱ�߶Գ�
	// if not (0 <= fangle < PI) �����(dx, dy)�Գ�
	void Reflection (double fangle=0.0, double dx=0.0, double dy=0.0);
	void Reflection (const ATransform& trform, double fangle=0.0, double dx=0.0, double dy=0.0) 
	{ LoadMatrix (trform); Reflection (fangle, dx,dy); }

public:
	//==============================================================
	//����Ϊ���ݾ�����������
	// dp : [in]point precise (0 <= dp < 1.0)

	//��������������
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

	//Poly ��ʽ�㴮��������
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

	//PolyPoly ��ʽ�㴮��������
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
	//������������
	void CalcDist (double & fDist) const;

	//==============================================================
	//����Ϊ���ݾ�����������
	//	��ʹ�� InverseCalPoint (...) ����ǰ����ʹ�� CanInverseCalc().
	//
	inline BOOL CanInverseCalc () const;		//�Ƿ���Խ��������������

	//��������������
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

	//Poly�㴮��ʽ��������
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

	//PolyPoly�㴮��ʽ��������
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
	
	//����ֵ��������
	void InverseCalcDist (double & fDist) const;

	//����ֽ⣬������ֽ�Ϊ������������ƽ�ơ�������ת��������ͬ����
	// ptS	: [in]Scaling Center;
	// ptR	: [in]Rotate Center;
	// pTrScale, pTrMove, pTrRotate : Receive resoluted transform
	//ע���˺�����δ���ƣ��ݲ�ʹ��
	// Resolution Transform Let Transform = pTrScale * pTrMove * pTrRotate;
	BOOL Resolution(const POINTD& ptS, const POINTD& ptR, ATransform* pTrScale = NULL, ATransform* pTrMove = NULL, ATransform* pTrRotate = NULL) const;

public:
	//	DWORD	ZoomScale; // Device:Logic = HiWord:LoWord

	// pPtOffFV is Flower (0, 0) trans to View coordinate.
	//�赥λ����ı仯��ϵ (Trans 'Flower To Flower' To 'Flower To View'��ϵ)
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
	//�赥λ����ı仯��ϵ (Trans 'View To View' To 'View To Flower'��ϵ)
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

//BITMAP ��չ��
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
//��ָ��λ���ϵ����˵�����ʱ�ã�һ��������������
class RP_EXT_CLASS CMenuWnd : public CWnd
{
	DECLARE_DYNCREATE(CMenuWnd)
		
		// Construction
public:
	CMenuWnd();
	
	// Attributes
public:
	void* m_pData;		//�������������ʱ������Ӧ����������������Ĭ��ΪNULL

	// Operations
public:
	//��ָ��λ�ô��������˵�������ѡ����˳�
	//	hInstance	:[in]nIDMenu��Ӧ��ʵ���������ֵ���������������п��ܴ�������
	//	nIDMenu		:[in]ID of menu, if uIDMenu==0 then failed
	//	point		:[in]screen coord
	//	nFlags		:[in]ָ����Ļλ�ñ�ʶ��������ʶ���μ���CMenu::TrackPopupMenu
	//	pData		:[in]�������������ʱ������Ӧ����������������Ĭ��ΪNULL
	//	nSubMenu	:[in]nIDMenu ���Ӳ˵���ֵ��-1ʱ��ʾ���Ӳ˵�
	//	pParentWnd	:[in]parent windows, if NULL the AfxGetMainWnd()
	//ע������������������Ӧ����������Ӧ����Ӧ�����أ����� OnInitMenuPopup Ҳ����û��
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
	//UINT uId : ��ʾѡ�еĲ˵���ID��uId==0 ��ʾû��ѡ�еĲ˵���
	//ע���˺�����������
	virtual LRESULT MenuCommand (UINT uId);
	//�Բ˵�����б�Ҫ�ĳ�ʼ���������ɲ����أ�
	virtual void InitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	
	//��ָ��λ�ô��������˵�������ѡ����˳�
	//	hInstance	:[in]nIDMenu��Ӧ��ʵ���������ֵ���������������п��ܴ�������
	//	nIDMenu		:[in]ID of menu, if uIDMenu==0 then failed
	//	point		:[in]screen coord
	//	nFlags		:[in]ָ����Ļλ�ñ�ʶ��������ʶ���μ���CMenu::TrackPopupMenu
	//	pData		:[in]�������������ʱ������Ӧ����������������Ĭ��ΪNULL
	//	nSubMenu	:[in]nIDMenu ���Ӳ˵���ֵ��-1ʱ��ʾ���Ӳ˵�
	//	pParentWnd	:[in]parent windows, if NULL the AfxGetMainWnd()
	//ע������������������Ӧ����������Ӧ����Ӧ�����أ����� OnInitMenuPopup Ҳ����û��
	//	MenuCommand (UINT uId);
	//	InitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//	����ɲμ� CMenuWnd::Create() ��ʵ��
	static LRESULT CreateMenu(HINSTANCE hInstance, 
							UINT nIDMenu, CPoint point, 
							UINT nFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON,
							void* pData = NULL,
							int nSubMenu=-1, 
							CWnd* pParentWnd=NULL);
};
//������״̬����ʾ��չ
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

//MFC��ԴIDЭ����
// ע����������ʱ���Զ�����Unlock()�ָ�ԭʼ��Lockǰ����״̬��������ԱҪ�����ֶ����Unlock��ϰ��
class RP_EXT_CLASS CResourceHandle
{
private:
	//ԭʼ��ԴID����ģ����
	HINSTANCE m_hInstanceOld;

public:
	CResourceHandle(void);
	virtual ~CResourceHandle(void);

	//������Դ�������
	//ע���ظ�����ʱ��Ӱ��ԭʼ���ֵ�����ı䵱ǰģ����
	void Lock(HINSTANCE hInstance);

	//������Դ�������
	void Unlock();
};

//���ܸ�Ҫ: ���ڴ��ڴ���ʱ����Ķ��ƴ���(ϵͳ״̬ת��), ��Ҫ���ڲ���ˢ��ʱ,���ڵĲ���(���ƶ���)
//  �Զ��Ͳ���ˢ��ͬ��,ʹCDialogPaint��������ɲ��ؿ��Ǻ�ϵͳ��ˢ��ͬ��,����ϵͳ���Ӷ�.
//  CDialogPaint��ϵͳ״̬(ȫ�ֱ���g_Flags)�й�
//  CDialogPaint��һ������,ԭ����ϵͳ�е����ж������ڶ�Ҫ��CDialogPaint����,��Ȼ�����ˢ������.
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
	//*pnReturn = 0  : ��Ϣδ����
	//*pnReturn < 0 : ��Ϣ�Ѵ���һ��������ٸ���Ϣ������
	//*pnReturn > 0 : ��Ϣ�Ѵ���һ���������ٸ���Ϣ������
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

//���ܸ�Ҫ: ϵͳ����,����ͼ��������ʾ�Ĺ���
class RP_EXT_CLASS AScrollView : public CScrollView, public CDialogPaint
{
protected:
	AScrollView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(AScrollView)

// Attributes
public:
	//�õ�View����Ļ����,��View����ԭ��(���������Ͻ�,������Ļ����)
	//	pPoint: [out]�������ֵ
	//ע��ϵͳ�е�View����,���ϸ�����ԭ����ǻ��ڻ��������Ͻǵ�����,
	//		��ʱ�ſɽ�������ת��(��ViewToFlower���ڲ�ʵ��),��ϵͳ�и�������ϵ
	//		���ն����ڻ��������Ͻ�(Flowerԭ�������½�,����View����ת����Flower
	//		ʱ��Ȼ�Ǵ����Ͻǿ�ʼ��)
	void GetViewPosition(POINT* pPoint) const;

	//ͬGetViewPosition��ֻ�Ƿ��ص���X����
	int GetViewLeftPos() const;
	//ͬGetViewPosition��ֻ�Ƿ��ص���Y����
	int GetViewTopPos() const;

public:
	//����Чͼ����������Ͻ����� (Left, Top)
	void GetPaintLeftTop(POINT* pPoint) const;
	//����Чͼ�������С��Pixel��
	void GetPaintSize(SIZE* pSize) const;
	//����Чͼ�������С
	void GetPaintRect(RECT* pRect) const;

protected:
	POINT	m_ptPaint; // Paint's left-top in view
	SIZE	m_szPaint; // Paint's size in view

public:
	int		m_nReserved0;	//��������������Ϊ��ʱ������
	int		m_nReserved1;
	double	m_dReserved0;
	double	m_dReserved1;

public:
	//�����Ӵ���ָ��λ��(�߼�����)
	//��ע��
	//	�йض���μ�MSDN���й� CScrollView::ScrollToPosition ��˵��
	virtual void AScrollToPosition(POINT pt);		     // replace ScrollToPosition 
	//�����Ӵ���ָ��λ��(�豸����)
	virtual void AScrollToDevicePosition(POINT ptDev);   // replace ScrollToDevicePosition
	//�Ӵ���Թ���һ������
	//������
	//	xAmount / yAmount : x / y ����ľ���
	//��ע��
	//	�йض���μ�MSDN���й� CWnd::ScrollWindow��˵��
	virtual void AScrollWindow(int xAmount, int yAmount);// replace ScrollWindow
	
	//�� OnScroll �����غ����л�õ�ǰ�Ĺ�����λ��
    //ע�����ļ��������������������ж�λʱ����View��λ�㳬�� 33000����ʱ��
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
	//*pnReturn = 0  : ��Ϣδ����
	//*pnReturn < 0 : ��Ϣ�Ѵ���һ��������ٸ���Ϣ������
	//*pnReturn > 0 : ��Ϣ�Ѵ���һ���������ٸ���Ϣ������
	LRESULT CustomPaintStart(UINT message, WPARAM wParam, LPARAM lParam, int* pnReturn=NULL);

public:
	//���� pWnd �� AScrollView ��������ʱ����ֵ������Ϊ 0x00010001
	// ZoomScale = HIWORD(m_ZoomScale) / LOWORD(m_ZoomScale)
	virtual DWORD ZoomScale() const;

protected:
	static AScrollView* m_pActiveView;	//ϵͳ��ǰ����Viewָ��

public:
	//�õ�ǰ����View
	static AScrollView* GetActiveView();
	//�赱ǰ����View
	//���أ�ǰһ�εļ���Viewָ��
	static AScrollView* SetActiveView(AScrollView* pView);

public:
	//������ʾ
	enum VIEWTOVIRTUALTYPE { vvtNull=0, vvtVBitmapToView=0x01, vvtViewToVBitmap=0x02 };
	BOOL ViewToVBitmap() const;	
	BOOL ViewToVBitmap(BOOL bViewToVBitmap);	
	BOOL VBitmapToView() const;	
	BOOL VBitmapToView(BOOL bVBitmapToView);	

protected:
	int				m_nViewToVBitmap;	//����״̬, VIEWTOVIRTUALTYPE �ĺϼ�

public:
	//��غ�̨ˢ���̣߳��뻨�����ݶ�ȡ��أ���������ñ��
	//�˱��һ����ڽ��ж�ͼ�����ݶ����ڴ����ﲻ����к�̨�������������ܣ�ʱ�������ã�Ĭ��ֵΪ normal
	enum BACKGROUND_PAINTFLAG
	{
		backpaint_disable   = 0x00,     //���������еĺ�̨ˢ�¹���

		backpaint_Display   = 0x01,     //����������������ʾ��ˢ��
		backpaint_Thumb     = 0x02,     //������/��Ի����е�Сͼ��ˢ��
		backpaint_Navigation= 0x03,     //����������ҳ�е���ͼˢ��

		backpaint_normal    = 0xFF      //�����������������ݵ�ˢ��
	};
	BYTE			m_bBackground_paintFlag;      //BACKGROUND_PAINTFLAG
};

//���ܸ�Ҫ: ��������, ���ڳ�ʱ�����ʱ,��״̬��ָʾ�����Ľ���. һ�����;�˳�����(��Esc)һ��ʵ�ֵ�
//�÷� 1:
//		1, Ԥ����			:	AProgress::GetStepSize(total_num,&nStep, &nStepSize);
//		2, ��  ��			:	AProgress	pro(nStepSize);
//		3, ������ʾ����		:	pro.SetPaintWnd(...)
//		4, ��������ʶ�ַ���	:	strOld = pro.ProgressName(...);
//		5, ��ʾ����			:	for(..; nCount<..; nCount++) 
//								{ ...
//									if (pro.Step()==VK_ESCAPE)
//									{	...;  break;	}
//								}
//		6, ��λ��������ʶ	:	pro.ProgressName(strOld);
//		7, ����������ʾ		:	pro.End();
//
//�÷� 2 �����ڼ���ֵ�ϴ������һ�㳬�� total_num > 2000 ���ϵĿ��Կ���ʹ�ã�:
//		1, Ԥ����			:	AProgress::GetStepSize(total_num,&nStep, &nStepSize);
//		2, ��  ��			:	pro.Rang(nStepSize)
//		3, ��������ʶ�ַ���	:	strOld = pro.ProgressName(...);
//		5, ��ʾ����			:	for(..; nCount<..; nCount++) 
//								{ ...
//									if ((nCount%nStep)==0 && pro.Step()==VK_ESCAPE)
//									{	...;  break;	}
//								}
//		6, ��λ��������ʶ	:	pro.ProgressName(strOld);
//		7, ����������ʾ		:	pro.End();
//
//�ο���AStatusProgress
class RP_EXT_CLASS AProgress
{
public:
	// ���캯����
	//	nEnd	:[in]����ʱ��ָʾ����ֵ��
	//	nStart	:[in]��ʼʱ��ָʾ����ֵ��
	AProgress(int nEnd=100, int nStart=0);
	virtual ~AProgress();

public:
	// ���ý�������ʾλ��
	//	pWnd	:[in]ָ�����������ڴ��ھ��
	//	prcPaint:[in]ָ���������ڴ����е���ʾ���򣬵�ΪNULLʱ����ʾʹ����������
	//
	//ע���������״̬��λ�ý��н�����ʾ����ʹ��g_progress���в���
	BOOL SetPaintWnd(CWnd* pWnd, const RECT* prcPaint=NULL);

	// ���÷�Χ
	void Range(int nEnd, int nStart=0);

	// ������ǰָʾ�ľ���ֵ
	//����ֵ��������ʾ�����еİ���״̬��ͬ WM_KEYDOWN �����е� msg.wParam
	WPARAM Current(int nPos);
	// ����ָʾ���߶������ľ���ֵ��
	void Step(int nStep);
	// ��ָʾ��ǰ��һ��
	WPARAM Step() {return Current(m_nPos+m_nStep);}
	
	//��ǰ��������������ʾ�ڽ������У�
	CString ProgressName() const {return m_strProgressName; }
	//����ֵ��ǰһ�εĲ���������
	CString ProgressName(const char *pszProgressName);

	//��������������
	void End();

public:	
	//��ǰ�Ƿ��н���������
	//������ڣ����йؽ�����λ���ϵĲ�����Ӧ����ʱֹͣ������ʾ��ǰ�ڴ�״̬��
	BOOL IsRuning();
	//����ˢ������
	void UpdateRect(const RECT* prcPaint);
	//��ˢ�½�����Ϣ
	void Repaint();

public:
	//�������ŵĽ��������Ʋ���ֵ
	//	nTotalSize 			��[in]�ܽ���Ҫ��
	//	nStep��nStepCount	��[out]�����Ĳ���������ȡ����㣩���������������ܽ��ȣ�
	//ʾ����
	// AProgress::GetStepSize(nTotalSize,nStep,nStepCount);
	// AProgress proc(nStepCount);
	// if ((nCurrentPos % nStep)==0 && proc.Step()==VK_ESCAPE) break;
	static void GetStepSize(int nTotalSize, int* nStep, int* nStepCount);

private:
	BOOL CreateMemDC();
	BOOL ReleaseMemDC();

	//	nCurrentPos :[in]����ֵ��<= _PROGRESS_PERCENT_SIZE_
	BOOL DrawProgressInfo(int nCurrentPos);

protected:
	CWnd*	m_pWnd;					//����������λ��
	CRect	m_rcPaint;				//������ʾ����
	int		m_nEnd, m_nStart;		//ָ����������ʾ��Χ
	int		m_nStep;				//���Ȳ���
	CString m_strProgressName;		//��ǰ��������������ʾ�ڽ������У�

protected:
	int		m_nPos;					//��ǰ����ֵ
	int		m_nCurrentOld;			//ǰһ�ν���ֵ
	int		m_nLength;				//�ܽ���ֵ
	BOOL	m_bUp;					//�Ƿ������
	HDC		m_hDC;					//��ʾ��HDC

private:
	HDC			m_hMemDC;			//�ڴ滺�壬������ʾ���̣� drawtext --> m_hMemDC(m_hMemBitmap) --> m_hDC
	HBITMAP		m_hMemBitmap;		//��ǰ Bitmap ��Ϣ������� m_hMemDC��
	HBITMAP		m_hOldBitmap;		//�����ϴε� Bitmap ��Ϣ
	HFONT		m_hOldFont;			//�����ϴε� Font ��Ϣ

	friend class AStatusProgress;
};

//���ܸ�Ҫ: ״̬��λ�õĽ�������, ���ڳ�ʱ�����ʱ,��״̬��ָʾ�����Ľ���. һ�����;�˳�����(��Esc)һ��ʵ�ֵ�
//�÷�:
//		1, ��ʼ������ʾ		:	bProgress = g_progress.Begin(bSubMode, pszProgressName);	�����д˲��ִ���
//		2, ��  ��			:	if (bProgress) g_progress.Rang(nStepSize);
//		3, ��ʾ����			:	for(..; nCount<..; nCount++) 
//								{ ...
//									if (bProgress&& g_progress.Step()==VK_ESCAPE)	��ʹ�� g_progress.Current(i) ָ������λ��
//									{	...;  break;	}
//								}
//		4, ����������ʾ		:	if (bProgress) g_progress.End();			�����д˲��ִ���
//
class CStatusProgressSubInfo;
class RP_EXT_CLASS AStatusProgress
{
public:
	// ���캯����
	AStatusProgress();
	virtual ~AStatusProgress();

public:
	//��ʼ����������
	//	bSubMode		:[in]�Ƿ�Ϊϸ��ģʽ����ϸ��ģʽ�£�����ʵ�ֶ����н�������ĳһ�� nStep �������ϸ�ֽ�����ʾ
	//	pszProgressName	:[in]���õ�ǰ��������������ʾ�ڽ������У���
	//						pszProgressName == NULL����ʹ����һ�ε���ʾ��Ϣ
	//						pszProgressName[0]=='\0'������յ�ǰ������
	//ע���˲��������浱ǰ��������Ϣ����End()ʱ�Զ��ָ�
	BOOL Begin(BOOL bSubMode, const char *pszProgressName);
	//��������������
	//ע���˲������ָ� Beginʱ�������һ�ν�������Ϣ
	void End();

	// ���÷�Χ
	//	nEnd	:[in]����ʱ��ָʾ����ֵ��
	//	nStart	:[in]��ʼʱ��ָʾ����ֵ��
	void Range(int nEnd, int nStart=0);
	// ������ǰָʾ�ľ���ֵ
	//����ֵ��������ʾ�����еİ���״̬��ͬ WM_KEYDOWN �����е� msg.wParam
	WPARAM Current(int nPos);
	// ����ָʾ���߶������ľ���ֵ��
	void Step(int nStep);
	// ��ָʾ��ǰ��һ��
	WPARAM Step();

public:	
	// ���ý�������ʾλ��
	//	pWnd	:[in]ָ�����������ڴ��ھ��
	//	prcPaint:[in]ָ���������ڴ����е���ʾ���򣬵�ΪNULLʱ����ʾʹ����������
	//
	//ע���������״̬��λ�ý��н�����ʾ����ʹ��g_progress���в���
	BOOL SetPaintWnd(CWnd* pWnd, const RECT* prcPaint=NULL);
	//��ǰ�Ƿ��н���������
	//������ڣ����йؽ�����λ���ϵĲ�����Ӧ����ʱֹͣ������ʾ��ǰ�ڴ�״̬��
	BOOL IsRuning();
	//����ˢ������
	void UpdateRect(const RECT* prcPaint);
	//��ˢ�½�����Ϣ
	void Repaint();

protected:
	//���ݵ�ǰ��Ϣ����ʼ��һ���µ� SubInfo
	BOOL BackupProgressInfo();
	//�ָ���һ�β�����Ϣ��ɾ����ǰ SubInfo
	BOOL RestoreProgressInfo();

	//���ݸ����Ľ���ֵ������ֵ������� Range���ý��Ȱٷֱ�ֵ��(0.0-1.0)��
	//	bGlobal :[in]�Ƿ񷵻�ȫ�ֽ���ֵ��ΪFALSEʱ������ϸ�ֽ��Ȱٷֱ�ֵ
	double GetPercent(int nPos, BOOL bGlobal) const;
	//���ݸ����Ľ��Ȱٷֱ�ֵ�ý���������ֵ������� m_progress.Range��
	//	bGlobal :[in]ָ��dPercent�Ƿ�Ϊȫ�ֽ���ֵ��FALSE��ʾΪϸ�ֽ��Ȱٷֱ�ֵ
	int GetPos(double dPercent, BOOL bGlobal) const;
	//���ݸ����Ľ��Ȱٷֱ�ֵ�ý���������ֵ������� Range��
	//	bGlobal :[in]ָ��dPercent�Ƿ�Ϊȫ�ֽ���ֵ��FALSE��ʾΪϸ�ֽ��Ȱٷֱ�ֵ
	int GetSubPos(double dPercent, BOOL bGlobal) const;

private:
	CStatusProgressSubInfo	*m_pSubInfo;	//ϸ�ֲ������ݻ��壬ָ��ǰ�Ĳ�������
	AProgress				m_progress;		//����������

};

class RP_EXT_CLASS APtrList : public CPtrList
{
public:
	APtrList(int nBlockSize = 10);
	~APtrList();
	
public:
	// If no find, return NULLINDEX.
	int FindPosition(POSITION pos) const; 
	//�����Ӧ���
	//	nIdxPos		:[in]ָ������λ�ã�
	//				< 0		: ��ӵ���ͷ
	//				> m_nCount : ��ӵ���β
	//				other	:�嵽ָ��λ��
	POSITION InsertAt(int nIdxPos, void* pNew);
};

//������ֵ�ؼ�
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
	//�Ƿ��������롰-����
	void Negative(BOOL bEnable=TRUE);

	void SetRange (int nMin=INT_MIN, int nMax=INT_MAX);
	void GetRange (int &nMin, int &nMax) const;
	void Hexadecimal(BOOL bHex);
	void CheckValue(int nData, BOOL bForce, BOOL bMsg=TRUE);
	//�������ʱ���ַ�����
	//bLimit : ��ʮ������ʱ��Ч����ʾת������ַ�������Ϊ0ʱ��ʾ�Զ������ΪMAX_LIMITWIDTH
	void SetLimitWidth(BYTE bLimit);

	static int ToInt(CString number, BOOL bHex);
	// if return FALSE then error for lpszText with pData
	// lpszText== "-0"ʱ �ж����󣬵�Ϊ���� MFC ͳһ�������޸�
	static BOOL SimpleScanf(LPCTSTR lpszText, int& nData, BOOL bNegative, BOOL bHex);
	//�����ַ����Ƿ���ȷ
	// bHex : �Ƿ�Ϊʮ����������ֵ
	static BOOL IsStrFormatOK(LPCTSTR lpszText, BOOL bHex);
protected:
	BOOL	m_bNegative;	//�Ƿ��������롰-����
	int		m_nMin, m_nMax;
	BOOL	m_bHex;			//�Ƿ�Ϊʮ����������ֵ
	BYTE	m_bLimit;		//����ʱ���ַ��������ƣ���ʮ�����Ʒ�ʽ��Ч��
	
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
	//�������ʱ���ַ�����
	//bLimit : ��ʮ������ʱ��Ч����ʾת������ַ�������Ϊ0ʱ��ʾ�Զ������ΪMAX_LIMITWIDTH
	void SetLimitWidth(BYTE bLimit);

	static DWORD ToDWORD(CString number, BOOL bHex);
	// if return FALSE then error for lpszText with pData
	// lpszText== "-0"ʱ �ж����󣬵�Ϊ���� MFC ͳһ�������޸�
	static BOOL SimpleScanf(LPCTSTR lpszText, DWORD &dwData, BOOL bHex);
	//��ǰ�����ַ����Ƿ���ȷ
	static BOOL IsStrFormatOK(LPCTSTR lpszText, BOOL bHex);
protected:
	DWORD	m_dwMin;
	DWORD	m_dwMax;
	BOOL	m_bHex;			//�Ƿ�Ϊʮ����������ֵ
	BYTE	m_bLimit;		//����ʱ���ַ��������ƣ���ʮ�����Ʒ�ʽ��Ч��
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

	//�󸡵�������ֵ��nPrecision:С����λ����bRound : �Ƿ���������
	void SetPrecision (int nPrecision=COMMONPRECISION, BOOL bRound=TRUE);
	void GetPrecision (int &nPrecision, BOOL& bRound) const;

	void Negative(BOOL bEnable=TRUE);
	void SetRange (double dMin=DBL_MIN, double dMax=DBL_MAX);
	void GetRange (double &dMin, double &dMax) const;

	void CheckValue(double dData, BOOL bForce, BOOL bMsg=TRUE);

	static double ToDouble(CString number);
	// if return FALSE then error for lpszText with d
	// lpszText== "-0.0"ʱ �ж����󣬵�Ϊ���� MFC ͳһ�������޸�
	static BOOL SimpleFloatParse(LPCTSTR lpszText, void* pData, BOOL bNegative, BOOL bDouble);
	static BOOL IsStrFormatOK(LPCTSTR lpszText);

protected:
	BOOL	m_bNegative;		//�Ƿ��������롰-����
	double	m_dMin, m_dMax;

	int		m_nPrecision;		//С����λ��
	BOOL	m_bRound;			//�Ƿ�������������
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


#define  GRIDCELL_SET_ITEMVALUE		(WM_USER + 201)			//������������ɣ��ؼ�ʧȥ�����ȷ�ϣ����͸�������
															//WPARAM	: MAKELONG(m_Row, m_Col)
															//LPARAM	: this->m_hWnd
#define  GRIDCELL_SET_ACTIVEITEM	(WM_USER + 202)			//��ǰѡ�м��ı���͸�������
															//WPARM		: m_nSelect
															//LPARAM	: this->m_hWnd
									

//CListCtrl���Զ�����ֵ��������������ؼ�
class RP_EXT_CLASS CListCtrlEdit : public CEdit
{
	DECLARE_DYNAMIC (CListCtrlEdit);
public:
	CListCtrlEdit();
	~CListCtrlEdit();
public:
	//	rowIndex, colIndex :[in]�б���еĶ�λ����
	void Edit(int rowIndex,int colIndex);

	BOOL ExitWithoutSetValue(){ return m_bExitWithoutSetValue;}

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void EndEdit();		//������ɺ��򸸴��ڷ��� GRIDCELL_SET_ITEMVALUE ��Ϣ
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
//CListCtrl���Զ�����ֵ��������������ؼ�
class RP_EXT_CLASS CListCtrlFloatEdit : public CFloatEdit
{
	DECLARE_DYNAMIC (CListCtrlFloatEdit);
public:
	CListCtrlFloatEdit();
	~CListCtrlFloatEdit();
public:
	//	rowIndex, colIndex :[in]�б���еĶ�λ����
	void Edit(int rowIndex,int colIndex);

	BOOL ExitWithoutSetValue(){ return m_bExitWithoutSetValue;}

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void EndEdit();		//������ɺ��򸸴��ڷ��� GRIDCELL_SET_ITEMVALUE ��Ϣ
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

//CListCtrl���Զ���CComboBox�ؼ�
class RP_EXT_CLASS CListCtrlComboBox : public CComboBox
{
	DECLARE_DYNAMIC (CListCtrlComboBox);
public:
	CListCtrlComboBox();
	~CListCtrlComboBox();
public:
	//	rowIndex, colIndex :[in]�б���еĶ�λ����
	void Edit(int rIndex,int cIndex);
	BOOL ExitWithoutSetValue(){ return m_bExitWithoutSetValue;}

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void EndEdit();			//������ɺ��򸸴��ڷ��� GRIDCELL_SET_ITEMVALUE ��Ϣ
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
//CListCtrl ��չ��ʵ�ֿ������б���

//�Զ���������
//	pszText		:[in]��ǰ�������ݣ��ַ�����
//	nSubItem	:[in]��ǰ�������ݶ�Ӧ������ţ���ʼֵΪ 0��
//	strText		:[out]�������ַ����������ڲ����ݴ��������Ϣ����󷵻ش��ַ����Ա�������������бȽ�
typedef LRESULT (CALLBACK *CHECKITEMTEXTPROC)(CString &strText, const char *pszText, int nSubItem);
class RP_EXT_CLASS CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();

// Attributes
public:
	//[out]���˫��ʱ�Ĺ��������
	struct DBL_LOCATION
	{
		int nItem;
		int nSubItem;
	};	
	DBL_LOCATION m_dblLoc;

protected:
	//�����ò���
	BOOL m_bRevert;
	int	m_nSortItem;
	BOOL m_bSortEnable;

	CHECKITEMTEXTPROC m_checkItemTextFunc;

public:
	//��������б�ʶ����
	static DWORD GetItemFlag();
	//����Ч����
	static int GetColumnCount(const CListCtrl *pListCtrl);
	//�õ�һ����ѡ����
	static int GetFirstSelected(const CListCtrl *pListCtrl);
	//����ָ���ַ��� ��������-����
	//	pszText			:[in]�ַ���
	//	nStart			:[in]��ʼ����ţ��������� nStart < 0ʱ����ʾ�ӵ�һ����ѡ�е��п�ʼ����
	//	bMatchWholeWord	:[in]ƥ�������ַ���
	//	bMatchCase		:[in]ƥ���Сд
	//���أ�
	//	< 0	: δ�ҵ�ָ���ַ���
	//	>= 0: ���ַ���ƥ�������ţ���ʼֵΪ 0��
	//
	//ע���������Զ�֧��ѭ�����ң�������ֵ nIndex �п��ܻ�С�� nStart��
	//	��ʱ��ʾ�� nStart ��ǰ��������ƥ����ַ������ڡ�
	static int SearchTextFromListCtrl(const CListCtrl *pListCtrl, const char *pszText, 
										int nStart, BOOL bMatchWholeWord, BOOL bMatchCase);

	//�����������
	void SetSortParam(BOOL bRevert, int nInitSortItem, CHECKITEMTEXTPROC checkItemTextFunc);
	//ִ���������
	BOOL SortItemsEx();
	//�����б�ʶ����
	BOOL SetItemDataEx(int nItem);
	
	//��������
	BOOL IsSortEnable() const;
	BOOL SortEnable(BOOL bEnable);

	//��Ч����
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
	dis_normal	= 0x0,	//��ͨ�������⺬��
	dis_imgdata	= 0x1,	//AImageData���
	dis_image,			//AImage	���
	dis_layer,			//ALayer	���
	dis_flower,			//AFlower	���
	dis_layout,			//ALayout	���
	dis_node,
	dis_count
};
struct DISCARDDATA
{
	int		nType;	// DISCARDTYPE, Mask=0x0FFF
	void*	pData;	// for example: pImageData, pImage, pLayer, pFlower, pLayout, ..., decided by nType
};

//UNDO����С��Ԫ������ָ��������UNDO����
//	Ӧ�ó�����ͨ����CUndoCell���������ʵ���Զ���UNDO����
class RP_EXT_CLASS CUndoCell : public CObject
{
	DECLARE_DYNCREATE(CUndoCell)
public:
	CUndoCell();
	virtual ~CUndoCell();

// Operators
public:
	//ϵͳִ��һ��Undo������Ҫ���ڴ˺���ʵ��ʱ����Redo���ݵ�׼��
	virtual void Undo() {}
	//��ǰUndo��Ԫ�Ƿ���Ա�����
	virtual BOOL CanDiscard(DISCARDDATA* pDiscard);
	//��Undo�����ļ�������Undo���ݱ�����ڴ����ļ�ʱ��Ч
	//����ֵ��m_uUnique
	virtual UINT GetTempFileName(LPTSTR szName);
	//��ǰUndo��Ԫ�Ƿ��Ѿ������ɹ�
	// Default is True;
	virtual BOOL CreateSuccessed() { return m_bSuccess; }
	//��ǰUndo��Ԫռ�õĻ�������С
	virtual DWORD WorkSpace() const;

protected:
	PVOID		m_pDiscard;		//Undo��Ԫ�������һ������CanDiscard���ж�
	UINT		m_uUnique;		// unique temp file name��ͬ GetTempFileName ����ֵ����
	BOOL		m_bInFile;		// if m_bInFile then m_uUnique != 0
	BOOL		m_bSuccess;		//Undo��Ԫ�Ƿ񴴽��ɹ�
	DWORD		m_dwWorkSpace;	//Undo��Ԫռ�õĻ�������С
	BOOL		m_bCanDiscard;	//Undo��Ԫ�Ƿ�ɱ� Discard
};
/////////////////////////////////////////////////////////////
//Undo��Ԫ�б�ͨ������ʵ��һ��Undo����
//ע��ϵͳ��Ҫ��һ��Undo��Ϊһ��Undo�����д���
class RP_EXT_CLASS CUndoObList : public CObList
{
	DECLARE_DYNCREATE(CUndoObList)
public:
	CUndoObList(int nBlockSize = 10);
	~CUndoObList();

	//Undo�������Ͷ���(Undo�����磺"Create line...", "Erase..."...
	void SetUndoName(HINSTANCE hInstance, UINT uID);
	void SetUndoName(LPCSTR lpszUndoName);
	void GetUndoName(CString& strUndoName) const;
public:
	CString m_strUndoName;		//Undo����������
	DWORD	m_dwWorkSpace;		//Undo��Ԫ�б�ռ�õĻ�������С

public:
	//Undo��Ԫ�б���ռ��������С
	DWORD WorkSpace() const;
	DWORD CalcWorkSpace();
};
typedef CTypedPtrList<CUndoObList, CObject*>	CUndoCellList;
typedef CTypedPtrList<CPtrList, CUndoCellList*>	CUndoList;
//Undo������װ�࣬ʵ��ϵͳUNDO���Ʒ�װ��ϵͳ�ɻ���AUndoʵ��ȫ��UNDO����
//ע��һ�������ʽ
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
	//Undo / Redo ʵ��
	void Undo();
	void Redo();

	//�½�һ��Undo����(Undo��Ԫ�б�
	void NewSeries(LPCSTR lpszUndoName=NULL);
	
	//���Undo��Ԫ
	BOOL Backup(CUndoCell* pCell);

	//�������ͷţ�ָ����Undo����
	void Discard(DISCARDDATA* pDiscard);

	//������ǰ����ӵ�Undo��Ԫ�б���Ϣ
	//ע����Ӧ�ó����У����ڲ���ʧ�ܣ���Ҫ�������ڴ˲�����������Undo��Ϣ
	void RollBackCurList();

	//�����һ�ε�Undo��Ԫ����
	const CUndoCell* GetLastUndo() const;

	//�Ƿ���Խ���Undo/Redo����
	BOOL CanUndo() const;
	BOOL CanRedo() const;
	//��ջ�������
	void Empty();
	
	//Undo��������
	// nOrder <= -1 means no limite
	void SetUndoOrder(int nOrder);
	int GetUndoOrder() const;
	BOOL IsUndoLimited() const;
	BOOL IsNeedUndo() const;
	//�Ƿ�ɴ���Undo
	BOOL IsNotCreateUndo() const;

	//��ǰUndo������������Undo��Ԫ�б�
	BOOL GetUndoName (CString& strUndoName) const;
	BOOL GetRedoName (CString& strUndoName) const;
	BOOL SetUndoName (LPCSTR lpszUndoName=NULL);

	//ϵͳUndoռ�õĻ�������С
	DWORD WorkSpace() const;

	//ϵͳ�Ƿ�֧��Undo����
	BOOL IsBackuped() const;
	void IsBackuped(BOOL bBackuped);

	//���ƽ���Discard����
	//	BeginDiscardLimit �� EndDiscardLimit����ɶ�ʹ��
	//����ֵ��ǰһ�β�����Discard��ʶ
	BOOL BeginDiscardLimit(BOOL bDiscardLimit = TRUE);
	void EndDiscardLimit(BOOL bDiscardLimit = FALSE);
	//�����Զ���ձ�־
	//����ֵ��ǰһ�β�����Emtpy��ʶ
	BOOL SetEmptyFlag(BOOL bEmtpy);

	//�õ�ǰUndo��Ԫ�б�
	const CUndoList* GetUndoList() const;

private:
	void DeleteTail(const POSITION& pos);
	void DeleteHead(const POSITION& pos);
	void DeleteHead();

private:
	int			m_nUndoOrder;		//Undo�������ƣ�< 0 ��ʾ������
	POSITION	m_UndoPos;			//��ǰUndoλ��
	CUndoCellList*	m_pCellList;	//��ǰUndo��Ԫ�б�
	CUndoList	m_UndoList;			//����Undo���ݣ����Undo��Ԫ�б�

	CString m_strUndoName;			//��ǰUndo����������

	DWORD	m_dwWorkSpace;			//ϵͳUndo��ռ�õĻ�������С

	BOOL	m_IsBackup;				//�Ƿ�����Undo

	BOOL    m_bDiscardLimitFlag;	//���ƽ���Discard������ʶ����ΪTRUEʱ��Undo��Discard������ִ��
									//ע����Move�����delete ������������ν���Discard;
	BOOL    m_bEmptyFlag;			//�Զ����Undo������ʶ����ΪTRUEʱ����ʾ����µ�Undo��Ԫǰ�Զ�������е�Undo����
	
private:
	//Undo Limit flag: 
	//	now only for backup graph object flag, 
	//	if (m_dwBackupLimitFlags & bkGraph) then can't create graph undo.
	DWORD	m_dwBackupLimitFlags;
public:
	enum BACKUPLIMIT{ bkNull=0x0000, bkGraph=0x0001 };
	//�Ƿ������ͼ��Ŀ��Ļָ�����
	//���������ָ�ԭ״̬
	bool BackupGraphLimit(bool bBackupGraph);
	bool BackupGraphLimit() const;

};


//���ܸ�Ҫ: ϵͳ������Ϣ�Ĵ��·��, �統ǰϵͳ��ִ���ļ�·��,FMT·����
class RP_EXT_CLASS CAppPath
{
public:
	CString	m_strApp;			// Applications path
	CString m_strFMT;			// ".\FMT\", file format path, rpFXXX.fmt
	CString m_strDrv;			// ".\DRV\", printer device drivers path, rpPXXX.drv
	CString m_strLib;			// ".\LIB\", rpLXXX.lib
	CString m_strRPT;			// ".\RPT\", tools plugin path, rpTXXX.rpt
	CString m_strProfile;		// ".\Profile\", ɫ��У�������ļ���Ŀ¼
	CString m_strConfig;		// ".\Config\", ����RIP������������Ϣ�����Ŀ¼
	CString m_strChart;			// "./Chart/"
	CString m_strTemplate;		// "./Template/"��ƽ���ģ�����Ŀ¼

	CString m_strDebug;			// "./Debug/"��Debug��Ϣ���Ŀ¼

public:
	CString m_strProfile_Input_RGB;		// ".\Profile\Input\"��������RGB�������ļ�Ŀ¼
	CString m_strProfile_Input_CMYK;	// ".\Profile\InputCMYK\"��������CMYK�������ļ�Ŀ¼
	CString m_strProfile_Monitor;		// ".\Profile\Monitor\"������ʾ�������ļ�Ŀ¼
	CString m_strProfile_Output;		// ".\Profile\Output\"�������������ӡ�������ļ�Ŀ¼
	CString m_strProfile_strCurve;		// ".\Profile\Curve\", �ܶ������ļ�Ŀ¼
	CString m_strProfile_strNameColor;	// ".\Profile\NameColor\",������ɫ�滻�ļ�Ŀ¼

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

	CString			m_strDeviceSN;	//��������Ϣ

public:
	BOOL Init(BOOL bCheckDeviceSN);

	void CheckDeviceSN();
};

class RP_EXT_CLASS ACommonWnd
{
private:
	static void* m_pSplashHandle;	//��������

public:
	static BOOL SetSplashHandle(void *pSplashHandle);

	//��ʾSplash ����
	//	pParentWnd		:[in]�����ھ����ΪNULLʱ��ʾ��������
	//	pszAppVerion	:[in]����汾��Ϣ
	//	pszLogoFileName	:[in]�Զ����Logoͼ���ļ���
	//	dwTimeOut		:[in]���峬ʱ�Զ��˳���Ϊ0ʱ��ʾ�޳�ʱ����λΪ��
	//	bClickToExit	:[in]�Ƿ������ڴ����ϵ���˳�
	//���أ�
	//	��Ӧ��Splash��������������� SplashScreen_Exit�����ͷţ���ΪNULLʱ��ʾ�������ɹ�
	//ע��
	//	�� dwTimeOut �� bClickToExit ��Чʱ������ֵ�������ж��Ƿ�ɹ����ã�
	//	���������� SplashScreen_ProgressInfo �� SplashScreen_Exit �У���Ӧ��������ʱϵͳ���Զ����������Դ
	static void* SplashScreen_Show(CWnd* pParentWnd, const char *pszAppVeriosn, const char *pszLogoFileName, DWORD dwTimeOut, BOOL bClickToExit);
	//��ʾ��Ӧ�Ľ�����Ϣ
	//	pSplashHandle	:[in]Splash�������
	//	pszInfo			:[in]������Ϣ
	static BOOL SplashScreen_ProgressInfo(void* pSplashHandle, const char *pszInfo);
	//��ʾ/���ش���
	static BOOL SplashScreen_Show(void* pSplashHandle, BOOL bShow);
	//�˳� Splash ����
	//	pSplashHandle	:[in]Splash�������
	static BOOL SplashScreen_Exit(void* pSplashHandle);
};

//Ϊ����Ҫ�󣬶���һЩ����/������������Ե��ã������ý��и���
#define _ACV_DBFF_SIZE	(255*255+1)
class RP_EXT_CLASS ACV	//	AConstValue
{
public:
	enum LyChange {lyNew=1, lyAdd=2, lyDelete=4, lyActive=8, lyRename=16, lyLinked=32, lyThumbUpdated=64};
	enum AColorMuster {foreColor=0, backColor=1, protColor=2, tranColor=3, defColor};
public:
	static WORD	M[256][256];			//���������M[i][j] = i*j
	static BYTE Dbff[_ACV_DBFF_SIZE];	//���������Dbff[n] = (BYTE)(n/0xFF) 
public:
	static BOOL Init();
	static void Exit();
};
//���ܸ�Ҫ: ��Ϣ������,������Windows����Ӧ����
//  AMsgSendʵ���Ͼ��ǵ���Windows����Ϣ����ʵ�ֵ�,дAMsgSend��Ŀ�����ڴ�����Ϣʱ,������һЩ�Լ��Ķ��⴦��
class RP_EXT_CLASS AMsgSend
{
public:
	// Send to MainFrame
	//
	//������Ϣ�������ڲ��ȴ��������,����ͬSDK�е� SendMessage����
	static LRESULT ToMain(UINT message, WPARAM wParam=0, LPARAM lParam=0);
	//������Ϣ�������ڲ�ֱ�ӷ��أ����ص������ڴ�����ɣ�,����ͬSDK�е� PostMessage����
	static BOOL PostToMain(UINT message, WPARAM wParam=0, LPARAM lParam=0);
};


//������īˮ�����ϣ���ص����ݽṹ
#include "../include/ColorInfo.h"


//�ļ��������װ
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
	//�ļ�����ʱ�Ĵ�����Ϣ����
	//	bPrompt :[in]ָ��������ʽ
	//			TRUE �� ��ʾ�Ի���
	//			FALSE������������Ϣ��ӵ� g_Error ��
	static void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault, BOOL bPrompt);

public:
	//ɾ��ָ���ļ�
	//	bDelete		: [in]�Ƿ�������ɾ���ļ���
	//		ΪFALSEʱ��ʾ��	g_Flags.bDirectDelete �����������Ƿ�ֱ��ɾ���ļ�(�ļ��滻ʱ)
	//	bMultifiles	: [in]�Ƿ�ָ�����ļ���
	//		��ΪTRUEʱ��ÿ���ļ���������һ��'\0'�������ַ������Ӧ������ '\0'(�� "\0\0")����
	//		���嶨��ɲμ� SHFILEOPSTRUCT ����
	static BOOL Remove(LPCTSTR lpszFileName, BOOL bDelete, BOOL bMultifiles=FALSE);
	//�ļ�������
	//	lpszFileNameSrc --> lpszFileNameDst
	//	bDelete		: [in]��lpszFileNameDst �Ѵ���ʱ��Ч������ͬ��
	static BOOL Rename (LPCTSTR lpszFileNameSrc, LPCTSTR lpszFileNameDst, BOOL bDelete);
	//����ָ��Ŀ¼
	//	lpszPath	:[in]ָ��Ҫ������Ŀ¼��
	//��ע��
	//	��Ŀ¼�Ѿ�����ʱ������ֵ��ΪTRUE
	static BOOL CreateDirectory(LPCTSTR lpszPath);
	//ɾ��ָ��Ŀ¼
	//	lpszPath	:[in]ָ��Ҫɾ����Ŀ¼��
	//��ע��
	//	��Ŀ¼������ʱ������ֵ��ΪTRUE
	static BOOL DeleteDirectory(LPCTSTR lpszPath);
	//ͨ���ļ�����Ŀ¼��
	//	strPathName	:[out]��������Ŀ¼
	//	lpszFileName:[in]ָ���ļ���
	static BOOL GetDirectory(CString& strPathName, LPCTSTR lpszFileName);
	//���ļ����ж�����
	//	lpszPathName:[in]�ļ���
	//	bPrompt		:[in]����ʱ�Ƿ�����Ϣ�Ի��򵯳�
	static BOOL OpenReadFile(LPCTSTR lpszPathName, CFile& file, BOOL bPrompt=TRUE); 
	//���ļ����ж�д����
	//	lpszPathName:[in]�ļ���
	//	bPrompt		:[in]����ʱ�Ƿ�����Ϣ�Ի��򵯳�
	static BOOL OpenWriteFile(LPCTSTR lpszPathName, CFile& file, BOOL bPrompt=TRUE); 
	//�ر��ļ�����
	//ע���� OpenXXXFile��Ӧ��ͬCFile::Close()����
	static void CloseFile(CFile& file);

	//	IDPDefault	:[in]Ĭ�ϵĳ�����ϢID��AFX_ ��ʼ���� AFX_IDP_FAILED_INVALID_PATH��afxres.h��
	//���ļ�����ʱ�Ĵ�����Ϣ
	static CString GetSaveLoadExceptionInfo(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault);
	//��ʾ�ļ�����ʱ�Ĵ�����Ϣ���Ի���
	static void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault);

	// ��ǰ����ϵͳ�ܷ����Ӧ��ߵ��ļ�
	// int nWidth, nHeight : Width and Height of image.
	// int nPrompt : 0 : no prompt, 1: prompt on status, 2 : prompt on dialog
	static BOOL CanOpenFile(int nWidth, int nHeight, int nPrompt=0);

	//�ֽ��ļ���
	// 	path	:[in]Full path. 
	// 	drive	:[out]Optional drive letter, followed by a colon (:). 
	// 	dir		:[out]Optional directory path, including trailing slash. Forward slashes ( / ), backslashes ( \ ), or both may be used. 
	// 	fname	:[out]Optional base filename (no extension). 
	// 	ext		:[out]Optional filename extension, including leading period (.). 
	//ע���ο� _splitpath_s ����
	static BOOL SplitPath(const char *path, CString *drive, CString *dir, CString *fname, CString *ext);

	//�ļ�����
	//	pszDstFileName	:[in]Ŀ���ļ��������ΪĿ¼�Ļ�������"\\"��β
	//	pszSrcFileName	:[in]Դ�ļ���������Ϊ��Ч�ĵ����ļ�
	//	bAutoRename		:[in]��Ŀ��λ���Ѵ���ͬ���ļ�ʱ���Ƿ��Զ�����
	//	bPrompt		:[in]����ʱ�Ƿ�����Ϣ�Ի��򵯳�
	static BOOL CopyFile(CString &strDstFileName, const char *pszSrcFileName, BOOL bAutoRename, BOOL bPrompt);

	// ����ĳ���˻���ĳ���ļ�(��)��ע�����Ĳ���Ȩ��
	//	pszAccount			:[in]�˻����ƣ��� Users
	//	ObjectType			:[in]Ŀ�����ͣ�SE_OBJECT_TYPE
	//							SE_FILE_OBJECT			�ļ�����
	//							SE_REGISTRY_KEY			ע���
	//							SE_REGISTRY_WOW64_32KEY	32λ�����64λϵͳ�ϵ�ע�����
	//	pszPath				:[in]�ļ�(��)·����ע����������� C:\Temp\��
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
	//	AccessPermissions	:[in]����Ȩ��
	// 							GENERIC_READ
	// 							GENERIC_WRITE
	// 							GENERIC_EXECUTE
	// 							GENERIC_ALL
	static BOOL EnableAccountPrivilege (PCTSTR pszAccount, SE_OBJECT_TYPE ObjectType, PCTSTR pszPath, DWORD AccessPermissions=GENERIC_READ |GENERIC_WRITE |GENERIC_EXECUTE);
public:
	//ϵͳ����·��
	//���º�������������ֵ�� GetTempPath(...)��GetTempFileName (...) ͬ
	static DWORD GetSysTempPath(int nBufferLength, LPTSTR lpBuffer);
	static UINT GetSysTempFileName (LPTSTR szTmpName, LPCTSTR lpPrefixString, UINT uUnique);

public:
	//ѡ���ļ���
	//	strPath		:[out]��ǰѡ����ļ��У���'\'�������硰C:\������C:\Temp\��
	//	pParentWnd	:[in]��������
	//	pszDirectory:[in]��ʼ�ļ���
	//	pszTitle	:[in]�Ի������
	//	ulFlag		:[in]�Ի���������ԣ��� BIF_NEWDIALOGSTYLE
	static BOOL GetSpecialFolder(CString &strPath, HWND pParentWnd, const char *pszDirectory, const char *pszTitle, UINT ulFlags);

	//���¶����Ϊ������չ��Ҫ
public:
	virtual const CString& GetPathName() const;
	virtual void SetPathName(LPCTSTR lpszPathName);
	virtual BOOL IsModified() const;
	virtual void SetModifiedFlag(BOOL bModified = TRUE);

protected:
	CString		m_strPathName;		//�ļ���������ȫ·����Ϣ��
	BOOL		m_bModified;		//�Ƿ��Ѿ��༭���ˣ�δ���棩
};


//Ԥ���建����������
//ע��ͨ���������Ԥ����һ��ȫ�֣���ֲ�������������ͨ�������л��������롢�ͷŵȲ���
class RP_EXT_CLASS CBufferManager
{
public:
	CBufferManager();
	virtual ~CBufferManager();
	
private:
	//Ԥ���建����m_pBuffer��С����ʼ��С�� InitBuffer() ָ��;
	DWORD	m_dwBufferSize;
	
	//Ԥ���建������ַ: ͨ��LockBuffer����,ͨ��m_bUseFlag��m_bufferCritical���ж��Ƿ��Ѿ���ʹ��
	LPBYTE	m_pBuffer;
	
	//�ٽ����������ȷ������ͬ���̵Ĳ�ͬ�߳�ͬʱ����
	CRITICAL_SECTION	m_bufferCritical;	
	//������ m_pBuffer ʹ�ñ�ǣ����m_bufferCriticalʹ��
	//	����ȷ������ͬ�̵߳Ĳ�ͬ����ͬʱ����
	BOOL	m_bUseFlag;
	
public:
	//	pdwDstSize	:[out]ʵ�ʿ�ʹ�û������Ĵ�С������ΪNULL�����п��ܲ����ڣ�����/С�ڣ����軺������С
	//	dwSize		:[in]��ʼ��Ԥ���建������С����Ϊ0ʱ��ʾ�ͷŵ�ǰ�������ռ�
	//����ֵ����ǰ�����Ƿ�ɹ�
	//ע�����۷���ֵ�Ƿ�ΪTRUE��*pdwDstSize�����ʵ�ʿɲ����Ļ�������С
	BOOL InitBuffer(DWORD *pdwDstSize, DWORD dwSize);
	//	dwSize		:[in]��Ҫ���뻺������С
	//	Alloc	:[in]�Ƿ���л������������룬
	//����ֵ����������ַ��ΪNULLʱ��ʾ�������ɹ�
	//	��dwSizeС�ڵ���Ԥ���建�����Ĵ�С������Ԥ���建����û�б������ط�ʹ�ã�����Ԥ���建������ָ�룬
	//	�������bAlloc�Ĳ�����ȷ���������ִ���ʽ:
	//		bAlloc==FALSE	������NULL����ʱ�������û��Լ�����������ڴ�
	//		bAlloc==TRUE	����Ԥ���建�����޷�����ʱ������ǿ���������Ļ����������أ����򷵻�NULL
	//ע���� LockBuffer ����ֵ��ΪNULL������£���ʹ���껺������һ��Ҫ���� UnlockBuffer() ����������Դ�ͷ�
	LPBYTE LockBuffer(DWORD dwSize, BOOL bAlloc);
	
	//�ͷ����û�ռ�õ�Ԥ���建����,�Ա�����������ط�ʹ��;
	//ע����ʹ�� LockBuffer ����,���ҷ��ز�ΪNULL�������,��ʹ���껺������,һ��Ҫ���ô˺���
	BOOL UnlockBuffer(LPBYTE pBuffer);
	
	//�õ��������ĳ���
	DWORD GetBufferLength() const;
	
};
//���������������롢�ͷţ���װ��
typedef CMap<DWORD, DWORD, int, int&> CDWORDintMap;
class RP_EXT_CLASS AGV : public CBufferManager	//	AGlobalValue
{
public:
	AGV();
	virtual ~AGV();
	
protected:
	//����Init()/Exit()���漰 static �����Ĳ���
	static BOOL Init();
	static void Exit();

	friend class CrpBaseApp;

public:
	//���¸��������ֱܷ��Ӧ���Ե�API����,��Ϊ�˱����ڴ�������,
	//��Ӧ�ó����в���ֱ��ʹ���й��ڴ������API����,Ӧʹ����ӳ��AGV������#define����
	//ʹ�÷�ʽ: AGV::A*******(�����б�);
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
	//ע��:�޸����¸�����ҲӦ�޸Ķ�Ӧ�ķ�DEBUG�汾
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
	//	���ո�ʽѹ��
	//ע��һ������BMPͼ�񱣴棬�������� nBitCount < 8ģʽ��1,2,4��
	//
	//���ո�ʽѹ��
	//	pData		:[in/out]���ԭʼͼ�����ݣ�δѹ����1���ֽڶ�Ӧ1�����أ�
	//					�� pFileData==NULLʱ��Ҳ���ڴ��ѹ���������
	//	dwReadStart	:[in]��ʼλ�ã�Pixel)��[0, 8/nBitCount)
	//	dwWidth		:[in]ԭʼͼ�����ݳ��ȣ����ص������
	//	nBitCount	:[in]ԭʼͼ�����ݱ�������������1,2,4
	//	pFileData	:[out]���ѹ�����ͼ�����ݣ���ΪNULLʱ��ѹ�������ݴ���� pData��
	//ע����nBitCountʱ������ֱ�ӷ���TRUE��pFileData������Ч
	static BOOL ShrinkBit(BYTE* pData, DWORD dwReadStart, DWORD dwWidth, int nBitCount, BYTE* pFileData=NULL);
	//���ո�ʽ��ѹ��
	//	pFileData	:[in]ԭʼ���ո�ʽͼ������
	//	dwWriteStart:[in]��ʼλ�ã�Pixel)
	//	dwWidth		:[in]ԭʼ���ո�ʽͼ�����ݳ��ȣ����ص������	
	//	nBitCount	:[in]ԭʼͼ�����ݱ�������������1,2,4
	//	pData		:[out]��Ž�ѹ�����ͼ������
	static BOOL ExpandBit(const BYTE* pFileData, DWORD dwWriteStart, DWORD dwWidth, int nBitCount, BYTE* pData);

	//----- MemMode(UnPackMode) Pack to/from MemMode(PackMode)---------
	// ѹ����[pData, dwData] --> [pComp, dwComp]
	// ��ѹ��[pComp, dwComp] --> [pData, dwData]
	//ѹ����
	//	nBitCount		:[in]ָ��ͼ�������
	//	pData, dwData	:[in]ָ��ԭʼͼ�����ݼ����ȣ��ֽ�����
	//	pComp, dwComp	:[out]ָ��Ŀ��ͼ�����ݼ����ȣ��ֽ�����
	//	dwUp			:[in]ָ��Ŀ�껺�������ߴ磨�ֽ�����
	static BOOL Pack(const BYTE* pData, DWORD dwData, int nBitCount, BYTE* pComp, DWORD dwUp, DWORD& dwComp);
	//��ѹ����
	//	nBitCount		:[in]ָ��ͼ�������
	//	pComp, dwComp	:[in]ָ��ԭʼѹ��ͼ�����ݼ����ȣ��ֽ�����
	//	pData			:[out]ָ����ѹ�����Ŀ��ͼ�����ݻ�����
	//	dwWidth			:[in]ָ��Ŀ��ͼ�����ݴ�С���ֽ�����
	//ע��������ѹ�����ݽ�ѹ��������ݴ�С == dwWidth
	static BOOL UnPack(const BYTE* pComp, DWORD dwComp,DWORD dwWidth, int nBitCount,BYTE* pData);
	//��ѹ��ָ�����������
	//	nBitCount		:[in]ָ��ͼ�������
	//	pComp, dwComp	:[in]ָ��ԭʼѹ��ͼ�����ݼ����ȣ��ֽ�����
	//	pData			:[out]ָ����ѹ�����Ŀ��ͼ�����ݻ�����
	//	dwWidth			:[in]ָ��Ŀ��ͼ�񻺳�����С�����ص㣩
	//	Start, Length	:[in]Pixel��ָ�����账���ͼ����������Start+Length <= dwWidth
	static BOOL UnPack(const BYTE* pComp, DWORD dwComp, 
							 DWORD dwWidth/*pixel*/, int nBitCount,
							 DWORD Start/*pixel*/, DWORD Length/*pixel*/,
							 BYTE* pData);

public:	// Block Data Compress
	//-- this fund is used in Image's Expand(Block Compress Mode)-----
	//-- szBlock	: size of Block
	//-- posInBlock	: start point'coord in szBlock;
	//-- RECT(posInData,szMap): zoomscale rect(������ZoomScale), pXMap and pYMap are datas of ZoomScale
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
	
	//����ָ���������� *pVal ����ĸ��������ص�����
	//ע����Ҫ���ڼ���ڰױ�
	static DWORD CalcMono(BYTE BitCount, int nCompMode, int nWidth, int nHeight, 
		const BYTE* pBits, DWORD dwBits, const BYTE* pVal);

	
public:
	// ѹ����[pData, dwData] --> [pComp, dwComp]
	// ��ѹ��[pComp, dwComp] --> [pData, dwData]
	
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
	// ��ʼ��һ������
	//	nWidth		��[in]���ȣ����ص㣩
	//	wBitCout	��[in]������
	//	bCompress	��[in]��ʼ��Ϊѹ����
	//	pValue		��[in]��ʼ��ֵ
	//	pDstBits	��[out]�ɵ���������,���ɵ������ͷ�
	// ����ֵ��==0 ʱ��������ʧ�ܣ����򵱳�ʼ��ѹ����ʱΪpDstBits�ֽ���������ΪnWidth
	static DWORD InitSpaceBits (LPBYTE pDstBits, int nWidth, WORD wBitCount, 
								const BYTE* pValue, BOOL bCompress);
	
	//ͼ�����ݼ����Ų���
	//	wBitCount	:[in]������
	//	pData		:[in]ԭʼͼ������
	//	nWidthPre, nHeightPre	:[in]ԭʼͼ���С
	//	pBits		:[out]Ŀ��ͼ�����ݻ�����
	//	nWidth, nHeight			:[in]Ŀ��ͼ���С
	static BOOL ZoomBits(int nWidthPre, int nHeightPre, WORD wBitCount, const BYTE* pData, 
						int nWidth, int nHeight, BYTE* pBits);

	//����Բ��ӳ������ͼ���������Ų���
	//	nBitCount	:[in]������
	//	pSrcBits, nWidthOld	:[in]ָ��ԭʼͼ�����ݼ�����
	//	pDstBits, nWidthNew	:[in]ָ��Ŀ��ͼ�����ݻ�����������
	//	pMap		:[in]ָ��nWidthNew-->nWidthOld��ӳ�����ΪNULLʱ�������ڲ��Զ�����
	//					�ο� AMathOpt::ResampleLineMap(nWidthNew, nWidthOld, pMap) ����
	//ע��pSrcBits != pBitDst
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
	//Base64����/����
	//	src		:[in]Դ�ַ���
	//���أ�����/�����Ŀ�껺�������ַ�����������
	static char* Base64_Encode(const char *src);
	static char* Base64_Decode(const char *src);
	//�ͷ� Base64_Encode / Base64_Decode ���ص�
	static void Base64_Free(char **ppBuf);

};

//������ɢ������
class RP_EXT_CLASS SEPARATE
{
public:
	SEPARATE();
	~SEPARATE();

	//-***  cv= cvBezier or cvBSpline ****
	//��������Ӿ���
	static BOOL GetCurveBorder(RECTD& rcDst, const POINTD* pCtrl, int nCtrl, CURVE cv);

	//-***  cv= cvBezier, cvBSpline, cvPolyline or cvTrace ****
	//Poly������ɢ
	static BOOL Separate( CPOINTDArray& ptArray,  const POINTD* pCtrlPt, int nCtrlPt, CURVE cv, BOOL bClose= FALSE, SEPWAY way=swRough);
	static BOOL Separate( CPOINTArray& ptArray,  const POINTD* pCtrlPt, int nCtrlPt, CURVE cv, BOOL bClose= FALSE, SEPWAY way=swSmooth);
	static BOOL Separate( CPOINTArray& ptArray,  const POINT* pCtrlPt, int nCtrlPt, CURVE cv, BOOL bClose= FALSE, SEPWAY way=swSmooth );
	//PolyPoly������ɢ
	static BOOL Separate( CPOINTArray& SprtArray, CintArray& CountArray, const POINTD* lpCtrl, const int* lpCount, int nPoly, CURVE cv, BOOL bClose= FALSE,SEPWAY way=swSmooth);
	static BOOL Separate( CPOINTArray& SprtArray, CintArray& CountArray, const POINT* lpCtrl, const int* lpCount, int nPoly, CURVE cv, BOOL bClose= FALSE, SEPWAY way=swSmooth);


	//BSpline <--> Bezier
	//	lpCtrl1, nNumPoint	:[in]ԭʼ�㴮
	//	lpCtrl2, nNumPoint2	:[out]Ŀ��㴮
	//	bBStoBe				:[in]�Ƿ�ΪBSpline-->Bezier����ΪFALSEʱ����ʾBezier-->BSpline
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
	//��δʵ��
	static BOOL Insert(const POINTD* lpPoint, int nPoint, POINTD*  pPointD,int nBufSize,int& nNumD, double dDest=5.0);

	static BOOL SeparateChar( CPOINTArray& ptArray,  const POINTD* pCtrlPt, int nCtrlPt, BOOL bClose= FALSE, SEPWAY way=swSmooth );

private:
	static RECTD GetOneSegBorder( const POINTD* pCtrl /*nCtrl=4*/);
	static BOOL SeparateOneSeg(LPPOINT pSprt, int nBufSize, int& nSprt, const double* dCtrlX, const double* dCtrlY/*nCtrl=4*/, int nPreNum);
	static BOOL SeparateOneSeg(POINTD* pSprt, int nBufSize, int& nSprt, const double* dCtrlX, const double* dCtrlY/*nCtrl=4*/, int nPreNum);
	static BOOL GetEnlg3(const POINT* pPt, POINTD* pPtF,double *t1,double *t2);
	//������Curve
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
	//�Ƕ����ͣ���ͨ��ˮƽ����ֱ
	enum ANGLESYTLE {ANGLE_NORMAL=0, ANGLE_HOR, ANGLE_VER};

private:
	static CRITICAL_SECTION	m_section_IsBadWritePtr;		//IsBadWritePt �����ٽ��ʶ	
protected:
	// AMathOpt��ز�����ʼ��/�ͷŲ�����CrpBaseApp����һ�β��ҽ��������һ�Ρ�
	static void Init();
	static void Exit();
	friend class CrpBaseApp;

public:
	//��ǰ�������ռ��Ƿ��д��ͬSDK�е� IsBadWritePtr ���壩
	//	lp	:[in]�ڴ��ַ
	//	ucb	:[in]�ڴ��С
	//ע��SDK�к�����֧�ֲ�������������ͨ����ӱ�����ٽ��ʶ�����иĽ�
	static BOOL MathOpt_IsBadWritePtr(LPVOID lp, UINT_PTR ucb);

public:
	//ȥ�����ڲ�����ͬ�������
	//	lpDst			:[out]Ŀ�껺����
	//	lpSrc, nCount	:[in]Դ�㴮���������㴮������������С����������
	//	����ֵ��Ŀ��㴮����
	//ע������lpDst, lpSrc ʹ����ָͬ��
	static int SprayPoints(LPPOINT lpDst, const POINT* lpSrc, int nCount);
	//ȥ����������е����ڲ�����ͬ�������
	//	lpPt	:[in/out]ָ���㴮����Σ�������
	//	lpCount	:[in/out]ָ����ε㴮��ÿ�εĴ�С����������
	//	nCount	:[in/out]ָ���㴮����
	//ע����������ĳһ������������Ч�����ʱ���п��ܸı� nCount��ֵ
	static int SprayPoints (LPPOINT lpPt, LPINT lpCount, int& nCount);

	//ֱ����ɢ
	//	lpPt	:[in/out]����ʱ���ԭʼ����㴮�����ʱ�����ɢ���Ŀ��㴮
	//	dwNumPt	:[in]ԭʼ�㴮����
	//	dwNumMax:[in]Ŀ�����껺������С���ɴ�ŵ�����������POINT��
	//����ֵ����ɢ��ĵ�����
	static DWORD StepPolyline (LPPOINT lpPt, DWORD dwNumPt, DWORD dwNumMax);

	//��ָ���㴮����Ӿ���
	static RECT GetBorderRect (const POINT* lpPt, DWORD dwNumPt);
	static RECTD GetBorderRect (const POINTD* lpPt, DWORD dwNumPt);

	//�㴮�ƶ�
	static VOID OffsetPolygon (LPPOINT lpPoly, DWORD dwNum, int nXOffset, int nYOffset);
	static VOID OffsetPolygon (LPPOINT lpDst, const POINT* lpSrc, DWORD dwNum, int nXOffset, int nYOffset);

	//�������ŵ㴮����
	//	rcFrame :[in]��������ָ���㴮����һ���ı������ź�����Ӿ������ֵ��ͬ
	static VOID FitPolyToRect (LPPOINT lpPoly, DWORD dwNumPt, RECT rcFrame);
	
	//�㴮��ת����
	//	pCenter	:[in]ָ����ת���ĵ㣬��� pCenter==NULL������ת���ĵ�ȡ�㴮��Ӿ�������
	static VOID RotatePoints (LPPOINT lpPt, DWORD dwNumPt, double fAngle, const POINT* pCenter=NULL);
	static VOID RotatePoints (LPPOINTD lpPt, DWORD dwNumPt, double fAngle, const POINTD* pCenter=NULL);

	//Bezier������ɢ
	//	lpPoints:[out]�����ɢ��ĵ㴮��Ϣ����ΪNULLʱ��������������ɢ�������
	//	ptBCP	:[in]Bezier���ߵĿ��Ƶ㣨4���㣩
	//	bSepWay	:[in]������ɢ��ʽ��SEPWAY (swRough, swFine, swSmooth)
	//	dwNumMax:[in]Ŀ�����껺������С���ɴ�ŵ�����������POINT��
	//����ֵ��������ɢ�������
	//------------------------------------------------------------------------
	static DWORD CreateBezierPoints (LPPOINT lpPoints, LPPOINT ptBCP, BYTE bSepWay, DWORD dwNumMax); 

public:
	//������С��Լ��
	static DWORD CommonMultiple(DWORD dwNum1, DWORD dwNum2);
	//������ת����ʽ
	//	dRatio	:[in]ָ��С��ֵ����������
	//	wSize	:[in]ָ����ʽ�еķ�ĸ���ֵ��(10, 0xFFFF)
	//����ֵ��dwScale�����ӱ��ʽ��dRatio �� HIWORD(dwScale)/LOWORD(dwScale)
	static DWORD DoubleToFraction(double dRatio, WORD wSize); // wSize >= 10
	//�򻯷���ʽ
	// HIWORD/LOWORD
	static DWORD SimpleFraction(DWORD dwFraction);
	//��������
	// HIWORD(dwScale1)*HIWORD(dwScale2) / (LOWORD(dwScale1)*LOWORD(dwScale2))
	static DWORD ScaleMultiply(DWORD dwScale1, DWORD dwScale2);

	// 0 <= dwPosition < dwOld
	// return dwPosition �� dwOld �� dwNew ��λ��
	static DWORD ResamplePosition (DWORD dwOld, DWORD dwNew, DWORD dwPosition);
	// 0 <= dwPosition < dwOld
	// pMap : dwPosition �� dwOld �� dwNew ��λ��
	// return dwPosition �� dwOld �� dwNew ʱ�ĸ���
	static DWORD ResamplePosition (DWORD dwOld, DWORD dwNew, DWORD dwPosition, DWORD* pMap);
	//	dwOld:		ԭʼ���ȡ�
	//	dwNew:		Բ����ĳ��ȡ�
	//	dwStart, dwEnd : �ô� dwStart �� dwEnd ֮��ĸ����Ӧֵ, dwStart < dwEnd
	//	pCount:		ԭʼ��ÿ������ظ���(������dwEnd), pCount size is dwEnd-dwStart
	static void ResampleSegCount(DWORD dwOld, DWORD dwNew, DWORD dwStart, DWORD dwEnd, DWORD* pCount);
	//	dwOld:		ԭʼ���ȡ�
	//	dwNew:		Բ����ĳ��ȡ�
	//	dwStart, dwEnd : �ô� dwStart �� dwEnd ֮��ĸ����Ӧֵ, dwStart < dwEnd
	//	pMap:		ԭʼ��ÿ����Ķ�Ӧֵ(������dwEnd), pMap size is dwEnd-dwStart
	static void ResampleSegMap(DWORD dwOld, DWORD dwNew, DWORD dwStart, DWORD dwEnd, DWORD* pMap);
	//	pCount : Repeat's count of every point on dwOld (pCount's size is dwOld)
	//	dwOld:		ԭʼ���ȡ�
	//	dwNew:		Բ����ĳ��ȡ�
	//	pCount:		ԭʼ��ÿ������ظ���(������dwEnd), (pCount's size is dwOld)
	static void ResampleLine (DWORD dwOld, DWORD dwNew, DWORD* pCount);
	//	pMap : Map coordinates of dwOld to dwNew (pMap's size is dwOld+1)
	//	dwOld:		ԭʼ���ȡ�
	//	dwNew:		Բ����ĳ��ȡ�
	//	pMap:		ԭʼ��ÿ����Ķ�Ӧֵ(������dwOld), (pMap's size is dwOld)
	static void ResampleLineMap (DWORD dwOld, DWORD dwNew, DWORD* pMap);

public:
	//�����Ƕ�ֵ�����ȣ��Ƿ����
	static BOOL EquAngle (double angle1, double angle2);
	//�ж�ָ���Ƕ�ֵ�����ȣ�״̬��ˮƽ��ֱ��
	static AMathOpt::ANGLESYTLE AngleStyle(double dbAngle);
	//����ֱ�߽Ƕ�
	//	ptFirst, ptCursor	:[in]ֱ�ߵ������˵�
	//	bRadian				:[in]�Ƿ��践�ػ���ֵ��ΪFALSE���ؽǶ�ֵ
	static double GetAngle (POINTD ptFirst, POINTD ptCursor, BOOL bRadian);
	static double GetAngle (POINT ptFirst, POINT ptCursor, BOOL bRadian) 
	{ return GetAngle (POINTD(ptFirst), POINTD(ptCursor), bRadian); }

public:
	//�õ㵽ֱ�߾���
	//	pt				:[in]��
	//	pt1, pt2		:[in]ֱ�߲�����pt1, pt2 ָ��ֱ�ߵ������˵㣩
	static double DistPtToLine (POINTD pt, const POINTD& pt1, const POINTD& pt2);
	//�õ㵽ֱ�߾���
	//	pt				:[in]��
	//	ptLine, dbAngle	:[in]ֱ�߲�����ͨ��ptLine���Ƕ�ΪdbAngle��
	static double DistPtToLine (POINT pt, POINT ptLine, double dbAngle);
	//�õ㵽ֱ�߾���
	//	pt				:[in]��
	//	pPtLine[2]		:[in]ֱ�߲�����pPtLine[0], pPtLine[1]ָ��ֱ�ߵ������˵㣩
	static double DistPtToLine (POINT pt, const POINT* pPtLine);
	
	//�������ľ���
	//	pt1, pt2		:[in]��
	//����ֵ��pt1,pt2֮��ľ���
	static double NearPoint (const POINT& pt1, const POINT& pt2);
	//���������Ƿ�С�� dDist
	//	pt1, pt2		:[in]��
	//	dDist			:[in]�ж�����
	//����ֵ��pt1,pt2֮��ľ��� <= dDist ʱ����TRUE�����򷵻�FALSE
	static BOOL NearPoint (const POINT& pt1, const POINT& pt2, double dDist);
	//�õ㵽���ϵ㣨�㴮������̾���
	//	pt				:[in]��
	//	lpPt, uNum		:[in]�㴮����
	//	pdRet			:[out]�����̾���ֵ
	//����ֵ����̾���ֵ��Ӧ�ĵ��ڵ㴮�е�����
	static UINT NearPoint(const POINT* lpPt , UINT uNum, const POINT & pt, double* pdRet=NULL);

public:
	//�������Ƿ���ͬ
	static BOOL EquRect (const RECT& rc1, const RECT& rc2)
	{
		return (BOOL)(rc1.left == rc2.left
			&& rc1.top == rc2.top
			&& rc1.right == rc2.right
			&& rc1.bottom == rc2.bottom
			);
	}
	//���κϲ����ο� SDK �� UnionRect ������
	static RECT UnionRect (const RECT& rcSrc1, const RECT& rcSrc2);
	//�þ����ĵ�����
	//	topleft, topright, bottomright, bottomleft
	static void GetRectPoints (POINT* pPts, const RECT& rect);

public:
	//��������ֱ�߶εĽ�������
	//  P1P2 Line: pt1, pt2; P3P4 Line: pt3, pt4
	//	lpPt	:[out]��Ž�������
	//����ֵ������״̬
	//	-1	:�޽��㣬����ֱ�߶�Ϊƽ����
	//	0	:������ֱ�߶��⣨����Ч���㣩
	//	1	:������ֱ�߶��ڣ�Ϊ��Ч����
	static int IntersectLine(LPPOINT lpPt, const POINT& pt1, const POINT& pt2, const POINT& pt3, const POINT& pt4);
	static int IntersectLine (LPPOINTD lpPt, const POINTD& pt1, const POINTD& pt2, const POINTD& pt3, const POINTD& pt4);
	//��ֱ�߽���
	//	ptInter			:[out]��������
	//	ptd1, dbAngle1	:[in]��һ��ֱ�߲���
	//	ptd2, dbAngle2	:[in]�ڶ���ֱ�߲���
	static BOOL IntersectLine (POINTD& ptInter, const POINTD& ptd1, double dbAngle1, const POINTD& ptd2, double dbAngle2);
	
	//��ֱ������εĽ���
	//	rc			:[in]Դ����
	//	pt, dbAngle	:[in]ֱ�߲���������pt���Ƕ�ΪdbAngle��ֱ�ߣ�
	//	pRet		:[out]�������껺��������������С�� >= 4
	//����ֵ��������������Ϊ4����
	static int IntersectRect (RECT rc, POINT pt, double dbAngle=0.0, POINT* pRet = NULL);
	//��Բ���㣬����ֵΪ�������
	//	ptInterArray	:[out]������������
	//	ptd1, dbDiameter1	:[in]��һԲ��������ptd1Ϊ���ģ�ֱ��ΪdbDiameter1��Բ��
	//	ptd2, dbDiameter2	:[in]�ڶ�Բ��������ptd2Ϊ���ģ�ֱ��ΪdbDiameter2��Բ��
	static int IntersectCircle (CPOINTDArray& ptInterArray, const POINTD& ptd1, double dbDiameter1, const POINTD& ptd2, double dbDiameter2);
	//Բ��ֱ�߽��㣬����ֵΪ�������
	//	ptInterArray		:[out]������������
	//	ptd1, dbDiameter1	:[in]Բ��������ptd1Ϊ���ģ�ֱ��ΪdbDiameter1��Բ��
	//	pt2, dbAngle2		:[in]ֱ�߲���������ptd2���Ƕ�ΪdbAngle2��ֱ�ߣ�
	static int IntersectCircleLine (CPOINTDArray& ptInterArray, const POINTD& ptd1, double dbDiameter1, const POINTD& ptd2, double dbAngle2);
	
public:
	//�����Բ
	//	pt1, pt2, pt3	:[in]Բ�ϵ�����
	//	ptCenter		:[out]����ó���Բ���ĵ�
	//����ֵ��
	//	����ó���Բ�뾶 radius���� radius < 0 ʱ��ʾ��������
	static double GetCircleFromThreePoints (POINTD& ptCenter, const POINTD& pt1, const POINTD& pt2, const POINTD& pt3);

public:
	//������Ӿ���
	static RECT GetBox(const POINT* pPts, const int* pCount, UINT uCount, const ATransform * ptrform=NULL, CURVE curve=cvBezier);
	static RECT GetBox (const POINT* pPts, UINT uNumPts, const ATransform* ptrform=NULL, CURVE curve=cvBezier);
	//�󻡶�, ptc: center point
	static BOOL ArcAngle(const POINTD& pt, const POINTD& ptc, double& dAngle);
	//�Ƿ�Ϊ��ʱ�뷽��
	static BOOL IsCounterClockwise(double dAngStart, double dAngMid, double dAngEnd);

public:
	//�ڴ濽��
	//��ָ�������ڵ�ԭ���ݿ�����Ŀ�껺������
	//	pDst				:[out]Ŀ�껺���������������Ϊ nDstRowBytes
	//	nDstRowBytes		:[in]Ŀ�껺�������
	//	pSrc, pSrcRowBytes	:[in]ԭʼ��������ÿ�п��
	//	nWidth, nHeight		:[in]ָ�����追���������С
	static BOOL myMemcpy(BYTE* pDst, int nDstRowBytes, BYTE BitCount,
				const BYTE* pSrc, int nSrcRowBytes, int nWidth, int nHeight);
	//	pRstInBlock			:[in]ָ�����追���������С
	//							pRstInBlock'(left,top) be equal to pSrc'start position
	//							pRstInBlock is protect Rgn of pDst
	//							pDst : Start write position ==> top*nDstRowBytes+left
	//							pSrc : Start read  position ==> 0
	static BOOL myMemcpy(BYTE* pDst, int nDstRowBytes, BYTE BitCount,
				const BYTE* pSrc, int nSrcRowBytes, const CRstLines* pRstInBlock);
	//�ڴ��ʼ������
	//	pDst		:[out]Ŀ�껺��������������СΪ nItem * nCount
	//	pSrc		:[in]ԭʼ������
	//	nItem		:[in]ָ��ԭʼ���ݵ�Ԫ�Ĵ�С����pSrc��������С������ sizeof(POINT)
	//	nCount		:[in]ָ�������ʼ�������ĵ�Ԫ����
	static void myMemset(BYTE* pDst, const BYTE* pSrc, int nItem, int nCount);

	//���ܸ�Ҫ:��rcSrc1����rcSrc2�еĲ���,���������rcDstArray��
	//��������:
	//	rcSrc1, rcSrc2	:[in]
	//	rcDstArray		:[out]
	//����ֵ:
	//	rcDstArray����ĳ���,����Ϊ0,��ȫ����rcSrc2��
	static int DifferRect(const RECT rcSrc1, const RECT rcSrc2, CRECTArray& rcDstArray);

	//���ܸ�Ҫ:��rcSrcArray����rcSrc�еĲ���,���������rcDstArray��
	//��������:
	//	rcSrcArray, rcSrc	:[in]
	//	rcDstArray			:[out]
	//����ֵ:
	//	rcDstArray����ĳ���,����Ϊ0,��ȫ����rcSrc��
	static int DifferRect(const CRECTArray& rcSrcArray, const RECT rcSrc, CRECTArray& rcDstArray);

	//���ܸ�Ҫ:��rcSrc����rcSrcArray�еĲ���,���������rcDstArray��
	//��������:
	//	rcSrc, rcSrcArray	:[in]
	//	rcDstArray			:[out]
	//����ֵ:
	//	rcDstArray����ĳ���,����Ϊ0,��ȫ����rcSrcArray��
	static int DifferRect(const RECT rcSrc, const CRECTArray& rcSrcArray, CRECTArray& rcDstArray);
	
	//���ܸ�Ҫ:��rcSrcArray1����rcSrcArray2�еĲ���,���������rcDstArray��
	//��������:
	//	rcSrcArray1, rcSrcArray2	:[in]
	//	rcDstArray					:[out]
	//����ֵ:
	//	rcDstArray����ĳ���,����Ϊ0,��ȫ����rcSrcArray2��
	static int DifferRect(const CRECTArray& rcSrcArray1, const CRECTArray& rcSrcArray2, CRECTArray& rcDstArray);

public:
	// ����	   : �� �������� A[0] + A[1]*x +A[2]*x^2 + A[3]*x^3 + A[4]^4 = 0 ��ʵ��(���4��)
	// ����������nA>=2 && nA<=5. x ����>= nA-1.
	// ���̴���: nA-1.
	// ����    : if return TRUE:	n Ϊʵ������. x ���ʵ��
	//			 if return FALSE:   data error.
	static BOOL Equation( const double* A, int nA, double* x, int& n);
	//	5 �δ���������ʵ��, ����ͬ��.
	static BOOL Equation5(const double* A/*nA=6*/, double x0, double x1, double* x, int& n, double eps, BYTE bUp);
	
protected:
	// u + v*i = (x + y*i)^(1/n) : i= sqrt(-1)
	static void Ontrt(double x, double y, int n, double* u, double* v);
	static BOOL ddhrt(const double* A, double a,double b, double* x, double eps);

public:
	//��������������
	//	base		:[in/out]����ʱ���ԭʼ���������ݣ����ʱ�������ú������
	//	width,length:[in]ָ����������С��size=width*lenght
	//				width :ָ�����������ݳ�Ա�Ĵ�С���� sizeof(POINT)
	//				length:ָ�������������г�Ա����
	//	p_buffer	:[in/out]Ϊ�����ռ�, ���Ⱥ�baseһ��, p_buffer != base
	//				p_buffer�н������ʱ����
	//	comp		:[in]�Զ���ȽϺ��������庬��ͬ SDK �е� qsort
	//				comp ��������ֵ���壺
	//					< 0	: elem1 <  elem2 
	//					0	: elem1 == elem2 
	//					> 0 : elem1 >  elem2 
	//����
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
	//��base[s..t] (������t)���й鲢����
	//p_bufferΪ�����ռ�, ���Ⱥ�baseһ��, p_buffer != base
	static void _pri_merger_sort(
		void *base, Int32 width, Int32 s,	Int32 t,
		void *p_buffer,
		Int32 (__cdecl *comp)(const void * elem1, const void *elem2));
	//��SR[i..m]��SR[m..n] �鲢Ϊ�����SR[i..n]
	static void _pri_Merge(void* SR, Int32 width,
		Int32 i, Int32 m, Int32 n, 
		void *p_buffer,
		Int32 (__cdecl *comp)(const void * elem1, const void *elem2));

public:
	//�ڻ�����(pBuffer)�в���ָ�����ݴ�(pData)
	//���أ�ָ��ڻ�������ƫ�ƣ���Ϊ 0xFFFFFFFF ʱ��ʾδ���ҵ�������
	static DWORD FindData(const BYTE *pBuffer, DWORD dwBufferSize, const BYTE *pData, DWORD dwDataSize);
	static DWORD FindData(const BYTE *pBuffer, DWORD dwBufferSize, BYTE bData);
	//�ڻ�����(pBuffer)�в���ָ������(bData)ǰ����ֵ�ַ���('0'-'9')
	//���أ�ָ��ڻ�������ƫ�ƣ���Ϊ 0xFFFFFFFF ʱ��ʾδ���ҵ�������
	static DWORD FindData_Int(const BYTE *pBuffer, DWORD dwBufferSize, BYTE bData);
	//�ڻ�����(pBuffer)�з������ָ�����ݴ�(pData)
	//���أ�ָ��ڻ�������ƫ�ƣ����򣩣���Ϊ 0xFFFFFFFF ʱ��ʾδ���ҵ�������
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


//�ӻ�ͷ��������
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

//�ӻ�ͷ��������-������
struct CYCLEPARAMS_FLOAT
{
	double		m_float_cycWidth;		//	Pixels
	double		m_float_cycHeight;		//	Pixels
	double		m_float_cycRatio;		//	���ӱ������Է�ʽ��ʽ��ʾ���� 1/2��1/3������������С�ڵ���1��0�൱��1����ʾƽ�ӣ�
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



//g_Flags���ȫ��״̬��Ϣ, �������ݺ�ע����й�(��������ʱ��ע���,�ر�ʱдע���)
#define DEFAULT_RESUNIT		ruDPI		//ȱʡ���ȵ�λ
#define DEFAULT_SIZEUNIT	suMM		//ȱʡ�ߴ絥λ
#define DEFAULT_RES 		600			//ruDPI
#define DEFAULT_WIDTH		1800		//suMM
#define DEFAULT_HEIGHT		10000		//suMM
#define RULER_WIDTH			16			//��߿��(Pixel)
struct RP_EXT_CLASS AGFLAGS
{
	AResUnit	ResUnit;				//���ȵ�λ: ����/Ӣ��,����/˿��,����/����,����/����,����/��
	ASizeUnit	SizeUnit;				//�ߴ絥λ ����,˿��,����,����,��,Ӣ��
	
	double		dDefRes;				//�½��ļ���ȱʡ����, AResUnit
	double		dDefWidth;				//�½��ļ���ȱʡ���, suMM
	double		dDefHeight;				//�½��ļ���ȱʡ�߶�, suMM

	BOOL		ShowRuler;				//��ʾ���

	BOOL		ShowGridline;			//��ʾ������

	int			nLRSpace;				//���ǽӻ�ͷ״̬ʱ���������ߵĿ�϶����Ļ�����С��
	int			nTBSpace;				//���ǽӻ�ͷ״̬ʱ���������ߵĿ�϶����Ļ�����С��

	BOOL		bDirectDelete;			//�Ƿ�ֱ��ɾ���ļ�(�ļ��滻ʱ)��Ϊ FALSE ʱΪ������������

	CString 	strCacheDirectory; 		//ϵͳ����·��

	BOOL		bShowColorGroup;		//ɫ��Ի����Ƿ���ȫͼ����ʾ��־ɫ�ͳ�Աɫ����ʽ��ʾ

	BOOL		bEnableToolTip;			//�Ƿ���ʾ������ʾ

	UINT		nApplicationLook;		//Ӧ�ó��������ID�������� CMainFrame��

	CString		strPrintDeviceModule;	//��ӡ�豸����ģ�����ƣ��� "rp16DGU.drv"

	BOOL		bThumbWithMono;			//���б�-����ͼ-��ɫ��/�Ҷȸ��Ե�ɫģʽ��ʾ
	
	BOOL		bScaleLimit;			//�ڽ���ͼ���С�ı���صĲ���ʱ����X/Y����ȱ�����

	enum DYEMIX_WAY
	{
		dyemix_way_anseries = 0,		//AnSeries ģʽ��͸����Ӧ��ʱ�����Ǹ�ɫ��ȫ��Ӱ��
		dyemix_way_opacity	= 1,		//͸����ģʽ��Ӧ��ʱ���Ǹ�ɫ�����и�ɫ���ֽ��л�ɫ

		dyemix_way_count
	};
	BYTE		bDyeMixWay;								//���ϻ��ģʽ��DYEMIX_WAY
	int			nDefaultOpacity[dyemix_way_count];		//Ĭ��͸����

	enum DYEMIX_OPACITY_WAY
	{
		dyemix_opacity_way_normal = 0,	//��ͨ��ʽ�������ڱ�ɫ�������ϻ��ģʽ���� R=R1*R2/255
		dyemix_opacity_way_addup,		//ֱ���ۼ�ģʽ��C=C1+C2����һͨ�����������ͨ��ͬ��ѹ��

		dyemix_opacity_way_count
	};
	BYTE		bDyeMixOpacityWay;		//͸����ģʽ�µĻ�ϼ��㷽ʽ������� dyemix_way_opacity ģʽ

	BOOL		bLoadCMYKFileToRGB;		//�Ƿ�CMYK��ʽ��TIF/JPG�ļ�����RGBͼ����ΪFALSEʱ����ʱ��Ϊ4��רɫ�㴦��
	BOOL		bUsedEmbedICC;			//�Ƿ�ʹ��ͼ���ļ���ǶICC���н��е��봦�����CMYK��ʽ����bLoadCMYKFileToRGB==TRUEʱ��Ч��

	BOOL		bNeedCloth;				//�Ƿ���ʾ����ͼ��

	BOOL		bBackgroundPaintEnable;	//�Ƿ������̨ˢ��ģʽ
	int			nPaintSize;				//��̨ˢ�������С���ֿ飩

public:
	BOOL		bStatusShowColorInfo;	//�Ƿ���״̬������ʾ��ɫ��Ϣ����ʱ���������������Ϣ�У�
	BOOL		bTimerPaint;			//��ʱˢ�±�ǣ�ΪFALSEʱ��Ļʵʱˢ�£���bBackgroundPaintEnable==TRUEʱ��Ч

public:
	//���²�����ʵ��RIPʱʹ�õ� ClrSepParam ֵӦ��һ�£�
	//	�ڴ˴������Ϊ��֤���еײ����ܽ��е��ã����������� RainBow�����н����޸�
	CString			strInIccProfile;		//���� icc profile ��RGB��
	CString			strInCMYKIccProfile;	//���� icc profile ��CMYK��
	CString			strMoniIccProfile;		//��ʾ�� icc profile
	CString			strOutIccProfile;		//��� icc profile
	enum INTENT_WAY
	{
		intent_Perceptual	= 0,		//�ɸ�֪	��Perceptual
		intent_Relative		= 1,		//��Ա�ɫ	��Relative colorimetric
		intent_Saturation	= 2,		//���Ͷ�	��Saturation
		intent_Absolute		= 3,		//���Ա�ɫ	��Absolute colormietirc

		intent_count
	};
	UINT32			nIntent;			//ƥ��ģʽ��INTENT_WAY

	BOOL			bMapDataFileEnable;			//�Ƿ������Զ�����ӳ�������ļ�
	CString			strMapDataCacheDirectory;	// RGB->CMYK�Ŀ���ӳ������Ŀ¼�����Զ���

	BOOL			bShowSpashScreen;			//����ʱ�Ƿ���ʾSpashScreen

	DWORD 			dwPrnFileSizeLimit;			//����PRN�ļ���С���ƣ���Ϊ0ʱ��ʾ�������ƣ���λ��MB

	BOOL			bMouseWheelForZoomScale;	//���������ţ�ΪFALSEʱ���ֽ�������/�����ƶ�
	
	BOOL			bProofDisplay;				//�Ƿ�Ĭ��Ϊ��Ļ��У��ģʽ����ʾ��

	CString			strRIPDataCacheDirectory;	// RIP����ʱ���ݻ���Ŀ¼�����Զ���

	BOOL			page_bLayoutLimit;			//ѡ��ͼ����λʱ�����ڰ�����
	BOOL			page_bSpaceLimit_Enable;	//ѡ��ͼ��ƴ��ʱ�Ƿ��м������
	double			page_dSpaceLimit_hor;		//ͼ��ˮƽ�������Сֵ����mm��< 0 ʱ��ʾ�಻����
	double			page_dSpaceLimit_ver;		//ͼ����ֱ�������Сֵ����mm��< 0 ʱ��ʾ�಻����

	BOOL			page_bDefaultToCustomCut;	//Ĭ��Ϊ�򵥼��ã�FALSE��ʾĬ��Ϊ����

	CString			strDefaultFontName;			//����ʹ�õ�Ĭ��������

public:
	//��ʼ��ȫ�ֲ���ֵ����g_Flags����
	BOOL Init();

	//�������ļ�/ע����ж�ȡ��Ϣ
	BOOL LoadFromProfile();
	BOOL LoadFromProfile(const char *pszIniFile);
	//������д�������ļ�/ע�����
	void SaveToProfile();
	BOOL SaveToProfile(const char *pszIniFile);
};

//���ܸ�Ҫ: ��ͷ������,�ṩ�ӻ�ͷ���㷨(��ͷ�ָ�,��ͷ��λ��), �����Ǵ��㷨��
//ע��
//	�йؽӻ�ͷ��������� CYCLEINFO ���У�������Ի�ͷ�����������ŵȲ���ʱ�Ų��� CYCLEINFO_FLOAT
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
// ע����������( RasterArray -> PolyPolygon) ��CLoopGrpOpt��
class RP_EXT_CLASS CRstLines
{
public:
	enum COMBINE_MODE { COMBINE_AND=5, COMBINE_OR, COMBINE_DIFF, COMBINE_XOR};
	enum DATA_TYPE { DATA_OK, DATA_NULL, DATA_ERROR,DATA_EMPTY};	//DATA_EMPTY������ʱdelete

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
	//�ڽӻ�ͷʱ�ϲ���ͷ���ɨ������,���غϲ������
	//lpSrcRstLines  : Դɨ����ָ��
	//lpDstRstLines	 : Ŀ��ɨ����ָ��
	//return -1 : no changed, 0 : failed , 1 : succeed
	static int  CombineCycleRst(CRstLines* lpDstRstLines,const CRstLines* lpSrcRstLines, const ACycle* pCycle=NULL);//add by lcw 2003-7-30

	static int  Combine(const RSRLINEArray* lpRasterArray1, const RSRLINEArray* lpRasterArray2, int nCombineMode, LPRSRLINEArray& pResult,const ACycle* pCycle=NULL);

	//only set pRstArray to m_pRasterArray,not copy. while release CRstLines, not delete pRstArray
	static CRstLines* FromHand(const RSRLINEArray* pRstArray);
	static void FromHand(CRstLines* pObj, const RSRLINEArray* pRstArray);

	// return RasterArray count . after useing , need Release!
	static int DividebyLayer(const RSRLINEArray* lpRst, const RECT& rcLayer, LPRSRLINEArray* pResult, const ACycle* pCycle=NULL);

	//  ���Բ���
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
	// ��������
	static BOOL Combine(RSRLINEArray* pDst, const RECT& rcSrc, const RSRLINE* pRaster, int nCombineMode);
	static BOOL ANDPolylines (const RSRLINE* lpRaster1, const RSRLINE* lpRaster2,LPRSRLINEArray lpRasterArray);
	static BOOL ORPolylines  (const RSRLINE* lpRaster1, const RSRLINE* lpRaster2,LPRSRLINEArray lpRasterArray);
	static BOOL DIFFPolylines(const RSRLINE* lpRaster1, const RSRLINE* lpRaster2,LPRSRLINEArray lpRasterArray);
	static BOOL XORPolylines (const RSRLINE* lpRaster1, const RSRLINE* lpRaster2,LPRSRLINEArray lpRasterArray);

	// ����Raster
	static LPRSRLINE CreateRaster(const RSRLINE*);
	static LPRSRLINE CreateRaster(const RECT& rcRgn, BOOL bRect=TRUE,double dAngle=0,POINT* ptCent=NULL);
	static LPRSRLINE CreateRaster(const RECTD* rcRgn, const ATransform* Trans);
	static LPRSRLINE CreateRaster(const POINT* lpPt, int nCount, int nFillMode=ALTERNATE);
	static LPRSRLINE CreateRaster(const ANLOOP*, int nFillMode=ALTERNATE);

	// ����RasterArray
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

	// �ָ�ɨ����, ���ص�ɨ����(RSRLINEArray�ڰ�����RSRLINE) �� Windows �� Polygon ��Ӧ.
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
	//�����жϱպͶ���εĻ��Ʒ���,����TRUEΪ˳ʱ�뷽�򣬷�֮Ϊ��ʱ�뷽��
	static BOOL JudgeDirection(const POINT* lpPt, int nCount);

public:
	//�� pRsrLine ����� rcBox ���������Ҿ���
	static void HorMirror(LPRSRLINE pRsrLine, const CRect& rcBox);
	//�� pRsrLine ����� rcBox ���������¾���
	static void VerMirror(LPRSRLINE pRsrLine, const CRect& rcBox);
	//�� pRasterArray ����� rcBox ���������Ҿ���
	static void HorMirror(LPRSRLINEArray pRasterArray, const CRect& rcBox);
	//�� pRasterArray ����� rcBox ���������¾���
	static void VerMirror(LPRSRLINEArray pRasterArray, const CRect& rcBox);

	//ɨ������������������Ӿ��� m_rcBox �����߾���
	void HorMirror();
	void VerMirror();

public:
	BOOL GetBaryCenter(POINTD &PointBC);
	static BOOL GetBaryCenter(LPRSRLINEArray lpRsrAry , POINTD  & point);
	//ɨ����������
	static LPRSRLINEArray ExpandRaster(const LPRSRLINEArray lpSrc, int nAdd);
	static LPRSRLINEArray ContractRaster(const LPRSRLINEArray lpSrc, int nSub);
};
typedef CRstLines* PRstLines;

// ��������( RasterArray -> PolyPolygon)
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
	
	// ��������		IntArray.GetSize() == nNumPolygon
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

//���ܸ�Ҫ: ��ɫ������ࡣ
//		AClrGroup������ɫ���飬����ҡ����롢ɾ����ɫ�ȣ��������ݱ��浽�����ļ��С�
//ע�⣺
//   AClrGroup������������ɫ�飨32λ��COLORREF������Index��ɫ������AByteArray��
//	 AClrGroup����δ�������ܵ��Ż�������ɫ�����ǰ�˳����ҵġ������ṩʹ�õķ����ԣ�
//	��˲��ʺ���������Ҫ��ߵĳ��ϣ���ʱӦ�Լ��Ż�����
//
//ɫ��״̬{��ͨ��ȡ������Ч}
enum AClrGrpState {cgsNormal=0, cgsInverse, cgsDisable};
class RP_EXT_CLASS AClrGroup : public CObject
{
	DECLARE_SERIAL(AClrGroup)
public:
	AClrGroup();
	~AClrGroup();
	AClrGroup& operator = (const AClrGroup& src);

public:
	//ɫ��״̬
	void State(AClrGrpState nState) {m_nState=nState;}
	AClrGrpState State() const {return m_nState;}

public:
	//��־ɫ
	COLORREF FlagColor() const;
	//�趨��־ɫ
	//	clr		:[in]��־ɫ��ɫֵ
	//	bAdd	:[in]�Ƿ���ӵ�ɫ����
	//���أ�
	//	��clr�ڵ�ǰɫ����ʱ����������ɫ���е����
	//	��clr���ڵ�ǰɫ����ʱ������bAdd�������в���
	//		bAdd==TRUE : ���clr����ǰɫ��ĩβ
	//		bAdd==FALSE: ��clr�滻��ǰ��־ɫ���������ڵ�ǰ��־ɫʱ����ӵ�ɫ��ĩβ
	int FlagColor(COLORREF clr, BOOL bAdd=FALSE);
	//��־ɫ���
	// Zero-base index in color-array.
	int FlagPosition() const {return m_nFlag;}
	BOOL FlagPosition(int nPos);

	//����ɫ���е���ɫ����
	const CClrArray& ColorArray() const {return m_Color;}
	//��Ҫ�ӿڣ�[in], ����һ��ɫ���滻��ǰ��ɫ��(����һ��)
	void ColorArray(const CClrArray& clrArray) {m_Color.Copy(clrArray);}
	//��Ҫ�ӿڣ�[out],�õ���ɫ��ĳ��ȡ�
	int GetCount() const {return m_Color.GetSize();}
	//��Ҫ�ӿڣ�[out],�õ���ɫ���ָ�롣
	const COLORREF* GetData() const {return m_Color.GetData();}
	//��Ҫ�ӿڣ�[out],�õ���ɫ��ĵ�nIndex����ɫ��
	//    nIndex: [in], ָ����Ҫ����ɫ����ɫ���е����
	//    return: COLORREF���͵���ɫ
	COLORREF GetAt(int nIndex) const {return m_Color.GetAt(nIndex);}
	//��Ҫ�ӿڣ�[in], ����ɫ���õ���ɫ���еĵ�nIndex��λ���С�
	//    nIndex: [in], ָ����ɫ���е�λ��
	void SetAt(int nIndex, COLORREF newClr) {m_Color.SetAt(nIndex, newClr);}
	//��Ҫ�ӿڣ�[out], ����ָ����ɫ����ɫ���е�λ�á�
	//    clr: [in], ָ����ɫ
	//	  nStart: [in], ��ʼ���ҵ�λ��
	// return: [out], ����ɫ���е�λ�á����Ҳ���������NULLINDEX
	int	 Find(COLORREF clr, int nStart=0) const; 
	//��Ҫ�ӿڣ�[out], ����ָ����ɫ����ɫ���е�λ�á�
	//    pBytes: [in], ָ����ɫ(��RGBģʽ��ʾ��B=pBytes[0]��G=pBytes[1]�� R= pBytes[2])
	//	  nStart: [in], ��ʼ���ҵ�λ��
	// return: [out], ����ɫ���е�λ�á����Ҳ���������NULLINDEX
	int  Find(const BYTE* pBytes, int nStart) const;

	//��Ҫ�ӿڣ�[out], �ж�ָ����ɫ�ǲ�������ɫ����.
	// clr:[in], ָ������ɫ
	// return: ����FALSE(TRUE)��ʾ��(����)��ɫ����.
	//ע��:
	//	��m_nState==cgsNormalʱm_Color��������ɫ��, 
	//	��m_nState==cgsInverse,m_Color�������ɫ����ɫ��
	//	��m_nState==cgsDisable��ʼ�շ���TRUE
	BOOL NoClaim(COLORREF clr) const; 
	//��Ҫ�ӿڣ�[out], ͬNoClaim(COLORREF clr),ֻ����BYTE�����ʾ��ɫ
	BOOL NoClaim(const BYTE* pBytes) const;

	//��ɫ���ڲ�������ɫֵ����ż䣩�ƶ�
	//	nFromIndex	:[in]ָ��Դ���
	//	nToIndex	:[in]ָ��Ŀ�����
	//ע��������nFromIndex����Ӧ����ɫ���ƶ������뵽 nToIndexλ��
	BOOL Migrate(int nFromIndex, int nToIndex);
	//��Ҫ�ӿ�: ����ɫ��������ɫ
	//  pColor: [in], ָ�����ӵ���ɫ�����ݿ�
	//  nCount: [in], pColor�ĳ���
	//  return: ԭ��ɫ��ĳ���,��ʧ�ܷ���-1
	int  Add(const COLORREF* pColor, int nCount=1);
	//��Ҫ�ӿ�: ����ɫ�������ɫ
	//  nIndex: [in], �����λ��
	//  pColor: [in], ָ�����ӵ���ɫ�����ݿ�
	//  nCount: [in], pColor�ĳ���
	//  return: ʵ�ʲ������ɫ��
	int  InsertAt(int nIndex, const COLORREF* pColor, int nCount=1);
	//��Ҫ�ӿ�: ����ɫ����������ɫ
	//  pColor: [in], Ҫ���ߵ���ɫ
	//  nCount: [in], pColor�ĳ���
	//  return: ʵ�����ߵ���ɫ��
	int  Remove(const COLORREF* pColor, int nCount=1);
	//��Ҫ�ӿ�: ��λ��nIndex��ʼ,����ɫ��������nCount����ɫ
	//  nIndex: [in], Ҫ���ߵ���ɫ��ʼλ��
	//  nCount: [in], Ҫ���ߵ���ɫ��
	BOOL RemoveAt(int nIndex, int nCount=1);
	//��Ҫ�ӿ�: ����ɫ��������������ɫ
	BOOL RemoveAll();

protected:
	int				m_nFlag;
	CClrArray		m_Color;
	AClrGrpState	m_nState;

public:
	//��Ҫ�ӿ�: ���ļ��ж�����ɫ��
	// pFile: [in], ָ��������ļ�, ReadFileֱ��ʹ��pFile, �����Ҫ�������Ƚ�pFile���ļ�(pFile->Open())
	BOOL ReadFile(CFile* pFile);
	//��Ҫ�ӿ�: ����ɫ��д���ļ���, ��������ͬReadFile(CFile* pFile)
	BOOL SaveFile(CFile* pFile) const;
};

#define MAP_MAXNUM	256
//���ܸ�Ҫ: BYTE���������, ��Ҫ����Index��ɫ��Ĺ���, ������й��Ż�,�����ڸ�����Ҫ��ĳ���
//  AByteArray����������ɫ��Ϊ256
class RP_EXT_CLASS AByteArray
{

public:
	AByteArray();

public:
	//��Ҫ�ӿ�: [in], ��AClrGroup��ɫ�龭ת�����Ƶ���ǰ����
	AByteArray& operator = (const AClrGroup& src);
	//��Ҫ�ӿ�: [in], ��AByteArray��ɫ�鸴�Ƶ���ǰ����
	AByteArray& operator = (const AByteArray& src);
	//��Ҫ�ӿ�: [in], ������ɫ��״̬(����ģʽ\ȡ��ģʽ\ʧЧģʽ)
	void State(AClrGrpState nState) {m_nState=nState;}
	//��Ҫ�ӿ�: [out], �õ���ǰ����ɫ��״̬
	AClrGrpState State() const {return m_nState;}
	//��Ҫ�ӿ�: [out], �õ���ǰ��ɫ���һ�ݿ���, ����clrGroup��
	int GetClrGrp(AClrGroup& clrGroup) const;

public:
	//��Ҫ�ӿ�: ����ɫ�������ɫ
	//  pByte : [in], �������ɫ
	//  nCount: [in], pByte�ĳ���(����ɫ��)
	void Insert(const BYTE* pByte, int nCount=1);
	//��Ҫ�ӿ�: ������ɫ���е���ɫ
	//  pByte : [in], �������ɫ
	//  nCount: [in], pByte�ĳ���(����ɫ��)
	void Modify(const BYTE* pByte, const BYTE* pNewByte);
	//��Ҫ�ӿ�: ����ɫ��������ɫ
	//  pByte : [in], Ҫ���ߵ���ɫ
	//  nCount: [in], pByte�ĳ���(����ɫ��)
	void Remove(const BYTE* pByte, int nCount=1);
	//��Ҫ�ӿ�: �����ɫ���е���ɫ
	void EmptyAll();
	BOOL IsNull() const;
	BOOL IsAll() const;
	//��Ҫ�ӿڣ�[out], �ж�ָ����ɫ�ǲ�������ɫ����.
	// pByte:[in], ָ������ɫ
	// return: ����FALSE(TRUE)��ʾ��(����)��ɫ����.
	//ע��:
	//	��m_nState==cgsNormalʱm_Color��������ɫ��, 
	//	��m_nState==cgsInverse,m_Color�������ɫ����ɫ��
	//	��m_nState==cgsDisable��ʼ�շ���TRUE
	BOOL NoClaim(const BYTE* pByte) const; //Protected, Transparent, ... 
	
	//���ص�ǰ��BYTEɫ�黺������Ϊ��������������ʹ�ã�
	const BYTE* IndexMapPointer(WORD* pSize=NULL) const
	{ if (pSize) *pSize=m_nSize; return m_bIndexMap; }
protected:
	AClrGrpState	m_nState;
	BYTE			m_bIndexMap[MAP_MAXNUM];	// size is 256, Only set 0 or 1.
	WORD			m_nSize;					// max is 256, m_bIndexMap �е���Чλ����ӳ�������Ч������
};

typedef CArray<BGRVALUE, BGRVALUE>	CBgrArray;	

//���ܸ�Ҫ: ��ɫ�������, ��Ҫ����CBgrArray��ɫ��Ĺ���, ������й��Ż�,�����ڸ�����Ҫ��ĳ���
// ����������AClrGroup,AByteArray
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
	//��Ҫ�ӿڣ�[out], �ж�ָ����ɫ�ǲ�������ɫ����.
	// pBgr:[in], ָ������ɫ(RGB)
	// return: ����FALSE(TRUE)��ʾ��(����)��ɫ����.
	//ע��:
	//	��m_nState==cgsNormalʱm_Color��������ɫ��, 
	//	��m_nState==cgsInverse,m_Color�������ɫ����ɫ��
	//	��m_nState==cgsDisable��ʼ�շ���TRUE
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
//ͼ��˳��
enum  IMAGE_ORDER	
{
	IMAGE_UNKNOWN		= 0,	//δ֪˳��
	IMAGE_LINE_DOWNTOUP,		//���µ�������
	IMAGE_LINE_UPTODOWN,		//���ϵ�������
	IMAGE_ROW_LEFTTORIGHT,		//����������
	IMAGE_ROW_RIGHTTOLEFT,		//���ҵ�������
	IMAGE_BLOCK_DOWNTOUP,		//���µ��Ͽ���
	IMAGE_BLOCK_UPTODOWN		//���ϵ��¿���
};
//ͼ���ļ���Ϣ����Ҫ�����ļ�����
//ע��
//	�޸ı����ݽṹʱ���������ڽṹβ����ӱ������Ҳ������޸Ļ�ɾ�� Reserve ��ǰ�Ĳ�������
//	��أ�AFile, inside.h

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
	WORD			Compression;		//ѹ����ʽ
	
	Float64			XRes, YRes; 		//dpmm
	
	PALETTEENTRY	Palette[256]; 		//BYTE:    peRed,peGreen,peBlue,peFlags;
	WORD			palNumUsed;			// 1--256
	
	TYPE			Type;				//Image����, ��RGB,GRAY,INDEX,CMYK��
	BYTE			BitChannel; 		//8��16λ/ͨ��
	
	DWORD			Reserve;			//����
	//---------------���ϵĲ������ͼ����������������޸� --------------------------//

	//��չ��ɫ����Ϣ
	KY_EXPANDPALETTE	m_expandPalette;	//��չ��ɫ����Ϣ

};
//ͼ����Ϣ����Ҫ����ϵͳͼ�����ɴ�AIMAGEINFO��ת���õ�
#pragma pack(push)
#pragma pack(1)
struct RP_EXT_CLASS APictureInfo
{
	// ���: Pixel
	LONG		Width; 
	LONG		Height;
	WORD		BitCount;

	// ����ֵ����λΪ g_Flags.ResUnit;
	double		XRes, YRes; 
	double		XOutRes, YOutRes;	//	Used by paint, transform graph to image, screen etc.
	
	CPalette*	pPalette;
	WORD		Compression; 

	KY_EXPANDPALETTE	m_expandPalette;	//��չ��ɫ����Ϣ

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
	//�ڲ��Ƕ� pInfo �����ò�������������ֵΪFALSEʱ��Ҫ�ⲿ�����ͷ� pInfo ��Դ
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
	
	// ����ֵ����λΪ g_Flags.ResUnit;
	double XRes () const {return m_Info.XRes;}
	double YRes () const {return m_Info.YRes;}
	double XOutRes () const {return m_Info.XOutRes;}
	double YOutRes () const {return m_Info.YOutRes;}

	WORD Compress () const {return m_Info.Compression;}

	virtual void SetWidth (int nWidth);
	virtual void SetHeight (int nHeight);
	virtual void SetBitCount (WORD wBitCount);

	// ����ֵ����λΪ g_Flags.ResUnit;
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
	//ֻ������Ӧ�ľ��Ȳ��������ı���������
	BOOL SetSameRes (const APictureInfo* pInfo);

public:
	//	Coordination is relative to layout
	//	If prcPaint is NULL, paint on all area of related views.
	//	If pView is NULL, paint for all view
	virtual void Repaint(AScrollView* pView, const RECT* prcPaint);

public:
	virtual const CPalette* Palette() const;
	virtual DWORD ColorUsed () const;

	//���õ�ɫ��
	//bResetExpandPalette :[in]�Ƿ������չ��ɫ����Ϣ
	virtual UINT SetPalEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPalEntry, BOOL bResetExpandPalette);
	virtual BOOL SetPalette(const CPalette *pPalette, BOOL bResetExpandPalette);
	//RGB��ɫ��ı�
	virtual void PaletteModified();
	//��չ��ɫ��ı�
	virtual void ExpandPaletteModified();

	//����չ��ɫ��
	//	bForce	:[in]�Ƿ�ǿ�Ʒ����ڲ���չ��ɫ�������
	//			��ΪFALSEʱ��������Ϣ����Ч�Խ����жϣ�����Ч��չ��ɫ����Ϣʱ���� NULL
	virtual const KY_EXPANDPALETTE* GetExpandPalette(BOOL bForce) const;
	//����չ��ɫ��
	//	bForce	:[in]�Ƿ�ǿ�Ʒ����ڲ���չ��ɫ�������
	//			��ΪFALSEʱ��������Ϣ����Ч�Խ����жϣ�����Ч��չ��ɫ����Ϣʱ���� 0
	//���أ���ɫ����ɫ��
	virtual int GetExpandPalette(KY_EXPANDPALETTE &expandPalette, BOOL bForce) const;
	//����չ��ɫ��
	//	bGray :[in]�Ƿ�Ϊ�Ҷȸ�/ɫ���壬��ΪTRUEʱ����nClrUsed����ȡ��һ��ͨ��ɫ��Ϣ
	virtual BOOL SetExpandPalette(const KY_EXPANDPALETTE *pExpandPalette, int nClrUsed, BOOL bGray);
	//��չ��ɫ��ʹ��
	virtual BOOL ExpandPaletteEnable() const;
	virtual BOOL ExpandPaletteEnable(BOOL bEnable);
	//�Ƿ�֧����չ��ɫ�����
	virtual BOOL SupportExpandPalette() const;

protected:
	BOOL m_bExpandPaletteEnable;	//�Ƿ�ʹ����չ��ɫ��
};

//////////////////////////////////////////////////////////
//	ELLPARAM�ṹ: ��ʾ��Բһ����η��̵�ϵ��
//	��Բһ����η���: dPA*X*X + dPB*X*Y + dPC*Y*Y + dPD*X + dPE*Y + dPF = 0
//	���е�ϵ������˫�������ͱ�ʾ
//	dPA: X�Ķ�����ϵ��
//	dPB: X*Y�Ķ�����ϵ��
//	dPC: Y�Ķ�����ϵ��
//	dPD: X��һ����ϵ��
//	dPE: Y��һ����ϵ��
//	dPF: ������ϵ��
//
struct RP_EXT_CLASS ELLPARAM
{
	ELLPARAM();
	ELLPARAM(const ELLPARAM& eParam);

	double dPA, dPB, dPC, dPD, dPE, dPF;
};

// ELLBOUND�ṹ: ��ʾ��Բ�����β���
// ptBound: ��Բ������ˮƽ������������ֱ����
//			����Բ���е��ĸ��е�
// pptBound[0] : ���е�
// pptBound[1] : ���е�
// pptBound[2] : ���е�
// pptBound[3] : ���е�
// ptCenter: ��Բ�����ĵ�
// rcBorder: ��Բ����Ӿ���
//
struct RP_EXT_CLASS ELLBOUND
{
	POINTD	ptBound[4];
	POINTD	ptCenter;
	CRect	rcBorder;
};

//��Բ��ɢʱʹ�õĺ�������
typedef BOOL (*ELLSEPPROC)(const ELLPARAM& eParam, int& nX, int& nY, int& nCount, const int& nSize, LPPOINT pptEnd, CPOINTArray& ptArray);

//////////////////////////////////////////////////////
// CEllipse��: �����е㷨��ɢ��Բ,����,ֱ��
//
class RP_EXT_CLASS CEllipse
{
public:
	// Get seperate point
	//ֱ����ɢ����
	//	ptStart, ptEnd: [in]�����ʾ����ɢ��ֱ��
	//	ptArray       : [out]POINT�ṹ���飬����ֱ�ߵ���ɢ��
	static void MidPointLine(POINT ptStart, POINT ptEnd, CPOINTArray& ptArray);

	//������ɢ����
	//	rect   : [in]��ʾ����ɢ�ľ���
	//	dAngle : [in]���ε���ת�Ƕȣ��û��ȱ�ʾ
	//	ptArray: [out]POINT�ṹ���飬���վ��ε���ɢ��
	static void MidPointRect(const RECT& rect, double dAngle, CPOINTArray& ptArray)
	{ BOUNDANGLE(dAngle); POINT pt[4]; RotateRect(rect, dAngle, pt); MidPointParalgram(pt, ptArray); }
	static void MidPointRect(const RECT& rect, const ATransform& Trans, CPOINTArray& ptArray)
	{ POINT pt[4]; RectVertex(rect, pt); Trans.CalcPoints(pt, 4); MidPointParalgram(pt, ptArray); }

	// �ı�����ɢ����
	//	pt     : [in]��ʾ����ɢ���ı��ε��ĸ�����
	//	ptArray: [out]POINT�ṹ���飬�����ı��ε���ɢ��
	static void MidPointParalgram(const POINT pt[4], CPOINTArray& ptArray);


	// Բ��ɢ����
	//	rc     : [in]��ʾԲ����Ӿ���
	//	ptArray: [out]POINT�ṹ���飬����Բ����ɢ��
	static void MidPointCircle(const CRect& rc, CPOINTArray&);
	// ��Բ��ɢ����
	//	rect   : [in]��ʾ����ɢ������Բ����Ӿ���
	//	dAngle : [in]����Բ����ת�Ƕȣ��û��ȱ�ʾ
	//	Trans  : [in]����Բ�ĵı任����
	//	ptArray: [out]POINT�ṹ���飬������Բ����ɢ��
	static void MidPointEllipse(const RECT& rc, double dAngle, CPOINTArray&);
	static void MidPointEllipse(const RECT& rc, const ATransform& Trans, CPOINTArray& ptArray)
	{ RECTD rcd(rc); MidPointEllipse(rcd, Trans, ptArray); }
	static void MidPointEllipse(const RECTD& rc, const ATransform& Trans, CPOINTArray&);

	// ��Բ����ɢ����
	//	rect   : [in]��ʾ����ɢ������Բ����Ӿ���
	//	Trans  : [in]����Բ�ı任����
	//	ptArray: [out]POINT�ṹ���飬������Բ����ɢ��
	//	dAngStart, dAngEnd: [in]���������Բ��Բ������ʼ�ǶȺ���ֹ�Ƕȣ��Ƕ�Ϊ��ʱ�뷽��
	static void MidPointArc(const RECTD& rc, const ATransform& Trans, double dAngStart, double dAngEnd, CPOINTArray&);
	static void MidPointArc(const RECT& rc, const ATransform& Trans, double dAngStart, double dAngEnd, CPOINTArray& ptArray)
	{ RECTD rcd(rc); MidPointArc(rcd, Trans, dAngStart, dAngEnd, ptArray); }

	// ����Բ��һ��ĺ���
	//	eParam	: [in]��ʾ��Բ��һ����η��̵�ϵ��
	//	pptCenter, dAngle: [in]��ʾ�������pptCenter�������ɵ�ֱ����X��ļн�ΪdAngle
	//						���ָ��pptCenterΪ�գ�������Բ�����ĵ���档
	//	point	: [out]POINT/POINTD�ṹ����������ĵ�
	static BOOL EllipsePoint(const ELLPARAM& eParam, const POINT* pptCenter, double dAngle, POINT& point);
	static BOOL EllipsePoint(const ELLPARAM& eParam, const POINTD* pptCenter, double dAngle, POINTD& point);

	// ����Բ��һ��ĺ���
	//	rect	: [in]��ʾ����ɢ������Բ����Ӿ���
	//	Trans	: [in]����Բ�ı任����
	//	point	: [out]POINT/POINTD�ṹ����������ĵ�
	//	pptCenter, dAngle: [in]��ʾ�������pptCenter�������ɵ�ֱ����X��ļн�ΪdAngle
	//						���ָ��pptCenterΪ�գ�������Բ�����ĵ���档
	static BOOL EllipsePoint(const RECTD& rc, const ATransform& Trans, const POINT* pptCenter, double dAngle, POINT& point);
	static BOOL EllipsePoint(const RECT& rc, const ATransform& Trans, const POINT* pptCenter, double dAngle, POINT& point)
	{ RECTD rcd(rc); return EllipsePoint(rcd, Trans, pptCenter, dAngle, point); }
	static BOOL EllipsePoint(const RECTD& rc, const ATransform& Trans, const POINTD* pptCenter, double dAngle, POINTD& point);
	static BOOL EllipsePoint(const RECT& rc, const ATransform& Trans, const POINTD* pptCenter, double dAngle, POINTD& point)
	{ RECTD rcd(rc); return EllipsePoint(rcd, Trans, pptCenter, dAngle, point); }

	// ����Բ��һ��ĺ���
	//	eParam	: [in]��ʾ��Բ��һ����η��̵�ϵ��
	//	eBound	: [in]��ʾ��Բ�����β���
	//	dAngle	: [in]����ĵ�����Բ�����ĵ������ɵ�ֱ����X��ļн�
	//	point	: [out]POINT/POINTD�ṹ����������ĵ�
	static BOOL EllipsePoint(const ELLPARAM& eParam, const ELLBOUND& eBound, double dAngle, POINT& point)
	{ POINTD pt; BOOL bRet = EllipsePoint(eParam, &eBound.ptCenter, dAngle, pt); CPrecise::PrecisePoint(pt, point); return bRet; }
	static BOOL EllipsePoint(const ELLPARAM& eParam, const ELLBOUND& eBound, double dAngle, POINTD& point)
	{ return EllipsePoint(eParam, &eBound.ptCenter, dAngle, point); }

public:	//ɨ���߲���
	//
	// ��ֱ�ߵ���ɨ���ߺ���
	//	ptStart, ptEnd: [in]�����ʾ����ɢ��ֱ��
	//	ptRelate: [in]���ݵ�ptRelate����ֱ�ߵ��ϰ��������°�������������ɨ���������ֱ�ߵ��ϰ��������°���
	//	nXArray : [out]�������飬����������ɨ����X���꣬ nXArray��Ԫ�ظ�����ʾ��ɨ���ߵ���Ŀ
	//			��ɨ���Y�����ptStart��Y���꿪ʼ��ptEnd��Y�������
	static void MidPointLine(POINT ptStart, POINT ptEnd, POINT ptRelate, CintArray& nXArray);

	// ����ε���ɨ���ߺ���
	//	rect   : [in]��ʾ����ɢ�ľ���
	//	dAngle : [in]���ε���ת�Ƕȣ��û��ȱ�ʾ
	//	pRect  : [in]���ڽ�����Ӿ���
	//	Trans  : [in]���εı任����
	//	ptArray: [out]POINT�ṹ���飬����������ɨ����X���꣬ ptArray��Ԫ�ظ�����ʾ��ɨ���ߵ���Ŀ
	//			ptArray��Ԫ�ص�X������ʾ��ɨ������˵��X���꣬ptArray��Ԫ�ص�Y������ʾ��ɨ�����Ҷ˵��X����
	//	rsrLine: [out]RSRLINE�ṹ����������ɨ����
	// ����ֵ : ��һ����ɨ���ߵ�Y����ֵ
	static int RectRasterLine(const RECT& rect, double dAngle, CPOINTArray& ptArray, LPRECT pRect = NULL);
	static int RectRasterLine(const RECT& rect, const ATransform& Trans, CPOINTArray& ptArray, LPRECT pRect = NULL);
	static int RectRasterLine(const RECT& rect, double dAngle, RSRLINE& rsrLine);
	static int RectRasterLine(const RECT& rect, const ATransform& Trans, RSRLINE& rsrLine);

	// ����Բ����ɨ���ߺ���
	//	rect   : [in]��ʾ����ɢ����Բ
	//	dAngle : [in]��Բ����ת�Ƕȣ��û��ȱ�ʾ
	//	Trans  : [in]��Բ�ı任����
	//	ptArray: [out]POINT�ṹ���飬����������ɨ����X���꣬ ptArray��Ԫ�ظ�����ʾ��ɨ���ߵ���Ŀ
	//			ptArray��Ԫ�ص�X������ʾ��ɨ������˵��X���꣬ptArray��Ԫ�ص�Y������ʾ��ɨ�����Ҷ˵��X����
	//	pRect  : [out]���ڽ�����Ӿ���
	//	rsrLine: [out]RSRLINE�ṹ����������ɨ����
	// ����ֵ : ��һ����ɨ���ߵ�Y����ֵ
	static int EllipseRasterLine(const CRect& rect, double dAngle, CPOINTArray&, LPRECT pRect = NULL);
	static int EllipseRasterLine(const RECTD& rect, const ATransform& Trans, CPOINTArray&, LPRECT pRect = NULL);
	static int EllipseRasterLine(const RECT& rect, const ATransform& Trans, CPOINTArray& ptArray, LPRECT pRect = NULL)
	{ RECTD rcd(rect); return EllipseRasterLine(rcd, Trans, ptArray, pRect); }
	static int EllipseRasterLine(const RECT& rect, double dAngle, RSRLINE& rsrLine);
	static int EllipseRasterLine(const RECT& rect, const ATransform& Trans, RSRLINE& rsrLine);
	static int EllipseRasterLine(const RECTD& rect, const ATransform& Trans, RSRLINE& rsrLine);

public:	// Get Ellipse's parameter
	// ��ֻ�нǶȱ仯����Բ��һ����η��̵�ϵ��
	//	ֻ�нǶȱ仯����Բ��һ����η���:dPA * X * X + dPB * Y * Y + dPC * X * Y - dPD = 0
	//	rect	: [in]��ʾ����Բ����Ӿ���
	//	dAngle	: [in]��Բ����ת�Ƕȣ��û��ȱ�ʾ
	//	Trans	: [in]��Բ�ı任����
	//	dPA, dPB, dPC, dPD: [out]���ڽ�����Բ��һ����η��̵�ϵ��
	//	eParam	: [out]���ڽ�����Բ��һ����η��̵�ϵ��
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
	// ����ε���Ӿ���
	//	rcBox   : [in]��ʾ���ľ���
	//	dAngle  : [in]���ε���ת�Ƕȣ��û��ȱ�ʾ
	//	Trans   : [in]���εı任����
	//	rcBorder: [out]���ڽ����������Ӿ���
	static void GetRectBorder(const RECT& rcBox, double dAngle, RECT& rcBorder);
	static void GetRectBorder(const RECT& rcBox, const ATransform& Trans, RECT& rcBorder);

	// ����Բ����Ӿ���
	//	rcBox   : [in]��ʾ����Բ����Ӿ���
	//	dAngle  : [in]��Բ����ת�Ƕȣ��û��ȱ�ʾ
	//	eParam  : [in]��ʾ��Բ��һ����η��̵�ϵ��
	//	Trans   : [in]��Բ�ı任����
	//	rcBorder: [out]���ڽ����������Ӿ���
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
	// ����Բ���β����ĺ���, ��POINT/POINTD�ṹ��ʾ��
	//	rcBox   : [in]��ʾ����Բ����Ӿ���
	//	Trans	: [in]��Բ�ı任����
	//	eParam      : [in]��Բ��һ����η��̵�ϵ��
	//	pptBound    : [out]������Բ������ˮƽ������������ֱ��������Բ���е��ĸ��е�
	// 		pptBound[0] : ���е�
	// 		pptBound[1] : ���е�
	// 		pptBound[2] : ���е�
	// 		pptBound[3] : ���е�
	//	prBorder    : [out]������Բ����Ӿ���
	//	pptCenter   : out]������Բ�����ĵ�
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

	// �Ѿ���ת��Ϊ�ĸ������ʾ�ĺ���
	//	rc	: [in]��ת���ľ���
	//	pt	: [out]�����ĸ����������
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
//��ȷ�ĺ�ʱ����
//����:
// CCostTime costTime;
// double dCostTime, dMaxTime, dMinTime, dCurrentTime;
// costTime.Init();
// costTime.Time_Init();
// costTime.Time_Exit();
// dCostTime = costTime.GetCostTime(&dMaxTime, &dMinTime, &dCurrentTime); //xxxx.xx ms
class RP_EXT_CLASS CCostTime
{
private:
	LONGLONG	m_lFrequency;				//��Ϊ0 ʱ���� GetTickCount()ȡֵ
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
	//dwFlag:������������ı�ǣ���ʾ��ǰ��ʱ��״̬
	void Time_Init(DWORD dwCurrentFlag);
	void Time_Exit();
	double GetCostTime(double *pMaxTime, double *pMinTime, double *pCurrentTime) const; //xxxx.xx ms
	BOOL GetCostTimeFlag(DWORD *pdwMaxTimeFlag, DWORD *pdwMinTimeFlag, DWORD *pdwCurrentFlag) const;
	//����ʱ��
	double GetTotoalTime() const;	//xxxx.xx ms
};

class RP_EXT_CLASS CErrorInfo
{
public:
	CErrorInfo();
	~CErrorInfo();

protected:
	BOOL				m_bError;		//����״̬��FALSE��ʾ�޴���
	CString				m_strError;		//������Ϣ

	//�ٽ����������ȷ������ͬ���̵Ĳ�ͬ�߳�ͬʱ����
	CRITICAL_SECTION	m_critical;	
public:
	//�����Ĵ�����Ϣ
	//���أ�
	//	TRUE �����ڴ���strError ��Ч
	//	FALSE���޴��󣬺��� strError
	BOOL IsError() const;
	BOOL GetLastError(CString &strError) const;

	//�������Ĵ�����Ϣ
	//	hInstance, nID	:[in]����ID���ڵ���Դʵ��������ڲ����ݴ���Ϣ�Զ���ȡ�ַ���ֵ
	//						ע�����޷�������ȡ�ַ�����Ϣʱ������FALSE
	//	lpFormat,...	:[in]������Ϣ�ַ�������ָ����ʽ��
	//ע��
	//	������ֵΪNULLʱ���Զ����������Ϣ����ֵ���� ClearError()
	BOOL SetLastError(HINSTANCE hInstance, UINT nID);
	BOOL SetLastError(LPCTSTR lpFormat, ...);
	
	//����ϵͳ����������ô�����Ϣ
	//	dwErrCode		:[in]ȫ�ִ�����룬ͬ Windows SDK�е�::GetLastError()���壬�� ERROR_OUTOFMEMORY (WinError.h)
	BOOL SetLastErrorFromSystem(DWORD dwErrCode);
	BOOL SetLastErrorFromSystem();

	//������д�����Ϣ
	void ClearError();

public:
	//ת�� windows SDK �е�ϵͳ������루::GetLastError()�����ַ���
	static CString TransSystemErrorCodeToString(DWORD dwErrCode);

};


class RP_EXT_CLASS CInkCountInfo
{
public:
	CInkCountInfo();
	~CInkCountInfo();

protected:
	static BYTE			m_bMap[256];

	//�ٽ����������ȷ������ͬ���̵Ĳ�ͬ�߳�ͬʱ����
	CRITICAL_SECTION	m_critical;	

	ULONGLONG			m_ulTotalCount;		//��������
	ULONGLONG			m_ulValidCount;		//��Ч����������<= m_ulTotalCount

public:
	static void InitMap();

	//��ʼ������ֵ
	void Init();

	//�õ�ǰͳ����Ϣ
	ULONGLONG GetValidCount(ULONGLONG *pulTotalCount) const;

	//�����Ϣ��
	//	pLineData		:[in]�����л�����
	//	width			:[in]�����л�������Ӧ�ĵ�������Pixel
	//	dwValidCount	:[in]��Ч��������Ϊ1��
	BOOL AddLineInfo_Mono(BYTE *pLineData, DWORD width);
	BOOL AddLineInfo_Index(BYTE *pLineData, DWORD width);
	BOOL AddLineInfo(DWORD dwValidCount, DWORD width);

public:
	static DWORD GetValidDataCount_Mono(BYTE *pLineData, DWORD width);
	static DWORD GetValidDataCount_Index(BYTE *pLineData, DWORD width);

};

//����ȫ�ֱ���ʵ������Ӧ�ó����ж���
extern RP_EXT_DATA AGFLAGS			g_Flags;				//ȫ�ֲ���
extern RP_EXT_DATA AStatusProgress	g_progress;				//��������ʾ������������ͨ�� SetPaintWnd ���趨ȫ�ֽ�������ʾλ��
extern RP_EXT_DATA AEnvObj			g_EnvObj;				//��������
extern RP_EXT_DATA CAppPath			g_AppPath;				//ϵͳ·��
extern RP_EXT_DATA AGV				g_AGV;					//���������������롢�ͷţ���װ��
extern RP_EXT_DATA AUndo			g_Undo;					//ȫ��UNDO����������RIPӦ�ó�����

extern RP_EXT_DATA HCURSOR			g_hCursor;				//ȫ�ֹ��

extern RP_EXT_DATA ARGBToCMYK		g_RGBtoCMYK;			//ȫ����� RGB<-->CMYK��RGB<-->LAB֮���ת����װ�ࣨ����BYTE��
extern RP_EXT_DATA ARGBToCMYK		g_InputCMYK;			//ȫ������ RGB<-->CMYK��RGB<-->LAB֮���ת����װ�ࣨ����BYTE��

extern RP_EXT_DATA CErrorInfo		g_Error;				//ȫ�ִ�����Ϣ

extern RP_EXT_DATA CString			g_strAppVersion;		//Ӧ�ó���汾��Ϣ��ȫ�֣�

