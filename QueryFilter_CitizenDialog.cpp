// QueryFilter_CitizenDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryFilter_CitizenDialog.h"
#include "QueryDialog.h"

// QueryFilter_CitizenDialog dialog

IMPLEMENT_DYNAMIC(QueryFilter_CitizenDialog, CDialog)

QueryFilter_CitizenDialog::QueryFilter_CitizenDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryFilter_CitizenDialog::IDD, pParent)
{

}

QueryFilter_CitizenDialog::~QueryFilter_CitizenDialog()
{
}

void QueryFilter_CitizenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NORTH, c_CitizenNumbers);
}


BEGIN_MESSAGE_MAP(QueryFilter_CitizenDialog, CDialog)
	ON_BN_CLICKED(IDC_CONFIRM2, &QueryFilter_CitizenDialog::OnBnClickedConfirm2)
	ON_BN_CLICKED(IDC_UNSELECT, &QueryFilter_CitizenDialog::OnBnClickedUnselect)
END_MESSAGE_MAP()


// QueryFilter_CitizenDialog message handlers

void QueryFilter_CitizenDialog::OnBnClickedConfirm2()
{
	((QueryDialog*)GetParent())->Filter_SelectCitizen( atoi(c_CitizenNumbers.GetText()), true);
}

void QueryFilter_CitizenDialog::OnBnClickedUnselect()
{
	((QueryDialog*)GetParent())->Filter_SelectCitizen( atoi(c_CitizenNumbers.GetText()), false);
}

