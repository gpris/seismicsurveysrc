#pragma once
#include ".\serialport\serialport.h"

#include "SpiritSPacket.h"
#include "SeismicData.h"
#include "SignalProcessingParameter.h"

class CMainFrame;

class CSeismicSonde :	public CSerialPort
{
public:
	CSeismicSonde(void);
	~CSeismicSonde(void);
	int m_PortNumber;
	bool m_bConnected;

	enum {SONDE_DEBUGMODE, SONDE_ADCDATAMODE};
	int * m_SondeMode;

	enum {TRIG_INTERNALMODE, TRIG_EXTERNALMODE, TRIG_COMMANDMODE};
	int m_TriggerMode;

	int * m_Rx1stAmpID;
	int * m_Rx2ndAmpID;

	int * m_SamplingDelayID;

	enum {SAMPLING_100KHZ, SAMPLING_200KHZ, SAMPLING_50KHZ, SAMPLING_20KHZ, SAMPLING_10KHZ};
	int * m_SamplingClkID;
	int * m_Live;

	enum {PRBSFREQ_5KHZ, PRBSFREQ_10KHZ, PRBSFREQ_20KHZ};
	int m_PRBSFreqID;

	enum {PRBSLENGTH_4BIT, PRBSLENGTH_5BIT, PRBSLENGTH_6BIT, PRBSLENGTH_7BIT, PRBSLENGTH_8BIT};
	int m_PRBSLengthID;

	BYTE m_PRBSDelay;
	BYTE m_PRBSRepeat;
	
	enum {TRIG_FREQ_1, TRIG_FREQ_2, TRIG_FREQ_4, TRIG_FREQ_5, TRIG_FREQ_10};
	BYTE m_TriggerFreq;

	int m_RealTriggerFreq[5];

	void SetPRBSDelay(BYTE delay);
	void SetPRBSRepeat(BYTE repeat);

	void SetTriggerMode(int mode);
	void SetTriggerFreq(int mode);
	void SetSondeMode(int ch, int mode);
	void SetTriggerReady();

	BOOL StartCmdTrigger();
	BOOL StartCmdTriggerDlg();
	BOOL StartCmdTriggerDlgThread(CMainFrame * pMainWnd);

	void ReadADCDataLSB();
	void ReadADCDataMSB();

	void SetADCClock(int ch, int mode);
	void SetADCDelay(int ch, int mode);
	void SetVGASet(int ch);

	void SetPRBSFreq(int mode);
	void SetPRBSLength(int mode);

	void SetAccumulateNo(int ch, int mode);
	void SetTriggerNo(int mode);

	CSpiritSPacket m_SpiritSPacket;
	CSpiritSData * m_CurrentData;
	CArray <CSeismicData *, CSeismicData *> m_ChData;

	bool PacketParsing(BYTE achar);
	bool DirectPacketParsing();

	int m_ChanNo;
	int m_DataNo;
	float m_dt;
	void ReadADCData(int ch);
	void ReadMessage();

	void ReadScanData();
	void ReadScanDataDlg();

	char * m_Packet;
	char * m_Mesg;
	int m_ReadBytes;

	int m_SelectedChannel;

	int * m_AccumulID;
	int m_TriggerNo;

	float m_TxDepthOffset;
	float m_RxDepthOffset;

	float m_TxDepth;
	float m_RxDepth;

	bool m_bCmdTriggerTerminated;



	CSignalProcessingParameter m_SigParameter;
};

