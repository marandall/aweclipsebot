#pragma once
#include <list>
#include "property_query_5x5.h"
#include "property_query.h"

namespace Querying
{
	class QuerySegment
	{
	public:
		// position of the query
		int query_x;
		int query_z;
		int sector_x;
		int sector_z;

		// section query code
		int sequence[5][5];
	};


	class MultiQueue : public IQuery
	{
	public: // multiqueue
		MultiQueue(CommonBot* C);

	public: // the boundary conditions
		AW::Location		bounds_nw;		// north west
		AW::Location		bounds_se;		// south east

	public: // the querying subsystem
		Query_5x5			query;			// the query subsystem
		list<AW::Location>	queue;

	public: // the segment tracking
		std::vector<int>						pending;
		QuerySegment								_seg;

	public: // setup query styles
		int		queryBetween(AW::Location NW, AW::Location SE);
		int		queryAround(AW::Location Mid, int Width, int Height);
		int		queryNext();

	public: // sdk interfaces
		void	handleCellBegin(void);
		void	handleCellEnd(void);
		void	handleCallbackQuery(int rc);
		void	handleCellObject(ObjectData& OD);

	public: // instancing
		static MultiQueue& getInstance();
	};
}