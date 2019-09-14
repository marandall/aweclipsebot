// ZonesConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ZonesConfigDialog.h"
#include "Zones_ConfigGeneral.h"

// ZonesConfigDialog dialog

IMPLEMENT_DYNAMIC(ZonesConfigDialog, CDialog)

ZonesConfigDialog::ZonesConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ZonesConfigDialog::IDD, pParent)
{

}

ZonesConfigDialog::~ZonesConfigDialog()
{
}

void ZonesConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, c_Tabs);
	DDX_Control(pDX, IDC_TRACKING, c_RectTracker);
}


BEGIN_MESSAGE_MAP(ZonesConfigDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ZonesConfigDialog::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &ZonesConfigDialog::OnTabs_Changed)
END_MESSAGE_MAP()


// ZonesConfigDialog message handlers
enum PAGES_ENUM
{
	PAGE_ID_GENERAL,
	PAGE_ID_HYPERION
};

BOOL ZonesConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// create the tracker on screen
	c_PsHelper.create_control(*this, c_RectTracker);
	c_PsHelper.tabs	= &c_Tabs;

	// add the objects
	c_PsHelper.create_group(PAGE_ID_GENERAL,	"General",	m_dsGeneral.assign_remote(Zones::g_Config));
	c_PsHelper.create_group(PAGE_ID_HYPERION,	"Hyperion",	m_dsHyperion.assign_remote(Zones::g_Config.getHyperionKernel()));
	c_PsHelper.attributes_to_dialog();
	c_PsHelper.window->show_group(PAGE_ID_GENERAL);

	return TRUE;
}

void ZonesConfigDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void ZonesConfigDialog::OnOK()
{
	c_PsHelper.dialog_to_attributes();
	__super::OnOK();
}
void ZonesConfigDialog::OnTabs_Changed(NMHDR *pNMHDR, LRESULT *pResult)
{
	c_PsHelper.window->show_group(c_Tabs.GetCurSel());
	*pResult = 0;
}
