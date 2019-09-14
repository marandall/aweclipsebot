#pragma once
#include "awsdk_objects_v3.h"
#include <list>
#include <vector>
#include <map>

namespace AW
{
	class Bot;
	class Object_Base;

	int MakeCellRef(short X, short Z);

	class Query_5x5
	{
	protected: // private variables
		int		m_sequence[5][5];			// cell sequence
		int		m_cell_x;					// cell x
		int		m_cell_z;					// cell z
		int		m_query_cell_x;
		int		m_query_cell_z;
		int		m_start_sector_x;			// start sector x
		int		m_start_sector_z;			// start sector z
		Bot*	m_pBot;						// client bot
		int		m_last_error;				// last error encountered
		bool	m_complete;
		void	reset();

	public:
		Query_5x5() { reset(); }

	public:
		int		getCellX() const		{ return m_cell_x;			}	// cell x
		int		getCellZ() const		{ return m_cell_z;			}	// cell z
		int		getStartSectorX() const { return m_start_sector_x;	}	// start sector x
		int		getStartSectorZ() const { return m_start_sector_z;	}	// start sector z
		int		GetLastError() const	{ return m_last_error;		}	// last error encountered
		int		getQueryCellX() const	{ return m_query_cell_x;	}	// cell x
		int		getQueryCellZ() const	{ return m_query_cell_z;	}	// cell z
		bool	IsComplete() const		{ return m_complete;		}

	public: // methods for linking external sdk events
		virtual void	HandleCellBegin		(void);
		virtual void	HandleCellEnd		(void);
		virtual void	HandleCallbackQuery	(int rc);

	public: // query launch mechanisms
		void			SetBot(Bot& B) { m_pBot = &B; }
		virtual int		Query(int Cell_X, int Cell_Z);
	};


	class Query_World
	{
	protected: // tracking variables
		bool	m_complete;
		int		m_last_error;
		int		m_cell_x;
		int		m_cell_z;

	public: // read properties
		int		getCellX() const	{ return m_cell_x;			}
		int		getCellZ() const	{ return m_cell_z;			}
		bool	getComplete() const	{ return m_complete;		}
		int		GetLastError() const{ return m_last_error;		}

	public: // methods for linking external sdk events
		virtual void	HandleCellBegin		(void);
		virtual void	HandleCellEnd		(void);
		virtual void	HandleCallbackQuery	(int rc);
		virtual void	HandleCallbackCellResult(int rc);

	public: // the bot control
		Bot*	m_pBot;		// client bot
		void	SetBot(Bot& B) { m_pBot = &B; }
	};


	class Query_Cache
	{
	public: // primary methods
		void	Reset();
		Query_Cache() { Reset(); } 

	public: // linked list used to high re-allocations
		typedef std::list<ObjectData> ListType;
		typedef std::list<ObjectData>::iterator Iterator;
		std::list<ObjectData>	m_list;			// linked list of objects

	public: // vector methdods
		Iterator	begin() { return m_list.begin(); }
		Iterator	end()   { return m_list.end();   }

	public:
		size_t	getObjectCount() const { return m_list.size(); }

	public:
		virtual void	InsertObject(ObjectData& Object);
		virtual void	RemoveObject(int ObjectId);

	public: // file routines
		virtual bool	Read_Propdump(String FilePath);
		virtual bool	Write_Propdump(String FilePath);
	};
}