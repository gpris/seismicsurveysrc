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

// SeismicSurveyView.cpp : CSeismicSurveyView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSeismicSurveyView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_DCCOUPLED_CHECK, &CSeismicSurveyView::OnUpdateDcCoupledCheck)
END_MESSAGE_MAP()

// CSeismicSurveyView ����/�Ҹ�

CSeismicSurveyView::CSeismicSurveyView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CSeismicSurveyView::~CSeismicSurveyView()
{
}

BOOL CSeismicSurveyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CSeismicSurveyView �׸���

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

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = drawrect.Width();
	sizeTotal.cy = drawrect.Height();
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CSeismicSurveyView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CSeismicSurveyView �μ�


void CSeismicSurveyView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSeismicSurveyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CSeismicSurveyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CSeismicSurveyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CSeismicSurveyView ����

#ifdef _DEBUG
void CSeismicSurveyView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSeismicSurveyView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSeismicSurveyDoc* CSeismicSurveyView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeismicSurveyDoc)));
	return (CSeismicSurveyDoc*)m_pDocument;
}
#endif //_DEBUG


// CSeismicSurveyView �޽��� ó����


void CSeismicSurveyView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonUp(nFlags, point);
}


void CSeismicSurveyView::OnUpdateDcCoupledCheck(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}
