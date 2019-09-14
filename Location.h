#pragma once
#include "AWSDK_Location.h"

class Avatar;

class Location :
	public AW::Coordinates
{
protected:
	bool	is_valid;
	bool	ScanCoords(String Indet);
	bool	ScanAvatar(String Ident, Avatar* SourceAvatar = NULL);
	
public:
	Location(String Ident, Avatar* SourceAvatar = NULL);
	Location() { }

	bool	parse(String Index, Avatar* Source = NULL);
	bool	IsValid() const { return is_valid; }
	bool	isValid() const { return is_valid; }

public: // editing
	void	setXYZ(int x, int y, int z, int yaw) 
	{ 
		m_x = x;
		m_y = y;
		m_z = z;
		m_yaw = yaw;
	}

	void setYaw (int t) { m_yaw   = t; }
	void setTilt(int t) { m_pitch = t; }
	void setRoll(int t) { m_roll  = t;	}

public: // copy
	Location& from_location(const AW::Location& Coords)
	{
		m_x = Coords.getX();
		m_y = Coords.getY();
		m_z = Coords.getZ();
		m_yaw	= Coords.getYAW();
		m_pitch	= Coords.getPitch();
		m_roll	= Coords.getRoll();
		return *this;
	}
};