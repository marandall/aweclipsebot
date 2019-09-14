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
		class WorldCioProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD | REQUIRES_CARETAKER; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "world/io"; }
			int		process();
		};

		//
		//
		//
		class WorldCcioProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD | REQUIRES_CARETAKER; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "world/io"; }
			int		process();
		};

		//
		//
		//
		class WorldImportProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD | REQUIRES_CARETAKER; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "world/attributes"; }
			int		process();
		};

		//
		//
		//
		class WorldMoveModeProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD | REQUIRES_CARETAKER; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "world/attrributes"; }
			int		process();
		};

		//
		//
		//
		class WorldLiProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
		};

		//
		//
		//
		class WorldModifyProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD | REQUIRES_CARETAKER; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "world/attributes"; }
			int		process();
		};

		// world/addrt
		// world/delrt
		class WorldGrantProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD | REQUIRES_CARETAKER; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "world/rights"; }
			int		process();
			int		resume();

		protected: // carry over variables
			bool	effect_all;
		};

		//
		//
		//
		class WorldRevokeProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD | REQUIRES_CARETAKER; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "world/rights"; }
			int		process();
			int		resume();

		protected: // carry over variables
			bool	effect_all;
		};

		// 
		//
		//
		class WorldEpProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD | REQUIRES_CARETAKER; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "world/attributes"; }
			int		process();
		};

		// 
		// 
		//
		class WorldShowrightsProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
			int		resume();

		};
	}
}

#define	INCLUDE_RIGHT(right, key)										\
		if (bot.wattr.rights. right .SetRight(userId, TRUE) == TRUE)	\
			klGood.push_back(key);										\
		else															\
			klFail.push_back(key);

#define	REVOKE_RIGHT(right, key)										\
		if (bot.wattr.rights. right .SetRight(userId, FALSE) == TRUE)	\
			klGood.push_back(key);										\
		else															\
			klFail.push_back(key);

#define	CHECK_RIGHT(right, key)											\
		if (bot.wattr.rights. right .ContainsUser(userId) == TRUE)		\
			klFound.push_back(key);
