#pragma once
#include "Zones_ConfigGeneral_AG.h"		// auto generated data file
#include "Zones_ConfigHyperion.h"		// hyperion

namespace Zones
{
	// kernel class
	class ConfigGeneralKernel : public AutoTemplates::ConfigGeneralData
	{
	protected:
		ConfigHyperionKernel	m_config_hyperion;

	public: // get the kernel
		ConfigHyperionKernel&	getHyperionKernel() { return m_config_hyperion; }

	public: // loading and saving pass-on
		void Save(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			__super::Save(SerializerSystem);
			getHyperionKernel().Save(SerializerSystem);
		}

		void Load(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			__super::Load(SerializerSystem);
			getHyperionKernel().Load(SerializerSystem);
		}
	};

	extern ConfigGeneralKernel g_Config;
}