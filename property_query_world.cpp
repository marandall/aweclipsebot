#include "stdafx.h"
#include "property_query_world.h"
#include "property_objects.h"
#include "property_projects.h"
#include "helpers_time.h"

namespace Querying
{
	WorldQuery::WorldQuery(CommonBot* C)
	{
		base_bot		= C;

		// reset averages
		count						= 0;
		averages_temp		= 0;
		start_time			= 0;
		iterator_switch	= 0;
		iterator_next		= 0;
		memset(averages, 0, sizeof averages);
		region_bounding	= false;
	}

	int WorldQuery::queryNext(int Itr, int Index)
	{
		// looks like we are expecting something
		pending_iterators.push_back(Index);

		// iterators.push_back(Itr);
		base_bot->_int_set(AW_CELL_ITERATOR, Itr);
		return base_bot->_cell_next();
	}

	int WorldQuery::queryNextBlock(bool IsFirst)
	{
		// if the iterator is in the value
		if (iterator_next >= (int)iterators.size())
		{
			// would seem to indicate we are done
			if (pending_iterators.size() == 0 && IsFirst == false)
			{
				String temp;
				temp.Format("#System:\tWorld Query has completed in %s with %d objects.",
						Helpers::Time::SecondsToTimeStr(getDuration()),
						count);

				completed = true;
				end_time = _time64(0);
				WriteMsg(temp, 0, OT_MAIN | MF_BOLD);
			}

			// push the final data to disk
			flush_write_cache();
			return -1;
		}


		// lookup the next iterator
		IteratorRange& itr = iterators[iterator_next];
		unsigned short bx = (unsigned short)itr.begin_x;
		unsigned int itr_value = (bx << 16) & 0xFFFF0000;

		// lookup the next user
		int rc = queryNext( itr_value, (int)iterator_next );
		itr.timer_start = _time64(0);

		// next index iterator
		return iterator_next++;
	}

	int	WorldQuery::start()
	{
		// reset averages
		count								= 0;
		averages_temp				= 0;

		// progress trackers
		start_time					= time64(0);
		end_time						= 0;
		completed						= false;

		// internal mechnaics
		memset(averages, 0, sizeof averages);
		processing_cells		= false;
		region_bounding			= false;
		this_iterator_done	= false;
		iterator_next				= 0;

		// prime the sdk for iterated query
		base_bot->_int_set (AW_CELL_ITERATOR, 0);
		base_bot->_bool_set(AW_CELL_COMBINE,  1);
		int world_size = base_bot->_int(AW_WORLD_SIZE);

		// divide the ranges up
		int cols_per_query = 5;
		iterators.clear();
		pending_iterators.clear();

		// update avatars
		if (world_size > 10)		cols_per_query = 5;
		if (world_size > 10)		cols_per_query = 5;
		if (world_size > 50)		cols_per_query = 10;
		if (world_size > 100)		cols_per_query = 20;
		if (world_size > 200)		cols_per_query = 30;
		if (world_size > 1000)	cols_per_query = 100;
		if (world_size > 10000)	cols_per_query = 200;

		// query from the start until the end
		for (int x = 0; x < world_size; x+= cols_per_query)
		{
			IteratorRange r(x, x + cols_per_query);
			iterators.push_back(r);
		}

		// tag the back end also
		iterators.back().end_x = -world_size + 2;

		// from the last to the zero
		for (int x = -world_size + 2; x < 0; x+= cols_per_query)
		{
			IteratorRange r(x, x + cols_per_query);
			iterators.push_back(r);
		}

		// next item
		for (size_t i = 0; i < 8; i++)
			queryNextBlock(true);
		return 0;
	}

	int	WorldQuery::queryBetween(AW::Location Top, AW::Location Bottom)
	{
		// load up item
		AW::Location hi = Top.toHighPoint(Bottom);
		AW::Location lo = Top.toLowPoint(Bottom);
		region_bounding	= true;

		// lowest point in the world
		int start_x = lo.getCellX();
		if (start_x < base_bot->_int(AW_WORLD_SIZE))
			start_x = -base_bot->_int(AW_WORLD_SIZE);

		// scan the area between in 10 cell units
		int cols_per_query = 10;
		for (int x = start_x; x <= hi.getCellX(); x+= cols_per_query)
		{
			IteratorRange r(x, x + cols_per_query);
			iterators.push_back(r);
		}

		// load each item
		for (size_t i = 0; i < 5; i++)
			queryNextBlock();

		return 0;
	}

	void WorldQuery::handleCellBegin(void)
	{
		cur_cell_x	= base_bot->_int(AW_CELL_X);
		cur_cell_z	= base_bot->_int(AW_CELL_Z);

		// this is the item we are responding to
		int index = pending_iterators.front();
		IteratorRange& r = iterators[index];

		// termination
		this_iterator_done = false;
		if (region_bounding == true)
		{

			processing_cells = true;
		}
		else
		{
			bool term = false;
			char ts = '0';
			
			// standard element where the start is less than the end
			if (r.begin_x < r.end_x)
			{
				if (cur_cell_x >= r.begin_x && cur_cell_x < r.end_x)
				{
				}
				else
				{
					term = true;
				}
			}

			// control element where not equal to bottom
			else
			{
				if (cur_cell_x >= r.begin_x)
				{
				}
				else if (cur_cell_x < r.end_x)
				{
				}
				else
				{
					term = true;
				}
			}

			// if we are still running add this as a tracked cell
			if (term == false)
			{
				r.total_cells++;
			}
			else
			{
			}

			// is it beyond our range
			processing_cells = !term;
		}
	}

	void WorldQuery::handleCellEnd(void)
	{
		// iterator ending
		IteratorRange& r = iterators[pending_iterators.front()];

		// dispatch message
		IteratorUpdatedMessage um;
		um.index = pending_iterators.front();
		um.iterator = &r;
		events.broadcastToListeners(EVID_ITERATOR_UPDATED, &um);
	}

	void WorldQuery::flush_write_cache()
	{
		// open save path for write out
		FILE*   fptr = 0;
		errno_t err = fopen_s(&fptr, save_to, "a");

		// every object at once
		for (std::list<ObjectData>::iterator itr = disk_write_cache.begin();
			itr != disk_write_cache.end();
			itr++)
		{
			fprintf(fptr, "%s\n", (const char*)itr->getVariantPropdump(save_format));
		}

		// not needed
		fclose(fptr);
		disk_write_cache.clear();
	}

	void WorldQuery::handleCellObject(ObjectData& OD)
	{
		// are we processing cells
		if (processing_cells == false) return;

		// add to tally for this iterator
		iterators[pending_iterators.front()].total_objects++;
		
		// new averages temp
		averages_temp++;
		count++;

		// bounding
		const char* proj_name = (LPCSTR)save_to;
		if (*proj_name == '@')
		{
			Property::Project* pProject = Property::g_Projects.Find((proj_name+1));
			if (pProject)
			{
				pProject->objects.insert_object(OD);
			}
		}
		else
		{
			disk_write_cache.push_back(OD);
			if (disk_write_cache.size() == 10000)
				flush_write_cache();
		}
	}

	void WorldQuery::handleTimer(void)
	{
		averages[time64(0) % AVERAGE_BUFFER] = averages_temp;
		averages_temp = 0;

		// timer
		int t = 0;
		for (size_t i = 0; i < (size_t)AVERAGE_BUFFER; i++)
			t+= averages[i];
		average = t / AVERAGE_BUFFER;
	}

	void WorldQuery::handleCallbackCell(int rc)
	{
		// which are we expecting first
		int index = pending_iterators.front();
		pending_iterators.erase(pending_iterators.begin());

		// have more cells
		if (rc == 0)
		{
			// must be processing cells
			if (region_bounding == true)
			{
				if (this_iterator_done == false)
				{
					queryNext(bot._int(AW_CELL_ITERATOR), index);
				}
			}
			else
			{
				if (processing_cells == true)
				{
					queryNext(bot._int(AW_CELL_ITERATOR), index);
				}
				else
				{
					iterators[index].timer_end = _time64(0);
					queryNextBlock();
				}
			}
		}
		else if (rc == RC_NO_SUCH_CELL)
		{
			iterators[index].timer_end = _time64(0);
			queryNextBlock();
		}
		else
		{
			iterators[index].timer_end = _time64(0);
			last_error_string = AW::Bot::GetReasonCode(rc);
		}

		// dispatch message
		IteratorUpdatedMessage um;
		um.index = index;
		um.iterator = &iterators[index];
		events.broadcastToListeners(EVID_ITERATOR_UPDATED, &um);
	}

}