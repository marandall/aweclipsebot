// InputLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "InputLineDialog.h"


// InputLineDialog dialog

IMPLEMENT_DYNAMIC(InputLineDialog, CDialog)

InputLineDialog::InputLineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(InputLineDialog::IDD, pParent)
	, v_Text(_T(""))
	, v_about(_T(""))
{

}

InputLineDialog::~InputLineDialog()
{
}

void InputLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, v_Text);
	DDX_Text(pDX, IDC_ABOUT, v_about);
}


BEGIN_MESSAGE_MAP(InputLineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &InputLineDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// InputLineDialog message handlers

BOOL InputLineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(title);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void InputLineDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
