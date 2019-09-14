#pragma once
#include <map>

namespace Property
{
	class Registry
	{
	public: // the registry entery
		struct Model { int x1, z1, x2, z2; };

	public: // the model types
		typedef std::map<CStringA, Model>	MapType;
		typedef MapType::iterator			iterator;

	public: // data
		MapType data;

	public: // load data
		errno_t load(const char* Path);
		Model*  operator[](const char* Data);
	};
}