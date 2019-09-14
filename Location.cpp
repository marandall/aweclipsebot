#include "stdafx.h"
#include "Location.h"
#include "Beacons.h"

bool Location::ScanCoords(String Ident)
{
	return FromString(Ident);
}

bool Location::ScanAvatar(String Ident, Avatar* SourceAvatar)
{
	Avatar* pTarget = avatars.GetByKey(Ident, SourceAvatar);
	if (pTarget == NULL)
		return false;
	m_x = pTarget->getX();
	m_y = pTarget->getY();
	m_z = pTarget->getZ();
	m_yaw = pTarget->getYAW();
	return true;
}

Location::Location(String Ident, Avatar* SourceAvatar)
{
	// is this taking a world into account
	if (Ident.Left(1) == "/")
	{
		SplitString str(Ident.Mid(1), " ", 2);
		m_world = str[0];
		Ident = str[1];
	}

	// teleport to an avatars location
	if (Ident.Left(3) == "av:")
	{	
		is_valid = ScanAvatar(Ident.Mid(3));
	}

	// teleport to a jump point
	else if (Ident.Left(3) == "jp:")
	{
		Beacons::Beacon* pBeacon = Beacons::Kernel::getInst().Search(Ident.Mid(3));
		if (pBeacon == 0)
		{
			is_valid = false;
		}
		else
		{
			// copy beacons across system
			m_x		= pBeacon->getX();
			m_y		= pBeacon->getY();
			m_z		= pBeacon->getZ();
			m_yaw	= pBeacon->getYAW();
			is_valid= true;
		}
	}

	// enter point
	else if (Ident.Left(4) == "ep()")
	{
		// use the coordinates from the enter point
		AW::Coordinates crds;
		crds.FromString(bot._string(AW_WORLD_ENTRY_POINT));

		// coopy coordinates acrosss
		m_x = crds.getX();
		m_y = crds.getY() * 10;
		m_z = crds.getZ();
		m_yaw = crds.getYAW();

		// if greater
		if (Ident.Left(5) == "ep()+")
		{
			m_y+= atoi(Ident.Mid(5)) * 100;
		}

		// if less than
		if (Ident.Left(5) == "ep()-")
		{
			m_y-= atoi(Ident.Mid(5)) * 100;
		}
	}

	// the bots location in the world
	else if (Ident == "sys()")
	{
		// coopy coordinates acrosss
		m_x		= bot.getX();
		m_y		= bot.getY();
		m_z		= bot.getZ();
		m_yaw	= bot.getYAW();
	}

	// teleport to standard coordinates
	else
	{
		is_valid = ScanCoords(Ident);
	}
}