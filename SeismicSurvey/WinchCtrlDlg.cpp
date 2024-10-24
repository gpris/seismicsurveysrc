// WinchCtrlDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "MainFrm.h"
#include "WinchCtrlDlg.h"
#include "afxdialogex.h"


// CWinchCtrlDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWinchCtrlDlg, CDialogEx)

CWinchCtrlDlg::CWinchCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinchCtrlDlg::IDD, pParent)
{
	m_pWinch = NULL;
}

CWinchCtrlDlg::~CWinchCtrlDlg()
{
}

void CWinchCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINCHPORTCOMBO, m_WinchPortComboCtrl);
	DDX_Control(pDX, IDC_WINCHCONNECTBTN, m_WinchConnectBtnCtrl);
	DDX_Control(pDX, IDC_TXDEPTH_EDIT, m_WinchTxDepthEditCtrl);
	DDX_Control(pDX, IDC_RXDEPTH_EDIT, m_WinchRxDepthEditCtrl);
	DDX_Control(pDX, IDC_WINCHMOVE_BTN, m_WinchMoveBtnCtrl);
	DDX_Control(pDX, IDC_WINCHSTOP_BTN, m_WinchStopBtnCtrl);
	DDX_Control(pDX, IDC_WINCHDEPTHINIT_BTN, m_WinchDepthInitBtnCtrl);
}


BEGIN_MESSAGE_MAP(CWinchCtrlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_WINCHMOVE_BTN, &CWinchCtrlDlg::OnBnClickedWinchMoveBtn)
	ON_BN_CLICKED(IDC_WINCHSTOP_BTN, &CWinchCtrlDlg::OnBnClickedWinchStopBtn)
	ON_BN_CLICKED(IDC_WINCHDEPTHINIT_BTN, &CWinchCtrlDlg::OnBnClickedWinchDepthInitBtn)
	ON_BN_CLICKED(IDC_WINCHCONNECTBTN, &CWinchCtrlDlg::OnBnClickedWinchConnectBtn)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWinchCtrlDlg �޽��� ó�����Դϴ�.

BOOL CWinchCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
//swkim> CGCColorStaticŬ���� �̿��� m_WinchDepthEditCtrl ȭ�� ����
	m_WinchTxDepthEditCtrl.SetOn(FALSE);
	m_WinchTxDepthEditCtrl.SetTextOffColor(RGB(0, 180, 255));
	m_WinchTxDepthEditCtrl.SetBackgroundOffColor(RGB(0, 0, 50));
	m_WinchTxDepthEditCtrl.SetBold();
	m_WinchTxDepthEditCtrl.SetPointFont(20, "Arial");
	m_WinchTxDepthEditCtrl.SetWindowTextA(_T(""));

	m_WinchRxDepthEditCtrl.SetOn(FALSE);
	m_WinchRxDepthEditCtrl.SetTextOffColor(RGB(0, 180, 255));
	m_WinchRxDepthEditCtrl.SetBackgroundOffColor(RGB(0, 0, 50));
	m_WinchRxDepthEditCtrl.SetBold();
	m_WinchRxDepthEditCtrl.SetPointFont(20, "Arial");
	m_WinchRxDepthEditCtrl.SetWindowTextA(_T(""));

//swkim> Get Comport information from Registry to fill up combo box
	CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	pMainFrame->GetComportInfoFromRegistry(&m_WinchPortComboCtrl);

	WinchControlEnable(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CWinchCtrlDlg::OnBnClickedWinchConnectBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	((CMainFrame *)AfxGetMainWnd())->WinchPortConnect();
	CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	m_pWinch = (CAuslogWinch *)&(pMainFrame->m_Winch);

	if(m_pWinch->m_bConnected)
	{
		pMainFrame->KillTimer(222);
		m_pWinch->ClosePort();

		m_pWinch->m_bConnected = FALSE;

		m_WinchConnectBtnCtrl.SetWindowTextA("����");
		WinchControlEnable(false);
	}
	else
	{
		CString tempName;
		m_WinchPortComboCtrl.GetWindowTextA(tempName);
		tempName.Delete(0,3);
		int portNumber = atoi(tempName);
		if(m_pWinch->InitPort(pMainFrame, portNumber,115200, 'n', 8, 1))
		{
			m_pWinch->m_bConnected = TRUE;
			m_pWinch->m_PortNumber = portNumber;
			m_pWinch->StartMonitoring();
			m_WinchConnectBtnCtrl.SetWindowTextA("����");
//			m_pWinch->DirectWriteToPort(_T("r 0x600 "));
			WinchDepthUpdate();
			WinchControlEnable(true);
			pMainFrame->SetTimer(222, 100, NULL);

		}
		else
		{
			AfxMessageBox("��Ʈ ���ῡ �����Ͽ����ϴ�");
		}
	}

}

void CWinchCtrlDlg::WinchControlEnable(bool bEnable)
{
	m_WinchMoveBtnCtrl.EnableWindow(bEnable);
	m_WinchStopBtnCtrl.EnableWindow(bEnable);
	m_WinchDepthInitBtnCtrl.EnableWindow(bEnable);
}
#include "WinchMoveDlg.h"

void CWinchCtrlDlg::OnBnClickedWinchMoveBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CWinchMoveDlg dlg;

	//CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	//CAuslogWinch * pWinch = &pMainFrame->m_Winch;

	dlg.m_TxTargetDepth = m_pWinch->m_TxDepth;
	dlg.m_RxTargetDepth = m_pWinch->m_RxDepth;
	dlg.m_TxMoveVel = 10;
	dlg.m_RxMoveVel = 10;

	if(dlg.DoModal() == IDOK)
	{
		float TxTargetDepth = dlg.m_TxTargetDepth;
		float RxTargetDepth = dlg.m_RxTargetDepth;
		float TxMoveVel = dlg.m_TxMoveVel;
		float RxMoveVel = dlg.m_RxMoveVel;
		m_pWinch->IndividualMove(TxMoveVel, TxTargetDepth, 
								RxMoveVel, RxTargetDepth, 
								0, 0, 0);
	}
}

void CWinchCtrlDlg::OnBnClickedWinchStopBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	//pMainFrame->m_Winch.StopPacket();
	m_pWinch->StopPacket();
}

#include "SetDepthDlg.h"

void CWinchCtrlDlg::OnBnClickedWinchDepthInitBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CSetDepthDlg dlg;

	dlg.m_TxCurDepth = 0;
	dlg.m_RxCurDepth = 0;

	if(dlg.DoModal() == IDOK)
	{
		float TxCurDepth = dlg.m_TxCurDepth;
		float RxCurDepth = dlg.m_RxCurDepth;

		m_pWinch->SetDepth(TxCurDepth, RxCurDepth, 0);
	}
}

void CWinchCtrlDlg::WinchDepthUpdate()
{
//	CMainFrame * pMainFrame;
//	pMainFrame = (CMainFrame *)AfxGetMainWnd();

	CString tempString;
	float TxDepth = m_pWinch->m_TxDepth;
	if(TxDepth<0)
	{
		tempString.Format("%+07.2f m", m_pWinch->m_TxDepth);
		m_WinchTxDepthEditCtrl.SetWindowTextA(tempString);
		tempString.Format("%+07.2f m", m_pWinch->m_RxDepth);
		m_WinchRxDepthEditCtrl.SetWindowTextA(tempString);
	}
	else
	{
		tempString.Format(" %06.2f m", m_pWinch->m_TxDepth);
		m_WinchTxDepthEditCtrl.SetWindowTextA(tempString);
		tempString.Format(" %06.2f m", m_pWinch->m_RxDepth);
		m_WinchRxDepthEditCtrl.SetWindowTextA(tempString);
	}

}

void CWinchCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch(nIDEvent)
	{
	case 222:
		WinchDepthUpdate();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
