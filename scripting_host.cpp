#include "stdafx.h"
#include "scripting_host.h"
#include "umbra/umbra_executioncontroller.h"
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_compiler.h"
#include "scripting_aw.h"
#include "scripting_evo2.h"


void ScriptingHost::ErrorWriter::onError(Umbra::NativeRuntimeErrorException& Exception)
{
	String log;
	
	// format the header log
	log.Format("[Error : %s]\n", Exception.error_info.c_str());
	WriteMsg(log, RGB_RED, OT_DEBUG | OT_MAIN | OM_TIME);

	// write out each line
	for (int i = (int)Exception.trace.size() - 1; i >= 0; i--)
	{
		log.Format("\t\t%d = %s", i + 1, Exception.trace[i].c_str());
		WriteMsg(log, RGB_RED, OT_MAIN);	
	}
}

void ScriptingHost::CompileWriter::onError(Umbra::CompileError& Error)
{
	// specify the calling module for usage
	String err;
	err.Format("~UMBRA:\tCompile error in module '%s' = %s", 
			Error.module ? Error.module->getModuleId().c_str() : "Unknown Module",
			Error.about.c_str());
	WriteMsg(err, RGB_RED, OT_MAIN | OT_DEBUG | MF_FBOLD);

	if (Error.module)
	{
		err.Format("\t  [%d] = %s", 
			Error.line, 
			Error.module->getSourceLine(Error.line).c_str());
		WriteMsg(err, RGB_RED, OT_MAIN | OT_DEBUG);
	}
}

void ScriptingHost::TraceWriter::onTrace(std::string TraceInfo)
{
	String s;
	s.Format("<Umbra>:\t%s", TraceInfo.c_str());
	WriteMsg(s, RGB_BLUE, OT_DEBUG);
}

ScriptingHost::ScriptingHost()
{
	std::map<String, String> templates;

	// subclass
	init();

	// error handling
	irep_compiler							= &ew_compiler;
	irep_runtime							= &ew_runtime;
	irep_trace								= &tw_trace;
}

void ScriptingHost::reset()
{
	__super::reset();

	// additional namespaces
	app_globals["Aw"]					= ScriptingHost_ClassLib::AW::Namespace::getInstance();
	app_globals["Evo2"]				= ScriptingHost_ClassLib::App::Namespace::getInstance();
	app_globals["LogStyle"]		= ScriptingHost_ClassLib::App::Namespace::getInstance()._log_style;
	app_globals["_fs"]				= fs;
	app_globals["Timer"]			= timer;

	// global variables
	app_globals["avatars"]		= avatars;
	app_globals["bot"]				= bot;

}