#pragma once
#include "auslogwinchstatusdata.h"

#define STX	0x02
#define ETX 0x03

#define READY 0
#define DELIMETER 1
#define DELIMETERPARSING 2
#define CHECKSUMPARSING 3
#define DATA 4
#define CHECKSUMDATA 5

#define DADEPTH 1
#define DBDEPTH 2
#define DCDEPTH 3
#define DTTIME 4
#define CHECKSUM 5

class CAuslogWinchPkt
{
public:
	CAuslogWinchPkt(void);
	~CAuslogWinchPkt(void);
	CAuslogWinchStatusData m_WinchA;
	CAuslogWinchStatusData m_WinchB;
	CAuslogWinchStatusData m_WinchC;
	int m_FirstToArriveFlag;
	BOOL PacketParsing(BYTE aByte);
	CString SendHeartBeatPakcet(void);
	int m_PortNumber;
	BOOL m_bIsConnected;
	CString AddCheckSum(CString inputpacket);
	CString SetDepthToZeroPacket(void);
	CString StopPacket(void);
	CString SynchronousMove(float Speed, float Depth, int FirstToArriveFlag);
	int m_PacketStatus;
	int m_CommandID;
	CString m_DAString, m_DBString, m_DCString, m_DTString, m_CSString;
	CString GenerateCheckSum(CString inputpacket);
	CString m_ReceivedPacket;
	CString IndividualMove(float WASpeed, float WADepth,float WBSpeed, float WBDepth, float WCSpeed, float WCDepth, int FirstToArriveFlag);
	CString SetDepth(float WADepth,float WBDepth, float WCDepth);
	float m_PrevADepth;
	float m_PrevBDepth;
	float m_PrevCDepth;
	float m_AVelocity;
	float m_BVelocity;
	float m_CVelocity;
};
