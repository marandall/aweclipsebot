#pragma once
#include <list>
#include "umbra/umbra_remoteclass.h"
#include "listener.h"

namespace WorldList
{
	class World : public Umbra::RemoteClass
	{
	protected:
		String	name;
		int		users;
		int		rating;
		int		status;
		int		uid;
		bool	is_new;

	protected: // last change of users
		time_t	last_user_change;
		time_t	last_change;

	protected: // statictics
		int		user_peak;
		time_t	user_peak_time;
		time_t	last_occupied_time;

	public:
		World();
		virtual ~World();

	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);

	public: // access state checking
		bool	IsPublic				(void) const { return getStatus() == (int)AW_WORLDSTATUS_PUBLIC;	}
		bool	IsPrivate				(void) const { return getStatus() == (int)AW_WORLDSTATUS_PRIVATE;	}
		bool	IsUnknown				(void) const { return getStatus() == (int)AW_WORLDSTATUS_UNKNOWN;	}

	public: // read properties
		String	getName				(void) const { return name;					}
		String	getRatingString		(void) const { const char* ratings[] = { "G", "PG", "PG13", "R", "X" }; return rating < 5 ? ratings[rating] : "?"; }
		int			getUsers				(void) const { return users;				}
		int			getRating				(void) const { return rating;				}
		int			getStatus				(void) const { return status;				}
		int			getUID					(void) const { return uid;					}
		String	getIconKey			(void);

	public: // read properties of changes
		time_t	getLastUserChange		(void) const { return last_user_change;		}
		time_t	getLastChange			(void) const { return last_change;			} 		

	public: // read proprties of statistics
		int		getUserPeak			(void) const { return user_peak;			}
		time_t	getUserPeakTime		(void) const { return user_peak_time;		}
		time_t	getLastOccupiedTime	(void) const { return last_occupied_time;	}

	protected:
		friend class Kernel;
		void	from_sdk(AW::Bot& bot);
	};

	class WorldChangedMessage : public EventMessage
	{
	public: // event type
		static const int UPDATE_ADD			= 1;
		static const int UPDATE_CHANGED	= 2;
		static const int UPDATE_DELETED	= 3;

	public: // options
		World* world;
		int		 type;

	public: // construct only
		WorldChangedMessage(World* WPtr, int Msg = 0)
		{
			world = WPtr;
			type	= Msg;
		}
	};

	class Kernel : public Umbra::RemoteClass, public EventBroadcaster
	{
	public: // list extension
		typedef std::list<World>						ListType;
		typedef ListType::iterator					iterator;
		typedef ListType::const_iterator		const_iterator;
		typedef ListType::reverse_iterator	reverse_iterator;
		typedef ListType::reference					reference;
		typedef std::map<int, String>				IntStringMap;

	public: // event messages
		enum
		{
			LM_WORLD_ONLINE,
			LM_WORLD_CHANGED,
			LM_WORLD_OFFLINE,
			LM_WORLD_CLEARED
		};

	protected: // data storage
		ListType						worlds;
		EclipseBot*					m_pClient;
		bool								m_pending;	

	public: // read properties
		EclipseBot*	getClient			(void) { return m_pClient; }
		bool				getPending		(void) { return m_pending; }
		static			IntStringMap& StatusMap	(void);

	public: // constructor / destructor
		Kernel(EclipseBot& Client);
		~Kernel();

	public: // scripting
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);			
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

	public: // scripting iterator
		Umbra::IteratorSignalAgent _umbra_isa;
		typedef	Umbra::CodeIterator<iterator> ScriptIterator;

	public: // iterator access
		iterator				begin	(void) { return worlds.begin(); }
		const_iterator	begin	(void) const { return worlds.begin();	}
		iterator				end		(void) { return worlds.end();	}
		const_iterator	end		(void) const { return worlds.end();	}
		size_t					size	(void) { return worlds.size();	}

	public: // signal agents
		void						erase	(iterator Itr) { worlds.erase(Itr); _umbra_isa.signalInvalidated(); }
		void						clear	() { worlds.clear(); _umbra_isa.signalInvalidated(); }

	public: // updating mechanisms
		void	handle_world_info		(void);
		void	handle_callback			(int ReasonCode);
		void	handle_disconnection(void);
		
	protected: // special handling
		void	handle_world_offline(const char* Name);

	public: // actions
		int		query_list(void);
		void	clear_all	(void);

	public: // query mechanisms
		World*	lookup_world			(const char* WorldName);
		World*	lookup_by_id			(int ID);

	public: // counts etc
		int		getWorldCount(void);
		int		getUserCount(void);
		int		getEmptyWorldCount(void);
		int		getOccupiedWorldCount(void);
		int		getRatingCount(int Rating);
		int		getUserCountPerRating(int Rating);
		int		getUserCountPrivate(void);
		int		getUserCountPublic(void);
		int		getUserCountByStatus(int Status);
		int		getStatusCount(int Status);
	};
}