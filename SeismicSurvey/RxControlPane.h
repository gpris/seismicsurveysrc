#pragma once

#include "RxControlDlg.h"

// CRxControlPane

class CRxControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(CRxControlPane)

public:
	CRxControlPane();
	virtual ~CRxControlPane();

	CRxControlDlg m_RxControlDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


