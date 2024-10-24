// BaseData.h: interface for the BaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEDATA_H__671236C3_A7E7_4782_AE8A_B81C4676A97C__INCLUDED_)
#define AFX_BASEDATA_H__671236C3_A7E7_4782_AE8A_B81C4676A97C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<typename T>
struct TPoint2D
{
	TPoint2D() : x(0), y(0) {}
	TPoint2D(T xx, T yy) : x(xx), y(yy) {}
	T x;
	T y;
};

typedef TPoint2D<double> Point2D;

template<typename T>
struct TPoint3D
{
	TPoint3D() : x(0), y(0), z(0) {}
	TPoint3D(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	T x;
	T y;
	T z;
};

typedef TPoint3D<double> Point3D;

inline bool Point3DCompare(const Point3D& pt1, const Point3D& pt2)
{
	return pt1.z < pt2.z;
}

template<typename T>
struct TContourLine
{
	TContourLine() : x1(0), y1(0), x2(0),y2(0) {}
	TContourLine(T xx1, T yy1, T xx2, T yy2) : x1(xx1), y1(yy1), x2(xx2), y2(yy2) {}
	T x1;
	T y1;
	T x2;
	T y2;
};

typedef TContourLine<double> ContourLine;

template<typename T>
struct TPolygon
{
	TPolygon() : nPolygon(0) {}
	T x[4];
	T y[4];
	int nPolygon;
};

typedef TContourLine<double> ContourLine;
#endif // !defined(AFX_BASEDATA_H__671236C3_A7E7_4782_AE8A_B81C4676A97C__INCLUDED_)
