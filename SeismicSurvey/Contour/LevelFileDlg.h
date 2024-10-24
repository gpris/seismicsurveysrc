#pragma once
#include "LevelFile.h"

// CLevelFileDlg 대화 상자입니다.

class CLevelFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CLevelFileDlg)

public:
	CLevelFileDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLevelFileDlg();

	CListCtrl m_LevelListCtrl;
	CLevelFile * m_pLevelFile;
	void FillupListCtrl();
	int m_sel_row;
	int m_sel_col;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LEVELFILE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLevelfileLoadBtn();
	afx_msg void OnBnClickedLevelfileSaveBtn();
	afx_msg void OnBnClickedLeveladdBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLeveldeleteBtn();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnNMClickLevelList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLevelList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedLeveldeleteBtnfromDlg(int lvindex);
};
