#pragma once
#include "cis_exec.h"

class Avatar;
class Bot;

namespace CIS 
{
	class Request;
	class Response;

	// the command interpretter
	class CommandInterpreter
	{
	public: // the get function
		static Exec::IExec* getClass(const char* Id);
		static int beginAvatar(Avatar& Source, String Text, bool RunAdmin, int ChatType);
		static int beginConsole(String Text, bool Supress = false);
	};
}