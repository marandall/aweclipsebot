#include "stdafx.h"
#include "property_object.h"
#include "property_async_command.h"

namespace Property
{
	AsyncMonitor g_AsyncMonitor;

	bool AsyncMonitor::dispatch_result(int ReasonCode, AW::Bot& Client)
	{
		int obj_id = Client._int(AW_OBJECT_NUMBER);
		bool found = false;

		for (iterator reqPtr = begin(); reqPtr != end(); )
		{
			if (reqPtr->obj_number == obj_id)
			{
				// local reference for easy use
				AsyncRequest& req = *reqPtr;
				req.rc		= ReasonCode;
				req.bot_ptr = &Client;

				// call handler function
				(*req.callback)(req);
				if (req.result != 0) delete req.result;
				reqPtr = m_list.erase(reqPtr);
				found = true;
			}
			else
				reqPtr++;
		}

		// return if we have dispatched anything
		return found;
	}

	void AsyncMonitor::purge_all(int ReasonCode, AW::Bot& Client)
	{
		while (m_list.size())
		{
			iterator reqPtr = begin();
			
			// local reference for easy use
			AsyncRequest& req = *reqPtr;
			req.rc		= ReasonCode;
			req.bot_ptr = &Client;

			// call handler function
			(*req.callback)(req);
			if (req.result != 0) delete req.result;
			m_list.erase(reqPtr);
		}
	}
}