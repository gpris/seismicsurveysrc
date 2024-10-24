#include "StdAfx.h"
#include "Contour.h"
#include <cmath>

#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])
#define MIN(X,Y) ((X) < (Y) ?  (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ?  (X) : (Y))

CContour::CContour(void)
{
	m_xmax = 0;
	m_xmin = 0;
	m_ymin = 0;
	m_ymax = 0;
}


CContour::~CContour(void)
{
	for(int indx = 0;indx<m_LevelNo; indx++)
	{
		m_PolygonList[indx].RemoveAll();
		m_LineList[indx].RemoveAll();
	}
}

void CContour::Contour(double **d,int ilb,int iub,int jlb,int jub,
   double *x,double *y,CLevelFile * pLevelFile)
{
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

   	m_data2d = d;
	m_xaxis = x;
	m_yaxis = y;
	m_LevelFile = pLevelFile;
	m_xmin = x[ilb];
	m_xmax = x[iub];
	m_ymin = y[jlb];
	m_ymax = y[jub];
	m_LevelNo = pLevelFile->m_LevelNum;

   int nPolygon = 0;
//   Point2D * tempPoint = new Point2D[4];
   Point3D * tempPoint = new Point3D[4];

   int nc = pLevelFile->m_LevelNum;

   double * z = new double[nc];
   double * area = new double[nc];
   CPen * contourpen = new CPen[nc];
   CBrush * fillbrush = new CBrush[nc];

   m_LineList = new CArray<CContourLine *, CContourLine *> [nc];
   m_PolygonList = new CArray<CContourPolygon *, CContourPolygon *> [nc];
   for (k=0;k<nc;k++) 
   {
	   CLevelProperty * tempproperty = pLevelFile->m_LevelList.GetAt(k); 
	   z[k] = tempproperty->m_Level;

	   m_LineList[k].RemoveAll();
	   m_PolygonList[k].RemoveAll();
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
                           |  m=4    X   m=2   |       the centre is vertex 0
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

				   CContourLine * tempLine = new CContourLine;
					tempLine->m_Lines[0].x = x1;
					tempLine->m_Lines[0].y = y1;
					tempLine->m_Lines[0].z = z[k];
					tempLine->m_Lines[1].x = x2;
					tempLine->m_Lines[1].y = y2;
					tempLine->m_Lines[1].z = z[k];

					m_LineList[k].Add(tempLine);
				} /* m */

				if(nPolygon > 0)
				{
					CContourPolygon * tempPolygon = new CContourPolygon;
					tempPolygon->m_nPolygon = nPolygon;

					for(int indx = 0;indx<nPolygon; indx++)
					{
						tempPolygon->m_Points[indx].x = tempPoint[indx].x;
						tempPolygon->m_Points[indx].y = tempPoint[indx].y;
						tempPolygon->m_Points[indx].z = tempPoint[indx].z;
					}

					m_PolygonList[k].Add(tempPolygon);

					area[k] += PolyArea(tempPoint, nPolygon);
				}
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

float CContour::PolyArea(Point3D * Points, int nPolygon)
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
