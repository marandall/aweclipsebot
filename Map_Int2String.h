#pragma once
#include <map>

class Map_Int2String
{
public: // a map of entries
	struct Entry
	{
		int		 Key;
		CStringA Value;
	};

public: // map data
	typedef std::map<int, CStringA> Map;
	typedef Map::iterator			iterator;
	typedef Map::reverse_iterator	reverse_iterator;
	
public: // select data
	Map map;
	iterator begin()		{ return map.begin();	}
	iterator end()			{ return map.end();		}
	CStringA& operator[](int Key) { return map[Key]; }

public: // constructor
	Map_Int2String(Entry en[], int size)
	{
		for (size_t i = 0; i < size / sizeof(en[0]); i++)
			map[en[i].Key] = en[i].Value;
	}
};