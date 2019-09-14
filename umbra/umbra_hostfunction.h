#pragma once
#include "umbra_variant.h"
#include "umbra_variantmap.h"
#include "umbra_instructions_managed.h"
#include "umbra_uptr.h"
#include <map>

namespace Umbra
{
	class FunctionRequest;
	class IInternalFunction
	{
	public: // the override function
		virtual bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) = 0;
	};

	namespace HostObjects
	{
		//
		// Function Defenition
		//
		
		class FunctionArg
		{
		public: // default information
			std::string func_name;
			Umbra::Instructions::ManagedOpcode default_value;
		};

		//
		// Function Class
		//

		class Function : public Umbra::u_ptrbase
		{
		public: // name of the function
			std::vector<FunctionArg>	args;	// function argumen list
			string		name;				// name of the function
			string		module;			// the module name
			int				flags;			// the number of flags
			int				access;			// the access level
			int				safety;			// the safety level

		public: // function call type
			IInternalFunction* internal_func_ptr;
			Umbra::Instructions::ManagedControlStatement code;

		public: // operations
			Function()
			{
				safety = access = flags = 0;
				internal_func_ptr = 0;
			}
		};

		//
		// Function Pointer
		//

		typedef Umbra::u_ptr<Function> FunctionPtr;
	}
}