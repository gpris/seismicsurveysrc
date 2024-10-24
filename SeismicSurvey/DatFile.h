#pragma once

#include "DatPacket.h"

class CDatFile
{
public:
	CDatFile(void);
	~CDatFile(void);
	CStringList m_DatFileList;
	CArray <CDatPacket *, CDatPacket *> m_DatPacketList;
	void LoadFiles();
	void DeleteData();
	int GetIndexFromFilename(CString filename);
	void SaveFile(CString filename, CDatPacket * pDatPacket);
	void SaveCsvFile(CString filename, CDatPacket* pDatPacket, int startd, int stopd, int chindx, float sampling);
};

