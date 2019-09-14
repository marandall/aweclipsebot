#pragma once
#include "awsdk_objects_v3.h"

namespace Property
{
	class Region
	{
	public: // position
		short	x;
		short	z;
		int		pos;

	public: // statistical counts
		short	count;								// the total count
		short	count_types[MAX_OBJECT_TYPE];		// each set of objects
		short	selected;							// the total selected
		short	max_height;							
		__int64	last_updated;						// when was Region last updated (__time64_t)
	};
}