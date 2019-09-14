// QueryAction_DeleteDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryAction_DeleteDialog.h"
#include "QueryDialog.h"

// QueryAction_DeleteDialog dialog

IMPLEMENT_DYNAMIC(QueryAction_DeleteDialog, CDialog)

QueryAction_DeleteDialog::QueryAction_DeleteDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryAction_DeleteDialog::IDD, pParent)
{

}

QueryAction_DeleteDialog::~QueryAction_DeleteDialog()
{
}

void QueryAction_DeleteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REMAINING,			c_ObjectsRemaining);
	DDX_Control(pDX, IDC_SUCCESS,			c_Successful);
	DDX_Control(pDX, IDC_FAILURE,			c_Failed);
	DDX_Control(pDX, IDC_BUILD_PROGRESS,	c_Progress);
	DDX_Control(pDX, IDC_STATUS,			c_Status);
	DDX_Control(pDX, IDC_PAUSE,				c_Paused);
	DDX_Control(pDX, IDC_TS, c_TrackSuccess);
	DDX_Control(pDX, IDC_TF, c_TrackFailed);
}


BEGIN_MESSAGE_MAP(QueryAction_DeleteDialog, CDialog)
	ON_BN_CLICKED(IDC_BUILD, &QueryAction_DeleteDialog::OnBnClickedBuild)
END_MESSAGE_MAP()


// QueryAction_DeleteDialog message handlers

void QueryAction_DeleteDialog::OnBnClickedBuild()
{
	((QueryDialog*)GetParent())->SuperMethod_Delete(
			c_Paused.GetCheck() == TRUE,
			c_TrackSuccess.GetCheck() == TRUE,
			c_TrackFailed.GetCheck() == TRUE);
}

void QueryAction_DeleteDialog::OnBnClickedCheck4()
{
	// TODO: Add your control notification handler code here
}
