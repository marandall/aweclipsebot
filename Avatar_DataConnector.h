#pragma once

class Avatar_Usersettings;

class Avatar_DataConnector
{
private:
	Avatar_Usersettings* m_dc_user_settings;

public:
	Avatar_DataConnector();
	~Avatar_DataConnector();
	Avatar_Usersettings& get_settings()	{ return *m_dc_user_settings; }
};