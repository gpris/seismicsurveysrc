#pragma once


// CSetDepthDlg ��ȭ �����Դϴ�.

class CSetDepthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDepthDlg)

public:
	CSetDepthDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSetDepthDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SETDEPTHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float m_TxCurDepth;
	float m_RxCurDepth;
};
