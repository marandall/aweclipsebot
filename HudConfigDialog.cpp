// HudConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "HudConfigDialog.h"
#include "HeadsUp_UserPlugin.h"


// HudConfigDialog dialog
using namespace HeadsUp;

IMPLEMENT_DYNAMIC(HudConfigDialog, CDialog)

HudConfigDialog::HudConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(HudConfigDialog::IDD, pParent)
{

}

HudConfigDialog::~HudConfigDialog()
{
}

void HudConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOCATION, c_location);
	DDX_Control(pDX, IDC_LOCATION2, c_location2);
	DDX_Control(pDX, IDC_ICONS, c_list);
}


BEGIN_MESSAGE_MAP(HudConfigDialog, CDialog)
	ON_BN_CLICKED(IDOK, &HudConfigDialog::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_ICONS, &HudConfigDialog::OnIconList_IconActivated)
	ON_BN_CLICKED(IDC_ADD_ITEM, &HudConfigDialog::OnBnClickedAddItem)
	ON_NOTIFY(NM_CLICK, IDC_ICONS, &HudConfigDialog::OnIcons_Clicked)
	ON_BN_CLICKED(IDC_CLEAR_ITEM, &HudConfigDialog::OnBnClickedClearItem)
END_MESSAGE_MAP()


int CALLBACK SortListItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   ListCtrlGpx* pListCtrl = (ListCtrlGpx*) lParamSort;
   String strItem1 = pListCtrl->GetItemText((int)lParam1, 1);
   String strItem2 = pListCtrl->GetItemText((int)lParam2, 1);
   return strcmp(strItem2, strItem1);
}


// HudConfigDialog message handlers

void HudConfigDialog::OnBnClickedOk()
{
	OnOK();
}

void HudConfigDialog::InsertItemToList(HeadsUp::StoredTrayItem& Sti, bool Insert)
{
	int id = -1;
	
	// is it a standard insert
	if (Insert == true)
	{
		id = c_list.InsertItem(c_list.GetItemCount(), Sti.key);
	}
	else
	{
		id = c_list.GetByItemData((UINT_PTR)&Sti);
	}

	c_list.EditItem(id, 0, Sti.key);
	c_list.EditItem(id, 1, Sti.row);
	c_list.EditItem(id, 2, Sti.image);
	c_list.EditItem(id, 3, HeadsUp::Element::MethodToString(Sti.click_method));
	c_list.EditItem(id, 4, Sti.click_function);
	c_list.SetItemData(id, (DWORD_PTR)&Sti);

	// sort the list
	c_list.SortItems(&SortListItems, (DWORD_PTR)&c_list);
}

void HudConfigDialog::SelectItem(HeadsUp::StoredTrayItem* Sti)
{
	// if there is an old icon stored
	if (current_icon != 0)
	{
		m_dsStored.dialog_to_attributes(*current_icon);
		InsertItemToList(*current_icon, false);
	}

	// is there a new item to load into
	if (Sti != 0)
	{
		m_dsStored.attributes_to_dialog(*Sti);
	}

	// assign the data
	current_icon = Sti;
}

BOOL HudConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	current_icon = 0;

	// create window for stored item
	c_psc_stored.create_control(*this, c_location);
	c_psc_stored.create_group(0, "Stored Item", m_dsStored);
	c_psc_stored.window->CONTROL_INSET = 100;
	c_psc_stored.window->show_group(0);

	// create window for config
	c_psc_config.create_control(*this, c_location2);
	c_psc_config.create_group(0, "Configuration", m_dsConfig);
	c_psc_config.window->CONTROL_INSET = 100;
	c_psc_config.window->show_group(0);
	m_dsConfig.pPosition->Insert(HeadsUp::Config::Positions());

	// add the tray icons
	c_list.InsertColumn(0, "Key",		0, 100);
	c_list.InsertColumn(1, "Row",		0, 50);
	c_list.InsertColumn(2, "Image",		0, 130);
	c_list.InsertColumn(3, "Method",	0, 70);
	c_list.InsertColumn(4, "Function",	0, 200);

	// add each element to the display
	typedef HeadsUp::Config::TrayItemArray TrayItemArray;
	for (TrayItemArray::iterator itr = icon_array.begin(); itr != icon_array.end(); itr++)
	{
		InsertItemToList(*itr, true);
	}

	// add the items to each element
	for (int i = 0; i < HeadsUp::Element::CLICK_METHOD_MAX; i++)
	{
		int id = m_dsStored.pClickMethod->AddString(HeadsUp::Element::MethodToString(i));
		m_dsStored.pClickMethod->SetItemData(id, (int)i);
	}

	// add item
	m_dsConfig.attributes_to_dialog(HeadsUp::Config::getInstance());

	return TRUE;
}

void HudConfigDialog::OnTabs_SelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
}

void HudConfigDialog::OnOK()
{
	// save everything back
	SelectItem(NULL);

	// save the settings
	m_dsConfig.dialog_to_attributes(HeadsUp::Config::getInstance());
	CDialog::OnOK();
}

void HudConfigDialog::OnIcons_CreateIcon()
{
}

void HudConfigDialog::OnIconList_IconActivated(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
}

void HudConfigDialog::OnBnClickedAddItem()
{
	HeadsUp::StoredTrayItem sti;
	sti.key = "new_hud_item";

	// add to the list
	icon_array.push_back(sti);
	InsertItemToList(icon_array.back(), true);
}

void HudConfigDialog::OnIcons_Clicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	// add an item
	int hit = c_list.GetByHitTest();
	if (hit == -1) return;

	// show this item
	HeadsUp::StoredTrayItem* sti = (HeadsUp::StoredTrayItem*)c_list.GetItemData(hit);
	SelectItem(sti);
}

void HudConfigDialog::OnBnClickedClearItem()
{
	// add each element to the display
	typedef HeadsUp::Config::TrayItemArray TrayItemArray;
	for (TrayItemArray::iterator itr = icon_array.begin(); itr != icon_array.end(); itr++)
	{
		if (&(*itr) == current_icon)
		{
			icon_array.erase(itr);
			
			// get the item by icon data
			int index = c_list.GetByItemData((UINT_PTR)current_icon);
			c_list.DeleteItem(index);

			// delete the old item
			current_icon = 0;
			SelectItem(0);
			return;
		}
	}
}
