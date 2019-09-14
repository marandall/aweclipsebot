#pragma once
#include <list>

#include "umbra/umbra_remoteclass.h"
#include "property_object.h"
#include "property_regions.h"
#include "property_markers.h"
#include "property_method_controls.h"
#include "property_filters_fref.h"	// forward references
#include "scripting_iterator.h"

// forward references
class ObjectData;


namespace Property
{
	namespace Filters
	{
		class Filter;
	}

	class Objects : public Umbra::RemoteClass
	{
	public:
		Objects();
		~Objects();

	public: // umbra control
		virtual bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
		virtual bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest&);

	public: // type defenitions for a project
		typedef std::list<ObjectData>				ListType;
		typedef ListType::iterator					iterator;
		typedef ListType::const_iterator		const_iterator;
		typedef ListType::reverse_iterator	reverse_iterator;
		typedef ListType::pointer						pointer;

	public: // the iterator type
		typedef	Umbra::CodeIterator<iterator> ScriptIterator;

	protected: // internal lists
		ListType									m_objects;
		Umbra::IteratorSignalAgent _umbra_isa;

	public:   // external lists	
		Regions				cells;
		Regions				sectors;
		bool					track_cells;
		bool					track_sectors;
		int						object_types[MAX_OBJECT_TYPE];
		MarkerRange		cell_boundary;
		MarkerRange		sector_boundary;
		int						limbo_count;

	public: // access to the list
		iterator					begin(void)		  { return m_objects.begin();	}
		iterator					end	 (void)		  { return m_objects.end();		}
		const_iterator		begin(void)	const { return m_objects.begin();	}
		const_iterator		end  (void) const { return m_objects.end();		}
		reverse_iterator	rbegin(void)	  { return m_objects.rbegin();	}
		reverse_iterator	rend (void)		  { return m_objects.rend();	}
		size_t						size (void) const { return m_objects.size();	}
		iterator					erase(iterator I) { return m_objects.erase(I);	} 
		void							reset(void);

	protected: // update the trackers
		bool					update_trackers_add(ObjectData& Object);
		bool					update_trackers_del(ObjectData& Object);
		bool					update_trackers_selected(ObjectData& Object);
		bool					update_trackers_unselected(ObjectData& Object);
	public:	
		bool					update_trackers(ObjectData& Object);
		void					clear_trackers();

	public: // external items forcing rebuild
		void					rebuild_trackers();

	public: // modify the list with objects
		bool					insert_object	(ObjectData& Object);
		iterator			remove_object	(iterator objPtr);
		bool					remove_object	(int Number, int Cell_X, int Cell_Z);
		void					empty_project	(void);
		iterator			find_object		(int Number, int Cell_X, int Cell_Z);
		ObjectData*		find_object_ptr	(int Number, int Cell_X, int Cell_Z);

	public: // propdump manipulation routines
		errno_t				write_propdump   (const char* Path, int Version = 4);
		errno_t				write_project_v1 (const char* Path);
		errno_t				write_cache		 (const char* Path, int SectorX, int SectorZ);
		errno_t				read_project_v1  (const char* Path);
		errno_t				read_propdump	 (const char* Path);

	public: // edit translations
		int					Translate(AW::Location& Source, AW::Location& Destination);
		int					Translate(AW::Location& Offset);

	public: // selection tols
		int					run_filter				(Filters::Filter* Options, bool Select);
		int					deleteSelected			(bool Selected = true);
		int					invertSelection();
		int					clearSelection();

	public: // modification of set
		int					Modify_DeleteSelected	(void);
		int					Modify_ImportMerge		(Objects& ObjectList);
		int					Modify_ReplaceStrings	(MethodControls::ReplaceString& Options);
		int					Modify_Translate		(MethodControls::Translate& Options);
		int					Modify_DeleteSectors	(int SectorX, int SectorZ, int Size = 1);

	public: // copy clones
		int					Copy_CloneSelected		(Objects& ObjList);
	};
}