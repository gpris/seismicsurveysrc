// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://msdn.microsoft.com/officeui�� �����Ͻʽÿ�.
//
// Copyright (C) Microsoft Corporation
// ��� �Ǹ� ����.

// ChildFrm.cpp : CContourChildFrame Ŭ������ ����
//

#include "stdafx.h"
#include "..\SeismicSurvey.h"

#include "ContourChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CContourChildFrame

IMPLEMENT_DYNCREATE(CContourChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CContourChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CContourChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CContourChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CContourChildFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CContourChildFrame::OnUpdateFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_MDIACTIVATE()
//	ON_MESSAGE(WM_PROPERTYCOMBO_SELECTED, OnPropertyComboSelected)
END_MESSAGE_MAP()

// CContourChildFrame ����/�Ҹ�

CContourChildFrame::CContourChildFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CContourChildFrame::~CContourChildFrame()
{
}


BOOL CContourChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	cs.style &=~(LONG)FWS_ADDTOTITLE;

	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CContourChildFrame ����

#ifdef _DEBUG
void CContourChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CContourChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CContourChildFrame �޽��� ó����

void CContourChildFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CContourChildFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // �μ� �̸� ���� ��带 �ݽ��ϴ�.
	}
}

void CContourChildFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}


BOOL CContourChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
    //if (!m_Rulers.CreateRulers(this, pContext)) {
    //    TRACE("Error creation of rulers\n");
    //    return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
    //}

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CContourChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

#include "ContourDoc.h"

void CContourChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CContourDoc * pDoc = (CContourDoc *) GetActiveDocument();
	if (pDoc->m_bPropDisplayed )
	{

		pDoc->m_bPropDisplayed = false;

	}
	else
	{

		pDoc->m_bPropDisplayed = true;
		//switch (pDoc->m_nPropId)
		//{
		//case 0:
  //          pDoc->FillUpGeneralProperty();
  //          break;
		//case 1:
  //          pDoc->FillUpProperty();
  //          break;
  //      case 2:
  //          pDoc->FillUpDispProperty();
  //          break;
		//}
	}
}

LONG CContourChildFrame::OnPropertyComboSelected(UINT wParam, LONG lParam)
{
	int aaa = wParam;
	long bbb = lParam;

	return 0;
}