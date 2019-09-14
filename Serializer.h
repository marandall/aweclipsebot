#pragma once

#include "awsdk_support.h"
#include "awsdk_colours.h"
#include "messages.h"

class Serializer : public AW::EnhancedINI
{
public: // exchange mechanisms
	bool Exchange(bool Direction, const char* Sect, const char* KeyId, MessageInfo& Value);
	bool Exchange(bool Read, const char* Sect, const char* KeyId, String& Value);
	bool Exchange(bool Read, const char* Sect, const char* KeyId, time_t& Value);
	bool Exchange(bool Read, const char* Sect, const char* KeyId, int&	Value);
	bool Exchange(bool Read, const char* Sect, const char* KeyId, float& Value);
	bool Exchange(bool Read, const char* Sect, const char* KeyId, bool& Value);
	bool Exchange(bool Read, const char* Sect, const char* KeyId, DWORD& Value);
	bool Exchange(bool Read, const char* Sect, const char* KeyId, AW::Colour& Value);

public: // copy key information
	Serializer copyEqualKeyNames(Serializer& Other);
};

class SerializerMap
{
public:
	Serializer* serializer;
	String		group_key;
};