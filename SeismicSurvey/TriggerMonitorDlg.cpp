// TriggerMonitorDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "TriggerMonitorDlg.h"
#include "afxdialogex.h"

#include "SeismicSonde.h"

// CTriggerMonitorDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTriggerMonitorDlg, CDialogEx)

CTriggerMonitorDlg::CTriggerMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTriggerMonitorDlg::IDD, pParent)
{
	m_pSonde = NULL;
	m_bResult = false;
	m_Count = 0;
	m_DataNo = 0;
	m_BytesReadCompleted = false;
	CString temp = _T("CMPLT ");
	memcpy(m_CompletedMessage, temp.GetBuffer(), temp.GetLength() );

	m_MaxWaitTimeInSec = 60;
}

CTriggerMonitorDlg::~CTriggerMonitorDlg()
{
}

void CTriggerMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTriggerMonitorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TRIGGERSTOP_BUTTON, &CTriggerMonitorDlg::OnBnClickedTriggerstopButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTriggerMonitorDlg �޽��� ó�����Դϴ�.

BOOL CTriggerMonitorDlg::CreateModaless(CSeismicSonde * pSonde)
{
	BOOL res = CDialog::Create(IDD_TRIGGERMONITORDLG, NULL);

	m_pSonde = pSonde;


	return res;
}

void CTriggerMonitorDlg::OnBnClickedTriggerstopButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pSonde->m_bCmdTriggerTerminated = true;
}



BOOL CTriggerMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if(m_pSonde)
	{
		SetTimer(111, 50, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTriggerMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch(nIDEvent)
	{
	case 111:
		CheckCompleteMessage();
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}

void CTriggerMonitorDlg::CheckCompleteMessage()
{
	int BytesRead;

	if(m_pSonde->m_bCmdTriggerTerminated)
	{
		m_bResult = false;
		OnCancel();
	}
	else
	{
		BytesRead = m_pSonde->DirectReadFromPort(&m_TempPacket[m_DataNo]);
		m_DataNo += BytesRead;
		m_Count++;
		if(m_DataNo>6)
		{
			m_TempPacket[m_DataNo] = 0;
			if(m_CompletedMessage[0] == m_TempPacket[1] && m_CompletedMessage[1] == m_TempPacket[2])
			{
				m_bResult = true;
				OnOK();
			}
		}
		if(m_Count>m_MaxWaitTimeInSec * 10)
		{
			CString tempmessage;
			tempmessage.Format("Trigger is failed!:%s", &m_TempPacket[1]);
	//			AfxMessageBox("Trigger is failed! Please retry...");
			AfxMessageBox(tempmessage);
			m_bResult = false;
			OnCancel();
		}
	}
}

void CTriggerMonitorDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_bResult = true;
	KillTimer(111);

	CDialogEx::OnOK();
}


void CTriggerMonitorDlg::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_bResult = false;
	KillTimer(111);

	CDialogEx::OnCancel();
}
