#pragma once
class CGridPacket
{
public:
	CGridPacket(void);
	~CGridPacket(void);

	int m_Version;

	int m_nRow;
	int m_nCol;

	double m_xLL;
	double m_yLL;

	double m_xUL;
	double m_yUL;

	double m_xSize;
	double m_ySize;

	double m_zMin;
	double m_zMax;

	double m_Rotation;
	double m_BlankValue;

	double **m_Data;

	double ** Make2DData(int nCol, int nRow);
	void Delete2DData(double **temp, int nCol);
	bool Load(CString filename);
	void Save(CString filename);
	bool LoadDSRBFormat(CString filename);
	bool LoadDSAAFormat(CString filename);
};

