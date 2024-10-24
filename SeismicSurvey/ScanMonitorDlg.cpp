// ScanMonitorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "ScanMonitorDlg.h"
#include "afxdialogex.h"

#include "SeismicSonde.h"

// CScanMonitorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CScanMonitorDlg, CDialogEx)

CScanMonitorDlg::CScanMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScanMonitorDlg::IDD, pParent)
{
	m_pSonde = NULL;
	for(int ch = 0; ch<16; ch++)
	{
		m_ReadDataNo[ch] =0;
	}

	m_Count = 0;
	m_SuccessChannelNo = 0;

	m_CurrentCh = 0;

	m_bIsReadMode = false;
}

CScanMonitorDlg::~CScanMonitorDlg()
{
}

void CScanMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CH0, m_ProgressCtrlCH[0]);
	DDX_Control(pDX, IDC_PROGRESS_CH1, m_ProgressCtrlCH[1]);
	DDX_Control(pDX, IDC_PROGRESS_CH2, m_ProgressCtrlCH[2]);
	DDX_Control(pDX, IDC_PROGRESS_CH3, m_ProgressCtrlCH[3]);
	DDX_Control(pDX, IDC_PROGRESS_CH4, m_ProgressCtrlCH[4]);
	DDX_Control(pDX, IDC_PROGRESS_CH5, m_ProgressCtrlCH[5]);
	DDX_Control(pDX, IDC_PROGRESS_CH6, m_ProgressCtrlCH[6]);
	DDX_Control(pDX, IDC_PROGRESS_CH7, m_ProgressCtrlCH[7]);
	DDX_Control(pDX, IDC_PROGRESS_CH8, m_ProgressCtrlCH[8]);
	DDX_Control(pDX, IDC_PROGRESS_CH9, m_ProgressCtrlCH[9]);
	DDX_Control(pDX, IDC_PROGRESS_CH10, m_ProgressCtrlCH[10]);
	DDX_Control(pDX, IDC_PROGRESS_CH11, m_ProgressCtrlCH[11]);
	DDX_Control(pDX, IDC_PROGRESS_CH12, m_ProgressCtrlCH[12]);
	DDX_Control(pDX, IDC_PROGRESS_CH13, m_ProgressCtrlCH[13]);
	DDX_Control(pDX, IDC_PROGRESS_CH14, m_ProgressCtrlCH[14]);
	DDX_Control(pDX, IDC_PROGRESS_CH15, m_ProgressCtrlCH[15]);
	DDX_Control(pDX, IDC_EDIT_CH0, m_PercentEditCtrl[0]);
	DDX_Control(pDX, IDC_EDIT_CH1, m_PercentEditCtrl[1]);
	DDX_Control(pDX, IDC_EDIT_CH2, m_PercentEditCtrl[2]);
	DDX_Control(pDX, IDC_EDIT_CH3, m_PercentEditCtrl[3]);
	DDX_Control(pDX, IDC_EDIT_CH4, m_PercentEditCtrl[4]);
	DDX_Control(pDX, IDC_EDIT_CH5, m_PercentEditCtrl[5]);
	DDX_Control(pDX, IDC_EDIT_CH6, m_PercentEditCtrl[6]);
	DDX_Control(pDX, IDC_EDIT_CH7, m_PercentEditCtrl[7]);
	DDX_Control(pDX, IDC_EDIT_CH8, m_PercentEditCtrl[8]);
	DDX_Control(pDX, IDC_EDIT_CH9, m_PercentEditCtrl[9]);
	DDX_Control(pDX, IDC_EDIT_CH10, m_PercentEditCtrl[10]);
	DDX_Control(pDX, IDC_EDIT_CH11, m_PercentEditCtrl[11]);
	DDX_Control(pDX, IDC_EDIT_CH12, m_PercentEditCtrl[12]);
	DDX_Control(pDX, IDC_EDIT_CH13, m_PercentEditCtrl[13]);
	DDX_Control(pDX, IDC_EDIT_CH14, m_PercentEditCtrl[14]);
	DDX_Control(pDX, IDC_EDIT_CH15, m_PercentEditCtrl[15]);
}


BEGIN_MESSAGE_MAP(CScanMonitorDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CScanMonitorDlg 메시지 처리기입니다.

BOOL CScanMonitorDlg::CreateModaless()
{
	BOOL res = CDialog::Create(IDD_SCANMONITORDLG, NULL);

	return res;
}

void CScanMonitorDlg::UpdateStatus(int ch, int npos, int nBytes)
{
	m_ProgressCtrlCH[ch].SetPos(npos);
	CString tempstring;
	tempstring.Format("%d", nBytes);
	m_PercentEditCtrl[ch].SetWindowTextA(tempstring);
	UpdateWindow();
}

BOOL CScanMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(m_pSonde)
	{
//		ReadScanData();
		SetTimer(333, 100, NULL);
	}
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CScanMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 333:
		if(m_bIsReadMode)
		{
			ReadScanData2();
		}
		else
		{
			WriteReadCommand();
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CScanMonitorDlg::ReadScanData2()
{
	int BytesRead = m_pSonde->DirectReadFromPort(&m_pSonde->m_Packet[m_ReadDataNo[m_CurrentCh]]);
	m_ReadDataNo[m_CurrentCh] += BytesRead;
	int nPos = (int)(((float)m_ReadDataNo[m_CurrentCh]*100)/((float)(m_pSonde->m_SpiritSPacket.m_HeaderSize 
													+ m_pSonde->m_SpiritSPacket.m_DataSize)));
	UpdateStatus(m_CurrentCh, nPos, m_ReadDataNo[m_CurrentCh]);
	if(BytesRead)
	{
		m_Count = 0;
		if(m_ReadDataNo[m_CurrentCh]<6168)
		{
		}
		else
		{
			m_pSonde->m_ReadBytes = m_ReadDataNo[m_CurrentCh];
			if(m_pSonde->DirectPacketParsing())
			{
				m_SuccessChannelNo++;
				if(m_SuccessChannelNo<16)
				{
					m_CurrentCh = (m_CurrentCh+1)%16;
					m_bIsReadMode = false;
				}
				else
				{
					OnOK();
				}
			}
			else
			{
				m_ReadDataNo[m_CurrentCh] = 0;
			}
		}
	}
	else
	{
		m_Count++;
		if(m_Count>10)
		{
			m_CurrentCh = (m_CurrentCh+1)%16;
			m_bIsReadMode = false;
		}
	}
}

void CScanMonitorDlg::WriteReadCommand()
{
	if(m_ReadDataNo[m_CurrentCh] != 6168)
	{
		int datano = m_pSonde->m_CurrentData[m_CurrentCh].m_DataNo;

		CSeismicData * tempData = m_pSonde->m_ChData.GetAt(m_CurrentCh);
		for(int dindx = 0; dindx<datano; dindx++)
		{
			tempData->m_Data[dindx] = 0.0;
		}

		CString packet;
		packet.Format(_T("w 0x2%X2 0x01 "), m_CurrentCh);
		m_pSonde->DirectWriteToPort((char *)packet.GetBuffer());
		m_bIsReadMode = true;
	}
	else
	{
		m_CurrentCh = (m_CurrentCh+1)%16;
	}
}

INT_PTR CScanMonitorDlg::DoModal()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DoModal();
}

void CScanMonitorDlg::ReadScanData()
{
	int datano;
	int ReadDataNo[16];
	int failedchannelno;

	for(int ch = 0; ch<16; ch++)
	{
		ReadDataNo[ch] =0;
	}

	do{
		failedchannelno = 0;
		for(int ch = 0; ch<16; ch++)
		{
			if(ReadDataNo[ch] != 6168)
			{
				datano = m_pSonde->m_CurrentData[ch].m_DataNo;
				CSeismicData * tempData = m_pSonde->m_ChData.GetAt(ch);
				for(int dindx = 0; dindx<datano; dindx++)
				{
					tempData->m_Data[dindx] = 0.0;
				}


				CString packet;
				packet.Format(_T("w 0x2%X2 0x01 "), ch);
				m_pSonde->DirectWriteToPort((char *)packet.GetBuffer());

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
					BytesRead = m_pSonde->DirectReadFromPort(&m_pSonde->m_Packet[ReadDataNo[ch]]);
					ReadDataNo[ch] += BytesRead;
					int nPos = (int)(((float)ReadDataNo[ch]*100)/((float)(m_pSonde->m_SpiritSPacket.m_HeaderSize 
																	+ m_pSonde->m_SpiritSPacket.m_DataSize)));
					UpdateStatus(ch, nPos, ReadDataNo[ch]);
					count++;
				}while(BytesRead);

				m_pSonde->m_ReadBytes = ReadDataNo[ch];
				m_pSonde->DirectPacketParsing();
				if(ReadDataNo[ch] != 6168)
				{
					failedchannelno++;
					ReadDataNo[ch] = 0;
				}
				Sleep(500);
			}
		}
	}while(failedchannelno >0);
}


void CScanMonitorDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	KillTimer(333);

	CDialogEx::OnOK();
}
