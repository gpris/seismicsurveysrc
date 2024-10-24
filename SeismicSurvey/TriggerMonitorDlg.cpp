// TriggerMonitorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SeismicSurvey.h"
#include "TriggerMonitorDlg.h"
#include "afxdialogex.h"

#include "SeismicSonde.h"

// CTriggerMonitorDlg 대화 상자입니다.

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


// CTriggerMonitorDlg 메시지 처리기입니다.

BOOL CTriggerMonitorDlg::CreateModaless(CSeismicSonde * pSonde)
{
	BOOL res = CDialog::Create(IDD_TRIGGERMONITORDLG, NULL);

	m_pSonde = pSonde;


	return res;
}

void CTriggerMonitorDlg::OnBnClickedTriggerstopButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSonde->m_bCmdTriggerTerminated = true;
}



BOOL CTriggerMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if(m_pSonde)
	{
		SetTimer(111, 50, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTriggerMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_bResult = true;
	KillTimer(111);

	CDialogEx::OnOK();
}


void CTriggerMonitorDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_bResult = false;
	KillTimer(111);

	CDialogEx::OnCancel();
}
