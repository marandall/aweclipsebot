#pragma once
#include "umbra_classlib.h"
#include <time.h>

// open namespace
namespace Umbra { namespace ClassLib {

	class IFunction
	{
	public: // same function
		virtual bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) = 0;
	};

	class Time : public IFunction
	{
	public: // sole function
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
		{
			// get the time
			UMBRA_FUNCTION_BEGIN(time) {
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET(time(0));
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// format the time
			UMBRA_FUNCTION_BEGIN(format_date) {
				UMBRA_FUNCTION_DA(0, Time, Umbra::VT_INTEGER);
				UMBRA_FUNCTION_DA(1, Format, Umbra::VT_STRING);
				UMBRA_FUNCTION_CS() {

					// assign the data required
					tm tms = *gmtime( vaTime.toInt64() );
					char buffer[256] = { 0 };

					// format based on data
					if (strftime(buffer, sizeof(buffer), vaFormat.toString().c_str(), &tms) == 0)
					{
						// failure
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
					else
					{
						// success
						UMBRA_FUNCTION_RET( buffer );
					}

				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();
		}
	};

// end of namespace
} }