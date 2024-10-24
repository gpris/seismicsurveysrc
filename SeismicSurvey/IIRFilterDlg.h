#pragma once
#include "afxwin.h"


// CIIRFilterDlg 대화 상자입니다.


class CIIRFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIIRFilterDlg)

public:
	CIIRFilterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIIRFilterDlg();



// 대화 상자 데이터입니다.
	enum { IDD = IDD_IIRFILTER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	float m_SamplingFreq;
	float m_HPFFreq;
	float m_LPFFreq;
};
