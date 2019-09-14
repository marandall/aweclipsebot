#include "stdafx.h"				// for precompiled header
#include "EclipseBot.h"			// for EclipseBot
#include "Avatar.h"				// for avatar class
#include "Resource.h"
#include <vector>
#include "CIS_Dispatch.h"		// for CIS::control
#include "DialogCore.h"
#include "ConsoleFilter.h"
#include "SystemWordsFilter.h"
#include "FloodPreventionPolicy.h"
#include "RelayRelay.h"
#include "CIS_ConfigInterface.h"
#include "helpers_xml.h"
#include "InterfaceConfig.h"
#include "scripting_host.h"
#include "recorder.h"
#include "BotgramProtocolV2.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

CStringA OsVersionToText(int _Major, int _Minor, int _Revision, int _Type) ;

void EclipseBot::onProcessChat(Avatar& User, CStringA Msg, int Type)
{
	if (User.getMuted() == true)
		return;

	// hit update change
	User.updateLastChatTime();

	// the standard colours
	COLORREF	chat_colour = 0x00;
	int			chat_style	= 0x00;
	String		logText;

	// history
	Recorder::Recording::getInstance().onChat(User, Msg, Type);

	/* are we printing the user information as a whisper */
	if (Type == AW_CHAT_WHISPER)  
	{
		chat_colour	= RGB(0,0,0xff);
		chat_style	= OT_MAIN | MF_ITALIC;
	}
	else 
	{
		/* colour types */	
		COLORREF chat_clr[] = { 
			g_InterfaceConfig.getChatDisplay().getDefaultTouristColour().getColour(), 
			g_InterfaceConfig.getChatDisplay().getDefaultCitizenColour().getColour(), 
			g_InterfaceConfig.getChatDisplay().getDefaultBotColour().getColour()};
		
		/* if the chat colour is white we use a default colour */	
		if (User.getDisplayColour() == 0xFFFFFF)
		{
			chat_colour = chat_clr[User.getType()];
		}
		else
		{
			chat_colour = User.getDisplayColour();
		}
		chat_style = OT_MAIN | (Type == AW_CHAT_BROADCAST ? MF_FBOLD : 0);
	}

	// write the message
	if (g_InterfaceConfig.getLogging().getShowChat()) {
		logText.Format("%s:\t%s%s", User.getName(), g_InterfaceConfig.getChatDisplay().getLogicalChatPrefix(), Msg);
		WriteMsg(logText, chat_colour, chat_style);
	}

	// scripting host
	ScriptingHost::getInst().doFunction("EventChat", &Umbra::VariantArray(User, (const char*)Msg, Type), 0, &bot);


	// chat relay relay if possible
	if (Type != AW_CHAT_WHISPER && HasCT() && 
		ChatControllers::g_Relay.getRelayMode() != ChatControllers::RelayKernel::USE_NOTHING)
	{
		String key = CIS::Config::g_Interface.getPrefixKey();
		if (Msg.Left(key.GetLength()) == key)
		{
			if (_bool(AW_WORLD_DISABLE_CHAT) == TRUE)
			{
				CIS::Config::InterfaceKernel& kernel = CIS::Config::g_Interface;
				int style = AW::MakeConsoleStyle(
					kernel.getOutputBold(),
					kernel.getOutputItalic(), 
					true);

				String log;
				log.Format("@Command:\t%s", Msg.Mid(key.GetLength()));
				User.ConsoleMessagePrefixed("Command #", Msg.Mid(key.GetLength()), kernel.getOutputColour().getColour(), style);
			}
		}
		else
		{
			ChatControllers::g_Relay.handle_chat(User, Msg, Type);
		}
	}


	// response 
	CIS::CommandInterpreter::beginAvatar(User, Msg, false, Type);
	
	// does the user need kicking
	if (HasEject() && Security::g_FilterPolicy.enabled && Security::g_FilterPolicy.words.IsFilteredAnywhere(Msg))
	{
		User.Eject(Security::g_FilterPolicy.duration * 60,
			Security::g_FilterPolicy.message, "Chat included words prohibited by filter policy");
		return;
	}
	
	// inc count
	User.getFPD().IncrementMPS(static_cast<int>(time64(0) % 5));

	/* check the auto mute policy */
	if (Security::g_FloodPolicy.enabled)
	{
		/* tally the total count */
		int chat_count = 0;
		for (size_t i = 0; i < 5; i++)
			chat_count+= User.getFPD().getMPS(i);

		/* do we need to mute the user */
		if (Security::g_FloodPolicy.auto_mute_on != 0 && 
			chat_count > Security::g_FloodPolicy.auto_mute_on && 
			User.getMuted() == false)
		{
			User.getFPD().setMutedTime(time64(0));
			User.Mute();

			// notify user
			String logText;
			logText.Format("AUTO_MUTE has been applied to %s for %d seconds (flooding policy)",
				User.getName(), Security::g_FloodPolicy.auto_mute_len);
			WriteMsg(logText, 0x884444, OM_TIME | OT_MAIN);
		}

		/* is it greater than the maximum value */
		if (Security::g_FloodPolicy.max_rate != 0 && 
			HasEject() && 
			chat_count > Security::g_FloodPolicy.max_rate)
		{
			User.Eject(5 * 60, "Flooding the screen", 
				"Excess lines/second prohibited by flooding policy");
		}
	}
}

void EclipseBot::onProcessConsoleMessage(void)
{
	CStringA msgText = _string(AW_CONSOLE_MESSAGE);
	CStringA log	 = "";
	int tabFound	 = msgText.Find(":\t");
	COLORREF colour  = RGB(_int(AW_CONSOLE_RED), _int(AW_CONSOLE_GREEN), _int(AW_CONSOLE_BLUE));

	// history
	Recorder::Recording::getInstance().onConsoleMessage(msgText);

	/* vis style */
	int vis_style = (_bool(AW_CONSOLE_BOLD)    ? MF_FBOLD  : 0) |
					(_bool(AW_CONSOLE_ITALICS) ? MF_ITALIC : 0);

	//
	//  Handle when the message is split into tabs
	//
	if (tabFound < 18 && tabFound != -1) 
	{
		SplitString tokens(msgText, ":\t", 2);

		/* filter via prefix */
		if (!Comms::g_ConsoleFilter.IsFiltered(tokens[0]))
		{
			log.Format(">%s:\t%s%s", tokens[0], g_InterfaceConfig.getChatDisplay().getLogicalConsolePrefix(), tokens[1]);
			WriteMsg(log, colour, OT_MAIN | vis_style);
		}

	} 
	
	//
	// Standard Console
	//
	else 
	{
		/* display using the normal cmsg format */
		WriteMsg(msgText, colour, OT_MAIN | vis_style);
	}

	// scripting host
	ScriptingHost::getInst().doFunction("EventConsoleMessage", 
		&Umbra::VariantArray((const char*)msgText, (int)colour), 0, &bot);
}

void EclipseBot::onProcessBotgram(void)
{
	//
	// Copy Data from SDK
	//

	CStringA sentText = _string(AW_BOTGRAM_TEXT);
	CStringA sentName = _string(AW_BOTGRAM_FROM_NAME);
	int		 sentFrom = _int(AW_BOTGRAM_FROM);
	CStringA outMsg, logText;

	//
	// Botgram Scripting
	//

	ScriptingHost::getInst().doFunction("EventBotgram", 
		&Umbra::VariantArray(sentFrom, (const char*)sentText, (const char*)sentName), 0, &bot);

	//
	// Evo Formatted Message
	//

	if (sentText.Left(8) == "BGTP-1: ")
	{
		bot.getBotgramReceiver().onReceive(sentFrom, sentName, sentText);
	}

	else if (sentText.Left(5) == "<evo ") 
	{
		Helpers::XML::DataKeyset dsKeys;
		SplitString argLines(sentText, "\n", 2);
		
		/* parse xml sequencer */
		int result = Helpers::XML::ParseSingleLine(argLines[0], dsKeys);
		if (result == false) {
			return;				// silent discard of this data
		}

		if (dsKeys["msg"] == "startup" || dsKeys["msg"] == "checkpoint") {
			int		 msg_ver	= atol(dsKeys["ver"]);			// message version
			CStringA user_name	= dsKeys["name"];				// user login name
			CStringA client		= dsKeys["client"];				// client type (EVOLUTION_IDENTIFY)
			int		 client_ver = atol(dsKeys["cver"]);			// client version (EVOLUTION_VERSION)
			CStringA os_ver		= dsKeys["os"];					// OS build
			CStringA guid		= dsKeys["guid"];				// login GUID

			/* if we have an os split it */
			int os_major = 0, os_minor = 0, os_revision = 0, os_type = 0, os_proc = 0;
			if (os_ver.GetLength() > 0) {
				SplitString osi(os_ver, ".", 4);
				if (osi.size() == 4) {
					os_major	= atol(osi[0]);
					os_minor	= atol(osi[1]);
					os_revision	= atol(osi[2]);
					os_type		= atol(osi[3]);
				}
			}

			/* check on the message type */
			if (dsKeys["msg"] == "startup") {
				logText.Format("@ecom:\t[%s] %s (%d - %s) connect version %s (%s)",
					getTime(), sentName, sentFrom, user_name, client, OsVersionToText(os_major, os_minor, os_revision, os_type));
				WriteMsg(logText, 0x3f1c7c, OT_TECH | MF_FBOLD);
			} else { 
				logText.Format("@ecom:\t[%s] %s (%d - %s) HBT_6H version %s",
					getTime(), sentName, sentFrom, user_name, client);
				WriteMsg(logText, 0x3f1c7c, OT_TECH | MF_FBOLD);
			}
		}
	}

	////
	//// Show Public Botgrams
	////

	//if (g_InterfaceConfig.getLogging().getShowBotgrams())
	//{
	//	// if we are not on a valid stream
	//	if (g_InterfaceConfig.getLogging().getShowProtocolBotgramsOnly() && sentText.Left(8) != "<stream ")
	//		return;

	//	// log message
	//	String log;
	//	log.Format("Botgram from %s (#%d)", sentName,sentFrom);
	//	WriteMsg(log, 0x333355, OT_MAIN); 

	//	// display message
	//	SplitString lines(sentText, "\n", -1);
	//	for (size_t i = 0; i < lines.size(); i++)
	//	{
	//		
	//		log.Format(">\t%s", lines[i]);
	//		WriteMsg(log, 0x333355, OT_MAIN);
	//	}
	//}
}

void EclipseBot::onProcessUrlClick(Avatar& User, CStringA Url)
{
	// scripting host
	ScriptingHost::getInst().doFunction("EventUrlClick", 
		&Umbra::VariantArray(User, (const char*)Url), 0, &bot);
}

int EclipseBot::Botgram(int Citizen, const char* Message, bool Force)
{
	// assign attribute sets
	_int_set	(AW_BOTGRAM_TYPE, 0);
	_int_set	(AW_BOTGRAM_TO,   Citizen);
	_string_set (AW_BOTGRAM_TEXT, Message);
	return aw_botgram_send();
}

CStringA OsVersionToText(int _Major, int _Minor, int _Revision, int _Type) 
{
	if (_Major == 6 && _Minor == 0 )
	{
		if(_Type == VER_NT_WORKSTATION)
		{
			return "Windows Vista";
		}
		else 
		{
			return "Windows Server Longhorn";
		}
	}
	else if (_Major == 5 && _Minor == 2 ) 
	{
		return "Windows Server 2003";
	}
	else if ( _Major == 5 && _Minor == 1 )
	{
		return "Windows XP";
	}
	else if ( _Major == 5 && _Minor == 0 )
	{
		return "Windows 2000";
	}
	else if ( _Major <= 4 )
	{
		return "Windows NT";
	}
	else
	{
		return "???";
	}
}

int	EclipseBot::botgram_stream_send(int Citizen, const char* Reducer, const char* Type, const char* Text)
{
	// stream header
	//   <stream to="334303" reduce="[Mark]" type="email" length="1024" />
	// 

	// botgram back to the developer citizenship
	Helpers::XML::DataKeyset dsKeys;
	dsKeys.InsertKey("to",		Citizen);
	dsKeys.InsertKey("reducer",	Reducer);
	dsKeys.InsertKey("type",	Type);
	dsKeys.InsertKey("size",	(int)strlen(Text));
	dsKeys.InsertKey("self",	getName());

	// return stream
	String msg;
	msg.Format("%s\n\n%s", dsKeys.ToSingleXML("stream"), Text);
	msg = msg.Left(800);

	// botgram out
	return Botgram(Citizen, msg, true);
}