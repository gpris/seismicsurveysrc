#include "StdAfx.h"
#include "LevelFile.h"

CLevelProperty::CLevelProperty(void)
{
	m_Level = 0.0;
	m_Flag = 0;
	m_LineColor = _T("Black");
	m_LineStyle = _T("SOLID");
	m_LineWidth = 1;
	m_FVersion = 0;
	m_FFGColor = _T("White");
	m_FBGColor = _T("White");
	m_FPattern = _T("SOLID");
	m_OffsetX = 0.0;
	m_OffsetY = 0.0;
	m_ScaleX = 0.0;
	m_ScaleY = 0.0;
	m_Angle = 0.0;
	m_Coverage = 0.0;
	m_Area = 0.0;
}


CLevelProperty::~CLevelProperty(void)
{
}

CLevelFile::CLevelFile(void)
{
	m_Identifier = _T("LVL3");

	m_Description[0] = _T("Level");
	m_Description[1] = _T("Flags");
	m_Description[2] = _T("LColor");
	m_Description[3] = _T("LStyle");
	m_Description[4] = _T("LWidth");
	m_Description[5] = _T("FVersion");
	m_Description[6] = _T("FFGColor");
	m_Description[7] = _T("FBGColor");
	m_Description[8] = _T("FPattern");
	m_Description[9] = _T("OffsetX");
	m_Description[10] = _T("OffsetY");
	m_Description[11] = _T("ScaleX");
	m_Description[12] = _T("ScaleY");
	m_Description[13] = _T("AngleX");
	m_Description[14] = _T("Coverage");

	m_LevelList.RemoveAll();

	m_FileName = _T("");
}


CLevelFile::~CLevelFile(void)
{
}

bool CLevelFile::Load(CString filename)
{

	FILE *fin;

	if((fin = fopen(filename, "r")) == NULL)
	{
		AfxMessageBox("File open fail! :");
		return false;
	}

	m_FileName = filename;

	char Buffer[200];
	fgets(Buffer, 200, fin);
	if(m_Identifier.Compare(Buffer))
	{
		int result;
		fgets(Buffer, 200, fin);
		char tempstring[200];
		CString tempfull;
		CString * tempslice = new CString[15];
		m_LevelList.RemoveAll();

		while(fgets(tempstring, 200, fin) != NULL)
		{
			tempfull.Format("%s", tempstring);
			for(int indx = 0;indx<15;indx++)
			{
				tempslice[indx] = _T("");
			}
			StringSplit(tempfull, tempslice);
			CLevelProperty * tempProperty = new CLevelProperty;

			tempProperty->m_Level = atof(tempslice[0]);
			tempProperty->m_Flag = atoi(tempslice[1]);
			tempProperty->m_LineColor = tempslice[2];
			tempProperty->m_LineStyle = tempslice[3];
			tempProperty->m_LineWidth = atoi(tempslice[4]);
			tempProperty->m_FVersion = atoi(tempslice[5]);
			tempProperty->m_FFGColor = tempslice[6];
			tempProperty->m_FBGColor = tempslice[7];
			tempProperty->m_FPattern = tempslice[8];
			tempProperty->m_OffsetX = atof(tempslice[9]);
			tempProperty->m_OffsetY = atof(tempslice[10]);
			tempProperty->m_ScaleX = atof(tempslice[11]);
			tempProperty->m_ScaleY = atof(tempslice[12]);
			tempProperty->m_Angle = atof(tempslice[13]);
			tempProperty->m_Coverage = atof(tempslice[14]);

//			COLORREF tempcolor = GetColorFromString(tempProperty->m_FFGColor);
			m_LevelList.Add(tempProperty);
	
		};

		m_LevelNum = m_LevelList.GetSize();
	}
	else
	{
		AfxMessageBox("This file is not supported. Please make sure file type.");
	}
	fclose(fin);
	return true;
}

bool CLevelFile::Save(CString filename)
{

	CFile  fout2;
	CFileException ex;
	if(!fout2.Open(filename,  CFile::modeCreate|CFile::modeWrite, &ex))
	{
		AfxMessageBox("File open fail! :" + ex.m_cause);
	}

	m_FileName = filename;

	CString fileheader = _T("LVL3\n");
	fout2.Write(fileheader, (fileheader.GetLength())*sizeof(char));
	CString indextitle =_T("\'Level Flags LColor LStyle LWidth FVersion FFGColor FBGColor FPattern OffsetX OffsetY ScaleX ScaleY Angle Coverage\n");
	fout2.Write(indextitle,(indextitle.GetLength())*sizeof(char));
	for (int indx = 0 ; indx < m_LevelNum; indx++)
	{
		CLevelProperty * tempProperty = m_LevelList.GetAt(indx);

		CString temploop=_T("");
		temploop.Format(_T("%.1f"), tempProperty->m_Level);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %d"), tempProperty->m_Flag);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" \"%s\""), tempProperty->m_LineColor);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" \"%s\""), tempProperty->m_LineStyle);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %d"), tempProperty->m_LineWidth);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %d"), tempProperty->m_FVersion);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" \"%s\""), tempProperty->m_FFGColor);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" \"%s\""), tempProperty->m_FBGColor);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" \"%s\""), tempProperty->m_FPattern);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %.1f"), tempProperty->m_OffsetX);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %.1f"), tempProperty->m_OffsetY);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %.1f"), tempProperty->m_ScaleX);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %.1f"), tempProperty->m_ScaleY);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %.1f"), tempProperty->m_Angle);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
		temploop.Format(_T(" %.1f\n"), tempProperty->m_Coverage);
		fout2.Write(temploop,(temploop.GetLength())*sizeof(char));
	}
	fout2.Close();
	return true;
}

void CLevelFile::StringSplit(CString tempfull, CString *tempslice)
{
	int countstring = 0;
	int countword = 0;

	int datanum = tempfull.GetLength();

	char oldchar = ' ';
	int stringmode = 0;
	for(int i = 0; i<datanum; i++)
	{
		char tempchar = tempfull.GetAt(i);
		if(tempchar == '"')
		{
			if(stringmode)
			{
				stringmode = 0;
			}
			else
			{
				stringmode = 1;
			}
		}
		else
		{
			if(stringmode)
			{
				tempslice[countstring].AppendChar(tempchar);
			}
			else
			{
				if(tempchar == ' ')
				{
					countstring++;
				}
				else
				{
					tempslice[countstring].AppendChar(tempchar);
				}
			}
		}
	}
}

#ifndef RGBA
#define RGBA(r,g,b,a)        ((COLORREF)( (((DWORD)(BYTE)(a))<<24) |     RGB(r,g,b) ))
#endif
COLORREF CLevelFile::GetColorFromString(CString strColor)
{
	CString nColor[4];
	char chDelimeter[3] = {' ',' ',' '};
	int pos;
	COLORREF result;

	if((pos = strColor.Find(' ')) == -1)
	{
		if(strColor == _T("Black")) result = RGB(0, 0, 0);
		if(strColor == _T("Red")) result = RGB(255, 0, 0);
		if(strColor == _T("Blue")) result = RGB(0, 0, 255);
		if(strColor == _T("Green")) result = RGB(0, 255, 0);
		if(strColor == _T("White")) result = RGB(255, 255, 255);
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			if((pos = strColor.Find(chDelimeter[i])) == -1)
			{
				result = RGB(255, 255, 255);
				break;
			}
			else
			{
				nColor[i] = strColor.Left(pos);
				strColor = strColor.Mid(pos+1);
				if(i == 2)
				{
					nColor[i+1] = strColor;
				}
			}

		}
		int R = atoi(nColor[0].Mid(1));
		int G = atoi(nColor[1].Mid(1));
		int B = atoi(nColor[2].Mid(1));
		int A = atoi(nColor[3].Mid(1));
		result = RGB(R, G, B);
	}
	return result;
}

#define GetAlphaValue(rgb)   ((BYTE) ((rgb) >> 24)) 
CString CLevelFile::GetStringFromColor(COLORREF colorval)
{
	CString ColorString;

	ColorString.Format("R%d G%d B%d A%d", GetRValue(colorval), GetGValue(colorval), GetBValue(colorval), GetAlphaValue(colorval));

	return ColorString;
}

	int CLevelFile::GetIntFromWidth(int dlgwidth){
		int retvalue=0;
		switch (dlgwidth){
			case 0:
				retvalue= 0;
				break;
			case 1:
				retvalue= 1;
				break;
			case 2:
				retvalue= 10;
				break;
			case 3:
				retvalue= 15;
				break;
			case 4:
				retvalue= 20;
				break;
			case 5:
				retvalue= 30;
				break;

			default:
				retvalue= 1;
		}
		return retvalue;
	}

	int CLevelFile::GetLWidthFromInt(int linewidth){
		int retvalue=0;
		if (linewidth == 0){
				retvalue= 0;
		}
		if(linewidth<= 10&&linewidth>0){
				retvalue= 1;
		}
		if(linewidth> 10 &&linewidth<30){
				retvalue= 3;
		}
		if(linewidth >=30){
				retvalue=5;
		}
		return retvalue;
	}

	CString  CLevelFile::GetStringFromStyle(int dlgstyle){
		CString stretvalue ="";
		switch (dlgstyle){
			case 0:
				stretvalue= "Invisible";
				break;
			case 1:
				stretvalue="Solid";
				break;
			case 2:
				stretvalue= "Dot";
				break;
			case 3:
				stretvalue= "Dash";
				break;
			case 4:
				stretvalue= "Dash dot";
				break;
			case 5:
				stretvalue= "Dash dot dot";
				break;

			default:
				stretvalue= "Invisible";
		}
		return stretvalue;

	}
