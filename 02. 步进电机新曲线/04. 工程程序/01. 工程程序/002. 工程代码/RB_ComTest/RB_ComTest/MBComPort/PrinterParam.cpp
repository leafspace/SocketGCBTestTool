///////////////////////////////////////////////////////////////////////////
// PrinterParam.cpp : APrnBase class implementation file (PRINTER param)
//
// Created by hzm 2010-05-07
//
///////////////////////////////////////////////////////////////////////////
#include "../stdafx.h"
#include "../Include/rpPrnData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TRACELESSPRINT_DEFHEIGHT		2.0									//无痕打印处理-最小单元默认高度，mm
#define TRACELESSPRINT_DEFWIDTH			(TRACELESSPRINT_DEFHEIGHT * 4)		//无痕打印处理-最小单元默认宽度，mm

//bool APrnBase::Config_Load(PRINTER* pPrinterInfo, const char *pszConfigFile)
//{
//	bool bResult = false;
//    CString strPathName;
//	PRINTER		oldInfo;
//
//	if (pszConfigFile==NULL || pszConfigFile[0]=='\0')
//		return false;
//	if (pPrinterInfo==NULL)
//		return false;
//
//	oldInfo = *pPrinterInfo;
//
//	pPrinterInfo->Initialize();
//    if (ADocObj::GetDirectory(strPathName, pszConfigFile))
//    {
//        ADocObj::CreateDirectory(strPathName);
//    }
//
//    CFileFind find;
//    if (find.FindFile(pszConfigFile))
//        bResult = pPrinterInfo->GetFromProfile(pszConfigFile);
//    else
//    {
//        bResult = false;
//    }
//	if (!bResult)
//	{//出错时恢复原来参数值
//		*pPrinterInfo = oldInfo;
//	}
//		
//	return bResult;
//}
//bool APrnBase::Config_Save(const PRINTER* pPrinterInfo, const char *pszConfigFile)
//{
//    bool    bResult = false;
//    CString strPathName;
//
//	if (pszConfigFile==NULL || pszConfigFile[0]=='\0')
//		return false;
//	if (pPrinterInfo==NULL)
//		return false;
//
//    if (ADocObj::GetDirectory(strPathName, pszConfigFile))
//    {
//        ADocObj::CreateDirectory(strPathName);
//    }
//    bResult = pPrinterInfo->SetToProfile(pszConfigFile);
//
//	return bResult;
//}

//初始化
void PRINTHEAD::Initialize()
{
	BZERO(*this);

	nType = printhead_Unknown;

	nNozzleSize = 256;
	dNozzleDistance = 0.254;

	bRowCount = 1;

	BZERO(dDistance_Hor);

	nColorLevel = 1;
	nNozzleDropSizeMin = 30;

}

//读入参数
bool PRINTHEAD::GetFromProfile(const char *pszSection, const char *pszProfile)
{
	CString strSection, strIniFile;
	TCHAR szReg[100];

	Initialize();

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	GET_PROFILE_INFO_INT(nType, printhead_none, nType, printhead_Unknown, WORD);

	GET_PROFILE_INFO_INT(nNozzleSize, 1, nNozzleSize, 100000, WORD);
	GET_PROFILE_INFO_DOUBLE(dNozzleDistance, 0.0001, dNozzleDistance, 1000);

	GET_PROFILE_INFO_INT(bRowCount, 1, bRowCount, 0xFF, BYTE);
	if (bRowCount > _PRINTHEAD_ROW_MAX_)
		bRowCount = _PRINTHEAD_ROW_MAX_;
	if (bRowCount > 1)
	{
		GET_PROFILE_INFO_ARRAY_DOUBLE(dDistance_Hor, bRowCount, -1000.0, 0.0, 1000.0);
	}

	GET_PROFILE_INFO_INT(nColorLevel, 0, nColorLevel, 256, UINT);
	GET_PROFILE_INFO_INT(nNozzleDropSizeMin, 1, nNozzleDropSizeMin, 1000, UINT);

	return IsValid();
}
//写入参数
bool PRINTHEAD::SetToProfile(const char *pszSection, const char *pszProfile) const
{
	CString strSection, strIniFile;
	CString strReg;
	int		bCount;

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	WRITE_PROFILE_INFO("%d", nType);

	WRITE_PROFILE_INFO("%d", nNozzleSize);
	WRITE_PROFILE_INFO("%g", dNozzleDistance);

	WRITE_PROFILE_INFO("%d", bRowCount);
	bCount = bRowCount;
	if (bCount > _PRINTHEAD_ROW_MAX_)
		bCount = _PRINTHEAD_ROW_MAX_;
	if (bCount > 1)
	{
		WRITE_PROFILE_INFO_ARRAY("%f", dDistance_Hor, bCount);
	}

	WRITE_PROFILE_INFO("%d", nColorLevel);
	WRITE_PROFILE_INFO("%d", nNozzleDropSizeMin);

	return true;
}
//参数是否有效
bool PRINTHEAD::IsValid() const
{
	if (nType <= printhead_none || nType >= printhead_Unknown)
		return false;

	if (nNozzleSize <= 0)
		return false;
	if (dNozzleDistance <= DOUBLEZERO)
		return false;

	if (bRowCount > _PRINTHEAD_ROW_MAX_)
		return false;
	if (bRowCount > 1)
	{
		int		i, j;
		BOOL	bInitZero = FALSE;
		for (i = 0; i < (bRowCount - 1); i++)
		{
			for (j = i + 1; j < bRowCount; j++)
			{
				if (EQUDOUBLE(dDistance_Hor[i], dDistance_Hor[j]))
					return false;	//有至少2行间距相等
			}
		}
		for (i = 0; i < bRowCount; i++)
		{
			if (EQUZERO(dDistance_Hor[i]))
			{
				bInitZero = TRUE;
				break;
			}
		}
		if (!bInitZero)
			return false;		//无起始喷嘴定义
	}

	if (nColorLevel < 0 || nColorLevel > 256)
		return false;

	return true;
}

//初始化
void PRINTHEADGROUP::Initialize()
{
	printhead.Initialize();

	nHeadNumber = 8;

	nGroupCount = 1;

	bCombineWay = combineway_normal;

	BZERO(nFilterIndex);

	BZERO(nColorTable);

	BZERO(nDataFilter);

}
//喷头组归类信息是否有效
//  pFilterIndex:[in]喷头组归类信息，同 nFilterIndex 定义
//  nHead		:[in]总的喷头数
//注：pFilterIndex 中的每个值不能相同
bool PRINTHEADGROUP::IsValidFilter(const UINT *pFilterIndex, int nHead)
{
	int		i;
	int		m;
	UINT	nValue;

	if (pFilterIndex == NULL)
		return false;
	for (i = 0; i < nHead; i++)
	{
		nValue = pFilterIndex[i];
		for (m = i + 1; m < nHead; m++)
		{
			if (nValue == pFilterIndex[m])
			{//存在相同值
				return false;
			}
		}
	}
	return true;
}
//每组喷头的颜料信息是否有效
//  pColorTable :[in]喷头颜料信息
//  nColor      :[in]颜色数
//注：喷头颜料信息值不能相同
bool PRINTHEADGROUP::IsValidColorTable(const DWORD *pColorTable, int nColor)
{
	int i, j;
	DWORD nValue;

	if (pColorTable == NULL)
		return false;

	for (i = 0; i < nColor; i++)
	{
		nValue = pColorTable[i];
		switch (nValue)
		{
		case kypc_spotcolor:
		case kypc_nocolor:
			continue;
			break;
		case kypc_unknown:
			return false;
		}
		for (j = i + 1; j < nColor; j++)
		{
			if (nValue == pColorTable[j])
				return false;
		}
	}
	return true;
}
//喷头对应喷头板通道信息是否有效
//	pDataFilter	:[in]喷头对应的喷头板通道信息，同 nDataFilter 定义
//	nChannels	:[in]有效通道总数
//注：通道序号值不能相同
bool PRINTHEADGROUP::IsValidDataFilter(const UINT *pDataFilter, int nChannels)
{
	int		i, j;
	UINT	nValue;

	if (pDataFilter == NULL)
		return false;

	for (i = 0; i < nChannels; i++)
	{
		nValue = pDataFilter[i];
		if (nValue >= (UINT)nChannels)
			return false;
		for (j = i + 1; j < nChannels; j++)
		{
			if (nValue == pDataFilter[j])
				return false;
		}
	}
	return true;
}
//得喷嘴叠加个数
//返回值：
//	>= 0 	:喷嘴叠加个数
//	< 0 	:无效组合方式
int PRINTHEADGROUP::GetOverlappingNozzles(BYTE bCombineWay)
{
	int nOverlappingNozzles = -1;
	switch (bCombineWay)
	{
	default:
		ASSERT(0);
	case PRINTHEADGROUP::combineway_normal:
		nOverlappingNozzles = 0;
		break;
		//串行连接，不影响喷头间距
	case PRINTHEADGROUP::combineway_series_connection_normal:
		nOverlappingNozzles = 0;
		break;
	case PRINTHEADGROUP::combineway_series_connection_overlapping_1:
		nOverlappingNozzles = 1;
		break;
	case PRINTHEADGROUP::combineway_series_connection_overlapping_2:
		nOverlappingNozzles = 2;
		break;
		//并行连接，影响喷头间距
	case PRINTHEADGROUP::combineway_parallel_connection_normal:
		nOverlappingNozzles = -1;
		//dNozzleDistance /= m_info.nGroupCount;
		ASSERT(0);
		break;
	}
	return nOverlappingNozzles;
}

//得实际有效的喷嘴数
//	nNozzles	:[in]单个喷头喷嘴数
//	nGroupCount	:[in]喷头组数（同色）
//	bCombineWay	:[in]喷头拼接方式
int PRINTHEADGROUP::GetTotalNozzles(int nNozzles, int nGroupCount, BYTE bCombineWay)
{
	int nOverlappingNozzles, nTotalNozzles = 0;

	ASSERT(nGroupCount >= 1);
	ASSERT(nNozzles >= 1);

	nOverlappingNozzles = PRINTHEADGROUP::GetOverlappingNozzles(bCombineWay);
	if (nOverlappingNozzles < 0)
		return 0;		//invalid combine way

	nTotalNozzles = nGroupCount*(nNozzles - nOverlappingNozzles) + nOverlappingNozzles;

	return nTotalNozzles;
}


//喷头组参数是否有效
bool PRINTHEADGROUP::IsValid() const
{
	if (!printhead.IsValid())
		return false;

	if (nHeadNumber <= 0 || nHeadNumber > KY_MAX_COLORS)
		return false;

	if (nGroupCount <= 0 || (nHeadNumber % nGroupCount) != 0)
		return false;

	int nColor;

	nColor = (nHeadNumber / nGroupCount);
	if (!PRINTHEADGROUP::IsValidFilter(nFilterIndex, nHeadNumber))
		return false;
	if (!PRINTHEADGROUP::IsValidColorTable(nColorTable, nColor))
		return false;
	if (!PRINTHEADGROUP::IsValidDataFilter(nDataFilter, nColor))
		return false;

	return true;
}
//读入参数
bool PRINTHEADGROUP::GetFromProfile(const char *pszSection, const char *pszProfile)
{
	int nColor;
	CString strSectionTmp;
	CString strSection, strIniFile;
	UINT	nFilter[KY_MAX_CHANNELS][KY_MAX_GROUP];
	int		i, j;

	Initialize();

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	strSectionTmp = strSection;
	strSection = strSectionTmp + "_PRINTHEAD";
	if (!printhead.GetFromProfile(strSection, strIniFile))
		return false;

	strSection = strSectionTmp;

	GET_PROFILE_INFO_INT(nHeadNumber, 1, nHeadNumber, KY_MAX_COLORS, UINT);
	GET_PROFILE_INFO_INT(nGroupCount, 1, nGroupCount, KY_MAX_GROUP, UINT);
	GET_PROFILE_INFO_INT(bCombineWay, 0, bCombineWay, 0xFF, BYTE);

	ASSERT((nHeadNumber % nGroupCount) == 0);
	ASSERT(nHeadNumber >= nGroupCount);
	if (nHeadNumber < nGroupCount)
		nGroupCount = 1;
	if ((nHeadNumber % nGroupCount) != 0)
		nGroupCount = 1;
	nColor = nHeadNumber / nGroupCount;

	strSection = strSectionTmp + "_FILTER";
	BZERO(nFilter);
	GET_PROFILE_INFO_ARRAYEX_UINT(nFilter, nColor, nGroupCount, 0, 0, KY_MAX_COLORS, UINT);
	for (i = 0; i < nColor; i++)
	{
		for (j = 0; j < (int)nGroupCount; j++)
		{
			nFilterIndex[TransToFilterIndex(j, i, nColor)] = nFilter[i][j];
		}
	}
	strSection = strSectionTmp + "_COLORTABLE";
	GET_PROFILE_INFO_ARRAY_UINT(nColorTable, nColor, 0x00000000, 0x00000000, 0xFFFFFFFF);
	strSection = strSectionTmp + "_DATAFILTER";
	BZERO(nDataFilter);
	GET_PROFILE_INFO_ARRAY_UINT(nDataFilter, nColor, 0, 0, 0xFFFFFFFF);

	return IsValid();
}
//写入参数
bool PRINTHEADGROUP::SetToProfile(const char *pszSection, const char *pszProfile) const
{
	int nColor;
	CString strSectionTmp;
	CString strSection, strIniFile;
	CString strReg;
	UINT	nFilter[KY_MAX_CHANNELS][KY_MAX_GROUP];
	int		i, j;

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	strSectionTmp = strSection;

	strSection = strSectionTmp;
	WRITE_PROFILE_INFO("%u", nHeadNumber);
	WRITE_PROFILE_INFO("%u", nGroupCount);
	WRITE_PROFILE_INFO("%d", bCombineWay);

	nColor = nHeadNumber / nGroupCount;

	strSection = strSectionTmp + "_PRINTHEAD";
	if (!printhead.SetToProfile(strSection, strIniFile))
		return false;

	strSection = strSectionTmp + "_FILTER";
	BZERO(nFilter);
	for (i = 0; i < nColor; i++)
	{
		for (j = 0; j < (int)nGroupCount; j++)
		{
			nFilter[i][j] = nFilterIndex[TransToFilterIndex(j, i, nColor)];
		}
	}
	WRITE_PROFILE_INFO_ARRAYEX("%u", nFilter, nColor, nGroupCount);
	strSection = strSectionTmp + "_COLORTABLE";
	WRITE_PROFILE_INFO_ARRAY("%u", nColorTable, nColor);
	strSection = strSectionTmp + "_DATAFILTER";
	WRITE_PROFILE_INFO_ARRAY("%u", nDataFilter, nColor);

	return true;
}

//初始化
void SPRAYPARAM::Initialize()
{
	bSprayForIdleEanble = FALSE;
	bSprayForPrintingEanble = FALSE;

	wSprayFrequencyForIdel = 0;
	wSprayPassForPrint = 0;
}

//读入参数
bool SPRAYPARAM::GetFromProfile(const char *pszSection, const char *pszProfile)
{
	CString strSection, strIniFile;

	Initialize();

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	GET_PROFILE_INFO_INT(bSprayForIdleEanble, 0, bSprayForIdleEanble, 1, BYTE);
	GET_PROFILE_INFO_INT(bSprayForPrintingEanble, 0, bSprayForPrintingEanble, 1, BYTE);

	GET_PROFILE_INFO_INT(wSprayFrequencyForIdel, 0, wSprayFrequencyForIdel, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(wSprayPassForPrint, 0, wSprayPassForPrint, 0xFFFF, WORD);

	return IsValid();
}
//写入参数
bool SPRAYPARAM::SetToProfile(const char *pszSection, const char *pszProfile) const
{
	CString strSection, strIniFile;
	CString strReg;

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	WRITE_PROFILE_INFO("%d", bSprayForIdleEanble);
	WRITE_PROFILE_INFO("%d", bSprayForPrintingEanble);

	WRITE_PROFILE_INFO("%d", wSprayFrequencyForIdel);
	WRITE_PROFILE_INFO("%d", wSprayPassForPrint);
	return true;
}
//参数是否有效
bool SPRAYPARAM::IsValid() const
{
	return true;
}

//初始化
void CLEANHEADPARAM::Initialize()
{
	bEnable = FALSE;
	wPass = 0;
	wPurgeTime = 0;
	wPurgeDelay = 0;
	bCleanTime = 1;
}

//读入参数
bool CLEANHEADPARAM::GetFromProfile(const char *pszSection, const char *pszProfile)
{
	CString strSection, strIniFile;

	Initialize();

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	GET_PROFILE_INFO_INT(bEnable, 0, bEnable, 1, BYTE);
	GET_PROFILE_INFO_INT(wPass, 0, wPass, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(wPurgeTime, 0, wPurgeTime, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(wPurgeDelay, 0, wPurgeDelay, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(bCleanTime, 0, bCleanTime, 0xFF, BYTE);

	return IsValid();
}
//写入参数
bool CLEANHEADPARAM::SetToProfile(const char *pszSection, const char *pszProfile) const
{
	CString strSection, strIniFile;
	CString strReg;

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	WRITE_PROFILE_INFO("%d", bEnable);
	WRITE_PROFILE_INFO("%d", wPass);
	WRITE_PROFILE_INFO("%d", wPurgeTime);
	WRITE_PROFILE_INFO("%d", wPurgeDelay);
	WRITE_PROFILE_INFO("%d", bCleanTime);

	return true;
}
//参数是否有效
bool CLEANHEADPARAM::IsValid() const
{
	return true;
}


//初始化
void MOTOPARAM_GENERAL::Initialize()
{
	X_MAX_WIDTH = 3500;				//x方向最大打印宽度(mm)
	PRINT_MAX_FREQ = 40000;			//打印最大频率(Hz)
	X_MAX_SPEED = 700;				//x方向最大速度(mm/s)
	X_COEF = 1.0f;				//x方向速度较正系数
}

//读入参数
bool MOTOPARAM_GENERAL::GetFromProfile(const char *pszSection, const char *pszProfile)
{
	CString strSection, strIniFile;
	TCHAR	szReg[200];

	Initialize();

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	GET_PROFILE_INFO_INT(X_MAX_WIDTH, 100, X_MAX_WIDTH, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(PRINT_MAX_FREQ, 100, PRINT_MAX_FREQ, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(X_MAX_SPEED, 10, X_MAX_SPEED, 0xFFFF, WORD);
	GET_PROFILE_INFO_FLOAT(X_COEF, 0, X_COEF, 1000000.0f);

	return IsValid();
}
//写入参数
bool MOTOPARAM_GENERAL::SetToProfile(const char *pszSection, const char *pszProfile) const
{
	CString strSection, strIniFile;
	CString strReg;

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	WRITE_PROFILE_INFO("%d", X_MAX_WIDTH);
	WRITE_PROFILE_INFO("%d", PRINT_MAX_FREQ);
	WRITE_PROFILE_INFO("%d", X_MAX_SPEED);
	WRITE_PROFILE_INFO("%f", X_COEF);

	return true;
}
//参数是否有效
bool MOTOPARAM_GENERAL::IsValid() const
{
	if (X_MAX_WIDTH == 0)
		return false;
	if (PRINT_MAX_FREQ == 0)
		return false;
	if (X_MAX_SPEED == 0)
		return false;
	if (X_COEF < 0.0000001)
		return false;

	return true;
}

//初始化
void MOTOPARAM_X::Initialize()
{
	SERVE_PER_PULSE = 10000;				//电子齿轮比（若加减速机，则为经减速机后的比值）
	X_P_DIS = 70;					//x方向打印时走加速距离(mm)
	X_S_DIS = 70;					//x方向打印时走减速距离(mm)
	IO_SPACE = 50;					//光栅IO间距(mm) IO_SPACE
	IO_PULSE = 12500;				//光栅IO间脉冲数 ，IO_PULSE
	X_DELAY_F = 15;					//小车快速移动延时时间（us），值越大，小车移动越慢
	X_DELAY_S = 30;					//小车慢速移动延时时间（us）,值越大，小车移动越慢
	SERVE_PER_CYCLE = 100.0f;				//服电机转一周小车走的距离（mm）
}

//读入参数
bool MOTOPARAM_X::GetFromProfile(const char *pszSection, const char *pszProfile)
{
	CString strSection, strIniFile;
	TCHAR	szReg[200];

	Initialize();

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	GET_PROFILE_INFO_INT(SERVE_PER_PULSE, 1, SERVE_PER_PULSE, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(X_P_DIS, 1, X_P_DIS, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(X_S_DIS, 1, X_S_DIS, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(IO_SPACE, 1, IO_SPACE, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(IO_PULSE, 1, IO_PULSE, 0xFFFF, WORD);
	GET_PROFILE_INFO_UINT(X_DELAY_F, 0, X_DELAY_F, 0x7FFFFFFF);
	GET_PROFILE_INFO_UINT(X_DELAY_S, X_DELAY_F, X_DELAY_S, 0x7FFFFFFF);
	GET_PROFILE_INFO_FLOAT(SERVE_PER_CYCLE, 0.000001f, SERVE_PER_CYCLE, 1000000.0f);

	return IsValid();
}

//写入参数
bool MOTOPARAM_X::SetToProfile(const char *pszSection, const char *pszProfile) const
{
	CString strSection, strIniFile;
	CString strReg;

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	WRITE_PROFILE_INFO("%d", SERVE_PER_PULSE);
	WRITE_PROFILE_INFO("%d", X_P_DIS);
	WRITE_PROFILE_INFO("%d", X_S_DIS);
	WRITE_PROFILE_INFO("%d", IO_SPACE);
	WRITE_PROFILE_INFO("%d", IO_PULSE);
	WRITE_PROFILE_INFO("%u", X_DELAY_F);
	WRITE_PROFILE_INFO("%u", X_DELAY_S);
	WRITE_PROFILE_INFO("%f", SERVE_PER_CYCLE);

	return true;
}
//参数是否有效
bool MOTOPARAM_X::IsValid() const
{
	if (SERVE_PER_PULSE == 0)
		return false;
	if (X_P_DIS == 0)
		return false;
	if (X_S_DIS == 0)
		return false;
	if (IO_SPACE == 0 || IO_PULSE == 0)
		return false;
	if (X_DELAY_S == 0 || X_DELAY_F == 0)
		return false;
	if (SERVE_PER_CYCLE < 0.000001)
		return false;

	return true;
}

//初始化
void MOTOPARAM_Y::Initialize()
{
	Y_PER_PULSE = 1000;					//Y向电子齿轮比（若加减速机，则为经减速机后的比值）
	Y_MAX_SPEED = 500;					//Y方向最大速度（mm/s）
	Y_DELAY = 15;					//打印时匀速延时时间（us）
	Y_DELAY_F = 50;					//Y向快速移动延时时间(us) 
	Y_DELAY_S = 20;					//Y向慢速移动延时时间(us) 
	Y_WHORL = 10.0f;				//Y方向螺矩(mm) 
}

//读入参数
bool MOTOPARAM_Y::GetFromProfile(const char *pszSection, const char *pszProfile)
{
	CString strSection, strIniFile;
	TCHAR	szReg[200];

	Initialize();

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	GET_PROFILE_INFO_INT(Y_PER_PULSE, 1, Y_PER_PULSE, 0xFFFF, WORD);
	GET_PROFILE_INFO_INT(Y_MAX_SPEED, 1, Y_MAX_SPEED, 0xFFFF, WORD);
	GET_PROFILE_INFO_UINT(Y_DELAY, 0, Y_DELAY, 0x7FFFFFFF);
	GET_PROFILE_INFO_UINT(Y_DELAY_F, 0, Y_DELAY_F, 0x7FFFFFFF);
	GET_PROFILE_INFO_UINT(Y_DELAY_S, Y_DELAY_F, Y_DELAY_S, 0x7FFFFFFF);
	GET_PROFILE_INFO_FLOAT(Y_WHORL, 0.000001f, Y_WHORL, 1000000.0f);

	return IsValid();
}
//写入参数
bool MOTOPARAM_Y::SetToProfile(const char *pszSection, const char *pszProfile) const
{
	CString strSection, strIniFile;
	CString strReg;

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	WRITE_PROFILE_INFO("%d", Y_PER_PULSE);
	WRITE_PROFILE_INFO("%d", Y_MAX_SPEED);
	WRITE_PROFILE_INFO("%u", Y_DELAY);
	WRITE_PROFILE_INFO("%u", Y_DELAY_F);
	WRITE_PROFILE_INFO("%u", Y_DELAY_S);
	WRITE_PROFILE_INFO("%f", Y_WHORL);

	return true;
}
//参数是否有效
bool MOTOPARAM_Y::IsValid() const
{
	if (Y_PER_PULSE == 0)
		return false;
	if (Y_MAX_SPEED == 0)
		return false;

	return true;
}

//初始化
void MOTOPARAM_Z::Initialize()
{
	Z_DELAY = 400;					//Z方向移动延时（us）
	Z_WHORL = 10.0f;				//Z方向螺矩(mm) 
	Z_POSITION_MIN = 1.0f;					//Z方向最小距离(pulse)  
	Z_POSITION_MAX = 666.6f;				//Z方向最大距离(pulse) 
}

//读入参数
bool MOTOPARAM_Z::GetFromProfile(const char *pszSection, const char *pszProfile)
{
	CString strSection, strIniFile;
	TCHAR	szReg[200];

	Initialize();

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	GET_PROFILE_INFO_UINT(Z_DELAY, 0, Z_DELAY, 0x7FFFFFFF);
	GET_PROFILE_INFO_FLOAT(Z_WHORL, 0.000001f, Z_WHORL, 1000000.0f);
	GET_PROFILE_INFO_FLOAT(Z_POSITION_MAX, 0.0f, Z_POSITION_MAX, 1000000.0f);
	GET_PROFILE_INFO_FLOAT(Z_POSITION_MIN, 0.0f, Z_POSITION_MIN, Z_POSITION_MAX);

	return IsValid();
}
//写入参数
bool MOTOPARAM_Z::SetToProfile(const char *pszSection, const char *pszProfile) const
{
	CString strSection, strIniFile;
	CString strReg;

	strSection = pszSection;
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	WRITE_PROFILE_INFO("%u", Z_DELAY);
	WRITE_PROFILE_INFO("%f", Z_WHORL);
	WRITE_PROFILE_INFO("%f", Z_POSITION_MAX);
	WRITE_PROFILE_INFO("%f", Z_POSITION_MIN);

	return true;
}
//参数是否有效
bool MOTOPARAM_Z::IsValid() const
{
	if (Z_WHORL <= 0.0000001)
		return false;

	if (Z_POSITION_MIN <= 0.0000001 || Z_POSITION_MIN > 1000.0f)
		return false;
	if (Z_POSITION_MAX <= 0.0000001 || Z_POSITION_MAX > 1000.0f)
		return false;

	return true;
}


//初始化
void PRINTER::Initialize()
{
	memset(this, 0, sizeof(*this));

	nType = printer_none;
	strcpy_s(szName, _T("printer_none"));

	dMinStartX = 0.0;
	dMinStartY = 0.0;

	dMaxWidth = 0.0;
	dMaxHeight = 0.0;

	printgroup.Initialize();

	BZERO(nInkModeLevel);
	nInkModeCount = 0;
	nInkModeType = 0;

	BZERO(szSpeedLevel);
	BZERO(bSpeedValue);

	BZERO(nXResLevel);

	BZERO(nYPassLevel);
	BZERO(bYPassWay);

	bSprayEnable = FALSE;
	bCleanHeadEnable = FALSE;

	nSpeedType = 0;
	nXResType = 0;
	nYPassType = 0;

	BZERO(nDistance_Hor);
	BZERO(nDistance_Hor_Reversed);
	BZERO(dDistance_Ver);
	BZERO(nAdjust_bi_directional_compensation);
	BZERO(dAdjust_step_compensation);

	BZERO(fade_nLevel);

	bTracelessPrintWay = tracelessprint_way_none;		//无痕打印处理-效果定义
	dTracelessPrintDefWidth = TRACELESSPRINT_DEFWIDTH;		//无痕打印处理-最小单元默认宽度，mm
	dTracelessPrintDefHeight = TRACELESSPRINT_DEFHEIGHT;		//无痕打印处理-最小单元默认高度，mm

	nPrintColorLevel = 1;							//实际打印使用的灰度级别，<= PRINTHEAD::nColorLevel


	sprayparam.Initialize();
	cleanheadparam.Initialize();

	bPrintway = PRINTER::printway_unidirection;

	bJumpWay = PRINTER::jumpway_all;		//快进（跳白）处理方式
	dJumpLimit_x = 30;						//X向最小跳白宽度，mm
	dJumpLimit_y = 100000;						//Y向最大跳白长度，mm

	bPrintToFile = FALSE;				//输出到PRN文件，为FALSE时直接输出到打印机

	bVirtualPrinter = FALSE;				//是否为虚拟打印机（不连接实际硬件设备）

	bFlatMachine = FALSE;				//设备类型，默认为导带机型

	bOriginPosition_posXEnable = FALSE;			//是否支持X向相对定位操作，IO定位控制模式暂不支持，需设置为FALSE

	bOriginPosition_start = origin_pos_zero_xy;	//打印起始前小车位置
	bOriginPosition_end = origin_pos_x_zero;	//打印结束时小车位置

	bOriginPosition_start = Check_OriginPosition_Start(bOriginPosition_start, bFlatMachine, bOriginPosition_posXEnable);
	bOriginPosition_end = Check_OriginPosition_End(bOriginPosition_end, bFlatMachine, bOriginPosition_posXEnable);

	bOriginPosition_prompt = TRUE;					//打印起始位置不是“打印原点”时提示

	bThicknessEnable = FALSE;				//是否可以自定义布厚参数

	nPriority = THREAD_PRIORITY_NORMAL;	//打印处理线程优先级，默认为 THREAD_PRIORITY_NORMAL

}

//读入参数
bool PRINTER::GetFromProfile(const char *pszProfile)
{
	int     i, j, k;
	CString strSectionTmp, strEntry;
	CString strSection, strIniFile, strReg;
	CString strTmp;
	char	szReg[200], *pszTmp;
	UINT	nTmp;
	int		nColor;
	int		nCount;

	Initialize();

	strSection = "Config";
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	GET_PROFILE_INFO_INT(nType, printer_none, nType, printer_Unknown, WORD);
	GET_PROFILE_INFO_STRINGEX(szName);

	GET_PROFILE_INFO_DOUBLE(dMinStartX, 0.0, dMinStartX, 2000.0);
	GET_PROFILE_INFO_DOUBLE(dMinStartY, 0.0, dMinStartY, 2000.0);

	GET_PROFILE_INFO_DOUBLE(dMaxWidth, 0.0, dMaxWidth, 100000.0);
	GET_PROFILE_INFO_DOUBLE(dMaxHeight, INT_MIN, dMaxHeight, INT_MAX);

	GET_PROFILE_INFO_INT(bFlatMachine, 0, bFlatMachine, 1, BOOL);
	if (bFlatMachine)
	{//平板机型，需限制 Y向高度
		if (dMaxHeight < _FLATMACHINE_HEIGHT_LIMIT_)
		{//修正默认的有效打印长度值
			dMaxHeight = _FLATMACHINE_HEIGHT_LIMIT_;
		}
	}
	else
	{//导带机型
		dMinStartY = 0;
	}

	strSectionTmp = strSection;
	strSection = "PRINTHEADGROUP";
	if (!printgroup.GetFromProfile(strSection, strIniFile))
		return false;

	nColor = printgroup.nHeadNumber / printgroup.nGroupCount;
	if (nColor <= 0)
		return false;

	strSection = "INKMODE";
	BZERO(nInkModeLevel);
	nCount = nInkModeCount;
	GET_PROFILE_INFO_INT(nCount, 0, nInkModeCount, INKMODE_MAX, int);
	for (i = 0; i < nCount; i++)
	{
		strEntry.Format("nInkModeLevel[%d]", i);
		nTmp = (UINT)GetPrivateProfileInt(strSection, strEntry, INK_NONE, strIniFile);
		if (nTmp <= INK_NONE || nTmp >= INKMODE_MAX)
			break;
		nInkModeLevel[i] = INKMODE(nTmp);
	}
	nInkModeCount = i;

	if (nInkModeCount <= 0)
		return false;
	if (nInkModeCount >= INKMODE_MAX)
		nInkModeCount = INKMODE_MAX;
	nInkModeLevel[nInkModeCount] = INK_NONE;

	strSection = "SPEEDLEVEL";
	BZERO(szSpeedLevel);
	BZERO(bSpeedValue);
	nCount = nSpeedCount;
	k = 1;		//级数值
	GET_PROFILE_INFO_INT(nCount, 0, nSpeedCount, _MAX_SPEED_LEVEL_, int);
	for (i = 0; i < nCount; i++)
	{
		//	szSpeedLevel[0] = fast speed, 1
		strEntry.Format("szSpeedLevel[%d]", i);
		if (!GetPrivateProfileString(strSection, strEntry, NULL, szReg, _countof(szReg), strIniFile))
			break;
		if (strlen(szReg) <= 0)
			break;
		pszTmp = strrchr(szReg, ',');
		if (pszTmp == NULL)
		{
			strcpy_s(szSpeedLevel[i], szReg);
			bSpeedValue[i] = k;
			k++;
		}
		else
		{
			*pszTmp++ = '\0';
			if (strlen(szReg) <= 0)
				break;
			strcpy_s(szSpeedLevel[i], szReg);
			nTmp = atoi(pszTmp);
			if (nTmp <= 0 || nTmp > _MAX_SPEED_LIMIT_)
				nTmp = k;
			k = nTmp;
			bSpeedValue[i] = k;
			k++;
		}
	}
	nSpeedCount = i;

	if (nSpeedCount <= 0)
		return false;
	if (nSpeedCount >= _MAX_SPEED_LEVEL_)
		nSpeedCount = _MAX_SPEED_LEVEL_;
	szSpeedLevel[nSpeedCount][0] = '\0';

	strSection = "XRESLEVEL";
	BZERO(nXResLevel);
	nCount = nXResCount;
	GET_PROFILE_INFO_INT(nCount, 0, nXResCount, _MAX_XRES_LEVEL_, int);
	for (i = 0; i < nCount; i++)
	{
		strEntry.Format("nXResLevel[%d]", i);
		nTmp = (UINT)GetPrivateProfileInt(strSection, strEntry, 0, strIniFile);
		if (nTmp <= 0)
			break;
		nXResLevel[i] = nTmp;
	}
	nXResCount = i;
	if (nXResCount <= 0)
		return false;
	if (nXResCount >= _MAX_XRES_LEVEL_)
		nXResCount = _MAX_XRES_LEVEL_;
	nXResLevel[nXResCount] = 0;

	strSection = "YPASSLEVEL";
	BZERO(nYPassLevel);
	BZERO(bYPassWay);
	nCount = nYPassCount;
	GET_PROFILE_INFO_INT(nCount, 0, nYPassCount, _MAX_YPASS_LEVEL_, int);
	for (i = 0; i < nCount; i++)
	{
		// nYPassLevel[0] = 1,0
		strEntry.Format("nYPassLevel[%d]", i);
		if (!GetPrivateProfileString(strSection, strEntry, NULL, szReg, _countof(szReg), strIniFile))
			break;
		if (strlen(szReg) <= 0)
			break;

		pszTmp = strrchr(szReg, ',');
		if (pszTmp == NULL)
		{
			nTmp = atoi(szReg);
			if (nTmp <= 0)
				break;
			nYPassLevel[i] = nTmp;
			bYPassWay[i] = passway_normal;
		}
		else
		{
			*pszTmp++ = '\0';
			nTmp = atoi(szReg);
			if (nTmp <= 0)
				break;
			nYPassLevel[i] = nTmp;
			nTmp = atoi(pszTmp);
			if (nTmp < passway_normal || nTmp >= passway_count)
				nTmp = passway_normal;
			bYPassWay[i] = (BYTE)nTmp;
		}
	}
	nYPassCount = i;
	if (nYPassCount <= 0)
		return false;
	if (nYPassCount >= _MAX_YPASS_LEVEL_)
		nYPassCount = _MAX_YPASS_LEVEL_;
	nYPassLevel[nYPassCount] = 0;

	//淡入淡出处理
	int		pass, nozzlesize, nfadeLimit;
	nozzlesize = PRINTHEADGROUP::GetTotalNozzles(printgroup.printhead.nNozzleSize, printgroup.nGroupCount, printgroup.bCombineWay);

	strSection = "YFADELEVEL";
	BZERO(fade_nLevel);

	BOOL fade_bValid;
	nTmp = GetPrivateProfileInt(strSection, "fade_bValid", -1, strIniFile);
	fade_bValid = (nTmp == 1) ? TRUE : FALSE;
	if (!fade_bValid)
	{// init
		BZERO(fade_nLevel);
		for (i = 0; i < nYPassCount; i++)
		{
			pass = nYPassLevel[i];
			ASSERT(pass > 0);
			if (bYPassWay[i] == PRINTER::passway_plus)
			{
				//淡入淡出处理-得最大参数值
				fade_nLevel[i] = PRINTER::fade_calclimit(nozzlesize, pass, false);
			}
		}
		fade_bValid = TRUE;
		WRITE_PROFILE_INFO("%d", fade_bValid);
	}
	else
	{// get from profile
		GET_PROFILE_INFO_ARRAY_INT(fade_nLevel, nYPassCount, -1, 0, 1000000, int);
	}
	for (i = 0; i < nYPassCount; i++)
	{
		pass = nYPassLevel[i];
		ASSERT(pass > 0);

		//淡入淡出处理-得最大参数值
		nfadeLimit = PRINTER::fade_calclimit(nozzlesize, pass, true);
		if (fade_nLevel[i] < 0)
			fade_nLevel[i] = 0;
		else if (fade_nLevel[i] > nfadeLimit)
		{
			fade_nLevel[i] = nfadeLimit;
		}
	}

	strSection = strSectionTmp;		//"Config"

	GET_PROFILE_INFO_INT(bSprayEnable, 0, bSprayEnable, 1, BYTE);
	GET_PROFILE_INFO_INT(bCleanHeadEnable, 0, bCleanHeadEnable, 1, BYTE);

	//无痕打印处理方式
	GET_PROFILE_INFO_INT(bTracelessPrintWay, tracelessprint_way_none, tracelessprint_way_none, tracelessprint_way_limit, BYTE);

	GET_PROFILE_INFO_DOUBLE(dTracelessPrintDefWidth, 0.5, TRACELESSPRINT_DEFWIDTH, 50.0);		//无痕打印处理-最小单元默认宽度，mm
	GET_PROFILE_INFO_DOUBLE(dTracelessPrintDefHeight, 0.5, TRACELESSPRINT_DEFHEIGHT, 20.0);		//无痕打印处理-最小单元默认高度，mm

	//实际打印时的灰度级别
	GET_PROFILE_INFO_INT(nPrintColorLevel, 0, nPrintColorLevel, 256, UINT);
	if (nPrintColorLevel > printgroup.printhead.nColorLevel)
		nPrintColorLevel = printgroup.printhead.nColorLevel;

	//对于同一款设备（确定机型），以下参数可通过应用程序进行更改（需配合机械）
	GET_PROFILE_INFO_INT(nSpeedType, 0, nSpeedType, nSpeedCount, UINT);
	GET_PROFILE_INFO_INT(nInkModeType, 0, nInkModeType, nInkModeCount, UINT);
	GET_PROFILE_INFO_INT(nXResType, 0, nXResType, nXResCount, UINT);
	GET_PROFILE_INFO_INT(nYPassType, 0, nYPassType, nYPassCount, UINT);

	//distance x / distance x reversed
	for (i = 0; i < nSpeedCount; i++)
	{
		for (j = 0; j < nXResCount; j++)
		{
			strSection.Format("ADJUST_DISTANCE_HOR_%d_%d", i, j);
			for (k = 0; k < (int)printgroup.nHeadNumber; k++)
			{
				strEntry.Format("%2d", k);
				nTmp = (UINT)GetPrivateProfileInt(strSection, strEntry, 0, strIniFile);
				nDistance_Hor[i][j][k] = nTmp;
			}
			strSection.Format("ADJUST_DISTANCE_HOR_REVERSED_%d_%d", i, j);
			for (k = 0; k < (int)printgroup.nHeadNumber; k++)
			{
				strEntry.Format("%2d", k);
				nTmp = (UINT)GetPrivateProfileInt(strSection, strEntry, 0, strIniFile);
				nDistance_Hor_Reversed[i][j][k] = nTmp;
			}
		}
	}
	strSection = "ADJUST_DISTANCE_VER";
	GET_PROFILE_INFO_ARRAY_DOUBLE(dDistance_Ver, (int)printgroup.nHeadNumber, INT_MIN, 0, INT_MAX);

	strSection = "ADJUST_BI";
	GET_PROFILE_INFO_ARRAYEX_INT(nAdjust_bi_directional_compensation, nSpeedCount, nXResCount, -1000000, 0, 1000000, int);
	strSection = "ADJUST_STEP";
	GET_PROFILE_INFO_ARRAY_DOUBLE(dAdjust_step_compensation, nYPassCount, -1000000, 0, 1000000);

	strSection = "SPRAYPARAM";
	if (!sprayparam.GetFromProfile(strSection, strIniFile))
		return false;

	strSection = "CLEANPARAM";
	if (!cleanheadparam.GetFromProfile(strSection, strIniFile))
		return false;

	strSection = strSectionTmp;		//"Config"
	GET_PROFILE_INFO_INT(bPrintway, printway_unidirection, bPrintway, (printway_count - 1), BYTE);

	GET_PROFILE_INFO_INT(bJumpWay, jumpway_normal, bJumpWay, jumpway_all, BYTE);
	GET_PROFILE_INFO_DOUBLE(dJumpLimit_x, 10, 30, 5000);
	GET_PROFILE_INFO_DOUBLE(dJumpLimit_y, 100, 2000, 1000000);

	GET_PROFILE_INFO_INT(bPrintToFile, 0, bPrintToFile, 1, BYTE);
	GET_PROFILE_INFO_INT(bVirtualPrinter, 0, bVirtualPrinter, 1, BYTE);

	GET_PROFILE_INFO_INT(bOriginPosition_posXEnable, 0, bOriginPosition_posXEnable, 1, BOOL);
	GET_PROFILE_INFO_INT(bOriginPosition_start, 0, bOriginPosition_start, 0xFF, BYTE);
	bOriginPosition_start = Check_OriginPosition_Start(bOriginPosition_start, bFlatMachine, bOriginPosition_posXEnable);

	GET_PROFILE_INFO_INT(bOriginPosition_end, 0, bOriginPosition_end, 0xFF, BYTE);
	bOriginPosition_end = Check_OriginPosition_End(bOriginPosition_end, bFlatMachine, bOriginPosition_posXEnable);

	GET_PROFILE_INFO_INT(bOriginPosition_prompt, 0, bOriginPosition_prompt, 1, BOOL);

	GET_PROFILE_INFO_INT(bThicknessEnable, 0, bThicknessEnable, 1, BOOL);

	GET_PROFILE_INFO_INT(nPriority, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_HIGHEST, int);

	return IsValid();
}
//写入参数
bool PRINTER::SetToProfile(const char *pszProfile) const
{
	CString strSectionTmp, strEntry;
	CString strSection, strIniFile;
	CString strTmp;
	CString strReg;
	int     i, j, k;
	int		nCount, nInkModeCountTmp, nSpeedCountTmp, nXResCountTmp;

	strSection = "Config";
	strIniFile = pszProfile;
	if (strSection.IsEmpty() || strIniFile.IsEmpty())
		return false;

	WRITE_PROFILE_INFO("%d", nType);
	WRITE_PROFILE_INFO("%s", szName);

	WRITE_PROFILE_INFO("%g", dMinStartX);
	WRITE_PROFILE_INFO("%g", dMinStartY);

	WRITE_PROFILE_INFO("%g", dMaxWidth);
	WRITE_PROFILE_INFO("%g", dMaxHeight);

	WRITE_PROFILE_INFO("%d", bFlatMachine);

	strSectionTmp = strSection;
	strSection = "PRINTHEADGROUP";
	if (!printgroup.SetToProfile(strSection, strIniFile))
		return false;

	strSection = "INKMODE";
	nCount = nInkModeCount;
	for (i = 0; i < nCount; i++)
	{
		if (nInkModeLevel[i] <= INK_NONE || nInkModeLevel[i] >= INKMODE_MAX)
		{
			nCount = i;
			break;
		}
	}
	nInkModeCountTmp = nCount;
	WRITE_PROFILE_INFO("%d", nCount);
	WRITE_PROFILE_INFO_ARRAY("%d", nInkModeLevel, nCount);

	strSection = "SPEEDLEVEL";
	nCount = nSpeedCount;
	for (i = 0; i < nCount; i++)
	{
		if (szSpeedLevel[i][0] == '\0')
		{
			nCount = i;
			break;
		}
	}
	nSpeedCountTmp = nCount;
	WRITE_PROFILE_INFO("%d", nCount);
	for (i = 0; i < nCount; i++)
	{
		//	szSpeedLevel[0] = fast speed, 1
		strEntry.Format("szSpeedLevel[%d]", i);
		strTmp.Format("%s,%d", szSpeedLevel[i], bSpeedValue[i]);
		WritePrivateProfileString(strSection, strEntry, strTmp, strIniFile);
	}

	strSection = "XRESLEVEL";
	nCount = nXResCount;
	for (i = 0; i < nCount; i++)
	{
		if (nXResLevel[i] <= 0)
		{
			nCount = i;
			break;
		}
	}
	nXResCountTmp = nCount;
	WRITE_PROFILE_INFO("%d", nCount);
	WRITE_PROFILE_INFO_ARRAY("%d", nXResLevel, nCount);

	strSection = "YPASSLEVEL";
	nCount = nYPassCount;
	for (i = 0; i < nCount; i++)
	{
		if (nYPassLevel[i] <= 0)
		{
			nCount = i;
			break;
		}
	}
	WRITE_PROFILE_INFO("%d", nCount);
	for (i = 0; i < nCount; i++)
	{
		// nYPassLevel[0] = 1,0
		strEntry.Format("nYPassLevel[%d]", i);
		strTmp.Format("%d,%d", nYPassLevel[i], bYPassWay[i]);
		WritePrivateProfileString(strSection, strEntry, strTmp, strIniFile);
	}

	strSection = "YFADELEVEL";
	WRITE_PROFILE_INFO_ARRAY("%d", fade_nLevel, nCount);

	strSection = strSectionTmp;
	WRITE_PROFILE_INFO("%d", bSprayEnable);
	WRITE_PROFILE_INFO("%d", bCleanHeadEnable);

	//无痕打印处理方式
	WRITE_PROFILE_INFO("%d", bTracelessPrintWay);
	WRITE_PROFILE_INFO("%f", dTracelessPrintDefWidth);
	WRITE_PROFILE_INFO("%f", dTracelessPrintDefHeight);

	//实际打印时的灰度级别
	WRITE_PROFILE_INFO("%d", nPrintColorLevel);

	//对于同一款设备（确定机型），以下参数可通过应用程序进行更改（需配合机械）
	WRITE_PROFILE_INFO("%d", nInkModeType);
	WRITE_PROFILE_INFO("%d", nSpeedType);
	WRITE_PROFILE_INFO("%d", nXResType);
	WRITE_PROFILE_INFO("%d", nYPassType);

	//distance x / distance x reversed
	for (i = 0; i < nSpeedCountTmp; i++)
	{
		for (j = 0; j < nXResCountTmp; j++)
		{
			strSection.Format("ADJUST_DISTANCE_HOR_%d_%d", i, j);
			for (k = 0; k < (int)printgroup.nHeadNumber; k++)
			{
				strEntry.Format("%2d", k);
				strTmp.Format("%d", nDistance_Hor[i][j][k]);
				WritePrivateProfileString(strSection, strEntry, strTmp, strIniFile);
			}
			strSection.Format("ADJUST_DISTANCE_HOR_REVERSED_%d_%d", i, j);
			for (k = 0; k < (int)printgroup.nHeadNumber; k++)
			{
				strEntry.Format("%2d", k);
				strTmp.Format("%d", nDistance_Hor_Reversed[i][j][k]);
				WritePrivateProfileString(strSection, strEntry, strTmp, strIniFile);
			}
		}
	}
	strSection = "ADJUST_DISTANCE_VER";
	WRITE_PROFILE_INFO_ARRAY("%g", dDistance_Ver, printgroup.nHeadNumber);

	strSection = "ADJUST_BI";
	WRITE_PROFILE_INFO_ARRAYEX("%d", nAdjust_bi_directional_compensation, nSpeedCount, nXResCount);
	strSection = "ADJUST_STEP";
	WRITE_PROFILE_INFO_ARRAY("%g", dAdjust_step_compensation, nYPassCount);

	strSection = "SPRAYPARAM";
	if (!sprayparam.SetToProfile(strSection, strIniFile))
		return false;

	strSection = "CLEANPARAM";
	if (!cleanheadparam.SetToProfile(strSection, strIniFile))
		return false;

	strSection = strSectionTmp;
	WRITE_PROFILE_INFO("%d", bPrintway);

	WRITE_PROFILE_INFO("%d", bJumpWay);
	WRITE_PROFILE_INFO("%f", dJumpLimit_x);
	WRITE_PROFILE_INFO("%f", dJumpLimit_y);

	WRITE_PROFILE_INFO("%d", bPrintToFile);

	WRITE_PROFILE_INFO("%d", bVirtualPrinter);

	WRITE_PROFILE_INFO("%d", bOriginPosition_posXEnable);

	WRITE_PROFILE_INFO("%d", bOriginPosition_start);
	WRITE_PROFILE_INFO("%d", bOriginPosition_end);

	WRITE_PROFILE_INFO("%d", bOriginPosition_prompt);

	WRITE_PROFILE_INFO("%d", bThicknessEnable);

	WRITE_PROFILE_INFO("%d", nPriority);
	return true;
}
//参数是否有效
bool PRINTER::IsValid() const
{
	int		i;
	BYTE	bOriginPosition;

	if (nType == printer_none || nType == printer_Unknown)
		return false;

	if (strlen(szName) <= 0)
		return false;

	if (dMinStartX < 0.0
		|| dMinStartY < 0.0)
	{
		return false;
	}

	if (dMaxWidth <= 1)
		return false;

	if (bFlatMachine)
	{//平板机型，需限制 Y向高度
		if (dMaxHeight < _FLATMACHINE_HEIGHT_LIMIT_)
			return false;
	}

	if (!printgroup.IsValid())
		return false;

	if (nPrintColorLevel < 0 || nPrintColorLevel > printgroup.printhead.nColorLevel)
		return false;

	if (nInkModeCount <= 0)
		return false;
	for (i = 0; i < nInkModeCount; i++)
	{
		if (nInkModeLevel[i] <= INK_NONE || nInkModeLevel[i] >= INKMODE_MAX)
			break;
	}
	if (i == 0)
		return false;

	if (nSpeedCount <= 0)
		return false;

	if (nXResCount <= 0)
		return false;
	ASSERT(nXResCount <= _MAX_XRES_LEVEL_);
	for (i = 0; i < nXResCount; i++)
	{
		if (nXResLevel[i] == 0)
			break;
	}
	if (i == 0)
		return false;

	if (nYPassCount <= 0)
		return false;
	ASSERT(nYPassCount <= _MAX_YPASS_LEVEL_);
	for (i = 0; i < (_MAX_YPASS_LEVEL_ + 1); i++)
	{
		if (nYPassLevel[i] == 0)
			break;
	}
	if (i == 0)
		return false;

	if (!sprayparam.IsValid())
		return false;

	if (!cleanheadparam.IsValid())
		return false;

	if (bPrintway < printway_unidirection || bPrintway >= printway_count)
		return false;

	if (bJumpWay < jumpway_normal || bJumpWay > jumpway_all)
		return false;

	bOriginPosition = Check_OriginPosition_Start(bOriginPosition_start, bFlatMachine, bOriginPosition_posXEnable);
	if ((bOriginPosition & origin_pos_mask_x) == PRINTER::origin_pos_unknown)
	{
		//		return false;
	}
	bOriginPosition = Check_OriginPosition_End(bOriginPosition_end, bFlatMachine, bOriginPosition_posXEnable);
	if ((bOriginPosition & origin_pos_mask_y) == PRINTER::origin_pos_unknown)
	{
		//		return false;
	}

	return true;
}

//修正打印起始前小车位置参数
//	IsPosXEnable	:[in]是否支持X向相对定位操作，IO定位控制模式暂不支持，需设置为FALSE
BYTE PRINTER::Check_OriginPosition_Start(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL IsPosXEnable)
{
	BYTE bMask;
	BYTE bResult = origin_pos_unknown;

	if (IsFlatMachine)
	{//平板机型
		bMask = bOriginPosition & origin_pos_mask_x;
		switch (bMask)
		{
		case PRINTER::origin_pos_x_zero:
			bResult |= bMask;
			break;
		case PRINTER::origin_pos_x_current:
			if (IsPosXEnable)
				bResult |= bMask;
			else
				bResult |= PRINTER::origin_pos_x_zero;
			break;
		case PRINTER::origin_pos_x_max:
		case PRINTER::origin_pos_unknown:
		default:
			bResult |= PRINTER::origin_pos_x_zero;
			break;
		}
		bMask = bOriginPosition & origin_pos_mask_y;
		switch (bMask)
		{
		case PRINTER::origin_pos_y_zero:
		case PRINTER::origin_pos_y_current:
			bResult |= bMask;
			break;
		case PRINTER::origin_pos_y_max:
		case PRINTER::origin_pos_unknown:
		default:
			bResult |= PRINTER::origin_pos_y_zero;
			break;
		}
	}
	else
	{//导带机型
		bMask = bOriginPosition & origin_pos_mask_x;
		switch (bMask)
		{
		case PRINTER::origin_pos_x_zero:
			bResult |= bMask;
			break;
		case PRINTER::origin_pos_x_current:
			if (IsPosXEnable)
				bResult |= bMask;
			else
				bResult |= PRINTER::origin_pos_x_zero;
			break;
		case PRINTER::origin_pos_x_max:
		case PRINTER::origin_pos_unknown:
		default:
			bResult |= PRINTER::origin_pos_x_zero;
			break;
		}
		//导带机无Y向定位要求
		bMask = bOriginPosition & origin_pos_mask_y;
		switch (bMask)
		{
		case PRINTER::origin_pos_y_zero:
		case PRINTER::origin_pos_y_current:
		case PRINTER::origin_pos_y_max:
		case PRINTER::origin_pos_unknown:
		default:
			bResult |= PRINTER::origin_pos_y_current;
			break;
		}
	}
	return bResult;
}
//修正打印结束时小车位置参数
BYTE PRINTER::Check_OriginPosition_End(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL IsPosXEnable)
{
	BYTE bMask;
	BYTE bResult = origin_pos_unknown;

	if (IsFlatMachine)
	{//平板机型
		bMask = bOriginPosition & origin_pos_mask_x;
		switch (bMask)
		{
		case PRINTER::origin_pos_x_zero:
		case PRINTER::origin_pos_x_max:
			bResult |= bMask;
			break;
		case PRINTER::origin_pos_x_current:
			if (IsPosXEnable)
				bResult |= bMask;
			else
				bResult |= PRINTER::origin_pos_x_max;
			break;
		case PRINTER::origin_pos_unknown:
		default:
			bResult |= PRINTER::origin_pos_x_max;
			break;
		}
		bMask = bOriginPosition & origin_pos_mask_y;
		switch (bMask)
		{
		case PRINTER::origin_pos_y_zero:
		case PRINTER::origin_pos_y_max:
		case PRINTER::origin_pos_y_current:
			bResult |= bMask;
			break;
		case PRINTER::origin_pos_unknown:
		default:
			bResult |= PRINTER::origin_pos_y_max;
			break;
		}
	}
	else
	{//导带机型
		bMask = bOriginPosition & origin_pos_mask_x;
		switch (bMask)
		{
		case PRINTER::origin_pos_x_zero:
		case PRINTER::origin_pos_x_max:
			bResult |= bMask;
			break;
		case PRINTER::origin_pos_x_current:
			if (IsPosXEnable)
				bResult |= bMask;
			else
				bResult |= PRINTER::origin_pos_x_max;
			break;
		case PRINTER::origin_pos_unknown:
		default:
			bResult |= PRINTER::origin_pos_x_max;
			break;
		}
		//导带机无Y向定位要求
		bMask = bOriginPosition & origin_pos_mask_y;
		switch (bMask)
		{
		case PRINTER::origin_pos_y_zero:
		case PRINTER::origin_pos_y_current:
		case PRINTER::origin_pos_y_max:
		case PRINTER::origin_pos_unknown:
		default:
			bResult |= PRINTER::origin_pos_y_current;
			break;
		}
	}
	return bResult;
}


//-------- 与打印原点定义相关的控制选项操作函数-----
//	IsFlatMachine	:[in]是否为平板机型，为FALSE时表示导带机型
//	bStart			:[in]是否打印起始前的定位状态，为FALSE时表示针对打印结束时定位状态操作
//	IsPosXEnable	:[in]是否支持X向相对定位操作，IO定位控制模式暂不支持，需设置为FALSE
void PRINTER::InitOriginPositionList(CComboBox *pList, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable)
{
	if (pList == NULL)
		return;

	pList->ResetContent();
	if (IsFlatMachine)
	{//平板机型
		if (bStart)
		{
			pList->AddString(_T("打印原点"));
			pList->AddString(_T("小车方向打印原点（丝杆不动）"));
			if (IsPosXEnable)
			{
				pList->AddString(_T("当前位置"));
				pList->AddString(_T("丝杆方向打印原点（小车不动）"));
			}
		}
		else
		{
			pList->AddString(_T("设备待机"));						//平板机待机位
			pList->AddString(_T("打印原点"));
			pList->AddString(_T("小车方向打印原点（丝杆不动）"));
			pList->AddString(_T("散喷位（丝杆不动）"));				//散喷位，也即导带机待机位
		}
	}
	else
	{//导带机型
		if (bStart)
		{
			pList->AddString(_T("打印原点"));
			if (IsPosXEnable)
			{
				pList->AddString(_T("当前位置"));
			}
		}
		else
		{
			pList->AddString(_T("设备待机"));						//导带机待机位，即散喷位
			pList->AddString(_T("打印原点"));
		}
	}
}

BYTE PRINTER::TransIndexToOriginPosition(int nIndex, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable)
{
	BYTE		bOriginPosition;

	bOriginPosition = PRINTER::origin_pos_unknown;
	if (IsFlatMachine)
	{//平板机型
		if (bStart)
		{
			switch (nIndex)
			{
			case 0:
				//打印原点
				bOriginPosition = PRINTER::origin_pos_zero_xy;
				break;
			case 1:
				//小车方向打印原点（丝杆不动）
				bOriginPosition = PRINTER::origin_pos_zero_x_current_y;
				break;
			case 2:
				//当前位置
				if (IsPosXEnable)
					bOriginPosition = PRINTER::origin_pos_current_xy;
				else
					bOriginPosition = PRINTER::origin_pos_zero_x_current_y;
				break;
			case 3:
				//丝杆方向打印原点（小车不动）
				if (IsPosXEnable)
					bOriginPosition = PRINTER::origin_pos_current_x_zero_y;
				else
					bOriginPosition = PRINTER::origin_pos_zero_xy;
				break;
			default:
				//默认状态：打印原点
				ASSERT(0);
				bOriginPosition = PRINTER::origin_pos_zero_xy;
				break;
			}
		}
		else
		{
			switch (nIndex)
			{
			case 0:
				//平板机待机位
				bOriginPosition = PRINTER::origin_pos_max_xy;
				break;
			case 1:
				//打印原点
				bOriginPosition = PRINTER::origin_pos_zero_xy;
				break;
			case 2:
				//小车方向打印原点（丝杆不动）
				bOriginPosition = PRINTER::origin_pos_zero_x_current_y;
				break;
			case 3:
				//散喷位（丝杆不动），也即导带机待机位
				bOriginPosition = PRINTER::origin_pos_max_x_current_y;
				break;
			default:
				//默认状态：平板机待机位
				ASSERT(0);
				bOriginPosition = PRINTER::origin_pos_max_xy;
				break;
			}
		}
	}
	else
	{//导带机型
		if (bStart)
		{
			switch (nIndex)
			{
			case 0:
				//打印原点
				bOriginPosition = PRINTER::origin_pos_zero_x_current_y;
				break;
			case 1:
				//当前位置
				if (IsPosXEnable)
					bOriginPosition = PRINTER::origin_pos_current_xy;
				else
					bOriginPosition = PRINTER::origin_pos_zero_x_current_y;
				break;
			default:
				//默认状态：打印原点
				ASSERT(0);
				bOriginPosition = PRINTER::origin_pos_zero_x_current_y;
				break;
			}
		}
		else
		{
			switch (nIndex)
			{
			case 0:
				//导带机待机位，即散喷位
				bOriginPosition = PRINTER::origin_pos_max_x_current_y;
				break;
			case 1:
				//打印原点
				bOriginPosition = PRINTER::origin_pos_zero_x_current_y;
				break;
			default:
				//默认状态：导带机待机位，即散喷位
				ASSERT(0);
				bOriginPosition = PRINTER::origin_pos_max_x_current_y;
				break;
			}
		}
	}
	ASSERT(bOriginPosition != PRINTER::origin_pos_unknown);

	return bOriginPosition;
}
int PRINTER::TransOriginPositionToIndex(BYTE bOriginPosition, BOOL IsFlatMachine, BOOL bStart, BOOL IsPosXEnable)
{
	int			nIndex;

	nIndex = 0;		//当前位置
	if (bStart)
		bOriginPosition = PRINTER::Check_OriginPosition_Start(bOriginPosition, IsFlatMachine, IsPosXEnable);
	else
		bOriginPosition = PRINTER::Check_OriginPosition_End(bOriginPosition, IsFlatMachine, IsPosXEnable);

	if (IsFlatMachine)
	{//平板机型
		if (bStart)
		{
			switch (bOriginPosition)
			{
			case PRINTER::origin_pos_zero_xy:
				//打印原点
				nIndex = 0;
				break;
			case PRINTER::origin_pos_zero_x_current_y:
				//小车方向打印原点（丝杆不动）
				nIndex = 1;
				break;
			case PRINTER::origin_pos_current_xy:
				//当前位置
				if (IsPosXEnable)
					nIndex = 2;
				else
					nIndex = 1;
				break;
			case PRINTER::origin_pos_current_x_zero_y:
				//丝杆方向打印原点（小车不动）
				if (IsPosXEnable)
					nIndex = 3;
				else
					nIndex = 0;
				break;
			default:
				//默认状态：打印原点
				ASSERT(0);
				nIndex = 0;
				break;
			}
		}
		else
		{
			switch (bOriginPosition)
			{
			case PRINTER::origin_pos_max_xy:
				//默认状态：平板机待机位
				nIndex = 0;
				break;
			case PRINTER::origin_pos_zero_xy:
				//打印原点
				nIndex = 1;
				break;
			case PRINTER::origin_pos_zero_x_current_y:
				//小车方向打印原点（丝杆不动）
				nIndex = 2;
				break;
			case PRINTER::origin_pos_max_x_current_y:
				//散喷位，也即导带机待机位
				nIndex = 3;
				break;
			default:
				//默认状态：散喷位，也即导带机待机位
				ASSERT(0);
				nIndex = 2;
				break;
			}
		}
	}
	else
	{//导带机型
		if (bStart)
		{
			switch (bOriginPosition)
			{
			case PRINTER::origin_pos_zero_x_current_y:
				//打印原点
				nIndex = 0;
				break;
			case PRINTER::origin_pos_current_xy:
				//当前位置
				if (IsPosXEnable)
					nIndex = 1;
				else
					nIndex = 0;
				break;
			default:
				//默认状态：打印原点
				ASSERT(0);
				nIndex = 0;
				break;
			}
		}
		else
		{
			switch (bOriginPosition)
			{
			case PRINTER::origin_pos_max_x_current_y:
				//导带机待机位，即散喷位
				nIndex = 0;
				break;
			case PRINTER::origin_pos_zero_x_current_y:
				//打印原点
				nIndex = 1;
				break;
			default:
				//默认状态：导带机待机位，即散喷位
				ASSERT(0);
				nIndex = 0;
				break;
			}
		}
	}

	return nIndex;
}

//计算Pass步进参数
//	pLeapNum	:[out]每Pass步进值（Pixel）
//	nozzlesize	:[in]喷嘴数
//	pass		:[in]PASS数
//返回：
//	最大的步进值
int PRINTER::step_calcleapnum(DWORD *pLeapNum, int nozzlesize, int pass, int nFadeLevel)
{
	ASSERT(pLeapNum != NULL);
	ASSERT(nozzlesize >= 2);
	ASSERT(pass >= 1);
	if (pLeapNum == NULL || nozzlesize < 2 || pass < 1)
		return 0;
	if (nFadeLevel <= 0)
		nFadeLevel = nozzlesize;

	int		i, residual;

	if (pass <= 1)
	{// 1 Pass
		pLeapNum[0] = nozzlesize;
		return pLeapNum[0];
	}
	int s, supp;
	s = (nozzlesize - nFadeLevel * pass) / (pass - 1);
	//初始化 Pass 步进距离，Pixel
	pLeapNum[0] = (nozzlesize - 1) / pass * pass + 1;
	ASSERT(pLeapNum[0] <= (DWORD)nozzlesize);
	if (nFadeLevel < nozzlesize / pass)
	{
		for (i = 1; i < pass; i++)
		{
			pLeapNum[i] = pLeapNum[0] /*- nFadeLevel/pass*pass*pass*/;
			supp = s + nFadeLevel - pLeapNum[i] / pass;
			pLeapNum[i] += supp*pass;
		}

		residual = nozzlesize - nFadeLevel - pLeapNum[1] / pass*(pass - 1);
		if (residual > 0)
		{//每Pass步进距离补偿（不同）
			for (i = (pass - 1); i >= 0; i--)
			{
				pLeapNum[i] += pass;
				residual--;
				if (residual == 0)
					break;
			}
		}
		//pLeapNum[0] -= nFadeLevel/pass*pass*pass;
		supp = s + nFadeLevel - pLeapNum[0] / pass;
		pLeapNum[0] += supp*pass;
	}
	else
	{
		for (i = 1; i < pass; i++)
		{
			pLeapNum[i] = pLeapNum[0];
		}

		residual = nozzlesize - pLeapNum[0];
		if (residual > 0)
		{//每Pass步进距离补偿（不同）
			for (i = (pass - 1); i >= 0; i--)
			{
				pLeapNum[i] += pass;
				residual--;
				if (residual == 0)
					break;
			}
		}
	}

	return pLeapNum[pass - 1];
}

//淡入淡出处理-得淡入参数值
//	blimit	:[in]是否计算最大值，当为FALSE时，计算默认参数值
int PRINTER::fade_calclimit(int nozzlesize, int pass, bool blimit)
{
	int		nFade = 0;
	DWORD	leapnum[_MAX_YPASS_LEVEL_ + 10], leapLimit;
	int		passFade = pass + 1;

	ASSERT(nozzlesize >= 2);
	ASSERT(pass >= 1);
	if (nozzlesize < 2 || pass < 1)
		return 0;
	if (pass < 0 || pass >= _MAX_YPASS_LEVEL_)
		return 0;

	BZERO(leapnum);

	leapLimit = PRINTER::step_calcleapnum(leapnum, nozzlesize, passFade, nozzlesize);		//max, pixel
	ASSERT(leapLimit > 0);

	if (blimit)
	{
		ASSERT(leapLimit >= leapnum[passFade - 1]);
		nFade = nozzlesize / pass;		//max limit
	}
	else
	{//default
		leapLimit = leapnum[0];				//min, pixel
		nFade = leapLimit / passFade;		//nozzle
	}

	return nFade;
}

//计算每个喷嘴占用的数据位数
//	nPrintColorLevel	:[in]实际打印使用的灰度级别，<= PRINTHEAD::nColorLevel
int PRINTER::ColorLevel_GetStepPixel(UINT nPrintColorLevel)
{
	int		nStepPixel = 1;		//每个喷嘴占用的数据位数

	if (nPrintColorLevel <= 2)
		nStepPixel = 1;
	else if (nPrintColorLevel <= 4)
		nStepPixel = 2;
	else if (nPrintColorLevel <= 16)
		nStepPixel = 4;
	else
	{
		ASSERT(0);
		nStepPixel = 1;
	}

	return nStepPixel;
}

//淡入淡出处理-是否有效
BOOL PRINTER::fade_IsValid(int nYPassType) const
{
	if (bVirtualPrinter)
		return FALSE;

	if (nYPassType < 0 || nYPassType >= nYPassCount)
		return FALSE;

	if (bYPassWay[nYPassType] != PRINTER::passway_plus)
		return FALSE;

	if (fade_nLevel[nYPassType] <= 0)
		return FALSE;

	return TRUE;
}
