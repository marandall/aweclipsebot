#include "stdafx.h"				// for precompiled header
#include "EclipseBot.h"			// for EclipseBot
#include "Zones.h"				// for ns zones
#include "DialogCore.h"
#include "Resource.h"
#include "EclipseEvolutionDialog.h"
#include "EclipseDialog_Timers.h"
#include "property_objects.h"
#include "property_query_5x5.h"
#include "awsdk_world.h"
#include "CommonAutoResponding.h"
#include "CIS_Dispatch.h"
#include "CIS_Configuration.h"
#include "umbra/umbra_variantmap.h"
#include "HeadsUp_UserPlugin.h"
#include "scripting_host.h"
#include "worldattributes.h"
#include "world_ejections.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

void EclipseBot::onCallbackWorldEjection(int rc)
{
	getEjectManager().HandleCallbackWorldEjection(rc);
}

class BotCapabilities
{
protected: // the check to carry out
	struct BoolCheck
	{
		AW_ATTRIBUTE attr_id;
		String		 key;
		bool		 available;
	};

	vector<BoolCheck>	m_list;

public:
	size_t size() const { return m_list.size(); }

	void push_back(AW_ATTRIBUTE Att, String Key)
	{
		BoolCheck bc = { Att, Key, false };
		m_list.push_back(bc);
	}

	void CheckNew(vector<String>& T)
	{
		for (size_t i = 0; i < m_list.size(); i++)
			if (bot._bool(m_list[i].attr_id) == TRUE && m_list[i].available == false)
				T.push_back(m_list[i].key);
	}

	void CheckLost(vector<String>& T)
	{
		for (size_t i = 0; i < m_list.size(); i++)
			if (bot._bool(m_list[i].attr_id) == FALSE && m_list[i].available == true)
				T.push_back(m_list[i].key);
	}

	void UpdateList()
	{
		for (size_t i = 0; i < m_list.size(); i++)
			m_list[i].available = bot._bool(m_list[i].attr_id) == TRUE;
	}
};

BotCapabilities	world_rights;

/* events triggered by world events or connection states */
void EclipseBot::onEventVisible(void)
{
	String logText;
	logText.Format("I appear visible in %s", getWorld());
	WriteMsg(logText, RGB_NOTIFY, OT_MAIN | OM_TIME);

	/* update position */
	onEventMoveStateChanged();
}

bool EclipseBot::GetGlobal(void) const
{
	return _int(AW_ENTER_GLOBAL) == TRUE && HasCT();
}

void EclipseBot::onEventWorldDisconnect(int ReasonCode)
{
	/* variables for this fucntion */
	String	logText, eject_list;
	bool	srcFound = false;

	/* signal disconnection */
	switch ( ReasonCode )
	{
	case RC_EJECTED:
		logText.Format("WORLD_DISCONNECT: I have been ejected from %s. Reason %d (%s)",
			getWorld(), RC_EJECTED, AW::Bot::GetReasonCode( ReasonCode ));

		/* break out */
		break;

	case RC_NOT_WELCOME:
		logText.Format("WORLD_DISCONNECT: I am no longer permitted in %s. Reason %d (%s)",
			getWorld(), RC_NOT_WELCOME, AW::Bot::GetReasonCode( ReasonCode ));
		break;
	default:
		logText.Format("WORLD_DISCONNECT: I have been disconnected from the world %s. Reason %d (%s)",
			getWorld(), ReasonCode, AW::Bot::GetReasonCode( ReasonCode ));
		break;
	};

	/* print out message */
	WriteMsg(logText, RGB_FAIL, OT_DEBUG | OT_MAIN | OT_TECH | OM_TIME);

	// carry out cleanup on connection based sessions
	avatars.Clear("World Disconnect");
	getLiveCache().empty_project();

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventWorldDisconnect", 
		&Umbra::VariantArray(ReasonCode), 0, &bot);
	
	// are we attempting to recover
	if ( getWorldConnection() == ConState::Recovering )
	{
		logText.Format("Attempting to restore connection to %s", getWorld());
		WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);
	} 
	else 
	{
		logText.Format("Closing connection to %s due to explicit removal", getWorld());
		WriteMsg(logText, RGB_FAIL, OMT_SIGNAL);
		ExitWorld();
	}
}

void EclipseBot::onEventWorldIOChange(void)
{	
	/* variables used */
	String logText;
	SplitString split(_string(AW_WORLD_WELCOME_MESSAGE), "\n", -1);
	logText.Format("World IO:\t%s", split[0]);
	WriteMsg(logText, RGB_IOMSG, MF_FBOLD | OT_MAIN);

	/* print everything else off thereafter */
	for (size_t i = 1; i < split.size(); i++) 
	{
		logText.Format(">\t%s", split[i]);
		WriteMsg(logText, RGB_IOMSG, OT_MAIN);
	}
}

UINT_PTR m_pTimerRes = 0;

void EclipseBot::onEventWorldAttributes(void)
{
	String logText;

	// execute updated timer
	int time_x = _int(AW_WORLD_AVATAR_REFRESH_RATE);
	if (time_x == 0) time_x = 1;
	if (m_pTimerRes) rt_diag->KillTimer(m_pTimerRes);
	m_pTimerRes = rt_diag->SetTimer(TIMER_PROC_AVATAR_UPDATES, 1000 / time_x, NULL);

	// if this is the first time we encounter world attributes we create our world rights
	if (world_rights.size() == 0)
	{
		world_rights.push_back(AW_WORLD_CARETAKER_CAPABILITY,		"Caretaker");
		world_rights.push_back(AW_WORLD_EJECT_CAPABILITY,			"Eject");
		world_rights.push_back(AW_WORLD_PUBLIC_SPEAKER_CAPABILITY,	"Public Speaker");
		world_rights.push_back(AW_WORLD_EMINENT_DOMAIN_CAPABILITY,	"Eminent Domain");
		world_rights.push_back(AW_WORLD_BUILD_CAPABILITY,			"Build");
		world_rights.push_back(AW_WORLD_SPEAK_CAPABILITY,			"Speak");
	}

	// check for comparisons
	vector<String> rt_new, rt_lost;
	world_rights.CheckNew(rt_new);
	world_rights.CheckLost(rt_lost);
	if (rt_new.size() || rt_lost.size())
	{
		KnownList klNew, klLost;
		for (size_t i = 0; i < rt_new.size(); i++)
			klNew.push_back(rt_new[i]);
		for (size_t i = 0; i < rt_lost.size(); i++)
			klLost.push_back(rt_lost[i]);

		// display  the changes
		String logText;

		// notify of rights that the system has aquired
		if (klNew.size())
		{
			logText.Format("@CapMgr+:\tI have aquired %s", klNew.toString("and"));
			WriteMsg(logText, RGB_NOTIFY, OT_MAIN | MF_FBOLD);
		}

		// notify of rights that the system has lost
		if (klLost.size())
		{
			logText.Format("@CapMgr-:\tI have lost %s", klLost.toString("and"));
			WriteMsg(logText, RGB_RED, OT_MAIN | MF_FBOLD);
		}

		// force an update of the command codes
		world_rights.UpdateList();
	}

	/* automatically resolve users if we have eject */
	if (HasCT() || HasEject())
	{
		for (size_t i = 0; i < avatars.size(); i++)
		{
			Avatar& user = avatars[i];
			if (user.getAddressLUS() == LookupState::Complete || user.getAddressLUS() == LookupState::Complete)
				continue;
			else
			{
				_address(user.getSession());
				user.BeginLookupIp();
			}
		}
	}

	// update the records of everyones avatars
	if (_int(AW_ATTRIB_SENDER_SESSION) == 0)
	{
		// reset each user
		for (size_t i = 0; i < avatars.size(); i++)
		{
			avatars[i].getWorldOverride().from_sdk(*this);
			avatars[i].activateHyperion();
		}
	}

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventWorldAttributes", 
		&Umbra::VariantArray(), 0, &bot);
}

void EclipseBot::onEventMoveStateChanged(void)
{
	String dispText;
	if (IsWorldConnected()) {
		String global_addon = GetGlobal() ? " [global]" : "";
		if (getInvisible())
			dispText.Format("\t%s : Invisible%s", getWorld(), global_addon);
		else
			dispText.Format("\t%s : %s%s", getWorld(), getCellCoordinates(), global_addon);
	} else {
		dispText.Format("\tTargeting %s (status %s)", getAL_World(), "1");
	}
	rt_diag->setSB_Position(dispText);
}

void EclipseBot::onEventMove(void)
{
	/* update position */
	onEventMoveStateChanged();
}

void EclipseBot::onEventWorldConnectionChange(int state)
{
	bool connected = IsWorldConnected();

	// world messages */
	SetRootMenuEnabled(ID_COMMS_CONSOLE_MESSAGE,				connected);
	SetRootMenuEnabled(ID_COMMS_CONSOLE_BROADCAST,				connected);

	// world menu
	SetRootMenuEnabled(ID_WORLD_CHANGE_WELCOME_MESSAGE,			connected);
	SetRootMenuEnabled(ID_WORLD_IMPORT_SETTINGS,				connected);
	SetRootMenuEnabled(ID_WORLD_EXPORT_SETTINGS,				connected);
	SetRootMenuEnabled(ID_WORLD_BACKUPWORLD,					connected);

	// url messages 
	SetRootMenuEnabled(ID_SUPPORT_URL_MESSAGE,					connected);
	SetRootMenuEnabled(ID_SUPPORT_URL_BROADCAST, 				connected);

	// hermes commands */
	SetRootMenuEnabled(ID_HERMES_COMMANDS_BEGIN,				connected);
	SetRootMenuEnabled(ID_HERMES_COMMANDS_PASSIVE,				connected);
	SetRootMenuEnabled(ID_HERMES_COMMANDS_PASSIVE_OFF,			connected);
	SetRootMenuEnabled(ID_HERMES_COMMANDS_PASSIVE_ON,			connected);
	SetRootMenuEnabled(ID_HERMES_COMMANDS_END,					connected);

	// evo gcs commands */
	SetRootMenuEnabled(ID_EVOLUTIONGCS_COMMANDS_RATIOS,			connected);
	SetRootMenuEnabled(ID_EVOLUTIONGCS_COMMANDS_PRESENT,		connected);
	SetRootMenuEnabled(ID_EVOLUTIONGCS_COMMANDS_WORLDINFO,		connected);
	SetRootMenuEnabled(ID_EVOLUTIONGCS_COMMANDS_WORLDLIGHTING,	connected);
	SetRootMenuEnabled(ID_EVOLUTIONGCS_COMMANDS_VERSION,		connected);
	SetRootMenuEnabled(ID_EVOLUTIONGCS_COMMANDS_WORLD_LIGHTING,	connected);
	SetRootMenuEnabled(ID_EVOLUTIONGCS_COMMANDS_WORLD_INFO,		connected);
	
	// xelagot commands */
	SetRootMenuEnabled(ID_XELAGOT_COMMANDS_REPORT,				connected);
	SetRootMenuEnabled(ID_XELAGOT_COMMANDS_STUPID_BOT,			connected);

	// world information */
	SetRootMenuEnabled(ID_WORLD_OPTIONS,		connected);
	SetRootMenuEnabled(ID_WORLD_EJECTIONS,	connected);

	// update position */
	onEventMoveStateChanged();

	// link open
	if (state == ConState::Connected)
		onProcessWorldLinkOpened();
	else if (state == ConState::Nothing || state == ConState::Recovering)
		onProcessWorldLinkClosed();
		

	// dialog update */
	rt_diag->UpdateTrayIcon();

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventWorldConnectionChange", 
		&Umbra::VariantArray(state), 0, &bot);
}

void EclipseBot::onProcessWorldLinkOpened()
{
	// query for the first time
	if (HasCT() == true)
		getEjectManager().beginQuery(true);

	// scripting host
	ScriptingHost::getInst().doFunction("EventWorldLinkOpened", 
		&Umbra::VariantArray(), 0, &bot);
}

void EclipseBot::onProcessWorldLinkClosed(void)
{
	// commit and clear the cache
	getQuery5x5().CommitCache();
	getQuery5x5().ResetCache();

	// erase the current link
	getLiveCache().reset();
	getEjectManager().cleanQuery();

	// assign zero query
	setQueryMode(CommonBot::QUERY_NONE);

	// scripting host
	ScriptingHost::getInst().doFunction("EventWorldLinkClosed", 
		&Umbra::VariantArray(), 0, &bot);
}

int EclipseBot::ExitWorld(void)	
{
	/* this is the exit function that checks if we can exit and does it and launches */
	String logText;
	if (getWorldConnection() == ConState::Connecting)
	{
		logText.Format("[%s] aborts entering %s", getName(), getWorld());
	}
	else if (getWorldConnection() == ConState::Recovering)
	{
		logText.Format("[%s] aborts recovering into %s", getName(), getWorld());
	}
	else
	{
		logText.Format("[%s] exits %s", getName(), getWorld());
	}

	WriteMsg(logText, RGB_NOTIFY, OM_TIME | OT_BOTH);

	/* clea up the hud */
	if (HasCT())
		for (size_t i = 0; i < avatars.size(); i++)
			avatars[i].getHud().hide_all();

	/* clear up the avatars list and exit */
	avatars.Clear("Exiting World...");
	return AW::Bot::ExitWorld();
}

#include "movement_interface.h"

void EclipseBot::onEventTeleport(void)
{
	// world and coordinates
	AW::Location dest(_int(AW_TELEPORT_X), _int(AW_TELEPORT_Y), _int(AW_TELEPORT_Z), _int(AW_TELEPORT_YAW));
	CStringA world = _string(AW_TELEPORT_WORLD);
	if (world.GetLength() == 0) world = bot.getWorld();

	// announce  teleport command
	CStringA logMsg;
	logMsg.Format("EVENT_TELEPORT instructs %s to teleport to %s %s", getBotName(), world, dest.getCoordinates());

	// if we are on eject and we are set to ignore them on eject, only print it
	if (getInvisible() == TRUE)
	{
		logMsg += " (Ignored due to broadcast)";
		WriteMsg(logMsg, RGB_RED, OT_MAIN | OM_TIME);
		return;
	}
	else if (CommonFeatures::g_AutoResponding.getIgnoreTeleportsOnEject() && HasEject())
	{
		logMsg += " (Ignored due to config)";
		WriteMsg(logMsg, RGB_RED, OT_MAIN | OM_TIME);
		return;
	}
	else if (CommonFeatures::g_AutoResponding.getAllowTeleportsBetweenWorlds() == false 
			&& bot.getWorld().CompareNoCase(world) != 0)
	{
		logMsg += " (Ignored due to config)";
		WriteMsg(logMsg, RGB_RED, OT_MAIN | OM_TIME);
		return;
	}

	// notify of teleport
	WriteMsg(logMsg, RGB_BLUE, OT_MAIN | OM_TIME);
	teleport(world, dest.getCoordinates(), GetGlobal(), false);
}

void EclipseBot::onEventHudClick(void)
{
	// session lookups
	int session = _int(AW_HUD_ELEMENT_SESSION	);
	int id			= _int(AW_HUD_ELEMENT_ID		);
	int click_x	= _int(AW_HUD_ELEMENT_CLICK_X	);
	int click_y = _int(AW_HUD_ELEMENT_CLICK_Y	);

	// lookup the avatar
	Avatar* pUser = avatars.getSession(session);
	if (pUser == NULL) 
		return;

	// check icons
	HeadsUp::Element* pElement = pUser->getHud().lookup_id(id);
	if (pElement == NULL)
	{
		// search general set
		pElement = pUser->getFreeTray().getElementById(id);
		if (pElement == 0)
			return;
	}
	// lookup in general set


	// look up the key
	int click_method = pElement->click_method;
	if (click_method == HeadsUp::Element::CLICK_METHOD_CIS || click_method == HeadsUp::Element::CLICK_METHOD_URL)
	{
		// format message
		Umbra::VariantMap vMap = bot.CreateVariantMap(pUser);
		Umbra::string text = vMap.parse_string((LPCSTR)pElement->click_function);

		if (click_method == HeadsUp::Element::CLICK_METHOD_CIS)
		{
			String cmdText;
			cmdText.Format("%s%s", CIS::Config::g_Interface.getPrefixKey(), text.c_str());
			CIS::CommandInterpreter::beginAvatar(*pUser, cmdText, false, AW_CHAT_SAID);
		}
		else if (click_method == HeadsUp::Element::CLICK_METHOD_URL)
		{
			pUser->SendURL(text.c_str(), "", true, "");
		}
	}
	else if (click_method == HeadsUp::Element::CLICK_METHOD_SIDE_URL)
	{
		pUser->SendURL(pElement->click_function, "", false);
	}
	else if (pElement->click_method == HeadsUp::Element::CLICK_METHOD_SCRIPT)
	{
		// click position
		pElement->click_x = _int(::AW_HUD_ELEMENT_CLICK_X);
		pElement->click_y = _int(::AW_HUD_ELEMENT_CLICK_Y);
		pElement->click_z = _int(::AW_HUD_ELEMENT_CLICK_Z);

		// scripting host
		ScriptingHost::getInst().doFunction(pElement->click_function, 
			&Umbra::VariantArray(*pUser, *pElement), 0, &bot);
	}
	else if (pElement->click_method == HeadsUp::Element::CLICK_METHOD_TELEPORT)
	{
		::Location lc(pElement->click_function, pUser);
		pUser->Teleport(lc);
	}
	else if (pElement->click_method == HeadsUp::Element::CLICK_METHOD_EXPAND)
	{
		pUser->getHud().expand();
	}
	else if (pElement->click_method == HeadsUp::Element::CLICK_METHOD_COLLAPSE)
	{
		pUser->getHud().collapse();
	}
	else if (pElement->click_method == HeadsUp::Element::CLICK_METHOD_CHANGEROW)
	{
		pUser->getHud().row = pElement->click_function;
		pUser->getHud().update_all();
	}
}