// TxControlPane.cpp : ���� �����Դϴ�.
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



// CTxControlPane �޽��� ó�����Դϴ�.




int CTxControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	BOOL bRet = m_TxControlDlg.Create(IDD_TXCONTROLDLG, this);
	ASSERT(bRet);
	m_TxControlDlg.ShowWindow(SW_SHOW);

	return 0;
}


void CTxControlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//2. m_WinchCtrlDlg�� ũ�Ⱑ ������ ũ�⿡ ���� ���ϵ��� ����
	m_TxControlDlg.MoveWindow(0,0, cx, cy);
}
