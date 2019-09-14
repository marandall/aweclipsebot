#include "stdafx.h"
#include "property_regions.h"
#include "property_markers.h"

namespace Property
{
	// functions for creating cell data
	int make_cell_iterator(int x, int z) { return ((LONG)(((WORD)((DWORD_PTR)(x) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(z) & 0xffff))) << 16)); }
	int getcell_x(int l) {	return ((WORD)((DWORD_PTR)(l) & 0xffff)); }
	int getcell_z(int l) { return ((WORD)((DWORD_PTR)(l) >> 16)); }
	int getobject_number(__int64 Index) { return static_cast<int>((Index >> 32) & 0xFFFFFFFF); }
	__int64 make_object_cell_key(int X, int Z, int Number) {
		__int64 result = Number; 
		result = (result << 32);
		result|= make_cell_iterator(X, Z);
		return result;
	}



	Regions::iterator Regions::find(int Region_X, int Region_Z)
	{
		return find(Property::make_cell_iterator(Region_X, Region_Z));
	}

	Regions::iterator Regions::find(int Region_Iterator)
	{
		return m_regions.find(Region_Iterator);
	}

	Region& Regions::operator[](int Region_Iterator)
	{
		return m_regions[Region_Iterator];
	}

	Region& Regions::GetRegion(int X, int Z)
	{
		return GetRegion(Property::make_cell_iterator(X, Z));
	}

	Region& Regions::GetRegion(int Region_Iterator)
	{
		Region& cell = m_regions[Region_Iterator];
		if (cell.last_updated == 0) {
			cell.pos	= Region_Iterator;
			cell.x		= Property::getcell_x(Region_Iterator);
			cell.z		= Property::getcell_z(Region_Iterator); 
		} // end of update
		return cell;
	}

	time_t Regions::getLastUpdated(int X, int Y)
	{
		iterator itr = m_regions.find(Property::make_cell_iterator(X, Y));
		if (itr == m_regions.end())
			return 0;
		else
			return itr->second.last_updated;
	}



	void MarkerRange::InsertPoint(int X, int Y, int Z, bool Always)
	{
		if (Always)
		{
			hi.x = X;	hi.z = Z;
			lo.x = X;	lo.z = Z;
		}
		else
		{
			if (hi.x < X) hi.x = X;	if (hi.z < Z) hi.z = Z;
			if (lo.x > X) lo.x = X;	if (lo.z > Z) lo.z = Z;	
		}
	}
}