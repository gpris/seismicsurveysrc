// ContourDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\BridgeSurvey.h"
#include "ContourDoc.h"
#include "ContourView.h"
#include "..\MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static COLORREF Colors[20] = { RGB(0, 255, 150), RGB(0, 255, 200), RGB(0, 255, 255), RGB(0, 200, 255), RGB(0, 150, 255), 
								RGB(0, 100, 255), RGB(0, 50, 255), RGB(0, 0, 255), RGB(255, 0, 200), RGB(255, 0, 150), 
								RGB(85, 0, 100), RGB(170, 0, 50), RGB(255, 0, 0), RGB(255, 50, 0), RGB(255, 100, 0), 
								RGB(255, 150, 0), RGB(255, 200, 0), RGB(255, 255, 0), RGB(200, 255, 0), RGB(150, 255, 0) };

/////////////////////////////////////////////////////////////////////////////
// CContourDoc

IMPLEMENT_DYNCREATE(CContourDoc, CDocument)

CContourDoc::CContourDoc()
{
	m_zMax = 11.5;
	m_zMin = 2.5;

	m_bAutoGain = false;

	m_LevelFile.m_LevelNum = 20;

	float colorstep = 255./m_LevelFile.m_LevelNum;

	for(int indx = 0; indx< m_LevelFile.m_LevelNum; indx++)
	{
		CLevelProperty * tempProperty = new CLevelProperty;

		tempProperty->m_Level = indx;
		tempProperty->m_Flag = 1;
		tempProperty->m_LineColor = m_LevelFile.GetStringFromColor(Colors[indx]);
		tempProperty->m_LineStyle = _T("Solid");
		tempProperty->m_LineWidth = 1;
		tempProperty->m_FVersion = 1;
		int colorval = 255 - (int)(indx*colorstep);
		tempProperty->m_FFGColor = m_LevelFile.GetStringFromColor(RGB(colorval, colorval, colorval));
		tempProperty->m_FBGColor = _T("White");
		tempProperty->m_FPattern = _T("Solid");
		tempProperty->m_OffsetX = 0;
		tempProperty->m_OffsetY = 0;
		tempProperty->m_ScaleX = 1;
		tempProperty->m_ScaleY = 1;
		tempProperty->m_Angle = 0;
		tempProperty->m_Coverage = 0;
		m_LevelFile.m_LevelList.Add(tempProperty);
	}

	m_bRedraw = true;
	m_bPropDisplayed = false; //Sang for PropertyWnd

}

BOOL CContourDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CContourDoc::~CContourDoc()
{
}


BEGIN_MESSAGE_MAP(CContourDoc, CDocument)
	//{{AFX_MSG_MAP(CContourDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_COMPARESURFER, &CContourDoc::OnCompareWithSurfer)
	ON_COMMAND(ID_COMP_RST_EXPORT, &CContourDoc::OnCompRstExport)
	ON_COMMAND(ID_GRID_SAVE, &CContourDoc::OnGridSave)
	ON_COMMAND(ID_LEVELAPPLY_BTN, &CContourDoc::OnLevelApplyBtn)
	ON_CBN_SELCHANGE(ID_PROPS_COMBO, OnComboBoxSelChange) //Sang for PropertyWnd

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContourDoc diagnostics

#ifdef _DEBUG
void CContourDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CContourDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CContourDoc serialization

void CContourDoc::Serialize(CArchive& ar)
{
	CContourView* pView = static_cast<CContourView*>(m_viewList.GetHead());

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CContourDoc commands

void CContourDoc::Init(LPCTSTR lpszPathName)
{
	m_ir.Read(lpszPathName);
	SetTitle(_T("Untitled.bmp"));
}

BOOL CContourDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_PathName.Format("%s", lpszPathName);

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	//m_GridPacket.Load(lpszPathName);
	//m_ir.LoadFromGridPacket(&m_GridPacket);

	InitFromGridFile(lpszPathName);

	UpdateAllViews(NULL, NULL, NULL);

	return true;
}

#include "..\PathNameInfo.h"

void CContourDoc::InitFromGridFile(LPCTSTR lpszPathName)
{
	m_GridPacket.Load(lpszPathName);
	m_ir.LoadFromGridPacket(&m_GridPacket);
	
	m_PathName.Format("%s", lpszPathName); // sanghyun 102314
	
	CPathNameInfo filename(lpszPathName);
	SetTitle(filename.m_FileName);
	FillUpProperty(); //Sang for PropertyWnd
}

#include "Kriging.h"
#include <algorithm>
#include <fstream>

void CContourDoc::GlobalKriging(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
{
	double xstart, xend, xstep;
	double ystart, yend, ystep;

	xstart = 0.0;
	xend = 30.0;
//	xend = 120.0;
	xstep = xaxisstep;

	m_ir.LoadFromLayerData(&(pLayerDoc->m_LayerData), xaxisindx, yaxisindx, dataindx, 0.5, xstart, xend); 

	vector<Point3D>& input = const_cast<vector<Point3D>&>(m_ir.Get3DPoints());

	Point3D *   Pbegin = &(*(input.begin()));
	Point3D *   Pend = &(*(input.end()-1));

	float semivariance = 10.0;
	Interpolater* pInterpolater = NULL;
	TRACE("Kriging Start\n");
	pInterpolater = new Kriging(Pbegin, Pend, semivariance, 2);
	TRACE("Kriging Stop\n");

	vector<double> vecZs;
	int countx = 0;
	int county = 0;


	ystart = -10.7;
	yend = -1.5;
	ystep = 0.5;

	double zmax, zmin;
	zmax = -1e12;
	zmin = 1e12;

	m_GridPacket.m_Version = 2;
	m_GridPacket.m_nCol = (int)((xend - xstart)/xstep) + 1;
	m_GridPacket.m_nRow = (int)((yend - ystart)/ystep) + 1;
	m_GridPacket.m_xLL = 0.0;
	m_GridPacket.m_yLL = -10.7;
	m_GridPacket.m_xSize = xstep;
	m_GridPacket.m_ySize = ystep;

	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);

	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
	{
		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
		{

			double x = xstart + xindx * xstep;
			double y = ystart + yindx * ystep;

			double z = pInterpolater->GetInterpolatedZ(x, y, Pbegin, Pend);
			if(z > zmax) zmax = z;
			if(z < zmin) zmin = z;
			m_GridPacket.m_Data[xindx][yindx] = z;
			vecZs.push_back(z);		

		}
	TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
	}

	delete pInterpolater;


	m_GridPacket.m_zMin = zmin;
	m_GridPacket.m_zMax = zmax;
	m_GridPacket.m_Rotation = 0.0;
	m_GridPacket.m_BlankValue = 1.7014e+38;
}

void CContourDoc::LocalKriging(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
{
	m_ir.LoadFromLayerData(&(pLayerDoc->m_LayerData), xaxisindx, yaxisindx, dataindx, 0.5); 

	vector<Point3D>& input = const_cast<vector<Point3D>&>(m_ir.Get3DPoints());

	Point3D *   Pbegin = &(*(input.begin()));
	Point3D *   Pend = &(*(input.end()-1));

	float semivariance = 4;

	int countx = 0;
	int county = 0;
	double xstart, xend, xstep;
	double ystart, yend, ystep;

	xstart = 0.0;
//	xend = 30.0;
	xend = 120.0;
	xstep = 0.5;

	ystart = -10.7;
	yend = -1.5;
	ystep = 0.5;

	double zmax, zmin;
	zmax = -1e12;
	zmin = 1e12;

	m_GridPacket.m_Version = 2;
	m_GridPacket.m_nCol = (int)((xend - xstart)/xstep) + 1;
	m_GridPacket.m_nRow = (int)((yend - ystart)/ystep) + 1;
	m_GridPacket.m_xLL = 0.0;
	m_GridPacket.m_yLL = -10.7;
	m_GridPacket.m_xSize = xstep;
	m_GridPacket.m_ySize = ystep;

	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);

	int nSize = 0;
	Point3D * start = Pbegin;
	Point3D * last = Pend;

	while(start != last) {
		++nSize;
		++start;
	}

	start = Pbegin;
	InputReader temp_ir;
	temp_ir.ClearAll();

	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
	{
		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
		{
			double x = xstart + xindx * xstep;
			double y = ystart + yindx * ystep;

			int count = 0;
			for(int i=0; i<nSize; i++) {
				double dist = ::GetDistance(x, y, start, i);
				//if(xindx == 229)
				//{
				//	TRACE("x=%f, y=%f, xx = %f, yy = %f, dist = %f\n", x, y, (*(start+i)).x, (*(start+i)).y, dist); 
				//}
				if(dist <semivariance)
				{
					temp_ir.Add3DPoint((*(start+i)).x, (*(start+i)).y, (*(start+i)).z);
					count++;
				}
			}
			TRACE("count = %d/%d\n", count, nSize);

			vector<Point3D>& tinput = const_cast<vector<Point3D>&>(temp_ir.Get3DPoints());

			Point3D *   tPbegin = &(*(tinput.begin()));
			Point3D *   tPend = &(*(tinput.end()-1));

			Interpolater* tpInterpolater = NULL;
			tpInterpolater = new Kriging(tPbegin, tPend, semivariance, 2);

			double z = tpInterpolater->GetInterpolatedZ(x, y, tPbegin, tPend);
			delete tpInterpolater;
			temp_ir.ClearAll();

			if(z > zmax) zmax = z;
			if(z < zmin) zmin = z;
			m_GridPacket.m_Data[xindx][yindx] = z;
		}
	TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
	}


	m_GridPacket.m_zMin = zmin;
	m_GridPacket.m_zMax = zmax;
	m_GridPacket.m_Rotation = 0.0;
	m_GridPacket.m_BlankValue = 1.7014e+38;
}

void CContourDoc::LocalKriging4(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
{
	m_ir.LoadFromLayerData(&(pLayerDoc->m_LayerData), xaxisindx, yaxisindx, dataindx, 0.5); 

	vector<Point3D>& input = const_cast<vector<Point3D>&>(m_ir.Get3DPoints());

	Point3D *   Pbegin = &(*(input.begin()));
	Point3D *   Pend = &(*(input.end()-1));

	float semivariance = 3;

	int countx = 0;
	int county = 0;
	double xstart, xend, xstep;
	double ystart, yend, ystep;

	xstart = 0.0;
//	xend = 30.0;
	xend = 120.0;
	xstep = 0.5;

	ystart = -10.7;
	yend = -1.5;
	ystep = 0.5;

	double zmax, zmin;
	zmax = -1e12;
	zmin = 1e12;

	m_GridPacket.m_Version = 2;
	m_GridPacket.m_nCol = (int)((xend - xstart)/xstep) + 1;
	m_GridPacket.m_nRow = (int)((yend - ystart)/ystep) + 1;
	m_GridPacket.m_xLL = 0.0;
	m_GridPacket.m_yLL = -10.7;
	m_GridPacket.m_xSize = xstep;
	m_GridPacket.m_ySize = ystep;

	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);

	int nSize = 0;
	Point3D * start = Pbegin;
	Point3D * last = Pend;

	while(start != last) {
		++nSize;
		++start;
	}

	start = Pbegin;
	InputReader temp_ir;
	temp_ir.ClearAll();

	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
	{
		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
		{
			double x = xstart + xindx * xstep;
			double y = ystart + yindx * ystep;

			vector<double> distvector;
			for(int i=0; i<nSize; i++) {
				double dist = ::GetDistance(x, y, start, i);
				distvector.push_back(dist);
			}

			sort(distvector.begin(), distvector.end());

			double distthreshold = distvector[10];

			distvector.clear();

			int count = 0;
			for(int i=0; i<nSize; i++) {
				double dist = ::GetDistance(x, y, start, i);
				if(dist <=distthreshold)
				{
					temp_ir.Add3DPoint((*(start+i)).x, (*(start+i)).y, (*(start+i)).z);
					count++;
				}
			}
			TRACE("count = %d/%d\n", count, nSize);

			vector<Point3D>& tinput = const_cast<vector<Point3D>&>(temp_ir.Get3DPoints());

			Point3D *   tPbegin = &(*(tinput.begin()));
			Point3D *   tPend = &(*(tinput.end()-1));

			Interpolater* tpInterpolater = NULL;
			tpInterpolater = new Kriging(tPbegin, tPend, 10, 1);

			double z = tpInterpolater->GetInterpolatedZ(x, y, tPbegin, tPend);
			delete tpInterpolater;
			temp_ir.ClearAll();

			if(z > zmax) zmax = z;
			if(z < zmin) zmin = z;
			m_GridPacket.m_Data[xindx][yindx] = z;
		}
	TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
	}


	m_GridPacket.m_zMin = zmin;
	m_GridPacket.m_zMax = zmax;
	m_GridPacket.m_Rotation = 0.0;
	m_GridPacket.m_BlankValue = 1.7014e+38;
}

void CContourDoc::LocalKriging3(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
{
	m_ir.LoadFromLayerData(&(pLayerDoc->m_LayerData), xaxisindx, yaxisindx, dataindx, xaxisstep); 

	vector<Point3D>& input = const_cast<vector<Point3D>&>(m_ir.Get3DPoints());

	Point3D *   Pbegin = &(*(input.begin()));
	Point3D *   Pend = &(*(input.end()-1));

	float semivariance = 15;

	vector<double> vecZs;
	int countx = 0;
	int county = 0;
	double xstart, xend, xstep;
	double ystart, yend, ystep;

	xstart = 0.0;
	xend = 30.0;
	xstep = 0.5;

	ystart = -10.7;
	yend = -1.5;
	ystep = 0.5;

	double zmax, zmin;
	zmax = -1e12;
	zmin = 1e12;

	m_GridPacket.m_Version = 2;
	m_GridPacket.m_nCol = (int)((xend - xstart)/xstep) + 1;
	m_GridPacket.m_nRow = (int)((yend - ystart)/ystep) + 1;
	m_GridPacket.m_xLL = 0.0;
	m_GridPacket.m_yLL = -10.7;
	m_GridPacket.m_xSize = xstep;
	m_GridPacket.m_ySize = ystep;

	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);

	int nSize = 0;
	Point3D * start = Pbegin;
	Point3D * last = Pend;

	while(start != last) {
		++nSize;
		++start;
	}

	start = Pbegin;
	InputReader temp_ir;
	temp_ir.ClearAll();

	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
	{
		double x = xstart + xindx * xstep;
		int count = 0;
		for(int i=0; i<nSize; i++) {
			double dist = fabs(x-(*(start+i)).x);
			//if(xindx == 229)
			//{
			//	TRACE("x=%f, y=%f, xx = %f, yy = %f, dist = %f\n", x, y, (*(start+i)).x, (*(start+i)).y, dist); 
			//}
			if(dist <semivariance)
			{
				temp_ir.Add3DPoint((*(start+i)).x, (*(start+i)).y, (*(start+i)).z);
				count++;
			}
		}
		TRACE("count = %d/%d\n", count, nSize);

		vector<Point3D>& tinput = const_cast<vector<Point3D>&>(temp_ir.Get3DPoints());

		Point3D *   tPbegin = &(*(tinput.begin()));
		Point3D *   tPend = &(*(tinput.end()-1));

		Interpolater* tpInterpolater = NULL;
		tpInterpolater = new Kriging(tPbegin, tPend, 10, 2);

		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
		{
			double y = ystart + yindx * ystep;


			double z = tpInterpolater->GetInterpolatedZ(x, y, tPbegin, tPend);

			if(z > zmax) zmax = z;
			if(z < zmin) zmin = z;
			m_GridPacket.m_Data[xindx][yindx] = z;
			vecZs.push_back(z);		

		}
		delete tpInterpolater;
		temp_ir.ClearAll();
		TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
	}


	m_GridPacket.m_zMin = zmin;
	m_GridPacket.m_zMax = zmax;
	m_GridPacket.m_Rotation = 0.0;
	m_GridPacket.m_BlankValue = 1.7014e+38;
}


void CContourDoc::InitFromLayerInformation(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
{
	AfxGetApp()->DoWaitCursor(1);
//	GlobalKriging(pLayerDoc, xaxisindx, yaxisindx, dataindx, xaxisstep);
	LocalKriging4(pLayerDoc, xaxisindx, yaxisindx, dataindx, xaxisstep);
//	LocalKriging2(pLayerDoc, xaxisindx, yaxisindx, dataindx, xaxisstep);
//	LocalKriging3(pLayerDoc, xaxisindx, yaxisindx, dataindx, xaxisstep);
	AfxGetApp()->DoWaitCursor(-1);
}

void CContourDoc::OnCompareWithSurfer()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(TRUE, _T("grd"), _T("*.grd"), NULL, _T("Grid files(*.grd)"), NULL);
	CString filename;

	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName();

		CBridgeSurveyApp* pApp = static_cast<CBridgeSurveyApp*>(::AfxGetApp());

		CContourDoc* p2DDoc = static_cast<CContourDoc*>(pApp->m_pContourTemplate->CreateNewDocument());
		p2DDoc->CompareWithSurfer(this, filename);
		CFrameWnd* p2DFrame = pApp->m_pContourTemplate->CreateNewFrame(p2DDoc, NULL);
		pApp->m_pContourTemplate->InitialUpdateFrame(p2DFrame, p2DDoc);
	}
}

void CContourDoc::CompareWithSurfer(CContourDoc * pSrcDoc, LPCTSTR lpszPathName)
{
	CGridPacket RefGridPacket;
	RefGridPacket.Load(lpszPathName);

	CPathNameInfo filename(lpszPathName);
	SetTitle("comp - " + filename.m_FileName);


	m_GridPacket.m_Version = pSrcDoc->m_GridPacket.m_Version;
	m_GridPacket.m_nCol = pSrcDoc->m_GridPacket.m_nCol;//(int)((xend - xstart)/xstep) + 1;
	m_GridPacket.m_nRow = pSrcDoc->m_GridPacket.m_nRow;
	m_GridPacket.m_xLL = pSrcDoc->m_GridPacket.m_xLL;
	m_GridPacket.m_yLL = pSrcDoc->m_GridPacket.m_yLL;
	m_GridPacket.m_xSize = pSrcDoc->m_GridPacket.m_xSize;
	m_GridPacket.m_ySize = pSrcDoc->m_GridPacket.m_ySize;

	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);

	double zmax, zmin;
	zmax = -1e12;
	zmin = 1e12;
	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
	{
		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
		{
			double z = pSrcDoc->m_GridPacket.m_Data[xindx][yindx] - RefGridPacket.m_Data[xindx][yindx];
			if(z > zmax) zmax = z;
			if(z < zmin) zmin = z;
			m_GridPacket.m_Data[xindx][yindx] = z;
//			vecZs.push_back(z);			
		}
	TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
	}
	m_zMax = m_GridPacket.m_zMax = zmax;
	m_zMin = m_GridPacket.m_zMin = zmin;
	m_ir.LoadFromGridPacket(&m_GridPacket);
}

void CContourDoc::OnCompRstExport()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), NULL, _T("Text files(*.txt)"), NULL);
	CString filename;

	FILE *fout;
	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName();

		fout = fopen(filename, "w");

	}
	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
	{
		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
		{
			fprintf(fout, "%lf ", m_GridPacket.m_Data[xindx][yindx]);
//			vecZs.push_back(z);			
		}
		fprintf(fout, "\n");
	}
	fclose(fout);

}


char szFilter_grid[] = "Grid file for Surfer 12.0(*.grd) |*.grd|";

void CContourDoc::OnGridSave()
{
	CFileDialog dlg(FALSE, _T("grd"), NULL, OFN_HIDEREADONLY, szFilter_grid);

	if(IDOK == dlg.DoModal()) {
			CString strPathName = dlg.GetPathName();
			m_GridPacket.Save(strPathName);
			((CMainFrame *)AfxGetMainWnd())->m_wndFileView.m_GridFileHistory.Add(dlg.GetFileName());
			((CMainFrame *)AfxGetMainWnd())->m_wndFileView.RefreshFileView();
	}	
}

#include "..\LevelFileDlg.h"

void CContourDoc::OnLevelApplyBtn()
{
	CLevelFileDlg dlg;

	dlg.m_pLevelFile = &m_LevelFile;
	if(dlg.DoModal() == IDOK)
	{
		m_bRedraw = true;
		UpdateAllViews(NULL, NULL, NULL);
	}
}

void CContourDoc::FillUpGeneralProperty() // Sanghyun 102314 for PropertyWnd START
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	CPropertiesWnd* pPropWnd =& (pMainWnd->m_wndProperties);
	CMFCPropertyGridCtrl* pPropList =(CMFCPropertyGridCtrl*) &( pPropWnd->m_wndPropList );

	CString tempString;

	pPropList->RemoveAll();
	pPropWnd->m_wndObjectCombo.SetCurSel(0); // Sanghyun 102314 for PropertyWnd

	pPropWnd->SetPropListFont();

	pPropList->EnableHeaderCtrl(FALSE);
	pPropList->EnableDescriptionArea();
	pPropList->SetVSDotNetLook();
	pPropList->MarkModifiedProperties();

	CPathNameInfo m_TempName = CPathNameInfo(m_PathName);

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("�Ϲ�����")); 
	tempString.Format("%s",m_TempName.m_FileName); 
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("�����̸�"),tempString, _T("���� �̸��� ǥ���մϴ�")));
	tempString = m_TempName.m_Drive + m_TempName.m_Dir;
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("���ϰ��"), tempString, _T("���� ��θ� ǥ���մϴ�")));
	pPropList->AddProperty(pGroup1);
} // Sanghyun 102314 for PropertyWnd END

void CContourDoc::FillUpProperty() //Sang for PropertyWnd
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	CPropertiesWnd* pPropWnd =& (pMainWnd->m_wndProperties);
	CMFCPropertyGridCtrl* pPropList =(CMFCPropertyGridCtrl*) &( pPropWnd->m_wndPropList );

	CString tempString;

	pPropList->RemoveAll();
		pPropWnd->m_wndObjectCombo.SetCurSel(1); // Sanghyun 102314 for PropertyWnd

	pPropWnd->SetPropListFont();

	pPropList->EnableHeaderCtrl(FALSE);
	pPropList->EnableDescriptionArea();
	pPropList->SetVSDotNetLook();
	pPropList->MarkModifiedProperties();

	// Header file info
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("��� ����"));
	tempString.Format("%d",m_GridPacket.m_Version);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("����"),tempString, _T("Grid������ ������ ǥ���մϴ�")));
	pPropList->AddProperty(pGroup1);


	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("�׸����׸�"));
	tempString.Format("%d",m_GridPacket.m_nRow);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��"), tempString, _T("������ ���� ���� ǥ���մϴ�")));
	tempString.Format("%d",m_GridPacket.m_nCol);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��"),tempString, _T("������ ���� ���� ǥ���մϴ�")));
	tempString.Format("%.2f",m_GridPacket.m_xLL);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("xLL"), tempString, _T("�׸��� ���ʾƷ��� x��")));
	tempString.Format("%.2f",m_GridPacket.m_yLL);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("yLL"),tempString, _T("�׸��� ���ʾƷ��� y��")));
	tempString.Format("%.2f",m_GridPacket.m_xSize);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("xSize"), tempString, _T("x���� ���� ���� ��尣 �Ÿ�")));
	tempString.Format("%.2f",m_GridPacket.m_ySize);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ySize"),tempString, _T("y���� �ణ ���� ��尣 �Ÿ�")));
	tempString.Format("%.2f",m_GridPacket.m_zMin);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("zMin"), tempString, _T("�׸��忡�� z�ּҰ�")));
	tempString.Format("%.2f",m_GridPacket.m_zMax);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("zMax"),tempString, _T("�׸��忡�� z�ִ밪")));
	tempString.Format("%.2f",m_GridPacket.m_Rotation);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Rotation"), tempString, _T("���� ������")));
	tempString.Format("%.2f",m_GridPacket.m_BlankValue);
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("BlankValue"),tempString, _T("�̰����� ���ų� Ŭ��� ��尪�� ������")));

	pPropList->AddProperty(pGroup2);
	

	
}

void CContourDoc::FillUpDispProperty()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	CPropertiesWnd* pPropWnd =& (pMainWnd->m_wndProperties);
	CMFCPropertyGridCtrl* pPropList =(CMFCPropertyGridCtrl*) &( pPropWnd->m_wndPropList );

	CString tempString;

	pPropList->RemoveAll();
		pPropWnd->m_wndObjectCombo.SetCurSel(2); // Sanghyun 102314 for PropertyWnd

	pPropWnd->SetPropListFont();

	pPropList->EnableHeaderCtrl(FALSE);
	pPropList->EnableDescriptionArea();
	pPropList->SetVSDotNetLook();
	pPropList->MarkModifiedProperties();

	//		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("����Ӽ�"));
//		tempString.Format("%d",m_RadanPacket.m_Header[0].nchan);
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("ä��ǥ��"),tempString, _T("ǥ���ϴ� ä���� ǥ���մϴ�")));
//		tempString.Format("%d",m_RadanPacket.m_Header[0].nchan);
		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("����Ӽ�"));
    tempString.Format("");
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("ä��ǥ��"), tempString, _T("ǥ���ϴ� ä���� ǥ���մϴ�")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��Ŀ����"), tempString, _T("��Ŀ������ �����մϴ�")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("�׸������ǥ��"), tempString, _T("���ڹ��� ǥ�ø� �˷��ݴϴ�")));

	pPropList->AddProperty(pGroup1);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("�����ͼӼ�"));

	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ǥ����"), tempString, _T("ǥ��(Surface) ����� ǥ���մϴ�")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("���鵥���ͼ���"), tempString, _T("���鵥���� ������ ǥ���մϴ�")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("���ۻ���"), tempString, _T("���ۻ����� ǥ���մϴ� ")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("������"), tempString, _T("�������� ǥ���մϴ� ")));

	pPropList->AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Oscope-�Ķ����"));

	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("ä���"), tempString, _T("ä��⸦ ǥ���մϴ�")));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("ä���ܰ�"), tempString, _T("ä���ܰ�(Fill level)�� ǥ���մϴ�")));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("����"), tempString, _T("������ ǥ���մϴ� ")));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������"), tempString, _T("���β��� ǥ���մϴ� ")));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("����"), tempString, _T("���λ����� ǥ���մϴ�")));
	pPropList->AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Wiggle �Ķ����"));

	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("Chop"), tempString, _T("ä�μ��� ǥ���մϴ�")));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("ä���"),tempString, _T("���׳� ������ ǥ���մϴ�")));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("ä��ⷹ��"), tempString, _T("���׳� �Ϸù�ȣ�� ǥ���մϴ� ")));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("������"), tempString, _T("��ġ��  ǥ���մϴ�")));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("��ŵ"), tempString, _T("������ ǥ���մϴ�")));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("����"), tempString, _T("�ֻ��� ǥ���� ǥ���մϴ� ")));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("�����̽�"), tempString, _T("���̸� ǥ���մϴ� ")));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("������"), tempString, _T("���β��� ǥ���մϴ� ")));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("����"), tempString, _T("���λ����� ǥ���մϴ�")));
	
	pPropList->AddProperty(pGroup4);	
}

void CContourDoc::OnComboBoxSelChange()
{
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	CPropertiesWnd* pPropWnd =& (pMainWnd->m_wndProperties);
	CMFCPropertyGridCtrl* pPropList =(CMFCPropertyGridCtrl*) &( pPropWnd->m_wndPropList );

    switch ( pPropWnd->m_wndObjectCombo.GetCurSel()) {
		case 0:
            FillUpGeneralProperty();
            break;
		case 1:
            FillUpProperty();
            break;
        case 2:
            FillUpDispProperty();
            break;
    };
} //Sang for PropertyWnd