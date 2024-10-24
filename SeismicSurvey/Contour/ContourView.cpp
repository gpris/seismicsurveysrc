// ContourView.cpp : implementation file
//

#include "stdafx.h"
#include "..\SeismicSurvey.h"
#include "ContourView.h"
#include "InverseDist.h"
#include "..\MainFrm.h"
#include "Kriging.h"
#include <algorithm>
#include <fstream>
#include "Contour.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static COLORREF FillColors[20];

const int FLAT_HEIGHT = 600;
const int LEGEND_WIDTH = 200;

static COLORREF Colors[20] = { RGB(0, 255, 150), RGB(0, 255, 200), RGB(0, 255, 255), RGB(0, 200, 255), RGB(0, 150, 255), 
								RGB(0, 100, 255), RGB(0, 50, 255), RGB(0, 0, 255), RGB(255, 0, 200), RGB(255, 0, 150), 
								RGB(85, 0, 100), RGB(170, 0, 50), RGB(255, 0, 0), RGB(255, 50, 0), RGB(255, 100, 0), 
								RGB(255, 150, 0), RGB(255, 200, 0), RGB(255, 255, 0), RGB(200, 255, 0), RGB(150, 255, 0) };

/////////////////////////////////////////////////////////////////////////////
// CContourView

IMPLEMENT_DYNCREATE(CContourView, CScrollView)

CContourView::CContourView() : m_pImage(NULL), m_bImageReady(false), m_nZoom(10), m_nBaseWidth(208), m_nBaseHeight(208),
m_nNumOfLevels(11), m_bLegend(true), m_bValues(true), m_dThickMax(0), m_dThickMin(0)
{
	m_bContourlines = true;
	m_bImage = true;
	m_bDrawLanes = true;
//	m_bRedraw = true;

	m_xmargin = 50;
	m_ymargin = 30;

	m_xaxis = NULL;
	m_yaxis = NULL;

	m_bZoomFitMode = true;

	for (int indx = 0; indx<20; indx++)
	{
		float colorstep = 255/20;
		int colorvalue = 255-(int)(indx * colorstep);
		FillColors[indx] = RGB(colorvalue, colorvalue, colorvalue);
	}
}

CContourView::~CContourView()
{
	if(m_pImage)
		delete m_pImage;
}


BEGIN_MESSAGE_MAP(CContourView, CScrollView)
	//{{AFX_MSG_MAP(CContourView)
	//ON_COMMAND(ID_SHOWVALUES, OnShowvalues)
	//ON_COMMAND(ID_SHOWLEGEND, OnShowlegend)
	//ON_UPDATE_COMMAND_UI(ID_SHOWVALUES, OnUpdateShowvalues)
	//ON_UPDATE_COMMAND_UI(ID_SHOWLEGEND, OnUpdateShowlegend)
	//////////////////////ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//////////////////////ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//////////////////////ON_WM_SIZE()
	//////////////////////ON_COMMAND(ID_STATUSBAR_ZOOM_SLIDER, &CContourView::OnZoomSlider2)
	//////////////////////ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CContourView::OnFilePrintPreview)
	//////////////////////ON_COMMAND(ID_BUTTON_SAVE_IMAGE,&CContourView::OnSaveImageAs) // Sanghyun 103114 for Save image as
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_SHOWCONTOURLINES, &CContourView::OnShowcontourlines)
	//ON_UPDATE_COMMAND_UI(ID_SHOWCONTOURLINES, &CContourView::OnUpdateShowcontourlines)
	//ON_COMMAND(ID_OPTION_SHOWIMAGE, &CContourView::OnOptionShowimage)
	//ON_UPDATE_COMMAND_UI(ID_OPTION_SHOWIMAGE, &CContourView::OnUpdateOptionShowimage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContourView drawing

void CContourView::OnDraw(CDC* pDC)
{
	CContourDoc* pDoc = (CContourDoc*)GetDocument();

	if(m_bZoomFitMode)
	{
		CRect rect;
		GetClientRect(&rect);

//		m_nZoom = (rect.Width() - 2*m_xmargin)/(m_xaxis[pDoc->m_GridPacket.m_nCol-1] - m_xaxis[0]);
		m_nZoom = (rect.Width() - 2*m_xmargin)/(pDoc->m_XDrawEnd - pDoc->m_XDrawStart);
	}

	//m_CanvasSize.cx = int((m_xaxis[pDoc->m_GridPacket.m_nCol-1] - m_xaxis[0])*m_nZoom) + 2*m_xmargin;
	//m_CanvasSize.cy = int((m_yaxis[pDoc->m_GridPacket.m_nRow-1] - m_yaxis[0])*m_nZoom) + 2*m_ymargin;
	m_CanvasSize.cx = int((pDoc->m_XDrawEnd - pDoc->m_XDrawStart)*m_nZoom) + 2*m_xmargin;
	m_CanvasSize.cy = int((pDoc->m_YDrawEnd - pDoc->m_YDrawStart)*m_nZoom) + 2*m_ymargin;

    // 메모리 DC 선언
    CDC memDC;
    CBitmap *pOldBitmap, bitmap;
 
    // 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);
 
    // 마찬가지로 화면 DC와 호환되는 Bitmap 생성
    bitmap.CreateCompatibleBitmap(pDC, m_CanvasSize.cx, m_CanvasSize.cy);
 
    pOldBitmap = memDC.SelectObject(&bitmap);
    memDC.PatBlt(0, 0, m_CanvasSize.cx, m_CanvasSize.cy, WHITENESS); // 흰색으로 초기화
 
    // 메모리 DC에 그리기
	//if(pDoc->m_bRedraw)
	//{
//		DrawContourLinesHS(pDC);
		DrawContour(pDC);
		pDoc->m_bRedraw = false;
		//switch (pDoc->m_nPropId)
		//{
		//case 0:
  //          pDoc->FillUpGeneralProperty();
  //          break;
		//case 1:
  //          pDoc->FillUpProperty();
  //          break;
  //      case 2:
  //          pDoc->FillUpDispProperty();
  //          break;
		//}
	//}
	CBitmap* pOldBmp;
	pOldBmp = memDC.SelectObject(&m_Bitmap);

//	DrawValues(&memDC);

	m_bImageReady = true;
 
    // 메모리 DC를 화면 DC에 고속 복사
    pDC->BitBlt(0, 0, m_CanvasSize.cx, m_CanvasSize.cy, &memDC, 0, 0, SRCCOPY);
 
    memDC.SelectObject(pOldBitmap);
    memDC.DeleteDC();
    bitmap.DeleteObject();

	DrawAxis(pDC);
	// TODO: add draw code here
	//if(m_bImageReady) {
	//	if(m_bImage)
	//	{
	//		try {
	//			m_pImage->Draw(pDC, 0, 0, m_nZoom);
	//		} catch(CImageException& e) {
	//			::AfxMessageBox(e.what());
	//			m_bImageReady = false;
	//		}
	//	}

	//	if(m_bValues)
	//		DrawValues(pDC);

	//	if(m_bLegend)
	//		DrawLegend(pDC);

	//	if(m_bContourlines)
	//		DrawContourLines(pDC);
	//}
}

/////////////////////////////////////////////////////////////////////////////
// CContourView diagnostics

#ifdef _DEBUG
void CContourView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CContourView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CContourDoc* CContourView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CContourDoc)));
	return (CContourDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CContourView message handlers

void CContourView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

    GetParent()->SetWindowText(GetDocument()->GetTitle()+"-2D Contour");

	CContourDoc* pDoc = GetDocument();

	m_data2d = pDoc->m_GridPacket.m_Data;

	m_xaxis = new double[pDoc->m_GridPacket.m_nCol];
	m_yaxis = new double[pDoc->m_GridPacket.m_nRow];

	m_nNumOfLevels = 19;
	m_levels = new double[m_nNumOfLevels];

	for(int col = 0; col<pDoc->m_GridPacket.m_nCol; col++)
	{
		m_xaxis[col] = pDoc->m_GridPacket.m_xLL + col * pDoc->m_GridPacket.m_xSize;
	}

	for(int row = 0; row<pDoc->m_GridPacket.m_nRow; row++)
	{
		m_yaxis[row] = pDoc->m_GridPacket.m_yLL + row * pDoc->m_GridPacket.m_ySize;
	}

	double levelstep = (pDoc->m_zMax - pDoc->m_zMin)/(m_nNumOfLevels - 1);
//	double levelstep = (18 - 0)/(m_nNumOfLevels - 1);
	for(int level = 0; level<m_nNumOfLevels; level++)
	{
//		m_levels[level] = pDoc->m_zMin + levelstep * level;
		m_levels[level] = 0 + levelstep * level;
	}

	
//swkim : CView에서 CScrollView로 바꿀때 반드시 여기에 삽입해 줘야 한다.
	m_CanvasSize.cx = int((m_xaxis[pDoc->m_GridPacket.m_nCol-1] - m_xaxis[0])*m_nZoom) + 2*m_xmargin;
	m_CanvasSize.cy = int((m_yaxis[pDoc->m_GridPacket.m_nRow-1] - m_yaxis[0])*m_nZoom) + 2*m_ymargin;
	SetScrollSizes(MM_TEXT, m_CanvasSize);

}

void CContourView::ResizeFrame()
{
	CMDIChildWnd* pChild = static_cast<CMDIChildWnd*>(GetParent());
	int nWidth = m_nBaseWidth * m_nZoom + 2*(::GetSystemMetrics(SM_CXSIZEFRAME)+::GetSystemMetrics(SM_CXEDGE));
	int nHeight = m_nBaseHeight * m_nZoom + 2*(::GetSystemMetrics(SM_CYSIZEFRAME)+::GetSystemMetrics(SM_CYEDGE))
		+ ::GetSystemMetrics(SM_CYCAPTION);
	if(m_bLegend)
		nWidth += LEGEND_WIDTH;

	WINDOWPLACEMENT wp;
	pChild->GetWindowPlacement(&wp);
	pChild->MoveWindow(wp.rcNormalPosition.left, wp.rcNormalPosition.top, nWidth, nHeight);
}

void CContourView::DrawValues(CDC* pDC)
{
	CContourDoc* pDoc = GetDocument();
	vector<Point3D>& input = const_cast<vector<Point3D>&>(pDoc->m_ir.Get3DPoints());

	CBrush br;
	br.CreateSolidBrush(RGB(0, 0, 0));
	CBrush* pOld;
	pOld = pDC->SelectObject(&br);
	CString thickness;
	pDC->SetBkMode(TRANSPARENT);
	for(int i=0; i<input.size(); i++) {
		//int x1 = (int)(m_nZoom*(input[i].x+m_nBaseWidth/2-1));
		//int y1 = (int)(m_nZoom*(0.5 * m_nBaseHeight-input[i].y+1));
		//int x2 = (int)(m_nZoom*(input[i].x+m_nBaseWidth/2+1));
		//int y2 = (int)(m_nZoom*(0.5 * m_nBaseHeight-input[i].y-1));
		int x0 = (int)(m_nZoom*(input[i].x-m_xaxis[0]) + m_xmargin);
		int y0 = (int)(m_nZoom*(m_yaxis[pDoc->m_GridPacket.m_nRow-1] -input[i].y) + m_ymargin);
		int x1 = (int)(m_nZoom*(input[i].x-m_xaxis[0])-3 + m_xmargin);
		int y1 = (int)(m_nZoom*(m_yaxis[pDoc->m_GridPacket.m_nRow-1] -input[i].y)+3 + m_ymargin);
		int x2 = (int)(m_nZoom*(input[i].x+m_xaxis[0])+3 + m_xmargin);
		int y2 = (int)(m_nZoom*(m_yaxis[pDoc->m_GridPacket.m_nRow-1]-input[i].y)-3 + m_ymargin);

		pDC->MoveTo(x0, y1);
		pDC->LineTo(x0, y2);

		pDC->MoveTo(x1, y0);
		pDC->LineTo(x2, y0);

//		pDC->Ellipse(x1, y1, x2, y2);
//		thickness.Format(_T("%4.2f"), input[i].z);
//		pDC->TextOut(x1-20, y1+4, thickness);
	}
	pDC->SelectObject(pOld);
}

void CContourView::DrawLegend(CDC* pDC)
{
	const int RECT_HEIGHT = 18;
	const int RECT_WIDTH = 40;

	int x1 = m_nZoom * m_nBaseWidth + 10;
	pDC->Rectangle(x1, 16, x1+164, (m_nNumOfLevels+1)*(RECT_HEIGHT+2));
	CString thickness;
	double interval = (m_dThickMax - m_dThickMin) / m_nNumOfLevels;
	for(int i=0; i<m_nNumOfLevels; i++) {
		CBrush br;
		br.CreateSolidBrush(Colors[i]);
		pDC->SelectObject(&br);
		pDC->Rectangle(x1+3, 20+i*20, x1+RECT_WIDTH, 30+i*20);
		thickness.Format(_T("%4.2f - %4.2f"), m_dThickMin+(i*interval), m_dThickMin+((i+1)*interval));
		pDC->TextOut(x1+RECT_WIDTH+4, 20+i*20, thickness);
	}
}

void CContourView::OnShowvalues() 
{
	// TODO: Add your command handler code here
	m_bValues = !m_bValues;
	Invalidate();
}

void CContourView::OnShowlegend() 
{
	// TODO: Add your command handler code here
	m_bLegend = !m_bLegend;
	Invalidate();
	ResizeFrame();
}

void CContourView::OnUpdateShowvalues(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bValues);
}

void CContourView::OnUpdateShowlegend(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bLegend);
}

void CContourView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	if(!m_bImageReady)
		return;
	CWaitCursor wait;

	CClientDC scrDC(this);
	CDC memDC;           // screen DC and memory DC
	CBitmap bitmap;
	CBitmap* pOldBitmap;  // handles to deice-dependent bitmaps

	CRect rect;
	GetClientRect(&rect);

	memDC.CreateCompatibleDC(&scrDC);
	/* create a bitmap compatible with the screen DC */
//	bitmap.CreateCompatibleBitmap(&scrDC, rect.Width(), rect.Height());
	bitmap.CreateCompatibleBitmap(&scrDC, m_CanvasSize.cx, m_CanvasSize.cy);

	pOldBitmap = memDC.SelectObject(&bitmap);// select new bitmap into memory DC

    memDC.PatBlt(0, 0, m_CanvasSize.cx, m_CanvasSize.cy, WHITENESS); // 흰색으로 초기화
 
    // 메모리 DC에 그리기
//		DrawContourLinesHS(&scrDC);

		CBitmap* pOldBmp;
	pOldBmp = memDC.SelectObject(&m_Bitmap);

//	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &scrDC, 0, 0, SRCCOPY);
//	memDC.BitBlt(0, 0, m_CanvasSize.cx, m_CanvasSize.cy, &scrDC, 0, 0, SRCCOPY);
	HBITMAP hBitmap = (HBITMAP)m_Bitmap.Detach();

	memDC.SelectObject(pOldBitmap);

	// Place the copy on the clipboard.
	::OpenClipboard(m_hWnd);
    ::EmptyClipboard();
    ::SetClipboardData(CF_BITMAP, hBitmap);
    ::CloseClipboard();

	// clean up 
	memDC.DeleteDC();
}

void CContourView::OnFileSave() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;

	CClientDC scrDC(this);
	CDC memDC;           // screen DC and memory DC
	CBitmap bitmap;
	CBitmap* pOldBitmap;  // handles to deice-dependent bitmaps

	CRect rect;
	GetClientRect(&rect);

	memDC.CreateCompatibleDC(&scrDC);
//	bitmap.CreateCompatibleBitmap(&scrDC, rect.Width(), rect.Height());
	bitmap.CreateCompatibleBitmap(&scrDC, m_CanvasSize.cx, m_CanvasSize.cy);

	pOldBitmap = memDC.SelectObject(&m_Bitmap);// select new bitmap into memory DC



//	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &scrDC, 0, 0, SRCCOPY);
	memDC.BitBlt(0, 0, m_CanvasSize.cx, m_CanvasSize.cy, &scrDC, 0, 0, SRCCOPY);

	TCHAR szFilters[] = _T ("bmp files (*.bmp)|*.bmp|All files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T ("bmp"), _T ("*.bmp"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilters);
	
	CString filename;
	if(dlg.DoModal() == IDOK)
		filename = dlg.GetPathName();
	else
		return;
 
	CDib dib;
	try {
		dib.Create(&m_Bitmap);
		dib.Save(filename);
	} catch(CImageException& e) {
		::AfxMessageBox(e.what());
	}

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	CContourDoc* pDoc = GetDocument();
	pDoc->SetTitle(dlg.GetFileName());
}

/*
   Derivation from the fortran version of CONREC by Paul Bourke
   d               ! matrix of data to contour
   ilb,iub,jlb,jub ! index bounds of data matrix
   x               ! data matrix column coordinates
   y               ! data matrix row coordinates
   nc              ! number of contour levels
   z               ! contour levels in increasing order
*/
void CContourView::Contour(double **d,int ilb,int iub,int jlb,int jub,
   double *x,double *y,int nc,double *z,
   CDC * pDC)
{
#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])
#define MIN(X,Y) ((X) < (Y) ?  (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ?  (X) : (Y))

   int m1,m2,m3,case_value;
   double dmin,dmax,x1=0,x2=0,y1=0,y2=0;
   int i,j,k,m;
   double h[5];
   int sh[5];
   double xh[5],yh[5];
   int im[4] = {0,1,1,0},jm[4]={0,0,1,1};
   int castab[3][3][3] = {
     { {0,0,8},{0,2,5},{7,6,9} },
     { {0,3,4},{1,3,1},{4,3,0} },
     { {9,6,7},{5,2,0},{8,0,0} }
   };
   double temp1,temp2;

   int nPolygon = 0;
   Point2D * tempPoint = new Point2D[4];

   for (k=0;k<nc;k++) 
   {

	   for (j=(jub-1);j>=jlb;j--) 
	   {
			for (i=ilb;i<=iub-1;i++) 
			{
//				TRACE("j = %d, i = %d\n", j, i);
				//temp1 = MIN(d[i][j],d[i][j+1]);
				//temp2 = MIN(d[i+1][j],d[i+1][j+1]);
				//dmin  = MIN(temp1,temp2);
				//temp1 = MAX(d[i][j],d[i][j+1]);
				//temp2 = MAX(d[i+1][j],d[i+1][j+1]);
				//dmax  = MAX(temp1,temp2);
				//if (dmax < z[0] || dmin > z[nc-1])
				//	continue;
				//if (z[k] < dmin || z[k] > dmax)
				//	continue;
				for (m=4;m>=0;m--) 
				{
				   if (m > 0) {
					  h[m]  = d[i+im[m-1]][j+jm[m-1]]-z[k];
					  xh[m] = x[i+im[m-1]];
					  yh[m] = y[j+jm[m-1]];
				   } else {
					  h[0]  = 0.25 * (h[1]+h[2]+h[3]+h[4]);
					  xh[0] = 0.50 * (x[i]+x[i+1]);
					  yh[0] = 0.50 * (y[j]+y[j+1]);
				   }
				   if (h[m] > 0.0)
					  sh[m] = 1;
				   else if (h[m] < 0.0)
					  sh[m] = -1;
				   else
					  sh[m] = 0;
				}

            /*
               Note: at this stage the relative heights of the corners and the
               centre are in the h array, and the corresponding coordinates are
               in the xh and yh arrays. The centre of the box is indexed by 0
               and the 4 corners by 1 to 4 as shown below.
               Each triangle is then indexed by the parameter m, and the 3
               vertices of each triangle are indexed by parameters m1,m2,and m3.
               It is assumed that the centre of the box is always vertex 2
               though this isimportant only when all 3 vertices lie exactly on
               the same contour level, in which case only the side of the box
               is drawn.
                  vertex 4 +-------------------+ vertex 3
                           | \               / |
                           |   \    m-3    /   |
                           |     \       /     |
                           |       \   /       |
                           |  m=2    X   m=2   |       the centre is vertex 0
                           |       /   \       |
                           |     /       \     |
                           |   /    m=1    \   |
                           | /               \ |
                  vertex 1 +-------------------+ vertex 2
            */
            /* Scan each triangle in the box */
            for (m=1;m<=4;m++) 
			{
               m1 = m;
               m2 = 0;
               if (m != 4)
                  m3 = m + 1;
               else
                  m3 = 1;
               if ((case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1]) == 0)
			   {
				   if(!(sh[m1] < 0||sh[m2] < 0||sh[m3] < 0))
				   {
					   nPolygon = 3;

						tempPoint[0] = Point2D(xh[m1], yh[m1]);
						tempPoint[1] = Point2D(xh[m2], yh[m2]);
						tempPoint[2] = Point2D(xh[m3], yh[m3]);

				   }
			   }
			   else
			   {
				   switch (case_value) 
				   {
				   case 1: /* Line between vertices 1 and 2 */
					   x1 = xh[m1];
					   y1 = yh[m1];
					   x2 = xh[m2];
					   y2 = yh[m2];
					   if(sh[m3] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m1], yh[m1]);
							tempPoint[1] = Point2D(xh[m2], yh[m2]);
							tempPoint[2] = Point2D(xh[m3], yh[m3]);

					   }
					   break;
				   case 2: /* Line between vertices 2 and 3 */
					   x1 = xh[m2];
					   y1 = yh[m2];
					   x2 = xh[m3];
					   y2 = yh[m3];
					   if(sh[m1] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m1], yh[m1]);
							tempPoint[1] = Point2D(xh[m2], yh[m2]);
							tempPoint[2] = Point2D(xh[m3], yh[m3]);

					   }
					   break;
				   case 3: /* Line between vertices 3 and 1 */
					   x1 = xh[m3];
					   y1 = yh[m3];
					   x2 = xh[m1];
					   y2 = yh[m1];
					   if(sh[m2] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m1], yh[m1]);
							tempPoint[1] = Point2D(xh[m2], yh[m2]);
							tempPoint[2] = Point2D(xh[m3], yh[m3]);
					   }

					   break;
				   case 4: /* Line between vertex 1 and side 2-3 */
					   x1 = xh[m1];
					   y1 = yh[m1];
					   x2 = xsect(m2,m3);
					   y2 = ysect(m2,m3);
					   if(sh[m2] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m1], yh[m1]);
							tempPoint[1] = Point2D(xh[m2], yh[m2]);
							tempPoint[2] = Point2D(x2, y2);
					   }
					   else
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m1], yh[m1]);
							tempPoint[1] = Point2D(x2, y2);
							tempPoint[2] = Point2D(xh[m3], yh[m3]);
					   }

					   break;
				   case 5: /* Line between vertex 2 and side 3-1 */
					   x1 = xh[m2];
					   y1 = yh[m2];
					   x2 = xsect(m3,m1);
					   y2 = ysect(m3,m1);
					   if(sh[m3] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m2], yh[m2]);
							tempPoint[1] = Point2D(xh[m3], yh[m3]);
							tempPoint[2] = Point2D(x2, y2);
					   }
					   else
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m2], yh[m2]);
							tempPoint[1] = Point2D(x2, y2);
							tempPoint[2] = Point2D(xh[m1], yh[m1]);
					   }
					   break;
				   case 6: /* Line between vertex 3 and side 1-2 */
					   x1 = xh[m3];
					   y1 = yh[m3];
					   x2 = xsect(m1,m2);
					   y2 = ysect(m1,m2);
					   if(sh[m1] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m3], yh[m3]);
							tempPoint[1] = Point2D(xh[m1], yh[m1]);
							tempPoint[2] = Point2D(x2, y2);
					   }
					   else
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m3], yh[m3]);
							tempPoint[1] = Point2D(x2, y2);
							tempPoint[2] = Point2D(xh[m2], yh[m2]);
					   }
					   break;
				   case 7: /* Line between sides 1-2 and 2-3 */
					   x1 = xsect(m1,m2);
					   y1 = ysect(m1,m2);
					   x2 = xsect(m2,m3);
					   y2 = ysect(m2,m3);
					   if(sh[m2] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m2], yh[m2]);
							tempPoint[1] = Point2D(x1, y1);
							tempPoint[2] = Point2D(x2, y2);
					   }
					   else
					   {
						   nPolygon = 4;

							tempPoint[0] = Point2D(xh[m1], yh[m1]);
							tempPoint[1] = Point2D(x1, y1);
							tempPoint[2] = Point2D(x2, y2);
							tempPoint[3] = Point2D(xh[m3], yh[m3]);
					   }
					   break;
				   case 8: /* Line between sides 2-3 and 3-1 */
					   x1 = xsect(m2,m3);
					   y1 = ysect(m2,m3);
					   x2 = xsect(m3,m1);
					   y2 = ysect(m3,m1);
					   if(sh[m3] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m3], yh[m3]);
							tempPoint[1] = Point2D(x1, y1);
							tempPoint[2] = Point2D(x2, y2);
					   }
					   else
					   {
						   nPolygon = 4;

							tempPoint[0] = Point2D(xh[m2], yh[m2]);
							tempPoint[1] = Point2D(x1, y1);
							tempPoint[2] = Point2D(x2, y2);
							tempPoint[3] = Point2D(xh[m1], yh[m1]);
					   }					   
					   break;
				   case 9: /* Line between sides 3-1 and 1-2 */
					   x1 = xsect(m3,m1);
					   y1 = ysect(m3,m1);
					   x2 = xsect(m1,m2);
					   y2 = ysect(m1,m2);
					   if(sh[m1] >= 0)
					   {
						   nPolygon = 3;

							tempPoint[0] = Point2D(xh[m1], yh[m1]);
							tempPoint[1] = Point2D(x1, y1);
							tempPoint[2] = Point2D(x2, y2);
					   }
					   else
					   {
						   nPolygon = 4;

							tempPoint[0] = Point2D(xh[m3], yh[m3]);
							tempPoint[1] = Point2D(x1, y1);
							tempPoint[2] = Point2D(x2, y2);
							tempPoint[3] = Point2D(xh[m2], yh[m2]);
					   }					   
					   break;
				   default:
					   break;
				   }

				   /* Finally draw the line */
					int posx1 = (int)(m_nZoom*(x1-x[ilb]) + m_xmargin);
					int posy1 = (int)(m_nZoom*(y[jub]-y1) + m_ymargin);
					int posx2 = (int)(m_nZoom*(x2-x[ilb]) + m_xmargin);
					int posy2 = (int)(m_nZoom*(y[jub]-y2) + m_ymargin);
					//int posx1 = (int)(m_nZoom*(x1-3+m_nBaseWidth/2));
					//int posy1 = (int)(m_nZoom*(0.5 * m_nBaseHeight-y1-5));
					//int posx2 = (int)(m_nZoom*(x2-3+m_nBaseWidth/2));
					//int posy2 = (int)(m_nZoom*(0.5 * m_nBaseHeight-y2-5));

					CPen contourpen;
					contourpen.CreatePen(PS_SOLID, 1, Colors[k]);
					CPen * pOldPen = pDC->SelectObject(&contourpen);
					pDC->MoveTo(posx1, posy1);
					pDC->LineTo(posx2, posy2);
					pDC->SelectObject(pOldPen);
	//               ConrecLine(x1,y1,x2,y2,z[k]);
				} /* m */

					CPen noPen;
					noPen.CreatePen(PS_NULL, 1, Colors[k]);
					CPen * pOldPen = pDC->SelectObject(&noPen);
					CBrush brushRed(FillColors[k]);
					CBrush* pOldBrush = pDC->SelectObject(&brushRed);


					CPoint pts[4];

				   for(int indx = 0; indx < nPolygon; indx++)
				   {
					   pts[indx].x = (int)(m_nZoom*(tempPoint[indx].x-x[ilb]) + m_xmargin);
					   pts[indx].y = (int)(m_nZoom*(y[jub]-tempPoint[indx].y) + m_ymargin);
				   }

				   // Calling Polygon() on that array will draw three lines
				   // between the points, as well as an additional line to
				   // close the shape--from the last point to the first point
				   // we specified.
				   pDC->Polygon(pts, nPolygon);

				   // Put back the old objects.
					pDC->SelectObject(pOldPen);
				   pDC->SelectObject(pOldBrush);
				   nPolygon = 0;
			}
         } /* k - contour */
      } /* i */
   } /* j */
}

/*
   Derivation from the fortran version of CONREC by Paul Bourke
   d               ! matrix of data to contour
   ilb,iub,jlb,jub ! index bounds of data matrix
   x               ! data matrix column coordinates
   y               ! data matrix row coordinates
   nc              ! number of contour levels
   z               ! contour levels in increasing order
*/
void CContourView::Contour(double **d,int ilb,int iub,int jlb,int jub,
   double *x,double *y,CLevelFile * pLevelFile,
   CDC * pDC)
{
#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])
#define MIN(X,Y) ((X) < (Y) ?  (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ?  (X) : (Y))

   int m1,m2,m3,case_value;
   double dmin,dmax,x1=0,x2=0,y1=0,y2=0;
   int i,j,k,m;
   double h[5];
   int sh[5];
   double xh[5],yh[5];
   int im[4] = {0,1,1,0},jm[4]={0,0,1,1};
   int castab[3][3][3] = {
     { {0,0,8},{0,2,5},{7,6,9} },
     { {0,3,4},{1,3,1},{4,3,0} },
     { {9,6,7},{5,2,0},{8,0,0} }
   };
   double temp1,temp2;

   int nPolygon = 0;
//   Point2D * tempPoint = new Point2D[4];
   Point3D * tempPoint = new Point3D[4];

   int nc = pLevelFile->m_LevelNum;

   double * z = new double[nc];
   double * area = new double[nc];
   CPen * contourpen = new CPen[nc];
   CBrush * fillbrush = new CBrush[nc];

   for (k=0;k<nc;k++) 
   {
	   CLevelProperty * tempproperty = pLevelFile->m_LevelList.GetAt(k); 
	   z[k] = tempproperty->m_Level;

	   int penWidth = tempproperty->m_LineWidth/10;
	   COLORREF pencolor = pLevelFile->GetColorFromString(tempproperty->m_LineColor);
	   contourpen[k].CreatePen(PS_SOLID, penWidth, pencolor);

	   COLORREF brushcolor = pLevelFile->GetColorFromString(tempproperty->m_FFGColor);
	   fillbrush[k].CreateSolidBrush(brushcolor);
   }

   for (k=0;k<nc;k++) 
   {
	   area[k] = 0.0;
	   for (j=(jub-1);j>=jlb;j--) 
	   {
			for (i=ilb;i<=iub-1;i++) 
			{
//				TRACE("j = %d, i = %d\n", j, i);
				//temp1 = MIN(d[i][j],d[i][j+1]);
				//temp2 = MIN(d[i+1][j],d[i+1][j+1]);
				//dmin  = MIN(temp1,temp2);
				//temp1 = MAX(d[i][j],d[i][j+1]);
				//temp2 = MAX(d[i+1][j],d[i+1][j+1]);
				//dmax  = MAX(temp1,temp2);
				//if (dmax < z[0] || dmin > z[nc-1])
				//	continue;
				//if (z[k] < dmin || z[k] > dmax)
				//	continue;
				if(z[k] == 15)
					int dfg = 0;
				for (m=4;m>=0;m--) 
				{
				   if (m > 0) {
					  h[m]  = d[i+im[m-1]][j+jm[m-1]]-z[k];
					  xh[m] = x[i+im[m-1]];
					  yh[m] = y[j+jm[m-1]];
				   } else {
					  h[0]  = 0.25 * (h[1]+h[2]+h[3]+h[4]);
					  xh[0] = 0.50 * (x[i]+x[i+1]);
					  yh[0] = 0.50 * (y[j]+y[j+1]);
				   }
				   if (h[m] > 0.0)
					  sh[m] = 1;
				   else if (h[m] < 0.0)
					  sh[m] = -1;
				   else
					  sh[m] = 0;
				}

            /*
               Note: at this stage the relative heights of the corners and the
               centre are in the h array, and the corresponding coordinates are
               in the xh and yh arrays. The centre of the box is indexed by 0
               and the 4 corners by 1 to 4 as shown below.
               Each triangle is then indexed by the parameter m, and the 3
               vertices of each triangle are indexed by parameters m1,m2,and m3.
               It is assumed that the centre of the box is always vertex 2
               though this isimportant only when all 3 vertices lie exactly on
               the same contour level, in which case only the side of the box
               is drawn.
                  vertex 4 +-------------------+ vertex 3
                           | \               / |
                           |   \    m-3    /   |
                           |     \       /     |
                           |       \   /       |
                           |  m=2    X   m=2   |       the centre is vertex 0
                           |       /   \       |
                           |     /       \     |
                           |   /    m=1    \   |
                           | /               \ |
                  vertex 1 +-------------------+ vertex 2
            */
            /* Scan each triangle in the box */
            for (m=1;m<=4;m++) 
			{
               m1 = m;
               m2 = 0;
               if (m != 4)
                  m3 = m + 1;
               else
                  m3 = 1;
               if ((case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1]) == 0)
			   {
				   if(!(sh[m1] < 0||sh[m2] < 0||sh[m3] < 0))
				   {
					   nPolygon = 3;

						//tempPoint[0] = Point2D(xh[m1], yh[m1]);
						//tempPoint[1] = Point2D(xh[m2], yh[m2]);
						//tempPoint[2] = Point2D(xh[m3], yh[m3]);
						tempPoint[0] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
						tempPoint[1] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
						tempPoint[2] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);

				   }
			   }
			   else
			   {
				   switch (case_value) 
				   {
				   case 1: /* Line between vertices 1 and 2 */
					   x1 = xh[m1];
					   y1 = yh[m1];
					   x2 = xh[m2];
					   y2 = yh[m2];
					   if(sh[m3] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m1], yh[m1]);
							//tempPoint[1] = Point2D(xh[m2], yh[m2]);
							//tempPoint[2] = Point2D(xh[m3], yh[m3]);
							tempPoint[0] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
							tempPoint[1] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
							tempPoint[2] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);

					   }
					   break;
				   case 2: /* Line between vertices 2 and 3 */
					   x1 = xh[m2];
					   y1 = yh[m2];
					   x2 = xh[m3];
					   y2 = yh[m3];
					   if(sh[m1] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m1], yh[m1]);
							//tempPoint[1] = Point2D(xh[m2], yh[m2]);
							//tempPoint[2] = Point2D(xh[m3], yh[m3]);
							tempPoint[0] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
							tempPoint[1] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
							tempPoint[2] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);

					   }
					   break;
				   case 3: /* Line between vertices 3 and 1 */
					   x1 = xh[m3];
					   y1 = yh[m3];
					   x2 = xh[m1];
					   y2 = yh[m1];
					   if(sh[m2] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m1], yh[m1]);
							//tempPoint[1] = Point2D(xh[m2], yh[m2]);
							//tempPoint[2] = Point2D(xh[m3], yh[m3]);
							tempPoint[0] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
							tempPoint[1] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
							tempPoint[2] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);
					   }

					   break;
				   case 4: /* Line between vertex 1 and side 2-3 */
					   x1 = xh[m1];
					   y1 = yh[m1];
					   x2 = xsect(m2,m3);
					   y2 = ysect(m2,m3);
					   if(sh[m2] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m1], yh[m1]);
							//tempPoint[1] = Point2D(xh[m2], yh[m2]);
							//tempPoint[2] = Point2D(x2, y2);
							tempPoint[0] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
							tempPoint[1] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);
					   }
					   else
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m1], yh[m1]);
							//tempPoint[1] = Point2D(x2, y2);
							//tempPoint[2] = Point2D(xh[m3], yh[m3]);
							tempPoint[0] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
							tempPoint[1] = Point3D(x2, y2, z[k]);
							tempPoint[2] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);
					   }

					   break;
				   case 5: /* Line between vertex 2 and side 3-1 */
					   x1 = xh[m2];
					   y1 = yh[m2];
					   x2 = xsect(m3,m1);
					   y2 = ysect(m3,m1);
					   if(sh[m3] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m2], yh[m2]);
							//tempPoint[1] = Point2D(xh[m3], yh[m3]);
							//tempPoint[2] = Point2D(x2, y2);
							tempPoint[0] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
							tempPoint[1] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);

					   }
					   else
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m2], yh[m2]);
							//tempPoint[1] = Point2D(x2, y2);
							//tempPoint[2] = Point2D(xh[m1], yh[m1]);
							tempPoint[0] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
							tempPoint[1] = Point3D(x2, y2, z[k]);
							tempPoint[2] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
					   }
					   break;
				   case 6: /* Line between vertex 3 and side 1-2 */
					   x1 = xh[m3];
					   y1 = yh[m3];
					   x2 = xsect(m1,m2);
					   y2 = ysect(m1,m2);
					   if(sh[m1] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m3], yh[m3]);
							//tempPoint[1] = Point2D(xh[m1], yh[m1]);
							//tempPoint[2] = Point2D(x2, y2);
							tempPoint[0] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);
							tempPoint[1] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);
					   }
					   else
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m3], yh[m3]);
							//tempPoint[1] = Point2D(x2, y2);
							//tempPoint[2] = Point2D(xh[m2], yh[m2]);
							tempPoint[0] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);
							tempPoint[1] = Point3D(x2, y2, z[k]);
							tempPoint[2] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
					   }
					   break;
				   case 7: /* Line between sides 1-2 and 2-3 */
					   x1 = xsect(m1,m2);
					   y1 = ysect(m1,m2);
					   x2 = xsect(m2,m3);
					   y2 = ysect(m2,m3);
					   if(sh[m2] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m2], yh[m2]);
							//tempPoint[1] = Point2D(x1, y1);
							//tempPoint[2] = Point2D(x2, y2);
							tempPoint[0] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
							tempPoint[1] = Point3D(x1, y1, z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);
					   }
					   else
					   {
						   nPolygon = 4;

							//tempPoint[0] = Point2D(xh[m1], yh[m1]);
							//tempPoint[1] = Point2D(x1, y1);
							//tempPoint[2] = Point2D(x2, y2);
							//tempPoint[3] = Point2D(xh[m3], yh[m3]);
							tempPoint[0] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
							tempPoint[1] = Point3D(x1, y1, z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);
							tempPoint[3] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);
					   }
					   break;
				   case 8: /* Line between sides 2-3 and 3-1 */
					   x1 = xsect(m2,m3);
					   y1 = ysect(m2,m3);
					   x2 = xsect(m3,m1);
					   y2 = ysect(m3,m1);
					   if(sh[m3] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m3], yh[m3]);
							//tempPoint[1] = Point2D(x1, y1);
							//tempPoint[2] = Point2D(x2, y2);
							tempPoint[0] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);
							tempPoint[1] = Point3D(x1, y1, z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);
					   }
					   else
					   {
						   nPolygon = 4;

							//tempPoint[0] = Point2D(xh[m2], yh[m2]);
							//tempPoint[1] = Point2D(x1, y1);
							//tempPoint[2] = Point2D(x2, y2);
							//tempPoint[3] = Point2D(xh[m1], yh[m1]);
							tempPoint[0] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
							tempPoint[1] = Point3D(x1, y1, z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);
							tempPoint[3] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
					   }					   
					   break;
				   case 9: /* Line between sides 3-1 and 1-2 */
					   x1 = xsect(m3,m1);
					   y1 = ysect(m3,m1);
					   x2 = xsect(m1,m2);
					   y2 = ysect(m1,m2);
					   if(sh[m1] >= 0)
					   {
						   nPolygon = 3;

							//tempPoint[0] = Point2D(xh[m1], yh[m1]);
							//tempPoint[1] = Point2D(x1, y1);
							//tempPoint[2] = Point2D(x2, y2);
							tempPoint[0] = Point3D(xh[m1], yh[m1], h[m1]+z[k]);
							tempPoint[1] = Point3D(x1, y1, z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);
					   }
					   else
					   {
						   nPolygon = 4;

							//tempPoint[0] = Point2D(xh[m3], yh[m3]);
							//tempPoint[1] = Point2D(x1, y1);
							//tempPoint[2] = Point2D(x2, y2);
							//tempPoint[3] = Point2D(xh[m2], yh[m2]);
							tempPoint[0] = Point3D(xh[m3], yh[m3], h[m3]+z[k]);
							tempPoint[1] = Point3D(x1, y1, z[k]);
							tempPoint[2] = Point3D(x2, y2, z[k]);
							tempPoint[3] = Point3D(xh[m2], yh[m2], h[m2]+z[k]);
					   }					   
					   break;
				   default:
					   break;
				   }

				   /* Finally draw the line */
					int posx1 = (int)(m_nZoom*(x1-x[ilb]) + m_xmargin);
					int posy1 = (int)(m_nZoom*(y[jub]-y1) + m_ymargin);
					int posx2 = (int)(m_nZoom*(x2-x[ilb]) + m_xmargin);
					int posy2 = (int)(m_nZoom*(y[jub]-y2) + m_ymargin);
					//int posx1 = (int)(m_nZoom*(x1-3+m_nBaseWidth/2));
					//int posy1 = (int)(m_nZoom*(0.5 * m_nBaseHeight-y1-5));
					//int posx2 = (int)(m_nZoom*(x2-3+m_nBaseWidth/2));
					//int posy2 = (int)(m_nZoom*(0.5 * m_nBaseHeight-y2-5));
					CPen * pOldPen = pDC->SelectObject(&contourpen[k]);
					pDC->MoveTo(posx1, posy1);
					pDC->LineTo(posx2, posy2);
					pDC->SelectObject(pOldPen);
	//               ConrecLine(x1,y1,x2,y2,z[k]);
				} /* m */

				CPen noPen;
				noPen.CreatePen(PS_NULL, 1, RGB(0, 0, 0));
					CPen * pOldPen = pDC->SelectObject(&noPen);

				   CBrush* pOldBrush = pDC->SelectObject(&fillbrush[k]);

				   area[k] += PolyArea(tempPoint, nPolygon);

				   CPoint pts[4];

				   for(int indx = 0; indx < nPolygon; indx++)
				   {
					   pts[indx].x = (int)(m_nZoom*(tempPoint[indx].x-x[ilb]) + m_xmargin);
					   pts[indx].y = (int)(m_nZoom*(y[jub]-tempPoint[indx].y) + m_ymargin);
				   }

				   // Calling Polygon() on that array will draw three lines
				   // between the points, as well as an additional line to
				   // close the shape--from the last point to the first point
				   // we specified.
				   pDC->Polygon(pts, nPolygon);

				   // Put back the old objects.
					pDC->SelectObject(pOldPen);
				   pDC->SelectObject(pOldBrush);
				   nPolygon = 0;
			}
         } /* k - contour */
      } /* i */
   } /* j */

   for (k=0;k<nc;k++) 
   {
	   CLevelProperty * tempproperty = pLevelFile->m_LevelList.GetAt(k); 
	   tempproperty->m_Area = area[k];
	}
}

float CContourView::PolyArea(Point3D * Points, int nPolygon)
{
	float sum = 0.0;
	int curindx, nextindx;
	for(int indx = 0; indx<nPolygon; indx++)
	{
		curindx = indx;
		nextindx = indx+1;
		if(nextindx == nPolygon)
		{
			nextindx = 0;
		}
		sum += Points[curindx].x*Points[nextindx].y - Points[curindx].y*Points[nextindx].x; 
	}
	return fabs(0.5*sum);
}

void CContourView::DrawContourLines(CDC* pDC)
{
//	Contour(m_data2d, 0, m_nDiameter-1, 0, m_nDiameter-1, m_xaxis, m_yaxis, m_nNumOfLevels, m_levels, pDC);
	CContourDoc* pDoc = GetDocument();

	Contour(m_data2d, 0, pDoc->m_GridPacket.m_nCol-1, 0, pDoc->m_GridPacket.m_nRow-1, m_xaxis, m_yaxis, m_nNumOfLevels, m_levels, pDC);

	SetScrollSizes(MM_TEXT, m_CanvasSize);

}

void CContourView::DrawContourLinesHS(CDC* pDC)
{
	CBitmap* pOldBmp; //oldbmp

	CDC * targetDC;

	CDC BufferDC;
//	CDC MemDC; //메모리DC

	m_Bitmap.DeleteObject(); //기존꺼 지워줌

	m_Bitmap.CreateCompatibleBitmap(pDC, m_CanvasSize.cx, m_CanvasSize.cy); //새로 만들어줌

	BufferDC.CreateCompatibleDC(pDC); //memDC도 새로 만들어줌

	pOldBmp = BufferDC.SelectObject(&m_Bitmap); //memDC의 비트맵 선택


	targetDC = &BufferDC;

	targetDC->FillSolidRect(0, 0, m_CanvasSize.cx, m_CanvasSize.cy, RGB(255,255,255));

	CContourDoc* pDoc = GetDocument();

//	Contour(m_data2d, 0, pDoc->m_GridPacket.m_nCol-1, 0, pDoc->m_GridPacket.m_nRow-1, m_xaxis, m_yaxis, m_nNumOfLevels, m_levels, targetDC);
	pDoc->m_TotalArea =  (m_xaxis[pDoc->m_GridPacket.m_nCol-1] -m_xaxis[0]) * (m_yaxis[pDoc->m_GridPacket.m_nRow-1] -m_yaxis[0]) ; // sanghyun 102414
	Contour(m_data2d, 0, pDoc->m_GridPacket.m_nCol-1, 0, pDoc->m_GridPacket.m_nRow-1, m_xaxis, m_yaxis, &(pDoc->m_LevelFile), targetDC);

	BufferDC.SelectObject(pOldBmp); //oldbmp 선택

	BufferDC.DeleteDC(); //memDC를 지운다.	
	SetScrollSizes(MM_TEXT, m_CanvasSize);

}

void CContourView::DrawContour(CDC* pDC)
{
	CBitmap* pOldBmp; //oldbmp

	CDC * targetDC;

	CDC BufferDC;
//	CDC MemDC; //메모리DC

	m_Bitmap.DeleteObject(); //기존꺼 지워줌

	m_Bitmap.CreateCompatibleBitmap(pDC, m_CanvasSize.cx, m_CanvasSize.cy); //새로 만들어줌

	BufferDC.CreateCompatibleDC(pDC); //memDC도 새로 만들어줌

	pOldBmp = BufferDC.SelectObject(&m_Bitmap); //memDC의 비트맵 선택


	targetDC = &BufferDC;

	targetDC->FillSolidRect(0, 0, m_CanvasSize.cx, m_CanvasSize.cy, RGB(255,255,255));

	CContourDoc* pDoc = GetDocument();

//	Contour(m_data2d, 0, pDoc->m_GridPacket.m_nCol-1, 0, pDoc->m_GridPacket.m_nRow-1, m_xaxis, m_yaxis, m_nNumOfLevels, m_levels, targetDC);
	pDoc->m_TotalArea =  (m_xaxis[pDoc->m_GridPacket.m_nCol-1] -m_xaxis[0]) * (m_yaxis[pDoc->m_GridPacket.m_nRow-1] -m_yaxis[0]) ; // sanghyun 102414

	CLevelFile * pLevelFile = &(pDoc->m_LevelFile);
	int nc = pLevelFile->m_LevelNum;

	CPen * contourpen = new CPen[nc];
	CBrush * fillbrush = new CBrush[nc];

	for (int k=0;k<nc;k++) 
	{
	   CLevelProperty * tempproperty = pLevelFile->m_LevelList.GetAt(k); 

		int penWidth = tempproperty->m_LineWidth/10;
		COLORREF pencolor = pLevelFile->GetColorFromString(tempproperty->m_LineColor);
		contourpen[k].CreatePen(PS_SOLID, penWidth, pencolor);

		COLORREF brushcolor = pLevelFile->GetColorFromString(tempproperty->m_FFGColor);
		fillbrush[k].CreateSolidBrush(brushcolor);

		CArray<CContourLine *, CContourLine *> * pLineList;
		CArray<CContourPolygon *, CContourPolygon *> *pPolygonList;
		
		double xmax = pDoc->m_XDrawEnd;
		double xmin = pDoc->m_XDrawStart;
		double ymax = pDoc->m_YDrawEnd;
		double ymin = pDoc->m_YDrawStart;

//		ymax = 5.0;
//		ymin = -30.0;
		pPolygonList = &(pDoc->m_Contour.m_PolygonList[k]);
		int totalno = pPolygonList->GetSize();

		CPen noPen;
		noPen.CreatePen(PS_NULL, 1, RGB(0, 0, 0));
		CPen * pOldPen = targetDC->SelectObject(&noPen);

		CBrush* pOldBrush = targetDC->SelectObject(&fillbrush[k]);

		for(int cindx = 0; cindx<totalno; cindx++)
		{
			CContourPolygon * tempPolygon = pPolygonList->GetAt(cindx);


			CPoint pts[4];

			int nPolygon = tempPolygon->m_nPolygon;

			for(int indx = 0; indx < nPolygon; indx++)
			{
				pts[indx].x = (int)(m_nZoom*(tempPolygon->m_Points[indx].x-xmin) + m_xmargin);
				pts[indx].y = (int)(m_nZoom*(ymax-tempPolygon->m_Points[indx].y) + m_ymargin);
			}

			// Calling Polygon() on that array will draw three lines
			// between the points, as well as an additional line to
			// close the shape--from the last point to the first point
			// we specified.
			targetDC->Polygon(pts, nPolygon);

			// Put back the old objects.

		}
		targetDC->SelectObject(pOldPen);
		targetDC->SelectObject(pOldBrush);

		pLineList = &(pDoc->m_Contour.m_LineList[k]);
		totalno = pLineList->GetSize();

		pOldPen = targetDC->SelectObject(&contourpen[k]);
		for(int cindx = 0; cindx<totalno; cindx++)
		{
			CContourLine * tempLine = pLineList->GetAt(cindx);

			float x1 = tempLine->m_Lines[0].x;
			float y1 = tempLine->m_Lines[0].y;
			float x2 = tempLine->m_Lines[1].x;
			float y2 = tempLine->m_Lines[1].y;

			int posx1 = (int)(m_nZoom*(x1-xmin) + m_xmargin);
			int posy1 = (int)(m_nZoom*(ymax-y1) + m_ymargin);
			int posx2 = (int)(m_nZoom*(x2-xmin) + m_xmargin);
			int posy2 = (int)(m_nZoom*(ymax-y2) + m_ymargin);
			//int posx1 = (int)(m_nZoom*(x1-3+m_nBaseWidth/2));
			//int posy1 = (int)(m_nZoom*(0.5 * m_nBaseHeight-y1-5));
			//int posx2 = (int)(m_nZoom*(x2-3+m_nBaseWidth/2));
			//int posy2 = (int)(m_nZoom*(0.5 * m_nBaseHeight-y2-5));
			targetDC->MoveTo(posx1, posy1);
			targetDC->LineTo(posx2, posy2);

		}
		targetDC->SelectObject(pOldPen);

	}


	BufferDC.SelectObject(pOldBmp); //oldbmp 선택

	BufferDC.DeleteDC(); //memDC를 지운다.	
	SetScrollSizes(MM_TEXT, m_CanvasSize);
}

void CContourView::DrawAxis(CDC* pDC)
{
	CContourDoc* pDoc = GetDocument();

	CPen blackPen;

	blackPen.CreatePen(PS_SOLID, 1.0, RGB(0,0,0));

	pDC->SelectObject(&blackPen);


	//Boundary
	int posx, posy;
	double x, y;
	x = pDoc->m_XDrawStart;
	y = pDoc->m_YDrawStart;
	posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
	posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
	pDC->MoveTo(posx, posy);

	x = pDoc->m_XDrawEnd;
	y = pDoc->m_YDrawStart;
	posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
	posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
	pDC->LineTo(posx, posy);

	x = pDoc->m_XDrawEnd;
	y = pDoc->m_YDrawEnd;
	posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
	posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
	pDC->LineTo(posx, posy);

	x = pDoc->m_XDrawStart;
	y = pDoc->m_YDrawEnd;
	posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
	posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
	pDC->LineTo(posx, posy);

	x = pDoc->m_XDrawStart;
	y = pDoc->m_YDrawStart;
	posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
	posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
	pDC->LineTo(posx, posy);

	///X Tick
	for (int xindx = 0; xindx< pDoc->m_XDrawTickNo; xindx++)
	{
		x = pDoc->m_XDrawTickValues[xindx];
		y = pDoc->m_YDrawStart;
		if(x>= pDoc->m_XDrawStart && x<= pDoc->m_XDrawEnd)
		{
			posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
			posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
			pDC->MoveTo(posx, posy);
			pDC->LineTo(posx, posy+5);
			CString tempstring;
			tempstring.Format("%.1f", x);
			pDC->TextOut(posx-15, posy+10, tempstring);
		}
	}
	//YTick
	for (int yindx = 0; yindx< pDoc->m_YDrawTickNo; yindx++)
	{
		y = pDoc->m_YDrawTickValues[yindx];
		x = pDoc->m_XDrawStart;
		if(y>= pDoc->m_YDrawStart && y<= pDoc->m_YDrawEnd)
		{
			posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
			posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
			pDC->MoveTo(posx, posy);
			pDC->LineTo(posx-5, posy);
			CString tempstring;
			tempstring.Format("%.1f", y);
			pDC->TextOut(posx-40, posy-10, tempstring);
		}
	}

	/////X Tick
	//for( x = pDoc->m_XDrawStart; x<=pDoc->m_XDrawEnd; x+= pDoc->m_XDrawTickStep)
	//{
	//	y = pDoc->m_YDrawStart;
	//	posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
	//	posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
	//	pDC->MoveTo(posx, posy);
	//	pDC->LineTo(posx, posy+5);
	//	CString tempstring;
	//	tempstring.Format("%.1f", x);
	//	pDC->TextOut(posx-15, posy+10, tempstring);
	//}
	////YTick
	//for( y = pDoc->m_YDrawStart; y<=pDoc->m_YDrawEnd; y+= pDoc->m_YDrawTickStep)
	//{
	//	x = pDoc->m_XDrawStart;
	//	posx = (int)(m_nZoom*(x-pDoc->m_XDrawStart) + m_xmargin);
	//	posy = (int)(m_nZoom*(pDoc->m_YDrawEnd-y) + m_ymargin);
	//	pDC->MoveTo(posx, posy);
	//	pDC->LineTo(posx-5, posy);
	//	CString tempstring;
	//	tempstring.Format("%.1f", y);
	//	pDC->TextOut(posx-40, posy-10, tempstring);
	//}
}

void CContourView::OnShowcontourlines()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bContourlines = !m_bContourlines;
	Invalidate();
}


void CContourView::OnUpdateShowcontourlines(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bContourlines);

}


void CContourView::OnOptionShowimage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bImage = !m_bImage;
	Invalidate();
}


void CContourView::OnUpdateOptionShowimage(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bImage);
}

BOOL CContourView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	TRACE("OnEraseBkgnd\n");
 
	return true;

//	return CScrollView::OnEraseBkgnd(pDC);
}

void CContourView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SetScrollSizes(MM_TEXT, m_CanvasSize);

//    UpdateRulersInfo(RW_POSITION, GetScrollPosition());
    Invalidate();
	TRACE("OnUpdate\n");

}

void CContourView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	TRACE("OnSize\n");

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(cx>0)
	{
		CContourDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;
		pDoc->m_bRedraw = true;
	}
	OnUpdate(NULL, 0, NULL);
}

//void CContourView::OnZoomSlider2()
//{
//
//	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
//	
//	CMFCRibbonStatusBar* pRibbonStatusBar = &(pMainFrame->m_wndStatusBar);
//	//CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*) AfxGetMainWnd())->GetRibbonBar();
//
//	//CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbonStatusBar->find FindByID(ID_STATUSBAR_ZOOM_SLIDER));
//	//ASSERT_VALID(pSlider); ////////////////
//	CMFCRibbonSlider* pSlider = pMainFrame->m_pSlider;
//	int nPos = pSlider->GetPos();
//
//	CContourDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//	
//	m_bZoomFitMode = false;
//	pDoc->m_bRedraw = true;
//	m_nZoom = (float)nPos; // Sang for Zoom slider
//
//	OnUpdate(NULL, 0, NULL);
//
//
//	CMFCRibbonStatusBarPane* pZoom = pMainFrame->m_pZoom;
//	ASSERT_VALID(pZoom);
//
//	CString strZoom;
//	strZoom.Format(_T("%d%%"), nPos);
//
//	pZoom->SetText(strZoom);
//	pZoom->Redraw();
//
//}

void CContourView::OnFilePrintPreview()
{
//#ifndef SHARED_HANDLERS
	int i = 0;
	AFXPrintPreview(this);
//#endif
}

BOOL CContourView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CContourView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CContourView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}
 // Sanghyun 103114 for Save image as

#include <atlimage.h>
#include <comdef.h>
void CContourView::SaveImageFile(CString strFileName)
{
	CContourDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	HRESULT hResult;
	// CImage 개체에 출력
	// 1단계 : 그림 크기 계산 Calculate Original Size
	CClientDC dc(this);

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);
	dcMemory.SetMapMode(MM_TEXT);

	float m_oldvZoomFactor=m_nZoom; // sanghyun 120414 Save purpose
	float vScaleFactor=100.0; // make it 100% of image 
	m_nZoom = vScaleFactor;
	pDoc->m_bRedraw = true;
	BOOL oldbZoomFitMode = m_bZoomFitMode;
	m_bZoomFitMode = false;
	// DeterminePlotRegion();

	m_CanvasSize.cx = int((m_xaxis[pDoc->m_GridPacket.m_nCol-1] - m_xaxis[0])*m_nZoom) + 2*m_xmargin;
	m_CanvasSize.cy = int((m_yaxis[pDoc->m_GridPacket.m_nRow-1] - m_yaxis[0])*m_nZoom) + 2*m_ymargin;

//	CSize viewSize = this->GetTotalSize();
	CSize viewSize;
	viewSize.SetSize(m_CanvasSize.cx, m_CanvasSize.cy);
//	CSize deviceSize = this->GetTotalSize();
	CSize deviceSize;
	deviceSize.SetSize(m_CanvasSize.cx, m_CanvasSize.cy);
	dcMemory.LPtoDP(&deviceSize);



	// 2단계 : CImage 개체 생성 Prepare CImage class
	ATL::CImage saveImage;

	saveImage.Create(deviceSize.cx,deviceSize.cy,16);

	CDC* pMemoryDC = CDC::FromHandle(saveImage.GetDC());
	pMemoryDC->SetMapMode(MM_TEXT);

	// 3단계 : 개체에 그리기 Draw Image
	pMemoryDC->FillSolidRect(0,0,viewSize.cx, viewSize.cy, RGB(255,255,255));
	OnDraw(pMemoryDC);

	// 4단계 : 파일로 저장 Save Image
	// the extension on the file name will determine the file type that is saved
	hResult = saveImage.Save(strFileName);
	
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format("Save image failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}

	// 5단계 : GDI 개체 정리
	pMemoryDC->Detach();
	pMemoryDC->DeleteDC();
	saveImage.ReleaseDC();

	m_nZoom = m_oldvZoomFactor;
	m_bZoomFitMode = oldbZoomFitMode;
	pDoc->m_bRedraw = true;
	OnUpdate(NULL, 0, NULL);
} // Sanghyun 120414 for Save image as

// Sanghyun 103114 
// #include ".\BMPStore\bmpstore.h"

void CContourView::OnSaveImageAs()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

//		((CMainFrame *)AfxGetMainWnd())->m_ReceiverPort.StopMonitoring();
		static CDC dcPrint;

		CSeismicSurveyApp * pApp = (CSeismicSurveyApp *)AfxGetApp();
		//SetCurrentDirectory(pApp->m_ProjectInfo.m_strReportFolder);
		CFileDialog fDlg(FALSE, _T("png"), _T("*.png"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("jpg 파일|*.jpg|png파일|*.png|bmp파일|*.bmp||"), this);
//		CFileDialog fDlg(FALSE, _T("jpg"), _T("*.jpg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("png파일|*.png|bmp파일|*.bmp|jpg 파일*.jpg||"), this);

		char img_name[128];


		if (fDlg.DoModal() == IDOK) 
		{

			SaveImageFile(fDlg.GetPathName());
		}

} // Sanghyun 103114 
