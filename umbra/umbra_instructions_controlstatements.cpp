#include "stdafx.h"
#include "umbra_instructions_managed.h"
#include "umbra_instructions_controlstatements.h"
#include "umbra_instructions_opcodes.h"

// debug code using CRT.
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// start of namespace 
namespace Umbra { namespace Instructions { namespace ControlStatements {

	void ForLoop::run(Umbra::ExecutionController& Ec)
	{
		// stack unit
		RuntimeStackUnit rsuTemp, rsu;

		// run init
		init.run(Ec, rsuTemp);

		// loop while ok
		while (true)
		{
			// run the check - should this loop execute
			check.run(Ec, rsu);
			if (rsu.ret.toBool() == false)
				break;

			// run the body
			try 
			{
				body.run(Ec);
			}
			catch (RuntimeBreakException&)
			{
				break;
			}
			catch (RuntimeContinueException&)
			{
			}

			// postop
			inc.run(Ec, rsuTemp);
		}
	}

	void WhileLoop::run(Umbra::ExecutionController& Ec)
	{
		// stack unit
		RuntimeStackUnit rsu;

		// loop while status is ok
		while (true)
		{
			// run the check - should this loop execute
			check.run(Ec, rsu);
			if (rsu.ret.toBool() == false)
				break;

			// run the body
			try 
			{
				body.run(Ec);
			}
			catch (RuntimeBreakException&)
			{
				break;
			}
			catch (RuntimeContinueException&)
			{
			}
		}
	}

	void IfElse::run(Umbra::ExecutionController& Ec)
	{
		// go through each element with a check
		for (size_t i = 0; i < checks.size(); i++)
		{
			RuntimeStackUnit rsu;

			// does this expression evaluate to true
			checks[i].check.run(Ec, rsu);
			if (rsu.ret.toBool() == true)
			{
				checks[i].body.run(Ec);
				return;
			}
		}

		// run the code that executes otherwse
		otherwise.run(Ec);
	}

	void LineExpression::run(Umbra::ExecutionController& Ec)
	{
		// stack unit
		RuntimeStackUnit rsu;
		code.run(Ec, rsu);
	}

	void BreakExpression::run(Umbra::ExecutionController& Ec)
	{
		throw RuntimeBreakException();
	}

	void ContinueExpression::run(Umbra::ExecutionController& Ec)
	{
		throw RuntimeContinueException();
	}

	void ReturnExpression::run(Umbra::ExecutionController& Ec)
	{
		// use a runtime stack unit to access the variable
		RuntimeStackUnit rsu;
		expr.run(Ec, rsu);

		// assign the result to the exception
		ReturnException re;
		re.result = &rsu.ret.getVM();

		// throw it out there
		throw re;
	}

// end of namespace
} } }