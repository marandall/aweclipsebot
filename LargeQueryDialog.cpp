// LargeQueryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "LargeQueryDialog.h"


// LargeQueryDialog dialog

IMPLEMENT_DYNAMIC(LargeQueryDialog, CDialog)

LargeQueryDialog::LargeQueryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(LargeQueryDialog::IDD, pParent)
	, v_top(_T(""))
	, v_bottom(_T(""))
	, v_aroundCenter(_T(""))
	, v_aroundWidth(0)
	, v_aroundHeight(0)
{

}

LargeQueryDialog::~LargeQueryDialog()
{
}

void LargeQueryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TOP, v_top);
	DDX_Text(pDX, IDC_BOTTOM, v_bottom);
	DDX_Text(pDX, IDC_TOP3, v_aroundCenter);
	DDX_Text(pDX, IDC_BOTTOM3, v_aroundWidth);
	DDX_Text(pDX, IDC_BOTTOM4, v_aroundHeight);
}


BEGIN_MESSAGE_MAP(LargeQueryDialog, CDialog)
END_MESSAGE_MAP()


// LargeQueryDialog message handlers
