#pragma once
#include <list>
#include <vector>
#include "listener.h"

namespace AW
{
	class Bot;
}

namespace UniverseUserList
{
	class List;
	class User;

	class Visit
	{
	protected: // variables
		time_t	timestamp;
		String	world;

	public: // read access
		time_t	getTimestamp() const { return timestamp; }
		String	getWorld() const { return world; }

	public: // constructor
		Visit(const User*);
	};

	class Visits
	{
	public: // type defenition expansions
		typedef std::list<Visit>			ListType;
		typedef ListType::iterator			iterator;
		typedef ListType::reverse_iterator	reverse_iterator;
		typedef ListType::reference			reference;
		typedef ListType::size_type			size_type;

	public: // var
		ListType list;

	public: // read operations
		iterator begin()			{ return list.begin();		}
		iterator end()				{ return list.end();		}
		reverse_iterator rbegin()	{ return list.rbegin();		}
		reverse_iterator rend()		{ return list.rend();		}
		size_type size()			{ return list.size();		}
	};

	class User : public AW::AvatarData
	{
		friend class List;

	protected: // string representations
		String	email;
		String	world;
		int			state;
		Visits	visits;
		int			id;

	public: // time tracking
		time_t	enter_time;
		time_t	exit_time;

	public: // read string representations
		String	getEmail()			const { return email;		}
		String	getWorld()			const { return world;		}
		Visits& getVisits()			      { return visits;	}
		int			getUserState()	const	{ return state;		}
		int			getId()					const { return id;			}

	public: // sdk updates
		void	updateDataBySdk(AW::Bot& Client);

	public: // scripting
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);

	public: // checks on state
		bool	isOnline()		const { return state == AW_USERLIST_STATE_ONLINE;		}
		bool	isOffline()		const { return state == AW_USERLIST_STATE_OFFLINE;	}
		bool	isInvite()		const { return state == AW_USERLIST_STATE_INVITE;		}

	public: // user checking
		bool	isCitizen()		const { return m_name[0] != '"' && m_name[0] != '[';	}
		bool	isTourist()		const { return m_name[0] == '"';	}
		bool	isBot()				const { return m_name[0] == '[';	}
	};

	typedef std::vector<User*> SearchResults;

	class UserChangedMessage : public EventMessage
	{
	public: 
		User* user;

	public: // constructor
		UserChangedMessage(User* P)
		{
			user = P;
		}
	};

	class Kernel : public EventBroadcaster
	{
	public: // type defenition expansions
		typedef std::list<User>				ListType;
		typedef ListType::iterator		iterator;
		typedef ListType::size_type		size_type;

	public: // var
		ListType list;

	public: // read operations
		iterator begin()					{ return list.begin();	}
		iterator end()						{ return list.end();		}
		size_type size()					{ return list.size();		}

	public: // event messages
		enum
		{
			LM_ONLINE,
			LM_CHANGED,
			LM_OFFLINE,
			LM_CLEARED
		};

	public: // lookup routines
		User* getId(int Id);

	public: // handling routines
		void	onUserInfo(CommonBot* Bot);
		void	onDisconnect();

	public: // querying
		SearchResults getUsersInWorld(const char* World);
	};
}