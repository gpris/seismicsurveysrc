// RxControlPane.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "RxControlPane.h"


// CRxControlPane

IMPLEMENT_DYNAMIC(CRxControlPane, CDockablePane)

CRxControlPane::CRxControlPane()
{

}

CRxControlPane::~CRxControlPane()
{
}


BEGIN_MESSAGE_MAP(CRxControlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CRxControlPane �޽��� ó�����Դϴ�.




int CRxControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	BOOL bRet = m_RxControlDlg.Create(IDD_RXCONTROLDLG, this);
	ASSERT(bRet);
	m_RxControlDlg.ShowWindow(SW_SHOW);

	return 0;
}


void CRxControlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//2. m_WinchCtrlDlg�� ũ�Ⱑ ������ ũ�⿡ ���� ���ϵ��� ����
	m_RxControlDlg.MoveWindow(0,0, cx, cy);
}
