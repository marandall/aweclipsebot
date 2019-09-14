#include "stdafx.h"
#include "property_projects.h"
#include "property_asynchandler.h"
#include "property_objects.h"
#include "property_object.h"

namespace Property
{
	void Project::setJobType(int Assgn)
	{
		job_type = Assgn;
		broadcastToListeners(Project::LE_BUILD_STATE_CHANGE, 0);
	}

	void Project::recount()
	{
		Property::BuildStateCounts bsc;

		for (Objects::iterator obj = objects.begin(); obj != objects.end(); obj++)
		{
			if (obj->isQueued()) bsc.queued++;
			else if (obj->isPending()) bsc.pending++;
			else if (obj->isSucceeded()) bsc.succeeded++;
			else if (obj->isFailed()) bsc.failed++;
		}

		// set counts
		state_counts = bsc;
	}

	AsyncHandler_ObjectAction::AsyncHandler_ObjectAction(Project* P, ObjectData* Obj)
	{
		v_project	= P->getUmbraRemoteClass();
		v_object	= Obj->getUmbraRemoteClass();
	}

	void AsyncHandler_ObjectAction::handle_result(EclipseBot& Client, int Result)
	{
		// cast to project
		Project* pProject = dynamic_cast<Project*>(v_project.toRemoteClass());
		if (pProject == NULL) return;

		// cast to object
		ObjectData* pObject = dynamic_cast<ObjectData*>(v_object.toRemoteClass());
		if (pObject == NULL) return; 

		// dispatch to project
		pProject->useObjectResult(Result, *pObject);
	}
}