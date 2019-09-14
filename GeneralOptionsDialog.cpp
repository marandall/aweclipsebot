// GeneralOptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "GeneralOptionsDialog.h"
#include "CommonFeatures.h"


// GeneralOptionsDialog dialog

IMPLEMENT_DYNAMIC(GeneralOptionsDialog, CDialog)

GeneralOptionsDialog::GeneralOptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GeneralOptionsDialog::IDD, pParent)
{
}

GeneralOptionsDialog::~GeneralOptionsDialog()
{
	delete c_pProperties;
}

void GeneralOptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACKING, c_TrackingRect);
	DDX_Control(pDX, IDC_TAB1, c_Tabs);
}


BEGIN_MESSAGE_MAP(GeneralOptionsDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &GeneralOptionsDialog::OnTabs_Switch)
END_MESSAGE_MAP()


// GeneralOptionsDialog message handlers

BOOL GeneralOptionsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create control
	c_pProperties = new PropertyScriptCtrl(this);
	c_pProperties->Create(PropertyScriptCtrl::IDD, this);
	c_pProperties->ShowWindow(SW_SHOW);

	RECT rt;
	c_TrackingRect.GetWindowRect(&rt);
	ScreenToClient(&rt);
	c_pProperties->MoveWindow(&rt);

	// create groups
	c_pProperties->create_group(GOG_GREETINGS,			"Greetings");
	c_pProperties->create_group(GOG_AUTO_RESPONDING,	"Automatic Responding");

	// create tabs
	c_Tabs.InsertItem(GOG_GREETINGS,					_T("Greetings"));
	c_Tabs.InsertItem(GOG_AUTO_RESPONDING,		_T("Automatic Responding"));

	// create the objects
	m_dsGreetings.		create_objects(*c_pProperties, GOG_GREETINGS);
	m_dsAutoResponding.	create_objects(*c_pProperties, GOG_AUTO_RESPONDING);

	// copy attributes
	m_dsGreetings.		attributes_to_dialog(CommonFeatures::g_Greetings.getDataRef());
	m_dsAutoResponding. attributes_to_dialog(CommonFeatures::g_AutoResponding.getDataRef());

	// default group
	c_pProperties->show_group(GOG_GREETINGS);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void GeneralOptionsDialog::OnTabs_Switch(NMHDR *pNMHDR, LRESULT *pResult)
{
	c_pProperties->show_group(c_Tabs.GetCurSel());
	*pResult = 0;
}

void GeneralOptionsDialog::OnOK()
{
	m_dsGreetings.		dialog_to_attributes(CommonFeatures::g_Greetings.		getDataRef());
	m_dsAutoResponding. dialog_to_attributes(CommonFeatures::g_AutoResponding.	getDataRef());

	CDialog::OnOK();
}
