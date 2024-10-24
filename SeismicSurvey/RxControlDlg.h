#pragma once
#include "afxwin.h"

#include "SeismicSonde.h"
#include "afxcmn.h"

// CRxControlDlg 대화 상자입니다.

class CRxControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRxControlDlg)

public:
	CRxControlDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRxControlDlg();
	void RxControlEnable(bool bEnable);
	void RxChannelUpdate();
	CSeismicSonde * m_pSeismicRx;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_RXCONTROLDLG };

	int m_StartCh;
	int m_ChannelNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_RxComportComboCtrl;
	afx_msg void OnBnClickedRxConnectbtn();
	CButton m_RxConnectBtnCtrl;
	afx_msg void OnBnClickedTriggerApplyBtn();
	CComboBox m_RxTriggerComboCtrl;
	CComboBox m_Rx1stAmpComboCtrl;
	CComboBox m_Rx2ndAmpComboCtrl;
	CComboBox m_SamplingClkComboCtrl;
	CComboBox m_RxPRBSFreqComboCtrl;
	CComboBox m_RxPRBSLengthComboCtrl;
	CButton m_RxTriggerApplyBtnCtrl;
	afx_msg void OnCbnSelchangeRxTriggerCombo();
	afx_msg void OnCbnSelchangeRx1stAmpCombo();
	afx_msg void OnCbnSelchangeRx2ndAmpCombo();
	afx_msg void OnCbnSelchangeSamplingClkCombo();
	afx_msg void OnCbnSelchangeRxPRBSFreqCombo();
	afx_msg void OnCbnSelchangeRxPRBSLengthCombo();
	afx_msg void OnBnClickedUartMode1Radio();
	afx_msg void OnBnClickedUartMode2Radio();
	int m_UARTMode;
	CButton m_UARTModeRadio1BtnCtrl;
	CButton m_UARTModeRadio2BtnCtrl;

	CEdit m_PRBSDelayEditCtrl;
	int m_PRBSDelay;
	CEdit m_PRBSRepeatEditCtrl;
	int m_PRBSRepeat;
	afx_msg void OnDeltaposRxprbsdelaySpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRxprbsrepeatSpin(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_RxDelayComboCtrl;
	CComboBox m_RxDeviceComboCtrl;
	CButton m_RxDeviceEnableCheckCtrl;
	int m_DeviceID;
	afx_msg void OnCbnSelchangeRxdevicecombo();
	afx_msg void OnCbnSelchangeRxtriggerfreqcombo();
	CComboBox m_RxTriggerFreqCtrl;
	afx_msg void OnCbnSelchangeRxdelaycombo();
	afx_msg void OnBnClickedRxdeviceenableCheck();
	afx_msg void OnBnClickedDatareadbtn();
	CProgressCtrl m_RxStatusProgressCtrl;
	CStatic m_RxStatusCtrl;
	CEdit m_RxStatusEditCtrl;
	CButton m_DataReadBtnCtrl;
	afx_msg void OnEnKillfocusRxprbsdelayEdit();
	afx_msg void OnCbnSelchangeRxAccmulcombo();
	CComboBox m_RxAccumComboCtrl;
};
