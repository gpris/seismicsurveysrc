// CorrelationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "CorrelationDlg.h"
#include "afxdialogex.h"


// CCorrelationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCorrelationDlg, CDialogEx)

CCorrelationDlg::CCorrelationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCorrelationDlg::IDD, pParent)
	, m_PRBSPath(_T(""))
	, m_DutyCycle(0)
{

}

CCorrelationDlg::~CCorrelationDlg()
{
}

void CCorrelationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CORR_PRBSPATH_EDIT, m_PRBSPathEditCtrl);
	DDX_Control(pDX, IDC_CORR_DUTYCYCLE_EDIT, m_DutyCycleEditCtrl);
	DDX_Text(pDX, IDC_CORR_PRBSPATH_EDIT, m_PRBSPath);
	DDX_Text(pDX, IDC_CORR_DUTYCYCLE_EDIT, m_DutyCycle);
}


BEGIN_MESSAGE_MAP(CCorrelationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PRBSPATHOPEN_BTN, &CCorrelationDlg::OnBnClickedPRBSpathOpenBtn)
END_MESSAGE_MAP()


// CCorrelationDlg 메시지 처리기입니다.


void CCorrelationDlg::OnBnClickedPRBSpathOpenBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, _T("txt"), _T("*.txt"), NULL, _T("PRBS code files(*.txt)"), NULL);

	CString temppath;
	temppath = m_PRBSPathInfo.m_Drive + m_PRBSPathInfo.m_Dir;

	dlg.GetOFN().lpstrInitialDir = temppath;

	if(dlg.DoModal() == IDOK)
	{
		m_PRBSPath = dlg.GetPathName();
		m_PRBSPathInfo = CPathNameInfo(m_PRBSPath);

		UpdateData(false);
	}

}


BOOL CCorrelationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_PRBSPathInfo = CPathNameInfo(m_PRBSPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
