#include "stdafx.h"
#include "RepetitionTimer.h"

void RepetitionTimer::hit(String Key)
{
	map[Key] = time64(0);
}

bool RepetitionTimer::test(String Key, time_t Time)
{
	Map::iterator itr = map.find(Key);
	if (itr == map.end()) return true;
	else return (itr->second + Time) < time64(0);
}

void RepetitionTimer::prune()
{
}

void RepetitionTimer::clear()
{
	map.clear();
}