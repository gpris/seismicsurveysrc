#include "StdAfx.h"
#include "SpiritSPacket.h"

#define SPIRIT_S_PACKET_READY 0
#define SPIRIT_S_PACKET_STARTDEL1 1
#define SPIRIT_S_PACKET_STARTDEL2 2
#define SPIRIT_S_PACKET_STARTDEL3 3
#define SPIRIT_S_PACKET_STARTDEL4 4
#define SPIRIT_S_PACKET_STARTDEL5 5
#define SPIRIT_S_PACKET_STARTDEL6 6
#define SPIRIT_S_PACKET_STARTDEL7 7
#define SPIRIT_S_PACKET_STARTDEL8 8
#define SPIRIT_S_PACKET_HEADER	9
#define SPIRIT_S_PACKET_DATA 10
#define SPIRIT_S_PACKET_ENDDEL1 11
#define SPIRIT_S_PACKET_ENDDEL2 12
#define SPIRIT_S_PACKET_ENDDEL3 13


CSpiritSPacket::CSpiritSPacket(void)
{
	m_PacketStatus = SPIRIT_S_PACKET_READY;
	m_HeaderSize = 16;
//	m_DataSize = 4096;
	m_DataSize = 4096 + 2048 + 9;
	m_DataNo = 2048;
	m_bPacketCompleted = FALSE;
	m_StartDelimeter[0] = 0x00;
	m_StartDelimeter[1] = 0x53;
	m_StartDelimeter[2] = 0x50;
	m_StartDelimeter[3] = 0x49;
	m_StartDelimeter[4] = 0x52;
	m_StartDelimeter[5] = 0x49;
	m_StartDelimeter[6] = 0x54;
	m_StartDelimeter[7] = 0x2D;
	m_StartDelimeter[8] = 0x53;

	m_EndDelimeter[0] = 0x45;
	m_EndDelimeter[1] = 0x4E;
	m_EndDelimeter[2] = 0x44;

	m_DelimeterSize = 9;
	m_Header = new BYTE[m_HeaderSize];
	m_Data = new BYTE[m_DataSize];
	m_CurrentIndex = 0;
}


CSpiritSPacket::~CSpiritSPacket(void)
{
}

bool CSpiritSPacket::Parsing(CSpiritSData * pData)
{
	pData->m_RxChannelID = m_Header[9];
	pData->m_GainID = m_Header[10];
	pData->m_SamplingCLKID = m_Header[11];
	pData->m_PRBSCLKID = m_Header[12];
	pData->m_PRBSLengthID = m_Header[13];

	pData->m_DataNo = m_DataNo;
	//if(pData->m_Data)
	//{
	//	pData->m_Data = new USHORT[pData->m_DataNo];
	//}

	for (int dindx = 0; dindx<m_DataNo; dindx++)
	{
//		pData->m_Data[dindx] = vec2uint2_bigendian(&m_Data[dindx*2]);
		pData->m_Data[dindx] = vec2uint3_bigendian(&m_Data[dindx*3]);
//		TRACE("dindx = %d, value = %d\n", dindx, pData->m_Data[dindx]);
	}

//	memcpy(pData->m_Data, m_Data, m_DataSize);

	return true;
}

USHORT CSpiritSPacket::vec2uint2_bigendian(BYTE * start)
{
	USHORT output = start[0] * 256 + start[1];

	return(output);
}

int CSpiritSPacket::vec2uint3_bigendian(BYTE * start)
{
	int output = start[0] * 65536 + start[1] * 256 + start[2];

	return(output);
}

bool CSpiritSPacket::Build(BYTE achar)
{
	int dataindex;

	switch(m_PacketStatus)
	{
	case SPIRIT_S_PACKET_READY:
		if(achar == m_StartDelimeter[0])
		{
			m_PacketStatus = SPIRIT_S_PACKET_STARTDEL1;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		break;
	case SPIRIT_S_PACKET_STARTDEL1:
		if(achar == m_StartDelimeter[1])
		{
			m_PacketStatus = SPIRIT_S_PACKET_STARTDEL2;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		else
		{
			if(achar == m_StartDelimeter[0])
			{
				m_PacketStatus = SPIRIT_S_PACKET_STARTDEL1;
				m_Header[m_CurrentIndex] = achar;
				m_CurrentIndex=1;
			}
			else
			{
				m_PacketStatus = SPIRIT_S_PACKET_READY;
				m_CurrentIndex = 0;
			}
		}
		break;
	case SPIRIT_S_PACKET_STARTDEL2:
		if(achar == m_StartDelimeter[2])
		{
			m_PacketStatus = SPIRIT_S_PACKET_STARTDEL3;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_STARTDEL3:
		if(achar == m_StartDelimeter[3])
		{
			m_PacketStatus = SPIRIT_S_PACKET_STARTDEL4;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_STARTDEL4:
		if(achar == m_StartDelimeter[4])
		{
			m_PacketStatus = SPIRIT_S_PACKET_STARTDEL5;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_STARTDEL5:
		if(achar == m_StartDelimeter[5])
		{
			m_PacketStatus = SPIRIT_S_PACKET_STARTDEL6;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_STARTDEL6:
		if(achar == m_StartDelimeter[6])
		{
			m_PacketStatus = SPIRIT_S_PACKET_STARTDEL7;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_STARTDEL7:
		if(achar == m_StartDelimeter[7])
		{
			m_PacketStatus = SPIRIT_S_PACKET_STARTDEL8;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_STARTDEL8:
		if(achar == m_StartDelimeter[8])
		{
			m_PacketStatus = SPIRIT_S_PACKET_HEADER;
			m_Header[m_CurrentIndex] = achar;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_HEADER:
		m_Header[m_CurrentIndex] = achar;
		m_CurrentIndex++;
		if(m_CurrentIndex == m_HeaderSize)
		{
			m_PacketStatus = SPIRIT_S_PACKET_DATA;
		}
		break;
	case SPIRIT_S_PACKET_DATA:
		dataindex = m_CurrentIndex - m_HeaderSize;
		m_Data[dataindex] = achar;
//		TRACE("%d:%X\n", dataindex, achar);
		m_CurrentIndex++;
		if(m_CurrentIndex == m_HeaderSize + m_DataSize)
		{
			m_PacketStatus = SPIRIT_S_PACKET_ENDDEL1;
			//m_PacketStatus = SPIRIT_S_PACKET_READY;
			//m_CurrentIndex = 0;
			//m_bPacketCompleted = TRUE;

			//return(TRUE);
		}
		else
		{
			if(m_CurrentIndex>m_HeaderSize + m_DataSize)
			{
				m_PacketStatus = SPIRIT_S_PACKET_READY;
				m_CurrentIndex = 0;
			}
		}
		break;
	case SPIRIT_S_PACKET_ENDDEL1:
		if(achar == m_EndDelimeter[0])
		{
			m_PacketStatus = SPIRIT_S_PACKET_ENDDEL2;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_ENDDEL2:
		if(achar == m_EndDelimeter[1])
		{
			m_PacketStatus = SPIRIT_S_PACKET_ENDDEL3;
			m_CurrentIndex++;
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}
		break;
	case SPIRIT_S_PACKET_ENDDEL3:
		if(achar == m_EndDelimeter[2])
		{
			m_PacketStatus = SPIRIT_S_PACKET_ENDDEL3;
			m_CurrentIndex++;
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
			m_bPacketCompleted = TRUE;
			return(TRUE);
		}
		else
		{
			m_PacketStatus = SPIRIT_S_PACKET_READY;
			m_CurrentIndex = 0;
		}		break;
	}

	return(FALSE);
}