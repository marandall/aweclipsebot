#include "stdafx.h"				// for precompiled header
#include "EclipseBot.h"			// for EclipseBot
#include "awsdk_query.h"
#include "property_objects.h"
#include "property_query_5x5.h"
#include "property_query_queue.h"
#include "property_query_world.h"
#include "CallbackQueue.h"
#include "worldlist.h"
#include "UniverseUserList.h"
#include "movement.h"
#include "BotgramProtocolV2.h"
#include "world_ejections.h"
#include "scripting_host.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

class EclipseBotEjectionListener : public Listener
{
public:
	void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* Msg)
	{
		if (Broadcaster == &bot.getEjectManager())
		{
			using namespace Ejections;
			String text;

			if (Id == EjectionManager::LE_EJECTION_FOUND)
			{
				MessageEjectionFound* ejm = dynamic_cast<MessageEjectionFound*>(Msg);
				ScriptingHost::getInst().doFunction("EventWorldEjectionFound", 
					&Umbra::VariantArray(*ejm->eject), 0, &bot);
			}
			else if (Id == EjectionManager::LE_EJECTION_UPDATED)
			{
				MessageEjectionChanged* ejm = dynamic_cast<MessageEjectionChanged*>(Msg);
				ScriptingHost::getInst().doFunction("EventWorldEjectionChanged", 
					&Umbra::VariantArray(*ejm->eject), 0, &bot);
			}
			else if (Id == EjectionManager::LE_EJECTION_EXPIRED)
			{
				MessageEjectionExpired* ejm = dynamic_cast<MessageEjectionExpired*>(Msg);
				ScriptingHost::getInst().doFunction("EventWorldEjectionExpired", 
					&Umbra::VariantArray(*ejm->eject), 0, &bot);
			}
		}
	}
};


EclipseBotEjectionListener botEjectListener;

EclipseBot::EclipseBot(void)
{
	m_query_5x5							= NULL;
	m_query_world						= NULL;
	m_live_cache						= NULL;

	// create the callback queues
	using namespace CallbackQueue;
	m_pCQ_object_result			= new Queue(this);
	m_pCQ_world_list				= new Queue(this);
	m_pCQ_botgram_result		= new Queue(this);
	m_pCQ_citizen_result		= new Queue(this);
	m_pCQ_licence_attributes= new Queue(this);
	m_pCQ_licence_result		= new Queue(this);

	// create world list
	m_pWorldList						= new WorldList::Kernel(*this);
	_universeUserList				= new UniverseUserList::Kernel();
	_pBotgramReceiver				= new BotgramTransferProtocol::ReceiveProcessor();

	// movement
	m_pMovementKernel				= new Movement::Controller();

	// the other query system
	query_multiqueue				= new Querying::MultiQueue(this);
	query_world							= new Querying::WorldQuery(this);
	_ejectManager						= new Ejections::EjectionManager(this);

	botEjectListener.connectListener(getEjectManager());
}

EclipseBot::~EclipseBot(void)
{
	delete m_query_5x5;
	delete m_query_world;
	delete query_multiqueue;
	delete query_world;

	// delete callback queues
	delete m_pCQ_object_result;
	delete m_pCQ_world_list;
	delete m_pCQ_botgram_result;
	delete m_pCQ_citizen_result;
	delete m_pCQ_licence_attributes;
	delete m_pCQ_licence_result;

	// delete world list
	delete m_pWorldList;
	delete _universeUserList;
	delete _pBotgramReceiver;
	delete _ejectManager;

	// movement
	delete m_pMovementKernel;
}

void EclipseBot::onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* Msg)
{
}

void EclipseBot::DEM_Message(const char* MsgID, void* pData)
{
	String logMsg;

	// is the query complete
	if (strcmp(MsgID, "5x5_query_complete") == 0)
	{
		logMsg.Format("5x5 Query Complete; %d objects found", (int)getLiveCache().size());
		WriteMsg(logMsg, RGB_NOTIFY, OT_MAIN | OT_DEBUG | OM_TIME);
	}
}

int	EclipseBot::InitialiseClient(void)
{
	setWorldConnection(ConState::Nothing);
	setUniverseConnection(ConState::Nothing);
	
	return 0;
}