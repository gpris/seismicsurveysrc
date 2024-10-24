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

// ChildFrm.cpp : CSplitterChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "SeismicSurvey.h"

#include "SplitterChildFrm.h"
#include "SeismicSurveyView.h"
#include "FileTreeView.h"
#include "ScopeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSplitterChildFrame

IMPLEMENT_DYNCREATE(CSplitterChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CSplitterChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CSplitterChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CSplitterChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSplitterChildFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CSplitterChildFrame::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

// CSplitterChildFrame 생성/소멸

CSplitterChildFrame::CSplitterChildFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CSplitterChildFrame::~CSplitterChildFrame()
{
}


BOOL CSplitterChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CSplitterChildFrame 진단

#ifdef _DEBUG
void CSplitterChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CSplitterChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CSplitterChildFrame 메시지 처리기

void CSplitterChildFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CSplitterChildFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 인쇄 미리 보기 모드를 닫습니다.
	}
}

void CSplitterChildFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}


BOOL CSplitterChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/* CSplitterWnd사용시 
	if (!m_wndSplitter.CreateStatic(this,1, 2))
		return false;
	m_wndSplitter.CreateView(0,0, RUNTIME_CLASS(CFileTreeView), CSize(50,100), pContext);
	m_wndSplitter.CreateView(0,1, RUNTIME_CLASS(CSeismicSurveyView), CSize(800,100), pContext);
*/

/* CBiSplitterWnd사용 */
	if (!m_wndBiSplitter.Create(this, VSPLITTER | FIXED0 | VIEW3D ))
		return FALSE;


	if(!m_wndBiSplitter.CreatePane(0, THIN_BORDER | SMCAPTION | THIN_CAPTIONBORDER | FLAT_BUTTON
			,_T("파일"), m_wndBiSplitter.CreateView(RUNTIME_CLASS(CFileTreeView), 11, pContext) )
		||
		!m_wndBiSplitter.CreatePane(1, FLAT_BUTTON, NULL
			, m_wndBiSplitter.CreateView(RUNTIME_CLASS(CBiSplitterWnd), 12, HSPLITTER|FIXED0|VIEW3D)))
	{
		m_wndBiSplitter.DestroyWindow();
		return FALSE;
	}
	
	m_wndBiSplitter.SetSplitterPos(200);
	m_wndBiSplitter.SetSplitterGap(6);	

	//시작시 DataView는 숨김상태로 시작
	//
	m_pFileTreeView  = (CFileTreeView *)m_wndBiSplitter.GetPaneView(0);
	m_pSeismicFrame = (CBiSplitterWnd *)m_wndBiSplitter.GetPaneView(1);

	if(!m_pSeismicFrame->CreatePane(0, THIN_BORDER | SMCAPTION | THIN_CAPTIONBORDER | FLAT_BUTTON
			,_T("그래프"), m_pSeismicFrame->CreateView(RUNTIME_CLASS(CSeismicSurveyView), 111, pContext) )
		||
		!m_pSeismicFrame->CreatePane(1, THIN_BORDER | SMCAPTION | THIN_CAPTIONBORDER | FLAT_BUTTON
			, _T("파형"),  m_pSeismicFrame->CreateView(RUNTIME_CLASS(CScopeView), 112, pContext)))
	{
		m_wndBiSplitter.DestroyWindow();
		return FALSE;
	}
	m_pSeismicSurveyView  = (CSeismicSurveyView *)m_pSeismicFrame->GetPaneView(0);
	m_pScopeView  = (CScopeView *)m_pSeismicFrame->GetPaneView(1);

	m_pSeismicFrame->SetSplitterPos(200);
	m_pSeismicFrame->SetSplitterGap(6);	

	return true;
}
