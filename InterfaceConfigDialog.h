#pragma once
#include "InterfaceConfig.h"

// InterfaceConfigDialog dialog
using namespace InterfaceConfig;

class InterfaceConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(InterfaceConfigDialog)

public:
	InterfaceConfigDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~InterfaceConfigDialog();

// Dialog Data
	enum { IDD = IDD_INTERFACE_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl c_Tabs;
	CStatic c_TrackingRect;

protected:
	PropertyBuilderHelper								c_PsHelper;
	InterfaceConfig::ChatDisplay::PropertyBuilder		m_dsChatDisplay;
	InterfaceConfig::Logging::PropertyBuilder			m_dsLogging;

public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnTabs_ChangeSel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
