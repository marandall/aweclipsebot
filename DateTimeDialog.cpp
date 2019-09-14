// DateTimeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "DateTimeDialog.h"


// DateTimeDialog dialog

IMPLEMENT_DYNAMIC(DateTimeDialog, CDialog)

DateTimeDialog::DateTimeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DateTimeDialog::IDD, pParent)
{

}

DateTimeDialog::~DateTimeDialog()
{
}

void DateTimeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE, c_Date);
	DDX_Control(pDX, IDC_TIME, c_Time);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, c_month);
}


BEGIN_MESSAGE_MAP(DateTimeDialog, CDialog)
	ON_BN_CLICKED(IDOK, &DateTimeDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEVER, &DateTimeDialog::OnBnClickedNever)
END_MESSAGE_MAP()


// DateTimeDialog message handlers

void DateTimeDialog::OnBnClickedOk()
{
	OnOK();
}

BOOL DateTimeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	c_Date.SetTime(&timestamp);
	c_Time.SetTime(&timestamp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DateTimeDialog::OnBnClickedNever()
{
	timestamp = 0;
	EndDialog(IDOK);
}
