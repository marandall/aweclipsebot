#pragma once
#include "umbra_variant.h"
#include "umbra_variantmap.h"
#include "umbra_instructions_managed.h"
#include "umbra_hostfunction.h"
#include "umbra_hostclass.h"
#include "umbra_callstackmonitor.h"
#include <map>

namespace Umbra
{
	// forward reference the tokens
	class TokenArray;
	class Host;
	class FunctionRequest;
	class CompileError;

	//
	//
	// MODULE - A particular file
	//
	//

	typedef HostObjects::Function Function;

	class Module
	{
		friend class Host;

	protected: // the module source code
		std::string	source_code;
		std::string file_path;
		bool				modified;
		Host*				host_ptr;
		int					_autoid;
		std::string module_id;
		std::vector<std::string> source_lines;

	protected: // compile state error
		bool		is_ready;
		string		compile_error;

	public: // a map defenition for function mapping
		typedef std::map<string, HostObjects::FunctionPtr> FunctionMap;
		typedef FunctionMap::iterator	FunctionIterator;

	public: // the class reference
		typedef std::map<string, HostObjects::ClassInstancePtr> ClassMap;
		typedef ClassMap::iterator	ClassIterator;

	public: // the host specific modules
		FunctionMap	functions;				// functions in the global namespace
		ClassMap	class_list;				// classes in the global namespace

	public: // automatic id
		int			getAutoId()		{ return _autoid;		}
		std::string	getSourceCode() { return source_code;	}
		std::string getModuleId()	{ return module_id;		}
		std::string getSourceLine(size_t LineID);

	public: // compile state error
		bool isReady() const { return is_ready; }
		string getCompileError() const { return compile_error; }

	protected: // the current control module
		Instructions::ManagedControlStatement code;

	public: // execute in this context
		Variant execute(VariantMap* ArgPtr, RemoteClass* ThisContext);

	public: // constructors
		template<class T> T* SysAllocClass(bool RefCounted = true)
		{
			T* pT = new T();
			if (RefCounted) T->setUmbraGarbageCollected();
			return pT;
		}

	public: // construction
		Module();
		Module(Host* Ptr);
		~Module();

	public: // compile code
		bool compile(const char* Code);
		size_t compileClass(TokenArray& T, size_t Start);

	public: // operations
		errno_t	load(std::string FilePath);
		errno_t save(std::string FilePath);	// saving to empty saves in the existing path
	};

	//
	// Error Handling
	//

	class IHostReport
	{
	public: // the report interface
		virtual void onError(Umbra::NativeRuntimeErrorException& Exception) = 0;
	};

	class IHostCompilerReport
	{
	public: // the report interface
		virtual void onError(Umbra::CompileError& Error) = 0;
	};

	//
	// Error Reporting - Default Stream
	//

	class IHostReport_StandardOut : public IHostReport
	{
	public: // the report interface
		void onError(Umbra::NativeRuntimeErrorException& Exception)
		{
			printf("[Error : %s]\n", Exception.error_info.c_str());
			for (int i = (int)Exception.trace.size() - 1; i >= 0; i--)
			{
				printf("  %d. %s\n", i + 1, Exception.trace[i].c_str());
			}
		}
	};

	class IHostCompilerReport_StandardOut : public IHostCompilerReport
	{
	public: // the report interface
		void onError(Umbra::CompileError& Error)
		{
		}
	};

	//
	// Tracing routine
	//

	class IHostTrace
	{
	public: // the trace interface
		virtual void onTrace(std::string TraceInfo) = 0;
	};

	//
	//
	// HOST - The core of an application. All applications derive from the host
	//
	//

	class Host
	{
	public: // a list defenition of code modules used for this execution
		typedef std::map<string, Module> ModuleMap;
		typedef ModuleMap::iterator	 ModuleIterator;

	public: // a map defenition for function mapping
		typedef std::map<string, HostObjects::FunctionPtr> FunctionMap;
		typedef FunctionMap::iterator	FunctionIterator;

	public: // a map defenition for function mapping
		typedef std::map<string, HostObjects::ClassInfoPtr> ClassMap;
		typedef ClassMap::iterator	ClassIterator;

	public: // the global objects made available
		VariantMap	std_globals;		// class system created by the system
		VariantMap	app_globals;
		HostObjects::MapReflector app_globals_reflector;

	public: // the host specific modules
		ModuleMap	modules;
		FunctionMap	functions;
		ClassMap	classes;

	public: // the reporting modules
		IHostCompilerReport*	irep_compiler;
		IHostReport*					irep_runtime;
		IHostTrace*						irep_trace;

	protected: // standard out
		IHostReport_StandardOut	_irep_stdout;
	
	public: // module access
		Module* getModule(string name);
		Module* getModuleById(int Id);
		Module* getNewModule(string name);

	public: // function
		HostObjects::Function* getFunction(const char* Name);
		// Variant doFunction(const char* Name, VariantMap* ArgList, RemoteClass* ThisContext);
		Variant doFunction(const char* Name, VariantArray* ArgList, VariantMap* ExtArg, RemoteClass* ThisContext);
		Variant	doInlineFunction(Function& Func, VariantArray* ArgList, ExecutionController Ec);

	public: // construction
		virtual void init();
		virtual void reset() 
		{ 
			app_globals.m_map.clear();
			modules.clear(); 
			functions.clear(); 
			classes.clear();

			init();
		}

	protected: // project information
		std::string project_path;
			
	public: // projects information
		errno_t	loadProject(const char* ProjectPath);
		errno_t saveProject(const char* ProjectPath = 0);

	public: // constructors
		Host()
		{
			irep_compiler = 0;
			
			// direct any errors to standard output
			irep_runtime  = &_irep_stdout;
		}

		~Host()
		{
		}
	};
}