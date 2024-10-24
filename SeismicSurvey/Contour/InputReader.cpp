// InputReader.cpp: implementation of the InputReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InputReader.h"
#include <fstream>
#include <sstream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void InputReader::Read(string filename) throw()
{
	ifstream in(filename.c_str());
	if(!in) {
		::AfxMessageBox(_T("Failed to open..."));
		return;
	}

	double x, y, z;
	m_vecPoints.clear();
	while(!in.eof()) {
		string line;
		getline(in, line);
		istringstream iss(line);
		iss >> x >> y >> z;
		m_vecPoints.push_back(Point3D(x, y, z));
	}
}

void InputReader::Dump(ostream& os) const throw()
{
	for(int i=0; i<m_vecPoints.size(); i++)
		os << m_vecPoints[i].x << "\t" << m_vecPoints[i].y << "\t" << m_vecPoints[i].z << endl;
}

void InputReader::LoadFromGridPacket(CGridPacket * pPacket)
{
	double x, y, z;
	for (int col = 0; col < pPacket->m_nCol; col++)
	{
		x = pPacket->m_xLL + col * pPacket->m_xSize;
		for(int row = 0; row < pPacket->m_nRow; row++)
		{
			y = pPacket->m_yLL + row * pPacket->m_ySize;
			z = pPacket->m_Data[col][row];
			m_vecPoints.push_back(Point3D(x, y, z));
		}
	}
}

void InputReader::LoadFromLayerData(CLayerData * pLayerData, int xindx, int yindx, int zindx, float xstep)
{
	double x, y, z;
	double lastx, lasty, lastz;
	float xstart, xnext, lastxnext;
	if(xindx != 0)
	{
		xstep = -xstep;
	}

	for (int col = 0; col < pLayerData->m_TotalTraceNumber; col++)
	{
		CLayerCellOfTwoLayers * tempcell;
		tempcell = pLayerData->m_LayerCells.GetAt(col);
		x = tempcell->m_Data[xindx];
		y = tempcell->m_Data[yindx];
		z = tempcell->m_Data[zindx];


		//if(xstep > 0)
		//{
			if(col == 0 || lasty != y)
			{
				xstart = x;
				xnext = xstart;
				lastx = x;
				lasty = y;
				lastz = z;
			}

			if((lastx - xnext)*(x - xnext) <= 0)
			{
				if(fabs(lastx - xnext) < fabs(x-xnext))
				{
					m_vecPoints.push_back(Point3D(lastx, lasty, lastz));
				}
				else
				{
					m_vecPoints.push_back(Point3D(x, y, z));
				}

				if(xstep>0)
				{
					do{
						lastxnext = xnext;
						xnext += xstep;
					}while(xnext < x);
				}
				else
				{
					do{
						lastxnext = xnext;
						xnext += xstep;
					}while(xnext > x);

				}
			}
			lastx = x;
			lasty = y;
			lastz = z;
		//}
		//else
		//{
		//	m_vecPoints.push_back(Point3D(x, y, z));
		//}
	}
}

void InputReader::LoadFromLayerData(CLayerData * pLayerData, int xindx, int yindx, int zindx, float xstep, float xbegin, float xend)
{
	double x, y, z;
	double lastx, lasty, lastz;
	float xstart, xnext, lastxnext;

	for (int col = 0; col < pLayerData->m_TotalTraceNumber; col++)
	{
		CLayerCellOfTwoLayers * tempcell;
		tempcell = pLayerData->m_LayerCells.GetAt(col);
		x = tempcell->m_Data[xindx];
		y = tempcell->m_Data[yindx];
		z = tempcell->m_Data[zindx];

		if(xstep > 0)
		{
			if(col == 0 || lasty != y)
			{
				xstart = x;
				xnext = xstart;
				lastx = x;
				lasty = y;
				lastz = z;
			}

			if((lastx - xnext)*(x - xnext) <= 0)
			{
				if(fabs(lastx - xnext) < fabs(x-xnext))
				{
					if(lastx>=xbegin && lastx<=xend)
						m_vecPoints.push_back(Point3D(lastx, lasty, lastz));
				}
				else
				{
					if(x>=xbegin && x<=xend)
						m_vecPoints.push_back(Point3D(x, y, z));
				}

				if(xstep>0)
				{
					do{
						lastxnext = xnext;
						xnext += xstep;
					}while(xnext < x);
				}
				else
				{
					do{
						lastxnext = xnext;
						xnext += xstep;
					}while(xnext > x);

				}
			}
			lastx = x;
			lasty = y;
			lastz = z;
		}
		else
		{
			if(x>=xbegin && x<=xend)
				m_vecPoints.push_back(Point3D(x, y, z));
		}
	}
}

void InputReader::Add3DPoint(float x, float y, float z)
{
	m_vecPoints.push_back(Point3D(x, y, z));
}

void InputReader::ClearAll()
{
	m_vecPoints.clear();
}
