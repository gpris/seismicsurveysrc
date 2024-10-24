// LevelEditorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BridgeSurvey.h"
#include "LevelEditorDlg.h"
#include "afxdialogex.h"
#include "LevelFileDlg.h"


// CLevelEditorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLevelEditorDlg, CDialogEx)

CLevelEditorDlg::CLevelEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLevelEditorDlg::IDD, pParent)
	, m_LevelValue(0)
{
	m_LineWeight_index = 0;
	m_LineStyle_index =0;
}

CLevelEditorDlg::~CLevelEditorDlg()
{
}

void CLevelEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LE_VALUE, m_LevelValue);
	DDX_Control(pDX, IDC_LE_WEIGHT_COMBO, m_LineWeight_Combo);
	DDX_Control(pDX, IDC_LE_STYLE_COMBO, m_LineStyle_Combo);
	DDX_Text(pDX, IDC_LE_LCOLOR_COMBO, m_LineColor);
	DDX_Text(pDX, IDC_LE_FGCOLORBUTTON, m_FG_color);
	DDX_Text(pDX, IDC_LE_BGCOLORBUTTON, m_BG_color); 
	DDX_Control(pDX, IDC_LE_LCOLOR_COMBO, m_LE_LColor);
	DDX_Control(pDX, IDC_LE_FGCOLORBUTTON, m_LE_FG_Color);
	DDX_Control(pDX, IDC_LE_BGCOLORBUTTON, m_LE_BG_Color);
}


BEGIN_MESSAGE_MAP(CLevelEditorDlg, CDialogEx)

	ON_BN_CLICKED(IDC_LEVEL_DELETE_BUTTON, &CLevelEditorDlg::OnBnClickedLevelDeleteButton)
	ON_CBN_SELCHANGE(IDC_LE_WEIGHT_COMBO, &CLevelEditorDlg::OnCbnSelchangeLeWeightCombo)
END_MESSAGE_MAP()


// CLevelEditorDlg 메시지 처리기입니다.


BOOL CLevelEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString s;
	CString s2;

	COMBOBOXEXITEM comboboxexitem;
	COMBOBOXEXITEM comboboxexitem2;

	m_imagelist_weight.Create(IDB_LINE_WEIGHT_LIST,50,0,RGB(255,0,255));
	m_LineWeight_Combo.SetImageList(&m_imagelist_weight);
	CString s0[6]={"0","1","10","15","20","30"};
	CString s3[6]={"Invisible","Solid","Dot","Dash","Dash dot", "Dash dot dot"};

	for (int i=0;i<6;i++) // LineWidth, linethickness or LineWeight 0px 1px 2px 3px 4px and 5px 
	{
	comboboxexitem.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_OVERLAY;
	comboboxexitem.iItem = i;

	s.Format(_T("%s"),s0[i]);
	comboboxexitem.pszText=(LPTSTR)(LPCTSTR)s; // CString -> LPSTR
	comboboxexitem.iImage=i;
	comboboxexitem.iSelectedImage = comboboxexitem.iImage;
	comboboxexitem.iOverlay = i;
	m_LineWeight_Combo.InsertItem(&comboboxexitem);
	}
	int nLWeight =1;
	if (m_LineWidth ==0)
		nLWeight = 0;
	if (m_LineWidth >0 && m_LineWidth<=10)
		nLWeight = 1;
	if (m_LineWidth >10 && m_LineWidth<30)
		nLWeight = 3;
	if (m_LineWidth >=30)
		nLWeight = 5;

	m_LineWeight_Combo.SetCurSel(nLWeight);

	m_imagelist_style.Create(IDB_LINE_STYLE_LIST,50,0,RGB(255,0,255));
	m_LineStyle_Combo.SetImageList(&m_imagelist_style);

	for (int j=0;j<6;j++) // 6 styles: Invisible, Solid, Dotted, Dash, Dash dot, and Dash dot dot 
	{
	comboboxexitem2.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_OVERLAY;
	comboboxexitem2.iItem =j;

	s2.Format(_T("%s"),s3[j]);
	comboboxexitem2.pszText=(LPTSTR)(LPCTSTR)s2;
	comboboxexitem2.iImage=j;
	comboboxexitem2.iSelectedImage = comboboxexitem2.iImage;
	comboboxexitem2.iOverlay = i;
	m_LineStyle_Combo.InsertItem(&comboboxexitem2);
	}

	int nLStyle = 1;
	if (m_LineStyle =="Invisible")
		nLStyle = 0;
	if (m_LineStyle =="Solid")
		nLStyle = 1;
	if (m_LineStyle =="Dot")
		nLStyle = 2;
	if (m_LineStyle =="Dash")
		nLStyle = 3;

	m_LineStyle_Combo.SetCurSel(nLStyle);

	m_LE_LColor.SetColor(m_LineColor);
	m_LE_FG_Color.SetColor(m_FG_color);
	m_LE_BG_Color.SetColor(m_BG_color);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLevelEditorDlg::OnCbnSelchangeLeStyleCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_LineStyle_index = m_LineStyle_Combo.GetCurSel();
}


void CLevelEditorDlg::OnBnClickedLevelDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CLevelFileDlg *)GetParent())->OnBnClickedLeveldeleteBtnfromDlg( m_LevelIndex);
	EndDialog(-1);
}


void CLevelEditorDlg::OnCbnSelchangeLeWeightCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_LineWeight_index = m_LineWeight_Combo.GetCurSel();
}
