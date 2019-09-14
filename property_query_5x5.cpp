#include "stdafx.h"
#include "property_query_5x5.h"
#include "property_objects.h"

namespace Querying
{
	errno_t BuildDirectoryPath(String Path)
	{
		// create directory
		int rc =  SHCreateDirectoryEx(NULL, Path, NULL);
		return rc;
	}

	Query_5x5::Query_5x5()
	{
		reset();
	}

	void Query_5x5::reset()
	{
		// reset sequences
		memset(m_sequence, 0, sizeof(m_sequence));
		
		// reset variables
		m_cell_x			= 0;
		m_cell_z			= 0;
		m_query_cell_x		= 0;
		m_query_cell_z		= 0;
		m_start_sector_x	= 0;
		m_start_sector_z	= 0;
		m_complete			= false;
	}

	void Query_5x5::ResetCache()
	{
		reset();
	}

	void Query_5x5::handleCellObject(ObjectData& OD)  
	{
		bot.getLiveCache().insert_object(OD);
	}

	void Query_5x5::handleCellBegin	(void)
	{
		// update the current cell
		m_cell_x = aw_int(AW_CELL_X);
		m_cell_z = aw_int(AW_CELL_Z);
		
		// re-calculate sectors
		int sector_x = aw_sector_from_cell(m_cell_x) - m_start_sector_x;
		int sector_z = aw_sector_from_cell(m_cell_z) - m_start_sector_z;

		// store the cell sequence
		m_sequence[sector_z + 2][sector_x + 2] = m_pBot->_int(AW_CELL_SEQUENCE);
	}
	
	void Query_5x5::handleCellEnd(void)
	{
	}
	
	void Query_5x5::handleCallbackQuery(int rc)
	{
		BOOL is_complete = m_pBot->_bool(AW_QUERY_COMPLETE);
		if (is_complete == TRUE)
		{
			m_complete = true;
		}
		else
		{
			m_complete = false;
			int rc = m_pBot->_query_5x5(m_start_sector_x, m_start_sector_z, m_sequence);
		}
	}

	int Query_5x5::CacheSectorLoad(int SectorX, int SectorZ)
	{
		// the sector path that we are going to use. This introduces the
		// new path 'worksets' as in:
		// \worksets\active worlds\awteen query\1287\1827\ 
		String sector_path;
		sector_path.Format("%s\\worksets\\%s\\%s query\\%d\\%d\\", GetAppPath(), 
			m_pBot->getAL_UniName(), m_pBot->getWorld(), SectorX, SectorZ);

		// read the statistics file
		AW::EnhancedINI stat_file;
		if (stat_file.Load(sector_path + "sequence.ini") == false)
			return 0;		// set the sequence to zero

		// read the key data
		int cell_sequence = atoi(stat_file.Read("sequence", "sequence", "0"));
		if (cell_sequence != 0)
		{
			// load the objects into the live cache
			m_pObjects->Modify_DeleteSectors(SectorX, SectorZ);

			DWORD ticks = GetTickCount();
			m_pObjects->read_project_v1(sector_path + "cache.e2pp");
			int run_time = GetTickCount() - ticks;
			int j = 0;
		}

		// return the cell sequence
		return cell_sequence;
	}

	errno_t Query_5x5::CommitCache()
	{
		for (int x = 0; x < 5; x++)
		{
			for (int z = 0; z < 5; z++)
			{
				// ignore anything that doesnt exist (ie: has sequence zero)
				if (m_sequence[x][z] == 0)
					continue;
				
				// map the sectors
				int sector_x = x + m_start_sector_x - 2;
				int sector_z = z + m_start_sector_z - 2;

				// write the sector paths
				String sector_path;
				sector_path.Format("%s\\worksets\\%s\\%s query\\%d\\%d", GetAppPath(), 
					m_pBot->getAL_UniName(), m_pBot->getWorld(), sector_x, sector_z);

				// build the path if needed
				BuildDirectoryPath(sector_path);

				// write the sequence number to file
				AW::EnhancedINI stat_file;
				stat_file.Write("sequence", "sequence", m_sequence[x][z]);
				stat_file.Save(sector_path + "\\sequence.ini");

				// write cache for this sector
				m_pObjects->write_cache(sector_path + "\\cache.e2pp", sector_x, sector_z);
			}
		}

		// return NfN
		return (errno_t)0;
	}

	int	Query_5x5::QueryInit(int Cell_X, int Cell_Z)
	{
		// commit to cache
		// CommitCache();

		// has to erase the cells that are to be queried
		memset(m_sequence, 0, sizeof(m_sequence));

		// calculate starting sector
		m_query_cell_x		= Cell_X;
		m_query_cell_z		= Cell_Z;
		m_start_sector_x	= aw_sector_from_cell(Cell_X);
		m_start_sector_z	= aw_sector_from_cell(Cell_Z);
		
		// update the cache sectors
		for (int x = 0; x < 5; x++)
			for (int z = 0; z < 5; z++)
				m_sequence[x][z] = 0; // CacheSectorLoad(x + m_start_sector_x - 2, 
									  //		   z + m_start_sector_z - 2);

		// query the location
		m_complete = false;
		int rc = m_pBot->_query_5x5(m_start_sector_x, m_start_sector_z, m_sequence);
		return m_last_error = rc;
	}

	int	Query_5x5::Query(int Cell_X, int Cell_Z)
	{		
		return QueryInit(Cell_X, Cell_Z);
		return 0;
	}
}