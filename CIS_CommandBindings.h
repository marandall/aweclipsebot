#pragma once
#include <vector>
#include "serializer.h"

namespace CIS
{
	class CommandBindings :
		public IConfigLoader
	{
	public:
		struct Binding
		{
			CStringA	id;
			CStringA	command;
			bool		remote;
			CStringA	right_required;
		};

	public: // the iterator
		typedef std::vector<Binding>	List;
		typedef List::iterator			iterator;

	public: // access
		List list;
		Binding& operator[](size_t i) { return list[i]; }
		size_t	 size() { return list.size(); }

	public: // loading and saving
		void Load(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			list.clear();

			// load the key
			const char* KeyName = "CommandBindings";
			int count = SerializerSystem.getKeyInt(KeyName, "count");
			CStringA child;

			// go through each key
			for (int i = 0; i < count; i++)
			{
				child.Format("binding%d", i);
				Binding b;
				b.id				= SerializerSystem[KeyName][child		 ];
				b.command			= SerializerSystem[KeyName][child + "cmd"];
				b.remote			= SerializerSystem[KeyName][child + "rem"] == "1";
				b.right_required	= SerializerSystem[KeyName][child + "rr" ];
				list.push_back(b);
			}
		}
			
		virtual void Save(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			// write count
			const char* KeyName = "CommandBindings";
			SerializerSystem[KeyName]["count"] = ToString(list.size());
			CStringA child;
			
			// go through each item
			for (int i = 0; i < (int)list.size(); i++)
			{
				child.Format("binding%d", i);
				Binding& b = list[i];

				SerializerSystem[KeyName][child]		 = b.id;
				SerializerSystem[KeyName][child + "cmd"] = b.command;
				SerializerSystem[KeyName][child + "rem"] = b.remote ? "1" : "0";
				SerializerSystem[KeyName][child + "rr" ] = b.right_required;
			}
		}

		Binding* getBinding(const char* Text)
		{
			for (size_t i = 0; i < size(); i++)
				if (list[i].id.CompareNoCase(Text) == 0)
					return &list[i];
			return 0;
		}

		bool unbind(const char* Text)
		{
			for (size_t i = 0; i < size(); i++)
				if (list[i].id.CompareNoCase(Text) == 0)
				{
					list.erase(list.begin() + i);
					return true;
				}
			return false;
		}

		static CommandBindings& getInstance()
		{
			static CommandBindings b;
			return b;
		}

	};
}