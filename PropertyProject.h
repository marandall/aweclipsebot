#pragma once

#include "property_markers.h"

// include: 
#include "property_object.h"
#include "property_objects.h"

// include: project tracking and object data
#include "property_project.h"
#include "property_projects.h"

// include: regional information tracking for density maps
#include "property_region.h"
#include "property_regions.h"


namespace Property
{

	// helper functions
	int		signed_hex_to_int	(const char* Value);
	int		make_cell_iterator	(int x, int z);
	int		getcell_x			(int x);
	int		getcell_z			(int x);
	int		getobject_number	(__int64 Index);
	__int64 make_object_cell_key(int X, int Z, int Number);


	extern Projects g_Projects;
}