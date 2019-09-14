// UniverseCitizenAddDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "UniverseCitizenAddDialog.h"


// UniverseCitizenAddDialog dialog

IMPLEMENT_DYNAMIC(UniverseCitizenAddDialog, CDialog)

UniverseCitizenAddDialog::UniverseCitizenAddDialog(CWnd* pParent /*=NULL*/)
	: CDialog(UniverseCitizenAddDialog::IDD, pParent)
	, v_UserName(_T(""))
	, v_EMail(_T(""))
	, v_Password(_T(""))
{

}

UniverseCitizenAddDialog::~UniverseCitizenAddDialog()
{
}

void UniverseCitizenAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USER_NAME, v_UserName);
	DDX_Text(pDX, IDC_EMAIL, v_EMail);
	DDX_Text(pDX, IDC_PASSWORD, v_Password);
	DDX_Control(pDX, IDC_USER_NAME, c_User);
}


BEGIN_MESSAGE_MAP(UniverseCitizenAddDialog, CDialog)
END_MESSAGE_MAP()


// UniverseCitizenAddDialog message handlers

BOOL UniverseCitizenAddDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	c_User.SetFocus();
	return FALSE;
}
