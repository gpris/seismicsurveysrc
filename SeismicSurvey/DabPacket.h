#pragma once

#include "SeismicData.h"

class CSignalProcessingParameter;

class CDabPacket
{
public:
	CDabPacket(void);
	~CDabPacket(void);
	float m_dt;
	short m_datano;
	short m_channo;
	short m_SelectedChannel;
//	CArray <float *, float *> m_ScanData;
	CArray <CSeismicData *, CSeismicData *> m_ScanData;
	bool Load(CString filename);
	bool Load(CString filename, CSignalProcessingParameter * pSigParameter);
	bool Save(CString filename);
	void DeleteData();

	CSignalProcessingParameter * m_pSigParameter;
};

