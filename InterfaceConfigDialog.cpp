// InterfaceConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "InterfaceConfigDialog.h"


// InterfaceConfigDialog dialog

IMPLEMENT_DYNAMIC(InterfaceConfigDialog, CDialog)

InterfaceConfigDialog::InterfaceConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(InterfaceConfigDialog::IDD, pParent)
{

}

InterfaceConfigDialog::~InterfaceConfigDialog()
{
}

void InterfaceConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, c_Tabs);
	DDX_Control(pDX, IDC_TRACKING, c_TrackingRect);
}


BEGIN_MESSAGE_MAP(InterfaceConfigDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &InterfaceConfigDialog::OnTabs_ChangeSel)
	ON_BN_CLICKED(IDOK, &InterfaceConfigDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// InterfaceConfigDialog message handlers
void buildTargetList(ComboBox& C)
{
	C.SetItemData(C.AddString("None / Disabled"),	0);
	C.SetItemData(C.AddString("Main"),				OT_MAIN);
	C.SetItemData(C.AddString("Technical"),			OT_TECH);
	C.SetItemData(C.AddString("Main + Technical"),	OT_MAIN + OT_TECH);
	C.SetItemData(C.AddString("Security"),			OT_SECURITY);
	C.SetItemData(C.AddString("Debug"),				OT_DEBUG);
}

BOOL InterfaceConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create the tracker on screen
	c_PsHelper.create_control(*this, c_TrackingRect);
	c_PsHelper.tabs	= &c_Tabs;

	enum { 
		TABID_CHAT,
		TABID_LOGGING,
		TABID_PROPERTY_UPDATES,
		TABID_WORLD_LIST_UPDATES
	};

	// update items
	c_PsHelper.create_group(TABID_CHAT, "Chat Options",	m_dsChatDisplay.assign_remote(g_InterfaceConfig.getChatDisplay()));
	c_PsHelper.create_group(TABID_LOGGING, "Logging Options", m_dsLogging.assign_remote(g_InterfaceConfig.getLogging()));

	// update
	c_PsHelper.attributes_to_dialog();
	c_PsHelper.window->show_group(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void InterfaceConfigDialog::OnOK()
{
	// update
	c_PsHelper.dialog_to_attributes();
	CDialog::OnOK();
}

void InterfaceConfigDialog::OnTabs_ChangeSel(NMHDR *pNMHDR, LRESULT *pResult)
{
	c_PsHelper.window->show_group(c_Tabs.GetCurSel());
	*pResult = 0;
}

void InterfaceConfigDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
