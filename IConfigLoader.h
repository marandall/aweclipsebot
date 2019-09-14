#pragma once

class IConfigLoader
{
public:
	virtual void Load(Serializer& SerializerSystem, const char* IniKey = NULL) = 0;
	virtual void Save(Serializer& SerializerSystem, const char* IniKey = NULL) = 0;
};