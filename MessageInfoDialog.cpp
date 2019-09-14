// MessageInfoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "MessageInfoDialog.h"
#include "messages.h"
#include "GlobalGui.h"

// MessageInfoDialog dialog

IMPLEMENT_DYNAMIC(MessageInfoDialog, CDialog)

MessageInfoDialog::MessageInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MessageInfoDialog::IDD, pParent)
	, v_Message(_T(""))
	, v_Style(0)
	, v_Bold(FALSE)
	, v_Italic(FALSE)
	, v_Suppress(FALSE)
{

}

MessageInfoDialog::~MessageInfoDialog()
{
}

void MessageInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WM_MESSAGE, v_Message);
	DDX_CBIndex(pDX, IDC_WM_STYLE, v_Style);
	DDX_Control(pDX, IDC_WM_COLOUR, c_Colour);
	DDX_Check(pDX, IDC_WM_BOLD, v_Bold);
	DDX_Check(pDX, IDC_WM_ITALIC, v_Italic);
	DDX_Control(pDX, IDC_WM_STYLE, c_Style);
	DDX_Check(pDX, IDC_SUPPRESS, v_Suppress);
}


BEGIN_MESSAGE_MAP(MessageInfoDialog, CDialog)
	ON_BN_CLICKED(IDOK, &MessageInfoDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// MessageInfoDialog message handlers

BOOL MessageInfoDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	int id = 0;

	c_Style.InsertString(id++, "Say");
	c_Style.InsertString(id++, "Whisper");
	c_Style.InsertString(id++, "Console");
	c_Style.InsertString(id++, "Plain Console");
	c_Style.InsertString(id++, "Broadcast");
	c_Style.SetCurSel(v_Style);

	RESIZE_LISTBOX(c_Style);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MessageInfoDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
