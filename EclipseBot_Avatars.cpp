#include "stdafx.h"					// for precompiled header
#include "EclipseBot.h"				// for EclipseBot
#include "EclipseEvolutionDialog.h"	// for main dialog, special rtf writing
#include "GlobalEvent.h"			// handles the tracking of citizenships
#include "IpToDNS.h"				// lookup the dns information
#include "AvatarHistory.h"
#include "DialogCore.h"
#include "avatar_usersettings.h"
#include "awsdk_world.h"
#include "CommonGreetings.h"
#include "RelayRelay.h"
#include "messages.h"
#include "HeadsUp_UserPlugin.h"

#include "scripting_host.h"
#include "RelayGlobalChat.h"
#include "recorder.h"
#include "worldattributes.h"
#include "umbra/umbra_classlib_usl_vector.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

#define avatars		()

// prototype for later function
void event_handle_privilege_lookup(Async::Header& Header);
void event_handle_dns_result(Async::Header& Header);

void EclipseBot::onProcessAvatarAdd(Avatar& User)
{
	// call superclass
	__super::onProcessAvatarAdd(User);

	//
	// Update first time information
	//

	if (User.first_encounter_time == 0)
	{
		User.first_encounter_time		= time64(0);
		User.first_encounter_world		= _string(AW_WORLD_NAME);
		User.first_encounter_position	= getCellCoordinates();
	}
	
	//
	// Welcome Messages
	//

	// welcome message
	String logText, encounterLog;
	logText.Format("AVATAR_ADD: %s ", User.getName());

	// encounter type
	if (User.getPreviousEncounterCount() == 0)
		encounterLog = "(first encounter)";
	else
		encounterLog.Format("(%d encounters)", User.getPreviousEncounterCount());

	// add the rest of the message
	logText.AppendFormat("#%d enters [%s] at %s build %d %s", 
		User.getSession(), User.getCID(), User.getCoordinates(), User.getBuild(),
		encounterLog);
	WriteMsg(logText, 0x883333, OT_MAIN | OM_TIME);

	// 
	// Initiate subsystems
	// 

	// load the users profile
	User.setDisplayColour(0xffffff);
	
	// Initiate world override
	User.m_pWorldOverride->from_sdk(*this);

	//
	// scripting host
	//

	ScriptingHost::getInst().doFunction("EventAvatarCreate", 
		&Umbra::VariantArray(User), 0, &bot);

	//
	// Automatic Welcome Messages
	//

	// Welcome Message
	Umbra::VariantMap uv_map = CreateVariantMap(&User);
	if (CommonFeatures::g_Greetings.getEnabled())
		CommonFeatures::g_Greetings.getEnter().Execute(&User, uv_map);

	// Sound Messages
	if (HasCT() && CommonFeatures::g_Greetings.getSound())
		User.SendSound(CommonFeatures::g_Greetings.getSound());

	//
	// Head Up Display
	//

	
	// add the data elements for this code
	HeadsUp::Element em1;	
	em1.text			= "hud_collapse_br.png";
	em1.click_method	= HeadsUp::Element::CLICK_METHOD_COLLAPSE;
	em1.setSize(32, 64, 0);
	em1.index			= 10000; 		
	User.getHud().push_back(em1);

	HeadsUp::Element em2;	
	em2.text			= "hud_expand_br.png";
	em2.click_method	= HeadsUp::Element::CLICK_METHOD_EXPAND;
	em2.setSize(32, 64, 0);
	em1.index			= 100000;
	User.getHud().push_back(em2);

	//
	// allocate each item
	//

	for (HeadsUp::Config::TrayItemArray::iterator itr = HeadsUp::Config::getInstance().stored_items.begin();
		itr != HeadsUp::Config::getInstance().stored_items.end();
		itr++)
	{
		// skip over each element that is not enabled
		if (itr->enabled == false)
			continue;

		// if pattern is ok if one exists
		if (itr->pattern.GetLength() != 0 && User.IsPattern(itr->pattern) == false)
			continue;

		// add the item
		HeadsUp::Element e;
		
		// key index
		e.index				= itr->index;
		e.key				= itr->key;
		e.visible			= true;
		e.row				= itr->row;

		// image type
		e.text				= itr->image;
		e.size_x			= itr->size_x;
		e.size_y			= itr->size_z;

		// click method use
		e.click_function	= itr->click_function;
		e.click_method		= itr->click_method;

		// add to the tray icon
		User.getHud().push_back(e);
	}

	//
	// Final 
	//

	// zone initialisation
	User.ZoneModifyPos(User);

	//
	// scripting host
	//

	ScriptingHost::getInst().doFunction("EventAvatarAdd", 
		&Umbra::VariantArray(User), 0, &bot);
	
	// 
	// History
	//

	User.setHistoricId(History::AvatarHistory::getInstance().begin(User));
	Recorder::Recording::getInstance().onAvatarAdd(User);

	//
	// handle avatar entry
	//
	if (HasCT() == true) 
	{
		ChatControllers::g_Relay.handle_avatar_add(User);
	}

	// trigger updates
	User.getHud().update_all();

	//
	// Graphical User Interface
	//

	// add to the dialog
	rt_diag->AvatarAdd(User);
	rt_diag->UpdateTrayIcon();

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventAvatarAdd2", 
		&Umbra::VariantArray(User), 0, &bot);
}

void EclipseBot::onProcessAvatarChange(Avatar& User)
{
	// call superclass
	__super::onProcessAvatarChange(User);

	// updates to GUI
	rt_diag->c_AvatarsDialog.AvatarMove(User);

	// modify the users position
	User.updateLastMovementTime();
	User.ZoneModifyPos(User);

	// trigger reflective updates on this avatar
	User.activateHyperionByRefreshConditions();

	//
	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventAvatarChange", 
		&Umbra::VariantArray(User), 0, &bot);

	// history
	// Recorder::Recording::getInstance().onAvatarChange(User);
}

void EclipseBot::onProcessAvatarDelete(Avatar& User)
{
	// call superclass
	__super::onProcessAvatarDelete(User);

	// history
	Recorder::Recording::getInstance().onAvatarDelete(User);

	// handle avatar exiting
	if (HasCT() == true )
		ChatControllers::g_Relay.handle_avatar_delete(User);

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventAvatarDelete", 
		&Umbra::VariantArray(User), 0, &bot);

}

void EclipseBot::onProcessAvatarCleanup(Avatar& User)
{
	// call superclass
	__super::onProcessAvatarCleanup(User);

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventAvatarCleanup", 
		&Umbra::VariantArray(User), 0, &bot);

	// cleanup
	User.OnExit();
	rt_diag->UpdateTrayIcon();
}

void EclipseBot::onProcessAvatarClick(Avatar& User, Avatar& Clicked)
{
	// call superclass
	__super::onProcessAvatarClick(User, Clicked);

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventAvatarClick", 
		&Umbra::VariantArray(User, Clicked), 0, &bot);
}

// repetition of checks
#include "RepetitionTimer.h"
#include "InterfaceConfig.h"
#define AKA_CHECK_INTERVAL (60 * 30)			// 30 minutes
RepetitionTimer	akaTimer;

void EclipseBot::onProcessAvatarAddress(Avatar& User, int Reason)
{
	// call superclass
	__super::onProcessAvatarAddress(User, Reason);

	// reason code
	unsigned int addr_uint = (unsigned int)(_int(AW_AVATAR_ADDRESS));
	User.OnIpLookupResult(addr_uint, Reason);

	// query the list of users that have previously had this IP
	History::AvatarHistory::ResultSet historyResult = History::AvatarHistory::getInstance().queryIp(User.getAddressLng());
	History::AvatarHistory::ResultSet usedResults;
	KnownList pastAv;
	String	  akaList;

	for (int i = (int)historyResult.size() - 1; i >= 0; i--)
	{
		// if our user is a bot, only show other bots
		if (User.IsBot() == true)
			continue;

		// condition requires a different name and not to be bot
		if (historyResult[i]->getName() == User.getName() || historyResult[i]->IsBot() == true)
			continue;


		// duality checking on the names
		bool bDual = false;
		for (size_t j = 0; j < usedResults.size(); j++)
		{
			if (usedResults[j]->getName() == historyResult[i]->getName())
			{
				bDual = true;
				break;
			}
		}

		// only if there are no duplicates by this name
		if (bDual == false)
		{
			usedResults.push_back(historyResult[i]);
			pastAv.push_back(historyResult[i]->getName());
		}
	}

	// build the impersonate list
	if (pastAv.size() != 0) 
	{
		// check if aka is to be shown
		bool inTimer = akaTimer.test(User.GetBestLock(), g_InterfaceConfig.getLogging().getShowAkaRepetition());

		if (g_InterfaceConfig.getLogging().getShowAka() && inTimer)
		{ 
			akaList.Format("<Sec.aka>\t%s (%s) is also known as: " + pastAv.toString("and") + "", User.getName(), User.getAddress());
			WriteMsg(akaList, RGB_NOTIFY, OT_MAIN);
		}

		// aka hit test
		if (inTimer)
		{
			using namespace Umbra::ClassLib::Usl;
			VectorImpl res;
			for (size_t i = 0; i < usedResults.size(); i++)
				res.arr.push_back(*usedResults[i]);

			// scripting hosting
			ScriptingHost::getInst().doFunction("EventAvatarHistoricIpMatch", 
				&Umbra::VariantArray(User, res), 0, &bot);
		}

		// test done
		akaTimer.hit(User.GetBestLock());
	}

	// lookup dns
	IpToDNS::g_Host.LookupNow(addr_uint, event_handle_dns_result, NULL);
	User.BeginLookupDns();
	rt_diag->c_AvatarsDialog.AvatarUpdate(User);
}

void EclipseBot::onProcessAvatarPrivilege(Avatar& User) 
{
	rt_diag->c_AvatarsDialog.AvatarUpdate(User);
}

void event_handle_dns_result(Async::Header& Header)
{
	/* precalculate changes */
	unsigned int ip_uint = (unsigned int)(Header.u_msg);		// copy buffer
	int ip_int			 = (int)(Header.u_msg); 
	String ip_str		 = UnsignedIpToString(ip_uint);
	HOSTENT* pHostEnt	 = (HOSTENT*)Header.p_result;

	/* for each of our avatars check if the DNS is needed */
	for (size_t i = 0; i < bot.getAvatars().size(); i++)
	{
		Avatar& user = bot.getAvatars()[i];

		unsigned int address = user.getAddressLng();

		/* check if this avatar needs resolving */
		if (!(address == ip_uint && 
			user.getDnsLUS() == LookupState::Resolving))
		{
			continue;
		}

		/* if we have an error, set as nothing */
		if (Header.u_rc == 0) 
			user.OnDnsLookupResult(pHostEnt->h_name, LookupState::Complete);
		else 
			user.OnDnsLookupResult("", LookupState::Unavailable);

		// scripting host
		//
		ScriptingHost::getInst().doFunction("EventAvatarAddress", 
			&Umbra::VariantArray(user), 0, &bot);

		user.EndLookupDns();
		rt_diag->c_AvatarsDialog.AvatarUpdate(user);
	}
}