#pragma once
#include "afxwin.h"


// CIIRFilterDlg ��ȭ �����Դϴ�.


class CIIRFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIIRFilterDlg)

public:
	CIIRFilterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CIIRFilterDlg();



// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IIRFILTER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	float m_SamplingFreq;
	float m_HPFFreq;
	float m_LPFFreq;
};
