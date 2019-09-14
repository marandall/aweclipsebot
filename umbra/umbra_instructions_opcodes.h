#pragma once
#include "umbra_token.h"
#include "umbra_variant.h"
#include "umbra_variantmap.h"
#include "umbra_instructions_managed.h"


// debug code using CRT.
#ifdef _DEBUG

#endif


namespace Umbra
{
	class Variant;

	class RuntimeStackUnit
	{
	public: // dedicated to returning values
		Umbra::Variant ret;			// the returned value
		Umbra::Variant cls;			// the class described
		char*		   sym;			// the name of the symbol
	};

	class OpcodeEvaluation
	{
	public: // the Opcode information
		VariantMap		vars;
		bool			access_for_read;
				
	public: // caching information
		struct CacheEntry { std::string em; Variant res; };
		CacheEntry cache[5];

	public: // initialise for first read only
		OpcodeEvaluation()
		{
			access_for_read = false;
		}
	};

	namespace Instructions
	{
		namespace Opcodes
		{
			class Opcode;

			//
			// the operations code defenition determines in what order instructions execute
			//

			class OperatorDefenition
			{
			public: // variables
				int order;
				int direction;
				int type;

				OperatorDefenition(int Order, int Direction, int Effect = T_NONE) 
				{ 
					order	  = Order; 
					direction = Direction; 
					type	  = Effect; 
				}

				OperatorDefenition()	
				{
					order = direction = type = 0;
				}
			
			public: // directions
				static const int LEFT_TO_RIGHT	= 1;
				static const int RIGHT_TO_LEFT	= 2;

			public: // operator type
				static const int T_NONE		= 0;
				static const int T_UNARY	= 1;
				static const int T_BINARY	= 2;

			public: // operator type checkings
				bool isUnary()  { return type == T_UNARY; }
				bool isBinary() { return type == T_BINARY; }
			};

			//
			//
			//

			class OpcodeParser
			{
			public: // accessing operators
				static std::vector<OperatorDefenition>& getOperatorDefenitions();
				static OperatorDefenition* getOperatorDefenition(int OpType);

			public: // testing routines
				Opcode* PinchElements(Opcode* first);
			};

			//
			//
			//

			class Opcode
			{
			public: // clonable
				virtual Opcode* clone() = 0;
				virtual void print(string buffer) = 0;
				virtual void run(ExecutionController& Ec, OpcodeEvaluation& Ee, RuntimeStackUnit& RSU) = 0;

			public: // attach
				Opcode();
				Opcode(const Opcode& C);
				virtual ~Opcode();

			public: // destroy
				void destroy();
				Opcode* clone2() { return dynamic_cast<Opcode*>(clone()); }

			public: // additional flags
				int	flags;
				Opcode*	prev, *next;

			public: // the line number for the operation
				short	debug_line;
				char*	debug_file;

			public: // the types of operators
				static const int ET_BINARY			= 1;
				static const int ET_EFECTING_BINARY	= 2;
				static const int ET_UNARY_OPERATOR	= 3;
				static const int ET_CONSTANT		= 4;

			public: // constants for the type
				static const int OPTYPE_SYMBOL		= 0;
				static const int OPTYPE_FUNCTION	= 1;
				static const int OPTYPE_BINARY		= 2;
				static const int OPTYPE_UNARY		= 3;
				static const int OPTYPE_CONSTANT	= 4;
				static const int OPTYPE_BRACKETED	= 5;
				static const int OPTYPE_BASE		= 6;

			protected: // the operation code
				int opcode_type;
			public: // get the operation code - as before
				inline int getOpcodeType() const { return opcode_type; }
				inline int getOpType() const { return opcode_type; }
			};


			//
			//
			//
			class TextSymbol : public Opcode
			{
			public: // clonable
				Opcode* clone();
				string text;

			public: // specific constructor
				TextSymbol(string Type) : Opcode() 
				{
					opcode_type = OPTYPE_SYMBOL;
					text = Type;
				}

			public: // debugging
				virtual void print(string buffer)
				{
					printf("%sSymbolic %s\n", buffer.c_str(), text.c_str());
				}

			public: // runtime process
				virtual void run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU);
			};

			//
			//
			//
			class Function : public Opcode
			{
			public: // clonable
				Opcode* clone();
				void run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU);

			public: // the list of arguments
				std::vector<ManagedOpcode> arglist;
				string name;

			public: // constructor
				Function() : Opcode() { opcode_type = OPTYPE_FUNCTION; }

			public: // debugging
				virtual void print(string buffer)
				{
					printf("%sFunction %s\n", buffer.c_str(), name.c_str());
					buffer.append("| ");

					// for each element
					for(size_t i = 0; i < arglist.size(); i++)
						arglist[i].expr->print(buffer);
				}
			};

			//
			//
			//
			//
			//

			class Bracketed : public Opcode
			{
			public: // clonable
				Opcode* clone();

			public: // run procedure
				void run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU);

			public: // constructor
				Bracketed() : Opcode() { opcode_type = OPTYPE_BRACKETED; }

			public: // the child expressions within
				ManagedOpcode child;

			public: // debugging
				virtual void print(string buffer)
				{
					printf("%sBracketed\n", buffer.c_str());
					buffer.append("  ");

					// print the child
					child.expr->print(buffer);
				}
			};

			//
			//
			//
			//
			//

			class BinaryOperator : public Opcode
			{
			public: // clonable
				Opcode* clone();

			public: // constructor
				BinaryOperator() : Opcode() { opcode_type = OPTYPE_BINARY; }

			public: // the control elements
				int			op_type;
				string		op_debug;

			public: // specific operator
				BinaryOperator(int Type, const char* Debug = "")
				{
					op_type = Type;
					op_debug = Debug;

					// bugfix: binary type
					opcode_type = OPTYPE_BINARY;
				}

			public: // debugging
				virtual void print(string buffer)
				{
					printf("%sBinaryOperator %s\n", buffer.c_str(), op_debug.c_str());
					buffer.append("  ");
					if (prev) prev->print(buffer);
					if (next) next->print(buffer);
				}

			public: // runtime process
				virtual void run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU);
			};

			//
			//
			//
			//
			//
			
			class UnaryOperator : public Opcode
			{
			public: // clonable
				Opcode* clone();

			public: // constructor
				UnaryOperator() : Opcode() { opcode_type = OPTYPE_UNARY; }

			public: // the control elements
				int		op_type;
				string	op_debug;

			public: // specific operator
				UnaryOperator(int Type, const char* Debug)
				{
					op_type  = Type;
					op_debug = Debug;
					opcode_type = OPTYPE_UNARY;
				}

			public: // debugging
				virtual void print(string buffer)
				{
					printf("%sUnaryOperator %s\n", buffer.c_str(), op_debug.c_str());
					buffer.append("  ");
					if (prev) prev->print(buffer);
					if (next) next->print(buffer);
				}

			public: // runtime process
				virtual void run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU);
			};

			//
			//
			//
			//
			//

			class Constant : public Opcode
			{
			public: // clonable
				Opcode* clone();

			public: // constructor
				Constant() : Opcode() { opcode_type = OPTYPE_CONSTANT; }

			public: // the additional information
				::Umbra::Variant var;
				string text;

			public: // debug
				virtual void print(string buffer)
				{
					printf("%sConstant %s\n", buffer.c_str(), text.c_str());
					buffer.append("  ");
				}

			public: // runtime process
				void run(ExecutionController& Ec, OpcodeEvaluation& Ee, Umbra::RuntimeStackUnit& RSU);
			};

		}
	}
}