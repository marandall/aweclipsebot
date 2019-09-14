#include "stdafx.h"
#include "awsdk_bot.h"
#include "awsdk_query.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace AW
{
	int MakeCellRef(short X, short Z)
	{
		return MAKELONG(X, Z);
	}

	void Query_5x5::reset()
	{
		m_cell_x			= 0;
		m_cell_z			= 0;
		m_query_cell_x		= 0;
		m_query_cell_z		= 0;
		m_start_sector_x	= 0;
		m_start_sector_z	= 0;
		m_complete			= false;
	}

	void Query_5x5::HandleCellBegin(void)
	{
		// update the current cell
		m_cell_x = aw_int (AW_CELL_X);
		m_cell_z = aw_int (AW_CELL_Z);
		
		// re-calculate sectors
		int sector_x = aw_sector_from_cell (m_cell_x) - m_start_sector_x;
		int sector_z = aw_sector_from_cell (m_cell_z) - m_start_sector_z;

		m_sequence[sector_z + 2][sector_x + 2] = m_pBot->_int(AW_CELL_SEQUENCE);
	}
	
	void Query_5x5::HandleCellEnd(void)
	{
	}
	
	void Query_5x5::HandleCallbackQuery(int rc)
	{
		if (m_pBot->_bool(AW_QUERY_COMPLETE) == FALSE)
		{
			m_pBot->_query_5x5(m_start_sector_x, m_start_sector_z, m_sequence);
		}
		else
		{
			m_complete = true;
		}
	}
	
	int	Query_5x5::Query(int Cell_X, int Cell_Z)
	{
		// erase tables
		memset(m_sequence, 0, sizeof(m_sequence));

		// calculate starting sector
		m_query_cell_x		= Cell_X;
		m_query_cell_z		= Cell_Z;
		m_start_sector_x	= aw_sector_from_cell(Cell_X);
		m_start_sector_z	= aw_sector_from_cell(Cell_Z);

		// query the location
		m_complete = false;
		int rc = m_pBot->_query_5x5(m_start_sector_x, m_start_sector_z, m_sequence);
		return m_last_error = rc;
		
		return 0;
	}





	void Query_World::HandleCellBegin(void)
	{
		m_cell_x	= m_pBot->_int(AW_CELL_X);
		m_cell_z	= m_pBot->_int(AW_CELL_Z);
	}

	void Query_World::HandleCellEnd(void)
	{
	}

	void Query_World::HandleCallbackQuery(int rc)
	{
	}

	void Query_World::HandleCallbackCellResult(int rc)
	{
		if (rc)
		{
			m_last_error = rc;
			if (rc == RC_NO_SUCH_CELL)
			{
				m_last_error = 0;
				m_complete = true;
			}
		}
		else
		{
			m_pBot->_cell_next();
		}
	}
}