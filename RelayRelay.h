#pragma once
#include "RelayRelay_AG.h"		// auto generated data file
#include "RelayGlobalChat.h"
#include "RelayGlobalChatAex.h"
#include "RelayAdvancedRelay.h"

#include "RelayMuteList.h"
#include "RelayPrivateChannels.h"

namespace ChatControllers
{
	// kernel class
	class RelayKernel : public AutoTemplates::RelayData
	{
	public: // what is being used
		static const int USE_NOTHING		= 0;
		static const int USE_GLOBAL_CHAT	= 1;
		static const int USE_TOTAL_RELAY	= 3;

	public: // array of names
		typedef std::map<int, CStringA> MapMode2String;
		static MapMode2String& GetMapMode2String();

	public: // quick checks
		bool	IsGlobalChat() const			{ return getRelayMode() == USE_GLOBAL_CHAT; }
		bool	IsNothing() const				{ return getRelayMode() == USE_NOTHING; }
		bool	IsAdvancedRelay() const			{ return getRelayMode() == USE_TOTAL_RELAY; }

	public: // the variables
		GlobalChatKernel		global_chat;
		AdvancedRelayKernel		total_relay;
		Channels::ChannelList	channels;

	public: // return gets
		GlobalChatKernel&		getGlobalChat()	{ return global_chat;	}
		AdvancedRelayKernel&		getAdvancedRelay() { return total_relay;	} 
		Channels::ChannelList&	getChannels()	{ return channels;		}

	public: // process handling routines
		errno_t handle_avatar_add(Avatar& User);
		errno_t handle_avatar_delete(Avatar& User);
		errno_t handle_chat(Avatar& User, String Text, int Method);

	public: // save overrides for members
		void Load(Serializer& SerializerSystem, const char* SzKey = 0);
		void Save(Serializer& SerializerSystem, const char* SzKey = 0);
	};

	// external handler
	extern RelayKernel g_Relay;
}