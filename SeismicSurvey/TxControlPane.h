#pragma once

#include "TxControlDlg.h"

// CTxControlPane

class CTxControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(CTxControlPane)

public:
	CTxControlPane();
	virtual ~CTxControlPane();

	CTxControlDlg m_TxControlDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


