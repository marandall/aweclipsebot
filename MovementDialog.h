#pragma once
#include "textbox.h"
#include "afxwin.h"
#include "listctrlgpx.h"


// MovementDialog dialog
namespace Movement
{
	class Waypoints;
}

class MovementDialog : public CDialog
{
	DECLARE_DYNAMIC(MovementDialog)

public:
	MovementDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MovementDialog();

// Dialog Data
	enum { IDD = IDD_MOVEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // tracking object
	Umbra::Variant controlEditor;

public: // reload the information
	void reloadWaypointList(Movement::Waypoints& Wp);

public:
	TextBox c_wp_target;
	TextBox c_wp_waypoint;
	CButton c_wp_fixyaw;
	CButton c_wp_autopitch;
	CButton c_wp_hitexact;
	TextBox c_wp_rollrate;
	TextBox c_wp_rollratio;
	TextBox c_wp_yawrate;
	ListCtrlGpx c_wp_waypoints;
	afx_msg void OnBnClickedApply();
	afx_msg void OnWaypoints_Load();
	virtual BOOL OnInitDialog();
	TextBox c_speed;
	afx_msg void OnBnClickedOk();
	afx_msg void OnFile_SwitchToWaypoints();
	CButton c_execute;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
