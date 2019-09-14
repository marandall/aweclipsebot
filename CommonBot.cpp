#include "stdafx.h"
#include "CommonBot.h"
#include "movers.h"
#include "movement.h"
#include "InterfaceConfig.h"
#include "CitizenLookup_Kernel.h"
#include "scripting_host.h"

CStringA m_OutputBreakers;

void CommonBot::WriteMsg(char const * Message, long Colour, int Style)
{
	::WriteMsg(Message, Colour, Style);
}

//
//
// Identification
//
//

String CommonBot::getName(void) const
{
	return _string(AW_LOGIN_NAME);
}

String CommonBot::getBotName(void) const
{
	return _string(AW_LOGIN_NAME);
}

String CommonBot::getPrivilegeName(void) const
{
	return _string(AW_LOGIN_PRIVILEGE_NAME);
}

Avatar& CommonBot::getAvatar(size_t Index)
{
	return getAvatars()[Index];
}

//
//
// Login Handling
//
//
int	CommonBot::quickstart(const char* Host, int Port, const char* Universe, int Citizen, const char* Privilege, const char* Name, 
							const char* World, const char* Coordinates, bool Global, bool Invisible)
{
	// Initialise bot
	Initialise();
	CStringA logText;

	//
	// Assign
	//
	m_al_citizen		= Citizen;
	m_al_name			= Name;
	m_al_world			= World;
	m_al_coordinates	= Coordinates;
	m_al_global			= Global;
	m_al_invisible		= Invisible;

	//
	// Create Connection
	//
	
	// initial message
	logText.Format("Attempting to establish link to %s on %s:%d", Universe, Host, Port);
	WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);

	// create connection
	int rc = AW::Bot::Create(Host, Port, Universe, FALSE);

	// operation has failed
	if ( rc != RC_SUCCESS) 
	{
		logText.Format("Unable to establish link to %s. Reason %d (%s)",
			Universe, rc, AW::Bot::GetReasonCode(rc));
		WriteMsg(logText, RGB_FAIL, OMT_SIGNAL);
	}

	// operation has succeeded
	else 
	{
		logText.Format("Established link to universe %s", Host);
		WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);
	}

	//
	// Setup Events and Callbacks
	//

	// standard callback
	CallbackSet(AW_CALLBACK_LOGIN,						true);
	CallbackSet(AW_CALLBACK_ENTER,						true);
	CallbackSet(AW_CALLBACK_ADDRESS,					true);
	CallbackSet(AW_CALLBACK_CITIZEN_ATTRIBUTES,	true);
	CallbackSet(AW_CALLBACK_CITIZEN_RESULT,		true);
	CallbackSet(AW_CALLBACK_OBJECT_RESULT,		true);
	CallbackSet(AW_CALLBACK_CELL_RESULT,			true);
	CallbackSet(AW_CALLBACK_HUD_RESULT,				true);
	CallbackSet(AW_CALLBACK_WORLD_LIST,				true);
	CallbackSet(AW_CALLBACK_QUERY,						true);
	CallbackSet(AW_CALLBACK_BOTGRAM_RESULT,		true);
	CallbackSet(AW_CALLBACK_WORLD_EJECTION,		true);
	
	// standard events
	EventSet	(AW_EVENT_WORLD_INFO,						true);
	EventSet	(AW_EVENT_OBJECT_CLICK,					true);
	EventSet	(AW_EVENT_URL_CLICK,						true);
	EventSet	(AW_EVENT_TELEPORT,							true);
	EventSet	(AW_EVENT_HUD_CLICK,						true);

	// enable groups
	setAvatarEvents(true);
	setWorldEvents(true);
	setUniverseEvents(true);
	setCommunicationEvents(true);
	setUserListEvents(true);
	setEntityEvents(true);
	SetRiderEvents(true);
	setObjectEvents(true);
	setCellEvents(true);

	// 
	// Attempt Login
	//

	// initial message
	logText.Format("Attempting to login in as [%s] using citizen %d", Name, Citizen);
	WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);

	// underlying operation
	rc = AW::Bot::Login(Citizen, Privilege, Name, "Eclipse Evolution", TRUE);
	if (rc != RC_SUCCESS) 
	{
		logText.Format("Login failure [oncall]. Reason %d (%s)", rc, AW::Bot::GetReasonCode( rc ));
		WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);
	}

	// is running ok
	return rc;
}

void CommonBot::onCallbackCreate(int Reason)
{ 
	// trigger processor
	onProcessCallbackCreate(Reason); 
}

void CommonBot::onCallbackLogin(int Reason) 
{ 
	// variable copy
	String logText, reasonText;
	
	//
	// Check Login Status
	//

	if (Reason) 
	{
		logText.Format("Unable to login to universe '%s' on %s:%d. Reason %d (%s)",
			getAL_UniName(), getAL_UniHost(), getAL_UniPort(), Reason, AW::Bot::GetReasonCode(Reason));
		WriteMsg(logText, RGB_FAIL, OMT_SIGNAL);

		// universe connection
		Destroy();
		return;
	}

	//
	// Status Messages
	//
	logText.Format("I have succesfully logged into %s", getAL_UniName());
	WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);

	//the relevant user name
	logText.Format("@Login:\tPrivilege Name: %s", _string(AW_LOGIN_PRIVILEGE_NAME));
	WriteMsg(logText, RGB_NOTIFY, MF_FBOLD | OT_MAIN);

	// universe connection
	setUniverseConnection( ConState::Connected );

	//
	// trigger processor
	//
	onProcessCallbackLogin(Reason);

	// universe build number
	int uni_build = _int(AW_UNIVERSE_BUILD_NUMBER);
	logText.Format("Universe:\t%s on %s:%d. Build %d (%s)", 
		getAL_UniName(), getAL_UniHost(), getAL_UniPort(), uni_build, AW::Bot::getUniverseVerID(uni_build));
	WriteMsg(logText, RGB_IOMSG, MF_FBOLD | OT_MAIN); 

	if ( getAL_Citizen() == 1 ) 
	{
		logText.Format("Universe:\tRoot access available");
		WriteMsg(logText, RGB_IOMSG, MF_FBOLD | OT_MAIN);
	}

	//
	// Check Login Requirements
	//

	if (getAL_World().GetLength() == 0)
	{
		logText.Format("Login complete, holding at universe level in %s", getAL_UniName());
		WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);
	}
	else
	{
		// attempt to enter world
		if (getWorldConnection() == ConState::Nothing)
		{

		}
	}

	/* due to this being the main class module; we now log in */
	if (getAL_World().GetLength() == 0) 
	{
		logText.Format("Login complete, holding at universe level in %s", getAL_UniName());
		WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);
	}
	else
	{
		if (getWorldConnection() == ConState::Nothing)
		{
			enterWorld(getAL_World(), getAL_Global());
		}
	}
}

int	CommonBot::enterWorld(const char* World, bool Global)
{
	// initial message
	String logText;
	logText.Format("Attempting to enter %s%s", getAL_World(), getAL_Global() ? " (as global)" : "");
	WriteMsg(logText, RGB_NOTIFY, OMT_SIGNAL);

	// underlying implementation
	int rc = AW::Bot::EnterWorld(getAL_World(), getAL_Global(), TRUE);
	if (rc != RC_SUCCESS)
	{
		String logText;
		logText.Format("Failed to enter %s. aw_enter(..) = %d (%s)", getAL_World(), rc, AW::Bot::GetReasonCode(rc));
		WriteMsg(logText, RGB_FAIL, OMT_SIGNAL);
		setWorldConnection(ConState::Nothing);
	}

	// there was an error
	return rc;
}

void CommonBot::onCallbackEnter(int ReasonCode)
{
	/* check if the function did not succeed */
	String logText;
	if ( ReasonCode != RC_SUCCESS)
	{
		logText.Format("Failed to enter %s [cbk]. Reason %d (%s)", getAL_World(), ReasonCode, AW::Bot::GetReasonCode(ReasonCode));
		WriteMsg(logText, RGB_FAIL, OMT_SIGNAL);
		ExitWorld();
		return;
	} else {
		setWorldConnection(ConState::Connected);
	}

	/* write success message */
	logText.Format("Successfully entered %s", getWorld());
	WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);

	/* do we stay invisible or do something */
	if (getAL_Invisible() == TRUE) {
		logText.Format("Login complete, holding invisible in %s", getAL_World());
		WriteMsg(logText, RGB_SUCCESS, OMT_SIGNAL);
		return;
	}

	/* continue on to do our movement */
	AW::Coordinates coords;
	coords.FromString( getAL_Coordinates() );
	Move(coords);
}

int	CommonBot::teleport(const char* World, const char* Coordinates, bool Global, bool Invisible)
{
	// must be universe connected
	if (getUniverseConnection() == ConState::Nothing) 
		return RC_NO_CONNECTION;
	
	// debug
	CStringA log;
	log.Format("Teleporting to %s in %s", Coordinates, World);
	WriteMsg(log, 0, OT_MAIN | OM_TIME);
	
	// world change states
	bool WorldDif	= (getWorld().CompareNoCase(World) != 0) || getWorldConnection() == ConState::Nothing;
	bool GlobalDif	= (Global != (_bool(AW_ENTER_GLOBAL) == 1));
	bool InvisDif	= (bB(Invisible) != getInvisible()) && getInvisible() == FALSE;

	// disable movement
	getMovementKernel().standby();

	/* are we in a different world, or do we need to change state */
	if (WorldDif || GlobalDif || InvisDif)
	{
		// BUGFIX: Repeated Cleanup processed when teleporting after disconnection
		if (getWorldConnection() != ConState::Nothing)
			ExitWorld();

		// set coordinates
		m_al_world			= World;
		m_al_coordinates	= Coordinates;
		m_al_global			= Global;
		m_al_invisible		= Invisible;
		return enterWorld(World, Global);
	}
	else
	{
		/* simply teleport around */
		::Location loc(Coordinates);
		return Move(loc);
	}
}

int	CommonBot::destroy(void)
{
	// if we are connected to a world
	if (getWorldConnection() != ConState::Nothing)
		ExitWorld();

	// initial message
	CStringA logText;
	logText.Format("%s Attempts to destroy session %d and logs out of %s", getBotName(), _session(), getAL_UniName());
	WriteMsg(logText, RGB(33, 99, 33), OT_MAIN | OM_TIME);

	// destroy it
	return Bot::Destroy();
}

//
//
// Avatar Events
//
//

class PrivilegeLookupClass : public CitizenLookup::LookupBase
{
public:
	void Result(AW::Citizen& Info, int Reason)
	{
		if (Reason == 0)
		{
			bot.onEventPrivilegeResolved(Info);
		}
	}
};

void CommonBot::onEventAvatarAdd(void)
{
	// create a new avatar
	Avatar* newUser = getAvatars().create(0, "");
	newUser->plc_bot = this;

	// load the avatars data in
	newUser->getStartup(*this);
	newUser->getChanges(*this);
	newUser->CompileData();
	newUser->updateLastMovementTime();

	// load the users profile information
	newUser->profileLoad();

	// database
	newUser->db_WriteEnter();

	// lookup ip?
	if (HasEject() || HasCT())
	{
		_address(newUser->getSession());
		newUser->BeginLookupIp();
	}

	// check privileges
	if (newUser->getPrivilege() == 0)
	{
		newUser->OnPrivilegeResolved("~tourist");
	}
	else if (newUser->getCitizen() == newUser->getPrivilege() )
	{
		newUser->OnPrivilegeResolved(newUser->getName());
	}
	else
	{
		for (size_t i = 0; i < getAvatars().size(); i++)
		{
			if (getAvatar(i).getCitizen() == newUser->getPrivilege() && getAvatar(i).getCitizen() != 0)
				newUser->OnPrivilegeResolved(getAvatar(i).getName());
			else if (getAvatar(i).getPrivilege() == newUser->getPrivilege() && getAvatar(i).getPrivName().GetLength() > 0)
				newUser->OnPrivilegeResolved(getAvatar(i).getPrivName());
		}

		// lookup the user
		if (newUser->getPrivName().GetLength() == 0)
		{
			PrivilegeLookupClass* pLC = new PrivilegeLookupClass();
			CitizenLookup::g_Kernel.lookup_by_number(newUser->getPrivilege(), pLC);
		}
	}



	// trigger processor
	onProcessAvatarAdd(*newUser);
}

void CommonBot::onEventAvatarChange(void)
{
	// search for the avatar
	Avatar* pUser = getAvatars().getSession(_int(AW_AVATAR_SESSION));
	if (pUser == 0) return;

	// update the avatars position
	pUser->getChanges(*this);
	pUser->updateLastMovementTime();

	// trigger processor
	onProcessAvatarChange(*pUser);
}

void CommonBot::onEventAvatarDelete(void)
{
	// search for the avatar
	Avatar* pUser = getAvatars().getSession(_int(AW_AVATAR_SESSION));
	if (pUser == 0) return;

	// delete
	getAvatars().remove(pUser);

	// remove
	pUser->OnExit();

	// trigger processor
	onProcessAvatarDelete(*pUser);
	delete pUser;
}

void CommonBot::onEventAvatarClick()
{
	// search for the avatar
	Avatar* pUser = getAvatars().getSession(_int(AW_AVATAR_SESSION));
	if (pUser == 0) return;

	// search for the avatar
	Avatar* pClicked = getAvatars().getSession(_int(AW_CLICKED_SESSION));
	if (pClicked == 0) return;

	// trigger processor
	onProcessAvatarClick(*pUser, *pClicked);
}

void CommonBot::onCallbackAddress(int ReasonCode)
{
	// search for the avatar
	Avatar* pUser = getAvatars().getSession(_int(AW_AVATAR_SESSION));
	if (pUser == 0) return;

	// trigger processor
	onProcessAvatarAddress(*pUser, ReasonCode);
}

void CommonBot::onEventPrivilegeResolved(AW::Citizen& Info)
{
	for (av_iterator av = av_begin(); av != av_end(); av++)
	{
		if (av->getPrivilege() == Info.getCitizen() && av->getPrivLUS() != LookupState::Complete)
		{
			av->OnPrivilegeResolved(Info.getName());
			onProcessAvatarPrivilege(*av);
		}
	}
}

//
//
// Mover Events
//
//

void CommonBot::onEventEntityAdd(void)
{
	// add a mover to the entity list
	Movers::Mover* pMover = getMovers().getById(_int(AW_ENTITY_ID));
	if (pMover == 0)
	{
		Movers::Mover mv;
		mv.on_entity_add(*this);
		mv.m_pBot = this;

		// add to the list of movers
		getMovers().push_back(mv);

		// look up the id again
		Movers::Mover* pMover = getMovers().getById(_int(AW_ENTITY_ID));
		if (pMover == 0) return;

		// trigger processor
		onProcessEntityAdd(*pMover);

		// scripting host
		ScriptingHost::getInst().doFunction("EventEntityAdd", 
			&Umbra::VariantArray(*pMover), 0, &bot);
	}
	else
	{
		if (pMover->getState() != _int(AW_ENTITY_STATE))
		{
			pMover->on_entity_change(*this);
			onProcessEntityChange(*pMover);
		}
	}
}

void CommonBot::onEventEntityChange(void)
{
	// search for mover
	Movers::Mover* pMover = getMovers().getById(_int(AW_ENTITY_ID));
	if (pMover == 0) return;

	// update changes
	// pMover->on_entity_change(*this);

	// trigger processor
	onProcessEntityChange(*pMover);

	// scripting host
	ScriptingHost::getInst().doFunction("EventEntityChange", 
		&Umbra::VariantArray(*pMover), 0, &bot);
}

void CommonBot::onEventEntityDelete(void)
{
	// search for mover
	Movers::Mover* pMover = getMovers().getById(_int(AW_ENTITY_ID));
	if (pMover == 0) return;

	// update changes
	pMover->on_entity_delete(*this);

	// trigger processor
	onProcessEntityDelete(*pMover);

	// scripting host
	ScriptingHost::getInst().doFunction("EventEntityDelete", 
		&Umbra::VariantArray(*pMover), 0, &bot);

	// delete it from the list
	// getMovers().remove_by_id(_int(AW_ENTITY_ID));
}

void CommonBot::onEventEntityRiderAdd(void)
{
	// search for user
	Avatar* pUser = getAvatars().getSession(_int(AW_AVATAR_SESSION));
	if (pUser == 0) return;
	
	// search for mover
	Movers::Mover* pMover = getMovers().getById(_int(AW_ENTITY_ID));
	if (pMover == 0) return;

	// assign mover
	pUser->connected_mover_id = pMover->getId();
	pMover->connected_avatars.push_back(pUser->getSession());

	// trigger rider add
	onProcessEntityRiderAdd(*pUser, *pMover);

	// scripting host
	ScriptingHost::getInst().doFunction("EventEntityRiderAdd", 
		&Umbra::VariantArray(*pUser, *pMover), 0, &bot);
}

void CommonBot::onEventEntityRiderChange(void)
{
	// search for user
	Avatar* pUser = getAvatars().getSession(_int(AW_AVATAR_SESSION));
	if (pUser == 0) return;
	
	// search for mover
	Movers::Mover* pMover = getMovers().getById(_int(AW_ENTITY_ID));
	if (pMover == 0) return;

	// trigger rider add
	onProcessEntityRiderChange(*pUser, *pMover);
}

void CommonBot::onEventEntityRiderDelete(void)
{
	// search for user
	Avatar* pUser = getAvatars().getSession(_int(AW_AVATAR_SESSION));
	if (pUser == 0) return;
	
	// search for mover
	Movers::Mover* pMover = getMovers().getById(_int(AW_ENTITY_ID));
	if (pMover == 0) return;

	// de-assign mover
	pUser->connected_mover_id = 0;

	// remove the avatar
	for (size_t i = 0; i < pMover->connected_avatars.size(); i++)
	{
		if (pMover->connected_avatars[i] != pUser->getSession())
			continue;

		pMover->connected_avatars.erase(pMover->connected_avatars.begin() + i);
		break;
	}

	// trigger rider add
	onProcessEntityRiderDelete(*pUser, *pMover);

	// scripting host
	ScriptingHost::getInst().doFunction("EventEntityRiderDelete", 
		&Umbra::VariantArray(*pUser, *pMover), 0, &bot);

	// delete the mover
	getMovers().remove_by_id(pMover->getId());
}

void CommonBot::onEventEntityLinks(void)
{
}

//
//
// World Connection Edits
//
//

void CommonBot::onEventVisible(void)
{
	// trigger processor
	onProcessWorldVisible();
}

void CommonBot::onEventWorldDisconnect(int Reason)
{
	// trigger disconnect
	onProcessWorldDisconnect(Reason);
}

void CommonBot::onEventWorldConnectionChange(int State)
{
	// is the connection activated
	if (State == AW::Enums::ConState::Connected)
		onProcessWorldLinkOpened();

	// is no longer open
	if (State != AW::Enums::ConState::Connected)
	{
		// close the world link
		onProcessWorldLinkClosed();

		// remove each avatar
		for (size_t i = 0; i < getAvatars().size(); i++)
			onProcessAvatarCleanup(getAvatars()[i]);

		// clean up the avatars list
		getAvatars().Clear("");
	}

	// trigger process
	onProcessWorldConnectionChange(State);
}

void CommonBot::onEventWorldAttributes(void)
{
	// trigger process
	onProcessWorldAttributes();
}

//
//
// Universe Connection
//
//

void CommonBot::onEventUniverseDisconnect(int Reason)
{
	onProcessUniverseDisconnect(Reason);
}

void CommonBot::onEventUniverseRecover(void)
{
}

void CommonBot::onEventUniverseAttributes(void)
{
	// trigger processor
	onProcessUniverseAttributes();
}

void CommonBot::onEventUniverseLinkClosed(void)
{
	// trigger processor
	onProcessUniverseLinkClosed();
}

void CommonBot::onEventUniverseLinkOpened(void)
{
	// trigger processor
	onProcessUniverseLinkOpened();
}

//
//
// Communications Events
//
//

void CommonBot::onEventChat(void)
{
	// search for user
	Avatar* pUser = getAvatars().getSession(_int(AW_CHAT_SESSION));
	if (pUser == 0) return;

	// trigger processor
	pUser->updateLastChatTime();
	onProcessChat(*pUser, _string(AW_CHAT_MESSAGE), _int(AW_CHAT_TYPE));
}

void CommonBot::onEventConsoleMessage(void)
{
	// trigger processor
	onProcessConsoleMessage();
}

void CommonBot::onEventBotgram(void)
{
	// trigger processor
	onProcessBotgram();
}

void CommonBot::onEventUrlClick(void)
{
	// search for user
	Avatar* pUser = getAvatars().getSession(_int(AW_CHAT_SESSION));
	if (pUser == 0) return;

	// trigger processor
	onProcessUrlClick(*pUser, _string(AW_URL_NAME));
}

//
//
// Communications
//
//

int CommonBot::say(const char* Message)
{
	/* variables for this function */
	CStringA	logText, text;

	/* preprocess the size and style */
	std::vector<CStringA> lines;
	size_t c = SplitToMultipart(lines, Message, AW_MAX_ATTRIBUTE_LENGTH, m_OutputBreakers);
	int typeID = (OT_MAIN | (HasPS() ? MF_FBOLD : 0));

	for (size_t i = 0; i < c; i++) {
		text = lines[i];
		
		/* write information message */
		logText.Format("[%s]:\t%s%s", getName(), "", text);
		WriteMsg(logText, 0xC000C0, OT_MAIN | typeID);

		// return the reason code
		_say(text);
	}
	return 0;
}

int	CommonBot::consoleBroadcast(const char* MessageText, COLORREF Colour, int Style)
{
	std::vector<String> lines;
	int length = AW_MAX_ATTRIBUTE_LENGTH;
	size_t c = SplitToMultipart(lines, MessageText, length, bot.getOuputBreakers());

	for (size_t i = 0; i <c; i++)
	{
		String line = lines[i];
		String log, out;
		if ((Style & CM_SUPPRESSED) == 0) 
		{
			log.Format("%s:\t(c-to broadcast()) %s", getName(), line);
			WriteMsg(log, Colour, OT_MAIN);
		}
		/* whisper to the target */
		out.Format("%s", line);
		int rc = ConsoleMessage(0, out, Colour, Style);
		if (rc) return rc;
	}
	return 0;
}

int	CommonBot::consoleBroadcastPrefixed(String Prefix, String MsgText, COLORREF Colour, int Style)
{
	std::vector<String> lines;
	int length = AW_MAX_ATTRIBUTE_LENGTH - 3 - Prefix.GetLength();
	size_t c = SplitToMultipart(lines, MsgText, length, bot.getOuputBreakers());

	for (size_t i = 0; i <c; i++)
	{
		String line = lines[i];
		String log, out;
		if ((Style & CM_SUPPRESSED) == 0) 
		{
			log.Format("%s:\t(c-to broadcast()) %s", Prefix, getName(), line);
			WriteMsg(log, Colour, OT_MAIN);
		}
		/* whisper to the target */
		out.Format("%s:\t%s", Prefix, line);
		int rc = ConsoleMessage(0, out, Colour, Style);
		if (rc) return rc;
	}
	return 0;
}

int	CommonBot::broadcastUrl(String UrlText, String Frame, String PostData, bool Target3d)
{
	Lock();
	_bool_set	(AW_URL_TARGET_3D, Target3d ? TRUE : FALSE);
	_string_set	(AW_URL_POST,	   PostData);
	int rc = _url_send(0, UrlText, Frame);
	Unlock();

	String logMsg;
	logMsg.Format("Broadcast URL '%s' to all", UrlText);
	WriteMsg(logMsg, RGB_BLUE, OT_MAIN | OM_TIME);
	return rc;
}