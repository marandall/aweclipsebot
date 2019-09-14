#include "stdafx.h"
#include "SystemWordsFilter.h"
#include "FloodPreventionPolicy.h"

namespace Security
{
	GlobalFilterPolicy g_FilterPolicy;
	FloodPreventionPolicy g_FloodPolicy;

	bool GlobalFilterPolicy::Load(AW::EnhancedINI& INI)
	{
		const int READ_FROM_INI = true;
		const char* sect_name = "filter_policy";

		// configure defaults
		enabled  = FALSE;
		duration = 5;
		message  = "Inappropriate Language (5 minute ejection)";
	
		// load from INI
		INI.Exchange(READ_FROM_INI, sect_name, "enabled",	enabled);
		INI.Exchange(READ_FROM_INI, sect_name, "duration",	duration);
		INI.Exchange(READ_FROM_INI, sect_name, "message",	message);
		return true;
	}

	bool GlobalFilterPolicy::Save(AW::EnhancedINI& INI)
	{
		const int WRITE_TO_INI = false;
		const char* sect_name = "filter_policy";

		INI.Exchange(WRITE_TO_INI, sect_name, "enabled",	enabled);
		INI.Exchange(WRITE_TO_INI, sect_name, "duration",	duration);
		INI.Exchange(WRITE_TO_INI, sect_name, "message",	message);
		return true;
	}

	bool FloodPreventionPolicy::Load(AW::EnhancedINI& INI)
	{
		const int READ_FROM_INI = true;
		const char* sect_name = "flood_prevention_policy";

		// configure defaults
		enabled			= TRUE;
		max_rate		= 0;
		max_repeat		= 0;
		auto_mute_on	= 20;
		auto_mute_len	= 30;

		// load from INI
		INI.Exchange(READ_FROM_INI, sect_name, "enabled",	enabled);
		INI.Exchange(READ_FROM_INI, sect_name, "max_rate",	max_rate);
		INI.Exchange(READ_FROM_INI, sect_name, "max_repeat",max_repeat);
		INI.Exchange(READ_FROM_INI, sect_name, "am_trig",	auto_mute_on);
		INI.Exchange(READ_FROM_INI, sect_name, "am_len",	auto_mute_len);
		return true;
	}

	bool FloodPreventionPolicy::Save(AW::EnhancedINI& INI)
	{
		const int WRITE_TO_INI = false;
		const char* sect_name = "flood_prevention_policy";

		INI.Exchange(WRITE_TO_INI, sect_name, "enabled",	enabled);
		INI.Exchange(WRITE_TO_INI, sect_name, "max_rate",	max_rate);
		INI.Exchange(WRITE_TO_INI, sect_name, "max_repeat",	max_repeat);
		INI.Exchange(WRITE_TO_INI, sect_name, "am_trig",	auto_mute_on);
		INI.Exchange(WRITE_TO_INI, sect_name, "am_len",		auto_mute_len);
		return true;
	}
}