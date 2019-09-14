// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ProgressDialog.h"


// ProgressDialog dialog

IMPLEMENT_DYNAMIC(ProgressDialog, CDialog)

ProgressDialog::ProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressDialog::IDD, pParent)
{

}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ProgressDialog, CDialog)
END_MESSAGE_MAP()


// ProgressDialog message handlers
