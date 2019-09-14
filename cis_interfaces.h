#pragma once
#include "cis_macros.h"

namespace CIS
{
	namespace RespType
	{
		/* enum function responses */
		enum  {
			Default,
			Whisper,
			Broadcast,
			Said,
			Console,
			Display,
			LogFile,
			None
		};
	}

	namespace Reason
	{
		enum {
			Success,
			NoSuchFunction,
			InvalidCallState,
			InsufficentRights,
			AccessDenied,
			BadArgument,
			InvalidArgument,
			ArgumentBeyondRange,
			Failed,
			NotImplemented,
			CompletionDelayed,
		};
	}

	namespace CallType
	{
		enum {
			Client,
			Remote,
			Server,
			Avatar,
			Console
		};
	}
}