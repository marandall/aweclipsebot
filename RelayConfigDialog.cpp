// RelayConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "RelayConfigDialog.h"
#include "RelayRelay.h"
#include "RelayConfigUsersDialog.h"

// RelayConfigDialog dialog

enum RELAY_DIALOG_PAGES
{
	RELAY_PAGE_GENERAL,
	RELAY_PAGE_TOTAL,
	RELAY_PAGE_GLOBAL
};

IMPLEMENT_DYNAMIC(RelayConfigDialog, CDialog)

RelayConfigDialog::RelayConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RelayConfigDialog::IDD, pParent)
{

}

RelayConfigDialog::~RelayConfigDialog()
{
}

void RelayConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACKING, c_RectTracker);
	DDX_Control(pDX, IDC_TAB1, c_Tabs);
}


BEGIN_MESSAGE_MAP(RelayConfigDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &RelayConfigDialog::OnTabs_SelChanged)
	ON_BN_CLICKED(IDOK, &RelayConfigDialog::OnBnClickedOk)
	ON_BN_CLICKED(ID_GLOBAL_STYLUS, &RelayConfigDialog::OnBnClickedGlobalStylus)
END_MESSAGE_MAP()


// RelayConfigDialog message handlers

using namespace ChatControllers;

BOOL RelayConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create the tracker on screen
	c_PsHelper.create_control(*this, c_RectTracker);
	c_PsHelper.tabs	= &c_Tabs;

	// setup the pages
	c_PsHelper.create_group(RELAY_PAGE_GENERAL,  "General",		m_dsRelay.assign_remote(g_Relay));
	c_PsHelper.create_group(RELAY_PAGE_TOTAL,		 "Advanced Relay",	m_dsTotal.assign_remote(g_Relay.getAdvancedRelay()));
	c_PsHelper.create_group(RELAY_PAGE_GLOBAL,	 "Global Isolation",	m_dsGlobalChat.assign_remote(g_Relay.getGlobalChat()));
	
	// setup the listbox options
	m_dsRelay.pRelayMode->InsertByMap(g_Relay.GetMapMode2String());

	// add the relay modes
	for (int i = 0; i < AdvancedRelayKernel::WORLD_MODE_MAX; i++)
		m_dsTotal.pDefaultChannelMode->InsertString(i, AdvancedRelayKernel::GetWorldModeString(i));

	// select general information
	c_PsHelper.attributes_to_dialog();

	// select first item
	c_PsHelper.window->show_group(RELAY_PAGE_GENERAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void RelayConfigDialog::OnOK()
{
	// save information
	c_PsHelper.dialog_to_attributes();
	CDialog::OnOK();
}

void RelayConfigDialog::OnTabs_SelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	c_PsHelper.window->show_group(c_Tabs.GetCurSel());
	*pResult = 0;
}

void RelayConfigDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void RelayConfigDialog::OnBnClickedGlobalStylus()
{
}
