#pragma once
#include "umbra_instructions_managed.h"
#include "umbra_executioncontroller.h"

#define new DEBUG_NEW

namespace Umbra
{
	namespace Instructions
	{

		//
		// Type defenitions for the managed Opcode
		//

		namespace ControlStatements
		{
			//
			// Base - forms the basis function of each function
			//

			class ControlStatement
			{
			public: // next item
				ManagedControlStatement next;
				virtual ~ControlStatement() 
				{
					if (next.expr)
					{
						delete next.expr;
						next.expr = 0;
					}
				}

			public: // cloning procedure and runtime
				virtual ControlStatement* clone() = 0;
				virtual void run(Umbra::ExecutionController& Ec) = 0;

			public: // following procedure to add events
				void pushBack(ControlStatement* Cs) 
				{ 
					if (next.expr == 0) 
						next = Cs; 
					else 
						next.expr->pushBack(Cs); 
				}
			};

			//
			// For Loop - executes while a condition is true
			//

			class ForLoop : public ControlStatement
			{
			public: // the requirements
				ManagedOpcode			init;
				ManagedOpcode			check;
				ManagedOpcode			inc;
				ManagedControlStatement	body;

			public: // cloning procedure and runtime
				ControlStatement* clone() { return new ForLoop(*this); }
				void run(Umbra::ExecutionController& Ec); 
			};

			//
			// While Loop - executes while a condition is true
			//

			class WhileLoop : public ControlStatement
			{
			public: // the requirements
				ManagedOpcode			check;
				ManagedControlStatement	body;

			public: // cloning procedure and runtime
				ControlStatement* clone() { return new WhileLoop(*this); }
				void run(Umbra::ExecutionController& Ec); 
			};

			//
			// An if block
			//

			class IfBlock
			{
			public:
				ManagedOpcode			check;
				ManagedControlStatement	body;
			};

			//
			// If Statement
			//

			class IfElse : public ControlStatement
			{
			public: // blocks of commands
				std::vector<IfBlock>	checks;
				ManagedControlStatement	otherwise;
			
			public: // cloning procedure and runtime
				ControlStatement* clone() { return new IfElse(*this); }
				void run(Umbra::ExecutionController& Ec); 
			};

			//
			// Expression
			//

			class LineExpression : public ControlStatement
			{
			public: // the code to be executed
				ManagedOpcode	code;

			public: // cloning procedure and runtime
				ControlStatement* clone() { return new LineExpression(*this); }
				void run(Umbra::ExecutionController& Ec); 
			};

			//
			// Break
			//

			class BreakExpression : public ControlStatement
			{
			public: // cloning procedure and runtime
				ControlStatement* clone() { return new BreakExpression(*this); }
				void run(Umbra::ExecutionController& Ec); 
			};

			//
			// Continue
			//

			class ContinueExpression : public ControlStatement
			{
			public: // cloning procedure and runtime
				ControlStatement* clone() { return new ContinueExpression(*this); }
				void run(Umbra::ExecutionController& Ec); 
			};

			//
			// Return
			//

			class ReturnExpression : public ControlStatement
			{
			public: // cloning procedure and runtime
				ControlStatement* clone() { return new ReturnExpression(*this); }
				void run(Umbra::ExecutionController& Ec); 

			public: // the expression
				ManagedOpcode expr;
			};

			//
			// Break Exception
			//

			class RuntimeBreakException
			{
			};

			//
			// Continue Exception
			//

			class RuntimeContinueException
			{
			};

			//
			// Return Exception
			//

			class ReturnException
			{
			public:
				Umbra::Variant result;
			};
		}
	}
}