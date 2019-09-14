#pragma once
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variant.h"
#include <list>

namespace ScriptingHost_ClassLib
{
	class D_Timer : public Umbra::RemoteClass
	{
	protected: // the events
		struct Trigger
		{
			time_t					ticks;		// the full number of seconds plus a number of ticks
			String					function;	// the function name to call
			Umbra::Variant	arg;			// the argument to pass
		};

	public: // type defenition
		typedef std::list<Trigger> TriggerList;
		typedef TriggerList::iterator iterator;
		TriggerList triggers;

	public: // add a number of ticks
		int insert(int Miliseconds, String Function, Umbra::Variant& Arg);
		void process();
		void clear();

	public: // handler
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};
}