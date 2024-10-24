#pragma once
#include "LevelFile.h"

// CLevelFileDlg ��ȭ �����Դϴ�.

class CLevelFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CLevelFileDlg)

public:
	CLevelFileDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLevelFileDlg();

	CListCtrl m_LevelListCtrl;
	CLevelFile * m_pLevelFile;
	void FillupListCtrl();
	int m_sel_row;
	int m_sel_col;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LEVELFILE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
