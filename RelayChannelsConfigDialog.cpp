// RelayChannelsConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "RelayChannelsConfigDialog.h"


// RelayChannelsConfigDialog dialog

IMPLEMENT_DYNAMIC(RelayChannelsConfigDialog, CDialog)

RelayChannelsConfigDialog::RelayChannelsConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RelayChannelsConfigDialog::IDD, pParent)
	, v_public(FALSE)
{

}

RelayChannelsConfigDialog::~RelayChannelsConfigDialog()
{
}

void RelayChannelsConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PUBLIC, v_public);
	DDX_Control(pDX, IDC_LIST1, c_acl);
}


BEGIN_MESSAGE_MAP(RelayChannelsConfigDialog, CDialog)
	ON_BN_CLICKED(IDOK, &RelayChannelsConfigDialog::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &RelayChannelsConfigDialog::OnACL_DoubleClick)
	ON_COMMAND(ID_ACCESSCONTROL_CREATEUSER, &RelayChannelsConfigDialog::OnACL_CreateUser)
END_MESSAGE_MAP()

const char* PermissionMap(int type)
{
	using namespace ChatControllers::Channels;
	if (type == Permission::NONE) return "none";
	else if (type == Permission::MEMBER) return "member";
	else if (type == Permission::MODERATOR) return "mod";
	else if (type == Permission::ADMINISTRATOR) return "admin";
	else return "?";
}

// RelayChannelsConfigDialog message handlers
void RelayChannelsConfigDialog::resetUsers()
{
	using namespace ChatControllers::Channels;
	c_acl.DeleteAllItems();

	// each right
	for (size_t i = 0; i < perms.size(); i++)
	{
		int n = c_acl.InsertItem(c_acl.GetItemCount(), perms[i].pattern);
		c_acl.EditItem(n, 1, PermissionMap(perms[i].level));
	}
}

BOOL RelayChannelsConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create styles
	c_acl.InsertColumn(0, "Key", 0, 100);
	c_acl.InsertColumn(1, "Type", 0, 70);
	resetUsers();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void RelayChannelsConfigDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

#include "RelayChannelPermissionDialog.h"
void RelayChannelsConfigDialog::OnACL_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// value
	int id = c_acl.GetByHitTest();
	if (id == -1) return;

	// value
	RelayChannelPermissionDialog d;
	d.v_user = perms[id].pattern;
	d.v_accessType = perms[id].level;

	// check status
	if (d.DoModal() == IDOK)
	{
		// is the access type a void
		if (d.v_accessType == ChatControllers::Channels::Permission::NONE)
		{
			c_acl.DeleteItem(id);
			perms.erase(perms.begin() + id);
		}
		else
		{
			perms[id].pattern = d.v_user;
			perms[id].level	  = d.v_accessType;

			// update icons
			c_acl.EditItem(id, 0, perms[id].pattern);
			c_acl.EditItem(id, 1, PermissionMap(perms[id].level));
		}
	}
}

void RelayChannelsConfigDialog::OnACL_CreateUser()
{
	// value
	RelayChannelPermissionDialog d;

	// check status
	if (d.DoModal() == IDOK)
	{
		// is the access type a void
		if (d.v_accessType == ChatControllers::Channels::Permission::NONE)
		{
		}
		else
		{
			ChatControllers::Channels::Permission p;
			p.pattern = d.v_user;
			p.level	  = d.v_accessType;
			p.timestamp = time64(0);
			perms.push_back(p);

			// update icons
			resetUsers();
		}
	}
}
