#include "StdAfx.h"
#include "DatFile.h"

CDatFile::CDatFile(void)
{

}


CDatFile::~CDatFile(void)
{
}

void CDatFile::DeleteData()
{
	for(int indx = 0; indx<m_DatPacketList.GetSize(); indx++)
	{
		CDatPacket * tempPacket = m_DatPacketList.GetAt(indx);
		delete tempPacket;
	}
	m_DatFileList.RemoveAll();
	m_DatPacketList.RemoveAll();
}

void CDatFile::LoadFiles(void)
{
	for(POSITION pos = m_DatFileList.GetHeadPosition(); pos != NULL; )
	{
		CString tempString = m_DatFileList.GetNext(pos);
		CDatPacket * pDatPacket = new CDatPacket;
		pDatPacket->Load(tempString);
		m_DatPacketList.Add(pDatPacket);
	}
}

#include "PathNameInfo\PathNameInfo.h"

int CDatFile::GetIndexFromFilename(CString filename)
{
	int indx = 0;
	int result = -1;

	for(POSITION pos = m_DatFileList.GetHeadPosition(); pos != NULL; )
	{
		CString tempString;

		tempString =m_DatFileList.GetNext(pos);
		CPathNameInfo tempPathInfo = CPathNameInfo(tempString);
		if(filename == tempPathInfo.m_FileNameOnly)
		{
			break;
		}
		indx++;
	}
	if(indx < m_DatFileList.GetSize())
		result = indx;

	return result;
}

void CDatFile::SaveFile(CString filename, CDatPacket * pDatPacket)
{
	m_DatFileList.AddTail(filename);
	pDatPacket->Save(filename);
	m_DatPacketList.Add(pDatPacket);

	int totalno = m_DatPacketList.GetSize();
}

void CDatFile::SaveCsvFile(CString filename, CDatPacket* pDatPacket, int startd, int stopd, int chindx, float sampling)
{
	//m_DatFileList.AddTail(filename);
	pDatPacket->CsvSave(filename, startd, stopd, chindx, sampling);
	//m_DatPacketList.Add(pDatPacket);

	int totalno = m_DatPacketList.GetSize();
}