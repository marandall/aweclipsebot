#pragma once
#include "awsdk_support.h"

class Avatar_Usersettings : 
	public AW::EnhancedINI
{
public:
	int getInt		(String Section, String Key, int Default = 0) { return this->getKeyInt(Section, Key); }
	bool GetBool	(String Section, String Key, bool Default = false) { return getKeyInt(Section, Key) != FALSE; }
};;