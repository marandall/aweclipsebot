#pragma once
#include "umbra_variant.h"

namespace Umbra 
{

}

namespace Umbra { 
	class RuntimeStackUnit;
	class ExecutionController;
	class RuntimeStackUnit;	

namespace Instructions { namespace Opcodes
{
	class Opcode;

	//
	// managed operational code
	//
	
	class ManagedOpcode
	{
	public:
		Opcode* expr;

	public: // constructors
		ManagedOpcode(Opcode* Expr);
		ManagedOpcode(const ManagedOpcode& V);
		ManagedOpcode();

	public: // deconstructors
		~ManagedOpcode();

	public: // other operators
		ManagedOpcode& operator=(const ManagedOpcode& V);

	public: // pushing onto the list
		void pushBack(Opcode& Cs);
		void clearRight();
		void disconnect() { expr = 0; }

	public: // the management of runtime
		void run(Umbra::ExecutionController& Ec, Umbra::RuntimeStackUnit& Rsu);
	};

} } }

namespace Umbra { namespace Instructions { namespace ControlStatements
{
	//
	// Managed control item
	//
	
	class ControlStatement;

	class ManagedControlStatement
	{
	public:
		ControlStatement* expr;

	public: // constructors
		ManagedControlStatement(ControlStatement* Expr);
		ManagedControlStatement(const ManagedControlStatement& V);
		ManagedControlStatement();

	public: // deconstructors
		~ManagedControlStatement();

	public: // other operators
		ManagedControlStatement& operator=(const ManagedControlStatement& V);
		ManagedControlStatement& operator=(ControlStatement* CS);

	public: // pushing onto the list
		void pushBack(ControlStatement& Cs);

	public: // execution context
		void run(Umbra::ExecutionController& Ec);
	};

} } }

namespace Umbra { namespace Instructions 
{ 
	//
	// Bringing the managed classes into the parent reference
	//
	
	typedef Opcodes::ManagedOpcode ManagedOpcode;
	typedef ControlStatements::ManagedControlStatement ManagedControlStatement;

} }