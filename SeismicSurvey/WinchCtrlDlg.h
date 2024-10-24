#pragma once
#include "afxwin.h"
#include "Resource.h"

#include ".\GCColorStatic\GCColorStatic.h"
#include "AuslogWinch.h"

// CWinchCtrlDlg 대화 상자입니다.

class CWinchCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWinchCtrlDlg)

public:
	CWinchCtrlDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWinchCtrlDlg();
	void WinchControlEnable(bool bEnable);
	void WinchDepthUpdate();
	CAuslogWinch * m_pWinch;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WINCHCTRLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_WinchPortComboCtrl;
	CButton m_WinchConnectBtnCtrl;
	CGCColorStatic m_WinchTxDepthEditCtrl;
	CGCColorStatic m_WinchRxDepthEditCtrl;
	afx_msg void OnBnClickedWinchMoveBtn();
	afx_msg void OnBnClickedWinchStopBtn();
	afx_msg void OnBnClickedWinchDepthInitBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWinchConnectBtn();
	CButton m_WinchMoveBtnCtrl;
	CButton m_WinchStopBtnCtrl;
	CButton m_WinchDepthInitBtnCtrl;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
