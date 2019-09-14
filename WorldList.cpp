#include "stdafx.h"
#include "WorldList.h"
#include "scripting_host.h"
#include "scripting_iterator.h"

namespace WorldList
{
	Kernel::IntStringMap& Kernel::StatusMap(void)
	{
		static IntStringMap status_text;
		if (status_text.size() == 0)
		{
			// the status text of the types
			status_text[AW_WORLDSTATUS_PRIVATE] = "private";
			status_text[AW_WORLDSTATUS_PUBLIC]  = "public";
			status_text[AW_WORLDSTATUS_UNKNOWN] = "unknown";
		}

		// return completed
		return status_text;
	}

	Kernel::Kernel(EclipseBot& Client)
	{
		m_pClient = &Client;
		m_pending = false;
	}
	
	Kernel::~Kernel()
	{
		clear();
	}

	bool Kernel::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
	{ 
		// update controls
		UMBRA_PROPERTY_R(count,						getWorldCount());
		UMBRA_PROPERTY_R(users,						getUserCount());
		UMBRA_PROPERTY_R(empty_count,			getEmptyWorldCount());
		UMBRA_PROPERTY_R(occupied_count,	getOccupiedWorldCount());
		
		// unsupported element
		return Umbra::PropertyRequest::NOT_HANDLED;  
	}

	bool Kernel::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) 
	{ 
		UMBRA_FUNCTION_BEGIN(getIterator) {
			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( (new ScriptIterator(*this, _umbra_isa, begin(), end()))->setUmbraGarbageCollected() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(find) {
			UMBRA_FUNCTION_DA(0, World, Umbra::VT_STRING);
			// code information break
			UMBRA_FUNCTION_CS() {
				World* pWorld = lookup_world(vaWorld.toString().c_str());
				UMBRA_FUNCTION_RET( pWorld ? *pWorld : Umbra::Variant() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// unsupported element
		return Umbra::FunctionRequest::NOT_HANDLED;  
	}

	int	Kernel::query_list(void)
	{
		// query the list
		int rc = m_pClient->_world_list();
		if (rc == 0)
			m_pending = true;
		return rc;
	}

	void Kernel::handle_world_info(void)
	{
		// information from the client
		EclipseBot& client = *m_pClient;
		String	name	= client._string(AW_WORLDLIST_NAME);
		int		rating	= client._int(AW_WORLDLIST_RATING);
		int		status	= client._int(AW_WORLDLIST_STATUS);
		int		users	= client._int(AW_WORLDLIST_USERS);

		// does the world already exist
		World* pWorld = lookup_world(name);

		// update world
		if (pWorld != NULL)
		{
			pWorld->from_sdk(client);

			// if the world is stopped we should remove it from our list
			if (status == AW_WORLDSTATUS_STOPPED)
			{
				for (iterator ptrWorld = begin(); ptrWorld != end(); ptrWorld++)
				{
					// check the name
					if (ptrWorld->getName().CompareNoCase(name) != 0)
						continue;

					// scripting host
					ScriptingHost::getInst().doFunction("EventWorldsDelete", 
						&Umbra::VariantArray(*pWorld), 0, &bot);

					// erase from list
					erase(ptrWorld);

					// gone away
					broadcastToListeners(LM_WORLD_OFFLINE, &WorldChangedMessage(pWorld, 0));
					return;
				}
			}
			else
			{
				pWorld->from_sdk(client);

				// is changing
				Umbra::Variant		v;
				Umbra::VariantMap	vm = bot.CreateVariantMap(0);

				// scripting host
				ScriptingHost::getInst().doFunction("EventWorldsChange", 
					&Umbra::VariantArray(*pWorld), 0, &bot);

				// add this world
				broadcastToListeners(LM_WORLD_CHANGED, &WorldChangedMessage(pWorld, 0));
			}
		}
		else if (status != AW_WORLDSTATUS_STOPPED)
		{
			// create new world
			World w_info;
			w_info.from_sdk(client);

			// push it to the back of the list
			worlds.push_back(w_info);

			// notify send ok
			pWorld = lookup_world(w_info.getName());
			broadcastToListeners(LM_WORLD_ONLINE, &WorldChangedMessage(pWorld, 0));

			// scripting host
			ScriptingHost::getInst().doFunction("EventWorldsAdd", 
				&Umbra::VariantArray(*pWorld), 0, &bot);
		}
	}

	void Kernel::handle_world_offline(const char* Name)
	{

	}
	
	void Kernel::handle_callback(int ReasonCode)
	{
		// if there was an error, inform the user
		if (ReasonCode)
		{
			String logMsg;
			logMsg.Format("There was an error querying the world list; rc %d (%s)", ReasonCode, AW::Bot::GetReasonCode(ReasonCode));
			WriteMsg(logMsg, RGB_RED, OM_TIME | OT_DEBUG | OT_TECH);
		}
		else
		{
			// check client state
			if (m_pClient->_bool(AW_WORLDLIST_MORE) == TRUE)
				query_list();
			else
				m_pending = false;
		}
	}
	
	World* Kernel::lookup_world(const char* Name)
	{
		// get the users count
		for (iterator ptrWorld = begin(); ptrWorld != end(); ptrWorld++)
			if (ptrWorld->getName().CompareNoCase(Name) == 0)
				return &(*ptrWorld);

		// nothing
		return NULL;
	}

	void Kernel::handle_disconnection(void)
	{
		// remove each world
		clear();

		// gone away
		broadcastToListeners(LM_WORLD_CLEARED, &WorldChangedMessage(0, 0));
	}

	int	Kernel::getWorldCount(void) 
	{
		return static_cast<int>(worlds.size());	
	}

	int	Kernel::getUserCount(void)
	{
		int user_count = 0;

		// get the users count
		for (iterator ptrWorld = begin(); ptrWorld != end(); ptrWorld++)
			user_count+= ptrWorld->getUsers();

		// return
		return user_count;
	}

	int	Kernel::getEmptyWorldCount(void)
	{
		// get the empty count
		int count = 0;
		for (iterator ptrWorld = begin(); ptrWorld != end(); ptrWorld++)
			if (ptrWorld->getUsers() == 0)
				count++;

		// return
		return count;
	}

	int	Kernel::getOccupiedWorldCount(void)
	{
		// get the empty count
		int count = 0;
		for (iterator ptrWorld = begin(); ptrWorld != end(); ptrWorld++)
			if (ptrWorld->getUsers() > 0)
				count++;

		// return
		return count;
	}

	int	Kernel::getRatingCount(int Rating)
	{
		// get the count
		int count = 0;
		for (iterator ptrWorld = begin(); ptrWorld != end(); ptrWorld++)
			if (ptrWorld->getRating() == Rating)
				count++;

		// return
		return count;
	}

	int	Kernel::getUserCountPerRating(int Rating)
	{
		// get the count
		int count = 0;
		for (iterator ptrWorld = begin(); ptrWorld != end(); ptrWorld++)
			if (ptrWorld->getRating() == Rating)
				count+= ptrWorld->getUsers();

		// return
		return count;
	}

	int	Kernel::getUserCountPrivate(void) 
	{
		// return using support
		return getUserCountByStatus(AW_WORLDSTATUS_PRIVATE);
	}

	int	Kernel::getUserCountPublic(void) 
	{
		// return using support
		return getUserCountByStatus(AW_WORLDSTATUS_PUBLIC);
	}

	int	Kernel::getUserCountByStatus(int Status) 
	{
		// get the count
		int count = 0;
		for (iterator ptrWorld = begin(); ptrWorld != end(); ptrWorld++)
			if (ptrWorld->getStatus() == Status)
				count+= ptrWorld->getUsers();

		// return
		return count;
	}

	int	Kernel::getStatusCount(int Status) 
	{
		return 0;
	}

	void Kernel::clear_all(void)
	{
		worlds.clear();
	}

	//
	//
	// World Class
	//
	//

	World::World()
	{
		static int sWorldId = 0;
		uid = ++sWorldId;

		// is new, it has not been used before
		is_new = true;

		// statistics
		user_peak			= 0;
		user_peak_time		= time64(0);
		last_occupied_time	= 0;
	}

	World::~World()
	{

	}

	String World::getIconKey(void)
	{
		return getRatingString() + (IsPrivate() ? "_PRI" : "_PUB");
	}

	bool World::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// static sdk information
		UMBRA_PROPERTY_R(name, name);
		UMBRA_PROPERTY_R(rating, rating);
		UMBRA_PROPERTY_R(rating_text, getRatingString());
		UMBRA_PROPERTY_R(status, status);
		UMBRA_PROPERTY_R(users, users);

		// access status enumerations
		UMBRA_PROPERTY_R(is_public,  IsPublic());
		UMBRA_PROPERTY_R(is_private, IsPrivate());
		UMBRA_PROPERTY_R(is_unknown, IsUnknown());

		// user peak statistics
		UMBRA_PROPERTY_R(last_occupied_time, last_occupied_time);

		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	void World::from_sdk(AW::Bot& bot)
	{
		// for change tracking
		int prev_users = users;
		int prev_status = status;

		// update from client
		name	= bot._string(AW_WORLDLIST_NAME);
		rating	= bot._int(AW_WORLDLIST_RATING);
		status	= bot._int(AW_WORLDLIST_STATUS);
		users	= bot._int(AW_WORLDLIST_USERS);
		
		// last users change
		if (prev_users != users)
			last_user_change = time64(0);

		// another change
		last_change = time64(0);

		// user limits
		if (users > user_peak)
		{
			user_peak = users;
			user_peak_time = time64(0);
		}

		// last occupied
		if (users > 0)
			last_occupied_time = time64(0);

		// no longer new
		is_new = false;
	}
}