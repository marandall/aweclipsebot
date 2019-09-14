#include "stdafx.h"
#include "relayprivatechannels.h"
#include "RelayRelay.h"
#include "autonumber.h"
#include "scripting_host.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ChatControllers
{
	namespace Channels
	{
		void MembershipBindings::clear()
		{
			while (members.size() > 0) {
				unlink(members[0]->channel);
			}
		}

		bool MembershipBindings::isIncluding(Channel* C)
		{
			for (size_t i = 0; i < members.size(); i++)
				if (members[i]->channel == C)
					return true;
			return false;
		}

		bool MembershipBindings::isIncluding(String C)
		{
			for (size_t i = 0; i < members.size(); i++)
				if (members[i]->channel->name.CompareNoCase(C) == 0)
					return true;
			return false;
		}

		void MembershipBindings::setPassive(bool Pasv)
		{
			_passive = Pasv;

			// scripting host
			ScriptingHost::getInst().doFunction("EventAvatarRelayPassiveChange", 
				&Umbra::VariantArray(*user, Pasv), 0, &bot);
		}

		void MembershipBindings::setDefault(Channel* Chan)
		{
			_default_channel = Chan->name;

			// scripting host
			ScriptingHost::getInst().doFunction("EventAvatarRelayDefaultChange", 
				&Umbra::VariantArray(*user, *Chan), 0, &bot);
		}

		void MembershipBindings::save(Serializer& Sz)
		{
			const char* group = "channels";
			Sz[group]["count"]	 = ToString((int)members.size());
			Sz[group]["default"] = getDefault();
			Sz[group]["passive"] = getPassive() ? "1" : "0";

			// go through each binding and save its name
			for (size_t i = 0; i < members.size(); i++)
			{
				CStringA key;
				key.Format("channel%d", (int)i);
				Sz[group][key] = members[i]->channel->name;
			}
		}

		void MembershipBindings::load(Serializer& Sz)
		{
			prep_joinlist.clear();
			const char* group = "channels";

			// the number of channels joined
			size_t joined	= (size_t)atoi(Sz[group]["count"]);
			_default_channel	= Sz[group]["default"];
			_passive					= Sz[group]["passive"] == "1"; 

			// load each one into the prejoin buffer
			for (size_t i = 0; i < joined; i++)
			{
				CStringA key;
				key.Format("channel%d", (int)i);

				// get the string
				CStringA chan = Sz[group][key];
				prep_joinlist.push_back( chan );
			}

			// reload default
			if (_default_channel == "")
				_default_channel = "#world";
		}

		void MembershipBindings::unlink(Channel* C)
		{
			// check for the item
			for (size_t i = 0; i < C->members.size(); i++)
			{
				// skip?
				if (C->members[i]->av != user)
					continue;
				
				// remove this user
				C->members.erase(C->members.begin() + i);
			}

			// bindings
			MembershipBindings& mb = user->getChannelBindings();
			for (size_t j = 0; j < mb.members.size(); j++)
			{
				// is this the relevant channel
				if (mb.members[j]->channel != C)
					continue;

				// erase it
				delete mb.members[j];
				mb.members.erase(mb.members.begin() + j);
				break;
			}
		}

		void MembershipBindings::quit(bool Announce)
		{
			// create a list of channels that would have been employed
			vector<Channel*> joinedlist;
			while (members.size())
			{
				joinedlist.push_back(members[0]->channel);
				members[0]->channel->leave(user, false);
			}

			// if notification is not required go no futher
			if (Announce == false)
				return;

			// go through each avatar
			for (size_t i = 0; i < avatars.size(); i++)
			{
				// the list of joined channels
				KnownList joinedText;

				// list
				if (&avatars[i] == user)
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
					temp.Format("%s has left the the %s channel%s.",
						user->getName(), joinedText.toString("and"), 
						joinedText.size() == 1 ? "" : "s");

					// tell this user
					AdvancedRelayKernel& trk = g_Relay.getAdvancedRelay();
					avatars[i].ConsoleWhisper(temp, trk.getChannelNotifyColor().getColour(), 
						trk.getChannelMessageStyle() | CM_SUPPRESSED); 
				}
			}
		}

		MembershipBindings::~MembershipBindings()
		{
			clear();
		}

		//
		// Scripting Handlers
		//

		bool Channel::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
		{
			return false;
		}

		bool Channel::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
			UMBRA_PROPERTY_R(name, this->name);
			UMBRA_PROPERTY_R(user_count, (int)members.size());
			UMBRA_PROPERTY_R(is_public, is_public);
			UMBRA_PROPERTY_R(password, password);;

			return false;
		}


		bool Channel::checkMember(Avatar* Av)
		{
			for (size_t i = 0; i < members.size(); i++)
				if (members[i]->av == Av)
					return true;
			return false;
		}

		Membership* Channel::join(Avatar* Av, bool Announce)
		{
			// link references
			AdvancedRelayKernel& trk = g_Relay.getAdvancedRelay();

			// does the user already exist?
			if (checkMember(Av) == true)
				return 0;

			// format the message
			if (Announce)
			{
				Umbra::VariantMap vm = bot.CreateVariantMap(Av);
				vm["channelname"] = (LPCSTR)name;

				// parse and write
				String out = vm.parse_string((const char*)trk.getChannelJoinMessage()).c_str();
				notify(out);
			}

			// add a new member element
			Membership* pMembership = new Membership(Av, this);
			members.push_back(pMembership);

			// Attach the avatar list
			Av->getChannelBindings().members.push_back(pMembership);

			// scripting host
			ScriptingHost::getInst().doFunction("EventChannelConnectionAdd", 
				&Umbra::VariantArray(*Av, *this), 0, &bot);

			return pMembership;
		}
		
		void Channel::leave(Avatar* Av, bool Announce)
		{
			// link references
			AdvancedRelayKernel& trk = g_Relay.getAdvancedRelay();

			// erase this link
			Av->getChannelBindings().unlink(this);

			// format the message
			if (Announce)
			{
				Umbra::VariantMap vm = bot.CreateVariantMap(Av);
				vm["channelname"] = (LPCSTR)name;

				// parse and write
				String out = vm.parse_string((const char*)trk.getChannelQuitMessage()).c_str();
				notify(out);
			}

			// scripting host
			ScriptingHost::getInst().doFunction("EventChannelConnectionDelete", 
				&Umbra::VariantArray(*Av, *this), 0, &bot);
		}

		void Channel::kick(Avatar* P, int Duration, const char* Comment)
		{
			enterBan(*P, Duration, Comment);
			leave(P, true);
		}

		void Channel::clear()
		{
			// erase each member
			while (members.size()) {
				leave(members[0]->av, false);
			}
		}

		void Channel::notify(CStringA Message)
		{
			// link references
			AdvancedRelayKernel& trk = g_Relay.getAdvancedRelay();

			// for each avatar
			for (size_t i = 0; i < members.size(); i++)
			{
				members[i]->av->ConsoleWhisper(Message, trk.getChannelNotifyColor().getColour(), trk.getChannelMessageStyle() | CM_SUPPRESSED); 
			}
		}

		//
		// Access Control
		//

		int	Channel::getPermission(Avatar& Av)
		{
			int max_perm = Permission::NONE;
			for (size_t i = 0; i < acl.size(); i++) {
				if (Av.IsPattern(acl[i].pattern) == false)
					continue;
				else if (acl[i].level > max_perm)
					max_perm = acl[i].level;
			}

			// return permission
			return max_perm;
		}

		void Channel::editPermission(const char* User, int Level)
		{
			// go through levels
			for (size_t i = 0; i < acl.size(); i++) {
				if (acl[i].pattern.CompareNoCase(User) == 0) {
					acl[i].level= Level;
					return;
				}
			}

			// new pattern
			Permission p;
			p.timestamp = time64(0);
			p.level		= Level;
			p.pattern	= User;
			acl.push_back(p);
		}

		// 
		// Loading and Saving
		//


		void Channel::save(Serializer& Sz, const char* Key)
		{
			// parent loader
			__super::Save(Sz, Key);

			// write the permissions
			Sz.Write(Key, "acl_size", (int)acl.size()); 

			// write all of the moderators
			for (size_t i = 0; i < acl.size(); i++)
			{
				CStringA k;	k.Format("acl_%d_", (int)i );
			
				// write the information
				Sz.Write(Key, k + "pattern",	acl[i].pattern);
				Sz.Write(Key, k + "level",		acl[i].level);
				Sz.Write(Key, k + "timestamp",	acl[i].timestamp);
			}
		}

		void Channel::load(Serializer& Sz, const char* Key)
		{
			// parent loader
			__super::Load(Sz, Key);

			// write the permissions
			size_t sizer = (size_t)Sz.Read(Key, "acl_size", 0); 

			// write all of the moderators
			for (size_t i = 0; i < sizer; i++)
			{
				CStringA k;	k.Format("acl_%d_", (int)i );
			
				// write the information
				Permission p;
				p.pattern	= Sz.Read(Key, k + "pattern", "~~~");
				p.level		= Sz.Read(Key, k + "level",	0);
				p.timestamp	= _atoi64(Sz.Read(Key, k + "timestamp", "0"));
				acl.push_back(p);
			}
		}

		bool Channel::isProtected()
		{
			return name == "#world" || name[0] == '/' || name == "#zone";
		}

		//
		//
		//
			
		int	Channel::enterBan(Avatar& Av, time_t Length, const char* Info)
		{
			// the user key
			CStringA ukey = Av.getName();
			if (Av.IsCitizen()) ukey.Format("^%d", Av.getCitizen());

			// create the ban
			Ban b;
			b.auto_id	= AutoNumber::getNext();
			b.key		= ukey;
			b.note		= Info;
			b.created	= time64(0);
			b.expires	= time64(0) + Length;

			// insert
			bans.push_back(b);
			return b.auto_id;
		}

		void Channel::flushBans()
		{
			for (int i = (int)bans.size(); i >= 0; i--)
				if (bans[i].expires <= time64(0))
					bans.erase(bans.begin() + i);
			return;
		}
		
		void Channel::eraseBan(int Id)
		{
			for (size_t i = 0; i < bans.size(); i++)
			{
				// automatic id only
				if (bans[i].auto_id != Id)
					continue;

				// kill it
				bans.erase(bans.begin() + i);
				return;
			}
		}

		bool Channel::isBanned(Avatar& Av)
		{
			for (size_t i = 0; i < bans.size(); i++)
				if (Av.IsPattern(bans[i].key) && bans[i].expires > time64(0))
					return true;
			return false;
		}

		//
		//
		//
		//
		//

		Channel* ChannelList::search(const char* Name)
		{
			for (iterator ptr = begin(); ptr != end(); ptr++)
				if (ptr->name.CompareNoCase(Name) == 0)
					return &*ptr;
			return 0;
		}

		Channel* ChannelList::getAuto(int ID)
		{
			for (iterator ptr = begin(); ptr != end(); ptr++)
				if (ptr->getAutoNumber() == ID)
					return &*ptr;
			return 0;
		}

		void ChannelList::deleteChannel(Channel* pChan)
		{
			for (iterator ptr = begin(); ptr != end(); ptr++)
			{
				if (&*ptr == pChan)
				{
					ptr->clear();
					list.erase(ptr);
					return;
				}
			}
		}

		void ChannelList::flushBans()
		{
			for (iterator ptr = begin(); ptr != end(); ptr++)
				ptr->flushBans();
		}

		//
		// Counters
		//
		int	ChannelList::getUserCount()
		{
			int count = 0;
			for (size_t i = 0; i < avatars.size(); i++) {
				if (avatars[i].getChannelBindings().size()) {
					count++;
				}
			}
			return count;
		}

		int	ChannelList::getOccupiedCount()
		{
			int count = 0;
			for (iterator ptr = begin(); ptr != end(); ptr++)
				if (ptr->members.size() != 0)
					count++;
			return count;
		}

		//
		// File IO
		//

		void ChannelList::push_back(Channel& C)
		{
			list.push_back(C);
		}
		
		void ChannelList::load(const char* FilePath)
		{
			// clear list
			clear();

			// open
			Serializer s;
			s.Load(FilePath);

			// save each channels
			size_t counter = (size_t)atoi(s.Read("header", "count", "0")); 

			// load each item
			for (size_t i = 0; i < counter; i++)
			{
				CStringA key; key.Format("chan_%d", (int)i);
				Channel c;

				// load the channel
				c.load(s, key);
				push_back(c);
			}

			// check if a world item exists
			if (search("#world") == 0)
			{
				Channel c;
				c.is_public	= true;
				c.name		= "#world";
				c.owner_citizen	= -1;
			
				// add to the list
				push_back(c);
			}
		}

		void ChannelList::save(const char* FilePath)
		{
			// file data
			Serializer s;
			size_t i = 0;

			// save each
			s.Write("header", "count", (int)list.size());
			for (iterator ptr = begin(); ptr != end(); ptr++)
			{
				CStringA key; key.Format("chan_%d", (int)i++);
				ptr->save(s, key);
			}

			// close file
			s.Save(FilePath);
		}

		bool ChannelList::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
		{
			//
			// get a script var
			//

			UMBRA_FUNCTION_BEGIN(getChannel) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Key,  Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					Channel* pChan = search(vaKey.toString().c_str());
					UMBRA_FUNCTION_RET( pChan ? *pChan : Umbra::Variant() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			//
			//

			UMBRA_FUNCTION_BEGIN(add) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Name,  Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					String name = vaName.toString().c_str();
					if (search(name) != 0)
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
					else
					{
						// create the new channel
						Channel c;
						c.name = name;

						// push it to the bcak
						list.push_back(c);
						UMBRA_FUNCTION_RET( list.back() );					
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// nothing
			return R.NOT_HANDLED;
		}
	}
}