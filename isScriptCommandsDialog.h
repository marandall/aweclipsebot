#pragma once
#include "CisScriptCommands.h"
#include "listctrlgpx.h"

// CisScriptCommandsDialog dialog

class CisScriptCommandsDialog : public CDialog
{
	DECLARE_DYNAMIC(CisScriptCommandsDialog)

public:
	CisScriptCommandsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CisScriptCommandsDialog();

// Dialog Data
	enum { IDD = IDD_CIS_SCRIPT_COMMANDS };

public: // a copy of the scripted commands
	CIS::ScriptCommands command_copy;
	void refreshCommands();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	ListCtrlGpx c_list;
protected:
	virtual void OnOK();
public:
	afx_msg void OnScriptedCis_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCommands_CreateNewMapping();
};
