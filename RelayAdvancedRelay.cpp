#include "stdafx.h"
#include "RelayRelay.h"
#include "zones.h"
#include "SystemWordsFilter.h"
#include "RelayPrivateChannels.h"
#include "scripting_host.h"

namespace ChatControllers
{
	bool CONFIRM_YES	= true;
	bool CONFIRM_NO		= false;

	// send chat to a pure channel
	errno_t AdvancedRelayKernel::write_channel(Avatar& User, String Text, Channels::Channel& ChanOut, bool SendConfirm)
	{	
		// global references
		Channels::ChannelList& channels	= g_Relay.getChannels();
		Channels::Channel* pChannel = &ChanOut;

		// format the messages
		Umbra::VariantMap vm = bot.CreateVariantMap(&User);
		vm["channelname"] = (LPCSTR)pChannel->name;
		vm["message"]     = (LPCSTR)Text;

		// handle the channel processing
		Umbra::Variant v = ScriptingHost::getInst().doFunction("EventChannelMessage", 
			&Umbra::VariantArray(User, ChanOut, (const char*)Text), 0, &bot);
		if (v.IsBool() && v.toBool() == false)
			return 0;

		// state information
		bool chat_hidden	= bot._bool(AW_WORLD_DISABLE_CHAT) == 1;
		int	sent_count = 0;

		// the sections
		string first	= vm.parse_string((const char*)getChannelFormatPrefix());
		string second = vm.parse_string((const char*)getChannelFormatMessage());
		string alt		= vm.parse_string((const char*)getChannelFormatAltMessage());

		// color types
		COLORREF clr = getDefaultCitizenColour().getColour();
		if (User.IsTourist()) clr = getDefaultTouristColour().getColour();
		else if (User.IsBot()) clr = getDefaultBotColour().getColour();

		// avatar iteration over each avatar in the world collection
		for (size_t i = 0; i < pChannel->members.size(); i++)
		{
			// reference avatar
			Avatar& tav = *pChannel->members[i]->av;
			
			// chat is not hidden and within range of speaker
			bool ir = tav.InRect(User, getServerNeighbourRange());
			if (ir == true && chat_hidden == false)
				continue;

			// is global hidden but a chat range is specified
			if (chat_hidden && getChatRange() != 0)
			{
				if (tav.InRect(User, getChatRange()) == false)
				{
					continue;
				}
			}

			// check the zone mode
			if (getDefaultChannelMode() == WORLD_MODE_GLOBAL)
			{
				// nothing
			}
			else if (getDefaultChannelMode() == WORLD_MODE_ZONED)
			{
				// this uses a zone to determine if they are in the same channel
				if (User.getZone()->getLogicalCommsChannel() != tav.getZone()->getLogicalCommsChannel())
				{
					continue;
				}
			}

			// is muted
			if (tav.getMuteList().is_filtered(User))
				continue;

			// is this a channel other ot our default
			bool ec = tav.getChannelBindings().getDefault() == ChanOut.getName();

			tav.ConsoleMessagePrefixed(first.c_str(), 
				ec ? second.c_str() : alt.c_str(),				// main channel or alternate
				ec ? clr : getAltChannelColor().getColour(),	// the channel used 
				(User.HasPS() ? CM_BOLD : 0) | CM_SUPPRESSED);

			// sent count
			sent_count++;
		}

		// do we need to confirm
		if (SendConfirm == true)
		{
			String temp;
			temp.Format("Messsage sent to %s channel (%d users)", pChannel->getName(), sent_count);
			AdvancedRelayKernel& trk = g_Relay.getAdvancedRelay();

			// for each avatar
			User.ConsoleWhisper(temp, trk.getChannelNotifyColor().getColour(), trk.getChannelMessageStyle() | CM_SUPPRESSED); 
		}

		return 0;
	}

	errno_t AdvancedRelayKernel::handle_chat(Avatar& User, String Text, int Method)
	{
		// repeated accesses useful for the function
		Channels::MembershipBindings& binding = User.getChannelBindings();
		Channels::ChannelList& channels		  = g_Relay.getChannels();

		bool chat_hidden		= bot._bool(AW_WORLD_DISABLE_CHAT) == 1;
		bool target_single	= false;
		bool out_of_passive	= false;
		bool should_send		= binding.getPassive() == false;
		bool send_confirm		= false;
		int  sent_count			= 0;

		// escape bad chars that could be abused
		//
		Text.Replace("\r", " ");
		Text.Replace("\n", " ");

		// is the chat being filtered
		//
		if (g_Relay.getFilterBadWords() == true && Security::g_FilterPolicy.words.IsSmartFiltered(Text))
		{
			User.ConsoleWhisper("Your message was blocked by filter policy", RGB_RED, AW::MakeConsoleStyle(true, false, true));
			return 1;
		}

		// editable information
		//
		String target_channel = binding.getDefault();
		if (target_channel == "")
			target_channel = "#world";


		// does it begin with a slash?
		//

		// comparison
		SplitString em(Text, " ", 3);
		if (em[0] == "/1" || em[0] == "/2" || em[0] == "/3" || em[0] == "/4" || em[0] == "/5" ||
			em[0] == "/6" || em[0] == "/7" || em[0] == "/8" || em[0] == "/9") 
		{
			int c_id = atoi(em[0].Mid(1)) - 1;
			if (c_id < 0 || c_id >= binding.size())
			{
				User.ConsoleWhisper("Channel number is not valid", 0xFF, CM_SUPPRESSED);
				return 0;
			}
			else
			{
				target_channel	= binding[c_id].channel->name;
				Text						= Text.Mid(3);
				should_send			= true;
				send_confirm		= true;
			}
		}
		else if (Text[0] == '/')
		{
			if (em[0] == "/c")
			{
				// find the channel or quit
				Channels::Channel* pChannel = channels.search(em[1]);
				if (pChannel == 0)
				{
					User.ConsoleWhisper("There is no such channel or you are not a member of that channel", 0xFF, CM_SUPPRESSED);
					return 0;
				}

				// check membership
				if (pChannel->checkMember(&User) == false)
				{
					User.ConsoleWhisper("There is no such channel or you are not a member of that channel", 0xFF, CM_SUPPRESSED);
					return 0;
				}

				// reply to
				target_channel = pChannel->name;
				Text = em[2];
				should_send = true;
				send_confirm = true;
			}

			// changes a passive user into normal
			else if (Text.Left(3) == "/s ")
			{
				should_send = true;
				send_confirm = true;
				Text = Text.Mid(3);
			}
			else if (Text.Left(3) == "/w ")
			{
				SplitString el(Text.Mid(3), ",", 2);

				// trim everything
				String to	= el[0]; to.Trim();
				String msg	= el[1]; msg.Trim();

				// lookup the user
				Avatar* pUser = avatars.GetByKey(to, &User);
				if (pUser == 0)
				{
					User.ConsoleWhisper("There is no such user or the user has you muted", 0xFF, CM_SUPPRESSED);
					return -1;
				}

				// send the message
				if (pUser->getMuteList().is_filtered(User))
				{
					User.ConsoleWhisper("There is no such user or the user has you muted", 0xFF, CM_SUPPRESSED);
					return 0;
				}

				// message out
				pUser->ConsoleMessagePrefixed(User.getName(), String("(via relay): ") + msg, RGB(0, 0, 0xFF), CM_ITALIC);
				
				// notify
				String temp;
				temp.Format("Messsage sent to %s", pUser->getName());
				AdvancedRelayKernel& trk = g_Relay.getAdvancedRelay();

				// for each avatar
				User.ConsoleWhisper(temp, trk.getChannelNotifyColor().getColour(), trk.getChannelMessageStyle() | CM_SUPPRESSED); 
				return 0;
			}
		}

		// should not continue
		if (should_send == false)
			return -1;

		// if they are a member of a channel
		Channels::Channel* pChannel = ChatControllers::g_Relay.getChannels().search(target_channel);
		if (pChannel == 0) {
			User.ConsoleWhisper("Internal target channel is unknown", 0xFF, CM_SUPPRESSED);
			return -1;
		}

		// must be a member of this channel
		if (pChannel->checkMember(&User) == false)
			return -1;

		// format the messages
		write_channel(User, Text, *pChannel, send_confirm);
		return 0;
	}

	errno_t AdvancedRelayKernel::handle_avatar_add(Avatar& User)
	{
		// knownlist tells user every channel they enter
		KnownList entered;

		// must have relay use rights
		if (User.GetRight("relay/use") == false)
			return -1;

		// check if sessions should be restored
		if (getRestoreSessions() == false)
			return -1;

		using namespace ChatControllers;

		// channel info
		Channels::ChannelList& cl = g_Relay.getChannels();

		// join each of the chat chanels that are applicable
		std::vector<Channels::Channel*> joinedlist; 
		Channels::MembershipBindings& mb = User.getChannelBindings();

		// is default first joining enabled and a previous encounter count of zero
		if (getDefaultFirstJoin() == true && User.getPreviousEncounterCount() == 0 && User.IsBot() == false)
		{
			mb.prep_joinlist.push_back("#world");
		}

		for (size_t i = 0; i < mb.prep_joinlist.size(); i++)
		{
			// channel name
			CStringA cname = mb.prep_joinlist[i];

			// attempt to find this chanenl
			Channels::Channel* pChannel = g_Relay.getChannels().search(cname);
			if (pChannel == 0)
				continue;

			// banned!
			if (pChannel->isBanned(User) == true)
				continue;

			// permissions
			if (User.GetRight("relay/moderator") || 
				pChannel->getPermission(User) >= Channels::Permission::MEMBER ||
				pChannel->is_public)
			{
				// join this channel
				pChannel->join(&User, false);
				entered.push_back(cname);
				joinedlist.push_back(pChannel);
			}
		}

		// go through each avatar
		if (getAnnounceByDefault() == true)
		{
			for (size_t i = 0; i < avatars.size(); i++)
			{
				// the list of joined channels
				KnownList joinedText;

				// list
				if (&avatars[i] == &User)
					continue;

				// go through the list of channels they have entered
				for (size_t j = 0; j < joinedlist.size(); j++)
					if (avatars[i].getChannelBindings().isIncluding(joinedlist[j]))
						joinedText.push_back(joinedlist[j]->name);

				// temp it
				if (joinedText.size() != 0)
				{
					// sort it by alphabet
					joinedText.SortAZnc();

					// build a formatted message
					CStringA temp;
					temp.Format("%s has joined the %s channel%s.",
						User.getName(), joinedText.toString("and"), 
						joinedText.size() == 1 ? "" : "s");

					// tell this user
					AdvancedRelayKernel& trk = g_Relay.getAdvancedRelay();
					avatars[i].ConsoleWhisper(temp, trk.getChannelNotifyColor().getColour(), 
						trk.getChannelMessageStyle() | CM_SUPPRESSED); 
				}
			}
		}


		// there are channels available
		if (entered.size() != 0)
		{
			CStringA notify;
			notify.Format("Relay Subsystem is ONLINE - You have joined %d of %d channels: %s (default channel is %s / %s mode) - relay totals %d users.", 
				(int)entered.size(), 
				cl.getCount(),
				entered.toString("and"), 
				mb.getDefault(),
				mb.getPassive() ? "passive" : "standard",
				
				// statistics
				cl.getUserCount() );
			User.ConsoleWhisper(notify, 0x008888, CM_SUPPRESSED);
		}
		else
		{
			// count the number of people
			CStringA temp;
			temp.Format("Relay Subsystem is ONLINE - To join the global channel say ^jc - relay totals %d users in %d channels.",
				cl.getUserCount(), cl.getOccupiedCount());
			User.ConsoleWhisper(temp, 0x008888, CM_SUPPRESSED);
		}

		return 0;
	}

	errno_t AdvancedRelayKernel::handle_avatar_delete(Avatar& User)
	{
		// leave each channel
		User.getChannelBindings().quit(getAnnounceByDefault());
		return 0;
		
	}

	void AdvancedRelayKernel::tellMembersJoined(Avatar& User, ChatControllers::Channels::Channel& C)
	{
		// the join message
		Umbra::VariantMap vm = bot.CreateVariantMap(&User);
		
		// iterate each avatar
		for (size_t i = 0; i < C.members.size(); i++)
		{
			
		}
	}




}