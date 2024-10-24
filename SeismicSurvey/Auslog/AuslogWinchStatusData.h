#pragma once

class CAuslogWinchStatusData
{
public:
	CAuslogWinchStatusData(void);
	~CAuslogWinchStatusData(void);
	float m_Depth;
	float m_Speed;
	bool m_bIsLive;
	float m_OffsetDepth;
};
