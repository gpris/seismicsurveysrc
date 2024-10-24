#pragma once

class CSeismicSurveyDoc;

// CFileTreeView 뷰입니다.

class CFileTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CFileTreeView)

protected:
	CFileTreeView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFileTreeView();

public:
	CString GetPathStringSelectedItem(HTREEITEM hItem);
	CSeismicSurveyDoc* GetDocument() const;
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void FillFileView();
	void InitClassic();
	//CTreeCtrl m_wndFileView;
	CImageList m_FileViewImages;
	void AdjustLayout();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnChangeVisualStyle();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // SeismicSurveyView.cpp의 디버그 버전
inline CSeismicSurveyDoc* CFileTreeView::GetDocument() const
   { return reinterpret_cast<CSeismicSurveyDoc*>(m_pDocument); }
#endif
