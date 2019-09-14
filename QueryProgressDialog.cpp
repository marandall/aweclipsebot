// QueryProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryProgressDialog.h"


// QueryProgressDialog dialog

IMPLEMENT_DYNAMIC(QueryProgressDialog, CDialog)

QueryProgressDialog::QueryProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryProgressDialog::IDD, pParent)
{

}

QueryProgressDialog::~QueryProgressDialog()
{
}

void QueryProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(QueryProgressDialog, CDialog)
END_MESSAGE_MAP()


// QueryProgressDialog message handlers
