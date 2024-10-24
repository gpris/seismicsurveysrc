#include "StdAfx.h"
#include "SeismicSonde.h"

#include <cmath>

float SamplingTime[5] = {1e-5, 5e-6, 2e-5, 5e-5, 1e-4};

CSeismicSonde::CSeismicSonde(void)
{
	m_PortNumber = 0;
	m_bConnected = false;

	m_TriggerMode = TRIG_COMMANDMODE;
	m_TriggerFreq = TRIG_FREQ_1;
	m_PRBSFreqID = PRBSFREQ_5KHZ;
	m_PRBSLengthID = PRBSLENGTH_4BIT;
	m_PRBSDelay = 0;
	m_PRBSRepeat = 1;

	m_ChanNo = 16;
	m_DataNo = 2048;
	m_CurrentData = new CSpiritSData[m_ChanNo];
	m_SondeMode = new int[m_ChanNo];
	m_Rx1stAmpID = new int[m_ChanNo];
	m_Rx2ndAmpID = new int[m_ChanNo];
	m_SamplingClkID = new int[m_ChanNo];
	m_SamplingDelayID = new int[m_ChanNo];
	m_Live = new int[m_ChanNo];
	m_AccumulID = new int[m_ChanNo];

	for(int indx = 0; indx<m_ChanNo; indx++)
	{
		m_SondeMode[indx] = SONDE_DEBUGMODE;
		m_SamplingClkID[indx] = SAMPLING_100KHZ;
		m_Rx1stAmpID[indx] = 0;
		m_Rx2ndAmpID[indx] = 0;
		m_Live[indx] = 0;
		m_AccumulID[indx] = 0;
	}

	for(int chindx = 0; chindx<m_ChanNo; chindx++)
	{
		CSeismicData * tempData = new CSeismicData(m_DataNo);
		for(int dindx = 0; dindx<m_DataNo; dindx++)
		{
			tempData->m_Data[dindx] = 0.0;
		}
		m_ChData.Add(tempData);
	}

	m_Packet = new char[20000];
	m_Mesg = new char[20000];

	m_ReadBytes = 0;
	m_dt = 1/100e3;

	m_SelectedChannel = 0;
	m_TriggerNo = 0;

	m_TxDepthOffset = 7.0;  // Distance from marker to the center of Tx in meter
	m_RxDepthOffset = 7.0;  // Distance from marker to the center of Hydrophone #7
	m_TxDepth = 0.0;   //Current depth;
	m_RxDepth = 0.0;   //Current depth

	m_bCmdTriggerTerminated = false;

	m_RealTriggerFreq[0] = 1;
	m_RealTriggerFreq[1] = 2;
	m_RealTriggerFreq[2] = 4;
	m_RealTriggerFreq[3] = 5;
	m_RealTriggerFreq[4] = 10;
}


CSeismicSonde::~CSeismicSonde(void)
{
}

void CSeismicSonde:: SetSondeMode(int ch, int mode)
{
	CString packet;
	packet.Format(_T("w 0x2%X0 0x0%d "), ch, mode);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetTriggerMode(int mode)
{
	CString packet;
	packet.Format(_T("w 0x21 0x0%d "), mode);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetTriggerReady()
{
	CString packet;
	packet.Format(_T("ready #"));
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

#include "MainFrm.h"
#include "TriggerMonitorDlg.h"

BOOL CSeismicSonde:: StartCmdTrigger()
{
	BOOL result = true;
	CString packet;
	packet.Format(_T("w 0x000 0x02 "));
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();

	int count = 0;
	int DataNo =0;
	int BytesRead = 0;
	bool BytesReadCompleted = false;
	CMainFrame * pMainWnd = (CMainFrame *)AfxGetMainWnd();
	char tempPacket[20000];
	char completedmessage[7] = "CMPLT ";
	
	pMainWnd->m_RxControlPane.m_RxControlDlg.m_DataReadBtnCtrl.EnableWindow(false);

	CTriggerMonitorDlg dlg;

	dlg.CreateModaless(this);

	do
	{
		if(m_bCmdTriggerTerminated) break;
		Sleep(1000);
		BytesRead = DirectReadFromPort(&tempPacket[DataNo]);
		DataNo += BytesRead;
		count++;
		if(DataNo>6)
		{
			tempPacket[DataNo] = 0;
			if(completedmessage[0] == tempPacket[1] && completedmessage[1] == tempPacket[2])
			{
				BytesReadCompleted = true;
				pMainWnd->m_RxControlPane.m_RxControlDlg.m_DataReadBtnCtrl.EnableWindow(true);
			}
		}
		if(count>60)
		{
			CString tempmessage;
			tempmessage.Format("Trigger is failed!:%s", &tempPacket[1]);
//			AfxMessageBox("Trigger is failed! Please retry...");
			AfxMessageBox(tempmessage);
			result = false;
			break;
		}
	}while(!BytesReadCompleted);

	dlg.DestroyWindow();;

	return result;
}

BOOL CSeismicSonde:: StartCmdTriggerDlg()
{
	BOOL result = true;
	CString packet;
	packet.Format(_T("w 0x000 0x02 "));
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();

	CMainFrame * pMainWnd = (CMainFrame *)AfxGetMainWnd();
	
	pMainWnd->m_RxControlPane.m_RxControlDlg.m_DataReadBtnCtrl.EnableWindow(false);

	CTriggerMonitorDlg dlg;

	m_bCmdTriggerTerminated = false;
	float temp1 = (1.0/(float)(m_RealTriggerFreq[m_TriggerFreq]));
	float temp2 = (32 * pow(2.0, m_AccumulID[0])) * 3.0;
	dlg.m_MaxWaitTimeInSec = (int)(temp1 * temp2) ;
	dlg.m_pSonde = this;
	dlg.DoModal();

	result = dlg.m_bResult;
	if(result)
	{
		pMainWnd->m_RxControlPane.m_RxControlDlg.m_DataReadBtnCtrl.EnableWindow(true);
	}

	return result;
}

BOOL CSeismicSonde:: StartCmdTriggerDlgThread(CMainFrame * pMainWnd)
{
	BOOL result = true;
	CString packet;
	packet.Format(_T("w 0x000 0x02 "));
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
	
	pMainWnd->m_RxControlPane.m_RxControlDlg.m_DataReadBtnCtrl.EnableWindow(false);

	CTriggerMonitorDlg dlg;

	m_bCmdTriggerTerminated = false;
	dlg.m_pSonde = this;
	dlg.DoModal();

	result = dlg.m_bResult;
	if(result)
	{
		pMainWnd->m_RxControlPane.m_RxControlDlg.m_DataReadBtnCtrl.EnableWindow(true);
	}

	return result;
}

void CSeismicSonde:: ReadADCData(int ch)
{

	int datano = m_CurrentData[ch].m_DataNo;
	CSeismicData * tempData = m_ChData.GetAt(ch);
	for(int dindx = 0; dindx<datano; dindx++)
	{
		tempData->m_Data[dindx] = 0.0;
	}


	CString packet;
	packet.Format(_T("w 0x2%X2 0x01 "), ch);
	DirectWriteToPort((char *)packet.GetBuffer());

	int count = 0;
	int DataNo =0;
	int BytesRead = 0;
	CMainFrame * pMainWnd = (CMainFrame *)AfxGetMainWnd();

	do
	{
		if (count)
		{
			Sleep(500);
		}
		else
		{
			Sleep(1000);
		}
		BytesRead = DirectReadFromPort(&m_Packet[DataNo]);
		DataNo += BytesRead;
		int nPos = (int)(((float)DataNo*100)/((float)(m_SpiritSPacket.m_HeaderSize + m_SpiritSPacket.m_DataSize)));
		pMainWnd->m_RxControlPane.m_RxControlDlg.m_RxStatusProgressCtrl.SetPos(nPos);
		CString tempstring;
		tempstring.Format("CH%d:", ch);
		pMainWnd->m_RxControlPane.m_RxControlDlg.m_RxStatusCtrl.SetWindowTextA(tempstring);
//		TRACE("count = %d, BytesRead = %d, DataNo = %d\n", count, BytesRead, DataNo);
		count++;
	}while(BytesRead);
	pMainWnd->m_RxControlPane.m_RxControlDlg.m_RxStatusProgressCtrl.SetPos(0);

	m_ReadBytes = DataNo;
	DirectPacketParsing();
}

void CSeismicSonde::ReadMessage()
{
	Sleep(100);
	int aa = DirectReadFromPort(m_Mesg);
	m_Mesg[aa] = 0x00;
	if(aa>0)
	{
		CMainFrame * pMainWnd = (CMainFrame *)AfxGetMainWnd();
		CString tempstring;
		tempstring.Format("%s:", &m_Mesg[1]);
		pMainWnd->m_RxControlPane.m_RxControlDlg.m_RxStatusEditCtrl.SetWindowTextA(tempstring);
	}
}

void CSeismicSonde:: ReadADCDataLSB()
{
	CString packet;
	packet.Format(_T("r 0x03 "));
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: ReadADCDataMSB()
{
	CString packet;
	packet.Format(_T("r 0x04 "));
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetADCClock(int ch, int mode)
{
	CString packet;
	int value;

	switch(mode)
	{
	case 0:
		value = 0;
		m_dt = 1/100e3;
		break;
	case 1:
		value = 1;
		m_dt = 1/200e3;
		break;
	case 2:
		value = 2;
		m_dt = 1/50e3;
		break;
	case 3:
		value = 4;
		m_dt = 1/20e3;
		break;
	case 4:
		value = 8;
		m_dt = 1/10e3;
		break;
	}
	packet.Format(_T("w 0x2%X5 0x0%d "), ch, value);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetADCDelay(int ch, int mode)
{
	CString packet;
	int value;

	switch(mode)
	{
	case 0:
		value = 0x0A;
		break;
	case 1:
		value = 0x13;
		break;
	case 2:
		value = 0x31;
		break;
	case 3:
		value = 0x63;
		break;
	case 4:
		value = 0x95;
		break;
	case 5:
		value = 0xC7;
		break;
	case 6:
		value = 0xF9;
		break;
	}
	packet.Format(_T("w 0x2%X9 0x%02X "), ch, value);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetVGASet(int ch)
{
	int iGainValue = 0;

	iGainValue = m_Rx2ndAmpID[ch]*8 + m_Rx1stAmpID[ch];

	CString packet;
	packet.Format(_T("w 0x2%X6 0x%02X "), ch, iGainValue);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetPRBSFreq(int mode)
{
	CString packet;
	packet.Format(_T("w 0x107 0x0%d "), mode);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();

}

void CSeismicSonde:: SetPRBSLength(int mode)
{
	CString packet;
	packet.Format(_T("w 0x108 0x0%d "), mode);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetPRBSDelay(BYTE delay)
{
	CString packet;
	m_PRBSDelay = delay;
	packet.Format(_T("w 0x109 0x%02X "), delay);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetPRBSRepeat(BYTE repeat)
{
	CString packet;
	m_PRBSRepeat = repeat;

	packet.Format(_T("w 0x10B 0x%02X "), repeat);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetTriggerFreq(int mode)
{
	CString packet;
	m_TriggerFreq = mode;
	int value;

	switch(mode)
	{
	case 0:
		value = 0; // 1Hz
		break;
	case 1:
		value = 1; // 2Hz
		break;
	case 2:
		value = 2; // 4Hz
		break;
	case 3:
		value = 3; // 5Hz
		break;
	case 4:
		value = 4; // 10Hz
		break;
	}

	packet.Format(_T("w 0x005 0x%02X "), value);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

bool CSeismicSonde:: PacketParsing(BYTE achar)
{
	if(m_SpiritSPacket.Build(achar))
	{
		int channo = m_SpiritSPacket.m_Header[9];

		m_SpiritSPacket.Parsing(&m_CurrentData[channo]);
		
		//if(channo == m_ChanNo - 1)
		//{
		//	if(!m_ChData.IsEmpty())
		//	{
		//		for(int indx = 0;indx<m_ChData.GetSize();indx++)
		//		{
		//			CSeismicData * pSeismicData = m_ChData.GetAt(indx);
		//			delete pSeismicData;
		//		}
		//		m_ChData.RemoveAll();
		//	}

		int datano = m_CurrentData[channo].m_DataNo;
		CSeismicData * tempData = m_ChData.GetAt(channo);

		int id = m_AccumulID[0];
		if(m_AccumulID[0] == 4)
			id = 3;

		for(int dindx = 0; dindx<datano; dindx++)
		{
			tempData->m_Data[dindx] = m_CurrentData[channo].m_Data[dindx] - pow(2.0, 20.0+ id);
		}

		tempData->m_SamplingTime = SamplingTime[m_CurrentData[channo].m_SamplingCLKID]; 
		tempData->Analyze();
//				m_ChData.Add(tempData);
		//}

		return true;
	}
	return false;
}

bool CSeismicSonde:: DirectPacketParsing()
{
	if(m_ReadBytes == 6168)
	{
		memcpy(m_SpiritSPacket.m_Header, m_Packet, m_SpiritSPacket.m_HeaderSize);
		memcpy(m_SpiritSPacket.m_Data, m_Packet + m_SpiritSPacket.m_HeaderSize, m_SpiritSPacket.m_DataSize);

		int channo = m_SpiritSPacket.m_Header[9];

		m_SpiritSPacket.Parsing(&m_CurrentData[channo]);
		
		//if(channo == m_ChanNo - 1)
		//{
		//	if(!m_ChData.IsEmpty())
		//	{
		//		for(int indx = 0;indx<m_ChData.GetSize();indx++)
		//		{
		//			CSeismicData * pSeismicData = m_ChData.GetAt(indx);
		//			delete pSeismicData;
		//		}
		//		m_ChData.RemoveAll();
		//	}

		int datano = m_CurrentData[channo].m_DataNo;
		CSeismicData * tempData = m_ChData.GetAt(channo);

		int id = m_AccumulID[0];

		if(m_AccumulID[0] == 4)
			id = 3;

		for(int dindx = 0; dindx<datano; dindx++)
		{
			tempData->m_Data[dindx] = m_CurrentData[channo].m_Data[dindx] - pow(2.0, 20.0+ id);
		}
		tempData->m_ChID = channo;
		tempData->m_SamplingTime = SamplingTime[m_CurrentData[channo].m_SamplingCLKID];
		tempData->m_TxDepth = m_TxDepth + m_TxDepthOffset;
		tempData->m_RxDepth = m_RxDepth + m_RxDepthOffset + (channo - 7) * 0.5;
		tempData->Analyze();
		tempData->ApplySignalProcessing(&m_SigParameter);
		m_SigParameter.SetBPFCorrelatedFlag(true);

//		tempData->MakeFilteredSignal(CSeismicData::FILTER_HPF, 1e3, 0);
	

//				m_ChData.Add(tempData);
		//}
		::SendMessage(m_pOwner->m_hWnd, WM_COMM_PACKET_RECEIVED, (WPARAM) channo, (LPARAM) m_nPortNr);

		return true;
	}
	return false;
}

void CSeismicSonde:: SetAccumulateNo(int ch, int mode)
{
	CString packet;
	packet.Format(_T("w 0x2%XB 0x%02d "), ch, mode);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

void CSeismicSonde:: SetTriggerNo(int mode)
{
	CString packet;
	packet.Format(_T("w 0x006 0x%02d "), mode);
	DirectWriteToPort((char *)packet.GetBuffer());
	ReadMessage();
}

#include "ScanMonitorDlg.h"

void CSeismicSonde:: ReadScanData()
{
	int datano;
	int ReadDataNo[16];
	int failedchannelno;

	CScanMonitorDlg dlg;

	for(int ch = 0; ch<16; ch++)
	{
		ReadDataNo[ch] =0;
	}
	dlg.CreateModaless();
//	dlg.DoModal();

	do{
		failedchannelno = 0;
		for(int ch = 0; ch<16; ch++)
		{
			if(ReadDataNo[ch] != 6168)
			{
				datano = m_CurrentData[ch].m_DataNo;
				CSeismicData * tempData = m_ChData.GetAt(ch);
				for(int dindx = 0; dindx<datano; dindx++)
				{
					tempData->m_Data[dindx] = 0.0;
				}


				CString packet;
				packet.Format(_T("w 0x2%X2 0x01 "), ch);
				DirectWriteToPort((char *)packet.GetBuffer());

				int count = 0;
				int BytesRead = 0;

				do
				{
					if (count)
					{
						Sleep(500);
					}
					else
					{
						Sleep(1000);
					}
					BytesRead = DirectReadFromPort(&m_Packet[ReadDataNo[ch]]);
					ReadDataNo[ch] += BytesRead;
					int nPos = (int)(((float)ReadDataNo[ch]*100)/((float)(m_SpiritSPacket.m_HeaderSize + m_SpiritSPacket.m_DataSize)));
					dlg.UpdateStatus(ch, nPos, ReadDataNo[ch]);
					count++;
				}while(BytesRead);

				m_ReadBytes = ReadDataNo[ch];
				DirectPacketParsing();
				if(ReadDataNo[ch] != 6168)
				{
					failedchannelno++;
					ReadDataNo[ch] = 0;
				}
				Sleep(500);
			}
		}
	}while(failedchannelno >0);

	dlg.DestroyWindow();;
}

void CSeismicSonde:: ReadScanDataDlg()
{
	CScanMonitorDlg dlg;

	dlg.m_pSonde = this;

	dlg.DoModal();
	//dlg.CreateModaless();

	//dlg.ReadScanData();

	//dlg.DestroyWindow();
}
