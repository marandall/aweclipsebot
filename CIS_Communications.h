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
		class BurstProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "comms/broadcast"; }
			int		process();
		};

		//
		//
		//
		class SayProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "comms/broadcast"; }
			int		process();
		};

		//
		//
		//
		class WhisperProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "comms/broadcast"; }
			int		process();
		};

		//
		//
		//
		class BotgramProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "comms/broadcast"; }
			int		process();
		};

		//
		//
		//
		class CMsgProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "comms/broadcast"; }
			int		process();
		};

		//
		//
		//
		class NoiseProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "comms/broadcast"; }
			int		process();
		};
	}
}