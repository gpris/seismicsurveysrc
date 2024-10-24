// IIRFilterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "IIRFilterDlg.h"
#include "afxdialogex.h"


// CIIRFilterDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CIIRFilterDlg, CDialogEx)

CIIRFilterDlg::CIIRFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIIRFilterDlg::IDD, pParent)
	, m_SamplingFreq(0)
	, m_HPFFreq(0)
	, m_LPFFreq(0)
{

}

CIIRFilterDlg::~CIIRFilterDlg()
{
}

void CIIRFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_IIR_SAMPFREQ_EDIT, m_SamplingFreq);
	DDX_Text(pDX, IDC_IIR_HPFFREQ_EDIT, m_HPFFreq);
	DDX_Text(pDX, IDC_IIR_LPFFREQ_EDIT, m_LPFFreq);
}


BEGIN_MESSAGE_MAP(CIIRFilterDlg, CDialogEx)
END_MESSAGE_MAP()


// CIIRFilterDlg �޽��� ó�����Դϴ�.


BOOL CIIRFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
