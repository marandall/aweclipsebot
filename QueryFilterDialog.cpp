// QueryFilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryFilterDialog.h"


// QueryFilterDialog dialog

IMPLEMENT_DYNAMIC(QueryFilterDialog, CDialog)

QueryFilterDialog::QueryFilterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryFilterDialog::IDD, pParent)
{

}

QueryFilterDialog::~QueryFilterDialog()
{
}

void QueryFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACKING, c_TrackingRect);
}


BEGIN_MESSAGE_MAP(QueryFilterDialog, CDialog)
	ON_BN_CLICKED(IDOK, &QueryFilterDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// QueryFilterDialog message handlers

BOOL QueryFilterDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

void QueryFilterDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
