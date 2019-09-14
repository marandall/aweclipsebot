#include "stdafx.h"
#include "world_ejections.h"

namespace Ejections
{
	bool Ejection::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		UMBRA_PROPERTY_R(type,				type);
		UMBRA_PROPERTY_R(type_str,		getTypeName());
		UMBRA_PROPERTY_R(address,			address);
		UMBRA_PROPERTY_R(address_str, toString());
		UMBRA_PROPERTY_R(created,			created);
		UMBRA_PROPERTY_R(target_name,	getCommentName());
		UMBRA_PROPERTY_R(source_name,	getCommentEjector());
		UMBRA_PROPERTY_RW(expires,		expires);
		UMBRA_PROPERTY_RW(comment,		comment);
		
		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	String Ejection::toString()
	{
		String ip;	
		if (type == AW_EJECT_BY_ADDRESS)
		{
			ip.Format("%d.%d.%d.%d", address & 0xFF, (BYTE)(address >> 8),(BYTE)(address >> 16),(BYTE)(address >> 24));
		}
		else if (type == AW_EJECT_BY_COMPUTER)
		{
			ip.Format("%X", address);
		}
		else if (type == AW_EJECT_BY_CITIZEN)
		{
			ip.Format("%d", address);
		}
		else
		{
			ip.Format("?? %d", address);
		}

		return ip;
	}

	String Ejection::getTypeName()
	{
		if (type == AW_EJECT_BY_ADDRESS)
			return "Address";
		else if (type == AW_EJECT_BY_COMPUTER)
			return "Computer";
		else if (type == AW_EJECT_BY_CITIZEN)
			return "Citizen";
		else
			return "Unknown";
	}

	String Ejection::getCommentName()
	{
		int pos = comment.Find(" (");
		if (pos != -1)
		{
			return comment.Left(pos);
		}
		else
		{
			return "";
		}
	}

	String Ejection::getCommentEjector()
	{
		// "noblecock" (0/0) ejected by [Sysops] (0/319340)
		int s1 = comment.Find(") ejected by ");
		if (s1 != -1)
		{
			int s2 = comment.Find(" (", s1);
			if (s2 != -1)
			{
				int len = s2 - s1 - 13;
				return comment.Mid(s1 + 13, len);
			}
			else
			{
				return "";
			}
		}
		else
		{
			return "";
		}
	}

	EjectionManager::EjectionManager(AW::Bot* BotPtr)
	{
		_pBot			= BotPtr;
		_querying = false;
		_error		= 0;
	}

	String EjectionManager::getStatus()
	{
		String s;

		if (_querying == true)
		{
			s.Format("Scanning Database (%d found)...", (int)_ejections.size());
			return s;
		}
		else
		{
			s.Format("Waiting");
			return s;
		}
	}

	void EjectionManager::HandleCallbackWorldEjection(int ReturnCode)
	{
		this;
		if (ReturnCode == RC_NO_SUCH_EJECTION)
		{
			// we have reached the end of the list
			_error					= 0;
			_querying				= false;
			_first					= false;

			// deal with anything that has expired
			for (iterator ej = begin(); ej != end(); )
			{
				if (ej->was_detected == false)
				{
					broadcastToListeners(LE_EJECTION_EXPIRED, &MessageEjectionExpired(&*ej));
					ej = _ejections.erase(ej);
				}
				else
				{
					ej++;
				}
			}
		}
		else if (ReturnCode != 0)
		{
			// there was an other form of error
			_error					= ReturnCode;
			_last_complete	= time64(0);
			_querying				= false;
		}
		else
		{
			// does an existing ejection occur
			Ejection* eject = getEjection(_pBot->_int(AW_EJECTION_ADDRESS), _pBot->_int(AW_EJECTION_TYPE), _pBot->_int(AW_EJECTION_CREATION_TIME));
			if (eject)
			{
				eject->was_detected = true;

				// check if changes have been made
				bool changes_made = false;
				if (eject->expires != _pBot->_int(AW_EJECTION_EXPIRATION_TIME) || eject->comment != _pBot->_string(AW_EJECTION_COMMENT))
					changes_made = true;

				// update the session
				eject->expires	= _pBot->_int(AW_EJECTION_EXPIRATION_TIME);
				eject->comment	= _pBot->_string(AW_EJECTION_COMMENT);
			
				// have chanegs been made
				if (changes_made)
					broadcastToListeners(LE_EJECTION_UPDATED, &MessageEjectionChanged(eject));
			}
			else
			{
				Ejection ej;
				ej.type					= _pBot->_int(AW_EJECTION_TYPE);
				ej.address			= _pBot->_int(AW_EJECTION_ADDRESS);
				ej.created			= _pBot->_int(AW_EJECTION_CREATION_TIME);
				ej.expires			= _pBot->_int(AW_EJECTION_EXPIRATION_TIME);
				ej.comment			= _pBot->_string(AW_EJECTION_COMMENT);
				ej.was_detected	= true;
				_ejections.push_back(ej);

				// alert all listeners
				broadcastToListeners(LE_EJECTION_FOUND, &MessageEjectionFound(&_ejections.back()));
			}

			QueryNext();
		}

		// status has been changed
		broadcastToListeners(LE_STATUS, 0);
	}
	
	Ejection* EjectionManager::getEjection(int Ip, int IpType, int Start)
	{
		for (iterator ej = begin(); ej != end(); ej++)
			if (ej->address == Ip && ej->type == IpType && ej->created == Start)
				return &(*ej);
		return 0;
	}

	int	EjectionManager::QueryNext()
	{
		int rc = _pBot->_world_ejection_next();
		if (rc)
		{
			_error			= rc;
			_last_query	= time64(0);
			_querying		= false;
		}
		else
		{
			// there was an other form of error
			_error			= 0;
			_last_query	= time64(0);
			_querying		= true;
		}
		return rc;
	}

	int	EjectionManager::cleanQuery()
	{
		_querying = false;
		_first		= true;
		_error		= 0;

		// notify everyone
		broadcastToListeners(LE_CLEARED, 0);
		_ejections.clear();
		return 0;
	}

	int EjectionManager::beginQuery(bool First)
	{
		// if this is the first time
		if (First)
		{
			_querying = false;
			_first		= true;
			_error		= 0;
		}

		// check first
		if (_querying == false)
		{
			// reset search attributes
			_pBot->_int_set(AW_EJECTION_TYPE, 0);
			_pBot->_int_set(AW_EJECTION_ADDRESS, 0);

			// reset the version
			for (iterator ej = begin(); ej != end(); ej++)
				ej->was_detected = false; 

			// status has been changed
			broadcastToListeners(LE_STATUS, 0);
		}
		else
		{
			return 0;
		}

		// query from the first item
		return QueryNext();
	}

	void EjectionManager::onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* Data)
	{
		if (Broadcaster == &bot && Id == CommonBot::LE_WORLD_CONNECTION)
		{
			_querying	= false;
			_complete	= false;
		}
	}
}