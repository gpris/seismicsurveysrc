#include "StdAfx.h"
#include "GridPacket.h"


CGridPacket::CGridPacket(void)
{
	m_Version = 0;
	m_nRow = 0;
	m_nCol = 0;

	m_xLL = 0;
	m_yLL = 0;

	m_xSize = 0;
	m_ySize = 0;

	m_zMin = 0;
	m_zMax = 0;

	m_Rotation = 0;
	m_BlankValue = 0;

	m_Data = NULL;
}


CGridPacket::~CGridPacket(void)
{
	if(m_Data != NULL)
	{
		Delete2DData(m_Data, m_nCol);
	}
}

double ** CGridPacket::Make2DData(int nCol, int nRow)
{
	int i;
	double **t;
 
	t = new double *[nCol];
	for( i =0; i<nCol ;i++)
		t[i]= new double [nRow];

/* return pointer to array of pointers to rows */
	return t;
}

void CGridPacket::Delete2DData(double **temp, int nCol)
{
	int i;
 
	//for(i =0;i<nCol;i++)
	//		delete temp[i];
	
	delete temp;
	temp = NULL;
}

bool CGridPacket::Load(CString filename)
{
	CFile fin;
	CFileException ex;

	CString fileheader;
	char tempfileheader[20];

	if(!fin.Open(filename, CFile::modeRead|CFile::shareDenyWrite, &ex))
	{
		return false;
	}

	fileheader = _T("DSRB");

	fin.Read(tempfileheader, 4*sizeof(char));
	tempfileheader[4] = '\0';
	if(!strcmp(tempfileheader, fileheader))
	{
		int size;
		fin.Read(&size, sizeof(int));
		fin.Read(&m_Version, sizeof(int));
	}
	else
		return false;

	fileheader = _T("GRID");

	fin.Read(tempfileheader, 4*sizeof(char));
	tempfileheader[4] = '\0';
	if(!strcmp(tempfileheader, fileheader))
	{
		int size;
		fin.Read(&size, sizeof(int));

		fin.Read(&m_nRow, sizeof(int));
		fin.Read(&m_nCol, sizeof(int));

		fin.Read(&m_xLL, sizeof(double));
		fin.Read(&m_yLL, sizeof(double));

		fin.Read(&m_xSize, sizeof(double));
		fin.Read(&m_ySize, sizeof(double));

		fin.Read(&m_zMin, sizeof(double));
		fin.Read(&m_zMax, sizeof(double));

		fin.Read(&m_Rotation, sizeof(double));
		fin.Read(&m_BlankValue, sizeof(double));
	}
	else
		return false;

	fileheader = _T("DATA");

	fin.Read(tempfileheader, 4*sizeof(char));
	tempfileheader[4] = '\0';
	if(!strcmp(tempfileheader, fileheader))
	{
		int size;
		fin.Read(&size, sizeof(int));

		if(m_Data != NULL)
		{
			Delete2DData(m_Data, 0);
		}
		m_Data = Make2DData(m_nCol, m_nRow);

		if(m_Data != NULL)
		{
			for(int row = 0; row<m_nRow; row++)
			{
				for(int col = 0; col<m_nCol; col++)
				{
					fin.Read(&m_Data[col][row], sizeof(double));
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CGridPacket::LoadDSRBFormat(CString filename)
{
	CFile fin;
	CFileException ex;

	CString fileheader;
	char tempfileheader[20];

	if(!fin.Open(filename, CFile::modeRead|CFile::shareDenyWrite, &ex))
	{
		return false;
	} 

	fileheader = _T("DSRB");

	fin.Read(tempfileheader, 4*sizeof(char));
	tempfileheader[4] = '\0';
	if(!strcmp(tempfileheader, fileheader))
	{
		int size;
		fin.Read(&size, sizeof(int));
		fin.Read(&m_Version, sizeof(int));
	}
	else
		return false;

	fileheader = _T("GRID");

	fin.Read(tempfileheader, 4*sizeof(char));
	tempfileheader[4] = '\0';
	if(!strcmp(tempfileheader, fileheader))
	{
		int size;
		fin.Read(&size, sizeof(int));

		fin.Read(&m_nRow, sizeof(int));
		fin.Read(&m_nCol, sizeof(int));

		fin.Read(&m_xLL, sizeof(double));
		fin.Read(&m_yLL, sizeof(double));

		fin.Read(&m_xSize, sizeof(double));
		fin.Read(&m_ySize, sizeof(double));

		fin.Read(&m_zMin, sizeof(double));
		fin.Read(&m_zMax, sizeof(double));

		fin.Read(&m_Rotation, sizeof(double));
		fin.Read(&m_BlankValue, sizeof(double));
	}
	else
		return false;

	fileheader = _T("DATA");

	fin.Read(tempfileheader, 4*sizeof(char));
	tempfileheader[4] = '\0';
	if(!strcmp(tempfileheader, fileheader))
	{
		int size;
		fin.Read(&size, sizeof(int));

		if(m_Data != NULL)
		{
			Delete2DData(m_Data, 0);
		}
		m_Data = Make2DData(m_nCol, m_nRow);

		if(m_Data != NULL)
		{
			for(int row = 0; row<m_nRow; row++)
			{
				for(int col = 0; col<m_nCol; col++)
				{
					fin.Read(&m_Data[col][row], sizeof(double));
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CGridPacket::LoadDSAAFormat(CString filename)
{
	CStdioFile fin;
	CFileException ex;

	CString fileheader;
	char tempfileheader[20];
	char tempchar;
	CString tempstring;
	int col, row;
	int count = 0;
	if(!fin.Open(filename, CFile::modeRead|CFile::shareDenyWrite, &ex))
	{
		return false;
	} 

	fileheader = _T("DSAA");

	fin.Read(tempfileheader, 4*sizeof(char));
	tempfileheader[4] = '\0';
	if(!strcmp(tempfileheader, fileheader))
	{
		while(fin.Read(&tempchar, sizeof(char)))
		{
			if(tempchar == ' ' || tempchar == 0x0D || tempchar == 0x0A)
			{
				if(tempstring.IsEmpty())
				{
				}
				else
				{
					TRACE("%s\n", tempstring);
					switch(count)
					{
					case 0:
						m_nCol = atoi(tempstring);
						break;
					case 1:
						m_nRow = atoi(tempstring);
						break;
					case 2:
						m_xLL = atof(tempstring);
						break;
					case 3:
						m_xUL = atof(tempstring);
						break;
					case 4:
						m_yLL = atof(tempstring);
						break;
					case 5:
						m_yUL = atof(tempstring);
						break;
					case 6:
						m_zMin = atof(tempstring);
						break;
					case 7:
						m_zMax = atof(tempstring);
						m_xSize = (m_xUL - m_xLL)/(m_nCol+1);
						m_ySize = (m_yUL - m_yLL)/(m_nRow+1);

						if(m_Data != NULL)
						{
							Delete2DData(m_Data, 0);
						}
						m_Data = Make2DData(m_nCol, m_nRow);
						col = 0;
						row = 0;
						break;
					default:
						if(col<m_nCol)
						{
							m_Data[col][row] = atof(tempstring);
							col++;
						}
						else
						{
							row++;
							col = 0;
							m_Data[col][row] = atof(tempstring);
							col++;
						}
						break;
					}
					count++;
					tempstring = _T("");
				}
			}
			else
			{
				tempstring += tempchar;
			}
		}
		
		float bb = 3;
	}
	else
		return false;

	return true;
}



void CGridPacket::Save(CString filename)
{
	CString fileheader;
	char tempfileheader[20];

	CFile fout;
	CFileException ex;
	int indx;
	if(!fout.Open(filename,  CFile::modeCreate|CFile::modeWrite, &ex))
	{
		AfxMessageBox("File open fail! :" + ex.m_cause);
	}

	fileheader = _T("DSRB");

	fout.Write(fileheader, 4*sizeof(char));
	int size = 4;
	fout.Write(&size, sizeof(int));
	fout.Write(&m_Version, sizeof(int));

	fileheader = _T("GRID");

	fout.Write(fileheader, 4*sizeof(char));

	size = 72;
	fout.Write(&size, sizeof(int));

	fout.Write(&m_nRow, sizeof(int));
	fout.Write(&m_nCol, sizeof(int));

	fout.Write(&m_xLL, sizeof(double));
	fout.Write(&m_yLL, sizeof(double));

	fout.Write(&m_xSize, sizeof(double));
	fout.Write(&m_ySize, sizeof(double));

	fout.Write(&m_zMin, sizeof(double));
	fout.Write(&m_zMax, sizeof(double));

	fout.Write(&m_Rotation, sizeof(double));
	fout.Write(&m_BlankValue, sizeof(double));

	fileheader = _T("DATA");

	fout.Write(fileheader, 4*sizeof(char));
	size = m_nRow*m_nCol*8;
	fout.Write(&size, sizeof(int));

	if(m_Data != NULL)
	{
		for(int row = 0; row<m_nRow; row++)
		{
			for(int col = 0; col<m_nCol; col++)
			{
				fout.Write(&m_Data[col][row], sizeof(double));
			}
		}
	}

	fout.Close(); //sanghyun 141211 autosave
}