// SetDepthDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "SetDepthDlg.h"
#include "afxdialogex.h"


// CSetDepthDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetDepthDlg, CDialogEx)

CSetDepthDlg::CSetDepthDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetDepthDlg::IDD, pParent)
	, m_TxCurDepth(0)
	, m_RxCurDepth(0)
{

}

CSetDepthDlg::~CSetDepthDlg()
{
}

void CSetDepthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXCURDEPTHEDIT, m_TxCurDepth);
	DDX_Text(pDX, IDC_RXCURDEPTHEDIT, m_RxCurDepth);
}


BEGIN_MESSAGE_MAP(CSetDepthDlg, CDialogEx)
END_MESSAGE_MAP()


// CSetDepthDlg 메시지 처리기입니다.
