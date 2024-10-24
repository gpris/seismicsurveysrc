// WinchMoveDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "WinchMoveDlg.h"
#include "afxdialogex.h"


// CWinchMoveDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWinchMoveDlg, CDialogEx)

CWinchMoveDlg::CWinchMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinchMoveDlg::IDD, pParent)
	, m_TxTargetDepth(0)
	, m_TxMoveVel(0)
	, m_RxTargetDepth(0)
	, m_RxMoveVel(0)
{

}

CWinchMoveDlg::~CWinchMoveDlg()
{
}

void CWinchMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXTARGETDEPTHEDIT, m_TxTargetDepth);
	DDX_Text(pDX, IDC_TXMOVEVELEDIT, m_TxMoveVel);
	DDX_Text(pDX, IDC_RXTARGETDEPTHEDIT, m_RxTargetDepth);
	DDX_Text(pDX, IDC_RXMOVEVELEDIT, m_RxMoveVel);
}


BEGIN_MESSAGE_MAP(CWinchMoveDlg, CDialogEx)
END_MESSAGE_MAP()


// CWinchMoveDlg �޽��� ó�����Դϴ�.
