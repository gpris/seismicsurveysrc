#include "StdAfx.h"
#include "SpiritSData.h"

CSpiritSData::CSpiritSData(void)
{
	m_RxChannelID =0;
	m_GainID = 0;
	m_SamplingCLKID = 0;
	m_PRBSCLKID = 0;
	m_PRBSLengthID = 0;

	m_DataNo = 2048;

	m_Data = new unsigned int[m_DataNo];;
}


CSpiritSData::~CSpiritSData(void)
{
	if(m_Data)
	{
		delete m_Data;
	}
}


