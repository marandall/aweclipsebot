#pragma once
#include "umbra_remoteclass.h"
#include "umbra_variant.h"
#include "umbra_typedef.h"
#include <map>

namespace Umbra
{
	class Host;

	namespace ClassRef
	{
		enum
		{
			UNKNOWN = 0,

			// system namespace
			SYSTEM_NS,							// [sealed]
			MATHS_NS,							// [sealed]

			// Graphics namespace
			USL_GRAPHICS_NS,					// [sealed]
			USL_GRAPHICS_PEN_CLASSDEF,			// [sealed]
			USL_GRAPHICS_BITMAP_CLASSDEF,		// [sealed]
			USL_GRAPHICS_BRUSH_CLASSDEF,		// [sealed]
			USL_GRAPHICS_COLOR_CLASSDEF,		// [sealed]
			USL_GRAPHICS_PEN,					// [sealed]
			USL_GRAPHICS_BITMAP,				// [sealed]
			USL_GRAPHICS_BRUSH,					// [sealed]
			USL_GRAPHICS_COLOR,					// [sealed]

			// Collections namespace
			USL_COLLECTIONS_VECTOR_CLASSDEF,	// [sealed]
			USL_COLLECTIONS_MAP_CLASSDEF,		// [sealed]
			USL_COLLECTIONS_ARRAY_CLASSDEF,		// [sealed]
			USL_COLLECTIONS_VECTOR,
			USL_COLLECTIONS_MAP,
			USL_COLLECTIONS_ARRAY
		};
	};

	//
	// Forward References
	//

	class Host;

	//
	// Allows the creation of a new instance of this class through the doUmbraNewInstance method
	//  this prevents it from being called as a direct method
	//

	class INewInstanceCreator
	{
	public: // a single function relevant for creating a new class
		virtual RemoteClass* doUmbraNewInstance(Host*) = 0;
	};
}