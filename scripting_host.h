#pragma once
#include "umbra/umbra_host.h"
#include "umbra/umbra_remoteclass.h"
#include "scripting_freestore.h"
#include "scripting_timer.h"

//
// Eclipse Scripting Host
//   Controls scripting events within the application
//

class ScriptingHost : public Umbra::Host
{
public: // error handling class
	class ErrorWriter : public Umbra::IHostReport
	{
	public: // error reporting
		void onError(Umbra::NativeRuntimeErrorException& Exception);
	};

	class TraceWriter : public Umbra::IHostTrace
	{
	public: // override
		virtual void onTrace(std::string TraceInfo);
	};
	
	class CompileWriter : public Umbra::IHostCompilerReport
	{
	public: // error reporting
		void onError(Umbra::CompileError& Error);
	};

public: // constructor
	ScriptingHost();
	void reset();

public: // generalised requirements
	ScriptingHost_ClassLib::FreeStore fs;
	ScriptingHost_ClassLib::D_Timer		timer;

public: // run a certain function
	ErrorWriter ew_runtime;
	CompileWriter ew_compiler;
	TraceWriter tw_trace;

public: // the vector information
	struct FuncTemplate{ String func, code; };
	std::vector<FuncTemplate> function_templates;

public: // static context
	static ScriptingHost& getInst() { static ScriptingHost host; return host; } 
};
