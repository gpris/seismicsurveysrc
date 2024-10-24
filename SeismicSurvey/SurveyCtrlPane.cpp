// SurveyCtrlPane.cpp : ���� �����Դϴ�.
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



// CSurveyCtrlPane �޽��� ó�����Դϴ�.




int CSurveyCtrlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	BOOL bRet = m_SurveyCtrlDlg.Create(IDD_SURVEYCTRLDLG, this);
	ASSERT(bRet);
	m_SurveyCtrlDlg.ShowWindow(SW_SHOW);

	return 0;
}


void CSurveyCtrlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_SurveyCtrlDlg.MoveWindow(0,0, cx, cy);
}
