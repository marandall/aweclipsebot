#include "stdafx.h"
#include "property_objects.h"
#include "property_object.h"
#include "property_filters.h"

namespace Property
{
	int	Objects::run_filter(Filters::Filter* Options, bool Select)
	{
		// counter of objects
		int mod_count = 0;

		// for each object...
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
		{
			// test selection
			if (objPtr->getSelected() == Select)
				continue;

			// execute rest of code here
			if (Options->run(*objPtr))
			{
				objPtr->changeSelected(Select);
				update_trackers_selected(*objPtr);
				mod_count++;
			}
		}
		
		// return result
		return mod_count;
	}

	int	Objects::deleteSelected(bool Selected)
	{
		// counter of objects
		int mod_count = 0;

		// for each object...
		for (iterator objPtr = begin(); objPtr != end(); )
		{
			// test selection
			if (objPtr->getSelected() == Selected)
			{
				objPtr = m_objects.erase(objPtr);
				mod_count++;
			}
			else
			{
				objPtr++;
			}
		}
		
		// return result
		rebuild_trackers();
		return mod_count;
	}

	int	Objects::invertSelection()
	{
		// counter of objects
		int mod_count = 0;

		// for each object...
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			objPtr->changeSelected(!objPtr->getSelected());
		
		// return result
		rebuild_trackers();
		return mod_count;
	}

	int	Objects::clearSelection()
	{
		// counter of objects
		int mod_count = 0;

		// for each object...
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			objPtr->changeSelected(false);
		
		// return result
		rebuild_trackers();
		return mod_count;
	}

}