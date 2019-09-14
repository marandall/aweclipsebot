// isScriptCommandsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "isScriptCommandsDialog.h"
#include "CisScriptCommands.h"

// CisScriptCommandsDialog dialog

IMPLEMENT_DYNAMIC(CisScriptCommandsDialog, CDialog)

CisScriptCommandsDialog::CisScriptCommandsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CisScriptCommandsDialog::IDD, pParent)
{

}

CisScriptCommandsDialog::~CisScriptCommandsDialog()
{
}

void CisScriptCommandsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEMS, c_list);
}


BEGIN_MESSAGE_MAP(CisScriptCommandsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CisScriptCommandsDialog::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_ITEMS, &CisScriptCommandsDialog::OnScriptedCis_DoubleClick)
	ON_COMMAND(ID_COMMANDS_CREATENEWMAPPING, &CisScriptCommandsDialog::OnCommands_CreateNewMapping)
END_MESSAGE_MAP()


// CisScriptCommandsDialog message handlers

void CisScriptCommandsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CisScriptCommandsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set style
	c_list.SetExtendedStyle(c_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	

	// insert every item
	c_list.InsertColumn(0, _T("Command ID"), 0, 80);
	c_list.InsertColumn(1, _T("Function"), 0, 100);
	c_list.InsertColumn(2, _T("Context"),  0, 150);
	c_list.InsertColumn(3, _T("Requires"), 0, 150);

	// assign the information
	command_copy = CIS::ScriptCommands::getInstance();
	refreshCommands();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CisScriptCommandsDialog::refreshCommands()
{
	// reset the list
	c_list.DeleteAllItems();

	// show each element
	for (CIS::ScriptCommands::Iterator itr = command_copy.begin(); itr != command_copy.end(); itr++)
	{
		// create a known list of context items
		KnownList context;
		if (itr->getContextAvatar() == true) context.push_back("Avatar");
		if (itr->getContextConsole() == true) context.push_back("Console");
		
		// create a known list of requirements
		KnownList requirements;
		if (itr->getRequiresCaretaker() == true) requirements.push_back("Caretaker");
		if (itr->getRequiresEject() == true) requirements.push_back("Eject");
		if (itr->getRequiresUniverse() == true) requirements.push_back("UniCon");
		if (itr->getRequiresWorld() == true) requirements.push_back("WorldCon");

		// insert an item
		int id = c_list.InsertItem(c_list.GetItemCount(), itr->getId());
		c_list.EditItem(id, 1, itr->getScriptFunction());
		c_list.EditItem(id, 2, context.toString(","));
		c_list.EditItem(id, 3, requirements.toString(","));
		c_list.SetItemData(id, (DWORD_PTR)itr->getAutoNumber());
	}
}

void CisScriptCommandsDialog::OnOK()
{
	CIS::ScriptCommands::getInstance() = command_copy;
	CDialog::OnOK();
}

#include "ModalPropertyDialog2.h"
void CisScriptCommandsDialog::OnScriptedCis_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// hit test
	int index = c_list.GetByHitTest();
	if (index == -1) return;
	int id = (int)c_list.GetItemData(index);

	// lookup the script
	CIS::ScriptedCommand* pScrCmd = command_copy.getAuto(id);
	if (pScrCmd == 0) return;

	// pop the dilaog
	ModalPropertyDialog2 d(this);
	CIS::Config::AutoTemplates::ScriptedCommandPropertyBuilder builder;
	builder.assign_remote(*pScrCmd);
	d.m_builderBase = &builder;
	d.title = "Command Script Bindings";
	d.DoModal();

	// refresh the list?
	refreshCommands();
}

#include "SingleLineDialog.h"
void CisScriptCommandsDialog::OnCommands_CreateNewMapping()
{
	// configure dialog
	SingleLineDialog d;
	d.v_about = "Please enter unique identifier for the command, it may not contain any spaces";
	d.v_title = "Enter ID";
	
	// do modal
	if (d.DoModal() != IDOK)
		return;

	// add an item
	CIS::ScriptedCommand cs;
	cs.id = d.v_text;
	command_copy.push_back(cs);

	// refresh the list?
	refreshCommands();
}
