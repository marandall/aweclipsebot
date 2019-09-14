#pragma once
#include <list>
#include "Serializer.h"

class Avatar;

namespace ChatControllers
{
	class MuteList
	{
	public: // type defenitions
		typedef std::list<String>		List;
		typedef List::iterator			iterator;
		typedef List::const_iterator	const_iterator;

	protected: // variables
		List	 m_list;

	public: // editors and iterator
		void	 push_back	(String E)	{ m_list.push_back   (E);	}
		const_iterator begin(void) const{ return m_list.begin();	} 
		const_iterator end	(void) const{ return m_list.end	 ();	}
		iterator begin		(void)		{ return m_list.begin();	} 
		iterator end		(void)		{ return m_list.end	 ();	}
		size_t	 size		(void) const{ return m_list.size ();	}
	
	public: // file syncronisation
		void	 load(Serializer& Sz);
		void	 save(Serializer& Sz);

	public: // checking
		bool	 is_filtered(Avatar& User) const;
		bool	 key_exists(String Key) const;
		void	 remove_key(String Key);
	};
}