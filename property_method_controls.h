#pragma once
#include "awsdk_location.h"

namespace Property
{
	namespace MethodControls
	{
	
		// replaces one string with anothing string
		// in classical v4 objects
		class ReplaceString
		{ public:
			bool			selected_only;
			bool			model, description, action;
			String			find, replace;
			bool			case_sensitive;
		};

		// moves an area around the plane without
		// rotation
		class Translate
		{ public:
			bool			selected_only;
			AW::Location	direction;			
		};
	}
}