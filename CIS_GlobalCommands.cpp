#include "stdafx.h"

#include "Validators.h"					// for double validator
#include "CIS_Interfaces.h"				// contains tracking classes
#include "DialogCore.h"					// for global command provider
#include "CIS_ConfigInterface.h"
#include "UserProfile_FreeJoin.h"
#include "cis_basic.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

namespace CIS 
{
	namespace Exec 
	{
		int FreeJoinProcessor::process() 
		{
			// check if the text is not empty 
			if (getFlagText().GetLength() != 0) 
			{
				// argumnet
				CStringA text = getFlagText();

				// turning freejoin on
				if (text == "on") 
				{
					write("Free Join has been enabled for this session");
					getAvatar()->getFreeJoin().setEnabled(B_TRUE);
				} 
				
				// turning freejoin off
				else if (text == "off") 
				{
					write("Free Join has been disabled for this session");
					getAvatar()->getFreeJoin().setEnabled(B_FALSE);
				} 
				
				else if (text == "list") 
				{
					KnownList klUsers;
					FOR_EACH_OF_AVATARS
						if (User.getFreeJoin().getEnabled() == B_TRUE)
						{
							klUsers.push_back(User.getName());
						}
					END_FOR_EACH

					/* tell the person */
					temp.Format("There are currently %d users with Free-Join enabled:", (int)klUsers.size());
					write(temp);
					temp.Format(">   %s", klUsers.toString("and"));
					write(temp);
					return Reason::Success;
				}
			}

			// check for silent flat
			if (getFlag("-silent")) 
			{
				write("Free Join is now in silent mode. You will no longer be notified when people join you for this session. To re-enable notifications say ^freejoin -notify");
				getAvatar()->getFreeJoin().setSilent(B_TRUE);
			}

			// notify the users
			else if (getFlag("-notify")) 
			{
				write("Free Join is now in notify mode. You will be notified when people join");
				getAvatar()->getFreeJoin().setSilent(B_FALSE);
			}
			return Reason::Success;
		}

		int JoinProcessor::process()
		{
			/* attempt to find the name of the avatar */
			if (getFlagText().GetLength() == 0) {
				write("You must provide a user id");
				return Reason::InvalidArgument;
			}

			/* attempt to find the user using local lookup */
			Avatar* pTarget = avatars.GetByKey(getFlagText(), NULL); 
			if (pTarget == NULL) 
			{
				write("Avatar could not be found or does not have Free-Join enabled");
				return Reason::BadArgument;
			} 
			else if ((pTarget->getFreeJoin().getEnabled() != B_TRUE)) 
			{
				write("Avatar could not be found or does not have Free-Join enabled");
				return Reason::BadArgument;
			}

			/* teleport to this person */
			int join_distance = 400;
			int x = (long)(pTarget->getX() + sin(pTarget->getrYaw()) * join_distance);
			int z = (long)(pTarget->getZ() + cos(pTarget->getrYaw()) * join_distance);
			getAvatar()->Teleport(x, pTarget->getY(), z, (pTarget->getYAW() + 1800) % 3600, "", false);

			/* is the target user in a silent join mode? */
			CStringA logText;
			
			if (pTarget->getFreeJoin().getSilent() == B_FALSE) 
			{
				logText.Format("You are being joined by %s (citizen: %d) using free-join. To disable this feature say ^freejoin off",
					getAvatar()->getName(), getAvatar()->getCitizen());
				pTarget->ConsoleWhisper(logText, CIS::Config::g_Interface.getOutputColour().getColour(), CM_ITALIC);
			}

			/* send log to target */
			logText.Format("You are being teleported to %s", (LPCSTR)pTarget->getName());
			write(logText);

			/* anyone can join a user that has free join enabled */
			return Reason::Success;
		}

		int ThrustProcessor::process()
		{
			/* move the target forward so far */
			Avatar* pTarget = getAvatar();
			int join_distance = (int)(atof(getFlagText()) * 100);
			int x = (long)(pTarget->getX() + sin(pTarget->getrYaw()) * join_distance);
			int z = (long)(pTarget->getZ() + cos(pTarget->getrYaw()) * join_distance);
			AW::Location lcDest(x, pTarget->getY(), z, pTarget->getYAW());
			
			/* notify them of the warp */
			CStringA logText;
			logText.Format("Warping you %.1f meters", (float)(join_distance / 100.0));
			write(logText);

			/* do teleport */
			pTarget->Teleport(lcDest, "", true);
			return Reason::Success;
		}
		
		int CoordsProcessor::process()
		{
			/* return text */
			String text;
			text.Format("You are located at %s", getAvatar()->getCoordinates());
			write(text);
			return Reason::Success;
		}
	}
}