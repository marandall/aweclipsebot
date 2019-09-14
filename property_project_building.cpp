#include "stdafx.h"
#include "property_objects.h"
#include "property_projects.h"
#include "property_async_command.h"
#include "callbackqueue.h"
#include "property_asynchandler.h"

namespace Property
{

	// calls too add another object request to the queue, only called
	// when the outstanding slots is less than that available
	// returns true is the slot has been used
	bool Project::handle_available_slot(EclipseBot& Client)
	{
		// check we are a builder type
		if (!IsBuilding() && !IsBuildDeleting() && !IsBuildLoading())
			return false;

		// if limbo count equals object count
		if (state_counts.queued == 0)
			return false;

		// find an object that is not in limbo
		int counter = 0;
		for (Objects::iterator objPtr = objects.begin(); objPtr != objects.end(); objPtr++)
		{
			// skip over anything that is in limbo
			if (objPtr->isQueued() == false)
				continue;

			// create async handler
			AsyncHandler_ObjectAction* pOA = new AsyncHandler_ObjectAction(this, &*objPtr);
			int rc = 0;

			// set status as pending
			objPtr->changeBuildState(ObjectData::BS_PENDING);
			objects.update_trackers(*objPtr);
			state_counts.pending++;
			state_counts.queued--;

			// which method to use
			if (IsBuilding() == true)
			{
				rc = Client.object_add(*objPtr, pOA);
			}
			else if (IsBuildLoading() == true)
			{
				rc = Client.object_load(*objPtr, pOA);
			}
			else 
			{
				rc = Client.object_delete(*objPtr, pOA);
			}

			// process 15 per second
			if (++counter % 100 == 0)
				return true;
		}

		// no object has been found available
		return false;
	}

	void Project::useObjectResult(int ReasonCode, ObjectData& Object)
	{
		// pending is reduced
		state_counts.pending--;
		objects.update_trackers(Object);

		// did the object fail
		if (ReasonCode == 0)
		{
			Object.changeBuildState(ObjectData::BS_SUCCEEDED);
			state_counts.succeeded++;
		}
		else
		{
			Object.changeBuildState(ObjectData::BS_FAILED);
			state_counts.failed++;
		}

		// none queued and non pending
		broadcastToListeners(LE_BUILDING_COMPLETED, 0);
	}
}