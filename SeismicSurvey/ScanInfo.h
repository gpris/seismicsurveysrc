#pragma once
class CScanInfo
{
public:
	CScanInfo(void);
	~CScanInfo(void);

	CString m_SiteName;
	CString m_SubSiteName;

	CString m_TxHoleName;
	CString m_RxHoleName;

	CTime m_MeasTime;

	float m_StartDepth;
	float m_StopDepth;
	float m_TargetDepth;
	float m_ScanVelocity;
	float m_ScanMode;
	enum SCANMODE {SCANBYDEPTH, SCANBYTIME};

	float m_ScanStep;
	float m_ScanDirection;
	enum SCANDIRECTON {UP_DIRECTION, DOWN_DIRECTION};

};

