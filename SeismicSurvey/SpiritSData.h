#pragma once
class CSpiritSData
{
public:
	CSpiritSData(void);
	~CSpiritSData(void);

	BYTE m_RxChannelID;
	BYTE m_GainID;
	BYTE m_SamplingCLKID;
	BYTE m_PRBSCLKID;
	BYTE m_PRBSLengthID;

	int m_DataNo;

	unsigned int * m_Data;
};

