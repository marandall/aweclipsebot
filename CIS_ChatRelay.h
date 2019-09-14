#pragma once
#include "cis_exec.h"

namespace CIS 
{
	class Request;
	class Response;

	namespace Exec 
	{
		//
		//
		//
		class LinkProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class PasvProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class UnlinkProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class MuteProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class UnmuteProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class KickProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/manage"; }
			int		process();
		};

		//
		//
		//
		class BanProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/manage"; }
			int		process();
		};

		//
		//
		//
		class RelayUoProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/manage"; }
			int		process();
		};

		//
		//
		//
		class ColorProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class JcProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class QcProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class ChanservProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class PassiveProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class ChanservCreateProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "relay/create_channel"; }
			int		process();
		};

		//
		//
		//
		class ChanservUsersProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class ChanservDeleteProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "relay/create_channel"; }
			int		process();
		};

		//
		//
		//
		class ChanservListProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class ChanservDefaultProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class ChanservKickProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};

		//
		//
		//
		class ChanservOptsProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "relay/use"; }
			int		process();
		};
	}
}