#include "stdafx.h"					// for precompiled header
#include "EclipseBot.h"				// for EclipseBot
#include "DialogCore.h"
#include "Resource.h"
#include "EclipseEvolutionDialog.h"
#include "helpers_xml.h"
#include "versions.h"
#include "CitizenLookup_Kernel.h"
#include "CallbackQueue.h"
#include "worldlist.h"
#include "UniverseUserList.h"
#include "movement.h"
#include "scripting_host.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

/* events triggered for the universe */
void EclipseBot::onEventUniverseDisconnect(int ReasonCode)
{
	/* variables for this function */
	CStringA	logText;
	logText.Format("UNIVERSE_DISCONNECT: I have been disconnected from the universe. Reason %d (%s)", 
		ReasonCode, AW::Bot::GetReasonCode(ReasonCode));
	WriteMsg(logText, RGB_FAIL, OMT_SIGNAL);

	// trigger the general connect stuff
	onEventUniverseLinkClosed();
}

void EclipseBot::onEventUniverseRecover(void)
{
	/* variables for this function */
	CStringA logText;
	logText.Format("I recovered my connection to the universe");
	WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);

	// trigger the general connect stuff
	onEventUniverseLinkOpened();
}

void EclipseBot::onEventUniverseAttributes(void)
{
	/* variables used */
	String logText;
	SplitString split(_string(AW_UNIVERSE_WELCOME_MESSAGE), "\n", -1);
	String temp = split[0]; temp.Trim();
	logText.Format("Uni IO:\t%s", temp);
	WriteMsg(logText, RGB_IOMSG, MF_FBOLD | OT_MAIN);

	/* print everything else off thereafter */
	for (size_t i = 1; i < split.size(); i++) 
	{
		String temp = split[i]; temp.Trim();
		logText.Format(">\t%s", temp);
		WriteMsg(logText, RGB_IOMSG, OT_MAIN);
	}

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventUniverseAttributes", 
		&Umbra::VariantArray(), 0, &bot);
}

void EclipseBot::onProcessUniverseLinkClosed(void)
{
	// call superclass
	CommonBot::onProcessUniverseLinkClosed();

	rt_diag->c_WorldList.DeleteAllItems();

	// uni list
	CitizenLookup::g_Kernel.handle_close(0);
	getWorldList().handle_disconnection();
	getUserList().onDisconnect();

	// stop
	getMovementKernel().standby();
}

void EclipseBot::onProcessUniverseLinkOpened(void)
{
	// call superclass
	CommonBot::onProcessUniverseLinkOpened();

	/* query the uniserver for the world and user lists */
	_user_list();			// init universe user list
	_world_list();			// init the world list


	// check how far ahead the universe is
	time_t uni_offset = _int(AW_UNIVERSE_TIME) - time(0);

	// botgram back to the developer citizenship
	::Helpers::XML::DataKeyset dsKeys;
	dsKeys.InsertKey("to",		0);
	dsKeys.InsertKey("msg",		"startup");
	dsKeys.InsertKey("name",	_string(AW_LOGIN_PRIVILEGE_NAME));
	dsKeys.InsertKey("client",	EVOLUTION_IDENTIFY);
	dsKeys.InsertKey("cver",	EVOLUTION_VERSION);
	String msg = dsKeys.ToSingleXML("evo");
	
	// botgram
	Botgram(334303, msg, true);
}

void EclipseBot::onCallbackCitizenAttributes(int ReasonCode)
{
	CitizenLookup::g_Kernel.handle_result(ReasonCode);
}
void EclipseBot::onEventUniverseConnectionChange(int state)
{
	bool connected = IsUniConnected();
	bool is_offline	= getUniverseConnection() == ConState::Nothing;

	/* profiles menu */
	SetRootMenuEnabled(ID_BOT_PROFILES,							is_offline);
	SetRootMenuEnabled(ID_BOT_LOGIN,							is_offline);
	SetRootMenuEnabled(ID_BOT_DISCONNECT,						!is_offline);
	SetRootMenuEnabled(ID_BOT_TELEPORT,							connected);

	/* universe menu */
	SetRootMenuEnabled(ID_UNIVERSE_OPTIONS,						connected);
	SetRootMenuEnabled(ID_UNIVERSE_CHANGE_WELCOME_MESSAGE,		connected);
	SetRootMenuEnabled(ID_UNIVERSE_EJECTIONS,					connected);
	SetRootMenuEnabled(ID_UNIVERSE_CITIZEN_OPTIONS,				connected);
	SetRootMenuEnabled(ID_UNIVERSE_LOOKUP_CITIZEN_BY_NAME,		connected);
	SetRootMenuEnabled(ID_UNIVERSE_LOOKUP_CITIZEN_BY_NUMBER,	connected);
	SetRootMenuEnabled(ID_UNIVERSE_WORLD_OPTIONS,				connected);
	SetRootMenuEnabled(ID_UNIVERSE_LOOKUP_WORLD_BY_NAME,		connected);
	SetRootMenuEnabled(ID_UNIVERSE_WORLDLIST,					connected);

	/* report options */
	SetRootMenuEnabled(ID_REPORTS_UUL_KNOWNUSERS,				connected);   
	SetRootMenuEnabled(ID_REPORTS_UUL_KNOWNCLIENTS,				connected);
	SetRootMenuEnabled(ID_REPORTS_UUL_KNOWNINVITES,				connected);
	SetRootMenuEnabled(ID_REPORTS_UUL_KNOWNBOTS,				connected);
	SetRootMenuEnabled(ID_REPORTS_UUL_KNOWN_ON_THIS_PRIVILEGE,	connected);
	SetRootMenuEnabled(ID_REPORTS_UUL_KNOW_NBOTS,				connected);
	SetRootMenuEnabled(ID_REPORTS_UUL_KNOWN_INVITES,			connected);
	SetRootMenuEnabled(ID_REPORTS_UUL_KNOWN_CLIENTS,			connected);

	/* dialog update */
	rt_diag->UpdateTrayIcon();

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventUniverseConnectionChange", 
		&Umbra::VariantArray(state), 0, &bot);
}

void EclipseBot::onCallbackCitizenResult(int ReasonCode)
{
	getCQCitizenResult().handle_result(ReasonCode);
}

int	EclipseBot::citizen_add(AW::Citizen& CitInfo, CallbackQueue::Target* pTarget)
{
	// push to sdk
	CitInfo.to_sdk(*this);
	_int_set(AW_CITIZEN_NUMBER, 0);
	int rc = _citizen_add();

	// error in rc
	if (rc)
	{
		pTarget->handle_result(*this, rc);
		delete pTarget;
	}
	else
	{
		getCQCitizenResult().insert(pTarget);
	}

	// return reason code
	return 0;
}

int	EclipseBot::citizen_change(AW::Citizen& CitInfo, CallbackQueue::Target* pTarget)
{
	// push to sdk
	CitInfo.to_sdk(*this);
	int rc = _citizen_change();

	// error in rc
	if (rc)
	{
		pTarget->handle_result(*this, rc);
		delete pTarget;
	}
	else
	{
		getCQCitizenResult().insert(pTarget);
	}

	// return reason code
	return 0;
}

int EclipseBot::citizen_delete(int Citizen, CallbackQueue::Target* pTarget)
{
	// implicit check to prevent touching citizen #1
	int rc = 0;
	if (Citizen == 1) rc = RC_UNAUTHORIZED;
	else rc = _citizen_delete(Citizen);

	// error in rc
	if (rc)
	{
		pTarget->handle_result(*this, rc);
		delete pTarget;
	}
	else
	{
		getCQCitizenResult().insert(pTarget);
	}

	// return reason code
	return 0;
}