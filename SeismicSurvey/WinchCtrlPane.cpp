// WinchCtrlPane.cpp : implementation file
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "WinchCtrlPane.h"


// CWinchCtrlPane

IMPLEMENT_DYNAMIC(CWinchCtrlPane, CDockablePane)

CWinchCtrlPane::CWinchCtrlPane()
{

}

CWinchCtrlPane::~CWinchCtrlPane()
{
}


BEGIN_MESSAGE_MAP(CWinchCtrlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CWinchCtrlPane message handlers



int CWinchCtrlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
//1. m_WinchCtrlDlg을 보이도록 설정
	BOOL bRet = m_WinchCtrlDlg.Create(IDD_WINCHCTRLDLG, this);
	ASSERT(bRet);
	m_WinchCtrlDlg.ShowWindow(SW_SHOW);


	return 0;
}

void CWinchCtrlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
//2. m_WinchCtrlDlg의 크기가 윈도우 크기에 맞춰 변하도록 지정
	m_WinchCtrlDlg.MoveWindow(0,0, cx, cy);
}
