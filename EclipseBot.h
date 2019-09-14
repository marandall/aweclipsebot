#pragma once

#include "AWSDK_Bot.h"				// for AW::Bot
#include "uul_Interface.h"			// for UUL List

// umbra script system
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variantmap.h"
#include "CommonBot.h"
#include "listener.h"

using namespace AW::Enums;

class UniUser;
class Avatar;
class ObjectData;

namespace CallbackQueue
{
	class Queue;
	class Target;
}

namespace AW
{
	class Query_5x5;
	class Query_World;
	class Query_Cache;
}

namespace Ejections
{
	class EjectionManager;
}

namespace Property
{
	class Objects;
}

namespace Querying
{
	class Query_5x5;
	class MultiQueue;
	class WorldQuery;
	class IQuery;

}

namespace UniverseUserList
{
	class Kernel;
};

namespace WorldList
{
	class Kernel;
}

namespace Movement
{
	class Kernel;
};

namespace BotgramTransferProtocol
{
	class ReceiveProcessor;
	class ReceiverThread;
	class ReceiveProcessor;
}

class EclipseBot :
	public		CommonBot,								// 4x compliant base bot
	public		Listener,
	public		EventBroadcaster
{
public:
	EclipseBot(void);
	~EclipseBot(void);
	void		DEM_Message(const char* MsgID, void* pData);

public: // umbra interfaces
	bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

protected: // listener
	void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo);

public:
	void	OnTick(int ms);
	void	Lock()   { ; }
	void	Unlock() { ; }

protected: // avatar events
	void	onProcessAvatarAdd			(Avatar& User);
	void	onProcessAvatarChange		(Avatar& User);
	void	onProcessAvatarDelete		(Avatar& User);
	void	onProcessAvatarCleanup	(Avatar& User);
	void	onProcessAvatarClick		(Avatar& User, Avatar& Clicked);
	void	onProcessAvatarAddress	(Avatar& User, int Reason);
	void	onProcessAvatarPrivilege(Avatar& User);

protected: // processing of events
	void	onProcessChat							(Avatar& User, CStringA Message, int Type);
	void	onProcessConsoleMessage		(void);
	void	onProcessBotgram					(void);
	void	onProcessUrlClick					(Avatar& User, CStringA Url);

protected: // universe link
	void	onProcessUniverseLinkClosed	(void);
	void	onProcessUniverseLinkOpened	(void);

protected: // world link
	void	onProcessWorldLinkClosed	(void);
	void	onProcessWorldLinkOpened	(void);

protected:
	/* events triggered for the universe */
	void	onEventUniverseDisconnect	(int ReasonCode);
	void	onEventUniverseRecover		(void);
	void	onEventUniverseAttributes	(void);
	void	onCallbackWorldList				(int ReasonCode);
	void	onEventUserInfo						(void);
	void	onCallbackUserList				(int ReasonCode);
	void	onEventUniverseConnectionChange(int state);

	// citizen lookup and manipulation callbacks
	void	onCallbackCitizenAttributes(int ReasonCode);
	void	onCallbackCitizenResult	(int ReasonCode);

	/* events triggered by world events or connection states */
	void	onEventVisible					(void);	
	void	onEventWorldAttributes	(void);
	void	onEventWorldDisconnect	(int ReasonCode);	
	void	onEventWorldIOChange		(void);		
	void	onEventWorldInfo				(void);
	void	onEventMove							(void);
	void	onEventWorldConnectionChange(int state);
	void	onEventMoveStateChanged	(void);
	void	onEventTeleport					(void);
	void	onEventHudClick					(void);
	void	onCallbackWorldEjection	(int rc);

	/* events triggered by objects and queries */
	void	onEventCellBegin			(void);
	void	onEventCellEnd				(void);
	void	onEventCellObject			(void);	
	void	onCallbackQuery				(int ReasonCode);
	void	onCallbackCellResult	(int ReasonCode);
	void	onCallbackObjectResult(int ReasonCode);
	void	onEventObjectAdd			(void);
	void	onEventObjectDelete		(void);
	void	onEventObjectClick		(void);

public:
	/* functions to control the universe connection state of the bot*/
	int		InitialiseClient	(void);
	bool	GetGlobal					(void) const;
					
	/* world related */
public:
	int		ExitWorld					(void);	

	/* relating to the communication message s*/
	int		ConsoleWhisper	(int SessionId, const char* MsgText, COLORREF ColourId, int Style);
	int		Botgram					(int Citizen, LPCSTR Message, bool Force = false);
	void	SwitchOutputCSL	(void);
	void	SwitchOutputSSL	(void);
	String	m_OutputBreakers;
	String	getOuputBreakers(void) const { return m_OutputBreakers; }

public: // additional umbra support
	Umbra::VariantMap		CreateVariantMap(Avatar* pUser = NULL);

protected: // universe user list
	UniverseUserList::Kernel* _universeUserList;

public: // uni host
	UniverseUserList::Kernel& getUserList() { return *_universeUserList; }

protected: // query modes
	Querying::Query_5x5*	m_query_5x5;
	Querying::MultiQueue*	query_multiqueue;
	Querying::WorldQuery*	query_world;

	Property::Objects*		m_live_cache;
	AW::Query_World*		m_query_world;
	int						m_query_mode;
	void					setQueryMode(int Mode) { m_query_mode = Mode; }

public: // access methods
	Querying::Query_5x5&	getQuery5x5();
	Querying::WorldQuery&	getQueryWorld() { return *query_world; }
	Querying::MultiQueue&	getQueryLarge() { return *query_multiqueue; }

	Property::Objects&		getLiveCache();
	int						getQueryMode() { return m_query_mode; }

protected: // ejection methods
	Ejections::EjectionManager* _ejectManager;
public:
	Ejections::EjectionManager& getEjectManager() { return *_ejectManager; }

public: // Query actions
	int						Query5x5(int X, int Z);
	int						queryBackup(int Format, CString File);
	int						QueryWorld();
	int						QueryEnd();
	int						queryLargeArea(AW::Coordinates NW, AW::Coordinates SE);
	int						queryLargeAreaAround(AW::Coordinates Mid, int Width, int Height);
	Querying::IQuery*		getCurrentQuery();

public: // Callback Results
	CallbackQueue::Queue*	m_pCQ_object_result;
	CallbackQueue::Queue*	m_pCQ_world_list;
	CallbackQueue::Queue*	m_pCQ_botgram_result;
	CallbackQueue::Queue*	m_pCQ_citizen_result;
	CallbackQueue::Queue*	m_pCQ_licence_attributes;
	CallbackQueue::Queue*	m_pCQ_licence_result;

public: // accessor functions for the callback queues
	CallbackQueue::Queue&	getCQObjectResult()		{ return *m_pCQ_object_result;		}
	CallbackQueue::Queue&	getCQWorldResult()			{ return *m_pCQ_world_list;			}
	CallbackQueue::Queue&	getCQBotgramResult()		{ return *m_pCQ_botgram_result;		}
	CallbackQueue::Queue&	getCQCitizenResult()		{ return *m_pCQ_citizen_result;		}
	CallbackQueue::Queue&	getCQLicenceAttributes()	{ return *m_pCQ_licence_attributes;	}
	CallbackQueue::Queue&	getCQLicenceResult()		{ return *m_pCQ_licence_result;		}

public: // citizen editing
	int		citizen_add		(AW::Citizen& CitInfo, CallbackQueue::Target* pTarget);
	int		citizen_change	(AW::Citizen& CitInfo, CallbackQueue::Target* pTarget);
	int		citizen_delete	(int Citizen, CallbackQueue::Target* pTarget);

public: // property manipulations
	int		object_handle_action(int Reason, CallbackQueue::Target* pTarget);
	int		object_add		(ObjectData& Object, CallbackQueue::Target* pTarget);
	int		object_delete	(ObjectData& Object, CallbackQueue::Target* pTarget);
	int		object_change	(ObjectData& Object, CallbackQueue::Target* pTarget);
	int		object_change2	(ObjectData& Object, ObjectData& ChangeTo, CallbackQueue::Target* pTarget);
	int		object_load		(ObjectData& Object, CallbackQueue::Target* pTarget);

public: // world list
	WorldList::Kernel* m_pWorldList;
	WorldList::Kernel& getWorldList() { return *m_pWorldList; }

public: // botgram handling
	BotgramTransferProtocol::ReceiveProcessor* _pBotgramReceiver;
	BotgramTransferProtocol::ReceiveProcessor& getBotgramReceiver() { return *_pBotgramReceiver; }

public: // streaming botgram communications
	int		botgram_stream_send(int Citizen, const char* Reducer, const char* Type, const char* Text);
};