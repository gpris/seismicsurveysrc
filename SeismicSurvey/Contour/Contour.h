#pragma once

#include "LevelFile.h"
#include "BaseData.h"

class CContourLine
{
public:
	Point3D m_Lines[2];
};

class CContourPolygon
{
public:
	Point3D m_Points[4];
	int m_nPolygon;
};

class CContour
{
public:
	CContour(void);
	~CContour(void);
	void Contour(double **d,int ilb,int iub,int jlb,int jub, double *x,double *y,CLevelFile * pLevelFile);
	float PolyArea(Point3D * Points, int nPolygon);
	double **m_data2d;
	double *m_xaxis;
	double *m_yaxis;
	double m_xmax;
	double m_ymax;
	double m_xmin;
	double m_ymin;
	int m_LevelNo;

	CLevelFile *m_LevelFile;
	CArray<CContourLine *, CContourLine *> * m_LineList;
	CArray<CContourPolygon *, CContourPolygon *> * m_PolygonList;
//	void DrawContour(CDC *pDC);
};

