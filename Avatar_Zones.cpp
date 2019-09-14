#include "stdafx.h"
#include "Avatar.h"
#include "Zones.h"
#include "DialogCore.h"
#include "Resource.h"
#include "AWSDK_HudElement.h"
#include "AWSDK_World.h"
#include "miniparser.h"
#include "Zones_ConfigGeneral.h"
#include "cis_dispatch.h"
#include "awsdk_world.h"
#include "scripting_host.h"
#include "worldattributes.h"

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Zones;

bool Avatar::ZAE_KeyConforms(String Key)
{
	return IsPattern(Key);
}

void Avatar::onEventZoneChanged(Zones::Zone* pOld, Zones::Zone* pNew)
{
	if (pOld == NULL) 
	{
		ZoneAttach(*pNew);
	} 
	else 
	{
		// old zone? detatch, kill, kill!
		if (pNew->parent_zone != pOld)
			ZoneDetach(*pOld);

		// scripting host for changing between the event hosts
		ScriptingHost::getInst().doFunction("EventAvatarZoneChanging", 
			&Umbra::VariantArray(*this, *pOld, *pNew), 0, &bot);

		// new zone connection is active
		ZoneAttach(*pNew);
	}
}

void Avatar::onEventZoneReset(void)
{
}

void Avatar::ZoneDetach(Zones::Zone& zone)
{
	//bool is_public = Zones::g_Settings.enable_notify;
	//if (is_public && zone.getExitMsg().GetLength() > 0) 
	//{
	//	String  msg = Zones::g_Settings.msg_exit;
	//	msg.Replace("{name}",			getName());
	//	msg.Replace("{zone_name}",		zone.getZoneName());
	//	msg.Replace("{zone_name_full}", zone.getFullDescriptor());
	//
	//	/* send the welcome message */
	//	ConsoleMessageEx(msg, 0x66AA44, CM_SUPPRESSED | CM_BOLD);
	//}
}

bool Avatar::ZoneDoWelcome(Zones::Zone& TheZone)
{
	///* send the welcome message */
	if (g_Config.getEnabled() && plc_bot->HasCT()) 
	{
		// create input map
		Umbra::VariantMap vMap	= bot.CreateVariantMap(this);
		vMap["zone"]			= *const_cast<Zones::Zone*>(&TheZone);

		// create the welcome
		String enter_msg = vMap.parse_string((LPCSTR)g_Config.getWelcomeMessage()).c_str();
		
		// if there is no data in it, quit
		if (enter_msg.IsEmpty() == true)
			return false;

		// pass to the announcement format
		vMap["text"]	= (LPCSTR)enter_msg; 
		g_Config.getAnnouncementFormat().Execute(this, vMap);

		// enter noise
		if (TheZone.getEnterNoise().GetLength())
		{
			SendSound(vMap.parse_string((LPCSTR)TheZone.getEnterNoise()).c_str());
		}

		return true;
	}

	return false;
}

void Avatar::useHyperionZone(WorldAttributes& Attr)
{
	// hyperion
	if (Zones::g_Config.getHyperionKernel().getEnabled() == false) 
		return;

	// selectivly target bots
	if (getType() == UserType::Bot && g_Config.getHyperionKernel().getTargetBots() == false)
		return;

	// current zone
	Zones::Zone& TheZone = *getZone();

	// do we have a local template?
	String sTemplate = TheZone.getLogicalTemplate();
	if (sTemplate.GetLength() != 0)
	{
		// lookup list of paths
		Umbra::VariantMap vMap = bot.CreateVariantMap(this);
		SplitString paths(g_Config.getHyperionKernel().getTemplatePath(), ";", -1);
		for (size_t i = 0; i < paths.size(); i++)
		{
			// file path
			vMap["template"] = (LPCSTR)sTemplate;;
			String file_path = vMap.parse_string((LPCSTR)paths[i]).c_str() ;
			file_path+= ".awatt";

			// file does not exist, skip
			if (FileExists(file_path) == false)
				continue;

			// find
			if (Attr.read_file(file_path) != 0)
			{
				String logText;
				logText.Format("Unable to access world template '%s' to apply to %s", file_path, getName());
				WriteMsg(logText, RGB_RED, OT_DEBUG | OT_ERROR | OM_TIME);
			}
		}
	}

	// do we override fly / shift etc
	bool others_overriden = false;
	tsb_t fly		= TheZone.getLogicalFly();
	tsb_t shift		= TheZone.getLogicalShift();
	tsb_t teleport	= TheZone.getLogicalTeleport();

	// are any forcing a change
	if (fly != B_EMPTY || shift != B_EMPTY || teleport != B_EMPTY)
		others_overriden = true;

	// find the determined booleans
	Attr.allow_flying		= ConvBool(tsb_check(fly,	   Attr.allow_flying));
	Attr.allow_passthru		= ConvBool(tsb_check(shift,	   Attr.allow_passthru));
	Attr.allow_teleport		= ConvBool(tsb_check(teleport, Attr.allow_teleport));

	// are there any additional commands that need to be applied
	String sFinal = TheZone.getLogicalFinal();
	if (sFinal.GetLength() > 0)
	{
		MiniParser mini(sFinal);
		for (size_t i = 0; i < mini.args.size(); i++)
			Attr.set_by_name(mini.args[i].key, mini.args[i].text);
	}
}


bool Avatar::ZoneDoRestriction(Zones::Zone& TheZone)
{
	// do we need to check for access permissions?
	if (g_Config.getEnableProtection() == false)
	{
		m_zone_confirmed = true;
		return false;
	}

	// check the permissions against the avatar
	if (TheZone.getCheckPermission(*this) == false)
	{
		// create a command to execute
		Umbra::VariantMap vMap = bot.CreateVariantMap(this);
		vMap["zone"] = TheZone;

		// message them
		ConsoleWhisper(g_Config.getProtectionAlert(), RGB_RED, CM_SUPPRESSED);

		// execute the command
		vMap["arg"] = (const char*)TheZone.getLogicalDenialArgument();
		CIS::CommandInterpreter::beginConsole(vMap.parse_string((LPCSTR)g_Config.getProtectionAction()).c_str(), true);
		m_zone_confirmed = false;
		return true;
	}
	else
	{
		m_zone_confirmed = true;
		return false;
	}
}


bool Avatar::ZoneDoWelcome2(Zones::Zone& TheZone)
{
	return false;
}

void Avatar::ZoneAttach(Zones::Zone& Zone)
{
	/* trigger zone functions */
	if (plc_bot->HasCT() == true)
	{
		ZoneDoWelcome(Zone);
	}
	
	// check the zone restriction
	if (ZoneDoRestriction(Zone) == true)
		return;

	// both systems require caretaker
	if (plc_bot->HasCT()) ZoneDoWelcome2(Zone);

	// scripting host
	ScriptingHost::getInst().doFunction("EventAvatarZoneChange", 
		&Umbra::VariantArray(*this, Zone), 0, &bot);
	
	// request hyperion re-enables
	activateHyperion();
}
