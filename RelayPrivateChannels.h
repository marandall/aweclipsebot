#pragma once
#include <list>
#include <map>
#include "relaychannel_ag.h"
#include "uptr.h"

class Serializer;

namespace ChatControllers
{
	namespace Channels
	{
		class Channel;

		class Membership : public u_ptrbase
		{
		public: // avatar connection
			Avatar*		av;				// relevant avatar
			Channel*	channel;		// which channel i sthis

		public: // constructor
			Membership(Avatar* Av, Channel* C)
			{
				av = Av;
				channel = C;
			}
		};

		//
		// MembershipTable
		// -------------------------------------------
		// Maintains associative array linking users to the 
		//

		class MembershipTable
		{
		public: // a link of members and tables
			typedef std::list<Membership> ListType;
			typedef ListType::iterator	  ListIterator;

		public: // associate list
			ListType	links;

		public: // statics
			static MembershipTable& getInstance() { static MembershipTable s; return s; }
		};

		//
		// AvatarLink
		// --------------------------------------------
		// Managed class for 
		//

		class AvatarLink
		{
		public: // the membership link
			Membership*	membership;
		};

		class MembershipBindings
		{
			friend class Channel;
			friend class Avatar;

		public: // groups exists
			bool	isIncluding(Channel*);
			bool	isIncluding(String);

		protected: // the linking to the server
			bool		_passive;
			String	_default_channel;

		public: // read styles
			bool		getPassive() { return _passive; }
			String	getDefault() { return _default_channel; }

		public: // styles
			void		setPassive(bool Pasv);
			void		setDefault(Channel* Chan);

		public: // file io
			void	save(Serializer& Sz);
			void	load(Serializer& Sz);

		public: // the list of channels to join after channel join
			std::vector<CStringA> prep_joinlist;

		public: // memory management
			void	clear();
			void	unlink(Channel*);
			void	quit(bool Announce = true);

		public: // constructor stuff
			Avatar*						user;

		protected:
			std::vector<Membership*>	members;

		public:
			size_t size() { return members.size(); }
			Membership& operator[](size_t Index) { return *members[Index]; }

			MembershipBindings(Avatar* P)
			{
				_passive					= false;
				_default_channel	= "#world";
				user							= P;
			}

			~MembershipBindings();
		};

		class Permission
		{
		public: // styles
			enum 
			{
				NONE,
				MEMBER,
				MODERATOR,
				ADMINISTRATOR
			};

		public: // access
			String	pattern;
			int		level;
			time_t	timestamp;
		};

		class Ban
		{
		public:
			int			auto_id;
			CStringA	key;
			CStringA	note;
			time_t		created;
			time_t		expires;
		};

		class Channel : public AutoTemplates::ChannelData
		{
		public: // users within this channel
			typedef std::vector<Membership*> MemberList;
			typedef MemberList::iterator member_iterator;

		public: // banning
			typedef std::vector<Ban> BanList;
			typedef BanList::iterator ban_iterator;

		bool Channel::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		bool Channel::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);

		public: // banning information
			BanList		bans;
			int			enterBan(Avatar& Av, time_t Length, const char* Info);
			void		flushBans();
			void		eraseBan(int Id);
			bool		isBanned(Avatar& Av);
			CStringA	getName() const { return name; }

		public: // static 
			static const bool ACCESS_PUBLIC		= true;
			static const bool ACCESS_PRIVATE	= false;

		public: // blindly detatch this member
			void blindDetatch(Membership* MemberPtr)
			{
				for (size_t i = 0; i < members.size(); i++)
				{
					if (members[i] == MemberPtr)
					{
						members.erase(members.begin() + i);
						return;
					}
				}
			}

		public: // protected names
			bool		isProtected();

		public: // memberlists
			MemberList	members;			// list of assigned members

		public: // access control lists
			typedef std::vector<Permission> PermissionList;
			PermissionList acl;	// the list of permissions
			int			getPermission(Avatar& );
			void		editPermission(const char* User, int Level);

		public: // serialize
			void		save(Serializer& Sz, const char* Key);
			void		load(Serializer& Sz, const char* Key);
	
		public: // iterator access
			member_iterator	begin()	{ return members.begin();	}
			member_iterator	end()	{ return members.end();		}

		public: // methods
			bool		checkMember(Avatar*);
			Membership*	join(Avatar*, bool Announce = false);
			void		leave(Avatar*, bool Announce = false);
			void		clear();
			void		kick(Avatar* P, int Duration, const char* Comment);

		public: // notify
			void		notify(CStringA Message);
			~Channel()	{ clear(); }
		};

		class ChannelList : public Umbra::RemoteClass
		{
		protected: // umbra interfaces
			bool		doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

		public: // type defenitions
			typedef		std::list<Channel>				MapType;
		
		public: // iterator defenitions
			typedef		MapType::iterator				iterator;
			typedef		MapType::reverse_iterator		reverse_iterator;
			typedef		MapType::const_reverse_iterator	const_reverse_iterator;
			typedef		MapType::const_iterator			const_iterator;
		
		public: // size iterators
			typedef		MapType::size_type				size_type;
		
		protected: // main variable
			MapType		list;
		
		public: // functions
			iterator	begin()			{ return list.begin();		}
			iterator	end()			{ return list.end();		}
			Channel&	back()			{ return list.back();		}
			size_t		size() const	{ return list.size();		}
			void		clear()			{ return list.clear();		}
			void		flushBans();
			

		public: // helper functions
			Channel*	search(const char* Name);
			Channel*	getAuto(int ID);
			void		deleteChannel(Channel* pChan);
			void		push_back(Channel& C);

		public: // counting
			int			getUserCount();
			int			getOccupiedCount();
			int			getCount() { return (int)list.size(); }
		
		public: // file load / save
			void		load(const char* FilePath);
			void		save(const char* FilePath);
		};

		//
		// AvatarLinkTable
		// --------------------------------------------
		// Maintains the 
		//

		class AvatarLinkTable
		{
		public: // links to membership
			std::vector<Membership*> links;

		public: // constructors
			~AvatarLinkTable()
			{
				MembershipTable& mt = MembershipTable::getInstance();
				while (mt.links.size())
				{
					Membership* pMember = &mt.links.front();

					// erase the item and detatch it after
					mt.links.erase(mt.links.begin());
					pMember->channel->blindDetatch(pMember);
				}
			}
		};
	};
}