// WorldEjectionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "WorldEjectionsDialog.h"
#include "world_ejections.h"

// WorldEjectionsDialog dialog
using namespace Ejections;

IMPLEMENT_DYNAMIC(WorldEjectionsDialog, CDialog)

WorldEjectionsDialog::WorldEjectionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(WorldEjectionsDialog::IDD, pParent)
{
	connectListener(bot.getEjectManager());
}

WorldEjectionsDialog::~WorldEjectionsDialog()
{
}

void WorldEjectionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EJECTIONS, _list);
}


BEGIN_MESSAGE_MAP(WorldEjectionsDialog, CDialog)
	ON_BN_CLICKED(IDC_QUERY, &WorldEjectionsDialog::OnBnClickedQuery)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &WorldEjectionsDialog::OnList_ToggleHeader)
	ON_WM_NCDESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// WorldEjectionsDialog message handlers

void WorldEjectionsDialog::OnBnClickedQuery()
{
}

void WorldEjectionsDialog::updateList(Ejections::Ejection& Ej, bool First)
{
	// durations
	CTime created = Ej.created;
	CTime expires = Ej.expires;

	// update control
	int id = _list.GetByItemData((UINT_PTR)&Ej);
	if (First) 
	{
		// ejection type
		String ejIcon = "ip";
		if (Ej.type == AW_EJECT_BY_CITIZEN) ejIcon = "cit";
		else if (Ej.type == AW_EJECT_BY_COMPUTER) ejIcon = "comp";

		int ei = _list.getIconKey(ejIcon);

		// update the main icon
		id = _list.InsertItem(_list.GetItemCount(), Ej.getTypeName());
		_list.EditItem(id, 0, Ej.getTypeName(), ei); 
		_list.SetItemData(id, (UINT_PTR)&Ej);
	}

	// update text
	_list.EditItem(id, 1, Ej.toString());
	_list.EditItem(id, 2, created.Format("%b %d ,%Y %H:%M"));
	_list.EditItem(id, 3, Ej.expires ? expires.Format("%b %d ,%Y %H:%M") : "Never");
	_list.EditItem(id, 4, Ej.getCommentName());
	_list.EditItem(id, 5, Ej.getCommentEjector());
	_list.EditItem(id, 6, Ej.comment);
}

void WorldEjectionsDialog::onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo)
{
	if (Broadcaster == &bot.getEjectManager() && Id == EjectionManager::LE_EJECTION_FOUND)
	{
		MessageEjectionFound* pMsg = dynamic_cast<MessageEjectionFound*>(EventInfo);
		updateList(*pMsg->eject, true);
	}
	else if (Broadcaster == &bot.getEjectManager() && Id == EjectionManager::LE_EJECTION_UPDATED)
	{
		MessageEjectionChanged* pMsg = dynamic_cast<MessageEjectionChanged*>(EventInfo);
		updateList(*pMsg->eject);
	}
	else if (Broadcaster == &bot.getEjectManager() && Id == EjectionManager::LE_EJECTION_EXPIRED)
	{
		MessageEjectionExpired* pMsg = dynamic_cast<MessageEjectionExpired*>(EventInfo);
		int nID = _list.GetByItemData(pMsg->eject->getAutoNumber());
		if (nID != -1)
			_list.DeleteItem(nID);
	}
	else if (Broadcaster == &bot.getEjectManager() && Id == EjectionManager::LE_CLEARED)
	{
		_list.DeleteAllItems();
	}
	else if (Broadcaster == &bot.getEjectManager() && Id == EjectionManager::LE_STATUS)
	{
		_statusBar.GetStatusBarCtrl().SetText(bot.getEjectManager().getStatus(), 0, SBT_POPOUT);
	}

	// update the sorting information
	_list.resort();
}

BOOL WorldEjectionsDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// create status bar
	_statusBar.Create(this);

	// create tool icons
	_list.InitIcons(ILC_COLOR4);
	_list.InsertIconByIconResource(IDI_EJECTION_CITIZEN, "cit");
	_list.InsertIconByIconResource(IDI_EJECTION_IP, "ip");
	_list.InsertIconByIconResource(IDI_EJECTION_COMPUTER, "comp");

	// create control options
	_list.useFullRowSelect().useGridlines();
	_list.InsertColumn(0, _T("Type"),				0, 80);
	_list.InsertColumn(1, _T("Ejection"),		0, 100);
	_list.InsertColumn(2, _T("Created"),		0, 130);
	_list.InsertColumn(3, _T("Expires"),		0, 130);
	_list.InsertColumn(4, _T("Name"),				0, 100);
	_list.InsertColumn(5, _T("Ejected By"),	0, 100);
	_list.InsertColumn(6, _T("Comment"),		0, 250);
	
	// sort handlers
	_list.addSorter(0,	&_sortType);
	_list.addSorter(1,	&_sortAddress);
	_list.addSorter(2,	&_sortCreated);
	_list.addSorter(3,	&_sortExpired);
	_list.addSorter(4,	&_sortCommentName);
	_list.addSorter(5,	&_sortCommentEjector);

	// set sorter
	_list.sortToggle(0);

	// insert all existing items into the list
	EjectionManager& em = bot.getEjectManager();
	for (EjectionManager::iterator ej = em.begin(); ej != em.end(); ej++)
		updateList(*ej, true);
	_list.resort();

	return TRUE;
}

void WorldEjectionsDialog::OnList_ToggleHeader(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;
	_list.sortToggle(phdr->iItem);
}

void WorldEjectionsDialog::OnNcDestroy()
{
	__super::OnNcDestroy();
	delete this;
}

void WorldEjectionsDialog::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (IsWindow(_statusBar) == TRUE)
	{
		_list.MoveWindow(5, 5, cx - 10, cy - 20 - 10);
		_statusBar.MoveWindow(0, cy - 20, cx, 20);
	}
}
