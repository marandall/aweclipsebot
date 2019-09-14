#pragma once
#include "umbra_classlib.h"
#include "winbase.h"

// start of FileSystem namespace
namespace Umbra { namespace ClassLib { namespace Usl {

	class DateTimeImpl : public RemoteClass
	{
	public: // the time information
		time_t _curtime;

	public: // constructor and destructor
		DateTimeImpl()
		{
			_curtime = time(0);
		}

		~DateTimeImpl()
		{
		}

	public: // umbra routines
		bool doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			// unable to service request
			return R.NOT_HANDLED;
		}

		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			// constructor
			//

			UMBRA_FUNCTION_BEGIN(__construct) {
				UMBRA_FUNCTION_CS() 
				{
				} 
				UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// format the time using crt functions
			//

			UMBRA_FUNCTION_BEGIN(format) {
				UMBRA_FUNCTION_DA(0, Format, Umbra::VT_STRING);
				UMBRA_FUNCTION_CS() {

					// assign the data required
					tm tms = *gmtime( &_curtime );
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



			// no function identified
			return R.NOT_HANDLED;
		}
	};

	class DateTimeClass : public RemoteClass, public Umbra::INewInstanceCreator
	{
	public: // references
		static const int SECONDS_IN_MINUTE	= 60;
		static const int MINUTES_IN_HOUR	= 60;
		static const int HOURS_IN_DAY		= 24;
		static const int DAYS_IN_WEEK		= 7;
		static const int SECONDS_IN_HOUR	= SECONDS_IN_MINUTE * MINUTES_IN_HOUR;
		static const int SECONDS_IN_DAY		= SECONDS_IN_HOUR * HOURS_IN_DAY;
		static const int SECONDS_IN_HORU	= SECONDS_IN_MINUTE * MINUTES_IN_HOUR;

	public:
		RemoteClass* doUmbraNewInstance(Umbra::Host* HostPtr)
		{
			return new DateTimeImpl();
		}

		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			// convert a number of seconds into a time format
			//

			UMBRA_FUNCTION_BEGIN(durationToTimeString) {
				UMBRA_FUNCTION_DA(0, Seconds, Umbra::VT_INTEGER);
				UMBRA_FUNCTION_CS() {
			
					// split into categories
					int argSeconds = vaSeconds.toInt32();
					int seconds	= (argSeconds % SECONDS_IN_MINUTE);
					int minutes	= (argSeconds / SECONDS_IN_MINUTE) % SECONDS_IN_MINUTE;
					int hours	= (argSeconds / SECONDS_IN_HOUR) % HOURS_IN_DAY;
					int days	= (argSeconds / (SECONDS_IN_HOUR * HOURS_IN_DAY));
					
					// format string
					char output[128] = { 0 };
					sprintf_s(output, "%0.2d:%0.2d:%0.2d", hours, minutes, seconds);
					UMBRA_FUNCTION_RET( output );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// no function identified
			return R.NOT_HANDLED;
		}
	};

// end of FileSystem namespace
} } }