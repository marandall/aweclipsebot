// EclipseEvolutionDialog.h : header file
//

#pragma once
#include "listctrlgpx.h"
#include "richeditctrl_ext.h"
#include "Resource.h"
#include "AvatarsDialog.h"
#include "EclipseInputDialog.h"
#include "EclipseLogDialog.h"
#include "afxwin.h"

class Avatar;

#define UWM_GETHOSTADDR_RESULT		(WM_USER + 1)
#define UWM_TRAYICON							(WM_USER + 2)
#define UWM_HIDE_MAIN_TASKBAR			(WM_USER + 3)

enum
{
	STATUSBAR_TIME			= 0,
	STATUSBAR_LOCATION,
	STATUSBAR_CITIZENS,
	STATUSBAR_TOURISTS,
	STATUSBAR_CLIENTS,
	STATUSBAR_BOTS,
	STATUSBAR_BANDWIDTH,
	STATUSBAR_STATUS,
};

namespace WorldList 
{ 
	class World;
}

// EclipseEvolutionDialog dialog
class EclipseEvolutionDialog : public CDialog, public Listener
{
// Construction
public:
	EclipseEvolutionDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ECLIPSEEVOLUTION_DIALOG };

public:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	DECLARE_MESSAGE_MAP()

public: // listener updates
	 void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo);

protected: /* interprocess IPC */
	UINT		m_ipc_shutdown;
	UINT		m_ipc_rebuild_icons;
	UINT		m_ipc_logout;
	UINT		m_ipc_message;

public:
	AvatarsDialog				c_AvatarsDialog;
	EclipseInputDialog	c_InputDialog;
	ListCtrlGpx					c_SideUserList;
	CRichEditCtrl_Ext		c_Log;
	EclipseLogDialog		c_TechLog;
	Avatar*							m_pContextAvatar;
	CStatusBar					c_StatusBar;

public: // context menus
	CMenu mnu_uni_users;
	CMenu	mnu_avatar;

	void ForceResize();
	int  last_resize_style;
	int	 last_resize_x, last_resize_y;

public: // avatar updates
	void	AvatarAdd(Avatar& User);
	void	AvatarDelete(Avatar& User);
	void	AvatarContextMenu(Avatar& User, int X, int Y);
	void	RecalculateIndexes();

public: // world updates
	void	WorldAdd(WorldList::World& World);
	void	WorldStatus(WorldList::World& World);
	void	WorldDelete(WorldList::World& World);

public: // deals with whispers
	void	WhisperTargetAdd	(Avatar& User);
	void	WhisperTargetChange	(Avatar& User);
	void	WhisperTargetDelete	(Avatar& User);
	void	WhisperTargetLock	(Avatar& User);
	void	WhisperTargetUnlock	(Avatar& User);
	void	WhisperTargetsReload();

public: // sets status bar errors
	void	setSB_Position(String Position);
	void	setSB_Status(String Text);

public: // deals with the task bar button
	NOTIFYICONDATA m_IconData;
	UINT	m_taskbarCreatedMsg;
	BOOL	CreateTrayIcon();
	BOOL	DestroyTrayIcon();
	BOOL	UpdateTrayIcon();
	BOOL	UpdateTrayWithPopup(String Title, String Msg, int Icon = NIIF_INFO);

public: // deals with menu setup


// Implementation
public:
	HICON m_hIcon;

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBot_Profiles();
	afx_msg void OnBotLogin();
	afx_msg void OnBot_Disconnect();
	afx_msg void OnBot_ExitWorld();
	afx_msg void OnBot_Teleport();
	afx_msg void OnMoveToHome();
	afx_msg void OnBot_MoveToSafetyLocation();
	afx_msg void OnBot_MoveToGroundZero();
	afx_msg void OnView_Options();
	afx_msg void OnUniverse_Options();
	afx_msg void OnUniverse_ChangeWelcomeMessage();
	afx_msg void OnUniverse_Ejections();
	afx_msg void OnUniverse_CitizenOptions();
	afx_msg void OnUniverse_LookupCitizenByName();
	afx_msg void OnUniverse_LookupCitizenByNumber();
	afx_msg void OnUniverse_WorldOptions();
	afx_msg void OnUniverse_LookupWorldByName();
	afx_msg void OnWorld_Options();
	afx_msg void OnWorld_ChangeWelcomeMessage();
	afx_msg void OnWorld_Ejections();
	afx_msg void OnComms_ConsoleMessage();
	afx_msg void OnComms_ConsoleBroadcast();
	afx_msg void OnComms_Hermes_CommandsBegin();
	afx_msg void OnComms_Hermes_PassiveOn();
	afx_msg void OnComms_Hermes_PassiveOff();
	afx_msg void OnComms_Hermes_End();
	afx_msg void OnComms_Eclipse_Ratios();
	afx_msg void OnComms_Eclipse_Present();
	afx_msg void OnComms_Eclipse_WorldLighting();
	afx_msg void OnComms_Eclipse_Version();
	afx_msg void OnComms_Eclipse_WorldInfo();
	afx_msg void OnComms_Xelagot_Report();
	afx_msg void OnComms_Xelagot_StupidBot();
	afx_msg void OnSupport_UrlMessage();
	afx_msg void OnSupport_UrlBroadcast();
	afx_msg void OnSupport_EnableGcs();
	afx_msg void OnSupport_ChangeCisColour();
	afx_msg void OnSupport_ChangeCisKey();
	afx_msg void OnSupport_Zones_EnablePublicZones();
	afx_msg void OnSupport_Zones_AllowCustomGreeting();
	afx_msg void OnSupport_Zones_ShowGreetingsInLog();
	afx_msg void OnSupport_Zones_ReloadZones();
	afx_msg void OnReports_Avatar();
	afx_msg void OnHelp_About();
	afx_msg void OnHelp_CheckForUpgrades();
	afx_msg void OnHelp_VisitTheWebsite();
public:

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	LRESULT OnWndProc_DnsLookup(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnWorld_Importsettings();
	afx_msg void OnWorld_Exportsettings();
	afx_msg void OnSecuity_RightsManager();
	afx_msg void OnAvatar_Eject();
	afx_msg void OnAvatar_Teleport();
	afx_msg void OnAvatar_ClickOnce();
	afx_msg void OnAvatar_ClickTimes10();
	afx_msg void OnAvatar_FollowNormal();
	afx_msg void OnAvatar_FollowBehind();
	afx_msg void OnAvatar_Circle();
	afx_msg void OnAvatar_Goto();
	afx_msg void OnAvatar_ShowReport();
	afx_msg void OnAvatar_ShowDrsProfile();
	afx_msg void OnSupport_Zones_Config();
	afx_msg void OnReports_AvatarHistory();
	afx_msg void OnComms_RepeaterWindow();
	afx_msg void OnSupport_AvatarRestrictions();
	afx_msg void OnSupport_ConfigureGcs();
	afx_msg void OnSideUsers_RightClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnUniverse_CreateCitizen();
	afx_msg void OnSupport_GlobalRelay();
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnAvatar_SendNoise();
	afx_msg void OnView_Map();
	ListCtrlGpx c_WorldList;
	afx_msg void OnComms_MuteList();
	afx_msg void OnComms_ConsoleMuteList();
	afx_msg void OnAvatar_ChangeDisplayColour();
	afx_msg void OnView_ChatSetup();
	ListCtrlGpx c_WhisperTargets;
	afx_msg void OnSideUsers_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);


public:
	afx_msg void WhisperTargets_OnDoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnAvatar_RelayJoin();
	afx_msg void OnAvatar_RelayKick();
	afx_msg void OnAvatar_RelayBan();
	afx_msg void OnAvatar_RelayPassiveOn();
	afx_msg void OnAvatar_RelayPassiveOff();
	afx_msg void OnAvatar_RelayLeave();
	afx_msg void OnSecuity_SystemWordFilter();
	afx_msg void OnSecuity_FloodPreventionPolicy();
	afx_msg void OnAvatar_DatabaseQueryName();
	afx_msg void OnAvatar_DatabaseQueryCitizen();
	afx_msg void OnAvatar_DatabaseQueryPrivilege();
	afx_msg void OnAvatar_DatabaseQueryIpAddress();
	afx_msg void OnAvatar_DatabaseQueryDnsAddress();
	afx_msg void OnWorld_QueryManager();
	afx_msg void OnSupport_ZoneEditor();
	afx_msg void OnAvatar_ChangeGlobalRelayColour();
	afx_msg void OnAvatar_ChangeGlobalRelaySettings();
	afx_msg void OnSupport_IpDnsAnnouncement();
	afx_msg void OnBot_Savestatus();
	afx_msg void OnBot_Crashbot();
	afx_msg void OnSupport_RelayManagement();
	afx_msg void OnBot_Shutdown();
	afx_msg void OnSupport_HeadsUp();
	afx_msg void OnAvatar_AssumeAvatar();
	afx_msg void OnAvatar_LookupCitizen();
	afx_msg void OnAvatar_LookupPrivilege();
	afx_msg void OnView_ClearMainLog();
	afx_msg void OnComms_BotgramEmail();
	afx_msg void OnSupport_ScriptingEditor();
	afx_msg void OnSupport_MoverControl();
	afx_msg void OnView_SelectChat();
	afx_msg void OnView_SelectWhisper();
	afx_msg void OnView_SelectCommands();
	afx_msg void OnView_SelectConsole();
	afx_msg void OnView_SelectConsoleMode();
	afx_msg void OnWorld_BackupView();
	afx_msg void OnFeatures_Recorder();
	afx_msg void OnFeatures_RelayChannelEditor();
	afx_msg void OnAvatar_BotgramToCitizen();
	afx_msg void OnAvatar_BotgramToPrivilege();
	afx_msg void OnFeatures_ScriptingCisCommands();
	afx_msg void OnReports_IpOwnershipAndColissions();
	afx_msg void OnUniverse_WorldList();
	afx_msg void OnWorldList_HeaderClicked(NMHDR *pNMHDR, LRESULT *pResult);
	CButton c_userToggle;
	CButton c_toggleWorld;
	ListCtrlGpx c_userList;
	afx_msg void OnBnClickedShowUsers();
	afx_msg void OnBnClickedShowWorlds();
	afx_msg void OnView_Layout();
	afx_msg void OnFeatures_JumpPointsManager();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};

extern EclipseEvolutionDialog* rt_diag;