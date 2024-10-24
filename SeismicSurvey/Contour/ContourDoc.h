#if !defined(AFX_CONTOURDOC_H__A90EE96B_170B_47C4_A3FF_70AFF5747013__INCLUDED_)
#define AFX_CONTOURDOC_H__A90EE96B_170B_47C4_A3FF_70AFF5747013__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContourDoc.h : header file
//

#include "InputReader.h"
#include "..\GridPacket\GridPacket.h"
//#include "..\GridCtrlView\GridCtrlDoc.h"
#include "LevelFile.h"
#include "Contour.h"

/////////////////////////////////////////////////////////////////////////////
// CContourDoc document


class CContourDoc : public CDocument
{
protected:
	CContourDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CContourDoc)

// Attributes
public:
	InputReader m_ir;
	CGridPacket m_GridPacket;

	double m_xMax;
	double m_xMin;
	double m_xStep;

	double m_yMax;
	double m_yMin;
	double m_yStep;

	double m_zMax;
	double m_zMin;
	int m_DataID;

	double *m_xaxis;
	double *m_yaxis;
	double **m_data2d;

	bool m_bAutoGain;
	CLevelFile m_LevelFile;
	bool m_bRedraw;
	BOOL m_bPropDisplayed; //Sang for PropertyWnd
	void FillUpGeneralProperty(); //Sanghyun 102314 for PropertyWnd
	void FillUpProperty();//Sang for PropertyWnd
	void FillUpDispProperty(); //Sang for PropertyWnd
	CString m_PathName;
	int m_nPropId; // Sanghyun 102414 
	double m_TotalArea; // Sanghyun 102414

	double m_XDrawStart;
	double m_XDrawEnd;
	double m_YDrawStart;
	double m_YDrawEnd;

	double m_XDrawTickStep;
	double m_YDrawTickStep;
	
	double * m_XDrawTickValues;
	double * m_YDrawTickValues;

	int m_XDrawTickNo;
	int m_YDrawTickNo;

	bool m_bXDrawTickAuto;
	bool m_bYDrawTickAuto;

	CContour m_Contour;
	CImageList m_imageList_weight[50];
	CImageList m_imageList_style[50];

// Operations
public:
	void Init(LPCTSTR lpszPathName);
	void InitFromGridFile(LPCTSTR lpszPathName);
	void CompareWithSurfer(CContourDoc * pSrcDoc, LPCTSTR lpszPathName);
	//void InitFromLayerInformation(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xstep);
	//void GlobalKriging(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xstep);
	//void LocalKriging(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xstep);
	//void LocalKriging2(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xstep);
	//void LocalKriging3(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xstep);
	//void LocalKriging4(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xstep);
	//void LocalKriging5(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xstep);
	void GenerateDefaultLevelFile();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContourDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CContourDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CContourDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnCompareWithSurfer();
	afx_msg void OnCompRstExport();
	afx_msg void OnGridSave();
	afx_msg void OnLevelApplyBtn();
	void OnComboBoxSelChange(); //Sang for PropertyWnd
	afx_msg void OnDamageAnalysis();
	void SaveImageAuto(CString title);
	afx_msg void OnContourAxisBtn();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURDOC_H__A90EE96B_170B_47C4_A3FF_70AFF5747013__INCLUDED_)
