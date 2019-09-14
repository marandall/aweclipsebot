#pragma once
#include <list>

namespace AW
{
	// GroupMap represents a list of key/value pairs that can be used for serialization
	// 
	// 

	class GroupMap
	{
	public: // structure
		struct Entry
		{
			String Group;
			String Child;
			String Label;
			String IniGroup;
		};

	public: // list typedefs
		typedef std::list<Entry>			List;
		typedef List::iterator				iterator;
		typedef List::const_iterator		const_iterator;
		typedef List::reference				reference;

	public: // the group list
		typedef std::list<String>			GroupList;
		typedef GroupList::iterator			g_iterator;
		typedef GroupList::const_iterator	g_const_iterator;
		typedef GroupList::reference		g_reference;

	public: // var and access operators
		List				m_list;
		iterator			begin()			{ return m_list.begin();	}
		const_iterator		begin() const	{ return m_list.begin();	}
		iterator			end()			{ return m_list.end();		}
		const_iterator		end()	const	{ return m_list.end();		}
		size_t				size() const	{ return m_list.size();		}

	public: // var and access operators
		GroupList			m_groups;
		g_iterator			g_begin()		{ return m_groups.begin();	}
		g_const_iterator	g_begin() const	{ return m_groups.begin();	}
		g_iterator			g_end()			{ return m_groups.end();	}
		g_const_iterator	g_end()	const	{ return m_groups.end();	}
		size_t				g_size() const	{ return m_groups.size();	}

	public: // modification
		void push_back(String Group, String IniGroup, String Child, String Label)
		{
			bool b_found = false;
			for (g_iterator gPtr = g_begin(); gPtr != g_end(); gPtr++)
			{
				if ((*gPtr) == Group)
				{
					b_found = true;
					break;
				}
			}

			// check if found
			if (b_found == false)
				m_groups.push_back(Group);

			// insert each element
			Entry entry;
			entry.Child	= Child;
			entry.Group = Group;
			entry.IniGroup = IniGroup;
			entry.Label = Label;
			m_list.push_back(entry);
		}
	};
}