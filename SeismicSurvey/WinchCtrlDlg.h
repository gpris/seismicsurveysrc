#pragma once
#include "afxwin.h"
#include "Resource.h"

#include ".\GCColorStatic\GCColorStatic.h"
#include "AuslogWinch.h"

// CWinchCtrlDlg ��ȭ �����Դϴ�.

class CWinchCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWinchCtrlDlg)

public:
	CWinchCtrlDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWinchCtrlDlg();
	void WinchControlEnable(bool bEnable);
	void WinchDepthUpdate();
	CAuslogWinch * m_pWinch;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WINCHCTRLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
