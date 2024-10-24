#pragma once
#include "afxwin.h"

#include "SeismicSonde.h"

// CTxControlDlg ��ȭ �����Դϴ�.

class CTxControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTxControlDlg)

public:
	CTxControlDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTxControlDlg();
	void TxControlEnable(bool bEnable);
	CSeismicSonde * m_pSeismicTx;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TXCONTROLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_TxComportComboCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTxconnectbtn();
	CButton m_TxConnectBtnCtrl;
	CComboBox m_TxPRBSFreqComboCtrl;
	CComboBox m_TxPRBSLengthComboCtrl;
	afx_msg void OnCbnSelchangeTxPRBSFreqCombo();
	afx_msg void OnCbnSelchangeTxPRBSLengthCombo();
	CEdit m_PRBSDelayEditCtrl;
	int m_PRBSDelay;
	CEdit m_PRBSRepeatEditCtrl;
	int m_PRBSRepeat;
	afx_msg void OnDeltaposPRBSDelaySpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPRBSRepeatSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
