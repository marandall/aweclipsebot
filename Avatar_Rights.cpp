#include "stdafx.h"				// for precompiled header
#include "Avatar.h"				// for avatar class


// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define getBOOL_RIGHT(x)											\
	bool own = plc_bot->_has_world_right(citizen ? m_citizen : m_privilege, x) == 1;	\
	bool all = plc_bot->_has_world_right_all(x) == 1;									\
	if (own == true) {								\
		if (sing && all) return false;				\
		else return true;							\
	} else if (sing == false && all == true)		\
		return true;								\
	else											\
		return false;


void Avatar::CalculateRights(void)
{
	wr_own.		FromBot(*plc_bot,	getCitizen());
	wr_privs.	FromBot(*plc_bot,	getPrivilege());
}

bool Avatar::HasEject(bool sing, bool citizen) const
{
	getBOOL_RIGHT(AW_WORLD_EJECT_RIGHT); 
}

bool Avatar::HasPS(bool sing, bool citizen) const
{
	getBOOL_RIGHT(AW_WORLD_PUBLIC_SPEAKER_RIGHT);
}

bool Avatar::HasCT(bool citizen) const 
{
	return m_privilege == plc_bot->getCitizen() && plc_bot->HasCT(); 
}

bool Avatar::HasRoot(bool sing, bool citizen) const 
{
	return m_privilege == 1; 
}

bool Avatar::HasBots(bool sing, bool citizen) const 
{
	getBOOL_RIGHT(AW_WORLD_BOTS_RIGHT); 
}

bool Avatar::HasEminent(bool sing, bool citizen) const 
{
	getBOOL_RIGHT(AW_WORLD_EMINENT_DOMAIN_RIGHT);
}

bool Avatar::HasBuild(bool sing, bool citizen) const 
{
	getBOOL_RIGHT(AW_WORLD_BUILD_RIGHT); 
}

bool Avatar::HasSpecialObjects(bool sing, bool citizen) const 
{
	getBOOL_RIGHT(AW_WORLD_SPECIAL_OBJECTS_RIGHT); 
}

bool Avatar::HasSpecialCommands(bool sing, bool citizen) const 
{
	getBOOL_RIGHT(AW_WORLD_SPECIAL_COMMANDS_RIGHT); 
}

bool Avatar::HasSpeak(bool sing, bool citizen) const
{
	getBOOL_RIGHT(AW_WORLD_SPEAK_RIGHT); 
}

bool Avatar::HasEnter(bool sing, bool citizen) const 
{
	getBOOL_RIGHT(AW_WORLD_ENTER_RIGHT); 
}

bool Avatar::HasV4(bool sing, bool citizen) const 
{
	getBOOL_RIGHT(AW_WORLD_V4_OBJECTS_RIGHT); 
}

bool Avatar::HasVoIP(bool sing, bool citizen) const 
{
	getBOOL_RIGHT(AW_WORLD_VOIP_RIGHT); 
}


CStringA Avatar::GetRightsString(bool sing, bool citizen) const 
{
	/* the list of variables */
	KnownList known;
	if (HasEnter(sing, citizen))						known.push_back("Enter");
	if (HasSpeak(sing, citizen))						known.push_back("Speak");
	if (HasPS(sing, citizen))								known.push_back("Public Speaker");
	if (HasEject(sing, citizen))						known.push_back("Eject");
	if (HasBots(sing, citizen))							known.push_back("Bots");
	if (HasBuild(sing, citizen))						known.push_back("Build");
	if (HasSpecialObjects(sing, citizen))		known.push_back("Special Objects");
	if (HasSpecialCommands(sing, citizen))	known.push_back("Special Commands");
	if (HasEminent(sing, citizen))					known.push_back("Eminent Domain");
	if (HasV4(sing, citizen))								known.push_back("V4");
	if (HasVoIP(sing, citizen))							known.push_back("VoIP");

	return known.toString("and");
}

bool Avatar::HasIndividualRights(bool citizen) const
{
	return GetRightsString(true, citizen).GetLength() > 0; 
}
