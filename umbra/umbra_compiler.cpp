#include "stdafx.h"
#include "umbra_instructions_managed.h"
#include "umbra_instructions_opcodes.h"
#include "umbra_instructions_controlstatements.h"
#include "umbra_tokenlist.h"
#include "umbra_compiler.h"
#include "umbra_tokenmap.h"
#include <assert.h>

// debug code using CRT.
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//
//

namespace Umbra
{
	Instructions::Opcodes::Opcode* PinchElements(Instructions::Opcodes::Opcode* first);

	Instructions::ManagedOpcode Compiler::compileStatement(TokenArray T)
	{
		// mapping infomation
		Umbra::TokenMap& tm = Umbra::TokenMap::GetMap();
		char debug[1024] = { 0 };

		// search every token - must be valid
		for (size_t i = 0; i < T.size(); i++)
		{
			Token t = T[i];
			if (t.IsOperator() == true)
			{
				if ((tm.LookupPlace(t.r_type) & TM_EXPR) != TM_EXPR)
				{
					sprintf_s(debug, sizeof(debug), "[%d] Unexpected token in expression.", t.line); 
					throw Umbra::CompileError(Umbra::CompileError::E_UNMATCHED, debug, t);
				}
			}
		}

		// uses the opcodes namespace
		using namespace Umbra::Instructions::Opcodes;
		{

			// use a constant - we will be eradicating it anyway
			ManagedOpcode first;

			// through each token
			for (size_t i = 0; i < T.size(); )
			{
				Token t = T[i];

				// switch type
				if (t.IsEncapsed() == true)
				{
					Constant pConst;
					pConst.var = t.text.c_str();
					pConst.debug_line = t.line;
					first.pushBack(pConst);
				}

				// bracketed function
				else if (t.IsType(T_BRACKETS_OPEN) == true)
				{
				// go through each element
					size_t open_count	 = 0;
					size_t open_start	 = i + 1;
					bool should_continue = false;

					// for each of the elements
					while (i < T.size())
					{
						if (T[i].IsType(T_BRACKETS_OPEN) == true)
						{
							open_count++;
						}
						else if (T[i].IsType(T_BRACKETS_CLOSE) == true)
						{
							if (--open_count == 0)
							{
								// create opcode
								Bracketed br;
								br.child = compileStatement(T.copy_between(open_start, i - 1));
					
								// attach bracketed expression
								first.pushBack( br );

								// want to proceed to end of operation
								should_continue = true;
								break;
							}
						}

						// next iteration
						i++;
					}

					// reached end without finding closing bracket
					if (should_continue == false)
					{
						char debug[1024] = { 0 };
						sprintf_s(debug, sizeof(debug), "[%d] Unable to find T_BRACKETS_CLOSE", t.line); 
						throw Umbra::CompileError(Umbra::CompileError::E_UNMATCHED, debug, t);
					}
				}

				// certain function call
				else if (t.IsConstant() == true && T[i + 1].IsType(T_BRACKETS_OPEN) == true)
				{
					Function fn;
					fn.name					= t.text;
					fn.debug_line		= t.line;

					// start the i at the start
					i++;

					// go through each element
					size_t open_count	 = 0;
					size_t open_start	 = i + 1;
					bool should_continue = false;

					while (i < T.size())
					{
						if (T[i].IsType(T_BRACKETS_OPEN) == true)
						{
							open_count++;
						}
						else if (T[i].IsType(T_BRACKETS_CLOSE) == true)
						{
							if (--open_count == 0)
							{
								if ((open_start) != i)
								{
									ManagedOpcode ep = compileStatement(T.copy_between(open_start, i - 1));
									fn.arglist.push_back(ep);
								}

								should_continue = true;
								break;
							}
						}
						else if (T[i].IsType(T_COMMA) == true && open_count == 1)
						{
							ManagedOpcode ep = compileStatement(T.copy_between(open_start, i - 1));
							fn.arglist.push_back(ep);
							open_start = i + 1;
						}

						// inc push
						i++;
					}

					// reached end without finding closing bracket
					if (should_continue == false)
					{
						char debug[1024] = { 0 };
						sprintf_s(debug, sizeof(debug), "[%d] Unable to find T_BRACKETS_CLOSE", t.line); 
						throw Umbra::CompileError(Umbra::CompileError::E_UNMATCHED, debug, t);
					}

					// add it
					first.pushBack( fn );
				}

				// constant number
				else if (t.IsNumber() == true)
				{
					Constant pConst;
					if (strstr(t.text.c_str(), "."))
					{
						pConst.var = atof(t.text.c_str());
					}
					else
					{
						pConst.var = (__int64)atoi(t.text.c_str());;
					}
					pConst.text = t.text;
					first.pushBack(pConst);
				}

				// unexpected closing
				else if (t.IsType(T_BRACKETS_CLOSE) == true)
				{
					// error in parsing
					char debug[1024] = { 0 };
					sprintf_s(debug, sizeof(debug), "[%d] T_BRACKETS_CLOSE found before T_BRACKETS_OPEN", t.line); 
					throw Umbra::CompileError(Umbra::CompileError::E_UNMATCHED, debug, t);
				}

				// operator
				else if (t.IsOperator() == true || t.IsType(T_NEW) == true)
				{
					// lookup the operator code
					OperatorDefenition* opCode = OpcodeParser::getOperatorDefenition(t.r_type);
					if (opCode == 0)
					{
						assert(false);
					}

					// is an opening brackets
					if (t.IsType(T_BRACKETS_OPEN) == true)
					{
						size_t open_count	 = 0;
						size_t open_start	 = i;
						bool should_continue = false;
						while (i < T.size())
						{
							if (T[i].IsType(T_BRACKETS_OPEN) == true)
							{
								open_count++;
							}
							else if (T[i].IsType(T_BRACKETS_CLOSE) == true)
							{
								if (--open_count == 0)
								{
									T.copy_between(open_start + 1, i - 1);
									should_continue = true;
									break;
								}
							}
						}

						// error, could not find closing item
						if (should_continue == false)
						{
							char debug[1024] = { 0 };
							sprintf_s(debug, sizeof(debug), "[%d] Unable to find T_BRACKETS_CLOSE", t.line); 
							throw Umbra::CompileError(Umbra::CompileError::E_UNMATCHED, debug, t);
						}
					}
					else
					{
						// is it binary or not
						if (opCode->isBinary() == true)
						{
							BinaryOperator bo(t.r_type, t.text.c_str());
							bo.debug_line = t.line;
							first.pushBack( bo );
						}
						else if (opCode->isUnary() == true)
						{
							UnaryOperator uo(t.r_type, t.text.c_str());
							uo.debug_line = t.line;
							first.pushBack( uo );
						}
						else
						{
							assert(false);
						}
					}
				}
				
				// general constant
				else if (t.IsConstant() == true || t.IsVariable() == true)
				{
					TextSymbol ts(t.text);
					ts.debug_line = t.line;

					first.pushBack( ts );
				}

				else if (t.IsType(T_END) == true)
				{
					break;
				}
				else
				{
					assert(false);
				}

				// increment unless otherwise bothered
				i++;
			}

			// assign temp structure
			Opcode* op4 = first.expr;
			first.expr = 0;
			first.disconnect();

			op4 = PinchElements(op4);
			ManagedOpcode moc = op4;
			delete op4;
			return moc;
		}

		return 0;

	}

	Instructions::Opcodes::Opcode* PinchElements(Instructions::Opcodes::Opcode* first)
	{
		using namespace Instructions::Opcodes;
		if (first == 0) return 0;

		// pinch each of the items into a tree structure
		Opcode* opl			  = first;		
		int			highest_index = 0;

		// rewind into the first position
		while (opl && opl->prev)
			opl = opl->prev;
		Opcode* highest_op = first;
		Opcode* last = 0;

		// the operator type tags
		BinaryOperator* bop = 0;
		UnaryOperator* uop = 0;

		// scan left to right
		while (opl != 0)
		{
			if (bop = dynamic_cast<BinaryOperator*>(opl))
			{
				const char* symbol = bop->op_debug.c_str();
				OperatorDefenition* opCode = OpcodeParser::getOperatorDefenition(bop->op_type);
				if (opCode->order > highest_index && opCode->direction == OperatorDefenition::LEFT_TO_RIGHT)
				{
					if ((bop->op_type == Umbra::T_SUBTRACT || bop->op_type == Umbra::T_ADD)
						&& (bop->prev && 
							(bop->prev->getOpcodeType() == Opcode::OPTYPE_BINARY 
							|| bop->prev->getOpcodeType() == Opcode::OPTYPE_UNARY)))
					{
					}
					else
					{
						highest_op    = bop;
						highest_index = opCode->order;
					}
				}
			}
			else if (uop = dynamic_cast<UnaryOperator*>(opl))
			{
				OperatorDefenition* opCode = OpcodeParser::getOperatorDefenition(uop->op_type);
				if (opCode->order > highest_index && opCode->direction == OperatorDefenition::LEFT_TO_RIGHT)
				{
					highest_op    = uop;
					highest_index = opCode->order;
				}
			}

			last = opl;
			opl = opl->next;
		}

		// scan right to left
		opl = last;
		while (opl != 0)
		{
			
			if (bop = dynamic_cast<BinaryOperator*>(opl))
			{
				const char* symbol = bop->op_debug.c_str();
				OperatorDefenition* opCode = OpcodeParser::getOperatorDefenition(bop->op_type);
				if (opCode->order > highest_index && opCode->direction == OperatorDefenition::RIGHT_TO_LEFT)
				{
					highest_op    = bop;
					highest_index = opCode->order;
				}
			}
			else if (uop = dynamic_cast<UnaryOperator*>(opl))
			{
				OperatorDefenition* opCode = OpcodeParser::getOperatorDefenition(uop->op_type);
				if (opCode->order > highest_index && opCode->direction == OperatorDefenition::RIGHT_TO_LEFT)
				{
					highest_op    = uop;
					highest_index = opCode->order;
				}
			}

			last = opl;
			opl = opl->prev;
		}

		// scan left to right for negatives
		while (opl != 0)
		{
			if (bop = dynamic_cast<BinaryOperator*>(opl))
			{
				const char* symbol = bop->op_debug.c_str();
				OperatorDefenition* opCode = OpcodeParser::getOperatorDefenition(bop->op_type);
				if (opCode->type == Umbra::T_SUBTRACT)
				{

				}
			}

			last = opl;
			opl = opl->next;
		}

		// pinch the items
		if (highest_op->prev)
		{
			highest_op->prev->next = 0;
			highest_op->prev = PinchElements(highest_op->prev);
		}

		// pinch the items
		if (highest_op->next)
		{
			highest_op->next->prev = 0;
			highest_op->next = PinchElements(highest_op->next);
		}

		// return this element
		return highest_op;
	}

	// extracts the header information from a function beginning at Start
	// 
	//
	size_t Compiler::extract_functionheader(StatementHeaderInfo& Info, TokenArray& T, size_t Start, const char* Type, bool IgnoreHeader)
	{
		size_t i = Start;
		char debug[1024];

		// must be followed by an opening brackets
		if (T[i].IsType(T_BRACKETS_OPEN) == false && IgnoreHeader == false)
		{
			// throw an error based upon not being able to find opening brackets
			char debug[1024];
			sprintf_s(debug, sizeof(debug), 
				"[%d] Unexpected symbol following %s; expecting T_BRACKETS_OPEN.", 
				T[i].line, Type); 
			throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, T[i]);
		}

		// loop through until we find the closing bracket
		bool should_continue = false;
		size_t open_count = 0;
		while (i < T.size())
		{
			if (T[i].IsType(T_BRACKETS_OPEN) == true)
			{
				open_count++;
			}
			else if (T[i].IsType(T_BRACKETS_CLOSE) == true || IgnoreHeader == true)
			{
				if (--open_count == 0 || IgnoreHeader == true)
				{
					// copy the argument information
					if (IgnoreHeader == false)
					{
						Info.arg = T.copy_between(Start + 1, i - 1);
						i++;
					}

					// is the next element an open braces
					// this will determine how far our scope goes
					if (T[i].IsType(T_CURLEY_OPEN) == true)
					{
						int		open_count	 = 0;
						size_t	curley_start = i;

						// Loop through the following tokens opening and closing
						// the curley brackets to find the closing location
						//
						while (i < T.size())
						{
							for (; i < T.size(); i++)
							{
								// opening scope
								if (T[i].IsType(T_CURLEY_OPEN) == true)
								{
									open_count++;
								}

								// closing scope
								else if (T[i].IsType(T_CURLEY_CLOSE) == true)
								{
									if (--open_count == 0)
									{
										// this is the end of our cope
										Info.body = T.copy_between(curley_start + 1, i - 1);
										return i + 1;
									}
								}
							}

							// error - could not find end of scope
							sprintf_s(debug, sizeof(debug), 
								"[%d] Could not find end of scope for %s", 
								T[i].line, Type); 
							throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, T[i]);
						}
					}

					// this option is for when we do not start a curly scope
					else
					{
						// search for the next semicolon
						size_t func_start = i;
						int	   open_count = 0;

						for (; i < T.size(); i++)
						{
							if (T[i].IsType(T_BRACKETS_OPEN) == true)
								open_count++;
							
							else if (T[i].IsType(T_BRACKETS_CLOSE) == true)
								open_count--;

							else if (T[i].IsType(T_SEMICOLON) == true && open_count == 0)
							{
								Info.body = T.copy_between(func_start, i);
								return i + 1;
							}
						}

						// error - could not find end of scope
						sprintf_s(debug, sizeof(debug), 
							"[%d] Could not find end of scope for %s", 
							T[i].line, Type); 
						throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, T[i]);
					}
				}
			}

			// next symbol
			i++;
		}

		// throw an error based upon not being able to find closing 
		sprintf_s(debug, sizeof(debug), 
			"[%d] Could not find closing T_BRACKETS_CLOSE", 
			T[i].line); 
		throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, T[Start]);
	}

	Instructions::ManagedControlStatement Compiler::compileCodeblock(TokenArray T)
	{
		// body variables	
		size_t	i = 0;
		char	debug[1024] = { 0 };
		Instructions::ManagedControlStatement top;

		while (i < T.size())
		{
			Token t = T[i];

			//
			// condition: for loop
			//

			if (t.IsType(T_FOR) == true)
			{
				// query the next statement, it is this used for the compilation
				StatementHeaderInfo shi;
				size_t next_itr = extract_functionheader(shi, T, i + 1, "T_FOR");
				
				// split the header information
				std::vector<TokenArray> params;
				size_t start = 0;
				for (size_t j = 0; j < shi.arg.size(); j++)
				{
					if (shi.arg[j].IsType(T_SEMICOLON) == true)
					{
						params.push_back( shi.arg.copy_between(start, j - 1) );
						start = j + 1;
					}
				}

				// copy the final elements
				params.push_back( shi.arg.copy_between(start, start + 1000) );

				// there must be 3 elements
				if (params.size() != 3)
				{
					// throw an error based upon not being able to find closing brackets
					sprintf_s(debug, sizeof(debug), 
						"[%d] Unexpected number of parameters for T_FOR - Expected 3 found %d", 
						T[i].line, (int)params.size()); 
					throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, t);
				}

				// build a for loop object
				Instructions::ControlStatements::ForLoop cs;
				cs.init		= compileStatement(params[0]);
				cs.check	= compileStatement(params[1]);
				cs.inc		= compileStatement(params[2]);

				// compile the body element
				cs.body		= compileCodeblock(shi.body);

				// append this to the top
				top.pushBack(cs);
				i = next_itr;
			}

			//
			// Condition: While Loop 
			//

			else if (t.IsType(T_WHILE) == true)
			{
				// query the next statement, it is this used for the compilation
				StatementHeaderInfo shi;
				size_t next_itr = extract_functionheader(shi, T, i + 1, "T_WHILE");

				// header is a boolean expression
				Instructions::ControlStatements::ForLoop cs;
				cs.check	= compileStatement(shi.arg);
				cs.body		= compileCodeblock(shi.body);

				// add the code
				top.pushBack(cs);
				i = next_itr;
			}

			//
			// Condition: If Statement
			//
			else if (t.IsType(T_IF) == true)
			{
				// the if object
				Instructions::ControlStatements::IfElse cs;

				// query the next statement, it is this used for the compilation
				StatementHeaderInfo shi;
				size_t next_itr = extract_functionheader(shi, T, i + 1, "T_IF");

				// create a new header block
				Instructions::ControlStatements::IfBlock ib;
				ib.check = compileStatement(shi.arg);
				ib.body	 = compileCodeblock(shi.body);
				cs.checks.push_back(ib);

				// i becomes the closing statement
				i = next_itr;

				// while we are going to use if blocks
				while (true)
				{
					if (T[i].IsType(T_ELSE_IF) == true)
					{
						next_itr = extract_functionheader(shi, T, i + 1, "T_ELSEIF");
						
						// create a new header block
						Instructions::ControlStatements::IfBlock ib;
						ib.check = compileStatement(shi.arg);
						ib.body	 = compileCodeblock(shi.body);
						cs.checks.push_back(ib);

						// i is the next pointer
						i = next_itr;
					}
					else
					{
						break;
					}
				}

				// if it an else statement
				Token tx = T[i];
				if (T[i].IsType(T_ELSE) == true)
				{
					next_itr = extract_functionheader(shi, T, i + 1, "T_ELSE", true);
					cs.otherwise = compileCodeblock(shi.body); 
					i = next_itr;
				}

				// add this statement
				top.pushBack(cs);
			}

			//
			// Condition: break statement
			//

			else if (t.IsType(T_BREAK) == true)
			{
				// must be followed by semicolon
				if (T[i + 1].IsType(T_SEMICOLON) == false)
				{
					sprintf_s(debug, sizeof(debug), "[%d] Unexpected symbol following T_BREAK, expecting T_SEMICOLON", t.line); 
					throw Umbra::CompileError(Umbra::CompileError::E_UNMATCHED, debug, T[i + 1]);
				}

				// attach to the back of the flow control
				Instructions::ControlStatements::BreakExpression cs;
				top.pushBack( cs );
				i+= 2; // the symbol following the semicolon
			}

			//
			// Condition:continue statement
			//
			
			else if (t.IsType(T_CONTINUE) == true)
			{
				// must be followed by semicolon
				if (T[i + 1].IsType(T_SEMICOLON) == false)
				{
					sprintf_s(debug, sizeof(debug), "[%d] Unexpected symbol following T_CONTINUE, expecting T_SEMICOLON", t.line); 
					throw Umbra::CompileError(Umbra::CompileError::E_UNMATCHED, debug, t);
				}

				// attach to the back of the flow control
				Instructions::ControlStatements::ContinueExpression cs;
				top.pushBack( cs );
				i+= 2; // the symbol following the semicolon
			}

			// break statement
			else if (t.IsType(T_RETURN) == true)
			{
				// find the ending expression
				size_t semicolon = T.find_next_semicolon(i);
				if (semicolon == TokenArray::CANNOT_FIND)
				{
					sprintf_s(debug, sizeof(debug), "[%d] Unable to find T_SEMICOLON following T_RETURN expression", t.line); 
					throw Umbra::CompileError(Umbra::CompileError::E_UNMATCHED, debug, t);
				}

				// copy the value up
				Instructions::ControlStatements::ReturnExpression cs;
				cs.expr = compileStatement(T.copy_between(i + 1, semicolon - 1));
				
				// attach to the back of the flow control
				top.pushBack( cs );
				i = semicolon + 1; // the symbol following the semicolon
			}

			// must be an expression of some kind
			else
			{
				// lookup the token expression
				size_t semicolon = T.find_next_semicolon(i);
				if (semicolon == TokenArray::CANNOT_FIND)
				{
					return 0;
					// throw an error because unable to find end of expression
					sprintf_s(debug, sizeof(debug), 
						"[%d] Unable to find T_SEMICOLON for end of expression", 
						T[i].line); 
					throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, t);
				}

				// extract these elements
				Instructions::ControlStatements::LineExpression cs;
				cs.code = compileStatement(T.copy_between(i, semicolon - 1));
				top.pushBack(cs);
				i = (semicolon + 1);
			}
		}

		// return the index
		return top;
	}

	int Compiler::compileFunction(TokenArray T, size_t Start, HostObjects::Function& FuncResult)
	{
		size_t i = Start;
		char debug[1024];

		if (T[i + 1].IsConstant() == false)
		{
		}

		// extract the header information
		Compiler::StatementHeaderInfo shi;
		size_t func_end = extract_functionheader(shi, T, i + 2, "T_FUNCTION", false);
		
		// could not find
		if (func_end == TokenArray::CANNOT_FIND)
		{
		}

		// function header
		HostObjects::Function func;
		func.code = compileCodeblock(shi.body);
		func.name = T[i + 1].text;

		// assign function name
		std::string function_name = T[i + 1].text;

		// run through the header
		TokenArray& ta = shi.arg;
		for (size_t j = 0; j < shi.arg.size(); )
		{
			// must be a constant varable
			if (ta[j].IsConstant() == false)
			{
				sprintf_s(debug, sizeof(debug), 
					"[%d] Unexpected symbol, expecting CONSTANT.", 
					ta[j].line); 
				throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, ta[j]);
			}

			// the function header
			Umbra::HostObjects::FunctionArg fa;
			fa.func_name = ta[j].text;

			// is it a comma, continue!
			if (ta[j + 1].IsType(T_COMMA) == true || (j + 1) == ta.size())
			{
				// add to the function arguments
				func.args.push_back(fa);

				j+= 2;
				continue;
			}

			// must be of type T_ASSIGN
			if (ta[j + 2].IsType(T_ASSIGN) == false)
			{
				sprintf_s(debug, sizeof(debug), 
					"[%d] Unexpected symbol following ARGUMENT_NAME; expecting T_ASSIGN.", 
					ta[j + 2].line); 
				throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, ta[j + 2]);
			}

			// must be a constant
			if (ta[j + 3].IsConstant() || ta[j + 3].IsNumber() || ta[j + 3].IsEncapsed())
			{
				// assign the default value as a constant
				fa.default_value = compileStatement(ta.copy_between(j + 3, j + 3));
				func.args.push_back(fa);
			}
			else
			{
				sprintf_s(debug, sizeof(debug), 
					"[%d] Unexpected symbol following T_ASSIGN; expecting CONSTANT, NUMBER or STRING.", 
					ta[j + 3].line); 
				throw Umbra::CompileError(Umbra::CompileError::E_FATAL, debug, ta[j + 3]);
			}

			// assign j to next element
			j = j + 4;

			// upon the end of the file jump out
			if ((j + 1) >= ta.size())
			{
				break;
			}

			// it must be comma or else
			if (ta[j].IsType(T_COMMA) == false && (j + 1) == ta.size())
			{
				// add to the function arguments
				func.args.push_back(fa);

				j+= 2;
				continue;
			}
		}

		FuncResult = func;
		return (int)func_end;
	}
}