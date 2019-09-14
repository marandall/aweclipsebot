#pragma once
#include <list>
#include "cis_configscriptedcommand_ag.h"

namespace CIS
{
	//
	// scripted command
	//

	class ScriptedCommand : public Config::AutoTemplates::ScriptedCommandData
	{
	};

	//
	// The controller for however many script commands exist
	//

	class ScriptCommands : public IConfigLoader
	{
	public: // the list of commands
		typedef std::list<ScriptedCommand> ListType;
		typedef ListType::iterator Iterator;

	public: // the list of items
		ListType list;

	public: // access options
		Iterator begin() 
		{ 
			return list.begin(); 
		}

		Iterator end() 
		{ 
			return list.end(); 
		}

		void clear() 
		{ 
			return list.clear();
		}

		size_t size() const
		{ 
			return list.size(); 
		}

		int push_back(ScriptedCommand& C)
		{
			list.push_back(C);
			return list.back().getAutoNumber();
		}

	public: // searching routines
		ScriptedCommand* getAuto(int ID)
		{
			// search for the item
			for (Iterator itr = begin(); itr != end(); itr++)
				if (itr->getAutoNumber() == ID)
					return &(*itr);

			// no item found
			return 0;
		}

		ScriptedCommand* getId(const char* Name)
		{
			// search for the item
			for (Iterator itr = begin(); itr != end(); itr++)
				if (itr->getId().CompareNoCase(Name) == 0)
					return &(*itr);

			// no item found
			return 0;
		}

	public: // file loading and saving routines
		void Load(Serializer& Sz, const char* IniKey = NULL)
		{
			// erase the list
			clear();

			// write section header
			const char* section = "scripted_commands";
			int count = atoi(Sz[section]["count"]);

			// read each item
			for (int i = 0; i < count; i++)
			{
				ScriptedCommand sc;
				sc.Load(Sz, String("scripted_commands_") + ToString(i));
				push_back(sc);
			}
		}
		
		void Save(Serializer& Sz, const char* IniKey = NULL)
		{
			// write section header
			const char* section = "scripted_commands";
			Sz[section]["count"] = ToString((int)list.size());

			// read each item
			int id = 0;
			for (Iterator itr = begin(); itr != end(); itr++)
			{
				itr->Save(Sz, String("scripted_commands_") + ToString(id++));
			}
		}

	public: // static instance
		static ScriptCommands& getInstance()
		{
			static ScriptCommands sc;
			return sc;
		}
	};
}