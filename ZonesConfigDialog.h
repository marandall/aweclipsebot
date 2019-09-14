#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "PropertyScriptCtrl.h"
#include "Zones_ConfigGeneral_AG.h"
#include "Zones_ConfigHyperion_AG.h"

// ZonesConfigDialog dialog

class ZonesConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(ZonesConfigDialog)

public:
	ZonesConfigDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZonesConfigDialog();

// Dialog Data
	enum { IDD = IDD_ZONES_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void OnOK();

public: // property builder stuff
	PropertyBuilderHelper	c_PsHelper;
	Zones::AutoTemplates::ConfigGeneralPropertyBuilder	m_dsGeneral;
	Zones::AutoTemplates::ConfigHyperionPropertyBuilder	m_dsHyperion;

	CTabCtrl c_Tabs;
	CStatic c_RectTracker;
	afx_msg void OnTabs_Changed(NMHDR *pNMHDR, LRESULT *pResult);
};
