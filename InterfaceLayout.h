#pragma once
#include "InterfaceConfig_Layout_AG.h"
#include "EclipseEvolutionDialog.h"
#include <map>

class InterfaceLayout : public InterfaceConfig::AutoTemplates::LayoutData
{
public: // enumerations for positions
	static const int POSITION_NONE	= 0;
	static const int POSITION_LEFT	= 1;
	static const int POSITION_RIGHT	= 2;

public: // enumeration position
	std::map<int, String> positions;

public: // constructor
	InterfaceLayout()
	{
		positions[POSITION_NONE]	= "None";
		positions[POSITION_LEFT]	= "Left";
		positions[POSITION_RIGHT]	= "Right";
	}

public: // apply changes
	void apply(EclipseEvolutionDialog& dlg)
	{
		dlg.c_SideUserList.ShowWindow(getAvatarsList()	? SW_SHOW : SW_HIDE);
		dlg.c_userList.ShowWindow(getUsersList()		? SW_SHOW : SW_HIDE);
		dlg.c_WorldList.ShowWindow(getWorldsList()	? SW_SHOW : SW_HIDE); 
		dlg.c_TechLog.ShowWindow(getShowMiniLogs()	? SW_SHOW : SW_HIDE);
	}

	int getLeftCount()
	{
		int c = 0;
		c+= getAvatarsList() == POSITION_LEFT ? 1 : 0;
		c+= getUsersList()	 == POSITION_LEFT ? 1 : 0;
		c+= getWorldsList()  == POSITION_LEFT ? 1 : 0;
		return c;
	}

	int getRightCount()
	{
		int c = 0;
		c+= getAvatarsList() == POSITION_RIGHT ? 1 : 0;
		c+= getUsersList()	 == POSITION_RIGHT ? 1 : 0;
		c+= getWorldsList()  == POSITION_RIGHT ? 1 : 0;
		return c;
	}

public: // static access
	static InterfaceLayout& getInst()
	{
		static InterfaceLayout sli;
		return sli;
	}
};