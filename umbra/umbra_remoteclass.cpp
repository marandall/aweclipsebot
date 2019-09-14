#include "stdafx.h"
#include "umbra_remoteclass.h"
#include "umbra_resourcemanager.h"
#include "umbra_variantmap.h"
#include "umbra_callstackmonitor.h"

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace Umbra
{
	NativeRuntimeErrorException::NativeRuntimeErrorException(CallStackMonitor* CallStack, std::string Err)
	{
		error_info = Err;

		// allocate the given number of items for the stack trace
		trace.resize(CallStack->_frames.size());

		// for every item
		for (size_t i = 0; i < CallStack->_frames.size(); i++)
		{
			trace[i] = CallStack->_frames[i]->info();
		}
	}

	RemoteClass::RemoteClass()
	{
		// set defaults
		_umbra_garbage_collected = false;
		_umbra_res_id				= 0;
		_umbra_ref_count		= 0;
		_umbra_owning_host	= 0;

		// on register
		onUmbraRegister();
	}

	RemoteClass::RemoteClass(const RemoteClass& Remote)
	{
		// set defaults
		_umbra_garbage_collected = false;
		_umbra_res_id			 = 0;
		_umbra_ref_count		 = 0;

		// on register
		onUmbraRegister();
	}

	RemoteClass& RemoteClass::operator=(const RemoteClass& Remote)
	{
		return *this;
	}

	RemoteClass::~RemoteClass()
	{
		// if the resource does not exist
		if (_umbra_res_id != 0)
		{
			// erase it from the list
			ResourceManager& rm = ResourceManager::GetRM();
			int index = _umbra_res_id % rm.MAX_RES_FRAMES;

			// access the iterator index for this item
			if (rm.running == true)
			{
				ResourceFrame::iterator ri = rm.frames[index].indexes.find(_umbra_res_id);
				if (ri != rm.frames[index].indexes.end())
				{
					rm.frames[index].indexes.erase(ri);
				}
			}
	
			// no longer has a key
			_umbra_res_id = 0;
		}
	}

	RemoteClass& RemoteClass::setUmbraGarbageCollected()
	{
		// if the value is not available then now would be a good time to register it
		if (_umbra_res_id == 0)
			_umbra_res_id = ResourceManager::GetRM().register_resource(*this);

		// set as collected
		_umbra_garbage_collected = true;
		return *this;
	}

	//
	// Referencing
	//

	int RemoteClass::doUmbraAddRef()
	{
		// if the value is not available then now would be a good time to register it
		if (_umbra_res_id == 0)
			_umbra_res_id = ResourceManager::GetRM().register_resource(*this);

		// increment the reference count
		_umbra_ref_count++;
		
		// nothing
		return 0;
	}

	int RemoteClass::doUmbraDeleteRef()
	{
		// increment the reference count
		_umbra_ref_count--;

		// if ref count is null and garbage collected
		int flags = getUmbraFlags(); 
		if (_umbra_ref_count == 0)
		{
			// remove instance only if invalidated
			if ((flags & UMBRA_FLAGS_ALLOCATE_ONCE) == UMBRA_FLAGS_ALLOCATE_ONCE || 
				_umbra_garbage_collected == true)
			{
				// erase it from the list
				ResourceManager& rm = ResourceManager::GetRM();
				int index = _umbra_res_id % rm.MAX_RES_FRAMES;

				// access the iterator index for this item
				ResourceFrame::iterator ri = rm.frames[index].indexes.find(_umbra_res_id);
				if (ri != rm.frames[index].indexes.end())
				{
					rm.frames[index].indexes.erase(ri);
				}

				// no longer has a key
				_umbra_res_id = 0;
			}

			// is this garbage collected?
			if (_umbra_garbage_collected == true)
			{
				delete this;
			}
		}

		// nothing
		return 0;
	}

	//
	// Parse a variable mapping in the form a.b.c
	//
	
	Umbra::Variant VariantMap::queryVariant(const char* Param)
	{
		char* start = (char*)Param;
		char* p = start;

		// the variant used for tracking
		bool    first = true;
		Variant tracker;

		// run through
		while (true)
		{
			if (*p == '.' || *p == '\0')
			{
				std::string token(start, p - start);
				if (first == true)
				{
					// if the first element doesnt exist we have a major problem
					if (id_exists(token.c_str()) == end())
					{
						char error[1024];
						sprintf_s(error, sizeof(error), "[unknown:%s", token.c_str());
						return error;
					}
					else
					{
						tracker = m_map[token];
						first = false;
					}

					// assign start
					start = (p + 1);
				}
				else
				{
					// attempt only if a class interface, otherwise throw a null
					RemoteClass* pRem = tracker.toRemoteClass();
					if (pRem == 0)
					{
						return Variant();
					}

					// create a property request to access next
					PropertyRequest pr;
					pr.method = PropertyRequest::METHOD_GET;
					pr.id = token;

					// call function
					Variant ret;
					if (pRem->doUmbraProperty(ret, pr) == false)
					{
						char error[1024];
						sprintf_s(error, sizeof(error), "[unknown_property:%s", token.c_str());					
						return error;
					}

					// assign to tracker
					tracker = ret;
				}

				// was it a nul byte that caused it to end
				if (*p == '\0')
					break;
			}
			p++;
		}

		// tracker is good
		return tracker;
	}

	std::string VariantMap::parse_string(const char* Input)
	{
		char* p = const_cast<char*>(Input);
		char* start = p;

		std::string processed;

		// through each element
		while (*p != '\0')
		{
			// scan while we arnt doing a bracket
			start = p;
			while (*p != '{' && *p != '\0')
				p++;

			// copy the string element
			std::string raw(start, p - start);
			processed.append(raw);

			// loop through
			if (*p == '{')
			{
				p++;
				start = p;
				while (*p != '}' && *p != '\0')
					p++;

				// copy element
				std::string raw(start, p - start);

				// is the first elemnt a $
				if (raw[0] == '$')
				{
					Variant v = queryVariant(raw.substr(1).c_str());
					processed.append(v.toString());
				}
				else
				{
				}
				p++;
			}
		}

		return processed;
	}

}