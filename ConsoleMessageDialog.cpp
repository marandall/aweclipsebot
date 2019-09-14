// onsoleMessageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ConsoleMessageDialog.h"


// ConsoleMessageDialog dialog

IMPLEMENT_DYNAMIC(ConsoleMessageDialog, CDialog)

ConsoleMessageDialog::ConsoleMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ConsoleMessageDialog::IDD, pParent)
	, v_Prefix(_T(""))
	, v_Message(_T(""))
	, v_Bold(FALSE)
	, v_Italic(FALSE)
{

}

ConsoleMessageDialog::~ConsoleMessageDialog()
{
}

void ConsoleMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PREFIX, v_Prefix);
	DDX_Text(pDX, IDC_MESSAGE, v_Message);
	DDX_Control(pDX, IDC_BOLD, c_Bold);
	DDX_Control(pDX, IDC_ITALICS, c_Italics);
	DDX_Control(pDX, IDC_COLOUR, c_Colour);
	DDX_Check(pDX, IDC_BOLD, v_Bold);
	DDX_Check(pDX, IDC_ITALICS, v_Italic);
}


BEGIN_MESSAGE_MAP(ConsoleMessageDialog, CDialog)
END_MESSAGE_MAP()


// ConsoleMessageDialog message handlers
