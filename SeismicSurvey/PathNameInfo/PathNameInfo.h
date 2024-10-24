#pragma once
class CPathNameInfo
{
public:
	CPathNameInfo(void);
	CPathNameInfo(LPCTSTR lpszPathName);
	~CPathNameInfo(void);

	CString m_FileNameOnly;
	CString m_FileName;
	CString m_FileExt;
	CString m_Drive;
	CString m_Dir;
};

