#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CSeismicSonde;

// CScanMonitorDlg 대화 상자입니다.

class CScanMonitorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScanMonitorDlg)

public:
	CScanMonitorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScanMonitorDlg();

	BOOL CreateModaless();
	void UpdateStatus(int ch, int npos, int nBytes);

	CSeismicSonde * m_pSonde;

	int m_Count;
	int m_ReadDataNo[16];
	int m_SuccessChannelNo;
	int m_CurrentCh;

	void ReadScanData();
	void ReadScanData2();
	void WriteReadCommand();

	bool m_bIsReadMode;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SCANMONITORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ProgressCtrlCH[16];
	CEdit m_PercentEditCtrl[16];
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual INT_PTR DoModal();
	virtual void OnOK();
};
