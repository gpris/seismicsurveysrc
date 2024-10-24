#if !defined(AFX_CONTOURVIEW_H__F1D75CCE_6DF2_4DB6_96EE_F9207A771C9D__INCLUDED_)
#define AFX_CONTOURVIEW_H__F1D75CCE_6DF2_4DB6_96EE_F9207A771C9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContourView.h : header file
//

#include "ContourDoc.h"
#include "Dib.h"
#include "LevelFile.h"
/////////////////////////////////////////////////////////////////////////////
// CContourView view

using namespace KHC;

class CContourView : public CScrollView
{
protected:
	CContourView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CContourView)

// Attributes
public:
	CContourDoc* GetDocument();

// Operations
public:
	KHC::CImage* m_pImage;
	void Contour(double **d,int ilb,int iub,int jlb,int jub, double *x,double *y,int nc,double *z, CDC * pDC);
	void Contour(double **d,int ilb,int iub,int jlb,int jub, double *x,double *y,CLevelFile * pLevelFile, CDC * pDC);
	double **m_data2d;
	double *m_xaxis;
	double *m_yaxis;
	double *m_levels;
	double *m_area;
	int m_nDiameter;
	void DrawContour(CDC* pDC);
	void DrawContourLines(CDC* pDC);
	void DrawContourLinesHS(CDC* pDC);
	void DrawAxis(CDC* pDC);
	bool m_bContourlines;
	bool m_bDrawLanes;
	float PolyArea(Point3D * Points, int nPolygon);
	int m_xmargin;
	int m_ymargin;
//	bool m_bRedraw;

	CBitmap m_Bitmap;

private:
	bool    m_bImage;
	bool	m_bImageReady;
	bool	m_bLegend;
	bool	m_bValues;
	float		m_nZoom;
	int		m_nNumOfLevels;

	int		m_nBaseWidth;
	int		m_nBaseHeight;

	double	m_dThickMax;
	double	m_dThickMin;

	void DrawValues(CDC* pDC);
	void DrawLegend(CDC* pDC);
	void ResizeFrame();

	CSize m_CanvasSize;

	bool m_bZoomFitMode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContourView)
	public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);


	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CContourView();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CContourView)
	afx_msg void OnFilePrintPreview();
	afx_msg void OnShowvalues();
	afx_msg void OnShowlegend();
	afx_msg void OnUpdateShowvalues(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowlegend(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowcontourlines();
	afx_msg void OnUpdateShowcontourlines(CCmdUI *pCmdUI);
	afx_msg void OnOptionShowimage();
	afx_msg void OnUpdateOptionShowimage(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void 	OnZoomSlider2();
	void SaveImageFile(CString strFileName); // Sanghyun 103114 
	void OnSaveImageAs();// Sanghyun 103114 
};

#ifndef _DEBUG  // debug version in WaferPainterView.cpp
inline CContourDoc* CContourView::GetDocument()
   { return (CContourDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURVIEW_H__F1D75CCE_6DF2_4DB6_96EE_F9207A771C9D__INCLUDED_)
