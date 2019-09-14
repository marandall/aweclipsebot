#pragma once
#include "afxwin.h"
#include "colourbutton.h"
#include "PropertyScriptCtrl.h"
#include "afxcmn.h"
#include "awsdk_world.h"
#include "worldoptionsdialog_ag.h"
#include "worldattributes.h"

class WorldOptionsDialog : public CDialog
{
	DECLARE_DYNAMIC(WorldOptionsDialog)

public:
	WorldOptionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~WorldOptionsDialog();

// Dialog Data
	enum { IDD = IDD_WORLD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP();

	void create_group(int ID, CStringA Title);
public:
	PropertyScriptCtrl* c_pProperties;
	virtual BOOL OnInitDialog();
	CStatic c_TrackingRect;
	afx_msg void OnBnClickedGeneral();
	CTabCtrl c_Tabs;
	afx_msg void OnTabs_SwitchTab(NMHDR *pNMHDR, LRESULT *pResult);

public: // automatically generated functions
	void	select_combo_by_index(CComboBox& CB, int Index);
	void	create_objects();
	void	attributes_to_screen(WorldAttributes& attr);
	void	screen_to_attributes(WorldAttributes& attr);

public: // world attributes information
	WORLDOPTIONS_OBJECTS controls;
	WorldAttributes	attributes;

public:
	afx_msg void OnFile_SaveAttributes();
	afx_msg void OnFile_OpenAttributes();
	afx_msg void OnSdk_Reload();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
};
