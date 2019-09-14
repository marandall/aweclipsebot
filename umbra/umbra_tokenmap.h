#pragma once
#include <map>
#include "umbra_token.h"
#include "umbra_typedef.h"

namespace Umbra
{
	const int TM_EXPR	= 1;
	const int TM_STRUCT	= 2;

	class TokenMap
	{
	protected:
		typedef std::map<string, int> MapType;
		MapType	m_map;

	public: // the mapping info
		std::map<int, int> places;

	public:
		TokenMap()
		{
			/* LOGICAL FUNCTIONS */
			m_map["&&"		]	= T_LOGICAL_AND;
			m_map["||"		]	= T_LOGICAL_OR;
			m_map["!"		]	= T_LOGICAL_NOT;
			m_map["=="		]	= T_LOGICAL_EQUALS;
			m_map["==="		]	= T_LOGICAL_IDENTICAL;
			m_map["!="		]	= T_LOGICAL_NOT_EQUALS;
			m_map["!=="		]	= T_LOGICAL_NOT_IDENTICAL;
			m_map["<"		]	= T_LESS_THAN;
			m_map["<="		]	= T_LESS_THAN_OR_EQUAL;
			m_map[">"		]	= T_GREATER_THAN;
			m_map[">="		]	= T_GREATER_THAN_OR_EQUAL;

			/* BOOLEAN OPERATORS */
			m_map["&"		]	= T_BITWISE_AND;
			m_map["|"		]	= T_BITWISE_OR;
			m_map["^"		]	= T_BITWISE_XOR;
			m_map["<<"		]	= T_BITWISE_LEFT_SHIFT;
			m_map[">>"		]	= T_BITWISE_RIGHT_SHIFT;

			/* BOOLEAN ASSIGNMENT OPERATORS */
			m_map["&="		]	= T_BITWISE_AND_ASSIGN;
			m_map["|="		]	= T_BITWISE_OR_ASSIGN;
			m_map["^="		]	= T_BITWISE_XOR_ASSIGN;
			m_map["<<="		]	= T_BITWISE_LEFT_SHIFT_ASSIGN;
			m_map[">>="		]	= T_BITWISE_RIGHT_SHIFT_ASSIGN;

			/* MATHS OPERATORS */
			m_map["="		]	= T_ASSIGN;
			m_map["+"		]	= T_ADD;
			m_map["-"		]	= T_SUBTRACT;
			m_map["*"		]	= T_MULTIPLY;
			m_map["/"		]	= T_DIVIDE;
			m_map["%"		]	= T_MODERUS;
			m_map["--"		]	= T_DECREMENT;
			m_map["++"		]	= T_INCREMENT;
			
			/* MATHS ASSIGNMENT OPERATORS */
			m_map["+="		]	= T_ADD_ASSIGN;
			m_map["-="		]	= T_SUBTRACT_ASSIGN;
			m_map["*="		]	= T_MULTIPLY_ASSIGN;
			m_map["/="		]	= T_DIVIDE_ASSIGN;
			m_map["%="		]	= T_MODERUS_ASSIGN;
		
			/* SCOPING OPERATORS */
			m_map["{"		]	= T_CURLEY_OPEN;
			m_map["}"		]	= T_CURLEY_CLOSE;
			m_map["["		]	= T_SQUARE_OPEN;
			m_map["]"		]	= T_SQUARE_CLOSE;
			m_map["("		]	= T_BRACKETS_OPEN;
			m_map[")"		]	= T_BRACKETS_CLOSE;

			/* CASTING */
			m_map["(int)"		]	= T_CAST_INT;
			m_map["(bool)"	]	= T_CAST_BOOL;
			m_map["(float)"	]	= T_CAST_FLOAT;

			/* LOOP SYNTAX */
			m_map["for"		]	= T_FOR;
			m_map["foreach"	]	= T_FOR_EACH;
			m_map["do"		]	= T_DO;
			m_map["until"	]	= T_UNTIL;
			m_map["loop"	]	= T_LOOP;
			m_map["while"	]	= T_WHILE;
			m_map["break"	]	= T_BREAK;
			m_map["continue"]	= T_CONTINUE;

			/* CLASS SYNTAX */
			m_map["class"	]	= T_CLASS;
			m_map["extends"	]	= T_EXTENDS;
			m_map["::"		]	= T_RESOLUTION;
			m_map["of"		]	= T_OF;
			m_map["new"		]	= T_NEW;
			m_map["var"		]	= T_VAR;
			m_map["static"	]	= T_STATIC;
			m_map["const"	]	= T_CONST;

			/* ACCESS OPERATORS */
			m_map["private"	]	= T_PRIVATE;
			m_map["protected"]	= T_PROTECTED;
			m_map["public"	]	= T_PUBLIC;
			m_map["friend"	]	= T_FRIEND;

			/* ERROR HANDLING */
			m_map["try"		]	= T_TRY;
			m_map["catch"	]	= T_CATCH;
			m_map["throw"	]	= T_THROW;
			m_map["finally"	]	= T_FINALLY;

			/* FUNCTIONS */
			m_map["function"]	= T_FUNCTION;
			m_map["safe"	]	= T_SAFE;
			m_map["return"	]	= T_RETURN;

			/* SYNTAX SEPERATORS */
			m_map[":"		]	= T_COLON;
			m_map[";"		]	= T_SEMICOLON;
			m_map[","		]	= T_COMMA;
			m_map["."		]	= T_MEMBER_ACCESS;

			/* DETERMINISM */
			m_map["if"			]	= T_IF;
			m_map["else"		]	= T_ELSE;
			m_map["elseif"	]	= T_ELSE_IF;
			m_map["switch"	]	= T_SWITCH;
			m_map["case"		]	= T_CASE;


			//
			//
			//


			/* LOGICAL FUNCTIONS */
			places[T_LOGICAL_AND					] = TM_EXPR;
			places[T_LOGICAL_OR						] = TM_EXPR;
			places[T_LOGICAL_NOT					] = TM_EXPR;
			places[T_LOGICAL_EQUALS				] = TM_EXPR;
			places[T_LOGICAL_IDENTICAL		] = TM_EXPR;
			places[T_LOGICAL_NOT_EQUALS		] = TM_EXPR;
			places[T_LOGICAL_NOT_IDENTICAL] = TM_EXPR;
			places[T_LESS_THAN						] = TM_EXPR;
			places[T_LESS_THAN_OR_EQUAL		] = TM_EXPR;
			places[T_GREATER_THAN					] = TM_EXPR;
			places[T_GREATER_THAN_OR_EQUAL] = TM_EXPR;

			/* BOOLEAN OPERATORS */
			places[T_BITWISE_AND					] = TM_EXPR;
			places[T_BITWISE_OR						] = TM_EXPR;
			places[T_BITWISE_XOR					] = TM_EXPR;
			places[T_BITWISE_LEFT_SHIFT		] = TM_EXPR;
			places[T_BITWISE_RIGHT_SHIFT	] = TM_EXPR;

			/* BOOLEAN ASSIGNMENT OPERATORS */
			places[T_BITWISE_AND_ASSIGN		] = TM_EXPR;
			places[T_BITWISE_OR_ASSIGN		] = TM_EXPR;
			places[T_BITWISE_XOR_ASSIGN		] = TM_EXPR;
			places[T_BITWISE_LEFT_SHIFT_ASSIGN	] = TM_EXPR;
			places[T_BITWISE_RIGHT_SHIFT_ASSIGN	] = TM_EXPR;

			/* MATHS OPERATORS */
			places[T_ASSIGN						] = TM_EXPR;
			places[T_ADD							] = TM_EXPR;
			places[T_SUBTRACT					] = TM_EXPR;
			places[T_MULTIPLY					] = TM_EXPR;
			places[T_DIVIDE						] = TM_EXPR;
			places[T_MODERUS					] = TM_EXPR;
			places[T_DECREMENT				] = TM_EXPR;
			places[T_INCREMENT				] = TM_EXPR;
			
			/* CASTING */
			places[T_CAST_INT					] = TM_EXPR;
			places[T_CAST_BOOL				] = TM_EXPR;
			places[T_CAST_FLOAT				] = TM_EXPR;

			/* MATHS ASSIGNMENT OPERATORS */
			places[T_ADD_ASSIGN				] = TM_EXPR;
			places[T_SUBTRACT_ASSIGN	] = TM_EXPR;
			places[T_MULTIPLY_ASSIGN	] = TM_EXPR;
			places[T_DIVIDE_ASSIGN		] = TM_EXPR;
			places[T_MODERUS_ASSIGN		] = TM_EXPR;
		
			/* SCOPING OPERATORS */
			places[T_CURLEY_OPEN			] = TM_STRUCT;
			places[T_CURLEY_CLOSE			] = TM_STRUCT;
			places[T_SQUARE_OPEN			] = TM_EXPR;
			places[T_SQUARE_CLOSE			] = TM_EXPR;
			places[T_BRACKETS_OPEN		] = TM_EXPR;
			places[T_BRACKETS_CLOSE		] = TM_EXPR;

			/* LOOP SYNTAX */
			places[T_FOR							] = TM_STRUCT;
			places[T_FOR_EACH					] = TM_STRUCT;
			places[T_DO								] = TM_STRUCT;
			places[T_UNTIL						] = TM_STRUCT;
			places[T_LOOP							] = TM_STRUCT;
			places[T_WHILE						] = TM_STRUCT;
			places[T_BREAK						] = TM_STRUCT;
			places[T_CONTINUE					] = TM_STRUCT;

			/* CLASS SYNTAX */
			places[T_CLASS						] = TM_STRUCT;
			places[T_EXTENDS					] = TM_STRUCT;
			places[T_RESOLUTION				] = TM_STRUCT;
			places[T_OF								] = TM_STRUCT;
			places[T_NEW							] = TM_STRUCT;

			/* ACCESS OPERATORS */
			places[T_PRIVATE					] = TM_STRUCT;
			places[T_PROTECTED				] = TM_STRUCT;
			places[T_PUBLIC						] = TM_STRUCT;
			places[T_FRIEND						] = TM_STRUCT;

			/* ERROR HANDLING */
			places[T_TRY							] = TM_STRUCT;
			places[T_CATCH						] = TM_STRUCT;
			places[T_THROW						] = TM_STRUCT;
			places[T_FINALLY					] = TM_STRUCT;

			/* FUNCTIONS */
			places[T_FUNCTION					] = TM_STRUCT;
			places[T_SAFE							] = TM_STRUCT;
			places[T_RETURN						] = TM_STRUCT;

			/* SYNTAX SEPERATORS */
			places[T_COLON						] = TM_STRUCT;
			places[T_SEMICOLON				] = TM_STRUCT;
			places[T_COMMA						] = TM_EXPR;
			places[T_MEMBER_ACCESS		] = TM_EXPR;

			/* DETERMINISM */
			places[T_IF						] = TM_STRUCT;
			places[T_ELSE					] = TM_STRUCT;
			places[T_ELSE_IF				] = TM_STRUCT;
			places[T_SWITCH					] = TM_STRUCT;
			places[T_CASE					] = TM_STRUCT;
		}

		string Int2String(int Value)
		{
			for (MapType::iterator itr = m_map.begin(); itr != m_map.end(); itr++)
			{
				if (itr->second == Value)
					return itr->first;
			}
			return "";
		}

		int Lookup(const char* LookupText)
		{
			// lookup the result
			MapType::iterator itr = m_map.find(LookupText);
			
			// no element could be found
			if (itr == m_map.end())
				return T_UNKNOWN;

			// return the result
			return itr->second;
		}

		int LookupPlace(int Token)
		{
			return places[Token];
		}

		static TokenMap& GetMap();
	};
}