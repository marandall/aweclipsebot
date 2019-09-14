// SecurityHeaderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "SecurityHeaderDialog.h"


// SecurityHeaderDialog dialog

IMPLEMENT_DYNAMIC(SecurityHeaderDialog, CDialog)

SecurityHeaderDialog::SecurityHeaderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SecurityHeaderDialog::IDD, pParent)
	, v_identifier(_T(""))
	, v_name(_T(""))
	, v_description(_T(""))
{

}

SecurityHeaderDialog::~SecurityHeaderDialog()
{
}

void SecurityHeaderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, v_identifier);
	DDX_Text(pDX, IDC_EDIT3, v_name);
	DDX_Text(pDX, IDC_EDIT4, v_description);
}


BEGIN_MESSAGE_MAP(SecurityHeaderDialog, CDialog)
END_MESSAGE_MAP()


// SecurityHeaderDialog message handlers
