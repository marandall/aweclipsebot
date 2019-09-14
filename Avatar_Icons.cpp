#include "stdafx.h"						// for precompiled header
#include "EclipseBot.h"					// for EclipseBot
#include "Avatar.h"						// for avatar class
#include "EclipseDialog_MainIcons.h"	// will have a list of the main icons

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Data;

int Avatar::getBuildState() const
{
	return 0;
}

/* functions to get the number of icons */
int	Avatar::getIconIP(void) const
{
	if (getAddressLUS() == LookupState::None)
		return av_icons.main;
	if (getAddressLUS() == LookupState::Resolving)
		return av_icons.working;
	else if (getAddressLUS() == LookupState::Denied)
		return av_icons.locked;
	else {
		if (getAddress().GetLength() == 0)
			return av_icons.warning;
		else
			return av_icons.ticked;
	}
}

int	Avatar::getIconPrivName(void) const
{
	if (getCitizen() == getPrivilege()) {
		if (getCitizen() == 0)
			return av_icons.equal_b;
		else
			return av_icons.equal;
	} else {
		if (getPrivLUS() == LookupState::Resolving)
			return av_icons.working;
		else if (getPrivLUS() == LookupState::Complete)
			return av_icons.equal;
		else
			return av_icons.alert;
	}

	return 0;
}

int Avatar::getIconState(void)	const
{
	return Data::av_icons.move_t[getState()];
}

int	Avatar::getIconDNS(void) const
{
	int state = getDnsLUS();

	if (state == LookupState::Resolving)
		return Data::av_icons.working;
	else if (state == LookupState::Denied)
		return Data::av_icons.locked;
	else {
		if (getDns().GetLength() > 0)
			return Data::av_icons.ticked;
		else
			return Data::av_icons.warning;
	}
}

int	Avatar::getIconSide(void) const
{
	return 0;
}

int	Avatar::getIconCitizen(void) const
{
	const int ROOT = 1;
	if (wr_own.HasPublicSpeaker() && getCitizen() == ROOT)
		return Data::av_icons.ps_root;
	else if (wr_own.HasPublicSpeaker() && wr_own.HasEject())
		return Data::av_icons.ps_eject;
	else if (getCitizen() == ROOT)
		return Data::av_icons.root;
	else if (wr_own.HasPublicSpeaker())
		return Data::av_icons.ps;
	else if (wr_own.HasEject())
		return Data::av_icons.eject;
	return Data::av_icons.main;
}

int	Avatar::getIconPrivilege(void)	const
{
	const int ROOT = 1;
	if (wr_privs.HasPublicSpeaker() && getPrivilege() == ROOT)
		return Data::av_icons.ps_root;
	else if (wr_privs.HasPublicSpeaker() && wr_privs.HasEject())
		return Data::av_icons.ps_eject;
	else if (getPrivilege() == ROOT)
		return Data::av_icons.root;
	else if (wr_privs.HasPublicSpeaker())
		return Data::av_icons.ps;
	else if (wr_privs.HasEject())
		return Data::av_icons.eject;
	return Data::av_icons.main;
}

int	Avatar::getIconBuild(void)	const
{
	return Data::av_icons.build_t[getBuildState()];
}

CStringA Avatar::getBuildDescription() const
{
	// minimum, maximum and builder
	int min   = plc_bot->_int(AW_UNIVERSE_BROWSER_MINIMUM);
	int rel   = plc_bot->_int(AW_UNIVERSE_BROWSER_RELEASE);
	int build = plc_bot->_int(AW_UNIVERSE_BROWSER_BETA);

	// bot
	if (IsBot())
	{
		// compare to own
		if (getBuild() > AW_BUILD)
			return "beta";
		else if (getBuild() == AW_BUILD)
			return "bot";
		else
			return "old";
	}
	else
	{
		// release strings
		if (getBuild() < min)
			return "old";
		else if (getBuild() <= rel)
			return "release";
		else
			return "beta";
	}
}