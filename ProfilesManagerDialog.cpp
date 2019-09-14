// ProfilesManagerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ProfilesManagerDialog.h"
#include "profiles.h"
#include "ProfilesDialog.h"

using namespace Profiles;

// ProfilesManagerDialog dialog

IMPLEMENT_DYNAMIC(ProfilesManagerDialog, CDialog)

ProfilesManagerDialog::ProfilesManagerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ProfilesManagerDialog::IDD, pParent)
{

}

ProfilesManagerDialog::~ProfilesManagerDialog()
{
}

void ProfilesManagerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, c_List);
	DDX_Control(pDX, IDC_PROFID, c_ProfileId);
}


BEGIN_MESSAGE_MAP(ProfilesManagerDialog, CDialog)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &ProfilesManagerDialog::OnList_DoubleClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &ProfilesManagerDialog::OnList_DoubleHit)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST, &ProfilesManagerDialog::OnList_DoubleRightClick)
	ON_BN_CLICKED(IDOK, &ProfilesManagerDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// ProfilesManagerDialog message handlers

BOOL ProfilesManagerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// init list
	c_List.InitIcons();
	// c_List.InsertIcon(GetAppPath() + "\\graphics\\drs\\acc_group.bmp", "");

	// insert cols
	c_List.InsertColumn(0, "Profile",	0, 120);
	c_List.InsertColumn(1, "Universe",	0, 100);
	c_List.InsertColumn(2, "Name",		0, 120);

	// load the list of icons
	for (size_t i = 0; i < g_Icons.list.size(); i++)
	{
		HICON ico = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(g_Icons.list[i].resource_id),
						IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR); 
		c_List.InsertIconByHandle(ico, g_Icons.list[i].name);
	}

	// create list of profiles
	for (size_t i = 0; i < g_Profiles.size(); i++)
	{
		Profile& profile = g_Profiles[i];
		int id = c_List.InsertItem(c_List.GetItemCount(), profile.getId());
		UpdateGrid(id, profile);
	}

	return TRUE; 
}
void ProfilesManagerDialog::UpdateGrid(int id, Profile& profile)
{
	c_List.SetItemData(id, profile.getIndex());
	c_List.EditItem(id, 1, profile.getUniName());
	c_List.EditItem(id, 2, profile.getLoginName());

	// edit the item to include the icon
	c_List.EditItem(id, 0, profile.getId(), c_List.getIconKey(profile.getIconId()));
}

void ProfilesManagerDialog::OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
}

void ProfilesManagerDialog::OnList_DoubleHit(NMHDR *pNMHDR, LRESULT *pResult)
{
	// aquire the variable
	int item = c_List.GetByHitTest();
	if (item == -1)
		return;

	// find the pid
	int pID  = (int)c_List.GetItemData(item);
	::Profiles::Profile* pProfile = ::Profiles::g_Profiles.GetByIndex(pID);
	if (pProfile)
	{
		selected_profile = pProfile->getId();
	}
	OnOK();
}

void ProfilesManagerDialog::OnList_DoubleRightClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// aquire the variable
	int item = c_List.GetByHitTest();
	if (item == -1)
		return;

	// find the pid
	int pID  = (int)c_List.GetItemData(item);

	// load the profile
	ProfilesDialog dlg(this);
	dlg.m_pCurrentProfile = g_Profiles.GetByIndex(pID);
	if (dlg.DoModal() == IDOK)
	{
		Profile* pProfile = dlg.m_pCurrentProfile;
		pProfile->setTarget	 (dlg.v_World, ConvBool(dlg.v_Global), ConvBool(dlg.v_Invisible), dlg.v_Coords, 0);
		pProfile->setLogin	 (dlg.v_Citizen, dlg.v_Password, dlg.v_BotName);
		pProfile->setUniverse(dlg.v_Universe);
		pProfile->SetIconId	 (dlg.v_Icon);
		pProfile->Save();
		UpdateGrid(item, *pProfile);
	}
}

void ProfilesManagerDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}