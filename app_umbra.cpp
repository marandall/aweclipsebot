#include "stdafx.h"
#include "scripting_host.h"
#include "KnownList.h"
#include "CommonFeatures.h"
#include "RelayRelay.h"
#include "versions.h"
#include "worldlist.h"
#include "property_objects.h"

// debug the error message
void debugUmbraModule(void* Module, const char* Error)
{
	String log;
	log.Format("Scripting:\tError! %s", Error);
	WriteMsg(log, RGB_RED, OT_MAIN | OT_DEBUG);
}

namespace AppUmbra
{


	/*
	class D_AppLib_BuildType : public Umbra::ClassDefenition
	{
		void onUmbraRegister()
		{
			UMBRA_PROPERTY_R(BuildType, Anchient);
			UMBRA_PROPERTY_R(BuildType, Old);
			UMBRA_PROPERTY_R(BuildType, Current);
			UMBRA_PROPERTY_R(BuildType, Beta);
			UMBRA_PROPERTY_R(BuildType, BetaPlus);
			UMBRA_PROPERTY_R(BuildType, Bot);
				
			// unsupported element
			return Umbra::PropertyRequest::NOT_HANDLED; 
		}
	};

	class D_AppLib_LookupState : public Umbra::ClassDefenition
	{
		void onUmbraRegister()
		{
			UMBRA_PROPERTY_R(LookupState, None);
			UMBRA_PROPERTY_R(LookupState, Denied);
			UMBRA_PROPERTY_R(LookupState, Resolving);
			UMBRA_PROPERTY_R(LookupState, Waiting);
			UMBRA_PROPERTY_R(LookupState, Unavailable);
			UMBRA_PROPERTY_R(LookupState, Warning);
			UMBRA_PROPERTY_R(LookupState, Alert);
			UMBRA_PROPERTY_R(LookupState, Complete);
				
			// unsupported element
			return Umbra::PropertyRequest::NOT_HANDLED; 
		}
	};

	class D_AppLib_CMState : public Umbra::ClassDefenition
	{
		void onUmbraRegister()
		{
			constants["BOLD"]		= CM_BOLD;
			constants["ITALIC"]		= CM_ITALIC;
			constants["SUPRESSED"]	= CM_SUPPRESSED;
			constants["BOLD_ITALIC"]= CM_BOLD | CM_ITALIC;
		}
	}; */

	// the main instance of app
	class ThisApp : public Umbra::RemoteClass
	{	
	protected: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
		{ 
			// application constants
			UMBRA_PROPERTY_R(APP_BUILD,	 0);
			UMBRA_PROPERTY_R(APP_ID,	 "");

			// primary access to kernels
			UMBRA_PROPERTY_R(bot, bot);
			UMBRA_PROPERTY_R(avatars, avatars);
			UMBRA_PROPERTY_R(worlds_list, bot.getWorldList());

			// commmon features
			UMBRA_PROPERTY_R(common_greetings,		CommonFeatures::g_Greetings);
			UMBRA_PROPERTY_R(common_auto_responder, CommonFeatures::g_AutoResponding);

			// chat controllers
			UMBRA_PROPERTY_R(relay_kernel, ChatControllers::g_Relay);
		}
	};
}

	/*
#include "crypto_md5.h"

	// the main instance of app
	class D_Crypto : public Umbra::ClassDefenition
	{
	protected:
		string				getUmbraClassName() { return "AppLib.Crypto"; }

		// execute a function
		void executeUmbraFunction(Umbra::Variant& Returned, Umbra::string ID, Umbra::VariantMap& VMap, Umbra::ES_CallFunction& ES) 
		{
			// the main bot
			UMBRA_RMTCLS_FUNCTION_BEGIN(md5String);				
				UMBRA_RMTCLS_REQUIRE_ARG(String);

				// use md5 parser
				unsigned char md5_result[16] = { 0 };
				string data = vargString.toString().c_str();
				md5( (unsigned char *) data.c_str(), (int)data.size(), md5_result);

				// form data
				char result[40] = { 0 }; // capable of holding md5
				sprintf_s(result, 40, "%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x",
					md5_result[0], md5_result[1], md5_result[2], md5_result[3], md5_result[4], 
					md5_result[5], md5_result[6], md5_result[7], md5_result[8], md5_result[9], 
					md5_result[10], md5_result[11], md5_result[12], md5_result[13], md5_result[14], 
					md5_result[15] );

				UMBRA_RMTCLS_RETURN( result );		
			UMBRA_RMTCLS_FUNCTION_END();
			throw Umbra::UnsupportedFunctionException(ID);
		}
	};

	// store all of the globals in a single class
	class D__Globals
	{
	public:
		Umbra::NamespaceDefenition		m_ns_applib;
		Umbra::NamespaceDefenition		m_ns_eclipse;

		D_KnownList						m_knownlist;
		App								m_app;

	public: // applib
		D_AppLib_CMState				m_applib_cmstate;
		D_Crypto						m_applib_crypto;

	public:
		D__Globals()
		{
			// build the known list
			m_ns_eclipse.constants["KnownList"]	= m_knownlist;
			m_ns_eclipse.constants["Crypto"]	= m_applib_crypto;
		}
	};

	// global variable
	D__Globals d_globals;
}

UmbraInstance::UmbraInstance()
{
	//app.globals["App"]		= AppUmbra::d_globals.m_app;
	//app.globals["Aw"]		= AppUmbra::Aw::g_n_aw;
	//app.globals["Eclipse"]	= AppUmbra::d_globals.m_ns_eclipse;
}*/