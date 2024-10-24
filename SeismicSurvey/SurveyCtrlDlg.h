#pragma once

#include "resource.h"
#include "afxwin.h"

// CSurveyCtrlDlg 대화 상자입니다.

class CSurveyCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSurveyCtrlDlg)

public:
	CSurveyCtrlDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSurveyCtrlDlg();

	void UpdateAntennaPosition();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_SURVEYCTRLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSurveystartBtn();
	afx_msg void OnBnClickedSurveyinitBtn();
	afx_msg void OnCbnSelchangeSurveymethodCombo();
	CButton m_ScanStartBtnCtrl;
	CEdit m_SiteNameEditCtrl;
	CEdit m_SubSiteNameEditCtrl;
	CEdit m_TxHoleEditCtrl;
	CEdit m_RxHoleEditCtrl;
	CEdit m_StartDepthEditCtrl;
	CEdit m_EndDepthEditCtrl;
	CEdit m_ScanVelEditCtrl;
	CComboBox m_SurveyMethodComboCtrl;
	CEdit m_RealTxEditCtrl;
	CEdit m_RealRxEditCtrl[16];
};
