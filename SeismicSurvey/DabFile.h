#pragma once

#include "DabPacket.h"

#include "SignalProcessingParameter.h"

class CSeismicSonde;

class CDabFile
{
public:
	CDabFile(void);
	~CDabFile(void);
	CStringList m_DabFileList;
	CArray <CDabPacket *, CDabPacket *> m_DabPacketList;
	void LoadFiles();
	void DeleteData();
	int GetIndexFromFilename(CString filename);
	void SaveFile(CString filename, CSeismicSonde * pSonde, int direction);

	CSignalProcessingParameter m_SigParameter;
};

