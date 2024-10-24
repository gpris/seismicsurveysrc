// TxControlPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "TxControlPane.h"


// CTxControlPane

IMPLEMENT_DYNAMIC(CTxControlPane, CDockablePane)

CTxControlPane::CTxControlPane()
{

}

CTxControlPane::~CTxControlPane()
{
}


BEGIN_MESSAGE_MAP(CTxControlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CTxControlPane 메시지 처리기입니다.




int CTxControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	BOOL bRet = m_TxControlDlg.Create(IDD_TXCONTROLDLG, this);
	ASSERT(bRet);
	m_TxControlDlg.ShowWindow(SW_SHOW);

	return 0;
}


void CTxControlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//2. m_WinchCtrlDlg의 크기가 윈도우 크기에 맞춰 변하도록 지정
	m_TxControlDlg.MoveWindow(0,0, cx, cy);
}
