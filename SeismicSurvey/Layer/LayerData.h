#pragma once

#include <afxtempl.h>
#include "LayerCellOfTwoLayers.h"

//typedef CArray<CLayerCellOfTwoLayers ^, CLayerCellOfTwoLayers ^> LayerArray;

class CLayerData
{
public:
	CLayerData(void);
	CString m_ColumnHeaders[10];
	float m_min[10];
	float m_max[10];
	float m_avg[10];
	float m_std[10];

	CArray<CLayerCellOfTwoLayers *, CLayerCellOfTwoLayers *> m_LayerCells; 
	int m_TotalTraceNumber;

	void LoadfromExcelAutomation(CString filename);
	void LoadfromXlsxLibXL(CString filename);
//	void LoadfromXlsLibXL(CString filename);
	wchar_t* CharToWChar(const char* pstrSrc);
	char* WCharToChar(const wchar_t* pwstrSrc);
	void SavetoXlsxLibXL(CString filename);
	void CalculateStatistic();

};

