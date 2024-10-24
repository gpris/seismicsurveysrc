// SurveyCtrlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "SurveyCtrlDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CSurveyCtrlDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSurveyCtrlDlg, CDialogEx)

CSurveyCtrlDlg::CSurveyCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSurveyCtrlDlg::IDD, pParent)
{

}

CSurveyCtrlDlg::~CSurveyCtrlDlg()
{
}

void CSurveyCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SURVEYSTART_BTN, m_ScanStartBtnCtrl);
	DDX_Control(pDX, IDC_SITENAMEEDIT, m_SiteNameEditCtrl);
	DDX_Control(pDX, IDC_SUBSITENAMEEDIT, m_SubSiteNameEditCtrl);
	DDX_Control(pDX, IDC_TXHOLE_EDIT, m_TxHoleEditCtrl);
	DDX_Control(pDX, IDC_RXHOLE_EDIT, m_RxHoleEditCtrl);
	DDX_Control(pDX, IDC_STARTDEPTH_EDIT, m_StartDepthEditCtrl);
	DDX_Control(pDX, IDC_TARGETDEPTH_EDIT, m_EndDepthEditCtrl);
	DDX_Control(pDX, IDC_SURVEYVEL_EDIT, m_ScanVelEditCtrl);
	DDX_Control(pDX, IDC_SURVEYMETHOD_COMBO, m_SurveyMethodComboCtrl);
	DDX_Control(pDX, IDC_REALTX_EDIT, m_RealTxEditCtrl);
	DDX_Control(pDX, IDC_REALRX_EDIT0, m_RealRxEditCtrl[0]);
	DDX_Control(pDX, IDC_REALRX_EDIT1, m_RealRxEditCtrl[1]);
	DDX_Control(pDX, IDC_REALRX_EDIT2, m_RealRxEditCtrl[2]);
	DDX_Control(pDX, IDC_REALRX_EDIT3, m_RealRxEditCtrl[3]);
	DDX_Control(pDX, IDC_REALRX_EDIT4, m_RealRxEditCtrl[4]);
	DDX_Control(pDX, IDC_REALRX_EDIT5, m_RealRxEditCtrl[5]);
	DDX_Control(pDX, IDC_REALRX_EDIT6, m_RealRxEditCtrl[6]);
	DDX_Control(pDX, IDC_REALRX_EDIT7, m_RealRxEditCtrl[7]);
	DDX_Control(pDX, IDC_REALRX_EDIT8, m_RealRxEditCtrl[8]);
	DDX_Control(pDX, IDC_REALRX_EDIT9, m_RealRxEditCtrl[9]);
	DDX_Control(pDX, IDC_REALRX_EDIT10, m_RealRxEditCtrl[10]);
	DDX_Control(pDX, IDC_REALRX_EDIT11, m_RealRxEditCtrl[11]);
	DDX_Control(pDX, IDC_REALRX_EDIT12, m_RealRxEditCtrl[12]);
	DDX_Control(pDX, IDC_REALRX_EDIT13, m_RealRxEditCtrl[13]);
	DDX_Control(pDX, IDC_REALRX_EDIT14, m_RealRxEditCtrl[14]);
	DDX_Control(pDX, IDC_REALRX_EDIT15, m_RealRxEditCtrl[15]);
}


BEGIN_MESSAGE_MAP(CSurveyCtrlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SURVEYSTART_BTN, &CSurveyCtrlDlg::OnBnClickedSurveystartBtn)
	ON_BN_CLICKED(IDC_SURVEYINIT_BTN, &CSurveyCtrlDlg::OnBnClickedSurveyinitBtn)
	ON_CBN_SELCHANGE(IDC_SURVEYMETHOD_COMBO, &CSurveyCtrlDlg::OnCbnSelchangeSurveymethodCombo)
END_MESSAGE_MAP()


// CSurveyCtrlDlg 메시지 처리기입니다.


void CSurveyCtrlDlg::OnBnClickedSurveystartBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	char tempbuffer[200];

	if(pMainFrame->m_bScanActivated )
	{
		pMainFrame->BScanTerminated();
		m_ScanStartBtnCtrl.SetWindowTextA(_T("탐사시작"));
	}
	else
	{
		m_SiteNameEditCtrl.GetWindowTextA(tempbuffer, 200);
		pMainFrame->m_ScanInfo.m_SiteName.Format("%s", tempbuffer);

		m_SubSiteNameEditCtrl.GetWindowTextA(tempbuffer, 200);
		pMainFrame->m_ScanInfo.m_SubSiteName.Format("%s", tempbuffer);

		m_TxHoleEditCtrl.GetWindowTextA(tempbuffer, 200);
		pMainFrame->m_ScanInfo.m_TxHoleName.Format("%s", tempbuffer);

		m_RxHoleEditCtrl.GetWindowTextA(tempbuffer, 200);
		pMainFrame->m_ScanInfo.m_RxHoleName.Format("%s", tempbuffer);

		pMainFrame->m_ScanInfo.m_MeasTime.GetTime();

		m_StartDepthEditCtrl.GetWindowTextA(tempbuffer, 200);
		pMainFrame->m_ScanInfo.m_StartDepth = atof(tempbuffer);

		m_EndDepthEditCtrl.GetWindowTextA(tempbuffer, 200);
		pMainFrame->m_ScanInfo.m_StopDepth = atof(tempbuffer);

		m_ScanVelEditCtrl.GetWindowTextA(tempbuffer, 200);
		pMainFrame->m_ScanInfo.m_ScanVelocity = atof(tempbuffer);

		m_SurveyMethodComboCtrl.GetWindowTextA(tempbuffer, 200);
		pMainFrame->m_ScanInfo.m_ScanStep = atof(tempbuffer);

		if(pMainFrame->m_ScanInfo.m_StopDepth < pMainFrame->m_ScanInfo.m_StartDepth)
			pMainFrame->m_ScanInfo.m_ScanDirection = pMainFrame->m_ScanInfo.UP_DIRECTION;
		else
			pMainFrame->m_ScanInfo.m_ScanDirection =  pMainFrame->m_ScanInfo.DOWN_DIRECTION;

		pMainFrame->BScanStart();
		m_ScanStartBtnCtrl.SetWindowTextA(_T("탐사중지"));
	}

}


void CSurveyCtrlDlg::OnBnClickedSurveyinitBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());

	pMainFrame->BScanInitialize();
}


void CSurveyCtrlDlg::OnCbnSelchangeSurveymethodCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}

void CSurveyCtrlDlg::UpdateAntennaPosition()
{
	CMainFrame * pMainFrame = ((CMainFrame *)AfxGetMainWnd());
	CString tempstring;

	tempstring.Format("%.2f", pMainFrame->m_SeismicRx.m_TxDepth + pMainFrame->m_SeismicRx.m_TxDepthOffset);
	m_RealTxEditCtrl.SetWindowTextA(tempstring);

	for(int channo = 0; channo<16; channo++)
	{
		tempstring.Format("%.2f", pMainFrame->m_SeismicRx.m_RxDepth + pMainFrame->m_SeismicRx.m_TxDepthOffset + (channo - 7) * 0.5);
		m_RealRxEditCtrl[channo].SetWindowTextA(tempstring);
	}

}