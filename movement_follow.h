#pragma once
#include "umbra/umbra_remoteclass.h"
#include "movement_interface.h"

namespace AW 
{ 
	class Location; 
	class LocationEx; 
	class Bot; 
}

namespace Movement
{
	class FollowKernel : public IMovement, public Umbra::RemoteClass
	{
	public:
		int		distance_min;		// how far before it should back off
		int		distance_max;		// how far before it should be followed
		int		angle;				// what angle to the target in degrees
		int		offsety;			// what vertical offset
		bool	behind;
		String	target;

	public: // read methods
		int		getMaxDistance() const { return distance_max;	}
		int		getMinDistance() const { return distance_min;	}
		int		getAngle()		 const { return angle;			}
		int		getOffsetY()	 const { return offsety;		}
		bool	getBehind()		 const { return behind;			}
		String	getTarget()		 const { return target;			}

	public:
		FollowKernel();
		int		getNextChange(Controller* Control, AW::Location Current, AW::Location& Result, float Resolution);
	};
}