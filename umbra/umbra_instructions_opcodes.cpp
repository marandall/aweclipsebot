#include "stdafx.h"
#include "umbra_instructions_opcodes.h"
#include "umbra_instructions_controlstatements.h"
#include "umbra_remoteclass.h"
#include "umbra_executioncontroller.h"
#include "umbra_host.h"
#include "umbra_classlib.h"
#include "umbra_rtstring.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BORT_REQUIRE_BOTH()																										\
			/* left hand side operations */																						\
			if (prev) { switch (prev->getOpType()) {																			\
			case Opcode::OPTYPE_BINARY:		(static_cast<BinaryOperator*>(prev))->run(Ec, Ee, left); break;						\
			case Opcode::OPTYPE_UNARY:		(static_cast<UnaryOperator*>(prev))->run(Ec, Ee, left); break;						\
			case Opcode::OPTYPE_CONSTANT:	(static_cast<Constant*>(prev))->run(Ec, Ee, left);  break;							\
			case Opcode::OPTYPE_BRACKETED:	(static_cast<Bracketed*>(prev))->run(Ec, Ee, left);	 break;							\
			case Opcode::OPTYPE_FUNCTION:	(static_cast<Function*>(prev))->run(Ec, Ee, left);  break;							\
			case Opcode::OPTYPE_SYMBOL:		(static_cast<TextSymbol*>(prev))->run(Ec, Ee, left); break;							\
			default: __assume(0); } }																							\
			/* right hand side operations*/																						\
			if (next) { switch(next->getOpType()) {																				\
			case Opcode::OPTYPE_BINARY:		(static_cast<BinaryOperator*>(next))->run(Ec, Ee, right); break;					\
			case Opcode::OPTYPE_UNARY:		(static_cast<UnaryOperator*>(next))->run(Ec, Ee, right); break;						\
			case Opcode::OPTYPE_CONSTANT:	(static_cast<Constant*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_BRACKETED:	(static_cast<Bracketed*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_FUNCTION:	(static_cast<Function*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_SYMBOL:		(static_cast<TextSymbol*>(next))->run(Ec, Ee, right); break;						\
			default: __assume(0); } }

#define BORT_REQUIRE_LEFT()																										\
			/* left hand side operations */																						\
			switch (prev->getOpType()) {																						\
			case Opcode::OPTYPE_BINARY:		(static_cast<BinaryOperator*>(prev))->run(Ec, Ee, left); break;						\
			case Opcode::OPTYPE_UNARY:		(static_cast<UnaryOperator*>(prev))->run(Ec, Ee, left); break;						\
			case Opcode::OPTYPE_CONSTANT:	(static_cast<Constant*>(prev))->run(Ec, Ee, left);  break;							\
			case Opcode::OPTYPE_BRACKETED:	(static_cast<Bracketed*>(prev))->run(Ec, Ee, left);	 break;							\
			case Opcode::OPTYPE_FUNCTION:	(static_cast<Function*>(prev))->run(Ec, Ee, left);  break;							\
			case Opcode::OPTYPE_SYMBOL:		(static_cast<TextSymbol*>(prev))->run(Ec, Ee, left); break;							\
			default: __assume(0); }

#define BORT_REQUIRE_RIGHT()																										\
			if (next) { switch(next->getOpType()) {																				\
			case Opcode::OPTYPE_BINARY:		(static_cast<BinaryOperator*>(next))->run(Ec, Ee, right); break;					\
			case Opcode::OPTYPE_UNARY:		(static_cast<UnaryOperator*>(next))->run(Ec, Ee, right); break;						\
			case Opcode::OPTYPE_CONSTANT:	(static_cast<Constant*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_BRACKETED:	(static_cast<Bracketed*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_FUNCTION:	(static_cast<Function*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_SYMBOL:		(static_cast<TextSymbol*>(next))->run(Ec, Ee, right); break;						\
			default: __assume(0); } }

#define BORT_BINARY_FUNCTION(Symbol, FunctionX)																					\
		case Symbol: 																											\
			/* left hand side operations */																						\
			if (prev) { switch (prev->getOpType()) {																			\
			case Opcode::OPTYPE_BINARY:		(static_cast<BinaryOperator*>(prev))->run(Ec, Ee, left); break;						\
			case Opcode::OPTYPE_UNARY:		(static_cast<UnaryOperator*>(prev))->run(Ec, Ee, left); break;						\
			case Opcode::OPTYPE_CONSTANT:	(static_cast<Constant*>(prev))->run(Ec, Ee, left);  break;							\
			case Opcode::OPTYPE_BRACKETED:	(static_cast<Bracketed*>(prev))->run(Ec, Ee, left);	 break;							\
			case Opcode::OPTYPE_FUNCTION:	(static_cast<Function*>(prev))->run(Ec, Ee, left);  break;							\
			case Opcode::OPTYPE_SYMBOL:		(static_cast<TextSymbol*>(prev))->run(Ec, Ee, left); break;							\
			default: __assume(0); } }																							\
			/* right hand side operations*/																						\
			if (next) { switch(next->getOpType()) {																				\
			case Opcode::OPTYPE_BINARY:		(static_cast<BinaryOperator*>(next))->run(Ec, Ee, right); break;					\
			case Opcode::OPTYPE_UNARY:		(static_cast<UnaryOperator*>(next))->run(Ec, Ee, right); break;						\
			case Opcode::OPTYPE_CONSTANT:	(static_cast<Constant*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_BRACKETED:	(static_cast<Bracketed*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_FUNCTION:	(static_cast<Function*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_SYMBOL:		(static_cast<TextSymbol*>(next))->run(Ec, Ee, right); break;						\
			default: __assume(0); } }																							\
			Umbra::Variant::FunctionX(RSU.ret, left.ret, right.ret);															\
			return;

#define BORT_BINARY_FUNCTION_SE(Symbol, FunctionX)																				\
		case Symbol: 																											\
			/* left hand side operations */																						\
			if (prev) { switch (prev->getOpType()) {																			\
			case Opcode::OPTYPE_BINARY:		(static_cast<BinaryOperator*>(prev))->run(Ec, Ee, left); break;						\
			case Opcode::OPTYPE_UNARY:		(static_cast<UnaryOperator*>(prev))->run(Ec, Ee, left); break;						\
			case Opcode::OPTYPE_CONSTANT:	(static_cast<Constant*>(prev))->run(Ec, Ee, left);  break;							\
			case Opcode::OPTYPE_BRACKETED:	(static_cast<Bracketed*>(prev))->run(Ec, Ee, left);	 break;							\
			case Opcode::OPTYPE_FUNCTION:	(static_cast<Function*>(prev))->run(Ec, Ee, left);  break;							\
			case Opcode::OPTYPE_SYMBOL:		(static_cast<TextSymbol*>(prev))->run(Ec, Ee, left); break;							\
			default: __assume(0); } }																							\
			/* right hand side operations*/																						\
			if (next) { switch(next->getOpType()) {																				\
			case Opcode::OPTYPE_BINARY:		(static_cast<BinaryOperator*>(next))->run(Ec, Ee, right); break;					\
			case Opcode::OPTYPE_UNARY:		(static_cast<UnaryOperator*>(next))->run(Ec, Ee, right); break;						\
			case Opcode::OPTYPE_CONSTANT:	(static_cast<Constant*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_BRACKETED:	(static_cast<Bracketed*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_FUNCTION:	(static_cast<Function*>(next))->run(Ec, Ee, right); break;							\
			case Opcode::OPTYPE_SYMBOL:		(static_cast<TextSymbol*>(next))->run(Ec, Ee, right); break;						\
			default: __assume(0); } }																							\
																																\
			Umbra::Variant::FunctionX(left.ret, left.ret, right.ret);															\
			RSU.ret = left.ret;																								\
			if (left.cls.IsClassInterface() == true)																			\
			{																													\
				RemoteClass* pRemote = left.cls.toRemoteClass();																\
				if (pRemote != 0)																								\
				{																												\
					PropertyRequest pr;																							\
					pr.id		= left.sym;																						\
					pr.method	= Umbra::PropertyRequest::METHOD_SET;															\
					pRemote->doUmbraProperty(left.ret, pr);																		\
				}																												\
			}


// start of namespace
namespace Umbra { namespace Instructions { namespace Opcodes {

	//
	// Operation defenition system
	//

	std::vector<OperatorDefenition> op_order(T_TOKEN_MAX);

	std::vector<OperatorDefenition>& OpcodeParser::getOperatorDefenitions()
	{
		static bool empty_list = true;
		if (empty_list == true)
		{
			int order		= 0;
			int direction	= OperatorDefenition::RIGHT_TO_LEFT;
			empty_list		= false;

			// constant orders
			const int T_BINARY = OperatorDefenition::T_BINARY;
			const int T_UNARY  = OperatorDefenition::T_UNARY;

			// stage 0
			order++;
			direction	= OperatorDefenition::RIGHT_TO_LEFT;
			op_order[T_MEMBER_ACCESS			] = OperatorDefenition(order, direction, T_BINARY);

			// stage 1
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_INCREMENT					] = OperatorDefenition(order, direction, T_UNARY);
			op_order[T_DECREMENT					] = OperatorDefenition(order++, direction, T_UNARY);
		
			// stage 2 - casting
			// direction = OperatorDefenition::RIGHT_TO_LEFT;
			op_order[T_CAST_INT						] = OperatorDefenition(order, direction, T_UNARY);
			op_order[T_CAST_BOOL					] = OperatorDefenition(order, direction, T_UNARY);
			op_order[T_CAST_FLOAT					] = OperatorDefenition(order, direction, T_UNARY);
			op_order[T_LOGICAL_NOT				] = OperatorDefenition(order, direction, T_UNARY);
			op_order[T_NEW								] = OperatorDefenition(order, direction, T_UNARY);

			// stage 3
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_DIVIDE							] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_MULTIPLY						] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_MODERUS						] = OperatorDefenition(order, direction, T_BINARY);

			// stage 4
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_SUBTRACT						] = OperatorDefenition(order, direction, T_BINARY);
			op_order[T_ADD								] = OperatorDefenition(order, direction, T_BINARY);

			// stage 5
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_BITWISE_LEFT_SHIFT	] = OperatorDefenition(order, direction, T_BINARY);
			op_order[T_BITWISE_RIGHT_SHIFT] = OperatorDefenition(order, direction, T_BINARY);

			// stage 6 - logic
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_LESS_THAN					] = OperatorDefenition(order, direction, T_BINARY);
			op_order[T_LESS_THAN_OR_EQUAL	] = OperatorDefenition(order, direction, T_BINARY);
			op_order[T_GREATER_THAN				] = OperatorDefenition(order, direction, T_BINARY);
			op_order[T_GREATER_THAN_OR_EQUAL		] = OperatorDefenition(order, direction, T_BINARY);

			// stage 7 - equality
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_LOGICAL_EQUALS			] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_LOGICAL_IDENTICAL	] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_LOGICAL_NOT_EQUALS	] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_LOGICAL_NOT_IDENTICAL ] = OperatorDefenition(order++, direction, T_BINARY);


			// stage 8 - bitwise logical
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_BITWISE_AND				] = OperatorDefenition(order, direction, T_BINARY);
			op_order[T_BITWISE_XOR				] = OperatorDefenition(order, direction, T_BINARY);
			op_order[T_BITWISE_OR					] = OperatorDefenition(order, direction, T_BINARY);

			// stage 9 - logical functions
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_LOGICAL_AND				] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_LOGICAL_OR					] = OperatorDefenition(order++, direction, T_BINARY);

			// stage 10 - assignment
			order++;
			direction	= OperatorDefenition::LEFT_TO_RIGHT;
			op_order[T_ASSIGN							] = OperatorDefenition(order, direction, T_BINARY);

			direction	= OperatorDefenition::RIGHT_TO_LEFT;
			op_order[T_ADD_ASSIGN					] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_SUBTRACT_ASSIGN		] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_MULTIPLY_ASSIGN		] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_DIVIDE_ASSIGN			] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_MODERUS_ASSIGN			] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_BITWISE_AND_ASSIGN	] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_BITWISE_OR_ASSIGN	] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_BITWISE_XOR_ASSIGN	] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_BITWISE_LEFT_SHIFT_ASSIGN	] = OperatorDefenition(order++, direction, T_BINARY);
			op_order[T_BITWISE_RIGHT_SHIFT_ASSIGN	] = OperatorDefenition(order++, direction, T_BINARY);

			// returning
			order++;
			op_order[T_RETURN							] = OperatorDefenition::OperatorDefenition(order, direction, T_BINARY);
		}

		return op_order;
	}

	OperatorDefenition* OpcodeParser::getOperatorDefenition(int OpType)
	{
		std::vector<OperatorDefenition>& ops = getOperatorDefenitions();

		// check if the opcode is in the list
		for (size_t i = 0; i < ops.size(); i++)
			if (i == OpType)
				return &(ops[i]);
		return 0;
	}

	//
	// Constructor
	//

	Opcode::Opcode() 
	{ 
		prev = 0; 
		next = 0; 
		opcode_type = 0; 
		debug_line = 0;
	}
	
	Opcode::Opcode(const Opcode& C)
	{
		prev = C.prev ? C.prev->clone() : 0;
		next = C.next ? C.next->clone() : 0;
		
		opcode_type = C.opcode_type;
		debug_line = C.debug_line;
	}

	//
	// Destructors
	//

	Opcode::~Opcode()
	{
		destroy();
	}

	void Opcode::destroy()
	{
		//if (prev) { Opcode* p = prev; prev = 0; delete prev; }
		//if (next) { Opcode* p = next; next = 0; delete next; }
		if (prev) { delete prev; }
		if (next) { delete next; }

		prev = next = 0;
	}

	//
	// Cloners
 	//

	Opcode* Bracketed::clone()
	{
		return new Bracketed(*this);
	}

	Opcode* TextSymbol::clone()
	{
		return new TextSymbol(*this);
	}

	Opcode* Constant::clone()
	{
		return new Constant(*this);
	}

	Opcode* Function::clone()
	{
		return new Function(*this);
	}

	Opcode* BinaryOperator::clone()
	{
		return new BinaryOperator(*this);
	}

	Opcode* UnaryOperator::clone()
	{
		return new UnaryOperator(*this);
	}


	void Bracketed::run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU)
	{
		// execute the child
		child.run(Ec, RSU);
	}

	void TextSymbol::run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU)
	{
		// check if a term has been applied to it
		VariantMap::iterator itr = Ee.vars.m_map.find(text);
		if (itr != Ee.vars.m_map.end())
		{
			RSU.ret = &Ee.vars.m_map[text].getVM();
			return;
		}

		// constant terms
		else if (text.compare("null") == 0)
		{
			RSU.ret = Variant();
			return;
		}

		// boolean terms
		else if (text.compare("true") == 0)
		{
			RSU.ret = true;
			return;
		}

		// boolean terms
		else if (text.compare("false") == 0)
		{
			RSU.ret = false;
			return;
		}

		// predefined variables
		if (Ec.extarg_ptr)
		{
			VariantMap::iterator itr = Ec.extarg_ptr->m_map.find(text);
			if (itr != Ec.extarg_ptr->end())
			{
				RSU.ret = &itr->second.getVM();
				return;
			}
		}

		// this pointer
		if (Ec.this_context && text.compare("this") == 0)
		{
			RSU.ret = *Ec.this_context;
			return;
		}

		// access the application store
		else if (Ec.host_ptr)
		{
			VariantMap::iterator vi = Ec.host_ptr->app_globals.m_map.find(text);
			if (vi != Ec.host_ptr->app_globals.end())
			{
				RSU.ret = vi->second;
				return;
			}
		}

		// new variable on the stack
		if (false) // Ee.access_for_read == true)
		{
			UMBRA_THROW_NRE(Ec.call_stack_monitor, "Attemping to read uninitialised variable '%s'", text.c_str());
		}
		else
		{
			RSU.ret = &Ee.vars.m_map[text].getVM();
		}
	}

	void Constant::run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU)
	{
		if (var.IsString() == true)
		{
			// allocate new string
			Umbra::ClassLib::RuntimeString* pStr = new Umbra::ClassLib::RuntimeString(var.toString());
			pStr->setUmbraGarbageCollected();

			// assign to this host
			RSU.ret.assignString(*pStr);
		}
		else if (var.IsClassInterface() == true)
		{
			RSU.ret = var;
		}
		else
		{
			RSU.ret = var;
		}
	}

	void Function::run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU)
	{
		// require a new execution controller
		ExecutionController ecChild;
		ecChild.host_ptr						= Ec.host_ptr;
		ecChild.this_context				= Ec.this_context;
		ecChild.call_stack_monitor	= Ec.call_stack_monitor;

		// lookup the function code from the function id
		Umbra::Host::FunctionIterator fi = Ec.host_ptr->functions.find(name);
		if (fi != Ec.host_ptr->functions.end())
		{
			// build a function request object
			FunctionRequest rq;
			RuntimeStackUnit rsu;

			VariantMap vmArgs;

			// the variant map is linked to the rsu
			ecChild.extarg_ptr		 = &vmArgs;

			// for every argument
			for (size_t i = 0; i < fi->second->args.size(); i++)
			{
				// if we have a relevant function
				if (i < arglist.size())
				{
					arglist[i].run(Ec, rsu);
				}
				else
				{
					fi->second->args[i].default_value.run(Ec, rsu);
				}
				
				// assign to the list
				vmArgs[fi->second->args[i].func_name] = rsu.ret;	
			}


			// execute the function
			try
			{
				// generate monitor stack
				CallStackMonitorFrame csmf(Ec.call_stack_monitor, fi->first, debug_line);
				fi->second->code.run(ecChild);
			}
			catch (Instructions::ControlStatements::ReturnException& re)
			{
				RSU.ret = &re.result.getVM();
			}
		}
		else
		{
			RemoteClass* pRemote = Ec.host_ptr->app_globals["Usl"].toRemoteClass();
			if (pRemote == 0) return;
			
			// create a functional request object
			FunctionRequest rq;
			rq.host								= Ec.host_ptr;
			rq.id									= name;
			rq.called_pointer			= pRemote;
			rq.call_stack_monitor	= Ec.call_stack_monitor;

			// add the arguments
			for (size_t i = 0; i < arglist.size(); i++)
			{
				RuntimeStackUnit rsu;
				arglist[i].run(Ec, rsu);

				// returned argument is set
				rq.args.push_back(rsu.ret);
			}

			// flip through the operations
			if (pRemote->doUmbraFunction(RSU.ret, rq) == FunctionRequest::HAS_HANDLED)
				return;

			// problem		
			UMBRA_THROW_NRE( Ec.call_stack_monitor, "[%d] Unable to find '%s' as function or static member", debug_line, name.c_str());
		}
	}

	inline void bop_opcode_assign(ExecutionController& Ec, Opcode* prev, Opcode* next, Umbra::RuntimeStackUnit& left, Umbra::RuntimeStackUnit& right, OpcodeEvaluation& Ee, RuntimeStackUnit& RSU)
	{
		// requires both arguments precalculated
		BORT_REQUIRE_BOTH();

		// the variable we are assigning to is just some other variable
		// therefore only set it and assign copy value
		if (left.cls.IsNull() == true)
		{
			left.ret = right.ret;
			RSU.ret  = left.ret;
			return;
		}
		else
		{
			RemoteClass* pRemote = left.cls.toRemoteClass();
			if (pRemote != 0)
			{
				// create property request object
				PropertyRequest pr;
				pr.id		= left.sym;
				pr.method	= Umbra::PropertyRequest::METHOD_SET;

				// run property set method
				pRemote->doUmbraProperty(right.ret, pr);
				RSU.ret = right.ret;
				return;
			}
		}
	}

	inline void bop_opcode_member(
		Umbra::ExecutionController& Ec,
		Opcode* CurOp,
		Opcode* prev, Opcode* next, 
		Umbra::RuntimeStackUnit& left, Umbra::RuntimeStackUnit& right, 
		OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU)
	{
		// requires a test on the left hand side first
		Ee.access_for_read = true;

		// we always knock out the new processor first thing
		bool is_new_processor = Ec.is_new_processor;
		Ec.is_new_processor = false;

		BORT_REQUIRE_LEFT();

		// cast to either a function or a 
		RemoteClass* pRemote = left.ret.toRemoteClass();

		// handle a function first
		if (next && next->getOpcodeType() == Opcode::OPTYPE_FUNCTION)
		{
			// cast to a function object
			Function* pFunc = static_cast<Function*>(next);

			// if there is no class
			if (pRemote == 0)
			{
				UMBRA_THROW_NRE( Ec.call_stack_monitor, "[%d] Attempting to call member function '%s' on non-class object", CurOp->debug_line, pFunc->name.c_str() );
			}
			else
			{
				// for new processing
				Variant newalloc_class;

				// is this new mode
				if (is_new_processor == true)
				{
					// the property function
					Umbra::PropertyRequest pr;
					pr.method = PropertyRequest::METHOD_GET;
					pr.host = Ec.host_ptr;
					pr.id   = pFunc->name;

					// search for this property
					Umbra::Variant vx;
					if (pRemote->doUmbraProperty(vx, pr) == pr.NOT_HANDLED)
					{
						UMBRA_THROW_NRE(Ec.call_stack_monitor, "[%d] Class interface does not support the property '%s'", CurOp->debug_line, pFunc->name.c_str());
					}

					// cast to new function
					pRemote = vx.toRemoteClass();
				}

				// create a functional request object
				FunctionRequest rq;
				rq.host								= Ec.host_ptr;
				rq.id									= pFunc->name;
				rq.called_pointer			= pRemote;
				rq.call_stack_monitor	= Ec.call_stack_monitor;

				// add the arguments
				for (size_t i = 0; i < pFunc->arglist.size(); i++)
				{
					RuntimeStackUnit rsu;
					pFunc->arglist[i].run(Ec, rsu);
					rq.args.push_back(rsu.ret);
				}

				// is this a new processor
				if (is_new_processor == true)
				{
					// does this class support the new instance creator
					Umbra::INewInstanceCreator* ic = dynamic_cast<Umbra::INewInstanceCreator*>(pRemote);
					if (ic != 0)
					{
						RemoteClass* pRem = ic->doUmbraNewInstance(Ec.host_ptr);
						pRem->setUmbraGarbageCollected();

						// becomes out new base pointer
						pRemote = pRem;
						RSU.ret = *pRem;

						rq.called_pointer = pRem;
					}

					// change the call style to be the constructor
					rq.id = "__construct";
				}

				// create actual name for this function
				const type_info& ti = typeid(*pRemote);
				std::string ti_str = ti.name();
				ti_str.append("::").append(rq.id);

				// generate stack frame
				CallStackMonitorFrame csmf(Ec.call_stack_monitor, ti_str, CurOp->debug_line);
					
				// call function
				if (is_new_processor == true)
				{
					RuntimeStackUnit rsu_dummy;
					pRemote->doUmbraFunction(rsu_dummy.ret, rq);
				}
				else
				{	
					bool req = pRemote->doUmbraFunction(RSU.ret, rq);
					if (req == FunctionRequest::NOT_HANDLED)
					{
						const type_info& ci = typeid(pRemote);
						UMBRA_THROW_NRE( Ec.call_stack_monitor, 
							"[%d] Class interface '%s' does not support the function '%s(...)'", 
							CurOp->debug_line, 
							ci.name(), 
							pFunc->name.c_str() );
					}
				}
			}
		}

		// handle as a property access
		else if (next && next->getOpcodeType() == Opcode::OPTYPE_SYMBOL)
		{
			// cast to a text symbol object
			TextSymbol* pConst = static_cast<TextSymbol*>(next);

			// if there is no class
			if (pRemote == 0)
			{
				UMBRA_THROW_NRE( Ec.call_stack_monitor, "[%d] Attempting to call member property '%s' on non-class object", 
					CurOp->debug_line, pConst->text.c_str() );
			}
			else
			{
				// create property request object
				PropertyRequest pr;
				pr.id									= pConst->text.c_str();
				pr.method							= PropertyRequest::METHOD_GET;
				pr.called_pointer			= pRemote;
				pr.host								= Ec.host_ptr;
				pr.call_stack_monitor	= Ec.call_stack_monitor;

				// run property get method
				if( pRemote->doUmbraProperty(RSU.ret, pr) == PropertyRequest::NOT_HANDLED )
				{
					UMBRA_THROW_NRE(Ec.call_stack_monitor, "[%d] Class interface does not support the property '%s'", CurOp->debug_line, pConst->text.c_str());
				}

				// use a reference point
				if (pr.explicit_reference == false)
				{
					RSU.cls = pRemote->getUmbraRemoteClass();
					RSU.sym = (char*)pConst->text.c_str();
				}
				return;
			}
		}
	}

	// used for function below and prevents accessing static data
	// that may throw off the performance counting
	Variant plus_one = 1;
	Variant minus_one = -1;

	void UnaryOperator::run(ExecutionController& Ec, OpcodeEvaluation& Ee, RuntimeStackUnit& RSU)
	{
		// b++ = prev
		// ++b = next

		this;
		RuntimeStackUnit left;

		// connect
		// left.ret = &vm1;

		// statics


		// is the previous item valid
		if (prev)
		{
			// lookup the code
			prev->run(Ec, Ee, left);
			
			// assign variable beforehand
			RSU.ret = left.ret;

			// what type of operator
			if (op_type == T_INCREMENT)
			{
				// performance: use direct memory manipulation upon the data types
				Variant& v = left.ret;
				if (v.IsInteger() == true)
				{
					v.getDU().v_int64++;
				}
				else if (v.IsDouble() == true)
				{
					v.getDU().v_double++;
				}
				else if (v.IsNull() == true)
				{
					v = 1;
				}
				else
				{
					UMBRA_THROW_NRE( Ec.call_stack_monitor, "[%d] Unable to perform increment on %s type", debug_line, v.getRawTypeName() );
				}
			}
			else if (op_type == T_DECREMENT)
			{
				// performance: use direct memory manipulation upon the data types
				Variant& v = left.ret;
				if (v.IsInteger() == true)
				{
					v.getDU().v_int64--;
				}
				else if (v.IsDouble() == true)
				{
					v.getDU().v_double--;
				}
				else if (v.IsNull() == true)
				{
					v = -1;
				}
				else
				{
					UMBRA_THROW_NRE( Ec.call_stack_monitor, "[%d] Unable to perform decrement on %s type", debug_line, v.getRawTypeName() );
				}
			}
			else if (op_type == T_LOGICAL_NOT)
			{
				bool not_value = !left.ret.toBool();
				RSU.ret = not_value;
				return;
			}

			// assign the new variable
			if (left.cls.IsClassInterface() == true)
			{
				RemoteClass* pRemote = left.cls.toRemoteClass();
				if (pRemote != 0)
				{
					// create property request object
					PropertyRequest pr;
					pr.id		= left.sym;
					pr.method	= Umbra::PropertyRequest::METHOD_SET;

					// run property set method
					pRemote->doUmbraProperty(left.ret, pr);
				}
			}
		}
		// is the previous item valid
		else if (next)
		{
			// what type of operator
			if (op_type == T_INCREMENT)
			{
				// call update
				next->run(Ec, Ee, left);

				// performance: use direct memory manipulation upon the data types
				Variant& v = left.ret;
				if (v.IsInteger() == true)
				{
					v.getDU().v_int64++;
				}
				else if (v.IsDouble() == true)
				{
					v.getDU().v_double++;
				}
				else
				{
					UMBRA_THROW_NRE( Ec.call_stack_monitor, "[%d] Unable to perform increment on %s type", debug_line, v.getRawTypeName() );
				}
			}
			else if (op_type == T_DECREMENT)
			{
				// call update
				next->run(Ec, Ee, left);

				// performance: use direct memory manipulation upon the data types
				Variant& v = left.ret;
				if (v.IsInteger() == true)
				{
					v.getDU().v_int64--;
				}
				else if (v.IsDouble() == true)
				{
					v.getDU().v_double--;
				}
				else
				{
					UMBRA_THROW_NRE( Ec.call_stack_monitor, "[%d] Unable to perform decrement on %s type", debug_line, v.getRawTypeName() );
				}
			}
			else if (op_type == T_LOGICAL_NOT)
			{
				// call update
				next->run(Ec, Ee, left);

				bool not_value = !left.ret.toBool();
				RSU.ret = not_value;
				return;
			}
			else if (op_type == T_NEW)
			{

				// lookup the code
				Ec.is_new_processor = true;
				next->run(Ec, Ee, left);
				Ec.is_new_processor = false;

			}
			else if (op_type == T_CAST_INT)
			{
				next->run(Ec, Ee, left);
				RSU.ret = left.ret.toInt64();
				return;
			}

			else if (op_type == T_CAST_FLOAT)
			{
				next->run(Ec, Ee, left);
				RSU.ret = left.ret.toDouble();
				return;
			}
			else if (op_type == T_CAST_BOOL)
			{
				next->run(Ec, Ee, left);
				RSU.ret = left.ret.toBool();
				return;
			}

			// assign the new variable
			RemoteClass* pRemote = left.cls.toRemoteClass();
			if (pRemote != 0)
			{
				// create property request object
				PropertyRequest pr;
				pr.id		= left.sym;
				pr.method	= Umbra::PropertyRequest::METHOD_SET;

				// run property set method
				pRemote->doUmbraProperty(left.ret, pr);

				// only assign a class reference if a resource
				RSU.cls = left.cls;
			}

			// assign
			RSU.ret = &left.ret.getVM();
		}
	}

	void BinaryOperator::run(ExecutionController& Ec, OpcodeEvaluation& Ee, RuntimeStackUnit& RSU)
	{
		// 2 elements required, one for the left and one for the right
		RuntimeStackUnit left, right;


		//
		// Basic Maths
		//

		// link to the variant memory
		VariantMemory vm1, vm2;
		left.ret = &vm1;
		right.ret = &vm2;

		// each item has a reference of one
		vm1.addRef();
		vm2.addRef();

		// dummy for else if
		switch (op_type)
		{
			// special logical operators
			case T_LOGICAL_AND:
			{
				// left side first
				BORT_REQUIRE_LEFT();
				if (left.ret.toBool() == false)
				{
					RSU.ret = false;
					return;
				}

				// right side next
				BORT_REQUIRE_RIGHT();
				RSU.ret = right.ret.toBool() ? true : false;
				return;
			}

			case T_LOGICAL_OR:
			{
				// left side first
				BORT_REQUIRE_LEFT();
				if (left.ret.toBool() == true)
				{
					RSU.ret = left.ret;
					return;
				}

				// right side next
				BORT_REQUIRE_RIGHT();
				RSU.ret = right.ret;
				return;
			}

			// member access may happen most frequetly
			case T_MEMBER_ACCESS:
			{
				bop_opcode_member(Ec, this, prev, next, left, right, Ee, RSU);
				return;
			}
			
			// basic mathematical units
			BORT_BINARY_FUNCTION(T_ADD,								binary_add) 
			BORT_BINARY_FUNCTION(T_SUBTRACT,						binary_subtract) 
			BORT_BINARY_FUNCTION(T_MULTIPLY,						binary_multiply) 
			BORT_BINARY_FUNCTION(T_DIVIDE,							binary_divide) 
			BORT_BINARY_FUNCTION(T_MODERUS,							binary_moderus) 

			// equality
			BORT_BINARY_FUNCTION(T_LOGICAL_EQUALS,					binary_equals)
			BORT_BINARY_FUNCTION(T_LOGICAL_NOT_EQUALS,				binary_not_equals)

			// inequality
			BORT_BINARY_FUNCTION(T_LESS_THAN,						binary_less_than)
			BORT_BINARY_FUNCTION(T_GREATER_THAN,					binary_greater_than)
			BORT_BINARY_FUNCTION(T_LESS_THAN_OR_EQUAL,				binary_less_than_or_eq)
			BORT_BINARY_FUNCTION(T_GREATER_THAN_OR_EQUAL,			binary_greater_than_or_eq)

			// bitwise mathematics
			BORT_BINARY_FUNCTION(T_BITWISE_AND,						binary_bitwise_and)
			BORT_BINARY_FUNCTION(T_BITWISE_OR,						binary_bitwise_or)
			BORT_BINARY_FUNCTION(T_BITWISE_XOR,						binary_bitwise_xor)
			BORT_BINARY_FUNCTION(T_BITWISE_LEFT_SHIFT,				binary_bitshift_left)
			BORT_BINARY_FUNCTION(T_BITWISE_RIGHT_SHIFT,				binary_bitshift_right)

			// basic assignment operators
			BORT_BINARY_FUNCTION_SE(T_ADD_ASSIGN,					binary_add)
			BORT_BINARY_FUNCTION_SE(T_SUBTRACT_ASSIGN,				binary_subtract)
			BORT_BINARY_FUNCTION_SE(T_MULTIPLY_ASSIGN,				binary_multiply)
			BORT_BINARY_FUNCTION_SE(T_DIVIDE_ASSIGN,				binary_divide)
			BORT_BINARY_FUNCTION_SE(T_MODERUS_ASSIGN,				binary_moderus)

			BORT_BINARY_FUNCTION_SE(T_BITWISE_AND_ASSIGN,			binary_bitwise_and)
			BORT_BINARY_FUNCTION_SE(T_BITWISE_OR_ASSIGN,			binary_bitwise_or)
			BORT_BINARY_FUNCTION_SE(T_BITWISE_XOR_ASSIGN,			binary_bitwise_xor)
			BORT_BINARY_FUNCTION_SE(T_BITWISE_LEFT_SHIFT_ASSIGN,	binary_bitshift_left)
			BORT_BINARY_FUNCTION_SE(T_BITWISE_RIGHT_SHIFT_ASSIGN,	binary_bitshift_right)

			//
			//
			// Assignment
			//

			// assignment operator
			case T_ASSIGN:
			{
				bop_opcode_assign(Ec, prev, next, left, right, Ee, RSU);
				break;
			}
			default:
			{
				__assume(0);
				break;
			}
		}

		// disconnect the items
	}

// end of namespace
} } }