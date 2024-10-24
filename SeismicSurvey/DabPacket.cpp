#include "StdAfx.h"
#include "DabPacket.h"

#include "SignalProcessingParameter.h"

CDabPacket::CDabPacket(void)
{
	m_SelectedChannel = 0;

	m_pSigParameter = NULL;
}


CDabPacket::~CDabPacket(void)
{
	DeleteData();
}

void CDabPacket::DeleteData()
{
	for(int indx = 0; indx<m_ScanData.GetSize(); indx++)
	{
		CSeismicData * tempData = m_ScanData.GetAt(indx);
		delete tempData;
	}
	m_ScanData.RemoveAll();
}

bool CDabPacket::Load(CString filename)
{
	CFile fin;
	CFileException ex;

	CString fileheader;
	char tempfileheader[20];

	if(!fin.Open(filename, CFile::modeRead|CFile::shareDenyWrite, &ex))
	{
		return false;
	}

	float txdepth = 0.0;
	float rxdepth = 0.0;

	CString TxDepthString;
	CString RxDepthString;

	int tindx = filename.Find(_T("tx"));
	int rindx = filename.Find(_T("rx"));
	int extindx = filename.Find(_T(".dab"));
	if(tindx>0 && rindx>0)
	{
		TxDepthString = filename.Mid(tindx+2, 3);
		txdepth = atof(TxDepthString)/10.0;

		RxDepthString = filename.Mid(rindx+2, 3);
		rxdepth = atof(RxDepthString)/10.0;
	}
	else
	{
		TxDepthString = filename.Mid(extindx-8, 4);
		txdepth = atof(TxDepthString)/10.0;
		RxDepthString = filename.Mid(extindx-4,4);
		rxdepth = atof(RxDepthString)/10.0 - 4.0;
	}

	fin.Read(&m_dt, sizeof(float));
	fin.Read(&m_datano, sizeof(short));
	fin.Read(&m_channo, sizeof(short));

	for(int chindx = 0; chindx<m_channo; chindx++)
	{
		CSeismicData * data = new CSeismicData(m_datano);
		fin.Read(data->m_Data, m_datano*sizeof(float));
		data->m_ChID = chindx;
		data->m_TxDepth = txdepth;
		data->m_RxDepth = rxdepth+(chindx - 7)*0.5;
		data->Analyze();
		data->ApplySignalProcessing(m_pSigParameter);
		m_ScanData.Add(data);
	}
	fin.Close();

	return true;
}

bool CDabPacket::Load(CString filename, CSignalProcessingParameter * pSigParameter)
{
	m_pSigParameter = pSigParameter;

	return(Load(filename));
}

bool CDabPacket::Save(CString filename)
{
	CFile fout;
	CFileException ex;

	CString fileheader;
	char tempfileheader[20];

	if(!fout.Open(filename, CFile::modeCreate|CFile:: modeReadWrite, &ex))
	{
		return false;
	}

	fout.Write(&m_dt, sizeof(float));
	fout.Write(&m_datano, sizeof(short));
	fout.Write(&m_channo, sizeof(short));

	for(int chindx = 0; chindx<m_channo; chindx++)
	{
		CSeismicData * data = m_ScanData.GetAt(chindx);
		fout.Write(data->m_Data, m_datano*sizeof(float));
	}
	fout.Close();
}