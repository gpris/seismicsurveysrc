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

// SeismicSurveyDoc.h : CSeismicSurveyDoc 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CSeismicSurveyDoc();
	DECLARE_DYNCREATE(CSeismicSurveyDoc)

// 특성입니다.
public:

// 작업입니다.
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

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CSeismicSurveyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
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
