#include "stdafx.h"				// for precompiled header
#include "Avatars.h"			// for avatars list
#include "Avatar.h"				// for avatar class
#include "GlobalTypedefs.h"		// for split string

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT AvatarList::AbsorbSessions(vector<int>& session_list, Avatars& avatars) 
{
	/* variables for function */
	Avatar* pUser;

	/* loop through each session, and try to absorb */
	for (size_t i = 0; i < session_list.size(); i++) {
		pUser = avatars.getSession(session_list[i]);
		if (pUser)
			AddSingle(pUser);
	}

	return GetCount();
}

Avatars::Avatars(void) 
{
	user_counts[0] = 0;
	user_counts[1] = 0;
	user_counts[2] = 0;
}

Avatars::~Avatars(void) 
{
	for (size_t i = 0; i < size(); i++)
		delete avCol[i];
	avCol.clear();
}

Avatar* Avatars::create(long session, LPCSTR name) 
{
	Avatar* av = new Avatar();
	avCol.push_back(av);
	return av;
}

Avatar* Avatars::getSession(long session) 
{
	FOR_EACH_INLINE_AVATAR
		if (pUser->getSession() == session)
			return pUser;
	END_EACH_AVATAR
	// return that nothing is valid
	return NULL;
}

bool Avatars::SessionExists(long lSession) 
{
	FOR_EACH_INLINE_AVATAR
		if (pUser->getSession() == lSession)
			return true;
	END_EACH_AVATAR
	// return that nothing is valid
	return false;
}

Avatar*	Avatars::getByAuto(int AutoId)
{
	FOR_EACH_INLINE_AVATAR
		// get the avatar identification and compare, return if found
		if (pUser->getAutoNumber() == AutoId)
			return pUser;
	END_EACH_AVATAR
	
	return 0;
}

Avatar* Avatars::getCitizen(long citizen) 
{
	FOR_EACH_INLINE_AVATAR
		// get the avatar identification and compare, return if found
		if (pUser->getCitizen() == citizen)
			return pUser;
	END_EACH_AVATAR
	// return that nothing is valid
	return NULL;
}

Avatar* Avatars::getName(LPCSTR name) 
{
	FOR_EACH_INLINE_AVATAR
		if (pUser->getName().CompareNoCase(name) == 0)
			return pUser;
	END_EACH_AVATAR

	// return that nothing is valid
	return NULL;
}

bool Avatars::remove(Avatar* av) 
{
	for (iterator ptr = begin(); ptr != end(); ptr++)
	{
		// get the avatar identification and compare, return if found
		if (ptr->getSession() == av->getSession()) 
		{
			avCol.erase(ptr.itr);
			DecrementTypes(av->getType());
			UpdateTypes();

			// signal an update
			_umbra_isa.signalInvalidated();
			return false;
		}
	}
	
	return true;
}

int Avatars::Clear(LPCSTR lpReason) 
{
	/* get the count if we are avoiding it */
	if (getCount() == 0)
		return 0;

	Avatar*	theAvatar;										// the avatar
	int			removed = 0;

	WriteMsg("Clearing up avatars list", RGB_SUCCESS, OT_BOTH | OM_TIME);

	/* pop from the back */
	while ( getCount() ) {
		/* get the avatat from the head - deleting on the fly - stack mentality */
		theAvatar = avCol.back();
		avCol.pop_back();

		/* remove the user via OnExit */
		theAvatar->OnExit();
		
		/* how many have been removed */
		removed++;

		/* delete the avatar */
		delete theAvatar;
	}

	/* print a break line */
	WriteMsg("\t---------------------------------------------", 0, OT_BOTH);

	/* return how many have been removed from the collection */
	return removed;
}

void Avatars::ResetRights() 
{
	FOR_EACH_INLINE_AVATAR
		/* reset the rights for each */
		pUser->ResetRights();
	END_EACH_AVATAR

	/* display information */
	WriteMsg("User rights have been reset by the console", 0, OT_TECH | OM_TIME);
}

void Avatars::ClearFlags() {
	FOR_EACH_INLINE_AVATAR
	END_EACH_AVATAR
}

Avatar* Avatars::GetByKey(const char* Key, Avatar* LocalUser) 
{
	String key = Key;
	key.MakeLower();

	// lookup ourself
	if (LocalUser && key.CompareNoCase("me()") == 0)
		return LocalUser;

	// authorise IP lookup
	bool ip_authorised = LocalUser ? LocalUser->GetRight("eject/info") : true;

	// ip users only
	for (size_t i = 0; i < avCol.size(); i++)
	{
		if (avCol[i]->IsPattern(key, ip_authorised))
		{
			return avCol[i];
		}
	}

	// nothing found
	return 0;
}

int Avatars::getWildcards(LPCSTR wildcards, AvatarList& list) {
	String full		= wildcards;
	String msg		= wildcards;
	String type		= msg.Left(1);
	msg					= msg.Mid(1).MakeLower();
	CString lMsg		= msg.MakeLower();
	Avatar*	pUser	= NULL;
	int		lValue		= atoi(lMsg);

	/* check each type value */
	if (full == "{*}") 
	{
		FOR_EACH_INLINE_AVATAR
			list.AddSingle(pUser);
		END_FOR_EACH
		return (int)list.vCol.size();
	} 
	else if (type == "$") 
	{
		FOR_EACH_INLINE_AVATAR
			if (pUser->getSession() == lValue)	list.AddSingle(pUser);
		END_FOR_EACH
	}
	else if (type == "^") 
	{
		FOR_EACH_INLINE_AVATAR
			if (pUser->getCitizen() == lValue)	list.AddSingle(pUser);
		END_FOR_EACH
	} 
	else if (type == "*") 
	{
		FOR_EACH_INLINE_AVATAR
			if (pUser->getPrivilege() == lValue) list.AddSingle(pUser);
		END_FOR_EACH
	} 
	else if (type == "!") 
	{
		FOR_EACH_INLINE_AVATAR
			if (pUser->getAddress().Left(msg.GetLength()) == msg) list.AddSingle(pUser);
		END_FOR_EACH
	} 
	else if (type == "%") 
	{
		FOR_EACH_INLINE_AVATAR
			if (String(pUser->getDns()).MakeLower().Find(msg, 0) != -1) list.AddSingle(pUser);
		END_FOR_EACH
	} 
	else if (type == "?") 
	{
		FOR_EACH_INLINE_AVATAR
			if (String(pUser->getName()).MakeLower().Find(msg, 0) != -1) list.AddSingle(pUser);
		END_FOR_EACH
	}
	else if (full == "bot()") 
	{
		FOR_EACH_INLINE_AVATAR
			if (pUser->getType() == UserType::Bot) list.AddSingle(pUser);
		END_FOR_EACH
	}
	else if (full == "tourist()") 
	{
		FOR_EACH_INLINE_AVATAR
			if (pUser->getType() == UserType::Tourist) list.AddSingle(pUser);
		END_FOR_EACH
	} 
	else if (full == "citizen()") 
	{
		FOR_EACH_INLINE_AVATAR
			if (pUser->getType() == UserType::Citizen) list.AddSingle(pUser);
		END_FOR_EACH
	} 
	else 
	{
		FOR_EACH_INLINE_AVATAR
			if (pUser->getName().CompareNoCase(wildcards) == 0) list.AddSingle(pUser);
		END_FOR_EACH
	}

	/* return the count */
	return (int)list.vCol.size();
}

int	Avatars::IncrementTypes(int ID) 
{ 
	/* change the count */
	user_counts[ID]++;
	UpdateTypes();
	return user_counts[ID];  
}

int	Avatars::DecrementTypes(int ID)	 
{ 
	/* change the count */
	user_counts[ID]--;
	UpdateTypes();
	return user_counts[ID];  
}

int	Avatars::ClearTypes() 
{
	return 0;
}

void Avatars::UpdateTypes() 
{
	/* the variables */
	char szTourists[16] = { 0 };
	char szCitizens[16] = { 0 };
	char szBots[16]	   = { 0 };

	int	 count	   = getCount();
	bool has_users = (count > 0);
}

int	Avatars::getTouristCount()
{
	return user_counts[UserType::Tourist];
}

int Avatars::getCitizenCount()
{
	return user_counts[UserType::Citizen];
}

int Avatars::getBotCount()
{
	return user_counts[UserType::Bot];
}

void Avatars::DetectRemote() 
{
}

int Avatars::getUserCount(int ID) 
{
	int count = 0;
	for (iterator2 i = begin(); i != end(); i++)
	{
		if (ID == UserType::Citizen && i->IsCitizen()) count++;
		else if (ID == UserType::Tourist && i->IsTourist()) count++;
		else if (ID == UserType::Bot && i->IsBot()) count++;
	}

	return count;
}



AvatarList::~AvatarList()
{
	vCol.RemoveAll();
}

bool AvatarList::AddSingle(Avatar* pAvatar) 
{
	Avatar* pUser;

	FOR_EACH_OF_DEFINED(pUser, pos, vCol);
		if (pAvatar == pUser)
			return false;
	END_FOR_EACH
	
	vCol.push_back(pAvatar);	
	return true;
}

bool AvatarList::FindSingle(Avatar* pAvatar) const 
{
	size_t count = vCol.size();
	for (size_t i = 0; i < count; i++)
		if (vCol.At(i) == pAvatar)
			return true;
	return false;
}

Avatar*& AvatarList::operator[](UINT index) 
{
	return vCol[index];
}

UINT AvatarList::GetCount(void) const 
{
	return (UINT)vCol.size();
}