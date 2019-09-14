#pragma once
#include "callbackqueue.h"
#include "property_object.h"

namespace CallbackQueue
{
	class ObjectAddCallback : public Target
	{
	public: // the object data to employ
		ObjectData obj_data;

	public: // processing
		void handle_result(EclipseBot& Client, int Result);
	};

	class ObjectChangeCallback : public Target
	{
	public: // the object data to employ
		ObjectData obj_data;		// code to change
		__int64	   obj_tf;			// object identification pair

	public: // processing
		void handle_result(EclipseBot& Client, int Result);
	};

	class ObjectDeleteCallback : public Target
	{
	public: // the object data to employ
		__int64	   obj_tf;			// object identification pair

	public: // processing
		void handle_result(EclipseBot& Client, int Result);
	};
}