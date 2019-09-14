// ZoneEditorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ZoneEditorDialog.h"
#include "zones.h"
#include "MiniParser.h"
#include "ZonesEditorRectangleDialog.h"
#include "ZonesEditorCylinderDialog.h"
#include "ZonesEditorSphereDialog.h"
#include "ZonesEditorPolygonDialog.h"

// ZoneEditorDialog dialog
using namespace Zones;

IMPLEMENT_DYNAMIC(ZoneEditorDialog, CDialog)

ZoneEditorDialog::ZoneEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ZoneEditorDialog::IDD, pParent)
{

}

ZoneEditorDialog::~ZoneEditorDialog()
{
}

void ZoneEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, c_Tree);
	DDX_Control(pDX, IDC_REGIONS, c_RegionsList);
	DDX_Control(pDX, IDC_TRACKER, c_RectTracker);
}


BEGIN_MESSAGE_MAP(ZoneEditorDialog, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &ZoneEditorDialog::OnTree_SelectionChange)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, &ZoneEditorDialog::OnTree_NodeSelect)
	ON_COMMAND(ID_INSERTZONE_CREATECHILD, &ZoneEditorDialog::OnContext_CreateChild)
	ON_COMMAND(ID_MANAGE_SAVE, &ZoneEditorDialog::OnManage_Save)
	ON_BN_CLICKED(IDC_NEW_RECT, &ZoneEditorDialog::OnBnClickedNewRect)
	ON_BN_CLICKED(IDC_BUTTON3, &ZoneEditorDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &ZoneEditorDialog::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_NEW_CYLINDER, &ZoneEditorDialog::OnBnClickedNewCylinder)
	ON_NOTIFY(NM_DBLCLK, IDC_REGIONS, &ZoneEditorDialog::OnRegionList_DoubleClick)
	ON_COMMAND(ID_INSERTZONE_DELETE, &ZoneEditorDialog::OnContext_DeleteZone)
	ON_BN_CLICKED(IDOK, &ZoneEditorDialog::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_REGIONS, &ZoneEditorDialog::OnLvnItemchangedRegions)
	ON_COMMAND(ID__DELETECHILDREN, &ZoneEditorDialog::OnMenu_DeleteChildren)
END_MESSAGE_MAP()

void ZoneEditorDialog::SelectZone(Zones::Zone& ZoneInfo)
{
}

// ZoneEditorDialog message handlers
HTREEITEM ZoneEditorDialog::EnumerateTree(Zones::Zone& ZoneData, HTREEITEM ParentNode)
{
	for (Zones::ZoneList::iterator zdPtr = ZoneData.children.begin(); 
		zdPtr != ZoneData.children.end(); 
		zdPtr++)
	{
		HTREEITEM hti = c_Tree.InsertItem(zdPtr->getName(), ParentNode);
		c_Tree.SetItemData(hti, (DWORD_PTR)zdPtr->getTrackingID());
		EnumerateTree(*zdPtr, hti);

		// expand node
		c_Tree.Expand(hti, TVE_EXPAND);
	}

	/* return the parent node, no better idea */
	return ParentNode;
}

BOOL ZoneEditorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create the tracker on screen
	c_PsHelper.create_control(*this, c_RectTracker);
	c_PsHelper.tabs	= NULL;

	// setup the pages	
	c_PsHelper.create_group(0,  "Zone",		m_dsZone.assign_remote(root));
	c_PsHelper.window->show_group(0);

	// edit the buttons
	SetWindowLong(c_Tree.m_hWnd, GWL_STYLE, GetWindowLong(c_Tree.m_hWnd, GWL_STYLE) | TVS_HASBUTTONS | 
						TVS_HASLINES | TVS_LINESATROOT); 
	menu_zone_id = 0;
	selected_id  = 0;

	// create the root
	HTREEITEM tree_item = c_Tree.InsertItem(root.getName());
	c_Tree.SetItemData	(tree_item, (DWORD_PTR)root.getTrackingID());
	EnumerateTree		(root, tree_item);

	// add cols here
	c_RegionsList.InsertColumn(0, _T("Area"), 0, 80);
	c_RegionsList.InsertColumn(1, _T("Type"), 0, 60);
	c_RegionsList.InsertColumn(2, _T("About"), 0, 200);

	return TRUE;
}

void ZoneEditorDialog::OnTree_SelectionChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	/* select the data */
	int zone_id = static_cast<int>(pNMTreeView->itemNew.lParam);
	SwitchZone(zone_id);
}

void ZoneEditorDialog::OnTree_NodeSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	
	// find the node
	POINT curPt; 
	GetCursorPos(&curPt);

	// translate into this node
	c_Tree.ScreenToClient(&curPt);
	HTREEITEM node_selected = c_Tree.HitTest(CPoint(curPt));
	if (node_selected != NULL)
	{
		Zones::Zone* zone = root.SearchID(static_cast<int>(c_Tree.GetItemData(node_selected)));
		if (zone)
		{
			menu_zone_id = zone->getTrackingID();

			CMenu* mn = GetMenu();
			CMenu* mx = mn->GetSubMenu(1);

			POINT point;
			GetCursorPos (&point);

			mx->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
	}
}

HTREEITEM search_tree(int tree_id, HTREEITEM root, CTreeCtrl& ct)
{
	HTREEITEM ci = ct.GetNextItem(root, TVGN_CHILD);
	while (ci)
	{
		if (ct.GetItemData(ci) == tree_id)
			return ci;
		HTREEITEM cx = search_tree(tree_id, ci, ct);
		if (cx) return cx;
		ci = ct.GetNextItem(ci, TVGN_NEXT);
	}

	// no next element could be found
	return NULL;
}

HTREEITEM ZoneEditorDialog::LookupTreeByID(int id)
{
	return search_tree(id, TVI_ROOT, c_Tree);
}

void ZoneEditorDialog::SwitchZone(int zone_id)
{
	// if we have an old zone that needs saving
	if (selected_id != 0)
	{
		Zone* zone = root.SearchID(selected_id);
		if (zone)
		{
			// update data
			m_dsZone.dialog_to_attributes(*zone);

			// update name and icon
			HTREEITEM hti = LookupTreeByID(zone->getTrackingID());
			if (hti) 
			{ 
				c_Tree.SetItemText(hti, zone->getName()); 
			}
		}
	}

	// this zone
	Zone* zone = root.SearchID(zone_id);
	if (zone == NULL) return;
	selected_id = zone_id;

	// update data
	m_dsZone.attributes_to_dialog(*zone);

	// new regions
	rebuild_region_list();
}

void ZoneEditorDialog::OnContext_CreateChild()
{
	// search for the current node
	Zone* pZone = root.SearchID(menu_zone_id);
	if (pZone == NULL) return;

	// insert the node
	Zone zn;
	zn.id	= pZone->id + ToString(rand());
	zn.name	= String("Child of " ) + pZone->getName();
	zn.parent_zone = pZone;
	pZone->children.push_back(zn);

	// look up root node
	HTREEITEM ht = LookupTreeByID(pZone->getTrackingID());

	// insert icon
	HTREEITEM tree_item = c_Tree.InsertItem(zn.getName(), ht);
	c_Tree.SetItemData	(tree_item, (DWORD_PTR)zn.getTrackingID());
	c_Tree.EnsureVisible(tree_item);
	c_Tree.SelectItem(tree_item); 
}

void ZoneEditorDialog::OnManage_Save()
{
	// open the save dialog
	const char szFilter[] = "Zone Defenition File (*.e2zdf)|*.e2zdf||";
	CFileDialog file(FALSE, "e2zdf", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// save
	root.save(file.GetPathName());
}

void ZoneEditorDialog::insert_region_list(int ID, CString Name, CStringA Type, CStringA Description)
{
	int id = c_RegionsList.InsertItem(c_RegionsList.GetItemCount(), Name);
	c_RegionsList.SetItemData(id, ID);
	c_RegionsList.EditItem(id, 1, CString(Type));
	c_RegionsList.EditItem(id, 2, CString(Description));
}

void ZoneEditorDialog::rebuild_region_list()
{
	// delete le items
	c_RegionsList.DeleteAllItems();

	// look up the zone we will be handling
	Zone* zone = root.SearchID(selected_id);
	if (!zone) 
		return;

	// create a list entry for each region
	// key type description
	
	// rectangles
	for (size_t i = 0; i < zone->regions.rectangles.size(); i++)
	{
		Regions::Rectangle& r = zone->regions.rectangles[i];
		insert_region_list(r.id, r.name, "Rect", r.getDescription());
	}

	// spheres
	for (size_t i = 0; i < zone->regions.spheres.size(); i++)
	{
		Regions::Sphere& r = zone->regions.spheres[i];
		insert_region_list(r.id, r.name, "Sphere",  r.getDescription());
	}

	// cylinders
	for (size_t i = 0; i < zone->regions.cylinders.size();  i++)
	{
		Regions::Cylinder& r = zone->regions.cylinders[i];
		insert_region_list(r.id, r.name, "Cylinder", r.getDescription());
	}

	// polygons
	for (size_t i = 0; i < zone->regions.polygons.size();  i++)
	{
		Regions::Polygon& r = zone->regions.polygons[i];
		insert_region_list(r.id, r.name, "Polygon", r.getDescription());
	}
}

void ZoneEditorDialog::OnBnClickedNewRect()
{
	// look up the zone we will be handling
	Zone* zone = root.SearchID(selected_id);
	if (!zone) return;

	// create a new rectangle and add it
	Regions::Rectangle r;
	r.name = "no-tag";
	zone->regions.push_back(r);

	// rebuild the list
	rebuild_region_list();
}

void ZoneEditorDialog::OnBnClickedButton3()
{
	// look up the zone we will be handling
	Zone* zone = root.SearchID(selected_id);
	if (!zone) return;

	// create a new rectangle and add it
	Regions::Sphere r;
	r.name = "no-tag";
	zone->regions.push_back(r);

	// rebuild the list
	rebuild_region_list();
}

void ZoneEditorDialog::OnBnClickedButton6()
{
	// look up the zone we will be handling
	Zone* zone = root.SearchID(selected_id);
	if (!zone) return;

	// create a new rectangle and add it
	Regions::Polygon r;
	r.name = "no-tag";
	zone->regions.push_back(r);

	// rebuild the list
	rebuild_region_list();
}

void ZoneEditorDialog::OnBnClickedNewCylinder()
{
	// look up the zone we will be handling
	Zone* zone = root.SearchID(selected_id);
	if (!zone) return;

	// create a new rectangle and add it
	Regions::Cylinder r;
	r.name = "no-tag";
	zone->regions.push_back(r);

	// rebuild the list
	rebuild_region_list();
}

void ZoneEditorDialog::OnRegionList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// look up the element clicked
	int id = c_RegionsList.GetByHitTest();
	if (id == -1) return;

	// get the associated item data
	int i_data = static_cast<int>(c_RegionsList.GetItemData(id));

	// look up the zone we will be handling
	Zone* zone = root.SearchID(selected_id);
	if (!zone) return;

	for (size_t i = 0; i < zone->regions.rectangles.size(); i++)
	{
		Regions::Rectangle& r = zone->regions.rectangles[i];
		if (r.id == i_data)
		{
			ZonesEditorRectangleDialog dlg(this);
			dlg.v_Name		= r.name;
			dlg.v_Top		= r.top.getCoordinates();
			dlg.v_Bottom	= r.bottom.getCoordinates();
			INT_PTR nCode = dlg.DoModal();
			if (nCode == IDOK)
			{
				Location lc1(dlg.v_Top), lc2(dlg.v_Bottom); 
				r.name		= dlg.v_Name;
				r.top			= lc1.toHighPoint(lc2);
				r.bottom	= lc1.toLowPoint(lc2);
			}
			else if (nCode == IDNO)
			{
				zone->regions.rectangles.erase(zone->regions.rectangles.begin() + i);
			}

			// completed handling
			rebuild_region_list();
			return;
		}
	}

	// spheres
	for (size_t i = 0; i < zone->regions.spheres.size(); i++)
	{
		Regions::Sphere& r = zone->regions.spheres[i];
		if (r.id == i_data)
		{
			ZonesEditorSphereDialog dlg(this);
			dlg.v_Name		= r.name;
			dlg.v_Centre	= r.centre.getCoordinates();
			dlg.v_Radius	= r.radius / 100.0F;
			INT_PTR nCode = dlg.DoModal();
			if (nCode == IDOK)
			{
				Location lc1(dlg.v_Centre); 
				r.name		= dlg.v_Name;
				r.centre	= lc1;
				r.radius	= static_cast<int>(dlg.v_Radius * 100.0F);
			}
			else if (nCode == IDNO)
			{
				zone->regions.spheres.erase(zone->regions.spheres.begin() + i);
			}

			// completed handling
			rebuild_region_list();
			return;
		}
	}

	// cylinders
	for (size_t i = 0; i < zone->regions.cylinders.size();  i++)
	{
		Regions::Cylinder& r = zone->regions.cylinders[i];
		if (r.id == i_data)
		{
			ZonesEditorCylinderDialog dlg(this);
			dlg.v_Name		= r.name;
			dlg.v_Base		= r.centre.getCoordinates();
			dlg.v_Height	= r.height / 100.0F;
			dlg.v_Radius	= r.radius / 100.0F;
			INT_PTR nCode = dlg.DoModal();
			if (nCode == IDOK)
			{
				Location lc1(dlg.v_Base); 
				r.name		= dlg.v_Name;
				r.centre	= lc1;
				r.height	= static_cast<int>(dlg.v_Height * 100.0F);
				r.radius	= static_cast<int>(dlg.v_Radius * 100.0F);
			}
			else if (nCode == IDNO)
			{
				zone->regions.cylinders.erase(zone->regions.cylinders.begin() + i);
			}

			// completed handling
			rebuild_region_list();
			return;
		}
	}

	// cylinders
	for (size_t i = 0; i < zone->regions.polygons.size();  i++)
	{
		Regions::Polygon& r = zone->regions.polygons[i];
		if (r.id == i_data)
		{
			ZonesEditorPolygonDialog dlg(this);
			dlg.v_name		= r.name;
			dlg.v_base		= r.base / 100.0f;
			dlg.v_height	= r.height / 100.f;
			
			// create a map
			String map;
			for (size_t j = 0; j < r.verts.size(); j++)
				map.AppendFormat("%s\r\n", r.verts[j].getCoordinates());
			dlg.v_map = map;

			INT_PTR nCode = dlg.DoModal();
			if (nCode == IDOK)
			{
				r.name		= dlg.v_name;
				r.base		= (int)(dlg.v_base * 100);
				r.height	= (int)(dlg.v_height * 100);
				
				// parse the map name
				SplitString m(dlg.v_map, "\n", -1);
				r.verts.clear();

				// iterate each line only
				for (size_t k = 0; k < m.size(); k++)
				{
					// parse the string
					String s = m[k];
					s.Trim();

					// skip empty line
					if (s.GetLength() == 0)
						continue;

					// parse it
					Location lc(s);
					r.AddPoint(lc.getX(), lc.getZ());
				}
			}
			else if (nCode == IDNO)
			{
				zone->regions.polygons.erase(zone->regions.polygons.begin() + i);
			}

			// completed handling
			rebuild_region_list();
			return;
		}
	}
}

void ZoneEditorDialog::OnOK()
{
	// switching to a non-existant zone saves our work
	SwitchZone(-1);
	CDialog::OnOK();
}

void ZoneEditorDialog::OnContext_DeleteZone()
{
	// search for the current node
	Zone* pZone = root.SearchID(menu_zone_id);
	if (pZone == NULL) return;

	// delete the zone
	if (pZone->getParent() == NULL)
	{
		MessageBox(_T("You cannot delete the root node"), _T("Zone Editor"), MB_ICONEXCLAMATION);
	}
	else
	{
		// display the user name
		String log;
		log.Format("Are you sure you wish to delete the '%s' zone?", pZone->getName());

		// confirm
		if (MessageBox(CString(log), "Zone Editor", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// backup tracking id
			int tracking_id = pZone->getTrackingID();;
			
			// delete the current zone 
			Zone* pParent = pZone->getParent();
			pParent->deleteChild(tracking_id);

			menu_zone_id = 0;
			selected_id  = 0;

			// delete this item
			HTREEITEM ht = LookupTreeByID(tracking_id);
			if (ht != NULL) 
				c_Tree.DeleteItem(ht);

			// force a parent refresh
			root.update_parents();

			SwitchZone(root.getTrackingID());

			root.getFullDescriptor();
		}
	}
}

void ZoneEditorDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void ZoneEditorDialog::OnLvnItemchangedRegions(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void ZoneEditorDialog::OnMenu_DeleteChildren()
{
	// search for the current node
	Zone* pZone = root.SearchID(menu_zone_id);
	if (pZone == NULL) return;

	// clear children
	pZone->children.clear();

	// find root
	HTREEITEM ht = LookupTreeByID(pZone->tracking_id);
	HTREEITEM node = 0;
	
	// go through each system
	while (node = c_Tree.GetNextItem(ht, TVGN_CHILD))
		c_Tree.DeleteItem(node);
}
