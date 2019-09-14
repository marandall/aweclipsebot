#pragma once
#include <vector>
#include "avatar.h"
#include "awsdk_support.h"
#include "FileLoader.h"

namespace Comms
{
	/**
		Basic filter list of items to filter
	*/
	class FilterList
	{
	protected:
		std::vector<String>	m_list;

	public:
		bool IsFiltered(String Key)
		{
			// everything filtered is lowercase
			Key.MakeLower();

			// zero
			if (Key.GetLength() == 0)
				return false;

			// search each
			for (size_t i = 0; i < m_list.size(); i++)
				if (m_list[i].CompareNoCase(Key) == 0)
					return true;
			return false;
		}

		bool IsFilteredAnywhere(String Key)
		{
			// everything searched is lowercase
			Key.MakeLower();

			for (size_t i = 0; i < m_list.size(); i++)
				if (Key.Find(m_list[i]) != -1 && m_list[i].GetLength() > 0)
					return true;
			return false;
		}

		bool existInCharset(char p, const char* set)
		{
			for (size_t i = 0; i < strlen(set); i++)
				if (set[i] == p)
					return true;
			return false;
		}

		bool IsSmartFiltered(String Key)
		{
			char* start = const_cast<char*>((LPCSTR)Key);
			char* p     = start;

			// grammar
			while (true)
			{
				// go through each item to tokenise
				start = p;
				while ( existInCharset(*p,"\t\n\r !()~[],.:;#~\\/<>") == false && *p != 0)
					p++;

				// copy group
				std::string word(start, p - start);
				if (IsFiltered(word.c_str())) 
					return true;

				// null check
				if (*p == 0) break;
				p++;
			}

			// no bad words
			return false;
		}

		void CreateFilter(String Key)
		{
			Key.MakeLower();
			if (IsFiltered(Key)) return;
			m_list.push_back(Key);
		}

		void Clear()
		{
			m_list.clear();
		}

		void Remove(String Key)
		{
			for (size_t i = 0; i < m_list.size(); i++) {
				if (!m_list[i].CompareNoCase(Key)) {
					m_list.erase(m_list.begin() + i);
					return;
				}
			}
		}

		void CreateFromList(String Key)
		{
			Clear();

			/* import each list */
			AW::SplitString keys(Key, "\n", -1);
			for (size_t i = 0; i < keys.size(); i++)
			{
				String sMsg = keys[i];
				sMsg.Trim();
				if (sMsg.GetLength() > 0)
					CreateFilter(sMsg);
			}
		}

		String CreateList()
		{
			String temp;
			for (size_t i = 0; i < m_list.size(); i++)
				temp.AppendFormat("%s\r\n", m_list[i]);
			return temp;
		}

		bool Save(String Path)
		{
			FileWriter file(Path);
			if (!file.IsOK()) return false;
			for (size_t i = 0; i < m_list.size(); i++)
			{
				file.WriteLine(m_list[i]);	
			}
			return true;
		}

		bool Load(String Path)
		{
			FileReader file(Path);
			if (!file.IsOK()) return false;
			for (size_t i = 0; i < file.size(); i++)
			{
				String s = file[i];
				s.Trim();
				if (s.GetLength() != 0)
				{
					CreateFilter(file[i]);
				}
			}
			return true;
		}
	};

	/**
		Filter list for the consoles
	*/
	class ConsoleFilter : 
		public FilterList
	{

	};

	/* filter for the avatars */
	class ChatFilter :
		public FilterList
	{
	public:
		bool IsFiltered(Avatar& User)
		{
			for (size_t i = 0; i < m_list.size(); i++)
				if (User.IsPattern(m_list[i]))
					return true;
			return false;
		}
	};

	/* vars for lookup */
	extern ConsoleFilter	g_ConsoleFilter;
	extern ChatFilter		g_ChatFilter;
}