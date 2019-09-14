#include "stdafx.h"
#include "PropertyProject.h"
#include "FileLoader.h"
#include "property_editors.h"
#include "property_filters.h"
#include "umbra/umbra_classlib_usl_vector.h"

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

	bool Objects::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// nothing found
		return R.NOT_HANDLED;
	}

	bool Objects::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// iteration handler
		UMBRA_FUNCTION_BEGIN(getIterator) {
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( (new ScriptIterator(*this, _umbra_isa, begin(), end()))->setUmbraGarbageCollected() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// executes a filter
		//
		
		UMBRA_FUNCTION_BEGIN(filter) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Filter, Umbra::VT_CLASS);
			UMBRA_FUNCTION_OA(1, Selected, Umbra::VT_BOOLEAN, false);
			UMBRA_FUNCTION_CC(Filter, Property::Filters::Filter);

			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( run_filter(pFilter, vaSelected.toBool()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// executes an editor
		//
		
		UMBRA_FUNCTION_BEGIN(sweep) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Editor, Umbra::VT_CLASS);
			UMBRA_FUNCTION_OA(1, Selected, Umbra::VT_BOOLEAN, false);
			UMBRA_FUNCTION_CC(Editor, Property::Editors::Editor);

			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( pEditor->runObjects(*this) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// deletes an object
		//
		
		UMBRA_FUNCTION_BEGIN(erase) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Pointer, Umbra::VT_CLASS);
			UMBRA_FUNCTION_CC(Pointer, ScriptIterator);

			// code information break
			UMBRA_FUNCTION_CS() {				
				UMBRA_FUNCTION_RET( (new ScriptIterator(*this, _umbra_isa, 
					m_objects.erase(pPointer->active_iterator), end()))
					->setUmbraGarbageCollected() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// searching parameters
		//

		UMBRA_FUNCTION_BEGIN(getByName) {
			// required arguments
			UMBRA_FUNCTION_DA(0, String, Umbra::VT_STRING);
			// code information break
			UMBRA_FUNCTION_CS() {
				// iterated search loop
				String name = vaString.toString().c_str();
				for (iterator o = begin(); o != end(); o++)
					if (o->name.CompareNoCase(name) == 0)
					{
						UMBRA_FUNCTION_RET( *o );
					}				
				// nothing found
				UMBRA_FUNCTION_RET( Umbra::Variant() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(getArrayByName) {
			// required arguments
			UMBRA_FUNCTION_DA(0, String, Umbra::VT_STRING);
			UMBRA_FUNCTION_OA(1, Prefixed, Umbra::VT_BOOLEAN, false);
			// code information break
			UMBRA_FUNCTION_CS() {
				// iterated search loop
				Umbra::ClassLib::Usl::VectorImpl* pVec = new Umbra::ClassLib::Usl::VectorImpl(0, true);
				String name = vaString.toString().c_str();
				if (vaPrefixed.toBool() == true)
				{
					for (iterator o = begin(); o != end(); o++)
						if (o->name.Left(name.GetLength()).CompareNoCase(name) == 0)
						{
							pVec->arr.push_back(*o);
						}
				}
				else
				{
					for (iterator o = begin(); o != end(); o++)
						if (o->name.CompareNoCase(name) == 0)
						{
							pVec->arr.push_back(*o);
						}
				}

				// copmleted vector
				UMBRA_FUNCTION_RET( *pVec );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// nothing found
		return R.NOT_HANDLED;
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
		rgn.last_updated = time64(0);

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
		rgn.last_updated = time64(0);

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
		
		// signal is invalidated 
		_umbra_isa.signalInvalidated();

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
		for (iterator objPtr = ObjList.begin(); objPtr != ObjList.end(); objPtr++)
		{
			if (objPtr->getSelected())
			{
				mod_count++;
				insert_object(*objPtr);
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