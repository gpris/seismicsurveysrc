// ContourDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\SeismicSurvey.h"
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
	m_xMax = 0;;
	m_xMin = 0;
	m_xStep = 0.5;

	m_yMax = 0;
	m_yMin = 0;
	m_yStep = 0.5;

	m_zMax = 20;
	m_zMin = 0;

	m_bAutoGain = false;

	m_LevelFile.m_LevelNum = 20;

	GenerateDefaultLevelFile();
	m_bRedraw = true;
	m_bPropDisplayed = false; //Sang for PropertyWnd
	m_nPropId = 0; // sanghyun 102414
	m_TotalArea = 0; // sanghyun 102414

	m_XDrawStart = 0;
	m_XDrawEnd = 0;
	m_YDrawStart = 0;
	m_YDrawEnd = 0;

	m_XDrawTickStep = 10;
	m_YDrawTickStep = 2;
	m_DataID = -1;

	m_XDrawTickValues = NULL;
	m_YDrawTickValues = NULL;

	m_XDrawTickNo = 0;
	m_YDrawTickNo = 0;

	m_bXDrawTickAuto = true;
	m_bYDrawTickAuto = true;
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
	//ON_COMMAND(ID_COMPARESURFER, &CContourDoc::OnCompareWithSurfer)
	//ON_COMMAND(ID_COMP_RST_EXPORT, &CContourDoc::OnCompRstExport)
	//ON_COMMAND(ID_GRID_SAVE, &CContourDoc::OnGridSave)
	//ON_COMMAND(ID_LEVELAPPLY_BTN, &CContourDoc::OnLevelApplyBtn)
	//ON_CBN_SELCHANGE(ID_PROPS_COMBO, OnComboBoxSelChange) //Sang for PropertyWnd
	//ON_COMMAND(ID_DAMAGEANALBTN, &CContourDoc::OnDamageAnalysis)
	//ON_COMMAND(ID_CONTOUR_AXIS_BTN, &CContourDoc::OnContourAxisBtn)
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

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//m_GridPacket.Load(lpszPathName);
	//m_ir.LoadFromGridPacket(&m_GridPacket);

	InitFromGridFile(lpszPathName);

	UpdateAllViews(NULL, NULL, NULL);

	return true;
}

#include "..\PathNameInfo\PathNameInfo.h"

//#include "..\BridgeSurvey.h"

void CContourDoc::InitFromGridFile(LPCTSTR lpszPathName)
{
//	m_GridPacket.Load(lpszPathName);
	m_GridPacket.LoadDSAAFormat(lpszPathName);
	m_ir.LoadFromGridPacket(&m_GridPacket);

	m_xaxis = new double[m_GridPacket.m_nCol];
	m_yaxis = new double[m_GridPacket.m_nRow];
	for(int col = 0; col<m_GridPacket.m_nCol; col++)
	{
		m_xaxis[col] = m_GridPacket.m_xLL + col * m_GridPacket.m_xSize;
	}

	for(int row = 0; row<m_GridPacket.m_nRow; row++)
	{
		m_yaxis[row] = m_GridPacket.m_yLL + row * m_GridPacket.m_ySize;
	}

	m_XDrawStart = m_xaxis[0];
	m_XDrawEnd = m_xaxis[m_GridPacket.m_nCol-1];
	m_YDrawStart = m_yaxis[0];
	m_YDrawEnd = m_yaxis[m_GridPacket.m_nRow-1];

	m_XDrawTickNo = (int)((m_XDrawEnd - m_XDrawStart)/m_XDrawTickStep) + 1;
	if(m_XDrawTickValues)
		delete m_XDrawTickValues;

	m_XDrawTickValues = new double[m_XDrawTickNo];
	for(int indx = 0; indx<m_XDrawTickNo; indx++)
	{
		m_XDrawTickValues[indx] = m_XDrawStart + m_XDrawTickStep * indx;
	}

	m_YDrawTickNo = (int)((m_YDrawEnd - m_YDrawStart)/m_YDrawTickStep) + 1;
	if(m_YDrawTickValues)
		delete m_YDrawTickValues;

	m_YDrawTickValues = new double[m_YDrawTickNo];
	for(int indx = 0; indx<m_YDrawTickNo; indx++)
	{
		m_YDrawTickValues[indx] = m_YDrawStart + m_YDrawTickStep * indx;
	}

	m_zMin = m_GridPacket.m_zMin;
	m_zMax = m_GridPacket.m_zMax;
	GenerateDefaultLevelFile();

	m_data2d = m_GridPacket.m_Data;

	m_Contour.Contour(m_data2d, 0, m_GridPacket.m_nCol-1, 0, m_GridPacket.m_nRow-1, m_xaxis, m_yaxis, &(m_LevelFile));

	m_PathName.Format("%s", lpszPathName); // sanghyun 102314
	
	CPathNameInfo filename(lpszPathName);
	SetTitle(filename.m_FileName);
	//FillUpProperty(); //Sang for PropertyWnd
}

#include "Kriging.h"
#include <algorithm>
#include <fstream>

//void CContourDoc::GlobalKriging(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
//{
//	double xstart, xend, xstep;
//	double ystart, yend, ystep;
//
//	xstart = 0.0;
//	xend = 30.0;
////	xend = 120.0;
//	xstep = xaxisstep;
//
//	m_ir.LoadFromLayerData(&(pLayerDoc->m_LayerData), xaxisindx, yaxisindx, dataindx, 0.5, xstart, xend); 
//
//	vector<Point3D>& input = const_cast<vector<Point3D>&>(m_ir.Get3DPoints());
//
//	Point3D *   Pbegin = &(*(input.begin()));
//	Point3D *   Pend = &(*(input.end()-1));
//
//	float semivariance = 10.0;
//	Interpolater* pInterpolater = NULL;
//	TRACE("Kriging Start\n");
//	pInterpolater = new Kriging(Pbegin, Pend, semivariance, 2);
//	TRACE("Kriging Stop\n");
//
//	vector<double> vecZs;
//	int countx = 0;
//	int county = 0;
//
//
//	ystart = -10.7;
//	yend = -1.5;
//	ystep = 0.5;
//
//	double zmax, zmin;
//	zmax = -1e12;
//	zmin = 1e12;
//
//	m_GridPacket.m_Version = 2;
//	m_GridPacket.m_nCol = (int)((xend - xstart)/xstep) + 1;
//	m_GridPacket.m_nRow = (int)((yend - ystart)/ystep) + 1;
//	m_GridPacket.m_xLL = 0.0;
//	m_GridPacket.m_yLL = -10.7;
//	m_GridPacket.m_xSize = xstep;
//	m_GridPacket.m_ySize = ystep;
//
//	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);
//
//	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
//	{
//		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
//		{
//
//			double x = xstart + xindx * xstep;
//			double y = ystart + yindx * ystep;
//
//			double z = pInterpolater->GetInterpolatedZ(x, y, Pbegin, Pend);
//			if(z > zmax) zmax = z;
//			if(z < zmin) zmin = z;
//			m_GridPacket.m_Data[xindx][yindx] = z;
//			vecZs.push_back(z);		
//
//		}
//	TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
//	}
//
//	delete pInterpolater;
//
//
//	m_GridPacket.m_zMin = zmin;
//	m_GridPacket.m_zMax = zmax;
//	m_GridPacket.m_Rotation = 0.0;
//	m_GridPacket.m_BlankValue = 1.7014e+38;
//}
//
//void CContourDoc::LocalKriging(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
//{
//	m_ir.LoadFromLayerData(&(pLayerDoc->m_LayerData), xaxisindx, yaxisindx, dataindx, 0.5); 
//
//	vector<Point3D>& input = const_cast<vector<Point3D>&>(m_ir.Get3DPoints());
//
//	Point3D *   Pbegin = &(*(input.begin()));
//	Point3D *   Pend = &(*(input.end()-1));
//
//	float semivariance = 4;
//
//	int countx = 0;
//	int county = 0;
//	double xstart, xend, xstep;
//	double ystart, yend, ystep;
//
////	xstart = 0.0;
//////	xend = 30.0;
////	xend = 120.0;
////	xstep = 0.5;
////
////	ystart = -10.7;
////	yend = -1.5;
////	ystep = 0.5;
//
//	xstart = m_xMin;
//	xend = m_xMax;
//	xstep = m_xStep;
//
//	ystart = m_yMin;
//	yend = m_yMax;
//	ystep = m_yStep;
//
//	double zmax, zmin;
//	zmax = -1e12;
//	zmin = 1e12;
//
//	m_GridPacket.m_Version = 2;
//	m_GridPacket.m_nCol = (int)((xend - xstart)/xstep) + 1;
//	m_GridPacket.m_nRow = (int)((yend - ystart)/ystep) + 1;
//	m_GridPacket.m_xLL = 0.0;
//	m_GridPacket.m_yLL = -10.7;
//	m_GridPacket.m_xSize = xstep;
//	m_GridPacket.m_ySize = ystep;
//
//	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);
//
//	int nSize = 0;
//	Point3D * start = Pbegin;
//	Point3D * last = Pend;
//
//	while(start != last) {
//		++nSize;
//		++start;
//	}
//
//	start = Pbegin;
//	InputReader temp_ir;
//	temp_ir.ClearAll();
//
//	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
//	{
//		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
//		{
//			double x = xstart + xindx * xstep;
//			double y = ystart + yindx * ystep;
//
//			int count = 0;
//			for(int i=0; i<nSize; i++) {
//				double dist = ::GetDistance(x, y, start, i);
//				//if(xindx == 229)
//				//{
//				//	TRACE("x=%f, y=%f, xx = %f, yy = %f, dist = %f\n", x, y, (*(start+i)).x, (*(start+i)).y, dist); 
//				//}
//				if(dist <semivariance)
//				{
//					temp_ir.Add3DPoint((*(start+i)).x, (*(start+i)).y, (*(start+i)).z);
//					count++;
//				}
//			}
//			TRACE("count = %d/%d\n", count, nSize);
//
//			vector<Point3D>& tinput = const_cast<vector<Point3D>&>(temp_ir.Get3DPoints());
//
//			Point3D *   tPbegin = &(*(tinput.begin()));
//			Point3D *   tPend = &(*(tinput.end()-1));
//
//			Interpolater* tpInterpolater = NULL;
//			tpInterpolater = new Kriging(tPbegin, tPend, semivariance, 2);
//
//			double z = tpInterpolater->GetInterpolatedZ(x, y, tPbegin, tPend);
//			delete tpInterpolater;
//			temp_ir.ClearAll();
//
//			if(z > zmax) zmax = z;
//			if(z < zmin) zmin = z;
//			m_GridPacket.m_Data[xindx][yindx] = z;
//		}
//	TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
//	}
//
//
//	m_GridPacket.m_zMin = zmin;
//	m_GridPacket.m_zMax = zmax;
//	m_GridPacket.m_Rotation = 0.0;
//	m_GridPacket.m_BlankValue = 1.7014e+38;
//}
//
//void CContourDoc::LocalKriging4(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
//{
//	m_ir.LoadFromLayerData(&(pLayerDoc->m_LayerData), xaxisindx, yaxisindx, dataindx, 0.5); 
//
//	vector<Point3D>& input = const_cast<vector<Point3D>&>(m_ir.Get3DPoints());
//
//	Point3D *   Pbegin = &(*(input.begin()));
//	Point3D *   Pend = &(*(input.end()-1));
//
//	float semivariance = 3;
//
//	int countx = 0;
//	int county = 0;
//	double xstart, xend, xstep;
//	double ystart, yend, ystep;
//
////	xstart = 0.0;
//////	xend = 30.0;
////	xend = 120.0;
////	xstep = 0.5;
////
////	ystart = -10.7;
////	yend = -1.5;
////	ystep = 0.5;
//
//	xstart = m_xMin;
//	xend = m_xMax;
//	xstep = m_xStep;
//
//	ystart = m_yMin;
//	yend = m_yMax;
//	ystep = m_yStep;
//
//	double zmax, zmin;
//	zmax = -1e12;
//	zmin = 1e12;
//
//	m_GridPacket.m_Version = 2;
//	m_GridPacket.m_nCol = (int)((xend - xstart)/xstep) + 1;
//	m_GridPacket.m_nRow = (int)((yend - ystart)/ystep) + 1;
//	m_GridPacket.m_xLL = xstart;
//	m_GridPacket.m_yLL = ystart;
//	m_GridPacket.m_xSize = xstep;
//	m_GridPacket.m_ySize = ystep;
//
//	m_XDrawStart = xstart;
//	m_XDrawEnd = xend;
//	m_YDrawStart = ystart;
//	m_YDrawEnd = yend;
//
//	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);
//
//	int nSize = 0;
//	Point3D * start = Pbegin;
//	Point3D * last = Pend;
//
//	while(start != last) {
//		++nSize;
//		++start;
//	}
//
//	start = Pbegin;
//	InputReader temp_ir;
//	temp_ir.ClearAll();
//
//	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
//	{
//		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
//		{
//			double x = xstart + xindx * xstep;
//			double y = ystart + yindx * ystep;
//
//			vector<double> distvector;
//			for(int i=0; i<nSize; i++) {
//				double dist = ::GetDistance(x, y, start, i);
//				distvector.push_back(dist);
//			}
//
//			sort(distvector.begin(), distvector.end());
//
//			double distthreshold = distvector[10];
//
//			distvector.clear();
//
//			int count = 0;
//			for(int i=0; i<nSize; i++) {
//				double dist = ::GetDistance(x, y, start, i);
//				if(dist <=distthreshold)
//				{
//					temp_ir.Add3DPoint((*(start+i)).x, (*(start+i)).y, (*(start+i)).z);
//					count++;
//				}
//			}
//			TRACE("count = %d/%d\n", count, nSize);
//
//			vector<Point3D>& tinput = const_cast<vector<Point3D>&>(temp_ir.Get3DPoints());
//
//			Point3D *   tPbegin = &(*(tinput.begin()));
//			Point3D *   tPend = &(*(tinput.end()-1));
//
//			Interpolater* tpInterpolater = NULL;
//			tpInterpolater = new Kriging(tPbegin, tPend, 10, 1);
//
//			double z = tpInterpolater->GetInterpolatedZ(x, y, tPbegin, tPend);
//			delete tpInterpolater;
//			temp_ir.ClearAll();
//
//			if(z > zmax) zmax = z;
//			if(z < zmin) zmin = z;
//			m_GridPacket.m_Data[xindx][yindx] = z;
//		}
//	TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
//	}
//
//
//	m_GridPacket.m_zMin = zmin;
//	m_GridPacket.m_zMax = zmax;
//	m_GridPacket.m_Rotation = 0.0;
//	m_GridPacket.m_BlankValue = 1.7014e+38;
//}
//
//void CContourDoc::LocalKriging3(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
//{
//	m_ir.LoadFromLayerData(&(pLayerDoc->m_LayerData), xaxisindx, yaxisindx, dataindx, xaxisstep); 
//
//	vector<Point3D>& input = const_cast<vector<Point3D>&>(m_ir.Get3DPoints());
//
//	Point3D *   Pbegin = &(*(input.begin()));
//	Point3D *   Pend = &(*(input.end()-1));
//
//	float semivariance = 15;
//
//	vector<double> vecZs;
//	int countx = 0;
//	int county = 0;
//	double xstart, xend, xstep;
//	double ystart, yend, ystep;
//
////	xstart = 0.0;
//////	xend = 30.0;
////	xend = 120.0;
////	xstep = 0.5;
////
////	ystart = -10.7;
////	yend = -1.5;
////	ystep = 0.5;
//
//	xstart = m_xMin;
//	xend = m_xMax;
//	xstep = m_xStep;
//
//	ystart = m_yMin;
//	yend = m_yMax;
//	ystep = m_yStep;
//
//	double zmax, zmin;
//	zmax = -1e12;
//	zmin = 1e12;
//
//	m_GridPacket.m_Version = 2;
//	m_GridPacket.m_nCol = (int)((xend - xstart)/xstep) + 1;
//	m_GridPacket.m_nRow = (int)((yend - ystart)/ystep) + 1;
//	m_GridPacket.m_xLL = 0.0;
//	m_GridPacket.m_yLL = -10.7;
//	m_GridPacket.m_xSize = xstep;
//	m_GridPacket.m_ySize = ystep;
//
//	m_XDrawStart = xstart;
//	m_XDrawEnd = xend;
//	m_YDrawStart = ystart;
//	m_YDrawEnd = yend;
//
//	m_GridPacket.m_Data = m_GridPacket.Make2DData(m_GridPacket.m_nCol, m_GridPacket.m_nRow);
//
//	int nSize = 0;
//	Point3D * start = Pbegin;
//	Point3D * last = Pend;
//
//	while(start != last) {
//		++nSize;
//		++start;
//	}
//
//	start = Pbegin;
//	InputReader temp_ir;
//	temp_ir.ClearAll();
//
//	for(int xindx = 0; xindx<m_GridPacket.m_nCol ; xindx++) 
//	{
//		double x = xstart + xindx * xstep;
//		int count = 0;
//		for(int i=0; i<nSize; i++) {
//			double dist = fabs(x-(*(start+i)).x);
//			//if(xindx == 229)
//			//{
//			//	TRACE("x=%f, y=%f, xx = %f, yy = %f, dist = %f\n", x, y, (*(start+i)).x, (*(start+i)).y, dist); 
//			//}
//			if(dist <semivariance)
//			{
//				temp_ir.Add3DPoint((*(start+i)).x, (*(start+i)).y, (*(start+i)).z);
//				count++;
//			}
//		}
//		TRACE("count = %d/%d\n", count, nSize);
//
//		vector<Point3D>& tinput = const_cast<vector<Point3D>&>(temp_ir.Get3DPoints());
//
//		Point3D *   tPbegin = &(*(tinput.begin()));
//		Point3D *   tPend = &(*(tinput.end()-1));
//
//		Interpolater* tpInterpolater = NULL;
//		tpInterpolater = new Kriging(tPbegin, tPend, 10, 2);
//
//		for(int yindx = 0; yindx<m_GridPacket.m_nRow; yindx++) 
//		{
//			double y = ystart + yindx * ystep;
//
//
//			double z = tpInterpolater->GetInterpolatedZ(x, y, tPbegin, tPend);
//
//			if(z > zmax) zmax = z;
//			if(z < zmin) zmin = z;
//			m_GridPacket.m_Data[xindx][yindx] = z;
//			vecZs.push_back(z);		
//
//		}
//		delete tpInterpolater;
//		temp_ir.ClearAll();
//		TRACE("Interpolate %d/%d\n", xindx, m_GridPacket.m_nCol);
//	}
//
//
//	m_GridPacket.m_zMin = zmin;
//	m_GridPacket.m_zMax = zmax;
//	m_GridPacket.m_Rotation = 0.0;
//	m_GridPacket.m_BlankValue = 1.7014e+38;
//}
//
//void CContourDoc::InitFromLayerInformation(CGridCtrlDoc * pLayerDoc, int xaxisindx, int yaxisindx, int dataindx, float xaxisstep)
//{
//	AfxGetApp()->DoWaitCursor(1);
////	GlobalKriging(pLayerDoc, xaxisindx, yaxisindx, dataindx, xaxisstep);
//	CString titlestring;
//	titlestring = pLayerDoc->GetTitle() + _T(" (") + pLayerDoc->m_LayerData.m_ColumnHeaders[dataindx] + _T(")");
//	SetTitle(titlestring);
////	SetTitle(pLayerDoc->GetTitle());
//
//	LocalKriging4(pLayerDoc, xaxisindx, yaxisindx, dataindx, xaxisstep);
////	LocalKriging2(pLayerDoc, xaxisindx, yaxisindx, dataindx, xaxisstep);
////	LocalKriging3(pLayerDoc, xaxisindx, yaxisindx, dataindx, xaxisstep);
//
//	m_XDrawTickNo = (int)((m_XDrawEnd - m_XDrawStart)/m_XDrawTickStep) + 1;
//	if(m_XDrawTickValues)
//		delete m_XDrawTickValues;
//
//	m_XDrawTickValues = new double[m_XDrawTickNo];
//	for(int indx = 0; indx<m_XDrawTickNo; indx++)
//	{
//		m_XDrawTickValues[indx] = m_XDrawStart + m_XDrawTickStep * indx;
//	}
//
//	m_YDrawTickNo = (int)((m_YDrawEnd - m_YDrawStart)/m_YDrawTickStep) + 1;
//	if(m_YDrawTickValues)
//		delete m_YDrawTickValues;
//
//	m_YDrawTickValues = new double[m_YDrawTickNo];
//	for(int indx = 0; indx<m_YDrawTickNo; indx++)
//	{
//		m_YDrawTickValues[indx] = m_YDrawStart + m_YDrawTickStep * indx;
//	}
//
//	GenerateDefaultLevelFile();
//	m_xaxis = new double[m_GridPacket.m_nCol];
//	m_yaxis = new double[m_GridPacket.m_nRow];
//	for(int col = 0; col<m_GridPacket.m_nCol; col++)
//	{
//		m_xaxis[col] = m_GridPacket.m_xLL + col * m_GridPacket.m_xSize;
//	}
//
//	for(int row = 0; row<m_GridPacket.m_nRow; row++)
//	{
//		m_yaxis[row] = m_GridPacket.m_yLL + row * m_GridPacket.m_ySize;
//	}
//
//	m_data2d = m_GridPacket.m_Data;
//
//	m_DataID = dataindx -4;
//	CBridgeSurveyApp * pApp = (CBridgeSurveyApp *)AfxGetApp();
//	CString levelfilename = pApp->m_DefaultLevelFileName[dataindx -4];
//	m_LevelFile.Load(levelfilename);
//
//	m_Contour.Contour(m_data2d, 0, m_GridPacket.m_nCol-1, 0, m_GridPacket.m_nRow-1, m_xaxis, m_yaxis, &(m_LevelFile));
//
//	//////////////////////////////////////////////////////////////////AUTOSAVE
//	if(pApp->m_ProjectInfo.m_bDefined)
//	{
//		SetCurrentDirectory(pApp->m_ProjectInfo.m_strGridFolder);
//		CString strPathName = pApp->m_ProjectInfo.m_strGridFolder+ _T("\\")+ titlestring +_T("_autosave")+_T(".grd");
//		m_GridPacket.Save(strPathName);
//		((CMainFrame *)AfxGetMainWnd())->m_wndFileView.m_GridFileHistory.Add(strPathName);
//		((CMainFrame *)AfxGetMainWnd())->m_wndFileView.RefreshFileView();
//	}
//	//////////////////////////////////////////////////////////////////AUTOSAVE
//	
//	AfxGetApp()->DoWaitCursor(-1);
//}

void CContourDoc::OnCompareWithSurfer()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, _T("grd"), _T("*.grd"), NULL, _T("Grid files(*.grd)"), NULL);
	CString filename;

	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName();

		CSeismicSurveyApp * pApp = static_cast<CSeismicSurveyApp*>(::AfxGetApp());

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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
	CSeismicSurveyApp * pApp = (CSeismicSurveyApp *)AfxGetApp();
	//SetCurrentDirectory(pApp->m_ProjectInfo.m_strGridFolder);
	CFileDialog dlg(FALSE, _T("grd"), NULL, OFN_HIDEREADONLY, szFilter_grid);

	if(IDOK == dlg.DoModal()) {
			CString strPathName = dlg.GetPathName();
			m_GridPacket.Save(strPathName);
			//((CMainFrame *)AfxGetMainWnd())->m_wndFileView.m_GridFileHistory.Add(dlg.GetFileName());
			//((CMainFrame *)AfxGetMainWnd())->m_wndFileView.RefreshFileView();
	}	
}

//#include "..\GridAxisDlg.h"
//
//void CContourDoc::OnContourAxisBtn()
//{
//	CGridAxisDlg dlg;
//	
//	dlg.m_XMin = m_XDrawStart;
//	dlg.m_XMax = m_XDrawEnd;
//	dlg.m_XTickStep = m_XDrawTickStep;
//
//	dlg.m_YMin = m_YDrawStart;
//	dlg.m_YMax = m_YDrawEnd;
//	dlg.m_YTickStep = m_YDrawTickStep;
//
//	dlg.m_bXTickAuto = m_bXDrawTickAuto;
//	dlg.m_bYTickAuto = m_bYDrawTickAuto;
//
//	if(dlg.DoModal() == IDOK)
//	{
//		 m_XDrawStart= dlg.m_XMin;
//		 m_XDrawEnd= dlg.m_XMax;
//
//		 m_bXDrawTickAuto = dlg.m_bXTickAuto;
//
//		 if(dlg.m_bXTickAuto)
//		 {
//			 m_XDrawTickStep= dlg.m_XTickStep;
//			 m_XDrawTickNo = (int)((m_XDrawEnd - m_XDrawStart)/m_XDrawTickStep) + 1;
//			 if(m_XDrawTickValues)
//				 delete m_XDrawTickValues;
//
//			 m_XDrawTickValues = new double[m_XDrawTickNo];
//			 for(int indx = 0; indx<m_XDrawTickNo; indx++)
//			 {
//				 m_XDrawTickValues[indx] = m_XDrawStart + m_XDrawTickStep * indx;
//			 }
//		 }
//		 else
//		 {
//			 CStringArray temparray;
//			 m_XDrawTickNo = dlg.ParseTokens(temparray, dlg.m_XTickValues);
//			 if(m_XDrawTickValues)
//				 delete m_XDrawTickValues;
//
//			 m_XDrawTickValues = new double[m_XDrawTickNo];
//			 for(int indx = 0; indx<m_XDrawTickNo; indx++)
//			 {
//				 m_XDrawTickValues[indx] = atof(temparray[indx]);
//			 }		 
//		 }
//
//
//		 m_YDrawStart= dlg.m_YMin;
//		 m_YDrawEnd= dlg.m_YMax;
//		 m_bYDrawTickAuto = dlg.m_bYTickAuto;
//
//		 if(dlg.m_bYTickAuto)
//		 {
//			 m_YDrawTickStep= dlg.m_YTickStep;
//			 m_YDrawTickNo = (int)((m_YDrawEnd - m_YDrawStart)/m_YDrawTickStep) + 1;
//			 if(m_YDrawTickValues)
//				 delete m_YDrawTickValues;
//
//			 m_YDrawTickValues = new double[m_YDrawTickNo];
//			 for(int indx = 0; indx<m_YDrawTickNo; indx++)
//			 {
//				 m_YDrawTickValues[indx] = m_YDrawStart + m_YDrawTickStep * indx;
//			 }
//		 }
//		 else
//		 {
//			 CStringArray temparray;
//			 m_YDrawTickNo = dlg.ParseTokens(temparray, dlg.m_YTickValues);
//			 if(m_YDrawTickValues)
//				 delete m_YDrawTickValues;
//
//			 m_YDrawTickValues = new double[m_YDrawTickNo];
//			 for(int indx = 0; indx<m_YDrawTickNo; indx++)
//			 {
//				 m_YDrawTickValues[indx] = atof(temparray[indx]);
//			 }		 
//		 }
//
//		 m_bRedraw = true;
//		 UpdateAllViews(NULL, NULL, NULL);
//	}
//}

//#include "..\LevelFileDlg.h"
//
//void CContourDoc::OnLevelApplyBtn()
//{
//	CLevelFileDlg dlg;
//
//	dlg.m_pLevelFile = &m_LevelFile;
//	if(dlg.DoModal() == IDOK)
//	{
//		m_bRedraw = true;
//		m_Contour.Contour(m_data2d, 0, m_GridPacket.m_nCol-1, 0, m_GridPacket.m_nRow-1, m_xaxis, m_yaxis, &(m_LevelFile));
//		UpdateAllViews(NULL, NULL, NULL);
//
//	}
//}

void CContourDoc::GenerateDefaultLevelFile()
{
	float colorstep = 255./m_LevelFile.m_LevelNum;
	float levelstep = (m_zMax - m_zMin)/m_LevelFile.m_LevelNum;

	if(!m_LevelFile.m_LevelList.IsEmpty())
	{
		m_LevelFile.m_LevelList.RemoveAll();
	}

	for(int indx = 0; indx< m_LevelFile.m_LevelNum; indx++)
	{
		CLevelProperty * tempProperty = new CLevelProperty;

		tempProperty->m_Level = indx * levelstep + m_zMin;
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
}

//void CContourDoc::FillUpGeneralProperty() // Sanghyun 102314 for PropertyWnd START
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
//	CPropertiesWnd* pPropWnd =& (pMainWnd->m_wndProperties);
//	CMFCPropertyGridCtrl* pPropList =(CMFCPropertyGridCtrl*) &( pPropWnd->m_wndPropList );
//
//	CString tempString;
//
//	pPropList->RemoveAll();
//	pPropWnd->m_wndObjectCombo.SetCurSel(0); // Sanghyun 102314 for PropertyWnd
//
//	pPropWnd->SetPropListFont();
//
//	pPropList->EnableHeaderCtrl(FALSE);
//	pPropList->EnableDescriptionArea();
//	pPropList->SetVSDotNetLook();
//	pPropList->MarkModifiedProperties();
//
//#if 0
//	CPathNameInfo m_TempName = CPathNameInfo(m_PathName);
//
//	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("일반정보")); 
//	tempString.Format("%s",m_TempName.m_FileName); 
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("파일이름"),tempString, _T("파일 이름을 표시합니다")));
//	tempString = m_TempName.m_Drive + m_TempName.m_Dir;
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("파일경로"), tempString, _T("파일 경로를 표시합니다")));
//	pPropList->AddProperty(pGroup1);
//
//#else
//	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("전체손상율계산")); 
//	pPropList->AddProperty(pGroup1);
//
//	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("아스팔트층손상율(Az)")); 
//	// CLevelProperty * tempproperty = m_LevelFile.m_LevelList.GetAt(index);
//	// tempString.Foramt("%d", tempproperty->m_Level);
//	tempString.Format("8.0");
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("하한레벨"), tempString, _T("하한값을 지정합니다")));
//	//m_LowerDamageArea = m_SurveyArea - pProperty->m_Area;
//	tempString.Format("25.12");
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("손상면적(m²)"), tempString, _T("손상면적을 출력합니다")));
//	tempString.Format("%.2f", m_TotalArea );
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("전체면적(m²)"), tempString, _T("전체면적을 출력합니다")));
//	//m_LowerDamageRate = 100.0 - (pProperty->m_Area/m_SurveyArea)*100;
//	tempString.Format("12.25");
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("손상율(%)"), tempString, _T("손상율을 계산합니다")));
//	pPropList->AddProperty(pGroup2);
//
//	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("상부철근층손상율(Rz)")); 
//	// CLevelProperty * tempproperty = m_LevelFile.m_LevelList.GetAt(index);
//	// tempString.Foramt("%d", tempproperty->m_Level);
//	tempString.Format("6.0");
//	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("하한레벨"), tempString, _T("하한값을 지정합니다")));
//	//m_LowerDamageArea = m_SurveyArea - pProperty->m_Area;
//	tempString.Format("25.12");
//	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("손상면적(m²)"), tempString, _T("손상면적을 출력합니다")));
//	tempString.Format("%.2f", m_TotalArea );
//	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("전체면적(m²)"), tempString, _T("전체면적을 출력합니다")));
//	//m_LowerDamageRate = 100.0 - (pProperty->m_Area/m_SurveyArea)*100;
//	tempString.Format("25.12");
//	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("손상율(%)"), tempString, _T("손상율을 계산합니다")));
//	pPropList->AddProperty(pGroup3);
//
//	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("유전율(Ea)")); 
//	// CLevelProperty * tempproperty = m_LevelFile.m_LevelList.GetAt(index);
//	// tempString.Foramt("%d", tempproperty->m_Level);
//	tempString.Format("4.5");
//	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("상한레벨"), tempString, _T("하한값을 지정합니다")));
//	tempString.Format("8");
//	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("하한레벨"), tempString, _T("하한값을 지정합니다")));
//	//m_LowerDamageArea = m_SurveyArea - pProperty->m_Area;
//	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("손상면적(m²)"), tempString, _T("손상면적을 출력합니다")));
//	tempString.Format("%.2f", m_TotalArea );
//	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("전체면적(m²)"), tempString, _T("전체면적을 출력합니다")));
//	//m_LowerDamageRate = 100.0 - (pProperty->m_Area/m_SurveyArea)*100;
//	tempString.Format("25.12");
//	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("손상율"), tempString, _T("손상율을 계산합니다")));
//	pPropList->AddProperty(pGroup4);
//
//	CMFCPropertyGridProperty* pGroup5 = new CMFCPropertyGridProperty(_T("유전율(Ec)")); 
//	// CLevelProperty * tempproperty = m_LevelFile.m_LevelList.GetAt(index);
//	// tempString.Foramt("%d", tempproperty->m_Level);
//	tempString.Format("6.0");
//	pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("상한레벨"), tempString, _T("하한값을 지정합니다")));
//	tempString.Format("12.0");
//	pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("하한레벨"), tempString, _T("하한값을 지정합니다")));
//	//m_LowerDamageArea = m_SurveyArea - pProperty->m_Area;
//	tempString.Format("25.12");
//	pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("손상면적(m²)"), tempString, _T("손상면적을 출력합니다")));
//	tempString.Format("%.2f", m_TotalArea );
//	pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("전체면적(m²)"), tempString, _T("전체면적을 출력합니다")));
//	//m_LowerDamageRate = 100.0 - (pProperty->m_Area/m_SurveyArea)*100;
//	tempString.Format("25.12");
//	pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("손상율(%)"), tempString, _T("손상율을 계산합니다")));
//	pPropList->AddProperty(pGroup5);
//
//	CMFCPropertyGridProperty* pGroup6 = new CMFCPropertyGridProperty(_T("AmpN")); 
//	// CLevelProperty * tempproperty = m_LevelFile.m_LevelList.GetAt(index);
//	// tempString.Foramt("%d", tempproperty->m_Level);
//	tempString.Format("-26.0");
//	pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("하한레벨"), tempString, _T("하한값을 지정합니다")));
//	//m_LowerDamageArea = m_SurveyArea - pProperty->m_Area;
//	tempString.Format("25.12");
//	pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("손상면적(m²)"), tempString, _T("손상면적을 출력합니다")));
//	tempString.Format("%.2f", m_TotalArea );
//	pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("전체면적(m²)"), tempString, _T("전체면적을 출력합니다")));
//	//m_LowerDamageRate = 100.0 - (pProperty->m_Area/m_SurveyArea)*100;
//	tempString.Format("25.12");
//	pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("손상율(%)"), tempString, _T("손상율을 계산합니다")));
//	pPropList->AddProperty(pGroup6);
//
//	CMFCPropertyGridProperty* pGroup7 = new CMFCPropertyGridProperty(_T("AmpN")); 
//	// CLevelProperty * tempproperty = m_LevelFile.m_LevelList.GetAt(index);
//	// tempString.Foramt("%d", tempproperty->m_Level);
//	tempString.Format("-10.0");
//	pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("하한레벨"), tempString, _T("하한값을 지정합니다")));
//	//m_LowerDamageArea = m_SurveyArea - pProperty->m_Area;
//	tempString.Format("25.12");
//	pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("손상면적(m²)"), tempString, _T("손상면적을 출력합니다")));
//	tempString.Format("%.2f", m_TotalArea );
//	pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("전체면적(m²)"), tempString, _T("전체면적을 출력합니다")));
//	//m_LowerDamageRate = 100.0 - (pProperty->m_Area/m_SurveyArea)*100;
//	tempString.Format("25.12");
//	pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("손상율(%)"), tempString, _T("손상율을 계산합니다")));
//	pPropList->AddProperty(pGroup7);
//#endif
//
//
//
//} // Sanghyun 102314 for PropertyWnd END
//
//void CContourDoc::FillUpProperty() //Sang for PropertyWnd
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
//	CPropertiesWnd* pPropWnd =& (pMainWnd->m_wndProperties);
//	CMFCPropertyGridCtrl* pPropList =(CMFCPropertyGridCtrl*) &( pPropWnd->m_wndPropList );
//
//	CString tempString;
//
//	pPropList->RemoveAll();
//		pPropWnd->m_wndObjectCombo.SetCurSel(1); // Sanghyun 102314 for PropertyWnd
//
//	pPropWnd->SetPropListFont();
//
//	pPropList->EnableHeaderCtrl(FALSE);
//	pPropList->EnableDescriptionArea();
//	pPropList->SetVSDotNetLook();
//	pPropList->MarkModifiedProperties();
//
//	// Header file info
//	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("헤더 정보"));
//	tempString.Format("%d",m_GridPacket.m_Version);
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("버젼"),tempString, _T("Grid파일의 버젼을 표시합니다")));
//	pPropList->AddProperty(pGroup1);
//
//
//	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("그리드항목"));
//	tempString.Format("%d",m_GridPacket.m_nRow);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("열"), tempString, _T("파일의 열의 수를 표시합니다")));
//	tempString.Format("%d",m_GridPacket.m_nCol);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("행"),tempString, _T("파일의 행의 수를 표시합니다")));
//	tempString.Format("%.2f",m_GridPacket.m_xLL);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("xLL"), tempString, _T("그리드 왼쪽아래의 x축")));
//	tempString.Format("%.2f",m_GridPacket.m_yLL);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("yLL"),tempString, _T("그리드 왼쪽아래의 y축")));
//	tempString.Format("%.2f",m_GridPacket.m_xSize);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("xSize"), tempString, _T("x방향 열간 인접 노드간 거리")));
//	tempString.Format("%.2f",m_GridPacket.m_ySize);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ySize"),tempString, _T("y방향 행간 인접 노드간 거리")));
//	tempString.Format("%.2f",m_GridPacket.m_zMin);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("zMin"), tempString, _T("그리드에서 z최소값")));
//	tempString.Format("%.2f",m_GridPacket.m_zMax);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("zMax"),tempString, _T("그리드에서 z최대값")));
//	tempString.Format("%.2f",m_GridPacket.m_Rotation);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Rotation"), tempString, _T("현재 사용안함")));
//	tempString.Format("%.2f",m_GridPacket.m_BlankValue);
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("BlankValue"),tempString, _T("이값보다 같거나 클경우 노드값이 지워짐")));
//
//	pPropList->AddProperty(pGroup2);
//	
//
//	
//}
//#include "LevelFile.h" // sanghyun 102314 for PropertyDispWnd START
//#include "..\Newcontrol\CustomProperties.h" //141227 sanghyun
//void CContourDoc::FillUpDispProperty()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
//	CPropertiesWnd* pPropWnd =& (pMainWnd->m_wndProperties);
//	CMFCPropertyGridCtrl* pPropList =(CMFCPropertyGridCtrl*) &( pPropWnd->m_wndPropList );
//
//	CString tempString, tempString2; // sanghyun 102314 for PropertyDispWnd
//
//	pPropList->RemoveAll();
//	for (int k=0;k<50;k++){
//	m_imageList_weight[k].DeleteImageList();
//	m_imageList_style[k].DeleteImageList();
//	}
//	pPropWnd->m_wndObjectCombo.SetCurSel(2); // Sanghyun 102314 for PropertyWnd
//
//	pPropWnd->SetPropListFont();
//
//	pPropList->EnableHeaderCtrl(FALSE);
//	pPropList->EnableDescriptionArea();
//	pPropList->SetVSDotNetLook();
//	pPropList->MarkModifiedProperties();
//
//	CPathNameInfo m_TempName = CPathNameInfo(m_LevelFile.m_FileName);
//
//	CMFCPropertyGridProperty* pGroup0 = new CMFCPropertyGridProperty(_T("총면적"));  // sanghyun 102414
//	tempString.Format("%.2f", m_TotalArea ); 
//	pGroup0->AddSubItem( new CMFCPropertyGridProperty(_T("면적(m²)"),tempString, _T("각 레벨 면적 정보 입니다")));
//	pPropList->AddProperty(pGroup0);
//
//	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("레벨파일 정보")); 
//	tempString.Format("%s",m_TempName.m_FileName); 
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("파일이름"),tempString, _T("파일 이름을 표시합니다")));
//	tempString = m_TempName.m_Drive + m_TempName.m_Dir;
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("파일경로"), tempString, _T("파일 경로를 표시합니다")));
//	pPropList->AddProperty(pGroup1); // sanghyun 102414
//	
//
//
//	std::auto_ptr<CMFCPropertyGridProperty> pGroup2(new CMFCPropertyGridProperty(_T("레벨"), 0, FALSE));
//
//	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Index"),_T("레벨값"), _T("각 레벨값입니다."));
//	for (int index=0; index < m_LevelFile.m_LevelNum; index++)
//	{
//	CLevelProperty * tempproperty = m_LevelFile.m_LevelList.GetAt(index);
//	tempString.Format("%.2f", tempproperty->m_Level);
//	//CMFCPropertyGridProperty* pGroup22 =  new CMFCPropertyGridProperty(tempString, 0, FALSE); 
//	CMFCPropertyGridProperty* pGroup22 =  new CMFCPropertyGridProperty(tempString, 0, TRUE); // to display brief info
//	//CMFCPropertyGridProperty* pGroup22 =  new CMFCPropertyGridProperty(tempString2, tempString, _T("레벨값입니다"));
//	pGroup2->AddSubItem(pGroup22);
//
//#if 0
//	//text version - 141227 sanghyun
//	tempString2.Format("라인 정보");
//	tempString.Format("%s , %d",tempproperty->m_LineStyle,tempproperty->m_LineWidth);
//	pProp =  new CMFCPropertyGridProperty(tempString2,tempString, _T("라인정보 입니다"));
//	//CMFCPropertyGridProperty* pGroup22 =  new CMFCPropertyGridProperty(tempString2);
//	pGroup22->AddSubItem(pProp);
//#else
//	CBitmap bmp;
//	bmp.LoadBitmap(IDB_LINE_WEIGHT_LIST);
//
//	m_imageList_weight[index].Create(50, 16, ILC_MASK | ILC_COLOR24, 0, 0);
//	m_imageList_weight[index].Add(&bmp, RGB(192, 192, 192));
//
//	CStringList lstIconNames_weight;
//	lstIconNames_weight.AddTail(_T(" 0"));
//	lstIconNames_weight.AddTail(_T(" 1"));
//	lstIconNames_weight.AddTail(_T("10"));
//	lstIconNames_weight.AddTail(_T("15"));
//	lstIconNames_weight.AddTail(_T("20"));
//	lstIconNames_weight.AddTail(_T("30"));
//	
//	int ntempSel = tempproperty->m_LineWidth;
//	int nSel;
//
//	switch (ntempSel)
//	{
//	case 0:
//			 nSel=0;
//			 break;
//	case 1:
//			 nSel=1;
//			 break;
//	case 10:
//			 nSel=2;
//			 break;
//	case 15:
//			 nSel=3;
//			 break;
//	case 20:
//			 nSel=4;
//			 break;
//	case 30:
//			 nSel=5;
//			 break;
//	default:
//			nSel=0;
//	}
//	//CMFCPropertyGridProperty* pProp2 = new CIconListProp(_T("Line weight"), m_imageList_weight, 0, &lstIconNames_weight, _T("Lineweight List Property")); 
//	pProp = new CIconListProp(_T("Line weight"), m_imageList_weight[index], nSel, &lstIconNames_weight, _T("Lineweight List Property")); 
//	pGroup22->AddSubItem(pProp);
//	//-----------------------------
//	CBitmap bmp2;
//	bmp2.LoadBitmap(IDB_LINE_STYLE_LIST);
//
//	m_imageList_style[index].Create(50, 16, ILC_MASK | ILC_COLOR24, 0, 0);
//	m_imageList_style[index].Add(&bmp2, RGB(192, 192, 192));
//
//	CStringList lstIconNames_style;
//	lstIconNames_style.AddTail(_T("Invisible"));
//	lstIconNames_style.AddTail(_T("Solid"));
//	lstIconNames_style.AddTail(_T("Dot"));
//	lstIconNames_style.AddTail(_T("Dash"));
//	lstIconNames_style.AddTail(_T("Dash dot"));
//	lstIconNames_style.AddTail(_T("Dash dot dot"));
//	
//	CString ntempSel2 = tempproperty->m_LineStyle;
//	int nSel2=0;
//
//	if(ntempSel2=="Invisible")
//	{
//			 nSel2=0;
//	}
//	if(ntempSel2=="Solid")
//	{
//			 nSel2=1;
//	}
//	if(ntempSel2=="Dot")
//	{
//			 nSel2=2;
//	}
//	if(ntempSel2=="Dash")
//	{
//			 nSel2=3;
//	}
//	if(ntempSel2=="Dash dot")
//	{
//			 nSel2=4;
//	}
//	if(ntempSel2=="Dash dot dot")
//	{
//			 nSel2=5;
//	}
//
//	//CMFCPropertyGridProperty* pProp2 = new CIconListProp(_T("Line weight"), m_imageList_weight, 0, &lstIconNames_weight, _T("Lineweight List Property")); 
//	pProp = new CIconListProp(_T("Line style"), m_imageList_style[index], nSel2, &lstIconNames_style, _T("Linewstyle List Property")); 
//	pGroup22->AddSubItem(pProp);
//
//	COLORREF tempcolor2;
//	tempcolor2 = m_LevelFile.GetColorFromString(tempproperty->m_LineColor);
//	CMFCPropertyGridColorProperty* pColorProp2 = new CMFCPropertyGridColorProperty(_T("Line color"), tempcolor2, NULL, _T("Line color"));
//	pColorProp2->EnableOtherButton(_T("Other..."));
//	pColorProp2->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
//	pGroup22->AddSubItem(pColorProp2);	
//#endif
//
//	COLORREF tempcolor;
//	tempcolor = m_LevelFile.GetColorFromString(tempproperty->m_FFGColor);
//	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("FG"), tempcolor, NULL, _T("Foreground color"));
//	pColorProp->EnableOtherButton(_T("Other..."));
//	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
//	pGroup22->AddSubItem(pColorProp);
//
//	tempcolor = m_LevelFile.GetColorFromString(tempproperty->m_FBGColor);
//	pColorProp = new CMFCPropertyGridColorProperty(_T("BG"), tempcolor, NULL, _T("Background color"));
//	pColorProp->EnableOtherButton(_T("Other..."));
//	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
//	pGroup22->AddSubItem(pColorProp);
//	
//	
//	tempString.Format("%.2f", tempproperty->m_Area);
//	pProp =  new CMFCPropertyGridProperty(_T("면적(m²)"),tempString, _T("각 레벨 면적 정보 입니다"));
//	pGroup22->AddSubItem(pProp);
//
//	tempString.Format("%.2f", ((tempproperty->m_Area/m_TotalArea)*100));
//	pProp =  new CMFCPropertyGridProperty(_T("면적율(%)"),tempString, _T("각 레벨 면적 정보 입니다"));
//	pGroup22->AddSubItem(pProp);
//
//	pGroup22->Expand(FALSE);
//	}
//	
//	pGroup2->Expand(TRUE);
//	pPropList->AddProperty(pGroup2.release());
//	
//} // sanghyun 102314 for PropertyDispWnd
//
//void CContourDoc::OnComboBoxSelChange()
//{
//		// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
//	CPropertiesWnd* pPropWnd =& (pMainWnd->m_wndProperties);
//	CMFCPropertyGridCtrl* pPropList =(CMFCPropertyGridCtrl*) &( pPropWnd->m_wndPropList );
//	 m_nPropId= pPropWnd->m_wndObjectCombo.GetCurSel();
//    switch (m_nPropId) {
//		case 0:
//            FillUpGeneralProperty();
//            break;
//		case 1:
//            FillUpProperty();
//            break;
//        case 2:
//            FillUpDispProperty();
//            break;
//    };
//} //Sang for PropertyWnd
//
//#include "..\DamageAnalysisDlg.h"
//
//void CContourDoc::OnDamageAnalysis()
//{
//	CDamageAnalysisDlg dlg;
//
//	dlg.m_pLevelFile = &m_LevelFile;
//	dlg.m_SurveyArea = m_TotalArea;
//	dlg.m_DataID = m_DataID;
//
//	
//	if(dlg.DoModal() == IDOK)
//	{
//#if 1
//		m_DataID = dlg.m_DataID;
//		if(m_DataID<0)
//		{
//			AfxMessageBox("Data ID is not proper value!");
//		}
//		else
//		{
////			CBridgeSurveyApp* pApp = (CBridgeSurveyApp *)(AfxGetApp());
//			CBridgeSurveyApp* pApp = static_cast<CBridgeSurveyApp*>(::AfxGetApp());
//			pApp->m_ProjectInfo.m_LowerBoundary[m_DataID] = dlg.m_LowerBoundary;
//			pApp->m_ProjectInfo.m_UpperBoundary[m_DataID] = dlg.m_UpperBoundary;
//
//			pApp->m_ProjectInfo.m_DamageArea[m_DataID] = dlg.m_DamageArea;
//			pApp->m_ProjectInfo.m_DamageAreaRate[m_DataID] = dlg.m_DamageRate;
//			pApp->m_ProjectInfo.m_TotalArea[m_DataID] = dlg.m_SurveyArea;
//		}
//#else
//		CBridgeSurveyApp* pApp = static_cast<CBridgeSurveyApp*>(::AfxGetApp());
//		for(int ii=0;ii<6;ii++){
//			pApp->m_ProjectInfo.m_LowerBoundary[ii] = dlg.m_LowerBoundary;
//			pApp->m_ProjectInfo.m_UpperBoundary[ii] = dlg.m_UpperBoundary;
//
//			pApp->m_ProjectInfo.m_DamageArea[ii] = dlg.m_DamageArea;
//			pApp->m_ProjectInfo.m_DamageAreaRate[ii] = dlg.m_DamageRate;
//			pApp->m_ProjectInfo.m_TotalArea[ii] = dlg.m_SurveyArea;
//		}
//
//#endif
//	}
//}


void CContourDoc::SaveImageAuto(CString titlestring)
{
	//CSeismicSurveyApp * pApp = (CSeismicSurveyApp *)AfxGetApp();
	//POSITION pos =GetFirstViewPosition();

 //   while (pos != NULL)
 //   {
	//CContourView* pView= (CContourView*) GetNextView(pos);
	////CContourView* pView= (CContourView*) AfxGetMainWnd()->MDIGetActive()->GetActiveView();
	//ASSERT_VALID(pView);
	//	SetCurrentDirectory(pApp->m_ProjectInfo.m_strReportFolder);
	//	CString pngPathName = pApp->m_ProjectInfo.m_strReportFolder+ _T("\\")+ titlestring +_T(".png");
	//	pView->SaveImageFile(pngPathName);
	//}
}
