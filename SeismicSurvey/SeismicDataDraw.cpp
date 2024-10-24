#include "StdAfx.h"
#include "SeismicDataDraw.h"

#include <cmath>

CSeismicDataDraw::CSeismicDataDraw(void)
{
	m_DrawRect = CRect(0,0,0,0);
	m_DrawDataStart = 0;
	m_DrawDataEnd = 0;

	m_XStep = 0.0;
	m_YStep = 0.0;

	m_InfoWidth = 150;
	m_GraphMode = 0;

}


CSeismicDataDraw::~CSeismicDataDraw(void)
{
}

void CSeismicDataDraw::DrawSeismicGraph(CDC * pDC, CRect drawRect, int mode, float maxvalue)
{
	switch(m_GraphMode)
	{
	case 0: //DAB file
		DrawDabSeismicGraph(pDC, drawRect, mode, maxvalue);
		break;
	case 1:
		DrawDatSeismicGraph(pDC, drawRect, mode, maxvalue);
		break;
	}
}


void CSeismicDataDraw::DrawOneSeismicGraph(CDC * pDC, CRect OnePlotRect, CSeismicData * pData, int mode, float maxvalue)
{
	int center_y = OnePlotRect.CenterPoint().y;
	float XStep = (float)OnePlotRect.Width()/((float)(m_DrawDataEnd - m_DrawDataStart));
	float valuerange;
	
	if(maxvalue == 0)
	{
		valuerange = pData->m_Max;
	}
	else
	{
		valuerange = maxvalue;
	}
	float weighting = 0.5*OnePlotRect.Height()/valuerange;

	int dindx = m_DrawDataStart;
	int xpos = OnePlotRect.left + dindx * XStep;
	int ypos;

	switch(mode)
	{
	case 1:
		ypos = -weighting * pData->m_ProcessedData[dindx] + center_y;
		break;
	case 0:
		ypos = -weighting * pData->m_Data[dindx] + center_y;
		break;
	}
	pDC->MoveTo(xpos, ypos);
	for(dindx = m_DrawDataStart + 1; dindx < m_DrawDataEnd; dindx++)
	{
		xpos = OnePlotRect.left + dindx * XStep;
		switch(mode)
		{
		case 1:
			ypos = -weighting * pData->m_ProcessedData[dindx] + center_y;
			break;
		case 0:
			ypos = -weighting * pData->m_Data[dindx] + center_y;
			break;
		}
		pDC->LineTo(xpos, ypos);
	}
}

void CSeismicDataDraw::DrawOneSeismicAxis(CDC * pDC, CRect OnePlotRect, CSeismicData * pData, int mode, float maxvalue, bool boundary, bool xgrid, bool ygrid)
{

	CPen graysolidpen;
	graysolidpen.CreatePen(PS_SOLID, 1, RGB(128,128,128));
	CPen graydotpen;
	graydotpen.CreatePen(PS_DOT, 1, RGB(200,200,200));
	CPen * oldpen;
	oldpen = pDC->SelectObject(&graysolidpen);
	if(boundary)
	{
		pDC->Rectangle(OnePlotRect);//Boundary Draw
	}

	pDC->SelectObject(&graydotpen);
	float maxtimevalue = pData->m_SamplingTime * pData->m_DataNo;
	int gridstep = 200;
	float timestep = gridstep * pData->m_SamplingTime;
	float XStep = (float)OnePlotRect.Width()/((float)(m_DrawDataEnd - m_DrawDataStart));

	int dindx;
	int xpos;
	int ypos1, ypos2;

	if(xgrid)
	{
		//XAxis grid
		for(dindx = m_DrawDataStart + gridstep; dindx < m_DrawDataEnd; dindx+=gridstep)
		{
			xpos = OnePlotRect.left + dindx * XStep;
			ypos1  = OnePlotRect.top;
			ypos2 = OnePlotRect.bottom;
			pDC->MoveTo(xpos, ypos1);
			pDC->LineTo(xpos, ypos2);
		}
	}

	if(ygrid)
	{
		//YAxis grid
		int center_y = OnePlotRect.CenterPoint().y;
		pDC->MoveTo(OnePlotRect.left, center_y);
		pDC->LineTo(OnePlotRect.right, center_y);
	}
	pDC->SelectObject(oldpen);
}

void CSeismicDataDraw::DrawOneSeismicDetailAxis(CDC * pDC, CRect OnePlotRect, CSeismicData * pData, int mode, float maxvalue)
{
	CPen graysolidpen;
	graysolidpen.CreatePen(PS_SOLID, 1, RGB(128,128,128));
	CPen graydotpen;
	graydotpen.CreatePen(PS_DOT, 1, RGB(200,200,200));
	CPen * oldpen;
	oldpen = pDC->SelectObject(&graysolidpen);
	pDC->Rectangle(OnePlotRect);//Boundary Draw

	pDC->SelectObject(&graydotpen);
	float maxtimevalue = pData->m_SamplingTime * pData->m_DataNo;
	int gridstep = 200;
	float timestep = gridstep * pData->m_SamplingTime;
	float XStep = (float)OnePlotRect.Width()/((float)(m_DrawDataEnd - m_DrawDataStart));

	int dindx;
	int xpos;
	int ypos1, ypos2;

	//XAxis grid
	for(dindx = m_DrawDataStart + gridstep; dindx < m_DrawDataEnd; dindx+=gridstep)
	{
		xpos = OnePlotRect.left + dindx * XStep;
		ypos1  = OnePlotRect.top;
		ypos2 = OnePlotRect.bottom;
		pDC->MoveTo(xpos, ypos1);
		pDC->LineTo(xpos, ypos2);
	}

	float valuerange;
	if(maxvalue == 0)
	{
		valuerange = pData->m_Max;
	}
	else
	{
		valuerange = maxvalue;
	}
	float weighting = 0.5*OnePlotRect.Height()/valuerange;

	int digit = (int)(log10(valuerange));
	int axisdigit = (int)(valuerange/pow(10.0, digit));
	float axismaxvalue = axisdigit * pow(10.0, digit);
	float axisstep = axismaxvalue/5.0;
	int ypos;
	int center_y = OnePlotRect.CenterPoint().y;

	for (float axisvalue = axismaxvalue; axisvalue >= -axismaxvalue; axisvalue -= axisstep)
	{
		ypos = -weighting * axisvalue + center_y;
		pDC->MoveTo(OnePlotRect.left, ypos);
		pDC->LineTo(OnePlotRect.right, ypos);
	}

	//YAxis grid
	pDC->MoveTo(OnePlotRect.left, center_y);
	pDC->LineTo(OnePlotRect.right, center_y);

	pDC->SelectObject(oldpen);
}

void CSeismicDataDraw::DrawOneSeismicDabInfo(CDC * pDC, CRect OnePlotRect, CSeismicData * pData, int mode, float maxvalue, int chid)
{
	CString tempstring;
	tempstring.Format("CH%d", pData->m_ChID);
	CRect chrect = CRect(OnePlotRect.left, OnePlotRect.top, OnePlotRect.right, OnePlotRect.CenterPoint().y);
	pDC->DrawText(tempstring, chrect, DT_RIGHT|DT_SINGLELINE|DT_BOTTOM);
	
	tempstring.Format("T%.1fm-R%.1fm", pData->m_TxDepth, pData->m_RxDepth );
	CRect depthrect = CRect(OnePlotRect.left, OnePlotRect.CenterPoint().y, OnePlotRect.right, OnePlotRect.bottom);
	pDC->DrawText(tempstring, depthrect, DT_RIGHT|DT_SINGLELINE|DT_TOP);
}

void CSeismicDataDraw::DrawOneSeismicDatInfo(CDC * pDC, CRect OnePlotRect, CSeismicData * pData, int mode, float maxvalue)
{
	CString tempstring;
	tempstring.Format("%05.1f", pData->m_TxDepth);
	CRect chrect = CRect(OnePlotRect.left, OnePlotRect.top-10, 0.5 * OnePlotRect.right, OnePlotRect.bottom+10);
	pDC->DrawText(tempstring, chrect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	
	tempstring.Format("%05.1f", pData->m_TxDepth);
	CRect depthrect = CRect(0.5 * OnePlotRect.right, OnePlotRect.top-10, OnePlotRect.right, OnePlotRect.bottom+10);
	pDC->DrawText(tempstring, depthrect, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

	pDC->MoveTo(m_DepthAxisRect.left,OnePlotRect.CenterPoint().y);
	pDC->LineTo(m_DepthAxisRect.left + 5,OnePlotRect.CenterPoint().y);

	pDC->MoveTo(m_DepthAxisRect.right-5,OnePlotRect.CenterPoint().y);
	pDC->LineTo(m_DepthAxisRect.right,OnePlotRect.CenterPoint().y);
}

void CSeismicDataDraw::DrawOneSeismicGraph(CDC * pDC, CRect DrawRect, int sindx, int mode, float maxvalue)
{
	CSeismicData * pData = (* m_pScanData)[sindx];
	CRect OneInfoRect = CRect(DrawRect.left, DrawRect.top, 
		DrawRect.left + m_InfoWidth - 30, DrawRect.bottom);
	CRect OnePlotRect = CRect(DrawRect.left + m_InfoWidth, DrawRect.top, 
		DrawRect.right, DrawRect.bottom);
              
	DrawOneSeismicDabInfo(pDC, OneInfoRect, pData, mode, maxvalue, sindx);
	DrawOneSeismicDetailAxis(pDC, OnePlotRect, pData, mode, maxvalue);
	DrawOneSeismicGraph(pDC, OnePlotRect, pData, mode, maxvalue);
}


void CSeismicDataDraw::DrawSeismicFilledGraph(CDC * pDC, CRect drawRect, int mode)
{
	m_DrawRect = drawRect;
	DeterminePlotRegion();

	for(int sindx = 0; sindx<m_ScanNo; sindx++)
	{
		CSeismicData * pData = m_pScanData->GetAt(sindx);
		CRect OnePlotRect = CRect(m_DrawRect.left, m_DrawRect.top+sindx*m_YStep, 
			m_DrawRect.right, m_DrawRect.top + (sindx+1)*m_YStep);

		int center_y = OnePlotRect.CenterPoint().y;
		float valuerange = pData->m_Max;
		float weighting = 0.5*OnePlotRect.Height()/valuerange;

		int dindx = m_DrawDataStart;
		int xpos = OnePlotRect.left + dindx * m_XStep;

		int ypos;
		switch(mode)
		{
		case 0:
			ypos = -weighting * pData->m_ProcessedData[dindx] + center_y;
			break;
		case 1:
			ypos = -weighting * pData->m_Data[dindx] + center_y;
			break;
		}

		pDC->MoveTo(xpos, ypos);
		for(dindx = m_DrawDataStart + 1; dindx < m_DrawDataEnd; dindx++)
		{
			xpos = OnePlotRect.left + dindx * m_XStep;
			ypos = weighting * pData->m_Data[dindx] + center_y;
			pDC->LineTo(xpos, ypos);
		}
	}
}

void CSeismicDataDraw::DeterminePlotRegion()
{
	m_XStep = ((float)m_DrawRect.Width())/(m_DrawDataEnd - m_DrawDataStart);
	m_YStep = (float)(m_DrawRect.Height())/m_ScanNo;
}

void CSeismicDataDraw::DrawOneFilledSeismicGraph(CDC * pDC, CRect OnePlotRect, CSeismicData * pData, int mode, float maxvalue)
{
	int center_y = OnePlotRect.CenterPoint().y;
	float XStep = (float)OnePlotRect.Width()/((float)(m_DrawDataEnd - m_DrawDataStart));
	float valuerange;
	
	if(maxvalue == 0)
	{
		valuerange = pData->m_Max;
	}
	else
	{
		valuerange = maxvalue;
	}
	float weighting = 0.5*OnePlotRect.Height()/valuerange;

	int dindx;
	int xpos, ypos;
	float data;
	CPoint point[2048];
	CPen redpen;
	redpen.CreatePen(PS_SOLID, 1.0, RGB(0,0,0));
	int pCount = 0;
	int totalPolygonNo;
	int polygonStart[2048];
	int polygonCount[2048];
	bool isPosRegion = false;

	CPen * oldPen = pDC->SelectObject(&redpen);
	CBrush blackbrush;
	blackbrush.CreateSolidBrush(RGB(0,0,0));

	polygonCount[pCount] = 0;
	for(dindx = m_DrawDataStart; dindx < m_DrawDataEnd; dindx++)
	{
		point[dindx].x = OnePlotRect.left + dindx * XStep;
		switch(mode)
		{
		case 1:
			data = pData->m_ProcessedData[dindx];
			break;
		case 0:
			data = pData->m_Data[dindx];
			break;
		}

		if(isPosRegion)
		{
			if(data>0)
			{
				polygonCount[pCount]++;
			}
			else
			{
				isPosRegion = false;
				pCount++;
				polygonCount[pCount] = 0;
			}
		}
		else
		{	
			if(data>0)
			{
				isPosRegion = true;
				polygonStart[pCount] = dindx;
				polygonCount[pCount]++;
			}
		}
//		TRACE("data = %f, polygonStart[%d] = %d, polygonCount[%d] = %d\n", data, pCount, polygonStart[pCount], pCount, polygonCount[pCount]);

		point[dindx].y = -weighting * data + center_y;
	}

	totalPolygonNo = pCount;

	pDC->Polyline(&point[m_DrawDataStart], m_DrawDataEnd-m_DrawDataStart);

	CBrush * oldBrush = pDC->SelectObject(&blackbrush);
	for(int pindx = 0; pindx<totalPolygonNo; pindx++)
	{
		if(polygonCount[pindx]>2)
		{
			pDC->Polygon(&point[polygonStart[pindx]], polygonCount[pindx]); 
		}
	}
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

}

void CSeismicDataDraw::DrawDabSeismicGraph(CDC * pDC, CRect drawRect, int mode, float maxvalue)
{
	m_DrawRect = drawRect;
	DeterminePlotRegion();

	for(int sindx = 0; sindx<m_ScanNo; sindx++)
	{
		CSeismicData * pData = (* m_pScanData)[sindx];
		CRect OneInfoRect = CRect(m_DrawRect.left, m_DrawRect.top+sindx*m_YStep, 
			m_DrawRect.left + m_InfoWidth - 30, m_DrawRect.top + (sindx+1)*m_YStep);
		CRect OnePlotRect = CRect(m_DrawRect.left + m_InfoWidth, m_DrawRect.top+sindx*m_YStep, 
			m_DrawRect.right, m_DrawRect.top + (sindx+1)*m_YStep);

		DrawOneSeismicDabInfo(pDC, OneInfoRect, pData, mode, maxvalue, sindx);
		DrawOneSeismicAxis(pDC, OnePlotRect, pData, mode, maxvalue,1,1,1);
		DrawOneFilledSeismicGraph(pDC, OnePlotRect, pData, mode, maxvalue);
	}
}

void CSeismicDataDraw::DrawDatSeismicGraph(CDC * pDC, CRect drawRect, int mode, float maxvalue)
{
	m_DrawRect = drawRect;
	DeterminePlotRegion();

	m_DepthAxisRect.left = 0;
	m_DepthAxisRect.right = m_InfoWidth;
	m_DepthAxisRect.top = drawRect.top;
	m_DepthAxisRect.bottom = drawRect.bottom;

	CSeismicData * pStartData = (* m_pScanData)[0];
	CSeismicData * pEndData = (* m_pScanData)[m_ScanNo-1];


	DrawDepthAxis(pDC, m_DepthAxisRect, pStartData, pEndData, 5.0, 1.0);

	for(int sindx = 0; sindx<m_ScanNo; sindx++)
	{
		CSeismicData * pData = (* m_pScanData)[sindx];
		CRect OneInfoRect = CRect(m_DrawRect.left, m_DrawRect.top+sindx*m_YStep, 
			m_DrawRect.left + m_InfoWidth - 30, m_DrawRect.top + (sindx+1)*m_YStep);
		CRect OnePlotRect = CRect(m_DrawRect.left + m_InfoWidth, m_DrawRect.top+sindx*m_YStep, 
			m_DrawRect.right, m_DrawRect.top + (sindx+1)*m_YStep);

		if(fmod(pData->m_TxDepth, (float)5.0) == 0)
		{
			DrawOneSeismicDatInfo(pDC, OneInfoRect, pData, mode, maxvalue);
		}
		DrawOneSeismicAxis(pDC, OnePlotRect, pData, mode, maxvalue, 0, 0, 1);
		DrawOneFilledSeismicGraph(pDC, OnePlotRect, pData, mode, maxvalue);
	}


}

void CSeismicDataDraw::DrawDepthAxis(CDC * pDC, CRect drawRect, CSeismicData * pStartData, CSeismicData * pEndData, float XMajorTick, float XMinorTick)
{
	pDC->Rectangle(drawRect);	
}
