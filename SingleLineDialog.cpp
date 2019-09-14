// SingleLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "SingleLineDialog.h"

// SingleLineDialog dialog

IMPLEMENT_DYNAMIC(SingleLineDialog, CDialog)

SingleLineDialog::SingleLineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SingleLineDialog::IDD, pParent)
	, v_text(_T(""))
{

}

SingleLineDialog::~SingleLineDialog()
{
}

void SingleLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ABOUT, c_about2);
	DDX_Control(pDX, IDC_EDIT1, c_input);
	DDX_Text(pDX, IDC_EDIT1, v_text);
}


BEGIN_MESSAGE_MAP(SingleLineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &SingleLineDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SingleLineDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// SingleLineDialog message handlers

void SingleLineDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void SingleLineDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL SingleLineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set the items
	SetWindowText(v_title);
	c_about2.SetWindowTextA(v_about);
	c_input.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

std::string retstr;
const char* SingleLineDialog::GetInputLine(CWnd* Wnd, const char* Question, const char* Title, const char* Text)
{
	SingleLineDialog d(Wnd);
	d.v_title = Title		? Title	   : "Input";
	d.v_about = Question	? Question : "ERR; no question given";
	d.v_text  = Text		? Text	   : "";

	// push, is it ok
	if (d.DoModal() == IDOK)
	{
		retstr = d.v_text;
		return retstr.c_str();
	}
	else
	{
		return 0;
	}
}