#include "StdAfx.h"
#include "PathNameInfo.h"

CPathNameInfo::CPathNameInfo(void)
{
}

CPathNameInfo::CPathNameInfo(LPCTSTR lpszPathName)
{
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szFileName[200];
	TCHAR szFileExt[20];

	_tsplitpath(lpszPathName, szDrive, szDir, szFileName, szFileExt);

	m_FileNameOnly.Format("%s", szFileName);
	m_FileNameOnly.MakeLower();

	m_FileName.Format("%s%s", szFileName, szFileExt);
	m_FileName.MakeLower();

	m_FileExt.Format("%s", szFileExt);
	m_FileExt.MakeLower();

	m_Drive.Format("%s", szDrive);
	m_Drive.MakeLower();

	m_Dir.Format("%s", szDir);
	m_Dir.MakeLower();
}


CPathNameInfo::~CPathNameInfo(void)
{
}
