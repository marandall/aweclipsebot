// TechLogDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "TechLogDialog.h"


// TechLogDialog dialog

IMPLEMENT_DYNAMIC(TechLogDialog, CDialog)

TechLogDialog::TechLogDialog(CWnd* pParent /*=NULL*/)
	: CDialog(TechLogDialog::IDD, pParent)
{

}

TechLogDialog::~TechLogDialog()
{
}

void TechLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TechLogDialog, CDialog)
END_MESSAGE_MAP()


// TechLogDialog message handlers
