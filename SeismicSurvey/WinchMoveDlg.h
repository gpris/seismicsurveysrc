#pragma once


// CWinchMoveDlg ��ȭ �����Դϴ�.

class CWinchMoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWinchMoveDlg)

public:
	CWinchMoveDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWinchMoveDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WINCHMOVEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float m_TxTargetDepth;
	float m_TxMoveVel;
	float m_RxTargetDepth;
	float m_RxMoveVel;
};
