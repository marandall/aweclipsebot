#include "stdafx.h"
#include "property_query_5x5.h"
#include "property_query_queue.h"
#include "property_objects.h"

namespace Querying
{
	MultiQueue& MultiQueue::getInstance()
	{
		static MultiQueue q(&bot);
		return q;
	}

	String seq_debug(int v[5][5])
	{
		String t;
		for (int i = 0; i < 5; i++)
		{
			t.AppendFormat("[");
			for (int j = 0; j < 5; j++)
			{
				t.AppendFormat("%d,", v[i][j]);
			}
			t.AppendFormat("],");
		}

		return t;
	}

	MultiQueue::MultiQueue(CommonBot* C)
	{
		base_bot = C;
	}

	int	MultiQueue::queryAround(AW::Location Mid, int Width, int Height)
	{
		AW::Location hi(Mid.getX() + Width * 500, 0, Mid.getZ() + Height * 500, 0);
		AW::Location lo(Mid.getX() - Width * 500, 0, Mid.getZ() - Height * 500, 0);
		return queryBetween(hi, lo);
	}

	int MultiQueue::queryBetween(AW::Location NW, AW::Location SE)
	{
		const int inset_size = 13000;
		const int query_size = 40000;

		// force the high and low points
		AW::Location hi = NW.toHighPoint(SE);
		AW::Location lo = NW.toLowPoint(SE);

		// boundary coordinates
		bounds_nw = hi;
		bounds_se = lo;

		// start 15 coordinates in from the top
		for (int x = (lo.getX() + inset_size); x <= hi.getX() + inset_size; x+= query_size)
		{
			for (int z = (lo.getZ() + inset_size); z <= hi.getZ() + inset_size; z+= query_size)
			{
				AW::Location pos(x, 0, z, 0);
				queue.push_back(pos);
			}
		}

		// begin querying the first item
		return queryNext();
	}

	int MultiQueue::queryNext()
	{
		// we are completed
		if (queue.size() == 0)
			return -1;

		// query segment
		_seg = QuerySegment();

		// find the next item
		AW::Location pos = queue.back();
		queue.pop_back();

		// select the desired segment
		_seg.sector_x = aw_sector_from_cell(pos.getCellX());
		_seg.sector_z = aw_sector_from_cell(pos.getCellZ());		
		base_bot->_query_5x5(_seg.sector_x, _seg.sector_z, _seg.sequence);

		// begin another query
		return 0;
	}

	void MultiQueue::handleCellBegin(void)
	{
		// update the current cell
		int cell_x = aw_int(AW_CELL_X);
		int cell_z = aw_int(AW_CELL_Z);

		// re-calculate sectors
		int sector_x = aw_sector_from_cell(cell_x) - _seg.sector_x + 2;
		int sector_z = aw_sector_from_cell(cell_z) - _seg.sector_z + 2;

		// store the cell sequence
		_seg.sequence[sector_z][sector_x] = base_bot->_int(AW_CELL_SEQUENCE);
	}

	void MultiQueue::handleCellEnd(void)
	{
	}
	
	void MultiQueue::handleCallbackQuery(int rc)
	{
		// is the query complete
		BOOL is_complete = base_bot->_bool(AW_QUERY_COMPLETE);
		if (is_complete == TRUE)
		{
			queryNext();
		}
		else
		{
			base_bot->_query_5x5(_seg.sector_x, _seg.sector_z, _seg.sequence);
		}
	}

	void MultiQueue::handleCellObject(ObjectData& OD)
	{
		if (OD.PointInRect_2D(bounds_nw, bounds_se) == true)
		{
			bot.getLiveCache().insert_object(OD);
		}
	}
}