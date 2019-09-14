// MoversDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "MoversDialog.h"
#include "movers.h"
#include "movers_autodriver.h"
#include "singlelinedialog.h"

// MoversDialog dialog
using namespace Movers;

IMPLEMENT_DYNAMIC(MoversDialog, CDialog)

MoversDialog::MoversDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MoversDialog::IDD, pParent)
{

}

MoversDialog::~MoversDialog()
{
}

void MoversDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, c_list);
}


BEGIN_MESSAGE_MAP(MoversDialog, CDialog)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &MoversDialog::OnList_DoubleClick)
	ON_COMMAND(ID_MOVERS_CREATENEWDRIVER, &MoversDialog::OnCreateNewDriver)
	ON_BN_CLICKED(IDC_BUTTON1, &MoversDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// MoversDialog message handlers

BOOL MoversDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create a gui timer
	SetTimer(1, 2000, NULL);

	// create items
	c_list.InsertColumn(0, "Mover", 0, 50);
	c_list.InsertColumn(1, "Position", 0, 200);

	// reload each item into the list
	reload();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MoversDialog::reload()
{
	// erase the list
	c_list.DeleteAllItems();

	// add each element to the list
	Movers::AutoDriverKernel& kernel = Movers::AutoDriverKernel::getInstance();
	for (Movers::AutoDriverKernel::iterator ptr = kernel.begin();
		ptr != kernel.end();
		ptr++)
	{
		int id = c_list.InsertItem(c_list.GetItemCount(), ptr->id);
		c_list.SetItemData(id, ptr->auto_id);
	}
}

void MoversDialog::OnTimer(UINT_PTR nIDEvent)
{
	// update each of the movers
	for (int i = 0; i < c_list.GetItemCount(); i++)
	{
		int ptr = (int)c_list.GetItemData(i);
		Movers::AutoDriver* pDriver = Movers::AutoDriverKernel::getInstance().getEntryByAuto(ptr);
	}

	CDialog::OnTimer(nIDEvent);
}

#include "Movereditordialog.h"

void MoversDialog::OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// hit test on the user
	int pos = c_list.GetByHitTest();
	if (pos == -1)
		return;

	int id = (int)c_list.GetItemData(pos);
	Movers::AutoDriver* pWe = Movers::AutoDriverKernel::getInstance().getEntryByAuto(id);
	if (pWe)
	{
		vntSelected = pWe->getUmbraRemoteClass();
		SetWindowText(String("Mover Control: ") + pWe->id);
	}

	*pResult = 0;
}

void MoversDialog::OnCreateNewDriver()
{
	const char* pName = 0;
	if (pName = SingleLineDialog::GetInputLine(this, "Please enter tha name of the mover to track", "New Mover"))
	{
		Movers::AutoDriver* pDriver = Movers::AutoDriverKernel::getInstance().createNew(pName);
		if (pDriver == 0)
		{
			MessageBox("You cannot create another mover using the name of one that already exists");
		}
		else
		{
			// it exists
			reload();
		}
	}
}

#include "MovementDialog.h"
void MoversDialog::OnBnClickedButton1()
{
	// lookup the currently selected driver and ensure it has not yet expired
	Movers::AutoDriver* pDriver = dynamic_cast<Movers::AutoDriver*>(vntSelected.toRemoteClass());
	if (pDriver == 0)
		return;

	// load up the viewer
	MovementDialog d;
	d.controlEditor = pDriver->ctrl.getUmbraRemoteClass();
	d.DoModal();
}
