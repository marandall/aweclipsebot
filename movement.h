#pragma once
#include "movement_circle.h"
#include "movement_follow.h"
#include "movement_waypoints.h"

namespace Movement
{
	class Controller : public Umbra::RemoteClass
	{
	public: // umbra interface
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

	protected: // kernel
		FollowKernel	follow;
		CircleKernel	circle;
		Waypoints		wps;

	public: // get access
		FollowKernel&	getFollow()		{ return follow; }
		CircleKernel&	getCircle()		{ return circle; }
		Waypoints&		getWaypoints()	{ return wps;    }

	public: // constructor
		Controller()
		{
			execute = true;
			speed   = 500;
		}

	protected: // methods
		int		mode;
		int		speed;

	public: // is this an executing mover
		bool	execute;

	public: // get functions
		int		getMode()		const { return mode;		}
		int		getSpeed()		const { return speed;		}
		void	setSpeed(int S) { speed = S;				}

	public: // checks
		bool	isStandby()		const { return getMode() == STANDBY_MODE;	}
		bool	isFollowing()	const { return getMode() == FOLLOW_MODE;	}
		bool	isCircling()	const { return getMode() == CIRCLE_MODE;	}
		bool	isWaypoints()	const { return getMode() == WAYPOINT_MODE;	}

	public: // methods
		void	standby()		{ mode = STANDBY_MODE;	}
		void	beginFollow()	{ mode = FOLLOW_MODE;	}
		void	beginCircle()	{ mode = CIRCLE_MODE;	}
		void	beginWaypoints(){ mode = WAYPOINT_MODE; }

	public: // forward control of waypoins
		errno_t	loadWaypoints(const char* FilePath, int Start);

	public: // conversion
		int		getNextChange(AW::Location& Source, AW::Location& Result, float Resolution);

	public: // enum of styles
		const static int STANDBY_MODE	= 0;
		const static int FOLLOW_MODE	= 1;
		const static int CIRCLE_MODE	= 2;
		const static int WAYPOINT_MODE	= 3;
	};
}