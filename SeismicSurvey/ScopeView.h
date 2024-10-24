#pragma once

#include "SeismicDataDraw.h"

// CScopeView 뷰입니다.

class CSeismicSurveyDoc;

class CScopeView : public CView
{
	DECLARE_DYNCREATE(CScopeView)

protected:
	CScopeView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CScopeView();
public:
	CSeismicSurveyDoc* GetDocument() const;
	void DetermineDrawSize();
	void DrawGrid(CDC* pDC);
	CRect m_GraphRect;
	CRect m_HeaderRect;

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // SeismicSurveyView.cpp의 디버그 버전
inline CSeismicSurveyDoc* CScopeView::GetDocument() const
   { return reinterpret_cast<CSeismicSurveyDoc*>(m_pDocument); }
#endif
