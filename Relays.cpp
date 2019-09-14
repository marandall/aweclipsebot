#include "stdafx.h"
#include "RelayRelay.h"

namespace ChatControllers
{
	RelayKernel g_Relay;

	RelayKernel::MapMode2String& RelayKernel::GetMapMode2String()
	{
		static RelayKernel::MapMode2String mapped;
		
		// assign variables
		if (mapped.size() == 0)
		{
			mapped[USE_NOTHING		] = "Disabled";
			mapped[USE_GLOBAL_CHAT	] = "Global";
			mapped[USE_TOTAL_RELAY	] = "Advanced";
		}

		// return the setup entry
		return mapped;
	}

	errno_t RelayKernel::handle_avatar_add(Avatar& User)
	{
		switch (getRelayMode())
		{
		case USE_GLOBAL_CHAT:
			return getGlobalChat().handle_avatar_add(User);
			break;

		case USE_TOTAL_RELAY:
			return getAdvancedRelay().handle_avatar_add(User);
			break;

		default:
			return 0;
			break;
		};
	}
	
	errno_t RelayKernel::handle_avatar_delete(Avatar& User)
	{
		switch (getRelayMode())
		{
		case USE_GLOBAL_CHAT:
			return getGlobalChat().handle_avatar_delete(User);
			break;

		case USE_TOTAL_RELAY:
			return getAdvancedRelay().handle_avatar_delete(User);
			break;

		default:
			return 0;
			break;
		};
	}
	
	errno_t RelayKernel::handle_chat(Avatar& User, String Text, int Method)
	{
		switch (getRelayMode())
		{
		case USE_GLOBAL_CHAT:
			return getGlobalChat().handle_chat(User, Text, Method);
			break;

		case USE_TOTAL_RELAY:
			return getAdvancedRelay().handle_chat(User, Text, Method);
			break;

		default:
			return 0;
			break;
		};
	}

	void RelayKernel::Load(Serializer& SerializerSystem, const char* SzKey)
	{
		getGlobalChat()	.Load(SerializerSystem);
		getAdvancedRelay()	.Load(SerializerSystem);
		RelayData::		 Load(SerializerSystem);

	}

	void RelayKernel::Save(Serializer& SerializerSystem, const char* SzKey)
	{
		getGlobalChat()	.Save(SerializerSystem);
		getAdvancedRelay()	.Save(SerializerSystem);
		RelayData::		 Save(SerializerSystem);
	}
}