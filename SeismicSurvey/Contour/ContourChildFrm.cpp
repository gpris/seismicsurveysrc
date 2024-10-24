// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://msdn.microsoft.com/officeui를 참조하십시오.
//
// Copyright (C) Microsoft Corporation
// 모든 권리 보유.

// ChildFrm.cpp : CContourChildFrame 클래스의 구현
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

// CContourChildFrame 생성/소멸

CContourChildFrame::CContourChildFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CContourChildFrame::~CContourChildFrame()
{
}


BOOL CContourChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.style &=~(LONG)FWS_ADDTOTITLE;

	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CContourChildFrame 진단

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

// CContourChildFrame 메시지 처리기

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
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 인쇄 미리 보기 모드를 닫습니다.
	}
}

void CContourChildFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}


BOOL CContourChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    //if (!m_Rulers.CreateRulers(this, pContext)) {
    //    TRACE("Error creation of rulers\n");
    //    return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
    //}

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CContourChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

#include "ContourDoc.h"

void CContourChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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