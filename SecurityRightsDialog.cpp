// SecurityRightsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "SecurityRightsDialog.h"
#include "DynamicRights.h"
#include "InputBoxDialog.h"

// SecurityRightsDialog dialog

IMPLEMENT_DYNAMIC(SecurityRightsDialog, CDialog)

SecurityRightsDialog::SecurityRightsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SecurityRightsDialog::IDD, pParent)
{

}

SecurityRightsDialog::~SecurityRightsDialog()
{
}

void SecurityRightsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERS, c_Users);
	DDX_Control(pDX, IDC_USER_ID, c_UserId);
	DDX_Control(pDX, IDC_FULL_ID, c_FullId);
	DDX_Control(pDX, IDC_DESCRIPTION, c_Description);
	DDX_Control(pDX, IDC_MEMBER_OF, c_MemberOf);
	DDX_Control(pDX, IDC_TREE_CTRL, c_RightsList);
}


BEGIN_MESSAGE_MAP(SecurityRightsDialog, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_USERS, &SecurityRightsDialog::OnUserList_ItemChanged)
	ON_BN_CLICKED(IDOK, &SecurityRightsDialog::OnBnClickedOk)
	ON_COMMAND(ID_USERS_NEW, &SecurityRightsDialog::OnUsers_New)
	ON_COMMAND(ID_USERS_DELETE, &SecurityRightsDialog::OnUsers_Delete)
	ON_COMMAND(ID_RIGHTS_RESETCUSTOMRIGHTS, &SecurityRightsDialog::OnRightsResetcustomrights)
	ON_COMMAND(ID_RIGHTS_IMPORT33314, &SecurityRightsDialog::OnRights_Import)
	ON_COMMAND(ID_RIGHTS_EXPORT33315, &SecurityRightsDialog::OnRights_Export)
	ON_COMMAND(ID_ACCOUNTS_NEWGROUP, &SecurityRightsDialog::OnAccounts_NewGroup)
END_MESSAGE_MAP()


// SecurityRightsDialog message handlers
using namespace DynamicRights;

BOOL SecurityRightsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	/* init rights list */
	m_RightsTree.Attach(c_RightsList);
	c_RightsList.ModifyStyle(0, TVS_CHECKBOXES	| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES);
	c_Users.SetExtendedStyle(c_Users.GetExtendedStyle() | LVS_EX_FLATSB | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	/* load icons */
	c_Users.InitIcons();
	String IconPath = GetAppPath() + "\\graphics\\drs";
	c_Users.InsertIcon(IconPath + "\\acc_main.bmp",		"main");
	c_Users.InsertIcon(IconPath + "\\acc_group.bmp",	"group");
	c_Users.InsertIcon(IconPath + "\\acc_user.bmp",		"user");

	/* set list ctrl */
	c_Users.InsertColumn(0, _T("User"), 0, 115);
	c_Users.InsertColumn(1, _T("Name"), 0, 200);

	/* load everything */
	InitAll();
	return TRUE;
}

void SecurityRightsDialog::InitAll()
{
	cur_user_id = -1;
	can_show = false;

	/* reset list */
	m_RightsTree.DeleteAll();
	c_RightsList.DeleteAllItems();
	c_Users.DeleteAllItems();

	/* init the rights list */
	for (size_t i = 0; i < provider.known_rights.size(); i++) 
	{
		String right = provider.known_rights[i];
		if (right.Find("/") != -1)
			continue;
		c_RightsList.Expand(m_RightsTree.InsertNode(right, right), TVE_EXPAND);
		
		String act_search = right + "/";
		for (size_t j = 0; j < provider.known_rights.size(); j++) 
		{
			String this_str = provider.known_rights[j];
			if (this_str.Left(act_search.GetLength()) == act_search)
			{
				m_RightsTree.InsertNode(this_str, right, this_str.Mid(this_str.Find("/") + 1));
			}
		}
	}

	/* expand out each node */
	HTREEITEM visItem = c_RightsList.GetFirstVisibleItem();
	while (visItem) {
		c_RightsList.Expand(visItem, TVE_EXPAND);
		visItem = c_RightsList.GetNextVisibleItem(visItem);
	}

	/* create list of users */
	for (size_t i = 0; i < provider.known_users.size(); i++) {
		User& user = provider.known_users[i];
		int icon = 0;

		if (user.user_id.Left(4) == "@")
			icon = c_Users.getIconKey("group");
		else if (user.user_id.Left(2) == "//")
			icon = c_Users.getIconKey("main");
		else
			icon = c_Users.getIconKey("user");

		int id = c_Users.InsertItem(c_Users.GetItemCount(), user.user_id, icon);
		c_Users.SetItemData(id, (DWORD_PTR)user.user_uid);
		c_Users.SetItemText(id, 1, user.full_id);
	}

	/* show item #1 */
	if (provider.known_users.size())
		ShowUser(provider.known_users[0]);
	can_show = true;
}

void SecurityRightsDialog::ShowUser(DynamicRights::User& UserBase)
{
	/* set text fields */
	c_UserId.SetWindowTextA		(UserBase.user_id);
	c_FullId.SetWindowTextA		(UserBase.full_id);
	c_Description.SetWindowText	(UserBase.description);

	/* go through the rights list and check the list */
	for (size_t i = 0; i < provider.known_rights.size(); i++) 
	{
		String right = provider.known_rights[i];
		m_RightsTree.setNodeChecked(right, UserBase.HasRight(right));
	}
}

void SecurityRightsDialog::SaveUser(DynamicRights::User& UserBase)
{
	/* copy the data back into the text */
	c_FullId.GetWindowTextA		(UserBase.full_id);
	c_Description.GetWindowText	(UserBase.description);

	/* copy rights back */
	for (size_t i = 0; i < provider.known_rights.size(); i++) 
	{
		String right = provider.known_rights[i];
		UserBase.setPerm(right, "", m_RightsTree.NodeChecked(right));
	}

	/* update data */
	int index = c_Users.GetByItemData(UserBase.user_uid);
	if (index != -1) 
	{
		c_Users.SetItemText(index, 0, UserBase.user_id);
		c_Users.SetItemText(index, 1, UserBase.full_id);
	}
}

void SecurityRightsDialog::OnUserList_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (!((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED)))
		return;

	/* do we have an existing item in need of saving? */
	User* pUser = provider.known_users.Find(cur_user_id);
	if (pUser) 
		SaveUser(*pUser);

	/* find this user */
	pUser = provider.known_users.Find((int)pNMLV->lParam);
	if (pUser) {
		ShowUser(*pUser);
		cur_user_id = (int)pNMLV->lParam;
	} else {
		MessageBox(_T("Unable to find the user - it may have been removed"));
	}
}

void SecurityRightsDialog::OnOK()
{
	/* do we have an existing item in need of saving? */
	User* pUser = provider.known_users.Find(cur_user_id);
	if (pUser) 
		SaveUser(*pUser);

	CDialog::OnOK();
}

void SecurityRightsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void SecurityRightsDialog::OnUsers_New()
{
	/* do we have an existing item in need of saving? */
	User* pUser = provider.known_users.Find(cur_user_id);
	if (pUser) 
		SaveUser(*pUser);

	/* new user */
	InputBoxDialog	input(this);
	input.v_String = "#";
	if (input.DoModal() != IDOK)
		return;
	else if (input.v_String.Left(2) == "//") {
		MessageBox(_T("Accounts starting with // are reserved for system use and cannot be added seperate"));
		return;
	}
	else if (provider.UserExists(input.v_String) || input.v_String.GetLength() == 0) {
		MessageBox("User already defined or name not valid");
		return;
	} 
	else {
		User user(input.v_String, input.v_String, "");
		provider.known_users.push_back(user);

		/* refresh list */
		InitAll();
	}
}

void SecurityRightsDialog::OnUsers_Delete()
{
	/* lookup user */
	User* pUser = provider.known_users.Find(cur_user_id);
	if (!pUser)
		return;

	/* double slashed? */
	if (pUser->user_id.Left(2) == "//") {
		MessageBox(_T("Accounts starting with // are reserved for system use and cannot be deleted"));
		return;
	}

	/* delete this user */
	if (MessageBox(_T("Are you sure you wish to delete this user?"),
		_T("Please Confirm"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		provider.known_users.Delete(pUser->user_id);
		InitAll();
	}
}

void SecurityRightsDialog::OnRightsResetcustomrights()
{
	if (MessageBox("This will delete any custom rights you have added and restore them to default settings\n\nContinue?", 0, MB_YESNOCANCEL) != IDYES)
	{
		provider.known_rights.clear();
		provider.CreateDefaultRights();
	}
}

void SecurityRightsDialog::OnRights_Import()
{
	// import the dynamic rights
	const char szFilter[] = "Dynamic Rights (*.ini)|*.ini||";
	CFileDialog file(TRUE, "ini", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// import
	provider.Load(file.GetFileName());
	InitAll();
}

void SecurityRightsDialog::OnRights_Export()
{
	// export the dynamic rights
	const char szFilter[] = "Dynamic Rights (*.ini)|*.ini||";
	CFileDialog file(FALSE, "ini", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// import
	if (provider.Save(file.GetFileName()) == 0)
	{
		MessageBox(String("The rights have been exported to:\n\n") + file.GetFileName());
	}
	else
	{
		MessageBox("Unable to export the rights profile...");
	}
}

#include "SecurityHeaderDialog.h"
void SecurityRightsDialog::OnAccounts_NewGroup()
{
	SecurityHeaderDialog d;
	if (d.DoModal() == IDOK)
	{
		if (provider.addGroup(d.v_identifier, d.v_name, d.v_description) == false)
		{
			MessageBox(_T("Unable to create group."));
		}
		else
		{
			// lookup the old user 
			User* pUser = provider.known_users.Find(cur_user_id);
			if (pUser) 
				SaveUser(*pUser);

			// reset list
			InitAll();

			// view the new user
			pUser = provider.known_users.Find(String("@") + d.v_identifier);
			if (pUser)
				ShowUser(*pUser);
		}
	}
}
