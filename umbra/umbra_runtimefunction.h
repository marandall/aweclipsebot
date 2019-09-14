#pragma once
#include "umbra_typedef.h"
#include "umbra_variant.h"

namespace Umbra
{
	class RuntimeFunction
	{
	public: // the identification of the function
		string	id;
		int		access;
		int		args_required;
		int		args_max;

	public: // the code for the function
	};
}