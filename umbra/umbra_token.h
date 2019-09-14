#pragma once
#include "umbra_typedef.h"

namespace Umbra
{
	enum 
	{
		TT_UNKNOWN,
		TT_OPERATOR,
		TT_ENCAPSED_STRING,
		TT_VARIABLE,
		TT_CONSTANT,
		TT_NUMBER,
		TT_SPECIAL
	};

	class Token
	{
	public:
		int		t_type;				// global tpye
		int		r_type;				// resolved type
		string  text;				// the text representation
		int		line;

	public: // global token type checking
		bool	IsOperator()		const { return t_type == TT_OPERATOR;		 }
		bool	IsEncapsed()		const { return t_type == TT_ENCAPSED_STRING; }
		bool	IsVariable()		const { return t_type == TT_VARIABLE;		 }
		bool	IsConstant()		const { return t_type == TT_CONSTANT;		 }
		bool	IsNumber()			const { return t_type == TT_NUMBER;			 }
		bool	IsConstantVar()		const { return IsEncapsed() || IsVariable() || IsNumber(); }

	public: // helpers
		string	strip_quotations()	const;
		bool	IsType(int X)		const { return r_type == X;					 }
		string	GetTypeName()		const;
	};

	enum 
	{
		/* BEGIN */
		T_UNKNOWN			= 0,

		/* LOGICAL FUNCTIONS */
		T_LOGICAL_AND,
		T_LOGICAL_OR,
		T_LOGICAL_NOT,
		T_LOGICAL_EQUALS,
		T_LOGICAL_IDENTICAL,
		T_LOGICAL_NOT_EQUALS,
		T_LOGICAL_NOT_IDENTICAL,
		T_LESS_THAN,
		T_LESS_THAN_OR_EQUAL,
		T_GREATER_THAN,
		T_GREATER_THAN_OR_EQUAL,

		/* BOOLEAN OPERATORS */
		T_BITWISE_AND,
		T_BITWISE_OR,
		T_BITWISE_XOR,
		T_BITWISE_LEFT_SHIFT,
		T_BITWISE_RIGHT_SHIFT,

		/* BOOLEAN ASSIGNMENT OPERATORS */
		T_BITWISE_AND_ASSIGN,
		T_BITWISE_OR_ASSIGN,
		T_BITWISE_XOR_ASSIGN,
		T_BITWISE_LEFT_SHIFT_ASSIGN,
		T_BITWISE_RIGHT_SHIFT_ASSIGN,

		/* MATHS OPERATORS */
		T_ASSIGN,
		T_ADD,
		T_SUBTRACT,
		T_MULTIPLY,
		T_DIVIDE,
		T_MODERUS,
		T_INCREMENT,
		T_DECREMENT,
		
		/* MATHS ASSIGNMENT OPERATORS */
		T_ADD_ASSIGN,
		T_SUBTRACT_ASSIGN,
		T_MULTIPLY_ASSIGN,
		T_DIVIDE_ASSIGN,
		T_MODERUS_ASSIGN,
	
		/* SCOPING OPERATORS */
		T_CURLEY_OPEN,
		T_CURLEY_CLOSE,
		T_SQUARE_OPEN,
		T_SQUARE_CLOSE,
		T_BRACKETS_OPEN,
		T_BRACKETS_CLOSE,

		/* CASTING */
		T_CAST_INT,
		T_CAST_BOOL,
		T_CAST_FLOAT,

		/* LOOP SYNTAX */
		T_FOR,
		T_FOR_EACH,
		T_DO,
		T_UNTIL,
		T_LOOP,
		T_WHILE,
		T_BREAK,
		T_CONTINUE,

		/* CLASS SYNTAX */
		T_CLASS,
		T_EXTENDS,
		T_RESOLUTION,
		T_NEW,
		T_OF,

		// class modifiers
		T_VAR,
		T_STATIC,
		T_CONST,

		/* ACCESS OPERATORS */
		T_PRIVATE,
		T_PROTECTED,
		T_PUBLIC,
		T_FRIEND,

		/* ERROR HANDLING */
		T_TRY,
		T_CATCH,
		T_THROW,
		T_FINALLY,

		/* FUNCTIONS */
		T_FUNCTION,
		T_SAFE,
		T_RETURN,

		/* SYNTAX SEPERATORS */
		T_COLON,
		T_SEMICOLON,
		T_COMMA,
		T_MEMBER_ACCESS,

		/* DETERMINISM */
		T_IF,
		T_ELSE,
		T_ELSE_IF,
		T_SWITCH,
		T_CASE,

		/* FINAL SPECIAL FUNCTIONS */
		T_END,
		T_TOKEN_MAX
	};
}