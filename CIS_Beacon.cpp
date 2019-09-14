#include "stdafx.h"

#include "Validators.h"					// for double validator
#include "CIS_Interfaces.h"				// contains tracking classes
#include "CIS_Eject.h"					// for eject namespace
#include "Beacons.h"					// for beacon code
#include "Sorting.h"					// for bubblesort

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

using namespace CoreSystem::Public::Beacons;

namespace CIS {
	namespace Exec {
		namespace Beacons {
			int Create(const CIS::Request& _Req, CIS::Response& _Resp) {
				CIS_REQUIRE_RIGHT("beacon/create");
				/* tourists are not allowed */
				if (_Req.GetAvatar()) {
					if (_Req.GetAvatar()->GetType() != USER_CITIZEN) {
						cis_respond_to("Sorry, only citizens can create Jump Points");
						return Reason::AccessDenied;
					} }

				// filter down the strings 
				CStringA nameText = _Req.TextF();
				if (nameText.GetLength() == 0) {
					cis_respond_to("You must provide a Jump Point name");
					return Reason::BadArgument;
				}

				int citizen_owner = _Req.IsAvatar() ? _Req.GetAvatar()->GetCitizen() : -1; /* create a filter */
				if (!g_Beacons.Insert(citizen_owner, nameText, 
					_Req.GetAvatar() ? _Req.GetAvatar()->GetCoordinates() : bot.GetCoordinates()))
				{
					cis_respond_to("Your Jump Point could not be created due to a name conflict");
					return Reason::BadArgument;
				} else {
					cis_respond_to("Your Jump Point has been created");
					return Reason::Success;
				}
			}
			
			int Use(const CIS::Request& _Req, CIS::Response& _Resp) {
				CIS_REQUIRE_RIGHT("beacon/use");
				if (!_Req.GetAvatar()) {
					cis_respond_to("This command can only be used by avatars");
					return Reason::InvalidCallState;
				}

				/* check beacons exist */
				if (g_Beacons.size() == 0) {
					cis_respond_to("There are no jump points yet defined");
					return Reason::Success;
				}

				/* find which point */
				CoreSystem::Public::Beacons::Beacon* pBeacon = NULL;
				CStringA logText;

				if (_Req.TextF() == "random()") pBeacon = &g_Beacons[aw_random() % g_Beacons.size()];
				else pBeacon = g_Beacons.Search(_Req.TextF());
				if (!pBeacon) {
					cis_respond_to("That Jump Point could not be found");
					return Reason::BadArgument;
				} else {

					/* check if a flag is given */
					if (_Req.Flag("-info")) {
						logText.Format("%s (owned by %d) is located at %s", pBeacon->GetName(), pBeacon->GetOwner(), pBeacon->GetCoordinates());
						cis_respond_to(logText);
					} else {
						CIS_CHECK_EJECT_MACRO

						/* calculate transport */
						AW::Coordinates coords;
						
						coords.FromString(pBeacon->GetLocation());
						logText.Format("Jumpgate Activated to %s", pBeacon->GetName());
						cis_respond_to(logText);

						_Req.GetAvatar()->Teleport(coords, "", _Req.Flag("-w"));
					}
					return Reason::Success;
				}
			}
			
			int Reset(const CIS::Request& _Req, CIS::Response& _Resp) {
				CIS_REQUIRE_RIGHT("beacon/manage");
			
				/* resets all of the beacons */
				g_Beacons.Erase();
				cis_respond_to("The Jump Point list has been reset...");
				return Reason::Success;
			}
			
			int Delete(const CIS::Request& _Req, CIS::Response& _Resp) {
				CIS_REQUIRE_RIGHT("beacon/create");
			
				/* active cit if possible*/
				int active_cit = 0;
				CStringA logText;
				if (_Req.GetAvatar()) {
					if (_Req.GetAvatar()->GetType() != USER_CITIZEN) {
						cis_respond_to("Sorry, only citizens can manage Jump Points");
						return Reason::InvalidCallState;
					}
					active_cit = _Req.GetAvatar()->GetCitizen();
				} else {
					active_cit = -1;
				}
			
				/* are we doing a proximity or citizen erase */
				if (_Req.Flag("-citizen") || _Req.Flag("-range")) {
					if (_Req.GetRight("beacon/manage") == false) {
						cis_respond_to("Sorry, only users with beacon/manage rights may use citizen or range deletion");
						return Reason::AccessDenied;
					}
					
					/* handle a citizenship based deletion */
					if (_Req.Flag("-citizen")) {
						Validators::Integer vdCitizen(_Req.TextF());
						if (vdCitizen.Valid() == false) {
							cis_respond_to("Invalid citizen number");
							return Reason::BadArgument;
						} else {
							int result = g_Beacons.RemoveBy(vdCitizen.Result());
							logText.Format("%d jump points owned by citizen %d have been deleted", result, vdCitizen.Result());
							cis_respond_to(logText);
							return Reason::Success;
						}
					} else {
						Validators::Integer vdRange(_Req.TextF());
						if (vdRange.Valid() == false) {
							cis_respond_to("Invalid Range Given");
							return Reason::BadArgument;
						} else {
							AW::Location loc;
							if (_Req.IsAvatar() == true) loc = *_Req.GetAvatar();
							else loc = bot;

							int result = g_Beacons.RemoveProximity(loc, vdRange.Result() * 100);
							logText.Format("%d jump points have been deleted", result);
							cis_respond_to(logText);
							return Reason::Success;
						}
					}
				} else {
					/* delete a beacon from the list */
					Beacon* pBeacon = g_Beacons.Search(_Req.TextF());
					if (!pBeacon) {
						cis_respond_to("There is no Jump Point by that name");
						return Reason::BadArgument;
					}
					/* check this beacons owner */
					if (pBeacon->GetOwner() != active_cit && 
						(!_Req.GetRight("beacon/manage")))
					{
						cis_respond_to("You can only manage your own Jump Points");
						return Reason::AccessDenied;
					} else {
						g_Beacons.Delete(_Req.TextF());
						cis_respond_to("Jump Point has been deleted");
						return Reason::Success;
					}
				}
				return Reason::Success;
			}

			/* range checker for structure */
			struct RANGE_POINT {
				int range;
				CStringA name;
			};

			int Near(const CIS::Request& _Req, CIS::Response& _Resp) {
				CIS_REQUIRE_RIGHT("beacon/use");

				/* avatar only */
				if (_Req.IsAvatar() == false) {
					cis_respond_to("Only avatars can use this feature");
					return Reason::InvalidCallState;
				}

				/* centre location */
				CKnownList klPoints; CStringA logText;
				AW::Location locFocus = *_Req.GetAvatar();

				/* range calculation */
				std::vector<RANGE_POINT> jumps;
				for (size_t i = 0; i < g_Beacons.size(); i++) {
					RANGE_POINT pt = { g_Beacons[i].DistanceToGD(locFocus), g_Beacons[i].GetName() };
					jumps.push_back(pt);
				}

				/* sort the list */
				BUBBLESORT_VECTOR(jumps, RANGE_POINT, .range);
				for (size_t i = 0; i < jumps.size(); i++) {
					if (i >= 7) break;  // only do 6
					logText.Format("%s (%dm)", jumps[i].name, (int)(jumps[i].range / 100));
					klPoints.push_back(logText);
				}

				/* print the list */
				logText.Format("Nearest Jump-Points: %s", klPoints.ToString("and")); 
				cis_respond_to(logText);
				return Reason::Success;
			}

			int List(const CIS::Request& _Req, CIS::Response& _Resp) {
				CIS_REQUIRE_RIGHT("beacon/use");

				/* list users on current citizenship */
				int owner = _Req.IsAvatar() ? _Req.GetAvatar()->GetCitizen() : -1;
				std::vector<Beacon*> beacon_array;
				CStringA logText;
				CKnownList klList;
				
				/* search for the list and notify */
				g_Beacons.SearchOwner(owner, beacon_array);
				if (beacon_array.size() == 0) {
					cis_respond_to("There are no jump points created by your citizenship");
				} else {
					for (size_t i = 0; i < beacon_array.size(); i++) {
						logText.Format("%s (%s)", beacon_array[i]->GetName(), beacon_array[i]->GetCoordinates());
						klList.push_back(logText);
					}
					logText.Format("There are %d jump points owned by your citizenship:", (int)beacon_array.size());
					cis_respond_to(logText);
					cis_respond_to(klList.ToString("and"));
				}

				/* successful */
				return Reason::Success;
			}
		}
	}
}