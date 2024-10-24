
#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxcolorbutton.h"


// CLevelEditorDlg ��ȭ �����Դϴ�.

class CLevelEditorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLevelEditorDlg)

public:
	CLevelEditorDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLevelEditorDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LEVEL_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	CImageList m_imagelist_weight;
	CImageList m_imagelist_style;

	DECLARE_MESSAGE_MAP()
public:
	float m_LevelValue;
	CComboBoxEx m_LineWeight_Combo;
	int m_LineWeight_index;
	CComboBoxEx m_LineStyle_Combo;
	int m_LineStyle_index;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeLeLweightCombobox();
	afx_msg void OnCbnSelchangeLeStyleCombo();
	COLORREF m_LineColor;
	COLORREF m_FG_color;
	COLORREF m_BG_color;
	int m_LineWidth;
	CString m_LineStyle;
	CMFCColorButton m_LE_LColor;
	CMFCColorButton m_LE_FG_Color;
	CMFCColorButton m_LE_BG_Color;
	int m_LevelIndex;
	afx_msg void OnBnClickedLevelDeleteButton();
	afx_msg void OnCbnSelchangeLeWeightCombo();
};
