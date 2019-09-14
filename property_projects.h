#pragma once
#include <list>
#include "property_project.h"

// forward references
namespace AW { class Bot; }

namespace Property
{
	class Projects
	{
	public: // type defenitions for a project
		typedef std::list<Project>			ListType;
		typedef ListType::iterator			iterator;
		typedef ListType::const_iterator	const_iterator;
		typedef ListType::reverse_iterator	reverse_iterator;
		typedef ListType::pointer			pointer;

	protected: // internal lists
		ListType			m_list;

	public: // access to the list
		iterator			begin(void)		  { return m_list.begin();	}
		iterator			end	 (void)		  { return m_list.end();	}
		const_iterator		begin(void)	const { return m_list.begin();	}
		const_iterator		end  (void) const { return m_list.end();	}
		reverse_iterator	rbegin(void)	  { return m_list.rbegin();	}
		reverse_iterator	rend (void)		  { return m_list.rend();	}
		size_t				size (void) const { return m_list.size();	}

	public: // list searching
		Project*			Find(const char* ProjectId);
		Project*			checkProjectPtr(Project* P);
		
	public: // list modification
		bool				Erase(const char* ProjectId);
		Project*			CreateProject(const char* ProjectId);

	public: // specifical circumstance modification
		void				fail_pending_objects(int ReasonCode, AW::Bot& Client);
	};

	extern Projects g_Projects;
}