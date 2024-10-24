// TxControlDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "TxControlDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CTxControlDlg ��ȭ �����Դϴ�.

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


// CTxControlDlg �޽��� ó�����Դϴ�.


BOOL CTxControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	((CMainFrame *)AfxGetMainWnd())->GetComportInfoFromRegistry(&m_TxComportComboCtrl);
	TxControlEnable(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTxControlDlg::OnBnClickedTxconnectbtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	((CMainFrame *)AfxGetMainWnd())->TxPortConnect();
	CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	m_pSeismicTx = (CSeismicSonde *)&(pMainFrame->m_SeismicTx);

	if(m_pSeismicTx->m_bConnected)
	{
		m_pSeismicTx->ClosePort();

		m_pSeismicTx->m_bConnected = FALSE;

		m_TxConnectBtnCtrl.SetWindowTextA("����");
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
			m_TxConnectBtnCtrl.SetWindowTextA("����");
			TxControlEnable(true);
		}
		else
		{
			AfxMessageBox("��Ʈ ���ῡ �����Ͽ����ϴ�");
		}
	}

}

void CTxControlDlg::TxControlEnable(bool bEnable)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pSeismicTx->m_PRBSFreqID = m_TxPRBSFreqComboCtrl.GetCurSel();

	m_pSeismicTx->SetPRBSFreq(m_pSeismicTx->m_PRBSFreqID);
}


void CTxControlDlg::OnCbnSelchangeTxPRBSLengthCombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pSeismicTx->m_PRBSLengthID = m_TxPRBSLengthComboCtrl.GetCurSel();

	m_pSeismicTx->SetPRBSLength(m_pSeismicTx->m_PRBSLengthID);
}


void CTxControlDlg::OnDeltaposPRBSDelaySpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
