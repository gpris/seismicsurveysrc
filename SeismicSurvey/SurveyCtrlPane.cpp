// SurveyCtrlPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "SurveyCtrlPane.h"


// CSurveyCtrlPane

IMPLEMENT_DYNAMIC(CSurveyCtrlPane, CDockablePane)

CSurveyCtrlPane::CSurveyCtrlPane()
{

}

CSurveyCtrlPane::~CSurveyCtrlPane()
{
}


BEGIN_MESSAGE_MAP(CSurveyCtrlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CSurveyCtrlPane 메시지 처리기입니다.




int CSurveyCtrlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	BOOL bRet = m_SurveyCtrlDlg.Create(IDD_SURVEYCTRLDLG, this);
	ASSERT(bRet);
	m_SurveyCtrlDlg.ShowWindow(SW_SHOW);

	return 0;
}


void CSurveyCtrlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_SurveyCtrlDlg.MoveWindow(0,0, cx, cy);
}
