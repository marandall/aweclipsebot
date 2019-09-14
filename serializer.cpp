#include "stdafx.h"
#include "serializer.h"

bool Serializer::Exchange(bool Direction, const char* Sect, const char* KeyId, MessageInfo& Value)
{
	String Key = KeyId;
	__super::Exchange(Direction, Sect, Key + ".bold",	Value.bold);
	__super::Exchange(Direction, Sect, Key + ".italic",	Value.italic);
	__super::Exchange(Direction, Sect, Key + ".colour",	Value.colour);
	__super::Exchange(Direction, Sect, Key + ".message", Value.message);
	__super::Exchange(Direction, Sect, Key + ".mode",	Value.mode);
	return true;
}

bool Serializer::Exchange(bool Read, const char* Sect, const char* KeyId, String& Value) 
{
	return __super::Exchange(Read, Sect, KeyId, Value);
}

bool Serializer::Exchange(bool Read, const char* Sect, const char* KeyId, time_t& Value) 
{
	return __super::Exchange(Read, Sect, KeyId, Value);
}

bool Serializer::Exchange(bool Read, const char* Sect, const char* KeyId, int&	Value)
{
	return __super::Exchange(Read, Sect, KeyId, Value);
}

bool Serializer::Exchange(bool Read, const char* Sect, const char* KeyId, float& Value)
{
	return __super::Exchange(Read, Sect, KeyId, Value);
}

bool Serializer::Exchange(bool Read, const char* Sect, const char* KeyId, bool& Value)
{
	return __super::Exchange(Read, Sect, KeyId, Value);
}

bool Serializer::Exchange(bool Read, const char* Sect, const char* KeyId, DWORD& Value)
{
	return __super::Exchange(Read, Sect, KeyId, Value);
}

bool Serializer::Exchange(bool Read, const char* Sect, const char* KeyId, AW::Colour& Value)
{
	return __super::Exchange(Read, Sect, KeyId, Value.value);
}

Serializer Serializer::copyEqualKeyNames(Serializer& Other)
{
	// dummy copy
	Serializer sz;

	// full sector iteration
	for (SectionMap::iterator itr = Other.list.begin(); itr != Other.list.end(); itr++)
	{
		for (std::map<String, String>::iterator i2 = itr->second.keys.begin(); i2 != itr->second.keys.end(); i2++)
		{
			// copy items as required
			sz[itr->first][i2->first] = i2->second;
		}
	}

	// return the items
	return sz;
}