#pragma once

/* defenitions for the types of console message streams */
#define	CM_BOLD					1
#define CM_ITALIC				2
#define CM_SUPPRESSED			4
#define CM_BN_PREFIX			8
#define CM_EXECUTIVE_PREFIX		16

namespace AW {
	namespace Enums {
		namespace ConState {
			enum {
				Nothing,
				Creating,
				Created,
				Connecting,
				Recovering,
				Connected
			};
		}

		namespace GlobalState {
			enum {
				Off,
				Requested,
				Enabled
			};
		}

		/* defenitions of the various builds */
		namespace BuildType {
			enum  {
				Anchient,
				Old,
				Current,
				Beta,
				BetaPlus,
				Bot
			};
		}

		namespace LookupState {
			enum {
				None,
				Denied,
				Resolving,
				Waiting,
				Unavailable,
				Warning,
				Alert,
				Complete
			};
		}

		namespace UserType {
			enum {
				Tourist,
				Citizen,
				Bot,
				InviteClient,
				Limit
			} AW_USER_TYPES;
		}
	}

	/* error codes for this bot */
	#define RC_BASE					600
	#define RC_INVALID_STATE		(RC_BASE + 1)
	#define RC_ACTION_BLOCKED		(RC_BASE + 2);

	/* specifications for the events and callbacks */
	typedef void (*pfnc_event)();
	typedef void (*pfnc_callback)(int rc);

	/* defenitions for different users */
	#define ROOT_CITIZEN			1
	#define NONE_CITIZEN			0
}