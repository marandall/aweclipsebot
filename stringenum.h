#pragma once
#include <map>

class StringEnum
{
public: // create the pair
	struct Pair
	{
		int pos;
		const char* name;
	};

public: // create association
	typedef std::map<int, String> MapType;
	typedef MapType::iterator iterator;
	MapType assoc;

public: // setup arrangement
	template <size_t ItemCount>
	StringEnum(Pair (&Items)[ItemCount])
	{
		for (size_t i = 0; i < ItemCount; i++)
			assoc[Items[i].pos] = Items[i].name;
	}

public: // extraction
	const size_t size() { return assoc.size(); }
	String operator[] (unsigned int Index) { return assoc[Index]; }
	iterator begin() { return assoc.begin(); }
	iterator end() { return assoc.end(); }
};