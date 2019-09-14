#include "stdafx.h"

#include "Validators.h"	
#include "CIS_Interfaces.h"				// contains tracking classes
#include "CIS_basic.h"				// for beacon tracking
#include "Beacons.h"					// for beacon code
#include "CIS_ConfigJumpPoints.h"		// jump point configs
#include "scripting_host.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

using namespace Beacons;
#define BUBBLESORT_VECTOR(_arr_, _type_, _method_)					\
   for (size_t pass = 1; pass < _arr_.size(); pass++) {				\
       for (size_t i = 0; i < _arr_.size() - pass; i++) {			\
           if (_arr_[i]_method_ > _arr_[i+1]_method_) {				\
               _type_ temp	= _arr_[i];								\
			   _arr_[i]		= _arr_[i+1];							\
			   _arr_[i+1]	= temp;									\
           }														\
       }															\
   }

namespace CIS 
{
	namespace Exec 
	{
		int JumpCreateProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("beacon/create") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("citizens_only", "Only citizens can create jump points, and you are not one");
					CIS_LANGUAGE("limit_reached", "You cannot create any more jump points. Your limit is {$limit} and you have used {$limit_used}");
					CIS_LANGUAGE("proximity", "There is already an existing jump point {$distance} away ({$jump.name}) - jump points must be at least {$_config.seperate_by_at_least} meters appart");
					CIS_LANGUAGE("no_name", "You must provide a jump point name");
					CIS_LANGUAGE("name_conflict", "Your jump point cannot be created due to a naming conflict");
					CIS_LANGUAGE("default", "Your jump point has been created");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			
			// only citizens are allowed
			if (pThisAvatar->IsCitizen() == false)
			{
				write(cisLang["citizen_only"], cisVM);
				return Reason::InvalidCallState;
			}

			// new reference to configuration kernel
			Beacons::Config& cfg = Beacons::Kernel::getInst().config;
			Beacons::Kernel& g_Beacons = Beacons::Kernel::getInst();

			// if there is a limit to # of jump points
			if (cfg.getMaxPerUser() != 0 && getAvatar())
			{
				int beacon_count = g_Beacons.GetUserBeaconCount(getAvatar()->getCitizen());
				if (beacon_count > cfg.getMaxPerUser() && getRight("system/administrator"))
				{
					// report
					cisVM["limit"] = cfg.getMaxPerUser();
					cisVM["limit_used"] = beacon_count;
					write(cisLang["limit_reached"], cisVM);
					return Reason::Success;
				}
			}

			// do we have a proximity limit
			if (cfg.getSeperateByAtLeast() != 0)
			{
				AW::Location lcPoint = *dynamic_cast<AW::Location*>(getAvatar());
				Beacon* pNear = g_Beacons.FirstFirstCloserThan(lcPoint, cfg.getSeperateByAtLeast());
				if (pNear)
				{
					// report
					cisVM["jump"] = *pNear;
					cisVM["distance"] = static_cast<float>(pNear->distanceTo(lcPoint) / 100.0F);
					cisVM["_config"] = cfg;
					write(cisLang["proximity"], cisVM);

					// bad
					return Reason::ArgumentBeyondRange;
				}
			}

			// filter down the strings 
			CStringA nameText = getFlagText();
			nameText.Trim();
			if (nameText.GetLength() == 0) 
			{
				write(cisLang["no_name"], cisVM);
				return Reason::BadArgument;
			}


			if (g_Beacons.Insert(pThisAvatar->getCitizen(), nameText, pThisAvatar->getCoordinates()) == false)
			{
				write(cisLang["name_conflict"], cisVM);
				return Reason::BadArgument;
			} 
			else 
			{
				write(cisLang["default"], cisVM);
				return Reason::Success;
			}
		}
		
		int JumpProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("jump") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("none_defined", "There are no jump points yet defined");
					CIS_LANGUAGE("not_found", "That jump point could not be found");
					CIS_LANGUAGE("info", "The jump point {$jump.name} (owned by {$jump.owner}) is located at {$jump.coordinates}");
					CIS_LANGUAGE("default", "Jumpgate Activated to {$jump.name}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// new reference to configuration kernel
			Beacons::Config& cfg = Beacons::Kernel::getInst().config;
			Beacons::Kernel& g_Beacons = Beacons::Kernel::getInst();

			// no points defined
			if (g_Beacons.size() == 0) 
			{
				// report
				write(cisLang["none_defined"], cisVM);
				return Reason::Success;
			}

			// detect jump point
			::Beacons::Beacon* pBeacon = NULL;

			// handle beacon special cases
			if (getFlagText() == "random()") 
				pBeacon = &g_Beacons[aw_random() % g_Beacons.size()];
			else 
				pBeacon = g_Beacons.Search(getFlagText());
			
			// test if it has been found
			if (!pBeacon) 
			{
				// report
				write(cisLang["not_found"], cisVM);
				return Reason::BadArgument;
			}
			else 
			{
				cisVM["jump"] = *pBeacon;

				/* check if a flag is given */
				if (getFlag("-info") == true)
					write(cisLang["info"], cisVM);
				else 
				{
					// coordinates to
					AW::Location pos;

					// scripting host
					Umbra::Variant res = ScriptingHost::getInst().doFunction("EventAvatarJump", &Umbra::VariantArray(*this_avatar, *pBeacon), 0, &bot);
					if (res.IsRemote() == true)
					{
						AW::Location* pLoc = dynamic_cast<AW::Location*>(res.toRemoteClass());
						if (pLoc)
						{
							pos = *pLoc;
						}
						else
						{
							pos = pBeacon->getThisLocation();
						}
					}
					else
					{
						pos = pBeacon->getThisLocation();
					}
					
					// report
					write(cisLang["default"], cisVM);
					getAvatar()->Teleport(pos, "", getFlag("-w"));
				}
				return Reason::Success;
			}
		}
		
		int JumpTakePossessionProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("jump/take-possession") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("not_found", "Unable to locate the {$target} jump point");
					CIS_LANGUAGE("default", "The {$jump.name} jump point has been transfered to your ownership");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// new reference to configuration kernel
			Beacons::Config& cfg = Beacons::Kernel::getInst().config;
			Beacons::Kernel& g_Beacons = Beacons::Kernel::getInst();

			// target
			String argTarget = getFlagText();
			argTarget.Trim();

			// update vm
			cisVM["target"] = (LPCSTR)argTarget;
			
			// this allows a manager to take aquisition of a jump point
			Beacon* pBeacon = g_Beacons.Search(argTarget);

			// do we have the beacon
			if (pBeacon == NULL)
			{
				write(cisLang["not_found"], cisVM);
				return Reason::BadArgument;
			}
			else
			{
				cisVM["jump"] = *pBeacon;
			}

			// take possession
			pBeacon->owner = getAvatar()->getCitizen();
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}

		int JumpRelocateProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("jump/relocate") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("not_found", "Unable to locate the {$target} jump point");
					CIS_LANGUAGE("not_owner", "The '{$jump.name}' gate belongs to {$jump.owner} - You must have jumpgate/manage rights to reposition it");
					CIS_LANGUAGE("default", "The '{$jump.name}' jump point has been relocated");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// new reference to configuration kernel
			Beacons::Config& cfg = Beacons::Kernel::getInst().config;
			Beacons::Kernel& g_Beacons = Beacons::Kernel::getInst();

			// this allows a manager to take aquisition of a jump point
			Beacon* pBeacon = g_Beacons.Search(getFlagText());
			String logText;

			// do we have the beacon
			if (pBeacon == NULL)
			{
				write(cisLang["not_found"], cisVM);
				return Reason::BadArgument;
			}

			// assign to cisvm
			cisVM["jump"] = *pBeacon;

			// is it one of ours?
			if (pBeacon->getOwner() != getAvatar()->getCitizen() && getRight("jumpgate/manage") == false)
			{
				write(cisLang["not_owner"], cisVM);
				return Reason::AccessDenied;
			}

			// take possession
			pBeacon->Update(pBeacon->getOwner(), pBeacon->getName(), getAvatar()->getCoordinates());
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}

		int JumpDeleteProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("jump/delete") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("citizens_only", "Sorry, this command is only available to citizens");
					CIS_LANGUAGE("must_have_manage", "Sorry, you must have jumpgate manage rights in order to use mass delete functions");
					CIS_LANGUAGE("total_deleted", "{$total_deleted} jump points have been deleted");
					CIS_LANGUAGE("not_found", "That jump point could not be found");
					CIS_LANGUAGE("not_owner", "Sorry, you must be this jump points owner in order to delete it");
					CIS_LANGUAGE("default", "Jump point has been deleted");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// new reference to configuration kernel
			Beacons::Config& cfg = Beacons::Kernel::getInst().config;
			Beacons::Kernel& g_Beacons = Beacons::Kernel::getInst();
		
			/* active cit if possible*/
			int active_cit = 0;
			CStringA logText;
			if (isAvatar() == true)
			{
				// must be citizen
				if (pThisAvatar->IsCitizen() == false) 
				{
					write(cisLang["citzens_only"], cisVM);
					return Reason::InvalidCallState;
				}

				// set active cit as citizen, rather than active cit as -1
				active_cit = pThisAvatar->getCitizen();
			} 
			else 
			{
				active_cit = -1;
			}
		
			// proximity or range erasure
			String argCitizen = getParam("citizen", "");
			String argRange   = getParam("radius",  "");

			if (argCitizen.IsEmpty() == false || argRange.IsEmpty() == false) 
			{
				// jumpgate manage only
				if (getRight("jumpgate/manage") == false) 
				{
					write(cisLang["must_have_manage"], cisVM);
					return Reason::AccessDenied;
				}
				
				// proximity range
				int range   = (int)(argRange == "world()" ? -1 : abs(atof(argRange) * 100.0F));
				int citizen = argCitizen == "any()" ? -1 : abs(atoi(argCitizen));
				int count   = 0;
				
				// go through the proxim list
				for (::Beacons::Kernel::iterator ptrJump = g_Beacons.begin(); ptrJump != g_Beacons.end(); )
				{
					// radius check first
					int distance = static_cast<int>(pThisAvatar->distanceTo(*ptrJump));
					if (range != -1 && distance > range)
					{
						ptrJump++;
						continue;
					}

					// citizen check first
					if (citizen != -1 && citizen != ptrJump->getOwner())
					{
						ptrJump++;
						continue;
					}

					// delete
					ptrJump = g_Beacons.list.erase(ptrJump);
					count++;
				}
				
				// report
				cisVM["total_deleted"] = count;
				write(cisLang["total_deleted"], cisVM);
			} 
			else 
			{
				// delete a beacon from the list
				Beacon* pBeacon = g_Beacons.Search(getFlagText());
				if (pBeacon == NULL) 
				{
					write(cisLang["not_found"], cisVM);
					return Reason::BadArgument;
				}
				
				// assign vm
				cisVM["jump"] = *pBeacon;

				// check this beacons owner
				if (isAvatar() == true)
				{
					if (pBeacon->getOwner() != active_cit && getRight("jumpgate/manage") == false)
					{
						write(cisLang["not_owner"], cisVM);
						return Reason::AccessDenied;
					}
				}

				// delete
				write(cisLang["default"], cisVM);
				g_Beacons.Delete(pBeacon->getName());
			}
			return Reason::Success;
		}

		/* range checker for structure */
		struct RANGE_POINT {
			int range;
			CStringA name;
		};

		int JumpNearProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("jump/near") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("", "");
					CIS_LANGUAGE("default", "Nearest jump points {$found.as_and}");
					CIS_LANGUAGE("", "");
					CIS_LANGUAGE("", "");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// new reference to configuration kernel
			Beacons::Config& cfg = Beacons::Kernel::getInst().config;
			Beacons::Kernel& g_Beacons = Beacons::Kernel::getInst();

			/* centre location */
			KnownList klPoints; 
			String logText;
			AW::Location locFocus = *getAvatar();

			/* range calculation */
			std::vector<RANGE_POINT> jumps;
			for (size_t i = 0; i < g_Beacons.size(); i++) {
				RANGE_POINT pt = { 
					(int)g_Beacons[i].distanceToGD(locFocus), 
					g_Beacons[i].getName() 
				};
				jumps.push_back(pt);
			}

			/* sort the list */
			BUBBLESORT_VECTOR(jumps, RANGE_POINT, .range);
			for (size_t i = 0; i < jumps.size(); i++) 
			{
				// only show up to the nearest <defined> jump points
				if (i >= (size_t)cfg.getSearchResultLimit()) 
					break;  // only do 6
				
				logText.Format("%s (%dm)", jumps[i].name, (int)(jumps[i].range / 100));
				klPoints.push_back(logText);
			}

			// list the array
			cisVM["found"] = klPoints;
			write(String("Jump points found: ") + klPoints.toString("and") );
			return Reason::Success;
		}

		int JumpListProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("jump/list") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("none_found", "There are no jump points created by your citizenship");
					CIS_LANGUAGE("default", "There are {$detected.size} jump points owned by your citizenship: {$detected.as_and}");
					CIS_LANGUAGE("", "");
					CIS_LANGUAGE("", "");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// new reference to configuration kernel
			Beacons::Config& cfg = Beacons::Kernel::getInst().config;
			Beacons::Kernel& g_Beacons = Beacons::Kernel::getInst();

			/* list users on current citizenship */
			int owner = isAvatar() ? getAvatar()->getCitizen() : -1;
			std::vector<Beacon*> beacon_array;
			String	  logText;
			KnownList klList;
			
			/* search for the list and notify */
			g_Beacons.SearchOwner(owner, beacon_array);
			if (beacon_array.size() == 0)
			{
				write(cisLang["none_found"], cisVM);
			} 
			else 
			{
				for (size_t i = 0; i < beacon_array.size(); i++) 
				{
					logText.Format("%s (%s)", beacon_array[i]->getName(), beacon_array[i]->getCellCoordinates());
					klList.push_back(logText);
				}

				// assign vm
				cisVM["detected"] = klList;
				write(String("Jump points created by your citizenship: ") + klList.toString("and"));
			}

			/* successful */
			return Reason::Success;
		}

		int JumpSearchProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("jump/search") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("not_found", "There were no jump points matching your query");
					CIS_LANGUAGE("default", "");
					CIS_LANGUAGE("", "");
					CIS_LANGUAGE("", "");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// new reference to configuration kernel
			Beacons::Config& cfg = Beacons::Kernel::getInst().config;
			Beacons::Kernel& g_Beacons = Beacons::Kernel::getInst();

			// required arguments setup
			CIS_BEGIN_REQUIRED_ARGUMENTS("jump/search");
				CIS_REQUIRE_ARGUMENT("query");
			CIS_END_REQUIRED_ARGUMENTS();

			// list of arguments
			String argQuery = getParam("query");
			String argUser  = getParam("user");

			// arguments to variables
			String logText;
			int user_id = atoi(argUser);
			argQuery.MakeLower();
			KnownList found;

			// go through each beacon
			for (::Beacons::Kernel::iterator jpPtr = g_Beacons.begin(); jpPtr != g_Beacons.end(); jpPtr++)
			{
				String jpName = jpPtr->getName();
				jpName.MakeLower();
				if (jpName.Find(argQuery) != -1)
					if (user_id == 0 || jpPtr->getOwner() == user_id)
						found.push_back(jpName);
				
				// limit to the same as the closest
				if (found.size() == cfg.getSearchResultLimit())
					break;
			}

			// check if any found
			if (found.size() == 0)
			{
				logText.Format("There we no jump points matching your query");
				write(logText);
			}
			else
			{
				logText.Format("The following jump points were found: %s", found.toString("and"));
				write(logText);
			}

			/* successful */
			return Reason::Success;
		}
	}
}