#pragma once
#include "umbra_variant.h"
#include "umbra_variantmap.h"

namespace Umbra
{
	class Host;
	class CallStackMonitor;

	//
	// Collection Information
	//

	class PropertyRequestCollector
	{
	public: // the requesting information
	};

	//
	//
	//

	class PropertyRequest
	{
	public: // information to get
		std::string			id;						// the request to serve
		int					method;					// get or set the property
		Host*				host;					// the host reference
		bool				explicit_reference;		// should the reference be set
		RemoteClass*		called_pointer;			// the base pointer called
		bool				constant_returned;		// has a constant been given
		CallStackMonitor*	call_stack_monitor;		// the pointer to the call stack

	public: // methods
		bool	isSet() const { return method == METHOD_SET; }
		bool	isGet() const { return method == METHOD_GET; }

	public: // constructor
		PropertyRequest()
		{
			explicit_reference	= false;
			called_pointer		= 0;
			host				= 0;
			constant_returned	= false;
			call_stack_monitor	= 0;
		}

	public: // the method employed
		static const int METHOD_SET = 0;
		static const int METHOD_GET = 1;

	public: // has it handled or not
		static const bool NOT_HANDLED = false;
		static const bool HAS_HANDLED = true;
	};

	//
	//
	//

	class FunctionRequest
	{
	public: // rect
		std::string					id;
		VariantArray				args;
		Host*								host;
		RemoteClass*				called_pointer;
		CallStackMonitor*		call_stack_monitor;

	public: // constructor
		FunctionRequest()
		{
			host = 0;
			called_pointer = 0;
		}

	public: // has it handled or not
		static const bool NOT_HANDLED = false;
		static const bool HAS_HANDLED = true;
	};

	// 
	// 
	// 

	class UnsupportedPropertyException
	{
	public:
		std::string id;

	public:
		UnsupportedPropertyException(PropertyRequest& ID)
		{
			// id = ID.id;
		}
	};

	//
	//
	//

	class UnsupportedFunctionException
	{
	public:
		std::string id;

	public:
		UnsupportedFunctionException(FunctionRequest& R)
		{
			// id = R.id;
		}
	};
};