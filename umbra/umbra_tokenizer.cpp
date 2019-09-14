#include "stdafx.h"
#include "umbra_tokenizer.h"
#include "umbra_tokenlist.h"
#include <ctype.h>

// debug code using CRT.
#ifdef _DEBUG

#endif

namespace Umbra
{
	TokenMap g_tokenmap;
	TokenMap& TokenMap::GetMap()
	{
		return g_tokenmap;
	}

	string Token::strip_quotations() const
	{
		string s;
		for (size_t i = 1; i < text.size() - 1; i++)
			s.push_back(text[i]);
		return s;
	}

	// map list
	string Token::GetTypeName() const 
	{
		return TokenMap::GetMap().Int2String(r_type);
	}


	// IsX functions
	// ----------------------------
	// Checks if a byte is of a certain type
	bool IsNumber(char C)		
	{ 
		return C >= '0' && C <= '9'; 
	}

	bool IsHexNumber(char C)		
	{ 
		return  (C >= '0' && C <= '9') || 
				(C >= 'A' && C <= 'F') ||
				(C >= 'a' && C <= 'f'); 
	}
	
	bool IsSymbolName(char C)	
	{	
		return isalnum((int)C) || C == '$' || C == '_' ;
	}
	
	bool IsFunctionName(char C) 
	{
		C = (int)(BYTE)C;
		return (C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z') || C == '_' || isalnum((int)C); 
	}
	
	bool IsSingleMaths(char C) 
	{ 
		return C == '(' || C == ')' || C == '?' || C == '[' || C == ']' ||C == ';' 
			|| C == '{' || C == '}' || C == ',' || C == '^' || C == '.'; 
	}
	
	bool IsWhitespace(char C) 
	{ 
		return C == ' '  ||  C == '\t' || C == '\n' || C == '\r'; 
	}
	
	bool IsOperator(char C) 
	{ 
		return C == '+' || C == '-' || C == '/' || C == '*' || C == '>' || C == '<' || 
				C == '=' || C == ':' || C == '|' || C == '&' || C == '!' || C == '%'; 
	}


	bool Tokenizer::InsertToken(std::string Token, int Type, int Line)
	{
		// set up the initial part of the token
		Umbra::Token token;
		token.t_type = Type;
		token.text	 = Token;
		token.line	 = Line;

		// bugfix: encased is not tokenable
		if (Type != TT_ENCAPSED_STRING)
			token.r_type = TokenMap::GetMap().Lookup(Token.c_str());

		// add it to the list of tokens
		list.push_back(token);

		return false;
	}

	TokenArray Tokenizer::to_token_array()
	{
		TokenArray arr;
		list.convert_to_array(arr);
		return arr;
	}

	void Tokenizer::handle_single_maths(char*& p, int Line)
	{
		// handle token
		InsertToken( string(p, 1) , TT_OPERATOR, Line);
		p++;
	}

	void Tokenizer::handle_negative(char*& p, int Line)
	{
		char* p_start = p;

		// only use this system if we are grouping
		if (IsNumber(*(p + 1)))
		{
			p++;
			while (IsNumber(*p))
				p++;
		
			// need to handle negative numbers here
			InsertToken( std::string(p_start, (int)(p - p_start)), TT_NUMBER, Line);
		}
		else
		{
			char* p_start = p;
			while (IsOperator(*p)) 
				p++;
			// extract the operator functions
			InsertToken(std::string(p_start, (int)(p - p_start)),
				TT_OPERATOR, Line);
		}
	}
	
	void Tokenizer::handle_operator(char*& p, int Line)
	{
		char* p_start = p;
		while (IsOperator(*p)) 
			p++;

		// handle token
		string op = std::string(p_start, (int)(p - p_start));
		InsertToken(op, TT_OPERATOR, Line);
	}

	void Tokenizer::handle_cast(char*& p, int Length, int Line)
	{
		// generate token
		InsertToken(std::string(p, Length), TT_OPERATOR, Line);
		p+= (size_t)Length;
	}

	void Tokenizer::handle_number(char*& p, int Line)
	{
		// is the next p an x
		if (*p == '0' && *(p + 1) == 'x')
		{
			// jump forward
			char* pFirst = p;
			p+= 2;
			
			// same loop as normal
			char* p_start = p;
			while (IsHexNumber(*p) == true)
				p++;

			// format string
			std::string dec(pFirst, (int)(p - pFirst));

			// parse as decimal
			char* pEnd = 0;
			int len = strtol(dec.c_str(), &pEnd, 16);

			// format again
			char result[20];
			sprintf_s(result, "%d", len);

			// handle token
			InsertToken(result, TT_NUMBER, Line);
		}
		else
		{
			// during this extraction accept the . as a decimal seperator
			char* p_start = p;
			while (IsNumber(*p) || *p == '.')
				p++;
			// handle token
			InsertToken(std::string(p_start, (int)(p - p_start)),
				TT_NUMBER, Line);
		}
	}

	void Tokenizer::handle_variable(char*& p, int Line)
	{
		// our variable name includes the $
		char* p_start = p;
		while (IsSymbolName(*p)) 
			p++;
		// handle token
		InsertToken(std::string(p_start, (int)(p - p_start)),
			TT_VARIABLE, Line);
	}

	void Tokenizer::handle_function(char*& p, int Line)
	{	
		// loop while we have a valid name, one containing only alphanumberics
		char* p_start = p;
		while (IsFunctionName(*p)) 
			p++;
		// handle token
		InsertToken(std::string(p_start, (int)(p - p_start)),
			TT_CONSTANT, Line);
	}

	void Tokenizer::handle_whitespace(char*& p, int Line)
	{
		// progress until we are no longer at a whitespace
		while (IsWhitespace(*p)) 
			p++;
	}

	void Tokenizer::handle_encapsed(char*& p, int Line)
	{
		// we include the first quote but need to skip over it for our counter
		char* p_start = p;
		p++;

		// we need a slash escape
		bool slashed = false;
		std::string line;
		line.reserve(128);

		// while we do not hit another double quote
		while (*p != '\0')
		{

			// if the element is slashed
			if (slashed)
			{
				if (*p == '\'') line.append("\\");
				else if (*p == '0') line.append("\0");
				else if (*p == 'n') line.append("\n");
				else if (*p == 'r') line.append("\r");
				else if (*p == 't') line.append("\t");
				else if (*p == 'v') line.append("\v");
				else if (*p == '"') line.append("\"");
				else if (*p == '\'') line.append("\'");
				else line.push_back(*p);
				slashed = false;
			}
			else
			{
				// is it a slashed
				if (*p == '\\')
				{
					// invert slashing
					slashed = !slashed;
				}
				else if (*p == '"')
				{
					break;
				}
				else
				{
					line.push_back(*p);
				}
			}

			p++;
		}

		// handle token
		InsertToken(line, TT_ENCAPSED_STRING, Line);
		p++;
	}

	int Tokenizer::ParseString(const char* InputString)
	{
		char* p = const_cast<char*>(InputString);
		
		// token list
		char* start = p;
		lines.reserve(5000);

		int line = 0;

		// loop through whilever we have not reached the final string
		while (*p != 0)
		{
			// is line escape
			if (*p == '#' || (*p == '/' && *(p + 1) == '/' )) 
			{
				while (*p != '\n' && *p != '\0')
					p++;
			}

			// is it a newline
			if (*p == '\n')
			{
				line++;
				lines.push_back(std::string(start, (int)(p - start)));
				start = p + 1;
				p++;
			}

			// the /* */ style of code
			if (*p == '/' && *(p + 1) == '*')
			{
				while (*p != '\0' && (*p == '*' && *(p + 1) == '/'))
				{
					if (*p == '\n')
					{
						line++;
						lines.push_back(std::string(start, (int)(p - start)));
						start = p + 1;
					}
					p++;
				}
			}

			// single quote
			if (strncmp(p, "(int)", 5) == 0)
				handle_cast(p, 5, line);

			else if (strncmp(p, "(bool)", 6) == 0)
				handle_cast(p, 6, line);

			else if (strncmp(p, "(float)", 7) == 0)
				handle_cast(p, 7, line);

			// is a single piece maths component, such as an opening bracket
			else if (IsSingleMaths(*p) == true)		
				handle_single_maths(p, line);

			// is the symbol a negative sign followed by a number
			//else if (*p == '-')					
			//	handle_negative(p, line);

			// is a symbol that represents an operator
			else if (IsOperator(*p) == true)	
				handle_operator(p, line);

			// is the symbol a number 
			else if (IsNumber(*p) == true)
				handle_number(p, line);

			// is this byte the start of a variable string
			else if (*p == '$')					
				handle_variable(p, line);

			// is this a byte of a constant
			else if (IsFunctionName(*p) == true)		
				handle_function(p, line);

			// skip over any whitespaces as a breaker and throw to next key
			else if (IsWhitespace(*p) == true)
			{
				// progress until we are no longer at a whitespace
				while (IsWhitespace(*p) == true) 
				{
					if (*p == '\n')
					{
						line++;
						int dif = (int)(p - start + 1);
						lines.push_back(std::string(start, dif));
						start = p + 1;
					}
					p++;
				}
			}

			// if the token is an opening quote read through all of the quote
			else if (*p == '"') 				
				handle_encapsed(p, line);

			else if (*p == '\0')
				break;

			// in other events, urgh... continue
			else
				p++;
		}

		// insert T_END
		Token t;
		t.text = "";
		t.r_type = T_END;
		list.push_back(t);
		
		return 0;
	}


	Token TokenArray::operator[](size_t T)
	{
		if (T >= m_list.size())
		{
			Token t;
			t.r_type = Umbra::T_END;
			return t;
		}
		else
		{
			return m_list[T];
		}
	}

	TokenArray&	TokenArray::append_end()
	{
		return *this;
	}

	TokenArray TokenArray::copy_between(size_t Start, size_t End)
	{
		TokenArray ta;
		for (size_t i = Start; i <= End && i < m_list.size(); i++)
		{
			ta.push_back(m_list[i]);
		}
		return ta;
	}

	size_t TokenArray::find_semicolon(size_t Start)
	{
		// search for the next semicolon from the position
		for (size_t i = Start; i < m_list.size(); i++)
		{
			if ((*this)[i].IsType(T_SEMICOLON) == true)
			{
				return i;
			}
		}

		// cannot find this position
		return (size_t)-1;
	}

	size_t TokenArray::find_closing(size_t start, int Open, int Close)
	{
		return 0;
	}

	size_t TokenArray::find_next_semicolon(size_t Start)
	{
		for (size_t i = Start; i < m_list.size(); i++)
		{
			if (m_list[i].IsType(T_SEMICOLON) == true)
			{
				return i;
			}
		}
		return CANNOT_FIND;
	}

	//
	// Finds the end of a combination of constants and dots
	//

	size_t TokenArray::find_end_of_memlist(size_t start)
	{
		// a counter and reference for ease
		TokenArray& t = *this;
		size_t i = start;

		// start must be constant
		if (t[i].IsConstant() == false)
			return start;

		// move iterator onto next dot
		i++;

		// must start with a constant
		while (t[i].IsType(T_MEMBER_ACCESS) == true && t[i + 1].IsConstant())
			i+= 2; // progress on to next element

		// return this item
		return i;
	}

	size_t TokenArray::find_end_of_expression(size_t Start)
	{
		return 0;
	}

	size_t TokenArray::find_closing_brace(size_t i)
	{
		size_t open = 0;
		TokenArray& t = *this;
		
		// counting log
		while (i < size())
		{
			if (t[i].IsType(T_CURLEY_OPEN) == true)
			{
				open++;
			}
			else if (t[i].IsType(T_CURLEY_CLOSE) == true)
			{
				if (--open == 0)
				{
					return i + 1;
				}
			}

			i++;
		}

		return TokenArray::CANNOT_FIND;
	}
}