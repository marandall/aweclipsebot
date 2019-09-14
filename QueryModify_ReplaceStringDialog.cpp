// QueryModify_ReplaceStringDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryDialog.h"
#include "QueryModify_ReplaceStringDialog.h"
#include "property_method_controls.h"

// QueryModify_ReplaceStringDialog dialog

IMPLEMENT_DYNAMIC(QueryModify_ReplaceStringDialog, CDialog)

QueryModify_ReplaceStringDialog::QueryModify_ReplaceStringDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryModify_ReplaceStringDialog::IDD, pParent)
{

}

QueryModify_ReplaceStringDialog::~QueryModify_ReplaceStringDialog()
{
}

void QueryModify_ReplaceStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH, c_Find);
	DDX_Control(pDX, IDC_REPLACE_WITH, c_Replace);
	DDX_Control(pDX, IDC_MODELS, c_Models);
	DDX_Control(pDX, IDC_DESCRIPTION, c_Descriptions);
	DDX_Control(pDX, IDC_ACTIONS, c_Actions);
	DDX_Control(pDX, IDC_MATCH_CASE, c_MatchCase);
}


BEGIN_MESSAGE_MAP(QueryModify_ReplaceStringDialog, CDialog)
	ON_BN_CLICKED(IDC_REPLACE_SELECTED, &QueryModify_ReplaceStringDialog::OnBnClickedReplaceSelected)
END_MESSAGE_MAP()


// QueryModify_ReplaceStringDialog message handlers

void QueryModify_ReplaceStringDialog::OnBnClickedReplaceSelected()
{
	Property::MethodControls::ReplaceString options;
	options.selected_only	= true;
	options.model			= c_Models.GetCheck() == TRUE;
	options.description		= c_Descriptions.GetCheck() == TRUE;
	options.action			= c_Actions.GetCheck() == TRUE;
	options.find			= c_Find.GetText();
	options.replace			= c_Replace.GetText();
	options.case_sensitive	= c_MatchCase.GetCheck() == TRUE;
	((QueryDialog*)GetParent())->Modify_ReplaceString(options);
}
