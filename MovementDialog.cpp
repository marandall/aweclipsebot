// MovementDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "MovementDialog.h"
#include "movement.h"

// MovementDialog dialog

IMPLEMENT_DYNAMIC(MovementDialog, CDialog)

MovementDialog::MovementDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MovementDialog::IDD, pParent)
{

}

MovementDialog::~MovementDialog()
{
}

void MovementDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT14, c_wp_target);
	DDX_Control(pDX, IDC_EDIT15, c_wp_waypoint);
	DDX_Control(pDX, IDC_CHECK5, c_wp_fixyaw);
	DDX_Control(pDX, IDC_CHECK6, c_wp_autopitch);
	DDX_Control(pDX, IDC_CHECK7, c_wp_hitexact);
	DDX_Control(pDX, IDC_EDIT16, c_wp_rollrate);
	DDX_Control(pDX, IDC_EDIT17, c_wp_rollratio);
	DDX_Control(pDX, IDC_EDIT18, c_wp_yawrate);
	DDX_Control(pDX, IDC_LIST1, c_wp_waypoints);
	DDX_Control(pDX, IDC_EDIT19, c_speed);
	DDX_Control(pDX, IDC_CHECK2, c_execute);
}


BEGIN_MESSAGE_MAP(MovementDialog, CDialog)
	ON_BN_CLICKED(IDC_APPLY, &MovementDialog::OnBnClickedApply)
	ON_COMMAND(ID_WAYPOINTS_LOAD, &MovementDialog::OnWaypoints_Load)
	ON_BN_CLICKED(IDOK, &MovementDialog::OnBnClickedOk)
	ON_COMMAND(ID_FILE_SWITCHTOWAYPOINTS, &MovementDialog::OnFile_SwitchToWaypoints)
	ON_BN_CLICKED(IDC_CHECK2, &MovementDialog::OnBnClickedCheck2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// MovementDialog message handlers

void MovementDialog::reloadWaypointList(Movement::Waypoints& Wp)
{
	// reload the waypoint lists
	c_wp_waypoints.DeleteAllItems();

	// load the waypoints
	for (size_t i = 0; i < Wp.path_list.size(); i++)
	{
		int id = c_wp_waypoints.InsertItem((int)i, ToString(i) );
		c_wp_waypoints.EditItem(id, 1, Wp.path_list[i]);
	}
}

void MovementDialog::OnBnClickedApply()
{
	// load the waypoint controller
	Movement::Controller* pController = dynamic_cast<Movement::Controller*>(controlEditor.toRemoteClass());
	if (pController == 0)
		return;

	// map waypoints
	Movement::Waypoints& wp = pController->getWaypoints();
	wp.m_fixed_yaw_target	= "";
	wp.m_auto_pitch			= c_wp_autopitch.GetCheck() == TRUE;
	wp.m_hit_exact			= c_wp_hitexact.GetCheck() == TRUE;
	wp.m_roll_rate_d		= atoi(c_wp_rollrate.GetText());
	wp.m_max_yaw_rate_d		= atoi(c_wp_yawrate.GetText());
	// wp.m_cur_line			= atoi(c_wp_waypoint.GetText());

	// set the actual speed
	pController->setSpeed(atoi(c_speed.GetText()));
	pController->beginWaypoints();
}

void MovementDialog::OnWaypoints_Load()
{
	// waypoint entry
	CFileDialog file(TRUE, "txt", 0, 4|2, "Waypoint Text Files (*.txt)|*.txt||", this);
	if (file.DoModal() != IDOK) 
		return;

	// load the waypoint controller
	Movement::Controller* pController = dynamic_cast<Movement::Controller*>(controlEditor.toRemoteClass());
	if (pController == 0)
		return;

	// attemp to load the waypoints
	if (pController->getWaypoints().LoadWaypoints(file.GetPathName()) == false) 
	{
		MessageBox("There was an error loading the waypoint file");
		return;
	}

	// reload the waypoints
	reloadWaypointList(pController->getWaypoints());
}

BOOL MovementDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// load the waypoint controller
	Movement::Controller* pController = dynamic_cast<Movement::Controller*>(controlEditor.toRemoteClass());
	if (pController == 0)
		return FALSE;

	// add list items
	c_wp_waypoints.InsertColumn(0, "#",			0, 30);
	c_wp_waypoints.InsertColumn(1, "Waypoints", 0, 290);

	// init update timer
	SetTimer(0, 1000, 0);

	// load the waypoints
	reloadWaypointList(pController->getWaypoints());
	return TRUE;
}

void MovementDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void MovementDialog::OnFile_SwitchToWaypoints()
{
	// load the waypoint controller
	Movement::Controller* pController = dynamic_cast<Movement::Controller*>(controlEditor.toRemoteClass());
	if (pController == 0)
		return;

	// set to waypoints
	pController->beginWaypoints();
}

void MovementDialog::OnBnClickedCheck2()
{
	// load the waypoint controller
	Movement::Controller* pController = dynamic_cast<Movement::Controller*>(controlEditor.toRemoteClass());
	if (pController == 0)
		return;

	// set the status
	pController->execute = (c_execute.GetCheck() == TRUE);
}

void MovementDialog::OnTimer(UINT_PTR nIDEvent)
{
	// load the waypoint controller
	Movement::Controller* pController = dynamic_cast<Movement::Controller*>(controlEditor.toRemoteClass());
	if (pController == 0)
		return;

	// is it executing
	if (pController->execute == false)
		return;

	// map waypoints
	Movement::Waypoints& wp = pController->getWaypoints();

	// copy waypoint attributes
	c_wp_rollrate			= ToString(wp.m_roll_rate_d);
	c_wp_yawrate			= ToString(wp.m_max_yaw_rate_d);
	c_wp_hitexact			.SetCheck(wp.m_hit_exact ? TRUE : FALSE);
	c_wp_autopitch			.SetCheck(wp.m_auto_pitch ? TRUE : FALSE);
	c_wp_waypoint			= ToString(wp.m_cur_line);

	CDialog::OnTimer(nIDEvent);
}
