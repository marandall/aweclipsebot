// ommandConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "CommandConfigDialog.h"
#include "GlobalDataExchange.h"

// CommandConfigDialog dialog

IMPLEMENT_DYNAMIC(CommandConfigDialog, CDialog)

CommandConfigDialog::CommandConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CommandConfigDialog::IDD, pParent)
{

}

enum {
	CISG_INTERFACE,
	CISG_JUMP_POINTS,
	CISG_EJECTION
};

CommandConfigDialog::~CommandConfigDialog()
{
}

void CommandConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, c_Tabs);
	DDX_Control(pDX, IDC_TRACKING, c_RectTracker);
}

BEGIN_MESSAGE_MAP(CommandConfigDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CommandConfigDialog::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CommandConfigDialog::OnTabs_Change)
	ON_BN_CLICKED(IDAPPLY, &CommandConfigDialog::OnBnClickedApply)
END_MESSAGE_MAP()


void CommandConfigDialog::OnBnClickedOk()
{
	OnOK();
}

void CommandConfigDialog::OnOK()
{
	c_PsHelper.dialog_to_attributes();
	CDialog::OnOK();
}

BOOL CommandConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create the tracker on screen
	c_PsHelper.create_control(*this, c_RectTracker);
	c_PsHelper.tabs	= &c_Tabs;

	// create the groups
	c_PsHelper.create_group(CISG_INTERFACE,		"Interface",	m_dsInterface.assign_remote(CIS::Config::g_Interface));
	c_PsHelper.create_group(CISG_EJECTION,		"Ejections",	m_dsEjection.assign_remote(CIS::Config::g_Ejection));

	// load and display
	c_PsHelper.attributes_to_dialog();

	// show interface
	c_PsHelper.window->show_group(CISG_INTERFACE);
	return TRUE;
}

void CommandConfigDialog::OnTabs_Change(NMHDR *pNMHDR, LRESULT *pResult)
{
	c_PsHelper.window->show_group(c_Tabs.GetCurSel());
	*pResult = 0;
}

void CommandConfigDialog::OnBnClickedApply()
{
	c_PsHelper.dialog_to_attributes();
}
