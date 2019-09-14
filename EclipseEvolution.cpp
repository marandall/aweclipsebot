// EclipseEvolution.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "EclipseEvolutionDialog.h"
#include "DynamicRights.h"
#include "Beacons.h"
#include "Profiles.h"
#include "DialogCore.h"
#include "ConsoleFilter.h"
#include "SystemWordsFilter.h"
#include "FloodPreventionPolicy.h"
#include "cis_dispatch.h"
#include "serializer.h"
#include "zones.h"
#include "CommonFeatures.h"
#include "CIS_Configuration.h"
#include "RelayRelay.h"
#include "Zones_ConfigGeneral.h"
#include "InterfaceConfig.h"
#include "scripting_host.h"
#include "RelayGlobalChat.h"
#include "movers.h"
#include "cis_CommandBindings.h"
#include "stringtoolkit.h"
#include "CisScriptCommands.h"
#include "headsup.h"
#include "interfacelayout.h"
#include "scripting_host.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// EclipseEvolutionApp
BEGIN_MESSAGE_MAP(EclipseEvolutionApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_COMMAND(ID_AVATAR_RELAY_LEAVE, &EclipseEvolutionApp::OnAvatar_RelayLeave)
END_MESSAGE_MAP()

const char* StringToolkit::AlphaChars	= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVQXYZ";
const char* StringToolkit::AlphaNumChars	= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVQXYZ1234567890";
const char* StringToolkit::NumChars		= "1234567890";
const char* StringToolkit::WhiteChars	= " \n\r\t";

//
// 
// Configure loader mapping each association
//
//

class ConfigLoader
{
private: // storage loader
	std::vector<IConfigLoader*> _list;

public: // linker
	void add(IConfigLoader* Cfg) 
	{ 
		_list.push_back(Cfg); 
	}
	
	void load(Serializer& Ini)
	{
		for (size_t i = 0; i < _list.size(); i++)
			_list[i]->Load(Ini);
	}

	void save(Serializer& Ini)
	{
		for (size_t i = 0; i < _list.size(); i++)
			_list[i]->Save(Ini);
	}

public: // static instance loader
	static ConfigLoader& getInst() 
	{ 
		static ConfigLoader cl; 
		return cl; 
	}
};


// EclipseEvolutionApp construction

EclipseEvolutionApp::EclipseEvolutionApp()
{
	ConfigLoader& cf = ConfigLoader::getInst();

	setlocale(LC_CTYPE, "en_GB.UTF-8");
	SetThreadLocale(65001);


	// standard loaders
	cf.add(&CommonFeatures::g_Greetings);
	cf.add(&CommonFeatures::g_AutoResponding);
	cf.add(&g_InterfaceConfig);
	cf.add(&InterfaceLayout::getInst());

	// cis interfaces
	cf.add(&CIS::Config::g_Ejection);
	cf.add(&CIS::Config::g_Interface);
	cf.add(&CIS::CommandBindings::getInstance());
	cf.add(&CIS::ScriptCommands::getInstance());
	cf.add(&ChatControllers::g_Relay);
	cf.add(&Zones::g_Config);

}

EclipseEvolutionApp theApp;

BOOL EclipseEvolutionApp::InitInstance()
{
	//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxInitRichEdit2();
	AfxEnableControlContainer();


	/* Gdi+ Initialisation */
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	SetRegistryKey(_T("Mark Randall presents Eclipse Evolution"));
	bot.p_avatars	= &avatars;
	bot.p_movers	= &Movers::g_Movers; 

	EclipseEvolutionDialog dlg;
	
	m_pMainWnd = &dlg;
	rt_diag	   = &dlg;		// root pointer

	// setup accelerator
	m_haccel=LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ROOT_ACC));


	INT_PTR nResponse = dlg.DoModal();

	/* Gdi+ Shutdown */
	Gdiplus::GdiplusShutdown(gdiplusToken);

	// shut down various managers
	ScriptingHost::getInst().reset();
	Umbra::ResourceManager::GetRM().running = false;
	Umbra::ResourceManager::GetRM().empty_resources();

	// clear all of the items
	ScriptingHost::getInst().reset();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

using namespace Security;


void OpenProfile(String ProfileId)
{
	/* profile path is determind by the profile id*/
	String ProfilePath;
	ProfilePath.Format("%s\\data\\profiles\\%s", GetAppPath(), ProfileId);

	using namespace ChatControllers;

	// create directories
	String path2;
	path2.Format("%s\\data\\profiles\\%s", GetAppPath(), ProfileId);
	CreateDirectory(path2 + "\\userdata", NULL);
	CreateDirectory(path2 + "\\logs", NULL);
	CreateDirectory(path2 + "\\temp", NULL);

	// initialise from the main configuration
	Serializer ini;
	ini.Load(ProfilePath + "\\config.ini");

	// jump point config
	Beacons::Kernel::getInst().load(ProfilePath + "\\beacons.xms");
	Beacons::Kernel::getInst().config.Load(ini);

	DynamicRights::g_Provider.Load		(ProfilePath + "\\dynamic_rights.ini");
	Comms::g_ChatFilter.Load					(ProfilePath + "\\chatfilter.txt");
	Comms::g_ConsoleFilter.Load				(ProfilePath + "\\consolefilter.txt");
	g_FilterPolicy.words.Load					(ProfilePath + "\\filter_words.txt");
	g_Relay.getChannels().load				(ProfilePath + "\\channels.ini");
	g_Relay.getGlobalChat().getUserStyles().load(ProfilePath + "\\relay_styles.ini");

	// load each of the policy values
	ConfigLoader::getInst().load(ini);
	HeadsUp::Config::getInstance().Load(ini, "headsup_tray");

	/* load zone configuration */
	BOOL zone_loaded = Zones::g_ZonesMgr.GetRoot()->load(ProfilePath + "\\zones.ini");

	/* load the icon for this profile */
	rt_diag->m_hIcon = AfxGetApp()->LoadIconA(Profiles::g_Icons.GetResource(Profiles::g_Profiles.getCurrent()->getIconId()));
	rt_diag->SetIcon(rt_diag->m_hIcon, TRUE);
	rt_diag->SetIcon(rt_diag->m_hIcon, FALSE);

	// update GUI
	rt_diag->UpdateTrayIcon();
	rt_diag->SetWindowTextA(CString(_T("Eclipse Evolution (")) + ProfileId + _T(")"));

	// save the project
	ScriptingHost::getInst().loadProject(ProfilePath + "\\default.ussp");
	InterfaceLayout::getInst().apply(*rt_diag);

	// scripting host
	ScriptingHost::getInst().doFunction("ProfileLoaded", &Umbra::VariantArray(), 0, &bot);
}

void CloseProfile(String ProfileId)
{
	/* profile path is determind by the profile id*/
	String ProfilePath;
	ProfilePath.Format("%s\\data\\profiles\\%s", GetAppPath(), ProfileId);

	using namespace ChatControllers;

	// trigger the timer
	ScriptingHost::getInst().timer.clear();
	Serializer ini;

	// jump point config
	Beacons::Kernel::getInst().save(ProfilePath + "\\beacons.xms");
	Beacons::Kernel::getInst().config.Save(ini);

	DynamicRights::g_Provider.Save		(ProfilePath + "\\dynamic_rights.ini");
	Comms::g_ChatFilter.Save			(ProfilePath + "\\chatfilter.txt");
	Comms::g_ConsoleFilter.Save			(ProfilePath + "\\consolefilter.txt");
	g_FilterPolicy.words.Save			(ProfilePath + "\\filter_words.txt");
	g_Relay.getChannels().save			(ProfilePath + "\\channels.ini");
	g_Relay.getGlobalChat().getUserStyles().save(ProfilePath + "\\relay_styles.ini");

	/* save ini functions */
	ConfigLoader::getInst().save(ini);
	HeadsUp::Config::getInstance().Save(ini, "headsup_tray");
	
	/* save relay mode */
	ini.Save(ProfilePath + "\\config.ini");

	/* load zone configuration */
	BOOL zone_loaded = Zones::g_ZonesMgr.GetRoot()->save(ProfilePath + + "\\zones.ini");

	// save the project
	ScriptingHost::getInst().saveProject(ProfilePath + "\\default.ussp");
}

void EclipseEvolutionApp::OnAvatar_RelayLeave()
{
	// TODO: Add your command handler code here
}

BOOL EclipseEvolutionApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
    if(m_haccel)
    {
        if (::TranslateAccelerator(m_pMainWnd->m_hWnd, m_haccel, lpMsg)) 
            return(TRUE);
    }

	return CWinApp::ProcessMessageFilter(code, lpMsg);
}


void WriteTracerLog(const char* Output)
{
	FILE* fptr = fopen("tracer.txt", "a");
	if (fptr)
	{
		fprintf(fptr, "%s\n", Output);
		fclose(fptr);
	}
}

time_t time64(time_t* t)
{
	_tzset();
	time_t ot = time(t) - _timezone;
	return ot;
}