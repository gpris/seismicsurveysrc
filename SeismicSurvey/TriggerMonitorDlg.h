#pragma once


// CTriggerMonitorDlg ��ȭ �����Դϴ�.

class CSeismicSonde;

class CTriggerMonitorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTriggerMonitorDlg)

public:
	CTriggerMonitorDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTriggerMonitorDlg();

	BOOL CreateModaless(CSeismicSonde * pSonde);
	CSeismicSonde * m_pSonde;

	void CheckCompleteMessage();
	bool m_bResult;
	int m_Count;
	int m_DataNo;
	bool m_BytesReadCompleted;
	char m_TempPacket[20000];
	char m_CompletedMessage[7];

	int m_MaxWaitTimeInSec;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TRIGGERMONITORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTriggerstopButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
};
