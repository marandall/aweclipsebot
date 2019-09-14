#pragma once
#include "afxcmn.h"
#include "textbox.h"
#include "zones.h"
#include "afxwin.h"
#include "listctrlgpx.h"

namespace Zones { class Zone; }

// ZoneEditorDialog dialog
class ZoneEditorDialog : public CDialog
{
	DECLARE_DYNAMIC(ZoneEditorDialog)

public:
	ZoneEditorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZoneEditorDialog();

// Dialog Data
	enum { IDD = IDD_ZONES_EDITOR };

protected:
	void	SelectZone(Zones::Zone& ZoneInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HTREEITEM	EnumerateTree(Zones::Zone& ZoneData, HTREEITEM ParentNode);
	HTREEITEM	LookupTreeByID(int id);
	void		SwitchZone(int zone_id);

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl c_Tree;
	virtual BOOL OnInitDialog();
	void rebuild_region_list();
	void insert_region_list(int ID, CString Name, CStringA Type, CStringA Description);

	int		menu_zone_id;
	int		selected_id;
	Zones::Zone	root;

public:
	afx_msg void OnTree_SelectionChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTree_NodeSelect(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContext_CreateChild();
	afx_msg void OnManage_Save();
	ListCtrlGpx c_RegionsList;
	afx_msg void OnBnClickedNewRect();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedNewCylinder();
	afx_msg void OnRegionList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void OnOK();
public:
	afx_msg void OnContext_DeleteZone();
	afx_msg void OnBnClickedOk();

public: // config dialog
	PropertyBuilderHelper c_PsHelper;
	Zones::Zone::PropertyBuilder m_dsZone;
	CStatic c_RectTracker;
	afx_msg void OnLvnItemchangedRegions(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenu_DeleteChildren();
};
