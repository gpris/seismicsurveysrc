// CorrelationDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "CorrelationDlg.h"
#include "afxdialogex.h"


// CCorrelationDlg ��ȭ �����Դϴ�.

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


// CCorrelationDlg �޽��� ó�����Դϴ�.


void CCorrelationDlg::OnBnClickedPRBSpathOpenBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_PRBSPathInfo = CPathNameInfo(m_PRBSPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
