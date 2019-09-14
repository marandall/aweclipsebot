#pragma once
#include "listener.h"
#include "autonumber.h"
#include <list>

namespace AW
{
	class Bot;
}

namespace Ejections
{
	class Ejection : public AutoNumberIdentifier, 
									 public Umbra::RemoteClass
	{
	public:
		int			type;
		int			address;
		int			created;
		int			expires;
		String	comment;
		bool		was_detected;

	public: // functions
		String	toString();
		String	getTypeName();
		String	getCommentName();
		String	getCommentEjector();

	public: // copy fields
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};

	// MessageEjectionFound
	//
	class MessageEjectionFound : public EventMessage
	{
	public: 
		Ejection* eject;
		MessageEjectionFound(Ejection* Ej) { eject = Ej; }
	};

	class MessageEjectionChanged : public EventMessage
	{
	public: 
		Ejection* eject;
		MessageEjectionChanged(Ejection* Ej) { eject = Ej; }
	};

	class MessageEjectionExpired : public EventMessage
	{
	public: 
		Ejection* eject;
		MessageEjectionExpired(Ejection* Ej) { eject = Ej; }
	};

	class EjectionManager : public EventBroadcaster, public Listener
	{
	public: // events
		static const int LE_QUERYING					= 1;
		static const int LE_QUERY_COMPLETE		= 2;
		static const int LE_EJECTION_FOUND		= 3;
		static const int LE_EJECTION_EXPIRED	= 4;
		static const int LE_EJECTION_UPDATED	= 5;
		static const int LE_CLEARED						= 6;
		static const int LE_STATUS						= 8;

	protected:
		std::list<Ejection> _ejections;
		bool				_complete;
		int					_error;
		bool				_querying;
		__time64_t	_last_query;
		__time64_t  _last_complete;
		AW::Bot*		_pBot;
		int					_first;
		
	public: // iteration
		typedef std::list<Ejection>::iterator iterator;
		iterator begin() { return _ejections.begin(); }
		iterator end()   { return _ejections.end();   }
		size_t   size()  { return _ejections.size();  }

	public: // configure access
		void		setClient(AW::Bot* pBot){ _pBot = pBot; }

	public: // read access
		bool				IsComplete()		const { return _complete;		}
		int					getError()			const { return _error;			}
		bool				IsQuerying()		const { return _querying;		}
		__time64_t	getLastQuery()		const { return _last_query;	}
		__time64_t	getLastComplete()	const { return _last_complete; }

	public:
		void				HandleCallbackWorldEjection(int RC);
		int					QueryNext();
		int					beginQuery(bool First = false);
		int					cleanQuery();
		Ejection*		getEjection(int Ip, int IpType, int Start);
		String			getStatus();

	public: // constructor
		EjectionManager(AW::Bot* pBot);

	public: // listener connection
		void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* Msg);
	};
};
