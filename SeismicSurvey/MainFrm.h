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

// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "WinchCtrlPane.h"
#include "RxControlPane.h"
#include "TxControlPane.h"
#include "SurveyCtrlPane.h"
#include "AuslogWinch.h"
#include "SeismicSonde.h"
#include "SeismicSurveyDoc.h"
#include "ScanInfo.h"

#include "SignalProcessingParameter.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 특성입니다.
public:

// 작업입니다.
public:
	BOOL m_bTabWindowEnabled;
	BOOL m_bPaneVisibleEnabled;

	CMFCRibbonBar		m_wndRibbonBar;
	CMFCRibbonStatusBar m_wndStatusBar;
	CFileView			m_wndFileView;
	CClassView			m_wndClassView;
	COutputWnd			m_wndOutput;
	CPropertiesWnd		m_wndProperties;
	CWinchCtrlPane		m_WinchCtrlPane;
	CRxControlPane		m_RxControlPane;
	CTxControlPane		m_TxControlPane;
	CSurveyCtrlPane		m_SurveyCtrlPane;

	CAuslogWinch		m_Winch;
	CSeismicSonde		m_SeismicTx;
	CSeismicSonde		m_SeismicRx;

	CScanInfo			m_ScanInfo;

	bool m_bScanActivated;

	void GetComportInfoFromRegistry(CComboBox* UserPortCtrl);
	void WinchPortConnect();
	void TxPortConnect();
	void RxPortConnect();

	void BScanStart();
	void BScanTerminated();
	void BScanActivated();
	void BScanInitialize();

	CWinThread * m_ScanThread;
	bool m_bScanThreadAlive;
	bool m_bThreadBscanActivated;
	bool m_bThreadBscanTerminated;
	void ThreadBScanStart();
	void ThreadBScanTerminated();
	void ThreadBScanActivated();

	void SaveScanData();
	void SaveScanDataThread(CMainFrame * pMainFrame);
	int m_FileCount;

	CSeismicSurveyDoc * m_pDoc;
	CSignalProcessingParameter m_CommonSigParameter;


//	afx_msg LRESULT OnUpdateConnection(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnCommunication(WPARAM, LPARAM);
	afx_msg LONG OnCTSDetected(UINT, LONG);
	afx_msg LONG OnPacketReceived(UINT, LONG);

	afx_msg LONG OnBScanTerminated(WPARAM, LPARAM);
	afx_msg LONG OnBScanActivated(WPARAM, LPARAM);

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnTabWindowChk();
	afx_msg void OnUpdateTabWindowChk(CCmdUI *pCmdUI);
	afx_msg void OnPaneVisibleChk();
	afx_msg void OnUpdatePaneVisibleChk(CCmdUI *pCmdUI);


	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


