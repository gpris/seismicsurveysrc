#include "StdAfx.h"
#include "ScanInfo.h"


CScanInfo::CScanInfo(void)
{
	m_SiteName = _T("");
	m_SubSiteName = _T("");
	m_TxHoleName = _T("");
	m_RxHoleName = _T("");


	m_MeasTime = 0;

	m_StartDepth = 0.0;
	m_StopDepth = 0.0;
	m_TargetDepth = 0.0;

	m_ScanStep = 0.5;
	m_ScanVelocity = 0.0;
	m_ScanMode = SCANBYDEPTH;
	m_ScanDirection = DOWN_DIRECTION;
}


CScanInfo::~CScanInfo(void)
{
}
