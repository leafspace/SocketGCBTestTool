///////////////////////////////////////////////////////////////////////////
// ColorInfo.h	: 定义与墨水（颜料）相关的数据结构
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


// 混色操作宏（仅支持RGB格式混合）
// 请注意：
//	pBits, pSrc		: 不宜为表达式，pBits[0], pBits[1], pBits[2] 分别对应于 B， G， R
//	pBgr			: BGRVALUE*
//	MIX_XXXXXXX_0	: 表示为无透明度的操作
//	MIX_XXXXXXX_1	: 表示为有透明度的操作
//	MIX_XXXXIDX_X	: 表示针对索引稿操作
//	MIX_XXXXX24_X	: 表示针对真彩稿（24Bits）操作
//	opacPnt			: 不透明度(0-255)
//	wOpacity		: 透明度(255-opacPnt)
//
// ------------------- AnSeries 兼容模式 --------------------------
//
//普通 （仅支持RGB格式混合）
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
//颜料混合（仅支持RGB格式混合）
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
//光混合（仅支持RGB格式混合）
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
//SetBits 中“普通”模式操作（仅支持RGB格式混合）
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

// ------------------- 油墨密度模式 （HH， PS）--------------------------
//
// 油墨混合（仅支持RGB格式混合）
//	bDst		:[in/out]目标数据索引（pal index）
//	bSrc		:[in]当前数据索引（pal index）
//	opacPnt		:[in]当前层不透明度
//	bSrcLevel	:[in]当前色的油墨量（配合 opacPnt使用），对真彩稿操作时一般默认为 g_Flags.nDefaultOpacity[dyemix_way_opacity]
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
{					// 参数值		实际 L*ab
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


//预定义颜料类型
enum ky_printer_color
{
	kypc_nocolor		= 0,				//初始化状态（无效颜料类型）
	kypc_c				= 0x00000001,		// C，	青（Cyan）
	kypc_m				= 0x00000002,		// M，	品（Magenta），又名洋红
	kypc_y				= 0x00000004,		// Y，	黄（Yellow）
	kypc_k				= 0x00000008,		// K，	黑（blacK）
	
	kypc_lc				= 0x00000010,		// lc， 浅青（Light Cyan），	lc = C * 1/n 
	kypc_lm				= 0x00000020,		// lm， 浅品（Light Magenta），	lm = M * 1/n
	kypc_ly				= 0x00000040,		// ly， 浅黄（Ligth Yellow），	ly = Y * 1/n
	kypc_lk				= 0x00000080,		// lk， 浅黑（Light Black），又名深灰（dark gray），lk = K * 1/n
	
	kypc_red			= 0x00010000,		// R，	红（Red），又名大红，	R = M + Y
	kypc_green			= 0x00020000,		// Gr，	绿（Green），			G = C + Y
	kypc_blue			= 0x00040000,		// B，	蓝（Blue），又名宝蓝，	B = C + M
	kypc_orange			= 0x00080000,		// Or，	橙（Orange），又名桔色，Or= Y + R
	
	//kypc_white			= 0x01000000,		// W，	白（White），W = C + M + Y
	
	kypc_spotcolor		= 0x80000000,		// Spot Color，专色（不参与混色）
	
	//以下预定义部分颜料类型掩码
	kypc_lightcolor_mask= 0x00000FF0,		//浅色颜料掩码
	kypc_basecolor_mask = 0x0000FFFF,		//基色颜料掩码
	
	//以下预定义部分颜色组合模式
	kypc_cmyk			= kypc_c | kypc_m | kypc_y | kypc_k,	//CMYK
	kypc_cmyklclm		= kypc_cmyk | kypc_lc| kypc_lm,			//CMYKLcLm
	kypc_cmykog			= kypc_cmyk | kypc_orange | kypc_green,	//CMYKOG
	kypc_cmykrb			= kypc_cmyk | kypc_red | kypc_blue,		//CMYKRB
	kypc_cmykob			= kypc_cmyk | kypc_orange | kypc_blue,	//CMYKOB
	
	kypc_unknown		= 0xFFFFFFFF		//未知色
};

//扩展调色板
#define KY_MAX_CHANNELS				8					//系统最大支持的颜色通道数

//目标墨水参数
//注：主要为编码实现过程中固定缓冲区序列而设定
//	为保证可能的文件存取的兼容性，原则上不允许修改 orderink_xxxx_start 的值
#define ORDERINK_BASECOLOR_MAX		20					//最大支持的基色数
enum ORDER_INKCOLOR
{
	//------------ base color ---------------
	orderink_base_start = 0,							//纯色墨水-起始
	orderink_base_c = orderink_base_start + 0,			//	kypc_c
	orderink_base_m = orderink_base_start + 1,			//	kypc_m
	orderink_base_y,									//	kypc_y
	orderink_base_k,									//	kypc_k
	orderink_base_r,									//	kypc_red
	orderink_base_b,									//	kypc_blue
	orderink_base_g,									//	kypc_green
	orderink_base_o,									//	kypc_orange

	orderink_base_end,									//纯色墨水-结束（不包含）
	orderink_base_count = orderink_base_end - orderink_base_start,	//纯色墨水总数，<=ORDERINK_BASECOLOR_MAX

	//------------ ligth color ---------------
	orderink_light_start = ORDERINK_BASECOLOR_MAX,						//浅色墨水-起始
	orderink_light_lc	= orderink_light_start + inkIndex_c,			//	kypc_lc
	orderink_light_lm	= orderink_light_start + inkIndex_m,			//	kypc_lm
	orderink_light_ly	= orderink_light_start + inkIndex_y,			//	kypc_ly
	orderink_light_lk	= orderink_light_start + inkIndex_k,			//	kypc_lk

	//orderink_light_llk,									//	kypc_llk

	orderink_light_end,										//浅色墨水-结束（不包含）
	orderink_light_count = orderink_light_end - orderink_light_start,	//浅色墨水总数，<=ORDERINK_BASECOLOR_MAX

	//------------ spot color ---------------
	orderink_spot_start = 200,											//专色墨水-起始
	orderink_spot_0 = orderink_spot_start + 0,							//	kypc_spotcolor
	orderink_spot_1 = orderink_spot_start + 1,
	
	//orderink_spot_n = orderink_spot_start + n,

	orderink_spot_count = ORDERINK_BASECOLOR_MAX,						//专色墨水总数
	
	orderink_spot_end	= orderink_spot_start + orderink_spot_count,	//专色墨水-结束（不包含）

	orderink_inkcount	= 0xFE,											//最大墨水类型数

	orderink_unknown	= 0xFF
};
#define orderink_base_id(i)			(orderink_base_start+(i))
#define orderink_base_index(id)		((id)-orderink_base_start)
//是否为基色
#define is_orderink_base(id)		((orderink_base_start<=(id)) && ((id)<orderink_base_end))

#define orderink_light_id(i)		(orderink_light_start+(i))
#define orderink_light_index(id)	((id)-orderink_light_start)
//是否为浅色
#define is_orderink_light(id)		((orderink_light_start<=(id)) && ((id)<orderink_light_end))

#define orderink_spot_id(i)			(orderink_spot_start+(i))
#define orderink_spot_index(id)		((id)-orderink_spot_start)
//是否为专色
#define is_orderink_spot(id)		((orderink_spot_start<=(id)) && ((id)<orderink_spot_end))

//颜色模式
//注：当有新的颜色模式时仅能添加到最后（INKMODE_MAX 前）
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
	INK_MONO			= 12,	//单色，单色以“专色”标识

	INK_CMYKOB			= 13,
	INK_CMYKLcLmLkB		= 14,
	INK_CMYKLcLmOB		= 15,
	INK_CMYKRBOG		= 16,

	INKMODE_MAX,
};

struct KY_INKVALUE
{
	// 实际多通道颜色信息
	double	dInkValue[KY_MAX_CHANNELS];	//	每个通道的墨水值 (0,255)，与 KY_INKINFO 配合使用
};

//墨水信息
struct KY_INKINFO
{
	BYTE	bInkCount;									// <= KY_MAX_CHANNELS
	BYTE	bInkOrder[KY_MAX_CHANNELS];				//墨水次序, ORDER_INKCOLOR
};

//扩展调色板信息
struct RP_EXT_CLASS KY_EXPANDPALETTE
{
	KY_INKINFO			inkInfo;			//定义扩展调色板信息，inkInfo.bInkCount==0 时表示无扩展调色板
	KY_INKVALUE			inkValue[256];		//实际个数与 pPalette 的颜色数一致

public:
	BOOL LoadFile(CFile* pFile);
	
	//存档时所需空间
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

	static void RGBtoSCREEN(BYTE* pScreen, const BYTE* pRGB, BYTE nBitCount);  //RGB==>显存数据格式
	static void SCREENtoRGB(const BYTE* pScreen, BYTE* pRGB, BYTE nBitCount);  //显存数据格式==>RGB

public:
	//根据颜料类型得相应的颜料名称
	//	inkOrder	:[in]ORDER_INKCOLOR
    //  bFullName   :[in]是否返回全称
	static CString ColorName(BYTE inkOrder, BOOL bFullName=FALSE);
	//根据颜料类型得默认的RGB颜色参数
	//	inkOrder	:[in]ORDER_INKCOLOR
	//返回：默认RGB值，当参数无效时返回NULLCOLOR
	static COLORREF DefaultColorValue(BYTE inkOrder);

	//默认的 RGB(0,255) <--> CMYK (0,255)转换公式
	//rgb --> cmyk
	static void RGBToCMYK(BYTE r, BYTE g, BYTE b, BYTE &c, BYTE &m, BYTE &y, BYTE &k);
	static void RGBToCMYK(double r, double g, double b, double &c, double &m, double &y, double &k);
	static void RGBToCMYK(LPBYTE pCmyk, const BYTE *pBgr, int nCount);
	static COLORREF CMYKToRGB(double c, double m, double y, double k);
	static void CMYKToRGB(double &r, double &g, double &b, double c, double m, double y, double k);
	static void CMYKToRGB(LPBYTE pBgr, const BYTE *pCmyk, int nCount);

	//转换 ky_printer_color 组合到 ORDER_INKCOLOR 数组
	//	dwColor		:[in]ky_printer_color 组合
	//	pInkOrder	:[out]ORDER_INKCOLOR 数组，为NULL时函数返回实际需要的缓冲区大小
	//	nBufferSize	:[in]pInkOrder缓冲区大小，当小于实际值时函数返回 0xFF
	//返回：有效的颜色个数
	static BYTE TransColorToInkOrder(DWORD dwColor, BYTE *pInkOrder, int nBufferSize);
	//转换 ky_printer_color（单色） 到 ORDER_INKCOLOR
	//	dwColor		:[in]ky_printer_color（单色）
	//返回：有效的颜色类别 ORDER_INKCOLOR
	static BYTE TransColorToInkOrder(DWORD dwColor);

	//转换 ORDER_INKCOLOR 数组到 ky_printer_color 组合
	//	bInkCount	:[in]颜色类别数，<= 32
	//	pInkOrder	:[out]ORDER_INKCOLOR 数组，与 bInkCount 对应
	//返回：ky_printer_color 组合
	static DWORD TransInkOrderToColor(BYTE bInkCount, const BYTE *pInkOrder);
	//转换 ORDER_INKCOLOR 到 ky_printer_color （单色）
	//	bInkOrder	:[in]ORDER_INKCOLOR
	//返回：有效的颜色类别 ky_printer_color（单色）
	static DWORD TransInkOrderToColor(BYTE bInkOrder);

	//多通道颜色 --> RGB
	//参见：KY_INKINFO, KY_RGBQUAD
	//注：pInkOrder 仅支持以下颜色
	//	必须含：orderink_base_c + orderink_base_m + orderink_base_y + orderink_base_k
	//	以下任意组合：
	//		orderink_base_r
	//		orderink_base_g
	//		orderink_base_b
	//		orderink_base_o
	//返回：转换后的RGB值，当参数无效时返回NULLCOLOR
	static COLORREF CMYK_N_To_RGB(BYTE bInkCount, const double *pInkValue, const BYTE *pInkOrder);
	//多通道颜色 --> RGB
	//	bGray			:[in]是否为灰度稿/色调稿，当为TRUE时忽略nClrUsed，仅取第一个通道色信息
	static BOOL CMYK_N_To_RGB(RGBQUAD* pRgb, const KY_EXPANDPALETTE* pExpandPalette, int nClrUsed, BOOL bGray);
	//RGB --> 多通道颜色(CMYK)
	static BOOL RGB_To_CMYK_N(KY_EXPANDPALETTE& expandPalette, const PALETTEENTRY* pPal, int nClrUsed);
	//多通道颜色-灰度修正
	//注：根据第一个通道色信息，自动修正后面的所有颜色值
	//返回：修正后颜色个数，正常情况下应该为256
	static int CMYK_N_CorrectGrayPalette(KY_EXPANDPALETTE& expandPalette);

    //当前多通道颜色参数是否有效
	static BOOL CMYK_IsValid(BYTE bInkCount, const BYTE *pInkOrder);
    static BOOL CMYK_IsValid(const KY_EXPANDPALETTE* pExpandPalette);

public:
	// Lab <--> RGB
	// Lab 参数范围参见 LABVALUE 说明
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

	//浮点计算
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
	//得两色间色差
	static double CalcDeltaE(double l1, double a1, double b1, double l2, double a2, double b2);

public:
	//根据 INKMODE 得相应的颜色模式标识串
	//	nInkMode	:[in]INKMODE
	static BOOL GetInkModeName(CString &strInkMode, int nInkMode);
	//根据 INKCLRINDEX 得相应的墨水色标识（基色）
	//	nInkclrIndex	:[in]INKCLRINDEX
	static BOOL GetInkNameFromInkclrIndex(CString &strInkName, int nInkclrIndex);

	//得缺省墨水序列
	//	inkInfo			:[out]墨水索引，根据 nInkMode 得到
	//	nInkMode	:[in]INKMODE
	//返回：单色通道总数
	static int GetInkInfoFromInkMode(KY_INKINFO &inkInfo, int nInkMode);
	//判断两个墨水序列是否相同 （除顺序）
	static bool IsInkInfoEqual(BYTE bInkCount1, const BYTE *bInkOrder1, BYTE bInkCount2, const BYTE *bInkOrder2);
};

//以下仅实现 RGB <-->CMYK 的互转换过程（基于BYTE）
class RP_EXT_CLASS ARGBToCMYK
{
public:
	ARGBToCMYK();
	~ARGBToCMYK();

	//转换意图，仅用于 RGB<-->CMYK
	enum ICC_INTENT
	{
		icc_intent_perceptual	= 0,		//可感知（perceptual rendering intent）
		icc_intent_relative		= 1,		//相对色度 (media-relative colorimetric rendering intent)
		icc_intent_saturation,				//饱和度 (saturation rendering intent) 
		icc_intent_absolute,				//绝对色度 (ICC-absolute colorimetric rendering intent )

		icc_intent_count
	};
	static BOOL InitRGBtoCMYK(ARGBToCMYK &rgb2cmyk, 
							const char *pszInProfile,		// RGB icc file
							const char *pszOutProfile,		// CMYK icc file
							int nIntent);
	static BOOL IsRGBProfile(const char *pszProfile);
	static BOOL IsCMYKProfile(const char *pszProfile);
public:
	//初始化操作类
	BOOL Init();

	//初始化 RGB <-> CMYK 转换
	enum ERROR_NO
	{
		error_succeed		= 0,	//成功
		error_format_input	= 1,	//输入文件（RGB）无法读取或格式不对
		error_format_output	= 2,	//输出文件（CMYK）无法读取或格式不对
		error_dll			= 3,	//转换库出错
		error_other					//其它错误
	};
	//	pszInputRGB		:[in]RGB色彩校正文件，如 sRGB Color Space Profile.icm
	//	pszOutputCMYK	:[in]CMYK色彩校正文件，如 Photoshop5DefaultCMYK.icc
	//	nIntent			:[in]感知意图，仅用于 CMYK 转换
	//返回：ERROR_NO
	int Begin(const char *pszInputRGB, const char *pszOutputCMYK, int nIntent);

	// RGB <----> CMYK
	//CMYK转换是否有效
	BOOL IsValidCMYK() const;
	// RGB --> CMYK
	BOOL TransRGBToCMYK(LPBYTE pCMYK, const BYTE *pBGR, int nPixels) const;
	// CMYK --> RGB
	BOOL TransCMYKToRGB(LPBYTE pBGR, const BYTE *pCMYK, int nPixels) const;

	// RGB <----> Lab
	// Lab 参数范围参见 LABVALUE 说明
	//Lab转换是否有效
	BOOL IsValidLab() const;
	// RGB --> Lab
	BOOL TransRGBToLab(LPBYTE pLab, const BYTE *pBGR, int nPixels) const;
	// Lab --> RGB
	BOOL TransLabToRGB(LPBYTE pBGR, const BYTE *pLab, int nPixels) const;

	//结束本次转换过程，释放与本次转换相关的资源
	BOOL End();

	//结束操作类，释放与操作类相关的资源
	BOOL Exit();

private:
	void *m_pData;			//转换类相关的数据结构
};
#define DeltaE(lab1,lab2)	(AClrObj::CalcDeltaE((lab1.l), (lab1.a), (lab1.b), (lab2.l), (lab2.a), (lab2.b)))
#define EQULAB(lab1,lab2)	(EQUDOUBLE(lab1.l,lab2.l) && EQUDOUBLE(lab1.a,lab2.a) && EQUDOUBLE(lab1.b,lab2.b))
