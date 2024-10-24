#pragma once

#include "SpiritSData.h"

class CSpiritSPacket
{
public:
	CSpiritSPacket(void);
	~CSpiritSPacket(void);
	bool Build(BYTE achar);
	bool Parsing(CSpiritSData * pData);

	BYTE m_StartDelimeter[9];
	BYTE m_EndDelimeter[3];

	bool m_bPacketCompleted;

	int m_PacketStatus;

	BYTE * m_Header;
	BYTE * m_Data;

	int m_CurrentIndex;
	int m_DataSize;
	int m_DataNo;
	int m_HeaderSize;
	int m_DelimeterSize;

	USHORT vec2uint2_bigendian(BYTE * start);
	int vec2uint3_bigendian(BYTE * start);
};

