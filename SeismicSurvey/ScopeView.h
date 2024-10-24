#pragma once

#include "SeismicDataDraw.h"

// CScopeView ���Դϴ�.

class CSeismicSurveyDoc;

class CScopeView : public CView
{
	DECLARE_DYNCREATE(CScopeView)

protected:
	CScopeView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CScopeView();
public:
	CSeismicSurveyDoc* GetDocument() const;
	void DetermineDrawSize();
	void DrawGrid(CDC* pDC);
	CRect m_GraphRect;
	CRect m_HeaderRect;

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // SeismicSurveyView.cpp�� ����� ����
inline CSeismicSurveyDoc* CScopeView::GetDocument() const
   { return reinterpret_cast<CSeismicSurveyDoc*>(m_pDocument); }
#endif
