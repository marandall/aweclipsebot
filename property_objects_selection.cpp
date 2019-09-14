#include "stdafx.h"
#include "property_objects.h"
#include "awsdk_location.h"

namespace Property
{
	void region_object_select(Regions& RG, int Index)
	{
		Region& rgn = RG.GetRegion(Index);
		rgn.selected++;
		rgn.last_updated = time64(0);
	}

	void region_object_unselected(Regions& RG, int Index)
	{
		Region& rgn = RG.GetRegion(Index);
		rgn.selected--;
		rgn.last_updated = time64(0);
	}

	bool Objects::update_trackers_selected(ObjectData& Object)
	{
		if (track_cells)
			region_object_select(cells, Object.getCellIndex());
		if (track_sectors)
			region_object_select(sectors, Object.getSectorIndex());
		return true;
	}

	bool Objects::update_trackers(ObjectData& Object)
	{
		Region& rgn = cells.GetRegion(Object.getCellIndex());
		rgn.last_updated = time64(0);
		return true;
	}

	bool Objects::update_trackers_unselected(ObjectData& Object)
	{
		if (track_cells)
			region_object_unselected(cells, Object.getCellIndex());
		if (track_sectors)
			region_object_unselected(sectors, Object.getSectorIndex());
		return true;
	}

}


