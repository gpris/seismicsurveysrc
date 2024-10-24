// ScopeView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "ScopeView.h"
#include "SeismicSurveyDoc.h"


// CScopeView

IMPLEMENT_DYNCREATE(CScopeView, CView)

CScopeView::CScopeView()
{

}

CScopeView::~CScopeView()
{
}

BEGIN_MESSAGE_MAP(CScopeView, CView)
END_MESSAGE_MAP()


// CScopeView 그리기입니다.

void CScopeView::OnDraw(CDC* pDC)
{
	CSeismicSurveyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 그리기 코드를 추가합니다.
	CRect rect;
	GetClientRect(rect);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if(pDoc->m_DataReady)
	{
		if(pDoc->m_bProcessed)
		{
			pDoc->m_SeismicDraw.DrawOneSeismicGraph(pDC, rect, pDoc->m_SelectedIndex, 1, pDoc->m_ViewScale);
		}
		else
		{
			pDoc->m_SeismicDraw.DrawOneSeismicGraph(pDC, rect, pDoc->m_SelectedIndex,  0, pDoc->m_ViewScale);
		}
	}
}

void CScopeView::DrawGrid(CDC* pDC)
{
}

void CScopeView::DetermineDrawSize()
{
}


// CScopeView 진단입니다.

#ifdef _DEBUG
void CScopeView::AssertValid() const
{
	CView::AssertValid();
}

CSeismicSurveyDoc* CScopeView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeismicSurveyDoc)));
	return (CSeismicSurveyDoc*)m_pDocument;
}
#ifndef _WIN32_WCE
void CScopeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScopeView 메시지 처리기입니다.
