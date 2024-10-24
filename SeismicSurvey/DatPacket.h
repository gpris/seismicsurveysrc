#pragma once

#include "SeismicData.h"

class CDatPacket
{
public:
	CDatPacket(void);
	~CDatPacket(void);

	float m_dt;
	int m_datano;
	int m_scanno;
	int m_SelectedIndex;
	float startdepth;
	float enddepth;

	CArray <CSeismicData *, CSeismicData *> m_ScanData;
	bool Load(CString filename);
	bool Save(CString filename);
	bool CsvSave(CString filename, int startd, int stopd, int chindx, float sampling);

	void DeleteData();
};

