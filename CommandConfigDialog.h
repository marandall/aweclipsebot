#pragma once
#include "PropertyScriptCtrl.h"
#include "CIS_Configuration.h"
#include "afxcmn.h"
#include "afxwin.h"

// CommandConfigDialog dialog

class CommandConfigDialog : 
	public CDialog
{
	DECLARE_DYNAMIC(CommandConfigDialog)

public:
	CommandConfigDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CommandConfigDialog();

// Dialog Data
	enum { IDD = IDD_COMMAND_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

public: // the config sets
	CIS::Config::AutoTemplates::EjectionPropertyBuilder		m_dsEjection;
	CIS::Config::AutoTemplates::InterfacePropertyBuilder	m_dsInterface;
	CIS::Config::AutoTemplates::JumpPointsPropertyBuilder	m_dsJumpPoints;

public: // support objects that are needed
	PropertyBuilderHelper	c_PsHelper;
	CTabCtrl				c_Tabs;
	CStatic					c_RectTracker;
	afx_msg void OnTabs_Change(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedApply();
};
