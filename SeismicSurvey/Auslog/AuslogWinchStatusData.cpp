#include "stdafx.h"
#include ".\auslogwinchstatusdata.h"

CAuslogWinchStatusData::CAuslogWinchStatusData(void)
: m_Depth(0)
, m_Speed(0)
, m_bIsLive(false)
{
	m_OffsetDepth = 0;
}

CAuslogWinchStatusData::~CAuslogWinchStatusData(void)
{
}
