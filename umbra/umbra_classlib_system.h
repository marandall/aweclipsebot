#pragma once
#include "umbra_remoteclass.h"
#include "umbra_variant.h"
#include "umbra_classlib.h"

// requires maths
#include <math.h>
#include <map>
#include <list>

namespace Umbra
{
	namespace ClassLib
	{
		//
		// std_library
		//   System.
		//   Math.
		//   Lib.
		//      Lib.Graphics
		//

		class SystemDef : public Umbra::RemoteClass
		{
		public: // property access from system
			bool doUmbraFunction(Variant& V, FunctionRequest& R);
			void doUmbraExtensibleDefSetup();

			static SystemDef& getInst()
			{
				static SystemDef d;
				return d;
			}
		};

		class TimeDef : public Umbra::RemoteClass
		{
		public: // constants for time periods
			static const int SECONDS_IN_MINUTE	= 60;
			static const int SECONDS_IN_HOUR	= 60 * 60;
			static const int SECONDS_IN_DAY		= 60 * 60 * 24;
			static const int MINUTES_IN_HOUR	= 60;
			static const int MINUTES_IN_DAY		= 60 * 24;
			static const int HOURS_IN_DAY		= 24;
		};
	}
}