#include "stdafx.h"
#include "PropertyProject.h"
#include "FileLoader.h"

namespace Property
{
	Objects::Objects()
	{
		track_cells		= true;
		track_sectors	= true;
		limbo_count		= 0;
		RtlZeroMemory(object_types, sizeof object_types);
	
		// initialise trackers using reset function
		clear_trackers();
	} 
	
	Objects::~Objects()
	{

	}

	void Objects::reset(void)
	{
		m_objects.clear();
		clear_trackers();
	}

	// support functions registered here and coded at the bottom
	String ReplaceNoCase( const char* instr, const char* oldstr, const char* newstr );

	void region_object_add(Regions& RG, int Index, ObjectData& Object)
	{
		Region& rgn = RG.GetRegion(Index);
		rgn.count++;
		rgn.count_types[Object.type]++;
		rgn.last_updated = _time64(NULL);

		// calculate height
		if (rgn.max_height < Object.getY()) 
			rgn.max_height = Object.getY();

		// selected
		if (Object.getSelected())
			rgn.selected++;
	}

	void region_object_delete(Regions& RG, int Index, ObjectData& Object)
	{
		Region& rgn = RG.GetRegion(Index);
		rgn.count--;
		rgn.count_types[Object.type]--;
		rgn.last_updated = _time64(NULL);

		// selected
		if (Object.getSelected())
			rgn.selected--;
	}

	bool Objects::update_trackers_add(ObjectData& Object)
	{
		if (track_cells)
			region_object_add(cells, Object.getCellIndex(), Object);
		if (track_sectors)
			region_object_add(sectors, Object.getSectorIndex(), Object);
		object_types[Object.type]++;

		// do we need to increase the bounding cell
		cell_boundary.InsertPoint(Object.getX(), Object.getY(), Object.getZ(),m_objects.size() == 0);
		sector_boundary.InsertPoint(Object.getSectorX(), Object.getY(), Object.getSectorZ(), m_objects.size() == 0);
		return true;
	}
	
	bool Objects::update_trackers_del(ObjectData& Object)
	{
		if (track_cells)
			region_object_delete(cells, Object.getCellIndex(), Object);
		if (track_sectors)
			region_object_delete(sectors, Object.getSectorIndex(), Object);
		object_types[Object.type]--;
		return true;
	}

	void Objects::clear_trackers()
	{
		// clear sectors and cells
		cells.clear();
		sectors.clear();

		// reset boundary conditions
		cell_boundary.hi.reset();
		cell_boundary.lo.reset();

		// clear object counters
		RtlZeroMemory(object_types, sizeof object_types);
	}

	void Objects::rebuild_trackers()
	{
		clear_trackers();
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			update_trackers_add(*objPtr);
	}

	bool Objects::insert_object(ObjectData& Object)
	{
		update_trackers_add(Object);
		m_objects.push_back(Object);
		return true;
	}
	
	Objects::iterator Objects::remove_object(iterator objPtr)
	{
		update_trackers_del(*objPtr);
		return m_objects.erase(objPtr);
	}
	
	bool Objects::remove_object(int Number, int Cell_X, int Cell_Z)
	{
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
		{
			if (objPtr->number == Number && 
				objPtr->getCellX() == Cell_X && objPtr->getCellZ() == Cell_Z)
			{
				remove_object(objPtr);
				return true;
			}
		}

		return false;
	}

	// for finding an object
	Objects::iterator Objects::find_object(int Number, int Cell_X, int Cell_Z)
	{
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			if (objPtr->number == Number && objPtr->getCellX() == Cell_X && objPtr->getCellZ() == Cell_Z)
				return objPtr;
		return end();
	}

	ObjectData* Objects::find_object_ptr(int Number, int Cell_X, int Cell_Z)
	{
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			if (objPtr->number == Number && objPtr->getCellX() == Cell_X && objPtr->getCellZ() == Cell_Z)
				return &(*objPtr);
		return NULL;
	}

	void Objects::empty_project(void)
	{
		m_objects.clear();
		clear_trackers();
	}
	
	int	Objects::Translate(AW::Location& Source, AW::Location& Destination)
	{
		// calculate translations
		int translate_x =  Destination.getX() - Source.getX();
		int translate_y =  Destination.getY() - Source.getY();
		int translate_z =  Destination.getZ() - Source.getZ();

		// translate the object
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			objPtr->ShiftXYZ(translate_x, translate_y, translate_z);

		// empty trackers and reset
		rebuild_trackers();
		return (int)m_objects.size();
	}

	int	Objects::Translate(AW::Location& Offset)
	{
		// translate the object
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			objPtr->ShiftXYZ(Offset.getX(), Offset.getY(), Offset.getZ());

		// empty trackers and reset
		rebuild_trackers();
		return (int)m_objects.size();
	}

	int	Objects::Copy_CloneSelected(Objects& ObjList)
	{
		int mod_count = 0;
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
		{
			if (objPtr->getSelected())
			{
				mod_count++;
				ObjList.insert_object(*objPtr);
			}
		}

		// return total edited
		return mod_count;
	}



	int	Objects::Modify_DeleteSelected(void)
	{
		int mod_count = 0;
		int expected = 0;
		
		// how many to delete
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			if (objPtr->getSelected())
				expected++;

		// delete each of the objects we have selected
		for (iterator objPtr = begin(); objPtr != end(); )
		{
			if (objPtr->getSelected())
			{
				mod_count++;
				objPtr = m_objects.erase(objPtr);
			}
			else
			{
				objPtr++;
			}

		}

		// empty trackers and reset
		rebuild_trackers();
		return mod_count;
	}

	int	Objects::Modify_DeleteSectors(int SectorX, int SectorZ, int Size)
	{
		int mod_count = 0;

		// erase each in this sector
		DWORD ticks = GetTickCount();
		for (iterator objPtr = begin(); objPtr != end(); )
		{
			if (abs(objPtr->getSectorX() - SectorX) < Size && abs(objPtr->getSectorZ() - SectorZ) < Size)
			{
				mod_count++;
				objPtr = remove_object(objPtr);
			}
			else
			{
				objPtr++;
			}
		}

		String lt;
		lt.Format("Run Time: %d", GetTickCount() - ticks);

		// return total changed
		return mod_count;
	}

	int	Objects::Modify_ImportMerge(Objects& ObjectList)
	{
		int mod_count = 0;

		// copy each of the objects
		for (iterator objPtr = ObjectList.begin(); objPtr != ObjectList.end(); objPtr++)
		{
			mod_count++;
			insert_object(*objPtr);
		}

		// empty trackers and reset
		rebuild_trackers();
		return mod_count;
	}


	void replace_ignore_case_mod(String& Text, bool& has_mod, MethodControls::ReplaceString& Options)
	{
		String pattern = Text; 
		pattern.MakeLower();
		if (pattern.Find(Options.find))
		{
			Text = ReplaceNoCase(Text, Options.find, Options.replace);
			has_mod = true;
		}
	}

	int	Objects::Modify_ReplaceStrings(MethodControls::ReplaceString& Options)
	{
		int mod_count = 0;

		// for case conversion if required
		String model, descr, action, pattern;
		String find = Options.find;
		find.MakeLower();

		// select only the objects in thiscell
		for (iterator objPtr = begin(); objPtr != end(); objPtr++) 
		{
			// do we want to only change selected items
			if ( !objPtr->getSelected() && Options.selected_only )
				continue;

			// must be found as lower case, bit cpu intensive but release mode should help
			if (Options.case_sensitive)
			{
				bool has_mod = false;
				if (Options.model && objPtr->model.Find(Options.find) != -1)	
				{
					objPtr->model.Replace(Options.find, Options.replace);
					has_mod = true;
				}
				if (Options.description && objPtr->description.Find(Options.find) != -1)	
				{
					objPtr->description.Replace(Options.find, Options.replace);
					has_mod = true;
				}
				if (Options.action && objPtr->action.Find(Options.find) != -1)	
				{
					objPtr->action.Replace(Options.find, Options.replace);
					has_mod = true;
				}

				// if we have a mod count
				if (has_mod) mod_count++;
			}
			else
			{
				bool has_mod = false;
				if (Options.model)		replace_ignore_case_mod(objPtr->model,		 has_mod, Options);
				if (Options.description)replace_ignore_case_mod(objPtr->description, has_mod, Options);
				if (Options.action)		replace_ignore_case_mod(objPtr->action,		 has_mod, Options);

				// if we have a mod count
				if (has_mod) mod_count++;
			}
		}

		// return total changed
		return mod_count;
	}

	int	Objects::Modify_Translate(MethodControls::Translate& Options)
	{
		int mod_count = 0;
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
		{
			// do we want to only change selected items
			if ( !objPtr->getSelected() && Options.selected_only )
				continue;

			// shift the object in the direction
			objPtr->ShiftXYZ(Options.direction.getX(), 
							 Options.direction.getY(), 
							 Options.direction.getZ()
							);
			mod_count++;
		}

		// empty trackers and reset
		rebuild_trackers();
		return mod_count;
	}

	void FileIO_CopyHexToBinary(std::vector<char>& Array, String Data)
	{
		int len = Data.GetLength();
		Array.reserve(len / 2);

		if (len == 0)
			return;

		// copy the structure backwards
		char byte_dual[3] = { 0, 0, 0 };
		char* pEnd;
		const char* pString = (LPCSTR)Data;
		bool is_ok = false;

		String sx;
		char temp[1024];

		for (int i = 0; i < len; i+=2)
		{
			int pos = (len / 2);

			byte_dual[0] = *(pString + i);
			byte_dual[1] = *(pString + i + 1);

			//sprintf(temp, "%c", c);
			temp[pos] = strtol(byte_dual, &pEnd, 16);
		}

		Array = std::vector<char>(temp[0], len / 2);
		// WriteMsg(sx, RGB_RED, OT_MAIN);
	}

	String FileIO_ArrayToHex(std::vector<char>& Array)
	{
		String var;
		for (size_t i = 0; i < Array.size(); i++)
			var.AppendFormat("%02x", Array[i]);
		return var;
	}





	String ReplaceNoCase( const char* instr, const char* oldstr, const char* newstr )
	{
		String output( instr );

		// lowercase-versions to search in.
		String input_lower( instr );
		String oldone_lower( oldstr );
		input_lower.MakeLower();
		oldone_lower.MakeLower();

		// search in the lowercase versions,
		// replace in the original-case version.
		int pos=0;
		while ( (pos=input_lower.Find(oldone_lower,pos))!=-1 ) {

			// need for empty "newstr" cases.
			input_lower.Delete( pos, lstrlen(oldstr) );	
			input_lower.Insert( pos, newstr );

			// actually replace.
			output.Delete( pos, lstrlen(oldstr) );
			output.Insert( pos, newstr );
		}

		return output;
	}
}