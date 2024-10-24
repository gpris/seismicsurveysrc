// ScopeView.cpp : ���� �����Դϴ�.
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


// CScopeView �׸����Դϴ�.

void CScopeView::OnDraw(CDC* pDC)
{
	CSeismicSurveyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	CRect rect;
	GetClientRect(rect);

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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


// CScopeView �����Դϴ�.

#ifdef _DEBUG
void CScopeView::AssertValid() const
{
	CView::AssertValid();
}

CSeismicSurveyDoc* CScopeView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
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


// CScopeView �޽��� ó�����Դϴ�.
