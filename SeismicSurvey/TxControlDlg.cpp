// TxControlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "TxControlDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CTxControlDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTxControlDlg, CDialogEx)

CTxControlDlg::CTxControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTxControlDlg::IDD, pParent)
	, m_PRBSDelay(0)
	, m_PRBSRepeat(0)
{
	m_pSeismicTx = NULL;
}

CTxControlDlg::~CTxControlDlg()
{
}

void CTxControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_TXCOMPORTCOMBO, m_TxComportComboCtrl);
	DDX_Control(pDX, ID_TXCONNECTBTN, m_TxConnectBtnCtrl);
	DDX_Control(pDX, IDC_TXPRBSFREQCOMBO, m_TxPRBSFreqComboCtrl);
	DDX_Control(pDX, IDC_TXPRBSLENGTHCOMBO, m_TxPRBSLengthComboCtrl);
	DDX_Control(pDX, IDC_PRBSDELAY_EDIT, m_PRBSDelayEditCtrl);
	DDX_Text(pDX, IDC_PRBSDELAY_EDIT, m_PRBSDelay);
	DDX_Control(pDX, IDC_PRBSREPEAT_EDIT, m_PRBSRepeatEditCtrl);
	DDX_Text(pDX, IDC_PRBSREPEAT_EDIT, m_PRBSRepeat);
}


BEGIN_MESSAGE_MAP(CTxControlDlg, CDialogEx)
	ON_BN_CLICKED(ID_TXCONNECTBTN, &CTxControlDlg::OnBnClickedTxconnectbtn)
	ON_CBN_SELCHANGE(IDC_TXPRBSFREQCOMBO, &CTxControlDlg::OnCbnSelchangeTxPRBSFreqCombo)
	ON_CBN_SELCHANGE(IDC_TXPRBSLENGTHCOMBO, &CTxControlDlg::OnCbnSelchangeTxPRBSLengthCombo)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PRBSDELAY_SPIN, &CTxControlDlg::OnDeltaposPRBSDelaySpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PRBSREPEAT_SPIN, &CTxControlDlg::OnDeltaposPRBSRepeatSpin)
END_MESSAGE_MAP()


// CTxControlDlg 메시지 처리기입니다.


BOOL CTxControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	((CMainFrame *)AfxGetMainWnd())->GetComportInfoFromRegistry(&m_TxComportComboCtrl);
	TxControlEnable(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTxControlDlg::OnBnClickedTxconnectbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	((CMainFrame *)AfxGetMainWnd())->TxPortConnect();
	CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	m_pSeismicTx = (CSeismicSonde *)&(pMainFrame->m_SeismicTx);

	if(m_pSeismicTx->m_bConnected)
	{
		m_pSeismicTx->ClosePort();

		m_pSeismicTx->m_bConnected = FALSE;

		m_TxConnectBtnCtrl.SetWindowTextA("연결");
		TxControlEnable(false);

		pMainFrame->m_pDoc = NULL;
	}
	else
	{
		CString tempName;
		m_TxComportComboCtrl.GetWindowTextA(tempName);
		tempName.Delete(0,3);
		int portNumber = atoi(tempName);
		if(m_pSeismicTx->InitPort(pMainFrame, portNumber,115200, 'n', 8, 1))
		{
			m_pSeismicTx->m_bConnected = TRUE;
			m_pSeismicTx->m_PortNumber = portNumber;
			m_pSeismicTx->StartMonitoring();
			m_TxConnectBtnCtrl.SetWindowTextA("해제");
			TxControlEnable(true);
		}
		else
		{
			AfxMessageBox("포트 연결에 실패하였습니다");
		}
	}

}

void CTxControlDlg::TxControlEnable(bool bEnable)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_TxPRBSFreqComboCtrl.EnableWindow(bEnable);
	m_TxPRBSLengthComboCtrl.EnableWindow(bEnable);
	m_PRBSDelayEditCtrl.EnableWindow(bEnable);
	m_PRBSRepeatEditCtrl.EnableWindow(bEnable);

	if(bEnable)
	{
		m_TxPRBSFreqComboCtrl.SetCurSel(m_pSeismicTx->m_PRBSFreqID);
		m_TxPRBSLengthComboCtrl.SetCurSel(m_pSeismicTx->m_PRBSLengthID);
	}
}


void CTxControlDlg::OnCbnSelchangeTxPRBSFreqCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSeismicTx->m_PRBSFreqID = m_TxPRBSFreqComboCtrl.GetCurSel();

	m_pSeismicTx->SetPRBSFreq(m_pSeismicTx->m_PRBSFreqID);
}


void CTxControlDlg::OnCbnSelchangeTxPRBSLengthCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSeismicTx->m_PRBSLengthID = m_TxPRBSLengthComboCtrl.GetCurSel();

	m_pSeismicTx->SetPRBSLength(m_pSeismicTx->m_PRBSLengthID);
}


void CTxControlDlg::OnDeltaposPRBSDelaySpin(NMHDR *pNMHDR, LRESULT *pResult)
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

	if(m_pSeismicTx != NULL && m_pSeismicTx->m_bConnected)
	{
		m_pSeismicTx->SetPRBSDelay(m_PRBSDelay);
	}

	*pResult = 0;
}

void CTxControlDlg::OnDeltaposPRBSRepeatSpin(NMHDR *pNMHDR, LRESULT *pResult)
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

	if(m_pSeismicTx != NULL && m_pSeismicTx->m_bConnected)
	{
		m_pSeismicTx->SetPRBSRepeat(m_PRBSRepeat);
	}
	*pResult = 0;
}
