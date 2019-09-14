#pragma once
#include <list>
#include "property_query.h"
#include "listener.h"

namespace Querying
{
	struct IteratorRange
	{
		// tracking information
		int			begin_x, begin_y;
		int			end_x, end_y;

		// timing statistics
		time_t	timer_start;
		time_t	timer_end;

		// object statistics
		int			total_cells;
		int			total_objects;
	
		IteratorRange()
		{
		}

		IteratorRange(int Bs, int Ex)
		{
			begin_x = Bs;
			end_x		= Ex;

			// reset certain numbers
			timer_start = timer_end = 0;
			total_cells = total_objects = 0;
		}
	};

	class IteratorUpdatedMessage : public EventMessage
	{
	public:
		int index;
		IteratorRange* iterator;
	};

	class WorldQuery : public IQuery
	{
	public: // multiqueue
		WorldQuery(CommonBot* C);

	public: // setup query styles
		int		queryNext(int Itr, int Index);
		int		start();
		int		queryBetween(AW::Location Top, AW::Location Bottom);
		int		queryNextBlock(bool IsFirst = false);

	public: // sdk interfaces
		void	handleCellBegin(void);
		void	handleCellEnd(void);
		void	handleCallbackQuery(int rc) { }
		void	handleCellObject(ObjectData& OD);
		void	handleCallbackCell(int rc);
		void	handleTimer(void);

	public: // averages
		static const int AVERAGE_BUFFER = 10;

	public: // event broadcasters
		EventBroadcaster events;

	public: // event enumerations
		static const int EVID_ITERATOR_UPDATED	= 1;
		static const int EVID_COMPLETED					= 2;

	public: // query begin
		int						save_format;
		CString				save_to;
		String				write_to_project;

	protected: // object count
		int						count;
		int						averages[AVERAGE_BUFFER];
		int						averages_temp;
		int						average;
		
	protected: // the length of time
		time_t				start_time;
		time_t				end_time;

		bool					completed;
		CString				last_error_string;
		bool					processing_cells;

	protected:// bounds mode
		bool					region_bounding;
		bool					this_iterator_done;
		AW::Location	bounds_top;
		AW::Location	bounds_bottom;

	protected: // disk caching
		std::list<ObjectData> disk_write_cache;
		void flush_write_cache();

	protected: // multi-iterator tracking
		static const int ITERATOR_COUNT = 2;
		int													iterator_switch;
		int													iterator_next;

	public: // the iterator ranges
		std::vector<IteratorRange>	iterators;
		std::list<int>							pending_iterators;

	public: // count
		int			getCount()					{ return count;			}
		int			getAverage()				{ return average;		}
		time_t	getStartTime()			{ return start_time;	}
		time_t	getEndTime()				{ return end_time;		}
		time_t	getDuration()				{ if (end_time) { return end_time - start_time; } else { return _time64(0) - start_time; } }
		bool		isComplete()				{ return completed;		}
		CString	getLastErrorString(){ return last_error_string; }
	};
}