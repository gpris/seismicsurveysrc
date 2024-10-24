// FileTreeView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"

#include "SeismicSurveyDoc.h"
#include "FileTreeView.h"

// CFileTreeView

IMPLEMENT_DYNCREATE(CFileTreeView, CTreeView)

CFileTreeView::CFileTreeView()
{

}

CFileTreeView::~CFileTreeView()
{
}

BEGIN_MESSAGE_MAP(CFileTreeView, CTreeView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CFileTreeView 진단입니다.

#ifdef _DEBUG
void CFileTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

CSeismicSurveyDoc* CFileTreeView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeismicSurveyDoc)));
	return (CSeismicSurveyDoc*)m_pDocument;
}


#ifndef _WIN32_WCE
void CFileTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileTreeView 메시지 처리기입니다.


void CFileTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
  
	GetTreeCtrl().ModifyStyle(NULL , TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);


	// 뷰 이미지를 로드합니다.
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	GetTreeCtrl().SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	OnChangeVisualStyle();
	FillFileView();
//	InitClassic();
}

void CFileTreeView::InitClassic()
{
 

    //TVINSERTSTRUCT tvi;
    //tvi.hParent         = TVI_ROOT;
    //tvi.hInsertAfter    = TVI_LAST;
    //tvi.item.mask       = TVIF_TEXT;
    //tvi.item.pszText    = "가족";
    //HTREEITEM hItem = m_wndFileView.InsertItem(&tvi);
 
    //tvi.hParent         = hItem;
    //tvi.hInsertAfter    = TVI_LAST;
    //tvi.item.mask       = TVIF_TEXT;
    //tvi.item.pszText    = "엄마";
    //m_wndFileView.InsertItem(&tvi);
 
    //tvi.hParent         = hItem;
    //tvi.hInsertAfter    = TVI_LAST;
    //tvi.item.mask       = TVIF_TEXT;
    //tvi.item.pszText    = "아빠";
    //m_wndFileView.InsertItem(&tvi);
 
    //tvi.hParent         = hItem;
    //tvi.hInsertAfter    = TVI_LAST;
    //tvi.item.mask       = TVIF_TEXT;
    //tvi.item.pszText    = "형";
    //m_wndFileView.InsertItem(&tvi);
 
    //tvi.hParent         = hItem;
    //tvi.hInsertAfter    = TVI_LAST;
    //tvi.item.mask       = TVIF_TEXT;
    //tvi.item.pszText    = "나";
    //m_wndFileView.InsertItem(&tvi);
}

void CFileTreeView::FillFileView()
{
	HTREEITEM hRoot = GetTreeCtrl().InsertItem(_T("Dab 파일"), 0, 0);
	GetTreeCtrl().SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	//HTREEITEM hSrc = GetTreeCtrl().InsertItem(_T("FakeApp 소스 파일"), 0, 0, hRoot);

	//GetTreeCtrl().InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	//GetTreeCtrl().InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	//GetTreeCtrl().InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	//GetTreeCtrl().InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	//GetTreeCtrl().InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	//GetTreeCtrl().InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	//HTREEITEM hInc = GetTreeCtrl().InsertItem(_T("FakeApp 헤더 파일"), 0, 0, hRoot);

	//GetTreeCtrl().InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	//GetTreeCtrl().InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	//GetTreeCtrl().InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	//GetTreeCtrl().InsertItem(_T("Resource.h"), 2, 2, hInc);
	//GetTreeCtrl().InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	//GetTreeCtrl().InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	//HTREEITEM hRes = GetTreeCtrl().InsertItem(_T("FakeApp 리소스 파일"), 0, 0, hRoot);

	//GetTreeCtrl().InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	//GetTreeCtrl().InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	//GetTreeCtrl().InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	//GetTreeCtrl().InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	GetTreeCtrl().Expand(hRoot, TVE_EXPAND);
	//GetTreeCtrl().Expand(hSrc, TVE_EXPAND);
	//GetTreeCtrl().Expand(hInc, TVE_EXPAND);
}

int CFileTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//CRect rectDummy;
	//rectDummy.SetRectEmpty();

	//// 뷰를 만듭니다.
	//const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	//if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 5))
	//{
	//	TRACE0("파일 뷰를 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	//m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	//FillFileView();
	//AdjustLayout();

	return 0;
}

void CFileTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	GetTreeCtrl().SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileTreeView::OnChangeVisualStyle()
{
	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("비트맵을 로드할 수 없습니다. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	GetTreeCtrl().SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

//void CFileTreeView::OnHistoryOpen()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	int i = 0;
//	HTREEITEM hItem;
//	 hItem = m_wndFileView.GetSelectedItem();
//	 CString temp = GetPathStringSelectedItem(hItem);
//
//
//}

CString CFileTreeView::GetPathStringSelectedItem(HTREEITEM hItem)
{
     CString strParentName;
     CString strFullName;
 
     strFullName = GetTreeCtrl().GetItemText(hItem)+ '\\';
     while ((hItem = GetTreeCtrl().GetParentItem(hItem)) != NULL)
     {
          strParentName = GetTreeCtrl().GetItemText(hItem);
          strFullName = strParentName + '\\' + strFullName;
     }

     return strFullName;

}

#include "PathNameInfo\PathNameInfo.h"

#define DABFILE 0
#define DATFILE 1

void CFileTreeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	HTREEITEM hItem;
	hItem = GetTreeCtrl().GetSelectedItem();
	CString selectedString = GetTreeCtrl().GetItemText(hItem);

	CSeismicSurveyDoc * pDoc = GetDocument();
	int indx;

	if (pDoc->m_FileType == DABFILE)
	{
		indx = pDoc->m_DabFiles.GetIndexFromFilename(selectedString);
		pDoc->JodexDabFileDraw(indx);
		//int indx = 0;
		//for(POSITION pos = pDoc->m_DabFiles.m_DabFileList.GetHeadPosition(); pos != NULL; )
		//{
		//	CString tempString;

		//	tempString = pDoc->m_DabFiles.m_DabFileList.GetNext(pos);
		//	CPathNameInfo tempPathInfo = CPathNameInfo(tempString);
		//	if(selectedString == tempPathInfo.m_FileNameOnly)
		//	{
		//		pDoc->JodexDabFileDraw(indx);
		//		break;
		//	}
		//}
		//indx++;
	}
	else
	{
		indx = pDoc->m_DatFiles.GetIndexFromFilename(selectedString);
		pDoc->JodexDatFileDraw(indx);
	}
	 
//	 CString temp = GetPathStringSelectedItem(hItem);

	CTreeView::OnLButtonDblClk(nFlags, point);
}
