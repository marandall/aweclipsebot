#pragma once

namespace Security
{
	class FloodPreventionPolicy
	{
	public:
		BOOL	enabled;
		int		max_rate;		// maximum per 5 seconds
		int		max_repeat;		// maximum of same phrase
		int		auto_mute_on;	// maximum per 5 before auto-mute
		int		auto_mute_len;	// how long is the mute in effect

	public:
		bool	Load(AW::EnhancedINI& INI);
		bool	Save(AW::EnhancedINI& INI);
	};

	extern FloodPreventionPolicy g_FloodPolicy;
}