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
		class InfoProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			string	getRightsRequired() const { return ""; }
			int		process();
		};

		//
		//
		//
		class PresentProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
		};

		//
		//
		//
		class NearProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
		};

		//
		//
		//
		class LocateProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
		};

		//
		//
		//
		class IdentifyProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
		};

		//
		//
		//
		class WildcardsProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
		};

		//
		//
		//
		class StatusProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return 0; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
		};

		//
		//
		//
		class RotProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return ""; }
			int		process();
		};

		//
		//
		//
		class RatiosProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
		};

		//
		//
		//
		class LastActiveProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
		};

		//
		//
		//
		class HelpProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
		};

		//
		//
		//
		class WhereProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			string	getRightsRequired() const { return "core/info"; }
			int		process();
		};

		//
		//
		//
		class MyRightsProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
		};

		//
		//
		//
		class MyCoordinatesProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
		};

		//
		//
		//
		class MyDrsProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
		};

		//
		//
		//
		class MyPrivilegeProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
		};

		//
		//
		//
		class FollowProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "core/move"; }
			int		process();
		};

		//
		//
		//
		class CircleProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "core/move"; }
			int		process();
		};

		//
		//
		//
		class HaltProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "core/move"; }
			int		process();
		};

		//
		//
		//
		class SpeedProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "core/move"; }
			int		process();
		};

		//
		//
		//
		
		class HudProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return ""; }
			int		process();
		};






		//
		//
		//
		class JumpProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "jumpgate/use"; }
			int		process();
		};

		//
		//
		//
		class JumpCreateProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "jumpgate/create"; }
			int		process();
		};

		//
		//
		//
		class JumpResetProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "jumpgate/manage"; }
			int		process();
		};

		//
		//
		//
		class JumpDeleteProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "jumpgate/create"; }
			int		process();
		};

		//
		//
		//
		class JumpTakePossessionProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "jumpgate/manage"; }
			int		process();
		};

		//
		//
		//
		class JumpRelocateProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "jumpgate/create"; }
			int		process();
		};

		//
		//
		//
		class JumpSearchProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "jumpgate/use"; }
			int		process();
		};

		//
		//
		//
		class JumpNearProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "jumpgate/use"; }
			int		process();
		};

		//
		//
		//
		class JumpListProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "jumpgate/use"; }
			int		process();
		};





		//
		//
		//
		class FreeJoinProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/teleport"; }
			int		process();
		};

		//
		//
		//
		class JoinProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/teleport"; }
			int		process();
		};

		//
		//
		//
		class CoordsProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/teleport"; }
			int		process();
		};

		//
		//
		//
		class ThrustProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "gcs/teleport"; }
			int		process();
		};

		//
		//
		// Teleportation
		//
		//

		//
		//
		//
		class AvTeleportProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "eject/teleport"; }
			int		process();
		};

		//
		//
		//
		class TakeMeProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "core/simple"; }
			int		process();
		};

		//
		//
		//
		class BringMeProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD; }
			int		getContext() const { return CONTEXT_AVATAR; }
			string	getRightsRequired() const { return "eject/teleport"; }
			int		process();
		};

		//
		//
		// GUI Interfaces
		//
		//

		//
		//
		//
		class BindProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return 0; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "system/administrator"; }
			int		process();
		};

		// ubind 
		//
		//
		class UbindProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return 0; }
			int		getContext() const { return CONTEXT_CONSOLE; }
			string	getRightsRequired() const { return "system/administrator"; }
			int		process();
		};

		//
		//
		// Ass Kicking
		//
		//

		// 
		//
		//
		class EjectProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD;; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "eject/eject"; }
			int		process();
		};

		// 
		//
		//
		class ClickmodeProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_WORLD;; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "gcs/building"; }
			int		process();
		};

		//
		//
		// User Lookups
		//
		//

		//
		//
		//

		class CitnumProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_UNIVERSE; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
			int		resume();
		};

		// 
		//
		//
		
		class CitnameProcessor : public IExec {
		protected: // overrides
			int		getBitmask() const { return REQUIRES_UNIVERSE; }
			int		getContext() const { return CONTEXT_ANY; }
			string	getRightsRequired() const { return "gcs/info"; }
			int		process();
			int		resume();
		};
	}
}