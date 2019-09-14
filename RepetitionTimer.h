#pragma once
#include <map>

class RepetitionTimer
{
protected: // type defenitions
	typedef std::map<String, time_t>	Map;
	typedef Map::iterator			iterator;

public: // impl
	Map map;
	iterator begin() { return map.begin(); }
	iterator end()   { return map.end();   }

public: // timing methods
	void	hit(String Key);
	bool	test(String Key, time_t Time);
	void	prune();
	void	clear();
};