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

		class ExecProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return 0; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "system/administrator"; }
			int		process();
		};

		class ToolkitSendUrlProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "system/administrator"; }
			int		process();
		};

		class ToolkitImpersonateProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "system/administrator"; }
			int		process();
		};

		class ToolkitSwitchFilter : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "system/administrator"; }
			int		process();
		};

		//
		// system/drs/createaccount
		//

		class SystemDrsCreateAccountProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "system/administrator"; }
			int		process();
			int		resume();
		};

		class SystemDrsCreateRightProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return 0; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "system/administrator"; }
			int		process();
		};
	}
}