// QueryInterfaceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryInterfaceDialog.h"
#include "QueryFilterDialog.h"
#include "property_projects.h"
#include "dialogcore.h"
#include "property_query_world.h"

// QueryInterfaceDialog dialog

IMPLEMENT_DYNAMIC(QueryInterfaceDialog, CDialog)

const int REDRAW_TIMER_EVENT = 5;

QueryInterfaceDialog::QueryInterfaceDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryInterfaceDialog::IDD, pParent)
{
	projectName = "__live";
}

QueryInterfaceDialog::~QueryInterfaceDialog()
{
}

void QueryInterfaceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, c_CommandTree);
	DDX_Control(pDX, IDC_EDIT1, c_Log);
	DDX_Control(pDX, IDC_MAP_ELEMENT, c_Map);
}


BEGIN_MESSAGE_MAP(QueryInterfaceDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &QueryInterfaceDialog::OnControlTree_Click)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &QueryInterfaceDialog::OnTree_DoubleClick)
	ON_COMMAND(ID_SELECTIONFILTERS_USECITIZEN, &QueryInterfaceDialog::OnFilters_Citizen)
	ON_COMMAND(ID_SELECTIONFILTERS_USEUSERTYPE, &QueryInterfaceDialog::OnFilters_UserType)
	ON_COMMAND(ID_SELECTIONFILTERS_USESELECTIONRECTANGLE, &QueryInterfaceDialog::OnFilters_SelectionRect)
	ON_BN_CLICKED(IDC_FILTER_SELECT, &QueryInterfaceDialog::OnBnClickedFilterSelect)
	ON_COMMAND(ID_OPEN_PROPERTYPROJECT, &QueryInterfaceDialog::OnProject_OpenPropertyProject)
	ON_COMMAND(ID_SAVEAS_PROPDUMPVERSION4, &QueryInterfaceDialog::OnProject_SaveV4)
	ON_BN_CLICKED(IDC_FILTER_UNSELECT, &QueryInterfaceDialog::OnBnClickedFilterUnselect)
	ON_COMMAND(ID_QUERY_BEGIN5X5, &QueryInterfaceDialog::OnQuery_5x5)
	ON_COMMAND(ID_SELECTIONFILTERS_RUN, &QueryInterfaceDialog::OnFilters_Select)
	ON_COMMAND(ID_SELECTIONFILTERS_RUN33224, &QueryInterfaceDialog::OnFilter_Deselect)
	ON_COMMAND(ID_OPEN_PROPDUMPVERSION4, &QueryInterfaceDialog::OnProject_OpenV4)
	ON_COMMAND(ID_SAVEAS_PROPERTYPROJECT, &QueryInterfaceDialog::OnProject_SaveProject)
	ON_COMMAND(ID_SAVEAS_PROPDUMPVERSION2, &QueryInterfaceDialog::OnProject_SaveV2)
	ON_COMMAND(ID_SAVEAS_PROPDUMPVERSION3, &QueryInterfaceDialog::OnProject_SaveV3)
	ON_COMMAND(ID_SELECTIONFILTERS_DETAILSELECTED, &QueryInterfaceDialog::OnFilters_DetailSelected)
	ON_COMMAND(ID_OPERATIONS_DUPLICATE, &QueryInterfaceDialog::OnOperations_Duplicate)
	ON_COMMAND(ID_EDITORS_RUN, &QueryInterfaceDialog::OnEditors_Run)
	ON_COMMAND(ID_BUILDING_BEGINCONSTRUCTING, &QueryInterfaceDialog::OnBuilding_BeginConstructing)
	ON_COMMAND(ID_BUILDING_BEGINLOADING, &QueryInterfaceDialog::OnBuilding_BeginLoading)
	ON_COMMAND(ID_BUILDING_BEGINDELETING, &QueryInterfaceDialog::OnBuilding_BeginDeleting)
	ON_COMMAND(ID_BUILDING_STOPBUILDING, &QueryInterfaceDialog::OnBuilding_StopBuilding)
	ON_COMMAND(ID_OPERATIONS_DELETESELECTED, &QueryInterfaceDialog::OnEditors_DeleteSelected)
	ON_COMMAND(ID_CONTEXT_SELECTSECTOR, &QueryInterfaceDialog::OnContext_SelectSector)
	ON_COMMAND(ID_SELECTIONFILTERS_INVERT, &QueryInterfaceDialog::OnSelection_Invert)
	ON_COMMAND(ID_BUILDING_ADDSELECTIONTOQUEUE, &QueryInterfaceDialog::OnBuilding_AddSelectionToQueue)
	ON_COMMAND(ID_BUILDING_ADDEVERYTHINGTOQUEUE, &QueryInterfaceDialog::OnBuilding_AddEverythingToQueue)
	ON_COMMAND(ID_VIEW_SWITCHTOSELECTIONVIEW, &QueryInterfaceDialog::OnView_SwitchSelectionView)
	ON_COMMAND(ID_VIEW_SWITCHTOBUILDSTATEVIEW, &QueryInterfaceDialog::OnView_SwitchBuildStateView)
	ON_COMMAND(ID_PROJECT_NEWPROJECT33192, &QueryInterfaceDialog::OnProject_NewFromSelection)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CONTEXT_SELECTRECTANGLE, &QueryInterfaceDialog::OnContext_SelectRectangle)
	ON_COMMAND(ID_PROJECT_NEWPROJECT, &QueryInterfaceDialog::OnProject_New)
	ON_COMMAND(ID_QUERYCONTROL_BEGINLARGEQUERY, &QueryInterfaceDialog::OnQuery_BeginLarge)
	ON_COMMAND(ID_VIEW_SWITCHTOCELLVIEW, &QueryInterfaceDialog::OnView_SwitchToCellView)
	ON_COMMAND(ID_QUERY_BEGINWHOLEWORLD, &QueryInterfaceDialog::OnQuery_WholeWorld)
END_MESSAGE_MAP()


// QueryInterfaceDialog message handlers
Property::Project* QueryInterfaceDialog::getProject()
{
	Property::Project* proj = Property::g_Projects.Find(projectName);
	if (proj == NULL)
	{
		MessageBox("The requested project no longer exists");
		OnOK();
		return 0;
	}
	else
	{
		return proj;
	}
}

void QueryInterfaceDialog::openProject(Property::Project& Proj)
{
	projectName = Proj.id;
	Proj.editor_wnd = this;
}

void QueryInterfaceDialog::OnSize(UINT nType, int cx, int cy)
{
	// configure
	layout_mgr.layout(0, 0, cx, cy);
	CDialog::OnSize(nType, cx, cy);

	// update map
	// m_query_map.setScale(map_width, map_height, m_query_map.m_visible_units);
	m_query_map.Reset();
	Invalidate(FALSE);
}

#include "EclipseEvolutionDialog.h"
void QueryInterfaceDialog::OnNcDestroy()
{
	// unlock project
	// set parent
	SetParent(rt_diag);
	Project* p = getProject();
	if (p) { p->editor_wnd = 0; }

	CDialog::OnNcDestroy();
	delete this;
}

enum 
{
	GR_FILTER_CITIZENS,
	GR_FILTER_INEQUALITY,
	GR_FILTER_MOVER_BOUNDS,
	GR_FILTER_PARTICLE_BOUNDS,
	GR_FILTER_SELECTION_RECT,
	GR_FILTER_STRING_FINDER,
	GR_FILTER_USER_TYPE,
	GR_FILTER_ZONE_BOUNDS,
	GR_FILTER_TYPE_FINDER,
	GR_FILTER_ZONES,
	GR_EDITOR_DUPLICATE,
	GR_EDITOR_STRING_REPLACE,
	GR_EDITOR_TRANSLATE
};

#define IMPL_CREATE_GROUP(group, id, name, Mode)	\
	c_PsHelper.create_group(id, name, group.Mode .assign_remote( group##_data.Mode ));

// the list of elements
enum 
{
	MV_NORTH,
	MV_SOUTH,
	MV_EAST,
	MV_WEST,
	MV_ZOOMIN,
	MV_ZOOMOUT,
	MV_MOVETO,
	
	// filters
	FL_CITIZENS,
	FL_INEQUALITY,
	FL_MOVERBOUNDS,
	FL_PARTICLEBOUNDS,
	FL_ZONEBOUNDS,
	FL_RECTANGLE,
	FL_STRINGFINDER,
	FL_TYPE,
	FL_USERTYPE,
	FL_CLEARSELECTION,
	FL_INVERTSELECTION,

	// edits the template by deleting part of it
	ED_DELETE_SELECTED,
	ED_CROP,

	// other transformations
	ED_DUPLICATE,
	ED_STRINGREPLACE,
	ED_TRANSLATE,

	// deals with RWX styles
	ED_APPENDRWX,
	ED_STRIPRWX,

	// quick translations of the current selection
	ED_TRANSLATE50N,
	ED_TRANSLATE50S,
	ED_TRANSLATE50E,
	ED_TRANSLATE50W,
};

BOOL QueryInterfaceDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// format tree
	SetWindowLong(c_CommandTree.m_hWnd, GWL_STYLE, GetWindowLong(c_CommandTree.m_hWnd, GWL_STYLE) | TVS_HASBUTTONS | 
						TVS_HASLINES | TVS_LINESATROOT); 


	// configure the editing tree
	TreeCtrlGpx_Std& tree = c_CommandTree;
	tree.insert_node("Movement",		"_move", TVI_ROOT);
	tree.insert_node("North",			"move_north",		"_move");
	tree.insert_node("South",			"move_south",		"_move");
	tree.insert_node("East",			"move_east",		"_move");
	tree.insert_node("West",			"move_west",		"_move");
	tree.insert_node("Zoom In",			"zoom_in",			"_move");
	tree.insert_node("Zoom Out",		"zoom_out",			"_move");
	
	// query map assign
	m_query_map.m_pQuery = &bot.getQuery5x5();
	m_query_map.m_pCache = &bot.getLiveCache();
	m_query_map.setScale(400, 400, 20000);
	m_query_map.changeCenter(bot.getX(), bot.getZ());

	c_Map.m_Surface			= &m_query_map;
	m_query_map.m_pWindow	= &c_Map;
	m_query_map.m_pCache	= &bot.getLiveCache();
	
	// trigger timers
	redrawTimer = SetTimer(REDRAW_TIMER_EVENT, 2000, 0);

	using namespace LayoutManager;

	// grid layout for buttons
	GridLayout* pGrid = new GridLayout(4, 2);
	// statistics panel


	// set control
	c_buttons.Create(ButtonListControl::IDD, this);
	c_buttons.ShowWindow(SW_SHOW);

	{ // add the filter groups
		ButtonListControl::Category& c = c_buttons.addCategory("Navigation", false);
		c.add(MV_NORTH,			"North");
		c.add(MV_SOUTH,			"South");
		c.add(MV_EAST,			"East");
		c.add(MV_WEST,			"West");
		c.add(MV_ZOOMIN,		"Zoom In");
		c.add(MV_ZOOMOUT,		"Zoom Out");
		c.add(MV_MOVETO,		"Move To");
	}

	{ // add the filter groups
		ButtonListControl::Category& c = c_buttons.addCategory("Selection Filters", false);
		c.add(FL_CITIZENS,				"Citizens");
		c.add(FL_INEQUALITY,			"Inequality");
		c.add(FL_MOVERBOUNDS,			"Mover Bounds");
		c.add(FL_PARTICLEBOUNDS,	"Particle Bounds");
		c.add(FL_ZONEBOUNDS,			"Zone Bounds");
		c.add(FL_RECTANGLE,				"Rectangle");
		c.add(FL_STRINGFINDER,		"String Finder");
		c.add(FL_TYPE,						"Type");
		c.add(FL_USERTYPE,				"User Type");
		c.add(FL_CLEARSELECTION,	"Clear Selection");
		c.add(FL_INVERTSELECTION,	"Invert Selection");
	}

	{ // add the filter groups
		ButtonListControl::Category& c = c_buttons.addCategory("Manipulations", false);
		c.add(ED_DUPLICATE,				"Duplicate");
		c.add(ED_STRINGREPLACE,		"String Replace");
		c.add(ED_TRANSLATE,				"Translate");
		c.add(ED_APPENDRWX,				"Append .rwx");
		c.add(ED_STRIPRWX,				"Strip .rwx");

		c.add(ED_DELETE_SELECTED,	"Delete Selection");
		c.add(ED_CROP,						"Crop Selection");
	}

	{ // add the quick shifting button
		ButtonListControl::Category& c = c_buttons.addCategory("Quick Translate", false);
		c.add(ED_TRANSLATE50N, 		"North (50m)");
		c.add(ED_TRANSLATE50S,		"South (50m)");
		c.add(ED_TRANSLATE50E,		"East (50m)");
		c.add(ED_TRANSLATE50W,		"West (50m)");
	}

	// the left hand pane
	BorderLayout* pLeft = new BorderLayout();
	pLeft->add(c_CommandTree,		BorderLayout::PAGE_TOP).assignHeight(75).assignBorder(10);
	pLeft->add(c_buttons,			BorderLayout::CENTER);

	// map and log
	BorderLayout* pMap = new BorderLayout();
	pMap->add(c_Map, BorderLayout::CENTER);
	pMap->add(c_Log, BorderLayout::PAGE_BOTTOM).assignHeight(100);

	// main dialog
	layout_mgr.add(*pLeft, BorderLayout::LEFT).assignWidth(200);
	layout_mgr.add(*pMap,  BorderLayout::CENTER);
	
	CenterWindow();
	RECT rt;
	GetWindowRect(&rt);
	rt.right++;
	MoveWindow(&rt);

	// link project
	connectListener(*getProject());
	connectListener(c_buttons);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

#include "ModalPropertyDialog2.h"

#define GETPSC_OPTIONS_FL(Type, Title)							\
	ModalPropertyDialog2 d(this);								\
	Type::PropertyBuilder builder;								\
	Type data;													\
	builder.assign_remote(data);								\
	d.m_builderBase = &builder;									\
	d.title = Title;											\
	INT_PTR nPtr = d.DoModal();									\
	int num = getProject()->objects.run_filter(&data, true);	\
	CString log;												\
	log.Format("FILTER '%s' selects %d object(s)", Title, num);	\
	writeLog(log);


#define GETPSC_OPTIONS_EDITOR(Type, Title)										\
	ModalPropertyDialog2 d(this);																\
	Type::PropertyBuilder builder;															\
	Type data;																									\
	builder.assign_remote(data);																\
	d.m_builderBase = &builder;																	\
	d.title = Title;																						\
	INT_PTR nPtr = d.DoModal();																	\
	if (nPtr != IDOK) return;																		\
	int num = data.runObjects(getProject()->objects);						\
	CString log;																								\
	log.Format("EDITOR '%s' edits %d object(s)", Title, num);		\
	writeLog(log);												\
	c_Map.Invalidate();

#define GETPSC_OPTIONS_EDITOR2(Type, Title)						\
	Type data;													\
	int num = data.runObjects(getProject()->objects);			\
	CString log;												\
	log.Format("EDITOR '%s' edits %d object(s)", Title, num);	\
	writeLog(log);												\
	c_Map.Invalidate();

#include "inputlinedialog.h"
void QueryInterfaceDialog::onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo)
{
	if (Broadcaster == getProject())
	{
		if (Id == Project::LE_BUILD_STATE_CHANGE)
		{
			Project* p = getProject();
			bool is_building = (p->getJobType() != PROJECT_WORKING);
			bool is_idle	 = (p->getJobType() == PROJECT_WORKING);

			// building menu
			changeMenuEnabled(ID_BUILDING_ADDSELECTIONTOQUEUE,				is_idle);
			changeMenuEnabled(ID_BUILDING_ADDEVERYTHINGTOQUEUE,				is_idle);
			changeMenuEnabled(ID_BUILDING_REMOVESELECTIONFROMQUEUE,		is_idle);
			changeMenuEnabled(ID_BUILDING_REMOVEEVERYTHINGFROMQUEUE,	is_idle);
			changeMenuEnabled(ID_BUILDING_BEGINCONSTRUCTING,					is_idle);
			changeMenuEnabled(ID_BUILDING_BEGINLOADING,								is_idle);
			changeMenuEnabled(ID_BUILDING_BEGINDELETING,							is_idle);
			changeMenuEnabled(ID_BUILDING_PAUSE,											is_idle);
			changeMenuEnabled(ID_BUILDING_STOPBUILDING,								is_building);
		}
	}
	else if (Broadcaster == &c_buttons)
	{
		// button states
		ButtonListControl::ClickNotificationMessage* pNM = dynamic_cast<ButtonListControl::ClickNotificationMessage*>(EventInfo);
		int button = pNM->clicked;
		
		// zooming size
		int move_size = 5000;

		// movement
		if (false) ;
		else if (button == MV_NORTH)
		{
			m_query_map.changeCenter(m_query_map.getX() + 0, m_query_map.getZ() + move_size);
			m_query_map.Reset();
			c_Map.Invalidate(FALSE);
		}
		else if (button == MV_SOUTH)
		{
			m_query_map.changeCenter(m_query_map.getX() + 0, m_query_map.getZ() - move_size);
			m_query_map.Reset();
			c_Map.Invalidate(FALSE);
		}
		else if (button == MV_EAST)
		{
			m_query_map.changeCenter(m_query_map.getX() - move_size, m_query_map.getZ() + 0);
			m_query_map.Reset();
			c_Map.Invalidate(FALSE);
		}
		else if (button == MV_WEST)
		{
			m_query_map.changeCenter(m_query_map.getX() + move_size, m_query_map.getZ() + 0);
			m_query_map.Reset();
			c_Map.Invalidate(FALSE);
		}
		else if (button == MV_ZOOMIN)
		{
			m_query_map.zoom_in(0.10F);
			m_query_map.Reset();
			c_Map.Invalidate(FALSE);
		}
		else if (button == MV_ZOOMOUT)
		{
			m_query_map.zoom_out(0.10F);
			m_query_map.Reset();
			c_Map.Invalidate(FALSE);
		}
		else if (button == MV_MOVETO)
		{
			// ask for coordinates
			InputLineDialog dlg;
			dlg.v_Text = "";
			dlg.title  = "Move Map Viewport";
			if (dlg.DoModal() != IDOK)
				return;

			// move map
			Location c(dlg.v_Text);
			m_query_map.changeCenter(c.getX(), c.getZ());
			m_query_map.Reset();
		}
		
		//
		// filters
		//

		else if (button == FL_CITIZENS) {
			GETPSC_OPTIONS_FL(Filters::Citizens, "Filter by Citizen");
		}
		else if (button == FL_INEQUALITY) {
			GETPSC_OPTIONS_FL(Filters::Inequality, "Filter by Inequalities");
		}
		else if (button == FL_MOVERBOUNDS) {
			GETPSC_OPTIONS_FL(Filters::MoverBounds, "Filter by Mover Bounds");
		}
		else if (button == FL_PARTICLEBOUNDS) {
			GETPSC_OPTIONS_FL(Filters::ParticleBounds, "Filter by Particle Bounds");
		}
		else if (button == FL_ZONEBOUNDS) {
			GETPSC_OPTIONS_FL(Filters::ZoneBounds, "Filter by Zone Bounds");
		}
		else if (button == FL_RECTANGLE) {
			GETPSC_OPTIONS_FL(Filters::SelectionRect, "Filter by Selection Rects");
		}
		else if (button == FL_STRINGFINDER) {
			GETPSC_OPTIONS_FL(Filters::StringFinder, "Filter by String Finder");
		}
		else if (button == FL_TYPE) {
			GETPSC_OPTIONS_FL(Filters::TypeFinder, "Filter by Type");
		}
		else if (button == FL_USERTYPE) {
			GETPSC_OPTIONS_FL(Filters::UserType, "Filter by User Type");
		}
		else if (button == FL_CLEARSELECTION) {
			getProject()->objects.clearSelection();
		}
		else if (button == FL_INVERTSELECTION) {
			getProject()->objects.invertSelection();
		}

		//
		// basic editors
		//

		else if (button == ED_DUPLICATE) {
			GETPSC_OPTIONS_EDITOR(Editors::Duplicate, "Duplicate");
		}
		else if (button == ED_STRINGREPLACE) {
			GETPSC_OPTIONS_EDITOR(Editors::StringReplace, "Find / Replace String");
		}
		else if (button == ED_TRANSLATE) {
			GETPSC_OPTIONS_EDITOR(Editors::Translate, "Translate");
		}
		else if (button == ED_APPENDRWX) {
			GETPSC_OPTIONS_EDITOR2(Editors::AppendRwx, "Append RWX");
		}
		else if (button == ED_STRIPRWX) {
			GETPSC_OPTIONS_EDITOR2(Editors::StripRwx, "Strip RWX");
		}

		//
		// selection editors
		//

		else if (button == ED_DELETE_SELECTED) {
			GETPSC_OPTIONS_EDITOR2(Editors::DeleteSelected, "Delete Selected");
		}		
		else if (button == ED_CROP) {
			GETPSC_OPTIONS_EDITOR2(Editors::Crop, "Crop Selected");
		}		

		//
		// quick translations
		//

		else if (button == ED_TRANSLATE50N)
		{
			Editors::Translate t;
			t.offset_result = AW::Location(0, 0, 5000, 0);
			t.runObjects(getProject()->objects);
		}
		else if (button == ED_TRANSLATE50S)
		{
			Editors::Translate t;
			t.offset_result = AW::Location(0, 0, -5000, 0);
			t.runObjects(getProject()->objects);
		}
		else if (button == ED_TRANSLATE50E)
		{
			Editors::Translate t;
			t.offset_result = AW::Location(-5000, 0, 0, 0);
			t.runObjects(getProject()->objects);
		}
		else if (button == ED_TRANSLATE50W)
		{
			Editors::Translate t;
			t.offset_result = AW::Location(5000, 0, 0, 0);
			t.runObjects(getProject()->objects);
		}
	}
}

void QueryInterfaceDialog::OnControlTree_Click(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM item = c_CommandTree.GetHitTest();
	if (item == 0)
		return;

	// key check
	TreeCtrlGpx_Std& tree = c_CommandTree;
	
	// movement options
	HTREEITEM hti_move_north		= tree["move_north"];
	HTREEITEM hti_move_south		= tree["move_south"];
	HTREEITEM hti_move_east			= tree["move_east"];
	HTREEITEM hti_move_west			= tree["move_west"];
	HTREEITEM hti_zoom_in			= tree["zoom_in"];
	HTREEITEM hti_zoom_out			= tree["zoom_out"];

	if (item == hti_move_north)
	{
		m_query_map.moveNorth(m_query_map.getMoveUnitZ());
		m_query_map.Reset();
		c_Map.Invalidate();
	}
	else if (item == hti_move_south)
	{
		m_query_map.moveSouth(m_query_map.getMoveUnitZ());
		m_query_map.Reset();
		c_Map.Invalidate();
	}

	else if (item == hti_zoom_in)
	{
		m_query_map.zoom_in(0.3F);
		m_query_map.Reset();
		c_Map.Invalidate();
	}
	else if (item == hti_zoom_out)
	{
		m_query_map.zoom_out(0.3F);
		m_query_map.Reset();
		c_Map.Invalidate();
	}
	*pResult = 0;
}

#include "performance.h"

void QueryInterfaceDialog::OnTimer(UINT_PTR nIDEvent)
{
	if ((time64(0) % 5) == 0)
		m_query_map.Reset();

	// guard only to handle on redrawtimer
	if (nIDEvent == REDRAW_TIMER_EVENT)
		c_Map.Invalidate();
	
	CDialog::OnTimer(nIDEvent);
}

void QueryInterfaceDialog::OnTree_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnControlTree_Click(pNMHDR, pResult);
}

void QueryInterfaceDialog::OnFilters_Citizen()
{
}

void QueryInterfaceDialog::changeFilter(int Type, Property::Filters::Filter* Filter)
{
	c_PsHelper.window->show_group(Type);
	m_selectedFilter = Type;

	// highlight
	m_pFilterPtr = Filter;
}

void QueryInterfaceDialog::changeEditor(int Type, Property::Editors::Editor* Filter)
{
	c_PsHelper.window->show_group(Type);
	m_pEditor = Filter;
}

void QueryInterfaceDialog::OnBnClickedFilterSelect()
{
}

void QueryInterfaceDialog::OnProject_OpenPropertyProject()
{
	// open file dialog
	const TCHAR szFilter[] = _T("Property Project (*.e2pp)|*.e2pp||");
	CFileDialog file(TRUE, "e2pp", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// load the file
	getProject()->objects.read_project_v1(file.GetPathName());
}


void QueryInterfaceDialog::OnBnClickedFilterUnselect()
{
	// TODO: Add your control notification handler code here
}

void QueryInterfaceDialog::OnFilters_UserType() {
}

void QueryInterfaceDialog::OnFilters_SelectionRect() {
}

void QueryInterfaceDialog::OnFilters_StringFinder() {
}

void QueryInterfaceDialog::OnFilters_UseZones() {
}

void QueryInterfaceDialog::OnFilters_UseInequality() {
}

void QueryInterfaceDialog::OnFilter_UseTypeFinder() {
}

void QueryInterfaceDialog::OnFilters_UseParticleBounds() {
}

void QueryInterfaceDialog::OnFilters_UseZoneBounds() {
}

void QueryInterfaceDialog::OnFilters_UseMoverBounds() {
}

#include "inputlinedialog.h"

void QueryInterfaceDialog::OnQuery_5x5()
{
	// check that the bot is currently not doing an existing query
	if (bot.getQueryMode() != CommonBot::QUERY_NONE)
	{
		MessageBox(_T("A query is currently running on this bot"), "", MB_ICONEXCLAMATION);
		return;
	}

	// input box for new coordinates
	InputLineDialog req;
	if (req.DoModal() != IDOK)
		return;

	// convert coordinates
	Location queryPos(req.v_Text);
	if (queryPos.isValid() == false)
	{
		MessageBox(_T("You entered invalid query coordinates"), "Error", MB_ICONEXCLAMATION);
		return;
	}

	// start a new query at the location
	bot.Query5x5(queryPos.getCellX(), queryPos.getCellZ());
	m_query_map.changeCenter(queryPos.getX(), queryPos.getZ());
}

void QueryInterfaceDialog::OnFilters_Select()
{
	// convert parameters
	c_PsHelper.dialog_to_attributes();

	// are we ok to proceed?
	m_query_map.m_pCache->run_filter(m_pFilterPtr, true);
}

void QueryInterfaceDialog::OnFilter_Deselect()
{
	// convert parameters
	c_PsHelper.dialog_to_attributes();

	// are we ok to proceed?
	m_query_map.m_pCache->run_filter(m_pFilterPtr, false);
}

void QueryInterfaceDialog::OnProject_OpenV4()
{
	// open file dialog
	const TCHAR szFilter[] = _T("Activeworlds Propdump (*.awpd)|*.awpd||");
	CFileDialog file(TRUE, "awpd", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// read the file
	getProject()->objects.read_propdump(file.GetPathName());
}

void QueryInterfaceDialog::OnProject_SaveProject()
{
	// open file dialog
	const TCHAR szFilter[] = _T("Eclipse Property Project (*.e2pp)|*.e2pp||");
	CFileDialog file(TRUE, "e2pp", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// read the file
	getProject()->objects.write_project_v1(file.GetPathName());
}

void QueryInterfaceDialog::OnProject_SaveV2()
{
	// open file dialog
	const TCHAR szFilter[] = _T("Activeworlds Propdump v2 (*.awpd)|*.awpd||");
	CFileDialog file(TRUE, "awpd", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// read the file
	getProject()->objects.write_propdump(file.GetPathName(), 2);
}

void QueryInterfaceDialog::OnProject_SaveV3()
{
	// open file dialog
	const TCHAR szFilter[] = _T("Activeworlds Propdump v3 (*.awpd)|*.awpd||");
	CFileDialog file(TRUE, "awpd", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// read the file
	getProject()->objects.write_propdump(file.GetPathName(), 3);
}

void QueryInterfaceDialog::OnProject_SaveV4()
{
	// open file dialog
	const TCHAR szFilter[] = _T("Activeworlds Propdump v4 (*.awpd)|*.awpd||");
	CFileDialog file(TRUE, "awpd", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// read the file
	getProject()->objects.write_propdump(file.GetPathName(), 4);
}

#include "property_objects.h"
#include "property_object.h"
#include "PropertyDetailDialog.h"

void QueryInterfaceDialog::OnFilters_DetailSelected()
{
	Property::Objects objs;
	
	// pull out each of the highlighted objects
	for (Property::Objects::iterator ptr = getProject()->objects.begin(); 
		ptr != getProject()->objects.end(); 
		ptr++)
	{
		// add the object if selected
		if (ptr->getSelected())
			objs.insert_object(*ptr);
	}

	// show detail dialog
	PropertyDetailDialog* pDlg = new PropertyDetailDialog(this);
	pDlg->Create(PropertyDetailDialog::IDD, this);
	pDlg->viewList(objs);
	pDlg->ShowWindow(SW_SHOW);
}

void QueryInterfaceDialog::OnOperations_Duplicate() {
}

void QueryInterfaceDialog::OnEditors_Run()
{
	// convert parameters
	c_PsHelper.dialog_to_attributes();

	// check the editor filter
	if (m_pEditor->getEditType() == Property::Editors::Editor::PER_COLLECTION)
	{
		m_pEditor->run(getProject()->objects);
	}
	else
	{
	}

	// are we ok to proceed?
	m_query_map.m_pCache->run_filter(m_pFilterPtr, false);
}

void QueryInterfaceDialog::enableMenu(int Id)
{
	changeMenuEnabled(Id, true);
}

void QueryInterfaceDialog::disableMenu(int Id)
{
	changeMenuEnabled(Id, false); 
}

void QueryInterfaceDialog::changeMenuEnabled(int Id, bool State)
{
	CMenu* mmenu = GetMenu();
	mmenu->EnableMenuItem(Id, State ? MF_ENABLED : MF_GRAYED);
}

void QueryInterfaceDialog::OnGLH_Event(GraphicsLabel& Control, const char* Msg, void* Data)
{
	CStringA msg = Msg;

	// right click context menu
	if (msg == "map_r_down")
	{
		// get the point on screen
		POINT screen;
		GetCursorPos (&screen);

		// select the menu
		CMenu* pMenu = GetMenu()->GetSubMenu(4);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, screen.x, screen.y, this);
	}
	else if (msg == "map_mouse")
	{
		CString title;
		title.Format("%s @ %s", projectName, m_query_map.pos_Mouse.getCellCoordinates());
		SetWindowText(title);
	}
}

void QueryInterfaceDialog::OnBuilding_BeginConstructing()
{
	// protect the building, count it, and set it to add them
	getProject()->protect();
	getProject()->recount();
	getProject()->setJobType(PROJECT_BUILDING_ADD);
}

void QueryInterfaceDialog::OnBuilding_BeginLoading()
{
	// protect the building, count it, and set it to add them
	getProject()->protect();
	getProject()->recount();
	getProject()->setJobType(PROJECT_BUILDING_LOAD);
}

void QueryInterfaceDialog::OnBuilding_BeginDeleting()
{
	// protect the building, count it, and set it to add them
	getProject()->protect();
	getProject()->recount();
	getProject()->setJobType(PROJECT_BUILDING_DELETE);
}

void QueryInterfaceDialog::OnBuilding_StopBuilding()
{
	// stop the building
	getProject()->unprotect();
	getProject()->recount();
	getProject()->setJobType(PROJECT_WORKING);
}

void QueryInterfaceDialog::OnEditors_DeleteSelected()
{
	getProject()->objects.deleteSelected(true);
}

void QueryInterfaceDialog::OnContext_SelectSector()
{
	// build a sector filter
	Filters::Sector secFilter;
	secFilter.sector_x	= m_query_map.getMouseLocation().getSectorX();
	secFilter.sector_z	= m_query_map.getMouseLocation().getSectorZ();
	m_query_map.m_pCache->run_filter(&secFilter, true);
}

void QueryInterfaceDialog::OnSelection_Invert()
{
	getProject()->objects.invertSelection();
}

void QueryInterfaceDialog::OnBuilding_AddSelectionToQueue()
{
	Editors::QueueSelected editor;
	editor.runObjects(getProject()->objects);
}

void QueryInterfaceDialog::OnBuilding_AddEverythingToQueue()
{
	Editors::QueueEverything editor;
	editor.runObjects(getProject()->objects);
}

void QueryInterfaceDialog::OnView_SwitchSelectionView()
{
	m_query_map.color_style = GraphicsDisplay::QueryMap::COLORSTYLE_SELECTED;
	m_query_map.view_type = GraphicsDisplay::QueryMap::VIEWTYPE_DOTS;
}

void QueryInterfaceDialog::OnView_SwitchBuildStateView()
{
	m_query_map.color_style = GraphicsDisplay::QueryMap::COLORSTYLE_BUILD_STATUS;
	m_query_map.view_type = GraphicsDisplay::QueryMap::VIEWTYPE_DOTS;
}

#include "inputlinedialog.h"
void QueryInterfaceDialog::OnProject_NewFromSelection()
{
	CStringA projectName;
	while (true)
	{
		InputLineDialog dlg;
		dlg.v_Text = "";

		// run dialog
		if (dlg.DoModal() != IDOK)
			return;

		// project name ok
		projectName = dlg.v_Text;
		projectName.Trim();
		Project* pExisting = Property::g_Projects.Find(projectName);
		if (pExisting)
		{
			// do we want to select another?
			int nId = MessageBox("That project name is already taken. Do you wish to select another?", 0, MB_YESNOCANCEL);
			if (nId == IDYES)
				continue;
			
			// abort
			return;
		}
		else
		{
			break;
		}
	}

	// create new project
	Project* pNew = Property::g_Projects.CreateProject(projectName);
	pNew->objects.Copy_CloneSelected(getProject()->objects);
	this->projectName = projectName;
	
	// open project
	openProject(*pNew);
}

void QueryInterfaceDialog::OnClose()
{
	getProject()->editor_wnd = 0;
	__super::OnClose();
	delete this;
}

void QueryInterfaceDialog::OnContext_SelectRectangle()
{
	Filters::SelectionRect sr;
	sr.bound_hi = m_query_map.pos_MouseDown;
	sr.bound_low = m_query_map.pos_MouseUp;

	Property::Project* pProj = getProject();
	pProj->objects.run_filter(&sr, true);	
}

void QueryInterfaceDialog::OnBnClickedFilterUsertype()
{
	// TODO: Add your control notification handler code here
}


/*void QueryInterfaceDialog::OnView_MoveTo()
{
	// ask for coordinates
	InputLineDialog dlg;
	dlg.v_Text = "";
	dlg.title  = "Move Map Viewport";
	if (dlg.DoModal() != IDOK)
		return;

	// move map
	Location c(dlg.v_Text);
	m_query_map.changeCenter(c.getX(), c.getZ());
	m_query_map.Reset();
}*/

void QueryInterfaceDialog::writeLog(const char* Text)
{
	c_Log.SetSel(c_Log.GetText().GetLength(), c_Log.GetText().GetLength());
	c_Log.ReplaceSel(CString(Text) + "\r\n");
}

void QueryInterfaceDialog::OnProject_New()
{
	CStringA projectName;
	while (true)
	{
		InputLineDialog dlg;
		dlg.v_Text = "";

		// run dialog
		if (dlg.DoModal() != IDOK)
			return;

		// project name ok
		projectName = dlg.v_Text;
		projectName.Trim();
		Project* pExisting = Property::g_Projects.Find(projectName);
		if (pExisting)
		{
			// do we want to select another?
			int nId = MessageBox("That project name is already taken. Do you wish to select another?", 0, MB_YESNOCANCEL);
			if (nId == IDYES)
				continue;
			
			// abort
			return;
		}
		else
		{
			break;
		}
	}

	// create new project
	Project* pNew = Property::g_Projects.CreateProject(projectName);
	
	// open project
	openProject(*pNew);
}

#include "LargeQueryDialog.h"
void QueryInterfaceDialog::OnQuery_BeginLarge()
{
	LargeQueryDialog dialog(this);
	if (dialog.DoModal() != IDOK) return;

	// is it a radial
	if (dialog.v_aroundCenter.GetLength() != 0)
	{
		Location mid(dialog.v_aroundCenter);
		bot.queryLargeAreaAround(mid, dialog.v_aroundWidth, dialog.v_aroundHeight);
		m_query_map.changeCenter(mid.getX(), mid.getZ());
	}
	else
	{
		// convert
		Location top(dialog.v_top);
		Location bottom(dialog.v_bottom);

		// the top and bottom locations
		bot.queryLargeArea(top, bottom);

		// go to the middle
		m_query_map.changeCenter(
			(top.getX() + bottom.getX()) / 2,
			(top.getZ() + bottom.getZ()) / 2);
	}
}

void QueryInterfaceDialog::OnView_SwitchToCellView()
{
	this->m_query_map.view_type = GraphicsDisplay::QueryMap::VIEWTYPE_CELLS;
}

void QueryInterfaceDialog::OnQuery_WholeWorld()
{
	bot.queryBackup(Property::FileFormat::FORMAT_V4, "@__live");
}
