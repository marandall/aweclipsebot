// TextListDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "TextListDialog.h"


// TextListDialog dialog

IMPLEMENT_DYNAMIC(TextListDialog, CDialog)

TextListDialog::TextListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(TextListDialog::IDD, pParent)
	, v_List(_T(""))
{

}

TextListDialog::~TextListDialog()
{
}

void TextListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LIST, v_List);
}


BEGIN_MESSAGE_MAP(TextListDialog, CDialog)
END_MESSAGE_MAP()


// TextListDialog message handlers
