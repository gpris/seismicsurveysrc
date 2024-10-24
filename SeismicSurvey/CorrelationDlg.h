#pragma once
#include "afxwin.h"

#include "PathNameInfo\PathNameInfo.h"

// CCorrelationDlg ��ȭ �����Դϴ�.

class CCorrelationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCorrelationDlg)

public:
	CCorrelationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCorrelationDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CORRELATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_PRBSPathEditCtrl;
	CEdit m_DutyCycleEditCtrl;
	CString m_PRBSPath;

	CPathNameInfo m_PRBSPathInfo;

	int m_DutyCycle;
	afx_msg void OnBnClickedPRBSpathOpenBtn();
	virtual BOOL OnInitDialog();
};
