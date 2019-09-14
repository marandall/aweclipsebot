#pragma once

#include <vector>
#include "umbra_variantmemory.h"

using namespace std;

typedef __int32		int32;
typedef __int64		int64;


namespace Umbra
{
	class RemoteClass;
	
	namespace ClassLib
	{
		class RuntimeString;
	}

	// Types
	// ---------------------------- 
	// Defines a variable type to be used in combination with
	// the dataunion
	//


	// The variable types
	//
	//

	const int VT_ANY		= 0;
	const int VT_INTEGER	= 1;
	const int VT_DOUBLE		= 2;
	const int VT_NUMBER		= 3;
	const int VT_STRING		= 4;
	const int VT_BOOLEAN	= 5;
	const int VT_CLASS		= 6;


	// Exception : Unsupported Operation
	// 
	//

	class UnsupportedOperationException
	{
	public: // the types
		int left, right;

	public: // constructor
		UnsupportedOperationException(int Left, int Right)
		{
			left	= Left;
			right	= Right;
		}
	};

	// Variant
	// ---------------------------- 
	// Data class to represent a single variable in the game, be it a
	// a primitive or otherwise
	class Variant
	{
	public: // the remote data
		VariantMemory*	m_ptr2vm;
		VariantMemory&	getVM();
		DataUnion&		getDU();

	protected: // purposefully unsafe
		DataUnion&		getDU() const;

	protected: // handling of attached memory
		void detatch_memory();
		void attach_memory(VariantMemory* VM);

	public: /* type checks */
		int  getType() const			{ return m_ptr2vm == 0 ? Types::Null : m_ptr2vm->type; }
		bool IsNull() const				{ return getType() == Types::Null;		}
		bool IsInteger() const			{ return getType() == Types::Integer;	}
		bool IsDouble()	const			{ return getType() == Types::Double;	}
		bool IsRemote()	const			{ return getType() == Types::Resource;	}
		bool IsString()	const			{ return getType() == Types::String;	}
		bool IsBool()  const			{ return getType() == Types::Boolean;	}
		bool IsClassInterface() const	{ return getType() == Types::Resource || getType() == Types::String; }

	protected: /* type setting routine */
		void SwitchType(int Type);

	public:
		Variant getReference() { return *this; }
		const char* getRawTypeName();

	public: /* initialisation and operators */
		~Variant()
		{
			// unhook the memory
			if (m_ptr2vm)
				m_ptr2vm->decRef();
		}

		Variant() 
		{
			m_ptr2vm = NULL;
		}

		Variant(const Variant& V) 
		{
			m_ptr2vm = NULL;
			*this = V;
		}

		Variant(int);
		Variant(int64);
		Variant(float);
		Variant(double);
		Variant(const char*);
		Variant(const RemoteClass&);
		Variant(VariantMemory*);
		Variant(bool);
		Variant(const std::string&);

	public: /* assignment operators */
		Variant& operator=(u_int32);
		Variant& operator=(int32);
		Variant& operator=(int64);
		Variant& operator=(float);
		Variant& operator=(double);
		Variant& operator=(const char*);
		Variant& operator=(const RemoteClass&);
		Variant& operator=(const Variant&);
		Variant& operator=(VariantMemory*);
		Variant& operator=(bool);
		Variant& operator=(const std::string&);

	public: // dirrect allocation
		Variant& assignString(ClassLib::RuntimeString& Rs);

	public: /* comparison operators */
		bool	operator<(const Variant& Var) const;
		bool	operator<=(const Variant& Var) const;
		bool	operator>(const Variant& Var) const;
		bool	operator>=(const Variant& Var) const;
		bool	operator==(const Variant& Var) const;

	public: // direct maths operators
		static  bool binary_add						(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_subtract				(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_multiply				(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_divide					(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_moderus					(Variant& Res, const Variant& Lhs, const Variant& Rhs);

	public: // binary logical
		static	bool binary_logic_and				(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_logic_or				(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_logic_xor				(Variant& Res, const Variant& Lhs, const Variant& Rhs);

	public: // binary  bitwise logical
		static	bool binary_bitwise_and				(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_bitwise_or				(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_bitwise_xor				(Variant& Res, const Variant& Lhs, const Variant& Rhs);

	public: // binary logical comparisons
		static	bool binary_less_than				(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_less_than_or_eq			(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_greater_than			(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_greater_than_or_eq		(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_equals					(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_not_equals				(Variant& Res, const Variant& Lhs, const Variant& Rhs);

	public: //  binary bit shifting operations
		static	bool binary_bitshift_left			(Variant& Res, const Variant& Lhs, const Variant& Rhs);
		static	bool binary_bitshift_right			(Variant& Res, const Variant& Lhs, const Variant& Rhs);

	public: // lower case
		string	toString() const;
		bool	toBool() const;
		
		/* floating point numbers */
		float	toFloat()	const;
		double	toDouble()	const;
		
		/* integer numbers */
		int32	toInt32()	const;
		int64	toInt64()	const;

		/* remote class */
		RemoteClass* toRemoteClass() const;

	public: // zipping and unzipping
		std::string encode();
		bool decode(std::string data, char* Start, char** Forward);

	public: // disconnect vm
		void disconnectVM();
	};
}