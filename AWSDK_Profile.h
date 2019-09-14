#pragma once

class CBaseProfile
{
public:
	String		universe_name, universe_host;
	int				universe_port;

	String		citizen_privilege;
	int				citizen_number;

	String		world_name;
	bool			world_global, world_invisible;

	String		bot_name;
	int				bot_avatar;

private:
	String ReadProfileString(LPCSTR Section, LPCSTR Key, LPCSTR file)
	{
		return "";
	}

public:
	bool LoadProfile(LPCSTR FileName)
	{
		return true;
	}

};