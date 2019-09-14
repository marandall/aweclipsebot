#include "stdafx.h"					// for precompiled header
#include "Avatar.h"					// for avatar class
#include "EclipseEvolutionDialog.h"	// for dialog core
#include "AvatarHistory.h"			// for the history  tracking
#include "Avatar_Usersettings.h"	// user settings class
#include "RelayGlobalChatAex.h"
#include "HeadsUp_UserPlugin.h"
#include "RelayMuteList.h"
#include "EclipseBot_PrivilegeResolver.h"
#include "CitizenLookup_Kernel.h"
#include "UserProfile_FreeJoin.h"
#include "scripting_host.h"
#include "RelayPrivateChannels.h"
#include "RelayRelay.h"
#include "scripting_host.h"
#include "dynamicrights.h"

// create overrides
#include "awsdk_world.h"
#include "AvatarOptions.h"
#include "worldattributes.h"

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Avatar::Avatar()
{
	m_muted						= false;
	m_display_colour			= 0xFFFFFF;
	whisper_target				= false;
	m_WorldOverrideState		= 0;

	// dynamic rights
	_permissions				= new DynamicRights::Permissions();

	// chat extender plugin
	m_global_chat_plugin		= new ChatControllers::GlobalChatAexPlugin(*this);
	m_mute_list					= new ChatControllers::MuteList();
	m_pChannelBindings			= new ChatControllers::Channels::MembershipBindings(this);

	// heads up display
	m_pHud						= new HeadsUp::IconTray(this);
	_pFreeTray				= new HeadsUp::FreeTray();

	// avatar extension
	m_pFreeJoin					= new UserProfile::FreeJoin;

	// automatically create attributes
	m_pWorldOverride			= new WorldAttributes(); 
	m_pAvatarOptions			= new AvatarOptions();

	// lookup of citizen information
	citizen_info_available		= false;	
	privilege_info_available	= false;

	// event timings
	last_event_time				= 0;
	last_chat_time				= 0;
	last_object_time			= 0;
	last_movement_time			= 0;
	last_enter_time				= 0;
	
	// encountered status
	first_encounter_time		= 0;
	first_encounter_world		= "";
	first_encounter_position	= "";
	last_encounter_time			= 0;
	last_encounter_world		= "";
	last_encounter_position		= "";
	previous_encounter_count	= 0;

	// map
	// m_automap				= new Umbra::ClassLib::Collections::C_AutoMap();
}

Avatar::~Avatar()
{
	delete _permissions;
	delete m_pWorldOverride;
	delete m_global_chat_plugin;
	delete m_mute_list;
	delete m_pFreeJoin;
	delete m_pHud;
	delete _pFreeTray;
	delete m_pChannelBindings;
	delete m_pAvatarOptions;

	// ensure cleanup of bindings
	getChannelBindings().clear();
}

long Avatar::Click(int Count)
{
	for (int i = 0; i < Count; i++)
		plc_bot->_avatar_click(getSession());
	return 0;
}

void Avatar::OnEnter(void)
{
	OnWorldAttributes();
}

void Avatar::OnExit(void)
{
	/* does all of the things relating to displaying that an avatar has left */
	/*        so that it can be called for each avatar in any situation      */
	String logText;
	logText.Format("AVATAR_DELETE: %s #%d exits at %s", m_name, m_session, getCoordinates());
	WriteMsg(logText, RGB_AVATAR, OT_MAIN | OM_TIME);

	/* at this point we should have the most data, so save it */
	History::AvatarHistory::getInstance().finalise(*this);

	//
	// Last position information
	//

	last_encounter_time			= time64(0);
	last_encounter_world		= plc_bot->_string(AW_WORLD_NAME);
	last_encounter_position		= getCoordinates();
	previous_encounter_count++;

	//
	// Write to persistant storage
	//

	db_WriteExit();
	profileSave();

	/* lookup information from profile dir */
	SyncData(AW::WRITE_TO_INI);

	/* delete the ID in the main list */
	rt_diag->AvatarDelete(*this);
	avatars.DecrementTypes(getType());
}

void Avatar::OnWorldAttributes(void)
{
	CalculateRights();
}

void Avatar::CompileData()
{
	CalculateRights();
	AW::WorldRights wr = wr_own;

	/* reset rights */
	ResetRights();
}

int	Avatar::LookupPrivilegeIfNeeded(void)
{
	// equal names
	if (getCitizen() == getPrivilege())
	{
		m_priv_name = getName();
		m_priv_lus  = LookupState::Complete;
		return 0;
	}

	// if we are complete return success
	if (getPrivLUS() == LookupState::Complete)
		return RC_SUCCESS;

	// look up from the citizens present
	for (size_t i = 0; i < avatars.size(); i++)
	{
		Avatar& user = avatars[i];
		if (user.getCitizen() == getPrivilege())
		{
			m_priv_name = user.getName();
			m_priv_lus  = LookupState::Complete;
			return 0;
		}
		else if (user.getPrivilege() == getPrivilege() && user.getPrivLUS() == LookupState::Complete)
		{
			m_priv_name = user.getName();
			m_priv_lus  = LookupState::Complete;
			return 0;
		}
	}

	// throw user id to the resolver
	m_priv_lus = LookupState::Resolving;
	CitizenLookup::PrivilegeLookupClass* pLookup = new CitizenLookup::PrivilegeLookupClass();
	CitizenLookup::g_Kernel.lookup_by_number(getPrivilege(), pLookup);

	return 0;
}

void Avatar::BeginLookupIp(void)
{
	setAddressLUS(LookupState::Resolving);
}

void Avatar::BeginLookupDns(void)
{
	setDnsLUS(LookupState::Resolving);
}

void Avatar::EndLookupDns(void)
{
	/* write ip information to log */
	String logText, dnsText;
	if (getDns().GetLength())
		dnsText.Format(" and DNS %s", getDns());
	logText.Format("%s #%d has IP %s%s", m_name, m_session, getAddress(), dnsText);
	WriteMsg(logText, 0x004080, OT_SECURITY | OM_TIME);

	/* update systems */
	History::AvatarHistory::getInstance().update(*this);
	rt_diag->c_AvatarsDialog.AvatarUpdate(*this);

	/* update into database */
	db_WriteUpdate();
}

void Avatar::ResetRights(void)
{
	String this_cit, this_priv;
	this_cit.	Format("#%d", getCitizen());			// format citizen 
	this_priv.	Format("#%d", getPrivilege());			// format privilege

	// reference main provider
	DynamicRights::Provider& provider = DynamicRights::g_Provider;
	DynamicRights::User* pUser = 0;

	/* handle the checking of each  */
	pUser = provider.getUser(this_cit);
	if (pUser && pUser->HasRight("system/enabled"))
	{
		_permissions->Merge(*pUser, pUser->user_id);
	}

	pUser = provider.getUser(this_priv);
	if (pUser && pUser->HasRight("system/enabled") && pUser->HasRight("system/privileged"))
	{
		_permissions->Merge(*pUser, pUser->user_id);	
	}

	/* check on the matching citizen to owner privilege */
	if (getCitizen() == bot.getCitizen()) 
	{
		pUser = provider.getUser("//owner");
		if (pUser && pUser->HasRight("system/enabled"))
		{
			_permissions->Merge(*pUser, pUser->user_id);
		}
	}

	/* check on the matching privilege to owner privilege */
	if (getPrivilege() == bot.getCitizen())
	{
		pUser = provider.getUser("//ownerpriv");
		if (pUser && pUser->HasRight("system/enabled"))
		{
			_permissions->Merge(*pUser, pUser->user_id);
		}
	}

	/* general account */
	pUser = provider.getUser("//everyone");
	if (pUser && pUser->HasRight("system/enabled"))
	{
		_permissions->Merge(*pUser, pUser->user_id);
	}

	// citizen state
	DynamicRights::User* pTourists	= provider.getUser("@tourists");
	DynamicRights::User* pCitizens	= provider.getUser("@citizens");
	DynamicRights::User* pBots		= provider.getUser("@bots");

	// check citizen state
	if (pTourists && IsTourist()) _permissions->Merge(*pTourists, pTourists->getUserId());
	if (pCitizens && IsCitizen()) _permissions->Merge(*pCitizens, pCitizens->getUserId());
	if (pBots     && IsBot()    ) _permissions->Merge(*pBots,     pBots    ->getUserId());

	// additional groups
	size_t osize = _permissions->size();
	for (size_t i = 0; i < osize; i++)
	{
		if ((*_permissions)[i].key_id.Left(7) == "groups/")
		{
			String groupType = "@";
			groupType+= (*_permissions)[i].key_id.Mid(7);
			
			// user exists
			pUser = provider.getUser(groupType);
			if (pUser && pUser->HasRight("system/enabled"))
			{
				_permissions->Merge(*pUser, String("Group(") + groupType + ")");
			}
		}
	}

	// scripting host
	//
	ScriptingHost::getInst().doFunction("EventAvatarPermissionsRebuild", 0, 0, &bot);
}

DynamicRights::User* Avatar::allocDynamicRights()
{
	DynamicRights::Provider& gp = DynamicRights::g_Provider;
			
	// only citizens may access this page
	if (IsCitizen() == true)
	{
		// lookup the existing user
		String uStr = String("#") + ToString(m_citizen);
		DynamicRights::User* pUsr = gp.getUser(uStr);
		if (pUsr)
		{
			return pUsr;
		}
		else
		{
			DynamicRights::User usr;
			usr.user_id			= uStr;
			usr.full_id			= getName();
			usr.description	= "";
			usr.Permit("system/enabled");

			// return the newly created
			gp.addUser(usr);
			return gp.getUser(uStr);
		}
	}
	else
	{
		return 0;
	}
}

bool Avatar::GetRight(String KeyCode) const
{
	return _permissions->HasRight(KeyCode) || _permissions->HasRight("system/administrator");
}

Avatar_DataConnector::Avatar_DataConnector()
{
	m_dc_user_settings = new Avatar_Usersettings();
}

Avatar_DataConnector::~Avatar_DataConnector()
{
	delete m_dc_user_settings;	
}

void Avatar::SyncData(bool Load)
{
}

ChatControllers::GlobalChatAexPlugin& Avatar::getGlobalChatPlugin()
{
	return *m_global_chat_plugin;
}

ChatControllers::MuteList& Avatar::getMuteList()
{
	return *m_mute_list;
}


void Avatar::profileExchange(bool Dir, Serializer& Sz)
{
	// time clock information
	Sz.Exchange(Dir, "time", "last_event", last_event_time);
	Sz.Exchange(Dir, "time", "last_chat", last_chat_time);
	Sz.Exchange(Dir, "time", "last_object", last_object_time);
	Sz.Exchange(Dir, "time", "last_movement", last_movement_time);
	Sz.Exchange(Dir, "time", "last_enter", last_enter_time);

	// encounters
	Sz.Exchange(Dir, "encounters", "last_time", last_encounter_time);
	Sz.Exchange(Dir, "encounters", "last_world", last_encounter_world);
	Sz.Exchange(Dir, "encounters", "last_position", last_encounter_position);
	Sz.Exchange(Dir, "encounters", "previous_encounters", previous_encounter_count);
	Sz.Exchange(Dir, "encounters", "first_time", first_encounter_time);
	Sz.Exchange(Dir, "encounters", "first_world", first_encounter_world);
	Sz.Exchange(Dir, "encounters", "first_position", first_encounter_position);
}

errno_t	Avatar::profileLoad()
{
	// file path to the information
	String filePath;
	filePath.Format("%s\\userdata\\%s.ini", getProfilePath(), GetBestLock());

	// open serializer
	Serializer sz;
	sz.Load(filePath);

	// load the filedata
	getGlobalChatPlugin()	.Load(sz);
	getMuteList()					.load(sz);
	getHud()							.Load(sz);
	getFreeJoin()					.Load(sz);
	getChannelBindings()	.load(sz);

	// exchange the information
	profileExchange(AW::READ_FROM_INI, sz);

	// goooood
	return 0;
}

errno_t	Avatar::profileSave()
{
	// generate serializer
	Serializer sz;
	getGlobalChatPlugin()	.Save(sz);
	getMuteList()					.save(sz);
	getHud()							.Save(sz);
	getFreeJoin()					.Save(sz);
	getChannelBindings()	.save(sz);

	// exchange the information
	profileExchange(AW::WRITE_TO_INI, sz);

	// file path to the information
	String filePath;
	filePath.Format("%s\\userdata\\%s.ini", getProfilePath(), GetBestLock());

	// write to ini
	return sz.Save(filePath) ? 0 : 1;
}

void Avatar::EventCitizenLookup(int rc, long citizen, LPCSTR name)
{
	m_priv_name		= name;
	m_priv_lus		= LookupState::Complete;
	rt_diag->c_AvatarsDialog.AvatarUpdate(*this);
}

#include "zones_configgeneral.h"

void Avatar::activateHyperion()
{
	// global hyperion interfaces
	if (Zones::g_Config.getHyperionKernel().getEnabled() == false)
		return;

	// must have CT
	if (plc_bot->HasCT() == false)
		return;

	// the new set of world attributes to be applied
	WorldAttributes attrs, backup;
	attrs.from_sdk(*plc_bot);

	// filter through the stages of potential changes
	useHyperionZone		(attrs);
	useHyperionPersonal	(attrs);
	useHyperionScript	(attrs);

	// world override has different attributes
	if (getWorldOverride().attributes_equal(attrs) == false)
	{
		// backup attributes from the world to restore after
		backup.from_sdk(*plc_bot);

		// apply changes
		getWorldOverride() = attrs;
		attrs.to_sdk(*plc_bot);
		bot._world_attributes_send(m_session);

		// copy backup back
		backup.to_sdk(*plc_bot);
	}

	// last update
	last_hyperion_update_time = time64(0);
	last_hyperion_update_position = getThisLocation();
}

bool Avatar::activateHyperionByRefreshConditions()
{
	Zones::ConfigHyperionKernel& hypConfig = Zones::g_Config.getHyperionKernel();
	
	// distances and temporal differences
	int horizontal = abs(distanceToGD(last_hyperion_update_position) * 100);
	int vertical   = abs((last_hyperion_update_position.getY() - getY()) * 100);

	// condition prechecks
	bool cond1 = hypConfig.getAutoRefreshSeconds() && ((time64(0) - last_hyperion_update_time) > hypConfig.getAutoRefreshSeconds());
	bool cond2 = hypConfig.getAutoRefreshDistance() && horizontal > hypConfig.getAutoRefreshDistance();
	bool cond3 = hypConfig.getAutoRefreshVerticalDistance() && vertical > hypConfig.getAutoRefreshVerticalDistance();
	bool check = cond1 || cond2 || cond3;

	// have the conditions been met
	if (check) activateHyperion();
	return check;
}

void Avatar::useHyperionPersonal(WorldAttributes& Attr)
{
}

void Avatar::useHyperionScript(WorldAttributes& Attr)
{
	// scripting host
	ScriptingHost::getInst().doFunction("EventAvatarHyperionRequest", 
		&Umbra::VariantArray(*this, Attr), 0, &bot);
}