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

// SeismicSurveyDoc.h : CSeismicSurveyDoc Ŭ������ �������̽�
//


#pragma once

#include "DabFile.h"
#include "DatFile.h"
#include "SplitterChildFrm.h"
//#include "MainFrm.h"
#include "FileTreeView.h"
#include "SeismicSurveyView.h"

#define DABFILE 0
#define DATFILE 1

class CMainFrame;
class CSignalProcessingParameter;

class CSeismicSurveyDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CSeismicSurveyDoc();
	DECLARE_DYNCREATE(CSeismicSurveyDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
	CDabFile m_DabFiles;
	CDatFile m_DatFiles;
	CSeismicDataDraw m_SeismicDraw; 
	bool m_DataReady;
	bool m_bProcessed;

	void JodexDabfileopen();
	void JodexDatfileopen();
	void JodexDabFileDraw(int fileno);
	void UpdateDabFileList();
	void UpdateDabFileListThread(CMainFrame * pMainFrame);
	void ClearDabFileList();
	void JodexDatFileDraw(int fileno);
	void UpdateDatFileList();
	void ClearDatFileList();
	void InitDocument();

	void SpiritSDataDraw();
	void SpiritSDataDrawThread(CMainFrame * pMainFrame);

	void UpdateSeismicSurveyView();
	void GenerateDatFileFromDabDoc(CSeismicSurveyDoc * pDoc);

	CString m_CurrentFolder;

	CSplitterChildFrame * m_pChildFrame;
//	CMainFrame * m_pMainFrame;
	CFileTreeView * m_pFileTreeView;
	CSeismicSurveyView * m_pSeismicSurveyView;

	int m_FileType;

	void JodexDabfilesave();
	void JodexDatCsvfilesave(CSeismicSurveyDoc* pDoc);
	int m_SelectedIndex;

	float m_ViewScale;
	bool m_bViewAutoScale;

	CArray <CSeismicData *, CSeismicData *> * m_pActiveScanData;
	CSignalProcessingParameter * m_pActiveSigParameter;

	float m_StartDepth;
	float m_DepthRange;
	int m_DispScanNo;

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CSeismicSurveyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnJodexDabfileopen();
	afx_msg void OnJodexDatfileopen();
	afx_msg void OnRibbonDetowBtn();
	afx_msg void OnUpdateRibbonDetowBtn(CCmdUI *pCmdUI);
	afx_msg void OnJodexDabfilesaveButton();
	afx_msg void OnJodexDatCsvfilesaveButton();
	afx_msg void OnGendatfileButton();
	afx_msg void OnDcCoupledCheck();
	afx_msg void OnBscanAutoscaleCheck();
	afx_msg void OnUpdateBscanAutoscaleCheck(CCmdUI *pCmdUI);
	afx_msg void OnViewscaleSlider();
	afx_msg void OnRibbonCorrelationButton();
	afx_msg void OnRibbonIIRFilterBtn();
	afx_msg void OnTomographgenButton();
	afx_msg void OnRibDrawstartSpin();
	afx_msg void OnRibDrawdatanoSpin();
	afx_msg void OnRibProcessedDispCheck();
	afx_msg void OnUpdateRibProcessedDispCheck(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRibbonCorrelationButton(CCmdUI *pCmdUI);
	afx_msg void OnRibStartDepthSpin();
	afx_msg void OnRibRangeDepthSpin();
	afx_msg void OnRibAllDepthCheck();
};
