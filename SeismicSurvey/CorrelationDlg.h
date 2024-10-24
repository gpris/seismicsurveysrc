#pragma once
#include "afxwin.h"

#include "PathNameInfo\PathNameInfo.h"

// CCorrelationDlg 대화 상자입니다.

class CCorrelationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCorrelationDlg)

public:
	CCorrelationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCorrelationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CORRELATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
