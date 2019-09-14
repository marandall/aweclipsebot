#include "stdafx.h"
#include "property_registry.h"
#include "FileLoader.h"


namespace Property
{
	errno_t Registry::load(const char* Path)
	{
		// load the registry
		FileReader reader(Path);
		for (size_t i = 0; i < reader.size(); i++)
		{
			// split data
			SplitString tokens(reader[i], " ", -1);
			Registry::Model model;
			model.x1 = atoi(tokens[2]);
			model.z1 = atoi(tokens[4]);
			model.x2 = atoi(tokens[5]);
			model.z2 = atoi(tokens[7]);

			// push data
			data[tokens[0]] = model;
		}

		return 0;
	}

	Registry::Model* Registry::operator[](const char* Data)
	{
		iterator ptr = data.find(Data);
		if (ptr == data.end())
			return 0;
		else
			return &ptr->second;
	}
}