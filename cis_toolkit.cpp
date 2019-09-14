#include "stdafx.h"
#include "cis_interfaces.h"				// contains tracking classes
#include "cis_toolkit.h"
#include "cis_delayedlookup.h"
#include "dynamicrights.h"
#include "cis_dispatch.h"
#include "scripting_host.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

namespace CIS 
{
	namespace Exec 
	{
		int ExecProcessor::process()
		{
			// split it into parts
			SplitString s(getFlagText(), " ", 2);
			String s1 = s[0];
			String s2 = s.size() == 2 ? s[1] : "";

			// scripting host
			ScriptingHost::getInst().doFunction(s1, 
				&Umbra::VariantArray(0, (const char*)s2), 0, &bot);

			// good
			return Reason::Success;
		}

		int ToolkitSendUrlProcessor::process()
		{
			// parameters required
			// @name
			// @url
			// @target3d
			// @silence 

			CStringA name   = getParam("name", "");
			CStringA url    = getParam("url", "");
			bool	 t3d    = getParam("target3d", "0") == "1";
			bool	 silent = getParam("silent", "0") == "1";
			CStringA post	= getParam("postdata");

			// lookup user
			Avatar* pTarget = 0;
			if ((pTarget = avatars.GetByKey(name)) == 0) {
				write("The user could not be found");
				return Reason::BadArgument;
			}

			// send url
			pTarget->SendURL(url, "", t3d, post);

			// do we have a good send?
			if (silent == false) {
				write(CStringA("URL sent to ") + pTarget->getName());
			}

			// good
			return Reason::Success;
		}

		int ToolkitImpersonateProcessor::process()
		{
			SplitString args(getFlagText(), ",", 2);

			// lookup avatar
			if (getFlag("-pmatch"))
			{
				int count = 0;
				for (size_t i =0; i < avatars.size(); i++)
				{
					if (avatars[i].IsPattern(args[0]))
					{
						CIS::CommandInterpreter::beginAvatar(avatars[i], args[1], false, AW_CHAT_SAID);
						count++;
					}
				}

				// format output
				temp.Format("Impersonating command to %d users...", count);
				write(temp);
			}
			else
			{
				Avatar* pImp = avatars.GetByKey(args[0]);
				if (pImp == 0)
				{
					write("That user cannot be found");
					return Reason::BadArgument;
				}

				// execute against runtime
				int rc = CIS::CommandInterpreter::beginAvatar(*pImp, args[1], false, AW_CHAT_SAID);
				temp.Format("Impersonating %s for command '%s' returned %d", pImp->getName(), args[1], rc);
				write(temp);
			}

			return Reason::Success;
		}


		int SystemDrsCreateAccountProcessor::process()
		{
			// lookup generator
			CitizenGroupLookupCompleter* pQuery = new CitizenGroupLookupCompleter(this, &CitizenLookup::g_Kernel);
			pQuery->preserveChildren();
			pQuery->open();

			// add each element
			String lookupName = getParam("bycit", "unspecified");
			pQuery->lookupByName(lookupName);

			// close each element
			if (pQuery->getOutstanding() == 0)
			{
				pQuery->close();
				return Reason::Success;
			}
			else
			{
				write(String("Looking up citizen ") + lookupName);
				pQuery->deleteChildren();
				return Reason::CompletionDelayed;
			}
		}

		int	SystemDrsCreateAccountProcessor::resume()
		{
			// uses drs
			using namespace DynamicRights;
			DynamicRights::Provider& drsProvider = g_Provider;

			// get the first and only citizen lookup
			CIS::CitizenGroupLookupCompleter* pLookupResults = dynamic_cast<CitizenGroupLookupCompleter*>(resumption_args["results"].toRemoteClass());
			CitizenLookup::GroupLookupHelper& u = (*pLookupResults)[0];

			// is it a citizen?
			if (u.result != 0)
			{
				temp.Format("Unable to create a drs profile for '%s', citizen lookup error %d.", u.name, u.result);
				write(temp);
			}
			else
			{
				// does the citizen number already exist?
				temp.Format("#%d", u.info.getNumber());
				if (drsProvider.getUser(temp) != 0)
				{
					temp.Format("The citizen lookup resolved to #%d however that account already exists.", u.info.getNumber());
					write(temp);
				}
				else
				{
					// create the user
					User drsUser;

					// are we cloning
					String cloneFrom = getParam("clonefrom");
					if (cloneFrom.GetLength() != 0)
					{
						User* pCloneSrc = drsProvider.getUser(cloneFrom);
						if (pCloneSrc == 0)
						{
							write("The account attempted to clone from does not exist");
						}
						else
						{
							drsUser = *pCloneSrc;
						}
					}

					// set a name and id
					drsUser.user_id	= temp;
					drsUser.full_id		= getParam("name", u.info.getName());
					drsUser.description	= getParam("description", drsUser.description);
					drsProvider.addUser(drsUser);

					// ok
					temp.Format("Created new DRS account for %s (%s) cloned via %s", 
						drsUser.user_id, drsUser.full_id, cloneFrom.GetLength() ? cloneFrom : "[new_user]");
					write(temp);
				}
			}

			// its all ok
			return Reason::Success;
		}

		int SystemDrsCreateRightProcessor::process()
		{
			// uses drs
			using namespace DynamicRights;
			DynamicRights::Provider& drsProvider = g_Provider;

			// does the right exist
			String right = getFlagText();
			if (drsProvider.RightExists(right) == true)
			{
				write("That right already exists in the known rights list");
			}
			else
			{
				drsProvider.known_rights.Insert(right);
				
				// use temp
				temp.Format("Added %s to the known rights list", right);
				write(temp);
			}

			// good
			return Reason::Success;
		}
	} 
}