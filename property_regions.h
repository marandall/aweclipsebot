#pragma once
#include <map>
#include "property_region.h"

namespace Property
{
	class Regions
	{
	public: // type defenitions for Regions
		typedef std::map<int, Region>		MapType;
		typedef MapType::iterator			iterator;
		typedef MapType::const_iterator		const_iterator;
		typedef MapType::reverse_iterator	reverse_iterator;
		typedef MapType::pointer			pointer;
		typedef MapType::value_type			value_type;

	protected: // internal lists
		MapType				m_regions;

	public: // access to the list
		iterator			begin(void)		  { return m_regions.begin();		}
		iterator			end	 (void)		  { return m_regions.end();			}
		const_iterator		begin(void)	const { return m_regions.begin();		}
		const_iterator		end  (void) const { return m_regions.end();			}
		reverse_iterator	rbegin(void)	  { return m_regions.rbegin();		}
		reverse_iterator	rend (void)		  { return m_regions.rend();		}

	public: // Region access functions
		iterator			find		(int Region_X, int Region_Z);
		iterator			find		(int Region_Iterator);
		Region&				operator[]	(int Id);
		Region&				GetRegion	(int X, int Z);
		Region&				GetRegion	(int Region_Iterator);
		time_t				getLastUpdated(int X, int Y);

	public: // control functions
		size_t				size (void) const { return m_regions.size();		}
		void				clear(void)		  { return m_regions.clear();		}
	};
}