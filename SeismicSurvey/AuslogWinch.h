#pragma once

#include ".\serialport\serialport.h"
#include "Auslog\AuslogWinchPkt.h"

#define WM_BSCAN_ACTIVATED		WM_USER+100	// BScan Activate message. 
#define WM_BSCAN_TERMINATED		WM_USER+101	// BScan Activate message. 

class CAuslogWinch :public CSerialPort
{
public:
	CAuslogWinch(void);
	~CAuslogWinch(void);
	int m_PortNumber;
	bool m_bConnected;
	CAuslogWinchPkt m_AuslogWinchPkt;
	void SendHeartBeatPakcet(void);
	void AddCheckSum(CString inputpacket);
	void SetDepthToZeroPacket(void);
	void StopPacket(void);
	void SynchronousMove(float Speed, float Depth, int FirstToArriveFlag);
	void IndividualMove(float WASpeed, float WADepth,float WBSpeed, float WBDepth, float WCSpeed, float WCDepth, int FirstToArriveFlag);
	void SetDepth(float WADepth,float WBDepth, float WCDepth);
	bool PacketParsing(BYTE aByte);

	float m_TxDepth;
	float m_RxDepth;

	float m_ScanStartDepth;
	float m_ScanStopDepth;
	float m_ScanStepDepth;
	float m_ScanTargetDepth;
	float m_ScanOldDepth;
	bool m_bScanEnable;
	int m_ScanDirection;
	int m_ScanDevice;

	void ScanSet(float ScanStartDepth, float ScanStopDepth, float ScanStepDepth, int indx);
	void ScanClear();

};

