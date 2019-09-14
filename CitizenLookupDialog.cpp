// CitizenLookupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "CitizenLookupDialog.h"
#include <winerror.h>

// CitizenLookupDialog dialog

IMPLEMENT_DYNAMIC(CitizenLookupDialog, CDialog)

CitizenLookupDialog::CitizenLookupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CitizenLookupDialog::IDD, pParent)
	, v_Query(_T(""))
	, v_Format(_T(""))
	, v_ShowFailures(FALSE)
{
}

CitizenLookupDialog::~CitizenLookupDialog()
{
}

void CitizenLookupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_LIST, v_Query);
	DDX_Text(pDX, IDC_EDIT2, v_Format);
	DDX_Check(pDX, IDC_CHECK2, v_ShowFailures);
}


BEGIN_MESSAGE_MAP(CitizenLookupDialog, CDialog)
END_MESSAGE_MAP()


// CitizenLookupDialog message handlers
