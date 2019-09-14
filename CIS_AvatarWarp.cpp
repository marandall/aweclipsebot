#include "stdafx.h"

#include "Validators.h"					// for double validator
#include "CIS_Interfaces.h"				// contains tracking classes
#include "CIS_basic.h"					// for eject namespace
#include "CIS_ConfigEjection.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

namespace CIS 
{
	namespace Exec 
	{			
		int AvTeleportProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("avteleport") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("could_not_find",		"I could not find that user");
					CIS_LANGUAGE("could_not_translate", "I could not determine the destinationc oordinates");
					CIS_LANGUAGE("default",				"I have teleported {$target.name}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			SplitString args(getFlagText(), ",", -1);

			// must have 2
			if (args.size() != 2)
				return Reason::InvalidArgument;

			// argument strings
			String argUser	= args[0]; argUser.Trim();
			String argTo 	= args[1]; argTo.Trim();
			bool argWarp	= getFlag("-warp");

			// convert to the coordinates and the like
			Location lcDest	= Location(argTo, pThisAvatar);
			Avatar* pTarget = avatars.GetByKey(argUser, pThisAvatar);
			bool use_warp	= argWarp;

			// check avatar exists
			if (pTarget == NULL)
			{
				write(cisLang["could_not_find"], cisVM);
				return Reason::BadArgument;
			}

			// check translation
			if (lcDest.IsValid() == false)
			{
				write(cisLang["could_not_translate"], cisVM);
				return Reason::BadArgument;
			}

			// teleport user
			pTarget->Teleport(lcDest, "", use_warp);
			cisVM["target"] = *pTarget;
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}

		int TakeMeProcessor::process()
		{
			/* variables section */
			String	logText, nameText;
			long		x, y, z, yaw;

			nameText = getFlagText();					// trim the text
			nameText.Trim();							// for best effect

			/* try and find the avatar */
			Avatar* pTarget = avatars.GetByKey(nameText, this_avatar);
			if (pTarget == NULL) {
				logText.Format("I cannot find '%s'", nameText);
				write(logText);
				return Reason::InvalidArgument;
			} 
			
			/* tell the user we are teleporting them */
			logText.Format("Teleporting you to %s", pTarget->getName());
			write(logText);

			/* tell avatar someones coming to em */
			if (getFlag("-silent") == false) {
				logText.Format("You are being bot-joined by %s, %s", getAvatar()->getName(), pTarget->getName());
				pTarget->WhisperEx(logText, false);
			}

			/* where are we going? */
			int join_distance = (600 * (getFlag("-behind") ? -1 : 1));

			/* do we need to cut down distance for -c flag */
			if (getFlag("-short")) 
				join_distance = join_distance / 2;
			x = (long)(pTarget->getX() + sin(pTarget->getrYaw()) * join_distance);
			z = (long)(pTarget->getZ() + cos(pTarget->getrYaw()) * join_distance);
			
			if (getFlag("-ep")) y = pTarget->getY() + 1000;
			else if (getFlag("-en")) y = pTarget->getY() - 1000;
			else y = pTarget->getY();

			/* do we join behind them */
			if (!getFlag("-behind"))
				yaw = (pTarget->getYAW() + 1800) % 3600;
			else
				yaw = pTarget->getYAW();

			/* teleport caller */
			getAvatar()->Teleport(x, y, z, yaw, false);
			return Reason::Success;
		}

		int BringMeProcessor::process()
		{
			// function header
			CIS_BEGIN_FUNCTION("BringMe") {
			} CIS_END_FUNCTION

			/* begin variable section header */
			String	logText, nameText;
			long		x, y, z, yaw;

			/* copy over final data */
			nameText = getFlagText(); nameText.Trim();
			Avatar& srcUser = *getAvatar();

			/* try and find the avatar */
			Avatar* pTarget = avatars.GetByKey(nameText);
			if (pTarget == NULL) {
				logText.Format("I cannot find %s", nameText);
				write(logText);
				return Reason::InvalidArgument;
			} 
		
			/* tell the user we are teleporting them */
			logText.Format("Bringing you %s", pTarget->getName());
			write(logText);

			/* tell avatar someones coming to em */
			if (!getFlag("-silent")) {
				logText.Format("You are being bot-invited by %s, %s", getName(), pTarget->getName());
				pTarget->WhisperEx(logText, false);
			}

			/* where are we going? */
			int join_distance = (600 * (getFlag("-behind") ? -1 : 1));			// difference one: position reversal

			/* do we need to cut down distance for -c flag */
			if (getFlag("-short"))
				join_distance = join_distance / 2;

			/* calculate positions relative to avatar */
			x = (long)(srcUser.getX() + sin(srcUser.getrYaw()) * join_distance);
			z = (long)(srcUser.getZ() + cos(srcUser.getrYaw()) * join_distance);
			
			if (getFlag("-ep"))		y = this_avatar->getY() + 1000;
			else if (getFlag("-en"))	y = this_avatar->getY() - 1000;
			else y = this_avatar->getY();

			/* do we join behind them */
			if (!getFlag("-behind"))								// difference two: angle reversal
				yaw = (srcUser.getYAW() + 1800) % 3600;
			else
				yaw = srcUser.getYAW();

			/* teleport caller */
			pTarget->Teleport(x, y, z, yaw, false);
			return Reason::Success;
		}
	}
}



