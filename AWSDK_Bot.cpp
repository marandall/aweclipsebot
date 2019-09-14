#include "stdafx.h"

/* inclusions for the AWSDK C++ Edition */
#include "awsdk_avatar.h"
#include "awsdk_bot.h"
#include "awsdk_bots.h"
#include "awsdk_citizen.h"
#include "awsdk_profile.h"
#include "awsdk_support.h"
#include "awsdk_universe.h"
#include "awsdk_world.h"
#include "awsdk_hudelement.h"
#include "awsdk_colours.h"

#include <math.h>

// special check to handle pointer returns
#define PTR_BASE_CHECK()										\
	if (getInitialised() == FALSE) return NULL;					\
	if (SelectInstance() == FALSE) return NULL;


namespace AW {
	/* statics */
	Bots bots;
	bool Bot::m_initialised;
}

int ext_check_world_right(int citizen, LPCSTR string);

namespace AW 
{

	float Location::GetRadianYAW(void) const
	{
		return m_yaw * 0.001745329f;
	}
	
	/* creates formatted citizen info */
	String AvatarData::getCitizenInfo(void) const
	{
		/* DEFINE: variables for in function */
		String name_extension, text;
		if (getPrivLUS() == LookupState::Complete)
			name_extension.Format(" - %s", m_priv_name);	
		
		/* SITUATION: User is a tourist */
		if (m_citizen == 0 && m_privilege == 0)	
			return "@tourist";
		else if (m_citizen == m_privilege)
			text.Format("Citizen: %d", getCitizen());
		else if (m_citizen == 0) 
			text.Format("Privilege: %d%s", m_privilege, name_extension);	
		else 
			text.Format("Citizen: %d, Privilege: %d%s", m_citizen, m_privilege, name_extension);
		return text;
	}

	/* creates formatted address info */
	String AvatarData::getAddressInfo(void) const
	{
		/* variables used */
		if (getAddressLUS() == LookupState::Complete) {
			String text;
			text.Format("addr: %s", m_address);
			/* do we also need to include the dns? */
			if (getDnsLUS() == LookupState::Complete)
				text.AppendFormat(" (dns: %s)", getDns());
			return text;
		} 
		else
			return "Unavailable";
	}

	bool AvatarData::IsPattern(String Pattern, bool IpAuthorised) const
	{
		/* type checks */
		String type	= Pattern.Left(1);
		String data	= Pattern.Mid(1);
		int	  l_data= atoi(data);
		int	  l_len = data.GetLength();

		/* check each type value */
		if (Pattern  == "{*}")					return true;
		else if (Pattern == "any()")			return true;
		else if (Pattern == "all()")			return true;
		else if (type == "$")					return getSession() == l_data;
		else if (type == "^")					return getCitizen() == l_data;
		else if (type == "#")					return getCitizen() == l_data;
		else if (type == "*")					return getPrivilege() == l_data;
		else if (type == "!" && IpAuthorised)	return getAddress().Left(l_len) == data;
		else if (type == "%" && IpAuthorised)	return getDns().MakeLower().Find(data, 0) != -1;
		else if (type == "?")					return getName().MakeLower().Find(data, 0) != -1;
		else if (Pattern == "bot()")			return IsBot();
		else if (Pattern == "tourist()")		return IsTourist();
		else if (Pattern == "citizen()")		return IsCitizen();
		else return getName().CompareNoCase(Pattern) == 0;
	}

	int	Avatar::Whisper(LPCSTR message) 
	{
		return plc_bot->Whipser(getSession(), message);
	}

	int Avatar::ConsoleMsg(LPCSTR message, COLORREF colour, int Style)
	{
		return plc_bot->ConsoleMessage(getSession(), message, colour, Style);
	}

	int	Avatar::Noise(LPCSTR sound) 
	{
		plc_bot->_string_set(AW_SOUND_NAME, sound);
		return plc_bot->_noise(m_session);
	}

	int Avatar::Teleport(int x, int y, int z, int yaw, LPCSTR world, bool warp) 
	{
		return plc_bot->TeleportUser(getSession(), x, y, z, yaw, world, warp);
	}

	int Avatar::Teleport(const Location& pt, LPCSTR world, bool warp) 
	{
		return Teleport(pt.getX(), pt.getY(), pt.getZ(), pt.getYAW(), world, warp);
	}

	int Avatar::Eject(int duration) 
	{
		return plc_bot->Eject(getSession(), duration);
	}

	int Avatar::Avatarset(int avatar, int gesture, int state) 
	{
		plc_bot->_int_set(AW_AVATAR_TYPE,		avatar == -1 ? m_avatar : avatar);
		plc_bot->_int_set(AW_AVATAR_GESTURE,	gesture == -1 ? m_gesture : gesture);
		plc_bot->_int_set(AW_AVATAR_STATE,		state == -1 ? m_state	: state);
		return plc_bot->_avatar_set(m_session);
	}

	void Avatar::OnIpLookupResult(unsigned int ip, int rc) {
		String ip_str = _XConvert::long_to_ip_string(ip);
		setAddress(		ip_str );
		m_address_lng	= ip;
		setAddressLUS(	LookupState::Complete	);
	}

	void Avatar::OnDnsLookupResult(LPCSTR dns, int state) {
		setDns   (	dns		);
		setDnsLUS(	state	);
	}

	int	Avatar::LookupIP() {
		return plc_bot->_address(m_session);
	}

	/* feedback messages for the privilege name */
	void Avatar::OnPrivilegeResolved(LPCSTR name) {
		setPrivName	( name );
		setPrivLUS	( LookupState::Complete );
	}


	int	Avatar::getStartup	(const Bot& bot)
	{
		m_name		= plc_bot->_string(AW_AVATAR_NAME);
		m_session	= plc_bot->_int(AW_AVATAR_SESSION);

		/* session and such information */
		setCitizen		(plc_bot->_int(AW_AVATAR_CITIZEN));
		setPrivilege	(plc_bot->_int(AW_AVATAR_PRIVILEGE));
		setBuild		(plc_bot->_int(AW_AVATAR_VERSION));

		/* work out the type */
		if (getCitizen() + getPrivilege() == 0)
			setType( UserType::Tourist );
		else if (getCitizen() == 0)
			setType( UserType::Bot );
		else
			setType( UserType::Citizen );
		return TRUE;
	}

	int	Avatar::getChanges(const Bot& bot)
	{
		/* uses the sdk to read the avatars position from the memory and convert to the */
		/*       active location (used in conjunction with event avatar change)         */
		m_x				= plc_bot->_int(AW_AVATAR_X);
		m_y				= plc_bot->_int(AW_AVATAR_Y);
		m_z				= plc_bot->_int(AW_AVATAR_Z);
		m_yaw			= plc_bot->_int(AW_AVATAR_YAW);
		m_pitch			= plc_bot->_int(AW_AVATAR_PITCH);
		m_state			= plc_bot->_int(AW_AVATAR_STATE);
		m_avatar		= plc_bot->_int(AW_AVATAR_TYPE);
		m_gesture		= plc_bot->_int(AW_AVATAR_GESTURE);
		m_ryaw			= _XConvert::aw_angle_to_radians(m_yaw);

		// following is for 4.1 builds only
	#if (AW_BUILD > 51)
		m_flags			= plc_bot->_int(AW_AVATAR_FLAGS);
		m_locked		= plc_bot->_bool(AW_AVATAR_LOCK);
	#endif
		return TRUE;
	}

	int	Avatar::CreateHUD(HUD_Element& Element)
	{
		Element.CopyToSDK(*this->plc_bot);
		plc_bot->_int_set(AW_HUD_ELEMENT_SESSION, m_session);

		/*
		if (m_name == "Strike Rapier")
		{
			CStringA log;
			log.Format("HUD_CREATE #%d %s pos(%d, %d), dim(%d, %d), flags=%d",
				Element.id, Element.text, Element.x, Element.y, Element.size_x, Element.size_y, Element.flags);
			Whisper(log);
		}*/

		return plc_bot->_hud_create();
	}
	
	int Avatar::DestroyHUD(int ElementID)
	{
		return plc_bot->_hud_destroy(m_session, ElementID);
	}

	int	Avatar::ClearHUD()
	{
		return plc_bot->_hud_clear(m_session);
	}
}


namespace AW 
{
	int MakeConsoleStyle(bool Bold, bool Italic, bool Supressed)
	{
		return (Bold ? CM_BOLD : 0) |
			(Italic ? CM_ITALIC : 0) |
			(Supressed ? CM_SUPPRESSED : 0);
	}


	Bot::Bot(void) {
		setUniverseConnection(ConState::Nothing);
		setWorldConnection(ConState::Nothing);
	}

	Bot::~Bot(void)
	{
	}

	CString Bot::ConnectionToType(int type, bool ext) {
		switch (type) {
		case ConState::Nothing:		return ext ? "CLSE" : "Closed";
		case ConState::Creating:	return ext ? "CRTG" : "Creating";
		case ConState::Created:		return ext ? "CRTD" : "Created";
		case ConState::Connecting:	return ext ? "CNCT" : "Connecting";
		case ConState::Connected:	return ext ? "ESTB" : "Established";
		case ConState::Recovering:	return ext ? "RCVR" : "Recovering";
		default:					return ext ? "UNKN" : "Unknown";
		}
	}

	bool Bot::IsWorldConnected(void) const { return m_world_connection	== ConState::Connected;	}
	bool Bot::IsWorldClosed(void) const { return m_world_connection	== ConState::Nothing;	}
	bool Bot::IsUniConnected(void) const { return m_universe_connection == ConState::Connected;	}
	bool Bot::IsUniClosed(void) const { return m_universe_connection == ConState::Nothing;	}

	CString Bot::getUniverseVerID(int build) {
		if (build < 70)
			return "3.6";
		else if (build == 71)
			return "4.1 Beta 1";
		else if (build == 71)
			return "4.1 Beta 2";
		else if (build == 73)
			return "4.1 Beta 3";
		else if (build == 74)
			return "4.1 Beta 4";
		else if (build == 75)
			return "4.1 Beta 5";
		else
			return "4.1 Release";
	}


	void Bot::WriteMsgB(LPCSTR msg, long col, int opts) {
		CString out;
		out.Format("%s\t%s", m_UniqueID, msg);
	}


	int Bot::_int_set(AW_ATTRIBUTE attribute, int value) const{
		/* check instance */
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;
		return aw_int_set(attribute, value);
	}

	int Bot::_string_set(AW_ATTRIBUTE attribute, LPCSTR value) const{
		/* check instance */
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;
		return aw_string_set(attribute, value);
	}

	int	Bot::_float_set(AW_ATTRIBUTE attribute, float value) const{
		/* check instance */
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;
		return aw_float_set(attribute, value);
	}

	int	Bot::_bool_set(AW_ATTRIBUTE attribute, BOOL value) const{
		/* check instance */
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;
		return aw_bool_set(attribute, value);
	}

	int	Bot::_data_set(AW_ATTRIBUTE attribute, LPCSTR data, unsigned int length) const {
		/* check instance */
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;
		return aw_data_set(attribute, data, length);
	}

	int	Bot::_int(AW_ATTRIBUTE attribute) const {
		/* check instance */
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;
		return aw_int(attribute);
	}

	String Bot::_string(AW_ATTRIBUTE attribute) const {
		/* check instance */
		if (SelectInstance() == FALSE)		return "444";
		return aw_string(attribute);
	}

	float Bot::_float(AW_ATTRIBUTE attribute) const {
		/* check instance */
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;
		return aw_float(attribute);
	}

	BOOL Bot::_bool(AW_ATTRIBUTE attribute) const {
		/* check instance */
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;
		return aw_bool(attribute);
	}

	void Bot::_exchange(bool ToSdk, AW_ATTRIBUTE Attribute, int& Var)
	{
		if (ToSdk == AW::EXCHANGE_UPDATE_DATA) Var = _int(Attribute);
		else _int_set(Attribute, Var);
	}
	
	void Bot::_exchange(bool ToSdk, AW_ATTRIBUTE Attribute, float& Var)
	{
		if (ToSdk == AW::EXCHANGE_UPDATE_DATA) Var = _float(Attribute);
		else _float_set(Attribute, Var);
	}
	
	void Bot::_exchange(bool ToSdk, AW_ATTRIBUTE Attribute, String& Var)
	{
		if (ToSdk == AW::EXCHANGE_UPDATE_DATA) Var = _string(Attribute);
		else _string_set(Attribute, Var);
	}

	void Bot::_exchange(bool ToSdk, AW_ATTRIBUTE Attribute, ColourTripletWrapper& Var)
	{
		if (ToSdk == AW::EXCHANGE_UPDATE_DATA) Var.pClr->value = RGB(_int(Attribute), _int((AW_ATTRIBUTE)(Attribute + 1)), _int((AW_ATTRIBUTE)(Attribute + 2)));
		else { 
			_int_set(Attribute, Var.pClr->GetRed());
			_int_set((AW_ATTRIBUTE)(Attribute + 1), Var.pClr->GetGreen());
			_int_set((AW_ATTRIBUTE)(Attribute + 2), Var.pClr->GetBlue());
		}
	}


	char* Bot::_data(AW_ATTRIBUTE attribute, UINT* length) const{
		/* check instance */
		if (SelectInstance() == FALSE)		return NULL;
		return aw_data(attribute, length);
	}

	void* Bot::_event(AW_EVENT_ATTRIBUTE A)
	{
		if (SelectInstance() == FALSE) return NULL;
		return aw_event(A);
	}
	
	void* Bot::_callback(AW_CALLBACK A)
	{
		if (SelectInstance() == FALSE) return NULL;
		return aw_callback(A);
	}

	int	Bot::Initialise()
	{
		/* check existing initialised state */
		if (getInitialised() == TRUE)
			return RC_SUCCESS;

		/* attempt to initialise bots */
		int rc = aw_init(AW_BUILD);

		/* set initialised state */
		m_initialised = (rc == RC_SUCCESS ? TRUE : FALSE);

		/* return rc */
		return rc;
	}

	int	Bot::TriggerEvents(int ms)
	{
		/* check initialised state */
		if (getInitialised() == FALSE)
			return RC_NOT_INITIALIZED;

		/* select instance ptr for our use */
		if (SelectInstance() == FALSE)
			return RC_NO_INSTANCE;

		/* now we trigger them */
		return aw_wait(ms);
	} 

	BOOL Bot::SelectInstance() const 
	{
		/* are we initialised? If not we return NULL */
		if (getInitialised() == FALSE) return FALSE;
		
		// already selected
		if (aw_instance() == m_instance)
			return TRUE;
		
		/* set our new instance */
		void* vptr = (void*)this;
		int rc = aw_instance_set(m_instance);
		return TRUE;
	}

	unsigned int Bot::_session() const {
		if (SelectInstance() == FALSE)
			return RC_NO_INSTANCE;
		return aw_session();
	}

	int Bot::_callback_set(AW_CALLBACK c, pfnc_callback fptr) {
		/* check instance */
		if (SelectInstance() == FALSE)
			return RC_NO_INSTANCE;

		/* call events */
		return aw_callback_set(c, fptr);
	}

	int Bot::CallbackSet(AW_CALLBACK cbe, bool set) {
		/* check initialised state */
		if (getInitialised() == FALSE)	return RC_NOT_INITIALIZED;
		if (SelectInstance() == FALSE)	return RC_NO_INSTANCE;

		/* switch through callbacks */
		switch (cbe) {
			case AW_CALLBACK_CREATE:			
				return _callback_set(cbe, set ? AWAPI_callback_create	: NULL);
			
			case AW_CALLBACK_LOGIN:				
				return _callback_set(cbe, set ? AWAPI_callback_login : NULL);
			
			case AW_CALLBACK_ENTER:				
				return _callback_set(cbe, set ? AWAPI_callback_enter : NULL);
			
			case AW_CALLBACK_ADDRESS:			
				return _callback_set(cbe, set ? AWAPI_callback_address : NULL);
			
			case AW_CALLBACK_CITIZEN_ATTRIBUTES:
				return _callback_set(cbe, set ? AWAPI_callback_citizen_attributes	: NULL);
			
			case AW_CALLBACK_CITIZEN_RESULT:	
				return _callback_set(cbe, set ? AWAPI_callback_citizen_result	: NULL);
			
			case AW_CALLBACK_QUERY:				
				return _callback_set(cbe, set ? AWAPI_callback_query : NULL);
			
			case AW_CALLBACK_CELL_RESULT:		
				return _callback_set(cbe, set ? AWAPI_callback_cell_result : NULL);
			
			case AW_CALLBACK_USER_LIST:			
				return _callback_set(cbe, set ? AWAPI_callback_user_list : NULL);
			
			case AW_CALLBACK_OBJECT_RESULT:		
				return _callback_set(cbe, set ? AWAPI_callback_object_result : NULL);
			
			case AW_CALLBACK_WORLD_LIST:		
				return _callback_set(cbe, set ? AWAPI_callback_world_list : NULL);
			
			case AW_CALLBACK_HUD_RESULT:		
				return _callback_set(cbe, set ? AWAPI_callback_hud_result : NULL);
			
			case AW_CALLBACK_BOTGRAM_RESULT:	
				return _callback_set(cbe, set ? AWAPI_callback_botgram_result	: NULL);
			
			case AW_CALLBACK_WORLD_EJECTION:	
				return _callback_set(cbe, set ? AWAPI_callback_world_ejection	: NULL);
			
			case AW_CALLBACK_WORLD_EJECTION_RESULT:
				return _callback_set(cbe, set ? AWAPI_callback_world_ejection_result : NULL);
		};

		return RC_SUCCESS;
	}

	/* dispatch handlers to the AWAPI*/
	#define BASE_BOT_EVENT_DEFINE(handle, caller)		\
		void Bot::AWAPI_event_##handle()			\
		{												\
			Bot* pBot = bots.getCurInstance();		\
			if (pBot == NULL)							\
				return;									\
			pBot->onEvent##caller();					\
		}

	#define BASE_BOT_CALLBACK_DEFINE(handle, caller)	\
		void Bot::AWAPI_callback_##handle(int rc)	\
		{												\
			Bot* pBot = bots.getCurInstance();		\
			if (pBot == NULL)							\
				return;									\
			pBot->onCallback##caller(rc);				\
		}

	#define BASE_BOT_CALLBACK_SYS_DEFINE(handle, caller)\
		void Bot::AWAPI_callback_##handle(int rc)	\
		{												\
			Bot* pBot = bots.getCurInstance();		\
			if (pBot == NULL)							\
				return;									\
				pBot->OnSys_Callback##caller(rc);		\
		}

	/* DISPATCH   Avatar Events */
	BASE_BOT_EVENT_DEFINE(avatar_add,			AvatarAdd);
	BASE_BOT_EVENT_DEFINE(avatar_change,		AvatarChange);
	BASE_BOT_EVENT_DEFINE(avatar_delete,		AvatarDelete);
	BASE_BOT_EVENT_DEFINE(avatar_click,			AvatarClick);
	BASE_BOT_CALLBACK_DEFINE(address,			Address);

	/* DISPATCH   Universe Queries */
	BASE_BOT_CALLBACK_DEFINE(citizen_result,	CitizenResult);
	BASE_BOT_CALLBACK_DEFINE(citizen_attributes,CitizenAttributes);

	/* DISPATCH   Query Callbacks */
	BASE_BOT_CALLBACK_DEFINE(query, Query);

	/* DISPATCH   Object Events */
	BASE_BOT_EVENT_DEFINE(object_add,			ObjectAdd);
	BASE_BOT_EVENT_DEFINE(object_delete,		ObjectDelete);
	BASE_BOT_EVENT_DEFINE(object_click,			ObjectClick);
	BASE_BOT_EVENT_DEFINE(object_select,		ObjectSelect);
	BASE_BOT_CALLBACK_DEFINE(object_result,		ObjectResult);

	/* DISPATCH:  Terrain Events */
	BASE_BOT_EVENT_DEFINE(terrain_begin,		TerrainBegin);
	BASE_BOT_EVENT_DEFINE(terrain_changed,		TerrainChanged);
	BASE_BOT_EVENT_DEFINE(terrain_data,			TerrainData);
	BASE_BOT_EVENT_DEFINE(terrain_end,			TerrainEnd);

	/* DISPATCH:  Hud Events */
	BASE_BOT_EVENT_DEFINE(hud_click,			HudClick);
	BASE_BOT_EVENT_DEFINE(hud_create,			HudCreate);
	BASE_BOT_EVENT_DEFINE(hud_destroy,			HudDestroy);
	BASE_BOT_EVENT_DEFINE(hud_clear,			HudClear);

	void Bot::AWAPI_callback_hud_result(int rc)
	{
		if (rc)
		{
			int k = 0;
		}
	}

	// BASE_BOT_CALLBACK_DEFINE(hud_result,		HudResult);

	/* DISPATCH:  Entity Movers */
	BASE_BOT_EVENT_DEFINE(entity_add,			EntityAdd);
	BASE_BOT_EVENT_DEFINE(entity_change,		EntityChange);
	BASE_BOT_EVENT_DEFINE(entity_delete,		EntityDelete);
	BASE_BOT_EVENT_DEFINE(entity_rider_add,		EntityRiderAdd);
	BASE_BOT_EVENT_DEFINE(entity_rider_change,	EntityRiderChange);
	BASE_BOT_EVENT_DEFINE(entity_rider_delete,	EntityRiderDelete);
	BASE_BOT_EVENT_DEFINE(entity_links,			EntityLinks);

	/* DISPATCH   Chat and Communications Events */
	BASE_BOT_EVENT_DEFINE(chat,					Chat);
	BASE_BOT_EVENT_DEFINE(console_message,		ConsoleMessage);
	BASE_BOT_EVENT_DEFINE(botgram,				Botgram);
	BASE_BOT_EVENT_DEFINE(url_click,			UrlClick);
	BASE_BOT_CALLBACK_DEFINE(botgram_result,	BotgramResult);

	/* DISPATCH   Cell Events*/
	BASE_BOT_EVENT_DEFINE(cell_begin,			CellBegin);
	BASE_BOT_EVENT_DEFINE(cell_end,				CellEnd);
	BASE_BOT_EVENT_DEFINE(cell_object,		CellObject);
	BASE_BOT_CALLBACK_DEFINE(cell_result,	CellResult);

	BASE_BOT_CALLBACK_DEFINE(create,			Create);
	BASE_BOT_CALLBACK_SYS_DEFINE(enter,		Enter);

	/* DISPATCH	  User List Events */
	BASE_BOT_EVENT_DEFINE(user_info,			UserInfo);
	BASE_BOT_CALLBACK_DEFINE(user_list,		UserList);

	/* DISPATCH	  World List Events */
	BASE_BOT_EVENT_DEFINE(world_info,			WorldInfo);
	BASE_BOT_CALLBACK_DEFINE(world_list,	WorldList);

	/* DISPATCH   Teleport Mechanisms */
	BASE_BOT_EVENT_DEFINE(teleport,				Teleport);

	// callback for eject and eject result
	BASE_BOT_CALLBACK_DEFINE(world_ejection,	WorldEjection);
	BASE_BOT_CALLBACK_DEFINE(world_ejection_result,	WorldEjectionResult);

	void Bot::setAvatarEvents(bool yes){
		EventSet(AW_EVENT_AVATAR_ADD,			yes);	
		EventSet(AW_EVENT_AVATAR_CHANGE,		yes);
		EventSet(AW_EVENT_AVATAR_DELETE,		yes);	
		EventSet(AW_EVENT_AVATAR_CLICK,			yes);
	}

	void Bot::setObjectEvents(bool yes){
		EventSet(AW_EVENT_OBJECT_ADD,			yes);	
		EventSet(AW_EVENT_OBJECT_CLICK,			yes);
		EventSet(AW_EVENT_OBJECT_DELETE,		yes);	
		EventSet(AW_EVENT_OBJECT_SELECT,		yes);
	}

	void Bot::setCellEvents(bool yes){
		EventSet(AW_EVENT_CELL_BEGIN,			yes);	
		EventSet(AW_EVENT_CELL_END,				yes);
		EventSet(AW_EVENT_CELL_OBJECT,			yes);
	}

	void Bot::setWorldEvents(bool yes){
		EventSet(AW_EVENT_WORLD_ATTRIBUTES,		yes);	
		EventSet(AW_EVENT_WORLD_DISCONNECT,		yes);
	}

	void Bot::setUniverseEvents(bool yes){
		EventSet(AW_EVENT_UNIVERSE_ATTRIBUTES,	yes);
		EventSet(AW_EVENT_UNIVERSE_DISCONNECT,	yes);
	}

	void Bot::setCommunicationEvents(bool yes)
	{
		EventSet(AW_EVENT_CHAT,					yes);	
		EventSet(AW_EVENT_CONSOLE_MESSAGE,		yes);
		EventSet(AW_EVENT_BOTGRAM,				yes);
	}

	void Bot::setUserListEvents(bool yes) 
	{
		EventSet(AW_EVENT_USER_INFO,			yes);
		CallbackSet(AW_CALLBACK_USER_LIST,		yes);
	}

	void Bot::setTerrainEvents(bool yes) 
	{
		EventSet(AW_EVENT_TERRAIN_BEGIN,		yes);
		EventSet(AW_EVENT_TERRAIN_END,			yes);
		EventSet(AW_EVENT_TERRAIN_DATA,			yes);
		EventSet(AW_EVENT_TERRAIN_CHANGED,		yes);
	}

	void Bot::setHudEvents(bool yes) {
		EventSet(AW_EVENT_HUD_CLICK,			yes);
		EventSet(AW_EVENT_HUD_CREATE,			yes);
		EventSet(AW_EVENT_HUD_DESTROY,			yes);
		EventSet(AW_EVENT_HUD_CLEAR,			yes);
	}

	void Bot::setEntityEvents(bool yes)
	{
		int rc = EventSet	(AW_EVENT_ENTITY_ADD,		yes);
		EventSet	(AW_EVENT_ENTITY_CHANGE,	yes);
		EventSet	(AW_EVENT_ENTITY_DELETE,	yes);
		EventSet	(AW_EVENT_ENTITY_LINKS,		yes);
	}

	void Bot::SetRiderEvents(bool yes)
	{
		EventSet	(AW_EVENT_ENTITY_RIDER_ADD,		yes);
		EventSet	(AW_EVENT_ENTITY_RIDER_CHANGE,	yes);
		EventSet	(AW_EVENT_ENTITY_RIDER_DELETE,	yes);
	}

	int Bot::_event_set(AW_EVENT_ATTRIBUTE a, pfnc_event fptr) {
		/* check instance */
		if (SelectInstance() == FALSE)	return RC_NO_INSTANCE;
		/* call events */
#	if AW_BUILD < 51
		return aw_event_set(a, fptr);
	#else
		return aw_instance_event_set(a, fptr);
	#endif
	}

	int	Bot::EventSet(AW_EVENT_ATTRIBUTE evt, bool set)
	{
		/* check initialised state */
		if (getInitialised() == FALSE)		return RC_NOT_INITIALIZED;
		if (SelectInstance() == FALSE)		return RC_NO_INSTANCE;

		/* switch through events */
		switch (evt) {
		case AW_EVENT_AVATAR_ADD:			return _event_set(evt,	set ? AWAPI_event_avatar_add		: NULL);
		case AW_EVENT_AVATAR_CHANGE:		return _event_set(evt,	set ? AWAPI_event_avatar_change		: NULL);
		case AW_EVENT_AVATAR_DELETE:		return _event_set(evt,	set ? AWAPI_event_avatar_delete		: NULL);
		case AW_EVENT_AVATAR_CLICK:			return _event_set(evt,	set ? AWAPI_event_avatar_click		: NULL);

		case AW_EVENT_OBJECT_ADD:			return _event_set(evt,	set ? AWAPI_event_object_add		: NULL);
		case AW_EVENT_OBJECT_DELETE:		return _event_set(evt,	set ? AWAPI_event_object_delete		: NULL);
		case AW_EVENT_OBJECT_CLICK:			return _event_set(evt,	set ? AWAPI_event_object_click		: NULL);
		case AW_EVENT_OBJECT_SELECT:		return _event_set(evt,	set ? AWAPI_event_object_select		: NULL);

		case AW_EVENT_WORLD_ATTRIBUTES:		return _event_set(evt,	set ? AWAPI_event_world_attributes	: NULL);
		case AW_EVENT_WORLD_DISCONNECT:		return _event_set(evt,	set ? AWAPI_event_world_disconnect	: NULL);
		case AW_EVENT_TELEPORT:				return _event_set(evt,	set ? AWAPI_event_teleport			: NULL);

		case AW_EVENT_UNIVERSE_ATTRIBUTES:	return _event_set(evt,	set ? AWAPI_event_universe_attributes	: NULL);
		case AW_EVENT_UNIVERSE_DISCONNECT:	return _event_set(evt,	set ? AWAPI_event_universe_disconnect	: NULL);

		case AW_EVENT_CHAT:					return _event_set(evt,	set ? AWAPI_event_chat				: NULL);
		case AW_EVENT_CONSOLE_MESSAGE:		return _event_set(evt,	set ? AWAPI_event_console_message	: NULL);
		case AW_EVENT_BOTGRAM:				return _event_set(evt,	set ? AWAPI_event_botgram			: NULL);
		case AW_EVENT_URL_CLICK:			return _event_set(evt,	set ? AWAPI_event_url_click			: NULL);

		case AW_EVENT_CELL_BEGIN:			return _event_set(evt, set ? AWAPI_event_cell_begin			: NULL);
		case AW_EVENT_CELL_END:				return _event_set(evt, set ? AWAPI_event_cell_end			: NULL);
		case AW_EVENT_CELL_OBJECT:			return _event_set(evt, set ? AWAPI_event_cell_object		: NULL);

		case AW_EVENT_TERRAIN_BEGIN:		return _event_set(evt, set ? AWAPI_event_terrain_begin		: NULL);
		case AW_EVENT_TERRAIN_END:			return _event_set(evt, set ? AWAPI_event_terrain_end		: NULL);
		case AW_EVENT_TERRAIN_DATA:			return _event_set(evt, set ? AWAPI_event_terrain_data		: NULL);
		case AW_EVENT_TERRAIN_CHANGED:		return _event_set(evt, set ? AWAPI_event_terrain_changed	: NULL);
		
		case AW_EVENT_USER_INFO:			return _event_set(evt, set ? AWAPI_event_user_info			: NULL);
		case AW_EVENT_WORLD_INFO:			return _event_set(evt, set ? AWAPI_event_world_info			: NULL);
		
		case AW_EVENT_HUD_CLICK:			return _event_set(evt, set ? AWAPI_event_hud_click			: NULL);
		case AW_EVENT_HUD_CREATE:			return _event_set(evt, set ? AWAPI_event_hud_create			: NULL);
		case AW_EVENT_HUD_DESTROY:			return _event_set(evt, set ? AWAPI_event_hud_destroy		: NULL);
		case AW_EVENT_HUD_CLEAR:			return _event_set(evt, set ? AWAPI_event_hud_clear			: NULL);

		case AW_EVENT_ENTITY_ADD:			return _event_set(evt, set ? AWAPI_event_entity_add			: NULL);
		case AW_EVENT_ENTITY_CHANGE:		return _event_set(evt, set ? AWAPI_event_entity_change		: NULL);
		case AW_EVENT_ENTITY_DELETE:		return _event_set(evt, set ? AWAPI_event_entity_delete		: NULL);
		case AW_EVENT_ENTITY_LINKS:			return _event_set(evt, set ? AWAPI_event_entity_links		: NULL);
		case AW_EVENT_ENTITY_RIDER_ADD:		return _event_set(evt, set ? AWAPI_event_entity_rider_add	: NULL);
		case AW_EVENT_ENTITY_RIDER_CHANGE:	return _event_set(evt, set ? AWAPI_event_entity_rider_change: NULL);
		case AW_EVENT_ENTITY_RIDER_DELETE:	return _event_set(evt, set ? AWAPI_event_entity_rider_delete: NULL);
		};

		return NULL;
	}


	String
	Bot::GetReasonCode(int rc)
	{
		switch(rc) {
		case 0:		return "RC_SUCCESS";
		case 1:		return "RC_CITIZENSHIP_EXPIRED";
		case 2:		return "RC_LAND_LIMIT_EXCEEDED";
		case 3:		return "RC_NO_SUCH_CITIZEN";
		case 4:		return "RC_MESSAGE_LENGTH_BAD";
		case 5:		return "RC_LICENSE_PASSWORD_CONTAINS_SPACE";
		case 6:		return "RC_LICENSE_PASSWORD_TOO_LONG";
		case 7:		return "RC_LICENSE_PASSWORD_TOO_SHORT";
		case 8:		return "RC_LICENSE_RANGE_TOO_LARGE";
		case 9:		return "RC_LICENSE_RANGE_TOO_SMALL";
		case 10:	return "RC_LICENSE_USERS_TOO_LARGE";
		case 11:	return "RC_LICENSE_USERS_TOO_SMALL";
		case 12:	return "RC_LICENSE_CONTAINS_INVALID_CHAR";
		case 13:	return "RC_INVALID_PASSWORD";
		case 14:	return "RC_UNABLE_TO_MAIL_BACK_NUMBER";
		case 15:	return "RC_LICENSE_WORLD_TOO_SHORT";
		case 16:	return "RC_LICENSE_WORLD_TOO_LONG";
		case 17:	return "RC_SERVER_OUT_OF_MEMORY";
		case 20:	return "RC_INVALID_WORLD";
		case 21:	return "RC_SERVER_OUTDATED";
		case 22:	return "RC_WORLD_ALREADY_STARTED";
		case 27:	return "RC_NO_SUCH_WORLD";
		case 31:	return "RC_NOT_LOGGED_IN";
		case 32:	return "RC_UNAUTHORIZED";
		case 33:	return "RC_WORLD_ALREADY_EXISTS";
		case 34:	return "RC_NO_SUCH_LICENSE";
		case 39:	return "RC_IDENTITY_ALREADY_IN_USE";
		case 40:	return "RC_UNABLE_TO_REPORT_LOCATION";
		case 41:	return "RC_INVALID_EMAIL";
		case 42:	return "RC_NO_SUCH_ACTING_CITIZEN";
		case 43:	return "RC_ACTING_PASSWORD_INVALID";
		case 45:	return "RC_UNIVERSE_FULL";
		case 46:	return "RC_BILLING_TIMEOUT";
		case 47:	return "RC_BILLING_RECV_FAILED";
		case 48:	return "RC_BILLING_RESPONSE_INVALID";
		case 55:	return "RC_BILLING_REJECTED";
		case 56:	return "RC_BILLING_BLOCKED";
		case 57:	return "RC_TOO_MANY_WORLDS";
		case 58:	return "RC_MUST_UPGRADE";
		case 59:	return "RC_BOT_LIMIT_EXCEEDED";
		case 61:	return "RC_WORLD_EXPIRED";
		case 62:	return "RC_CITIZEN_DOES_NOT_EXPIRE";
		case 64:	return "RC_LICENSE_STARTS_WITH_NUMBER";
		case 66:	return "RC_NO_SUCH_EJECTION";
		case 67:	return "RC_NO_SUCH_SESSION";
		case 69:	return "RC_EJECTION_EXPIRED";
		case 70:	return "RC_ACTING_CITIZEN_EXPIRED";
		case 71:	return "RC_ALREADY_STARTED";
		case 72:	return "RC_WORLD_RUNNING";
		case 73:	return "RC_WORLD_NOT_SET";
		case 74:	return "RC_NO_SUCH_CELL";
		case 75:	return "RC_NO_REGISTRY";
		case 76:	return "RC_CANT_OPEN_REGISTRY";
		case 77:	return "RC_CITIZEN_DISABLED";
		case 78:	return "RC_WORLD_DISABLED";
		case 79:	return "RC_BETA_REQUIRED";
		case 80:	return "RC_ACTING_CITIZEN_DISABLED";
		case 81:	return "RC_INVALID_USER_COUNT";
		case 91:	return "RC_PRIVATE_WORLD";
		case 92:	return "RC_NO_TOURISTS";
		case 100:	return "RC_EMAIL_CONTAINS_INVALID_CHAR";
		case 101:	return "RC_EMAIL_ENDS_WITH_BLANK";
		case 102:	return "RC_EMAIL_MISSING_DOT";
		case 103:	return "RC_EMAIL_MISSING_AT";
		case 104:	return "RC_EMAIL_STARTS_WITH_BLANK";
		case 105:	return "RC_EMAIL_TOO_LONG";
		case 106:	return "RC_EMAIL_TOO_SHORT";
		case 107:	return "RC_NAME_ALREADY_USED";
		case 108:	return "RC_NAME_CONTAINS_NONALPHANUMERIC_CHAR";
		case 109:	return "RC_NAME_CONTAINS_INVALID_BLANK";
		case 110:	return "RC_NAME_DOESNT_EXIST";
		case 111:	return "RC_NAME_ENDS_WITH_BLANK";
		case 112:	return "RC_NAME_TOO_LONG";
		case 113:	return "RC_NAME_TOO_SHORT";
		case 114:	return "RC_NAME_UNUSED";
		case 115:	return "RC_PASSWORD_TOO_LONG";
		case 116:	return "RC_PASSWORD_TOO_SHORT";
		case 117:	return "RC_PASSWORD_WRONG";
		case 119:	return "RC_UNABLE_TO_DELETE_NAME";
		case 120:	return "RC_UNABLE_TO_getCITIZEN";
		case 121:	return "RC_UNABLE_TO_INSERT_CITIZEN";
		case 122:	return "RC_UNABLE_TO_INSERT_NAME";
		case 123:	return "RC_UNABLE_TO_PUT_CITIZEN_COUNT";
		case 124:	return "RC_UNABLE_TO_DELETE_CITIZEN";
		case 126:	return "RC_NUMBER_ALREADY_USED";
		case 127:	return "RC_NUMBER_OUT_OF_RANGE";
		case 128:	return "RC_PRIVILEGE_PASSWORD_IS_TOO_SHORT";
		case 129:	return "RC_PRIVILEGE_PASSWORD_IS_TOO_LONG";
		case 203:	return "RC_NOT_CHANGE_OWNER";
		case 204:	return "RC_CANT_FIND_OLD_ELEMENT";
		case 210:	return "RC_UNABLE_TO_CHANGE_ATTRIBUTE";
		case 211:	return "RC_CANT_CHANGE_OWNER";
		case 212:	return "RC_IMPOSTER";
		case 213:	return "RC_INVALID_REQUEST";
		case 216:	return "RC_CANT_BUILD_HERE";
		case 300:	return "RC_ENCROACHES";
		case 301:	return "RC_NO_SUCH_OBJECT";
		case 302:	return "RC_NOT_DELETE_OWNER";
		case 303:	return "RC_TOO_MANY_BYTES";
		case 305:	return "RC_UNABLE_TO_STORE";
		case 306:	return "RC_UNREGISTERED_OBJECT";
		case 308:	return "RC_ELEMENT_ALREADY_EXISTS";
		case 309:	return "RC_RESTRICTED_COMMAND";
		case 310:	return "RC_NO_BUILD_RIGHTS";
		case 311:	return "RC_OUT_OF_BOUNDS";
		case 313:	return "RC_RESTRICTED_OBJECT";
		case 314:	return "RC_RESTRICTED_AREA";
		case 400:	return "RC_OUT_OF_MEMORY";
		case 401:	return "RC_NOT_YET";
		case 402:	return "RC_TIMEOUT";
		case 403:	return "RC_NULL_POINTER";
		case 404:	return "RC_UNABLE_TO_CONTACT_UNIVERSE";
		case 405:	return "RC_UNABLE_TO_CONTACT_WORLD";
		case 406:	return "RC_INVALID_WORLD_NAME";
		case 415:	return "RC_SEND_FAILED";
		case 416:	return "RC_RECEIVE_FAILED";
		case 421:	return "RC_STREAM_EMPTY";
		case 422:	return "RC_STREAM_MESSAGE_TOO_LONG";
		case 423:	return "RC_WORLD_NAME_TOO_LONG";
		case 426:	return "RC_MESSAGE_TOO_LONG";
		case 427:	return "RC_TOO_MANY_RESETS";
		case 428:	return "RC_UNABLE_TO_CREATE_SOCKET";
		case 429:	return "RC_UNABLE_TO_CONNECT";
		case 430:	return "RC_UNABLE_TO_SET_NONBLOCKING";
		case 434:	return "RC_CANT_OPEN_STREAM";
		case 435:	return "RC_CANT_WRITE_STREAM";
		case 436:	return "RC_CANT_CLOSE_STREAM";
		case 439:	return "RC_NO_CONNECTION";
		case 442:	return "RC_UNABLE_TO_INITIALIZE_NETWORK";
		case 443:	return "RC_INCORRECT_MESSAGE_LENGTH";
		case 444:	return "RC_NOT_INITIALIZED";
		case 445:	return "RC_NO_INSTANCE";
		case 446:	return "RC_OUT_BUFFER_FULL";
		case 447:	return "RC_INVALID_CALLBACK";
		case 448:	return "RC_INVALID_ATTRIBUTE";
		case 449:	return "RC_TYPE_MISMATCH";
		case 450:	return "RC_STRING_TOO_LONG";
		case 451:	return "RC_READ_ONLY";
		case 452:	return "RC_UNABLE_TO_REGISTER_RESOLVE";
		case 453:	return "RC_INVALID_INSTANCE";
		case 454:	return "RC_VERSION_MISMATCH";
		case 461:	return "RC_IN_BUFFER_FULL";
		case 463:	return "RC_PROTOCOL_ERROR";
		case 464:	return "RC_QUERY_IN_PROGRESS";
		case 465:	return "RC_WORLD_FULL";
		case 466:	return "RC_EJECTED";
		case 467:	return "RC_NOT_WELCOME";
		case 468:	return "RC_UNABLE_TO_BIND";
		case 469:	return "RC_UNABLE_TO_LISTEN";
		case 470:	return "RC_UNABLE_TO_ACCEPT";
		case 471:	return "RC_CONNECTION_LOST";
		case 473:	return "RC_NO_STREAM";
		case 474:	return "RC_NOT_AVAILABLE";
		case 487:	return "RC_OLD_UNIVERSE";
		case 488:	return "RC_OLD_WORLD";
		case 489:	return "RC_WORLD_NOT_RUNNING";
		case 505:	return "RC_INVALID_ARGUMENT";
		default:	return "RC_UNKNOWN_ERROR";
		};
	}

	int	Bot::_universe_attributes_change (void) {
		INT_BASE_CHECK()
		return aw_universe_attributes_change();
	}

	int	Bot::UniAttributes_ChangeBrowserRange(int min, int release, int beta) {
		_int_set(AW_UNIVERSE_BROWSER_MINIMUM,		min);
		_int_set(AW_UNIVERSE_BROWSER_RELEASE,		release);
		_int_set(AW_UNIVERSE_BROWSER_BETA,			beta);
		return _universe_attributes_change();
	}

	int	Bot::UniAttributes_ChangeWorldRange(int min, int release, int beta) {
		_int_set(AW_UNIVERSE_WORLD_MINIMUM,			min);
		_int_set(AW_UNIVERSE_WORLD_RELEASE,			release);
		_int_set(AW_UNIVERSE_WORLD_BETA,			beta);
		return _universe_attributes_change();
	}

	int	Bot::UniAttributes_ChangeIO(LPCSTR new_io) {
		_string_set(AW_UNIVERSE_WELCOME_MESSAGE, new_io);
		return _universe_attributes_change();
	}

	void Bot::AWAPI_event_universe_attributes()
	{
		/* get our pointer and check it */
		Bot* pBot = bots.getCurInstance();
		if (pBot == NULL)
			return;

		/* dispatch to relevant event */
		pBot->onEventUniverseAttributes();
	}

	void Bot::OnSys_UniverseAttributes()
	{
		/* signal world events */
		onEventUniverseAttributes();

		/* is the world immigration officer changed? */
		if (m_universe_io != _string(AW_UNIVERSE_WELCOME_MESSAGE)) {
			m_universe_io = _string(AW_UNIVERSE_WELCOME_MESSAGE);
			onEventUniverseIOChange();
		}
	}

	int	Bot::_citizen_add(void) {
		INT_BASE_CHECK()
		return aw_citizen_add();
	}

	int Bot::_citizen_change(void) {
		INT_BASE_CHECK()
		return aw_citizen_change();
	}

	int	Bot::_citizen_delete(int citizen_id) {
		INT_BASE_CHECK()
		return aw_citizen_delete(citizen_id);
	}

	int	Bot::_citizen_next	(void) {
		INT_BASE_CHECK()
		return aw_citizen_next();
	}

	int	Bot::_citizen_previous(void) {
		INT_BASE_CHECK()
		return aw_citizen_previous();
	}

	int	Bot::_citizen_attributes_by_number(int citizen) {
		INT_BASE_CHECK()
		return aw_citizen_attributes_by_number(citizen);
	}

	int	Bot::_citizen_attributes_by_name(LPCSTR name) {
		INT_BASE_CHECK()
		return aw_citizen_attributes_by_name(name);
	}

	int	Bot::CitizenAdd(LPCSTR name, LPCSTR password, LPCSTR email, BOOL beta)
	{
		INT_BASE_CHECK()
		_int_set	(AW_CITIZEN_NUMBER,		NULL);
		_string_set (AW_CITIZEN_NAME,		name);
		_string_set (AW_CITIZEN_PASSWORD,	password);
		_string_set (AW_CITIZEN_EMAIL,		email);
		_bool_set	(AW_CITIZEN_BETA,		beta);
		_string_set	(AW_CITIZEN_PRIVILEGE_PASSWORD,		"");
		_string_set	(AW_CITIZEN_URL,		"");
		return _citizen_add();
	}

	int	Bot::CitizenAdd(const Citizen& CitInfo)
	{
		return CitizenAdd(CitInfo.getName(), CitInfo.getPassword(), CitInfo.getEmail(), CitInfo.getBeta());
	}

	int	Bot::CitizenChange(int number, LPCSTR name, LPCSTR password, LPCSTR privilege, LPCSTR email, LPCSTR url, LPCSTR comment, BOOL beta, BOOL enabled, int bot_limit, int expires) 
	{
		INT_BASE_CHECK()
		_bool_set	(AW_CITIZEN_BETA,				beta);
		_int_set	(AW_CITIZEN_BOT_LIMIT,			bot_limit);
		_string_set	(AW_CITIZEN_COMMENT,			comment);
		_string_set	(AW_CITIZEN_EMAIL,				email);
		_bool_set	(AW_CITIZEN_ENABLED,			enabled);
		_int_set	(AW_CITIZEN_EXPIRATION_TIME,	expires);
		_string_set	(AW_CITIZEN_NAME,				name);
		_string_set	(AW_CITIZEN_PASSWORD,			password);
		_string_set	(AW_CITIZEN_PRIVILEGE_PASSWORD,	privilege);
		_string_set	(AW_CITIZEN_URL,				url);
		return _citizen_change();
	}

	int Bot::CitizenChange(const Citizen &cit) 
	{
		return CitizenChange(
			cit.getCitizen(), 
			cit.getName(), 
			cit.getPassword(), 
			cit.getPrivilegePassword(), 
			cit.getEmail(), 
			cit.getUrl(), 
			cit.getComment(),
			cit.getBeta(), 
			cit.getEnabled(), 
			cit.getBotLimit(), 
			cit.getExpirationTime());
	}

	int Bot::_user_list(void) 
	{
		INT_BASE_CHECK();
		return aw_user_list();
	}

	int Bot::_world_list(void) {
		INT_BASE_CHECK();
		return aw_world_list();
	}

	int	Bot::setUniverseConnection(int state) 
	{
		int prev = m_universe_connection;
		m_universe_connection = state;
		onEventUniverseConnectionChange(state);
		
		// link states
		if (prev == ConState::Connected && state != ConState::Connected)
			onEventUniverseLinkClosed();
		else if (prev != ConState::Connected && state == ConState::Connected)
			onEventUniverseLinkOpened();

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// creating and destroying the instance 
	int	Bot::_create(LPCSTR universe_host, int universe_port) {
		int rc = aw_create(universe_host, universe_port, &m_instance);
		aw_user_data_set(this);
		return rc;
	}

	int	Bot::_login(void) {
		INT_BASE_CHECK();
		return aw_login();
	}

	int Bot::_destroy(void) {
		INT_BASE_CHECK();
		return aw_destroy();
	}

	int Bot::_traffic_count(int& in, int& out) {
	#if AW_BUILD > 54
		INT_BASE_CHECK();
		return aw_traffic_count(&in, &out);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int Bot::Create(LPCSTR universe_host, int universe_port, LPCSTR universe_name, BOOL uses_callback) {
		/* check initialised state */
		if (getInitialised() == FALSE)
			return RC_NOT_INITIALIZED;
		m_instance = 0;
		m_universe_name = universe_name;
		/* set state to be connection */
		setUniverseConnection(ConState::Creating);
		/* create the instance ptr */
		int rc = _create(universe_host, universe_port);
		aw_instance_set(m_instance);
		
		/* are we initialised or what? */
		setUniverseConnection(rc == RC_SUCCESS ? ConState::Created : ConState::Nothing);
		/* return given code */
		return rc;
	}

	int Bot::Login(int citno, LPCSTR password, LPCSTR name, LPCSTR application, BOOL uses_callback) {
		/* check initialised state */
		if (getInitialised() == FALSE)
			return RC_NOT_INITIALIZED;
		/* check if we are awaiting logging in from creation yet */
		if (getUniverseConnection() != ConState::Created)
			return RC_INVALID_STATE;
		/* swap to this instance */
		if (SelectInstance() == FALSE)
			return RC_NO_INSTANCE;
		/* bring all of our information into the SDK */
		int _rc = 0;
		_rc+= _int_set		(AW_LOGIN_OWNER,				citno);
		_rc+= _string_set	(AW_LOGIN_PRIVILEGE_PASSWORD,	password);
		_rc+= _string_set	(AW_LOGIN_APPLICATION,			application);
		_rc+= _string_set	(AW_LOGIN_NAME,					name);
		/* set all of our local variables */
		m_name				= name;
		m_login_citizen		= citno;
		m_login_password	= password;
		m_login_application	= application;
		/* set connection state */
		setUniverseConnection( ConState::Connecting );

		/* trigger the callback */
		SelectInstance();

		int rc = aw_login();
		/* did we have an error connecting? */
		if (rc == RC_SUCCESS) 
		{
			if (uses_callback == TRUE)
				return RC_SUCCESS;
			else {
				onCallbackLogin(rc);
			}
			return RC_SUCCESS;
		} else {
			setUniverseConnection( ConState::Nothing );
			return rc;
		}
	}

	int	Bot::Destroy() {
		/* check initialised state */
		INT_BASE_CHECK()
		/* terminate our world connection if present */
		if (getWorldConnection() != ConState::Nothing)
			ExitWorld();
		/* destroy this instance */
		int rc = aw_destroy();
		setUniverseConnection ( ConState::Nothing );
		/* set instance ptr to null */
		m_instance = NULL;
		/* return given code */
		return rc;
	}

	//////////////////////////////////////////////////////////////////////////
	// callbacks for the login
	void Bot::AWAPI_callback_login(int rc) {
		DEFINE_BASE_BOT()
		/* dispatch to relevant event */
		pBot->OnSys_CallbackLogin(rc);
	}

	void Bot::onCallbackCreate(int rc)
	{
		/* get this instance */
		if (SelectInstance() == FALSE)
			return;

		/* permit response */
		setUniverseConnection(rc == RC_SUCCESS ? ConState::Created : ConState::Nothing);
	}

	void Bot::OnSys_CallbackLogin(int rc) {
		if (rc == RC_SUCCESS) {
			if (getUniverseConnection() == ConState::Recovering) {
				setUniverseConnection( ConState::Connected );
				onEventUniverseRecover();
				return;
			}
			else
				onEventLogin();
		} else {
			setUniverseConnection( ConState::Nothing );
			onEventLoginFailed(rc);
		}
		/* call main callback */
		onCallbackLogin(rc);
		/* if automatic login is enabled we carry out our callback here */
		if (m_al_enabled)
			OnALEvent_CallbackLogin( rc );
	}

	int	Bot::AL_Execute(const char* uni_host, int uni_port, const char* uni_name,
				int login_citizen, const char* login_password, const char* login_name,
				  const char*world_name, const char* world_coordinates,
				    bool world_global, bool world_invisible)
	{
		/* create the connection */
		int rc = Create(uni_host, uni_port, uni_name, FALSE);
		/* check the success value */
		if ( rc != RC_SUCCESS) {
			OnALEvent_Fail( rc );
		}

		/* variables employed */
		String log;
		/* create debug string */
		log.Format("Auto-Login: uni(%s:%d) cit(%d) world(%s - %s)",
			uni_host, uni_port, login_citizen, world_name, world_coordinates);
		/* copy variables over */
		m_al_uni_host			= uni_host;
		m_al_uni_name			= uni_name;
		m_al_uni_port			= uni_port;
		m_al_citizen			= login_citizen;
		m_al_password			= login_password;
		m_al_name				= login_name;
		m_al_world				= world_name;
		m_al_coordinates		= world_coordinates;
		m_al_global				= world_global;
		m_al_invisible			= world_invisible;
		m_al_enabled			= TRUE;

		/* login */
		return Login(m_al_citizen, m_al_password, m_al_name, "XPlane XOrion (C++ SDK)", TRUE);
	}

	void Bot::OnALEvent_CallbackLogin( int rc ) {
		/* variables */
		String log;
		/* do relevant actions for each */
		if (rc != RC_SUCCESS) {
			OnALEvent_Fail(rc);
			return;
		} else
			Login(m_al_citizen, m_al_password, m_al_name, "CAsyncBot", TRUE);
	}

	void Bot::OnALEvent_CallbackEnter( int rc )
	{
	}

	void Bot::AWAPI_event_universe_disconnect() {
		DEFINE_BASE_BOT()
		/* dispatch to relevant event */
		pBot->OnSys_UniverseDisconnect();
	}

	void Bot::OnSys_UniverseDisconnect() {
		/* check if we are just experiencing a repeat event */
		if (getUniverseConnection() == ConState::Recovering)
			return;
		/* set our universe reason */
		if (_int(AW_DISCONNECT_REASON) == RC_EJECTED)
			setUniverseConnection(ConState::Nothing);
		else
			setUniverseConnection(ConState::Recovering);
		/* dispatch to relevant event */
		onEventUniverseDisconnect(_int(AW_DISCONNECT_REASON));
	}

	int	Bot::_universe_ejection_add(void){
		INT_BASE_CHECK()
		return aw_universe_ejection_add();
	}

	int	Bot::_universe_ejection_delete(int address_id){
		INT_BASE_CHECK()
		return aw_universe_ejection_delete(address_id);
	}

	int	Bot::_universe_ejection_lookup(void){
		INT_BASE_CHECK()
		return aw_universe_ejection_lookup();
	}

	int	Bot::_universe_ejection_next(void){
		INT_BASE_CHECK()
		return aw_universe_ejection_next();
	}

	int	Bot::_universe_ejection_previous(void){
		INT_BASE_CHECK()
		return aw_universe_ejection_previous();
	}

	int	Bot::_license_add(void) {
		INT_BASE_CHECK()
		return 0; //aw_licence_add();
	}

	int	Bot::_license_attributes(LPCSTR name) {
		INT_BASE_CHECK()
		return 0; //aw_licence_attributes(name);
	}

	int	Bot::_license_change(void) {
		INT_BASE_CHECK()
		return 0; //aw_licence_change();
	}

	int	Bot::_license_next(void) {
		INT_BASE_CHECK()
		return 0; //aw_licence_next();
	}

	int	Bot::_license_delete(LPCSTR name) {
		INT_BASE_CHECK()
		return 0; //aw_licence_delete(name);
	}

	int	Bot::_license_previous(void) {
		INT_BASE_CHECK()
		return 0; //aw_licence_previous();
	}

	int	Bot::_world_attributes_change(void) {
		INT_BASE_CHECK();
		return aw_world_attributes_change();
	}

	int	Bot::_world_attribute_set(int attribute, char* value) {
		INT_BASE_CHECK();
		return aw_world_attribute_set(attribute, value);
	}

	int	Bot::_world_attribute_get(int attribute, int* read_only, char* value) {
		INT_BASE_CHECK();
		return aw_world_attribute_get(attribute, read_only, value);
	}

	int	Bot::_world_attributes_send(int Session)
	{
#if (AW_BUILD > 50)
		INT_BASE_CHECK();
		return aw_world_attributes_send(Session);
#else
		return RC_NOT_AVAILABLE;
#endif
	}

	/*****************************************************************************
	**                                                                          **
	**                                                                          **
	**                        WORLD ENTER AND EXIT ROUTINES                     **
	**                                Enter / Exit                              **
	**                                                                          **
	**                                                                          **
	*****************************************************************************/
	bool Bot::HasEject(void) const {	
		return _bool(AW_WORLD_EJECT_CAPABILITY) == TRUE || HasCT();
	}

	bool Bot::HasPS(void) const {	
		return _bool(AW_WORLD_PUBLIC_SPEAKER_CAPABILITY) == TRUE;	
	}

	bool Bot::HasCT(void) const {	
		return _bool(AW_WORLD_CARETAKER_CAPABILITY) == TRUE;		
	}

	bool Bot::HasRoot(void) const 
	{	int my_citnum = _int(AW_LOGIN_PRIVILEGE_NUMBER); 
		return this->m_login_citizen == ROOT_CITIZEN;		
	}

	bool Bot::HasUniUserList(void) const
	{
	#if AW_BUILD > 54
		return (_bool(AW_UNIVERSE_USER_LIST_ENABLED) == TRUE);
	#else
		return TRUE;
	#endif
	}

	int	Bot::_camera_set(int session_id)
	{
	#if AW_BUILD > 54
		INT_BASE_CHECK();
		return aw_camera_set(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int	Bot::CameraFaceObject(int session_id, LPCSTR object_name)
	{ 
	#if AW_BUILD > 54
		return _camera_set(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int Bot::CameraObjectTracksSession(int session_id, LPCSTR object, int tracking_session)
	{
	#if AW_BUILD > 54
		/* from the object*/
		_int_set	(AW_CAMERA_LOCATION_TYPE,		AW_CAMERA_TRACK_OBJECT);
		_string_set	(AW_CAMERA_LOCATION_OBJECT,		object);

		/* track the avatar */
		_int_set	(AW_CAMERA_TARGET_TYPE,			AW_CAMERA_TRACK_AVATAR);
		_int_set	(AW_CAMERA_TARGET_SESSION,		tracking_session);

		/* set the camera */
		return _camera_set(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int	Bot::CameraSessionTracksObject(int session_id, LPCSTR tracking_object, int tracking_session)
	{
	#if AW_BUILD > 54
		/* track the object */
		_int_set	(AW_CAMERA_TARGET_TYPE,			AW_CAMERA_TRACK_OBJECT);
		_string_set	(AW_CAMERA_TARGET_OBJECT,		tracking_object);

		/* from the avatar */
		_int_set	(AW_CAMERA_LOCATION_TYPE,		AW_CAMERA_TRACK_AVATAR);
		_int_set	(AW_CAMERA_LOCATION_SESSION,	tracking_session);

		/* set the camera */
		return _camera_set(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int	Bot::CameraFaceObject(int session_id, LPCSTR object_from, LPCSTR object_to)
	{
	#if AW_BUILD > 54
		return _camera_set(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int	Bot::CameraFaceSession(int session_id, int targetid)
	{
	#if AW_BUILD > 54
		_bool_set (AW_AVATAR_LOCK, FALSE);
		_int_set(AW_CAMERA_LOCATION_TYPE,	 AW_CAMERA_TRACK_DEFAULT);
		_int_set(AW_CAMERA_TARGET_TYPE,      AW_CAMERA_TRACK_AVATAR);
		_int_set(AW_CAMERA_TARGET_SESSION,   targetid);
		return _camera_set(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int	Bot::CameraFirstPerson(int session_id)
	{
	#if AW_BUILD > 54
		return _camera_set(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int	Bot::_console_message(int session) {
		INT_BASE_CHECK();
		return aw_console_message(session);
	}
	
	int	Bot::_say(LPCSTR message) 
	{
		INT_BASE_CHECK();
		String safe_msg = message;
		return aw_say(safe_msg.Left(AW_MAX_ATTRIBUTE_LENGTH));
	}
	
	int Bot::_whisper(int Session, const char* Message)
	{
		INT_BASE_CHECK();
		return aw_whisper(Session, Message);
	}

	int	Bot::Say(LPCSTR msg) 
	{
		/* check initialised state */
		INT_BASE_CHECK();
		return aw_say(msg);
	}
	
	int Bot::_noise(int session_id) 
	{
		INT_BASE_CHECK();
		/* return real result */
	#if AW_BUILD > 51
		return aw_noise(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int	Bot::Botgram(int citizen, LPCSTR message) 
	{
		/* check initialised state */
		INT_BASE_CHECK();
		// set the sdk variables up
		_int_set	(AW_BOTGRAM_TO,		citizen);
		_string_set	(AW_BOTGRAM_TEXT,	message);
		// return code
		return aw_botgram_send();
	}

	int	Bot::Whipser(int session, LPCSTR msg) 
	{
		/* check initialised state */
		INT_BASE_CHECK();
		return aw_whisper(session, msg);
	}

	int Bot::ConsoleMessage(int session, LPCSTR msg, COLORREF colour, int Style)
	{
		/* check initialised state */
		INT_BASE_CHECK();
		/* check authoriation */
		if (HasCT() == false)
			return RC_UNAUTHORIZED;
		/* set up our attributes */
		_string_set	(AW_CONSOLE_MESSAGE,	msg);
		_int_set	(AW_CONSOLE_RED,		GetRValue(colour));
		_int_set	(AW_CONSOLE_GREEN,		GetGValue(colour));
		_int_set	(AW_CONSOLE_BLUE,		GetBValue(colour));
		_bool_set	(AW_CONSOLE_BOLD,		(Style & CM_BOLD) != 0);
		_bool_set	(AW_CONSOLE_ITALICS,	(Style & CM_ITALIC) != 0);
		/* send back message */
		return _console_message(session);
	}

	int	Bot::SendNoise(int session_id, LPCSTR sound) 
	{
	#if AW_BUILD > 54
		_string_set(AW_SOUND_NAME, sound);
		return _noise(session_id);
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	int	Bot::_enter(LPCSTR world) {
		BOOL si = SelectInstance();
		return aw_enter(world);
	}

	int	Bot::_exit() {
		INT_BASE_CHECK();
		return aw_exit();
	}

	int	Bot::setWorldConnection(int state) 
	{
		int prev = m_world_connection;	
		m_world_connection = state;
		onEventWorldConnectionChange(state);

		// link states
		if (prev == ConState::Connected && state != ConState::Connected)
			onEventWorldLinkClosed();
		else if (prev != ConState::Connected && state == ConState::Connected)
			onEventWorldLinkOpened();

		return TRUE;
	}

	int Bot::EnterWorld(LPCSTR world, BOOL global, BOOL uses_callback) {
		/* check initialised state */
		INT_BASE_CHECK();
		/* check if we are awaiting logging in from creation yet */
		if (getUniverseConnection() != ConState::Connected)
			return RC_INVALID_STATE;
		/* set global type */
		_bool_set			( AW_ENTER_GLOBAL,	global );
		SetGlobalState		( global ? GlobalState::Requested : GlobalState::Off );
		setWorldConnection	( ConState::Connecting );
		setInvisible		( TRUE );
		/* call the function */
		void* pDum = m_instance;

		int rc = _enter(world);
		/* decide what to do in terms of callback */
		if (uses_callback == FALSE) {
			if (rc == 0)	setWorldConnection( ConState::Connected );
			else			setWorldConnection( ConState::Nothing   );
		}
		return rc;
	}

	void Bot::OnSys_CallbackEnter(int rc) {
		/* check if it is a 4.1 reconnection */
		if (getWorldConnection() == ConState::Recovering) {
			if (rc != 0) {
				setWorldConnection(ConState::Connected);
				SetGlobalState(_bool(AW_ENTER_GLOBAL) ? GlobalState::Enabled : GlobalState::Off);
				onEventWorldRecover();
			} else
				onEventWorldRecoverFailed(rc);
			return;
		}
		if (getWorldConnection() == ConState::Connected)
			return;

		/* trigger the world connection update */
		if (rc == RC_SUCCESS)	setWorldConnection( ConState::Connected );
		else					setWorldConnection( ConState::Nothing );
		/* cause main enter */
		SetGlobalState(HasCT() ? GlobalState::Enabled : GlobalState::Off);
		/* trigger main enter */
		onCallbackEnter(rc);
		/* if automatic login is enabled we carry out our callback here */
		if (m_al_enabled)
			OnALEvent_CallbackEnter( rc );
	}

	int Bot::ExitWorld() {
		/* check initialised state - if they close it before it returns */
		INT_BASE_CHECK();
		/* check if we are awaiting logging in from creation yet */
		if (getWorldConnection() == ConState::Nothing)
			return RC_SUCCESS;
		/* exit as required */
		int rc = aw_exit();
		/* set connection to offline */
		setWorldConnection( ConState::Nothing );
		return rc;
	}
	void Bot::AWAPI_event_world_attributes() {
		DEFINE_BASE_BOT();
		/* dispatch to relevant event */
		pBot->OnSys_WorldAttributes();
	}

	void	
	Bot::OnSys_WorldAttributes() {
		/* world behavior of 4.1 is different to 3.6, this not triggered on reconnect */
	//#if (AW_BUILD < 51)
		/* is this the signal that we have reconnected? */
		if (getWorldConnection() == ConState::Recovering) {
			setWorldConnection(ConState::Connected);
			onEventWorldRecover();
		}
	//#endif
		/* signal world events */
		onEventWorldAttributes();
		/* is the world immigration officer changed? */
		if (m_world_io != aw_string(AW_WORLD_WELCOME_MESSAGE)) {
			m_world_io = aw_string(AW_WORLD_WELCOME_MESSAGE);
			onEventWorldIOChange();
		}
	}

	void Bot::OnSys_WorldDisconnect(int reason) {
		/* this is triggered by the main caller routine, we handle which is which */
		reason = _int(AW_DISCONNECT_REASON);
		if (reason == RC_EJECTED) {
			setWorldConnection( ConState::Nothing );
			onEventWorldEjected();
		} else if (reason == RC_NOT_WELCOME) {
			setWorldConnection( ConState::Nothing );
		} else {
			setWorldConnection( ConState::Recovering );
		}
		/* trigger main reason */
		onEventWorldDisconnect(reason);
	}

	void Bot::AWAPI_event_world_disconnect() {
		DEFINE_BASE_BOT()
		/* dispatch to relevant event */
		int reason = aw_int(AW_DISCONNECT_REASON);
		pBot->OnSys_WorldDisconnect(pBot->_int(AW_DISCONNECT_REASON));
	}

	int Bot::_avatar_click(int session) {
		INT_BASE_CHECK()
		return aw_avatar_click(session);
	}

	int	Bot::_url_send	(int session_id, LPCSTR url, LPCSTR target) {
		INT_BASE_CHECK()
		return aw_url_send(session_id, url, target);
	}

	int Bot::_url_click(LPCSTR url) {
		INT_BASE_CHECK()
		return aw_url_click(url);
	}

	int Bot::_address(int session_id) {
		INT_BASE_CHECK()
		return aw_address(session_id);
	}

	int	Bot::_eject(void) {
		INT_BASE_CHECK()
		return aw_world_eject();
	}

	int	 Bot::Eject(int session, int duration) {
		INT_BASE_CHECK();
		_int_set(AW_EJECT_DURATION, duration);
		_int_set(AW_EJECT_SESSION,	session);
		return aw_world_eject();
	}

	int Bot::_teleport(int session_id) {
		INT_BASE_CHECK();
		return aw_teleport(session_id);
	}

	int	Bot::_avatar_set(int session_id) {
		INT_BASE_CHECK();
		return aw_avatar_set(session_id);
	}

	int	Bot::TeleportUser(int session,	int x, int y, int z, int yaw, CStringA world, bool warp) 
	{
		if (world.GetLength() == 0)
			world = _string(AW_WORLD_NAME);
			
		_int_set(AW_TELEPORT_X,		x);			_int_set(AW_TELEPORT_Y,		y);
		_int_set(AW_TELEPORT_Z,		z);			_int_set(AW_TELEPORT_YAW,	yaw);
		_bool_set(AW_TELEPORT_WARP, bB(warp));
		_string_set(AW_TELEPORT_WORLD, world);
		return _teleport(session);
	}

	int	Bot::TeleportUser(int session,	Location& pt, LPCSTR world, bool warp) {
		return TeleportUser(session, pt.getX(), pt.getY(), pt.getZ(), pt.getYAW(), world, warp);
	}




	int	Bot::_mover_set_state(int id, int state, int model_num)
	{
		INT_BASE_CHECK();
		return aw_mover_set_state(id, state, model_num);
	}

	int	Bot::_mover_set_position(int id, int x, int y, int z, int yaw, int pitch, int roll)
	{
		INT_BASE_CHECK();
		return aw_mover_set_position(id, x, y, z, yaw, pitch, roll);
	}

	int	Bot::_mover_rider_add(int id, int session, int dist, int angle, int y_delta, int yaw_delta, int pitch_delta)
	{
		INT_BASE_CHECK();
		return aw_mover_rider_add(id, session, dist, angle, y_delta, yaw_delta, pitch_delta);
	}

	int	Bot::_mover_rider_change(int id, int session, int dist, int angle, int y_delta, int yaw_delta, int pitch_delta)
	{
		INT_BASE_CHECK();
		return 0; // aw_mover_rider_change(id, session, dist, angle, y_delta, yaw_delta, pitch_delta);
	}

	int	Bot::_mover_rider_delete(int id, int session)
	{
		INT_BASE_CHECK();
		return aw_mover_rider_delete(id, session);
	}

	int Bot::_mover_links(int id)
	{
		INT_BASE_CHECK();
		return aw_mover_links(id);
	}





	int	Bot::_world_eject(void){
		INT_BASE_CHECK()
		return aw_world_eject();
	}

	int	Bot::_world_ejection_add(void){
		INT_BASE_CHECK()
		return aw_world_ejection_add();
	}

	int	Bot::_world_ejection_delete(void){
		INT_BASE_CHECK()
		return aw_world_ejection_delete();
	}

	int	Bot::_world_ejection_lookup(void){
		INT_BASE_CHECK()
		return aw_world_ejection_lookup();
	}

	int	Bot::_world_ejection_next(void){
		INT_BASE_CHECK()
		return aw_world_ejection_next();
	}

	int	Bot::_world_ejection_previous(void){
		INT_BASE_CHECK()
		return aw_world_ejection_previous();
	}

	int	Bot::WorldEjection_DeleteByType(int code_id, int type_id){
		INT_BASE_CHECK();
		_int_set(AW_EJECTION_TYPE,		type_id);
		_int_set(AW_EJECTION_ADDRESS,	code_id);
		return _world_ejection_delete();
	}

	int	Bot::WorldEjection_DeleteAddress(int ip_addr) {
		return WorldEjection_DeleteByType(ip_addr, AW_EJECT_BY_ADDRESS);
	}

	int	Bot::WorldEjection_DeleteCompID(int comp_id) {
		return WorldEjection_DeleteByType(comp_id, AW_EJECT_BY_COMPUTER);
	}

	int	Bot::WorldEjection_DeleteCitizen(int citizen_id) {
		return WorldEjection_DeleteByType(citizen_id, AW_EJECT_BY_CITIZEN);
	}

	int	Bot::WorldEjection_AddAddress(int ip_addr, LPCSTR comment, int expires) {
		return WorldEjection_AddByType(ip_addr, AW_EJECT_BY_ADDRESS, comment, expires);
	}

	int	Bot::WorldEjection_AddCompID(int comp_id, LPCSTR comment, int expires) {
		return WorldEjection_AddByType(comp_id, AW_EJECT_BY_ADDRESS, comment, expires);
	}

	int	Bot::WorldEjection_AddCitizen(int citizen_id, LPCSTR comment, int expires) {
		return WorldEjection_AddByType(citizen_id, AW_EJECT_BY_ADDRESS, comment, expires);
	}

	int	Bot::WorldEjection_AddByType(int code_id, int type_id, LPCSTR comment, int expires) {
		return WorldEjection_AddByType(code_id, type_id, comment, expires);
	}


	//////////////////////////////////////////////////////////////////////////////////////
	/* object related functions */
	int	Bot::_object_add(void) {
		INT_BASE_CHECK()
		return aw_object_add();
	}

	int	Bot::_object_load(void) {
		INT_BASE_CHECK()
		return aw_object_load();
	}

	int	Bot::_object_change(void) {
		INT_BASE_CHECK()
		return aw_object_change();
	}

	int	Bot::_object_delete(void) {
		INT_BASE_CHECK()
		return aw_object_delete();
	}

	int Bot::_object_click(void) {
		INT_BASE_CHECK()
		return aw_object_click();
	}

	int Bot::_object_bump(void) {
		INT_BASE_CHECK();
	#if AW_BUILD > 51
		return aw_object_bump();
	#else
		return RC_NOT_AVAILABLE;
	#endif
	}

	aw_object_data_zone* Bot::_object_zone(int* len)
	{
		PTR_BASE_CHECK();
		return aw_object_zone(len);
	}
	
	aw_object_data_particles* Bot::_object_particles(int* len)
	{
		PTR_BASE_CHECK();
		return aw_object_particles(len);
	}
	
	aw_object_data_camera* Bot::_object_camera(int* len)
	{
		PTR_BASE_CHECK();
		return aw_object_camera(len);
	}
	
	aw_object_data_mover* Bot::_object_mover(int* len)
	{
		PTR_BASE_CHECK();
		return aw_object_mover(len);
	}


	void Bot::setWorld(LPCSTR world) {
		m_world = world;
	}

	void Bot::setInvisible(BOOL mode) {
		m_invisible = mode;
	}

	void Bot::SetGlobalState(int state) {
		m_global_state = state;
	}

	int	Bot::_state_change() {
		if (SelectInstance() == FALSE) return RC_NO_INSTANCE;
		return aw_state_change();
	}

	int Bot::Move(int x, int y, int z, int yaw, int pitch) {
		/* check instance */
		if (SelectInstance() == FALSE)
			return RC_NO_INSTANCE;

		/* check off yaw */
		yaw = yaw % 3600;

		/* do we move or not? */
		if (dspc_OnPreMove(x, y, z, yaw, pitch) == FALSE)
			return RC_ACTION_BLOCKED;

		/* set up attributes */
		_int_set(AW_MY_X,		x);	
		_int_set(AW_MY_Y,		y);
		_int_set(AW_MY_Z,		z);
		_int_set(AW_MY_YAW,		yaw);
		_int_set(AW_MY_PITCH,	pitch);

		/* set up own variables */
		m_x			= x;	
		m_y			= y;
		m_z			= z;	
		m_yaw		= yaw;
		m_pitch		= pitch;

		/* state change */
		int rc = _state_change();

		/* do we change visible states */
		if (getInvisible() == TRUE) {
			setInvisible(FALSE);
			onEventVisible();        
		}

		/* trigger event */
		onEventMove();
		return rc;
	}

	int Bot::ChangeAvatar(int avatar, int gesture, int state, bool apply) {
		/* set our values */
		if (avatar  != -1) m_avatar		= avatar;	
		if (gesture != -1) m_gesture	= gesture; 
		if (state   != -1) m_state		= state;
		
		/* set our values */
		_int_set(AW_MY_TYPE,	avatar);	
		_int_set(AW_MY_GESTURE,	gesture);	
		_int_set(AW_MY_STATE,	state);
		
		/* trigger the movement if required */
		if (apply == false)
			return TRUE;

		return getInvisible() == TRUE ? RC_SUCCESS : Move(*this);
	}

	int	Bot::Move(const Location& coords) {
		return Move(coords.getX(), coords.getY(), coords.getZ(), coords.getYAW(), NULL);
	}

	int Bot::FaceTowards(int x, int z) {
		return Move(getX(), getY(), getZ(), AngleBetween(x, z), 0);
	}

	int Bot::FaceAway(int x, int z) {
		return Move(getX(), getY(), getZ(), (AngleBetween(x, z) + 1800) % 3600, 0);
	}

	int Bot::FaceTowards(const Location& coords) {
		return Move(getX(), getY(), getZ(), AngleBetween(coords.getX(), coords.getZ()), 0);
	}

	int Bot::FaceAway(const Location& coords) {
		return Move(coords.getX(), getY(), coords.getZ(), (AngleBetween(coords.getX(), coords.getZ()) + 1800) % 3600, 0);
	}

	int Bot::MoveInDirectionR(double angle, int distance, int height)
	{
		int new_x = getX() + static_cast<int>(sin(angle) * distance);
		int new_z = getZ() + static_cast<int>(cos(angle) * distance);
		return Move(new_x, height, new_z, AngleBetween(new_x, new_z), getPitch());
	}

	int Bot::MoveInDirectionRevR(double angle, int distance, int height)
	{
		int new_x = static_cast<int>(getX() + sin(angle) * distance);
		int new_z = static_cast<int>(getZ() + cos(angle) * distance);
		int dangle = AngleBetween(new_x, new_z);
		return Move(new_x, height, new_z, abs(dangle + 1800), getPitch());
	}


	int	Bot::_query(int x, int z, int sequence[3][3]) {
		INT_BASE_CHECK()
		return aw_query(x, z, sequence);
	}
	
	int	Bot::_query_5x5(int x, int z, int sequence[5][5]) {
		INT_BASE_CHECK()
		return aw_query_5x5(x, z, sequence);
	}

	int	Bot::_cell_next(void) {
		INT_BASE_CHECK()
		return aw_cell_next();
	}

	bool is_pos_num_ex(LPCSTR p) {
		for (int i = 0; i < static_cast<int>(strlen(p)); i++)
			if (!isdigit(*(p + i)))
				return false;
		return true;
	}

	/*****************************************************************************
	**                                                                          **
	**                                                                          **
	**                           WORLD RIGHTS LOOKUP                            **
	**                            lookup, check, all                            **
	**                                                                          **
	**                                                                          **
	*****************************************************************************/
		/* world rights lookup kind of stuff */
	int	Bot::_check_right(int citizen, char* str)const {
		INT_BASE_CHECK()

	#if (AW_BUILD > 55)
		return aw_check_right(citizen, str);
	#else
		return ext_check_world_right(citizen, str);
	#endif

	}

	int	Bot::_check_right_all(char* str) const {
		INT_BASE_CHECK()

	#if (AW_BUILD > 55)
		return aw_check_right_all(str);
	#else
		return String(str).Left(1) == "*";
	#endif

	}

	int	Bot::_has_world_right(int citizen, int right) const {
		INT_BASE_CHECK()

#if (AW_BUILD > 55)
		return aw_has_world_right(citizen, (AW_ATTRIBUTE)right);
	#else
		return ext_check_world_right(citizen, _string(static_cast<AW_ATTRIBUTE>(right)));
#endif

	}

	int	Bot::_has_world_right_all(int right) const {
		INT_BASE_CHECK()

#if (AW_BUILD > 55)
		return aw_has_world_right_all((AW_ATTRIBUTE)right);
#else
		return String(_string(static_cast<AW_ATTRIBUTE>(right))).Left(1) == "*";
#endif
	}


	//////////////////////////////////////////////////////////////////////////
	// Heads Up Display

	int	Bot::_hud_create(void) {
		INT_BASE_CHECK();
		return aw_hud_create();
	}

	int	Bot::_hud_click(void) {
		INT_BASE_CHECK();
		return aw_hud_click();
	}
	
	int	Bot::_hud_destroy(int session, int id) {
		INT_BASE_CHECK();
		return aw_hud_destroy(session, id);
	}
	
	int	Bot::_hud_clear(int session) {
		INT_BASE_CHECK();
		return aw_hud_clear(session);
	}
}

namespace AW {
	Bot* getCurInstance()
	{
		return bots.getCurInstance();
	}

	//////////////////////////////////////////////////////////////////////////
	// Base Bot Defenitions
	Bots::Bots(void) {

	}

	Bots::~Bots(void) {
		Bot* pBot;
		while (m_vCol.size()) {
			pBot = m_vCol.RemoveTail();
			if (pBot->getUniqueID() != "")
				delete pBot;
		}
	}

	Bot* Bots::getCurInstance() const 
	{
		// return the pointer
		Bot* pBot = (Bot*)aw_user_data();
		return pBot;

		void* vptr = aw_instance();

		/* loop through everything and get the code */
		for (size_t i = 0; i < m_vCol.size(); i++) {
			if (m_vCol[i]->getInstance() == vptr)
				return m_vCol[i];
		}

		/* return sod all */
		return NULL;
	}

	void Bots::InsertBot(Bot* pBot) {
		m_vCol.push_back(pBot);
	}

	int Bots::TriggerEvents() {
		Bot* pBot;
		static bool triggered = false;
		if (triggered == true)
			return 0;
		else {
			triggered = true;
			FOR_EACH_OF_DEFINED(pBot, pos_loop, m_vCol)
				pBot->TriggerEvents(0);
			END_FOR_EACH
			triggered = false;
		}
		return 0;
	}

	void Bots::RemoveBot(Bot* pBot)
	{
		for (size_t i = 0; i < this->m_vCol.size(); i++) {
			if (m_vCol[i] == pBot) {
				m_vCol.RemoveAt(i);
				return;
			}
		}
	}
}



namespace AW 
{
	Citizen::Citizen()
	{
		m_number			= 0;
		m_time_left			= 0;
		m_immigration_time	= 0;
		m_expiration_time	= 0;
		m_beta				= FALSE;
		m_last_login		= 0;
		m_bot_limit			= 0;
		m_total_time		= 0;
		m_enabled			= TRUE;
		m_privacy			= FALSE;
		m_trial				= FALSE;
		m_cav_enabled		= FALSE;
		m_cav_template		= FALSE;
	}
	
	Citizen::~Citizen() 
	{ 
	}

	String Citizen::getLastAddressString() const
	{
		return AW::_XConvert::long_to_ip_string(m_last_address);
	}

	bool Citizen::getDisableTelegrams() const 
	{ 
		return (m_privacy & AW_PRIVACY_BLOCK_TELEGRAMS) == AW_PRIVACY_BLOCK_TELEGRAMS;	
	}

	int Citizen::from_sdk(Bot& Client)
	{
		bool ex = AW::EXCHANGE_UPDATE_DATA;
		Client._exchange(ex, AW_CITIZEN_NUMBER,					m_number);
		Client._exchange(ex, AW_CITIZEN_NAME,					m_name);
		Client._exchange(ex, AW_CITIZEN_PASSWORD,				m_password);
		Client._exchange(ex, AW_CITIZEN_EMAIL,					m_email);
		Client._exchange(ex, AW_CITIZEN_TIME_LEFT,				m_time_left);
		Client._exchange(ex, AW_CITIZEN_PRIVILEGE_PASSWORD,		m_privilege_password);
		Client._exchange(ex, AW_CITIZEN_IMMIGRATION_TIME,		m_immigration_time);
		Client._exchange(ex, AW_CITIZEN_EXPIRATION_TIME,		m_expiration_time);
		Client._exchange(ex, AW_CITIZEN_BETA,					m_beta);
		Client._exchange(ex, AW_CITIZEN_LAST_LOGIN,				m_last_login);
		Client._exchange(ex, AW_CITIZEN_BOT_LIMIT,				m_bot_limit);
		Client._exchange(ex, AW_CITIZEN_TOTAL_TIME,				m_total_time);
		Client._exchange(ex, AW_CITIZEN_ENABLED,				m_enabled);
		Client._exchange(ex, AW_CITIZEN_COMMENT,				m_comment);
		Client._exchange(ex, AW_CITIZEN_URL,					m_url);
		Client._exchange(ex, AW_CITIZEN_PRIVACY,				m_privacy);
		Client._exchange(ex, AW_CITIZEN_TRIAL,					m_trial);
		Client._exchange(ex, AW_CITIZEN_CAV_ENABLED,			m_cav_enabled);
		Client._exchange(ex, AW_CITIZEN_LAST_ADDRESS,			m_last_address);

		return 0;
	}

	int Citizen::to_sdk(Bot& Client)
	{
		bool ex = AW::EXCHANGE_UPDATE_SDK;
		Client._exchange(ex, AW_CITIZEN_NUMBER,					m_number);
		Client._exchange(ex, AW_CITIZEN_NAME,					m_name);
		Client._exchange(ex, AW_CITIZEN_PASSWORD,				m_password);
		Client._exchange(ex, AW_CITIZEN_EMAIL,					m_email);
		Client._exchange(ex, AW_CITIZEN_PRIVILEGE_PASSWORD,		m_privilege_password);
		Client._exchange(ex, AW_CITIZEN_EXPIRATION_TIME,		m_expiration_time);
		Client._exchange(ex, AW_CITIZEN_BETA,					m_beta);
		Client._exchange(ex, AW_CITIZEN_LAST_LOGIN,				m_last_login);
		Client._exchange(ex, AW_CITIZEN_BOT_LIMIT,				m_bot_limit);
		Client._exchange(ex, AW_CITIZEN_ENABLED,				m_enabled);
		Client._exchange(ex, AW_CITIZEN_COMMENT,				m_comment);
		Client._exchange(ex, AW_CITIZEN_URL,					m_url);
		Client._exchange(ex, AW_CITIZEN_PRIVACY,				m_privacy);
		Client._exchange(ex, AW_CITIZEN_TRIAL,					m_trial);
		Client._exchange(ex, AW_CITIZEN_CAV_ENABLED,			m_cav_enabled);
		return 0;
	}
}




namespace AW {
	Coordinates::Coordinates() {
		m_x = m_y = m_z = m_pitch = m_roll = 0;
	}

	bool Coordinates::FromLocation(Location& location) 
	{
		/* convert from one to another */
		m_x		= location.getX();
		m_y		= location.getY();
		m_z		= location.getZ();
		m_yaw	= location.getYAW();
		m_pitch	= location.getPitch();

		return true;
	}

	Coordinates::~Coordinates() { 

	}

	bool Coordinates::FromString(LPCSTR source)
	{
		m_x		= 0;
		m_y		= 0;
		m_z		= 0;
		m_yaw	= 0;
		m_world = "";

		SplitString arr;
		size_t max = arr.split(source, " ", -1);
		
		// we handle each item in turn
		for (size_t i = 0; i < max; i++) 
		{
			char var = arr[i][0];
			
			// is it a world
			if ((var < '0' || var > '9') && var != '-')
			{
				m_world = arr[i];
			}
			else
			{
				// the result is needed by a parser
				double result; char* term = NULL;
				CStringA t = arr[i];
				result = strtod( t, &term);
				// switch to find the destination
				
				switch (*(term)) 
				{
					case 'n': case 'N':	
						m_z	= (long)(result * 1000);
						break;

					case 's': case 'S':	
						m_z	= (long)(result * -1000);	
						break;

					case 'w': case 'W':	
						m_x	= (long)(result * 1000);	
						break;

					case 'e': case 'E':	
						m_x	= (long)(result * -1000);	
						break;
					
					case 'a': case 'A':	
						m_y	= (long)(result * 100);		
						break;
					
					default:
						m_yaw = (long)(result * 10);
						break;
				}
			}
		}
		return true;
	}

	bool Coordinates::FromStringW(LPCTSTR source)
	{
		String temp = T2A((char*)source);
		return FromString(temp);
	}


	/* constructors */
	Location::Location(const Location& src)
	{
		m_x			= src.getX();	
		m_y			= src.getY();
		m_z			= src.getZ();	
		m_yaw		= src.getYAW();
		m_pitch	= src.m_pitch;
		m_roll	= src.m_roll;
	}

	Location::Location(int x, int y, int z, int yaw) 
	{
		setPosition(x, y, z, yaw);
	}

	Location& Location::operator=(const Location& src) 
	{
		m_x		= src.getX();	m_y		= src.getY();
		m_z		= src.getZ();	m_yaw	= src.getYAW();
		m_pitch	= src.m_pitch;	m_roll	= src.m_roll;
		return *this;
	}

	bool Location::operator==(const Location& src) const 
	{
		return (m_x == src.m_x && m_y == src.m_y && m_z == src.m_z && 
			m_yaw == src.m_yaw && m_pitch == src.m_pitch);
	}

	/* return the coordinates */
	String Location::getCoordinates(int Format) const
	{
		if (Format == Location::COORDS_FORMAT_FULL)
		{
			String format, sx, sz;
			sx.Format("%f", abs(m_x) / 1000.0);	sx.TrimRight("0"); sx.TrimRight(".");
			sz.Format("%f", abs(m_z) / 1000.0);	sz.TrimRight("0"); sz.TrimRight(".");
			/* format code*/
			format.Format("%s%c %s%c %.1fa %d",
				sz, m_z > 0 ? 'n' : 's', sx, m_x > 0 ? 'w' : 'e',
				(double)(m_y) / 100, m_yaw / 10);
			return format;
		}
		else if (Format == Location::COORDS_FORMAT_XZ)
		{
			String format, sx, sz;
			sx.Format("%f", abs(m_x) / 1000.0);	sx.TrimRight("0"); sx.TrimRight(".");
			sz.Format("%f", abs(m_z) / 1000.0);	sz.TrimRight("0"); sz.TrimRight(".");
			/* format code*/
			format.Format("%s%c %s%c",
				sz, m_z > 0 ? 'n' : 's', sx, m_x > 0 ? 'w' : 'e');
			return format;
		}
		else
		{
			return "Invalid Format";
		}
	}
	/* return cell coordinates only*/
	String Location::getCellCoordinates() const 
	{
		String format, sx, sz;
		sx.Format("%d", (int)(abs(m_x) / 1000.0));
		sz.Format("%d", (int)(abs(m_z) / 1000.0));	
		/* format code*/
		format.Format("%s%c %s%c",
			sz, m_z > 0 ? 'n' : 's', sx, m_x > 0 ? 'w' : 'e');
		return format;
	}

	void Location::setPosition(int x, int y, int z, int yaw)
	{
		m_x		= x;	
		m_y		= y;
		m_z		= z;	
		m_yaw = yaw;
	}
	/* calculate rectangle entry */
	bool Location::InRect(int ox, int oy, int oz, int distance) const 
	{
		int dif_x = abs(m_x - ox);
		int dif_y = abs(m_y - oy);
		int dif_z = abs(m_z - oz);
		return (dif_x <= distance && dif_y <= distance && dif_z <= distance);
	}
	
	bool  Location::InRect(const Location& pt, int distance) const 
	{
		return InRect(pt.getX(), pt.getY(), pt.getZ(), distance);
	}
	
	int Location::AngleBetween(int x, int z) const 
	{
		return static_cast<int>(_XConvert::aw_radians_to_angle(atan2((double)(x - getX()), (double)(z - getZ()))));
	}
	
	int Location::AngleBetween(const Location& pt) const 
	{
		return AngleBetween(pt.getX(), pt.getZ());
	}
	
	double Location::AngleBetweenR(int x, int z) const
	{
		return atan2((double)(x - getX()), (double)(z - getZ()));
	}
	
	double Location::AngleBetweenR(const Location& pt) const 
	{
		return AngleBetweenR(pt.getX(), pt.getZ());
	}
	
	bool Location::PointInRect(const Location& hi, const Location& lo) const
	{
		bool x = (lo.getX() <= getX() && getX() <= hi.getX());
		bool y = (lo.getY() <= getY() && getY() <= hi.getY());
		bool z = (lo.getZ() <= getZ() && getZ() <= hi.getZ());
		return x && y && z;
	}

	bool Location::PointInRect_2D(const Location& hi, const Location& lo) const
	{
		bool x = (lo.getX() <= getX() && getX() <= hi.getX());
		bool z = (lo.getZ() <= getZ() && getZ() <= hi.getZ());
		return x && z;
	}

	Location Location::getOffsetPosition(int Distance, int Elevation, int Rel)
	{
		// elevation control
		int new_x = (int)(getX() + sin(Location::GetRadianYAW()) * Distance);
		int new_z = (int)(getZ() + cos(Location::GetRadianYAW()) * Distance);
		int new_y = getY() + Elevation;
		int new_yaw = getYaw();

		// new location
		Location lc(new_x, new_y, new_z, new_yaw);
		return lc;
	}

	Location Location::toHighPoint(Location& Other)
	{
		return Location(max(getX(), Other.getX()), max(getY(), Other.getY()), max(getZ(), Other.getZ()), 0);
	}

	Location Location::toLowPoint(Location& Other)
	{
		return Location(min(getX(), Other.getX()), min(getY(), Other.getY()), min(getZ(), Other.getZ()), 0);
	}

	/* linear distance calculations */
	double Location::distanceTo(int x, int y, int z) const 
	{
		/* calculate the variable offsets*/
		double d_x = x - m_x;	double d_y = y - m_y;
		double d_z = z - m_z;
		/* return the power */
		return pow((double)(d_x*d_x)+(d_y*d_y)+(d_z*d_z), 0.5);
	}

	double Location::distanceTo(const Location& pt) const 
	{
		return distanceTo(pt.getX(), pt.getY(), pt.getZ());
	}

	double Location::distanceToGD(int x, int z) const 
	{
		return (distanceTo(x, getY(), z));
	}

	double Location::distanceToGD(const Location& pt) const
	{ 
		return distanceToGD(pt.getX(), pt.getZ());
	}

	/* calculating offsets from a location */
	Location Location::CalculateOffset(int x, int y, int z, int yaw) const
	{
		Location location(x - getX(),	y - getY(), z - getZ(),	yaw - getYAW());
		return location;
	}

	Location Location::CalculateOffset(const Location& pt) const
	{
		return CalculateOffset(pt.getX(), pt.getY(), pt.getZ(), pt.getYAW());
	}

	int	Location::CellFromPosition	(int position) 
	{
		if (position >= 0)
			return position / 1000;
		else
			return position / 1000 - ((position % 1000 == 0) ? 0 : 1);
	}

	int	Location::getCellX(void) const
	{	
		return CellFromPosition(m_x);	
	}

	int	Location::getCellZ(void) const
	{	
		return CellFromPosition(m_z);	
	}

	int	Location::getCellIndex(void) const
	{	
		return MAKELONG(getCellX(), getCellZ());	
	}

	int	Location::getSectorX(void) const
	{
		return aw_sector_from_cell(getCellX());
	}
	
	int Location::getSectorZ(void) const
	{
		return aw_sector_from_cell(getCellZ());
	}

	int	Location::getSectorIndex(void) const
	{	
		return MAKELONG(getSectorX(), getSectorZ());	
	}

}





namespace AW 
{
	int	UniverseOptions::FromSDK(Bot& bot)
	{
		bot.Lock();
		browser_minimum		= bot._int(AW_UNIVERSE_BROWSER_MINIMUM);
		browser_release		= bot._int(AW_UNIVERSE_BROWSER_RELEASE);
		browser_beta		= bot._int(AW_UNIVERSE_BROWSER_BETA);
		world_minimum		= bot._int(AW_UNIVERSE_WORLD_MINIMUM);
		world_release		= bot._int(AW_UNIVERSE_WORLD_RELEASE);
		world_beta			= bot._int(AW_UNIVERSE_WORLD_BETA);
		greeting			= bot._string(AW_UNIVERSE_WELCOME_MESSAGE);
		start_world			= bot._string(AW_UNIVERSE_WORLD_START);
		search_url			= bot._string(AW_UNIVERSE_SEARCH_URL);
	#if AW_BUILD > 50
		notepad_url			= bot._string(AW_UNIVERSE_NOTEPAD_URL);
	#endif
		allow_tourists		= bot._bool(AW_UNIVERSE_ALLOW_TOURISTS);
		bot.Unlock();
		return 0;
	}

	int	UniverseOptions::ToSDK(Bot& bot, bool Apply) 
	{
		bot.Lock();
		bot._int_set(AW_UNIVERSE_BROWSER_MINIMUM,	browser_minimum);
		bot._int_set(AW_UNIVERSE_BROWSER_RELEASE,	browser_release);
		bot._int_set(AW_UNIVERSE_BROWSER_BETA,		browser_beta);
		
		/* world section */
		bot._int_set(AW_UNIVERSE_WORLD_MINIMUM,		world_minimum);
		bot._int_set(AW_UNIVERSE_WORLD_RELEASE,		world_release);
		bot._int_set(AW_UNIVERSE_WORLD_BETA,		world_beta);

		/* strings section */
		bot._string_set(AW_UNIVERSE_WELCOME_MESSAGE,greeting);
		bot._string_set(AW_UNIVERSE_WORLD_START,	start_world);
		bot._string_set(AW_UNIVERSE_SEARCH_URL,		search_url);
	#if AW_BUILD > 50
		bot._string_set(AW_UNIVERSE_NOTEPAD_URL,	notepad_url.Left(AW_MAX_ATTRIBUTE_LENGTH));
	#endif
		bot._bool_set(AW_UNIVERSE_ALLOW_TOURISTS,	allow_tourists);
		int rc = 0;
		if (Apply == true) { rc = bot._universe_attributes_change(); }
		bot.Unlock();
		return rc;
	}

	bool UniverseOptions::FromFile(const char* File) 
	{
		/* load a new INI parser */
		EnhancedINI ini;
		if (ini.Load(File) == false)
			return false;
		Exchange(ini, true);
		return true;
	}

	bool UniverseOptions::ToFile(const char* File) 
	{
		/* load a new INI parser */
		EnhancedINI ini;
		Exchange(ini, false);
		return ini.Save(File);
	}

	void UniverseOptions::Exchange(EnhancedINI& INI, bool Reading) 
	{
		INI.Exchange(Reading, "uni_browser",	"minimum",			browser_minimum);
		INI.Exchange(Reading, "uni_browser",	"release",			browser_release);
		INI.Exchange(Reading, "uni_browser",	"beta",				browser_beta);
		INI.Exchange(Reading, "uni_world",		"minimum",			world_minimum);
		INI.Exchange(Reading, "uni_world",		"release",			world_release);
		INI.Exchange(Reading, "uni_world",		"beta",				world_beta);
		INI.Exchange(Reading, "uni_data",		"greeting",			greeting);
		INI.Exchange(Reading, "uni_data",		"start_world",		start_world);
		INI.Exchange(Reading, "uni_data",		"notepad",			notepad_url);
		INI.Exchange(Reading, "uni_data",		"search_url",		search_url);
		INI.Exchange(Reading, "uni_data",		"allow_tourists",	allow_tourists);
	}
}

namespace AW 
{
	void EnhancedINI::Section::InsertKey(const char* ID, const char* Data) 
	{
		keys[ID] = Data;
	}

	EnhancedINI::Section& EnhancedINI::getSection(const char* SectionID) 
	{
		SectionMap::iterator itr = list.find(SectionID);
		if (itr == list.end()) 
		{
			Section& sec = list[SectionID];
			sec.name     = SectionID;
			return sec;
		}
		else
		{
			return list[SectionID];
		}
	}

	EnhancedINI::Section* EnhancedINI::getSectionRaw(const char* SectionID) 
	{
		SectionMap::iterator itr = list.find(SectionID);
		if (itr == list.end()) 
		{
			return NULL;
		}
		else
		{
			return &list[SectionID];
		}
	}

	void EnhancedINI::Write(const char* Sect, const char* Key, const char* Text) 
	{
		getSection(Sect)[Key] = Text;
	}

	void EnhancedINI::Write(const char* Sect, const char* Key, int IntNum) 
	{
		String text; text.Format("%d", IntNum);
		getSection(Sect)[Key] = text;
	}
	
	void EnhancedINI::Write(const char* Sect, const char* Key, float FNum) 
	{
		String text; text.Format("%f", FNum);
		getSection(Sect)[Key] = text;
	}

	void EnhancedINI::Write(const char* Sect, const char* Key, time_t FNum) 
	{
		String text; text.Format("%i64", FNum);
		getSection(Sect)[Key] = text;
	}

	void EnhancedINI::Write(const char* Sect, const char* Key, bool BoolNum)
	{
		String text; text.Format("%d", BoolNum ? 1 : 0);
		getSection(Sect)[Key] = text;
	}

	String MultilineToSingle(String Text)
	{
		Text.Replace("\n", "\\n");
		Text.Replace("\r", "\\r");
		return Text;
	}

	String SingleToMultiline(String Text)
	{
		Text.Replace("\\n", "\n");
		Text.Replace("\\r", "\r");
		return Text;
	}

	String EnhancedINI::getFileData()
	{
		String temp;

		/* write each section */
		for (SectionMap::iterator itr = list.begin();
				itr != list.end();
				itr++ )
		{
			temp.AppendFormat("\n[%s]\n", (LPCSTR)itr->second.name);
			for (KeyMap::iterator itr2 = itr->second.keys.begin();
					itr2 != itr->second.keys.end();
					itr2++ )
			{
				temp.AppendFormat(" %s=%s\n", itr2->first, MultilineToSingle(itr2->second));
			}
		}

		return temp;
	}

	bool EnhancedINI::Load(const char* FilePath) {
		FILE*		fptr = NULL;
		char		temp[20480];
		String		line, cur_sect;

		/* open the file up for reading */
		list.clear();
		errno_t err = fopen_s(&fptr, FilePath, "r");
		if (err) return false;
		while (fgets(temp, 20480, fptr)) {
			line = temp; 
			line.Trim();
			/* ignore lines starting with a # - treat them as comments */
			if (line.Left(1) == "#") continue;
			if (line.Left(1) == "[") {
				cur_sect = line.Mid(1, line.GetLength() - 2);
			} else {
				int eq_at = line.Find("=");
				if (eq_at == -1) continue;				// no data
				String key = line.Mid(0, eq_at);
				String data= SingleToMultiline(line.Mid(eq_at + 1)); 
				Write(cur_sect, key, data);
			}
		}
		/* close the file and all is well */
		fclose(fptr);
		return true;
	}
	
	bool EnhancedINI::Save(const char* FilePath) {
		FILE*		fptr = NULL;
		String		line, cur_sect;

		/* open the file up for reading */
		errno_t err = fopen_s(&fptr, FilePath, "w");
		if (err) return false;

		/* write each section */
		for (SectionMap::iterator itr = list.begin();
				itr != list.end();
				itr++ )
		{
			fprintf_s(fptr, "\n[%s]\n", (LPCSTR)itr->second.name);
			for (KeyMap::iterator itr2 = itr->second.keys.begin();
					itr2 != itr->second.keys.end();
					itr2++ )
			{
				fprintf(fptr, " %s=%s\n", (const char*)itr2->first, (const char*)MultilineToSingle(itr2->second));
			}
		}

		/* close the file and all is well */
		fclose(fptr);
		return true;
	}

	bool EnhancedINI::KeyExists(const char* Sect, const char* KeyId)  const
	{
		// check section	
		SectionMap::iterator itr = (const_cast<EnhancedINI*>(this))->list.find(Sect);
		if (itr == list.end()) return false;

		// check key
		return (itr->second.keys.find(KeyId) != itr->second.keys.end());
	}

	String EnhancedINI::Read(const char* Sect, const char* KeyID, const char* Default) const
	{
		// go to default if the section doesnt exist
		Section* pSect = (const_cast<EnhancedINI*>(this))->getSectionRaw(Sect);
		if (pSect == NULL) return Default;	// defaults remain unchanged

		// go to default if key doesnt exist
		if ((*pSect).keys.find(KeyID) == pSect->keys.end()) return Default;
		return pSect->keys[KeyID];
	}

	int EnhancedINI::Read(const char* Sect, const char* KeyID, int Default) const
	{
		// go to default if the section doesnt exist
		Section* pSect = (const_cast<EnhancedINI*>(this))->getSectionRaw(Sect);
		if (pSect == NULL) return Default;	// defaults remain unchanged

		// go to default if key doesnt exist
		if ((*pSect).keys.find(KeyID) == pSect->keys.end()) return Default;
		return atoi(pSect->keys[KeyID]);
	}

	bool EnhancedINI::Exchange(bool Reading, const char* Sect, const char* KeyId, String& Value) 
	{
		if (Reading == AW::READ_FROM_INI)	
			Value = Read(Sect, KeyId, Value);
		else			
			Write(Sect, KeyId, Value);
		return true;
	}

	bool EnhancedINI::Exchange(bool Reading, const char* Sect, const char* KeyId, int& Value)
	{
		String text;
		if (Reading == AW::READ_FROM_INI)	{ text.Format("%d", Value); Value = atoi(Read(Sect, KeyId, text)); } 
		else			Write(Sect, KeyId, Value);
		return true;
	}

	bool EnhancedINI::Exchange(bool Reading, const char* Sect, const char* KeyId, time_t& Value)
	{
		String text;
		text.Format("%.I64d", Value); 
		
		if (Reading == AW::READ_FROM_INI)	
		{ 
			Value = _atoi64(Read(Sect, KeyId, text)); 
		} 
		else			
		{
			Write(Sect, KeyId, text);
		}
		return true;
	}
	
	bool EnhancedINI::Exchange(bool Reading, const char* Sect, const char* KeyId, DWORD& Value) 
	{
		String text;
		if (Reading == AW::READ_FROM_INI)	{ text.Format("%d", Value); Value = atoi(Read(Sect, KeyId, text)); } 
		else			Write(Sect, KeyId, (int)Value);
		return true;
	}

	bool EnhancedINI::Exchange(bool Reading, const char* Sect, const char* KeyId, bool& Value) 
	{
		String text;
		int   iVal = Value ? 1 : 0;
		Exchange(Reading, Sect, KeyId, iVal);
		Value = (iVal == 1);
		return true;
	}

	bool EnhancedINI::Exchange(bool Reading, const char* Sect, const char* KeyId, float& Value)
	{
		String text;
		if (Reading == AW::READ_FROM_INI)	{ 
			text.Format("%f", Value); text.TrimRight("0 "); 
			Value = (float)atof(Read(Sect, KeyId, text)); } 
		else			
			Write(Sect, KeyId, Value);
		return true;
	}

	int	EnhancedINI::getKeyInt(const char* SectionId, const char* Key) const
	{
		return Read(SectionId, Key, 0);
	}
		
	String EnhancedINI::getKey(const char* SectionId, const char* Key) const
	{
		return Read(SectionId, Key, "");
	}
}

namespace AW
{
	int WorldRights::FromBot(Bot& Source, int Citizen)
	{
		int rt_ok = Source._has_world_right(Citizen, AW_WORLD_EJECT_RIGHT);
		enter	= TRUE == Source._has_world_right(Citizen, AW_WORLD_ENTER_RIGHT);
		build	= TRUE == Source._has_world_right(Citizen, AW_WORLD_BUILD_RIGHT);
		ed		= TRUE == Source._has_world_right(Citizen, AW_WORLD_EMINENT_DOMAIN_RIGHT);
		spobjs	= TRUE == Source._has_world_right(Citizen, AW_WORLD_SPECIAL_OBJECTS_RIGHT);
		spcmds	= TRUE == Source._has_world_right(Citizen, AW_WORLD_SPECIAL_COMMANDS_RIGHT);
		ps		= TRUE == Source._has_world_right(Citizen, AW_WORLD_PUBLIC_SPEAKER_RIGHT);
		eject	= TRUE == Source._has_world_right(Citizen, AW_WORLD_EJECT_RIGHT);
		bots	= TRUE == Source._has_world_right(Citizen, AW_WORLD_BOTS_RIGHT);
		speak	= TRUE == Source._has_world_right(Citizen, AW_WORLD_SPEAK_RIGHT);
#if (AW_BUILD > 50)
		talk	= TRUE == Source._has_world_right(Citizen, AW_WORLD_VOIP_RIGHT);
		v4		= TRUE == Source._has_world_right(Citizen, AW_WORLD_V4_OBJECTS_RIGHT);
#endif
		return 0;
	}
}




namespace AW
{
	void HUD_Element::CopyToSDK(AW::Bot& SDK)
	{
		// identification and target
		SDK._int_set(AW_HUD_ELEMENT_TYPE,		type);
		SDK._int_set(AW_HUD_ELEMENT_ID,			id);
		SDK._int_set(AW_HUD_ELEMENT_SESSION,	session);

		// positions
		SDK._int_set(AW_HUD_ELEMENT_ORIGIN,		origin);
		SDK._int_set(AW_HUD_ELEMENT_X,			x);
		SDK._int_set(AW_HUD_ELEMENT_Y,			y);
		SDK._int_set(AW_HUD_ELEMENT_Z,			z);

		// configuration
		SDK._int_set(AW_HUD_ELEMENT_FLAGS,		flags);
		SDK._string_set(AW_HUD_ELEMENT_TEXT,	text);
		SDK._int_set(AW_HUD_ELEMENT_COLOR,		color);
		SDK._float_set(AW_HUD_ELEMENT_OPACITY,	opacity);
		
		// sizes
		SDK._int_set(AW_HUD_ELEMENT_SIZE_X,		size_x);
		SDK._int_set(AW_HUD_ELEMENT_SIZE_Y,		size_y);
		SDK._int_set(AW_HUD_ELEMENT_SIZE_Z,		size_z);
		
		// texture offsets
		SDK._int_set(AW_HUD_ELEMENT_TEXTURE_OFFSET_X, texture_offsetx);
		SDK._int_set(AW_HUD_ELEMENT_TEXTURE_OFFSET_Y, texture_offsety);
	}

	struct TGC { const char* str; int col; };
	int Colour::LookupString(const char* Name)
	{
		static TGC clr_strings[] = {
			{ "AliceBlue",  0xF0F8FF                    }, 
			{ "AntiqueWhite",  0xFAEBD7                    }, 
			{ "Aqua",  0x00FFFF                    }, 
			{ "Aquamarine",  0x7FFFD4                    }, 
			{ "Azure",  0xF0FFFF                    }, 
			{ "Beige",  0xF5F5DC                    }, 
			{ "Bisque",  0xFFE4C4                    }, 
			{ "Black",  0x000000                    }, 
			{ "BlanchedAlmond",  0xFFEBCD                    }, 
			{ "Blue",  0x0000FF                    }, 
			{ "BlueViolet",  0x8A2BE2                    }, 
			{ "Brown",  0xA52A2A                    }, 
			{ "BurlyWood",  0xDEB887                    }, 
			{ "CadetBlue",  0x5F9EA0                    }, 
			{ "Chartreuse",  0x7FFF00                    }, 
			{ "Chocolate",  0xD2691E                    }, 
			{ "Coral",  0xFF7F50                    }, 
			{ "CornflowerBlue",  0x6495ED                    },
			{ "Cornsilk",  0xFFF8DC                    }, 
			{ "Crimson",  0xDC143C                    }, 
			{ "Cyan",  0x00FFFF                    }, 
			{ "DarkBlue",  0x00008B                    }, 
			{ "DarkCyan",  0x008B8B                    }, 
			{ "DarkGoldenRod",  0xB8860B                    }, 
			{ "DarkGray",  0xA9A9A9                    }, 
			{ "DarkGrey",  0xA9A9A9                    }, 
			{ "DarkGreen",  0x006400                    }, 
			{ "DarkKhaki",  0xBDB76B                    }, 
			{ "DarkMagenta",  0x8B008B                    }, 
			{ "DarkOliveGreen",  0x556B2F                    }, 
			{ "Darkorange",  0xFF8C00                    }, 
			{ "DarkOrchid",  0x9932CC                    }, 
			{ "DarkRed",  0x8B0000                    }, 
			{ "DarkSalmon",  0xE9967A                    }, 
			{ "DarkSeaGreen",  0x8FBC8F                    }, 
			{ "DarkSlateBlue",  0x483D8B                    }, 
			{ "DarkSlateGray",  0x2F4F4F                    }, 
			{ "DarkSlateGrey",  0x2F4F4F                    }, 
			{ "DarkTurquoise",  0x00CED1                    }, 
			{ "DarkViolet",  0x9400D3                    }, 
			{ "DeepPink",  0xFF1493                    }, 
			{ "DeepSkyBlue",  0x00BFFF                    }, 
			{ "DimGray",  0x696969                    }, 
			{ "DimGrey",  0x696969                    }, 
			{ "DodgerBlue",  0x1E90FF                    }, 
			{ "FireBrick",  0xB22222                    }, 
			{ "FloralWhite",  0xFFFAF0                    }, 
			{ "ForestGreen",  0x228B22                    }, 
			{ "Fuchsia",  0xFF00FF                    }, 
			{ "Gainsboro",  0xDCDCDC                    }, 
			{ "GhostWhite",  0xF8F8FF                    }, 
			{ "Gold",  0xFFD700                    }, 
			{ "GoldenRod",  0xDAA520                    }, 
			{ "Gray",  0x808080                    }, 
			{ "Grey",  0x808080                    }, 
			{ "Green",  0x008000                    }, 
			{ "GreenYellow",  0xADFF2F                    }, 
			{ "HoneyDew",  0xF0FFF0                    }, 
			{ "HotPink",  0xFF69B4                    },
			{ "IndianRed ",  0xCD5C5C                    }, 
			{ "Indigo ",  0x4B0082                    }, 
			{ "Ivory",  0xFFFFF0                    }, 
			{ "Khaki",  0xF0E68C                    }, 
			{ "Lavender",  0xE6E6FA                    }, 
			{ "LavenderBlush",  0xFFF0F5                    }, 
			{ "LawnGreen",  0x7CFC00                    }, 
			{ "LemonChiffon",  0xFFFACD                    }, 
			{ "LightBlue",  0xADD8E6                    }, 
			{ "LightCoral",  0xF08080                    }, 
			{ "LightCyan",  0xE0FFFF                    }, 
			{ "LightGoldenRodYellow",  0xFAFAD2                    }, 
			{ "LightGray",  0xD3D3D3                    }, 
			{ "LightGrey",  0xD3D3D3                    }, 
			{ "LightGreen",  0x90EE90                    }, 
			{ "LightPink",  0xFFB6C1                    }, 
			{ "LightSalmon",  0xFFA07A                    }, 
			{ "LightSeaGreen",  0x20B2AA                    }, 
			{ "LightSkyBlue",  0x87CEFA                    }, 
			{ "LightSlateGray",  0x778899  			}, 
			{ "LightSlateGrey",  0x778899                    }, 
			{ "LightSteelBlue",  0xB0C4DE                    }, 
			{ "LightYellow",  0xFFFFE0                    }, 
			{ "Lime",  0x00FF00                    }, 
			{ "LimeGreen",  0x32CD32                    }, 
			{ "Linen",  0xFAF0E6                    }, 
			{ "Magenta",  0xFF00FF                    }, 
			{ "Maroon",  0x800000                    }, 
			{ "MediumAquaMarine",  0x66CDAA                    }, 
			{ "MediumBlue",  0x0000CD                    }, 
			{ "MediumOrchid",  0xBA55D3                    }, 
			{ "MediumPurple",  0x9370D8                    }, 
			{ "MediumSeaGreen",  0x3CB371                    }, 
			{ "MediumSlateBlue",  0x7B68EE                    }, 
			{ "MediumSpringGreen",  0x00FA9A                    }, 
			{ "MediumTurquoise",  0x48D1CC                    }, 
			{ "MediumVioletRed",  0xC71585                    }, 
			{ "MidnightBlue",  0x191970                    }, 
			{ "MintCream",  0xF5FFFA                    }, 
			{ "MistyRose",  0xFFE4E1                    }, 
			{ "Moccasin",  0xFFE4B5                    }, 
			{ "NavajoWhite",  0xFFDEAD                    }, 
			{ "Navy",  0x000080                    }, 
			{ "OldLace",  0xFDF5E6                    }, 
			{ "Olive",  0x808000                    }, 
			{ "OliveDrab",  0x6B8E23                    }, 
			{ "Orange",  0xFFA500                    }, 
			{ "OrangeRed",  0xFF4500                   }, 
			{ "Orchid",  0xDA70D6                      }, 
			{ "PaleGoldenRod",  0xEEE8AA               }, 
			{ "PaleGreen",  0x98FB98                   }, 
			{ "PaleTurquoise",  0xAFEEEE               }, 
			{ "PaleVioletRed",  0xD87093               }, 
			{ "PapayaWhip",  0xFFEFD5                  }, 
			{ "PeachPuff",  0xFFDAB9                   }, 
			{ "Peru",  0xCD853F                   }, 
			{ "Pink",  0xFFC0CB                   }, 
			{ "Plum",  0xDDA0DD                   }, 
			{ "PowderBlue",  0xB0E0E6                   }, 
			{ "Purple",  0x800080                   }, 
			{ "Red",  0xFF0000                   }, 
			{ "RosyBrown",  0xBC8F8F                   }, 
			{ "RoyalBlue",  0x4169E1                   }, 
			{ "SaddleBrown",  0x8B4513                 },    
			{ "Salmon",  0xFA8072                    }, 
			{ "SandyBrown",  0xF4A460                    }, 
			{ "SeaGreen",  0x2E8B57                    }, 
			{ "SeaShell",  0xFFF5EE                    }, 
			{ "Sienna",  0xA0522D                    }, 
			{ "Silver",  0xC0C0C0                    }, 
			{ "SkyBlue",  0x87CEEB                    }, 
			{ "SlateBlue",  0x6A5ACD                    }, 
			{ "SlateGray",  0x708090                    }, 
			{ "SlateGrey",  0x708090                    }, 
			{ "Snow",  0xFFFAFA                    }, 
			{ "SpringGreen",  0x00FF7F                    }, 
			{ "SteelBlue",  0x4682B4                    }, 
			{ "Tan",  0xD2B48C                    }, 
			{ "Teal",  0x008080                    }, 
			{ "Thistle",  0xD8BFD8                    }, 
			{ "Tomato",  0xFF6347                    }, 
			{ "Turquoise",  0x40E0D0                    }, 
			{ "Violet",  0xEE82EE                    }, 
			{ "Wheat",  0xF5DEB3                    }, 
			{ "White",  0xFFFFFF                    }, 
			{ "WhiteSmoke",  0xF5F5F5                    }, 
			{ "Yellow",  0xFFFF00                   }, 
			{ "YellowGreen",  0x9ACD32                } };
		
		// check for a found string
		for (size_t i = 0; i < sizeof(clr_strings) / sizeof(clr_strings[0]); i++)
		{
			if (_stricmp(clr_strings[i].str, Name) == 0)
			{
				// switch the data
				int cur_clr = clr_strings[i].col;
				return RGB(GetBValue(cur_clr), GetGValue(cur_clr), GetRValue(cur_clr));
			}
		}

		// is the string starting with a #
		if (*Name == '#')
		{
			char* blah;
			int cur_clr = strtol((Name + 1), &blah, 16);
			return RGB(GetBValue(cur_clr), GetGValue(cur_clr), GetRValue(cur_clr));
		}

		// return black
		return 0;
	}
}

int ext_check_world_right(int citizen, LPCSTR string)
{
	AW::SplitString split;
	size_t c = split.split(string, ",", -1);

	for (size_t i = 0; i < c; i++) {
		String key = split[i];
		if (atol(key) == citizen && AW::is_pos_num_ex(key)) {
			int t = 1;
			return TRUE;
		}
	}
	return FALSE;
}