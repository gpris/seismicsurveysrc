// IIRFilterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "IIRFilterDlg.h"
#include "afxdialogex.h"


// CIIRFilterDlg 대화 상자입니다.

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


// CIIRFilterDlg 메시지 처리기입니다.


BOOL CIIRFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
