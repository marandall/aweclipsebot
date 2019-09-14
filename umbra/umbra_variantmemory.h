#pragma once
#include "umbra_typedef.h"
#include "umbra_resourcemanager.h"
#include <vector>

using namespace std;

namespace Umbra
{
	class RemoteClass;

	// Types
	// ---------------------------- 
	// Defines a variable type to be used in combination with
	// the dataunion
	//
	namespace Types
	{
		enum 
		{
			Null		= 0, 
			Integer,
			Double, 
			String,
			Resource,
			Boolean
		};
	};

	class SolidTypes
	{
	public:
		static const char Null				= 'N'; 
		static const char Integer			= 'I';
		static const char Double			= 'D'; 
		static const char String			= 'S'; 
		static const char Resource		= 'R';
		static const char Boolean			= 'B';
	};

	// DataUnion
	// ----------------------------
	// Union of the data for strings
	union DataUnion
	{
		__int64 v_int64;
		double	v_double;
		int		v_resource;
		bool	v_boolean;
	};

	class VariantMemory
	{
	public: // data
		DataUnion	du;
		int			type;
		int			ref_count;
		bool		break_on_change;

	public: // reader methods
		int			getType() const { return type; }
		int			getRefCount() const { return ref_count; }

	public: // memory referencing
		void		addRef();
		void		decRef();

	public: // constructor and destructor
		VariantMemory();
		~VariantMemory();
		VariantMemory& operator=(const VariantMemory& VM);
	};
}