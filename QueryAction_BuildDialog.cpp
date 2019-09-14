// QueryAction_BuildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryAction_BuildDialog.h"
#include "QueryDialog.h"
#include "PropertyProject.h"

// QueryAction_BuildDialog dialog

IMPLEMENT_DYNAMIC(QueryAction_BuildDialog, CDialog)

QueryAction_BuildDialog::QueryAction_BuildDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryAction_BuildDialog::IDD, pParent)
{

}

QueryAction_BuildDialog::~QueryAction_BuildDialog()
{
}

void QueryAction_BuildDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUCCESSFUL, c_SuccessPath);
	DDX_Control(pDX, IDC_FAILED, c_FailurePath);
	DDX_Control(pDX, IDC_REMAINING, c_ObjectsRemaining);
	DDX_Control(pDX, IDC_SUCCESS, c_Successful);
	DDX_Control(pDX, IDC_FAILURE, c_Failed);
	DDX_Control(pDX, IDC_BUILD_PROGRESS, c_Progress);
	DDX_Control(pDX, IDC_STATUS, c_Status);
	DDX_Control(pDX, IDC_PAUSE, c_Paused);
	DDX_Control(pDX, IDC_MAINTAIN_CIT, c_MaintainED);
}


BEGIN_MESSAGE_MAP(QueryAction_BuildDialog, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PAUSE, &QueryAction_BuildDialog::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUILD, &QueryAction_BuildDialog::OnBnClickedBuild)
END_MESSAGE_MAP()


// QueryAction_BuildDialog message handlers

void QueryAction_BuildDialog::OnTimer(UINT_PTR nIDEvent)
{
	// update the project settings
	if (m_pProject->IsBuilding())
	{
		c_ObjectsRemaining.SetWindowText( ToString((int)m_pProject->objects.size() - m_pProject->objects.limbo_count) );
		c_Successful.SetWindowText( ToString(m_pProject->job_build_success) );
		c_Failed.SetWindowTextA(    ToString(m_pProject->job_build_failed)  );
		
		// set the status for this window
		if (m_pProject->objects.size() == 0)
			c_Status.SetWindowTextA ("Complete");
		else if (m_pProject->job_paused)
			c_Status.SetWindowTextA ("Paused");
		else
			c_Status.SetWindowTextA ("Running");

		// set the paused status
		c_Paused.SetCheck(m_pProject->job_paused ? TRUE : FALSE);
	}
	else
	{
		c_Status.SetWindowTextA("Standby");
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL QueryAction_BuildDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set update timer
	SetTimer(1, 500, NULL);
	return TRUE;
}

void QueryAction_BuildDialog::OnBnClickedPause()
{
	m_pProject->job_paused = (c_Paused.GetCheck() == TRUE);
}

void QueryAction_BuildDialog::OnBnClickedBuild()
{
	((QueryDialog*)GetParent())->SuperMethod_Build(c_Paused.GetCheck() == TRUE, 
		c_MaintainED.GetCheck() == TRUE);
}