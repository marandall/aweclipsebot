#pragma once
#include "ConsoleFilter.h"

// namespace inclusion of the enhanced INI
namespace AW
{
	class EnhancedINI;
}

namespace Security
{
	/**
		GlobalFilterPolicy
		---------------------------------
			- Maintains list of words to eject for etc
	*/
	class GlobalFilterPolicy
	{
	public:
		Comms::FilterList words;
		BOOL		enabled;
		int			duration;
		String		message;

	public:
		bool	Load(AW::EnhancedINI& INI);
		bool	Save(AW::EnhancedINI& INI);
	};

	// public access
	extern GlobalFilterPolicy g_FilterPolicy;
}