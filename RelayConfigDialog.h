#pragma once
#include "PropertyScriptCtrl.h"
#include "RelayRelay.h"
#include "afxwin.h"
#include "afxcmn.h"

// RelayConfigDialog dialog

class RelayConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(RelayConfigDialog)

public:
	RelayConfigDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~RelayConfigDialog();

// Dialog Data
	enum { IDD = IDD_RELAY_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	PropertyBuilderHelper c_PsHelper;

	DECLARE_MESSAGE_MAP()

protected:
	ChatControllers::AutoTemplates::RelayPropertyBuilder				m_dsRelay;
	ChatControllers::AutoTemplates::GlobalChatPropertyBuilder		m_dsGlobalChat;
	ChatControllers::AdvancedRelayKernel::PropertyBuilder		m_dsTotal;

public:
	CStatic c_RectTracker;
	CTabCtrl c_Tabs;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnTabs_SelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGlobalStylus();
};
