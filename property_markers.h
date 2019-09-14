#pragma once
#include "awsdk_location.h"

namespace Property
{
	class Marker
	{
	public:
		int		x;
		int		z;
		String	id;

	public: // get a coordinate version
		void			reset()		  { x = z = 0; }
		Marker()					  { reset();   }
		AW::Location	getLocation() { return AW::Location(x, 0, z, 0); }
	};

	class MarkerRange
	{
	public: // accessible objects for the marker ranges
		Marker	hi;
		Marker	lo;

	public: // handling functions
		void	InsertPoint(int X, int Y, int Z, bool Always);
	};

}