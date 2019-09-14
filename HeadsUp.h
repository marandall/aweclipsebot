#pragma once
#include "awsdk_hudelement.h"
#include "headsup_configuration_ag.h"
#include "headsup_storedtrayitem.h"
#include "stringenum.h"

class Avatar;
class EclipseBot;

namespace HeadsUp
{
	struct Duplicate
	{
		int index;
		const char* var;
	};

	class Config : public HeadsUp::AutoTemplates::ConfigurationData
	{
	public: // enumerations
		enum {
			POSITION_TOP,
			POSITION_TOP_LEFT,
			POSITION_TOP_RIGHT,
			POSITION_BOTTOM,
			POSITION_BOTTOM_LEFT,
			POSITION_BOTTOM_RIGHT
		};

		static StringEnum& Positions() {
			static StringEnum::Pair p[] = {
				{ POSITION_TOP,						"Top" },
				{ POSITION_TOP_LEFT,			"Top Left" },
				{ POSITION_TOP_RIGHT,			"Top Right" },
				{ POSITION_BOTTOM,				"Bottom" }, 
				{ POSITION_BOTTOM_LEFT,		"Bottom Left" },
				{ POSITION_BOTTOM_RIGHT,	"Bottom Right" } };
			static StringEnum x(p);
			return x;
		}

	public: // config instance
		static Config& getInstance() { static Config c; return c; }

	public: // the list of items
		typedef std::list<StoredTrayItem> TrayItemArray;
		TrayItemArray stored_items;

	public: // load each of the hud elements also
		void Load(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			__super::Load(SerializerSystem, IniKey);

			// erase each of the items
			stored_items.clear();

			// load each icon
			int item_count = atoi(SerializerSystem[IniKey]["stored_count"]);
			for (int i = 0; i < item_count; i++)
			{
				String loader;
				loader.Format("%s_stored_%d", IniKey, i);

				// load the template
				StoredTrayItem sti;
				sti.Load(SerializerSystem, loader);
				stored_items.push_back(sti);
			}
		}
		
		void Save(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			ini_exchange(SerializerSystem, AW::WRITE_TO_INI, IniKey);

			// save the number of items
			SerializerSystem[IniKey]["stored_count"] = ToString((int)stored_items.size());

			// save each item
			int item_count = 0;
			for (TrayItemArray::iterator itr = stored_items.begin(); itr != stored_items.end(); itr++)
			{
				String loader;
				loader.Format("%s_stored_%d", IniKey, item_count++);

				// save the template file
				itr->Save(SerializerSystem, loader);
			}
		}
	};
};