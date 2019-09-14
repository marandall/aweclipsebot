#include "stdafx.h"

#include "cis_interfaces.h"
#include "CIS_World.h"
#include "Helpers_Text.h"				// for text namespace
#include "DialogCore.h"					// for the cmsg type
#include "AWSDK_World.h"				// include world options
#include "RightsList.h"
#include "cis_delayedlookup.h"
#include "worldattributes.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

namespace CIS 
{
	namespace Exec 
	{
		int WorldCioProcessor::process()
		{
			// find the text
			Umbra::VariantMap vm;
			String text = vm.parse_string((LPCSTR)getRaw()).c_str();
			int rc = 0;

			// new immigration officer must be within max length
			if (text.GetLength() > AW_MAX_ATTRIBUTE_LENGTH) 
			{
				write("Sorry, the expanded message is too long"); 
				return Reason::BadArgument;
			}
			else
			{
				/* say we are changing the IO */
				write("Attempting to change Immigration Officer");
				bot._string_set(AW_WORLD_WELCOME_MESSAGE, text);
				rc = bot._world_attributes_change();
			}

			// update
			return rc ? Reason::Failed : Reason::Success;
		}
		
		int WorldCcioProcessor::process()
		{
			// find the text
			Umbra::VariantMap vm;
			String logText = vm.parse_string((LPCSTR)getRaw()).c_str();

			String new_io;
			new_io.Format("%s %s", bot._string(AW_WORLD_WELCOME_MESSAGE), logText);
		
			/* check size and do not bother adding it if it is too long */
			if (new_io.GetLength() > AW_MAX_ATTRIBUTE_LENGTH) 
			{
				logText.Format("Sorry, the message you have given would be too long to append by %d characters", 
					static_cast<int>(AW_MAX_ATTRIBUTE_LENGTH) - new_io.GetLength()); 
				write(logText);
				return Reason::BadArgument;
			}
		
			// give notice
			logText.Format("Attempting to append world IO");
			write(logText);
		
			// extend io
			bot._string_set(AW_WORLD_WELCOME_MESSAGE, new_io.Left(AW_MAX_ATTRIBUTE_LENGTH));
			int rc = bot._world_attributes_change();
			
			return rc ? Reason::Failed : Reason::Success;
		}

		int WorldImportProcessor::process()
		{
			WorldAttributes attr;
			attr.from_sdk(bot);

			// import
			int err = attr.loadTemplate(getFlagText());
			if (err)
			{
				write("Could not load the template file");
				return Reason::Failed;
			}

			// notify
			write("Importing world attributes file...");

			// assign
			attr.to_sdk(bot);
			bot._world_attributes_change();
			return Reason::Success;
		}

		BOOL TextToBOOL(String Text)
		{
			Text.MakeLower();
			if (Text == "yes" || Text == "on" || Text == "1") return TRUE;
			return FALSE;
		}

		int WorldMoveModeProcessor::process()
		{		
			// the stored keys
			if (getParamExists("fly")) 
			{ 
				bot._bool_set(AW_WORLD_ALLOW_FLYING, TextToBOOL(getParam("fly"))); 
			}
			if (getParamExists("shift")) 
			{ 
				bot._bool_set(AW_WORLD_ALLOW_PASSTHRU, TextToBOOL(getParam("shift"))); 
			}
			if (getParamExists("tp")) 
			{ 
				bot._bool_set(AW_WORLD_ALLOW_TELEPORT, TextToBOOL(getParam("tp"))); 
			}
		
			/* change the world attributes */
			write("Setting global world movement attributes");
			bot._world_attributes_change();
			return Reason::Success;
		}

		//int WorldAboutProcessor::process()
		//{
		//	/* variable keys AW_WORLD_EXPIRATION */
		//	String logText, createdText, expiresText;
		//	CTime tm64 = (__time64_t)bot._int(AW_WORLD_CREATION_TIMESTAMP);
		//	createdText = tm64.Format("%A, %B %d, %Y");
		//	tm64 = (__time64_t)bot._int(AW_WORLD_EXPIRATION);
		//	expiresText = ((bot._int(AW_WORLD_EXPIRATION) == 0) ? "Never" : tm64.Format("%A, %B %d, %Y"));
		//	logText.Format("%s is a P%d/%d world with ~%d objects created %s (expires: %s) - Cell Limit %d",
		//		bot._string(AW_WORLD_NAME),
		//		bot._int(AW_WORLD_SIZE), bot._int(AW_WORLD_MAX_USERS),bot._int(AW_WORLD_OBJECT_COUNT),
		//		createdText, expiresText, bot._int(AW_WORLD_CELL_LIMIT));
		//	write(logText);
		//	return Reason::Success;
		//}

		#define LIGHT_TRIPLET_LUMI(r, g, b)	((100.0 / 255.0) * ((r + g + b) / 3))
		int WorldLiProcessor::process() 
		{
			CStringA logText;
			logText.Format("World luminosity is %d%% ambient and %d%% directional",
				(int)LIGHT_TRIPLET_LUMI(bot._int(AW_WORLD_AMBIENT_LIGHT_RED), bot._int(AW_WORLD_AMBIENT_LIGHT_GREEN), bot._int(AW_WORLD_AMBIENT_LIGHT_BLUE)),
				(int)LIGHT_TRIPLET_LUMI(bot._int(AW_WORLD_LIGHT_RED), bot._int(AW_WORLD_LIGHT_GREEN), bot._int(AW_WORLD_LIGHT_BLUE)));					
			write(logText);
			return Reason::Success;
		}

		int WorldModifyProcessor::process()
		{
		//	// status
		//	String logText;
		//	KnownList good, bad;

		//	// get attributes
		//	WorldAttributes world;
		//	world.from_sdk(bot);

		//	// go through each key
		//	for (size_t i = 0; i < cmd_parser.size(); i++) 
		//	{
		//		if (world.set_by_name(cmd_parser.index(i).key, cmd_parser.index(i).text))
		//			good.push_back(cmd_parser.index(i).key);
		//		else
		//			bad.push_back(cmd_parser.index(i).key);
		//	}

		//	// push to world
		//	logText.Format("Pushing %d attribute(s) to world", (int)good.size());
		//	if (bad.size()) logText.AppendFormat(", %s are invalid identifiers", bad.toString("and"));
		//	write(logText);

		//	world.to_sdk(bot);
		//	bot._world_attributes_change();
			return Reason::Success;
		}

		class RightsHelper
		{
		public: // the list of attributes in the list
			static const int ENTER		= 0;
			static const int BUILD		= 1;
			static const int ED				= 2;
			static const int OBJECTS	= 3;
			static const int V4				= 4;
			static const int COMMANDS	= 5;
			static const int EJECT		= 6;
			static const int BOTS			= 7;
			static const int SPEAK		= 8;
			static const int VOICE		= 9;
			static const int PS				= 10;
			static const int TERRAIN	= 11;
			static const int MAX_RIGHTS	= 12;

		public: // class data on the array
			struct Config 
			{
				AW_ATTRIBUTE	attr;
				CStringA		long_name;

			public:
				Config(AW_ATTRIBUTE a, CStringA LongName)
				{
					attr		= a;
					long_name	= LongName;
				}

				Config()
				{
				}
			};

		public: // get the list of mappings
			typedef std::map<CStringA, int> StringIntMap;
			static StringIntMap& getNames()
			{
				static StringIntMap m;
				if (m.size() == 0)
				{
					m["enter"]		= ENTER;
					m["ent"]			= ENTER;
					m["build"]		= BUILD;
					m["ed"]				= ED;
					m["eminent"]	= ED;
					m["spo"]			= OBJECTS;
					m["objects"]	= OBJECTS;
					m["v4"]				= V4;
					m["spc"]			= COMMANDS;
					m["commands"]	= COMMANDS;
					m["eject"]		= EJECT;
					m["bots"]			= BOTS;
					m["speak"]		= SPEAK;
					m["voice"]		= VOICE;
					m["voip"]			= VOICE;
					m["ps"]				= PS;
					m["terrain"]	= TERRAIN;

				}

				// return the reference
				return m;
			}

			static int getIndexFromName(CStringA Name)
			{
				StringIntMap& m = getNames();
				StringIntMap::iterator itr = m.find(Name);
				if (itr == m.end()) return -1;
				else return itr->second;
			}

		public: // the list of mappings
			typedef std::vector<Config> ConfigMap;
			static ConfigMap getConfigMap()
			{
				ConfigMap m(MAX_RIGHTS);
				m[ENTER]		= Config(AW_WORLD_ENTER_RIGHT,				"enter");
				m[BUILD]		= Config(AW_WORLD_BUILD_RIGHT,				"build");
				m[ED]				= Config(AW_WORLD_EMINENT_DOMAIN_RIGHT,		"eminent_domain");
				m[OBJECTS]	= Config(AW_WORLD_SPECIAL_OBJECTS_RIGHT,	"special_objects");
				m[V4]				= Config(AW_WORLD_V4_OBJECTS_RIGHT,			"v4_objects");
				m[COMMANDS]	= Config(AW_WORLD_SPECIAL_COMMANDS_RIGHT,	"special_commands");
				m[EJECT]		= Config(AW_WORLD_EJECT_RIGHT,				"eject");
				m[BOTS]			= Config(AW_WORLD_BOTS_RIGHT,				"bots");
				m[SPEAK]		= Config(AW_WORLD_SPEAK_RIGHT,				"speak");
				m[VOICE]		= Config(AW_WORLD_VOIP_RIGHT,				"voip");
				m[PS]				= Config(AW_WORLD_PUBLIC_SPEAKER_RIGHT,		"public_speaker");
				m[TERRAIN]	= Config(AW_WORLD_TERRAIN_RIGHT,		"terrain");
				return m;
			}
		};

		template<class T>
		class RightsArray
		{			
		public: // array of indexes
			T list[RightsHelper::MAX_RIGHTS];
			T& operator[](size_t i) { return list[i]; }
		};

		class MiniMap
		{
		public: // add the list
			std::map<String, bool> reps;

		public: // list entry
			void add(String K) { reps[K] = true; }
			bool operator[](String K) { return reps.find(K) != reps.end(); }
		};

		int WorldGrantProcessor::process()
		{
			// groups
			// world/grant 334303
			SplitString args(getRaw(), ",", -1);
			MiniMap um;
			effect_all = false;
		
			// split the ranges
			SplitString citizens(args[0], "+", -1);
			std::vector<int> citlist;

			// lookup generator
			CitizenGroupLookupCompleter* pQuery = new CitizenGroupLookupCompleter(this, &CitizenLookup::g_Kernel);
			pQuery->preserveChildren();
			pQuery->open();
			
			// iterate each user to check if it is found
			for (size_t i = 0; i < citizens.size(); i++)
			{
				// copy and trim the name
				String name = citizens[i];
				name.Trim();

				// is this starting with ? then lookup by name
				if (name[0] == '#')
					pQuery->useDummyName("", abs(atoi(name.Mid(1))));
				else if (name[0] == '*')
					effect_all = true;
				else
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
				pQuery->deleteChildren();
				write("Please wait while looking up citizen numbers...");
				return Reason::CompletionDelayed;
			}
		}

		int	WorldGrantProcessor::resume()
		{
			// do we wish to edit everything
			SplitString args(getRaw(), ",", -1);
			CStringA allRights = args[1]; 
			allRights.Trim();
			
			// data
			RightsHelper::ConfigMap cfg = RightsHelper::getConfigMap();
			RightsArray<bool> triggered;
			std::vector<int> citlist;
			

			// set each right to be triggered to the rights check
			bool any = (allRights == "all");
			for (size_t i = 0; i < RightsHelper::MAX_RIGHTS; i++)
				triggered[i] = any;

			// go through each item and confirm it
			for (size_t i = 1; i < args.size(); i++) 
			{
				String t = args[i];
				t.Trim();
				
				// add each element
				int id = RightsHelper::getIndexFromName(t);
				if (id != -1)
					triggered[id] = true;
			}

			//
			// process each of the resultant citizenships
			//
			CIS::CitizenGroupLookupCompleter* pLookupResults = dynamic_cast<CitizenGroupLookupCompleter*>(resumption_args["results"].toRemoteClass());
			KnownList failed_lookups, added_numbers, changed_rights;
			CStringA  failed_reason;
			
			// include effected everything
			if (effect_all == true)
				added_numbers.push_back("*");

			// iterate each item in the results set
			for (size_t i = 0; i < pLookupResults->size(); i++)
			{
				if ((*pLookupResults)[i].result != 0)
				{
					failed_reason.Format("%s (%d)", (*pLookupResults)[i].name, (*pLookupResults)[i].result);
					failed_lookups.push_back(failed_reason);
				}
				else
				{
					citlist.push_back((*pLookupResults)[i].info.getNumber());
					added_numbers.push_back(ToString(citlist.back()));
				}
			}

			//
			// Edit each right
			//
			for (size_t i = 0; i < RightsHelper::MAX_RIGHTS; i++)
			{
				// require: the trigger to be valid
				if (triggered[i] == false)
					continue;

				// add to the rights
				RightsList r;
				r.parse(bot._string(cfg[i].attr));
				if (effect_all == true)
					r.everyone = true;
				else
				{
					for (size_t j = 0; j < citlist.size(); j++)
						r.add(citlist[j], citlist[j]);
				}
				changed_rights.push_back(cfg[i].long_name);

				// add onto the list
				bot._string_set(cfg[i].attr, r.list().c_str());
			}

			//
			// response
			//
			CStringA out;
			// every lookup failed
			if (failed_lookups.size() == pLookupResults->size() && effect_all == false)
			{
				out.Format("Unable to edit world rights lists", added_numbers.toString("and"), changed_rights.toString("and"));
			}
			else
			{
				out.Format("Successfully added %s to %s", added_numbers.toString("and"), changed_rights.toString("and"));
			}
			if (failed_lookups.size()) out.AppendFormat(", errors looking up %s.", failed_lookups.toString("and")); 
			write(out);

			bot._world_attributes_change();
			return Reason::Success;
		}

		//
		//
		//
		//
		//

		int WorldRevokeProcessor::process()
		{
			// groups
			// world/revoke 334303
			SplitString args(getRaw(), ",", -1);
			effect_all = false;
			MiniMap um;
		
			// split the ranges
			SplitString citizens(args[0], "+", -1);
			std::vector<int> citlist;

			// lookup generator
			CitizenGroupLookupCompleter* pQuery = new CitizenGroupLookupCompleter(this, &CitizenLookup::g_Kernel);
			pQuery->preserveChildren();
			pQuery->open();
			
			// iterate each user to check if it is found
			for (size_t i = 0; i < citizens.size(); i++)
			{
				// copy and trim the name
				String name = citizens[i];
				name.Trim();

				// is this starting with ? then lookup by name
				if (name[0] == '#')
					pQuery->useDummyName("", abs(atoi(name.Mid(1))));
				else if (name[0] == '*')
					effect_all = true;
				else
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

		int	WorldRevokeProcessor::resume()
		{
			// do we wish to edit everything
			SplitString args(getRaw(), ",", -1);
			CStringA allRights = args[1]; 
			allRights.Trim();
			
			// data
			RightsHelper::ConfigMap cfg = RightsHelper::getConfigMap();
			RightsArray<bool> triggered;
			std::vector<int> citlist;
			

			// set each right to be triggered to the rights check
			bool any = (allRights == "all");
			for (size_t i = 0; i < RightsHelper::MAX_RIGHTS; i++)
				triggered[i] = any;

			// go through each item and confirm it
			for (size_t i = 1; i < args.size(); i++) 
			{
				String t = args[i];
				t.Trim();
				
				// add each element
				int id = RightsHelper::getIndexFromName(t);
				if (id != -1)
					triggered[id] = true;
			}

			//
			// process each of the resultant citizenships
			//
			CIS::CitizenGroupLookupCompleter* pLookupResults = dynamic_cast<CitizenGroupLookupCompleter*>(resumption_args["results"].toRemoteClass());
			KnownList failed_lookups, added_numbers, changed_rights;
			CStringA  failed_reason;

			// include effected everything
			if (effect_all == true)
				added_numbers.push_back("*");

			// iterate each item in the results set
			for (size_t i = 0; i < pLookupResults->size(); i++)
			{
				if ((*pLookupResults)[i].result != 0)
				{
					failed_reason.Format("%s (%d)", (*pLookupResults)[i].name, (*pLookupResults)[i].result);
					failed_lookups.push_back(failed_reason);
				}
				else
				{
					citlist.push_back((*pLookupResults)[i].info.getNumber());
					added_numbers.push_back(ToString(citlist.back()));
				}
			}

			//
			// Edit each right
			//
			for (size_t i = 0; i < RightsHelper::MAX_RIGHTS; i++)
			{
				// require: the trigger to be valid
				if (triggered[i] == false)
					continue;

				// add to the rights
				RightsList r;
				r.parse(bot._string(cfg[i].attr));
				if (effect_all == true)
					r.everyone = false;
				else
				{
					for (size_t j = 0; j < citlist.size(); j++)
						r.remove(citlist[j]);
				}
				changed_rights.push_back(cfg[i].long_name);

				// add onto the list
				bot._string_set(cfg[i].attr, r.list().c_str());
			}

			//
			// response
			//
			CStringA out;
			// every lookup failed
			if (failed_lookups.size() == pLookupResults->size() && effect_all == false)
			{
				out.Format("Unable to edit world rights lists");
			}
			else
			{
				out.Format("Successfully revoked %s from %s", changed_rights.toString("and"), added_numbers.toString("and"));
			}
			if (failed_lookups.size()) out.AppendFormat(", errors looking up %s.", failed_lookups.toString("and")); 
			write(out);

			bot._world_attributes_change();
			return Reason::Success;
		}

		//
		//
		//
		//
		//

		int WorldShowrightsProcessor::process()
		{
			// static data
			RightsHelper::ConfigMap cfg = RightsHelper::getConfigMap();

			if (getFlag("-resolve") == true)
			{
				// only resolve each citizen once
				std::map<int, bool> needed;
				for (size_t i = 0; i < RightsHelper::MAX_RIGHTS; i++)
				{
					// create the rights list
					RightsList list;
					list.parse(bot._string(cfg[i].attr));
				
					// each number
					for (size_t j = 0; j < list.rights.size(); j++)
					{
						// reference
						RightsList::Entry e = list.rights[j];
						needed[e.from] = true;
						needed[e.to]   = true;
					}
				}

				// lookup generator
				CitizenGroupLookupCompleter* pQuery = new CitizenGroupLookupCompleter(this, &CitizenLookup::g_Kernel);
				pQuery->preserveChildren();
				pQuery->open();

				// add each element
				for (std::map<int, bool>::iterator itr = needed.begin(); itr != needed.end(); itr++)
					pQuery->lookupByNumber(itr->first);

				// close each element
				if (pQuery->getOutstanding() == 0)
				{
					pQuery->close();
					return Reason::Success;
				}
				else
				{
					write(String("Please wait while I resolve citizen numbers") + (needed.size() > 10 ? " (this may take a while)..." : "..."));
					pQuery->deleteChildren();
					return Reason::CompletionDelayed;
				}
			}
			else
			{
				// list each rights
				for (size_t i = 0; i < RightsHelper::MAX_RIGHTS; i++) {
					CStringA line;
					line.Format("%s: %s", cfg[i].long_name, bot._string(cfg[i].attr));
					write(line);
				}
			}

			// ok
			return Reason::Success;
		}

		int WorldShowrightsProcessor::resume()
		{
			// lookup the arguments
			CIS::CitizenGroupLookupCompleter* pLookupResults = dynamic_cast<CitizenGroupLookupCompleter*>(resumption_args["results"].toRemoteClass());
			RightsHelper::ConfigMap cfg = RightsHelper::getConfigMap();	// get a map of names

			// copy each results into a map
			std::map<int, CStringA> citnames;
			for (size_t i = 0; i < pLookupResults->size(); i++)
			{
				CitizenLookup::GroupLookupHelper& u = (*pLookupResults)[i];
				if (u.result == 0)
				{
					citnames[u.info.getCitizen()] = u.info.getName();
				}
				else
				{
					citnames[u.number].Format("NAC #%d", u.info.getCitizen());
				}
			}

			// go through each right
			for (size_t i = 0; i < RightsHelper::MAX_RIGHTS; i++)
			{
				// create the rights list
				RightsList list;
				KnownList  known;
				CStringA   temp;

				// parse the user list
				list.parse(bot._string(cfg[i].attr));
				if (list.everyone) known.push_back("[*]");

				// each right
				for (size_t j = 0; j < list.rights.size(); j++)
				{
					// reference
					RightsList::Entry e = list.rights[j];
					if (e.to == e.from)
					{
						temp.Format("%s%s", citnames[e.from], e.deny ? " (denied)" : "");
					}
					else
					{
						temp.Format("group[%s ~ %s]%s", citnames[e.from], citnames[e.to], e.deny ? " (denied)" : "");
					}

					// add to list
					known.push_back(temp);
				}

				// list the rights
				temp.Format("%s:\t%s", cfg[i].long_name, known.size() == 0 ? "[empty]" : known.toString("and"));
				write(temp);
			}

			// ok
			return Reason::Success;
		}

		//
		//
		//
		//
		//

		int WorldEpProcessor::process()
		{
			// set flag
			if (getFlag("-set") == true)
			{
				// coordinates
				Location pos(getFlagText(), this_avatar);
				if (pos.isValid() == false)
				{
					write("Those enter point coordinates are invalid");
				}
				else
				{
					// set the new position and copy to attributes
					pos.setXYZ(pos.getX(), pos.getY() / 10, pos.getZ(), pos.getYaw());
					bot._string_set(AW_WORLD_ENTRY_POINT, pos.getCoordinates());
					
					// change message
					write("Changing enter point...");
					bot._world_attributes_change();

					// notify
					if (getFlag("-notify") == true)
						bot.consoleBroadcast("The world enter point has been changed", 0x992222, CM_ITALIC | CM_SUPPRESSED);
				}
			}

			// ok
			return Reason::Success;
		}
	}
}