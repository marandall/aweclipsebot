#include "stdafx.h"				// for precompiled header
#include "EclipseBot.h"			// for EclipseBot
#include "Resource.h"			// for the root menu
#include "UniverseUserList.h"
#include "fileloader.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

void EclipseBot::onEventUserInfo(void) 
{
	getUserList().onUserInfo(this);
}

void EclipseBot::onCallbackUserList(int rc) 
{
	if ( _bool(AW_USERLIST_MORE) )
	{
		_user_list();
	}
}