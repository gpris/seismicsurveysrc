// RxControlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "RxControlDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CRxControlDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRxControlDlg, CDialogEx)

CRxControlDlg::CRxControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRxControlDlg::IDD, pParent)
	, m_UARTMode(0)
	, m_PRBSDelay(0)
	, m_PRBSRepeat(0)
{
	m_pSeismicRx = NULL;
	m_DeviceID = 0;
	m_StartCh = 0;
	m_ChannelNo = 1;
}

CRxControlDlg::~CRxControlDlg()
{
}

void CRxControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_RXCOMPORTCOMBO, m_RxComportComboCtrl);
	DDX_Control(pDX, ID_RXCONNECTBTN, m_RxConnectBtnCtrl);
	DDX_Control(pDX, IDC_RXTRIGGERCOMBO, m_RxTriggerComboCtrl);
	DDX_Control(pDX, IDC_RX1STAMPCOMBO, m_Rx1stAmpComboCtrl);
	DDX_Control(pDX, IDC_RX2NDAMPCOMBO, m_Rx2ndAmpComboCtrl);
	DDX_Control(pDX, IDC_SAMPLINGCLKCOMBO, m_SamplingClkComboCtrl);
	DDX_Control(pDX, IDC_RXPRBSFREQCOMBO, m_RxPRBSFreqComboCtrl);
	DDX_Control(pDX, IDC_RXPRBSLENGTHCOMBO, m_RxPRBSLengthComboCtrl);
	DDX_Control(pDX, IDC_TRIGGERAPPLYBTN, m_RxTriggerApplyBtnCtrl);
	DDX_Radio(pDX, IDC_UARTMODE1RADIO, m_UARTMode);
	DDX_Control(pDX, IDC_UARTMODE1RADIO, m_UARTModeRadio1BtnCtrl);
	DDX_Control(pDX, IDC_UARTMODE2RADIO, m_UARTModeRadio2BtnCtrl);
	DDX_Control(pDX, IDC_RXPRBSDELAY_EDIT, m_PRBSDelayEditCtrl);
	DDX_Text(pDX, IDC_RXPRBSDELAY_EDIT, m_PRBSDelay);
	DDX_Control(pDX, IDC_RXPRBSREPEAT_EDIT, m_PRBSRepeatEditCtrl);
	DDX_Text(pDX, IDC_RXPRBSREPEAT_EDIT, m_PRBSRepeat);
	DDX_Control(pDX, IDC_RXDELAYCOMBO, m_RxDelayComboCtrl);
	DDX_Control(pDX, IDC_RXDEVICECOMBO, m_RxDeviceComboCtrl);
	DDX_Control(pDX, IDC_RXDEVICEENABLE_CHECK, m_RxDeviceEnableCheckCtrl);
	DDX_Control(pDX, IDC_RXTRIGGERFREQCOMBO, m_RxTriggerFreqCtrl);
	DDX_Control(pDX, IDC_RXSTATUS_PROGRESS, m_RxStatusProgressCtrl);
	DDX_Control(pDX, IDC_RXSTATUS, m_RxStatusCtrl);
	DDX_Control(pDX, IDC_RXSTATUS_EDIT, m_RxStatusEditCtrl);
	DDX_Control(pDX, IDC_DATAREADBTN, m_DataReadBtnCtrl);
	DDX_Control(pDX, IDC_RXACCMULCOMBO, m_RxAccumComboCtrl);
}


BEGIN_MESSAGE_MAP(CRxControlDlg, CDialogEx)
	ON_BN_CLICKED(ID_RXCONNECTBTN, &CRxControlDlg::OnBnClickedRxConnectbtn)
	ON_BN_CLICKED(IDC_TRIGGERAPPLYBTN, &CRxControlDlg::OnBnClickedTriggerApplyBtn)
	ON_CBN_SELCHANGE(IDC_RXTRIGGERCOMBO, &CRxControlDlg::OnCbnSelchangeRxTriggerCombo)
	ON_CBN_SELCHANGE(IDC_RX1STAMPCOMBO, &CRxControlDlg::OnCbnSelchangeRx1stAmpCombo)
	ON_CBN_SELCHANGE(IDC_RX2NDAMPCOMBO, &CRxControlDlg::OnCbnSelchangeRx2ndAmpCombo)
	ON_CBN_SELCHANGE(IDC_SAMPLINGCLKCOMBO, &CRxControlDlg::OnCbnSelchangeSamplingClkCombo)
	ON_CBN_SELCHANGE(IDC_RXPRBSFREQCOMBO, &CRxControlDlg::OnCbnSelchangeRxPRBSFreqCombo)
	ON_CBN_SELCHANGE(IDC_RXPRBSLENGTHCOMBO, &CRxControlDlg::OnCbnSelchangeRxPRBSLengthCombo)
	ON_BN_CLICKED(IDC_UARTMODE1RADIO, &CRxControlDlg::OnBnClickedUartMode1Radio)
	ON_BN_CLICKED(IDC_UARTMODE2RADIO, &CRxControlDlg::OnBnClickedUartMode2Radio)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RXPRBSDELAY_SPIN, &CRxControlDlg::OnDeltaposRxprbsdelaySpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RXPRBSREPEAT_SPIN, &CRxControlDlg::OnDeltaposRxprbsrepeatSpin)
	ON_CBN_SELCHANGE(IDC_RXDEVICECOMBO, &CRxControlDlg::OnCbnSelchangeRxdevicecombo)
	ON_CBN_SELCHANGE(IDC_RXTRIGGERFREQCOMBO, &CRxControlDlg::OnCbnSelchangeRxtriggerfreqcombo)
	ON_CBN_SELCHANGE(IDC_RXDELAYCOMBO, &CRxControlDlg::OnCbnSelchangeRxdelaycombo)
	ON_BN_CLICKED(IDC_RXDEVICEENABLE_CHECK, &CRxControlDlg::OnBnClickedRxdeviceenableCheck)
	ON_BN_CLICKED(IDC_DATAREADBTN, &CRxControlDlg::OnBnClickedDatareadbtn)
	ON_EN_KILLFOCUS(IDC_RXPRBSDELAY_EDIT, &CRxControlDlg::OnEnKillfocusRxprbsdelayEdit)
	ON_CBN_SELCHANGE(IDC_RXACCMULCOMBO, &CRxControlDlg::OnCbnSelchangeRxAccmulcombo)
END_MESSAGE_MAP()


// CRxControlDlg 메시지 처리기입니다.


BOOL CRxControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	((CMainFrame *)AfxGetMainWnd())->GetComportInfoFromRegistry(&m_RxComportComboCtrl);
	RxControlEnable(false);

	m_RxDeviceComboCtrl.SetCurSel(m_DeviceID);
	m_RxTriggerFreqCtrl.SetCurSel(0);

	m_RxStatusProgressCtrl.SetRange(0, 100);
	m_RxStatusProgressCtrl.SetStep(1);
	m_RxStatusProgressCtrl.SetPos(0);

	m_DataReadBtnCtrl.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CRxControlDlg::OnBnClickedRxConnectbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	((CMainFrame *)AfxGetMainWnd())->RxPortConnect();
	CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	m_pSeismicRx = (CSeismicSonde *)&(pMainFrame->m_SeismicRx);

	if(m_pSeismicRx->m_bConnected)
	{
		m_pSeismicRx->ClosePort();

		m_pSeismicRx->m_bConnected = FALSE;

		m_RxConnectBtnCtrl.SetWindowTextA("연결");
		RxControlEnable(false);

		pMainFrame->m_pDoc = NULL;
	}
	else
	{
		CString tempName;
		m_RxComportComboCtrl.GetWindowTextA(tempName);
		tempName.Delete(0,3);
		int portNumber = atoi(tempName);
		if(m_pSeismicRx->InitPort(pMainFrame, portNumber,115200, 'n', 8, 1))
		{
			m_pSeismicRx->m_bConnected = TRUE;
			m_pSeismicRx->m_PortNumber = portNumber;
//			m_pSeismicRx->StartMonitoring();
			m_RxConnectBtnCtrl.SetWindowTextA("해제");
			RxControlEnable(true);

			CFrameWnd* pFrame = pMainFrame->MDIGetActive();
			pMainFrame->m_pDoc = (CSeismicSurveyDoc *)pFrame->GetActiveDocument();
		}
		else
		{
			AfxMessageBox("포트 연결에 실패하였습니다");
		}
	}
}


void CRxControlDlg::OnBnClickedTriggerApplyBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSeismicRx->StartCmdTriggerDlg();
//	m_pSeismicRx->StartCmdTrigger();
//	m_pSeismicRx->SetTriggerReady();
}

void CRxControlDlg::RxControlEnable(bool bEnable)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_RxTriggerComboCtrl.EnableWindow(bEnable);
	m_RxTriggerApplyBtnCtrl.EnableWindow(bEnable);
	m_Rx1stAmpComboCtrl.EnableWindow(bEnable);
	m_Rx2ndAmpComboCtrl.EnableWindow(bEnable);
	m_SamplingClkComboCtrl.EnableWindow(bEnable);
	m_RxPRBSFreqComboCtrl.EnableWindow(bEnable);
	m_RxPRBSLengthComboCtrl.EnableWindow(bEnable);
	m_UARTModeRadio1BtnCtrl.EnableWindow(bEnable);
	m_UARTModeRadio2BtnCtrl.EnableWindow(bEnable);

	m_PRBSDelayEditCtrl.EnableWindow(bEnable);
	m_PRBSRepeatEditCtrl.EnableWindow(bEnable);
	m_RxTriggerFreqCtrl.EnableWindow(bEnable);
	m_RxDelayComboCtrl.EnableWindow(bEnable);
	m_RxDeviceComboCtrl.EnableWindow(bEnable);
	m_RxDeviceEnableCheckCtrl.EnableWindow(bEnable);

	if(bEnable)
	{
		m_RxTriggerComboCtrl.SetCurSel(m_pSeismicRx->m_TriggerMode);
		m_RxPRBSFreqComboCtrl.SetCurSel(m_pSeismicRx->m_PRBSFreqID);
		m_RxPRBSLengthComboCtrl.SetCurSel(m_pSeismicRx->m_PRBSLengthID);
		RxChannelUpdate();
		//m_Rx1stAmpComboCtrl.SetCurSel(m_pSeismicRx->m_Rx1stAmpID[m_StartCh]);
		//m_Rx2ndAmpComboCtrl.SetCurSel(m_pSeismicRx->m_Rx2ndAmpID[m_StartCh]);
		//m_SamplingClkComboCtrl.SetCurSel(m_pSeismicRx->m_SamplingClkID[m_StartCh]);
		//m_UARTMode = m_pSeismicRx->m_SondeMode[m_StartCh];
		//UpdateData(false);
	}
}

void CRxControlDlg::OnCbnSelchangeRxTriggerCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSeismicRx->m_TriggerMode = m_RxTriggerComboCtrl.GetCurSel();

	m_pSeismicRx->SetTriggerMode(m_pSeismicRx->m_TriggerMode);
}


void CRxControlDlg::OnCbnSelchangeRx1stAmpCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int mode = m_Rx1stAmpComboCtrl.GetCurSel();
	for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
	{
		m_pSeismicRx->m_Rx1stAmpID[indx] = mode;
		m_pSeismicRx->SetVGASet(indx);
//		Sleep(50);
	}
}


void CRxControlDlg::OnCbnSelchangeRx2ndAmpCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int mode = m_Rx2ndAmpComboCtrl.GetCurSel();
	for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
	{
		m_pSeismicRx->m_Rx2ndAmpID[indx] = mode;
		m_pSeismicRx->SetVGASet(indx);
//		Sleep(50);
	}
}


void CRxControlDlg::OnCbnSelchangeSamplingClkCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int mode = m_SamplingClkComboCtrl.GetCurSel();

	for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
	{
		m_pSeismicRx->m_SamplingClkID[indx] = mode;
		m_pSeismicRx->SetADCClock(indx, mode);
//		Sleep(50);
	}
}

void CRxControlDlg::OnCbnSelchangeRxdelaycombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int mode = m_RxDelayComboCtrl.GetCurSel();
	for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
	{
		m_pSeismicRx->m_SamplingDelayID[indx] = mode;
		m_pSeismicRx->SetADCDelay(indx, mode);
//		Sleep(50);
	}
}

void CRxControlDlg::OnCbnSelchangeRxPRBSFreqCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSeismicRx->m_PRBSFreqID = m_RxPRBSFreqComboCtrl.GetCurSel();

	m_pSeismicRx->SetPRBSFreq(m_pSeismicRx->m_PRBSFreqID);
}


void CRxControlDlg::OnCbnSelchangeRxPRBSLengthCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSeismicRx->m_PRBSLengthID = m_RxPRBSLengthComboCtrl.GetCurSel();

	m_pSeismicRx->SetPRBSLength(m_pSeismicRx->m_PRBSLengthID);
}


void CRxControlDlg::OnBnClickedUartMode1Radio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_UARTMode = 0;

	for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
	{
		m_pSeismicRx->m_SondeMode[indx] = m_UARTMode;
		m_pSeismicRx->SetSondeMode(indx, m_UARTMode);
//		Sleep(50);
	}
}

void CRxControlDlg::OnBnClickedUartMode2Radio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_UARTMode = 1;

	for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
	{
		m_pSeismicRx->m_SondeMode[indx] = m_UARTMode;
		m_pSeismicRx->SetSondeMode(indx, m_UARTMode);
//		Sleep(50);
	}
}

void CRxControlDlg::OnDeltaposRxprbsdelaySpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMUpDown->iDelta > 0)
	{
		m_PRBSDelay--;
		if(m_PRBSDelay <0)
		{
			m_PRBSDelay = 0;
		}
	}
	else
	{
		m_PRBSDelay++;
		if(m_PRBSDelay >255)
		{
			m_PRBSDelay = 255;
		}
	}
	UpdateData(false);

	if(m_pSeismicRx != NULL && m_pSeismicRx->m_bConnected)
	{
		m_pSeismicRx->SetPRBSDelay(m_PRBSDelay);
	}

	*pResult = 0;
}


void CRxControlDlg::OnDeltaposRxprbsrepeatSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMUpDown->iDelta > 0)
	{
		m_PRBSRepeat--;
		if(m_PRBSRepeat <0)
		{
			m_PRBSRepeat = 0;
		}
	}
	else
	{
		m_PRBSRepeat++;
		if(m_PRBSRepeat >255)
		{
			m_PRBSRepeat = 255;
		}
	}
	UpdateData(false);

	if(m_pSeismicRx != NULL && m_pSeismicRx->m_bConnected)
	{
		m_pSeismicRx->SetPRBSRepeat(m_PRBSRepeat);
	}

	*pResult = 0;
}


void CRxControlDlg::OnCbnSelchangeRxdevicecombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_DeviceID = m_RxDeviceComboCtrl.GetCurSel();

	if(m_DeviceID)
	{
		m_StartCh = m_DeviceID - 1;
		m_ChannelNo = 1;
	}
	else
	{
		m_StartCh = 0;
		m_ChannelNo = m_pSeismicRx->m_ChanNo;
	}
	RxChannelUpdate();
}

void CRxControlDlg::RxChannelUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//switch(m_pSeismicRx->m_SondeMode[m_StartCh])
	//{
	//case 0:

	//	break;
	//case 1:
	//	break;
	//}
	m_Rx1stAmpComboCtrl.EnableWindow(m_pSeismicRx->m_Live[m_StartCh]);
	m_Rx2ndAmpComboCtrl.EnableWindow(m_pSeismicRx->m_Live[m_StartCh]);
	m_SamplingClkComboCtrl.EnableWindow(m_pSeismicRx->m_Live[m_StartCh]);
	m_RxDelayComboCtrl.EnableWindow(m_pSeismicRx->m_Live[m_StartCh]);

	if(m_pSeismicRx->m_Live[m_StartCh])
	{
		m_UARTMode = m_pSeismicRx->m_SondeMode[m_DeviceID - 1];
		UpdateData(false);
		m_Rx1stAmpComboCtrl.SetCurSel(m_pSeismicRx->m_Rx1stAmpID[m_StartCh]);
		m_Rx2ndAmpComboCtrl.SetCurSel(m_pSeismicRx->m_Rx2ndAmpID[m_StartCh]);
		m_SamplingClkComboCtrl.SetCurSel(m_pSeismicRx->m_SamplingClkID[m_StartCh]);
		m_RxDelayComboCtrl.SetCurSel(m_pSeismicRx->m_SamplingDelayID[m_StartCh]);
	}
	m_UARTMode = m_pSeismicRx->m_SondeMode[m_StartCh];
	UpdateData(false);

}

void CRxControlDlg::OnCbnSelchangeRxtriggerfreqcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSeismicRx->m_TriggerFreq = m_RxTriggerFreqCtrl.GetCurSel();

	m_pSeismicRx->SetTriggerFreq(m_pSeismicRx->m_TriggerFreq);
}




void CRxControlDlg::OnBnClickedRxdeviceenableCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bool CheckValue = m_RxDeviceEnableCheckCtrl.GetCheck();
	for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
	{
		m_pSeismicRx->m_Live[indx] = CheckValue;
	}
	RxChannelUpdate();
}


void CRxControlDlg::OnBnClickedDatareadbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_ChannelNo<16)
	{
		for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
		{
			if(!m_pSeismicRx->m_SondeMode[indx])
			{
				m_UARTMode = 1;
				UpdateData(false);
				m_pSeismicRx->m_SondeMode[indx] = m_UARTMode;
				m_pSeismicRx->SetSondeMode(indx, m_UARTMode);
				Sleep(100);
			}
			m_pSeismicRx->ReadADCData(indx);
			Sleep(100);		
		}
	}
	else
	{
		m_pSeismicRx->ReadScanDataDlg();
	}
}


void CRxControlDlg::OnEnKillfocusRxprbsdelayEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tempstring;
	m_PRBSDelayEditCtrl.GetWindowTextA(tempstring);
	m_PRBSDelay = atof(tempstring);

	if(m_PRBSRepeat <0)
	{
		m_PRBSRepeat = 0;
	}

	if(m_PRBSRepeat >255)
	{
		m_PRBSRepeat = 255;
	}

	UpdateData(false);

	if(m_pSeismicRx != NULL && m_pSeismicRx->m_bConnected)
	{
		m_pSeismicRx->SetPRBSDelay(m_PRBSDelay);
	}

}


void CRxControlDlg::OnCbnSelchangeRxAccmulcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int mode = m_RxAccumComboCtrl.GetCurSel();
	if(mode == 3) mode = 4;
	for(int indx = m_StartCh; indx<m_StartCh + m_ChannelNo; indx++)
	{
		m_pSeismicRx->m_AccumulID[indx] = mode;
		m_pSeismicRx->SetAccumulateNo(indx, mode);
//		Sleep(50);
	}
	m_pSeismicRx->m_TriggerNo = mode;
	m_pSeismicRx->SetTriggerNo(mode);

}
