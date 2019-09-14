#include "stdafx.h"
#include "CIS_Dispatch.h"
#include "CIS_ConfigInterface.h"
#include "AWSDK_Support.h"
#include <gdiplus.h>

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

#define IMPL_FUNCTION(phrase, process)	else if (cmd == phrase) result = process(_Req, _Resp)

/************************************************************************
 * Dispatches the commands
 ***********************************************************************/
namespace CIS 
{	

	namespace Control 
	{
		/*
			Returns a string containing the text used for searching for avatars
		*/
		inline String GetBotSearch() {
			String respText; 
			respText.Format("%s ", bot.getName()); respText.MakeLower();
			return respText;
		}

	
	}
}