#pragma once
#include "umbra_variantmap.h"
#include "umbra_instructions_opcodes.h"
#include <list>

namespace Umbra
{
	// monitor stack
	class CallStackMonitor;

	//
	// Runtime System
	//

	class ExecutionFrame
	{
	public: // mapping
		VariantMap	vars;

	};

	class Host;

	class ExecutionController
	{
	public: // variant mapping
		Umbra::OpcodeEvaluation eval;
		Host*					host_ptr;
		RemoteClass*			this_context;
		VariantMap*				extarg_ptr;
		CallStackMonitor*		call_stack_monitor;

	public: // is processing a new function
		bool			is_new_processor;

	public: // constructor
		ExecutionController()
		{
			host_ptr			= 0;
			this_context		= 0;
			extarg_ptr			= 0;
			is_new_processor	= false;
			call_stack_monitor	= 0;
		}

	public: // copy self
		ExecutionController copyNew()
		{
			ExecutionController ec;
			ec.host_ptr						= host_ptr;
			ec.call_stack_monitor	= call_stack_monitor;
			ec.extarg_ptr					= extarg_ptr;
			return ec;
		}

	public: // stack frames
		std::list<ExecutionFrame>	frames;
	};
}