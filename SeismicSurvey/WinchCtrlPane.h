#pragma once

#include "WinchCtrlDlg.h"

// CWinchCtrlPane

class CWinchCtrlPane : public CDockablePane
{
	DECLARE_DYNAMIC(CWinchCtrlPane)

public:
	CWinchCtrlPane();
	virtual ~CWinchCtrlPane();
	
	CWinchCtrlDlg m_WinchCtrlDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


