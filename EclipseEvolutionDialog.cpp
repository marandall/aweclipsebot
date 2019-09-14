// EclipseEvolutionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AWSDK_HudElement.h"
#include "EclipseEvolution.h"
#include "EclipseEvolutionDialog.h"
#include "EclipseInputDialog.h"
#include "AWSDK_Bots.h"
#include "UniverseOptionsDialog.h"
#include "InputBoxDialog.h"
#include "UniverseEjectionDialog.h"
#include "IpToDns.h"
#include "ConsoleMessageDialog.h"
#include "TeleportDialog.h"
#include "AWSDK_World.h"
#include "DynamicRights.h"
#include "SecurityRightsDialog.h"
#include "ZonesConfigDialog.h"
#include "AvatarHistoryDialog.h"
#include "Profiles.h"
#include "ProfilesDialog.h"
#include "RepeaterDialog.h"
#include "CommandConfigDialog.h"
#include "EclipseDialog_MainIcons.h"
#include "SecurityListDialog.h"
#include "CIS_Dispatch.h"
#include "UniverseCitizenAddDialog.h"
#include "GlobalDataExchange.h"
#include "BeyondRangeRelayDialog.h"
#include "WebClientDialog.h"
#include "MapDialog.h"
#include "ConsoleFilter.h"
#include "TextListDialog.h"
#include "afxdlgs.h"
#include "GlobalDataExchange.h"
#include "EjectDialog.h"
#include "GlobalVariables.h"
#include "ProfilesManagerDialog.h"
#include "ChatFilterDialog.h"
#include "SystemWordsFilter.h"
#include "ImmigrationOfficerDialog.h"
#include "FloodPreventionPolicy.h"
#include "FloodPreventionDialog.h"
#include "Database.h"
#include "AvatarsDatabaseDialog.h"
#include "Movers.h"
#include "EclipseDialog_Timers.h"
#include "PropertyProject.h"
#include "property_async_command.h"
#include "SelectUrlDialog.h"
#include "ZoneEditorDialog.h"
#include "wa_group.h"
#include "CommandParser.h"
#include "Zones.h"
#include "WorldOptionsDialog.h"
#include "CoreIPDialog.h"
#include "GeneralOptionsDialog.h"
#include "RelayConfigDialog.h"
#include "umbra/umbra_variantmap.h"
#include "HudConfigDialog.h"
#include "SerializationEditorDialog.h"
#include "awsdk_group_map.h"
#include "CitizenLookup_Kernel.h"
#include "UniverseCitizenDialog.h"
#include "QueryInterfaceDialog.h"
#include "InterfaceConfigDialog.h"
#include "botgram_protocol.h"
#include "Helpers_xml.h"
#include "Versions.h"
#include "scripting_host.h"
#include "movement.h"
#include "worldlist.h"
#include "InterfaceLayout.h"
#include "UniverseUserList.h"

Avatar* g_ContextAvatar = NULL;
ADODB::Connection g_Database;
ADODB::Connection UniverseUserList::g_DB;
//Waypoints::Driver g_TestMoveDriver;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CHECK_CONTEXT_AVATAR()			\
	if (!m_pContextAvatar)				\
		return;

/* relating to the worlds list */
bool world_list_has_changed	= false;

void EclipseEvolutionDialog::onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo)
{
	// world list notification messages
	if (Broadcaster == &bot.getWorldList())
	{
		// assign map
		ListCtrlGpx& wlc = c_WorldList;
		WorldList::WorldChangedMessage* pNM = dynamic_cast<WorldList::WorldChangedMessage*>(EventInfo);

		// a world has recently come online
		if (Id == WorldList::Kernel::LM_WORLD_ONLINE)
		{
			int iconKey = wlc.getIconKey(pNM->world->getIconKey());
			int id = wlc.InsertItem(wlc.GetItemCount(), pNM->world->getName(), iconKey);
			
			wlc.SetItemData(id, (DWORD_PTR)pNM->world);
			wlc.EditItem(id, 1, ToString(pNM->world->getUsers()));
		}
		else if (Id == WorldList::Kernel::LM_WORLD_CHANGED)
		{
			int iconKey = wlc.getIconKey(pNM->world->getIconKey());
			int id = wlc.GetByItemData((UINT_PTR)pNM->world);

			wlc.EditItem(id, 0, pNM->world->getName(), iconKey);
			wlc.EditItem(id, 1, ToString(pNM->world->getUsers()));
		}
		else if (Id == WorldList::Kernel::LM_WORLD_OFFLINE)
		{
			int nId = wlc.GetByItemData((DWORD_PTR)pNM->world);
			if (nId != -1)
			{
				wlc.DeleteItem(nId);
			}
		}
		else if (Id == WorldList::Kernel::LM_WORLD_CLEARED)
		{
			wlc.DeleteAllItems();
		}

		// resort the data
		wlc.resort();
	}
	else if (Broadcaster == &bot.getUserList())
	{
		UniverseUserList::UserChangedMessage* pNM = dynamic_cast<UniverseUserList::UserChangedMessage*>(EventInfo);
		ListCtrlGpx& ulc = c_userList;

		// a user has just come online
		if (Id == UniverseUserList::Kernel::LM_ONLINE)
		{
			int id = ulc.InsertItem(ulc.GetItemCount(), pNM->user->getName());
			ulc.SetItemData(id, (DWORD_PTR)pNM->user);
			ulc.EditItem(id, 1, pNM->user->getWorld());
		}

		// a user has changed
		else if (Id == UniverseUserList::Kernel::LM_CHANGED)
		{
			int id = ulc.GetByItemData((UINT_PTR)pNM->user);
			if (id != -1)
			{
				ulc.EditItem(id, 1, pNM->user->getWorld());
			}
		}

		// a user has gone away
		else if (Id == UniverseUserList::Kernel::LM_OFFLINE)
		{
			int id = ulc.GetByItemData((UINT_PTR)pNM->user);
			if (id != -1)
			{
				ulc.DeleteItem(id);
			}
		}

		// entire list has gone away
		else if (Id == UniverseUserList::Kernel::LM_CLEARED)
		{
			ulc.DeleteAllItems();
		}
	}
}

Umbra::VariantMap Umbra_GetGlobals(Avatar* pUser)
{
	Umbra::VariantMap vt_map;
	vt_map["bot"]		= bot;
	vt_map["avatars"]	= avatars;
	return vt_map;
}

void InvertMenuCheck(int Type)
{
	SetRootMenuChecked(Type, !IsRootMenuChecked(Type));
}

EclipseEvolutionDialog* rt_diag;

EclipseEvolutionDialog::EclipseEvolutionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EclipseEvolutionDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void EclipseEvolutionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIDE_USER_LIST, c_SideUserList);
	DDX_Control(pDX, IDC_PRINCIPLE_CHAT, c_Log);
	DDX_Control(pDX, IDC_WORLDS_LIST, c_WorldList);
	DDX_Control(pDX, IDC_WHISPER_LIST, c_WhisperTargets);
	DDX_Control(pDX, IDC_USER_LIST, c_userList);
}


void EclipseEvolutionDialog::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void EclipseEvolutionDialog::RecalculateIndexes()
{
	int count = this->c_AvatarsDialog.c_Avatars.GetItemCount();
	for (int i = 0; i < count; i++) 
	{
		INT_PTR uID = c_AvatarsDialog.c_Avatars.GetItemData(i);
		Avatar* pUser = avatars.getSession((int)uID);
		if (pUser) pUser->gui_main_index = (int)uID;
	}

	count = c_SideUserList.GetItemCount();
	for (int i = 0; i < count; i++) 
	{
		INT_PTR uID = c_SideUserList.GetItemData(i);
		Avatar* pUser = avatars.getSession((int)uID);
		if (pUser) pUser->gui_side_index = (int)uID;
	}
}

/*
	events for avatars arriving and leaving, to be setup	
*/
void EclipseEvolutionDialog::AvatarAdd(Avatar& User)
{
	/* show in the main dialog */
	c_AvatarsDialog.AvatarAdd(User);

	/* display as a side uer*/
	int id = c_SideUserList.InsertItem(0, User.getName());
	c_SideUserList.SetItemData(id, User.getSession());
	c_SideUserList.EditItem(id, 0, User.getName(), User.getIconPrivilege());
	c_SideUserList.EditItem(id, 1, "",			   Data::av_icons.blank);
}

void EclipseEvolutionDialog::AvatarDelete(Avatar& User)
{
	c_AvatarsDialog.AvatarDelete(User);
	int id = c_SideUserList.GetByItemData(User.getSession());
	if (id != -1)
		c_SideUserList.DeleteItem(id);
	g_ContextAvatar = NULL;
}

void EclipseEvolutionDialog::AvatarContextMenu(Avatar& User, int X, int Y)
{
	g_ContextAvatar = &User;
	CMenu* popupMenu = mnu_avatar.GetSubMenu(0);

	// set menu
	bool is_root = bot._int(AW_LOGIN_OWNER) == 1;
	SetRootMenuEnabled(ID_UNIVERSE_LOOKUP_CITIZEN,		g_ContextAvatar->getCitizen() && is_root);
	SetRootMenuEnabled(ID_UNIVERSE_LOOKUP_PRIVILEGE,	g_ContextAvatar->getPrivilege() && is_root);

	bool is_eject = bot.HasEject() || bot.HasCT();
	SetRootMenuEnabled(ID_AVATAR_EJECTION_EJECT,		is_eject);
	SetRootMenuEnabled(ID_AVATAR_EJECTION_TELEPORT,		is_eject);

	popupMenu->TrackPopupMenu(TPM_LEFTALIGN, X, Y, this);
}

void EclipseEvolutionDialog::setSB_Position(String Position)
{
	CStatusBarCtrl& bar = c_StatusBar.GetStatusBarCtrl();
	bar.SetText(CString(Position), STATUSBAR_LOCATION, SBT_POPOUT);
}

void EclipseEvolutionDialog::setSB_Status(String Text)
{
	CStatusBarCtrl& bar = c_StatusBar.GetStatusBarCtrl();
	bar.SetText(CString(Text), STATUSBAR_STATUS, SBT_POPOUT);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR EclipseEvolutionDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void EclipseEvolutionDialog::ForceResize()
{
	RECT rt;
	GetWindowRect(&rt);
	OnSize(last_resize_style, last_resize_x, last_resize_y);
}

void EclipseEvolutionDialog::OnBot_Profiles()
{
	// close current profile
	CloseProfile(Profiles::g_Profiles.getCurrent()->getId());

	// pop the dialog manager
	ProfilesManagerDialog dlg(this);
	if (dlg.DoModal() == IDOK && dlg.selected_profile.GetLength())
	{
		/* select this profile to open */
		Profiles::g_Profiles.SelectProfile(dlg.selected_profile);
		OpenProfile(dlg.selected_profile);
	}
}

void EclipseEvolutionDialog::OnBotLogin()
{
	Profiles::Profile* pLogin = Profiles::g_Profiles.getCurrent();
	if (!pLogin)
		return;

	// attempt to determine the universe
	Profiles::Universe* pUni = Profiles::g_Universes.getUniverse(pLogin->getUniName());
	if (pUni == NULL)
	{
		String logError;
		logError.Format("Unable to locate universe information for %s", pLogin->getUniName());
		WriteMsg(logError, RGB_RED, OT_ERROR | OT_MAIN);
		return;
	}
	else
	{
		// scripting host
		ScriptingHost::getInst().doFunction("EventLoad", 
			&Umbra::VariantArray(), 0, &bot);

		// trigger a login
		bot.quickstart(pUni->host, pUni->port, pLogin->getUniName(), 
				pLogin->getLoginCitizen(), pLogin->getLoginPassword(), pLogin->getLoginName(),
				pLogin->getTargetWorld(), pLogin->getTargetCoords(), pLogin->getTargetGlobal(), pLogin->getTargetInvisible()
			);
	}
}

void EclipseEvolutionDialog::OnBot_Disconnect()
{
	bot.Destroy();

	// clean up issues
	bot.getMovementKernel().standby();
}

void EclipseEvolutionDialog::OnBot_ExitWorld()
{
	/* in event our connection state is nothing we quit */
	if (bot.getWorldConnection() == ConState::Nothing)
		return;

	/* confirm exit world? */
	String text; 
	text.Format("Are you sure you wish to leave %s", bot.getWorld());
	if (MessageBoxA(text, "Exit World", MB_ICONQUESTION | MB_YESNO) == IDYES)
		bot.ExitWorld();
}

void EclipseEvolutionDialog::OnBot_Teleport()
{
	TeleportDialog dlg;
	dlg.v_Global	= bot.GetGlobal() ? TRUE : FALSE;
	dlg.v_Invisible	= bot.getInvisible() ? TRUE : FALSE;
	dlg.v_World		= bot.getWorld();
	dlg.v_String	= bot.getCoordinates();
	
	// open dialog for confirmation
	if (dlg.DoModal() != IDOK)
		return;

	bot.teleport(dlg.v_World, dlg.v_String, dlg.v_Global == TRUE, dlg.v_Invisible == TRUE);
}

void EclipseEvolutionDialog::OnMoveToHome()
{
	// TODO: Add your command handler code here
}

void EclipseEvolutionDialog::OnBot_MoveToSafetyLocation()
{
}

void EclipseEvolutionDialog::OnBot_MoveToGroundZero()
{
	if (!bot.IsWorldConnected())
		return;
	if (MessageBox(_T("Are you sure you wish to teleport to ground zero?"), 
				   _T("Teleport"), MB_YESNO) != IDYES)
	{
		Location lc("0n 0w 10a");
		bot.Move(lc);
	}
}

void EclipseEvolutionDialog::OnView_Options()
{
	GeneralOptionsDialog* pDialog = new GeneralOptionsDialog(this);
	pDialog->Create(GeneralOptionsDialog::IDD, this);
	pDialog->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnUniverse_Options()
{
	UniverseOptionsDialog* dlg = new UniverseOptionsDialog(this);
	dlg->Create(UniverseOptionsDialog::IDD, this);
	dlg->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnUniverse_ChangeWelcomeMessage()
{
	/* only do this if our bot is root */
	if (bot.HasRoot() == false)
		return;

	/* pop up the input box */
	InputBoxDialog input(this);
	input.v_String = bot._string(AW_UNIVERSE_WELCOME_MESSAGE);
	if (input.DoModal() != IDOK) 
		return;
	bot.Lock();
	bot._string_set(AW_UNIVERSE_WELCOME_MESSAGE, input.v_String.Left(AW_MAX_ATTRIBUTE_LENGTH));
	int rc = bot._universe_attributes_change();
	bot.Unlock();
	if (rc) {
		String logText;
		logText.Format("Unable to change universe attributes\n\nReason: %d\n%s", rc, AW::Bot::GetReasonCode(rc));
		MessageBox(logText);
	}
}

void EclipseEvolutionDialog::OnUniverse_Ejections()
{
	UniverseEjectionDialog dialog(this);
	dialog.DoModal();
}

void EclipseEvolutionDialog::OnUniverse_CitizenOptions()
{
	UniverseCitizenDialog* pDialog = new UniverseCitizenDialog(this);
	pDialog->Create(UniverseCitizenDialog::IDD, this);
	pDialog->ShowWindow(SW_SHOW);
}

void EclipseDialog_OnCallbackCitizenByName(Async::Header& Header)
{
	/* abort if we have an error code */
	AW::Citizen citizen = *((AW::Citizen*)Header.p_result);
	String logText;
	if (Header.u_rc != 0)
		logText.Format("@Resolver:\t[FAIL] %s is not a citizen", citizen.getName());
	else
		/* report back name and the likes */
		logText.Format("@Resolver:\t[SUCCESS] %s is citizen #%d (url: %s)",
			citizen.getName(), citizen.getCitizen(), citizen.getUrl());
	WriteMsg(logText, 0x7733CC, OT_MAIN);
}

void EclipseDialog_OnCallbackCitizenByNumber(Async::Header& Header)
{
	/* abort if we have an error code */
	AW::Citizen citizen = *((AW::Citizen*)Header.p_result);
	String logText;
	if (Header.u_rc != 0)
		logText.Format("@Resolver:\t[FAIL] Citizen #%d is not a citizen", citizen.getCitizen());
	else
		/* report back name and the likes */
		logText.Format("@Resolver:\t[SUCCESS] Citizen #%d is %s (url: %s)",
			citizen.getCitizen(), citizen.getName(), citizen.getUrl());
	WriteMsg(logText, 0x7733CC, OT_MAIN);
}

class CitizenLookupGuiClass : public CitizenLookup::LookupBase
{
public: // setup options
	bool	show_failures;
	String	format;

public:
	void Result(AW::Citizen& Info, int Reason)
	{
		/* abort if we have an error code */
		String logText;
		if (Reason != 0)
		{
			if (show_failures == true)
			{
				logText.Format("@Resolver:\t[FAIL] Citizen #%d is not a citizen", Info.getCitizen());
			}
		}
		else
		{
			// generate vmap
			Umbra::VariantMap vMap = bot.CreateVariantMap();
			vMap["citizen"] = Info;

			// generate response
			String msg = vMap.parse_string((LPCSTR)format).c_str();

			/* report back name and the likes */
			logText.Format("@Resolver:\t%s", (LPCSTR)msg);
		}
		
		WriteMsg(logText, 0x7733CC, OT_MAIN);
	}
};

#include "CitizenLookupDialog.h"
void EclipseEvolutionDialog::OnUniverse_LookupCitizenByName()
{
	// statics to preserve text between uses
	static String format_msg		= "{$citizen.name} is citizen #{$citizen.number}, url='{$citizen.url}'";
	static BOOL  show_failures	= TRUE;

	// citizen lookup box
	CitizenLookupDialog lookupDialog(this);
	lookupDialog.v_Format		= format_msg;
	lookupDialog.v_ShowFailures	= show_failures;
	
	// open dialog box for modal
	if (lookupDialog.DoModal() != IDOK)
		return;

	// copy back data
	format_msg		= lookupDialog.v_Format;
	show_failures	= lookupDialog.v_ShowFailures;

	// split the query data
	SplitString split(lookupDialog.v_Query, ",", -1);
	
	// go through each split item
	for (size_t i = 0; i < split.size(); i++) 
	{
		String trimmed = split[i]; 
		trimmed.Trim();
		
		// must exist
		if (trimmed.GetLength() < 2)
			continue;

		// display
		String logText;
		logText.Format("@Resolver:\tLooking up '%s'", trimmed);
		WriteMsg(logText, 0xCC33CC, OT_MAIN);

		// begin executing this query
		CitizenLookupGuiClass* pLookup = new CitizenLookupGuiClass();
		pLookup->format			= format_msg;
		pLookup->show_failures	= show_failures == TRUE;

		// lookup
		CitizenLookup::g_Kernel.lookup_by_name(trimmed, pLookup);
	}
}

void EclipseEvolutionDialog::OnUniverse_LookupCitizenByNumber()
{
	// statics to preserve text between uses
	static String format_msg		= "Citizen #{$citizen.number} is {$citizen.name}, url='{$citizen.url}'";
	static BOOL  show_failures	= TRUE;

	// citizen lookup box
	CitizenLookupDialog lookupDialog(this);
	lookupDialog.v_Format		= format_msg;
	lookupDialog.v_ShowFailures	= show_failures;
	
	// open dialog box for modal
	if (lookupDialog.DoModal() != IDOK)
		return;

	// copy back data
	format_msg		= lookupDialog.v_Format;
	show_failures	= lookupDialog.v_ShowFailures;

	// split the query data
	SplitString split(lookupDialog.v_Query, ",", -1);
	
	// lookup list
	KnownList list;

	// go through each split item
	for (size_t i = 0; i < split.size(); i++) 
	{
		String trimmed = split[i]; 
		trimmed.Trim();
		
		// must exist
		if (trimmed.GetLength() < 2)
			continue;

		// lookup citizen range
		if (trimmed.Find("~") != -1)
		{
			SplitString em(trimmed, "~", 2);
			if (em.size() != 2)
				continue;

			// elements 1 and 2
			int start = atoi(em[0]);
			int end	  = atoi(em[1]);

			// run through each element
			for (int i = start; i <= end; i++)
			{
				CitizenLookupGuiClass* pLookup = new CitizenLookupGuiClass();
				pLookup->format			= format_msg;
				pLookup->show_failures	= show_failures == TRUE;

				// lookup
				if (CitizenLookup::g_Kernel.lookup_by_number(i, pLookup) == 0)
				{
					list.push_back(ToString(i));
				}
			}
		}
		else
		{
			int user = atoi(trimmed);

			// begin executing this query
			CitizenLookupGuiClass* pLookup = new CitizenLookupGuiClass();
			pLookup->format			= format_msg;
			pLookup->show_failures	= show_failures == TRUE;

			// lookup
			if (CitizenLookup::g_Kernel.lookup_by_number(user, pLookup) == 0)
			{
				list.push_back(ToString(user));
			}
		}
	}

	// log it here
	String logText;
	logText.Format("@Resolver:\tLooking up citizen(s) ", list.toString("and"));
	WriteMsg(logText, 0xCC33CC, OT_MAIN);
}

void EclipseEvolutionDialog::OnUniverse_WorldOptions()
{
	// TODO: Add your command handler code here
}

void EclipseEvolutionDialog::OnUniverse_LookupWorldByName()
{
	// TODO: Add your command handler code here
}

void EclipseEvolutionDialog::OnWorld_Options()
{
	WorldOptionsDialog dlg(this);
	if (dlg.DoModal() == IDOK && bot.HasCT())
	{
		dlg.attributes.to_sdk(bot);
		bot._world_attributes_change();
	}
}

void EclipseEvolutionDialog::OnWorld_ChangeWelcomeMessage()
{
	ImmigrationOfficerDialog dialog(this);
	dialog.v_Message = bot._string(AW_WORLD_WELCOME_MESSAGE);
	dialog.v_Message.Replace("\n", "\r\n");

	// should we set?
	if (dialog.DoModal() != IDOK)
		return;

	// update SDK
	dialog.v_Message.Replace("\r\n", "\n");
	bot._string_set(AW_WORLD_WELCOME_MESSAGE, dialog.v_Message);
	bot._world_attributes_change();
}

#include "worldejectionsdialog.h"
void EclipseEvolutionDialog::OnWorld_Ejections()
{
	WorldEjectionsDialog* dlg = new WorldEjectionsDialog(0);
	dlg->Create(WorldEjectionsDialog::IDD, this);
	dlg->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnComms_ConsoleMessage()
{
}

void EclipseEvolutionDialog::OnComms_ConsoleBroadcast()
{
	static BOOL  bold	= TRUE;
	static BOOL  italic	= FALSE;
	static String prefix;
	static String message;
	static COLORREF colour = 0x00;

	ConsoleMessageDialog cm(this);
	cm.v_Bold	= bold;
	cm.v_Italic	= italic;
	cm.c_Colour = colour;
	if (cm.DoModal() == IDOK) {
		String text;
		if (cm.v_Prefix.GetLength())
			text = cm.v_Prefix + ":\t";
		text+= cm.v_Message;
		bot.consoleBroadcast(text, cm.c_Colour.getColour(), 
			AW::MakeConsoleStyle(cm.v_Bold == TRUE, cm.v_Italic == TRUE, false));

		bold	= cm.v_Bold;
		italic	= cm.v_Italic;
		colour	= cm.c_Colour.getColour();
	}
}

void EclipseEvolutionDialog::OnComms_Hermes_CommandsBegin()
{
	if (bot.IsWorldConnected() == true)
		bot.say("h:begin");
}

void EclipseEvolutionDialog::OnComms_Hermes_PassiveOn()
{
	if (bot.IsWorldConnected() == true)
		bot.say("h:pasv on");
}

void EclipseEvolutionDialog::OnComms_Hermes_PassiveOff()
{
	if (bot.IsWorldConnected() == true)
		bot.say("h:pasv off");
}

void EclipseEvolutionDialog::OnComms_Hermes_End()
{
	if (bot.IsWorldConnected() == true)
		bot.say("h:end");
}

void EclipseEvolutionDialog::OnComms_Eclipse_Ratios()
{
	if (bot.IsWorldConnected() == true)
		bot.say("^ratios");
}

void EclipseEvolutionDialog::OnComms_Eclipse_Present()
{
	if (bot.IsWorldConnected() == true)
		bot.say("^present");
}

void EclipseEvolutionDialog::OnComms_Eclipse_WorldLighting()
{
	if (bot.IsWorldConnected() == true)
		bot.say("^world/li");
}

void EclipseEvolutionDialog::OnComms_Eclipse_Version()
{
	if (bot.IsWorldConnected() == true)
		bot.say("^version");
}

void EclipseEvolutionDialog::OnComms_Eclipse_WorldInfo()
{
	if (bot.IsWorldConnected() == true)
		bot.say("^world/info");
}

void EclipseEvolutionDialog::OnComms_Xelagot_Report()
{
	if (bot.IsWorldConnected() == true)
		bot.say("xelagots report");
}

void EclipseEvolutionDialog::OnComms_Xelagot_StupidBot()
{
	if (bot.IsWorldConnected() == true)
		bot.say("stupid bot");
}

void EclipseEvolutionDialog::OnSupport_UrlMessage()
{
	/* must have a world connection and CT rights */
	if (!bot.HasCT() || !bot.IsWorldConnected())
		return;

	/* ask for the url */
	SelectUrlDialog dlg;
	dlg.v_Url = bot._string(AW_URL_NAME);
	if (dlg.DoModal() != IDOK) return;

	/* log the message */
	bot.broadcastUrl(dlg.v_Url, dlg.v_Target, dlg.v_Post, ConvBool(dlg.v_Target3D));
}

void EclipseEvolutionDialog::OnSupport_UrlBroadcast()
{
	OnSupport_UrlMessage();
}


void EclipseEvolutionDialog::OnReports_Avatar()
{
}


void EclipseEvolutionDialog::OnHelp_About()
{
	// TODO: Add your command handler code here
}

void EclipseEvolutionDialog::OnHelp_CheckForUpgrades()
{
	// TODO: Add your command handler code here
}

void EclipseEvolutionDialog::OnHelp_VisitTheWebsite()
{
	// TODO: Add your command handler code here
}

void EclipseEvolutionDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	last_resize_style = (int)nType;
	last_resize_x = cx;
	last_resize_y = cy;

	/* check window creation */
	if (!IsWindow(c_Log))
		return;

	// interface layout
	InterfaceLayout& layout = InterfaceLayout::getInst();

	/* size constants */
	int		AVATARS_LEFT_WIDTH	= layout.getSidePaneWidth();
	int		AVATARS_MAIN_HEIGHT	= c_AvatarsDialog.getStdHeight();
	int STATUS_BAR_HEIGHT	= 20;
	int WHISPER_HEIGHT		= 200;
	int WORLD_LIST_HEIGHT	= 190;

	int		TECH_HEIGHT			= 100;
	int		SEPERATOR			= 4;
	int		height_available	= cy;
	bool	use_tech_log		= true;

	/* variables */
	int input_top		= -1;
	int input_width		= -1;
	int tech_top		= -1;
	int tech_height		= -1;
	int avatars_left	= -1;
	int	avatars_top		= -1;
	int log_top			= -1;
	int users_height	= cy;

	// check the left alignment
	int main_left = (layout.getLeftCount() == 0 ? 0 : AVATARS_LEFT_WIDTH + SEPERATOR * 2);
	int main_width		= cx - main_left;

	// main width is dependant on range
	main_width = cx - SEPERATOR * 2;

	// left and right divisors
	int left_count	= layout.getLeftCount();
	int right_count	= layout.getRightCount();
	int left_gh			= cy - SEPERATOR - STATUS_BAR_HEIGHT ;
	int right_gh		= cy - SEPERATOR - STATUS_BAR_HEIGHT ;
	

	// tally left items
	if (left_count)
	{
		main_width	-= (AVATARS_LEFT_WIDTH + SEPERATOR);
		left_gh			 = left_gh / left_count;
		int top			 = SEPERATOR;
		int left		 = SEPERATOR;

		// avatars on the left
		if (layout.getAvatarsList() == InterfaceLayout::POSITION_LEFT)
		{
			c_SideUserList.MoveWindow(left, top, AVATARS_LEFT_WIDTH, left_gh - SEPERATOR);
			top+= left_gh;
		}

		// is the users list shown
		if (layout.getUsersList() == InterfaceLayout::POSITION_LEFT)
		{
			c_userList.MoveWindow(left, top, AVATARS_LEFT_WIDTH, left_gh - SEPERATOR);
			top+= left_gh;
		}

		// is the world list shown
		if (layout.getWorldsList() == InterfaceLayout::POSITION_LEFT)
		{
			c_WorldList.MoveWindow(left, top, AVATARS_LEFT_WIDTH, left_gh - SEPERATOR);
			top+= left_gh;
		}
	}

	// tally right items
	if (right_count)
	{
		main_width -= (AVATARS_LEFT_WIDTH + SEPERATOR);
		right_gh		= right_gh / right_count;
		int left		= cx - AVATARS_LEFT_WIDTH - SEPERATOR;
		int top			= SEPERATOR;

		// avatars on the left
		if (layout.getAvatarsList() == InterfaceLayout::POSITION_RIGHT)
		{
			c_SideUserList.MoveWindow(left, top, AVATARS_LEFT_WIDTH, right_gh - SEPERATOR);
			top+= right_gh;
		}

		// is the users list shown
		if (layout.getUsersList() == InterfaceLayout::POSITION_RIGHT)
		{
			c_userList.MoveWindow(left, top, AVATARS_LEFT_WIDTH, right_gh - SEPERATOR);
			top+= right_gh;
		}

		// is the world list shown
		if (layout.getWorldsList() == InterfaceLayout::POSITION_RIGHT)
		{
			c_WorldList.MoveWindow(left, top, AVATARS_LEFT_WIDTH, right_gh - SEPERATOR);
			top+= right_gh;
		}
	}

	/* set the status bar */
	int status_top = height_available - STATUS_BAR_HEIGHT;
	height_available-= STATUS_BAR_HEIGHT + SEPERATOR * 2;

	/* input box */
	input_top = height_available - c_InputDialog.getStdHeight() + SEPERATOR;
	height_available= input_top - SEPERATOR;

	/* tech log? */
	if (layout.getShowMiniLogs() == true) {
		tech_top = height_available - TECH_HEIGHT;
		height_available = tech_top - SEPERATOR;
	}

	/* avatars display */
	avatars_top = height_available - AVATARS_MAIN_HEIGHT;
	height_available = avatars_top - SEPERATOR;

	/* main display */
	log_top = height_available;

	// avatar height tracking
	int left_top = cy - STATUS_BAR_HEIGHT;

	/* force resizes here */
	c_StatusBar.	MoveWindow(0, status_top, cx, STATUS_BAR_HEIGHT);
	c_Log.			MoveWindow(main_left, SEPERATOR, main_width, height_available - SEPERATOR);
	c_InputDialog.	MoveWindow(main_left, input_top, main_width, c_InputDialog.getStdHeight());
	c_TechLog.		MoveWindow(main_left, tech_top, main_width, TECH_HEIGHT);
	c_AvatarsDialog.MoveWindow(main_left, avatars_top, main_width, AVATARS_MAIN_HEIGHT);
	Invalidate();
}

void terrain_callback(int rc)
{
}

void hud_callback(int rc)
{
}

LRESULT EclipseEvolutionDialog::OnWndProc_DnsLookup(WPARAM wParam, LPARAM lParam)
{
	IpToDNS::g_Host.OnResult((HANDLE)wParam, WSAGETASYNCERROR(lParam));
	return (LRESULT)0;
}

LRESULT EclipseEvolutionDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

		case UWM_GETHOSTADDR_RESULT:
		{
			return OnWndProc_DnsLookup(wParam, lParam);
			break;
		}
		
		case UWM_TRAYICON:
		{
			if (lParam == WM_LBUTTONDBLCLK)
			{
				if (IsIconic() == TRUE) 
				{
					ShowWindow(SW_RESTORE);
					SetForegroundWindow();
				} 
				else 
				{
					ShowWindow(SW_MINIMIZE);
					ShowWindow(SW_HIDE);
				}
			}
			else if (lParam == WM_RBUTTONDOWN)
			{
				CMenu* pMenu = GetMenu();
				pMenu = pMenu->GetSubMenu(0);
				
				DWORD pt = GetMessagePos();
				int x = GET_X_LPARAM(pt);	
				int y = GET_Y_LPARAM(pt);
		
				pMenu->TrackPopupMenu(TPM_LEFTALIGN, x, y, this);
			}
			break;
		}

		case UWM_HIDE_MAIN_TASKBAR:
		{
			ShowWindow(SW_HIDE);
			break;
		}
	};
	
	if (message == m_taskbarCreatedMsg)
	{
			CreateTrayIcon();
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void EclipseEvolutionDialog::OnWorld_Importsettings()
{
	const char szFilter[] = "World Attributes (*.awatt)|*.awatt||";
	CFileDialog file(TRUE, "awatt", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// import the data
	AW::EnhancedINI ini;
	ini.Load(file.GetPathName());

	// display exporter
	SerializationEditorDialog dlg;
	dlg.setup(ini, &WorldAttributes::StaticGroupMap());
	if (dlg.DoModal() != IDOK)
		return;

	// updated items
	WorldAttributes config;
	
	// first read from SDK then copy from enhanced INI
	config.from_sdk(bot);
	config.setEnhancedINI(ini);
	
	// shove to world attributes
	config.to_sdk(bot);
	bot._world_attributes_change();
}

void EclipseEvolutionDialog::OnWorld_Exportsettings()
{
	// export the data
	WorldAttributes config;
	config.from_sdk(bot);

	// create INI
	AW::EnhancedINI ini = config.getEnhancedINI();

	// display exporter
	SerializationEditorDialog dlg;
	dlg.setup(ini, &WorldAttributes::StaticGroupMap());
	if (dlg.DoModal() != IDOK)
		return;

	// attribute load
	const char szFilter[] = "World Attributes (*.awatt)|*.awatt||";
	CFileDialog file(FALSE, "awatt", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// save the INI here
	dlg.Result().Save(file.GetPathName());
}

#include "dynamicrights.h"
void EclipseEvolutionDialog::OnSecuity_RightsManager()
{
	// new rights dialog and copy back into main body
	SecurityRightsDialog dlg;
	dlg.provider = DynamicRights::g_Provider;
	
	// copy the assignemnt back if ok'd
	if (dlg.DoModal() == IDOK)
	{
		DynamicRights::g_Provider = dlg.provider;
		avatars.ResetRights();
	}
}

void EclipseEvolutionDialog::OnAvatar_Eject()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	EjectDialog d_Eject(this);
	d_Eject.m_TargetId = g_ContextAvatar->getName();
	d_Eject.m_TitleMsg = String("Eject: ") + g_ContextAvatar->getName();
	d_Eject.v_name		 = g_ContextAvatar->getName();
	d_Eject.v_dns			 = g_ContextAvatar->getAddressInfo();

	if (d_Eject.DoModal() != IDOK)
		return;

	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* create timestamp */
	int seconds = atoi(d_Eject.v_Minutes) * 60 + 
				  atoi(d_Eject.v_Hours) * 3600 + 
				  atoi(d_Eject.v_Days) * (3600 * 24);

	/* nuke the user */
	g_ContextAvatar->Eject(seconds, d_Eject.v_UserMessage, d_Eject.v_Log, NULL);
}

void EclipseEvolutionDialog::OnAvatar_Teleport()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	TeleportDialog dialog;
	dialog.v_String = g_ContextAvatar->getCoordinates();
	dialog.v_World  = bot.getWorld();
	if (dialog.DoModal() == IDOK) {
		/* sanity check */
		if (!g_ContextAvatar)
			return;

		/* translate and send */
		Location lPos(dialog.v_String, NULL);
		if (lPos.IsValid()) {
			String logText;
			logText.Format("Performed AVATAR_TELEPORT on %s to %s %s", g_ContextAvatar->getName(), 
				dialog.v_World.GetLength() == 0 ? bot.getWorld() : dialog.v_World,
				lPos.getCoordinates());
			WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
			g_ContextAvatar->Teleport(lPos, dialog.v_World, false);
		}
	}
}

void EclipseEvolutionDialog::OnAvatar_ClickOnce()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* click once and report */
	String logText;
	logText.Format("Performed AVATAR_CLICK on %s (x1)", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
	g_ContextAvatar->Click(1);
}

void EclipseEvolutionDialog::OnAvatar_ClickTimes10()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* click once and report */
	String logText;
	logText.Format("Performed AVATAR_CLICK on %s (x10)", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
	g_ContextAvatar->Click(10);
}

void EclipseEvolutionDialog::OnAvatar_FollowNormal()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* issue command */
	String cmdText;
	cmdText.Format("follow -normal $%d", g_ContextAvatar->getSession());
	CIS::CommandInterpreter::beginConsole(cmdText, false);
}

void EclipseEvolutionDialog::OnAvatar_FollowBehind()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* issue command */
	String cmdText;
	cmdText.Format("follow -behind $%d", g_ContextAvatar->getSession());
	CIS::CommandInterpreter::beginConsole(cmdText, false);
}

void EclipseEvolutionDialog::OnAvatar_Circle()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* issue command */
	String cmdText;
	cmdText.Format("circle $%d, 10", g_ContextAvatar->getSession());
	CIS::CommandInterpreter::beginConsole(cmdText, false);
}

void EclipseEvolutionDialog::OnAvatar_Goto()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* issue command */
	String cmdText;
	cmdText.Format("teleport av:$%d", g_ContextAvatar->getSession());
	CIS::CommandInterpreter::beginConsole(cmdText, false);
}

void EclipseEvolutionDialog::OnAvatar_ShowReport()
{
	WebClientDialog::createUrlWindow("http://www.awportals.com/", this);
}

void EclipseEvolutionDialog::OnAvatar_ShowDrsProfile()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* popup dialog */
	SecurityListDialog* pDialog = new SecurityListDialog(this);
	
	/* configure it */
	String title;
	pDialog->m_pPermissions = &g_ContextAvatar->getPermissions();

	title.Format("Active Rights of: %s", g_ContextAvatar->getName());
	pDialog->v_Title = title;
	
	pDialog->Create(SecurityListDialog::IDD, this);
	pDialog->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnSupport_Zones_Config()
{
	ZonesConfigDialog dialog;
	dialog.DoModal();
}

void EclipseEvolutionDialog::OnReports_AvatarHistory()
{
	AvatarHistoryDialog* p_Dialog = new AvatarHistoryDialog(this);
	p_Dialog->Create(AvatarHistoryDialog::IDD, this);
	p_Dialog->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnComms_RepeaterWindow()
{
	RepeaterDialog* pDialog = new RepeaterDialog(this);
	pDialog->Create(RepeaterDialog::IDD, this);
	pDialog->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnSupport_AvatarRestrictions()
{
}

void EclipseEvolutionDialog::OnSupport_ConfigureGcs()
{
	CommandConfigDialog dialog(this);
	dialog.DoModal();
}

void EclipseEvolutionDialog::OnSideUsers_RightClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT point, screen;
    GetCursorPos (&point);
	GetCursorPos (&screen);
    ScreenToClient(&point);
    HDHITTESTINFO HitTest;
    /* set up hit-test*/  
    HitTest.pt.x = point.x;
    HitTest.pt.y = point.y;

	int id = c_SideUserList.HitTest(point);
	if (id != -1)
		AvatarContextMenu(*avatars.getSession((int)c_SideUserList.GetItemData(id)), screen.x, screen.y);

	*pResult = 0;
}

String GetTrayIconText()
{
	if (Profiles::g_Profiles.getCurrent() != NULL)
	{
	String caption;
	caption.Format("Evolution: [%s] (%s)\n"
			   "World: %s\n"
			   "Users: %d",
			   bot.getName(), Profiles::g_Profiles.getCurrent()->getId(), bot.getWorld(),
				avatars.size());
	return caption;
	}
	return "";
}

BOOL EclipseEvolutionDialog::CreateTrayIcon()
{
	// connect to notification
	m_taskbarCreatedMsg = RegisterWindowMessage("TaskbarCreated");

	// set up the variables
	memset(&m_IconData, 0, sizeof(m_IconData));
	m_IconData.cbSize	 = sizeof(m_IconData);
	
	// copy over data
	m_IconData.hWnd	=				m_hWnd;
	strcpy(m_IconData.szTip,		"");
	m_IconData.uFlags				= NIF_ICON | NIF_TIP;
	m_IconData.hIcon =				m_hIcon;
	m_IconData.dwInfoFlags =		NIIF_INFO;
	m_IconData.uCallbackMessage	=	UWM_TRAYICON;

	// launch the shell
	return Shell_NotifyIcon(NIM_ADD, &m_IconData);
}

BOOL EclipseEvolutionDialog::DestroyTrayIcon()
{
	return Shell_NotifyIcon(NIM_DELETE, &m_IconData);
}
	
BOOL EclipseEvolutionDialog::UpdateTrayIcon()
{
	m_IconData.hWnd								= m_hWnd;
	strcpy(m_IconData.szTip,		(LPCSTR)GetTrayIconText());
	m_IconData.uFlags							= NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_ICON;
	m_IconData.uCallbackMessage		= UWM_TRAYICON;
	m_IconData.hIcon							= m_hIcon;
	return Shell_NotifyIcon(NIM_MODIFY, &m_IconData);
}

BOOL EclipseEvolutionDialog::UpdateTrayWithPopup(String Title, String Msg, int Icon)
{
	m_IconData.hWnd								= m_hWnd;
	m_IconData.uFlags							= NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_ICON | NIF_INFO;
	m_IconData.uCallbackMessage		= UWM_TRAYICON;
	m_IconData.hIcon							= m_hIcon;
	m_IconData.dwInfoFlags				= Icon;
	m_IconData.uTimeout						= 1000;
	strncpy(m_IconData.szInfoTitle,		(LPCSTR)Title, 63); 
	strncpy(m_IconData.szTip,					(LPCSTR)GetTrayIconText(), 127);
	strncpy(m_IconData.szInfo,				(LPCSTR)Msg, 255);
	return Shell_NotifyIcon(NIM_MODIFY, &m_IconData);
}

void EclipseEvolutionDialog::OnClose()
{
	if (MessageBox(_T("Are you sure you want to quit?"), 
		_T("Quit"), MB_YESNOCANCEL | MB_ICONQUESTION) != IDYES)
	{
		return;
	}
	
	// close the profile here
	CloseProfile(Profiles::g_Profiles.getCurrent()->getId());

	/* shut em down */
	bot.Destroy();
	DestroyTrayIcon();
	CDialog::OnClose();
}

void EclipseEvolutionDialog::OnCancel()
{
	CDialog::OnCancel();
}

void EclipseEvolutionDialog::OnOK()
{
}

void EclipseEvolutionDialog::OnUniverse_CreateCitizen()
{
	/* sanity checking */
	if (!bot.IsWorldConnected() || !bot.HasCT())
		return;

	/* show citizen creation dialog */
	String name, email, password;
	while (true) {
		UniverseCitizenAddDialog userDialog(this);
		userDialog.v_UserName	= name;
		userDialog.v_EMail		= email;
		userDialog.v_Password	= password;
		if (userDialog.DoModal() == IDOK) {
			String logText;
			int rc = bot.CitizenAdd(userDialog.v_UserName, userDialog.v_Password, userDialog.v_EMail, FALSE);
			if (rc == RC_SUCCESS) {
				logText.Format("User Created:\n\nUser: %s\nCitizen: %d", bot._string(AW_CITIZEN_NAME), bot._int(AW_CITIZEN_NUMBER));
				MessageBox(logText, NULL, MB_ICONINFORMATION);
				return;
			} else {
				logText.Format("Could not create '%s'\n\nReason: %d - %s", userDialog.v_UserName,
					rc, AW::Bot::GetReasonCode(rc));
				MessageBox(logText, NULL, MB_ICONWARNING);
				name	= userDialog.v_UserName;
				email	= userDialog.v_EMail;
				password= userDialog.v_Password;
			}
		} else {
			return;
		}
	}
}

void EclipseEvolutionDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	int nIDx = 0xFFF0 & nID;
	if (nIDx == SC_MINIMIZE) 
	{
		CDialog::OnSysCommand(nID, lParam);
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void EclipseEvolutionDialog::OnAvatar_SendNoise()
{

}

void EclipseEvolutionDialog::OnView_Map()
{
	MapDialog* pDlg = new MapDialog(this);
	pDlg->Create(MapDialog::IDD, this);
	pDlg->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnComms_MuteList()
{
	TextListDialog dialog(this);
	dialog.v_List = Comms::g_ChatFilter.CreateList();
	if (dialog.DoModal() == IDOK)
		Comms::g_ChatFilter.CreateFromList(dialog.v_List);
}

void EclipseEvolutionDialog::OnComms_ConsoleMuteList()
{
	TextListDialog dialog(this);
	dialog.v_List = Comms::g_ConsoleFilter.CreateList();
	if (dialog.DoModal() == IDOK)
		Comms::g_ConsoleFilter.CreateFromList(dialog.v_List);
}

void EclipseEvolutionDialog::OnAvatar_ChangeDisplayColour()
{
	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* show option box for avatars colour */
	CColorDialog d_colour(g_ContextAvatar->getDisplayColour(), NULL, this);
	if (d_colour.DoModal() != IDOK)
		return;

	/* sanity check */
	if (!g_ContextAvatar)
		return;

	/* save back */
	g_ContextAvatar->setDisplayColour(d_colour.GetColor());
}

void EclipseEvolutionDialog::OnView_ChatSetup()
{
	InterfaceConfigDialog dlg;
	dlg.DoModal();
}

void EclipseEvolutionDialog::OnSideUsers_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nID = c_SideUserList.GetByHitTest();
	if (nID == -1)
		return;
	
	// check avatar 
	Avatar *pClicked = avatars.getSession((int)c_SideUserList.GetItemData(nID));
	if (!pClicked)
		return;
	
	// check if target is already locked
	if (pClicked->whisper_target)
	{
		pClicked->whisper_target = false;
		c_SideUserList.EditItem(nID, 1, _T(""), Data::av_icons.blank);
	}
	else
	{
		pClicked->whisper_target = true;
		c_SideUserList.EditItem(nID, 1, _T(""), Data::av_icons.whisper);
	}
}

void EclipseEvolutionDialog::WhisperTargetAdd(Avatar& User)
{
}

void EclipseEvolutionDialog::WhisperTargetChange(Avatar& User)
{
}

void EclipseEvolutionDialog::WhisperTargetDelete(Avatar& User)
{
}

void EclipseEvolutionDialog::WhisperTargetLock(Avatar& User)
{
	const int locked_icon = c_WhisperTargets.getIconKey("locked");
	int nID = c_WhisperTargets.InsertItem(c_WhisperTargets.GetItemCount(), User.getName());
	c_WhisperTargets.SetItemData(nID, (DWORD_PTR)User.getSession());
	c_WhisperTargets.EditItem(nID, 0, User.getName(), locked_icon);
}

void EclipseEvolutionDialog::WhisperTargetUnlock(Avatar& User)
{
	int nID = c_WhisperTargets.GetByItemData((DWORD_PTR)User.getSession());
	if (nID != -1)
	{
		c_WhisperTargets.DeleteItem(nID);
	}
}


void EclipseEvolutionDialog::WhisperTargets_OnDoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// look up the item
	int id = c_WhisperTargets.GetByHitTest();
	if (id == -1)
		return;

	/* check avatar */
	Avatar *pClicked = avatars.getSession((int)c_WhisperTargets.GetItemData(id));
	if (!pClicked)
		return;

	/* remove whisper target */
	pClicked->whisper_target = false;

	/* delete from list */
	c_WhisperTargets.DeleteItem(id);
}

void EclipseEvolutionDialog::WhisperTargetsReload()
{
	int count = c_SideUserList.GetItemCount();
	for (int i = 0; i < count; i++)
	{
		DWORD_PTR nID = c_SideUserList.GetItemData(i);
		Avatar* pSession = avatars.getSession((int)nID);

		c_SideUserList.EditItem(i, 1, _T(""), 
			pSession->whisper_target ? Data::av_icons.whisper : Data::av_icons.blank);
	}
}
void EclipseEvolutionDialog::OnAvatar_RelayJoin()
{
	// sanity check
	if (!g_ContextAvatar) 
		return;

	// log the event
	String logText;
	logText.Format("Performed EXTENDER_JOIN on %s", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
}

void EclipseEvolutionDialog::OnAvatar_RelayKick()
{
	// sanity check
	if (!g_ContextAvatar) 
		return;

	// log the event
	String logText;
	logText.Format("Performed EXTENDER_KICK on %s", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
}

void EclipseEvolutionDialog::OnAvatar_RelayBan()
{
	// sanity check
	if (!g_ContextAvatar) 
		return;

	// log the event
	String logText;
	logText.Format("Performed EXTENDER_BAN on %s", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
}

void EclipseEvolutionDialog::OnAvatar_RelayPassiveOn()
{
	// sanity check
	if (!g_ContextAvatar) 
		return;

	// log the event
	String logText;
	logText.Format("Performed EXTENDER_PASSIVE_ENABLE on %s", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
}

void EclipseEvolutionDialog::OnAvatar_RelayPassiveOff()
{
	// sanity check
	if (!g_ContextAvatar) 
		return;

	// log the event
	String logText;
	logText.Format("Performed EXTENDER_PASSIVE_DISABLE on %s", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
}

void EclipseEvolutionDialog::OnAvatar_RelayLeave()
{
	// sanity check
	if (!g_ContextAvatar) 
		return;

	// log the event
	String logText;
	logText.Format("Performed EXTENDER_LEAVE on %s", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);
}

void EclipseEvolutionDialog::OnSecuity_SystemWordFilter()
{
	// create the dialog
	ChatFilterDialog dialog(this);
	dialog.v_List		= Security::g_FilterPolicy.words.CreateList();
	dialog.v_Enabled	= Security::g_FilterPolicy.enabled;
	dialog.v_Duration	= Security::g_FilterPolicy.duration;
	dialog.v_Message	= Security::g_FilterPolicy.message;

	// configure and open the dialog
	if (dialog.DoModal() != IDOK)
		return;

	// copy info back
	Security::g_FilterPolicy.words.CreateFromList(dialog.v_List);
	Security::g_FilterPolicy.enabled	= dialog.v_Enabled;
	Security::g_FilterPolicy.duration	= dialog.v_Duration;
	Security::g_FilterPolicy.message	= dialog.v_Message;
}

void EclipseEvolutionDialog::OnSecuity_FloodPreventionPolicy()
{
	FloodPreventionDialog dialog(this);
	dialog.v_MaxPer5					= Security::g_FloodPolicy.max_rate;
	dialog.v_MaxIdentical			= Security::g_FloodPolicy.max_repeat;
	dialog.v_AutoMute					= Security::g_FloodPolicy.auto_mute_on;
	dialog.v_AutoMuteDuration	= Security::g_FloodPolicy.auto_mute_len;
	dialog.v_Enabled					= Security::g_FloodPolicy.enabled;

	// configure and open the dialog
	if (dialog.DoModal() != IDOK)
		return;

	// copy back
	Security::g_FloodPolicy.max_rate			= dialog.v_MaxPer5;
	Security::g_FloodPolicy.max_repeat		= dialog.v_MaxIdentical;
	Security::g_FloodPolicy.auto_mute_on	= dialog.v_AutoMute;
	Security::g_FloodPolicy.auto_mute_len	= dialog.v_AutoMuteDuration;
	Security::g_FloodPolicy.enabled				= dialog.v_Enabled;
}

void LaunchDatabase(String SQL, String Title)
{
	AvatarsDatabaseDialog* pDialog = new AvatarsDatabaseDialog(rt_diag);
	pDialog->Create(AvatarsDatabaseDialog::IDD, rt_diag);
	pDialog->SetWindowTextA(Title);
	pDialog->Query(SQL);
	pDialog->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnAvatar_DatabaseQueryName()
{
	/* sanity check */
	if (!g_ContextAvatar) return;

	/* create string and execute */
	String sqlText;
	sqlText.Format("SELECT * FROM local_users WHERE name='%s' ORDER BY enter_time", g_ContextAvatar->getName());
	LaunchDatabase(sqlText, String("Matching: ") + g_ContextAvatar->getName());
}

void EclipseEvolutionDialog::OnAvatar_DatabaseQueryCitizen()
{
	/* sanity check */
	if (!g_ContextAvatar) return;

	/* create string and execute */
	String sqlText;
	sqlText.Format("SELECT * FROM local_users WHERE citizen=%d ORDER BY enter_time", g_ContextAvatar->getCitizen());
	LaunchDatabase(sqlText, "Lookup up citizen number");
}

void EclipseEvolutionDialog::OnAvatar_DatabaseQueryPrivilege()
{
	/* sanity check */
	if (!g_ContextAvatar) return;

	/* create string and execute */
	String sqlText;
	sqlText.Format("SELECT * FROM local_users WHERE privilege=%d ORDER BY enter_time", g_ContextAvatar->getPrivilege());
	LaunchDatabase(sqlText, "Lookup up privilege number");
}

void EclipseEvolutionDialog::OnAvatar_DatabaseQueryIpAddress()
{
	/* sanity check */
	if (!g_ContextAvatar) return;

	/* create string and execute */
	String sqlText;
	sqlText.Format("SELECT * FROM local_users WHERE ip='%s' ORDER BY enter_time", g_ContextAvatar->getAddress());
	LaunchDatabase(sqlText, String("Matching: ") + g_ContextAvatar->getAddress());
}

void EclipseEvolutionDialog::OnAvatar_DatabaseQueryDnsAddress()
{
	/* sanity check */
	if (!g_ContextAvatar) return;

	/* create string and execute */
	String sqlText;
	sqlText.Format("SELECT * FROM local_users WHERE name='%s' ORDER BY enter_time", g_ContextAvatar->getName());
	LaunchDatabase(sqlText, String("Matching: ") + g_ContextAvatar->getName());
}

#include "PropertySelectProject.h"
void EclipseEvolutionDialog::OnWorld_QueryManager()
{
	// show the window of projects
	PropertySelectProject dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// select this project
	CWnd* pWnd = GetDesktopWindow();
	QueryInterfaceDialog* pDialog = new QueryInterfaceDialog(pWnd);
	pDialog->projectName = dlg.selected_id;
	pDialog->Create(QueryInterfaceDialog::IDD, pWnd);
	pDialog->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnSupport_ZoneEditor()
{
	ZoneEditorDialog dlg;
	dlg.root = *Zones::g_ZonesMgr.GetRoot();
	dlg.root.update_parents();
	
	if (dlg.DoModal() == IDOK)
	{
		(*Zones::g_ZonesMgr.GetRoot()) = dlg.root;
		Zones::g_ZonesMgr.GetRoot()->update_after_changes();

		for (int i = 0; i < static_cast<int>(avatars.size()); i++)
		{
			avatars[i].ZoneReset();
			avatars[i].ZoneModifyPos(avatars[i]);
		}
	}
}

void EclipseEvolutionDialog::OnAvatar_ChangeGlobalRelayColour()
{
}

void EclipseEvolutionDialog::OnAvatar_ChangeGlobalRelaySettings()
{
}

void EclipseEvolutionDialog::OnSupport_IpDnsAnnouncement()
{
}

void EclipseEvolutionDialog::OnBot_Savestatus()
{
	if (Profiles::g_Profiles.getCurrent() != NULL)
		CloseProfile(Profiles::g_Profiles.getCurrent()->getId());		// autosave settings
	
}

void EclipseEvolutionDialog::OnBot_Crashbot()
{
	if (MessageBox(_T("Are you sure you wish to crash this program with a memory read exception?\n\nUnsaved settings will be lost."), NULL, MB_YESNOCANCEL) != IDYES)
		return;

	int* iPtr = reinterpret_cast<int*>(0);
	*iPtr = 0;
}

void EclipseEvolutionDialog::OnSupport_RelayManagement()
{
	RelayConfigDialog dlg(this);
	dlg.DoModal();
}

void EclipseEvolutionDialog::OnBot_Shutdown()
{
	// check they legitimately want to shut down
	if (MessageBox(_T("Are you sure you wish to shut down?"), NULL, MB_YESNOCANCEL) != IDYES)
		return;

	// close the profile here
	CloseProfile(Profiles::g_Profiles.getCurrent()->getId());

	/* shut em down */
	bot.Destroy();
	DestroyTrayIcon();
	EndDialog(IDOK);
}

void EclipseEvolutionDialog::OnSupport_HeadsUp()
{
	// create a new hud dialog and rig it with the relevant array
	HudConfigDialog dlg;
	dlg.icon_array = HeadsUp::Config::getInstance().stored_items;
	
	// start the icon
	if (dlg.DoModal() == IDOK)
	{
		HeadsUp::Config::getInstance().stored_items = dlg.icon_array;
	}
}

void EclipseEvolutionDialog::OnAvatar_AssumeAvatar()
{
	// sanity check
	if (!g_ContextAvatar) return;

	// log the event
	String logText;
	logText.Format("Performed ASSUME_AVATAR on %s", g_ContextAvatar->getName());
	WriteMsg(logText, RGB_BLUE, OT_MAIN | OM_TIME);

	// assume
	bot.ChangeAvatar(g_ContextAvatar->getAvatar());
}

void EclipseEvolutionDialog::OnAvatar_LookupCitizen()
{
	// sanity check
	if (!g_ContextAvatar) return;

	// new query dialog
	UniverseCitizenDialog* pDialog = new UniverseCitizenDialog(this);
	
	// set the auto query and launch
	pDialog->auto_query = g_ContextAvatar->getCitizen();
	pDialog->Create(UniverseCitizenDialog::IDD, this);
	pDialog->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnAvatar_LookupPrivilege()
{
	// sanity check
	if (!g_ContextAvatar) return;

	// new query dialog
	UniverseCitizenDialog* pDialog = new UniverseCitizenDialog(this);
	
	// set the auto query and launch
	pDialog->auto_query = g_ContextAvatar->getPrivilege();
	pDialog->Create(UniverseCitizenDialog::IDD, this);
	pDialog->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnView_ClearMainLog()
{
	if (MessageBox(_T("Do you wish to erase your current display log?\n\nExisting text remains in your file logs."), NULL, MB_ICONQUESTION | MB_YESNOCANCEL) == IDYES)
		c_Log.EraseContents();
}

void EclipseEvolutionDialog::OnComms_BotgramEmail()
{
}

#include "scripteditordialog.h"
void EclipseEvolutionDialog::OnSupport_ScriptingEditor()
{
	ScriptEditorDialog* pDlg = new ScriptEditorDialog(this);
	pDlg->Create(ScriptEditorDialog::IDD, this);
	pDlg->ShowWindow(SW_SHOW);
}

#include "MoversDialog.h"
void EclipseEvolutionDialog::OnSupport_MoverControl()
{
	MoversDialog* pDlg = new MoversDialog(this);
	pDlg->Create(MoversDialog::IDD, this);
	pDlg->ShowWindow(SW_SHOW);
}

//
//
// Hotkey switching of input mode
//
//

void EclipseEvolutionDialog::OnView_SelectChat()
{
	c_InputDialog.SwitchToCheck(c_InputDialog.c_Speak);
	c_InputDialog.c_InputBox.SetFocus();
}

void EclipseEvolutionDialog::OnView_SelectWhisper()
{
	c_InputDialog.SwitchToCheck(c_InputDialog.c_Whisper);
	c_InputDialog.c_InputBox.SetFocus();
}

void EclipseEvolutionDialog::OnView_SelectCommands()
{
	c_InputDialog.SwitchToCheck(c_InputDialog.c_Commands);
	c_InputDialog.c_InputBox.SetFocus();
}

void EclipseEvolutionDialog::OnView_SelectConsole()
{
	c_InputDialog.SwitchToCheck(c_InputDialog.c_Console);
	c_InputDialog.c_InputBox.SetFocus();
}

void EclipseEvolutionDialog::OnView_SelectConsoleMode()
{
	c_InputDialog.OnBnClickedConsoleMode();
}

#include "QueryWorldProgressDialog.h"
void EclipseEvolutionDialog::OnWorld_BackupView()
{
	QueryWorldProgressDialog* d = new QueryWorldProgressDialog(this);
	d->Create(QueryWorldProgressDialog::IDD, this);
	d->ShowWindow(SW_SHOW);
}

#include "RecordingDialog.h"
void EclipseEvolutionDialog::OnFeatures_Recorder()
{
	RecordingDialog d;
	d.DoModal();
}

#include "RelayChannelsDialog.h"
void EclipseEvolutionDialog::OnFeatures_RelayChannelEditor()
{
	RelayChannelsDialog d;
	d.DoModal();
}

void EclipseEvolutionDialog::OnAvatar_BotgramToCitizen()
{
}

void EclipseEvolutionDialog::OnAvatar_BotgramToPrivilege()
{
}

#include "isScriptCommandsDialog.h"
void EclipseEvolutionDialog::OnFeatures_ScriptingCisCommands()
{
	CisScriptCommandsDialog dialog(this);
	dialog.DoModal();
}

#include "ReportDialog.h"
#include "reports.h"

void EclipseEvolutionDialog::OnReports_IpOwnershipAndColissions()
{
	// a new report plz
	Reports::IpCollisions rep;
	rep.generateReport();

	// create a new window for it
	ReportDialog repWnd;
	repWnd.v_output = rep.generateReport();
	repWnd.DoModal();
}

#include "WorldsListDialog2.h"
void EclipseEvolutionDialog::OnUniverse_WorldList()
{
	// create a new control dialog
	WorldsListDialog* pDlg = new WorldsListDialog();
	pDlg->Create(WorldsListDialog::IDD, this);
	pDlg->ShowWindow(SW_SHOW);
}

void EclipseEvolutionDialog::OnWorldList_HeaderClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;

	// sort based on index
	c_WorldList.sortToggle(phdr->iItem);
}

void EclipseEvolutionDialog::OnBnClickedShowUsers()
{
	c_userList.ShowWindow(c_userToggle.GetCheck() ? SW_SHOW : SW_HIDE);
	ForceResize();
}

void EclipseEvolutionDialog::OnBnClickedShowWorlds()
{
	c_WorldList.ShowWindow(c_toggleWorld.GetCheck() ? SW_SHOW : SW_HIDE);
	ForceResize();
}

//
// Layout Routine
//

#include "ModalPropertyDialog2.h"
class LayoutCallbackClass : public ModalPropertyDialog2::ICallback
{
public:
	InterfaceLayout::PropertyBuilder *pb;

	void run()
	{
		pb->pAvatarsList->InsertByMap(InterfaceLayout::getInst().positions);
		pb->pUsersList->InsertByMap  (InterfaceLayout::getInst().positions);
		pb->pWorldsList->InsertByMap (InterfaceLayout::getInst().positions);
	}
};

void EclipseEvolutionDialog::OnView_Layout()
{
	ModalPropertyDialog2 d(this);
	
	// rig for update
	InterfaceLayout::PropertyBuilder builder;
	InterfaceLayout& data = InterfaceLayout::getInst();
	builder.assign_remote(data);

	// setup the header information
	LayoutCallbackClass lcc;
	lcc.pb = &builder;
	d.init_callback = &lcc;
	d.m_builderBase = &builder;	
	d.title = "Layout Options";				

	// popup info
	if (d.DoModal() != IDOK)
		return;

	// apply updates
	data.apply(*this);
	ForceResize();
}

#include "JumpManagerDialog.h"
void EclipseEvolutionDialog::OnFeatures_JumpPointsManager()
{
	// create window
	JumpManagerDialog* pWnd = new JumpManagerDialog(this);
	pWnd->Create(JumpManagerDialog::IDD, this);
	pWnd->ShowWindow(SW_SHOW);
}

#include "DesktopConnector.h"
BOOL EclipseEvolutionDialog::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
//	WriteMsg("IPC CopyData Received", 0, OT_MAIN);
	DesktopConnector::getInst().onMessage(*pCopyDataStruct, pWnd->GetSafeHwnd());
	return __super::OnCopyData(pWnd, pCopyDataStruct);
}
