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

// MainFrm.h : CMainFrame Ŭ������ �������̽�
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

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
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

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;

// ������ �޽��� �� �Լ�
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


