#pragma once
#include "umbra_remoteclass.h"

namespace Umbra
{
	namespace ClassLib
	{
		class RuntimeString : public Umbra::RemoteClass
		{
		public: // the string held within the container
			std::string data;

		public: // container
			RuntimeString(std::string Data)
			{
				data = Data;
				setUmbraGarbageCollected();
			}

			~RuntimeString()
			{
				int j = 0;
			}

		public: // string functions
			bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		};
	}
}