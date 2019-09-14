/* the CIS systems - Contained within the CIS folder which is 1 level below */
/* the root level - therefore all incldues must be upclassed			    */

#include "stdafx.h"
#include "CIS_Interfaces.h"				// contains tracking classes
#include "CitizenLookup_Kernel.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

namespace CIS 
{
	namespace Exec 
	{
		//namespace Universe
		//{
		//	class LookupByNumberClass : public CitizenLookup::LookupBase
		//	{
		//	public: // structures used
		//		CIS::Response	response;
		//		CIS::Request	request;
		//		bool			no_url;

		//	public:
		//		LookupByNumberClass()
		//		{
		//			no_url = false;
		//		}

		//	public:
		//		void Result(AW::Citizen& Info, int Reason)
		//		{
		//			// map to our usual variable names
		//			CIS::Response& _Resp = response;
		//			CIS::Request&  _Req  = request;
		//			String logText;

		//			// if it is an avatar it must still exist
		//			if (pTarget)
		//			{
		//				bool b_found = false;
		//				for (size_t i = 0; i < avatars.size(); i++)
		//					if (&avatars[i] == pTarget)
		//					{
		//						b_found = true;
		//						continue;
		//					}

		//				// if it isnt found then dont do anything
		//				if (b_found == false)
		//					return;
		//			}

		//			// name problem
		//			if (Reason == 0)
		//			{
		//				logText.Format("Citizen #%d is %s", Info.getCitizen(), Info.getName());
		//				if (Info.getUrl().GetLength() && no_url == false)
		//					logText.AppendFormat(" (url: %s)", Info.getUrl());
		//			}
		//			else
		//			{
		//				logText.Format("Unable to look up citizen #%d; error %d (%s)", number, Reason, AW::Bot::GetReasonCode(Reason));
		//			}

		//			// respond
		//			write(logText);
		//		}
		//	};

		//	class LookupByNameClass : public CitizenLookup::LookupBase
		//	{
		//	public: // structures used
		//		CIS::Response	response;
		//		CIS::Request	request;
		//		bool			no_url;

		//	public:
		//		void Result(AW::Citizen& Info, int Reason)
		//		{
		//			// map to our usual variable names
		//			CIS::Response& _Resp = response;
		//			CIS::Request&  _Req  = request;
		//			String logText;

		//			// if it is an avatar it must still exist
		//			if (pTarget)
		//			{
		//				bool b_found = false;
		//				for (size_t i = 0; i < avatars.size(); i++)
		//					if (&avatars[i] == pTarget)
		//					{
		//						b_found = true;
		//						continue;
		//					}

		//				// if it isnt found then dont do anything
		//				if (b_found == false)
		//					return;
		//			}

		//			// name problem
		//			if (Reason == 0)
		//			{
		//				logText.Format("Citizen %s is #%d", Info.getName(), Info.getCitizen());
		//				if (Info.getUrl().GetLength() && no_url == false)
		//					logText.AppendFormat(" (url: %s)", Info.getUrl());
		//			}
		//			else
		//			{
		//				logText.Format("Unable to look up %s error %d (%s)", name, Reason, AW::Bot::GetReasonCode(Reason));
		//			}

		//			// respond
		//			write(logText);
		//		}
		//	};



		//	int LookupName(const CIS::Request& _Req, CIS::Response& _Resp) 
		//	{
		//		// ------------------ function header ------------------
		//		CIS_BEGIN_FUNCTION("citname") {
		//			// default language
		//			CIS_BEGIN_LANGUAGE {
		//				CIS_LANGUAGE("lookup_success", "Looking up citizen numbers {$success.as_and}");
		//				CIS_LANGUAGE("lookup_skipped", "Skipped lookup on {$failed.as_and}");
		//			} CIS_END_LANGUAGE

		//			// execute the function checks				
		//			CIS_BEGIN_CHECKS {
		//				CIS_CHECK_RIGHT("gcs/info")
		//				CIS_CHECK_UNIVERSE_MACRO
		//			} CIS_END_CHECKS;
		//		} CIS_END_FUNCTION
		//		// ---------------- end function header ----------------


		//		// known lists
		//		KnownList good, bad;

		//		// citizen number
		//		SplitString str(getFlagText(), ",", -1);
		//		for (size_t i = 0; i < str.size(); i++)
		//		{
		//			// must be a citizen number
		//			String argNum = str[i];
		//			argNum.Trim();

		//			// invalid
		//			int lookup = atoi(argNum);
		//			if (lookup <= 0)
		//			{
		//				bad.push_back(argNum);
		//				continue;
		//			}

		//			// citnum number,number,number
		//			LookupByNumberClass* pLookup = new LookupByNumberClass();
		//			pLookup->request	= _Req;
		//			pLookup->response	= _Resp;
		//			pLookup->no_url		= getFlag("-nourl");

		//			// post to lookup
		//			if (CitizenLookup::g_Kernel.lookup_by_number(lookup, pLookup) == 0)
		//			{
		//				good.push_back(ToString(lookup));
		//			}
		//			else
		//			{
		//				bad.push_back(ToString(lookup));
		//			}
		//		}

		//		// add to cisVM
		//		cisVM["success"] = good;
		//		cisVM["failed"]  = bad;

		//		// respond
		//		if (good.size()) 
		//			write(cisLang["lookup_success"], cisVM); 
		//		if (bad.size())
		//			write(cisLang["lookup_failed"], cisVM);   
		//		
		//		// good
		//		return Reason::Success;
		//	}

		//	
		//	int LookupNumber(const CIS::Request& _Req, CIS::Response& _Resp) 
		//	{
		//		// ------------------ function header ------------------
		//		CIS_BEGIN_FUNCTION("") {
		//			// default language
		//			CIS_BEGIN_LANGUAGE {
		//				CIS_LANGUAGE("", "");
		//				CIS_LANGUAGE("", "");
		//				CIS_LANGUAGE("", "");
		//				CIS_LANGUAGE("", "");
		//			} CIS_END_LANGUAGE

		//			// execute the function checks				
		//			CIS_BEGIN_CHECKS {
		//				CIS_CHECK_RIGHT("gcs/info")
		//				CIS_CHECK_UNIVERSE_MACRO
		//			} CIS_END_CHECKS;
		//		} CIS_END_FUNCTION
		//		// ---------------- end function header ----------------


		//		CIS_CHECK_RIGHT("gcs/info");

		//		// known lists
		//		KnownList good, bad;

		//		// citizen number
		//		SplitString str(getFlagText(), ",", -1);
		//		for (size_t i = 0; i < str.size(); i++)
		//		{
		//			// must be a citizen number
		//			String argName = str[i];
		//			argName.Trim();

		//			// invalid
		//			if (argName.IsEmpty())
		//			{
		//				bad.push_back(argName);
		//				continue;
		//			}

		//			// citnum number,number,number
		//			LookupByNameClass* pLookup = new LookupByNameClass();
		//			pLookup->request	= _Req;
		//			pLookup->response	= _Resp;
		//			pLookup->no_url		= getFlag("-nourl");

		//			// post to lookup
		//			if (CitizenLookup::g_Kernel.lookup_by_name(argName, pLookup) == 0)
		//			{
		//				good.push_back(argName);
		//			}
		//			else
		//			{
		//				bad.push_back(argName);
		//			}
		//		}

		//		// respond
		//		String logText;
		//		if (good.size()) { logText.Format("Looking up citizen name %s. ", good.toString("and")); }
		//		if (bad.size())  { logText.AppendFormat("Skipping lookups on %s. ", bad.toString("and"));   }
		//		write(logText);

		//		return Reason::NotImplemented;
		//	}

		//}
	}
}