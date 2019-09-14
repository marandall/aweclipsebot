// EjectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "EjectDialog.h"


// EjectDialog dialog

IMPLEMENT_DYNAMIC(EjectDialog, CDialog)

EjectDialog::EjectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EjectDialog::IDD, pParent)
	, v_Minutes(_T(""))
	, v_Hours(_T(""))
	, v_Days(_T(""))
	, v_Log(_T(""))
	, v_UserMessage(_T(""))
	, v_name(_T(""))
	, v_dns(_T(""))
{

}

EjectDialog::~EjectDialog()
{
}

void EjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MIN, v_Minutes);
	DDX_Text(pDX, IDC_HOURS, v_Hours);
	DDX_Text(pDX, IDC_DAYS, v_Days);
	DDX_Text(pDX, IDC_LOG_REASON, v_Log);
	DDX_Text(pDX, IDC_REASON, v_UserMessage);
	DDX_Control(pDX, IDC_MIN, c_Minutes);
	DDX_Text(pDX, IDC_NAME, v_name);
	DDX_Text(pDX, IDC_DNS, v_dns);
}


BEGIN_MESSAGE_MAP(EjectDialog, CDialog)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// EjectDialog message handlers

void EjectDialog::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	// delete this;
}

BOOL EjectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_TitleMsg);
	v_Minutes = "5";

	c_Minutes.SetFocus();
	return FALSE;
}
