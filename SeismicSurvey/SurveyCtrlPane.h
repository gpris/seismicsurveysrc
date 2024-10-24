#pragma once

#include "SurveyCtrlDlg.h"

// CSurveyCtrlPane

class CSurveyCtrlPane : public CDockablePane
{
	DECLARE_DYNAMIC(CSurveyCtrlPane)

public:
	CSurveyCtrlPane();
	virtual ~CSurveyCtrlPane();

	CSurveyCtrlDlg m_SurveyCtrlDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


