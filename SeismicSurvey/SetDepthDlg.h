#pragma once


// CSetDepthDlg 대화 상자입니다.

class CSetDepthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDepthDlg)

public:
	CSetDepthDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetDepthDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETDEPTHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_TxCurDepth;
	float m_RxCurDepth;
};
