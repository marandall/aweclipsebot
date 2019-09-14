#pragma once
#include "awsdk_bot.h"
#include "avatars.h"
#include "movers.h"

// forward references
namespace Drones	{ class RelayConfig;	}
namespace Movers	{ class Tracker;		}
namespace Movement	{ class Controller;		}
namespace Movers	{ class Mover;			}

class Avatars;
class Avatar;

	class CommonBot : public AW::Bot
	{
	public: // events that this bot can produce
		static const int LE_CONSTRUCT			= 0;
		static const int LE_AVATAR_ADD			= 1;
		static const int LE_AVATAR_DELETE		= 2;
		static const int LE_WORLD_CONNECTION	= 3;
		static const int LE_UNIVERSE_CONNECTION	= 4;

	public: // interface types
		virtual CStringA getBaseClass() { return "BotBase"; }

	public: // avatars list
		Avatars* p_avatars;
		Avatars& getAvatars() { return *p_avatars; }
		Avatar&  getAvatar(size_t Index);

	public: // avatar access
		typedef Avatars::iterator	av_iterator;
		av_iterator	av_begin()  { return getAvatars().begin();	}
		av_iterator av_end()	{ return getAvatars().end();	}

	public: // movers list
		Movers::Tracker* p_movers;
		Movers::Tracker& getMovers() { return *p_movers; }

	public: // mover access
		typedef Movers::Tracker::Iterator mv_iterator;
		mv_iterator mv_begin()  { return getMovers().begin();	}
		mv_iterator mv_end()	{ return getMovers().end();		}

	public: // movement kernel
		Movement::Controller* m_pMovementKernel;
		Movement::Controller& getMovementKernel() { return *m_pMovementKernel; }

	public: // constants for querying
		static const int QUERY_NONE			= 0;
		static const int QUERY_5x5			= 1;			// queries a 5x5 grid (standard)
		static const int QUERY_LARGE_AREA	= 2;			// queries a large area
		static const int QUERY_CELLULAR		= 3;			// queries single cells
		static const int QUERY_WORLD		= 4;			// reads the whole world into memory
		static const int QUERY_BACKUP		= 5;			// writes world straight to a given file

	public: // relating to identification
		String	getName							(void) const;
		String	getBotName						(void) const;
		String	getPrivilegeName				(void) const;

	public: // login process
		int		destroy							(void);
		int		quickstart						(const char* Host, int Port, const char* Universe, 
													int Citizen, const char* Privilege, const char* Name, 
													const char* World, const char* Coordinates, bool Global, bool Invisible);
		int		enterWorld						(const char* World, bool Global);
		int		teleport						(const char* World, const char* Coordinates, bool Global, bool Invisible);

	public: // extended command methods
		int		whisperBroadcast				(const char* MessageText);
		int		whisperToRegisteredTargets		(const char* MessageText);
		int		postEjectMessage				(const char* MessageText);
		int		consoleBroadcast				(const char* MessageText, COLORREF color, int Style);
		int		consoleBroadcastPrefixed		(String Prefix, String MsgText, COLORREF Colour, int Style);
		int		broadcastUrl					(String UrlText, String Frame, String PostData = "", bool Target3d = false);
		int		say								(const char* Message);

	public: // logging information
		void	WriteMsg(char const * Message, long Colour, int Style);
	
	protected: // handling of the events
		void	onEventAvatarAdd				(void);
		void	onEventAvatarChange				(void);
		void	onEventAvatarDelete				(void);
		void	onEventAvatarClick				(void);	
		void	onCallbackAddress				(int ReasonCode);
	
	public: // updating of avatars	
		void	onEventPrivilegeResolved		(AW::Citizen& Info);
		void	onEventDnsResolved				();

	protected: // handling of the events
		void	onEventEntityAdd				(void);
		void	onEventEntityChange				(void);
		void	onEventEntityDelete				(void);
		void	onEventEntityRiderAdd			(void);
		void	onEventEntityRiderChange		(void);
		void	onEventEntityRiderDelete		(void);
		void	onEventEntityLinks				(void);

	protected: // handling of the events
		void	onEventVisible					(void);		
		void	onEventWorldDisconnect			(int Reason);
		void	onEventWorldConnectionChange	(int State);
		void	onEventWorldAttributes			(void);

	protected: // handling of the events
		void	onEventUniverseDisconnect		(int ReasonCode);
		void	onEventUniverseRecover			(void);
		void	onEventUniverseAttributes		(void);
		void	onEventUniverseLinkClosed		(void);
		void	onEventUniverseLinkOpened		(void);

	protected: // handling of events
		void	onEventChat						(void);
		void	onEventConsoleMessage			(void);
		void	onEventBotgram					(void);
		void	onEventUrlClick					(void);

	protected: // handling of events
		void	onCallbackCreate				(int Reason);
		void	onCallbackLogin					(int Reason);
		void	onCallbackEnter					(int Reason);

	protected: // processing of events
		virtual void onProcessAvatarAdd			(Avatar& User)	{ return; }
		virtual void onProcessAvatarChange		(Avatar& User)	{ return; }
		virtual void onProcessAvatarDelete		(Avatar& User)	{ return; }
		virtual void onProcessAvatarCleanup		(Avatar& User)  { return; }
		virtual void onProcessAvatarClick		(Avatar& User, Avatar& Clicked) { return; }
		virtual void onProcessAvatarAddress		(Avatar& User, int Reason)		{ return; }
		virtual void onProcessAvatarPrivilege	(Avatar& User)  { return; }

	protected: // processing of events
		virtual void onProcessEntityAdd			(Movers::Mover& Entity)				 { return; }
		virtual void onProcessEntityChange		(Movers::Mover& Entity)				 { return; }
		virtual void onProcessEntityDelete		(Movers::Mover& Entity)				 { return; }
		virtual void onProcessEntityRiderAdd	(Avatar& User, Movers::Mover& Mover) { return; }
		virtual void onProcessEntityRiderChange	(Avatar& User, Movers::Mover& Mover) { return; }
		virtual void onProcessEntityRiderDelete	(Avatar& User, Movers::Mover& Mover) { return; }

	protected: // processing of events
		virtual void onProcessWorldLinkOpened	(void)			{ return; }
		virtual void onProcessWorldLinkClosed	(void)			{ return; }
		virtual void onProcessWorldDisconnect	(int Reason)	{ return; }
		virtual void onProcessWorldVisible		(void)			{ return; }
		virtual void onProcessWorldConnectionChange(int State)	{ return; }
		virtual void onProcessWorldAttributes	(void)			{ return; }

	protected: // processing of events
		virtual void onProcessUniverseDisconnect(int Reason)	{ return; }
		virtual void onProcessUniverseRecover	(void)			{ return; }
		virtual void onProcessUniverseAttributes(void)			{ return; }
		virtual void onProcessUniverseLinkOpened(void)			{ return; }
		virtual void onProcessUniverseLinkClosed(void)			{ return; }

	protected: // login events
		virtual void onProcessCallbackCreate	(int Reason)	{ return; }
		virtual void onProcessCallbackLogin		(int Reason)	{ return; }

	protected: // processing of events
		virtual void onProcessChat				(Avatar& User, CStringA Message, int Type) { }
		virtual void onProcessConsoleMessage	(void)						{ return; }
		virtual void onProcessBotgram			(void)						{ return; }
		virtual void onProcessUrlClick			(Avatar& User, CStringA Url){ return; }
	};

	class CommonBots
	{
	public: // type defenition
		typedef std::vector<CommonBot*> ListType;
		typedef ListType::iterator		iterator;

	public: // the list of bots
		ListType list;
		iterator begin() { return list.begin(); }
		iterator end()	 { return list.end(); }
		size_t size()	 { return list.size(); }
		void push_back(CommonBot* P) { list.push_back(P); }

	public: // static data
		static CommonBots& getInstance() { static CommonBots bots; return bots; }
	};