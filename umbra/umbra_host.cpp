#include "stdafx.h"
#include "umbra_host.h"
#include "umbra_compiler.h"
#include "umbra_tokenlist.h"
#include "umbra_tokenizer.h"
#include "umbra_instructions_managed.h"
#include "umbra_executioncontroller.h"
#include "umbra_instructions_controlstatements.h"

#include "umbra_classlib_system.h"
#include "umbra_classlib_math.h"
#include "umbra_classlib_win32.h"
#include "umbra_helpertools.h"
#include "umbra_classlib_usl.h"
#include "umbra_hostfunction.h"

// debug code using CRT.
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace Umbra
{
	//
	// Module Constructor
	//

	Module::Module()
	{
		Module(0);
	}

	Module::Module(Host* Ptr)
	{
		is_ready = false;
		host_ptr = Ptr;

		// assign a number to this module
		_autoid = HelperTools::AutoNumber::getNext();
	}

	Module::~Module()
	{
	}

	//
	// Load a file from source code
	//

	std::string Module::getSourceLine(size_t LineID)
	{
		return LineID < source_lines.size() ? source_lines[LineID] : "__NO_LINE__";
	}

	errno_t Module::load(std::string FilePath)
	{
		// set as not ready
		is_ready = false;

		// open the file
		FILE* fptr = 0;
		errno_t err = fopen_s(&fptr, FilePath.c_str(), "r");

		// test if the file was opened
		if (err != 0)
		{
			return err;
		}

		// read the lines of source
		string code_read; char szLine[1024];
		while (fgets(szLine, sizeof szLine, fptr) != 0)
		{
			code_read.append(szLine);
		}

		// close the file
		fclose(fptr);

		// run the compiler
		return compile(code_read.c_str()) ? 0 : -1;
	}

	//
	//
	//

	errno_t Module::save(std::string FilePath)	// saving to empty saves in the existing path
	{
		// open the file
		FILE* fptr = 0;
		errno_t err = fopen_s(&fptr, FilePath.c_str(), "w");
		if (err != 0)
			return err;
		
		// print whole thing
		fputs(source_code.c_str(), fptr);
		fclose(fptr);
		return 0;
	}

	// 
	// Compiles all code within a module block
	//

	bool Module::compile(const char* SourceCode)
	{
		// load a tokenizer
		Tokenizer t_builder(SourceCode);
		Compiler c;

		// copy the code module to string
		TokenArray T = t_builder.to_token_array();
		TokenList body_list;

		// not ready yet
		source_code		= SourceCode;
		is_ready			= false;
		compile_error = "";
		source_lines	= t_builder.lines; 

		// erase the list of functions
		for (FunctionMap::iterator itr = host_ptr->functions.begin(); 
					itr != host_ptr->functions.end(); )
		{
			FunctionMap::iterator i2 = functions.find(itr->first);
			if (i2 != functions.end())
			{
				itr = host_ptr->functions.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		// internal kill
		functions.clear();

		// try the compiler
		try
		{
			// copy tokens back into a list if we are not dealing with a class
			size_t i = 0;
			while (i < T.size())
			{
				// we have intercepted the start of a class in this module
				if (T[i].IsType(T_CLASS) == true)
				{
					TRACE1("Compiling class '%s'\n", T[i+1].text.c_str());
					i = compileClass(T, i);
				}
				else if (T[i].IsType(T_FUNCTION) == true)
				{
					// search for the next item
					HostObjects::Function func;
					size_t func_end = c.compileFunction(T, i, func);

					// add into functions
					HostObjects::FunctionPtr fptr = new HostObjects::Function(func);
					host_ptr->functions[func.name] = fptr;
					functions[func.name] = fptr;

					// assign to next element item
					i = func_end;
				}
				else
				{
					body_list.push_back(T[i]);
					i++;
				}
			}
			
			// copy the body back into array
			TokenArray body_array;
			body_list.convert_to_array(body_array);

			// execute body
			code = c.compileCodeblock(body_array);
			is_ready = true;
			return true;
		}
		catch (CompileError& ce)
		{
			ce.module = this;
			host_ptr->irep_compiler->onError(ce);
			compile_error = ce.about;
			return false;
		}
		catch (...)
		{
			return false;
		}
	}

	//
	// Compiles everything past the T_CLASS keyword
	//

	size_t Module::compileClass(TokenArray& T, size_t Start)
	{
		size_t i = Start;
		char debug[1024];

		Compiler c;

		// class defenition
		std::string		class_name		= "";
		TokenArray		class_namespace;

		// the symbol following the class should be a constant
		if (T[i + 1].IsConstant() == false)
		{
			sprintf_s(debug, sizeof(debug), "[%d] Invalid token following T_CLASS, expecting CONSTANT", T[i + 1].line); 
			throw CompileError(CompileError::E_UNMATCHED, debug, T[i + 1]);
		}
		else
		{
			class_name = T[i + 1].text;
			i+= 2;
		}

		// is it within some form of class or should it be placed in the main namespace
		if (T[i].IsType(T_OF) == true)
		{
			// set the i position to the symbol after T_OF
			i+= 3;

			// extract the namespace is it from
			size_t namespace_end = T.find_end_of_memlist(i);
			if (namespace_end == i)
			{
				sprintf_s(debug, sizeof(debug), "[%d] Invalid token following T_OF, expecting CONSTANT", T[i].line); 
				throw CompileError(CompileError::E_UNMATCHED, debug, T[i - 3]);
			}

			// copy the elements in the namespace
			class_namespace = T.copy_between(i, namespace_end);
			i = (namespace_end + 1);
		}

		// is it an extension
		if (T[i].IsType(T_EXTENDS) == true)
		{
			// inc iterator to the extension name
			i++;

			// extract the namespace is it from
			size_t namespace_end = T.find_end_of_memlist(i);
			if (namespace_end == i)
			{
				sprintf_s(debug, sizeof(debug), "[%d] Invalid token following T_EXTENDS, expecting CONSTANT", T[i].line); 
				throw CompileError(CompileError::E_UNMATCHED, debug, T[i]);
			}

			// follow i on to be the next byte
			i = (namespace_end + 1);
		}

		// expecting an opening t curley
		if (T[i].IsType(T_CURLEY_OPEN) == false)
		{
			sprintf_s(debug, sizeof(debug), "[%d] Invalid token, expecting T_CURLEY_OPEN", T[i].line); 
			throw CompileError(CompileError::E_UNMATCHED, debug, T[i]);
		}

		// extract the curly namespace
		size_t class_end = T.find_closing_brace(i);
		if (class_end == TokenArray::CANNOT_FIND)
		{
			sprintf_s(debug, sizeof(debug), "[%d] Unable to find matching T_CURLEY_CLOSE to T_CURLEY_OPEN", T[i].line); 
			throw CompileError(CompileError::E_UNMATCHED, debug, T[i]);
		}

		// 'i' becomes the first token of the list
		i+= 1;

		// create class objcet for allocation
		HostObjects::ClassInfoPtr ciPtr;
		
		// does the class already exist
		Variant* v = host_ptr->app_globals.search(class_name);

		// variable does not yet exist
		if (v == 0)
		{
			ciPtr = new HostObjects::ClassInfo();
			host_ptr->app_globals[class_name]	= ciPtr->getUmbraRemoteClass();
			host_ptr->classes[class_name]		= ciPtr; 
		}
		else
		{
			// an old class perhaps
			HostObjects::ClassInfo* pOldInfo = dynamic_cast<HostObjects::ClassInfo*>(v->toRemoteClass());

			// variable exists but is not a class
			if (pOldInfo == 0)
			{
				int k = 0;
				// some kind of error here
			}
			else
			{
				// old pointer info
				ciPtr = pOldInfo;
				(*ciPtr.raw_ptr()) = HostObjects::ClassInfo();
			}
		}

		// we may have multiple functions or properties
		while (i < class_end)
		{
			if (T[i].IsType(T_FUNCTION) == true)
			{
				TRACE1("Compiling '%s' of class\n", T[i + 1].text.c_str());
					
				// search for the next item
				HostObjects::Function func;
				size_t func_end = c.compileFunction(T, i, func);

				// add into class based functions
				HostObjects::FunctionPtr fptr = new HostObjects::Function(func);
				ciPtr->functions[func.name] = fptr;

				// assign to next element item
				i = func_end;
			}
			else if (T[i].IsType(T_VAR) == true)
			{
				if(T[i + 1].IsConstant() == true)
				{
					// must be followed by a T_SEMICOLON for the time being
					if (T[i + 2].IsType(T_SEMICOLON) == true)
					{
						ciPtr->vars.push_back(T[i + 1].text);
						i = i + 3;
					}
					else
					{
						sprintf_s(debug, sizeof(debug), "[%d] Expecting T_SEMICOLON following variable name", T[i + 2].line); 
						throw CompileError(CompileError::E_UNMATCHED, debug, T[i + 2]);
					}
				}
				else
				{
					sprintf_s(debug, sizeof(debug), "[%d] Expecting variable id following T_VAR", T[i + 1].line); 
					throw CompileError(CompileError::E_UNMATCHED, debug, T[i + 1]);
				}
			}
			else if (T[i].IsType(T_STATIC) == true && T[i + 1].IsType(T_FUNCTION) == true)
			{
				// search for the next item which is offset by 2
				HostObjects::Function func;
				size_t func_end = c.compileFunction(T, i + 1, func);

				// add into class based functions
				HostObjects::FunctionPtr fptr = new HostObjects::Function(func);
				ciPtr->static_functions[func.name] = fptr;

				// assign to next element item
				i = func_end;
			}
			else if (T[i].IsType(T_STATIC) == true)
			{
				// token offsets from the original static
				const size_t OFFSET_VAR		= i + 1;
				const size_t OFFSET_NAME		= i + 2;
				const size_t OFFSET_ASSIGN		= i + 3;
				const size_t OFFSET_INIT		= i + 4;
				const size_t OFFSET_SEMICOLON	= i + 5;

				// token copy and data section
				Token t = T[OFFSET_VAR];
				std::string static_name;

				// must be a constant
				if(t.IsType(T_VAR) == false)
				{
					sprintf_s(debug, sizeof(debug), "[%d] Expecting T_VAR following T_STATIC", t.line); 
					throw CompileError(CompileError::E_FATAL, debug, t);
				}

				// must be a constant
				t = T[OFFSET_NAME];
				if(t.IsConstant() == false)
				{
					sprintf_s(debug, sizeof(debug), "[%d] Expecting constant following T_VAR", t.line); 
					throw CompileError(CompileError::E_FATAL, debug, t);
				}
				else
				{
					static_name = t.text;
				}

				// followed by an equals
				t = T[OFFSET_ASSIGN];
				if(t.IsType(T_ASSIGN) == false)
				{
					sprintf_s(debug, sizeof(debug), "[%d] Expecting T_ASSIGN following constant", t.line); 
					throw CompileError(CompileError::E_FATAL, debug, t);
				}

				// following must be a constant or such
				t = T[OFFSET_INIT];
				if(t.IsNumber() == true || t.IsEncapsed() == true)
				{
					Variant vNew;
					if (t.IsEncapsed() == true)
					{
						vNew = t.text.c_str();
					}
					else if (strstr(t.text.c_str(), ".") != 0)
					{
						vNew = atof(t.text.c_str());
					}
					else
					{
						vNew = atoi(t.text.c_str());
					}

					// add to the static data
					ciPtr->static_members[static_name] = vNew;
				}
				else
				{
					sprintf_s(debug, sizeof(debug), "[%d] Expecting number or encapsed string following T_ASSIGN", T[i + 1].line); 
					throw CompileError(CompileError::E_FATAL, debug, T[i + 1]);
				}

				// must be followed by a T_SEMICOLON for the time being
				t = T[OFFSET_SEMICOLON];
				if (t.IsType(T_SEMICOLON) == false)
				{
					sprintf_s(debug, sizeof(debug), "[%d] Expecting T_SEMICOLON following value", t.line); 
					throw CompileError(CompileError::E_UNMATCHED, debug, t);
				}

				// assign i to next posiion
				i = OFFSET_SEMICOLON + 1;
			}
			else
			{
				i++;
			}
		}

		return class_end;
	}

	//
	// Main runtime procedure
	//

	Variant Module::execute(VariantMap* ArgPtr, RemoteClass* ThisContext)
	{
		ExecutionController ec;
		ec.extarg_ptr	= ArgPtr;
		ec.this_context	= ThisContext;
		ec.host_ptr		= host_ptr;

		// call stack monitor
		CallStackMonitor csm;
		ec.call_stack_monitor = &csm;

		// run execution body
		try 
		{
			CallStackMonitorFrame csmf(&csm, "__module", 0);
			code.run(ec);
		}
		catch (Umbra::Instructions::ControlStatements::ReturnException& Re)
		{
			return &Re.result.getVM();
		}
		catch (Umbra::NativeRuntimeErrorException& Nre)
		{
			if (host_ptr->irep_runtime)
			{
				host_ptr->irep_runtime->onError(Nre);
			}
		}

		// error expression
		return Umbra::Variant();
	}

	//
	//
	// Host
	//
	//

	Variant	Host::doInlineFunction(Function& Func, VariantArray* ArgList, ExecutionController Ec)
	{
		// for each item
		for (size_t i = 0; i < Func.args.size(); i++)
		{
			Ec.eval.vars[Func.args[i].func_name] 
				= (i < ArgList->size()) ? ArgList->operator [](i) : Variant();
		}

		// run the function
		try
		{
			Func.code.run(Ec);
		}
		catch (Instructions::ControlStatements::ReturnException& re)
		{
			return re.result;
		}

		// either way hopefully just a nul copy
		return Variant();
	}

	Module* Host::getModule(string name)
	{
		// find the module
		ModuleIterator itr = modules.find(name);
		if (itr == modules.end())
			return 0;

		// return this module
		return &itr->second;
	}

	Module* Host::getModuleById(int Id)
	{
		for (ModuleIterator itr = modules.begin(); itr != modules.end(); itr++)
			if (itr->second.getAutoId() == Id)
				return &itr->second;
		return 0;
	}

	Module* Host::getNewModule(string name)
	{
		// check if the module already exists
		if (getModule(name) != 0)
			return 0;

		// create a new module
		Module m(this);
		m.module_id = name;

		modules[name] = m;
		return &modules[name];
	}

	HostObjects::Function* Host::getFunction(const char* Name)
	{
		FunctionIterator itr = functions.find(Name);
		if (itr == functions.end())
			return 0;
		return itr->second.raw_ptr();
	}

	//
	// Standard Functions
	//

	class VectorArray : public Umbra::IInternalFunction
	{
	public: // main function

	};

	Variant Host::doFunction(const char* Name, VariantArray* ArgList, VariantMap* ExtArg, RemoteClass* ThisContext)
	{
		// guard to prevent nothing coming
		if (strlen(Name) == 0) return Umbra::Variant();

		// search for the function
		HostObjects::Function* pFunction = getFunction(Name);
		if (pFunction == 0)
			return Umbra::Variant();

		// generate monitor stack
		CallStackMonitor csm;
		CallStackMonitorFrame csmf(&csm, Name, 0);

		// require a new execution controller
		Umbra::ExecutionController ecChild;
		ecChild.host_ptr			= this;
		ecChild.this_context		= ThisContext;
		ecChild.extarg_ptr			= ExtArg;
		ecChild.call_stack_monitor	= &csm;

		// for each item
		for (size_t i = 0; i < pFunction->args.size(); i++)
		{
			ecChild.eval.vars[pFunction->args[i].func_name] 
				= (i < ArgList->size()) ? &ArgList->operator [](i).getVM() : Variant();
		}

		// reference to this
		Umbra::Variant vThis;
		if (ThisContext) vThis = *ThisContext;

		// run the function
		try
		{
			// implement monitor stack
			pFunction->code.run(ecChild);
		}
		catch (Instructions::ControlStatements::ReturnException& re)
		{
			return re.result;
		}
		catch(Umbra::NativeRuntimeErrorException& Ei)
		{
			// if a runtime error handler is installed
			if (irep_runtime != 0)
			{
				irep_runtime->onError(Ei);
			}
		}

		// either way hopefully just a nul copy
		return Variant();
	}

	//
	// Process Creation
	//
	
	void Host::init()
	{
		// assign reflector
		app_globals_reflector.map = &app_globals;

		app_globals["System"] = Umbra::ClassLib::SystemDef::getInst();
		app_globals["Math"]   = Umbra::ClassLib::MathDef::getInst();
		app_globals["Usl"]    = Umbra::ClassLib::Usl::D_Usl::getInst();

		// windows namespace
		app_globals["Win32"]  = ClassLib::Win32::NamespaceDef::getInst();
		app_globals["_G"]	  = app_globals_reflector;

	}

	//
	// Project Information
	//

	std::string strip_filename(const char* ProjectPath)
	{
		for (int i = (int)strlen(ProjectPath) - 1; i >= 0; i--)
			if (ProjectPath[i] == '\\')
				return std::string(ProjectPath, i + 1);
		return "";
	}

	errno_t	Host::loadProject(const char* ProjectPath)
	{
		Umbra::HelperTools::MiniINI ini;

		// find the first slash
		std::string base_path = strip_filename(ProjectPath);

		// reset everything
		reset();

		// attempt load of projects or fail
		errno_t err_ini = ini.load(ProjectPath);
		if (err_ini != 0) return err_ini;

		// how many files are present
		int module_count = atoi(ini["module_count"].c_str());
		
		// load each module
		for (int i = 0; i < module_count; i++)
		{
			// module name
			char ini_index[16];
			sprintf_s(ini_index, sizeof(ini_index), "module_%d", i);

			// lookup this module
			std::string module_name = ini[ini_index];

			// create a new element
			Module* pModule = getNewModule(module_name);
			if (pModule)
			{
				// this modules file name
				std::string module_path = base_path;
				module_path.append(module_name).append(".ussf");

				// use the resulting module path
				pModule->load(module_path.c_str());
			}
		}

		// all is good
		return 0;
	}
	
	errno_t Host::saveProject(const char* ProjectPath)
	{
		// find the first slash
		std::string base_path = strip_filename(ProjectPath);
		Umbra::HelperTools::MiniINI ini;

		// write the modules
		int module_id = 0;
		for (ModuleIterator itr = modules.begin(); itr != modules.end(); itr++)
		{
			// module name
			char ini_index[16];
			sprintf_s(ini_index, sizeof(ini_index), "module_%d", module_id++);

			// module file name
			std::string module_path = base_path;
			module_path.append(itr->first).append(".ussf");

			// write the file name
			itr->second.save(module_path.c_str());

			// add to the file
			ini.write(ini_index, itr->first.c_str());
		}

		// total items
		char item_count[16];
		sprintf_s(item_count, sizeof(item_count), "%d", (int)modules.size());
		ini.write("module_count", item_count);

		// save
		return ini.save(ProjectPath);
	}

	//
	//
	// Class Info
	//
	//

	namespace HostObjects
	{
		Umbra::RemoteClass* ClassInfo::doUmbraNewInstance(Host* HostPtr)
		{
			// create a new IUnknown type of class
			ClassInstance* pNewInstance = new ClassInstance();
			pNewInstance->class_info = getUmbraRemoteClass();

			// allocate each variable
			for (size_t i = 0; i < vars.size(); i++)
				pNewInstance->vars[vars[i]] = Variant();

			// should be collected anyway, may be set by the host
			pNewInstance->setUmbraGarbageCollected();
			return pNewInstance;
		}

		bool ClassInfo::doStaticInitialise(Umbra::Host* HostPtr, Umbra::CallStackMonitor* Csm)
		{
			// has initialised
			initialised = true;

			// create new function request object
			FunctionRequest req;
			req.id									= "__staticinit";
			req.called_pointer			= this;
			req.host								= HostPtr;
			req.call_stack_monitor	= Csm;

			// call request
			Variant v;
			try
			{
				return doUmbraFunction(v, req);
			}
			catch (Instructions::ControlStatements::ReturnException&)
			{
				return true;
			}
			catch (Umbra::NativeRuntimeErrorException& Re)
			{
				initialised = false;
				throw Re;
			}
		}

		bool ClassInfo::doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			// must have initialised
			if (initialised == false)
				doStaticInitialise(R.host, R.call_stack_monitor);

			// only need to process get requests as the rest are explicit references
			if (R.isGet() == true)
			{
				VariantMap::iterator itrFound = static_members.id_exists(R.id);
				if (itrFound != static_members.end())
				{
					V = &itrFound->second.getVM();
					R.explicit_reference = true;		// do not need to apply settings to this
					
					// terminate handling at static functions
					return R.HAS_HANDLED;
				}
			}

			// could not service this request
			return R.NOT_HANDLED;
		}

		bool ClassInfo::doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			// must have initialised
			if (initialised == false)
				doStaticInitialise(R.host, R.call_stack_monitor);

			// lookup class function
			ClassInfo::FunctionMap::iterator fiItr = static_functions.find(R.id);
			if (fiItr == static_functions.end())
			{
				// could not handle
				return R.NOT_HANDLED;
			}

			// map of arguments
			Umbra::ExecutionController ec;
			ec.host_ptr						= R.host;
			ec.this_context				= R.called_pointer ? R.called_pointer : this;
			ec.is_new_processor		= false;
			ec.call_stack_monitor	= R.call_stack_monitor; 

			// create a functional request 
			HostObjects::Function* pFunc = fiItr->second.raw_ptr();

			// for every argument required
			for (size_t i = 0; i < pFunc->args.size(); i++)
			{
				if (i < R.args.size())
				{
					ec.eval.vars[pFunc->args[i].func_name] = R.args[i];
				}
				else
				{
					ec.eval.vars[pFunc->args[i].func_name] = Variant();
				}
			}

			// call function
			try
			{
				pFunc->code.run(ec);
			}
			catch (Instructions::ControlStatements::ReturnException& re)
			{
				V = re.result;
			}

			// can associate process
			return R.HAS_HANDLED;
		}

		bool ClassInstance::doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			// reference information easily
			ClassInfo* pClassInfo = (ClassInfo*)class_info.toRemoteClass();

			// does the function exist within the set
			ClassInfo::FunctionMap::iterator fiItr = pClassInfo->functions.find(R.id);
			if (fiItr == pClassInfo->functions.end())
			{
				// could not handle
				return R.NOT_HANDLED;
			}
			else
			{
				// create a functional request 
				HostObjects::Function* pFunc = fiItr->second.raw_ptr();
				
				// map of arguments
				Umbra::ExecutionController ec;
				ec.host_ptr						= R.host;
				ec.this_context				= R.called_pointer ? R.called_pointer : this;
				ec.is_new_processor		= false;
				ec.call_stack_monitor	= R.call_stack_monitor;

				// for every argument required
				for (size_t i = 0; i < pFunc->args.size(); i++)
				{
					if (i < R.args.size())
					{
						ec.eval.vars[pFunc->args[i].func_name] = R.args[i];
					}
					else
					{
						ec.eval.vars[pFunc->args[i].func_name] = Variant();
					}
				}

				// call function
				try
				{
					pFunc->code.run(ec);
				}
				catch (Instructions::ControlStatements::ReturnException& re)
				{
					V = re.result;
				}

				// status is good
				return R.HAS_HANDLED;
			}
		}

		bool ClassInstance::doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			// search for the element if one is not found then we cannot handle this request
			Variant* pV = vars.search(R.id);
			if (pV == 0)
			{
				return R.NOT_HANDLED;
			}

			// get or set
			if (R.isGet() == true)
			{
				V = &vars[R.id].getVM();
				R.explicit_reference = true;		// do not need to apply settings to this
			}

			// handled
			return R.HAS_HANDLED;
		}


		bool MapReflector::doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			// search for the element and create a new one as required
			Variant* pV = map->search(R.id);
			if (pV == 0) (*map)[R.id] = Variant();

			// get or set
			if (R.isGet() == true)
			{
				V = (*map)[R.id];
			}
			else
			{
				(*map)[R.id] = V;
			}

			// handled
			return R.HAS_HANDLED;
		}
	}
}