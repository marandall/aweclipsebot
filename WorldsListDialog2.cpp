// WorldsListDialog2.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "WorldsListDialog2.h"
#include "WorldList.h"

// WorldsListDialog dialog

IMPLEMENT_DYNAMIC(WorldsListDialog, CDialog)

enum
{
	COL_NAME = 0,
	COL_USERS,
	COL_RATING,
	COL_PEAK
};

class Sorters
{
public: // sorting mechanisms
	struct OnName : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			WorldList::World* pW1 = (WorldList::World*)Item1;
			WorldList::World* pW2 = (WorldList::World*)Item2;

			// comparisons
			return _stricmp(pW1->getName(), pW2->getName());
		}
	};

	struct OnPopulation : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			WorldList::World* pW1 = (WorldList::World*)Item1;
			WorldList::World* pW2 = (WorldList::World*)Item2;

			// comparisons
			if (pW1->getUsers() == pW2->getUsers()) return 0;
			return (pW1->getUsers() < pW2->getUsers()) ? -1 : 1;
		}
	};

	struct OnPeak : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			WorldList::World* pW1 = (WorldList::World*)Item1;
			WorldList::World* pW2 = (WorldList::World*)Item2;

			// comparisons
			if (pW1->getUserPeak() == pW2->getUserPeak()) return 0;
			return (pW1->getUserPeak() < pW2->getUserPeak()) ? -1 : 1;
		}
	};

	struct OnRating : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			WorldList::World* pW1 = (WorldList::World*)Item1;
			WorldList::World* pW2 = (WorldList::World*)Item2;

			// comparisons
			if (pW1->getRating() == pW2->getRating()) return 0;
			return (pW1->getRating() < pW2->getRating()) ? -1 : 1;
		}
	};

public: // sorting array
	OnName				name;
	OnPopulation	population;
	OnPeak				peak;
	OnRating			rating;

public: // access
	static Sorters& getInst()
	{
		static Sorters s;
		return s;
	}
};

WorldsListDialog::WorldsListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(WorldsListDialog::IDD, pParent)
{

}

WorldsListDialog::~WorldsListDialog()
{
}

void WorldsListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORLDS, c_list);
}


BEGIN_MESSAGE_MAP(WorldsListDialog, CDialog)
	ON_BN_CLICKED(IDOK, &WorldsListDialog::OnBnClickedOk)
	ON_WM_NCDESTROY()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &WorldsListDialog::OnList_HeaderClick)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// WorldsListDialog message handlers
void WorldsListDialog::updateItem(WorldList::World* World, int Index)
{
	// ref
	ListCtrlGpx& wlc = c_list;

	// require the index to be valid for this world
	int id = Index ? Index : c_list.GetByItemData((UINT_PTR)World);
	if (id == -1) 
		return;

	// get the percentage of users
	String perc = "0%";
	int uc = bot.getWorldList().getUserCount();
	if (uc)
		perc.Format("%d%%", (int)(100.0 / uc * World->getUsers()));

	// update the indexes
	wlc.EditItem(id, COL_NAME,		World->getName(), wlc.getIconKey(World->getIconKey()));
	wlc.EditItem(id, COL_USERS,		ToString(World->getUsers()));
	wlc.EditItem(id, COL_RATING,	World->getRatingString());
	// wlc.EditItem(id, COL_PEAK,		ToString(World->getUserPeak()));

	// resort list
	c_list.resort();
}

void WorldsListDialog::onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo)
{
	// world list notification messages
	if (Broadcaster == &bot.getWorldList())
	{
		// assign map
		ListCtrlGpx& wlc = c_list;
		WorldList::WorldChangedMessage* pNM = dynamic_cast<WorldList::WorldChangedMessage*>(EventInfo);

		// a world has recently come online
		if (Id == WorldList::Kernel::LM_WORLD_ONLINE)
		{
			int id = wlc.InsertItem(wlc.GetItemCount(), "");
			wlc.SetItemData(id, (DWORD_PTR)pNM->world);
			updateItem(pNM->world, id);
		}
		else if (Id == WorldList::Kernel::LM_WORLD_CHANGED)
		{
			updateItem(pNM->world);
		}
		else if (Id == WorldList::Kernel::LM_WORLD_OFFLINE)
		{
			int nId = wlc.GetByItemData((DWORD_PTR)pNM->world);
			if (nId != -1)
			{
				wlc.DeleteItem(nId);
			}
		}
		else if (Id == WorldList::Kernel::LM_WORLD_CLEARED)
		{
			wlc.DeleteAllItems();
		}

		// resort the data
		wlc.resort();
	}
}

void WorldsListDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL WorldsListDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// create icon list
	ListCtrlGpx& c_WorldList = c_list;
	c_WorldList.InitIcons();
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_G,							"G_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_G_PRIVATE,			"G_PRI");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_PG,						"PG_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_PG_PRIVATE,		"PG_PRI");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_PG13,					"PG13_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_PG13_PRIVATE,	"PG13_PRI");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_R,							"R_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_R_PRIVATE,			"R_PRI");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_X,							"X_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_X_PRIVATE,			"X_PRI");

	// styling
	c_list.useFullRowSelect().useGridlines();

	// insert new items
	c_list.InsertColumn(COL_NAME, _T("World Name"), 0, 100);
	c_list.InsertColumn(COL_USERS, _T("Users"), 0, 60);
	c_list.InsertColumn(COL_RATING, _T("Rating"), 0, 60);
	// c_list.InsertColumn(COL_PEAK, _T("Peak"), 0, 60);

	// sorting routines
	Sorters& s = Sorters::getInst();
	c_list.addSorter(COL_NAME, &s.name);
	c_list.addSorter(COL_USERS, &s.population);
	c_list.addSorter(COL_RATING, &s.rating);
	// c_list.addSorter(COL_PEAK, &s.peak);

	// sort item by population
	c_list.sort(COL_USERS, ListCtrlGpx::SORT_DECENDING);

	// insert every existing item
	c_list.SetItemCount(bot.getWorldList().getWorldCount());
	for (WorldList::Kernel::iterator w = bot.getWorldList().begin();
		w != bot.getWorldList().end(); w++)
	{
		int id = c_list.InsertItem(c_list.GetItemCount(), "");
		c_list.SetItemData(id, (DWORD_PTR)&(*w));
		updateItem(&(*w), id);
	}

	// connect listener
	connectListener(bot.getWorldList());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void WorldsListDialog::OnNcDestroy()
{
	__super::OnNcDestroy();
	delete this;
}

void WorldsListDialog::OnList_HeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;

	// sort the list items out
	c_list.sortToggle(phdr->iItem);
}

void WorldsListDialog::OnBnClickedOnTop()
{
}

void WorldsListDialog::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	c_list.MoveWindow(0, 0, cx, cy);
}
