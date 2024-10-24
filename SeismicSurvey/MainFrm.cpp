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

// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "SeismicSurvey.h"

#include "MainFrm.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_MESSAGE(WM_COMM_CTS_DETECTED, OnCTSDetected)
	ON_MESSAGE(WM_COMM_PACKET_RECEIVED, OnPacketReceived)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_TAB_WINDOW_CHK, &CMainFrame::OnTabWindowChk)
	ON_UPDATE_COMMAND_UI(ID_TAB_WINDOW_CHK, &CMainFrame::OnUpdateTabWindowChk)
	ON_COMMAND(ID_PANE_VISIBLE_CHK, &CMainFrame::OnPaneVisibleChk)
	ON_UPDATE_COMMAND_UI(ID_PANE_VISIBLE_CHK, &CMainFrame::OnUpdatePaneVisibleChk)
	ON_MESSAGE(WM_BSCAN_ACTIVATED, OnBScanActivated)
	ON_MESSAGE(WM_BSCAN_TERMINATED, OnBScanTerminated)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	m_bTabWindowEnabled = true;
	m_bPaneVisibleEnabled = true;

	m_pDoc = NULL;

	m_bScanActivated = false;

	m_ScanThread = NULL;
	m_bScanThreadAlive = false;

	m_bThreadBscanActivated = false;
	m_bThreadBscanTerminated = false;

	m_FileCount = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 보관된 값에 따라 비주얼 관리자 및 스타일을 설정합니다.
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 사용할 수 있는 다른 스타일...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // FALSE로 설정하여 탭 영역 오른쪽에 닫기 단추를 배치합니다.
	mdiTabParams.m_bTabIcons = FALSE;    // TRUE로 설정하여 MDI 탭의 문서 아이콘을 활성화합니다.
	mdiTabParams.m_bAutoColor = TRUE;    // FALSE로 설정하여 MDI 탭의 자동 색 지정을 비활성화합니다.
	mdiTabParams.m_bDocumentMenu = TRUE; // 탭 영역의 오른쪽 가장자리에 문서 메뉴를 활성화합니다.
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	CMFCRibbonEdit* pEdit1 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_RIB_DRAWSTART_SPIN));
	pEdit1->EnableSpinButtons(0, 2048);
	pEdit1->SetEditText("0");

	CMFCRibbonEdit* pEdit2 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_RIB_DRAWDATANO_SPIN));
	pEdit2->EnableSpinButtons(0, 2048);
	pEdit2->SetEditText("2048");

	CMFCRibbonEdit* pEdit3 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_RIB_STARTDEPTH_SPIN));
	pEdit3->EnableSpinButtons(0, 16);
	pEdit3->SetEditText("0");

	CMFCRibbonEdit* pEdit4 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_RIB_RANGEDEPTH_SPIN));
	pEdit4->EnableSpinButtons(0, 16);
	pEdit4->SetEditText("16");

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 메뉴 항목 이미지를 로드합니다(표준 도구 모음에 없음).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 도킹 창을 만듭니다.
	//if (!CreateDockingWindows())
	//{
	//	TRACE0("도킹 창을 만들지 못했습니다.\n");
	//	return -1;
	//}

	//m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndFileView);
	//CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);
	//m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndProperties);

	m_WinchCtrlPane.CreateEx(NULL, _T("Winch Control"), this, CRect(0,0,200,500), TRUE, 1235, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	m_WinchCtrlPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_WinchCtrlPane);

	m_RxControlPane.CreateEx(NULL, _T("Rx Control"), this, CRect(0,0,200,500), TRUE, 1236, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	m_RxControlPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_RxControlPane);

	m_TxControlPane.CreateEx(NULL, _T("Tx Control"), this, CRect(0,0,200,500), TRUE, 1237, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	m_TxControlPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_TxControlPane);

	m_SurveyCtrlPane.CreateEx(NULL, _T("Survey Control"), this, CRect(0,0,200,500), TRUE, 1238, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	m_SurveyCtrlPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_SurveyCtrlPane);

	// 향상된 창 관리 대화 상자를 활성화합니다.
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 창 제목 표시줄에서 문서 이름 및 응용 프로그램 이름의 순서를 전환합니다.
	// 문서 이름이 축소판 그림과 함께 표시되므로 작업 표시줄의 기능성이 개선됩니다.
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 클래스 뷰를 만듭니다.
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("클래스 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 파일 뷰를 만듭니다.
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("파일 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 출력 창을 만듭니다.
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 속성 창을 만듭니다.
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("속성 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnTabWindowChk()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bTabWindowEnabled = !m_bTabWindowEnabled;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 사용할 수 있는 다른 스타일...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // FALSE로 설정하여 탭 영역 오른쪽에 닫기 단추를 배치합니다.
	mdiTabParams.m_bTabIcons = FALSE;    // TRUE로 설정하여 MDI 탭의 문서 아이콘을 활성화합니다.
	mdiTabParams.m_bAutoColor = TRUE;    // FALSE로 설정하여 MDI 탭의 자동 색 지정을 비활성화합니다.
	mdiTabParams.m_bDocumentMenu = TRUE; // 탭 영역의 오른쪽 가장자리에 문서 메뉴를 활성화합니다.

	EnableMDITabbedGroups(m_bTabWindowEnabled, mdiTabParams);
}


void CMainFrame::OnUpdateTabWindowChk(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bTabWindowEnabled);
}


void CMainFrame::OnPaneVisibleChk()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bPaneVisibleEnabled = !m_bPaneVisibleEnabled;
	//m_wndOutput.ShowPane(m_bPaneVisibleEnabled, false, false);
	//m_wndFileView.ShowPane(m_bPaneVisibleEnabled, false, false);
	//m_wndClassView.ShowPane(m_bPaneVisibleEnabled, false, false);
	//m_wndProperties.ShowPane(m_bPaneVisibleEnabled, false, false);
	m_WinchCtrlPane.ShowPane(m_bPaneVisibleEnabled, false, false);
	m_RxControlPane.ShowPane(m_bPaneVisibleEnabled, false, false);
	m_TxControlPane.ShowPane(m_bPaneVisibleEnabled, false, false);
	m_SurveyCtrlPane.ShowPane(m_bPaneVisibleEnabled, false, false);
//	m_splitePane.ShowPane(m_bPaneVisibleEnabled, false, false);
//	m_wndRibbonBar.ShowPane(m_bPaneVisibleEnabled, false, false);
//	m_wndRibbonBar.ToggleMimimizeState();
}


void CMainFrame::OnUpdatePaneVisibleChk(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bPaneVisibleEnabled);
}

void CMainFrame::GetComportInfoFromRegistry(CComboBox* UserPortCtrl)
{
	DWORD   resVal;
    HKEY    hSerialCommKey = NULL;
    DWORD   i, status;
    DWORD   keyType, nameBufSize, dataBufSize;
    UCHAR   valueKeyName[64], valueKeyData[64];
	int		count;
	char CommList[256][12];
    resVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                           "HARDWARE\\DEVICEMAP\\SERIALCOMM",
                           0, KEY_READ, &hSerialCommKey);
	count = 0;
    if( resVal == ERROR_SUCCESS ) 
	{
        for( i = 0;; i++ ) {
            nameBufSize = dataBufSize = 64;
            status = RegEnumValue(
                        hSerialCommKey,
                        i,
                        (CHAR *)valueKeyName,
                        &nameBufSize,
                        NULL,
                        &keyType,
                        (UCHAR *)valueKeyData,
                        &dataBufSize
                        );
            if (status == ERROR_SUCCESS)
			{
                strcpy(CommList[i], (const char *)valueKeyData);
				UserPortCtrl->AddString(CommList[i]);
				UserPortCtrl->SetCurSel(count);
				count++;
			}
            else
                break;
        }
        RegCloseKey(hSerialCommKey);
	}	
}

void CMainFrame::WinchPortConnect()
{
	// TODO: Add your control notification handler code here
	if(m_Winch.m_bConnected)
	{
		m_Winch.ClosePort();

		m_Winch.m_bConnected = FALSE;

		m_WinchCtrlPane.m_WinchCtrlDlg.m_WinchConnectBtnCtrl.SetWindowTextA("연결");
	}
	else
	{
		CString tempName;
		m_WinchCtrlPane.m_WinchCtrlDlg.m_WinchPortComboCtrl.GetWindowTextA(tempName);
		tempName.Delete(0,3);
		int portNumber = atoi(tempName);
		if(m_Winch.InitPort(this, portNumber,115200, 'n', 8, 1))
		{
			m_Winch.m_bConnected = TRUE;
			m_Winch.m_PortNumber = portNumber;
			m_Winch.StartMonitoring();
			m_WinchCtrlPane.m_WinchCtrlDlg.m_WinchConnectBtnCtrl.SetWindowTextA("해제");
			m_Winch.DirectWriteToPort(_T("r 0x600 "));
		}
		else
		{
			AfxMessageBox("포트 연결에 실패하였습니다");
		}
	}
}

void CMainFrame::TxPortConnect()
{
//	m_MirecoEyeCtrlPane.m_MirecoEyeCtrlDlg.UpdateData(true);

	// TODO: Add your control notification handler code here
	if(m_SeismicTx.m_bConnected)
	{
		m_SeismicTx.ClosePort();

		m_SeismicTx.m_bConnected = FALSE;

		m_TxControlPane.m_TxControlDlg.m_TxConnectBtnCtrl.SetWindowTextA("연결");
	}
	else
	{
		CString tempName;
		m_TxControlPane.m_TxControlDlg.m_TxComportComboCtrl.GetWindowTextA(tempName);
		tempName.Delete(0,3);
		int portNumber = atoi(tempName);
		if(m_SeismicTx.InitPort(this, portNumber,115200, 'n', 8, 1))
		{
			m_SeismicTx.m_bConnected = TRUE;
			m_SeismicTx.m_PortNumber = portNumber;
			m_SeismicTx.StartMonitoring();
			m_TxControlPane.m_TxControlDlg.m_TxConnectBtnCtrl.SetWindowTextA("해제");
			m_SeismicTx.DirectWriteToPort(_T("r 0x600 "));
		}
		else
		{
			AfxMessageBox("포트 연결에 실패하였습니다");
		}
	}
}

void CMainFrame::RxPortConnect()
{
//	m_MirecoEyeCtrlPane.m_MirecoEyeCtrlDlg.UpdateData(true);

	// TODO: Add your control notification handler code here
	if(m_SeismicRx.m_bConnected)
	{
		m_SeismicRx.ClosePort();

		m_SeismicRx.m_bConnected = FALSE;

		m_RxControlPane.m_RxControlDlg.m_RxConnectBtnCtrl.SetWindowTextA("연결");
	}
	else
	{
		CString tempName;
		m_RxControlPane.m_RxControlDlg.m_RxComportComboCtrl.GetWindowTextA(tempName);
		tempName.Delete(0,3);
		int portNumber = atoi(tempName);
		if(m_SeismicRx.InitPort(this, portNumber,115200, 'n', 8, 1))
		{
			m_SeismicRx.m_bConnected = TRUE;
			m_SeismicRx.m_PortNumber = portNumber;
			m_SeismicRx.StartMonitoring();
			m_RxControlPane.m_RxControlDlg.m_RxConnectBtnCtrl.SetWindowTextA("해제");
			m_SeismicRx.DirectWriteToPort(_T("r 0x600 "));
		}
		else
		{
			AfxMessageBox("포트 연결에 실패하였습니다");
		}
	}
}

LONG CMainFrame::OnCommunication(WPARAM ch, LPARAM port)
{
	CString tempstring;

	if(port == m_Winch.m_PortNumber)
	{
		if(m_Winch.PacketParsing(ch))
		{
			m_SeismicTx.m_TxDepth = m_Winch.m_TxDepth;
			m_SeismicTx.m_RxDepth = m_Winch.m_RxDepth;

			m_SeismicRx.m_TxDepth = m_Winch.m_TxDepth;
			m_SeismicRx.m_RxDepth = m_Winch.m_RxDepth;

//			m_WinchCtrlPane.m_WinchCtrlDlg.WinchDepthUpdate();
		}
	}

	BOOL result = false;
	if(port == m_SeismicTx.m_PortNumber)
	{
		TRACE("%c", ch);
	}

	if(port == m_SeismicRx.m_PortNumber)
	{
//		TRACE("%c", ch);
		if(m_SeismicRx.PacketParsing(ch))
		{
			m_pDoc->SpiritSDataDraw();
		}
	}
	return 0;
}

LONG CMainFrame::OnPacketReceived(WPARAM ch, LPARAM port)
{

	m_pDoc->SpiritSDataDraw();

	return 0;
}
LONG CMainFrame::OnCTSDetected(WPARAM, LPARAM port)
{
	return 0;
}

UINT ScanThreadProc(LPVOID arg)
{
	CMainFrame * pMainFrame = (CMainFrame *)arg;
	BOOL result;

	while(pMainFrame->m_bScanThreadAlive)
	{
		if(pMainFrame->m_pDoc)
		{

			if(pMainFrame->m_bThreadBscanActivated)
			{
				do
				{
					result =pMainFrame-> m_SeismicRx.StartCmdTriggerDlgThread(pMainFrame);
				}while(!result);

				pMainFrame->m_bThreadBscanActivated = false;

				if(pMainFrame->m_bThreadBscanTerminated) break;
				pMainFrame->m_ScanInfo.m_TargetDepth += pMainFrame->m_Winch.m_ScanDirection 
					* pMainFrame->m_ScanInfo.m_ScanStep;

				if(fabs(pMainFrame->m_Winch.m_TxDepth - pMainFrame->m_ScanInfo.m_StopDepth) > 0.02)
				{
					pMainFrame->m_Winch.IndividualMove(pMainFrame->m_ScanInfo.m_ScanVelocity, pMainFrame->m_ScanInfo.m_TargetDepth,
											pMainFrame->m_ScanInfo.m_ScanVelocity, pMainFrame->m_ScanInfo.m_TargetDepth,
											0,0, 0);
				}
				if(pMainFrame->m_bThreadBscanTerminated) break;

				pMainFrame->m_SeismicRx.ReadScanData();
				pMainFrame->m_pDoc->SpiritSDataDrawThread(pMainFrame);
				pMainFrame->SaveScanDataThread(pMainFrame);
			}
		}
	}

	return 0;
}

void CMainFrame::ThreadBScanStart()
{
	m_bScanThreadAlive = true;
	m_ScanThread = AfxBeginThread(ScanThreadProc, this);
}

void CMainFrame::ThreadBScanActivated()
{
	m_bThreadBscanActivated = true;
}

void CMainFrame::ThreadBScanTerminated()
{
	m_bThreadBscanTerminated = true;
}

void CMainFrame::BScanActivated()
{
	BOOL result;
	if(m_pDoc)
	{

		do
		{
			result = m_SeismicRx.StartCmdTriggerDlg();
		}while(!result);

		m_ScanInfo.m_TargetDepth += m_Winch.m_ScanDirection * m_ScanInfo.m_ScanStep;

		if(fabs(m_Winch.m_TxDepth - m_ScanInfo.m_StopDepth) > 0.02)
		{
			m_Winch.IndividualMove(m_ScanInfo.m_ScanVelocity, m_ScanInfo.m_TargetDepth,
									m_ScanInfo.m_ScanVelocity, m_ScanInfo.m_TargetDepth,
									0,0, 0);
		}

		m_SeismicRx.ReadScanDataDlg();
		m_pDoc->SpiritSDataDraw();
		SaveScanData();
	}
}

void CMainFrame::BScanTerminated()
{
	if(m_pDoc)
	{
//		ThreadBScanTerminated();

		AfxMessageBox("탐사가 종료되었습니다.");
		m_SurveyCtrlPane.m_SurveyCtrlDlg.m_ScanStartBtnCtrl.SetWindowTextA(_T("탐사시작"));
		m_Winch.StopPacket();
//			m_pDoc->SaveAllData();
		m_bScanActivated = false;
	}
}

void CMainFrame::BScanInitialize()
{
	if(m_pDoc)
	{
		m_pDoc->ClearDabFileList();
		m_pDoc->m_DabFiles.DeleteData();
	}
}

void CMainFrame::BScanStart()
{
	if(m_pDoc)
	{
		m_bScanActivated = true;
		m_ScanInfo.m_TargetDepth = m_ScanInfo.m_StartDepth;
		m_Winch.ScanSet(m_ScanInfo.m_StartDepth, m_ScanInfo.m_StopDepth, m_ScanInfo.m_ScanStep, 0);

		for(int indx = 0; indx<16; indx++)
		{
			if(!m_SeismicRx.m_SondeMode[indx])
			{
				m_RxControlPane.m_RxControlDlg.m_UARTMode = 1;
				UpdateData(false);
				m_SeismicRx.m_SondeMode[indx] = m_RxControlPane.m_RxControlDlg.m_UARTMode;
				m_SeismicRx.SetSondeMode(indx, m_RxControlPane.m_RxControlDlg.m_UARTMode);
				Sleep(100);
			}
		}
//		ThreadBScanStart();
	}
}

LONG CMainFrame::OnBScanActivated(WPARAM currentDepth, LPARAM difference)
{
	if(m_pDoc)
	{
//		ThreadBScanActivated();
		BScanActivated();
	}
	return 0;
}

LONG CMainFrame::OnBScanTerminated(WPARAM currentDepth, LPARAM difference)
{
	if(m_pDoc)
	{
		BScanTerminated();
	}
	return 0;
}

#define REG_HOMEFOLDER	TEXT("HOMEFOLDER")

BOOL CreateDir(CString dir)
{
    CFileFind file;
    CString strFile = _T("*.*");
    BOOL bResult = file.FindFile(dir + strFile);
 
    if(!bResult)
    {
        bResult = CreateDirectory(dir, NULL);
    }
 
    return bResult;
}

void CMainFrame::SaveScanData()
{
	CString sitefolder;
	CString subsitefolder;
	CString holefolder;

	if(m_pDoc)
	{
		CString filename;
		CString homefolder;

		//homefolder = AfxGetApp()->GetProfileString(REG_SECTION_DEVICE, REG_HOMEFOLDER);
		if(homefolder == "")	//셋팅 다이얼로그에 값이 빈칸이라면
			homefolder = _T("c:\\");

		if(CreateDir(homefolder))
		{
			CTime currenttime;
			currenttime = CTime::GetCurrentTime();
			filename.Format(_T("%02d%02d%02d"), currenttime.GetYear()%100, currenttime.GetMonth(), currenttime.GetDay());

			filename = homefolder + filename;

			if(CreateDir(filename))
			{
				sitefolder = m_ScanInfo.m_SiteName;

				if(sitefolder.IsEmpty())
				{
					sitefolder = _T("unnamed");
				}

				filename = filename + _T("\\") + sitefolder;
				if(CreateDir(filename))
				{
					subsitefolder = m_ScanInfo.m_SubSiteName;

					filename = filename + _T("\\") + subsitefolder;
					if(CreateDir(filename))
					{
						holefolder = m_ScanInfo.m_TxHoleName + _T("-") + m_ScanInfo.m_RxHoleName;

						filename = filename + _T("\\") + holefolder;

						//BOOL bResult;

						//do
						//{
						//	CFileFind file;
						//	CString strFile = _T("*.*");
						//	bResult = file.FindFile(filename + _T("\\") + strFile);

						//	if(bResult)
						//	{
						//		CString FileCountString;
						//		FileCountString.Format("%02d", m_FileCount);
						//		filename = filename + FileCountString;
						//		m_FileCount++;
						//	}
						//}while(bResult);

						if(CreateDir(filename))
						{
							m_FileCount = 0;
							m_pDoc->m_CurrentFolder = filename;
							filename.Format(_T("%s\\%04d%04d.dab"), filename, 
								(int)((m_SeismicRx.m_TxDepth + m_SeismicRx.m_TxDepthOffset + 0.05)  * 10), 
								(int)((m_SeismicRx.m_RxDepth + m_SeismicRx.m_RxDepthOffset + 4.0 + 0.05)*10));
							m_pDoc->m_DabFiles.SaveFile(filename, &m_SeismicRx, m_ScanInfo.m_ScanDirection);
							m_pDoc->UpdateDabFileList();
						}
					}
				}
			}
		}
	}
}

void CMainFrame::SaveScanDataThread(CMainFrame * pMainFrame)
{
	CString sitefolder;
	CString subsitefolder;


	if(m_pDoc)
	{
		CString filename;
		CString homefolder;

		//homefolder = AfxGetApp()->GetProfileString(REG_SECTION_DEVICE, REG_HOMEFOLDER);
		if(homefolder == "")	//셋팅 다이얼로그에 값이 빈칸이라면
			homefolder = _T("c:\\");

		if(CreateDir(homefolder))
		{
			CTime currenttime;
			currenttime = CTime::GetCurrentTime();
			filename.Format(_T("%02d%02d%02d"), currenttime.GetYear()%100, currenttime.GetMonth(), currenttime.GetDay());

			filename = homefolder + filename;

			if(CreateDir(filename))
			{
				sitefolder = m_ScanInfo.m_SiteName;

				if(sitefolder.IsEmpty())
				{
					sitefolder = _T("unnamed");
				}

				filename = filename + _T("\\") + sitefolder;
				if(CreateDir(filename))
				{
					subsitefolder = m_ScanInfo.m_SubSiteName;

					filename = filename + _T("\\") + subsitefolder;
					if(CreateDir(filename))
					{
						filename.Format(_T("%s\\%04d%04d.dab"), filename, 
							(int)((m_SeismicRx.m_TxDepth + m_SeismicRx.m_TxDepthOffset + 0.05)  * 10), 
							(int)((m_SeismicRx.m_RxDepth + m_SeismicRx.m_RxDepthOffset + 4.0 + 0.05)*10));
						m_pDoc->m_DabFiles.SaveFile(filename, &m_SeismicRx, m_ScanInfo.m_ScanDirection);
						m_pDoc->UpdateDabFileListThread(pMainFrame);
					}
				}
			}
		}
	}
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 222:
		m_WinchCtrlPane.m_WinchCtrlDlg.WinchDepthUpdate();
		m_SurveyCtrlPane.m_SurveyCtrlDlg.UpdateAntennaPosition();
		break;
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}
