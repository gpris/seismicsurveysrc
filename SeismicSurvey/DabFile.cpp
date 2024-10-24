#include "StdAfx.h"
#include "DabFile.h"
#include "SeismicSonde.h"

CDabFile::CDabFile(void)
{
}


CDabFile::~CDabFile(void)
{
	DeleteData();
}

void CDabFile::DeleteData()
{
	for(int indx = 0; indx<m_DabPacketList.GetSize(); indx++)
	{
		CDabPacket * tempPacket = m_DabPacketList.GetAt(indx);
		delete tempPacket;
	}
	m_DabFileList.RemoveAll();
	m_DabPacketList.RemoveAll();
}

void CDabFile::LoadFiles(void)
{
	for(POSITION pos = m_DabFileList.GetHeadPosition(); pos != NULL; )
	{
		CString tempString = m_DabFileList.GetNext(pos);
		CDabPacket * pDabPacket = new CDabPacket;
		pDabPacket->Load(tempString, &m_SigParameter);
		m_DabPacketList.Add(pDabPacket);
	}
}

void CDabFile::SaveFile(CString filename, CSeismicSonde * pSonde, int direction)
{
	CDabPacket * pDabPacket = new CDabPacket;
	pDabPacket->m_channo = pSonde->m_ChanNo;
	pDabPacket->m_datano = pSonde->m_DataNo;
	pDabPacket->m_dt = pSonde->m_dt;

//	pDabPacket->m_ScanData.Copy((pSonde->m_ChData));
	for(int chindx = 0; chindx<pDabPacket->m_channo; chindx++)
	{
		CSeismicData * tempData = new CSeismicData(pDabPacket->m_datano);
		CSeismicData * srcData = pSonde->m_ChData.GetAt(chindx);
		tempData->m_DataNo = srcData->m_DataNo;
		tempData->m_SamplingTime = srcData->m_SamplingTime;
		tempData->m_ChID = srcData->m_ChID;
		tempData->m_TxDepth = srcData->m_TxDepth;
		tempData->m_RxDepth = srcData->m_RxDepth;

		for(int dindx = 0; dindx<pDabPacket->m_datano; dindx++)
		{
			tempData->m_Data[dindx] = srcData->m_Data[dindx];
			tempData->m_ProcessedData[dindx] = srcData->m_ProcessedData[dindx];
		}
		tempData->Analyze();
		pDabPacket->m_ScanData.Add(tempData);
	}

	pDabPacket->Save(filename);
	if(direction)  //Down direction
	{
		m_DabPacketList.Add(pDabPacket);
		m_DabFileList.AddTail(filename);
	} 
	else           //Up direction
	{
		if(m_DabPacketList.IsEmpty())
		{
			m_DabPacketList.Add(pDabPacket);
		}
		else
		{
			m_DabPacketList.InsertAt(0, pDabPacket);
		}

		if(m_DabFileList.IsEmpty())
		{
			m_DabFileList.AddTail(filename);
		}
		else
		{
			m_DabFileList.InsertBefore(0, filename);
		}
	}
}

#include "PathNameInfo\PathNameInfo.h"

int CDabFile::GetIndexFromFilename(CString filename)
{
	int indx = 0;
	int result = -1;

	for(POSITION pos = m_DabFileList.GetHeadPosition(); pos != NULL; )
	{
		CString tempString;

		tempString =m_DabFileList.GetNext(pos);
		CPathNameInfo tempPathInfo = CPathNameInfo(tempString);
		if(filename == tempPathInfo.m_FileNameOnly)
		{
			break;
		}
		indx++;
	}
	if(indx < m_DabFileList.GetSize())
		result = indx;

	return result;
}
