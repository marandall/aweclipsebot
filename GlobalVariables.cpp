#include "stdafx.h"
#include "GlobalVariables.h"
#include "ConsoleFilter.h"

EclipseBot	bot;								// main bot for entire application
Avatars		avatars;							// main avatar collection for entire application

/* console and chat filters */
Comms::ConsoleFilter	Comms::g_ConsoleFilter;
Comms::ChatFilter		Comms::g_ChatFilter;