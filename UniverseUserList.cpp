#include "stdafx.h"
#include "scripting_host.h"
#include "UniverseUserList.h"
#include "database.h"
#include "adodb_sqlbuilder.h"
#include "helpers_xml.h"

namespace UniverseUserList
{
	Visit::Visit(const User* User)
	{
		timestamp	= time64(0);
		world		= User->getWorld();
	}

	void Kernel::onUserInfo(CommonBot* Bot)
	{
		User* pUniUser = getId(Bot->_int(AW_USERLIST_ID));
		if (pUniUser)
		{
			pUniUser->updateDataBySdk(*Bot);
		
			// is the current state changed
			if (pUniUser->isOffline() == true)
			{
				// remove the user from the list
				for (iterator ptr = begin(); ptr != end(); ptr++)
				{
					if (ptr->getId() == pUniUser->getId())
					{
						// scripting host
						ScriptingHost::getInst().doFunction("EventUniverseUserDelete", 
							&Umbra::VariantArray(*ptr), 0, &bot);

						// signal yay
						broadcastToListeners(LM_OFFLINE, &UserChangedMessage(pUniUser));
						list.erase(ptr);
						return;
					}
				}
			}
			else
			{
				// scripting host
				ScriptingHost::getInst().doFunction("EventUniverseUserChange", 
					&Umbra::VariantArray(*pUniUser), 0, &bot);

				// signal yay
				broadcastToListeners(LM_CHANGED, &UserChangedMessage(pUniUser));
			}
		}
		else if (Bot->_int(AW_USERLIST_STATE) == AW_USERLIST_STATE_ONLINE)
		{
			// push into the existing list
			User u;
			list.push_back(u);

			// addition
			User &u2 = list.back();
			u2.updateDataBySdk(*Bot);

			// scripting host
			ScriptingHost::getInst().doFunction("EventUniverseUserAdd", 
				&Umbra::VariantArray(u2), 0, &bot);

			// signal yay
			broadcastToListeners(LM_ONLINE, &UserChangedMessage(&u2));
		}
	}

	void Kernel::onDisconnect()
	{
		list.clear();
		broadcastToListeners(LM_CLEARED, &UserChangedMessage(0));
	}

	User* Kernel::getId(int Id)
	{
		for (iterator ptr = begin(); ptr != end(); ptr++)
			if (ptr->getId() == Id)
				return &*ptr;
		return 0;
	}

	void User::updateDataBySdk(AW::Bot& Client)
	{
		String prev_world = world;

		// update information
		m_name				= Client._string(AW_USERLIST_NAME);
		email					= Client._string(AW_USERLIST_EMAIL);
		world					= Client._string(AW_USERLIST_WORLD);
		m_citizen			= Client._int(AW_USERLIST_CITIZEN);
		m_privilege		= Client._int(AW_USERLIST_PRIVILEGE);
		state					= Client._int(AW_USERLIST_STATE);
		m_address_lng	= Client._int(AW_USERLIST_ADDRESS);
		id						= Client._int(AW_USERLIST_ID);

		// has a change been made
		if (prev_world != world)
		{
			Visit visit(this);
			getVisits().list.push_back(visit);
		}
	}

	bool User::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{		
		// the properties
		UMBRA_PROPERTY_R(name,					getName());
		UMBRA_PROPERTY_R(session,				getSession());	
		UMBRA_PROPERTY_R(citizen,				getCitizen());
		UMBRA_PROPERTY_R(privilege,			getPrivilege());
		UMBRA_PROPERTY_R(priv_name,			getPrivName());
		UMBRA_PROPERTY_R(citizen_info,	getCitizenInfo());
		UMBRA_PROPERTY_R(email,					email);
		UMBRA_PROPERTY_R(world,					world);
		UMBRA_PROPERTY_R(id,						id);

		// element not found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}
}