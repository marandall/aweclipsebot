#pragma once

#define RESIZE_LISTBOX(x)									\
	{														\
		RECT area;											\
		x.GetWindowRect(&area);								\
		ScreenToClient(&area);								\
		area.bottom = area.bottom + 100;					\
		x.MoveWindow(&area);								\
	}