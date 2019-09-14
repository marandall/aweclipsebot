#include "stdafx.h"
#include "Movement.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Movement
{
	bool Controller::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// the controllers
		UMBRA_PROPERTY_R(waypoints,	wps);
		UMBRA_PROPERTY_R(follow,	follow);
		UMBRA_PROPERTY_R(circle,	circle);

		// control speed
		UMBRA_PROPERTY_RW(speed,		speed);

		// nothing
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool Controller::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// follow a target
		//
		UMBRA_FUNCTION_BEGIN(follow) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Avatar, Umbra::VT_CLASS);
			UMBRA_FUNCTION_CC(Avatar, Avatar);

			// code information break
			UMBRA_FUNCTION_CS() {
				follow.target = pAvatar->getName();
				beginFollow();
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();
		
		//
		// circle around a target
		//

		UMBRA_FUNCTION_BEGIN(circle) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Target, Umbra::VT_STRING);
			UMBRA_FUNCTION_DA(1, Radius, Umbra::VT_NUMBER);

			// code information break
			UMBRA_FUNCTION_CS() {
				circle.setup(vaTarget.toString().c_str(), vaRadius.toFloat() * 100);
				beginCircle();
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// track a waypoint
		//
		UMBRA_FUNCTION_BEGIN(trackWaypoints) {
			// code information break
			UMBRA_FUNCTION_CS() {
				beginWaypoints();
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// stop
		//
		UMBRA_FUNCTION_BEGIN(standby) {
			// code information break
			UMBRA_FUNCTION_CS() {
				standby();
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// nothing
		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	//
	//
	//
	int Controller::getNextChange(AW::Location& Source, AW::Location& Result, float Resolution)
	{
		// are we executing
		if (execute == false)
			return IMovement::DO_NOTHING;

		// for each status
		if (isCircling() == true)
		{
			return getCircle().getNextChange(this, Source, Result, Resolution);
		}
		else if (isFollowing() == true)
		{
			return getFollow().getNextChange(this, Source, Result, Resolution);
		}
		else if (isWaypoints() == true)
		{
			return getWaypoints().getNextChange(this, Source, Result, Resolution);
		}
		else
		{
			return IMovement::DO_NOTHING;
		}
	}

	//
	// CircleKernel
	//

	void CircleKernel::setup(String Pos, int Radius)
	{
		m_center = Pos;
		m_radius = (float)Radius;
	}

	int CircleKernel::getNextChange(Controller* Control, AW::Location Current, AW::Location& Result, float Resolution)
	{
		//// translate the destination
		Location lcCentre = Location(getCenter(), NULL);
	
		// angular change
		int edge = (int)(getRadius() * 2 * M_PI);

		// convert speed to radians
		int circumference = (int)((getRadius() * 2) * M_PI);
		double angular = (M_PI * 2) * (1.0 / circumference * Control->getSpeed() * Resolution);

		// find distance
		double new_angle = lcCentre.AngleBetweenR(Current) + angular;

		// new position
		int new_x = lcCentre.getX() + static_cast<int>(sin(new_angle) * getRadius()); 
		int new_z = lcCentre.getZ() + static_cast<int>(cos(new_angle) * getRadius());
		
		// return position;
		AW::LocationEx result;
		result.setX		(new_x);
		result.setY		(lcCentre.getY());
		result.setZ		(new_z);
		result.setYAW	(Current.AngleBetween(result));
		Result = result.getThisLocation();
		return DO_MOVE;
	}

	//
	//
	// Following Kernel
	//
	//

	FollowKernel::FollowKernel()
	{
		distance_min	= 400;		// how far before it should back off
		distance_max	= 600;		// how far before it should be followed
		angle			= 0;		// what angle to the target in degrees
		offsety			= 0;		// what vertical offset
		behind			= false;
	}

	int FollowKernel::getNextChange(Controller* Control, AW::Location Current, AW::Location& Result, float Resolution)
	{
		// the speed is the speed of the controller multiplied by the resolution
		int Speed = static_cast<int>(Control->getSpeed() * Resolution);

		// which is the user we are attempting to track
		Avatar* pTarget = avatars.GetByKey(getTarget());
		if (pTarget == NULL)
			return DO_NOTHING;

		AW::Location alloc_target = pTarget->getThisLocation();

		int distance_to = static_cast<int>(Current.distanceToGD(alloc_target));
		this;
		if (distance_to > getMinDistance() && distance_to < getMaxDistance())
			return DO_NOTHING;		// no movement required

		/* calculate movement direction */
		float move_dir = 0.0;
		int   move_distance = 0;
		int	  mid_point = (int)((getMinDistance() + getMaxDistance()) / 2.0);
		
		if (distance_to < getMinDistance()) 
		{
			move_dir      = (float)alloc_target.AngleBetweenR(Current);
			move_distance = mid_point - distance_to;
		} 
		else 
		{
			move_dir      = (float)Current.AngleBetweenR(alloc_target);
			int distance_dp = distance_to - mid_point;
			if (distance_dp >= Speed)
				move_distance = Speed;
			else
				move_distance = distance_dp;
			// move_distance = 0; // (float)((getMinDistance() + getMaxDistance()) / 2);
		}

		/* translate coordinates */
		int new_x = Current.getX() + static_cast<int>(sin(move_dir) * move_distance);
		int new_z = Current.getZ() + static_cast<int>(cos(move_dir) * move_distance);
		int angle_at = (int)pTarget->AngleBetween(new_x, new_z);
		
		/* copy coordinates into the buffer */
		AW::LocationEx new_pos;
		new_pos.set(new_x, pTarget->getY(), new_z, angle_at + 1800);
		new_pos.setTilt(0);

		Result = new_pos;
		return DO_MOVE;
	}
}