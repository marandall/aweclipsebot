// ProfilesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ProfilesDialog.h"
#include "Profiles.h"
#include "GlobalGui.h"

// ProfilesDialog dialog

IMPLEMENT_DYNAMIC(ProfilesDialog, CDialog)

ProfilesDialog::ProfilesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ProfilesDialog::IDD, pParent)
	, v_Citizen(0)
	, v_Password(_T(""))
	, v_BotName(_T(""))
	, v_World(_T(""))
	, v_Coords(_T(""))
	, v_Global(FALSE)
	, v_Invisible(FALSE)
	, v_Icon(_T(""))
	, v_Universe(_T(""))
{

}

ProfilesDialog::~ProfilesDialog()
{
}

void ProfilesDialog::ShowProfile(Profiles::Profile& ActiveProfile)
{
	v_Citizen		= ActiveProfile.getLoginCitizen();
	v_Password		= ActiveProfile.getLoginPassword();
	v_BotName		= ActiveProfile.getLoginName();
	v_World			= ActiveProfile.getTargetWorld();
	v_Coords		= ActiveProfile.getTargetCoords();
	v_Global		= ConvBool(ActiveProfile.getTargetGlobal());
	v_Invisible		= ConvBool(ActiveProfile.getTargetInvisible());
	UpdateData(FALSE);

	// select correct universe
	c_Universe.SelectString(-1, ActiveProfile.getUniName());
	
	// select correct string
	c_Style.SelectString(-1, ActiveProfile.getIconId());
	m_pCurrentProfile = &ActiveProfile;
}

void ProfilesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CITIZEN, v_Citizen);
	DDX_Text(pDX, IDC_PASS, v_Password);
	DDX_Text(pDX, IDC_NAME, v_BotName);
	DDX_Text(pDX, IDC_WORLD, v_World);
	DDX_Text(pDX, IDC_COORDS, v_Coords);
	DDX_Check(pDX, IDC_GLOBAL, v_Global);
	DDX_Check(pDX, IDC_INVISIBLE, v_Invisible);
	DDX_Control(pDX, IDC_STYLE, c_Style);
	DDX_Control(pDX, IDC_UNIVERSE, c_Universe);
	DDX_CBString(pDX, IDC_STYLE, v_Icon);
	DDX_CBString(pDX, IDC_UNIVERSE, v_Universe);
}


BEGIN_MESSAGE_MAP(ProfilesDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ProfilesDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &ProfilesDialog::OnBnClickedOk2)
END_MESSAGE_MAP()


// ProfilesDialog message handlers

void ProfilesDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL ProfilesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// update list
	m_UseDirectly = false;

	// expand the lists
	RESIZE_LISTBOX(this->c_Style);
	RESIZE_LISTBOX(this->c_Universe);

	// load the list of icons
	for (size_t i = 0; i < Profiles::g_Icons.list.size(); i++)
		c_Style.InsertString(c_Style.GetCount(), Profiles::g_Icons.list[i].name);

	// load the list of universes
	for (size_t i = 0; i < Profiles::g_Universes.size(); i++)
		c_Universe.InsertString(c_Universe.GetCount(), Profiles::g_Universes[i].name);

	ShowProfile(*m_pCurrentProfile);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ProfilesDialog::OnOK()
{
	/* copy attributes ftw */
	UpdateData(TRUE);
	CDialog::OnOK();
}

void ProfilesDialog::OnBnClickedOk2()
{
	m_UseDirectly = true;
	OnOK();
}

