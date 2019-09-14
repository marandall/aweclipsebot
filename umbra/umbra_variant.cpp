#include "stdafx.h"

// umbra classes
#include "umbra_variant.h"
#include "umbra_remoteclass.h"
#include "umbra_resourcemanager.h"
#include "umbra_rtstring.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// memory allocation checking
namespace Umbra
{
	VariantMemory& Variant::getVM()
	{
		if (m_ptr2vm)
		{
			return *m_ptr2vm;
		}
		else
		{
			attach_memory(new VariantMemory());
			return *m_ptr2vm;
		}
	}
	
	DataUnion& Variant::getDU()
	{
		return getVM().du;
	}

	DataUnion& Variant::getDU() const
	{
		return m_ptr2vm->du;
	}

	void Variant::detatch_memory()
	{
		if (m_ptr2vm)
		{
			m_ptr2vm->decRef();
			m_ptr2vm = 0;
		}
	}

	void Variant::attach_memory(VariantMemory* VM)
	{
		if (m_ptr2vm)
		{
			detatch_memory();
		}
		m_ptr2vm = VM;
		VM->addRef();
	}

	
	std::string Variant::encode()
	{
		char buffer[64] = { 0 };

		switch (getType()) 
		{
			case Types::Null: 
				{
					sprintf_s(buffer, sizeof(buffer), "%c:;", SolidTypes::Null);
					return buffer;
				}

			case Types::Integer: 
				{
					sprintf_s(buffer, sizeof(buffer), "%c:%d;", SolidTypes::Integer, toInt64());
					return buffer;
				}

			case Types::Double: 
				{
					sprintf_s(buffer, sizeof(buffer), "%c:%d;", SolidTypes::Double, toDouble());
					return buffer;
				}
				
			case Types::Boolean:
				{
					sprintf_s(buffer, sizeof(buffer), "%c:%d;", SolidTypes::Boolean, toBool() ? 1 : 0);
					return buffer;
				}

			case Types::Resource:
				{
					sprintf_s(buffer, sizeof(buffer), "%c:%d;", SolidTypes::Resource, 1);
					return buffer;
				}

			case Types::String:
				{
					// convert to string as reprise
					std::string run;
					std::string copy = toString();

					// copy first section followed by length
					sprintf_s(buffer, sizeof(buffer), "%c:%d:", SolidTypes::String, (int)copy.length());
					run.append(buffer)
						.append(copy)
						.append(";");
					return run;
				}

			default: 
				{
					sprintf_s(buffer, sizeof(buffer), "%c:%d;", SolidTypes::Null, 1);
					return buffer;
				}
		}
	}

	bool Variant::decode(std::string data, char* Start, char** Forward)
	{
		switch (data[0])
		{
			// nullified types
			case SolidTypes::Null:
				{
					SwitchType(Types::Null);

					// jump forward
					*Forward = (Start + 2);
					return true;
				}

			// numeric types
			case SolidTypes::Integer:
			case SolidTypes::Double:
				{
					char* sc = strstr(Start + 2, ";");
					if (sc)
					{
						std::string temp(Start + 2, sc - Start - 2);

						// check type
						if (data[0] == SolidTypes::Integer)
						{
							*this = _atoi64(temp.c_str());
						}
						else
						{
							char* ep = 0;
							*this = strtod(temp.c_str(), &ep);
						}

						// forward jump
						*Forward = sc + 1;
						return true;
					}
					else
					{
						return false;
					}
				}

			// string type
			case SolidTypes::String:
				{
					char* fc = strstr(Start + 2, ":");
					if (fc)
					{
						int length = atoi(std::string(Start + 2, fc - Start - 2).c_str());
						char* sc = strstr(Start + 2, ";");
						if (sc)
						{
							*this = std::string(fc + 1, length);
							
							// forward jump
							*Forward = sc + 1;
							return true;
						}
					}

					// could not process string
					return false;
				}
		};

		return false;
	}


	void Variant::SwitchType(int Type) 
	{
		// clean up the relevant types
		int current_type = getType();
		
		// clean up the remote class type
		if (current_type == Types::Resource || current_type == Types::String )
		{
			// lookup pointer and delete reference
			RemoteClass* pRemote = ResourceManager::GetRM().search(getDU().v_resource);
			if (pRemote)
			{
				pRemote->doUmbraDeleteRef();
			}
		}

		// reset the type
		getVM().type = Type;
	}

	const char* Variant::getRawTypeName()
	{
		switch (getType())
		{
		case Types::Null:		return "null"; 
		case Types::Integer:	return "Int64";
		case Types::Double:		return "double";
		case Types::String:		return "string";
		case Types::Resource:	return "class";
		case Types::Boolean:	return "bool";
		default:				return "Unknown";
		};
	}


	//
	//
	//
	// Constructors
	//
	//
	//

	// reference constructor
	Variant::Variant(VariantMemory* X) 
	{
		m_ptr2vm = NULL;
		*this = X;
	}

	// initialise with a 32 bit int
	Variant::Variant(int32 X) 
	{
		m_ptr2vm = NULL;
		*this = (int64)X;
	}

	// initialise with 64 bit int
	Variant::Variant(int64 X) 
	{
		m_ptr2vm = NULL;
		*this = (int64)X;
	}

	// initialise with floating point
	Variant::Variant(float X) 
	{
		*this = (double)X;
	}
	
	// initialise with double floating point
	Variant::Variant(double X) 
	{
		m_ptr2vm = NULL;
		*this = X;
	}

	// initialise with boolean
	Variant::Variant(bool X) 
	{
		m_ptr2vm = NULL;
		*this = X;
	}
	
	// initialise with string
	Variant::Variant(const char* X) 
	{
		m_ptr2vm = NULL;
		*this = X;
	}

	// initialise with actual string
	Variant::Variant(const std::string& String)
	{
		m_ptr2vm = 0;
		*this = (new Umbra::ClassLib::RuntimeString(String))->setUmbraGarbageCollected();
	}


	// copy constructor
	Variant::Variant(const RemoteClass& Data) 
	{
		m_ptr2vm = NULL;
		*this = Data;
	}

	//
	//
	//
	// Assignment Operators
	//
	//
	//

	// assignment operator
	Variant& Variant::operator=(const Variant& Data) 
	{
		if (Data.IsString() == true)
		{
			assignString(*((Umbra::ClassLib::RuntimeString*)Data.toRemoteClass()));
		}
		else if (Data.IsRemote() == true)
		{
			ResourceManager& rm = ResourceManager::GetRM();
			*this = *Data.toRemoteClass();
		}
		else if (Data.IsNull() == true)
		{
			SwitchType(Types::Null);
		}
		else
		{
			SwitchType(Data.getType());
			getVM().du = Data.getDU();
		}

		return *this;
	}

	//
	// assign against variant memory
	//
	Variant& Variant::operator=(VariantMemory* X) 
	{
		// detatch existing memory
		attach_memory(X);
		return *this;
	}

	// assign unsigned 32 bit integer
	Variant& Variant::operator=(u_int32 V)
	{
		*this = (int64)V;
		return *this;
	}

	//
	// Assign an 32 bit integer to a variant
	//
	Variant& Variant::operator=(int32 V)
	{
		*this = (int64)V;
		return *this;
	}

	//
	// Assign a 64 bit integer to a variant
	//
	Variant& Variant::operator=(int64 V)
	{
		SwitchType(Types::Integer);
		getVM().du.v_int64 = V;
		return *this;
	}

	// 
	// Assign a 32 bit floating point to a variant
	//
	Variant& Variant::operator=(float V)
	{
		*this = (int64)V;
		return *this;
	}

	Variant& Variant::operator=(double V)
	{
		SwitchType(Types::Double);
		getVM().du.v_double = V;
		return *this;
	}

	//
	// Boolean
	//
	Variant& Variant::operator=(bool V)
	{
		SwitchType(Types::Boolean);
		getVM().du.v_boolean = V;
		return *this;
	}

	Variant& Variant::operator=(const char* V)
	{
		SwitchType(Types::Null);

		// assign as a class
		*this = (new Umbra::ClassLib::RuntimeString(V))->setUmbraGarbageCollected();
		
		// hack: change to a string type without releating
		getVM().type = Types::String;
		return *this;
	}

	// initialise with actual string
	Variant& Variant::operator=(const std::string& String)
	{
		Umbra::ClassLib::RuntimeString* pStr = new Umbra::ClassLib::RuntimeString(String);
		pStr->setUmbraGarbageCollected();
		return assignString(*pStr);
	}

	Variant& Variant::assignString(ClassLib::RuntimeString& Rs)
	{
		SwitchType(Types::String);

		// lookup pointer and delete reference
		Rs.doUmbraAddRef();
		getDU().v_resource = Rs._umbra_res_id;

		// reference back
		return *this;
	}
	
	Variant& Variant::operator=(const RemoteClass& V)
	{
		SwitchType(Types::Resource);
		RemoteClass* pV = &const_cast<RemoteClass&>(V);

		// lookup pointer and delete reference
		pV->doUmbraAddRef();
		getDU().v_resource = pV->_umbra_res_id;

		// reference back
		return *this;
	}

	//
	//
	//
	// Maths Operators
	//
	//
	//

	
	// less than or equal to
	bool Variant::operator<=(const Variant& Var) const
	{
		// convert both to unconst
		Variant& v1 = *const_cast<Variant*>(this);
		Variant& v2 = *const_cast<Variant*>(&Var);

		// compare text type
		if (v1.IsString() == true || v2.IsString() == true)
		{
			// lookup string elements
			ClassLib::RuntimeString* pStr1 = static_cast<ClassLib::RuntimeString*>(v1.toRemoteClass());
			ClassLib::RuntimeString* pStr2 = static_cast<ClassLib::RuntimeString*>(v2.toRemoteClass());
	
			// both must exist for the function to work
			if (pStr1 && pStr2)
			{
				return strcmp(pStr1->data.c_str(), pStr2->data.c_str()) <= 0;
			}
			else
			{
				return false;
			}
		}

		// remote classes are compared by their type
		else if (v1.IsDouble() == true)
		{
			return v1.toDouble() <= v2.toDouble();
		}
		else if (v1.IsInteger() == true)
		{
			return v1.toInt64() <= v2.toInt64();
		}

		else if (v1.IsRemote() == true)
		{
			return v1.getVM().du.v_resource <= v2.getVM().du.v_resource;
		}
		else
		{
			return false;
		}
	}
	
	// greater than
	bool Variant::operator>(const Variant& Var) const 
	{
		return !operator<(Var);
	}
	
	// greater than or equal to
	bool Variant::operator>=(const Variant& Var) const 
	{
		return !operator<=(Var);
	}

	// equality
	bool Variant::operator==(const Variant& Var) const 
	{
		// convert both to unconst
		Variant& v1 = *const_cast<Variant*>(this);
		Variant& v2 = *const_cast<Variant*>(&Var);

		// always a null check
		if (v1.IsNull() && v2.IsNull())
		{
			return true;
		}

		// native boolean
		else if (v1.IsBool() && v2.IsBool())
		{
			return v1.toBool() == v2.toBool();
		}

		// string equality
		else if (v1.IsString() || v2.IsString())
		{
			ClassLib::RuntimeString* pS1 = static_cast<ClassLib::RuntimeString*>(v1.toRemoteClass());
			ClassLib::RuntimeString* pS2 = static_cast<ClassLib::RuntimeString*>(v2.toRemoteClass());
			if (pS1 && pS2)
			{
				return strcmp(pS1->data.c_str(), pS2->data.c_str()) == 0;
			}
			else
			{
				return false;
			}
		}

		// equality in remote classes is done via references
		else if (v1.IsRemote() || v2.IsRemote())
		{
			return v1.toRemoteClass() == v2.toRemoteClass();
		}
		
		else if (v1.IsDouble() || v2.IsDouble())
		{
			return fabs(v1.toDouble() - v2.toDouble()) < 0.000000001;
		}
		
		else if (v1.IsInteger() || v2.IsInteger() || v1.IsNull() || v2.IsNull())
		{
			return v1.toInt64() == v2.toInt64();
		}
		else
		{
			return false;
		}
	}

	//
	//
	//
	// Assignment Operators
	//
	//
	//

	// produce a boolean method
	bool Variant::toBool() const 
	{
		this;
		switch (getType())
		{
			case Types::Null:
				return false;
				
			case Types::Boolean: // boolean
				return getDU().v_boolean;

			case Types::Integer: 
			case Types::Double: 
				{
					return static_cast<bool>( toDouble() != 0 );
					break;
				}

			case Types::Resource: 
				{
					return toRemoteClass() ? true : false;
					break;
				}

			case Types::String:
				{
					return toString().length() > 0;
				}

			default: {
					return false;
					break;
				}
		};
	}

	// produce a 32 bit integer
	int32 Variant::toInt32() const 
	{
		switch (getType()) 
		{
			case Types::Null: 
				{
					return 0;
					break;
				}
			case Types::Integer: 
				{
					return (int)getDU().v_int64;
					break;
				}
			case Types::Double: 
				{
					return (int)getDU().v_double;
					break;
				}
			case Types::Resource: 
				{
					char* pEnd;
					return strtol(toString().c_str(), &pEnd, 0);
					break;
				}
			default: 
				{
					return 0;
					break;
				}
		};
	}
	
	// produce a 64 bit integer
	int64 Variant::toInt64() const		
	{
		switch (getType())
		{
			case Types::Null: 
				{
					return 0;
					break;
				}
			case Types::Integer: 
				{
					return getDU().v_int64;
					break;
				}
			case Types::Double:
				{
					return (int64)getDU().v_double;
					break;
				}
			case Types::Resource:
				{
					return _atoi64(toString().c_str());
					break;
				}
			default: 
				{
					return 0;
					break;
				}
		};
	}


	// produce a 32 bit floating point
	float Variant::toFloat() const
	{
		return (float)toDouble();
	}

	// produce a 64 bit floating point
	double Variant::toDouble() const
	{
		switch (getType()) 
		{
			case Types::Null: 
				{
					return 0;
					break;
				}

			case Types::Integer: 
				{
					return static_cast<double>(getDU().v_int64);
					break;
				}

			case Types::Double: 
				{
					return static_cast<double>(getDU().v_double);
					break;
				}

			default: 
				{
					return static_cast<double>(0.0F);
					break;
				}
		};
	}

	// produce a string
	string Variant::toString() const
	{
		char format_text[64] = { 0 };

		switch (getType()) 
		{
			case Types::Null: 
				{
					return "(null)";
				}

			case Types::Integer: 
				{
					sprintf_s(format_text, sizeof(format_text), "%d", getDU().v_int64);
					return format_text;
				}

			case Types::Double: 
				{
					sprintf_s(format_text, sizeof(format_text), "%f", getDU().v_double);
					return format_text;
				}
				
			case Types::Boolean:
				{
					return getDU().v_boolean ? "true" : "false";
				}

			case Types::Resource:
				{
					sprintf_s(format_text, sizeof(format_text), "(resource:%d)", getDU().v_resource);
					return format_text;
				}

			case Types::String:
				{
					ClassLib::RuntimeString* rs = static_cast<ClassLib::RuntimeString*>(toRemoteClass());
					if (rs)
					{
						return rs->data;
					}
					else
					{
						return "(null_string)";
					}
				}

			default: 
				{
					return "INVALID_CLASS";
					break;
				}
		};
	}

	// convert to a remote class or null
	RemoteClass* Variant::toRemoteClass() const 
	{
		// no remote class
		if (IsClassInterface() == false)
			return NULL;

		// lookup the index, if it does not exist, null
		return ResourceManager::GetRM().search(getDU().v_resource);
	}

	void Variant::disconnectVM()
	{
		if (m_ptr2vm)
		{
			SwitchType(Types::Null);
			m_ptr2vm;
		}
	}

	//
	//
	//
	// Binary Maths Operations
	//
	//
	//


	bool Variant::binary_subtract(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. doubles
		if (Lhs.IsDouble() == true || Rhs.IsDouble() == true)
		{
			Res = Lhs.toDouble() - Rhs.toDouble();
		}

		// 2. integers
		else if (Lhs.IsInteger() == true || Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() - Rhs.toInt64();
		}

		// 3. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	bool Variant::binary_multiply(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. doubles
		if (Lhs.IsDouble() == true || Rhs.IsDouble() == true)
		{
			double left = Lhs.toDouble();
			double right = Rhs.toDouble();

			Res = Lhs.toDouble() * Rhs.toDouble();
		}

		// 2. integers
		else if (Lhs.IsInteger() == true && Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() * Rhs.toInt64();
		}

		// 3. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	bool Variant::binary_divide(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. doubles
		if (Lhs.IsDouble() == true || Rhs.IsDouble() == true)
		{
			Res = Lhs.toDouble() / Rhs.toDouble();
		}

		// 2. integers
		else if (Lhs.IsInteger() == true && Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() / Rhs.toInt64();
		}

		// 3. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	bool Variant::binary_moderus(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 2. integers
		if (Lhs.IsInteger() == true || Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() % Rhs.toInt64();
		}

		// 3. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	// logical and based upon booleans
	bool Variant::binary_logic_and(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		bool lhs = Lhs.toBool();
		bool rhs = Rhs.toBool();

		Res = lhs && rhs;

		// 0. good
		return true;
	}

	// logical or based upon booleans
	bool Variant::binary_logic_or(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		Res = Lhs.toBool() || Rhs.toBool();

		// 0. good
		return true;
	}

	// logical xor using integers
	bool Variant::binary_logic_xor(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		Res = Lhs.toInt64() ^ Rhs.toInt64();

		// 0. good
		return true;
	}

	// bitwise and using integers
	bool Variant::binary_bitwise_and(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. integers
		if (Lhs.IsInteger() == true && Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() & Rhs.toInt64();
		}
		
		// 2. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	// bitwise or using integers
	bool Variant::binary_bitwise_or(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. integers
		if (Lhs.IsInteger() == true && Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() | Rhs.toInt64();
		}
		
		// 2. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	// bitwise xor using integers
	bool Variant::binary_bitwise_xor(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. integers
		if (Lhs.IsInteger() == true && Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() ^ Rhs.toInt64();
		}
		
		// 2. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	bool Variant::binary_bitshift_left(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. integers
		if (Lhs.IsInteger() == true && Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() << Rhs.toInt32();
		}
		
		// 2. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	bool Variant::binary_bitshift_right(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. integers
		if (Lhs.IsInteger() == true && Rhs.IsInteger() == true)
		{
			Res = Lhs.toInt64() >> Rhs.toInt32();
		}
		
		// 2. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}

	bool Variant::binary_less_than(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// priority double
		Res = (Lhs < Rhs);

		// 0. good
		return true;
	}

	bool Variant::binary_less_than_or_eq(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// priority double
		Res = (Lhs <= Rhs);

		// 0. good
		return true;
	}

	bool Variant::binary_greater_than(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// priority double
		if (Lhs.IsDouble() || Rhs.IsDouble())
		{
			Res = Lhs.toDouble() > Rhs.toDouble();
		}

		// priority int64
		else if (Lhs.IsInteger() || Rhs.IsInteger())
		{
			Res = Lhs.toInt64() > Rhs.toInt64();
		}

		// priority other
		else
		{
			Res = Lhs.toDouble() > Rhs.toDouble();
		}

		// 0. good
		return true;
	}

	bool Variant::binary_greater_than_or_eq(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// priority double
		if (Lhs.IsDouble() || Rhs.IsDouble())
		{
			Res = Lhs.toDouble() >= Rhs.toDouble();
		}

		// priority int64
		else if (Lhs.IsInteger() || Rhs.IsInteger())
		{
			Res = Lhs.toInt64() >= Rhs.toInt64();
		}

		// priority other
		else
		{
			Res = Lhs.toDouble() >= Rhs.toDouble();
		}

		// 0. good
		return true;
	}

	bool Variant::binary_equals(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		Res = (Lhs == Rhs);

		// 0. good
		return true;
	}

	bool Variant::binary_not_equals(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{					
		Res = !(Lhs == Rhs);

		// 0. good
		return true;
	}

	// less than operator
	bool Variant::operator<(const Variant& Var) const
	{
		// convert both to unconst
		Variant& v1 = *const_cast<Variant*>(this);
		Variant& v2 = *const_cast<Variant*>(&Var);

		// compare text type
		if (v1.IsString() == true || v2.IsString() == true)
		{
			// lookup string elements
			ClassLib::RuntimeString* pStr1 = static_cast<ClassLib::RuntimeString*>(v1.toRemoteClass());
			ClassLib::RuntimeString* pStr2 = static_cast<ClassLib::RuntimeString*>(v2.toRemoteClass());
	
			// both must exist for the function to work
			if (pStr1 && pStr2)
			{
				return strcmp(pStr1->data.c_str(), pStr2->data.c_str()) < 0;
			}
			else
			{
				return false;
			}
		}

		// remote classes are compared by their type
		else if (v1.IsDouble() == true || v2.IsDouble() == true)
		{
			return v1.toDouble() < v2.toDouble();
		}

		else if (v1.IsInteger() == true || v2.IsBool() == true)
		{
			int64 il = v1.toInt64();
			int64 ir = v2.toInt64();
			return il < ir;
		}

		else if (v1.IsRemote() == true)
		{
			return v1.getVM().du.v_resource < v2.getVM().du.v_resource;
		}

		else
		{
			return false;
		}
	}

	bool Variant::binary_add(Variant& Res, const Variant& Lhs, const Variant& Rhs)
	{
		// 1. handle strings first
		if (Lhs.IsString() == true || Rhs.IsString() == true)
		{
			string str1 = Lhs.toString();
			string str2 = Rhs.toString();

			str1.append(str2);
			Res = str1.c_str();
		}

		// 2. doubles
		else if (Lhs.IsDouble() == true || Rhs.IsDouble() == true)
		{
			Res = Lhs.toDouble() + Rhs.toDouble();
		}

		// 3. integers
		else if (Lhs.IsInteger() || Rhs.IsInteger())
		{
			Res = Lhs.toInt64() + Rhs.toInt64();
		}

		// 4. none
		else
		{
			return false;
		}

		// 0. good
		return true;
	}
}