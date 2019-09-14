#include "stdafx.h"
#include "CIS_Communications.h"
#include "CIS_World.h"
#include "CIS_Basic.h"
#include "CIS_Movers.h"
#include "CIS_Property.h"
#include "CIS_SlideShow.h"
#include "CIS_ChatRelay.h"
#include "CIS_ConfigInterface.h"
#include "CIS_Universe.h"
#include "CIS_UniverseUserList.h"
#include "scripting_host.h"
#include "cis_dispatch.h"
#include "cis_toolkit.h"
#include "cis_interfaces.h"
#include "cisscriptcommands.h"

using namespace CIS::Exec;
#define IMPL_FUNCTION(phrase, process)	if (cmd == phrase) { return new process(); }

namespace CIS
{
	String GetBotSearch()
	{
		CStringA text = bot.getName();
		text.MakeLower();
		return  text;
	}

	int CommandInterpreter::beginAvatar(Avatar& Source, String Text, bool RunAdmin, int ChatType)
	{
		// test the start of the command
		String lcmsgText = Text; 
		lcmsgText.MakeLower();
		String parseText;
		String bot_search = GetBotSearch();

		/* it must start with either our prefix, or via the bots name */
		String command_prefix = CIS::Config::g_Interface.getPrefixKey();

		// test the bot searches
		if (lcmsgText.Left(bot_search.GetLength()) == bot_search)
			parseText = Text.Mid(bot_search.GetLength());
		else if (lcmsgText.Left(command_prefix.GetLength()) == command_prefix)
			parseText = Text.Mid(command_prefix.GetLength());
		else
			return 0;

		// split the commands
		SplitString args(parseText, " ", 2);
		String argKey = args[0], argText;
		if (args.size() == 2) argText = args[1];

		// create execution param
		Exec::IExec* pExec = CommandInterpreter::getClass(argKey);
		if (pExec)
		{
			pExec->rigForAvatar(Source, argText);
			pExec->chat_type = ChatType;
			int reason = pExec->run();
			if (reason != Reason::CompletionDelayed)
				delete pExec;
		}

		return 0;
	}

	int CommandInterpreter::beginConsole(String Text, bool Admin)
	{
		// split the commands
		SplitString args(Text, " ", 2);
		String argKey = args[0], argText;
		if (args.size() == 2) argText = args[1];

		// create execution param
		Exec::IExec* pExec = CommandInterpreter::getClass(argKey);
		if (pExec)
		{
			pExec->rigForClient(argText);
			pExec->chat_type = 0;
			int reason = pExec->run();
			if (reason != Reason::CompletionDelayed)
				delete pExec;
		}

		return 0;
	}

	Exec::IExec* CommandInterpreter::getClass(const char* Id)
	{
		CStringA cmd = Id;

		// check for functions
		CIS::ScriptCommands& sc = CIS::ScriptCommands::getInstance();
		for (CIS::ScriptCommands::Iterator itr = sc.begin(); itr != sc.end(); itr++)
		{
			if (itr->getId().CompareNoCase(cmd) == 0)
			{
				using namespace CIS::Exec;
				ScriptedProcessor sp;
				
				// allocate bitmap
				sp.context = 0;
				if (itr->getContextAvatar() == true) sp.context|= IExec::CONTEXT_AVATAR;
				if (itr->getContextConsole() == true) sp.context|= IExec::CONTEXT_CONSOLE;

				// requirements
				sp.bitmask = 0;
				if (itr->getRequiresCaretaker() == true) sp.bitmask|= IExec::REQUIRES_CARETAKER;
				if (itr->getRequiresEject()		== true) sp.bitmask|= IExec::REQUIRES_EJECT;
				if (itr->getRequiresUniverse()	== true) sp.bitmask|= IExec::REQUIRES_UNIVERSE;
				if (itr->getRequiresWorld()		== true) sp.bitmask|= IExec::REQUIRES_WORLD;

				// set function name
				sp.function_name = itr->getScriptFunction();
				sp.required_right = itr->getRequiredRight();
				return new ScriptedProcessor(sp);
			}
		}

		IMPL_FUNCTION("version",				InfoProcessor);
		IMPL_FUNCTION("about",					InfoProcessor);

		// common commands
		IMPL_FUNCTION("present",				PresentProcessor);
		IMPL_FUNCTION("near",						NearProcessor);				// documented
		IMPL_FUNCTION("ratios",					RatiosProcessor);			// documented
		IMPL_FUNCTION("locate",					LocateProcessor);			// documented
		IMPL_FUNCTION("match",					WildcardsProcessor);		// documented
		IMPL_FUNCTION("identify",				IdentifyProcessor);			// documented
		IMPL_FUNCTION("botstatus",			StatusProcessor);			// documented
		IMPL_FUNCTION("myrights",				MyRightsProcessor);
		IMPL_FUNCTION("mydrs",					MyDrsProcessor);
		IMPL_FUNCTION("coords",					CoordsProcessor);
		IMPL_FUNCTION("where",					WhereProcessor);
		IMPL_FUNCTION("clickmode",			ClickmodeProcessor);
		IMPL_FUNCTION("hud",						HudProcessor);
		
		// universe functions
		IMPL_FUNCTION("citnum",					CitnumProcessor);
		IMPL_FUNCTION("citname",				CitnameProcessor);
		
		// toolkit commands
		IMPL_FUNCTION("exec",						ExecProcessor);
		IMPL_FUNCTION("toolkit/sendurl",ToolkitSendUrlProcessor);
		IMPL_FUNCTION("toolkit/imp",		ToolkitImpersonateProcessor);
	
		// movement
		IMPL_FUNCTION("follow",					FollowProcessor);
		IMPL_FUNCTION("circle",					CircleProcessor);
		IMPL_FUNCTION("halt",						HaltProcessor);

		// bindings
		IMPL_FUNCTION("bind",						BindProcessor);
		IMPL_FUNCTION("ubind",					UbindProcessor);

		// for the global commands
		IMPL_FUNCTION("freejoin",				FreeJoinProcessor);
		IMPL_FUNCTION("join",						JoinProcessor);
		IMPL_FUNCTION("thrust",					ThrustProcessor);
		IMPL_FUNCTION("boost",					ThrustProcessor);
		IMPL_FUNCTION("coords",					CoordsProcessor);

		// ejection and teleportation
		IMPL_FUNCTION("eject",					EjectProcessor);		// documented
		IMPL_FUNCTION("avteleport",			AvTeleportProcessor);		// documented
		IMPL_FUNCTION("takeme",					TakeMeProcessor);			// documented
		IMPL_FUNCTION("bringme",				BringMeProcessor);			// documented
		
		// driver control
		/*
		IMPL_FUNCTION("drv/create",			DriverCreateProcessor);
		IMPL_FUNCTION("drv/del",				DriverDeleteProcessor);
		IMPL_FUNCTION("drv/pause",			DriverPauseProcessor);
		IMPL_FUNCTION("drv/go",					DriverGoProcessor);
		*/

		// relay functions
		IMPL_FUNCTION("relay/uo",				RelayUoProcessor);
		IMPL_FUNCTION("mute",						MuteProcessor);
		IMPL_FUNCTION("unmute",					UnmuteProcessor);
		IMPL_FUNCTION("color",					ColorProcessor);
		IMPL_FUNCTION("colour",					ColorProcessor);
		
		// chat dynamics
		IMPL_FUNCTION("jc",							JcProcessor);
		IMPL_FUNCTION("qc",							QcProcessor);

		// channel server
		IMPL_FUNCTION("cs/join",				JcProcessor);
		IMPL_FUNCTION("cs/quit",				QcProcessor);
		IMPL_FUNCTION("cs/create",			ChanservCreateProcessor);
		IMPL_FUNCTION("cs/users",				ChanservUsersProcessor);
		IMPL_FUNCTION("cs/delete",			ChanservDeleteProcessor);
		IMPL_FUNCTION("cs/list",				ChanservListProcessor);
		IMPL_FUNCTION("cs/opts",				ChanservOptsProcessor);
		IMPL_FUNCTION("cs/pasv",				PassiveProcessor);
		IMPL_FUNCTION("cs/opts",				ChanservOptsProcessor);
		IMPL_FUNCTION("cs/default",			ChanservDefaultProcessor);
		IMPL_FUNCTION("cs/kick",				ChanservKickProcessor);

		// short methods for chat relay
		IMPL_FUNCTION("chat",						ChanservProcessor);
		IMPL_FUNCTION("channels",				ChanservListProcessor);
		IMPL_FUNCTION("who",						ChanservUsersProcessor);
		IMPL_FUNCTION("use",						ChanservDefaultProcessor);
		IMPL_FUNCTION("connect",				JcProcessor);
		IMPL_FUNCTION("quit",						QcProcessor);

		// world modification
		IMPL_FUNCTION("world/cio",			WorldCioProcessor);
		IMPL_FUNCTION("world/ccio",			WorldCcioProcessor);
		IMPL_FUNCTION("world/mvset",		WorldMoveModeProcessor);
		IMPL_FUNCTION("world/li",				WorldLiProcessor);
		IMPL_FUNCTION("world/modify",		WorldModifyProcessor);
		IMPL_FUNCTION("world/grant",		WorldGrantProcessor);
		IMPL_FUNCTION("world/revoke",		WorldRevokeProcessor);
		IMPL_FUNCTION("world/showrights",	WorldShowrightsProcessor);
		IMPL_FUNCTION("world/ep",				WorldEpProcessor);
		IMPL_FUNCTION("world/import",		WorldImportProcessor);

		// communications related
		IMPL_FUNCTION("say",					SayProcessor);
		IMPL_FUNCTION("whisper",			WhisperProcessor);
		IMPL_FUNCTION("ssound",				NoiseProcessor);
		IMPL_FUNCTION("cmsg",					CMsgProcessor);
	
		// jump points
		IMPL_FUNCTION("jump/create",		JumpCreateProcessor);
		IMPL_FUNCTION("jump",						JumpProcessor);
		IMPL_FUNCTION("jump/delete",		JumpDeleteProcessor);
		IMPL_FUNCTION("jump/replace",		JumpRelocateProcessor);
		IMPL_FUNCTION("jump/near",			JumpNearProcessor);
		IMPL_FUNCTION("jump/list",			JumpListProcessor);
		IMPL_FUNCTION("jump/search",		JumpSearchProcessor);

		// admin commands
		IMPL_FUNCTION("system/drs/createaccount",	SystemDrsCreateAccountProcessor);
		IMPL_FUNCTION("system/drs/createright",		SystemDrsCreateRightProcessor);

		// nothing found
		return NULL;
	}
}