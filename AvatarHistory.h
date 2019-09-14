#pragma once

#include "AWSDK_Avatar.h"
#include <list>

class Avatar;

namespace History
{
	class AvatarArticle :
		public AW::AvatarData
	{
	protected:
		__int64		 m_enter_time;				// when person entered
		AW::Location m_enter_position;			// where person entered
		__int64		 m_exit_time;				// when person left
		AW::Location m_exit_position;			// where person left
		int			 m_unique_id;				// the tracking id

	public: // accessor methods for the position
		__int64		 getEnterTime()		const	{ return m_enter_time;		}
		AW::Location getEnterPosition() const	{ return m_enter_position;	}
		__int64		 getExitTime()		const	{ return m_exit_time;		}
		AW::Location getExitPosition()	const	{ return m_exit_position;	}
		int			 getUniqueId()		const	{ return m_unique_id;		}

	public: // data copying mechanisms
		void		 begin			(const Avatar& Source);
		void		 update			(const Avatar& Source);
		void		 finalise		(const Avatar& Source);
	};

	class AvatarHistory
	{
	public: // type defenition for avatars
		typedef std::list<AvatarArticle> ListType;
		typedef ListType::iterator		 iterator;

	public: // query results
		typedef std::vector<AvatarArticle*> ResultSet;

	public: // list implementation
		ListType	list;
		iterator	begin() { return list.begin();	}
		iterator	end()	{ return list.end();	}
		size_t		size()	{ return list.size();	}
		void		clear()	{ list.clear();			}

	public: // update mechanisms
		AvatarArticle*	search		(int UniqueId);
		int			begin			(const Avatar& Source);
		void		update			(const Avatar& Source);
		void		finalise		(const Avatar& Source);

	public: // querying tools
		String		queryIpForDns	(const char* IP);
		ResultSet	queryIp			(int IP);
		ResultSet	queryCitizen	(int Citizen);
		ResultSet	queryPrivilege	(int Privilege);

	public: // singleton access
		static AvatarHistory& getInstance();
	};
}
