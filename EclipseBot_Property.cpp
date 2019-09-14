#include "stdafx.h"
#include "EclipseBot.h"
#include "property_object.h"
#include "CallbackQueue.h"

int EclipseBot::object_handle_action(int Reason, CallbackQueue::Target* pTarget)
{
	// error in rc
	if (Reason)
	{
		pTarget->handle_result(*this, Reason);
		delete pTarget;
	}
	else
	{
		getCQObjectResult().insert(pTarget);
	}

	return Reason;
}

int	EclipseBot::object_add(ObjectData& Object, CallbackQueue::Target* pTarget)
{
	// push to the sdk
	Object.to_sdk(*this);
	return object_handle_action(_object_add(), pTarget);
}

int EclipseBot::object_delete(ObjectData& Object, CallbackQueue::Target* pTarget)
{
	// push to the sdk
	Object.to_sdk(*this);
	return object_handle_action(_object_delete(), pTarget);
}

int EclipseBot::object_change(ObjectData& Object, CallbackQueue::Target* pTarget)
{
	// push to the sdk
	Object.to_sdk(*this);
	Object.PrepChange(*this);
	return object_handle_action(_object_change(), pTarget);
}

int EclipseBot::object_change2(ObjectData& Begin, ObjectData& End, CallbackQueue::Target* pTarget)
{
	// push to the sdk
	Begin.PrepChange(*this);
	End.to_sdk(*this);
	Begin.PrepChange(*this);
	return object_handle_action(_object_change(), pTarget);
}

int EclipseBot::object_load(ObjectData& Object, CallbackQueue::Target* pTarget)
{
	// push to the sdk
	Object.to_sdk(*this);
	return object_handle_action(_object_load(), pTarget);
}
