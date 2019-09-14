#include "stdafx.h"
#include "CallbackQueue.h"
#include "LiveCacheCallback.h"
#include "property_projects.h"

namespace CallbackQueue
{
	void Queue::insert(Target* pTarget, Target* PtrSystem)
	{
		// generate a new node
		Node n;
		n.target = pTarget;
		n.system_ptr = PtrSystem;

		// general a handler
		m_list.push_back(n);
	}
	
	void Queue::handle_result(int Result)
	{
		// if there is nothing in here then there is an error
		if (m_list.size() == 0)
			return;

		// use the front
		Node& node = m_list.front();
		node.target->handle_result(*m_pBot, Result);
		if (node.system_ptr)
		{
			node.system_ptr->handle_result(*m_pBot, Result);
		}

		// delete the front
		if (node.target) delete node.target;
		if (node.system_ptr) delete node.system_ptr;

		// kill the first item
		m_list.erase(m_list.begin());
	}

	void ObjectAddCallback::handle_result(EclipseBot& Client, int Result)
	{
		// if the result is null then it no longer exists in the cache
		if (Result != 0)
		{
			return;
		}

		// search for the live cache item
		Property::Project* pProj = Property::g_Projects.Find("__live");
		if (pProj != 0)
		{
			// update the object with the new data from the SDK
			obj_data.id	= Client._int(AW_OBJECT_ID);
			obj_data.number = Client._int(AW_OBJECT_NUMBER);

			// assign as a new object
			pProj->objects.insert_object(obj_data);
		}
	}

	void ObjectChangeCallback::handle_result(EclipseBot& Client, int Result)
	{
		// if the result is null then it no longer exists in the cache
		if (Result != 0)
		{
			return;
		}
	}
	
	void ObjectDeleteCallback::handle_result(EclipseBot& Client, int Result)
	{
	}
}