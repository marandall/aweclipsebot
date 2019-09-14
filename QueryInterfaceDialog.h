#pragma once
#include "GNet_QueryMap.h"
#include "GraphicsLabel.h"
#include "treectrlgpx_std.h"
#include "textbox.h"
#include "afxwin.h"
#include "PropertyScriptCtrl.h"
#include "property_filters.h"
#include "property_editors.h"
#include "layout_manager.h"
#include "property_project.h"
#include "afxcmn.h"
#include "listener.h"
#include "ButtonListControl.h"

// QueryInterfaceDialog dialog
using namespace Property;

class QueryInterfaceDialog : 
	public CDialog, 
	public Listener, 
	public GraphicsLabelHandler
{
	DECLARE_DYNAMIC(QueryInterfaceDialog)

public:
	QueryInterfaceDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryInterfaceDialog();
	
public: // message handling
	void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo);

	void OnGLH_Event(GraphicsLabel& Control, const char*, void* Data);

// Dialog Data
	enum { IDD = IDD_QUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
public:
	// the current selected item
	int							m_selectedFilter;
	Property::Filters::Filter*	m_pFilterPtr;
	Property::Editors::Editor*	m_pEditor;
	ButtonListControl			c_buttons;


	void		changeFilter(int Type, Property::Filters::Filter* Filter);
	void		changeEditor(int Type, Property::Editors::Editor* Filter);
	void		writeLog(const char* Text);

	// layout manager
	LayoutManager::BorderLayout layout_mgr;

public: // project
	Property::Project*	getProject();
	void				openProject(Property::Project& Proj);
	CStringA			projectName;


public: // mapping entity
	GraphicsDisplay::QueryMap			m_query_map;

protected: // menu editing
	void		enableMenu(int Id);
	void		disableMenu(int Id);
	void		changeMenuEnabled(int Id, bool State);

public:
	TreeCtrlGpx_Std			c_CommandTree;
	TextBox					c_Log;
	GraphicsLabel			c_Map;
	PropertyBuilderHelper	c_PsHelper;
	UINT_PTR				redrawTimer;		

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcDestroy();
	virtual BOOL OnInitDialog();
	CStatic c_TrackingRect;
	afx_msg void OnControlTree_Click(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTree_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFilters_Citizen();
	afx_msg void OnFilters_UserType();
	afx_msg void OnFilters_SelectionRect();
	afx_msg void OnBnClickedFilterSelect();
	afx_msg void OnProject_OpenPropertyProject();
	afx_msg void OnProject_SaveV4();
	afx_msg void OnBnClickedFilterUnselect();
	afx_msg void OnFilters_StringFinder();
	afx_msg void OnFilters_UseZones();
	afx_msg void OnFilters_UseInequality();
	afx_msg void OnFilter_UseTypeFinder();
	afx_msg void OnFilters_UseParticleBounds();
	afx_msg void OnFilters_UseZoneBounds();
	afx_msg void OnFilters_UseMoverBounds();
	afx_msg void OnQuery_5x5();
	afx_msg void OnFilters_Select();
	afx_msg void OnFilter_Deselect();
	afx_msg void OnProject_OpenV4();
	afx_msg void OnProject_SaveProject();
	afx_msg void OnProject_SaveV2();
	afx_msg void OnProject_SaveV3();
	afx_msg void OnFilters_DetailSelected();
	afx_msg void OnOperations_Duplicate();
	afx_msg void OnEditors_Run();
	afx_msg void OnBuilding_BeginConstructing();
	afx_msg void OnBuilding_BeginLoading();
	afx_msg void OnBuilding_BeginDeleting();
	afx_msg void OnBuilding_StopBuilding();
	afx_msg void OnEditors_DeleteSelected();
	afx_msg void OnContext_SelectSector();
	afx_msg void OnSelection_Invert();
	afx_msg void OnBuilding_AddSelectionToQueue();
	afx_msg void OnBuilding_AddEverythingToQueue();
	afx_msg void OnView_SwitchSelectionView();
	afx_msg void OnView_SwitchBuildStateView();
	afx_msg void OnProject_NewFromSelection();
	afx_msg void OnClose();
	afx_msg void OnContext_SelectRectangle();
	afx_msg void OnBnClickedFilterUsertype();
	afx_msg void OnProject_New();
	afx_msg void OnQuery_BeginLarge();
	afx_msg void OnView_SwitchToCellView();
	afx_msg void OnQuery_WholeWorld();
};
