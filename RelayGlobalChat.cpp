#include "stdafx.h"
#include "RelayRelay.h"
#include "RelayGlobalChat.h"
#include "RelayGlobalChatAex.h"
#include "umbra/umbra_variantmap.h"
#include "SystemWordsFilter.h"
#include "RelayRelay.h"
#include "zones.h"

namespace ChatControllers
{
	// kernel class
	errno_t GlobalChatKernel::handle_chat(Avatar& User, String Text, int Method)
	{
		// turned off, ignore?
		if (bot._bool(AW_WORLD_DISABLE_CHAT) == FALSE)
			return 1;


		// does this zone require the use of public speaker to chat
		if (User.getZone()->getLogicalChatRequiresPs() == B_TRUE && Method != AW_CHAT_BROADCAST)
		{
			User.ConsoleWhisper("Only public speakers are allowed to chat in this zone", RGB_RED, AW::MakeConsoleStyle(true, false, true));
			return 1;
		}

		// is the user validated for this zone
		if (g_Relay.getGlobalChat().getUseGlobalChannels() && User.getZoneConfirmed() == false)
			return 1;

		// convert new lines to \n
		Text.Replace("\n", " ");

		// is the chat being filtered
		if (g_Relay.getFilterBadWords() == true && Security::g_FilterPolicy.words.IsSmartFiltered(Text))
		{
			User.ConsoleWhisper("Your message was blocked by filter policy", RGB_RED, AW::MakeConsoleStyle(true, false, true));
			return 1;
		}

		// reference the plugin information
		GlobalChatAexPlugin&	plugin = User.getGlobalChatPlugin();
		String					logText, tag_sp;
		String					user_name = User.getName();
		COLORREF				clr = plugin.getEffectiveColour().getColour();
		
		// check the colour, if white select default
		if ((clr & 0xFFFFFF) == 0xFFFFFF)
		{
			if (User.IsTourist())
				clr = getDefaultTouristColour().getColour();
			else if (User.IsCitizen()) 
				clr = getDefaultCitizenColour().getColour();
			else
				clr = getDefaultBotColour().getColour();
		}

		// bool state
		bool bold	= tsb_check(plugin.getEffectiveBold(), Method == AW_CHAT_BROADCAST);
		bool italic	= tsb_check(plugin.getEffectiveItalic(), false);
		int cmf = AW::MakeConsoleStyle(bold, italic, true);

		// format the name, check for if impersonation is armed and used
		if (getEnableImpersonation() == true)
			if (plugin.getImpersonation().GetLength() != 0)
				user_name = plugin.getImpersonation();

		// format the name
		Umbra::VariantMap vMap = bot.CreateVariantMap(&User);
		vMap["name"]	= (LPCSTR)user_name;
		vMap["tag"]		= (LPCSTR)plugin.getTag();
		vMap["tagsp"]	= plugin.getTag().GetLength() == 0 ? "" : " ";
		vMap["text"]	= (LPCSTR)Text;

		// parse the text
		String output	= vMap.parse_string((LPCSTR)getFormat()).c_str();
		String src_comm	= User.getZone()->getLogicalCommsChannel();

		// push out to everyone via broadcast
		if (getUseBroadcastMethod() == true)
			bot.consoleBroadcast(output, clr, cmf);
		else
		{
			for (size_t i = 0; i < avatars.size(); i++)
			{
				// within range
				bool in_range	= chat_range ? avatars[i].InRect(User, chat_range * 100) : true;
				bool filtered	= avatars[i].getMuteList().is_filtered(User);
				bool in_channel	= getUseGlobalChannels() ? (src_comm == avatars[i].getZone()->getLogicalCommsChannel()) : true;
				
				// must match
				if (in_range && in_channel && !filtered)
					avatars[i].ConsoleMessageEx(output, clr, cmf);
			}
		}
		return 0;
	}

	errno_t GlobalChatKernel::handle_avatar_add(Avatar& User)
	{
		return 0;
	}
	
	errno_t GlobalChatKernel::handle_avatar_delete(Avatar& User)
	{
		return 0;
	}


	String GlobalChatAexPlugin::getDebugString() const
	{
		String format = "";
		format.Format("Impersonate='%s' tag='%s' bold=%s italic=%s colour=%x changable=%s",
			getImpersonation(), getTag(), 
			tsb_to_text(getBold()), 
			tsb_to_text(getItalic()), 
			getColour().getColour(), 
			tsb_to_text(getAllowUserColourChanges()));
		return format;
	}

	//
	//
	//
	//
	//
	ColorTableEntry* GlobalUserStyles::getEntry(const char* Data) 
	{
		for (iterator ptr = begin(); ptr != end(); ptr++)
			if (ptr->getId().CompareNoCase(Data) == 0)
				return &(*ptr);
		return NULL;
	}

	ColorTableEntry* GlobalUserStyles::getAuto(int AutoID)
	{
		for (iterator ptr = begin(); ptr != end(); ptr++)
			if (ptr->getAutoId() == AutoID)
				return &(*ptr);
		return NULL;
	}

	void GlobalUserStyles::load(const char* FilePath)
	{
		Serializer ini;
		CStringA key;

		ini.Load(FilePath);

		// clear the list
		list.clear();

		// load each item
		int count = ini.getKeyInt("_header", "count");
		for (int i = 0; i < count; i++)
		{
			key.Format("cte%d", i);

			// add a new item
			ColorTableEntry e;
			e.Load(ini, key);

			// push back
			list.push_back(e);
		}
	}

	void GlobalUserStyles::save(const char* FilePath)
	{
		// write the items
		Serializer ini;
		int count = 0;
		String key;

		ini["_header"]["count"] = ToString(list.size());

		// save each item
		for (iterator ptr = begin(); ptr != end(); ptr++)
		{
			key.Format("cte%d", count++);
			ptr->Save(ini, key);
		}

		// save
		ini.Save(FilePath);
	}

	void GlobalUserStyles::updateAvatar(Avatar& User)
	{
		for (iterator ptr = begin(); ptr != end(); ptr++)
		{
			if (User.IsPattern(ptr->getId()) == true)
			{
				User.getGlobalChatPlugin().setSessionBold	(ptr->getBold());
				User.getGlobalChatPlugin().setSessionItalic	(ptr->getItalic());
				User.getGlobalChatPlugin().setSessionColor	(ptr->getColor());
			}
		}
	}
}