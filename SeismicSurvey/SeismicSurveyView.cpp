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

// SeismicSurveyView.cpp : CSeismicSurveyView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SeismicSurvey.h"
#endif

#include "SeismicSurveyDoc.h"
#include "SeismicSurveyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSeismicSurveyView

IMPLEMENT_DYNCREATE(CSeismicSurveyView, CScrollView)

BEGIN_MESSAGE_MAP(CSeismicSurveyView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSeismicSurveyView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_DCCOUPLED_CHECK, &CSeismicSurveyView::OnUpdateDcCoupledCheck)
END_MESSAGE_MAP()

// CSeismicSurveyView 생성/소멸

CSeismicSurveyView::CSeismicSurveyView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CSeismicSurveyView::~CSeismicSurveyView()
{
}

BOOL CSeismicSurveyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CSeismicSurveyView 그리기

void CSeismicSurveyView::OnDraw(CDC* pDC)
{
	CSeismicSurveyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(rect);
	CRect drawrect;
	float drawheight;
	int scanno = pDoc->m_SeismicDraw.m_ScanNo;

	drawrect = rect;
	if(scanno>16)
	{
		drawrect = rect;
		drawheight = (float)(rect.Height())/pDoc->m_DispScanNo;
		drawrect.bottom = (int)((float)drawrect.top + drawheight*(float)scanno);
	}

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if(pDoc->m_DataReady)
	{
		if(pDoc->m_bProcessed)
		{
			pDoc->m_SeismicDraw.DrawSeismicGraph(pDC, drawrect, 1, pDoc->m_ViewScale);
		}
		else
		{
			pDoc->m_SeismicDraw.DrawSeismicGraph(pDC, drawrect, 0, pDoc->m_ViewScale);
		}
	}

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = drawrect.Width();
	sizeTotal.cy = drawrect.Height();
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CSeismicSurveyView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CSeismicSurveyView 인쇄


void CSeismicSurveyView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSeismicSurveyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSeismicSurveyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSeismicSurveyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CSeismicSurveyView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSeismicSurveyView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSeismicSurveyView 진단

#ifdef _DEBUG
void CSeismicSurveyView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSeismicSurveyView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSeismicSurveyDoc* CSeismicSurveyView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeismicSurveyDoc)));
	return (CSeismicSurveyDoc*)m_pDocument;
}
#endif //_DEBUG


// CSeismicSurveyView 메시지 처리기


void CSeismicSurveyView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonUp(nFlags, point);
}


void CSeismicSurveyView::OnUpdateDcCoupledCheck(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}
