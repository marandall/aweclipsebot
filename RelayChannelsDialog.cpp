// RelayChannelsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "RelayChannelsDialog.h"
#include "relayrelay.h"

// importz
using namespace ChatControllers;
using namespace ChatControllers::Channels;


// RelayChannelsDialog dialog

IMPLEMENT_DYNAMIC(RelayChannelsDialog, CDialog)

RelayChannelsDialog::RelayChannelsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RelayChannelsDialog::IDD, pParent)
{

}

RelayChannelsDialog::~RelayChannelsDialog()
{
}

void RelayChannelsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, c_list);
	DDX_Control(pDX, IDC_LIST2, c_members);
}


BEGIN_MESSAGE_MAP(RelayChannelsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &RelayChannelsDialog::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &RelayChannelsDialog::OnList_DoubleClick)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST1, &RelayChannelsDialog::OnList_ItemActivate)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &RelayChannelsDialog::OnList_ItemChanged)
END_MESSAGE_MAP()


// RelayChannelsDialog message handlers

BOOL RelayChannelsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create the channels
	enum {
		CI_NAME, CI_PUBLIC, CI_ABOUT
	};

	// create cols
	c_list.InsertColumn(CI_NAME, "Identifier", 0, 120);
	c_list.InsertColumn(CI_PUBLIC, "Access", 0, 60);
	c_list.InsertColumn(CI_ABOUT, "About", 0, 250);

	// display channels
	for (ChannelList::iterator c = g_Relay.getChannels().begin();
		c != g_Relay.getChannels().end();
		c++)
	{
		int n = c_list.InsertItem(c_list.GetItemCount(), c->name);
		c_list.SetItemData(n, (DWORD_PTR)c->getAutoNumber());
		c_list.EditItem(n, CI_PUBLIC, c->is_public ? "public" : "private");
		c_list.EditItem(n, CI_ABOUT, "No Information");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void RelayChannelsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

#include "RelayChannelsConfigDialog.h"
void RelayChannelsDialog::OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// hit test
	int id = c_list.GetByHitTest();
	if (id == -1)
		return;

	// get the id
	Channel* pChan = g_Relay.getChannels().getAuto((int)c_list.GetItemData(id));
	if (pChan == 0)
		return;

	// push to dialog
	RelayChannelsConfigDialog d;
	d.perms			= pChan->acl;
	d.v_public		= bB(pChan->is_public);
	
	// ok?
	if (d.DoModal() == IDOK)
	{
		pChan->acl = d.perms;
		pChan->is_public = Bb(d.v_public);
	}
}

void RelayChannelsDialog::OnList_ItemActivate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void RelayChannelsDialog::OnList_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (pNMLV->uNewState != 3)
		return;

	// erase the item
	c_members.DeleteAllItems();

	// display a list of members
	Channels::Channel* pChan = g_Relay.getChannels().getAuto((int)c_list.GetItemData(pNMLV->iItem));
	for (size_t i = 0; i < pChan->members.size(); i++)
		c_members.InsertItem((int)i, pChan->members[i]->av->getName());
}
