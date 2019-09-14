#include "stdafx.h"
#include "umbra_instructions_managed.h"
#include "umbra_instructions_opcodes.h"
#include "umbra_instructions_controlstatements.h"
#include "umbra_executioncontroller.h"

// debug code using CRT.
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace Umbra { namespace Instructions { namespace Opcodes
{
	//
	// the managed operator information
	// 

	ManagedOpcode::ManagedOpcode(Opcode* Expr)
	{
		if (Expr) 
			expr = Expr->clone2();
		else 
			expr = 0;
	}

	ManagedOpcode::ManagedOpcode(const ManagedOpcode& V)
	{
		if (V.expr == 0) 
			expr = 0;
		else 
			expr = V.expr->clone2();
	}

	ManagedOpcode::ManagedOpcode()
	{
		expr = 0;
	}

	ManagedOpcode::~ManagedOpcode()
	{
		if (expr != 0) 
		{
			Opcode* c = expr;
			delete c;
		}

		expr = 0;
	}

	ManagedOpcode& ManagedOpcode::operator=(const ManagedOpcode& V)
	{
		// if an existing pointer exists, wipe it
		if (expr != 0) 
			delete expr;

		// reset
		expr = 0;
		
		// must copy the item if it exists
		if (V.expr)
			expr = V.expr->clone2();
		
		return *this;
	}

	void ManagedOpcode::clearRight()
	{
		Opcode* pop = expr;

		while (pop)
		{

		}
	}

	void ManagedOpcode::run(Umbra::ExecutionController& Ec, RuntimeStackUnit& Rsu)
	{
		if (expr != 0)
		{
			expr->run(Ec, Ec.eval, Rsu);
		}
	}

	void ManagedOpcode::pushBack(Opcode& Cs)
	{
		// this item
		if (expr == 0)
		{
			expr = Cs.clone2();
		}
		else
		{
			// iterate to end
			Opcode* pNext = expr;
			while (pNext->next)
				pNext = pNext->next;

			// add to the end
			pNext->next = Cs.clone2();
			pNext->next->prev = pNext;
			return;
		}
	}

} } }

namespace Umbra { namespace Instructions { namespace ControlStatements
{
	//
	// the managed operator information
	// 

	ManagedControlStatement::ManagedControlStatement(ControlStatement* Expr)
	{
		if (Expr) expr = Expr->clone();
		else expr = 0;
	}

	ManagedControlStatement::ManagedControlStatement(const ManagedControlStatement& V)
	{
		if (V.expr)
		{
			expr = V.expr->clone();
		}
		else
		{
			expr = 0;
		}
	}

	ManagedControlStatement::ManagedControlStatement()
	{
		expr = 0;
	}

	ManagedControlStatement::~ManagedControlStatement()
	{
		if (expr) 
			delete expr;
		expr = 0;
	}

	ManagedControlStatement& ManagedControlStatement::operator=(const ManagedControlStatement& V)
	{
		if (expr != 0) 
		{
			delete expr;
			expr = 0;
		}

		// do we have something to clone
		if (V.expr)
		{
			expr = V.expr->clone();
		}
		
		return *this;
	}

	ManagedControlStatement& ManagedControlStatement::operator=(ControlStatement* CS)
	{
		if (expr == 0) 
			delete expr;
		
		// copy the new item
		expr = CS->clone();
		return *this;
	}

	void ManagedControlStatement::pushBack(ControlStatement& Cs)
	{
		if (expr == 0)
		{
			expr = Cs.clone();
		}
		else
		{
			// add an item
			expr->pushBack(&Cs);
		}
	}

	void ManagedControlStatement::run(ExecutionController& Ec)
	{
		// initial opcode
		ControlStatement* cs = expr;

		// each item
		while (cs)
		{
			cs->run(Ec);
			cs = cs->next.expr;
		}
	}

} } }