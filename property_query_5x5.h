#pragma once
#include <vector>
#include "property_query.h"

namespace AW
{
	class Bot;
}

namespace Property
{
	class Objects;
}

namespace Querying
{
	struct QueryPosition
	{
		int x, z;
	};

	typedef std::vector<QueryPosition> QueryQueue;

	class Query_5x5 : public IQuery
	{
	protected: // private variables
		int		m_sequence[5][5];			// cell sequence
		
	protected: // the cells that are currently being queried
		int		m_cell_x;					// cell x
		int		m_cell_z;					// cell z

	protected: // the cells that we are querying at
		int		m_query_cell_x;
		int		m_query_cell_z;

	protected: // the sectors we are querying at
		int		m_start_sector_x;			// start sector x
		int		m_start_sector_z;			// start sector z
		
	protected: // the bot that is being used
		AW::Bot* m_pBot;					// client bot
		Property::Objects* m_pObjects;		// client objects

	protected: // tracking of progress
		QueryQueue m_queue;					// the list of places to query
		int		m_last_error;				// last error encountered
		bool	m_complete;					// has the query completed yet
		void	reset();

	public:
		Query_5x5();

	public: // read access of the current cells
		int		getCellX() const		{ return m_cell_x;			}	// cell x
		int		getCellZ() const		{ return m_cell_z;			}	// cell z

	public: // read access of the start cell
		int		getQueryCellX() const	{ return m_query_cell_x;	}	// cell x
		int		getQueryCellZ() const	{ return m_query_cell_z;	}	// cell z

	public: // read access of the start sectors
		int		getStartSectorX() const { return m_start_sector_x;	}	// start sector x
		int		getStartSectorZ() const { return m_start_sector_z;	}	// start sector z
		
	public: // read access of tracking
		bool	IsComplete() const		{ return m_complete;		}
		int		GetLastError() const	{ return m_last_error;		}	// last error encountered
		
	public: // methods for linking external sdk events
		void	handleCellBegin		(void);
		void	handleCellEnd		(void);
		void	handleCallbackQuery	(int rc);
		void	handleCellObject	(ObjectData& OD);

	protected: // cache handling
		int		CacheSectorLoad(int SectorX, int SectorZ);
	
	public: // external cache handling
		void	ResetCache();
		errno_t CommitCache();

	protected: // query helpers
		int		QueryInit(int Cell_X, int Cell_Z);

	public: // query launch mechanisms
		void	SetBot(AW::Bot& B) { m_pBot = &B; }
		void	setObjects(Property::Objects& O) { m_pObjects = &O; }
		int		Query(int Cell_X, int Cell_Z);
	};
}