#pragma once

// stl includes
#include <list>
#include <vector>

// umbra includes
#include "umbra_token.h"

namespace Umbra
{
	const int SEMICOLON_MISSING = 0xFFFFF1;
	const int CANNOT_FIND		= 0xFFFFF1;

	class TokenArray
	{
	public: // extension types
		typedef			std::vector<Token>			ListType;
		typedef			ListType::iterator			iterator;
		typedef			ListType::reverse_iterator	reverse_iterator;
		typedef			ListType::reference			reference;

	public: // the search routines
		static const size_t	CANNOT_FIND	= (size_t)-1;
		typedef			std::vector<TokenArray>	TokenArrayArray;

	public: // forward functions
		ListType		m_list;
		iterator		begin()					{ return m_list.begin();	}
		iterator		end()					{ return m_list.end();		}
		reverse_iterator rbegin()			{ return m_list.rbegin();	}
		reverse_iterator rend()				{ return m_list.rend();		}
		size_t			size() const			{ return m_list.size();		}

	public: // editing
		void			push_back(Token T)		{ m_list.push_back(T);		}
		void			clear()					{ m_list.clear();			}
		Token			operator[](size_t T);
		TokenArray&		append_end();

	public: // support
		void			print(const char* About = "");
		TokenArray		copy_between		(size_t start, size_t end);
		size_t			find_next_semicolon	(size_t start);
		size_t			find_closing_brace	(size_t start);
		size_t			find_closing_bracket(size_t start);
		size_t			find_Opcode_end	(size_t start, size_t& begin);
		size_t			extract_Opcode	(size_t start, TokenArray& Array);
		size_t			find_end_of_memlist	(size_t start);

	public: // find the expression
		size_t			find_semicolon(size_t);
		size_t			find_closing(size_t start, int Open, int Close);
		size_t			find_end_of_expression(size_t start);
	};

	class TokenList
	{
	public: // extension types
		typedef		std::list<Token>			ListType;
		typedef		ListType::iterator			iterator;
		typedef		ListType::reverse_iterator	reverse_iterator;
		typedef		ListType::reference			reference;
	
	public: // forward functions
		ListType	m_list;
		iterator	begin()					{ return m_list.begin();	}
		iterator	end()					{ return m_list.end();		}
		reverse_iterator rbegin()			{ return m_list.rbegin();	}
		reverse_iterator rend()				{ return m_list.rend();		}
		size_t		size() const			{ return m_list.size();		}
	
	public: // editing
		void		push_back(Token T)		{ m_list.push_back(T);		}
		void		clear()					{ m_list.clear();			}

	public: // searching for the next token; returns the actual iterator
		iterator find_next_token(iterator Begin, int OperatorType)
		{
			iterator itr = Begin;
			while (itr != end())
			{
				if (itr->IsType(OperatorType))
					return itr;
				itr++;
			}
			return m_list.end();
		}

		// copy up to the string
		void copy_upto_into_array(iterator Begin, iterator End, TokenArray& Array)
		{
			for (iterator i = Begin; i != End; i++)
				Array.push_back(*i);
		}

		// copy between
		void copy_between_into_array(iterator Begin, iterator End, TokenList& Array)
		{
			for (iterator i = Begin; i != End; i++)
				Array.push_back(*i);
			Array.m_list.pop_front();
		}

		// full copy
		void convert_to_array(TokenArray& TA)
		{
			for (iterator i = begin(); i != end(); i++)
				TA.push_back(*i);
		}

		iterator find_closing_tag(iterator Begin, int OpenType, int CloseType)
		{
			// operator counting
			iterator itr = Begin;
			int count = 0;

			while (itr != end())
			{
				// handle the possible open and close types
				if (itr->IsType(OpenType))
					count++;
				else if (itr->IsType(CloseType))
					count--;

				// find the close
				if (count == 0)
					return itr;
				itr++;
			}

			// error
			return m_list.end();
		}
	};
}