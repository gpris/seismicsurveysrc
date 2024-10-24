#pragma once

#include "SeismicData.h"

class CSeismicDataDraw
{
public:
	CSeismicDataDraw(void);
	~CSeismicDataDraw(void);

	CArray <CSeismicData *, CSeismicData *> * m_pScanData;
	int m_ScanNo;
	int m_DataNo;

	CRect m_DrawRect;
	CRect m_DepthAxisRect;
	int m_DrawDataStart;
	int m_DrawDataEnd;
	int m_DrawScanNo;

	float m_XStep;
	float m_YStep;

	int m_InfoWidth;

	int m_GraphMode;

	void DrawOneSeismicAxis(CDC * pDC, CRect drawRect, CSeismicData * pData, int mode, float maxvalue, bool boundary, bool xgrid, bool ygrid);
	void DrawOneSeismicDetailAxis(CDC * pDC, CRect drawRect, CSeismicData * pData, int mode, float maxvalue);
	void DrawOneSeismicDabInfo(CDC * pDC, CRect drawRect, CSeismicData * pData, int mode, float maxvalue, int chid);
	void DrawOneSeismicDatInfo(CDC * pDC, CRect drawRect, CSeismicData * pData, int mode, float maxvalue);
	void DrawOneSeismicGraph(CDC * pDC, CRect drawRect, int index, int mode, float maxvalue);
	void DrawOneSeismicGraph(CDC * pDC, CRect drawRect, CSeismicData * pData, int mode, float maxvalue);
	void DrawSeismicGraph(CDC * pDC, CRect drawRect, int mode, float maxvalue);
	void DrawSeismicFilledGraph(CDC * pDC, CRect drawRect, int mode);
	void DeterminePlotRegion();
	void DrawOneFilledSeismicGraph(CDC * pDC, CRect drawRect, CSeismicData * pData, int mode, float maxvalue);

//Dat draw
	void DrawDatSeismicGraph(CDC * pDC, CRect drawRect, int mode, float maxvalue);
	void DrawDabSeismicGraph(CDC * pDC, CRect drawRect, int mode, float maxvalue);
	void DrawDepthAxis(CDC * pDC, CRect drawRect, CSeismicData * pStartData, CSeismicData * pEndData, float XMajorTick, float XMinorTick);

};

