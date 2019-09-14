#pragma once

#include "property_editors.h"

class AvatarOptions
{
protected: // click mode
	int							click_mode;
	time_t						click_mode_time;
	Property::Editors::Editor*	click_mode_editor;

public: // options
	int getClickMode() const { return click_mode; }
	Property::Editors::Editor* getClickModeEditor()
	{
		return click_mode_editor;
	}

	void changeClickMode(int T, Property::Editors::Editor* Editor) 
	{ 
		click_mode = T;
		
		// clean and reset editor
		if (click_mode_editor != 0) {
			delete click_mode_editor;
			click_mode_editor = 0;
		}

		click_mode_editor = Editor;
	}

public: // click mode types
	static const int CLICKMODE_NONE		= 0;
	static const int CLICKMODE_DELETE	= 1;
	static const int CLICKMODE_TELLPOS	= 2;
	static const int CLICKMODE_EDITOR	= 3;

public: // constructor and deconstructor
	AvatarOptions()
	{
		click_mode_editor = 0;
		changeClickMode(CLICKMODE_NONE, 0);
	}

	~AvatarOptions()
	{
		changeClickMode(CLICKMODE_NONE, 0);
	}
};