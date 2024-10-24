// Kriging.h: interface for the Kriging class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KRIGING_H__2D4FB688_334E_464E_9E9F_55D489A8E5FC__INCLUDED_)
#define AFX_KRIGING_H__2D4FB688_334E_464E_9E9F_55D489A8E5FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Interpolater.h"
#include "Matrix.h"
#include "Numeric.h"
#include <string>
#include <vector>
#include <tchar.h>
#include <iostream>
using namespace std;

template<class ForwardIterator>
double GetDistance(const ForwardIterator start, int i, int j)
{
	return ::sqrt(::pow(((*(start+i)).x - (*(start+j)).x), 2) + ::pow(((*(start+i)).y - (*(start+j)).y), 2));
}

template<class ForwardIterator>
double GetDistance(double xpos, double ypos, const ForwardIterator start, int i)
{
	return ::sqrt(::pow(((*(start+i)).x - xpos), 2) + ::pow(((*(start+i)).y - ypos), 2));
}

template<class T, class ForwardIterator>
class TKriging : public TInterpolater<ForwardIterator>
{
public:
	TKriging(const ForwardIterator first, const ForwardIterator last, double dSemivariance, int Option) : m_dSemivariance(dSemivariance)
	{
		m_Option = Option;
		m_nSize = 0;
		ForwardIterator start = first;
		while(start != last) {
			++m_nSize;
			++start;
		}
		m_nSize++;
		TRACE("Kriging size = %d\n", m_nSize);
		m_matA.SetDimension(m_nSize, m_nSize);

		for(int j=0; j<m_nSize; j++) {
			for(int i=0; i<m_nSize; i++) 
			{
				if(i == m_nSize-1 || j == m_nSize-1) {
					m_matA(i, j) = 1;
					if(i == m_nSize-1 && j == m_nSize-1)
						m_matA(i, j) = 0;
					continue;
				}
				double dist = ::GetDistance(first, i, j);
				switch(m_Option)
				{
				case 0:
					m_matA(i, j) =0.78*(1- dist / m_dSemivariance); //linear
					break;
				case 1:
					m_matA(i, j) = 0.78*(exp(-dist/m_dSemivariance));//exponential
					break;
				case 2:
					if(dist<m_dSemivariance)
						m_matA(i, j) = 0.78*(1-((3*dist)/(2*m_dSemivariance) - 0.5* ::pow(dist/m_dSemivariance, 3.0))) ;//spherical
					else
						m_matA(i, j) = 0.0;
					break;
				}
//				TRACE("%.4f ", m_matA(i,j));
			}
//			TRACE("\n");
		}
		int nD;
		LUDecompose(m_matA, m_Permutation, nD);
	}
	double GetInterpolatedZ(double xpos, double ypos, ForwardIterator first, ForwardIterator last) throw(InterpolaterException)
	{
		//FILE *fp;
		//CString filename;
		//filename.Format("TRACE_%d_%d.csv", (int)(xpos*10), (int)(ypos*10));
		//fp = fopen(filename, "w");
		std::vector<double> vecB(m_nSize);
		for(int i=0; i<m_nSize-1; i++) {
			double dist = ::GetDistance(xpos, ypos, first, i);
			switch(m_Option)
			{
			case 0:
				vecB[i] = 0.78*(1- dist / m_dSemivariance);
				break;
			case 1:
				vecB[i] = 0.78*(exp(-dist/m_dSemivariance));//exponential
				break;
			case 2:
				if(dist<m_dSemivariance)
					vecB[i] = 0.78*(1-((3*dist)/(2*m_dSemivariance) - 0.5* ::pow(dist/m_dSemivariance, 3.0))) ;//spherical
				else
					vecB[i] = 0.0;
				break;
			}
		}
		vecB[m_nSize-1] = 1;
		//TRACE("k_input :\n");
		//for(int i=0; i<=m_nSize-1; i++) 
		//{
		//	TRACE("%.4f ", vecB[i]);
		//}
		//TRACE("\n");
		LUBackSub(m_matA, m_Permutation, vecB);
		//TRACE("lambda :\n");
		//for(int i=0; i<=m_nSize-1; i++) 
		//{
		//	TRACE("%.4f ", vecB[i]);
		//}
		//TRACE("\n");

//Normalization of weighting
		double acc = 0;
		for(int i=0; i<m_nSize-1; i++) {
			double dist = ::GetDistance(xpos, ypos, first, i);
			acc += vecB[i];
		}
		for(int i=0; i<m_nSize-1; i++) {
			double dist = ::GetDistance(xpos, ypos, first, i);
			{
			vecB[i] = vecB[i]/acc;
			//fprintf(fp, "%f, %f, %f, %f, %f\n", (*(first+i)).x, (*(first+i)).y, (*(first+i)).z, dist, vecB[i]);
			}
		}
		double z = 0;
		for(int i=0; i<m_nSize-1; i++) {
			double inputz = (*(first+i)).z;
			double dist = ::GetDistance(xpos, ypos, first, i);
			z += vecB[i] * inputz;
		}
		//fprintf(fp, "%f, %f, %f, %f, %f\n", xpos, ypos, z, 0, 0);
		//fclose(fp);
		return z;
	}
private:
	TMatrix<T> m_matA;
	vector<int> m_Permutation;
	int m_nSize;
	double m_dSemivariance;
	int m_Option;
};

typedef TKriging<double, Point3D*> Kriging;

#endif // !defined(AFX_KRIGING_H__2D4FB688_334E_464E_9E9F_55D489A8E5FC__INCLUDED_)
