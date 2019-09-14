#pragma once
#include <windows.h>
#include <vector>

BOOL CALLBACK ChildWindowEnum_EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	ChildWindowEnum* pHandle = (ChildWindowEnum*)lParam;
	pHandle->push_back(hwnd);
}

class ChildWindowEnum : public std::vector<HWND>
{
public:
	ChildWindowEnum(HWND hWND)
	{
		EnumChildWindows(hWND,  &ChildWindowEnum_EnumWindowsProc, (LPARAM)this);
	}
};