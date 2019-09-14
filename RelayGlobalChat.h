#pragma once
#include "RelayGlobalChat_AG.h"			// auto generated data file
#include "RelayColorTableEntry_AG.h"	// auto generated data file

namespace ChatControllers
{
	class ColorTableEntry : public AutoTemplates::ColorTableEntryData
	{
	protected:
		int		auto_id;

	public:
		ColorTableEntry()
		{
			static int auto_id = 0;
			this->auto_id = auto_id++;
		}

		int getAutoId() { return auto_id; }
	};

	class GlobalUserStyles
	{
	public: // information
		typedef std::list<ColorTableEntry>	ListType;
		typedef ListType::iterator			iterator;
		typedef ListType::reverse_iterator	reverse_iterator;

	public: // impl
		ListType	list;
		iterator	begin()		{ return list.begin();	}
		iterator	end()		{ return list.end();	}
		void		clear()		{ list.clear();			}
		void		push_back(ColorTableEntry e) { list.push_back(e); }

	public: // routines
		ColorTableEntry* getEntry(const char* Data);
		ColorTableEntry* getAuto(int AutoID);
		void		load		(const char* FilePath);
		void		save		(const char* FilePath);
		void		updateAvatar(Avatar& User);
	};

	// kernel class
	class GlobalChatKernel : public AutoTemplates::GlobalChatData
	{
	public: // process handling routines
		errno_t handle_avatar_add(Avatar& User);
		errno_t handle_avatar_delete(Avatar& User);
		errno_t handle_chat(Avatar& User, String Text, int Method);

	protected:
		GlobalUserStyles	gus;

	public: // get style
		GlobalUserStyles&	getUserStyles() { return gus; }
	};
}