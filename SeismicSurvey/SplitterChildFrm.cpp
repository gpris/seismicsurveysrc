// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://msdn.microsoft.com/officeui�� �����Ͻʽÿ�.
//
// Copyright (C) Microsoft Corporation
// ��� �Ǹ� ����.

// ChildFrm.cpp : CSplitterChildFrame Ŭ������ ����
//

#include "stdafx.h"
#include "SeismicSurvey.h"

#include "SplitterChildFrm.h"
#include "SeismicSurveyView.h"
#include "FileTreeView.h"
#include "ScopeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSplitterChildFrame

IMPLEMENT_DYNCREATE(CSplitterChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CSplitterChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CSplitterChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CSplitterChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSplitterChildFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CSplitterChildFrame::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

// CSplitterChildFrame ����/�Ҹ�

CSplitterChildFrame::CSplitterChildFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CSplitterChildFrame::~CSplitterChildFrame()
{
}


BOOL CSplitterChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CSplitterChildFrame ����

#ifdef _DEBUG
void CSplitterChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CSplitterChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CSplitterChildFrame �޽��� ó����

void CSplitterChildFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CSplitterChildFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // �μ� �̸� ���� ��带 �ݽ��ϴ�.
	}
}

void CSplitterChildFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}


BOOL CSplitterChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
/* CSplitterWnd���� 
	if (!m_wndSplitter.CreateStatic(this,1, 2))
		return false;
	m_wndSplitter.CreateView(0,0, RUNTIME_CLASS(CFileTreeView), CSize(50,100), pContext);
	m_wndSplitter.CreateView(0,1, RUNTIME_CLASS(CSeismicSurveyView), CSize(800,100), pContext);
*/

/* CBiSplitterWnd��� */
	if (!m_wndBiSplitter.Create(this, VSPLITTER | FIXED0 | VIEW3D ))
		return FALSE;


	if(!m_wndBiSplitter.CreatePane(0, THIN_BORDER | SMCAPTION | THIN_CAPTIONBORDER | FLAT_BUTTON
			,_T("����"), m_wndBiSplitter.CreateView(RUNTIME_CLASS(CFileTreeView), 11, pContext) )
		||
		!m_wndBiSplitter.CreatePane(1, FLAT_BUTTON, NULL
			, m_wndBiSplitter.CreateView(RUNTIME_CLASS(CBiSplitterWnd), 12, HSPLITTER|FIXED0|VIEW3D)))
	{
		m_wndBiSplitter.DestroyWindow();
		return FALSE;
	}
	
	m_wndBiSplitter.SetSplitterPos(200);
	m_wndBiSplitter.SetSplitterGap(6);	

	//���۽� DataView�� ������·� ����
	//
	m_pFileTreeView  = (CFileTreeView *)m_wndBiSplitter.GetPaneView(0);
	m_pSeismicFrame = (CBiSplitterWnd *)m_wndBiSplitter.GetPaneView(1);

	if(!m_pSeismicFrame->CreatePane(0, THIN_BORDER | SMCAPTION | THIN_CAPTIONBORDER | FLAT_BUTTON
			,_T("�׷���"), m_pSeismicFrame->CreateView(RUNTIME_CLASS(CSeismicSurveyView), 111, pContext) )
		||
		!m_pSeismicFrame->CreatePane(1, THIN_BORDER | SMCAPTION | THIN_CAPTIONBORDER | FLAT_BUTTON
			, _T("����"),  m_pSeismicFrame->CreateView(RUNTIME_CLASS(CScopeView), 112, pContext)))
	{
		m_wndBiSplitter.DestroyWindow();
		return FALSE;
	}
	m_pSeismicSurveyView  = (CSeismicSurveyView *)m_pSeismicFrame->GetPaneView(0);
	m_pScopeView  = (CScopeView *)m_pSeismicFrame->GetPaneView(1);

	m_pSeismicFrame->SetSplitterPos(200);
	m_pSeismicFrame->SetSplitterGap(6);	

	return true;
}
