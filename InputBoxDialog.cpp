// InputBoxDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "InputBoxDialog.h"


// InputBoxDialog dialog

IMPLEMENT_DYNAMIC(InputBoxDialog, CDialog)

InputBoxDialog::InputBoxDialog(CWnd* pParent /*=NULL*/)
	: CDialog(InputBoxDialog::IDD, pParent)
	, v_String(_T(""))
{

}

InputBoxDialog::~InputBoxDialog()
{
}

void InputBoxDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT, c_Box);
	DDX_Text(pDX, IDC_INPUT, v_String);
}


BEGIN_MESSAGE_MAP(InputBoxDialog, CDialog)
	ON_BN_CLICKED(IDOK, &InputBoxDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// InputBoxDialog message handlers

void InputBoxDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void InputBoxDialog::PreInitDialog()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PreInitDialog();
}

BOOL InputBoxDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetWindowText(v_Title);
	c_Box.SetFocus();
	return FALSE;
}

LRESULT InputBoxDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDialog::WindowProc(message, wParam, lParam);
}

void InputBoxDialog::OnOK()
{
	CDialog::OnOK();
}
