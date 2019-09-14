#pragma once

#include <vector>
#include "umbra_remoteclassexceptions.h"

namespace Umbra
{
	class Variant;

	//
	//
	//

	class RemoteReference
	{
	protected:
		RemoteReference* next;			// the next reference item
		void*			 var;			// the pointer to the current item
	};

	class RemoteClass
	{
		friend class ResourceManager;
		friend class Variant;
		friend class VariantMemory;

	public: // static symbols for remote class
		static const int UMBRA_FLAGS_ALLOCATE_ONCE	= 1;			// is not removed from the list after first access

	protected: /* reference counting variables */
		bool		_umbra_garbage_collected;
		mutable int	_umbra_res_id;
		mutable int _umbra_ref_count;
		Host*		_umbra_owning_host;

	public: /* automatic registering / unregistering */
		RemoteClass();
		RemoteClass(const RemoteClass&);
		RemoteClass& operator=(const RemoteClass&);
		virtual ~RemoteClass();

	public: // executing internal code
		virtual bool doUmbraProperty   (Umbra::Variant& V, Umbra::PropertyRequest&) { return PropertyRequest::NOT_HANDLED; }
		virtual bool doUmbraFunction   (Umbra::Variant& V, Umbra::FunctionRequest&) { return PropertyRequest::NOT_HANDLED; }
		virtual void onUmbraRegister() { }
		virtual int  getUmbraFlags()	 { return 0; }


	protected: // umbra ref counting features
		int doUmbraAddRef();
		int doUmbraDeleteRef();

	public: // resource manager
		int				getUmbraClassId		(void)						{ return _umbra_res_id;							}
		RemoteClass&	getUmbraRemoteClass	(void)						{ return *dynamic_cast<RemoteClass*>(this);		}		
		void			setUmbraPersistant	(void);
		RemoteClass&	setUmbraGarbageCollected();
	};

	//
	// An exception for an error within an external function
	//

	class NativeRuntimeErrorException
	{
	public: // error occuring within an object
		std::string					error_info;			// the error message given
		std::vector<std::string>	trace;				// the stack trace

	public: // constructor
		NativeRuntimeErrorException(CallStackMonitor* CallStack, std::string Err);
	};

	const int EXM_UPDATE_VARIANT	= 1;
	const int EXM_UPDATE_DATA		= 2;
}

// Function Begin
//
#define UMBRA_FUNCTION_BEGIN(FunctionName)			\
	if (R.id.compare(#FunctionName)	== 0)			\
	{												\
		const char* func_id = #FunctionName;

#define UMBRA_FUNCTION_CS() {
#define UMBRA_FUNCTION_CE() return true; }

// Function End
//
#define UMBRA_FUNCTION_END()				}
#define UMBRA_FUNCTION_END_RN()				return Umbra::Variant(); }

// Function Copy
//
#define UMBRA_PROPERTY_AUTORW(x, Meth)										\
	if (R.isGet() == true)													\
	{	V = x;			}													\
	else if (R.isSet() == true)												\
	{	x = V.To##Meth;	}

// Read Copy
#define UMBRA_PROPERTY_R(name, val)											\
	if (R.isGet() && R.id.compare(#name) == 0)								\
	{																		\
		V = val;															\
		return true;														\
	}

// copy constant
#define UMBRA_PROPERTY_C(name, val) if (R.isGet() && R.id.compare(#name) == 0) { V = val;return true; }

// simple property read
#define UMBRA_PROPERTY_SR(Name, Value) if (R.isGet() && R.id.compare(#Name) == 0){ V = Value; return true; }

// reflected property read
#define UMBRA_PROPERTY_RR(Name)												\
	if (R.isGet() && R.id.compare(#Name) == 0)								\
	{																		\
		V = Name;															\
		return true;														\
	}

// redirect property
#define UMBRA_PROPERTY_REDIRECT(Class)										\
	if (Class::doUmbraProperty(V, R) == Umbra::PropertyRequest::HAS_HANDLED)\
		return Umbra::PropertyRequest::HAS_HANDLED;

//
// Redirect a function to a sub class
//
#define UMBRA_FUNCTION_REDIRECT(Class)										\
	if (Class::doUmbraFunction(V, R) == Umbra::FunctionRequest::HAS_HANDLED)\
		return Umbra::FunctionRequest::HAS_HANDLED;

#define UMBRA_FUNCTION_INCLUDE(Function)									\
	if (Function(V, R) == Umbra::FunctionRequest::HAS_HANDLED)				\
		return Umbra::FunctionRequest::HAS_HANDLED;

#define UMBRA_PROPERTY_WV(x, t) if (R.isSet()) { x = V.To##t (); }

// read write
#define UMBRA_PROPERTY_RW(Name, Var)										\
	if (R.id.compare(#Name) == 0)											\
	{																		\
		umbra_property_exchangefunc(V, Var, R.isGet() ? Umbra::EXM_UPDATE_VARIANT : Umbra::EXM_UPDATE_DATA); \
		return true;														\
	}
	
#define UMBRA_PROPERTY_RW_SN(Name)										\
	if (R.id.compare(#Name) == 0)											\
	{																		\
		umbra_property_exchangefunc(V, Name, R.isGet() ? Umbra::EXM_UPDATE_VARIANT : Umbra::EXM_UPDATE_DATA); \
		return true;														\
	}

#define UMBRA_PROPERTY_W(Name, Var)											\
	if (R.id.compare(#Name) == 0 && R.isGet())								\
	{																		\
		umbra_property_exchangefunc(V, Var, Umbra::EXM_UPDATE_DATA);		\
		return true;														\
	}
	
#define UMBRA_PROPERTY_WE(Name, Expr)										\
	if (R.id.compare(#Name) == 0 && R.isSet())								\
	{																		\
		Expr;																\
		return true;														\
	}
	

//
// Dictated Argument
//

#define UMBRA_FUNCTION_DA(Index, Key, TypeOf)								\
	if (Index >= (int)R.args.size())										\
	{																		\
		char error_info[1024];												\
		sprintf_s(error_info, sizeof(error_info),							\
			"Insufficent arguments given for %s, argument %d missing",				\
			R.id.c_str(), Index);															\
		throw Umbra::NativeRuntimeErrorException(R.call_stack_monitor, error_info);		\
	}																		\
	Umbra::Variant va##Key = R.args[Index];


// 
// Optional Argument
//
#define UMBRA_FUNCTION_OA(Index, Key, TypeOf, DefaultValue)					\
	Umbra::Variant va##Key;													\
	if (Index >= (int)R.args.size())										\
	{																		\
		va##Key = DefaultValue;												\
	}																		\
	else																	\
	{																		\
		va##Key = R.args[Index];											\
	}
//
// Macro to convert an argument into a class of a given type, or throw an error
//

#define UMBRA_THROW_NRE(Trace, ...)											\
	{																		\
		char error_info[1024] = { 0 };										\
		sprintf_s(error_info, sizeof(error_info), __VA_ARGS__);				\
		throw NativeRuntimeErrorException(Trace, error_info);				\
	}


#define UMBRA_FUNCTION_CC(Key, Class)										\
	Class* p##Key = dynamic_cast<Class*>(va##Key.toRemoteClass());			\
	if (p##Key == 0)														\
	{																		\
		char error_info[1024];												\
		sprintf_s(error_info, sizeof(error_info),							\
			"Invalid Argument, %s could not be converted to %s",			\
			#Key, #Class);													\
		throw Umbra::NativeRuntimeErrorException(R.call_stack_monitor, error_info);	\
	}

#define UMBRA_FUNCTION_RET(X) { V = X; return true; }

// reflect to function
#define UMBRA_FUNCTION_R2F(Id, Func)										\
	if (R.id.compare(#Id) == 0)												\
	{																		\
		Func(V, R);															\
		return;																\
	}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, int& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = X;
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = V.toInt32();
	else return false;
	return true;
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, Umbra::Variant& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = X;
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = V;
	else return false;
	return true;
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, float& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = X;
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = V.toFloat();
	else return false;
	return true;
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, double& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = X;
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = V.toDouble();
	else return false;
	return true;
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, std::string& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = X.c_str();
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = V.toString().c_str();
	else return false;
	return true;
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, bool& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = X;
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = V.toBool();
	else return false;
	return true;
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, char& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = (int)X;
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = (char)V.toInt32();
	else return false;
	return true;
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, unsigned char& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = (int)X;
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = (unsigned char)V.toInt32();
	else return false;
	return true;
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, unsigned short& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = (int)X;
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = (unsigned short)V.toInt32();
	else return false;
	return true;
}