// QueryFilter_StringsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryFilter_StringsDialog.h"
#include "QueryDialog.h"

// QueryFilter_StringsDialog dialog

IMPLEMENT_DYNAMIC(QueryFilter_StringsDialog, CDialog)

QueryFilter_StringsDialog::QueryFilter_StringsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryFilter_StringsDialog::IDD, pParent)
{

}

QueryFilter_StringsDialog::~QueryFilter_StringsDialog()
{
}

void QueryFilter_StringsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH, c_Search);
	DDX_Control(pDX, IDC_MODELS, c_Models);
	DDX_Control(pDX, IDC_DESCRIPTION, c_Descriptions);
	DDX_Control(pDX, IDC_ACTIONS, c_Actions);
}


BEGIN_MESSAGE_MAP(QueryFilter_StringsDialog, CDialog)
	ON_BN_CLICKED(IDC_CONFIRM2, &QueryFilter_StringsDialog::OnBnClickedConfirm2)
	ON_BN_CLICKED(IDC_UNSELECT, &QueryFilter_StringsDialog::OnBnClickedUnselect)
END_MESSAGE_MAP()


// QueryFilter_StringsDialog message handlers

void QueryFilter_StringsDialog::OnBnClickedConfirm2()
{
	((QueryDialog*)GetParent())->Filter_SelectStrings(c_Search.GetText(), 
			TRUE == c_Models.GetCheck(), 
			TRUE == c_Descriptions.GetCheck(), 
			TRUE == c_Actions.GetCheck(), 
			true);
}

void QueryFilter_StringsDialog::OnBnClickedUnselect()
{
	((QueryDialog*)GetParent())->Filter_SelectStrings(c_Search.GetText(), 
			c_Models.GetCheck() == TRUE, 
			c_Descriptions.GetCheck() == TRUE, 
			c_Actions.GetCheck() == TRUE, 
			false);
}