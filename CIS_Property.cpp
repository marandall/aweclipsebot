#include "stdafx.h"
#include "CIS_Interfaces.h"				// contains tracking classes
#include "CIS_Property.h"				// for ejection / ejnt commands
#include "property_object.h"
#include "CallbackQueue.h"

namespace CIS 
{
	namespace Exec 
	{
		//namespace Property
		//{
		//	int Query5x5(const CIS::Request& _Req, CIS::Response& _Resp)
		//	{
		//		String logText;

		//		// check the query status
		//		if (bot.getQueryMode() != QUERY_MODE_NONE)
		//		{
		//			write("Unable to query; currently querying in mode %d");
		//			return Reason::InvalidCallState;
		//		}
		//		else
		//		{
		//			Location pos(getFlagText(), getAvatar());
		//			int rc = bot.Query5x5(pos.getCellX(), pos.getCellZ());
		//			if (rc == RC_SUCCESS)
		//			{
		//				logText.Format("Querying at %s", pos.getCellCoordinates());
		//			}
		//			else
		//			{
		//				logText.Format("Unable to query at %s; rc %d", 
		//					pos.getCellCoordinates(), rc);
		//			}
		//			write(logText);
		//		}

		//		return Reason::Success;
		//	}

		//	// AsyncHandler_Seed
		//	//
		//	//
		//	class AsyncHandler_Seed : public CallbackQueue::Target
		//	{
		//	public: // structures used
		//		CIS::Response	response;
		//		CIS::Request	request;

		//	public: // result
		//		virtual void handle_result(EclipseBot& Client, int Result)
		//		{
		//			// map to our usual variable names
		//			CIS::Response& _Resp = response;
		//			CIS::Request&  _Req  = request;
		//			String logText;

		//			// ------------------ function header ------------------
		//			CIS_BEGIN_FUNCTION("seed/response") {
		//				// default language
		//				CIS_BEGIN_LANGUAGE {
		//					CIS_LANGUAGE("failed", "Unable to seed the object; rc {$rc} ({$rc_string})");
		//					CIS_LANGUAGE("default", "Seed successful");
		//				} CIS_END_LANGUAGE

		//				// execute the function checks				
		//				CIS_BEGIN_CHECKS {
		//					// ...
		//				} CIS_END_CHECKS;
		//			} CIS_END_FUNCTION
		//			// ---------------- end function header ----------------

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

		//			// reply to seed
		//			if (Result == 0)
		//			{
		//				write(cisLang["default"], cisVM);
		//			}
		//			else
		//			{
		//				cisVM["rc"]			= Result;
		//				cisVM["rc_string"]	= AW::Bot::GetReasonCode(Result);
		//				write(cisLang["failed"], cisVM);
		//			}
		//		}
		//	};

		//	int Seed(const CIS::Request& _Req, CIS::Response& _Resp)
		//	{
		//		// ------------------ function header ------------------
		//		CIS_BEGIN_FUNCTION("seed") {
		//			// default language
		//			CIS_BEGIN_LANGUAGE {
		//				CIS_LANGUAGE("failed", "There was an error seeding");
		//				CIS_LANGUAGE("default", "Attempting to seed {$object.model} at {$object.coords}...");
		//			} CIS_END_LANGUAGE
		//		} CIS_END_FUNCTION
		//		// ---------------- end function header ----------------

		//		// coordinates are based on either the calling avatars or the bots
		//		Location loc;
		//		loc.from_location(getThisPosition());

		//		// arguments
		//		// @ model
		//		// @ action
		//		// @ description
		//		String argModel	= _Req["model"];
		//		String argAction	= _Req["action"];
		//		String argDescr	= _Req["descr"];

		//		// is the command a prefix?
		//		if (getExists("model") == false)
		//			argModel = getFlagText();

		//		// ensure no rotations
		//		loc.setYaw(0);
		//		loc.setTilt(0);
		//		loc.setRoll(0);

		//		// create the model object
		//		ObjectData obj;
		//		obj.move(loc);
		//		obj.model		= argModel;
		//		obj.action		= argAction;
		//		obj.description	= argDescr;

		//		// seed at the given position
		//		AsyncHandler_Seed* pSeed = new AsyncHandler_Seed();
		//		pSeed->request	= _Req;
		//		pSeed->response	= _Resp;

		//		// send the notice
		//		cisVM["object"] = obj;
		//		write(cisLang["default"], cisVM);

		//		// post
		//		bot.object_add(obj, pSeed);
		//		return Reason::Success;
		//	}
		//}
	}
}
	