// JumpEditorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "JumpEditorDialog.h"


// JumpEditorDialog dialog

IMPLEMENT_DYNAMIC(JumpEditorDialog, CDialog)

JumpEditorDialog::JumpEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(JumpEditorDialog::IDD, pParent)
	, v_name(_T(""))
	, v_coordinates(_T(""))
	, v_owner(0)
{

}

JumpEditorDialog::~JumpEditorDialog()
{
}

void JumpEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, v_name);
	DDX_Text(pDX, IDC_EDIT3, v_coordinates);
	DDX_Text(pDX, IDC_EDIT4, v_owner);
	DDV_MinMaxInt(pDX, v_owner, 0, 1000000);
}


BEGIN_MESSAGE_MAP(JumpEditorDialog, CDialog)
END_MESSAGE_MAP()


// JumpEditorDialog message handlers
