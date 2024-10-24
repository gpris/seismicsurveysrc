// LevelFileDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BridgeSurvey.h"
#include "LevelFileDlg.h"
#include "afxdialogex.h"
#include "LevelEditorDlg.h"

// CLevelFileDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLevelFileDlg, CDialog)

CLevelFileDlg::CLevelFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLevelFileDlg::IDD, pParent)
{
	m_sel_row = 0;
	m_sel_col = 0;
}

CLevelFileDlg::~CLevelFileDlg()
{
}

void CLevelFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEVEL_LIST, m_LevelListCtrl);

}


BEGIN_MESSAGE_MAP(CLevelFileDlg, CDialog)
	ON_BN_CLICKED(IDC_LEVELFILE_LOAD_BTN, &CLevelFileDlg::OnBnClickedLevelfileLoadBtn)
	ON_BN_CLICKED(IDC_LEVELFILE_SAVE_BTN, &CLevelFileDlg::OnBnClickedLevelfileSaveBtn)
	ON_BN_CLICKED(IDC_LEVELADD_BTN, &CLevelFileDlg::OnBnClickedLeveladdBtn)
	ON_BN_CLICKED(IDC_LEVELDELETE_BTN, &CLevelFileDlg::OnBnClickedLeveldeleteBtn)
	ON_WM_DRAWITEM()
//	ON_NOTIFY_REFLECT( NM_CUSTOMDRAW, DrawItem )
ON_NOTIFY(NM_CLICK, IDC_LEVEL_LIST, &CLevelFileDlg::OnNMClickLevelList)
ON_NOTIFY(NM_DBLCLK, IDC_LEVEL_LIST, &CLevelFileDlg::OnNMDblclkLevelList)
END_MESSAGE_MAP()


// CLevelFileDlg �޽��� ó�����Դϴ�.


void CLevelFileDlg::OnBnClickedLevelfileLoadBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	char szFilter[] = "Level file (*.lvl) |*.lvl|All Files(*.*)|*.*||";

	CBridgeSurveyApp * pApp = (CBridgeSurveyApp *)AfxGetApp();
	SetCurrentDirectory(pApp->m_HomeFolder);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	dlg.m_ofn.lpstrTitle = "Select Level File";

	if(IDOK == dlg.DoModal()) 
	{
		m_pLevelFile->Load(dlg.GetPathName());
		FillupListCtrl();

	}
}

void CLevelFileDlg::FillupListCtrl()
{
	CRect tempRect;
	m_LevelListCtrl.GetClientRect(&tempRect);
	int tempWidth = (int)(tempRect.Width()/9.0 + 0.5);

	m_LevelListCtrl.DeleteAllItems();
	 
	// ����Ʈ ��Ÿ�� ����
	m_LevelListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_LevelListCtrl.InsertColumn(0, _T("Flag"), LVCFMT_CENTER, tempWidth, -1);
	m_LevelListCtrl.InsertColumn(1, _T("Level"), LVCFMT_CENTER, tempWidth, -1);
	m_LevelListCtrl.InsertColumn(2, _T("Line"), LVCFMT_CENTER, tempWidth, -1);
	m_LevelListCtrl.InsertColumn(3, _T("FG"), LVCFMT_CENTER, tempWidth, -1);
	m_LevelListCtrl.InsertColumn(4, _T("BG"), LVCFMT_CENTER, tempWidth, -1);
	m_LevelListCtrl.InsertColumn(5, _T("Offset"), LVCFMT_CENTER, tempWidth, -1);
	m_LevelListCtrl.InsertColumn(6, _T("Scale"), LVCFMT_CENTER, tempWidth, -1);
	m_LevelListCtrl.InsertColumn(7, _T("Angle"), LVCFMT_CENTER, tempWidth, -1);
	m_LevelListCtrl.InsertColumn(8, _T("Coverage"), LVCFMT_CENTER, tempRect.Width() - tempWidth*8, -1);

	for(int tempindx = 0; tempindx < m_pLevelFile->m_LevelNum; tempindx++)
	{
		CLevelProperty * tempproperty = m_pLevelFile->m_LevelList.GetAt(tempindx);

		CString tempStr;
		tempStr.Format("%d", tempindx);
		m_LevelListCtrl.InsertItem(tempindx, tempStr);
		m_LevelListCtrl.SetItem(tempindx, 0, LVIF_TEXT , _T(""), 0, 0, 0, NULL);
		tempStr.Format("%.1f", tempproperty->m_Level);
		m_LevelListCtrl.SetItem(tempindx, 1, LVIF_TEXT, tempStr, 0, 0, 0, NULL);
		tempStr.Format("%.1f, %.1f", tempproperty->m_OffsetX,tempproperty->m_OffsetY );
		m_LevelListCtrl.SetItem(tempindx, 5, LVIF_TEXT, tempStr, 0, 0, 0, NULL);
		tempStr.Format("%.1f, %.1f", tempproperty->m_ScaleX,tempproperty->m_ScaleY );
		m_LevelListCtrl.SetItem(tempindx, 6, LVIF_TEXT, tempStr, 0, 0, 0, NULL);
		tempStr.Format("%.1f",tempproperty->m_Angle);
		m_LevelListCtrl.SetItem(tempindx, 7, LVIF_TEXT, tempStr, 0, 0, 0, NULL);
		tempStr.Format("%d", tempproperty->m_Coverage);
		m_LevelListCtrl.SetItem(tempindx, 8, LVIF_TEXT, tempStr, 0, 0, 0, NULL);
	}
}

void CLevelFileDlg::OnBnClickedLevelfileSaveBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CBridgeSurveyApp * pApp = (CBridgeSurveyApp *)AfxGetApp();
	SetCurrentDirectory(pApp->m_ProjectInfo.m_strGridFolder);
	TCHAR szFilters[] = _T ("Level files (*.lvl)|*.lvl|All files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T ("lvl"), _T ("*.lvl"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilters);

	if(IDOK == dlg.DoModal()) {
			CString strPathName = dlg.GetPathName();
			m_pLevelFile->Save(strPathName);
/*			((CMainFrame *)AfxGetMainWnd())->m_wndFileView.m_GridFileHistory.Add(dlg.GetFileName());
			((CMainFrame *)AfxGetMainWnd())->m_wndFileView.RefreshFileView();*/
	}	


}

#include <afxcolordialog.h>


void CLevelFileDlg::OnBnClickedLeveladdBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*	CMFCColorDialog dlg;


		//dlg.SetCurrentColor(RGB(0,255,0));
		//dlg.SetNewColor(RGB(0,0,255));
		//// set the red, green, and blue components of a selected 
		//// color on the two property pages of the color dialog
		//dlg.SetPageOne(255,0,0);
		//dlg.SetPageTwo(0,255,0);

		if(dlg.DoModal()==IDOK)
		{
			COLORREF rrr = dlg.GetColor();
		}*/

	CLevelEditorDlg dlg;

		CString tempLineStyle = "Solid";
		int tempLineWidth = 1;
		COLORREF Linecolorval = RGB(0,0,0);
		COLORREF FGcolorval =  RGB(127,127,127);
		COLORREF BGcolorval =  RGB(255,255,255);

	dlg.m_LevelValue = 	0.0;
	dlg.m_LineColor = Linecolorval;
	dlg.m_BG_color = BGcolorval;
	dlg.m_FG_color = FGcolorval;
	dlg.m_LineStyle = tempLineStyle;
	dlg.m_LineWidth = tempLineWidth;
	
	int levelno = m_pLevelFile->m_LevelNum;
	if (dlg.DoModal()==IDOK)
	{
		for(int tempindx = 0; tempindx < levelno; tempindx++)
		{
			CLevelProperty * tempproperty = m_pLevelFile->m_LevelList.GetAt(tempindx);
			if (dlg.m_LevelValue < tempproperty->m_Level)
			{
				CLevelProperty * newproperty = new CLevelProperty;
				newproperty->m_Level=dlg.m_LevelValue;
				newproperty->m_LineWidth = m_pLevelFile->GetIntFromWidth(dlg.m_LineWidth);
				newproperty->m_LineColor = m_pLevelFile->GetStringFromColor(dlg.m_LE_LColor.GetColor());
				newproperty->m_LineStyle = dlg.m_LineStyle;
				newproperty->m_FFGColor = m_pLevelFile->GetStringFromColor(dlg.m_LE_FG_Color.GetColor());
				newproperty->m_FBGColor =m_pLevelFile->GetStringFromColor( dlg.m_LE_BG_Color.GetColor());
				m_pLevelFile->m_LevelList.InsertAt(tempindx,newproperty);
				m_pLevelFile->m_LevelNum++;
				dlg.m_LevelIndex = tempindx;

				break;
			}
			else
			{
				if(tempindx == m_pLevelFile->m_LevelNum - 1)
				{
					CLevelProperty * newproperty = new CLevelProperty;
					newproperty->m_Level=dlg.m_LevelValue;
					newproperty->m_LineWidth = m_pLevelFile->GetIntFromWidth(dlg.m_LineWidth);
					newproperty->m_LineColor = m_pLevelFile->GetStringFromColor(dlg.m_LE_LColor.GetColor());
					newproperty->m_LineStyle = dlg.m_LineStyle;
					newproperty->m_FFGColor = m_pLevelFile->GetStringFromColor(dlg.m_LE_FG_Color.GetColor());
					newproperty->m_FBGColor =m_pLevelFile->GetStringFromColor( dlg.m_LE_BG_Color.GetColor());
					m_pLevelFile->m_LevelList.Add(newproperty);
					m_pLevelFile->m_LevelNum++;
					dlg.m_LevelIndex = tempindx;
				}
			}
		}
		FillupListCtrl();
	}
}


void CLevelFileDlg::OnBnClickedLeveldeleteBtn()
{
	/*
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	NMITEMACTIVATE* nm=(NMITEMACTIVATE*)pNMHDR;
	m_sel_row=nm->iItem;
    m_sel_col=nm->iSubItem;

	m_pLevelFile->m_LevelList.*/

	m_pLevelFile->m_LevelList.RemoveAt(m_sel_row);

	m_pLevelFile->m_LevelNum--;
	FillupListCtrl();
}

void CLevelFileDlg::OnBnClickedLeveldeleteBtnfromDlg(int levelindex)
{
	m_pLevelFile->m_LevelList.RemoveAt(levelindex);
	m_pLevelFile->m_LevelNum--;
	FillupListCtrl();

}


BOOL CLevelFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ����Ʈ �ʱ�ȭ 
	FillupListCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CLevelFileDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
    // ����� ���� �׸���(���ʵ�ο�)�� �� ��Ʈ���� �ش� ����Ʈ��Ʈ���� ���
    if(IDC_LEVEL_LIST == nIDCtl){
        // ����Ʈ ��Ʈ���� �׸� �ε��� ���� ��´�.
        int index = lpDrawItemStruct->itemID;


        // �׸� �ε������� ��ȿ�� ���
        if(index >= 0 && index < m_LevelListCtrl.GetItemCount()){
            // ����� ���� �׸��⿡ ����� DC�� ��´�.
 			CLevelProperty * tempproperty = m_pLevelFile->m_LevelList.GetAt(index);
           CDC *p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
            // �� �׸��� ���� ������ ��´�.
            CRect r = lpDrawItemStruct->rcItem;

 

            // ����Ʈ ��Ʈ���� �׸��� ���õ� ���
            if(lpDrawItemStruct->itemState & ODS_SELECTED){
                // ���� ���� �����Ѵ�.
                p_dc->SetTextColor(RGB(30, 30, 250));
                // ���õ� �׸��� ǥ���ϱ����Ͽ� �ٸ������� �׸� ������ ä���.
                p_dc->FillSolidRect(r, RGB(188, 231, 241));
            } else {
                // ����Ʈ ��Ʈ���� �׸��� ���õ��� ���� ���
                p_dc->SetTextColor(RGB(0, 0, 128));
                p_dc->FillSolidRect(r, RGB(255, 255, 255));
            }

            // �� �÷��� �ʺ� ������ ������� ����ü�� �����Ѵ�.
            LV_COLUMN column_data;
            // �ʺ� ������ ��ȯ�ޱ����� �Ӽ����� �����Ѵ�.
            column_data.mask = LVCF_WIDTH; 

 

            CString str;
            // �� �÷��� �������� 5��ŭ�� ������ ���� ���ڸ� ����Ѵ�.
            // ����Ʈ ��Ʈ���� �������ִ� ��� ��Ʈ���� �����͸� ��, �÷��� ���� ���Ѵ�.
            int pos = 5, header_count = m_LevelListCtrl.GetHeaderCtrl()->GetItemCount();

 

            // ������ �÷��� �׷����� ��µǹǷ� (�÷��� - 1)��ŭ �ݺ����� ���� �����͸� ����Ѵ�.
            for(int i = 0; i < header_count; i++){
                // �׸��� ������ ���� ��´�.
			
			    str = m_LevelListCtrl.GetItemText(index, i);
                // �÷� ������ left ���� pos ���� ���ذ��� �ش� ��ġ�� �����͸� ����Ѵ�.


                // column_data ����ü�� �ʺ� ������ �޴´�.
                m_LevelListCtrl.GetColumn(i, &column_data);
                // �� �÷��� �ʺ� ���ذ��� �ش� �÷��� ��ġ���� ��� �����Ѵ�.

				if(index == m_sel_row && i == m_sel_col)
				{
					p_dc->Rectangle(r.left + pos - 4, r.top, r.left + pos + column_data.cx - 5, r.bottom -1);
				}
                p_dc->TextOut(r.left + pos, r.top + 2, str);

				if(i == 2)
				{
					CPen contourpen;
					int penwidth = m_pLevelFile->GetLWidthFromInt(tempproperty->m_LineWidth);
					COLORREF colorval = m_pLevelFile->GetColorFromString(tempproperty->m_LineColor);
					contourpen.CreatePen(PS_SOLID, penwidth, colorval);
					CPen * pOldPen = p_dc->SelectObject(&contourpen);
					p_dc->MoveTo(r.left + pos, r.CenterPoint().y);
					p_dc->LineTo(r.left + pos + column_data.cx - 10, r.CenterPoint().y);
					p_dc->SelectObject(pOldPen);
		   //         p_dc->FillSolidRect(CRect(r.left + pos, r.top + 3, r.left + pos + column_data.cx - 10,r.bottom - 3), 
					//colorval);
				}
				if(i == 3)
				{
					COLORREF colorval = m_pLevelFile->GetColorFromString(tempproperty->m_FFGColor);
		            p_dc->FillSolidRect(CRect(r.left + pos, r.top + 3, r.left + pos + column_data.cx - 10,r.bottom - 3), 
					colorval);
				}
				if(i == 4)
				{
					COLORREF colorval = m_pLevelFile->GetColorFromString(tempproperty->m_FBGColor);
		            p_dc->FillSolidRect(CRect(r.left + pos, r.top + 3, r.left + pos + column_data.cx - 10,r.bottom - 3), 
					colorval);
				}
                pos += column_data.cx;
            }

         }
    } else CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CLevelFileDlg::OnNMClickLevelList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	NMITEMACTIVATE* nm=(NMITEMACTIVATE*)pNMHDR;
	m_sel_row=nm->iItem;
    m_sel_col=nm->iSubItem;
	Invalidate();
//	POSITION pos = m_LevelListCtrl.GetFirstSelectedItemPosition();
//	int xx = m_LevelListCtrl.GetSelectedColumn();
//	int yy = m_LevelListCtrl.GetSelectionMark();
//	int zz = m_LevelListCtrl.GetSelectedCount();
//
////	int iNdex=m_LevelListCtrl.GetNextSelectedItem(pos);
//	 int selected=-1;
//	 if (pos != NULL)
//		{
//		 while (pos)
//			   {
//				int nItem = m_LevelListCtrl.GetNextSelectedItem(pos);
//				selected = nItem+1;
//			   }
//		}
//	 //returns -1 if not selected;
//	 int result =  selected;
//
//	 int i = m_LevelListCtrl.GetSelectionMark();

	*pResult = 0;
}


void CLevelFileDlg::OnNMDblclkLevelList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	NMITEMACTIVATE* nm=(NMITEMACTIVATE*)pNMHDR;
	m_sel_row=nm->iItem;
    m_sel_col=nm->iSubItem;

 		CLevelProperty * tempproperty = m_pLevelFile->m_LevelList.GetAt(m_sel_row);
		CString tempLineStyle = tempproperty->m_LineStyle;
		int tempLineWidth = tempproperty->m_LineWidth;
		COLORREF Linecolorval = m_pLevelFile->GetColorFromString(tempproperty->m_LineColor);
		COLORREF FGcolorval = m_pLevelFile->GetColorFromString(tempproperty->m_FFGColor);
		COLORREF BGcolorval = m_pLevelFile->GetColorFromString(tempproperty->m_FBGColor);

	CLevelEditorDlg dlg;
	dlg.m_LevelValue = 	tempproperty->m_Level;
	dlg.m_LineColor = Linecolorval;
	dlg.m_BG_color = BGcolorval;
	dlg.m_FG_color = FGcolorval;
	dlg.m_LineStyle = tempLineStyle;
	dlg.m_LineWidth = tempLineWidth;
	dlg.m_LevelIndex = m_sel_row;

	if (dlg.DoModal()==IDOK)
	{
				tempproperty->m_Level=dlg.m_LevelValue;
				dlg.m_LineWidth=m_pLevelFile->GetIntFromWidth(dlg.m_LineWeight_index);
				tempproperty->m_LineWidth = dlg.m_LineWidth;
				tempproperty->m_LineColor = m_pLevelFile->GetStringFromColor(dlg.m_LE_LColor.GetColor());
				tempproperty->m_LineStyle = dlg.m_LineStyle;
				tempproperty->m_FFGColor = m_pLevelFile->GetStringFromColor(dlg.m_LE_FG_Color.GetColor());
				tempproperty->m_FBGColor =m_pLevelFile->GetStringFromColor( dlg.m_LE_BG_Color.GetColor());

	}

	FillupListCtrl();


	*pResult = 0;
}
