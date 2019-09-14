 /* the CIS systems - Contained within the CIS folder which is 1 level below */
/* the root level - therefore all incldues must be upclassed			    */

#include "stdafx.h"
#include "CIS_Interfaces.h"				// contains tracking classes
#include "CIS_Dispatch.h"				// for CIS::Dispatch
#include "CIS_Communications.h"			// communications output
#include "validators.h"
#include "colours.h"
#include "EclipseEvolutionDialog.h"
#include "RelayRelay.h"
#include "HeadsUp_UserPlugin.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

/************************************************************************
 * Say Process - Sends a standard message to everyone
 * Required Rights:		speak/say				
 ***********************************************************************/

namespace CIS 
{
	namespace Exec 
	{
		int SayProcessor::process() 
		{
			/* the string in CString format */
			bot.say(getFlagText());

			return Reason::Success;
		}

		///************************************************************************
		// * Whisper Process - Whispers a message to the target
		// * Required Rights:		speak/say
		// ***********************************************************************/
		int CMsgProcessor::process()
		{
			/* variables at the top for now */
			SplitString args;
			size_t argc = args.split(getFlagText(), ",", 2);

			/* deduce what format the mesage is */
			if (argc != 2) {
				write("Invalid arguments, format is: [colour], [message]");
				return Reason::InvalidArgument;
			}

			/* get arguments */
			CStringA	argColour = args[0]; argColour.Trim();
			CStringA	argText	  = args[1]; argText.Trim();
			COLORREF	clrStyle  = AW::Colour::LookupString(argColour);

			/* find rights server as a pointer */
			Avatar* pAvatar		= getAvatar();
			Avatar* pUser		= NULL;
			long cntUsers		= 0;
			bot.consoleBroadcast(argText, clrStyle, 
				AW::MakeConsoleStyle(getFlag("-b"), getFlag("-i"), true));

			/* post back to user */
			write("Console Sent...");
			return Reason::Success;
		}

		///************************************************************************
		// * Whisper Process - Whispers a message to the target
		// * Required Rights:		speak/say
		// ***********************************************************************/
		int WhisperProcessor::process() 
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("whisper") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("invalid", "Invalid arguments; format is: [target], [message]");
					CIS_LANGUAGE("not_found", "There is no user named {$target}");
					CIS_LANGUAGE("default", "Message sent");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------


			// split to arguments
			SplitString	args;
			size_t argc = args.split(getFlagText(), ",", 2);

			// 2 arguments are required
			if (argc != 2) 
			{
				write(cisLang["invalid"], cisVM);
				return Reason::InvalidArgument;
			}
			
			// target
			String argTarget = args[0]; argTarget.Trim();
			cisVM["target"] = (LPCSTR)argTarget;

			// user must exist
			Avatar* pTarget = avatars.GetByKey(argTarget, this_avatar);
			if (pTarget == NULL) 
			{
				write(cisLang["not_found"], cisVM);
				return Reason::BadArgument;
			}

			/* whisper */
			pTarget->WhisperEx(args[1], false);
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}

		///************************************************************************
		// * Sound Send Process - Sends an audio queue
		// * Required Rights:		Client Only
		// ***********************************************************************/
		int NoiseProcessor::process() 
		{
			/* variables used in function */
			CStringA	logText;
			Avatar*		pUser = NULL;
			SplitString		args(getFlagText(), ",", 2);

			/* deduce what format the mesage is */
			if (args.size() != 2) {
				write("Invalid arguments, format is: [target], [sound]");
				return Reason::InvalidArgument;
			}

			/* now we get the user and sound */
			CStringA argTarget = args[0]; argTarget.Trim();
			CStringA argSound  = args[1]; argSound.Trim();
			bool targetAll	   = false;

			/* select if we target everyone or one particular user */
			if (argTarget == "all()")	targetAll = true;
			else pUser = avatars.GetByKey(argTarget, NULL);

			/* response yes or no */
			if (pUser == NULL && targetAll == false) {
				logText.Format("Sorry, unable to send sound. Cannot find %s", argTarget);
				write(logText);
				return Reason::BadArgument;
			}

			if (targetAll == true) {
				int rc = bot.SendNoise(0, argSound);
				logText.Format("Broadcasting %s to all, rc %d", argSound, rc);
			} else {
				int rc = pUser->SendSound(argSound);
				logText.Format("Sending %s to %s, rc %d", argSound, argTarget, rc);
			}
			
			/* send out */
			write(logText);
			return Reason::Success;
		}

	}
}