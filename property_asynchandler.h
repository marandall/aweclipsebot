#pragma once
#include "property_async_command.h"
#include "callbackqueue.h"

namespace Property
{
	class Project;

	class AsyncHandler_ObjectAction : public CallbackQueue::Target
	{
	public: // this object
		Umbra::Variant	v_project;
		Umbra::Variant	v_object;

	public: // tracking numbers required
		int			cell_x;
		int			cell_z;
		int			number;

	public: // constructor
		AsyncHandler_ObjectAction(Project* P, ObjectData* Obj);
		virtual void handle_result(EclipseBot& Client, int Result);
	};
}