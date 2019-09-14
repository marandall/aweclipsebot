#pragma once

// stl inclusions
#include <list>
#include <vector>

// umbra inclusions
#include "umbra_token.h"
#include "umbra_tokenmap.h"
#include "umbra_tokenlist.h"

namespace Umbra
{
	class Tokenizer
	{
	public: // variable tracking
		TokenList	list;
		std::vector<std::string> lines;

	public: // constructors
		Tokenizer() { }
		Tokenizer(const char* Code) { ParseString(Code); }

	public: // handling tokens
		bool InsertToken		(std::string Token, int Type, int Line);
		int  ParseString		(const char* InputString);

	protected: // handling of the different token types
		void handle_single_maths(char*& p, int Line);
		void handle_negative	(char*& p, int Line);
		void handle_operator	(char*& p, int Line);
		void handle_number		(char*& p, int Line);
		void handle_variable	(char*& p, int Line);
		void handle_function	(char*& p, int Line);
		void handle_whitespace(char*& p, int Line);
		void handle_encapsed	(char*& p, int Line);
		void handle_cast			(char*& p, int Length, int Line);

	public: // support
		TokenArray to_token_array();
	};
}