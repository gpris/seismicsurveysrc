#include "stdafx.h"
#include ".\auslogwinchpkt.h"
#include ".\auslogwinchstatusdata.h"

CAuslogWinchPkt::CAuslogWinchPkt(void)
: m_FirstToArriveFlag(0)
{
	m_PortNumber = 0;
	m_bIsConnected = FALSE;
	m_PacketStatus = READY;
	m_DAString = _T("");
	m_DBString = _T("");
	m_DCString = _T("");
	m_DTString = _T("");
	m_CSString = _T("");
	m_ReceivedPacket = _T("");
	m_PrevADepth = 0;
	m_PrevBDepth = 0;
	m_PrevCDepth = 0;
}

CAuslogWinchPkt::~CAuslogWinchPkt(void)
{
}

BOOL CAuslogWinchPkt::PacketParsing(BYTE aByte)
{
	BOOL result = FALSE;

	if(m_PacketStatus != READY)
	{
		m_ReceivedPacket.AppendChar(aByte);
	}
	switch(m_PacketStatus)
	{
	case READY:
        if(aByte == STX)
		{
			m_PacketStatus = DELIMETER;
			m_ReceivedPacket.AppendChar(aByte);
		}
		break;
	case DELIMETER:
		switch(aByte)
		{
		case 'D':
			m_PacketStatus = DELIMETERPARSING;
			break;
		case 'C':
			m_PacketStatus = CHECKSUMPARSING;
			break;
		default :
			m_PacketStatus = READY;
			break;
		}
		break;
	case DELIMETERPARSING:
		switch(aByte)
		{
		case 'A':
			m_PacketStatus = DATA;
			m_CommandID = DADEPTH;
			break;
		case 'B':
			m_PacketStatus = DATA;
			m_CommandID = DBDEPTH;
			break;
		case 'C':
			m_PacketStatus = DATA;
			m_CommandID = DCDEPTH;
			break;
		case 'T':
			m_PacketStatus = DATA;
			m_CommandID = DTTIME;
			break;
		default:
			m_PacketStatus = READY;
			break;
		}
		break;
	case CHECKSUMPARSING:
		switch(aByte)
		{
		case 'S':
			m_PacketStatus = CHECKSUMDATA;
			break;
		default:
			m_PacketStatus = READY;
			break;
		}
		break;
	case DATA:
		switch(aByte)
		{
		case 'D':
			m_PacketStatus = DELIMETERPARSING;
			break;
		case 'C':
			m_PacketStatus = CHECKSUMPARSING;
			break;
		case ETX:
			m_PacketStatus = READY;
			if(!m_CSString.Compare(GenerateCheckSum(m_ReceivedPacket)))
			{
				m_WinchA.m_Depth = atof(m_DAString.GetBuffer()) + m_WinchA.m_OffsetDepth;//+0.75;//안테나보정치 적용0.75
				m_WinchB.m_Depth = atof(m_DBString.GetBuffer()) + m_WinchB.m_OffsetDepth;//+0.75;
				m_WinchC.m_Depth = atof(m_DCString.GetBuffer()) + m_WinchC.m_OffsetDepth;//+0.75;
				result = TRUE;
			}
			m_DAString = _T("");
			m_DBString = _T("");
			m_DCString = _T("");
			m_DTString = _T("");
			m_CSString = _T("");
			m_ReceivedPacket = _T("");
			break;
		default:
			switch(m_CommandID)
			{
			case DADEPTH:
				m_DAString.AppendChar(aByte);
				break;
			case DBDEPTH:
				m_DBString.AppendChar(aByte);
				break;
			case DCDEPTH:
				m_DCString.AppendChar(aByte);
				break;
			case DTTIME:
				m_DTString.AppendChar(aByte);
				break;
			}
			break;	
		}
		break;
	case CHECKSUMDATA:
		switch(aByte)
		{
		case ETX:
			m_PacketStatus = READY;
//			if(!m_CSString.Compare(GenerateCheckSum(m_ReceivedPacket)))  //Checksum check is disabled!!
//			{
				m_WinchA.m_Depth = atof(m_DAString.GetBuffer()) + m_WinchA.m_OffsetDepth;//+0.75;//안테나보정치 적용0.75
				m_WinchB.m_Depth = atof(m_DBString.GetBuffer()) + m_WinchB.m_OffsetDepth;//+0.75;
				m_WinchC.m_Depth = atof(m_DCString.GetBuffer()) + m_WinchC.m_OffsetDepth;//+0.75;
				result = TRUE;
//			}
			m_DAString = _T("");
			m_DBString = _T("");
			m_DCString = _T("");
			m_DTString = _T("");
			m_CSString = _T("");
			m_ReceivedPacket = _T("");
			m_AVelocity = m_PrevADepth - m_WinchA.m_Depth;
			m_PrevADepth = m_WinchA.m_Depth;
			m_BVelocity = m_PrevBDepth - m_WinchB.m_Depth;
			m_PrevBDepth = m_WinchB.m_Depth;
			m_CVelocity = m_PrevCDepth - m_WinchC.m_Depth;
			m_PrevCDepth = m_WinchC.m_Depth;
			break;
		case STX:
			m_DAString = _T("");
			m_DBString = _T("");
			m_DCString = _T("");
			m_DTString = _T("");
			m_CSString = _T("");
			m_ReceivedPacket = _T("");
			m_PacketStatus = DELIMETER;
			m_ReceivedPacket.AppendChar(aByte);
			break;
		default :
			if(m_CSString.GetLength() < 4)
			{
				m_CSString.AppendChar(aByte);
			}
			else
			{
				m_DAString = _T("");
				m_DBString = _T("");
				m_DCString = _T("");
				m_DTString = _T("");
				m_CSString = _T("");
				m_ReceivedPacket = _T("");
				m_PacketStatus = READY;
			}
		}
		break;

	}

	return result;
}

CString CAuslogWinchPkt::SendHeartBeatPakcet(void)
{
	CString pkt_msg, finalpkt;

	pkt_msg.Format("%cMH", STX);
	finalpkt = AddCheckSum(pkt_msg);
	finalpkt.AppendChar(ETX);
	return finalpkt;
}

CString CAuslogWinchPkt::AddCheckSum(CString inputpacket)
{
	unsigned char checksum;
	CString checksumString;

	checksum = 0;
	for(int indx = 1; indx<inputpacket.GetLength(); indx++)
	{
		checksum += inputpacket.GetAt(indx);
	}
	inputpacket.Append(_T("CS"));
	checksumString.Format("%02X", checksum);
	inputpacket.Append(checksumString);
	return inputpacket;
}

CString CAuslogWinchPkt::GenerateCheckSum(CString inputpacket)
{
	unsigned char checksum;
	CString checksumString;

	checksum = 0;
	for(int indx = 1; indx<inputpacket.GetLength()-5; indx++)
	{
		checksum += inputpacket.GetAt(indx);
	}
	checksumString.Format("%02X", checksum);
	return checksumString;
}

CString CAuslogWinchPkt::SetDepthToZeroPacket(void)
{
	CString pkt_msg, finalpkt;

	pkt_msg.Format("%cMZ", STX);
	finalpkt = AddCheckSum(pkt_msg);
	finalpkt.AppendChar(ETX);
	return finalpkt;
}

CString CAuslogWinchPkt::StopPacket(void)
{
	CString pkt_msg, finalpkt;

	pkt_msg.Format("%cMX", STX);
	finalpkt = AddCheckSum(pkt_msg);
	finalpkt.AppendChar(ETX);
	return finalpkt;
}

CString CAuslogWinchPkt::SynchronousMove(float Speed, float Depth, int FirstToArriveFlag)
{
	CString pkt_msg, finalpkt;

	pkt_msg.Format("%cMPWP%.1f,%.3fFT%1d", STX, Speed, Depth, FirstToArriveFlag);
	finalpkt = AddCheckSum(pkt_msg);
	finalpkt.AppendChar(ETX);
	return finalpkt;
}

CString CAuslogWinchPkt::IndividualMove(float WASpeed, float WADepth, 
										float WBSpeed, float WBDepth, 
										float WCSpeed, float WCDepth, 
										int FirstToArriveFlag)
{
	CString pkt_msg, finalpkt;

	pkt_msg.Format("%cMIWA%.1f,%.3f,WB%.1f,%.3f,WC%.1f,%.3f,FT%1d,", STX, WASpeed, WADepth, WBSpeed, WBDepth, WCSpeed, WCDepth, FirstToArriveFlag);
	finalpkt = AddCheckSum(pkt_msg);
	finalpkt.AppendChar(ETX);
	return finalpkt;
}

CString CAuslogWinchPkt::SetDepth(float WADepth, float WBDepth, float WCDepth)
{
	CString pkt_msg, finalpkt;

	pkt_msg.Format("%cMDWA%.3f,WB%.3f,WC%.3f", STX, WADepth, WBDepth, WCDepth);
	finalpkt = AddCheckSum(pkt_msg);
	finalpkt.AppendChar(ETX);
	return finalpkt;
}
