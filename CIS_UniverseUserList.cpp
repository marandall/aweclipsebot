#include "stdafx.h"
#include "CIS_Interfaces.h"				// contains tracking classes
#include "CIS_UniverseUserList.h"
#include "UniverseUserList.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

namespace CIS 
{
	namespace Exec 
	{
		//namespace UUL
		//{
		//	int InWorld(const CIS::Request& _Req, CIS::Response& _Resp)
		//	{
		//		// ------------------ function header ------------------
		//		CIS_BEGIN_FUNCTION("uul/inworld") {
		//			// default language
		//			CIS_BEGIN_LANGUAGE {
		//				CIS_LANGUAGE("default", "System detects the following users: {$found.as_and}");
		//			} CIS_END_LANGUAGE

		//			// execute the function checks				
		//			CIS_BEGIN_CHECKS {
		//				CIS_CHECK_RIGHT("system/administrator")
		//			} CIS_END_CHECKS;
		//		} CIS_END_FUNCTION
		//		// ---------------- end function header ----------------

		//		// universe user list mapping
		//		UniverseUserList::List& uul = bot.getUniverseUsers();
		//		UniverseUserList::SearchResults sr = uul.getUsersInWorld(getFlagText());
		//		KnownList found;

		//		// map each
		//		for (size_t i = 0; i < sr.size(); i++)
		//			found.push_back(sr[i]->getName());

		//		// display to user
		//		cisVM["found"] = found;
		//		write(cisLang["default"], cisVM);
		//		return Reason::Success;
		//	}

		//	int RipEmails(const CIS::Request& _Req, CIS::Response& _Resp)
		//	{
		//		// ------------------ function header ------------------
		//		CIS_BEGIN_FUNCTION("uul/rip-email") {
		//			// default language
		//			CIS_BEGIN_LANGUAGE {
		//				CIS_LANGUAGE("default", "The following emails are known:");
		//				CIS_LANGUAGE("lineout", "{$source.name} = {$source.email}");
		//			} CIS_END_LANGUAGE

		//			// execute the function checks				
		//			CIS_BEGIN_CHECKS {
		//				CIS_CHECK_RIGHT("system/administrator")
		//				CIS_CHECK_CLIENT_ONLY;
		//			} CIS_END_CHECKS;
		//		} CIS_END_FUNCTION
		//		// ---------------- end function header ----------------

		//		// universe user list mapping
		//		using namespace UniverseUserList;
		//		List& uul = bot.getUniverseUsers();

		//		// details
		//		write(cisLang["default"], cisVM);

		//		// only email listed
		//		for (List::iterator ptr = uul.begin(); ptr != uul.end(); ptr++)
		//		{
		//			if (ptr->getEmail().GetLength() != 0)
		//			{
		//				String temp;
		//				temp.Format("%s = %s", ptr->getName(), ptr->getEmail());
		//				cis_respond(temp);
		//			}
		//		}
		//		return Reason::Success;
		//	}
		//	
		//	String ToStat(String Type, int count, int total)
		//	{
		//		String temp;
		//		temp.Format("%d %s (%.1f%%)", count, Type, 100.0F / total * count);
		//		return temp;
		//	}

		//	int Ratios(const CIS::Request& _Req, CIS::Response& _Resp)
		//	{
		//		// ------------------ function header ------------------
		//		CIS_BEGIN_FUNCTION("uul/ratios") {
		//			// default language
		//			CIS_BEGIN_LANGUAGE {
		//				CIS_LANGUAGE("default", "The universe contains the following: {$ratios.as_and}");
		//				CIS_LANGUAGE("lineout", "{$source.name} = {$source.email}");
		//			} CIS_END_LANGUAGE

		//			// execute the function checks				
		//			CIS_BEGIN_CHECKS {
		//				CIS_CHECK_RIGHT("gcs/info")
		//				CIS_CHECK_CLIENT_ONLY;
		//			} CIS_END_CHECKS;
		//		} CIS_END_FUNCTION
		//		// ---------------- end function header ----------------

		//		// universe user list mapping
		//		using namespace UniverseUserList;
		//		List& uul = bot.getUniverseUsers();

		//		// statistics
		//		int tourists = 0;
		//		int citizens = 0;
		//		int bots	 = 0;
		//		int invites	 = 0;
		//		int total	 = (int)uul.size();
		//		KnownList stats;

		//		// tally
		//		for (List::iterator ptr = uul.begin(); ptr != uul.end(); ptr++)
		//		{
		//			if (ptr->getInvite())
		//				invites++;
		//			else
		//			{
		//				if (ptr->getIsTourist())
		//					tourists++;
		//				else if (ptr->getIsCitizen())
		//					citizens++;
		//				else if (ptr->getIsBot())
		//					bots++;
		//			}
		//		}

		//		// results
		//		if (tourists) stats.push_back(ToStat("tourists", tourists, total));
		//		if (citizens) stats.push_back(ToStat("citizens", citizens, total));
		//		if (bots)     stats.push_back(ToStat("bots", bots, total));

		//		// result
		//		cisVM["ratios"] = stats;
		//		write(cisLang["default"], cisVM);
		//		return Reason::Success;
		//	}
		//}
	}
}