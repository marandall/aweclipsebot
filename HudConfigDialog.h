#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "PropertyScriptCtrl.h"
#include "headsup_storedtrayitem.h"
#include "headsup.h"
#include "listctrlgpx.h"

// HudConfigDialog dialog

class HudConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(HudConfigDialog)

public:
	HudConfigDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~HudConfigDialog();

// Dialog Data
	enum { IDD = IDD_HUD_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // the list of items
	HeadsUp::Config::TrayItemArray			icon_array;
	HeadsUp::StoredTrayItem*				current_icon;

public: // controls for the page 
	CStatic	 c_RectTracker;
	CTabCtrl c_Tabs;

protected: // script builders
	PropertyBuilderHelper	c_psc_config;
	PropertyBuilderHelper	c_psc_stored;

protected: // control builders
	HeadsUp::Config::PropertyBuilder		 m_dsConfig;
	HeadsUp::StoredTrayItem::PropertyBuilder m_dsStored;

public: // auto adding
	void InsertItemToList(HeadsUp::StoredTrayItem& Sti, bool Insert);
	void SelectItem(HeadsUp::StoredTrayItem* Sti);

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnTabs_SelChange(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnOK();
public:
	CStatic c_location;
	afx_msg void OnIcons_CreateIcon();
	CStatic c_location2;
	ListCtrlGpx c_list;
	afx_msg void OnIconList_IconActivated(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddItem();
	afx_msg void OnIcons_Clicked(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedClearItem();
};
