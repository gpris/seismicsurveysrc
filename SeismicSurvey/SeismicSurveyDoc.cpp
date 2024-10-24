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

// SeismicSurveyDoc.cpp : CSeismicSurveyDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SeismicSurvey.h"
#endif

#include "SeismicSurveyDoc.h"
#include <cmath>

#include <propkey.h>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSeismicSurveyDoc

IMPLEMENT_DYNCREATE(CSeismicSurveyDoc, CDocument)

BEGIN_MESSAGE_MAP(CSeismicSurveyDoc, CDocument)
	ON_COMMAND(ID_JODEX_DABFILEOPEN, &CSeismicSurveyDoc::OnJodexDabfileopen)
	ON_COMMAND(ID_JODEX_DABFILESAVE_BUTTON, &CSeismicSurveyDoc::OnJodexDabfilesaveButton)

	ON_COMMAND(ID_JODEX_DATFILEOPEN, &CSeismicSurveyDoc::OnJodexDatfileopen)
	ON_COMMAND(ID_GENDATFILE_BUTTON, &CSeismicSurveyDoc::OnGendatfileButton)
	ON_COMMAND(ID_JODEX_DATCSVFILESAVE, &CSeismicSurveyDoc::OnJodexDatCsvfilesaveButton)

	ON_COMMAND(ID_RIBBON_DETOW_BTN, &CSeismicSurveyDoc::OnRibbonDetowBtn)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_DETOW_BTN, &CSeismicSurveyDoc::OnUpdateRibbonDetowBtn)
	
	ON_COMMAND(ID_DCCOUPLED_CHECK, &CSeismicSurveyDoc::OnDcCoupledCheck)
	ON_COMMAND(ID_BSCANAUTOSCALE_CHECK, &CSeismicSurveyDoc::OnBscanAutoscaleCheck)
	ON_UPDATE_COMMAND_UI(ID_BSCANAUTOSCALE_CHECK, &CSeismicSurveyDoc::OnUpdateBscanAutoscaleCheck)
	ON_COMMAND(ID_VIEWSCALE_SLIDER, &CSeismicSurveyDoc::OnViewscaleSlider)
	ON_COMMAND(ID_RIBBON_CORRELATION_BUTTON, &CSeismicSurveyDoc::OnRibbonCorrelationButton)
	ON_COMMAND(ID_RIBBON_IIRFILTER_BTN, &CSeismicSurveyDoc::OnRibbonIIRFilterBtn)
	ON_COMMAND(ID_TOMOGRAPHGEN_BUTTON, &CSeismicSurveyDoc::OnTomographgenButton)
	ON_COMMAND(ID_RIB_DRAWSTART_SPIN, &CSeismicSurveyDoc::OnRibDrawstartSpin)
	ON_COMMAND(ID_RIB_DRAWDATANO_SPIN, &CSeismicSurveyDoc::OnRibDrawdatanoSpin)
	ON_COMMAND(ID_RIB_PROCESSED_DISP_CHECK, &CSeismicSurveyDoc::OnRibProcessedDispCheck)
	ON_UPDATE_COMMAND_UI(ID_RIB_PROCESSED_DISP_CHECK, &CSeismicSurveyDoc::OnUpdateRibProcessedDispCheck)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_CORRELATION_BUTTON, &CSeismicSurveyDoc::OnUpdateRibbonCorrelationButton)
	ON_COMMAND(ID_RIB_STARTDEPTH_SPIN, &CSeismicSurveyDoc::OnRibStartDepthSpin)
	ON_COMMAND(ID_RIB_RANGEDEPTH_SPIN, &CSeismicSurveyDoc::OnRibRangeDepthSpin)
	ON_COMMAND(ID_RIB_ALLDEPTH_CHECK, &CSeismicSurveyDoc::OnRibAllDepthCheck)
END_MESSAGE_MAP()


// CSeismicSurveyDoc 생성/소멸

CSeismicSurveyDoc::CSeismicSurveyDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_DataReady = false;
	m_FileType = DABFILE;
	m_bProcessed = true;
	m_SelectedIndex = 0;
	m_ViewScale = 0.0;

	m_pActiveScanData = NULL;
	m_pActiveSigParameter = NULL;
	m_CurrentFolder = _T("");

	m_DispScanNo = 16;
}

CSeismicSurveyDoc::~CSeismicSurveyDoc()
{
}

BOOL CSeismicSurveyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	InitDocument();
	return TRUE;
}



void CSeismicSurveyDoc::InitDocument()
{
	POSITION pos = GetFirstViewPosition();
	CSeismicSurveyView * pFirstView = (CSeismicSurveyView * )GetNextView( pos );
	m_pChildFrame = (CSplitterChildFrame *)pFirstView->GetParentFrame();
	//m_pwndReceiverCtrlDlgBar = &m_pChildFrame->m_wndReceiverCtrlDlgBar;
	//m_ReceiverID = m_pwndReceiverCtrlDlgBar->m_CHSetCombo.GetCurSel();
	m_pFileTreeView = m_pChildFrame->m_pFileTreeView;
	m_pSeismicSurveyView = m_pChildFrame->m_pSeismicSurveyView;

}
// CSeismicSurveyDoc serialization

void CSeismicSurveyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CSeismicSurveyDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CSeismicSurveyDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSeismicSurveyDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSeismicSurveyDoc 진단

#ifdef _DEBUG
void CSeismicSurveyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSeismicSurveyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSeismicSurveyDoc 명령

#include "PathNameInfo\PathNameInfo.h"
#include "DabPacket.h"

void CSeismicSurveyDoc::OnJodexDabfileopen()
{
	CSeismicSurveyApp* pApp = static_cast<CSeismicSurveyApp*>(::AfxGetApp());

	CSeismicSurveyDoc * pJodexDoc = static_cast<CSeismicSurveyDoc*>(pApp->m_pJodexDocTemplate->CreateNewDocument());
	CFrameWnd* p2DFrame = pApp->m_pJodexDocTemplate->CreateNewFrame(pJodexDoc, NULL);
	pApp->m_pJodexDocTemplate->InitialUpdateFrame(p2DFrame, pJodexDoc);
	pJodexDoc->InitDocument();
	pJodexDoc->JodexDabfileopen();
}

void CSeismicSurveyDoc::JodexDabfileopen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString str = _T("Jodex file format(*.dab)|*.dab|All Files(*.*)|*.*||");	// 선택할 파일 종류
	CString dabPath;
	CPathNameInfo dabPathInfo;
	CString DstPath;
	CString strFileList; 

	CFileDialog dlg(TRUE, _T("DAB file open"), NULL, OFN_ALLOWMULTISELECT, str, NULL);
	const int c_cMaxFiles = 400 /*선택할 파일 숫자*/ ;	// 메모리 부족현상으로 확장 안해주면 몇개 못씀
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;

	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;

	if( dlg.DoModal() == IDOK)
	{
		//m_DabFiles.m_DabFileList.RemoveAll();
		//m_DabFiles.m_DabPacketList.RemoveAll();
		CString tempstring = dlg.GetFolderPath();
		m_CurrentFolder = tempstring;
		SetTitle(_T("Dab files in ") + tempstring);
		m_DabFiles.DeleteData();
		for(POSITION pos=dlg.GetStartPosition(); pos != NULL;)
		{
			// 전체삭제는 ResetContent

			dabPath = dlg.GetNextPathName(pos);
			m_DabFiles.m_DabFileList.AddTail(dabPath);

			//CDabPacket * pDabPacket = new CDabPacket;
			//pDabPacket->Load(dabPath);
			//m_DabFiles.m_DabPacketList.Add(pDabPacket);
		}
		m_FileType = DABFILE;
		m_DabFiles.LoadFiles();
		JodexDabFileDraw(0);
		UpdateDabFileList();
		//for(POSITION pos = m_DabFileList.GetHeadPosition(); pos != NULL; )
		//{
		//	TRACE("%s\n", m_DabFileList.GetNext(pos));
		//}
	}
}

void CSeismicSurveyDoc::JodexDabFileDraw(int fileno)
{
	CDabPacket * pDabPacket = m_DabFiles.m_DabPacketList.GetAt(fileno);
	m_SeismicDraw.m_GraphMode = 0;
	m_SeismicDraw.m_DataNo = pDabPacket->m_datano;
	m_SeismicDraw.m_ScanNo = pDabPacket->m_channo;
	m_SeismicDraw.m_DrawDataStart = 0;
	m_SeismicDraw.m_DrawDataEnd = pDabPacket->m_datano;
	m_SeismicDraw.m_pScanData = &(pDabPacket->m_ScanData);
	m_pActiveScanData = &(pDabPacket->m_ScanData);
	m_pActiveSigParameter = pDabPacket->m_pSigParameter;
	m_SelectedIndex = pDabPacket->m_SelectedChannel;
	m_DataReady = true;
	UpdateAllViews(NULL, NULL, NULL);
}

void CSeismicSurveyDoc::UpdateDabFileList()
{
	m_pFileTreeView->GetTreeCtrl().DeleteAllItems();
	HTREEITEM hRoot = m_pFileTreeView->GetTreeCtrl().InsertItem(_T("Dab 파일"), 0, 0);
	m_pFileTreeView->GetTreeCtrl().SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	
	for(POSITION pos = m_DabFiles.m_DabFileList.GetHeadPosition(); pos != NULL; )
	{
		CString tempString = m_DabFiles.m_DabFileList.GetNext(pos);
		CPathNameInfo tempPathInfo = CPathNameInfo(tempString);
		m_pFileTreeView->GetTreeCtrl().InsertItem(tempPathInfo.m_FileNameOnly, 1, 1, hRoot);
	}
	m_pFileTreeView->GetTreeCtrl().Expand(hRoot, TVE_EXPAND);
}

void CSeismicSurveyDoc::UpdateDabFileListThread(CMainFrame * pMainFrame)
{
	m_pFileTreeView->GetTreeCtrl().DeleteAllItems();
	HTREEITEM hRoot = m_pFileTreeView->GetTreeCtrl().InsertItem(_T("Dab 파일"), 0, 0);
	m_pFileTreeView->GetTreeCtrl().SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	
	for(POSITION pos = m_DabFiles.m_DabFileList.GetHeadPosition(); pos != NULL; )
	{
		CString tempString = m_DabFiles.m_DabFileList.GetNext(pos);
		CPathNameInfo tempPathInfo = CPathNameInfo(tempString);
		m_pFileTreeView->GetTreeCtrl().InsertItem(tempPathInfo.m_FileNameOnly, 1, 1, hRoot);
	}
	m_pFileTreeView->GetTreeCtrl().Expand(hRoot, TVE_EXPAND);
}
void CSeismicSurveyDoc::ClearDabFileList()
{
	m_pFileTreeView->GetTreeCtrl().DeleteAllItems();
	HTREEITEM hRoot = m_pFileTreeView->GetTreeCtrl().InsertItem(_T("Dab 파일"), 0, 0);
	m_pFileTreeView->GetTreeCtrl().SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
}

void CSeismicSurveyDoc::UpdateSeismicSurveyView()
{
	m_pSeismicSurveyView->Invalidate(true);
}

void CSeismicSurveyDoc::OnJodexDatfileopen()
{
	CSeismicSurveyApp* pApp = static_cast<CSeismicSurveyApp*>(::AfxGetApp());

	CSeismicSurveyDoc * pJodexDoc = static_cast<CSeismicSurveyDoc*>(pApp->m_pJodexDocTemplate->CreateNewDocument());
	CFrameWnd* p2DFrame = pApp->m_pJodexDocTemplate->CreateNewFrame(pJodexDoc, NULL);
	pApp->m_pJodexDocTemplate->InitialUpdateFrame(p2DFrame, pJodexDoc);
	pJodexDoc->InitDocument();
	pJodexDoc->JodexDatfileopen();

}
void CSeismicSurveyDoc::JodexDatfileopen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString str = _T("Jodex file format(*.dat)|*.dat|All Files(*.*)|*.*||");	// 선택할 파일 종류
	CString datPath;
	CPathNameInfo datPathInfo;
	CString strFileList; 

	CFileDialog dlg(TRUE, _T("DAT file open"), NULL, OFN_ALLOWMULTISELECT, str, NULL);
	const int c_cMaxFiles = 400 /*선택할 파일 숫자*/ ;	// 메모리 부족현상으로 확장 안해주면 몇개 못씀
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;

	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;

	if( dlg.DoModal() == IDOK)
	{
		//m_DatFiles.m_DatFileList.RemoveAll();
		//m_DatFiles.m_DatPacketList.RemoveAll();
		CString tempstring = dlg.GetFolderPath();
		m_CurrentFolder = tempstring;

		SetTitle(_T("Dat files in ") + tempstring);

		m_DatFiles.DeleteData();
		for(POSITION pos=dlg.GetStartPosition(); pos != NULL;)
		{
			// 전체삭제는 ResetContent

			datPath = dlg.GetNextPathName(pos);
			m_DatFiles.m_DatFileList.AddTail(datPath);
		}
 		m_FileType = DATFILE;
		m_DatFiles.LoadFiles();
		UpdateDatFileList();
		JodexDatFileDraw(0);
	}
}

void CSeismicSurveyDoc::GenerateDatFileFromDabDoc(CSeismicSurveyDoc * pDoc)
{
	int totalscanno = pDoc->m_DabFiles.m_DabPacketList.GetSize();
	float starttxdepth  = 0.0f;
	float endtxdepth = 0.0f;
	CString filename = _T("");

	m_CurrentFolder = pDoc->m_CurrentFolder;
	SetTitle(_T("Dat files in ") + m_CurrentFolder);

	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();

	m_pActiveSigParameter = &(pMainFrame->m_CommonSigParameter);
	for(int chindx = 0; chindx < 16; chindx++)
	{
		CDatPacket * pDatPacket = new CDatPacket;

		for(int scanindx = 0; scanindx < totalscanno; scanindx++)
		{
			CDabPacket * pDabPacket = pDoc->m_DabFiles.m_DabPacketList.GetAt(scanindx);
			CSeismicData * pScanData = pDabPacket->m_ScanData.GetAt(chindx);

			pScanData->ApplySignalProcessing(m_pActiveSigParameter);
			if(scanindx ==0)
			{
				pDatPacket->m_dt = pDabPacket->m_dt;
				pDatPacket->m_datano = pDabPacket->m_datano;
				pDatPacket->m_scanno = totalscanno;
				pDatPacket->m_SelectedIndex = 0;
				if(chindx ==0)
				{
					pDatPacket->startdepth = pScanData->m_TxDepth;
					starttxdepth = pDatPacket->startdepth;
				}
			}

			if(chindx ==0 && scanindx ==totalscanno-1)
			{
				pDatPacket->enddepth = pScanData->m_TxDepth;
				endtxdepth = pDatPacket->enddepth;
			}

			CSeismicData * data = new CSeismicData(pDabPacket->m_datano);
			float deltaant = -3.5f + (chindx * 0.5f);
			pDatPacket->startdepth = starttxdepth + deltaant;
			pDatPacket->enddepth = endtxdepth + deltaant;
			data->CopyFrom(pScanData);
			pDatPacket->m_ScanData.Add(data);
		}
		filename.Format("%s\\%04d-%04d_%02d.dat",m_CurrentFolder, (int)((starttxdepth+0.05)*10.0), (int)((endtxdepth+0.05)*10.0), chindx);
		m_DatFiles.SaveFile(filename, pDatPacket);
	}

	CMFCRibbonEdit* pEdit3 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pMainFrame->m_wndRibbonBar.FindByID(ID_RIB_STARTDEPTH_SPIN));
	pEdit3->EnableSpinButtons(0, totalscanno);
	pEdit3->SetEditText("0");

	CMFCRibbonEdit* pEdit4 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pMainFrame->m_wndRibbonBar.FindByID(ID_RIB_RANGEDEPTH_SPIN));
	pEdit4->EnableSpinButtons(0, totalscanno);
	pEdit4->SetEditText("100");

	m_DispScanNo = 100;
	m_SeismicDraw.m_DrawDataEnd = 512;


 	m_FileType = DATFILE;
	UpdateDatFileList();
	JodexDatFileDraw(0);
}
void CSeismicSurveyDoc::JodexDatFileDraw(int fileno)
{
	CDatPacket * pDatPacket = m_DatFiles.m_DatPacketList.GetAt(fileno);
	m_SeismicDraw.m_GraphMode = 1;
	m_SeismicDraw.m_DataNo = pDatPacket->m_datano;
	m_SeismicDraw.m_ScanNo = pDatPacket->m_scanno;
	m_SeismicDraw.m_DrawDataStart = 0;
	m_SeismicDraw.m_DrawDataEnd = 512;

	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CMFCRibbonEdit* pEdit0 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pMainFrame->m_wndRibbonBar.FindByID(ID_RIB_DRAWDATANO_SPIN));
	pEdit0->SetEditText("512");

	m_bProcessed = true;

	m_SeismicDraw.m_pScanData = &(pDatPacket->m_ScanData);
	m_pActiveScanData = &(pDatPacket->m_ScanData);
//	m_pActiveSigParameter = pDatPacket->m_pSigParameter;
	m_SelectedIndex = pDatPacket->m_SelectedIndex;
	m_DataReady = true;
	UpdateAllViews(NULL, NULL, NULL);
}

void CSeismicSurveyDoc::UpdateDatFileList()
{
	m_pFileTreeView->GetTreeCtrl().DeleteAllItems();
	HTREEITEM hRoot = m_pFileTreeView->GetTreeCtrl().InsertItem(_T("Dat 파일"), 0, 0);
	m_pFileTreeView->GetTreeCtrl().SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	
	for(POSITION pos = m_DatFiles.m_DatFileList.GetHeadPosition(); pos != NULL; )
	{
		CString tempString = m_DatFiles.m_DatFileList.GetNext(pos);
		CPathNameInfo tempPathInfo = CPathNameInfo(tempString);
		m_pFileTreeView->GetTreeCtrl().InsertItem(tempPathInfo.m_FileNameOnly, 1, 1, hRoot);
	}
	m_pFileTreeView->GetTreeCtrl().Expand(hRoot, TVE_EXPAND);
}

#include "SeismicSonde.h"
#include "MainFrm.h"

void CSeismicSurveyDoc::SpiritSDataDraw()
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CSeismicSonde * pSeismicRx = &(pMainFrame->m_SeismicRx);
	m_SeismicDraw.m_DataNo = pSeismicRx->m_SpiritSPacket.m_DataNo;
	m_SeismicDraw.m_ScanNo = pSeismicRx->m_ChanNo;
	m_SeismicDraw.m_DrawDataStart = 0;
	m_SeismicDraw.m_DrawDataEnd = pSeismicRx->m_SpiritSPacket.m_DataNo;
	m_SeismicDraw.m_pScanData = &(pSeismicRx->m_ChData);
	m_pActiveScanData =&(pSeismicRx->m_ChData);
	m_pActiveSigParameter = &(pSeismicRx->m_SigParameter);
	m_SelectedIndex = pSeismicRx->m_SelectedChannel;
	m_DataReady = true;
	UpdateAllViews(NULL, NULL, NULL);
}

void CSeismicSurveyDoc::SpiritSDataDrawThread(CMainFrame * pMainFrame)
{
	CSeismicSonde * pSeismicRx = &(pMainFrame->m_SeismicRx);
	m_SeismicDraw.m_DataNo = pSeismicRx->m_SpiritSPacket.m_DataNo;
	m_SeismicDraw.m_ScanNo = pSeismicRx->m_ChanNo;
	m_SeismicDraw.m_DrawDataStart = 0;
	m_SeismicDraw.m_DrawDataEnd = pSeismicRx->m_SpiritSPacket.m_DataNo;
	m_SeismicDraw.m_pScanData = &(pSeismicRx->m_ChData);
	m_pActiveScanData =&(pSeismicRx->m_ChData);
	m_pActiveSigParameter = &(pSeismicRx->m_SigParameter);
	m_SelectedIndex = pSeismicRx->m_SelectedChannel;
	m_DataReady = true;
	UpdateAllViews(NULL, NULL, NULL);
}

void CSeismicSurveyDoc::OnRibbonDetowBtn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CSeismicSurveyDoc::OnUpdateRibbonDetowBtn(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_pActiveSigParameter)
		pCmdUI->SetCheck(m_pActiveSigParameter->m_bBPFCorrelated);
}


void CSeismicSurveyDoc::OnJodexDabfilesaveButton()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	JodexDabfilesave();
}

void CSeismicSurveyDoc::OnJodexDatCsvfilesaveButton()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	JodexDatCsvfilesave(this);
}



void CSeismicSurveyDoc::JodexDabfilesave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	char extension[4] = "dab";
	char extension2[7] = "*.dab";
	char description[40] = "Jodex DAB data(*.dab)";
	CFileDialog dlg(FALSE, (LPCTSTR)extension, (LPCTSTR)extension2, NULL, (LPCTSTR)description, NULL);


	if (dlg.DoModal()==IDOK)
	{
		CString filename = dlg.GetPathName();
		CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
		CSeismicSonde * pSeismicRx = &(pMainFrame->m_SeismicRx);
		m_DabFiles.SaveFile(filename, pSeismicRx, 1);

		UpdateDabFileList();
	}

}

void CSeismicSurveyDoc::JodexDatCsvfilesave(CSeismicSurveyDoc* pDoc)
{
	int totalscanno = 0;
	float starttxdepth = 0.0f;
	float endtxdepth = 0.0f;
	float samplingtime = 0.0;
	CString filename;

	//CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	//m_pActiveSigParameter = &(pMainFrame->m_CommonSigParameter);

	for (int chindx = 0; chindx < 16; chindx++)
	{
		CDatPacket* pDatPacket = pDoc->m_DatFiles.m_DatPacketList.GetAt(chindx);
		starttxdepth = pDatPacket->startdepth;
		endtxdepth = pDatPacket->enddepth;
		totalscanno = pDatPacket->m_scanno * pDatPacket->m_datano;

		//for (int scanindx = 0; scanindx < totalscanno; scanindx++)
		//{
		//	
		//	if (scanindx == 0)
		//	{
		//		starttxdepth = pDoc->m_SeismicDraw.m_YStep;
				samplingtime = pDatPacket->m_dt;
		//	}

		//	if (scanindx == totalscanno - 1)
		//	{
		//		endtxdepth = pDoc->m_SeismicDraw.m_YStep;
		//	}
		//}
		filename.Format(_T("%s\\%04d-%04d_%02d_processed.csv"), m_CurrentFolder, (int)((starttxdepth + 0.05) * 10.0), (int)((endtxdepth + 0.05) * 10.0), chindx);
		int startd = (int)((starttxdepth + 0.05) * 10.0);
		int endd = (int)((endtxdepth + 0.05) * 10.0);
		m_DatFiles.SaveCsvFile(filename, pDatPacket, startd, endd, chindx, pDatPacket->m_dt);
	}

}

void CSeismicSurveyDoc::OnGendatfileButton()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->m_CommonSigParameter.Copy(m_pActiveSigParameter);

	CSeismicSurveyApp* pApp = static_cast<CSeismicSurveyApp*>(::AfxGetApp());

	CSeismicSurveyDoc * pJodexDoc = static_cast<CSeismicSurveyDoc*>(pApp->m_pJodexDocTemplate->CreateNewDocument());
	CFrameWnd* p2DFrame = pApp->m_pJodexDocTemplate->CreateNewFrame(pJodexDoc, NULL);
	pApp->m_pJodexDocTemplate->InitialUpdateFrame(p2DFrame, pJodexDoc);
	pJodexDoc->InitDocument();
	pJodexDoc->GenerateDatFileFromDabDoc(this);
}


void CSeismicSurveyDoc::OnDcCoupledCheck()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CSeismicSurveyDoc::OnBscanAutoscaleCheck()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	m_bViewAutoScale = !m_bViewAutoScale;

	if(m_bViewAutoScale)
	{
		m_ViewScale = 0.0;
	}
	else
	{
		m_ViewScale = pow(2.0, 21.0);
	}
	UpdateAllViews(NULL, NULL, NULL);
}


void CSeismicSurveyDoc::OnUpdateBscanAutoscaleCheck(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	pCmdUI->SetCheck(m_bViewAutoScale);
}


void CSeismicSurveyDoc::OnViewscaleSlider()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*) AfxGetMainWnd())->GetRibbonBar(); 
	ASSERT_VALID(pRibbon);

	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST( CMFCRibbonSlider, pRibbon->FindByID(ID_VIEWSCALE_SLIDER)); 
	// Get current position 
	int position =pSlider->GetPos();

	TRACE("position = %d\n", position);

	//CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	//CSeismicSonde * pSeismicRx = &(pMainFrame->m_SeismicRx);
	//int id = pSeismicRx->m_AccumulID[0];

	//m_ViewScale = pow(2.0, 20.0+ id) * ((float)position/100.0);
	m_ViewScale = pow(2.0, position);

	UpdateAllViews(NULL, NULL, NULL);
}

#include "CorrelationDlg.h"

void CSeismicSurveyDoc::OnRibbonCorrelationButton()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCorrelationDlg dlg;

	if(m_pActiveSigParameter)
	{
		dlg.m_PRBSPath = m_pActiveSigParameter->m_PRBSCodePath;
		dlg.m_DutyCycle = m_pActiveSigParameter->m_DutyCycle;

		if(dlg.DoModal() == IDOK)
		{
			CString temppath = dlg.m_PRBSPath;
			int dutycycle = dlg.m_DutyCycle;

			m_pActiveSigParameter->ChangeCorrelationParameter(temppath, dutycycle);

			for(int sindx = 0; sindx<m_pActiveScanData->GetSize(); sindx++)
			{
				CSeismicData * pData = m_pActiveScanData->GetAt(sindx);
				pData->MakeCorrelation(m_bProcessed, m_pActiveSigParameter->m_PRBSCode);
			}

			m_bProcessed = true;
			UpdateAllViews(NULL, NULL, NULL);
		}
	}
	else
	{
		AfxMessageBox(_T("Signal Processing Parameter is not defined!"));
	}

}

#include "IIRFilterDlg.h"

void CSeismicSurveyDoc::OnRibbonIIRFilterBtn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CIIRFilterDlg dlg;

	if(m_pActiveSigParameter)
	{
		dlg.m_SamplingFreq = m_pActiveSigParameter->m_SamplingFreq;
		dlg.m_HPFFreq = m_pActiveSigParameter->m_HPFCutoffFreq;
		dlg.m_LPFFreq = m_pActiveSigParameter->m_LPFCutoffFreq;

		if(dlg.DoModal() == IDOK)
		{
			float SamplingFreq = dlg.m_SamplingFreq;
			float HPFCutoffFreq = dlg.m_HPFFreq;
			float LPFCutoffFreq = dlg.m_LPFFreq;
			m_pActiveSigParameter->ChangeIIRFilterCoeff(SamplingFreq, HPFCutoffFreq, LPFCutoffFreq);
			for(int sindx = 0; sindx<m_pActiveScanData->GetSize(); sindx++)
			{
				CSeismicData * pData = m_pActiveScanData->GetAt(sindx);
				pData->ApplyIIRFilter(m_bProcessed, m_pActiveSigParameter);
			}

			m_bProcessed = true;
			UpdateAllViews(NULL, NULL, NULL);
		}
	}
	else
	{
		AfxMessageBox(_T("Signal Processing Parameter is not defined!"));
	}

}

#include "Contour\ContourDoc.h"

void CSeismicSurveyDoc::OnTomographgenButton()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSeismicSurveyApp* pApp = static_cast<CSeismicSurveyApp*>(::AfxGetApp());

	CContourDoc* p2DDoc = static_cast<CContourDoc*>(pApp->m_pContourTemplate->CreateNewDocument());
//	p2DDoc->InitFromGridFile(filename);
	CFrameWnd* p2DFrame = pApp->m_pContourTemplate->CreateNewFrame(p2DDoc, NULL);
	pApp->m_pContourTemplate->InitialUpdateFrame(p2DFrame, p2DDoc);


}



void CSeismicSurveyDoc::OnRibDrawstartSpin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	
	CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*) AfxGetMainWnd())->GetRibbonBar();

	//CMFCRibbonSpinButtonCtrl* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSpinButtonCtrl, pRibbonStatusBar->find FindByID(ID_RIB_DRAWSTART_SPIN));
	////ASSERT_VALID(pSlider); ////////////////
	//CMFCRibbonSpinButtonCtrl * pSlider = pMainFrame->m_pSlider;
	//int nPos = pSlider->GetPos();

	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pRibbon->FindByID(ID_RIB_DRAWSTART_SPIN));

	LPCTSTR str = pEdit->GetEditText();
	if (!str)
		return;
	int i = _ttoi(str);

	int remain = i%10;
	int value = i/10;
	if (remain == 1)
	{
		m_SeismicDraw.m_DrawDataStart = (value+1)*10;
		CString tempstring;
		tempstring.Format("%d", m_SeismicDraw.m_DrawDataStart);
		pEdit->SetEditText(_T(tempstring));
	}
	if (remain == 9)
	{
		m_SeismicDraw.m_DrawDataStart = (value)*10;
		CString tempstring;
		tempstring.Format("%d", m_SeismicDraw.m_DrawDataStart);
		pEdit->SetEditText(_T(tempstring));
	}

	UpdateAllViews(NULL, NULL, NULL);
}


void CSeismicSurveyDoc::OnRibDrawdatanoSpin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*) AfxGetMainWnd())->GetRibbonBar();

	//CMFCRibbonSpinButtonCtrl* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSpinButtonCtrl, pRibbonStatusBar->find FindByID(ID_RIB_DRAWSTART_SPIN));
	////ASSERT_VALID(pSlider); ////////////////
	//CMFCRibbonSpinButtonCtrl * pSlider = pMainFrame->m_pSlider;
	//int nPos = pSlider->GetPos();

	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pRibbon->FindByID(ID_RIB_DRAWDATANO_SPIN));

	CString str = pEdit->GetEditText();

	str.Remove(',');

	if (!str)
		return;
	int i = _ttoi(str);

	int remain = i%256;
	int value = i/256;
	if (remain == 1)
	{
		m_SeismicDraw.m_DrawDataEnd = (value+1)*256;
		CString tempstring;
		tempstring.Format("%d", m_SeismicDraw.m_DrawDataEnd);
		pEdit->SetEditText(_T(tempstring));
	}
	if (remain == 255)
	{
		m_SeismicDraw.m_DrawDataEnd = (value)*256;
		CString tempstring;
		tempstring.Format("%d", m_SeismicDraw.m_DrawDataEnd);
		pEdit->SetEditText(_T(tempstring));
	}

	UpdateAllViews(NULL, NULL, NULL);
}


void CSeismicSurveyDoc::OnRibProcessedDispCheck()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bProcessed = !m_bProcessed;
	UpdateAllViews(NULL, NULL, NULL);
}


void CSeismicSurveyDoc::OnUpdateRibProcessedDispCheck(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bProcessed);
}


void CSeismicSurveyDoc::OnUpdateRibbonCorrelationButton(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CSeismicSurveyDoc::OnRibStartDepthSpin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CSeismicSurveyDoc::OnRibRangeDepthSpin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	
	CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*) AfxGetMainWnd())->GetRibbonBar();

	//CMFCRibbonSpinButtonCtrl* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSpinButtonCtrl, pRibbonStatusBar->find FindByID(ID_RIB_DRAWSTART_SPIN));
	////ASSERT_VALID(pSlider); ////////////////
	//CMFCRibbonSpinButtonCtrl * pSlider = pMainFrame->m_pSlider;
	//int nPos = pSlider->GetPos();

	CMFCRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pRibbon->FindByID(ID_RIB_RANGEDEPTH_SPIN));

	LPCTSTR str = pEdit->GetEditText();
	if (!str)
		return;
	int i = _ttoi(str);

	m_DispScanNo = i;

	UpdateAllViews(NULL, NULL, NULL);
}


void CSeismicSurveyDoc::OnRibAllDepthCheck()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
