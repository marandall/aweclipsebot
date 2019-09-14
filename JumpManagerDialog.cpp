// JumpManagerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "JumpManagerDialog.h"
#include "beacons.h"
#include "JumpEditorDialog.h"
#include "JumpTransferOwnershipDialog.h"

// JumpManagerDialog dialog

IMPLEMENT_DYNAMIC(JumpManagerDialog, CDialog)

enum
{
	COL_NAME,
	COL_COORDINATES,
	COL_OWNER,
	COL_ZONE
};

JumpManagerDialog::JumpManagerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(JumpManagerDialog::IDD, pParent)
{

}

JumpManagerDialog::~JumpManagerDialog()
{
}

void JumpManagerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, c_name);
	DDX_Control(pDX, IDC_EDIT3, c_owner);
	DDX_Control(pDX, IDC_LIST1, c_list);
}


BEGIN_MESSAGE_MAP(JumpManagerDialog, CDialog)
	ON_BN_CLICKED(IDC_SEARCH, &JumpManagerDialog::OnBnClickedSearch)
	ON_BN_CLICKED(IDOK, &JumpManagerDialog::OnBnClickedOk)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST1, &JumpManagerDialog::OnList_DoubleRightClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &JumpManagerDialog::OnList_DoubleClick)
	ON_COMMAND(ID_JUMPS_IMPORTJUMPS, &JumpManagerDialog::OnJumps_Import)
	ON_COMMAND(ID_JUMPS_IMPORTJUMPS33331, &JumpManagerDialog::OnJumps_ImportOld)
	ON_COMMAND(ID_JUMPS_EXPORTJUMPS, &JumpManagerDialog::OnJumps_Export)
	ON_COMMAND(ID_JUMPS_INSERTNEW, &JumpManagerDialog::OnJumps_InsertNew)
	ON_COMMAND(ID_JUMPS_RESETLIST, &JumpManagerDialog::OnJumps_Reset)
	ON_COMMAND(ID_SEARCH_EXPORTRESULTS, &JumpManagerDialog::OnSearch_ExportResults)
	ON_COMMAND(ID_SEARCH_EMPTYRESULTS, &JumpManagerDialog::OnSearch_EmptyResults)
	ON_COMMAND(ID_SEARCH_TRANSFEROWNER, &JumpManagerDialog::OnSearch_TransferOwner)
	ON_COMMAND(ID_SEARCH_ERAS, &JumpManagerDialog::OnSearch_EraseAll)
	ON_WM_NCDESTROY()
	ON_COMMAND(ID_FILE_CONFIGURATIONOPTIONS, &JumpManagerDialog::OnJumps_ConfigOptions)
END_MESSAGE_MAP()

void JumpManagerDialog::OnBnClickedSearch()
{
	// lookup query information
	String name = c_name.GetText(); name.MakeLower();
	int owner = atoi(c_owner.GetText());
	Beacons::Kernel& col = Beacons::Kernel::getInst();

	// reset the list
	ListCtrlGpx& gpx = c_list;
	gpx.DeleteAllItems();
	search_results.clear();

	// beacon map
	for (Beacons::Kernel::iterator jp = col.begin();
		jp != col.end(); jp++)
	{
		// check name
		if (name.GetLength() != 0)
		{
			// lowercase the string and check
			String lwr = jp->getName();
			lwr.MakeLower();

			// in string
			if (lwr.Find(name) == -1)
				continue;
		}

		// check number
		if (owner != 0 && jp->getOwner() != owner)
			continue;

		// insert into the list
		int id = gpx.InsertItem(gpx.GetItemCount(), jp->getName());
		gpx.SetItemData(id, (DWORD_PTR)jp->getAutoNumber());
		gpx.EditItem(id, COL_COORDINATES, jp->getCellCoordinates());
		gpx.EditItem(id, COL_OWNER, ToString(jp->getOwner()));
		gpx.EditItem(id, COL_ZONE,  "Unknown");

		// add to list of found
		search_results.add(jp->getAutoNumber());
	}
}

void JumpManagerDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL JumpManagerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create columns
	c_list.InsertColumn(COL_NAME, "Name", 0, 140);
	c_list.InsertColumn(COL_COORDINATES, "Coordinates", 0, 100);
	c_list.InsertColumn(COL_OWNER, "Owner", 0, 60);
	c_list.InsertColumn(COL_ZONE, "Zone", 0, 80);

	// format
	c_list.useFullRowSelect().useGridlines();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void JumpManagerDialog::OnList_DoubleRightClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	// lookup the id
	int id = c_list.GetByHitTest();
	if (id == -1)
		return;

	// lookup the item data for that row
	int auto_id = (int)c_list.GetItemData(id);
	
	// remove from the list of beacons
	Beacons::Kernel::getInst().deleteByAutoId(auto_id);
	c_list.DeleteItem(id);
}

void JumpManagerDialog::OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	// lookup the id
	int id = c_list.GetByHitTest();
	if (id == -1)
		return;

	// lookup the jump pont
	int row_id = (int)c_list.GetItemData(id);
	Beacons::Beacon* pJump = Beacons::Kernel::getInst().getByAutoID(row_id);
	if (pJump == 0)
	{
		MessageBox("Jump Point can no longer be found. It may have been deleted.");
		c_list.DeleteItem(id);
	}
	else
	{
		JumpEditorDialog jd;
		jd.v_name = pJump->getName();
		jd.v_coordinates = pJump->getCoordinates();
		jd.v_owner = pJump->getOwner();

		// require positive confirmation
		if (jd.DoModal() != IDOK)
			return;

		// update the list
		pJump = Beacons::Kernel::getInst().getByAutoID(row_id);
		if (pJump == 0)
		{
			MessageBox("The jump point has gone away.");
		}
		else
		{
			// update jump point
			pJump->name = jd.v_name;
			pJump->updatePosition(jd.v_coordinates);
			pJump->owner = jd.v_owner;

			// update the list
			c_list.EditItem(id, COL_NAME, pJump->getName());
			c_list.EditItem(id, COL_COORDINATES, pJump->getCellCoordinates());
			c_list.EditItem(id, COL_OWNER, ToString(pJump->getOwner()));
			c_list.EditItem(id, COL_ZONE,  "Unknown");
		}
	}
}

void JumpManagerDialog::OnJumps_Import()
{
	// openimporter
	CFileDialog dlg(TRUE, ".xms", "beacons.xms", 4 | 2, "Eclipse Beacons (*.xms)|*.xms||", this);
	if (dlg.DoModal() != IDOK) return;

	// import list
	size_t prev = Beacons::Kernel::getInst().size();
	bool load = Beacons::Kernel::getInst().load(dlg.GetPathName(), false);

	// number of items loaded
	String log;
	log.Format("%d additional jump points were imported", (int)(Beacons::Kernel::getInst().size() - prev));
	MessageBox(log);
}

void JumpManagerDialog::OnJumps_ImportOld()
{
	MessageBox("Enhanced Processor is not installed; please us Import Jumps");
}

void JumpManagerDialog::OnJumps_Export()
{
	// open exporter
	CFileDialog dlg(FALSE, ".xms", "beacons.xms", 4 | 2, "Eclipse Beacons (*.xms)|*.xms||", this);
	if (dlg.DoModal() != IDOK) return;

	// write out
	bool err = Beacons::Kernel::getInst().save(dlg.GetPathName());
	if (err == false)
	{
		MessageBox("There was a problem exporting the jump point list", "Error", MB_ICONWARNING);
	}
	else
	{
		MessageBox(String("The jump point list has been exported to: ") + dlg.GetPathName(), "Export", MB_ICONINFORMATION);
	}
}

void JumpManagerDialog::OnJumps_InsertNew()
{
	// ask for the editor and only proceed if ok'd
	JumpEditorDialog jd;
	if (jd.DoModal() != IDOK) 
		return;

	// search for an existing beacon by that name
	if (Beacons::Kernel::getInst().Search(jd.v_name) != NULL)
	{
		MessageBox("Unable to add that jump point; the name is already in use", "Insert Error", MB_ICONWARNING);
	}
	else
	{
		Beacons::Kernel::getInst().Insert(jd.v_owner, jd.v_name, jd.v_coordinates);
		MessageBox("Jump Point has been added");
	}
}

void JumpManagerDialog::OnJumps_Reset()
{
	if (MessageBox("WARNING\n\n"
								 "This will erase the entire Jump Point list and is not reversable\n\n"
								 "Areyou sure you wish to continue?",
								 "Reset List",
								 MB_ICONQUESTION | MB_YESNOCANCEL) == IDYES)
	{
		Beacons::Kernel::getInst().Erase();
		MessageBox("The Jump Points list has been reset!", "Jump Reset", MB_ICONINFORMATION);
	}
}

void JumpManagerDialog::OnSearch_ExportResults()
{
	MessageBox("Not available at this time...");
}

void JumpManagerDialog::OnSearch_EmptyResults()
{
	search_results.clear();
	c_list.DeleteAllItems();
}

void JumpManagerDialog::OnSearch_TransferOwner()
{
	// Transfer Ownership
	JumpTransferOwnershipDialog diag;
	if (diag.DoModal() != IDOK)
		return;

	// carry out transfer
	Beacons::Kernel::getInst().transferOwnership(search_results, diag.v_citizen);
	MessageBox("Jump Points have been transfered to the given citizen number", "Jump Transfer", MB_ICONINFORMATION);

	// rebuild the list using the existing search
	OnBnClickedSearch();
}

void JumpManagerDialog::OnSearch_EraseAll()
{
	if (MessageBox("WARNING\n\n"
								 "This will erase all Jump Points in the current search results and is not reversable\n\n"
								 "Areyou sure you wish to continue?",
								 "Reset Search",
								 MB_ICONQUESTION | MB_YESNOCANCEL) == IDYES)
	{
		// carry out the deletion
		Beacons::Kernel::getInst().deleteByProcessingList(search_results);
		search_results.clear();
		c_list.DeleteAllItems();

		// notify
		MessageBox("Deleted", "Jump Delete", MB_ICONINFORMATION);
	}
}

void JumpManagerDialog::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

#include "ModalPropertyDialog2.h"
void JumpManagerDialog::OnJumps_ConfigOptions()
{
	ModalPropertyDialog2 d(this);
	
	// rig for update
	Beacons::Config::PropertyBuilder builder;
	Beacons::Config& data = Beacons::Kernel::getInst().config;
	builder.assign_remote(data);
	d.m_builderBase = &builder;	
	d.title = "Jump Configuration";		

	// popup info
	if (d.DoModal() != IDOK)
		return;
}
