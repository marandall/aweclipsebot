#pragma once
#include "RelayAdvanced_AG.h"			// auto generated data 
#include "RelayPrivateChannels.h"

namespace ChatControllers
{
	namespace Channels
	{
		class Channel;
	}

	// kernel class
	class AdvancedRelayKernel : public AutoTemplates::AdvancedData
	{
	public: // process handling routines
		errno_t handle_avatar_add(Avatar& User);
		errno_t handle_avatar_delete(Avatar& User);
		errno_t handle_chat(Avatar& User, String Text, int Method);
		errno_t	write_channel(Avatar& User, String Text, Channels::Channel& ChanOut, bool SendConfirm = false);

	public: // notify style
		int getChannelMessageStyle() { return 0; }

	public: // notification messages
		void tellMembersJoined(Avatar& User, ChatControllers::Channels::Channel& C);
		void tellMembersQuit(Avatar& User, ChatControllers::Channels::Channel& C);

	public: // constants for type
		static const int WORLD_MODE_GLOBAL	= 0;
		static const int WORLD_MODE_ZONED	= 1;
		static const int WORLD_MODE_MAX		= 2;

	public: // constants for enumeration
		static const char* GetWorldModeString(int I)
		{
			char* types[] = { "Global", "Zoned" };
			return types[I];
		}
	};
}