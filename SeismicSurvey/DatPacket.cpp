#include "StdAfx.h"
#include "DatPacket.h"


CDatPacket::CDatPacket(void)
{
	m_SelectedIndex = 0;
}


CDatPacket::~CDatPacket(void)
{
	DeleteData();
}

void CDatPacket::DeleteData()
{
	for(int indx = 0; indx<m_ScanData.GetSize(); indx++)
	{
		CSeismicData * tempData = m_ScanData.GetAt(indx);
		delete tempData;
	}
	m_ScanData.RemoveAll();
}

bool CDatPacket::Load(CString filename)
{
	FILE * fin;

	fin = fopen(filename, "r");

	fscanf(fin, "%f %d %d", &m_dt, &m_datano, &m_scanno);

	for(int sindx = 0; sindx<m_scanno; sindx++)
	{
		CSeismicData * data = new CSeismicData(m_datano);
		float tempval;
		for(int dindx = 0; dindx<m_datano; dindx++)
		{
			fscanf(fin, "%f", &tempval);
			data->m_Data[dindx] = tempval;
		}
		data->Analyze();
		m_ScanData.Add(data);
	}
	fclose(fin);
	return true;
}

bool CDatPacket::Save(CString filename)
{
	bool result = false;
	//CFile fout;
	//CFileException ex;

	//CString fileheader;
	//char tempfileheader[20];

	//if(!fout.Open(filename, CFile::modeCreate|CFile:: modeReadWrite, &ex))
	//{
	//	return false;
	//}

	//fout.Write(&m_dt, sizeof(float));
	//fout.Write(&m_datano, sizeof(short));
	//fout.Write(&m_scanno, sizeof(short));

	//for(int chindx = 0; chindx<m_scanno; chindx++)
	//{
	//	CSeismicData * data = m_ScanData.GetAt(chindx);
	//	fout.Write(data->m_Data, m_datano*sizeof(float));
	//}
	//fout.Close();

	FILE * fout;
	fout = fopen(filename, "w");
	if(fout)
	{
		fprintf(fout, "%f %d %d\n", m_dt, m_datano, m_scanno);

		for(int sindx = 0; sindx<m_scanno; sindx++)
		{
			CSeismicData * data = m_ScanData.GetAt(sindx);;
			for(int dindx = 0; dindx<m_datano; dindx++)
			{
				fprintf(fout, "%f\n", data->m_Data[dindx]);
			}
		}
		fclose(fout);

		result = true;
	}
	return result;
}

bool CDatPacket::CsvSave(CString filename, int startd, int stopd, int chindx, float sampling)
{
	bool result = false;
	FILE* fout;
	fout = fopen(filename, "w");
	if (fout)
	{
		fprintf(fout, "%f\n", (float)(startd/10.0f));
		fprintf(fout, "%f\n", (float)(stopd/10.0f));
		fprintf(fout, "%d\n", chindx);
		fprintf(fout, "%f\n", sampling);
		//printf("%d\n", startd);
		//printf("%d\n", stopd);
		//printf("%d\n", chindx);
		//printf("%f\n", sampling);
		for (int sindx = 0; sindx < m_scanno; sindx++)
		{
			CSeismicData* data = m_ScanData.GetAt(sindx);;
			for (int dindx = 0; dindx < m_datano; dindx++)
			{
				fprintf(fout, "%f, ", data->m_ProcessedData[dindx]); 
				//printf("%f, ", data->m_ProcessedData[dindx]);
			}
			fprintf(fout, "\n");
		}
		fclose(fout);

		result = true;
	}
	return result;
	
}