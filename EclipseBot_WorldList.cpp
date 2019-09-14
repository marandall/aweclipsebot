#include "stdafx.h"					// for precompiled header
#include "EclipseBot.h"				// for EclipseBot
#include "worldlist.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

void EclipseBot::onCallbackWorldList(int ReasonCode)
{
	getWorldList().handle_callback(ReasonCode);
}

void EclipseBot::onEventWorldInfo(void)
{
	getWorldList().handle_world_info();
}
