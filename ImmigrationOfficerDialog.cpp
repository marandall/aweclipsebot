// ImmigrationOfficerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ImmigrationOfficerDialog.h"


// ImmigrationOfficerDialog dialog

IMPLEMENT_DYNAMIC(ImmigrationOfficerDialog, CDialog)

ImmigrationOfficerDialog::ImmigrationOfficerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ImmigrationOfficerDialog::IDD, pParent)
	, v_Message(_T(""))
{

}

ImmigrationOfficerDialog::~ImmigrationOfficerDialog()
{
}

void ImmigrationOfficerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTORY, c_History);
	DDX_Text(pDX, IDC_NEW_MESSAGE, v_Message);
	DDX_Control(pDX, IDC_NEW_MESSAGE, c_Message);
}


BEGIN_MESSAGE_MAP(ImmigrationOfficerDialog, CDialog)
END_MESSAGE_MAP()


// ImmigrationOfficerDialog message handlers

BOOL ImmigrationOfficerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	c_Message.SetLimitText(AW_MAX_ATTRIBUTE_LENGTH);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
