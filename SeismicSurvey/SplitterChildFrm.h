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

// ChildFrm.h : CSplitterChildFrame Ŭ������ �������̽�
//

#pragma once

#include "BiSplitterWnd\BiSplitterWnd.h"
#include "FileTreeView.h"
#include "SeismicSurveyView.h"

class CFileTreeView;
class CSeismicSurveyView;
class CScopeView;

class CSplitterChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CSplitterChildFrame)
public:
	CSplitterChildFrame();

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
//	CSplitterWnd m_wndSplitter;
	CBiSplitterWnd m_wndBiSplitter;
	CFileTreeView * m_pFileTreeView;

	CBiSplitterWnd * m_pSeismicFrame;
	CSeismicSurveyView * m_pSeismicSurveyView;
	CScopeView * m_pScopeView;


// �������Դϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CSplitterChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};
