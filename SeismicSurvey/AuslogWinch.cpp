#include "StdAfx.h"
#include "AuslogWinch.h"
#include <cmath>


CAuslogWinch::CAuslogWinch(void)
{
	m_PortNumber = 0;
	m_bConnected = false;
	m_TxDepth = 0;
	m_RxDepth = 0;

	m_bScanEnable = false;
}


CAuslogWinch::~CAuslogWinch(void)
{
}

void CAuslogWinch::SendHeartBeatPakcet(void)
{
	CString packet;

	packet = m_AuslogWinchPkt.SendHeartBeatPakcet();
	WriteToPort((char *)packet.GetBuffer());
}

void CAuslogWinch::SetDepthToZeroPacket(void)
{
	CString packet;

	packet = m_AuslogWinchPkt.SetDepthToZeroPacket();
	WriteToPort((char *)packet.GetBuffer());
}

void CAuslogWinch::StopPacket(void)
{
	CString packet;

	packet = m_AuslogWinchPkt.StopPacket();
	WriteToPort((char *)packet.GetBuffer());

	m_bScanEnable = false;
}

void CAuslogWinch::SynchronousMove(float Speed, float Depth, int FirstToArriveFlag)
{
	CString packet;

	packet = m_AuslogWinchPkt.SynchronousMove(Speed, Depth, FirstToArriveFlag);
	WriteToPort((char *)packet.GetBuffer());
}

void CAuslogWinch::IndividualMove(float WASpeed, float WADepth, 
										float WBSpeed, float WBDepth, 
										float WCSpeed, float WCDepth, 
										int FirstToArriveFlag)
{
	CString packet;

	packet = m_AuslogWinchPkt.IndividualMove(WASpeed, WADepth, 
										WBSpeed,WBDepth, 
										WCSpeed, WCDepth, 
										FirstToArriveFlag);
	DirectWriteToPort((char *)packet.GetBuffer());
}

void CAuslogWinch::SetDepth(float WADepth, float WBDepth, float WCDepth)
{
	CString packet;

	packet = m_AuslogWinchPkt.SetDepth(WADepth, WBDepth, WCDepth);
	WriteToPort((char *)packet.GetBuffer());
}

bool CAuslogWinch::PacketParsing(BYTE aByte)
{
	bool result;

	float TxNowDifference=0;
	float RxNowDifference=0;
	float TargetDifference = 0;

	if(result = m_AuslogWinchPkt.PacketParsing(aByte))
	{
		m_TxDepth = m_AuslogWinchPkt.m_WinchA.m_Depth;
		m_RxDepth = m_AuslogWinchPkt.m_WinchB.m_Depth;

		if(m_bScanEnable)
		{
			TxNowDifference = (m_ScanTargetDepth - m_TxDepth) * m_ScanDirection;
			RxNowDifference = (m_ScanTargetDepth - m_RxDepth) * m_ScanDirection;
			TargetDifference = ( m_ScanTargetDepth - m_ScanStopDepth) * m_ScanDirection;
			if(TxNowDifference<=0.05 && RxNowDifference<=0.05 && m_AuslogWinchPkt.m_AVelocity == 0 && m_AuslogWinchPkt.m_BVelocity ==0 )
			{
				::SendMessage(m_pOwner->m_hWnd, WM_BSCAN_ACTIVATED, (WPARAM) m_TxDepth, (LPARAM) TxNowDifference);				
//				TRACE("condition = %f\n", ( m_ScanTargetDepth - m_ScanStopDepth) * m_ScanDirection);
				if(abs(TargetDifference) <= 0.5 && m_AuslogWinchPkt.m_AVelocity ==0 && m_AuslogWinchPkt.m_BVelocity ==0 )
				{
					::SendMessage(m_pOwner->m_hWnd, WM_BSCAN_TERMINATED, (WPARAM) m_TxDepth, (LPARAM) TxNowDifference);
				}
				m_ScanTargetDepth = m_ScanTargetDepth + m_ScanDirection * m_ScanStepDepth;
			}
		}

	}

	return result;
}

void CAuslogWinch::ScanSet(float ScanStartDepth, float ScanStopDepth, float ScanStepDepth, int indx)
{
	m_ScanStartDepth = ScanStartDepth;
	m_ScanStopDepth = ScanStopDepth;
	m_ScanStepDepth = ScanStepDepth;
	m_ScanDevice = indx;

	m_ScanTargetDepth = m_ScanStartDepth;

	if(m_ScanStartDepth > m_ScanStopDepth)
	{
		m_ScanDirection = -1.0;
	}
	else
	{
		m_ScanDirection = 1.0;
	}
	m_bScanEnable = true;
}
