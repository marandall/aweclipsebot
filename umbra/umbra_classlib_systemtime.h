#pragma once

#include "umbra_classlib.h"
#include "umbra_classlib_systemtime.h"
#include <stdio.h>
#include <time.h>

// start of FileSystem namespace
namespace Umbra { namespace ClassLib { 

	//
	// Defenition for the DateTime class
	//

	class DateTimeDef : public RemoteClass, public INewInstanceCreator
	{
	public: // the instance creator interface
		Variant doUmbraNewInstance(FunctionRequest& R)
		{
			return (new DateTime())->setUmbraGarbageCollected();
		}
	};

	// 
	// DateTime class for reading data
	// 

	class DateTime : public RemoteClass
	{
	protected:
		time_t cur_time;

	public: // property access from system
		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{			
			return false;
		}
	};

// end of ClassLib namespace
} } 