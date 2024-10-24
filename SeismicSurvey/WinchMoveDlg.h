#pragma once


// CWinchMoveDlg 대화 상자입니다.

class CWinchMoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWinchMoveDlg)

public:
	CWinchMoveDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWinchMoveDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WINCHMOVEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_TxTargetDepth;
	float m_TxMoveVel;
	float m_RxTargetDepth;
	float m_RxMoveVel;
};
