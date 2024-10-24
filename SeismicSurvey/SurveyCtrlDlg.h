#pragma once

#include "resource.h"
#include "afxwin.h"

// CSurveyCtrlDlg ��ȭ �����Դϴ�.

class CSurveyCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSurveyCtrlDlg)

public:
	CSurveyCtrlDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSurveyCtrlDlg();

	void UpdateAntennaPosition();
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SURVEYCTRLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
