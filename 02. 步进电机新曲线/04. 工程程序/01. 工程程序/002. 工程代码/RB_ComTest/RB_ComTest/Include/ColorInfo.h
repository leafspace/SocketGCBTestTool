///////////////////////////////////////////////////////////////////////////
// ColorInfo.h	: ������īˮ�����ϣ���ص����ݽṹ
//
// Created by hzm 2010-09-03
//
///////////////////////////////////////////////////////////////////////////

#pragma once

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


// ��ɫ�����꣨��֧��RGB��ʽ��ϣ�
// ��ע�⣺
//	pBits, pSrc		: ����Ϊ���ʽ��pBits[0], pBits[1], pBits[2] �ֱ��Ӧ�� B�� G�� R
//	pBgr			: BGRVALUE*
//	MIX_XXXXXXX_0	: ��ʾΪ��͸���ȵĲ���
//	MIX_XXXXXXX_1	: ��ʾΪ��͸���ȵĲ���
//	MIX_XXXXIDX_X	: ��ʾ������������
//	MIX_XXXXX24_X	: ��ʾ�����ʸ壨24Bits������
//	opacPnt			: ��͸����(0-255)
//	wOpacity		: ͸����(255-opacPnt)
//
// ------------------- AnSeries ����ģʽ --------------------------
//
//��ͨ ����֧��RGB��ʽ��ϣ�
//
#define MIX_NORMALIDX_1(pBits,wOpacity,pBgr,opacPnt)		\
{	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][wOpacity]+ACV::M[pBgr->Blue][opacPnt]];		\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][wOpacity]+ACV::M[pBgr->Green][opacPnt]];	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][wOpacity]+ACV::M[pBgr->Red][opacPnt]];		\
}
#define MIX_NORMAL24_1(pBits,wOpacity,pSrc,opacPnt)			\
{	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][wOpacity]+ACV::M[*pSrc++][opacPnt]];	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][wOpacity]+ACV::M[*pSrc++][opacPnt]];	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][wOpacity]+ACV::M[*pSrc++][opacPnt]];	\
}
//
//���ϻ�ϣ���֧��RGB��ʽ��ϣ�
//
#define MIX_DYEMIXIDX_0(pBits,pBgr)	\
{	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][pBgr->Blue]];		\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][pBgr->Green]];		\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][pBgr->Red]];		\
}
#define MIX_DYEMIXIDX_1(pBits,opacPnt,pBgr)	\
{	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][0xFF-pBgr->Blue]]]];		\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][0xFF-pBgr->Green]]]];	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][0xFF-pBgr->Red]]]];		\
}
#define MIX_DYEMIX24_0(pBits,pSrc)	\
{	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][*pSrc++]];		\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][*pSrc++]];		\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][*pSrc++]];		\
}
#define MIX_DYEMIX24_1(pBits,pSrc,opacPnt)	\
{	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][0xFF-*pSrc++]]]];	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][0xFF-*pSrc++]]]];	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][0xFF-*pSrc++]]]];	\
}
//
//���ϣ���֧��RGB��ʽ��ϣ�
//
#define MIX_PHOMIXIDX_0(pBits,pBgr)		\
{	\
	*(pBits++)= ACV::Dbff[ACV::M[*pBits][0xFF-pBgr->Blue]];		\
	*(pBits++)= ACV::Dbff[ACV::M[*pBits][0xFF-pBgr->Green]];	\
	*(pBits++)= ACV::Dbff[ACV::M[*pBits][0xFF-pBgr->Red]];		\
}
#define MIX_PHOMIXIDX_1(pBits,opacPnt,pBgr)		\
{	\
	*pBits++= ACV::Dbff[(ACV::M)[*(pBits)][0xff-ACV::Dbff[(ACV::M)[opacPnt][(pBgr)->Blue ]]]];	\
	*pBits++= ACV::Dbff[(ACV::M)[*(pBits)][0xff-ACV::Dbff[(ACV::M)[opacPnt][(pBgr)->Green]]]];	\
	*pBits++= ACV::Dbff[(ACV::M)[*(pBits)][0xff-ACV::Dbff[(ACV::M)[opacPnt][(pBgr)->Red  ]]]];	\
}
#define MIX_PHOMIX24_0(pBits,pSrc)	\
{	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xff-*pSrc++]];		\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xff-*pSrc++]];		\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xff-*pSrc++]];		\
}
#define MIX_PHOMIX24_1(pBits,pSrc,opacPnt)	\
{	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][*pSrc++]]]];	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][*pSrc++]]]];	\
	*pBits++ = ACV::Dbff[ACV::M[*pBits][0xFF-ACV::Dbff[ACV::M[opacPnt][*pSrc++]]]];	\
}

//
//SetBits �С���ͨ��ģʽ��������֧��RGB��ʽ��ϣ�
//
#define NORMALMIXMASK(pDst, wOpacity, opacPnt, bgr) \
{	\
	*pDst++ = ACV::Dbff[ACV::M[*pDst][wOpacity] + ACV::M[bgr.Blue][opacPnt]];		\
	*pDst++ = ACV::Dbff[ACV::M[*pDst][wOpacity] + ACV::M[bgr.Green][opacPnt]];		\
	*pDst++ = ACV::Dbff[ACV::M[*pDst][wOpacity] + ACV::M[bgr.Red][opacPnt]];		\
}
#define NORMALMIX24MASK(pDst, wOpacity, opacPnt, pSrc) \
{	\
	*pDst++ = ACV::Dbff[ACV::M[*pDst][wOpacity] + ACV::M[*pSrc++][opacPnt]];		\
	*pDst++ = ACV::Dbff[ACV::M[*pDst][wOpacity] + ACV::M[*pSrc++][opacPnt]];		\
	*pDst++ = ACV::Dbff[ACV::M[*pDst][wOpacity] + ACV::M[*pSrc++][opacPnt]];		\
}

// ------------------- ��ī�ܶ�ģʽ ��HH�� PS��--------------------------
//
// ��ī��ϣ���֧��RGB��ʽ��ϣ�
//	bDst		:[in/out]Ŀ������������pal index��
//	bSrc		:[in]��ǰ����������pal index��
//	opacPnt		:[in]��ǰ�㲻͸����
//	bSrcLevel	:[in]��ǰɫ����ī������� opacPntʹ�ã�������ʸ����ʱһ��Ĭ��Ϊ g_Flags.nDefaultOpacity[dyemix_way_opacity]
#define MIX_DYEMIX_DNSITY_BASE(bDst,bSrc,bSrcLevel,opacPnt)		\
{	\
	bDst = ACV::Dbff[ACV::M[bSrc][0xFF-ACV::Dbff[ACV::M[0xFF-ACV::Dbff[ACV::M[opacPnt][bSrcLevel]]][0xFF-bDst]]]];		\
}
#define MIX_DYEMIXIDX_DENSITY_1(pBits,pBgr,bSrcLevel,opacPnt)			\
{	\
	MIX_DYEMIX_DNSITY_BASE(pBits[0],pBgr->Blue, bSrcLevel, opacPnt);	\
	MIX_DYEMIX_DNSITY_BASE(pBits[1],pBgr->Green,bSrcLevel, opacPnt);	\
	MIX_DYEMIX_DNSITY_BASE(pBits[2],pBgr->Red,	bSrcLevel, opacPnt);	\
	pBits += 3;															\
}
#define MIX_DYEMIXIDX_DENSITY_0(pBits,pBgr,bSrcLevel)	\
{	\
	MIX_DYEMIXIDX_DENSITY_1(pBits,pBgr,bSrcLevel,255);	\
}

#define MIX_DYEMIX24_DENSITY_1(pBits,pSrc,bSrcLevel,opacPnt)		\
{	\
	MIX_DYEMIX_DNSITY_BASE(pBits[0],pSrc[0], bSrcLevel, opacPnt);	\
	MIX_DYEMIX_DNSITY_BASE(pBits[1],pSrc[1], bSrcLevel, opacPnt);	\
	MIX_DYEMIX_DNSITY_BASE(pBits[2],pSrc[2], bSrcLevel, opacPnt);	\
	pBits += 3;														\
	pSrc  += 3;														\
}

//////////////////////////////////////////////////////////////
struct BGRVALUE
{
	BYTE	Blue;
	BYTE	Green;
	BYTE 	Red;
};
struct RGBVALUE
{
	double Red;				//	0.0 ~ 1.0  --> [0, 255]
	double Green;
	double Blue;
};
struct HSLVALUE
{
	double Hue;				//	0.0 ~ 1.0 --> [0, 360.0]
	double Sat;				//	0.0 ~ 1.0 --> [0, 100.0]
	double Lum;				//	0.0 ~ 1.0 --> [0, 100.0]
};
struct CMYKVALUE
{
	double Cyan;			// 0.0 ~ 1.0 --> [0, 255]
	double Magenta;	
	double Yellow;
	double Black;
};
struct LABVALUE
{					// ����ֵ		ʵ�� L*ab
	BYTE L;			// [0, 255] --> [0, 100.0]
	BYTE a;			// [0, 255] --> [-128.0, 127.0]	
	BYTE b;			// [0, 255] --> [-128.0, 127.0]	
};

struct ALOGPALETTE
{
	WORD         palVersion;
	WORD         palNumEntries;
	PALETTEENTRY palEntry[256];
};

struct ColorSpace_RGB
{
	double r;
	double g;
	double b;				// [0,1.0]  --> [0,255]
};
struct ColorSpace_Lab
{
	double l;				// [0,100.0]
	double a;				// [-128.0, 127.0]
	double b;				// [-128.0, 127.0]
};
struct ColorSpace_XYZ
{
	double X;
	double Y;
	double Z;
};

//Named Color 
struct NamedColor
{
	char name[256];
	int	 input[16];
	int	 output[16];
};

struct NamedColorList
{
	int			nColors;
	NamedColor  namedColorList[256];

	NamedColorList() { nColors = 0; }
};


enum INKCLRINDEX
{
	inkIndex_c = 0,
	inkIndex_m = 1,
	inkIndex_y,
	inkIndex_k,
	inkIndex_r,
	inkIndex_b,
	inkIndex_o,
	inkIndex_g,

	inkIndex_count
};	


//Ԥ������������
enum ky_printer_color
{
	kypc_nocolor		= 0,				//��ʼ��״̬����Ч�������ͣ�
	kypc_c				= 0x00000001,		// C��	�ࣨCyan��
	kypc_m				= 0x00000002,		// M��	Ʒ��Magenta�����������
	kypc_y				= 0x00000004,		// Y��	�ƣ�Yellow��
	kypc_k				= 0x00000008,		// K��	�ڣ�blacK��
	
	kypc_lc				= 0x00000010,		// lc�� ǳ�ࣨLight Cyan����	lc = C * 1/n 
	kypc_lm				= 0x00000020,		// lm�� ǳƷ��Light Magenta����	lm = M * 1/n
	kypc_ly				= 0x00000040,		// ly�� ǳ�ƣ�Ligth Yellow����	ly = Y * 1/n
	kypc_lk				= 0x00000080,		// lk�� ǳ�ڣ�Light Black����������ң�dark gray����lk = K * 1/n
	
	kypc_red			= 0x00010000,		// R��	�죨Red����������죬	R = M + Y
	kypc_green			= 0x00020000,		// Gr��	�̣�Green����			G = C + Y
	kypc_blue			= 0x00040000,		// B��	����Blue��������������	B = C + M
	kypc_orange			= 0x00080000,		// Or��	�ȣ�Orange����������ɫ��Or= Y + R
	
	//kypc_white			= 0x01000000,		// W��	�ף�White����W = C + M + Y
	
	kypc_spotcolor		= 0x80000000,		// Spot Color��רɫ���������ɫ��
	
	//����Ԥ���岿��������������
	kypc_lightcolor_mask= 0x00000FF0,		//ǳɫ��������
	kypc_basecolor_mask = 0x0000FFFF,		//��ɫ��������
	
	//����Ԥ���岿����ɫ���ģʽ
	kypc_cmyk			= kypc_c | kypc_m | kypc_y | kypc_k,	//CMYK
	kypc_cmyklclm		= kypc_cmyk | kypc_lc| kypc_lm,			//CMYKLcLm
	kypc_cmykog			= kypc_cmyk | kypc_orange | kypc_green,	//CMYKOG
	kypc_cmykrb			= kypc_cmyk | kypc_red | kypc_blue,		//CMYKRB
	kypc_cmykob			= kypc_cmyk | kypc_orange | kypc_blue,	//CMYKOB
	
	kypc_unknown		= 0xFFFFFFFF		//δ֪ɫ
};

//��չ��ɫ��
#define KY_MAX_CHANNELS				8					//ϵͳ���֧�ֵ���ɫͨ����

//Ŀ��īˮ����
//ע����ҪΪ����ʵ�ֹ����й̶����������ж��趨
//	Ϊ��֤���ܵ��ļ���ȡ�ļ����ԣ�ԭ���ϲ������޸� orderink_xxxx_start ��ֵ
#define ORDERINK_BASECOLOR_MAX		20					//���֧�ֵĻ�ɫ��
enum ORDER_INKCOLOR
{
	//------------ base color ---------------
	orderink_base_start = 0,							//��ɫīˮ-��ʼ
	orderink_base_c = orderink_base_start + 0,			//	kypc_c
	orderink_base_m = orderink_base_start + 1,			//	kypc_m
	orderink_base_y,									//	kypc_y
	orderink_base_k,									//	kypc_k
	orderink_base_r,									//	kypc_red
	orderink_base_b,									//	kypc_blue
	orderink_base_g,									//	kypc_green
	orderink_base_o,									//	kypc_orange

	orderink_base_end,									//��ɫīˮ-��������������
	orderink_base_count = orderink_base_end - orderink_base_start,	//��ɫīˮ������<=ORDERINK_BASECOLOR_MAX

	//------------ ligth color ---------------
	orderink_light_start = ORDERINK_BASECOLOR_MAX,						//ǳɫīˮ-��ʼ
	orderink_light_lc	= orderink_light_start + inkIndex_c,			//	kypc_lc
	orderink_light_lm	= orderink_light_start + inkIndex_m,			//	kypc_lm
	orderink_light_ly	= orderink_light_start + inkIndex_y,			//	kypc_ly
	orderink_light_lk	= orderink_light_start + inkIndex_k,			//	kypc_lk

	//orderink_light_llk,									//	kypc_llk

	orderink_light_end,										//ǳɫīˮ-��������������
	orderink_light_count = orderink_light_end - orderink_light_start,	//ǳɫīˮ������<=ORDERINK_BASECOLOR_MAX

	//------------ spot color ---------------
	orderink_spot_start = 200,											//רɫīˮ-��ʼ
	orderink_spot_0 = orderink_spot_start + 0,							//	kypc_spotcolor
	orderink_spot_1 = orderink_spot_start + 1,
	
	//orderink_spot_n = orderink_spot_start + n,

	orderink_spot_count = ORDERINK_BASECOLOR_MAX,						//רɫīˮ����
	
	orderink_spot_end	= orderink_spot_start + orderink_spot_count,	//רɫīˮ-��������������

	orderink_inkcount	= 0xFE,											//���īˮ������

	orderink_unknown	= 0xFF
};
#define orderink_base_id(i)			(orderink_base_start+(i))
#define orderink_base_index(id)		((id)-orderink_base_start)
//�Ƿ�Ϊ��ɫ
#define is_orderink_base(id)		((orderink_base_start<=(id)) && ((id)<orderink_base_end))

#define orderink_light_id(i)		(orderink_light_start+(i))
#define orderink_light_index(id)	((id)-orderink_light_start)
//�Ƿ�Ϊǳɫ
#define is_orderink_light(id)		((orderink_light_start<=(id)) && ((id)<orderink_light_end))

#define orderink_spot_id(i)			(orderink_spot_start+(i))
#define orderink_spot_index(id)		((id)-orderink_spot_start)
//�Ƿ�Ϊרɫ
#define is_orderink_spot(id)		((orderink_spot_start<=(id)) && ((id)<orderink_spot_end))

//��ɫģʽ
//ע�������µ���ɫģʽʱ������ӵ����INKMODE_MAX ǰ��
enum INKMODE
{
	INK_NONE			= 0,

	INK_CMYK			= 1,
	INK_CMYKLcLm		= 2,
	INK_CMYKLcLmLyLk	= 3,

	INK_CMYK_N1CLR		= 4,
	INK_CMYK_N2CLR		= 5,
	INK_CMYK_N3CLR		= 6,
	INK_CMYK_N4CLR		= 7,

	INK_CMYKLcLm_N1CLR	= 8,
	INK_CMYKLcLm_N2CLR	= 9,

	INK_CMYKLcLmLk		= 10,
	INK_CMYKLcLmLk_N1CLR= 11,

	//insert new mode here
	INK_MONO			= 12,	//��ɫ����ɫ�ԡ�רɫ����ʶ

	INK_CMYKOB			= 13,
	INK_CMYKLcLmLkB		= 14,
	INK_CMYKLcLmOB		= 15,
	INK_CMYKRBOG		= 16,

	INKMODE_MAX,
};

struct KY_INKVALUE
{
	// ʵ�ʶ�ͨ����ɫ��Ϣ
	double	dInkValue[KY_MAX_CHANNELS];	//	ÿ��ͨ����īˮֵ (0,255)���� KY_INKINFO ���ʹ��
};

//īˮ��Ϣ
struct KY_INKINFO
{
	BYTE	bInkCount;									// <= KY_MAX_CHANNELS
	BYTE	bInkOrder[KY_MAX_CHANNELS];				//īˮ����, ORDER_INKCOLOR
};

//��չ��ɫ����Ϣ
struct RP_EXT_CLASS KY_EXPANDPALETTE
{
	KY_INKINFO			inkInfo;			//������չ��ɫ����Ϣ��inkInfo.bInkCount==0 ʱ��ʾ����չ��ɫ��
	KY_INKVALUE			inkValue[256];		//ʵ�ʸ����� pPalette ����ɫ��һ��

public:
	BOOL LoadFile(CFile* pFile);
	
	//�浵ʱ����ռ�
	static DWORD StoreFileSize();

	BOOL StoreFile(CFile* pFile) const;

};

class RP_EXT_CLASS AClrObj
{

public:
	// if nBitCount==1  
	//		then call CreateMonoPalette((COLORREF)dwNumPalEntries)
	// else if (nBitCount==8) and (imgType==imgGrayType or imgType==imgClrGrayType) 
	//		then call CreateGrayPalette((COLORREF)dwNumPalEntries)
	// else if (nBitCount==24) 
	//		then call CreateRGBPalette();
	static CPalette* InitPalette(WORD nBitCount, DWORD dwNumPalEntries=0, AImageType imgType=imgClrType);
	//	If pPalette is null, call InitPalette(8,10,imgClrType)
	static CPalette* CopyPalette(const CPalette* pPalette);

	static int CreateMonoPalette(COLORREF rgb,PALETTEENTRY* pPalette, int palLength);
	static CPalette* CreateMonoPalette(COLORREF rgb=0);	//	rgb=0: Black color
	//	Default(pRgb=MULL): call InitPalette(8, wNumPalEntries, imgClrType)
	static CPalette* CreatePalette(WORD wNumPalEntries, const RGBQUAD* pRgb=NULL);
	static int CreateGrayPalette(COLORREF rgb, PALETTEENTRY* pPalette, int palLength);
	static CPalette* CreateGrayPalette(COLORREF rgb=0); //	rgb=0: Black color
	static CPalette* CreateRGBPalette();
	static int CreateRGBPalette(PALETTEENTRY* pPalette, int palLength);
	static COLORREF GetScreenColor(CWnd* pWnd, const LPPOINT pPoint); 

	static void TransformPalette(const RGBQUAD* pRGB, int nNumUsed,PALETTEENTRY* pPalette);
	static void TransformPalette(const PALETTEENTRY* pPalette, int nNumUsed, RGBQUAD* pRGB);

	static void HSLToRGB(const HSLVALUE& hsl, RGBVALUE& rgb);
	static void RGBToHSL(const RGBVALUE& rgb, HSLVALUE& hsl);
	static void HSLToCLR(const HSLVALUE& hsl, COLORREF& clr);
	static COLORREF HSLToCLR(double fHue, double fSat, double fLum);
	static void CLRToHSL(COLORREF clr, HSLVALUE& hsl);
	static void CLRToHSL(COLORREF clr, double* pHue, double* pSat, double* pLum);
	static void RGBToHSL(BYTE R, BYTE G, BYTE B, HSLVALUE& hsl);
	static void RGBToHSL(BYTE R, BYTE G, BYTE B, double* pHue, double* pSat, double* pLum);

	static AImageType JudgeImageType(CPalette* pPalette);
	static AImageType JudgeImageType(const PALETTEENTRY* pPalette, int palNumUsed);

public:

	static void RGBtoSCREEN(BYTE* pScreen, const BYTE* pRGB, BYTE nBitCount);  //RGB==>�Դ����ݸ�ʽ
	static void SCREENtoRGB(const BYTE* pScreen, BYTE* pRGB, BYTE nBitCount);  //�Դ����ݸ�ʽ==>RGB

public:
	//�����������͵���Ӧ����������
	//	inkOrder	:[in]ORDER_INKCOLOR
    //  bFullName   :[in]�Ƿ񷵻�ȫ��
	static CString ColorName(BYTE inkOrder, BOOL bFullName=FALSE);
	//�����������͵�Ĭ�ϵ�RGB��ɫ����
	//	inkOrder	:[in]ORDER_INKCOLOR
	//���أ�Ĭ��RGBֵ����������Чʱ����NULLCOLOR
	static COLORREF DefaultColorValue(BYTE inkOrder);

	//Ĭ�ϵ� RGB(0,255) <--> CMYK (0,255)ת����ʽ
	//rgb --> cmyk
	static void RGBToCMYK(BYTE r, BYTE g, BYTE b, BYTE &c, BYTE &m, BYTE &y, BYTE &k);
	static void RGBToCMYK(double r, double g, double b, double &c, double &m, double &y, double &k);
	static void RGBToCMYK(LPBYTE pCmyk, const BYTE *pBgr, int nCount);
	static COLORREF CMYKToRGB(double c, double m, double y, double k);
	static void CMYKToRGB(double &r, double &g, double &b, double c, double m, double y, double k);
	static void CMYKToRGB(LPBYTE pBgr, const BYTE *pCmyk, int nCount);

	//ת�� ky_printer_color ��ϵ� ORDER_INKCOLOR ����
	//	dwColor		:[in]ky_printer_color ���
	//	pInkOrder	:[out]ORDER_INKCOLOR ���飬ΪNULLʱ��������ʵ����Ҫ�Ļ�������С
	//	nBufferSize	:[in]pInkOrder��������С����С��ʵ��ֵʱ�������� 0xFF
	//���أ���Ч����ɫ����
	static BYTE TransColorToInkOrder(DWORD dwColor, BYTE *pInkOrder, int nBufferSize);
	//ת�� ky_printer_color����ɫ�� �� ORDER_INKCOLOR
	//	dwColor		:[in]ky_printer_color����ɫ��
	//���أ���Ч����ɫ��� ORDER_INKCOLOR
	static BYTE TransColorToInkOrder(DWORD dwColor);

	//ת�� ORDER_INKCOLOR ���鵽 ky_printer_color ���
	//	bInkCount	:[in]��ɫ�������<= 32
	//	pInkOrder	:[out]ORDER_INKCOLOR ���飬�� bInkCount ��Ӧ
	//���أ�ky_printer_color ���
	static DWORD TransInkOrderToColor(BYTE bInkCount, const BYTE *pInkOrder);
	//ת�� ORDER_INKCOLOR �� ky_printer_color ����ɫ��
	//	bInkOrder	:[in]ORDER_INKCOLOR
	//���أ���Ч����ɫ��� ky_printer_color����ɫ��
	static DWORD TransInkOrderToColor(BYTE bInkOrder);

	//��ͨ����ɫ --> RGB
	//�μ���KY_INKINFO, KY_RGBQUAD
	//ע��pInkOrder ��֧��������ɫ
	//	���뺬��orderink_base_c + orderink_base_m + orderink_base_y + orderink_base_k
	//	����������ϣ�
	//		orderink_base_r
	//		orderink_base_g
	//		orderink_base_b
	//		orderink_base_o
	//���أ�ת�����RGBֵ����������Чʱ����NULLCOLOR
	static COLORREF CMYK_N_To_RGB(BYTE bInkCount, const double *pInkValue, const BYTE *pInkOrder);
	//��ͨ����ɫ --> RGB
	//	bGray			:[in]�Ƿ�Ϊ�Ҷȸ�/ɫ���壬��ΪTRUEʱ����nClrUsed����ȡ��һ��ͨ��ɫ��Ϣ
	static BOOL CMYK_N_To_RGB(RGBQUAD* pRgb, const KY_EXPANDPALETTE* pExpandPalette, int nClrUsed, BOOL bGray);
	//RGB --> ��ͨ����ɫ(CMYK)
	static BOOL RGB_To_CMYK_N(KY_EXPANDPALETTE& expandPalette, const PALETTEENTRY* pPal, int nClrUsed);
	//��ͨ����ɫ-�Ҷ�����
	//ע�����ݵ�һ��ͨ��ɫ��Ϣ���Զ����������������ɫֵ
	//���أ���������ɫ���������������Ӧ��Ϊ256
	static int CMYK_N_CorrectGrayPalette(KY_EXPANDPALETTE& expandPalette);

    //��ǰ��ͨ����ɫ�����Ƿ���Ч
	static BOOL CMYK_IsValid(BYTE bInkCount, const BYTE *pInkOrder);
    static BOOL CMYK_IsValid(const KY_EXPANDPALETTE* pExpandPalette);

public:
	// Lab <--> RGB
	// Lab ������Χ�μ� LABVALUE ˵��
	static void LabToRGB(BYTE &R, BYTE &G, BYTE &B, BYTE l, BYTE a, BYTE b);
	static void LabToRGB(BYTE &R, BYTE &G, BYTE &B, LABVALUE labValue);
	static COLORREF LabToRGB(LABVALUE labValue);
	static COLORREF LabToRGB(BYTE l, BYTE a, BYTE b);
	static void LabToRGB(LPBYTE pBgr, const BYTE *pLab, int nCount);
	static void RGBToLab(BYTE &l, BYTE &a, BYTE &b, BYTE R, BYTE G, BYTE B);
	static void RGBToLab(BYTE &l, BYTE &a, BYTE &b, COLORREF clr);
	static LABVALUE RGBToLab(BYTE R, BYTE G, BYTE B);
	static LABVALUE RGBToLab(COLORREF clr);
	static void RGBToLab(LPBYTE pLab, const BYTE *pBgr, int nCount);

	//�������
	//	r, g, b : [0,255.0]
	//	l : [0, 100.0]
	//	a : [-128.0, 127.0]
	//	b : [-128.0, 127.0]
	//	nWhiteType	: WhitePointType
	enum WhitePointType
	{
		white_point_D50 = 0,
		white_point_D55,
		white_point_D65,
		white_point_D75,

		white_point_count
	};
	static void ColorSpace_LabToRGB(double &r, double &g, double &b, double Labl, double Laba, double Labb, int nWhiteType);
	static void ColorSpace_RGBToLab(double &Labl, double &Laba, double &Labb, double r, double g, double b, int nWhiteType);
	//����ɫ��ɫ��
	static double CalcDeltaE(double l1, double a1, double b1, double l2, double a2, double b2);

public:
	//���� INKMODE ����Ӧ����ɫģʽ��ʶ��
	//	nInkMode	:[in]INKMODE
	static BOOL GetInkModeName(CString &strInkMode, int nInkMode);
	//���� INKCLRINDEX ����Ӧ��īˮɫ��ʶ����ɫ��
	//	nInkclrIndex	:[in]INKCLRINDEX
	static BOOL GetInkNameFromInkclrIndex(CString &strInkName, int nInkclrIndex);

	//��ȱʡīˮ����
	//	inkInfo			:[out]īˮ���������� nInkMode �õ�
	//	nInkMode	:[in]INKMODE
	//���أ���ɫͨ������
	static int GetInkInfoFromInkMode(KY_INKINFO &inkInfo, int nInkMode);
	//�ж�����īˮ�����Ƿ���ͬ ����˳��
	static bool IsInkInfoEqual(BYTE bInkCount1, const BYTE *bInkOrder1, BYTE bInkCount2, const BYTE *bInkOrder2);
};

//���½�ʵ�� RGB <-->CMYK �Ļ�ת�����̣�����BYTE��
class RP_EXT_CLASS ARGBToCMYK
{
public:
	ARGBToCMYK();
	~ARGBToCMYK();

	//ת����ͼ�������� RGB<-->CMYK
	enum ICC_INTENT
	{
		icc_intent_perceptual	= 0,		//�ɸ�֪��perceptual rendering intent��
		icc_intent_relative		= 1,		//���ɫ�� (media-relative colorimetric rendering intent)
		icc_intent_saturation,				//���Ͷ� (saturation rendering intent) 
		icc_intent_absolute,				//����ɫ�� (ICC-absolute colorimetric rendering intent )

		icc_intent_count
	};
	static BOOL InitRGBtoCMYK(ARGBToCMYK &rgb2cmyk, 
							const char *pszInProfile,		// RGB icc file
							const char *pszOutProfile,		// CMYK icc file
							int nIntent);
	static BOOL IsRGBProfile(const char *pszProfile);
	static BOOL IsCMYKProfile(const char *pszProfile);
public:
	//��ʼ��������
	BOOL Init();

	//��ʼ�� RGB <-> CMYK ת��
	enum ERROR_NO
	{
		error_succeed		= 0,	//�ɹ�
		error_format_input	= 1,	//�����ļ���RGB���޷���ȡ���ʽ����
		error_format_output	= 2,	//����ļ���CMYK���޷���ȡ���ʽ����
		error_dll			= 3,	//ת�������
		error_other					//��������
	};
	//	pszInputRGB		:[in]RGBɫ��У���ļ����� sRGB Color Space Profile.icm
	//	pszOutputCMYK	:[in]CMYKɫ��У���ļ����� Photoshop5DefaultCMYK.icc
	//	nIntent			:[in]��֪��ͼ�������� CMYK ת��
	//���أ�ERROR_NO
	int Begin(const char *pszInputRGB, const char *pszOutputCMYK, int nIntent);

	// RGB <----> CMYK
	//CMYKת���Ƿ���Ч
	BOOL IsValidCMYK() const;
	// RGB --> CMYK
	BOOL TransRGBToCMYK(LPBYTE pCMYK, const BYTE *pBGR, int nPixels) const;
	// CMYK --> RGB
	BOOL TransCMYKToRGB(LPBYTE pBGR, const BYTE *pCMYK, int nPixels) const;

	// RGB <----> Lab
	// Lab ������Χ�μ� LABVALUE ˵��
	//Labת���Ƿ���Ч
	BOOL IsValidLab() const;
	// RGB --> Lab
	BOOL TransRGBToLab(LPBYTE pLab, const BYTE *pBGR, int nPixels) const;
	// Lab --> RGB
	BOOL TransLabToRGB(LPBYTE pBGR, const BYTE *pLab, int nPixels) const;

	//��������ת�����̣��ͷ��뱾��ת����ص���Դ
	BOOL End();

	//���������࣬�ͷ����������ص���Դ
	BOOL Exit();

private:
	void *m_pData;			//ת������ص����ݽṹ
};
#define DeltaE(lab1,lab2)	(AClrObj::CalcDeltaE((lab1.l), (lab1.a), (lab1.b), (lab2.l), (lab2.a), (lab2.b)))
#define EQULAB(lab1,lab2)	(EQUDOUBLE(lab1.l,lab2.l) && EQUDOUBLE(lab1.a,lab2.a) && EQUDOUBLE(lab1.b,lab2.b))
