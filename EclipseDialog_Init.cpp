#include "stdafx.h"
#include "EclipseEvolution.h"
#include "EclipseEvolutionDialog.h"
#include "EclipseDialog_MainIcons.h"
#include "EclipseDialog_Timers.h"
#include "Profiles.h"
#include "GlobalVariables.h"
#include "Database.h"
#include "awsdk_bots.h"
#include "CommandParser.h"
#include "AWSDK_Bots.h"
#include "Profiles.h"
#include "CIS_Dispatch.h"
#include "GlobalDataExchange.h"
#include "GlobalVariables.h"
#include "SystemWordsFilter.h"
#include "FloodPreventionPolicy.h"
#include "Database.h"
#include "Movers.h"
#include "EclipseDialog_Timers.h"
#include "PropertyProject.h"
#include "property_async_command.h"
#include "CommandParser.h"
#include "umbra/umbra_variantmap.h"
#include "CitizenLookup_Kernel.h"
#include "helpers_xml.h"
#include "versions.h"
#include "movers_autodriver.h"
#include "scripting_host.h"
#include "WorldList.h"
#include "InterfaceConfig.h"
#include "UniverseUserList.h"
#include "DesktopConnector.h"
#include "world_ejections.h"

class WorldSorters
{
public: // sorting mechanisms
	struct OnName : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			WorldList::World* pW1 = (WorldList::World*)Item1;
			WorldList::World* pW2 = (WorldList::World*)Item2;

			// comparisons
			return _stricmp(pW1->getName(), pW2->getName());
		}
	};

	struct OnPopulation : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			WorldList::World* pW1 = (WorldList::World*)Item1;
			WorldList::World* pW2 = (WorldList::World*)Item2;

			// comparisons
			if (pW1->getUsers() == pW2->getUsers()) return 0;
			return (pW1->getUsers() < pW2->getUsers()) ? -1 : 1;
		}
	};

public: // sorting array
	OnName				name;
	OnPopulation	population;

public: // access
	static WorldSorters& getInst()
	{
		static WorldSorters s;
		return s;
	}
};

// EclipseEvolutionDialog message handlers
BOOL EclipseEvolutionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// set big icon
	SetIcon(m_hIcon, FALSE);		// set small icon

	/* set the side users list */
	c_SideUserList.InitIcons();
	c_SideUserList.ModifyStyle(0, WS_BORDER, 0);
	c_SideUserList.SetExtendedStyle(c_SideUserList.GetExtendedStyle() 
			| LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	c_SideUserList.InsertColumn(0, "User", 0, 108 + 22);
	c_SideUserList.InsertColumn(1, "",     0, 16);

	c_SideUserList.GetHeaderCtrl()->ShowWindow(SW_HIDE);

	/* add the images to the whisper list */
	String path = GetAppPath() + "\\graphics\\whisper_modes\\";
	c_SideUserList.InsertIcon(path + "unlocked.bmp",	"unlocked");
	c_SideUserList.InsertIcon(path + "locked.bmp",		"locked");
	
	/* set maximum counts */
	c_SideUserList.SetItemCount(200);
	c_Log.		ModifyStyle(0, WS_BORDER, 0);
	c_SideUserList.ModifyStyle(0, WS_BORDER, 0);
	c_WorldList.ModifyStyle(0, WS_BORDER, 0);
	c_userList.ModifyStyle(0, WS_BORDER, 0);
	c_WorldList.SetExtendedStyle(c_WorldList.GetExtendedStyle() | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);

	c_WhisperTargets.ModifyStyle(0, WS_BORDER, 0);
	c_WhisperTargets.SetExtendedStyle(c_WhisperTargets.GetExtendedStyle() | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);

	//
	// Import World List Icons
	//

	c_WorldList.InitIcons();
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_G,							"G_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_G_PRIVATE,			"G_PRI");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_PG,						"PG_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_PG_PRIVATE,		"PG_PRI");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_PG13,					"PG13_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_PG13_PRIVATE,	"PG13_PRI");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_R,							"R_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_R_PRIVATE,			"R_PRI");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_X,							"X_PUB");
	c_WorldList.InsertIconByBitmapResource(IDB_WORLD_X_PRIVATE,			"X_PRI");

	//
	// Setup World List
	//

	c_WorldList.InsertColumn(0, _T("Name"), 0, 90);
	c_WorldList.InsertColumn(1, _T("#"),	0, 30);
	c_WorldList.addSorter(0, &WorldSorters::getInst().name);
	c_WorldList.addSorter(1, &WorldSorters::getInst().population);
	c_WorldList.sort(1, ListCtrlGpx::SORT_DECENDING);

	//
	// Setup headers for Universe Users
	//

	c_userList.InsertColumn(0, _T("Name"), 0, 100);
	c_userList.InsertColumn(1, _T("World"), 0, 60);

	//
	// Logging Options
	//

	c_Log.ShowScrollBar(SB_VERT, TRUE);
	c_Log.setFirstTab();
	c_Log.ShowScrollBar(SB_VERT, TRUE);

	/* load avatars window */
	c_AvatarsDialog.Create(AvatarsDialog::IDD, this);
	c_AvatarsDialog.ShowWindow(SW_SHOW);

	c_InputDialog.Create(EclipseInputDialog::IDD, this);
	c_InputDialog.ShowWindow(SW_SHOW);
	c_TechLog.Create(EclipseLogDialog::IDD, this);
	c_TechLog.ShowWindow(SW_SHOW);
	c_StatusBar.Create(this);
	c_StatusBar.ShowWindow(SW_SHOW);

	/* set the status bar */
	CStatusBarCtrl& bar = c_StatusBar.GetStatusBarCtrl();
	int align = 0;

	int widths[] = {
			align+=80,			// time
			align+=200,			// status and time
			align+=40,			// citizens
			align+=40,			// tourists
			align+=80,			// clients
			align+=40,			// bots,
			align+=110,
			-1};
	const int parts_total = sizeof(widths) / sizeof(widths[0]);
	int rt = bar.SetParts(parts_total, widths);
	bar.SetText("Initiated", 0, SBT_POPOUT);

	/* make this window resizing */
	ModifyStyle(0, WS_SIZEBOX);

	/* create tray icons */
	CreateTrayIcon();

	/* write an introduction message */
	CStringA logText, temp;
	WriteMsg("Eclipse Evolution 2", 0, MF_BOLD | OT_MAIN);
	WriteMsg("Copyright (c) Mark Randall 2005 - 2018. All Rights Reserved.", 0, MF_BOLD | OT_MAIN);
	WriteMsg("www.awportals.com", 0, MF_BOLD | OT_MAIN);
	WriteMsg("---------------------------- ", 0, MF_BOLD | OT_MAIN);
	WriteMsg(EVOLUTION_IDENTIFY, 0, MF_BOLD | OT_MAIN);

	// init sdk
	int rc = aw_init(AW_BUILD);
	if (rc == 0)
	{
		temp.Format("SDK initialized OK with build %d at %d.", AW_BUILD, _time64(0));
		WriteMsg(temp, 0, OT_MAIN | MF_BOLD);
	}
	else
	{
		temp.Format("SDK failed to initialize with build %d. Reason %d.", AW_BUILD, rc);
		WriteMsg(temp, RGB_RED, OT_MAIN | MF_BOLD);
	}

	// add some padding
	WriteMsg("", 0, OT_MAIN);
	WriteMsg("", 0, OT_MAIN);

	/* set the timers for the various bots */
	SetTimer(TIMER_PROC_BOT_HEARTBEAT, 50,   NULL);
	SetTimer(TIMER_PROC_SECONDS,       1000, NULL);

	/* write set of messages for testing */
	WriteMsg("Technical Log",	0, MF_BOLD | OT_TECH);
	WriteMsg("Debug Log",		0, MF_BOLD | OT_DEBUG);
	WriteMsg("Security Log",	0, MF_BOLD | OT_SECURITY);
	WriteMsg("Users Log",		0, MF_BOLD | OT_USERLIST);

	/* load icons into profile list */
	Profiles::g_Icons.InsertIcon("Red 1",		IDI_RED_1);
	Profiles::g_Icons.InsertIcon("Red 2",		IDI_RED_2);
	Profiles::g_Icons.InsertIcon("Red 3",		IDI_RED_3);
	
	Profiles::g_Icons.InsertIcon("Blue 1",		IDI_BLUE_1);
	Profiles::g_Icons.InsertIcon("Blue 2",		IDI_BLUE_2);
	Profiles::g_Icons.InsertIcon("Blue 3",		IDI_BLUE_3);

	Profiles::g_Icons.InsertIcon("Green 1",		IDI_GREEN_1);
	Profiles::g_Icons.InsertIcon("Green 2",		IDI_GREEN_2);
	Profiles::g_Icons.InsertIcon("Green 3",		IDI_GREEN_3);

	Profiles::g_Icons.InsertIcon("Pink 1",		IDI_PINK_1);
	Profiles::g_Icons.InsertIcon("Pink 2",		IDI_PINK_2);
	Profiles::g_Icons.InsertIcon("Pink 3",		IDI_PINK_3);

	Profiles::g_Icons.InsertIcon("Yellow 1",	IDI_YELLOW_1);
	Profiles::g_Icons.InsertIcon("Yellow 2",	IDI_YELLOW_2);
	Profiles::g_Icons.InsertIcon("Yellow 3",	IDI_YELLOW_3);

	/* copy imagelist */
	c_SideUserList.SetImageList(
		c_AvatarsDialog.c_Avatars.GetImageList(LVSIL_SMALL), LVSIL_SMALL); 

	/* add bots to sdk loop */
	AW::bots.InsertBot(&bot);
	bot.InitialiseClient();

	/* configure winsock */
	WORD	wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 0 );

	int err = WSAStartup( wVersionRequested, &wsaData );
	if (err != 0) {
		logText.Format("Unable to initialise Winsock (2, 0) reason: %d", err);
		WriteMsg(logText, RGB_RED, OT_DEBUG | OM_TIME);	
	} else {
		logText.Format("Winsock Init OK (2, 0)");
		WriteMsg(logText, 0, OT_DEBUG | OM_TIME);	
	}

	/* initialise database */
	String cmdText;
	cmdText.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;", GetAppPath() + "\\data\\core_v3.mdb");
	if (g_Database.Open(cmdText) == FALSE)
	{
		AfxMessageBox("There was an error opening the core_v3 database");
	}


	/* load universe information */
	if (!Profiles::g_Universes.Load(GetAppPath() + "\\data\\universes.lxm"))
	{
		logText.Format("Error opening the universe file '\\data\\universes.lxm");
		WriteMsg(logText, RGB_RED, OT_ERROR | OT_TECH | OT_MAIN);
		MessageBoxA(logText, "Universe Error", MB_ICONWARNING);
	}
	else
	{
		logText.Format("Universe load complete; %d configruations found", (int)Profiles::g_Universes.size());
		WriteMsg(logText, 0, OT_DEBUG);
	}

	/* inter process communication */
	m_ipc_shutdown			= RegisterWindowMessageA("ECLIPSE-EVOLUTION-IPC-SHUTDOWN");
	m_ipc_rebuild_icons		= RegisterWindowMessageA("ECLIPSE-EVOLUTION-IPC-REBUILD-ICONS");
	m_ipc_logout			= RegisterWindowMessageA("ECLIPSE-EVOLUTION-IPC-LOGOUT");
	m_ipc_message			= RegisterWindowMessageA("ECLIPSE-EVOLUTION-IPC-MESSAGE");

	/* load the first profile system */
	Profiles::g_Profiles.OpenDirectory(GetAppPath() + "\\data\\profiles");
	Profiles::g_Profiles.SelectProfile("default");
	
	/* parse commands */
	CommandParser cl(GetCommandLine());

	// new method 'backgroundload'
	if (cl.getText("autobgs") != "")
	{
		cl.insert_arg("profile", cl.getText("autobgs"));
		cl.insert_arg("login", "startup");
		cl.insert_arg("hide", "yes");
	}

	bool login_allowed = false;
	if (cl.getText("profile") != "")
	{
		CStringA profile_id = cl.getText("profile", "");
		if (Profiles::g_Profiles.SelectProfile(profile_id) == false)
		{
			WriteMsg(CStringA("AUTO_EXEC ERROR; could not open profile: ") + profile_id, RGB_RED, OT_MAIN);
		}
		else
		{
			login_allowed = true;
		}
	}

	// open up the currently selected profile
	OpenProfile(Profiles::g_Profiles.getCurrent()->getId());;

	// call connector
	DesktopConnector::getInst().connect();
	DesktopConnector::getInst().broadcastAppLoad();
	DesktopConnector::getInst().broadcastUpdate();

	// do we need to login?
	if (cl.getText("login", "no") == "startup" && login_allowed == true)
		OnBotLogin();

	if (cl.getText("hide", "no") == "yes")
		PostMessage(UWM_HIDE_MAIN_TASKBAR, 0, 0);

	// set console
	c_Log.SetSel(c_Log.GetTextLength(), c_Log.GetTextLength());

	// create universe context
	mnu_uni_users.LoadMenuA(IDR_UNIVERSE_USERS_CONTEXT);
	mnu_avatar.LoadMenuA(IDR_AVATAR_CONTEXT);

	// connect open listeners
	connectListener(bot.getWorldList());
	connectListener(bot);
	connectListener(bot.getUserList());

	return TRUE;  // return TRUE  unless you set the focus to a control
}



#include "movement_interface.h"
#include "property_query_world.h"
#include "BotgramProtocol.h"
#include "world_ejections.h"

void EclipseEvolutionDialog::OnTimer(UINT_PTR nIDEvent)
{
	// handle new timestamp events
	static int prev_day = -1;
	__int64 cur_time = time64(0);
	
	// structure for the system time
	FILETIME fileTime;
	SYSTEMTIME sysTime;

	// copy the UTC time into the file time
	memcpy(&fileTime, &cur_time, sizeof(__int64));
	FileTimeToSystemTime(&fileTime, &sysTime);
	
	if (nIDEvent == TIMER_PROC_BOT_HEARTBEAT) 
	{
		this->c_Log.SetRedraw(FALSE);
		
		if (bot.getUniverseConnection() != ConState::Recovering)
			aw_wait(0);
		else if ((time64(0) % 5) == 0)
			aw_wait(0);

		// cancel out the drawing on the logs
		c_Log.SetRedraw(TRUE);

		// if the document has changed then a redraw is necessary
		if (c_Log.ReadChangedState())
			c_Log.Invalidate();

		// trigger the timer
		ScriptingHost::getInst().timer.process();
		return;
	} 

	// world update resolution
	else if (nIDEvent == TIMER_PROC_AVATAR_UPDATES)
	{
		int time_x = bot._int(AW_WORLD_AVATAR_REFRESH_RATE);
		if (time_x == 0) time_x = 1;

		// update movers
		Movers::AutoDriverKernel::getInstance().process(1.0f / time_x, bot);

		// mover
		if (bot.getWorldConnection() == ConState::Connected)
		{
			AW::Location next;
			if (bot.getMovementKernel().getNextChange(bot, next, 1.0f / time_x) == Movement::IMovement::DO_MOVE)
			{
				bot.Move(next);
			}
		}
	}

	else if (nIDEvent == TIMER_PROC_SECONDS)
	{	
		//
		// Update Backup Processor
		//
		bot.getQueryWorld().handleTimer();

		//
		// Send any streams of botgrams
		//

		BotgramProtocol::SendingKernel::getInstance().onTick();

		//
		// update vrt
		//
		String vrtTime, dispText;
		vrtTime.Format("\t%s vrt", MakeGuiTime(time64(0) - 7200));
		CStatusBarCtrl& bar = c_StatusBar.GetStatusBarCtrl();
		bar.SetText(CString(vrtTime), STATUSBAR_TIME, SBT_POPOUT);

		// the world
		WorldList::World* pWorld = bot.getWorldList().lookup_world(bot._string(AW_WORLD_NAME));

		/* update avatar counts */
		int clients = avatars.getUserCount(UserType::Citizen) + avatars.getUserCount(UserType::Tourist);
		CStringA clitext;
		clitext.Format("%d of %d", clients, (pWorld ? pWorld->getUsers() : -1));

		bar.SetText(CString("\t^") + ToString(avatars.getUserCount(UserType::Citizen)), STATUSBAR_CITIZENS, SBT_POPOUT);
		bar.SetText(CString("\t\"") + ToString(avatars.getUserCount(UserType::Tourist)) + "\"", STATUSBAR_TOURISTS, SBT_POPOUT);
		bar.SetText(CString("\t") + clitext, STATUSBAR_CLIENTS, SBT_POPOUT);
		bar.SetText(CString("\t[") + ToString(avatars.getUserCount(UserType::Bot)) + "]", STATUSBAR_BOTS, SBT_POPOUT);
		
		/* update bandwidth */
		int bwIn = 0; 
		int bwOut = 0;
		if (bot.getWorldConnection() != ConState::Nothing || bot.getUniverseConnection() != ConState::Nothing)
			bot._traffic_count(bwIn, bwOut);

		dispText.Format("\t%.1f / %.1f KB/s", 
			(float)(bwIn / 1024.0), (float)(bwOut / 1024.0));
		bar.SetText(CString(dispText), STATUSBAR_BANDWIDTH, SBT_POPOUT);

		static int seconds = 0;
		seconds++;

		// every 30 seconds update the user list
		if (seconds % 30 == 0)
			bot._user_list();

		// check iteration
		if (seconds % 60 == 0 && bot.HasCT() && bot.getWorldConnection() == ConState::Connected)
			bot.getEjectManager().beginQuery(false);

		// clear the main log every minute or so
		if (seconds % 20 == 0)
			rt_diag->c_Log.cropOverflow(g_InterfaceConfig.getLogging().getDisplayLines());

		// every 30 minutes backup the profile
		if (seconds % (60 * 30) == 0) // every 30 minutes
		{	
			if (Profiles::g_Profiles.getCurrent() != NULL)
			{
				CloseProfile(Profiles::g_Profiles.getCurrent()->getId());		// autosave settings
			}
		}

		// update statistics on each avatar
		for (size_t i = 0; i < avatars.size(); i++)
		{
			// update anti flooding procedures		
			avatars[i].getFPD().setMPS(static_cast<int>(time64(0) % 5), 0);
		
			// update hyperion using a 2 second resolution
			if (seconds % 2 == 0)
			{
				avatars[i].activateHyperionByRefreshConditions();
			}
		}


		// conds clear up the avatars list
		__time64_t cur_time = time64(0);
		for (size_t i = 0; i < avatars.size(); i++)
		{
			AEX::FloodPrevention_Data& fpd = avatars[i].getFPD();

			// ignore if no muted time
			if (fpd.getMutedTime() == 0)
				continue;

			// has the time expired
			if (fpd.getMutedTime() + Security::g_FloodPolicy.auto_mute_len < cur_time)
			{
				// term mute
				avatars[i].Unmute();
				fpd.setMutedTime(0);

				// notify user
				String logText;
				logText.Format("AUTO_MUTE has ended on %s", avatars[i].getName());
				WriteMsg(logText, 0x884444, OM_TIME | OT_MAIN);
			}
		}

		// dispatch so many per second
		int build_dispatch = 0;
		int MAX_OUTSTANDING = 60;
		int not_ready_count = 0;
		int searched_count  = 0;
		bool escape_loop = false;

		// collect the limbo count
		int limbo_count = 0;		
		int MAX_BUILD_DISPATCH = static_cast<int>(MAX_OUTSTANDING - Property::g_AsyncMonitor.m_list.size());

		// loop through
		for (Property::Projects::iterator projPtr = Property::g_Projects.begin();
			projPtr != Property::g_Projects.end();
			projPtr++)
		{
			int count = 0;
			projPtr->handle_available_slot(bot);
		}

		// call back every 4 hours
		if ((seconds % (60 * 60 * 4)) == 0)
		{
			// botgram back to the developer citizenship
			::Helpers::XML::DataKeyset dsKeys;
			dsKeys.InsertKey("to",		0);
			dsKeys.InsertKey("msg",		"checkpoint");
			dsKeys.InsertKey("name",	bot._string(AW_LOGIN_PRIVILEGE_NAME));
			dsKeys.InsertKey("client",	EVOLUTION_IDENTIFY);
			dsKeys.InsertKey("cver",	EVOLUTION_VERSION);
			String msg = dsKeys.ToSingleXML("evo");
			
			// botgram
			bot.Botgram(334303, msg, true);
		}
		
		// handle world list updates
		if ((seconds % 30) == 0 && bot.IsUniConnected() == true)
		{
			bot.getWorldList().query_list();
		}

		// event processing
		if (seconds % 60 == 0)
		{
			// scripting host
			ScriptingHost::getInst().doFunction("EventMinuteTimer", 
				&Umbra::VariantArray(), 0, &bot);
		}
		
		// scripting host
		ScriptingHost::getInst().doFunction("EventSecondTimer", 
			&Umbra::VariantArray(), 0, &bot);
	}

	// get the bots first query
	bot.getLiveCache();
	CDialog::OnTimer(nIDEvent);
}