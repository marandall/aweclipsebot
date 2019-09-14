/* the CIS systems - Contained within the CIS folder which is 1 level below */
/* the root level - therefore all incldues must be upclassed			    */

#include "stdafx.h"
#include "versions.h"
#include "CommonAutoResponding.h"

#include "bubblesort.h"
#include <vector>

#include "CIS_Interfaces.h"				// contains tracking classes
#include "CIS_Basic.h"					// for basic commands
#include "RelayRelay.h"
#include "movement.h"
#include "cis_commandbindings.h"
#include "cis_dispatch.h"
#include "cis_configuration.h"
#include "avataroptions.h"
#include "property_editors.h"
#include "dynamicrights.h"
#include "cis_delayedlookup.h"
#include "headsup.h"
#include "headsup_userplugin.h"
#include "EclipseEvolutionDialog.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

typedef String String;


namespace CIS 
{
	namespace Exec 
	{

		///************************************************************************
		// * Bot Information Process - get the Owner of the Bot					
		// * Required Rights:		core/info										
		// ***********************************************************************/
		int InfoProcessor::process()
		{
			/* do we identify to all */
			if (getRight("gcs/info") == true)
			{
				// authorised by GCS rights
			}
			else if (CommonFeatures::g_AutoResponding.getIdentifyOwner())
			{
				// authorised
			}
			else if (getAvatar() && getAvatar()->HasEject())
			{
				// authorised by the user having eject
			}
			else if (CommonFeatures::g_AutoResponding.getIgnoreHailOnEject() && bot.HasEject())
			{
				// bot may not wish to respond
				return Reason::AccessDenied;
			}
			else
			{
				return Reason::AccessDenied;
			}

			// respond with the command information	
			String logText;
			logText.Format("I am Eclipse Evolution (Build %d - %s) [Umbra Enhanced] "
					   "using SDK %d - Copyright (c) Mark Randall 2006, all rights reserved - Compiled on " __DATE__
					   ". Owned by %s (citizen: %d)", 
					EVOLUTION_VERSION, EVOLUTION_IDENTIFY, AW_BUILD, bot._string(AW_LOGIN_PRIVILEGE_NAME),
					bot.getCitizen());

			/* push the message */
			write(logText);
			return Reason::Success;
		}

		int WhereProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("where") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("none_found", "I could not detect any users");
					CIS_LANGUAGE("avatar_only", "Only avatars may use this command");
					CIS_LANGUAGE("am_i", "You are at {$user.coordinates}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// method 1
			String method = getFlagText();
			String log;

			// basic locate
			if (method == "am i")
			{
				if (getAvatar() == 0)
				{
					write(cisLang["avatar_only"], cisVM);
				}
				else
				{
					cisVM["user"] = *getAvatar();
					write(cisLang["am_i"], cisVM);
				}
			}

			// return
			return Reason::Success;
		}

		///************************************************************************
		// * Avatar Listing Process - get the list of nearby avatars
		// * Required Rights:		core/info				
		// ***********************************************************************/
		int PresentProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("present") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("none_found", "I could not detect any users");
					CIS_LANGUAGE("default", "I detect {$detected.as_and}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			/* these are the variables used to loop */
			String	logText, name, names;
			KnownList	found;

			/* no avatars found, console command */
			size_t count = avatars.size();
			if (avatars.size() == 0) 
			{
				// write output
				Umbra::VariantMap cisVM = bot.CreateVariantMap();
				write(cisLang["none_found"], cisVM);
				return Reason::Success;
			}

			/* do we need to use flags? */
			if (getFlag("-uf") == true) {
				bool bn_inc[4];
				bn_inc[UserType::Tourist] = getFlag("-tourists") || getFlag("-t") || getFlag("-tc");
				bn_inc[UserType::Citizen] = getFlag("-citizens") || getFlag("-c") || getFlag("-tc");
				bn_inc[UserType::Bot]	  = getFlag("-bots")     || getFlag("-b");

				/* deal with it various different modes */
				FOR_EACH_OF_AVATARS
					if (&User != getAvatar() && bn_inc[User.getType()] == true)
						found.push_back(User.getName());
				END_FOR_EACH
			} else {
				FOR_EACH_OF_AVATARS
					if (&User != getAvatar())
						found.push_back(User.getName());
				END_FOR_EACH
			}

			/* sort the list */
			found.SortAZ();

			/* selection */
			cisVM["detected"] = found;
			if (found.size() > 0) 
			{
				write(cisLang["default"], cisVM);
			}
			else 
			{
				write(cisLang["none_found"], cisVM);
			}

			return Reason::Success;
		}

		struct AvatarRange {
			Avatar* pUser;
			double	range;
		};

		int NearProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("near") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("invalid_1",  "Invalid arguments, requires [source = 0], [range]")
					CIS_LANGUAGE("none_found", "I could not locate any users");
					CIS_LANGUAGE("default",    "I detect {$detected.as_and}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------


			int	radial = 20000;		// 200 meters

			/* near me(), 200 */
			SplitString				args(getFlagText(), ",", 2);
			String			logText;
			AW::Location		sourceLc;
			vector<AvatarRange>	distances;

			if (args.Count() != 2) 
			{
				write(cisLang["invalid_1"], cisVM);
				return Reason::InvalidArgument;
			} 
			else 
			{
				String argUser = args[0];	argUser.Trim();
				String argRange= args[1]; argRange.Trim();
				Avatar* pUser = avatars.GetByKey(argUser, getAvatar());
				
				if (!pUser) 
				{
					// write output
					cisVM["target"]	= (LPCSTR)argUser;
					write(cisLang["none_found"], cisVM);

					// its bad, bad bad bad
					return Reason::BadArgument;
				}
				else 
				{ 
					radial = (int)atof(argRange) * 100;
					sourceLc = *pUser;
				}
			}

			/* display all the avs in such range */
			FOR_EACH_OF_AVATARS
				double dist_to = 0;
				
				if (getFlag("-2d") == true) dist_to = User.distanceToGD(sourceLc);
				else dist_to = User.distanceTo(sourceLc);
				
				if (dist_to < radial) {
					AvatarRange rng = { &User, dist_to };
					distances.push_back(rng);
				}

			END_FOR_EACH

			/* do we have any users? If not it still worked */
			if (distances.size() == 0) 
			{
				write(cisLang["none_found"], cisVM);
				return Reason::Success;
			}

			/* go through each avatar and display them */
			KnownList	known;
			for (size_t i = 0; i < distances.size(); i++) 
			{
				String t;
				t.Format("%s @ %dm", distances[i].pUser->getName(), (int)(distances[i].range / 100.0F));
				known.push_back(t);
			}


			// write output
			cisVM["detected"] = known;
			cisVM["meters"]	= static_cast<float>(radial / 100.0F);
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}

		///************************************************************************
		// * Avatar Location Process - get information about avatar(s)
		// * Required Rights:		core/info [eject/info]
		// * locate -f Mark Randall, E N Z O, Cryonics, (...)
		// ***********************************************************************/
		int LocateProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("locate") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("none_found", "I could not locate {$target}");
					CIS_LANGUAGE("i_detect",   "I detect {$detected.as_and}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------


			/* these are the variables used to loop */
			SplitString		args;
			String			buffer, logText, complete;
			KnownList		found, missing;

			/* to save doing things twice - first we strip the calling arguments */
			size_t argc = args.split(getFlagText(), ",", -1);		// split the information up into its sections
			
			/* here we go through all the information - first of all the flags */
			bool full  = getFlag("-f")  | getFlag("-fe");
			bool eject = (getFlag("-e") | getFlag("-fe")) & getRight("eject/info");
			bool conc  = false;

			/* we can now go through each item individually and get what we need */
			for (size_t i = 0; i < argc; i++) {
				/* this is because we wish to trim everything */
				String userID = args[i]; 
				userID.Trim();
				
				/* check if empty, therefore skip*/
				if (userID.GetLength() == 0)
					continue;

				/* we use the trimmed name */
				Avatar* pUser = avatars.GetByKey(userID, this_avatar);
				
				/* do we have one or do we need to add it to the not found list */
				if (!pUser)
					missing.push_back(args[i]);
				else {
					/* this is a self modifier */
					bool is_self = (pUser != NULL && getAvatar() == pUser);		// evaluate to boolean
					logText = pUser->getOutputReport(full, eject, is_self);
					
					/* do we have a sl list, wtf is sl */
					if (getFlag("-sl") == true) 
						found.push_back(buffer);
					else 
						write(logText);
				}
			}

			/* do we add em all to a single line */
			if (getFlag("-sl") == true && found.size() > 0) {
				cisVM["detected"] = found; 
				write(cisLang["i_detect"], cisVM);
			}

			/* at the end of it we say who we can not find */
			if (missing.size() > 0)
			{
				temp.Format("I could not locate %s", missing.toString("or"));
				write(temp);
			}

			return Reason::Success;
		}

		int WildcardsProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("wildcards") {

			} CIS_END_FUNCTION
			// ---------------- end function header ----------------


			/* we handle with the different request users later */
			SplitString		args;
			String			buffer, logText;
			AvatarList		list;
			std::vector<Avatar*> scanlist;

			/* now take the result and parse that */
			size_t argc = args.split(getFlagText(), ",", -1);		// split the information up into its sections
			
			/* loop through each one */
			for (size_t i = 0; i < argc; i++) {
				CString trim = args[i]; trim = trim.Trim();
				avatars.getWildcards(trim, list);
			}

			/* copy to scanlist */
			for (size_t i = 0; i < list.size(); i++)
				scanlist.push_back(list[i]);

			/* print them all out */
			write("-------- Wildcard Results --------");

			/* go through each avatar */
			bool inc_eject = getFlag("-e") && getRight("eject/info");
			for (size_t i = 0; i < scanlist.size(); i++) {
				write(scanlist[i]->getOutputReport(getFlag("-f"), 
					inc_eject, false));
			}

			/* state how many have been found */
			logText.Format("%d wildcard matches found", (int)list.vCol.size());
			write(logText);
			return Reason::Success;
		}

		///************************************************************************
		// * Avatar Callback Process - Whispers to caller to appear in whisper box
		// * Required Rights:		core/info				
		// ***********************************************************************/
		int IdentifyProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("identify") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("hello", "Hello, {$user.name}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			temp.Format("Hello, %s", getName());
			write(temp);
			return Reason::Success;
		}


		int StatusProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("status") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("default", "I am {$bot.name} #{$bot.session} under citizen #{$bot.owner} ({$bot.owner_name}) with modes {$modes.as_and}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// erase message
			rt_diag->c_Log.cropOverflow(atoi(getFlagText()));

			// create basic modes array
			KnownList modes;
			if (bot.HasCT())		modes.push_back("ct");
			if (bot.GetGlobal())	modes.push_back("global");
			if (bot.getInvisible())modes.push_back("invisible");
			if (bot.HasEject())		modes.push_back("eject");

			// relay modes only when in global mode
			if (bot.GetGlobal())
			{
				using namespace ChatControllers;
				if (g_Relay.IsGlobalChat()) modes.push_back("relay(global)");
				if (g_Relay.IsAdvancedRelay()) modes.push_back("relay(advanced)");
			}

			// format message
			temp.Format("I am %s #%d under citizen #%d (%s) with modes %s", bot.getBotName(), bot._session(), 
				bot.getAL_Citizen(), bot._string(AW_LOGIN_PRIVILEGE_NAME), modes.toString("and"));
			write(temp);
			return Reason::Success;
		}

		String lc_TargetList(const char* type, int count, float per_user)
		{
			String ret;
			ret.Format("%s: %d (%.1f%%)", type, count, count * per_user);
			return ret;
		}

		int RatiosProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("ratios") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("none_found", "Ratios are unavailable while no users are present");
					CIS_LANGUAGE("default", "Current ratios are {$ratios.as_and}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------


			// no users detected
			if (avatars.getCount() == 0) {
				write(cisLang["none_found"], cisVM);
				return Reason::Failed;
			}

			// ratio list
			KnownList	list;
			float		per_user = static_cast<float>(100.0F / avatars.getCount());

			/* list the users */
			if (avatars.getUserCount(UserType::Tourist)) 
				list.push_back(lc_TargetList("Tourists", avatars.getUserCount(UserType::Tourist), per_user));
			if (avatars.getUserCount(UserType::Citizen))
				list.push_back(lc_TargetList("Citizens", avatars.getUserCount(UserType::Citizen), per_user));
			if (avatars.getUserCount(UserType::Bot))     
				list.push_back(lc_TargetList("Bots",	 avatars.getUserCount(UserType::Bot),	  per_user));
			
			/* everything is ok */
			cisVM["ratios"] = list;
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}

		int LastActiveProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("lastactive") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("none_found", "Could not find user");
					CIS_LANGUAGE("default", "Current ratios are {$ratios.as_and}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------


			/* have to find the avatar */
			Avatar* pTarget = avatars.GetByKey(getFlagText(), getAvatar());
			if (!pTarget) 
			{
				write(cisLang["none_found"], cisVM);
			}
			else
			{
			}
			
			return Reason::Success;
		}

		int HelpProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("help") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("default", "Eclipse Evolution help is available at http://www.awportals.com/eclipse/");
					CIS_LANGUAGE("url", "http://www.awportals.com/eclipse/?type=help&user={$user.name}&world={$bot.world}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// respond
			write(cisLang["default"], cisVM);
			
			// if avatar and has CT
			if (bot.HasCT() && isAvatar())
			{
				bot._bool_set(AW_URL_TARGET_3D, TRUE);
				getAvatar()->SendURL(cisVM.parse_string((LPCSTR)cisLang["url"]).c_str(), NULL, true);
			}

			// all is well
			return Reason::Success;
		}


		int MyPrivilegeProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("myprivs") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("only_citizens", "This command is only available to citizens");
					CIS_LANGUAGE("no_others", "There are currently no other users or bots under your privilege password in this world");
					CIS_LANGUAGE("default", "Users currently under your privilege: {$detected.as_and}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------


			/* must not be a tourist, define data */
			Avatar& user = *getAvatar();
			KnownList	list;

			// citizens only
			if (user.getType() != UserType::Citizen) {
				write(cisLang["only_citizens"], cisVM);
				return Reason::AccessDenied;
			}

			/* list the users */
			FOR_EACH_OF_AVATARS
				if (User.getPrivilege() == user.getCitizen() && (&User != &user))
					list.push_back(User.getName());
			END_FOR_EACH

			// write list
			cisVM["detected"] = list;

			// list container check
			if (list.size() == 0)
				write(cisLang["no_others"], cisVM);
			else
				write(cisLang["default"], cisVM);
			
			/* successful */
			return Reason::Success;
		}

		int MyCoordinatesProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("coords") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("default", "You are currently located at {$user.coords}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			
			// report
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}

		int MyRightsProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("myrights") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("default", "Your rights: {$rights_string}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			
			// report
			cisVM["rights_string"] = (LPCSTR)pThisAvatar->GetRightsString();
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}

		int MyDrsProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("myrights") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("default", "Your bot rights include {$rights.as_and}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// get a rights list
			KnownList rights;
			std::map<String, std::vector<String> > rights_map;

			// using each avatar
			DynamicRights::Permissions& permissions = this_avatar->getPermissions();
			for (size_t i = 0; i < permissions.size(); i++)
			{
				if (permissions[i].enabled == true)
				{
					SplitString str(permissions[i].key_id, "/", 2);
					if (str.size() == 2)
					{
						rights_map[str[0]].push_back(str[1]);
					}
				}
			}

			// build the list
			KnownList rights2;
			for (std::map<String, std::vector<String> >::iterator ptr = rights_map.begin();
				ptr != rights_map.end();
				ptr++)
			{
				KnownList group;
				for (size_t i = 0; i < ptr->second.size(); i++)
					group.push_back(ptr->second[i]);
				String res;
				res.Format("%s(%s)", ptr->first, group.toString("and"));
				rights2.push_back(res);
			}

			// report
			cisVM["rights"] = rights2;
			write(cisLang["default"], cisVM);
			return Reason::Success;
		}


		struct PBScore
		{
			Avatar* user;
			int		score;
		};

		int Vector(const CIS::Request& _Req, CIS::Response& _Resp)
		{
			return Reason::Success;
		}

		int SetCamera(const CIS::Request& _Req, CIS::Response& _Resp)
		{
			return 0;
		}



		int FollowProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("follow") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("default", "Following {$user.name}");
					CIS_LANGUAGE("error",   "Unable to follow {$target}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// search for user
			Avatar* pTarget = avatars.GetByKey(getFlagText(), getAvatar());
			if (pTarget == 0)
			{
				cisVM["target"] = (const char*)getFlagText();
				write(cisLang["error"], cisVM);
			}
			else
			{
				cisVM["user"] = *pTarget;
				write(cisLang["default"], cisVM);

				// setup
				bot.getMovementKernel().beginFollow();
			}

			// data
			return Reason::Success;
		}

		int CircleProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("circle") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("default", "Circling {$target}");
					CIS_LANGUAGE("error",   "Unable to lock on {$target}");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// syntax:
			// circle 1n 2w, 50
			SplitString s1(getFlagText(), ",", -1);
			if (s1.Count() != 2)
				return Reason::InvalidArgument;

			// test arguments
			String target = s1[0]; target.Trim();
			String radius = s1[1]; radius.Trim();
			cisVM["target"] = (const char*)target;

			// attempt to target
			Location lc(target, getAvatar());
			if (lc.isValid() == false)
			{
				cisVM["target"] = (const char*)getFlagText();
				write(cisLang["error"], cisVM);
			}
			else
			{
				write(cisLang["default"], cisVM);

				// setup
				bot.getMovementKernel().beginCircle();
				bot.getMovementKernel().getCircle().setup(target, (int)(atof(radius) * 100));
			}

			return Reason::NoSuchFunction;
		}
		
		int HaltProcessor::process()
		{
			// ------------------ function header ------------------
			CIS_BEGIN_FUNCTION("halt") {
				// default language
				CIS_BEGIN_LANGUAGE {
					CIS_LANGUAGE("default", "Stopped moving");
				} CIS_END_LANGUAGE
			} CIS_END_FUNCTION
			// ---------------- end function header ----------------

			// cancel
			write(cisLang["default"], cisVM);
			bot.getMovementKernel().standby();

			return Reason::NoSuchFunction;
		}
		
		//
		//
		//

		int SpeedProcessor::process()
		{
			return Reason::NoSuchFunction;
		}

		//
		//
		//

		int	HudProcessor::process()
		{
			// easy reference
			HeadsUp::Config& config = HeadsUp::Config::getInstance();
			if (config.getEnabled() == false)
				return Reason::AccessDenied;

			// user data
			if (getFlagText() == "on")
			{
				this_avatar->getHud().visible = B_EMPTY;
				write("Displaying the heads-up tray");
				this_avatar->getHud().update_all();
			}
			else if (getFlagText() == "off")
			{
				this_avatar->getHud().visible = B_FALSE;
				write("Hiding the heads-up tray");
				this_avatar->getHud().update_all();
			}
			else
			{
				write("Unknown HUD command, expecting 'on' or 'off'");			
			}

			return Reason::Success;
		}
	


		// command, text
		//
		//
		int BindProcessor::process()
		{
			// reference the bindings
			CIS::CommandBindings& cb = CIS::CommandBindings::getInstance();
				
			// show all
			if (getFlag("-showall"))
			{
				CStringA temp;
				for (size_t i = 0; i < cb.size(); i++) {
					temp.Format("%s = %s", cb[i].id, cb[i].command);
					write(temp);
				}
			}
			else if (getFlag("-unset"))
			{
				if (cb.unbind(getFlagText()) == true)
				{
					write("Command has been unbinded");
				}
				else
				{
					write("That binding does not exist");
				}
			}
			else
			{
				// resend the binded command
				SplitString s(getRaw(), ",", 2);
				if (s.Count() != 2) {
					write("Must provide 2 arguments, [name], [command]");
					return Reason::InvalidArgument;
				}

				// search for bindings
				CIS::CommandBindings::Binding* pBinding = cb.getBinding(s[0]);

				// check
				if (pBinding && getFlag("-ow") == false)
				{
					write("That command binding already exists, to force this change use -ow");
				}
				else
				{
					CIS::CommandBindings::Binding b;
					b.id				= s[0];
					b.command			= s[1];
					b.id.Trim();
					b.command.Trim();
					b.right_required	= "system/administrator";
					b.remote			= false;
					cb.list.push_back(b);

					write("Added command binding to the command binding list");
				}
			}

			return Reason::Success;
		}

		// command, text
		//
		//
		int UbindProcessor::process()
		{
			// search for bindings
			CIS::CommandBindings& cb = CIS::CommandBindings::getInstance();
			CIS::CommandBindings::Binding* pBinding = cb.getBinding(getRaw());

			// check
			if (pBinding)
			{
				return CIS::CommandInterpreter::beginConsole(pBinding->command, getFlag("-hide"));
			}
			else
			{
				write("That binding is not known");
				return Reason::Success;
			}
		}
		// command, text
		//
		//
		int EjectProcessor::process()
		{
			// split the information
			SplitString ops(getFlagText(), ",", 3);

			// assignment split
			String sTarg = ops[0]; sTarg.Trim();
			String sDur  = ops[1]; sDur.Trim();
			String sInfo = ops[2]; sInfo.Trim();

			// check that ejections are enabled
			if (CIS::Config::g_Ejection.getEnable() == false && this_avatar)
			{
				write("Ejections via command have been disabled upon this bot.");
				return Reason::Success;
			}

			// lookup the user to target
			Avatar* pTarget = avatars.GetByKey(sTarg, this_avatar);
			if (pTarget == 0) 
			{
				temp.Format("I am unable to locate '%s'", sTarg);
				write(temp);
				return Reason::BadArgument;
			}

			// duration conversion
			int duration = atoi(sDur);

			// limit check
			if (duration < 0)
			{
				write("You cannot eject for less than 0 minutes");
				return Reason::BadArgument;
			}

			// copy attr
			int maxEjectAdmin = CIS::Config::g_Ejection.getMaxEjectLengthAdmin();
			int maxEjectUsers = CIS::Config::g_Ejection.getMaxEjectLengthUsers();
			bool comRequired  = CIS::Config::g_Ejection.getRequireComment();

			// greater than the maximum amount of minutes

			if (maxEjectUsers != 0 && duration > maxEjectUsers && !getRight("system/administrator"))
			{
				temp.Format("You cannot eject for longer than %d minutes (user level)", maxEjectAdmin);
				write(temp);
				return Reason::BadArgument;
			}
			else if ( maxEjectAdmin != 0 && duration > maxEjectAdmin)
			{
				temp.Format("You cannot eject for longer than %d minutes (admin level)", maxEjectAdmin);
				write(temp);
				return Reason::BadArgument;
			}

			// protection check, admins cannot be ejected
			if (pTarget->GetRight("system/administrator") == true)
			{
				write("Unable to eject a user with administrator rights");
				return Reason::BadArgument;
			}

			// comment required
			if (comRequired && sInfo.GetLength() == 0)
			{
				write("You must provide a comment as a reason for your ejection");
				return Reason::BadArgument;
			}

			// notify
			temp.Format("Attempting to eject %s for %d minutes", pTarget->getName(), duration);
			write(temp);

			// eject the target
			if (pTarget->Eject(duration * 60, "", sInfo, this_avatar) == -1)
			{
				temp.Format("Ejection failed, target may be invulnerable");
				write(temp);
			}

			// whatever
			return Reason::Success;
		}

		//
		//
		//
		int ClickmodeProcessor::process()
		{
			// click modes
			SplitString em(getFlagText(), " ", 2);
			String cmd = em[0];
			String arg = em[1];

			// namespace
			using namespace Property::Editors;

			// tokenize pamra
			CommandParser p2(arg);

			// telling position
			if (cmd == "none" || cmd == "off")
			{
				this_avatar->getOptions().changeClickMode(AvatarOptions::CLICKMODE_NONE, 0);
				write("Changing your object click mode to 'none'");
			}
			else if (cmd == "tellpos")
			{
				this_avatar->getOptions().changeClickMode(AvatarOptions::CLICKMODE_TELLPOS, 0);
				write("Changing your object click mode to 'tell position'");
			}

			// deleting the objects
			else if (cmd == "delete")
			{
				this_avatar->getOptions().changeClickMode(AvatarOptions::CLICKMODE_DELETE, 0);
				write("Changing your object click mode to 'delete'");
			}

			// run a filter
			else if (cmd == "findreplace")
			{
				StringReplace s;
				
				// build string type
				s.find			= p2.getText("find");
				s.replacement	= p2.getText("replace");
				
				// the replacement values
				String fields	= p2.getText("fields");
				s.search_model	= strstr(fields, "m") != NULL;
				s.search_description = strstr(fields, "d") != NULL;
				s.search_action	= strstr(fields, "a") != NULL;

				// assign this filter
				this_avatar->getOptions().changeClickMode(AvatarOptions::CLICKMODE_EDITOR, new StringReplace(s));
				write("Changing your object click mode to 'string replace'");
			}

			// run a filter
			else if (cmd == "replace")
			{
				ReplaceField s;

				// the replacement values
				if (p2.getExists("model") == true)
				{
					s.do_model	= true;
					s.new_model	= p2.getText("model"); 
				}
				if (p2.getExists("action") == true)
				{
					s.do_action	= true;
					s.new_action= p2.getText("action"); 
				}
				if (p2.getExists("description") == true)
				{
					s.do_description = true;
					s.new_description = p2.getText("description"); 
				}

				// debug
				this_avatar->getOptions().changeClickMode(AvatarOptions::CLICKMODE_EDITOR, new ReplaceField(s));
				write("Changing your object click mode to 'field replace'");
			}

			// nothing
			else
			{
				write("Unknown clickmode filter");
			}

			// return ok
			return Reason::Success;
		}

		//
		// Universe
 		//

		int CitnumProcessor::process()
		{
			CitizenGroupLookupCompleter* pQuery = new CitizenGroupLookupCompleter(this, &CitizenLookup::g_Kernel);
			pQuery->preserveChildren();
			pQuery->open();
			
			// split into tokens
			SplitString elements(raw_text, ",", -1);

			// only certain elemenets
			for (size_t i = 0; i < elements.size(); i++)
			{
				// avatars are only allowed 10 lookups per request
				if (isAvatar() && i >= 10)
					break;

				// parse the name
				String name = elements[i];
				name.Trim();

				// look it up by number
				pQuery->lookupByName(name);
			}

			// close the group
			if (pQuery->getOutstanding() == 0)
			{
				pQuery->close();
				return Reason::Success;
			}
			else
			{
				write("Please wait while looking up citizen numbers...");
				pQuery->deleteChildren();
				pQuery->close();
				return Reason::CompletionDelayed;
			}
		}

		int CitnumProcessor::resume()
		{
			CIS::CitizenGroupLookupCompleter* pLookupResults 
				= dynamic_cast<CitizenGroupLookupCompleter*>(resumption_args["results"].toRemoteClass());

			// iterate each item in the results set
			for (size_t i = 0; i < pLookupResults->size(); i++)
			{
				CitizenLookup::GroupLookupHelper& helper = (*pLookupResults)[i];
				AW::Citizen& citinfo = helper.info;

				// has completed ok?
				if (helper.result != 0)
				{
					temp.Format("Unable to lookup '%s'; reason %d", helper.name, helper.result);
				}
				else
				{
					temp.Format("The user '%s' is citizen #%d", citinfo.getName(), citinfo.getNumber());
				}

				write(temp);
			}

			// all is good
			return Reason::Success;
		}



		int CitnameProcessor::process()
		{
			CitizenGroupLookupCompleter* pQuery = new CitizenGroupLookupCompleter(this, &CitizenLookup::g_Kernel);
			pQuery->preserveChildren();
			pQuery->open();
			
			// split into tokens
			SplitString elements(raw_text, ",", -1);

			// only certain elemenets
			for (size_t i = 0; i < elements.size(); i++)
			{
				// avatars are only allowed 10 lookups per request
				if (isAvatar() && i >= 10)
					break;

				// parse the name
				String name = elements[i];
				name.Trim();

				// look it up by number
				pQuery->lookupByNumber(atoi(name));

			}

			// close the group
			if (pQuery->getOutstanding() == 0)
			{
				pQuery->close();
				return Reason::Success;
			}
			else
			{
				write("Please wait while looking up citizen names...");
				pQuery->deleteChildren();
				pQuery->close();
				return Reason::CompletionDelayed;
			}
		}

		int CitnameProcessor::resume()
		{
			CIS::CitizenGroupLookupCompleter* pLookupResults 
				= dynamic_cast<CitizenGroupLookupCompleter*>(resumption_args["results"].toRemoteClass());

			// iterate each item in the results set
			for (size_t i = 0; i < pLookupResults->size(); i++)
			{
				CitizenLookup::GroupLookupHelper& helper = (*pLookupResults)[i];
				AW::Citizen& citinfo = helper.info;

				// has completed ok?
				if (helper.result != 0)
				{
					temp.Format("Unable to lookup citizen #%d; reason %d", helper.number, helper.result);
				}
				else
				{
					temp.Format("Citizen #%d is %s", citinfo.getNumber(), citinfo.getName());
				}

				write(temp);
			}

			// all is good
			return Reason::Success;
		}
	}
}