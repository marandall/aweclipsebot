#pragma once
#include "umbra_instructions_managed.h"
#include "umbra_typedef.h"
#include "umbra_tokenlist.h"
#include "umbra_hostfunction.h"

namespace Umbra
{
	class TokenArray;
	class Module;

	namespace HostObjects
	{
		class Function;
	}

	//
	//
	//

	class CompileError
	{
	public: // the error
		std::string			about;
		Umbra::Module*	module;
		int							err_no;
		int							line;

	public: // the error codes
		enum 
		{
			E_SUCCESS = 0,
			E_UNMATCHED,
			E_FATAL
		};

	public: // compile error
		CompileError(int E, const char* R, const Umbra::Token& RemToken)
		{
			about		= R;
			err_no	= E;
			module	= 0;
			line		= RemToken.line;
		}
	};

	//
	//
	//

	class Compiler
	{
	public: // compiler information
		Umbra::Instructions::ManagedOpcode compileStatement(TokenArray T);
		Umbra::Instructions::ManagedControlStatement compileCodeblock(TokenArray T);
		int compileFunction(TokenArray T, size_t Start, HostObjects::Function& FuncResult);

	public: // statement header
		class StatementHeaderInfo
		{
		public: // the array of tokens
			TokenArray	arg;
			TokenArray	body;
		};

	public: // extraction
		size_t extract_functionheader(StatementHeaderInfo& Info, TokenArray& T, size_t Start, const char* Type, bool IgnoreHeader = false);
	};
}