#pragma once

class CLevelProperty
{
public:
	float m_Level; // Level value
	int m_Flag; // 
	CString m_LineColor; //Line color
	CString m_LineStyle; //Line style
	int m_LineWidth; //Line weight 
	int m_FVersion;
	CString m_FFGColor; //Foreground color
	CString m_FBGColor; //Background color
	CString m_FPattern;
	float m_OffsetX;
	float m_OffsetY;
	float m_ScaleX;
	float m_ScaleY;
	float m_Angle;
	float m_Coverage;
	float m_Area;

	CLevelProperty(void);
	~CLevelProperty(void);
};

class CLevelFile
{
public:
	CLevelFile(void);
	~CLevelFile(void);
	CString m_FileName;
	CString m_Identifier;
	CString m_Description[15];
	int m_LevelNum;
	CArray <CLevelProperty *, CLevelProperty *> m_LevelList;
	bool Load(CString filename);
	bool Save(CString filenames);
	void StringSplit(CString tempfull, CString *tempslice);
	COLORREF GetColorFromString(CString strColor);
	CString GetStringFromColor(COLORREF colorval);
	int GetIntFromWidth(int dlgwidth); // sanghyun 111714
	CString  GetStringFromStyle(int dlgstyle); // sanghyun 111714
	int GetLWidthFromInt(int linewidth);
};

