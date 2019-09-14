#include "stdafx.h"
#include "CIS_Interfaces.h"				// contains tracking classes
#include "CIS_ChatRelay.h"					// for basic commands
#include "colours.h"
#include "relayrelay.h"

#include "RelayGlobalChatAex.h"
#include "RelayMuteList.h"
#include "RelayRelay.h"
#include "RelayprivateChannels.h"
#include "stringtoolkit.h"

namespace CIS 
{
	namespace Exec 
	{
		int MuteProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("mute") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("list", "Your current mute list: {$muted.as_and} ({$muted.size} entries)");
					CIS_LANGUAGE("already_muted", "That user is already muted");
					CIS_LANGUAGE("default", "User has been added to the mute list");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			
			// relay mode must be enabled
			if (ChatControllers::g_Relay.getRelayMode() == ChatControllers::RelayKernel::USE_NOTHING)
				return Reason::InvalidCallState;

			// avatar keys
			Avatar& user = *pThisAvatar;

			// check flag
			if (getFlag("-list") == true)
			{
				KnownList known;
				String	  log;
				for (ChatControllers::MuteList::iterator ptr = user.getMuteList().begin();
					ptr != user.getMuteList().end(); 
					ptr++)
				{
					known.push_back(*ptr);
				}

				// log formats
				cisVM["muted"] = known;
				write(cisLang["list"], cisVM);
			}
			else
			{
				// commands and args
				String argKey	= getFlagText();
				String argMatch	= user.GetBestLock();

				// existing
				argKey.Trim();
				if (user.getMuteList().key_exists(argKey))
				{
					write(cisLang["already_muted"], cisVM);
				}
				else
				{
					user.getMuteList().push_back(argKey);
					write(cisLang["default"], cisVM);
				}
			}

			return Reason::Success;
		}

		int UnmuteProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("unmute") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("not_muted", "That user is not muted");
					CIS_LANGUAGE("default", "Removed user from your mute list");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// relay mode must be enabled
			if (ChatControllers::g_Relay.getRelayMode() == ChatControllers::RelayKernel::USE_NOTHING)
				return Reason::InvalidCallState;

			// commands and args
			String argKey	= getFlagText();

			// existing
			argKey.Trim();
			if (pThisAvatar->getMuteList().key_exists(argKey) == false)
			{
				write(cisLang["not_muted"], cisVM);
			}
			else
			{
				pThisAvatar->getMuteList().remove_key(argKey);
				write(cisLang["default"], cisVM);
			}

			return Reason::Success;
		}

		tsb_t text_to_tsb(String Text)
		{
			Text.MakeLower();
			if (Text == "true" || Text == "yes" || Text == "on")
				return B_TRUE;
			else if (Text == "false" || Text == "no" || Text == "off")
				return B_FALSE;
			else
				return B_EMPTY;
		}

		int RelayUoProcessor::process()
		{
			/* command format
				relay/uo <~impersonate> <~tag> <~color> <~colorselect> <~bold> <~italic>  */

			/* find the user id */
			String user = getParam("user", "");
			Avatar* pUser = avatars.GetByKey(getParam("user", ""));
			if (pUser == NULL)
			{
				write("Unable to identify user");
				return Reason::BadArgument;
			}

			// get the global mode plugin
			ChatControllers::GlobalChatAexPlugin& plugin = pUser->getGlobalChatPlugin();
			AW::KnownList kl;

			/* update from available keys */
			if (getParamExists("impersonate"))
			{
				plugin.setImpersonation(getParam("impersonate"));
				kl.push_back("impersonation");
			}

			/* name tag */
			if (getParamExists("tag"))
			{
				plugin.setTag(getParam("tag"));
				kl.push_back("tag");
			}

			/* display colour */
			if (getParamExists("color"))
			{				
				AW::Colour clr;
				clr = ColourTable::FromString(getParam("color"));
				plugin.setColour(clr);
				kl.push_back("color");
			}

			/* colour change allowed */
			if (getParamExists("colourselect"))
			{
				plugin.setAllowUserColourChanges(text_to_tsb(getParam("colorselect")));
				kl.push_back("color-select");
			}

			/* custom bold */
			if (getParamExists("bold"))
			{
				plugin.setBold(text_to_tsb(getParam("bold")));
				kl.push_back("bold");
			}

			/* custom italics */
			if (getParamExists("italic"))
			{
				plugin.setItalic(text_to_tsb(getParam("italic")));
				kl.push_back("italics");
			}

			/* respond */
			String logText;
			logText.Format("Updated gr(%s) %s, result %s", pUser->getName(), kl.toString("and"), plugin.getDebugString());
			write(logText);
			return Reason::Success;
		}

		int ColorProcessor::process() 
		{
			using namespace ChatControllers;

			/* if it is not checked */
			if (g_Relay.getRelayMode() != RelayKernel::USE_GLOBAL_CHAT)
				return Reason::AccessDenied;

			// must be an avatar
			if (isAvatar() == false) 
			{
				write("Sorry, this is avatar only");
				return Reason::InvalidCallState;
			}

			// default allowed state
			bool default_allow = false;
			Avatar& user = *getAvatar();
			if (user.IsTourist()) 
			{
				default_allow = g_Relay.getGlobalChat().getAllowTouristColourChanges();
			}
			else 
			{
				default_allow = g_Relay.getGlobalChat().getAllowCitizenColourChanges();
			}

			// check if has permission to alter it
			if ( ! tsb_check(user.getGlobalChatPlugin().getAllowUserColourChanges(), default_allow) )
			{
				write("Sorry, you are not allowed to change your chat colour");
				return Reason::BadArgument;
			}

			///* find the colour */
			COLORREF clr = ColourTable::FromString(getFlagText());
			
			/* check max colour limits */
			if ((GetRValue(clr) + GetBValue(clr) + GetGValue(clr)) / 3 > (0xFF / 100.0 * g_Relay.getGlobalChat().getMaxWhitePercentage()))
			{
				write("Sorry; the colour you have chosen is too bright. Try something a little darker");
				return Reason::Failed;
			}

			//Avatar& user = *getAvatar();
			user.getGlobalChatPlugin().colour.value = clr;
			user.ConsoleMessageEx("This is your new chat colour", clr, CM_ITALIC);
			return Reason::Success;
		}

		int JcProcessor::process() 
		{
			// connect to relay system, must be enabled
			ChatControllers::AdvancedRelayKernel& kernel = ChatControllers::g_Relay.getAdvancedRelay();
			if (kernel.getAllowUserChannelOptions() == false)
			{
				write("Sorry, user channel commands are disabled.");
				return Reason::Failed;
			}

			// channel connections
			ChatControllers::Channels::MembershipBindings& binding = this_avatar->getChannelBindings();

			// is this a world
			string chan_id = getFlagText();
			if (getFlagText() == "")
				chan_id = "#world";
			
			ChatControllers::Channels::Channel* pChannel = ChatControllers::g_Relay.getChannels().search(chan_id.c_str());
			if (pChannel == 0)
			{
				write("There is no such channel");
			}
			else
			{
				if (pChannel->checkMember(this_avatar))
				{
					write("You are already a member of that channel");
				}
				else if (pChannel->is_public == false && pChannel->getPermission(*this_avatar) == ChatControllers::Channels::Permission::NONE)
				{
					write("The specified channel is private and you do not have permissions to join it");
				}
				else if (pChannel->isBanned(*this_avatar))
				{
					write("You are not currently allowed in this channel");
				}
				else
				{
					temp.Format("Connecting to the %s channel. There are %d connected user(s) on this channel.",
							pChannel->name, (int)pChannel->members.size());
					write(temp);
					pChannel->join(this_avatar, kernel.getAnnounceByDefault());

					// only one?
					if (binding.size() == 1)
						binding.setDefault(binding[0].channel);
				}
			}

			return Reason::Success;
		}

		int QcProcessor::process() 
		{
			// connect to relay system, must be enabled
			ChatControllers::AdvancedRelayKernel& kernel = ChatControllers::g_Relay.getAdvancedRelay();
			if (kernel.getAllowUserChannelOptions() == false)
			{
				write("Sorry, user channel commands are disabled.");
				return Reason::Failed;
			}

			// channel connections
			ChatControllers::Channels::MembershipBindings& binding = this_avatar->getChannelBindings();
			
			// is this a world
			string chan_id = getFlagText();
			if (getFlagText() == "")
				chan_id = binding.getDefault();
			
			ChatControllers::Channels::Channel* pChannel = ChatControllers::g_Relay.getChannels().search(chan_id.c_str());
			if (pChannel == 0)
			{
				write("There is no such channel");
			}
			else
			{
				if (pChannel->checkMember(this_avatar) == false)
				{
					write("You are not a member of that channel");
				}
				else
				{
					write("Disconnecting from channel...");
					pChannel->leave(this_avatar, kernel.getAnnounceByDefault());
				}
			}

			return Reason::Success;
		}

		using namespace ChatControllers;

		//
		//
		//

		int ChanservProcessor::process()
		{
			// connect to relay system, must be enabled
			ChatControllers::AdvancedRelayKernel& kernel = ChatControllers::g_Relay.getAdvancedRelay();
			ChatControllers::Channels::MembershipBindings& binding = this_avatar->getChannelBindings();
			String t;

			if (getFlagText() == "")
			{
				// list each of the channels
				write("Your Channels: ");
				for (size_t i = 0; i < binding.size(); i++)
				{
					int iv = binding[i].channel->getPermission(*this_avatar);
					String mod;

					// type ref string
					if (iv == Channels::Permission::MODERATOR)
						mod = " +mod";
					else if (iv == Channels::Permission::ADMINISTRATOR)
						mod = " +admin";

					t.Format("\t/%d - %s (%d users)%s%s%s", 
						(i + 1), 
						(binding[i].channel->name), 
						(binding[i].channel->members.size()),
						(mod),
						(binding[i].channel->name == binding.getDefault()) ? " ** Default Channel **" : "",
						(binding[i].channel->name == binding.getDefault() && binding.getPassive()) ? " (Passive)" : ""
						);
					write(t);
				}
			}
			else
			{
				if (getFlagText() == "passive")
				{
					// check if they are allowed to change
					if (kernel.getAllowUserChannelOptions() == false)
					{
						write("Sorry, user channel commands are disabled.");
						return Reason::Failed;
					}

					if (binding.getPassive())
					{
						write("You are already in passive mode");
					}
					else
					{
						binding.setPassive(true);
						write("You have entered passive mode. To chat in passive use /s [message].");
					}
				}
				else if (getFlagText() == "normal")
				{
					// check if they are allowed to change
					if (kernel.getAllowUserChannelOptions() == false)
					{
						write("Sorry, user channel commands are disabled.");
						return Reason::Failed;
					}

					if (binding.getPassive() == false)
					{
						write("You are already in normal mode");
					}
					else
					{
						binding.setPassive(false);
						write("You have entered normal mode.");
					}
				}
			}

			return Reason::Success;
		}

		//
		//
		//

		int PassiveProcessor::process()
		{
			// connect to relay system, must be enabled
			ChatControllers::AdvancedRelayKernel& kernel = ChatControllers::g_Relay.getAdvancedRelay();
			if (kernel.getAllowUserChannelOptions() == false)
			{
				write("Sorry, user channel commands are disabled.");
				return Reason::Failed;
			}


			// channel connections
			ChatControllers::Channels::MembershipBindings& binding = this_avatar->getChannelBindings();

			// type
			if (getFlagText() == "")
			{
				write(binding.getPassive() ? "You are in passive mode" : "You are in standard mode");
			}
			else if (getFlagText() == "on")
			{
				if (binding.getPassive() == true)
				{
					write("You are already in passive mode");
				}
				else
				{
					binding.setPassive(true);
					write("You have entered passive mode.");
				}
			}
			else if (getFlagText() == "off")
			{
				if (binding.getPassive() == false)
				{
					write("You are already in normal mode");
				}
				else
				{
					binding.setPassive(false);
					write("You have entered normal mode.");
				}
			}
			else
			{
				write("Invalid command, expecting 'on' or 'off'.");
			}

			// ok
			return Reason::Success;
		}

		//
		//
		//
		int ChanservCreateProcessor::process()
		{
			// connect to relay system, must be enabled
			ChatControllers::AdvancedRelayKernel& kernel = ChatControllers::g_Relay.getAdvancedRelay();
			if (kernel.getAllowUserChannelOptions() == false)
			{
				write("Sorry, user channel commands are disabled.");
				return Reason::Failed;
			}

			// chanserv/create -id=awteen -private=0/1 -color=#440022
			CStringA id		= getParam("id");

			// must exist as a valid name
			if (id[0] != '#' ||
				StringToolkit::LengthInRange(id, 2, 12) == false ||
				StringToolkit::OnlyContainsCharacters(((LPCSTR)id) + 1, StringToolkit::AlphaChars) == false)
			{
				write("Name contains invalid characters or is too long");
				return Reason::BadArgument;
			}

			// check if the channel already exists
			if (g_Relay.getChannels().search(id))
			{
				write("That channel name already exists");
				return Reason::BadArgument;
			}

			// must begin with #
			if (id[0] != '#')
			{
				write("Channel names must begin with #");
				return Reason::BadArgument;
			}

			// create new item
			Channels::Channel chan;
			chan.name						= id;
			chan.owner_citizen	= getAvatar() ? this_avatar->getCitizen() : -1;
			chan.is_public			= atoi(getParam("public", true ? "1" : "0")) == 1;

			// add the channel
			g_Relay.getChannels().push_back(chan);
			Channels::Channel& c = g_Relay.getChannels().back();
			
			// is this an owner
			if (isAvatar() == true)
			{
				c.join(this_avatar, true);
				c.editPermission(CString("^") + ToString(this_avatar->getCitizen()), Channels::Permission::ADMINISTRATOR);
			}
			else
			{
				c.editPermission(CString("*") + ToString(bot._int(AW_LOGIN_OWNER)), Channels::Permission::ADMINISTRATOR);
			}

			// created
			write("The channel has been created and you have been granted administrative access");
			return Reason::Success;
		}

		int ChanservDeleteProcessor::process()
		{
			// connect to relay system, must be enabled
			ChatControllers::AdvancedRelayKernel& kernel = ChatControllers::g_Relay.getAdvancedRelay();
			if (kernel.getAllowUserChannelOptions() == false)
			{
				write("Sorry, user channel commands are disabled.");
				return Reason::Failed;
			}

			// lookup channel
			Channels::Channel* pChannel = g_Relay.getChannels().search(getFlagText());
			if (pChannel == 0)
			{
				write("Invalid channel or channel not found");
				return Reason::BadArgument;
			}

			// get membership rating
			int perm_level = Channels::Permission::ADMINISTRATOR;
			if ( isAvatar() ) perm_level = pChannel->getPermission(*this_avatar);

			// is member allowed to delete this channel
			if (perm_level != Channels::Permission::ADMINISTRATOR && !getRight("relay/moderator"))
			{
				write("Only channel administrators or relay moderators may delete this channel");
				return Reason::Success;
			}

			// channel is protected
			if ( pChannel->isProtected() )
			{
				write("This channel cannot be deleted as it is marked as protected");
				return Reason::AccessDenied;
			}

			// write
			write("Removing all users from channel and deleting...");

			// kick everyone from the channel
			pChannel->notify(String("This channel has been deleted by ") + getName());
			pChannel->clear();

			// delete the channel
			g_Relay.getChannels().deleteChannel(pChannel);
			return Reason::Success;
		}

		int ChanservUsersProcessor::process()
		{
			// lookup channel
			CStringA cname = getFlagText();
			if (cname == "" && isAvatar()) 
				cname = this_avatar->getChannelBindings().getDefault();

			// check default name
			String nm = getFlagText();
			if (nm.GetLength() == 0)
				nm = this_avatar->getChannelBindings().getDefault();

			Channels::Channel* pChannel = g_Relay.getChannels().search(nm);
			if (pChannel == 0)
			{
				write("Invalid channel or channel not found");
				return Reason::BadArgument;
			}

			// is member or otherwise admin
			if (!pChannel->checkMember(this_avatar) && !getRight("relay/moderator"))
			{
				temp.Format("You must be a member of the %s channel or have relay moderation "
							"rights to view the users of this channel", pChannel->name);
				write(temp);
				return Reason::AccessDenied;
			}

			// list of the users
			KnownList listof;
			for (size_t i = 0; i < pChannel->members.size(); i++)
				listof.push_back(pChannel->members[i]->av->getName());
		
			// is there a list available
			if (listof.size() == 0)
			{
				write("There are no users in that channel");
			}
			else
			{
				temp.Format("The %s channel contains %d user(s): %s", 
					(LPCSTR)pChannel->name, (int)pChannel->members.size(),
					listof.toString("and"));
				write(temp);
			}

			// any
			return Reason::Success;
		}

		int ChanservListProcessor::process()
		{
			// list of channels
			KnownList chans;

			// show each of the channels
			for (Channels::ChannelList::iterator ctr = g_Relay.getChannels().begin();
				ctr != g_Relay.getChannels().end();
				ctr++)
			{
				temp.Format("%s (%d%s)", ctr->name, (int)ctr->members.size(), ctr->is_public ? "" : " - private");
				chans.push_back(temp);
			}

			// list each of the channels
			temp.Format("There are %d channel(s): %s", (int)chans.size(), chans.toString("and"));
			write(temp);
			return Reason::Success;
		}


		int ChanservDefaultProcessor::process()
		{
			// connect to relay system, must be enabled
			ChatControllers::AdvancedRelayKernel& kernel = ChatControllers::g_Relay.getAdvancedRelay();
			if (kernel.getAllowUserChannelOptions() == false)
			{
				write("Sorry, user channel commands are disabled.");
				return Reason::Failed;
			}

			// channel connections
			ChatControllers::Channels::MembershipBindings& binding = this_avatar->getChannelBindings();
			String cname = getFlagText();

			// empty, just give name
			if (cname == "")
			{
				temp.Format("Your current default channel is %s %s",
					binding.getDefault(), binding.getPassive()  ? "passive mode" : "normal mode");
				write(temp);
			}
			else
			{
				Channels::Channel* pChannel = g_Relay.getChannels().search(cname);
				if (pChannel == 0)
				{
					write("Cannot change your default channel; it does not exist");
				}

				// membership check
				else if (pChannel->checkMember(this_avatar) == false)
				{
					write("You must be a member of a channel before you can set it as your default");
				}

				// set in binding
				else
				{
					this_avatar->getChannelBindings().setDefault(pChannel);
					write("your default channel has been set to " + cname);
				}
			}

			return Reason::Success;
		}

		int ChanservKickProcessor::process()
		{
			// split the parameters
			SplitString args(getFlagText(), ",", 3);

			// params
			CStringA user = args[0]; user.Trim();
			CStringA chan = args[1]; chan.Trim();
			CStringA len  = args[2]; len. Trim();

			// conversion
			Avatar* pUser = avatars.GetByKey(user);
			Channels::Channel* pChan = g_Relay.getChannels().search(chan);
			int length = atoi(len);
		
			// error in lookup
			if (pUser == 0)
			{
				temp.Format("The user '%s' could not be found", user);
				write(temp);
				return Reason::BadArgument;
			}

			// error in lookup
			if (pChan == 0)
			{
				temp.Format("The channel '%s' could not be found", chan);
				write(temp);
				return Reason::BadArgument;
			}

			// length must be between 0 and 3600
			if (length < 0 || length > 3600)
			{
				temp.Format("Your kick length of %d is invalid, it must be between 0 and 3600 minutes", length);
				write(temp);
				return Reason::BadArgument;
			}

			// rights in the channel
			int perms = Channels::Permission::ADMINISTRATOR;
			if (isAvatar()) perms = pChan->getPermission(*this_avatar);

			// must be mod or have relevant rights
			if (perms < Channels::Permission::MODERATOR && !getRight("relay/moderator"))
			{
				write("You do not have sufficent rights to kick users from this channel");
				return Reason::AccessDenied;
			}

			// cannot kick administrators or mods
			if (pChan->getPermission(*pUser) == Channels::Permission::ADMINISTRATOR || pUser->GetRight("relay/moderator"))
			{
				write("You cannot kick channel administrators or global moderators");
				return Reason::AccessDenied;
			}
			
			// notice
			temp.Format("Kicking %s from the %s channel for %d minute(s)...", pUser->getName(), pChan->name, length);
			write(temp);

			// the note
			temp.Format("by %s", isAvatar() ? this_avatar->getName() : "sys()");

			// remove a user from this channel
			pChan->kick(pUser, length * 60, temp);
			return Reason::Success;
		}
		

		// allows a user to change their default channel and passive status
		//
		int ChanservOptsProcessor::process()
		{
			// connect to relay system, must be enabled
			ChatControllers::AdvancedRelayKernel& kernel = ChatControllers::g_Relay.getAdvancedRelay();
			if (kernel.getAllowUserChannelOptions() == false)
			{
				write("Sorry, user channel commands are disabled.");
				return Reason::Failed;
			}

			// changes
			KnownList changes;

			// default channel
			if (getParamExists("default"))
			{
				// lookup channel
				String cname = getParam("default");
				Channels::Channel* pChannel = g_Relay.getChannels().search(cname);
				if (pChannel == 0)
				{
					write("Cannot change your default channel; it does not exist");
				}

				// membership check
				else if (pChannel->checkMember(this_avatar) == false)
				{
					write("You must be a member of a channel before you can set it as your default");
				}

				// set in binding
				else
				{
					this_avatar->getChannelBindings().setDefault(pChannel);
					changes.push_back(String("your default channel has been set to ") + cname);
				}
			}

			// passive
			if (getParamExists("passive"))
			{
				bool pasv = getParam("passive", 0) == 1;
				this_avatar->getChannelBindings().setPassive(pasv);

				// style
				if (pasv) changes.push_back("passive mode has been activated");
				else changes.push_back("passive mode has been disabled");
			}

			// write any changes
			if (changes.size() != 0)
			{
				temp.Format("Settings updated: %s", changes.toString("and"));
				write(temp);
			}

			// reason
			return Reason::Success;
		}
	}
}